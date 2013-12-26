#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/nodemask.h>
#include <asm/setup.h>
#include <mach/fmem.h>
#include <linux/delay.h>

#define SATA_WORKAROUND

#ifdef SATA_WORKAROUND
#define SATA_ADDR_BOUNDARY  (0x10000000 - SZ_1M)
#endif

#define FMEM_PROCESS_SINGLE_PAGE

static g_page_info_t    g_page_info;
/* Internal Macro
 */
#define POOL_LIST(x)      g_page_info.list[x]
#define POOL_NODE_SZ(x)   g_page_info.node_sz[x]
#define POOL_PHY_START(x) g_page_info.phy_start[x]
#define ALLOC_BSZ         SZ_8M

extern void platform_get_memory(struct meminfo **p_memory);
extern void platform_get_gmmemory(struct meminfo **p_memory);

int fmem_give_pages(int node_id, unsigned int give_sz);
static int check_mem_range(int node_id, dma_addr_t phy_addr);

/*
 * @brief allocate the memory from linux kernel.
 * @function void *fmem_alloc_ex(size_t size, dma_addr_t * dma_handle, unsigned long flags, int ddr_id)
 * @parameter size indicates the memory size going to allocate
 * @parameter dma_handle indicates the physical address
 * @parameter flags indicates type of this allocated memory is cachable or non-cacheable or ....
 * @parameter ddr_id indicates which DDR the users want to allocate.
 * @return virutal memory if success or NULL for fail.
 */
void *fmem_alloc_ex(size_t size, dma_addr_t * dma_handle, unsigned long flags, int ddr_id)
{
    struct page *page;
    void *cpu_addr = NULL;
    struct page *end;

    if (ddr_id >= MAX_DDR_CHUNKS)
        return NULL;

    size = PAGE_ALIGN(size);
#ifdef CONFIG_ISOLATE_HIGHMEM    
    page = alloc_pages_node(ddr_id, GFP_HIGHUSER, get_order(size));
#else
    page = alloc_pages_node(ddr_id, GFP_KERNEL, get_order(size));
#endif    
    if (!page) {
        printk("alloc_pages fail! (requested %#x)", size);
        goto no_page;
    }

    *dma_handle = page_to_phys(page);
    if (check_mem_range(ddr_id, *dma_handle) < 0) {
        __free_pages(page, get_order(size));
        goto no_page;
    }
    
    if (flags == PAGE_COPY)
        cpu_addr = ioremap_cached(*dma_handle, size);
    else if (flags == pgprot_writecombine(pgprot_kernel))
        cpu_addr = ioremap_wc(*dma_handle, size);
    else
        cpu_addr = ioremap_nocache(*dma_handle, size);    

    if (cpu_addr) {
        end = page + (1 << get_order(size));
        do {
            init_page_count(page);
            SetPageReserved(page);
            page++;
            size -= PAGE_SIZE;
        } while (size != 0);

        /*
         * Free the otherwise unused pages.
         */
        while (page < end) {
            init_page_count(page);
            if (!PageReserved(page) && put_page_testzero(page))
                free_cold_page(page);
            page++;
        }
    } else {
        __free_pages(page, get_order(size));
        printk("__ioremap fail! (phy %#x)", *dma_handle);
    }

  no_page:
    return cpu_addr;
}
EXPORT_SYMBOL(fmem_alloc_ex);

/*
 * @brief free the memory which was allocated by fmem_alloc_ex().
 * @function void fmem_free_ex(size_t size, void *cpu_addr, dma_addr_t handle)
 * @parameter size indicates the memory size going to free
 * @parameter cpu_addr indicates the virtual memory going to free
 * @parameter handle indicates the physical memory going to free
 * @return None.
 */
void fmem_free_ex(size_t size, void *cpu_addr, dma_addr_t handle)
{
    struct page *page = pfn_to_page(handle >> PAGE_SHIFT);
    struct page *pg;
    unsigned int sz;

    pg = page;
    sz = size;

#ifdef FMEM_DEBUG_MODE2
    {
        unsigned long pfun;
      __asm__("ldr %0, [fp, #-4]\n\t":"=r"(pfun));
        print_symbol("fmem:(%s)", pfun);
        printk("->0x%x\n", handle);
    }
#endif

    __iounmap(cpu_addr);
    size = PAGE_ALIGN(size);

#ifdef FMEM_PROCESS_SINGLE_PAGE
    do {
        ClearPageReserved(page);
        if (!PageReserved(page) && put_page_testzero(page))
            free_cold_page(page);
        page++;
    } while (size -= PAGE_SIZE);

#else
    ClearPageReserved(page);
    if (!PageReserved(page) && set_page_count(page, 0))
        __free_pages_ok(page, get_order(size));
#endif

#ifdef FMEM_DEBUG_MODE
    fara_printf("fmem_free 0x%x size 0x%x\n", page_to_phys(pg), size);
#endif
}
EXPORT_SYMBOL(fmem_free_ex);


/*
 * @brief get the page with largest physical address and the page with smallest physical address.
 * @function void get_first_last_page(struct list_head *glist, page_node_t ** small_page,
                         page_node_t ** large_page)
 * @return None.
 */
void get_first_last_page(struct list_head *glist, page_node_t ** small_page,
                         page_node_t ** large_page)
{
    dma_addr_t addr, temp = 0xffffffff, temp2 = 0x0;
    page_node_t *page_node;
    struct list_head *node;

    list_for_each(node, glist) {
        page_node = list_entry(node, page_node_t, list);
        addr = page_to_phys(page_node->page);

        /* find the smallest page */
        if (addr < temp) {
            temp = addr;
            *small_page = page_node;
        }

        /* find the largest page */
        if (addr > temp2) {
            temp2 = addr;
            *large_page = page_node;
        }
    }
}

/*
 * @brief check if the address is within the valid range of the chunk
 *
 * @function static check_mem_range(int node_id, dma_addr_t phy_addr)
 * @param node_id is used to indicate chunk
 * @param phy_addr indicate the physical address want to check
 * @return 0 on success, !0 on error
 */
static int check_mem_range(int node_id, dma_addr_t phy_addr)
{
    u32 phy_base, phy_end;
    pg_data_t *pgdat;

    if (!node_online(node_id))
        return -1;

    pgdat = NODE_DATA(node_id);

    phy_base = (u32) pgdat->node_start_pfn << PAGE_SHIFT;
    phy_end = (u32) (phy_base + (pgdat->node_present_pages << PAGE_SHIFT));

    if ((phy_addr >= phy_base) && (phy_addr < phy_end))
        return 0;

    return -1;
}

/*
 * @brief This function examines if the allocated memory blocks are physical contingous
 *
 * @function int check_memory_hole(int node_id, struct page *spage, struct page *lpage)
 * @param node_id: node id
 * @param spage_node: small page node with the smallest physical address
 * @param lpage_node: large page node with the largeest physical address
 * @return value >= 0 means ok, < 0 means hole existence
 */
int check_memory_hole(int node_id, page_node_t *spage_node, page_node_t *lpage_node)
{
    dma_addr_t          addr, saddr, laddr, tmp_addr;
    page_node_t         *page_node;
    struct list_head    *node;
    int                 bFound;
        
    saddr = page_to_phys(spage_node->page);
    laddr = page_to_phys(lpage_node->page);
    
    for (tmp_addr = saddr; tmp_addr <= laddr; ) {
        bFound = 0;
        list_for_each(node, &POOL_LIST(node_id)) {
            page_node = list_entry(node, page_node_t, list);
            addr = page_to_phys(page_node->page);
            if (tmp_addr == addr) {
                bFound = 1;
                break;
            }
        }
        if (!bFound) {
            /* dump whole memory list */
            list_for_each(node, &POOL_LIST(node_id)) {
                page_node = list_entry(node, page_node_t, list);
                addr = page_to_phys(page_node->page);
                
                printk("The memory address: %#x \n", addr);
            }
            return -1;
        }
        
        //printk("The memory address: %#x \n", tmp_addr);
        tmp_addr += page_node->size;    //move to next address
    }
    
    return 0;
}

/*
 * @brief This function allocates ahead the memory in advance before fragementation happen
 *
 * @function int __init fmem_init(void)
 * @param none
 * @return none
 */
static int __init fmem_init(void)
{
#ifdef CONFIG_DISCONTIGMEM
    struct meminfo *pmem;
    struct meminfo *pmem_gm;
    page_node_t *page_node, *large_page, *small_page;
    pg_data_t *pgdat;
    struct page *page;
    u32 i, node_id, order;
    u32 round, tmp_sz, size;
    
    platform_get_memory(&pmem);
    platform_get_gmmemory(&pmem_gm);
    
    memset(&g_page_info, 0, sizeof(g_page_info));
    order = get_order(PAGE_ALIGN(ALLOC_BSZ));
    g_page_info.block_sz = PAGE_SIZE << order;

    for (node_id = 0; node_id < MAX_DDR_CHUNKS; node_id++) {
        if (!node_online(node_id))
            continue;

        pgdat = NODE_DATA(node_id);
        INIT_LIST_HEAD(&POOL_LIST(node_id));
        /* count the active node */
        g_page_info.nr_node++;
        
        /*
         * Note: The arguments from boot command line is higher than the MACRO defintion in memory.h.
         */
        if (node_id == 0) {
            tmp_sz = pmem_gm->bank[node_id].size ? pmem_gm->bank[node_id].size : HIGH_MEM_SIZE;
        } else {            
            if (pmem_gm->bank[node_id].size != 0) {
                /* boot command line specifies the gm memory size
                 */
                if (pmem_gm->bank[node_id].size >= (pmem->bank[node_id].size - ALLOC_BSZ))
                    tmp_sz = pmem->bank[node_id].size - ALLOC_BSZ;
                else 
                    tmp_sz = pmem_gm->bank[node_id].size;
            }
            else {
                /* boot command line doesn't specify the gm memory size
                 */
                if (FRAMMAP_MEM_SIZE == -1) {
                    /* reserved ALLOC_BSZ for kernel use */
                    tmp_sz = pmem->bank[node_id].size - ALLOC_BSZ;  
                }
                else {
                    tmp_sz = ((pmem->bank[node_id].size - ALLOC_BSZ) > FRAMMAP_MEM_SIZE) ? 
                            FRAMMAP_MEM_SIZE : (pmem->bank[node_id].size - ALLOC_BSZ);
                }
            }
        }
        
        round = (tmp_sz + ALLOC_BSZ - 1) / ALLOC_BSZ;
        POOL_NODE_SZ(node_id) = 0;
        for (i = 0; i < round; i++) {
            page_node = (page_node_t *)kzalloc(sizeof(page_node_t), GFP_KERNEL);
            if (page_node == NULL)
                panic("fmem_init: No memory for kmalloc! \n");

            INIT_LIST_HEAD(&page_node->list);
            page_node->page = alloc_pages_node(node_id, GFP_ATOMIC, order);
            if (page_node->page == NULL)
                panic("fmem_init: Page memory is not enough! page order = %d \n", order);
            
            
            page_node->phy_start = page_to_phys(page_node->page);
            page_node->size = ALLOC_BSZ;

            /*
             * if the allocated memory doesn't belong to the address range of node_id, then this
             * allocation will be given to kernel and stop memory allocation.
             */
            if (check_mem_range(node_id, page_node->phy_start) < 0) {
                /* skip this allocation, debug only */
                printk("ddr = %d, page = %x, addr = %x is skipped! \n", node_id, (u32)page_node->page, 
                            (u32)page_to_phys(page_node->page));
                __free_pages(page_node->page, order);
                kfree(page_node);
                break;
            }
            
            /* can't be swap out and mmap issue which can be mapped to reserved pages only */
            page = page_node->page;            
            size = page_node->size;
            
            do {
                /* seperate the group pages into individual page */
                init_page_count(page);
                SetPageReserved(page);
                page++;
            } while (size -= PAGE_SIZE);
            
            POOL_NODE_SZ(node_id) += page_node->size;
                        
            /* add to the global DDR list */
            list_add_tail(&page_node->list, &POOL_LIST(node_id));

            //printk("ddr = %d, page = %#x, addr = %#x \n", node_id, page_node->page, (u32)page_to_phys(page_node->page));
        };

        if (round != 0) {
            int ret = 0;
            
            if (ret) {} 

            /* who is the first page in several pages */
            get_first_last_page(&POOL_LIST(node_id), &small_page, &large_page);
            POOL_PHY_START(node_id) = page_to_phys(small_page->page);
            if (check_memory_hole(node_id, small_page, large_page) < 0)
                panic("The memory is not flat memory! \n");

            /* free the tail and also test the give function.
             */
            if (tmp_sz % ALLOC_BSZ) {
                ret = fmem_give_pages(node_id, (ALLOC_BSZ - (tmp_sz % ALLOC_BSZ)));
                if (ret != (ALLOC_BSZ - (tmp_sz % ALLOC_BSZ)))
                    panic("%s, bug! \n", __func__);
            }
        }
        //printk("ddr = %d, base page = %x, addr = %x \n\n", node_id, small_page->page, page_to_phys(small_page->page));
        printk("Fmem: node %d is online, alloc pages = %d(active pages = %d)\n", node_id,
               POOL_NODE_SZ(node_id) >> PAGE_SHIFT, (unsigned int)pgdat->node_present_pages);
    }
#endif /* CONFIG_DISCONTIGMEM */
    
    printk("high_memory:%#x, VM Start:%#x, End:%#x \n", (u32)high_memory, (u32)VMALLOC_START, (u32)VMALLOC_END);
    
    return 0;
}

core_initcall(fmem_init);

/*
 * @brief get the memory data information from frammap
 *
 * @function fmem_get_pageinfo(g_page_info_t **pg_info)
 * @param pg_info is the pointer wants to be assigned the info 
 * @return none
 */
void fmem_get_pageinfo(g_page_info_t ** pg_info)
{
    *pg_info = &g_page_info;

    return;
}

/*
 * @brief remove a page node from the DDRx's link list
 *
 * @function void remove_page_node(struct list_head *glist, page_node_t *rm_node)
 * @param glist is the page nodes in the specific DDR.
 * @param rm_node indicates the node is going to be remove.
 * @return None.
 */
void remove_page_node(struct list_head *glist, page_node_t *rm_node)
{    
    page_node_t *node, *ne;
    
    list_for_each_entry_safe(node, ne, glist, list) {
        if (node != rm_node)
            continue;
            
        list_del(&node->list);
        kfree(node);
        break;
    }
}

/*
 * @brief return the memory to the kernel from frammap
 *
 * @function int fmem_give_pages(int node_id, unsigned int give_sz)
 * @param node_id is the real node id instead of chunk id
 * @param node_id is the size you want to return to the kernel
 * @return the real size that gives to the kernel
 */
int fmem_give_pages(int node_id, unsigned int give_sz)
{
    unsigned int size, order;
    page_node_t *large_page, *small_page;
    struct page *page;
    int     ret_sz, tmp_sz;
    int     print = 0;
    
    if (print)  {}
    
    if (!node_online(node_id))
        return 0;
                
    order = get_order(PAGE_ALIGN(ALLOC_BSZ));
    //give_sz = (give_sz >> (order + PAGE_SHIFT)) << (order + PAGE_SHIFT);
    
    if (give_sz == 0)
        return 0;

    ret_sz = 0;
    do {
        get_first_last_page(&POOL_LIST(node_id), &small_page, &large_page);
        size = (give_sz > large_page->size) ? large_page->size : give_sz;
                        
        page = large_page->page + ((large_page->size >> PAGE_SHIFT) - 1); //move to the end page
        tmp_sz = size;

        do {
#ifdef SATA_WORKAROUND
            if (page_to_phys(page) >= SATA_ADDR_BOUNDARY) {
                /* FIXME: here will cause memory leakage due to workaround. However, we still consume
                 * 1M memory to fix SATA issue because last 1M accessed by SATA will cause problem.
                 */
                if (print == 0) {
                    printk("%s, skip to free physical memory from %#x! \n", __func__, 
                                    (u32)page_to_phys(page));
                    print = 1;
                }
                goto next_page;
            }
#endif
            ClearPageReserved(page);
            __free_page(page);
next_page:
            /* No matter wether the page is freed, we need to decrease the page size 
             */
            large_page->size -= PAGE_SIZE;
            page --;
        } while (tmp_sz -= PAGE_SIZE);
        
        /* free memory */
        if (!large_page->size)
            remove_page_node(&POOL_LIST(node_id), large_page);
        
        give_sz -= size;
        POOL_NODE_SZ(node_id) -= size;
        ret_sz += size;
    } while (give_sz != 0);

    return ret_sz;
}

/*
 * @brief calculte the memory size shift order
 * It is a macro defined in arch/arm/mach-GM/memory.h
 * @function get_mem_nodeshift(unsigned long addr)
 * @param addr is used to indicate the physical address
 * @return node id
*/
int get_mem_nodeshift(void)
{
    u32    node, size = 0;
    struct meminfo *pmem;
    static int node_shift = 0;
    
    if (unlikely(!node_shift)) {
        platform_get_memory(&pmem);
    
        /* get_mem_nodeid() is called before find_memend_and_nodes() */
        for (node = 0; node < pmem->nr_banks; node++) {
            if (size == 0) {
                size = pmem->bank[node].size;
                break;
            }
        }
        
        node_shift = get_order(size) + PAGE_SHIFT;
    }

    return node_shift;
}

/*
 * @brief Given a logical address, convert it to a node id. 
 * It is a macro defined in arch/arm/mach-GM/memory.h
 * @function unsigned int fmem_kvaddr_to_nid(unsigned long addr)
 * @param kaddr is used to indicate the kernel address
 * @return node id
*/
unsigned int fmem_kvaddr_to_nid(unsigned long kaddr)
{
    int shift = get_mem_nodeshift();
    
    return (((unsigned long)(kaddr) - PAGE_OFFSET) >> shift);
}

/*
 * @brief Given a page frame number, convert it to a node id.
 *  It is a macro defined in arch/arm/mach-GM/memory.h
 * @function unsigned int fmem_pfn_to_nid(u32 pfn)
 * @param pfn is used to indicate the pfn
 * @return node id
*/
unsigned int fmem_pfn_to_nid(u32 pfn)
{
    int shift = get_mem_nodeshift();
    
    return (((pfn) - PHYS_PFN_OFFSET) >> (shift - PAGE_SHIFT));   
}

/*
 * @brief Given a phyaddr, LOCAL_MEM_MAP finds the owning node of the memory
 * and returns the index corresponding to the appropriate page in the
 * node's mem_map.
 * It is a macro defined in arch/arm/mach-GM/memory.h
 * @function unsigned int fmem_local_map_nr(unsigned long addr)
 * @param addr is used to indicate the physical address 
 * @return pfn number of the corresponding node
*/
unsigned int fmem_local_map_nr(unsigned long addr)
{
    u32 mask;
    int shift = get_mem_nodeshift();
    
    mask = (0x1 << shift) - 1;
        
    return (((unsigned long)(addr) & mask) >> PAGE_SHIFT);
}

/*
 * @brief get ddr0 mask which is size-1
 *
 * It is a macro defined in arch/arm/mach-GM/memory.h
 * @function unsigned int fmem_get_ddr0_mask(void)
 * @param: none
 * @return the mask of DDR0
*/
unsigned int fmem_get_ddr0_mask(void)
{
    unsigned int shift = get_mem_nodeshift();
                
    return (unsigned int)((0x1 << shift) - 1);
}

/*
 * @brief get ddr0 size bits
 *
 * It is a macro defined in arch/arm/mach-GM/memory.h
 * @function unsigned int fmem_get_ddr0_szbits(void)
 * @param: none
 * @return the shift of ddr0
*/
unsigned int fmem_get_ddr0_szbits(void)
{
    unsigned int shift = get_mem_nodeshift();;
    
    return shift;
}

void CheckTxStatus(unsigned int port)
{
		// wait until Tx ready                                                                                                                                                                                                                                                                                                                          
    while (!(*(volatile unsigned int *)(port + 0x14) & 0x20));        
      udelay(1);
}

static void _putchar(char Ch)
{
    if(Ch != '\0'){
        CheckTxStatus(UART_FTUART010_VA_BASE);
        *(volatile unsigned int *)(UART_FTUART010_VA_BASE) = Ch;
    }

    if (Ch == '\n'){
        CheckTxStatus(UART_FTUART010_VA_BASE);
        *(volatile unsigned int *)(UART_FTUART010_VA_BASE) = '\r';
    }
}

/* If we want to use this API, please enable arch/arm/kernel/head.S
 * GM_CONSOLE_PRINT definition
*/
void debug_printk(const char *f, ...)
{
    int i;
    va_list arg_ptr;
    char buffer[256];

        va_start(arg_ptr, f);
        vsprintf(&buffer[0], f, arg_ptr);
        va_end(arg_ptr);

      //output the buffer
    i = 0;
    while (buffer[i]){
          _putchar(buffer[i]);
          i++;
    }
}
EXPORT_SYMBOL(debug_printk);
EXPORT_SYMBOL(fmem_get_pageinfo);
EXPORT_SYMBOL(fmem_give_pages);
EXPORT_SYMBOL(fmem_kvaddr_to_nid);
EXPORT_SYMBOL(fmem_pfn_to_nid);
EXPORT_SYMBOL(fmem_local_map_nr);
EXPORT_SYMBOL(fmem_get_ddr0_mask);
EXPORT_SYMBOL(fmem_get_ddr0_szbits);

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/miscdevice.h>
#include <linux/dma-mapping.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/mempool.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <mach/fmem.h>
#include "frammap.h"
#include "frm_api.h"

/* System memory default alignment
 */
static ushort sys_align = GRANU_ORDER;  /* 0 means PAGE alignment, 1: 1M alignment, 2: 2M alignment, .... 10:512k alignemnt */
module_param(sys_align, ushort, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(sys_align, "System memory alignment");

/* DDR-1 memory default alignment
 */
static ushort ddr1_align = GRANU_ORDER; /* 0 means PAGE alignment, 1: 1M alignment, 2: 2M alignment, .... 10:512k alignemnt */
module_param(ddr1_align, ushort, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(ddr1_align, "DDR-1 memory alignment");

/* DDR-2 memory default alignment
 */
static ushort ddr2_align = GRANU_ORDER; /* 0 means PAGE alignment, 1: 1M alignment, 2: 2M alignment, .... 10:512k alignemnt */
module_param(ddr2_align, ushort, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(ddr2_align, "DDR-2 memory alignment");

/* declare memory size parameter
 */
static ushort ddr0 = 0;
module_param(ddr0, ushort, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(ddr0, "system memory size in Mega");

static ushort ddr1 = 0;
module_param(ddr1, ushort, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(ddr1, "DDR-1 memory size in Mega");

static ushort ddr2 = 0;
module_param(ddr2, ushort, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(ddr2, "DDR-2 memory size in Mega");

static struct semaphore frammap_sema;

/* for backward compitable */
typedef struct kmem_cache kmem_cache_t;

#define SEMA_LOCK       down(&frammap_sema)
#define SEMA_UNLOCK     up(&frammap_sema)

/* page information from fmem.c
 */
static g_page_info_t *p_page_info;

/* 
 * Main structure for AP memory allocation
 */
typedef struct {
    int devidx;                 /* which DDR device */
    int frmidx;                 /* Frammap index for DDR user */
    struct semaphore sema;
    kmem_cache_t *node_cache;
    struct list_head node_list;
} private_data_t;

/*
 * Main structure of AP memory allocation
 */
typedef struct {
    struct frammap_buf_info buf_info;
    struct list_head list;
} ap_node_t;

static private_data_t private_data[MAX_DDR_NR];
extern struct miscdevice misdev[];
extern struct file_operations frmmap_fops;

/*
 * Macro definitions
 */
#define err(format, arg...)     printk(KERN_ERR "frammap(%s): " format "\n" , __FUNCTION__, ## arg)

#define STATS_INC_ALLOCATEHIT(x)	atomic_inc(&(x)->allochit)
#define STATS_INC_ALLOCATEMISS(x)	atomic_inc(&(x)->allocmiss)
#define STATS_INC_POOLBLK(x)        atomic_inc(&(x)->blk_inpool)
#define STATS_INC_ALLOC(x)	        atomic_inc(&(x)->blk_alloc)
#define STATS_DEC_ALLOC(x)	        atomic_dec(&(x)->blk_alloc)
#define STATS_INC_AVAIL(x)          atomic_inc(&(x)->blk_avail)
#define STATS_DEC_AVAIL(x)          atomic_dec(&(x)->blk_avail)

#define GFP_PRIVATE_SHIFT           22
#define SET_FRM_CACHE_TYPE(x, y)    do {(x) &= ~(0x3 << GFP_PRIVATE_SHIFT); \
                                        (x) |= ((y) << GFP_PRIVATE_SHIFT); \
                                    }while(0)

#define GET_FRM_CACHE_TYPE(x)       (((x) >> GFP_PRIVATE_SHIFT) & 0x3)

/* Define cache type
 */
#define CACHE_TYPE_CACHABLE         0x0
#define CACHE_TYPE_BUFFERABLE       0x1
#define CACHE_TYPE_NONCACHE         0x2

#define SET_FRM_ID(x, y)            do {(x) &= ~(0xFF << 24); (x) |= ((y) << 24); }while(0)
#define GET_FRM_ID(x)               (((x) >> 24) & 0xFF)

/*
 * Internal use, don't conflict to enum rmmp_index
 */
#define FRMIDX_DDR0     253
#define FRMIDX_DDR1     254
#define FRMIDX_DDR2     255

/*
 * Global variables declaration 
 */
#define NAME_SIZE       30
#define POOL_COUNT      50      /* define how many memory pools */
#define SIZE_ID_NONE    0

typedef enum {
    MEM_OWNER_NONE,
    MEM_OWNER_FRAMMAP,
    MEM_OWNER_KERNEL
} mem_owner_t;

/* Memory header for each allocated memory.
 */
typedef struct {
    u32 vaddr;                  //virtual address
    u32 paddr;                  //physical address
    DDR_ID_T ddr_id;            //which DDR is used
    SIZE_ID_T sizeid;           //corresponding to priv_mm_array
    int size;                   //allocate size
    int rmmp_index;             //who occupys this memory
    mem_owner_t owner;          //who is the owner, frammap or kernel?
    struct list_head list;
} memblk_hdr_t;

/*
 * DDR Descriptor
 */
typedef struct {
    u32 mem_base;
    u32 mem_end;
    u32 mem_size;               //whole memory size
    u32 avail_base;             //next avail base
    u32 allocated_cnt;          //how many blocks are allocated
    u32 avail_cnt;              //how many blocks are remaining
    u32 granular;               //real resolution
    char device_name[10];       //device name    
    struct list_head priv_mm_list;      //list of priv_mm_t
    struct list_head memblk_hdr_list;   //list of memory block header
    int valid;                  //entry valid or not
} ddr_desc_t;

static ddr_desc_t ddr_desc[MAX_DDR_NR];

#define SIZEID_TO_SIZE(x, ddr)    ((x) * ddr_desc[ddr].granular)
#define SIZE_TO_SIZEID(x, ddr)    ((x) / ddr_desc[ddr].granular)

/*
 * main structure
 */
typedef struct {
    SIZE_ID_T sizeid;           /* 0 means not used */
    mempool_t *pool;            /* memory pool */
    atomic_t allochit;
    atomic_t allocmiss;
    atomic_t blk_inpool;        /* how many slabs in pool */
    atomic_t blk_alloc;         /* how many slabs is allocated from DDR */
    atomic_t blk_avail;         /* how many slabs are available in cache_list */
    u32 blk_reserve;            /* how many slabs are reserved  */    
    char pool_name[20];
    struct list_head cache_list;        /* free object will be put here */
    struct list_head alloc_list;        /* allocated object will be put here */
    struct list_head list;      /* points to next pri_mm node */
} priv_mm_t;

/* memory cache for memory block header
 */
static kmem_cache_t *mb_hdr_cache;

/* 
 * MACRO defintion, x means DDR idx
 */
#define PRIV_MM_LIST(x)     ddr_desc[x].priv_mm_list
#define MEMBLK_HDR_LIST(x)  ddr_desc[x].memblk_hdr_list

/*
 * Local function declaration
 */
static DDR_ID_T get_ddrID(int rmmp_index);
static int frmmap_give_freepgs(int ddr_id, int size);
static void frm_proc_release(void);
static int frm_proc_init(void);

char ap_devname[MAX_DDR_NR][10];

#include "map.c"

/* Release vaddr
 */
void free_vaddr(memblk_hdr_t * entity)
{
    u32 vaddr = entity->vaddr;

    if (vaddr != 0) {
        __iounmap((void *)vaddr);
        entity->vaddr = 0;
    }
}

/*
 * @brief allocate virtual address based on the flag of cacheable
 *
 * @function u32 alloc_vaddr(memblk_hdr_t * entity, u32 vsize, int cacheable)
 * @param paddr indicate the physical address
 * @param vsize indicate the allocation size
 * @param cacheable indicate the MMU property
 * @return vaddr on success, 0 on error
*/
static u32 alloc_vaddr(u32 paddr, u32 vsize, int cacheable)
{
    u32 vaddr = 0;

    if (cacheable == ALLOC_CACHEABLE) {
        vaddr = (u32) ioremap_cached(paddr, (size_t) vsize);
    } else if (cacheable == ALLOC_BUFFERABLE) {
        vaddr = (u32) ioremap_wc(paddr, (size_t) vsize);
    } else {
        vaddr = (u32) ioremap_nocache(paddr, (size_t) vsize);
    }
    return vaddr;
}

/*
 * @brief get DDR ID of this module belonging to
 *
 * @function DDR_ID_T get_ddrID(int rmmp_index)
 * @param rmmp_index indicates the user module index 
 *
 * @return DDR id
 */
DDR_ID_T get_ddrID(int rmmp_index)
{
    int i;

    /* check the exception first */
    switch (rmmp_index) {
    case FRMIDX_AP0:    
    case FRMIDX_DDR0:
        return DDR_ID_SYSTEM;

    case FRMIDX_AP1:
    case FRMIDX_DDR1:
        return DDR_ID1;

    case FRMIDX_AP2:
    case FRMIDX_DDR2:
        return DDR_ID2;
    default:
        break;
    }

    for (i = 1; i < ARRAY_SIZE(id_string_arry); i++) {
        if (id_string_arry[i].rmmp_index == rmmp_index)
            return id_string_arry[i].ddr_id;
    }

    return DDR_ID_SYSTEM;
}

/*
 * @brief get memory pool
 *
 * @function priv_mm_t *get_pool_mm(SIZE_ID_T size_id, int ddr_id, int alloc)
 * @param size_id indicates which step of the granular
 * @param ddr_id indicate the ddr index
 * @param alloc indicate if it needs to pool allocation if pool doesn't exist
 *
 * @return 0 on success, !0 on error
 */
priv_mm_t *get_pool_mm(SIZE_ID_T size_id, int ddr_id, int alloc)
{
    int bFound = 0;
    priv_mm_t *node;

    if (unlikely(ddr_id >= MAX_DDR_NR))
        return NULL;

    if (!list_empty(&PRIV_MM_LIST(ddr_id))) {
        /* search the database to see if we have such memory pool already 
         */
        list_for_each_entry(node, &PRIV_MM_LIST(ddr_id), list) {
            if (node->sizeid == size_id) {
                bFound = 1;
                break;
            }
        }
    }

    /* allocate a new memory pool node */
    if (!bFound) {
        if (alloc) {
            node = kzalloc(sizeof(priv_mm_t), GFP_ATOMIC);
            if (node == NULL) {
                printk("FRAMMAP: kmalloc fail! \n");
                goto exit;
            }

            /* add to linklist */
            INIT_LIST_HEAD(&node->list);
            node->sizeid = size_id;
            list_add_tail(&node->list, &PRIV_MM_LIST(ddr_id));
        } else {
            node = NULL;
            goto exit;
        }
    }

  exit:
    return node;
}

/*
 * @brief find the nearest sizeid according to size
 *
 * @function SIZE_ID_T size_to_sizeid(u32 size, int ddr_id)
 * @param size indicates the user allocation size
 * @param ddr_id indicate the ddr index 
 *
 * @return 0 on success, !0 on error
 */
SIZE_ID_T size_to_sizeid(u32 size, int ddr_id)
{
    SIZE_ID_T sizeid;
    u32 rsize;

    /* Change to granular resolution 
     */
    rsize =
        ((size + ddr_desc[ddr_id].granular -
          1) / ddr_desc[ddr_id].granular) * ddr_desc[ddr_id].granular;

    /* Change to steping ID
     */
    sizeid = SIZE_TO_SIZEID(rsize, ddr_id);

    return sizeid;
}

/*
 * @brief allocate the memory from memory pool 
 *
 * @function void *memory_alloc_refill(DDR_ID_T ddr_id, priv_mm_t *priv_mm, int rmmp_index, int cacheable)
 * @param ddr_id indicate the DDR id
 * @param priv_mm indicate the memory pool info
 * @param rmmp_index indicate frammap index
 * @param rmmp_index indicate MMU flag 
 * @return memory or NULL if fail
 */
static void *memory_alloc_refill(DDR_ID_T ddr_id, priv_mm_t * priv_mm, int rmmp_index,
                                 int cacheable)
{
    u32 vaddr, paddr;
    //unsigned long flags;
    memblk_hdr_t *mb_hdr, *mb_hdr2;
    void *objp = NULL;

    /*
     * First, update DDR descriptor
     */
    if (unlikely(!ddr_desc[ddr_id].valid)) {
        err("Invalid DDR %d!\n", ddr_id);
        return NULL;
    }

    mb_hdr = (memblk_hdr_t *) kmem_cache_alloc(mb_hdr_cache, GFP_KERNEL);
    mb_hdr2 = (memblk_hdr_t *) kmem_cache_alloc(mb_hdr_cache, GFP_KERNEL);
    if (unlikely(mb_hdr2 == NULL)) {
        printk("Frammap: No cache memory for block header! \n");
        return NULL;
    }
    memset(mb_hdr, 0, sizeof(memblk_hdr_t));
    memset(mb_hdr2, 0, sizeof(memblk_hdr_t));

    /* save irq */    
    if (unlikely(ddr_desc[ddr_id].avail_base >= ddr_desc[ddr_id].mem_end))
        goto nomem;

    if (unlikely
        (ddr_desc[ddr_id].avail_base + SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id) >
         ddr_desc[ddr_id].mem_end))
        goto nomem;

    if (ddr_desc[ddr_id].avail_cnt <
        (SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id) / ddr_desc[ddr_id].granular)) {
        err("BUG! \n");
        goto nomem;
    }

    paddr = ddr_desc[ddr_id].avail_base;
    
    /*
     * Second, create memory block
     */
    //cacheable or non-cacheable
    vaddr = alloc_vaddr(paddr, SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id), cacheable);
    mb_hdr->paddr = (u32) paddr;
    mb_hdr->vaddr = vaddr;
    mb_hdr->sizeid = priv_mm->sizeid;
    mb_hdr->size = SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id);     //reference only
    mb_hdr->ddr_id = ddr_id;
    mb_hdr->rmmp_index = rmmp_index;
    mb_hdr->owner = MEM_OWNER_FRAMMAP;
    INIT_LIST_HEAD(&mb_hdr->list);

    /* 
     * Third, update priv_mm 
     */    

    /* Add this memory block into alloc_list. If this memory is released, it will move to cache_list in the future. 
     */
    if (vaddr) {
        list_add_tail(&mb_hdr->list, &priv_mm->alloc_list);
        memcpy(mb_hdr2, mb_hdr, sizeof(memblk_hdr_t));
        INIT_LIST_HEAD(&mb_hdr2->list);
        list_add_tail(&mb_hdr2->list, &MEMBLK_HDR_LIST(ddr_id));
        STATS_INC_POOLBLK(priv_mm);     //Get the block from DDR, so increase 1
        STATS_INC_ALLOC(priv_mm);

        /* move to next avaliable base */
        ddr_desc[ddr_id].avail_base += SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id);
        ddr_desc[ddr_id].allocated_cnt +=
            (SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id) / ddr_desc[ddr_id].granular);
        ddr_desc[ddr_id].avail_cnt -=
            (SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id) / ddr_desc[ddr_id].granular);

        objp = mb_hdr;
    } else {
        /* no virtual address for this allocation */
        objp = NULL;
        printk("Frammap: No virtual address for allocated size = 0x%x! \n",
               SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id));
        goto nomem;
    }    

    return (void *)objp;        //points to mb

  nomem:    

    /* free this header */
    kmem_cache_free(mb_hdr_cache, mb_hdr);
    kmem_cache_free(mb_hdr_cache, mb_hdr2);

    printk("Frammap:No memory in DDR %d, allocated size = 0x%x! \n", ddr_id,
           SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id));
    return NULL;
}

/*
 * @brief This function allocates memory from memory pool and then put it in cache.
 *        If the one wants to allcate memory, lookup the cache first.
 *
 * @function void *mempool_alloc_memory(gfp_t gfp_mask, void *pool_data)
 * @param gfp_mask indicate GFP_MASK and some info is hidden in this flag
 * @param pool_data indicate the memory pool
 * @return memory or NULL if fail
 */
void *mempool_alloc_memory(gfp_t gfp_mask, void *pool_data)
{
    priv_mm_t *priv_mm = (priv_mm_t *) pool_data;
    void *objp = NULL;
    DDR_ID_T ddr_id;
    //unsigned long flags;
    struct list_head *node;
    memblk_hdr_t *mb_hdr, *mb_hdr2;
    int rmmp_index, cacheable;

    rmmp_index = GET_FRM_ID(gfp_mask);
    cacheable = GET_FRM_CACHE_TYPE(gfp_mask);

    ddr_id = get_ddrID(rmmp_index);

    /* check if it is in cache first. If not, gets memory from memory pool.
     */

    //refer to a piece of kernel code... objp = ____cache_alloc(cachep, flags); 
    if (likely(atomic_read(&priv_mm->blk_avail))) {
        u32 vaddr;
        
        /*pick the memory up from cache_list and put it in alloc_list */
        if (unlikely(list_empty(&priv_mm->cache_list))) {            
            err("BUG!, list_empty() is true, but priv_mm->blk_avail is %d\n",
                atomic_read(&priv_mm->blk_avail));

            return NULL;
        }
        node = priv_mm->cache_list.next;
        /* find the hdr block according to list which is a member of memblk_hdr_t. In this case, 
         * we don't need to allocate header.
         */
        mb_hdr = list_entry(node, memblk_hdr_t, list);

        /* allocate vaddr */
        vaddr = alloc_vaddr(mb_hdr->paddr, SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id), cacheable);
        mb_hdr->rmmp_index = 0; //parking
        if (vaddr) {
            mb_hdr->rmmp_index = rmmp_index;
            mb_hdr->owner = MEM_OWNER_FRAMMAP;
            mb_hdr->vaddr = vaddr;
            objp = mb_hdr;      /* Note: objp will be still processed after return. */
            /* move from cache_list to alloc list */
            list_move_tail(&mb_hdr->list, &priv_mm->alloc_list);

            STATS_INC_ALLOCATEHIT(priv_mm);
            STATS_INC_ALLOC(priv_mm);
            STATS_DEC_AVAIL(priv_mm);

            mb_hdr2 = (memblk_hdr_t *) kmem_cache_alloc(mb_hdr_cache, GFP_KERNEL);
            if (mb_hdr2 == NULL)
                panic("Frammap: No memory for mb_hdr2! \n");

            /* create new node in the header node list for lookup
             */
            memcpy(mb_hdr2, mb_hdr, sizeof(memblk_hdr_t));
            INIT_LIST_HEAD(&mb_hdr2->list);
            list_add_tail(&mb_hdr2->list, &MEMBLK_HDR_LIST(ddr_id));
        } else {
            /* Still stay in allo_list 
             */
            printk("Frammap: No virtual address for allocated size = 0x%x! \n",
                   SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id));
        }
        
    } else {
        STATS_INC_ALLOCATEMISS(priv_mm);
        objp = memory_alloc_refill(ddr_id, priv_mm, rmmp_index, cacheable);
    }

    return objp;                // points to memblk_hdr_t
}

/*
 * element: the memory address is going to be freed.
 */
static void mempool_free_memory(void *element, void *pool_data)
{
    priv_mm_t *priv_mm = (priv_mm_t *) pool_data;
    memblk_hdr_t *mb_hdr = (memblk_hdr_t *) element;
    memblk_hdr_t *entry, *ne;
    DDR_ID_T ddr_id;
    //unsigned long flags;
    int bFound = 0;

    ddr_id = get_ddrID(mb_hdr->rmmp_index);

    /* remove the node from header link list 
     */
    if (!list_empty(&MEMBLK_HDR_LIST(ddr_id))) {
        list_for_each_entry_safe(entry, ne, &MEMBLK_HDR_LIST(ddr_id), list) {
            if ((entry->paddr == mb_hdr->paddr) && (entry->vaddr == mb_hdr->vaddr)) {
                list_del(&entry->list);
                kmem_cache_free(mb_hdr_cache, entry);
                bFound = 1;
                break;
            }
        }
        if (!bFound)
            printk("BUG in Frammap!!! \n");
    }

    /*
     * Release the relate memory and update the statistic
     */    
    /* release the virtual address */
    free_vaddr(mb_hdr);

    /* move from alloc_list to cache_list */
    list_move_tail(&mb_hdr->list, &priv_mm->cache_list);
    STATS_DEC_ALLOC(priv_mm);
    STATS_INC_AVAIL(priv_mm);    

    return;
}

/*
 * @brief find the memory pool. Create a new one if it doesn't exist.
 *
 * @function mempool_t *find_memory_pool(SIZE_ID_T sizeid, int min_nr, DDR_ID_T ddr_id)
 * @param sizeid indicate the size id of granular
 * @param min_nr indicate low water marker. This value is used in kernel. Currently it should be 0.
 * @param min_nr indicate DDR id
 * @return memory pool if success. NULL for fail.
*/
static mempool_t *find_memory_pool(SIZE_ID_T sizeid, int min_nr, DDR_ID_T ddr_id)
{
    priv_mm_t *priv_mm;
    
#ifdef CONFIG_ISOLATE_HIGHMEM
    /* In DDR0, if HIGHMEM Zone exists, it means highmem is managed by kernel, instead of frammap 
     */
    if (ddr_id == DDR_ID_SYSTEM) 
        return (void *)0xEFEF;
#endif
    
    priv_mm = get_pool_mm(sizeid, ddr_id, 1);

    if (!priv_mm) {
        printk("Frammap: No memory for memory pool! \n");
        return NULL;
    }

    if (priv_mm->pool != NULL)
        return priv_mm->pool;

    /*
     * Create a new memory pool entry
     */
    priv_mm->sizeid = sizeid;
    priv_mm->blk_reserve = min_nr;
    sprintf(priv_mm->pool_name, "mempool_%d", sizeid);
    INIT_LIST_HEAD(&priv_mm->cache_list);
    INIT_LIST_HEAD(&priv_mm->alloc_list);
    
    /* it will call mempool_alloc_memory() to reserve min_nr blocks
     */
    priv_mm->pool = mempool_create(min_nr,
                                   mempool_alloc_memory, mempool_free_memory, (void *)priv_mm);
    if (unlikely(!priv_mm->pool)) {
        /* release this memory pool due to get_pool_mm() */
        priv_mm->sizeid = 0;
        return NULL;
    }

    return priv_mm->pool;
}

void dump_highmem_info(u32 paddr, int size)
{
    if (paddr || size) {
    }
}

/**
 * @brief de-allocate a buffer. It is a pair of frm_release_buf_info(). Note: The return value
 * @brief info->size will be update according to the real allocated size.
 *
 * @parm rmmp_index indicates the unique module id given by FRAMMAP. 
 * @parm info->size indicates the allocated memory size.
 *
 * @return  0 for success, otherwise for fail.
 */
int frm_get_buf_info(u32 rmmp_index, struct frammap_buf_info *info)
{
    int size, cacheable = 0, retVal = -1;
    unsigned long flags;
    SIZE_ID_T sizeid;
    mempool_t *pool = NULL;
    memblk_hdr_t *mb_hdr = NULL;
    DDR_ID_T ddr_id;
    gfp_t gfp_mask;
        
    if (info == NULL) {
        err("Caller passes null pointer! rmmp_index = %d!\n", rmmp_index);
        return -1;
    }

    ddr_id = get_ddrID(rmmp_index);

    if (unlikely(!ddr_desc[ddr_id].valid)) {
        err("%s get memory from DDR %d which is not active! \n", id_to_namestring(rmmp_index),
            ddr_id);
        return -1;
    }
    
    SEMA_LOCK;
    
    size = PAGE_ALIGN(info->size);
    sizeid = size_to_sizeid(size, ddr_id);

    if (sizeid == SIZE_ID_NONE) {
        err("Can't find a match size with size = %x for %s\n", info->size,
            id_to_namestring(rmmp_index));
            
        retVal = -1;
        goto exit;
    }
        
    pool = find_memory_pool(sizeid, 0, ddr_id); //it will call mempool_alloc to reserve memory
    if (unlikely(pool == NULL)) {
        printk(KERN_ERR "Frammap: can't create mempool with sizeid = %d for %s\n", sizeid,
               id_to_namestring(rmmp_index));

        retVal = -1;
        goto exit;
    }
    
    if (pool == (void *)0xEFEF)    
        pool = NULL;
            
    gfp_mask = GFP_ATOMIC;
    SET_FRM_ID(gfp_mask, rmmp_index);
    
    switch (info->alloc_type) {
    case ALLOC_CACHEABLE:
        flags = PAGE_SHARED;
        cacheable = CACHE_TYPE_CACHABLE;
        break;
    case ALLOC_BUFFERABLE:
        flags = pgprot_writecombine(pgprot_kernel);
        cacheable = CACHE_TYPE_BUFFERABLE;
        break;
    default:
        flags = pgprot_noncached(pgprot_kernel);
        cacheable = CACHE_TYPE_NONCACHE;
        break;
    }
    
    SET_FRM_CACHE_TYPE(gfp_mask, cacheable);

    if (pool != NULL)
        mb_hdr = (memblk_hdr_t *) mempool_alloc(pool, gfp_mask);

    if (mb_hdr != NULL) {
        /* who gets this memory */
        //mb_hdr->rmmp_index = rmmp_index;
        info->phy_addr = mb_hdr->paddr;
        info->va_addr = mb_hdr->vaddr;
        info->size = size;
        retVal = 0;
    } else {
        u32 vaddr;
        dma_addr_t paddr;
        memblk_hdr_t *mb_hdr;

        retVal = -1;
        
        if (pool != NULL) {
            printk("Frammap: No memory for rmmp_index=%d(%s) with size = %d bytes! "
                   "Try to allocate kernel memory! \n", 
                   rmmp_index, id_to_namestring(rmmp_index), size);
        }
        
        /* allocate highmem zone */
        vaddr = (u32) fmem_alloc_ex(size, &paddr, flags, ddr_id);
            
        if (vaddr) {
            /* allocate a memory block header for this allocation */
            mb_hdr = (memblk_hdr_t *) kmem_cache_alloc(mb_hdr_cache, GFP_KERNEL);
            if (unlikely(mb_hdr == NULL)) {
                printk("Frammap: No cache memory for block header! \n");
                fmem_free_ex(size, (void *)vaddr, paddr);
                goto exit;
            }

            memset(mb_hdr, 0, sizeof(memblk_hdr_t));
            mb_hdr->rmmp_index = rmmp_index;
            mb_hdr->paddr = (u32) paddr;
            mb_hdr->vaddr = vaddr;
            mb_hdr->ddr_id = ddr_id;
            mb_hdr->size = size;
            mb_hdr->owner = MEM_OWNER_KERNEL;
            /* put the memory block header into the list */
            INIT_LIST_HEAD(&mb_hdr->list);
            list_add_tail(&mb_hdr->list, &MEMBLK_HDR_LIST(ddr_id));
            info->phy_addr = mb_hdr->paddr;
            info->va_addr = mb_hdr->vaddr;
            info->size = size;
            retVal = 0;
        }
    }

  exit:  
    SEMA_UNLOCK;
    return retVal;
}

EXPORT_SYMBOL(frm_get_buf_info);

/*
 * @brief get memblk header which was allocated. It includes the node for the kernel
 *
 * @function static inline int func(int value, int type)
 * @param value is used to indicate ¡K
 * @param type indicate the format¡K
 * @return 0 on success, !0 on error
*/
static memblk_hdr_t *get_memblk_header(u32 rmmp_index, struct frammap_buf_info *info)
{
    int bFound = 0, ddr_id;
    memblk_hdr_t *node;

    ddr_id = get_ddrID(rmmp_index);
    if (unlikely(!ddr_desc[ddr_id].valid)) {
        err("Frammp: %s get memory from DDR %d which is not active! \n",
            id_to_namestring(rmmp_index), ddr_id);
        return NULL;
    }

    list_for_each_entry(node, &MEMBLK_HDR_LIST(ddr_id), list) {
        if ((node->rmmp_index == rmmp_index) &&
            (node->paddr == info->phy_addr) && (node->vaddr == info->va_addr)) {
            bFound = 1;
            break;
        }
    }
    if (bFound)
        return node;

    return NULL;
}

/**
 * @brief de-allocate a buffer. It is a pair of frm_get_buf_info()
 *
 * @parm rmmp_index indicates the unique module id given by FRAMMAP. Besides, both info->va_addr and
 *          info->phy_addr are necessary.
 *
 * @return  0 for success, otherwise for fail.
 */
int frm_release_buf_info(u32 rmmp_index, struct frammap_buf_info *info)
{
    memblk_hdr_t *mb_hdr, *mb_hdr_alloc = NULL;
    priv_mm_t *priv_mm;
    struct list_head *node;
    int bMatched = 0;
    //unsigned long flags;
    int inid, retVal = -1;
    
    SEMA_LOCK;
    
    mb_hdr = get_memblk_header(rmmp_index, info);
    if (mb_hdr == NULL) {
        printk("Frammap: Can't find proper node for %s! vaddress = 0x%x, paddress = 0x%x\n",
               id_to_namestring(rmmp_index), info->va_addr, info->phy_addr);
                
        goto exit;
    }

    if (mb_hdr->owner == MEM_OWNER_KERNEL) {
        memblk_hdr_t *entry, *ne;
        int bFound = 0;

        fmem_free_ex(mb_hdr->size, (void *)mb_hdr->vaddr, mb_hdr->paddr);

        /* check the array and remove it from the header list */
        list_for_each_entry_safe(entry, ne, &MEMBLK_HDR_LIST(mb_hdr->ddr_id), list) {
            if ((entry->paddr == mb_hdr->paddr) && (entry->vaddr == mb_hdr->vaddr)) {
                list_del(&entry->list);
                kmem_cache_free(mb_hdr_cache, entry);
                bFound = 1;
                break;
            }
        }
        
        retVal = 0;
        
        if (!bFound) {
            printk("Oh~ BUG in Frammap!!! \n");
            retVal = -1;
        }        
        
        goto exit;
    }
    
    inid = mb_hdr->ddr_id;
    priv_mm = get_pool_mm(mb_hdr->sizeid, inid, 0);

    if (unlikely((priv_mm == NULL) || (priv_mm->pool == NULL))) {
        printk("Frammap:%s, bad in memory header: sizeid = %d is not allocated before! \n",
               id_to_namestring(rmmp_index), mb_hdr->sizeid);
                
        goto exit;
    }
    
    /*check if it is in alloc_list. It should be YES and move to cache_list */
    list_for_each(node, &priv_mm->alloc_list) {
        mb_hdr_alloc = list_entry(node, memblk_hdr_t, list);
        if ((mb_hdr_alloc->vaddr == mb_hdr->vaddr) && (mb_hdr_alloc->paddr == mb_hdr->paddr)) {
            bMatched = 1;
            break;
        }
    }    

    if (unlikely(!bMatched)) {
        printk("Frammap:%s, can't find a match memory block in alloc_list, rmmp_index:%d \n",
               id_to_namestring(rmmp_index), rmmp_index);
        goto exit;
    }
    
    retVal = 0;
    /* free memory. It just move the node from alloc_list to cache_list */
    mempool_free(mb_hdr_alloc, priv_mm->pool);

  exit:
    SEMA_UNLOCK;
    //module_put(THIS_MODULE);

    return retVal;
}

EXPORT_SYMBOL(frm_release_buf_info);

/**
 * @brief allocate a buffer from the designated DDR. It is a pair of frm_free_buf_ddr().
 * @brief Note: the return value info->size will be update according to the real allocated size.
 * 
 * @parm ddr_id is used to indicate the designated DDR
 *
 * @parm info->size in info is necessary.
 * @return  0 for success, otherwise for fail.
 */
int frm_get_buf_ddr(DDR_ID_T ddr_id, struct frammap_buf_info *info)
{
    int rmmp_index, ret;

    switch (ddr_id) {
    case DDR_ID_SYSTEM:
        rmmp_index = FRMIDX_DDR0;
        break;
    case DDR_ID1:
        rmmp_index = FRMIDX_DDR1;
        break;
    case DDR_ID2:
        rmmp_index = FRMIDX_DDR2;
        break;
    default:
        return -1;
    }

    ret = frm_get_buf_info(rmmp_index, info);

    return ret;
}

EXPORT_SYMBOL(frm_get_buf_ddr);

/**
 * @brief de-allocate a buffer from the designated DDR. It is a pair of frm_get_buf_ddr()
 *
 * @parm both info->va_addr and info->phy_addr are necessary.
 * @return  0 for success, otherwise for fail.
 */
int frm_free_buf_ddr(struct frammap_buf_info *info)
{
    int rmmp_index, ret;
    
    SEMA_LOCK;
    
    if (ddr_desc[0].valid && get_memblk_header(FRMIDX_DDR0, info))
        rmmp_index = FRMIDX_DDR0;
    else if (ddr_desc[1].valid && get_memblk_header(FRMIDX_DDR1, info))
        rmmp_index = FRMIDX_DDR1;
    else if (ddr_desc[2].valid && get_memblk_header(FRMIDX_DDR2, info))
        rmmp_index = FRMIDX_DDR2;
    else {
        printk
            ("Frammap: Can't find proper node for frm_free_buf_ddr()! vaddress = 0x%x, paddress = 0x%x\n",
             info->va_addr, info->phy_addr);
             
        SEMA_UNLOCK;
        return -1;
    }
    
    SEMA_UNLOCK;
    
    ret = frm_release_buf_info(rmmp_index, info);

    return ret;
}

EXPORT_SYMBOL(frm_free_buf_ddr);

/**
 * @brief put the remaining unused memory to the kernel
 * 
 * @parm ddr_id indicate whihc DDR will give the un-used pages to the kernel
 * @return  0 for success, otherwise for fail.
 */
int frmmap_give_freepgs(int ddr_id, int size)
{
    u32 give_sz;
    int ret;

    if (unlikely(!ddr_desc[ddr_id].valid))
        return -1;

    if (!size) {
        give_sz = ddr_desc[ddr_id].mem_end - ddr_desc[ddr_id].avail_base;
        give_sz = (give_sz >> PAGE_SHIFT) << PAGE_SHIFT;
    } else
        give_sz = (size >> PAGE_SHIFT) << PAGE_SHIFT;

    if (unlikely(!give_sz)) {
        printk("Frammap: DDR%d No memory to free! \n", ddr_id);
        return 0;               /* no more memory to give */
    }

    ret = fmem_give_pages(ddr_id, give_sz);
    if (ret > 0) {
        /* update the ddr database */
        ddr_desc[ddr_id].mem_size = p_page_info->node_sz[ddr_id];
        ddr_desc[ddr_id].avail_cnt -= (ret / ddr_desc[ddr_id].granular);
        ddr_desc[ddr_id].mem_end = ddr_desc[ddr_id].mem_base + ddr_desc[ddr_id].mem_size;

        printk("Frammap: %d pages in DDR%d are freed. \n", (ret >> PAGE_SHIFT), ddr_id);
        return 0;
    }

    return -1;
}

#include <linux/kernel.h>

#include <asm/io.h>
#include <asm/uaccess.h>
/*
 * Entry point of frammap
 */
static int __init frammap_init(void)
{
    int ddr_idx, i, ret;
    u32 parameter_ddr_sz = 0;
    
    memset(&ddr_desc[0], 0, sizeof(ddr_desc));
    memset(misdev, 0, MAX_DDR_NR * sizeof(struct miscdevice));
    memset(&private_data[0], 0x0, MAX_DDR_NR * sizeof(private_data_t));

    if (__GFP_BITS_SHIFT >= GFP_PRIVATE_SHIFT) {
        printk("Frammp: new __GFP_BITS_SHIFT is %x \n", __GFP_BITS_SHIFT);
        panic("Please modify the code!");
    }

    printk("\n");
    /* Get page related information from kernel.
     */
    fmem_get_pageinfo(&p_page_info);

#if 0                           // debug only */
    {
        int node_id;
        page_node_t *page_node;
        struct list_head *node;

        for (node_id = 0; node_id < p_page_info->nr_node; node_id++) {
            list_for_each(node, &p_page_info->list[node_id]) {
                page_node = list_entry(node, page_node_t, list);
                printk("ddr = %d, pg = %x, addr = %x \n", node_id, page_node->page,
                       page_to_phys(page_node->page));
            }
            printk(" \n\n\n");
        }
    }
#endif

    /*
     * Prepare the database for the DDR related information.
     */
    for (ddr_idx = 0; ddr_idx < p_page_info->nr_node; ddr_idx++) {
        ddr_desc[ddr_idx].mem_base = p_page_info->phy_start[ddr_idx];
        ddr_desc[ddr_idx].mem_size = p_page_info->node_sz[ddr_idx];
        ddr_desc[ddr_idx].mem_end = p_page_info->phy_start[ddr_idx] + p_page_info->node_sz[ddr_idx];
        ddr_desc[ddr_idx].avail_base = p_page_info->phy_start[ddr_idx];

        if (ddr_idx == 0)
            ddr_desc[ddr_idx].granular = (0x1000 << sys_align);
        if (ddr_idx == 1)
            ddr_desc[ddr_idx].granular = (0x1000 << ddr1_align);
        if (ddr_idx == 2)
            ddr_desc[ddr_idx].granular = (0x1000 << ddr2_align);

        ddr_desc[ddr_idx].avail_cnt = ddr_desc[ddr_idx].mem_size / ddr_desc[ddr_idx].granular;
        
        /* memory pool list */
        INIT_LIST_HEAD(&PRIV_MM_LIST(ddr_idx));
        /* memory header list */
        INIT_LIST_HEAD(&MEMBLK_HDR_LIST(ddr_idx));

        ddr_desc[ddr_idx].valid = 1;

        if (ddr_idx + 1 >= MAX_DDR_NR)
            return -1;

        /* change to mega bytes */
        if (ddr_idx == 0)
            parameter_ddr_sz = ddr0 << 20;
        if (ddr_idx == 1)
            parameter_ddr_sz = ddr1 << 20;
        if (ddr_idx == 2)
            parameter_ddr_sz = ddr2 << 20;

        if (parameter_ddr_sz != 0) {
            if (parameter_ddr_sz > p_page_info->node_sz[ddr_idx]) {
                printk("Frammap: DDR%d size only has %dM bytes! \n", ddr_idx,
                       p_page_info->node_sz[ddr_idx] >> 20);
                return -1;
            }

            /* at least one page left, then we can release memory */
            if ((parameter_ddr_sz + PAGE_SIZE) < p_page_info->node_sz[ddr_idx])
                frmmap_give_freepgs(ddr_idx, p_page_info->node_sz[ddr_idx] - parameter_ddr_sz);
        }

        printk("Frammap: DDR%d: memory base=0x%x, memory size=0x%x, alignment=%dK \n", ddr_idx,
               ddr_desc[ddr_idx].mem_base, ddr_desc[ddr_idx].mem_size,
               ddr_desc[ddr_idx].granular >> 10);
    }

    mb_hdr_cache = kmem_cache_create("frm_hdr_cache", sizeof(memblk_hdr_t), 0, 0, NULL);
    if (!mb_hdr_cache) {
        printk("Frammap: create kmem_cache_create fail! \n");
        return -EFAULT;
    }

    frm_proc_init();
    frmmap_read_cfgfile(CFG_FILE_PATH);

    printk("Frammap: version %s, %d DDR is managed. \n", FRM_VERSION_CODE, p_page_info->nr_node);

    /* 
     * create memory node for AP to allocate memory or IOCTL 
     */
    for (i = 0; i < p_page_info->nr_node; i++) {
        misdev[i].minor = FRAMMAP_MINOR + i;
        memset(ddr_desc[i].device_name, 0x0, sizeof(ddr_desc[i].device_name));
        sprintf(ddr_desc[i].device_name, "frammap%d", i);
        misdev[i].name = ddr_desc[i].device_name;
        misdev[i].fops = (struct file_operations *)&frmmap_fops;
        ret = misc_register(&misdev[i]);
        if (ret < 0) {
            printk("Frammap: register misc device %d fail! \n", i);
            memset(&misdev[i], 0, sizeof(struct miscdevice));   /* reset the structure */
        }
    }
    
    init_MUTEX(&frammap_sema);

    return 0;
}

static void __exit frammap_exit(void)
{
    int dev, ddr;
    memblk_hdr_t *entry, *ne;
    priv_mm_t *priv_mm, *mm_ne;
                
    for (ddr = DDR_ID_SYSTEM; ddr < MAX_DDR_NR; ddr++) {
        if (ddr_desc[ddr].valid == 0)
            continue;

        /* destroy the memory pool */
        list_for_each_entry_safe(priv_mm, mm_ne, &PRIV_MM_LIST(ddr), list) {
            if (!list_empty(&priv_mm->cache_list)) {
                list_for_each_entry_safe(entry, ne, &priv_mm->cache_list, list) {
                    list_del(&entry->list);
                    free_vaddr(entry);
                    kmem_cache_free(mb_hdr_cache, entry);
                }
            }

            /* free kernel pool first. To have element freed in the kernel */
            mempool_destroy(priv_mm->pool);
            priv_mm->pool = NULL;

            /* check who not free memory and cause memory leakage */
            if (!list_empty(&priv_mm->alloc_list)) {
                list_for_each_entry_safe(entry, ne, &priv_mm->alloc_list, list) {
                    printk("rmmp_index %d(%s): Cause memory leakage! \n", entry->rmmp_index,
                           id_to_namestring(entry->rmmp_index));
                    list_del(&entry->list);
                    free_vaddr(entry);
                    kmem_cache_free(mb_hdr_cache, entry);
                }
            }

            /* release the memory */
            kfree(priv_mm);
        }

        /* free all nodes in the header link list 
         */
        list_for_each_entry_safe(entry, ne, &MEMBLK_HDR_LIST(ddr), list) {
            list_del(&entry->list);
            kmem_cache_free(mb_hdr_cache, entry);
        }
    }

    frm_proc_release();

    /* destroy the cache */
    kmem_cache_destroy(mb_hdr_cache);

    /* release the device node */
    for (dev = 0; dev < MAX_DDR_NR; dev++) {
        if (ddr_desc[dev].valid == 0)
            continue;

        if (misdev[dev].name != NULL)
            misc_deregister(&misdev[dev]);
    }
    return;
}

/** 
 * @brief Get memory allocation alignment.
 * @parm None
 * @return return the memory alignment. It is suitable for system memory.
 */
u32 frm_get_sysmem_alignment(void)
{
    return ddr_desc[0].granular;
}

/*
 * @This function gets the managed memory allocation alignment.
 *
 * @function u32 frm_get_memory_alignment(void)
 * @param: ddr_id: ddr idx from 0 
 * @return value: 2^align * 4K or 0 for fail
 */
u32 frm_get_memory_alignment(ddr_id_t ddr_id)
{
    u32 ret = 0;

    if (ddr_id >= MAX_DDR_NR)
        return 0;

    if (ddr_desc[0].valid)
        ret = ddr_desc[0].granular;

    return ret;
}

EXPORT_SYMBOL(frm_get_sysmem_alignment);

static struct proc_dir_entry *frm_proc_root = NULL;
static struct proc_dir_entry *ddr_info = NULL;
static struct proc_dir_entry *mmap0 = NULL;
static struct proc_dir_entry *mmap1 = NULL;
static struct proc_dir_entry *mmap2 = NULL;
static struct proc_dir_entry *config_file = NULL;
static struct proc_dir_entry *mem_ownerinfo = NULL;
static struct proc_dir_entry *frammap_debug = NULL;
static struct proc_dir_entry *frammap_free_pages = NULL;
static struct proc_dir_entry *mem_kernel = NULL;

/**
 * @brief Show the DDR usage status and memory allocation distribution.
 *
 * @parm None.
 *
 * @return  None.
 */
void show_ddr_info(void)
{
    int i, ddr_id, minus_one;
    priv_mm_t *priv_mm;

    for (i = DDR_ID_SYSTEM; i < MAX_DDR_NR; i++) {
        if (!ddr_desc[i].valid)
            continue;
        
        minus_one = (ddr_desc[i].mem_base == ddr_desc[i].mem_end) ? 0 : 1;
                        
        printk("Memory Statistic: DDR:%d, each block size = %dK \n", i, ddr_desc[i].granular >> 10);
        printk("MemBase       MemEnd        Next Avail    AllocBlks   FreeBlks   MemSize\n");
        printk("----------------------------------------------------------------------------\n");
        printk("0x%-10x  0x%-10x  0x%-10x  %-10d  %-10d 0x%x\n\n", ddr_desc[i].mem_base,
               ddr_desc[i].mem_end - minus_one, ddr_desc[i].avail_base, ddr_desc[i].allocated_cnt,
               ddr_desc[i].avail_cnt, ddr_desc[i].mem_size);
    }

    if (mmap0) {
        ddr_id = DDR_ID_SYSTEM;
        printk("\nMemory Pool Distribution in DDR: 0 \n");
        printk("MemSize(K)  AvailBlk  AllocBlk  ReserveBlk  Blk_in_Pool  CacheHit  CacheMiss \n");
        printk("---------------------------------------------------------------------------- \n");

        list_for_each_entry(priv_mm, &PRIV_MM_LIST(ddr_id), list) {
            printk("%-10d  %-8d  %-8d  %-10d  %-11d  %-8d  %-9d \n",
                   SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id) >> 10, atomic_read(&priv_mm->blk_avail),
                   atomic_read(&priv_mm->blk_alloc), priv_mm->blk_reserve,
                   atomic_read(&priv_mm->blk_inpool), atomic_read(&priv_mm->allochit),
                   atomic_read(&priv_mm->allocmiss));

        }
    }

    if (mmap1) {
        ddr_id = DDR_ID1;
        printk("\nMemory Pool Distribution in DDR: 1 \n");
        printk("MemSize(K)  AvailBlk  AllocBlk  ReserveBlk  Blk_in_Pool  CacheHit  CacheMiss \n");
        printk("---------------------------------------------------------------------------- \n");

        list_for_each_entry(priv_mm, &PRIV_MM_LIST(ddr_id), list) {
            printk("%-10d  %-8d  %-8d  %-10d  %-11d  %-8d  %-9d \n",
                   SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id) >> 10, atomic_read(&priv_mm->blk_avail),
                   atomic_read(&priv_mm->blk_alloc), priv_mm->blk_reserve,
                   atomic_read(&priv_mm->blk_inpool), atomic_read(&priv_mm->allochit),
                   atomic_read(&priv_mm->allocmiss));

        }
    }

    if (mmap2) {
        ddr_id = DDR_ID2;
        printk("\nMemory Pool Distribution in DDR: 2 \n");
        printk("MemSize(K)  AvailBlk  AllocBlk  ReserveBlk  Blk_in_Pool  CacheHit  CacheMiss \n");
        printk("---------------------------------------------------------------------------- \n");
        list_for_each_entry(priv_mm, &PRIV_MM_LIST(ddr_id), list) {
            printk("%-10d  %-8d  %-8d  %-10d  %-11d  %-8d  %-9d \n",
                   SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id) >> 10, atomic_read(&priv_mm->blk_avail),
                   atomic_read(&priv_mm->blk_alloc), priv_mm->blk_reserve,
                   atomic_read(&priv_mm->blk_inpool), atomic_read(&priv_mm->allochit),
                   atomic_read(&priv_mm->allocmiss));

        }
    }
}

static int proc_read_ddr_info(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0, i, minus_one;

    for (i = 0; i < MAX_DDR_NR; i++) {
        if (!ddr_desc[i].valid)
            continue;
        
        minus_one = (ddr_desc[i].mem_base == ddr_desc[i].mem_end) ? 0 : 1;
        
        len +=
            sprintf(page + len, "Memory Statistic: DDR:%d, each block size = %dK\n", i,
                    ddr_desc[i].granular >> 10);
        len +=
            sprintf(page + len,
                    "MemBase       MemEnd        Next Avail    AllocBlks   FreeBlks   MemSize\n");
        len +=
            sprintf(page + len,
                    "----------------------------------------------------------------------------\n");
        len +=
            sprintf(page + len, "0x%-10x  0x%-10x  0x%-10x  %-10d  %-10d 0x%x\n\n",
                    ddr_desc[i].mem_base, ddr_desc[i].mem_end - minus_one, ddr_desc[i].avail_base,
                    ddr_desc[i].allocated_cnt, ddr_desc[i].avail_cnt, ddr_desc[i].mem_size);
    }

    return len;
}

/* Read memory allocation distribution for System DDR
 */
static int proc_read_mmap0(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0, ddr_id = DDR_ID_SYSTEM;
    priv_mm_t *priv_mm;

    len += sprintf(page + len, "\nMemory Pool Distribution in DDR: System \n");
    len +=
        sprintf(page + len,
                "MemSize(K)  AvailBlk  AllocBlk  ReserveBlk  Blk_in_Pool  CacheHit  CacheMiss \n");
    len +=
        sprintf(page + len,
                "---------------------------------------------------------------------------- \n");
    list_for_each_entry(priv_mm, &PRIV_MM_LIST(ddr_id), list) {
        len +=
            sprintf(page + len, "%-10d  %-8d  %-8d  %-10d  %-11d  %-8d  %-9d \n",
                    SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id) >> 10, atomic_read(&priv_mm->blk_avail),
                    atomic_read(&priv_mm->blk_alloc), priv_mm->blk_reserve,
                    atomic_read(&priv_mm->blk_inpool), atomic_read(&priv_mm->allochit),
                    atomic_read(&priv_mm->allocmiss));

    }

    return len;
}

/* Read memory allocation distribution for DDR-1
 */
static int proc_read_mmap1(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0, ddr_id = DDR_ID1;
    priv_mm_t *priv_mm;

    len += sprintf(page + len, "\nMemory Pool Distribution in DDR: 1 \n");
    len +=
        sprintf(page + len,
                "MemSize(K)  AvailBlk  AllocBlk  ReserveBlk  Blk_in_Pool  CacheHit  CacheMiss \n");
    len +=
        sprintf(page + len,
                "---------------------------------------------------------------------------- \n");
    list_for_each_entry(priv_mm, &PRIV_MM_LIST(ddr_id), list) {
        len +=
            sprintf(page + len, "%-10d  %-8d  %-8d  %-10d  %-11d  %-8d  %-9d \n",
                    SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id) >> 10, atomic_read(&priv_mm->blk_avail),
                    atomic_read(&priv_mm->blk_alloc), priv_mm->blk_reserve,
                    atomic_read(&priv_mm->blk_inpool), atomic_read(&priv_mm->allochit),
                    atomic_read(&priv_mm->allocmiss));

    }

    return len;
}

/* Read memory allocation distribution for DDR-2
 */
static int proc_read_mmap2(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0, ddr_id = DDR_ID2;
    priv_mm_t *priv_mm;

    len += sprintf(page + len, "\nMemory Pool Distribution in DDR: 2 \n");
    len +=
        sprintf(page + len,
                "MemSize(K)  AvailBlk  AllocBlk  ReserveBlk  Blk_in_Pool  CacheHit  CacheMiss \n");
    len +=
        sprintf(page + len,
                "---------------------------------------------------------------------------- \n");
    list_for_each_entry(priv_mm, &PRIV_MM_LIST(ddr_id), list) {
        len +=
            sprintf(page + len, "%-10d  %-8d  %-8d  %-10d  %-11d  %-8d  %-9d \n",
                    SIZEID_TO_SIZE(priv_mm->sizeid, ddr_id) >> 10, atomic_read(&priv_mm->blk_avail),
                    atomic_read(&priv_mm->blk_alloc), priv_mm->blk_reserve,
                    atomic_read(&priv_mm->blk_inpool), atomic_read(&priv_mm->allochit),
                    atomic_read(&priv_mm->allocmiss));

    }

    return len;
}

/* Read module locates in which DDR
 */
static int proc_read_config_file(char *page, char **start, off_t off, int count, int *eof,
                                 void *data)
{
    int len = 0, i;
    char *name[DDR_ID_MAX + 1] = { "SYSTEM", "DDR-1", "DDR-2", "" };

    len += sprintf(page + len, "frmidx    Name                            DDR Location\n");
    len += sprintf(page + len, "------------------------------------------------------\n");
    for (i = 1; i < ARRAY_SIZE(id_string_arry); i++) {
#if 1                           /* for special case: H264E */
        if (id_string_arry[i].rmmp_index == FRMIDX_H264E_DMAMASK0)
            len +=
                sprintf(page + len, "%-8d  %-30s  %s\n", id_string_arry[i].rmmp_index,
                        id_string_arry[i].string, h264e_dmamask0);
        else if (id_string_arry[i].rmmp_index == FRMIDX_H264E_DMAMASK1)
            len +=
                sprintf(page + len, "%-8d  %-30s  %s\n", id_string_arry[i].rmmp_index,
                        id_string_arry[i].string, h264e_dmamask1);
        else
            len +=
                sprintf(page + len, "%-8d  %-30s  %s\n", id_string_arry[i].rmmp_index,
                        id_string_arry[i].string, name[id_string_arry[i].ddr_id]);

#else
        len +=
            sprintf(page + len, "%-8d  %-30s  %s\n", id_string_arry[i].rmmp_index,
                    id_string_arry[i].string, name[id_string_arry[i].ddr_id]);
#endif
    }

    return len;
}

/* read memory ownder 
 */
static int proc_read_mem_ownerinfo(char *page, char **start, off_t off, int count, int *eof,
                                   void *data)
{
    int ddr_id;
    memblk_hdr_t *mb_hdr;
    priv_mm_t *priv_mm;
    struct list_head *node;

    printk("\n");
    for (ddr_id = DDR_ID_SYSTEM; ddr_id < MAX_DDR_NR; ddr_id++) {
        if (ddr_desc[ddr_id].valid == 0)
            continue;

        /* go through the memory pool */
        list_for_each_entry(priv_mm, &PRIV_MM_LIST(ddr_id), list) {
            if (list_empty(&priv_mm->alloc_list))
                continue;

            /* go through each node */
            list_for_each(node, &priv_mm->alloc_list) {
                mb_hdr = list_entry(node, memblk_hdr_t, list);
                printk("%-18s ---> %-8d bytes in DDR%d, va=0x%x, pa=0x%x \n",
                       id_to_namestring(mb_hdr->rmmp_index), mb_hdr->size, mb_hdr->ddr_id,
                       mb_hdr->vaddr, mb_hdr->paddr);
            }
        }
    }

    return 0;
}

/* frammap internal debug info.
 */
static int proc_read_debuginfo(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int node_id;
    page_node_t *page_node;
    struct list_head *node;

    for (node_id = 0; node_id < p_page_info->nr_node; node_id++) {
        list_for_each(node, &p_page_info->list[node_id]) {
            page_node = list_entry(node, page_node_t, list);
            printk("ddr = %d, page = 0x%x, addr = 0x%x, size = %d \n", node_id,
                   (u32) page_node->page, (u32) page_to_phys(page_node->page),
                   (u32) page_node->size);
        }
        printk(" \n");
    }

    return 0;
}

/* frammap free available and unused pages
 */
static int proc_write_freepages(struct file *file, const char *buffer, unsigned long count,
                                void *data)
{
    int len = count;
    unsigned char value[20];
    uint tmp;

    if (copy_from_user(value, buffer, len))
        return 0;
    value[len] = '\0';

    sscanf(value, "%u\n", &tmp);
    if (tmp >= 0 && tmp < MAX_DDR_NR) {
        if (ddr_desc[tmp].valid) {
            printk("\n");
            frmmap_give_freepgs(tmp, 0);
        } else
            printk("Out of range! \n");
    }

    return count;
}

/* Show kernel memory distribution 
 */
static int proc_read_kernelmem(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int ddr_id;
    memblk_hdr_t *mb_hdr;

    printk("Show who allocate the memory from the kernel. \n");
    for (ddr_id = 0; ddr_id < MAX_DDR_NR; ddr_id++) {
        if (!ddr_desc[ddr_id].valid)
            break;

        list_for_each_entry(mb_hdr, &MEMBLK_HDR_LIST(ddr_id), list) {
            if (mb_hdr->owner != MEM_OWNER_KERNEL)
                continue;

            printk("DDR%d, %s, vaddr = 0x%x, paddr = 0x%x, size = %d \n", ddr_id,
                   id_to_namestring(mb_hdr->rmmp_index), mb_hdr->vaddr, mb_hdr->paddr,
                   mb_hdr->size);
        }
    }
    return 0;
}

/* proc init.
 */
static int frm_proc_init(void)
{
    int ddr_idx, ret = 0;
    struct proc_dir_entry *p;

    p = create_proc_entry("frammap", S_IFDIR | S_IRUGO | S_IXUGO, NULL);
    if (p == NULL) {
        ret = -ENOMEM;
        goto end;
    }
    p->owner = THIS_MODULE;
    frm_proc_root = p;

    /*
     * ddr_info
     */
    ddr_info = create_proc_entry("ddr_info", S_IRUGO, frm_proc_root);
    if (ddr_info == NULL) {
        err("Fail to create proc ddr_info!\n");
        remove_proc_entry(frm_proc_root->name, frm_proc_root);
        ret = -EINVAL;
        goto end;
    }
    ddr_info->read_proc = (read_proc_t *) proc_read_ddr_info;
    ddr_info->write_proc = NULL;
    ddr_info->owner = THIS_MODULE;

    /*
     * mmap
     */

    /* system mmap */
    ddr_idx = DDR_ID_SYSTEM;
    if ((ddr_idx < MAX_DDR_NR) && ddr_desc[ddr_idx].valid) {
        mmap0 = create_proc_entry("mmap0", S_IRUGO, frm_proc_root);
        if (mmap0 == NULL) {
            err("Fail to create proc mmap0!\n");
            remove_proc_entry(ddr_info->name, frm_proc_root);
            remove_proc_entry(frm_proc_root->name, frm_proc_root);
            ret = -EINVAL;
            goto end;
        }
        mmap0->read_proc = (read_proc_t *) proc_read_mmap0;
        mmap0->write_proc = NULL;
        mmap0->owner = THIS_MODULE;
    }

    /* ddr-1 mmap */
    ddr_idx = DDR_ID1;
    if ((ddr_idx < MAX_DDR_NR) && ddr_desc[ddr_idx].valid) {
        mmap1 = create_proc_entry("mmap1", S_IRUGO, frm_proc_root);
        if (mmap1 == NULL) {
            err("Fail to create proc mmap1!\n");
            remove_proc_entry(mmap0->name, frm_proc_root);
            remove_proc_entry(ddr_info->name, frm_proc_root);
            remove_proc_entry(frm_proc_root->name, frm_proc_root);
            ret = -EINVAL;
            goto end;
        }
        mmap1->read_proc = (read_proc_t *) proc_read_mmap1;
        mmap1->write_proc = NULL;
        mmap1->owner = THIS_MODULE;
    }

    /* ddr-2 mmap */
    ddr_idx = DDR_ID2;
    if ((ddr_idx < MAX_DDR_NR) && ddr_desc[ddr_idx].valid) {
        mmap2 = create_proc_entry("mmap2", S_IRUGO, frm_proc_root);
        if (mmap2 == NULL) {
            err("Fail to create proc mma2p!\n");
            remove_proc_entry(mmap1->name, frm_proc_root);
            remove_proc_entry(mmap0->name, frm_proc_root);
            remove_proc_entry(ddr_info->name, frm_proc_root);
            remove_proc_entry(frm_proc_root->name, frm_proc_root);
            ret = -EINVAL;
            goto end;
        }
        mmap2->read_proc = (read_proc_t *) proc_read_mmap2;
        mmap2->write_proc = NULL;
        mmap2->owner = THIS_MODULE;
    }

    /*
     * name table
     */
    config_file = create_proc_entry("config_file", S_IRUGO, frm_proc_root);
    if (config_file == NULL) {
        err("Fail to create proc config_file!\n");
        remove_proc_entry(mmap2->name, frm_proc_root);
        remove_proc_entry(mmap1->name, frm_proc_root);
        remove_proc_entry(mmap0->name, frm_proc_root);
        remove_proc_entry(ddr_info->name, frm_proc_root);
        remove_proc_entry(frm_proc_root->name, frm_proc_root);
        ret = -EINVAL;
        goto end;
    }
    config_file->read_proc = (read_proc_t *) proc_read_config_file;
    config_file->write_proc = NULL;
    config_file->owner = THIS_MODULE;

    /*
     * memory owner distribution status
     */
    mem_ownerinfo = create_proc_entry("mem_owner", S_IRUGO, frm_proc_root);
    if (mem_ownerinfo == NULL) {
        err("Fail to create proc mem_owner!\n");
        remove_proc_entry(config_file->name, frm_proc_root);
        remove_proc_entry(mmap2->name, frm_proc_root);
        remove_proc_entry(mmap1->name, frm_proc_root);
        remove_proc_entry(mmap0->name, frm_proc_root);
        remove_proc_entry(ddr_info->name, frm_proc_root);
        remove_proc_entry(frm_proc_root->name, frm_proc_root);
        ret = -EINVAL;
        goto end;
    }
    mem_ownerinfo->read_proc = (read_proc_t *) proc_read_mem_ownerinfo;
    mem_ownerinfo->write_proc = NULL;
    mem_ownerinfo->owner = THIS_MODULE;

    /*
     * debug frammap internal
     */
    frammap_debug = create_proc_entry("debug", S_IRUGO, frm_proc_root);
    if (frammap_debug == NULL) {
        err("Fail to create proc frammap_debug!\n");
        remove_proc_entry(mem_ownerinfo->name, frm_proc_root);
        remove_proc_entry(config_file->name, frm_proc_root);
        remove_proc_entry(mmap2->name, frm_proc_root);
        remove_proc_entry(mmap1->name, frm_proc_root);
        remove_proc_entry(mmap0->name, frm_proc_root);
        remove_proc_entry(ddr_info->name, frm_proc_root);
        remove_proc_entry(frm_proc_root->name, frm_proc_root);
        ret = -EINVAL;
        goto end;
    }
    frammap_debug->read_proc = (read_proc_t *) proc_read_debuginfo;
    frammap_debug->write_proc = NULL;
    frammap_debug->owner = THIS_MODULE;

    /*
     * Free unuse and available pages
     */
    frammap_free_pages = create_proc_entry("free_pages", S_IRUGO, frm_proc_root);
    if (frammap_free_pages == NULL) {
        err("Fail to create proc free_pages!\n");
        remove_proc_entry(frammap_debug->name, frm_proc_root);
        remove_proc_entry(mem_ownerinfo->name, frm_proc_root);
        remove_proc_entry(config_file->name, frm_proc_root);
        remove_proc_entry(mmap2->name, frm_proc_root);
        remove_proc_entry(mmap1->name, frm_proc_root);
        remove_proc_entry(mmap0->name, frm_proc_root);
        remove_proc_entry(ddr_info->name, frm_proc_root);
        remove_proc_entry(frm_proc_root->name, frm_proc_root);
        ret = -EINVAL;
        goto end;
    }
    frammap_free_pages->read_proc = NULL;
    frammap_free_pages->write_proc = proc_write_freepages;
    frammap_free_pages->owner = THIS_MODULE;

    /*
     * Show kernel memory allocation
     */
    mem_kernel = create_proc_entry("mem_kernel", S_IRUGO, frm_proc_root);
    if (mem_kernel == NULL) {
        err("Fail to create proc mem_kernel!\n");
        remove_proc_entry(frammap_free_pages->name, frm_proc_root);
        remove_proc_entry(frammap_debug->name, frm_proc_root);
        remove_proc_entry(mem_ownerinfo->name, frm_proc_root);
        remove_proc_entry(config_file->name, frm_proc_root);
        remove_proc_entry(mmap2->name, frm_proc_root);
        remove_proc_entry(mmap1->name, frm_proc_root);
        remove_proc_entry(mmap0->name, frm_proc_root);
        remove_proc_entry(ddr_info->name, frm_proc_root);
        remove_proc_entry(frm_proc_root->name, frm_proc_root);
        ret = -EINVAL;
        goto end;
    }
    mem_kernel->read_proc = proc_read_kernelmem;
    mem_kernel->write_proc = NULL;
    mem_kernel->owner = THIS_MODULE;

  end:
    return ret;
}

static void frm_proc_release(void)
{
    if (ddr_info != NULL)
        remove_proc_entry(ddr_info->name, frm_proc_root);

    if (mmap0 != NULL)
        remove_proc_entry(mmap0->name, frm_proc_root);

    if (mmap1 != NULL)
        remove_proc_entry(mmap1->name, frm_proc_root);

    if (mmap2 != NULL)
        remove_proc_entry(mmap2->name, frm_proc_root);

    if (config_file != NULL)
        remove_proc_entry(config_file->name, frm_proc_root);

    if (mem_ownerinfo != NULL)
        remove_proc_entry(mem_ownerinfo->name, frm_proc_root);

    if (frammap_debug != NULL)
        remove_proc_entry(frammap_debug->name, frm_proc_root);

    if (frammap_free_pages != NULL)
        remove_proc_entry(frammap_free_pages->name, frm_proc_root);

    if (mem_kernel != NULL)
        remove_proc_entry(mem_kernel->name, frm_proc_root);

    /* last one */
    if (frm_proc_root != NULL)
        remove_proc_entry(frm_proc_root->name, NULL);
}

/*
 * Following defines the interface to have the AP use the memory from the DDRs 
 */

/* ioctl
 */
static int frmmap_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    unsigned int dev;

    dev = iminor(inode) - FRAMMAP_MINOR;

    switch (cmd) {
    case FRM_IOGBUFINFO:
        {
            frmmap_meminfo_t mem;

            /* get how many bytes the DDR pool has */
            mem.aval_bytes = ddr_desc[dev].avail_cnt * ddr_desc[dev].granular;
            if (copy_to_user((void *)arg, (void *)&mem, sizeof(frmmap_meminfo_t)))
                ret = -EFAULT;
            break;
        }
    case FRM_GIVE_FREEPAGES:
        if (frmmap_give_freepgs(dev, 0) < 0)
            ret = -EFAULT;
        break;
    default:
        ret = -1;
        break;
    }

    return ret;
}

static int frmmap_mmap(struct file *filp, struct vm_area_struct *vma)
{
    unsigned long size = vma->vm_end - vma->vm_start;
    private_data_t *cookie;
    unsigned long off = vma->vm_pgoff << PAGE_SHIFT;    /* offset from the buffer start point */
    ap_node_t *node;
    unsigned long pfn;          //which page number is the start page
    int ret, dev = -1, aval_bytes;

    if (filp->private_data == NULL)
        return -EINVAL;

    cookie = filp->private_data;

    switch (cookie->devidx) {
    case 0:
        dev = 0;
        cookie->frmidx = FRMIDX_AP0;
        break;    
    case 1:
        dev = 1;
        cookie->frmidx = FRMIDX_AP1;
        break;
    case 2:
        dev = 2;
        cookie->frmidx = FRMIDX_AP2;
        break;
    case 3:
        dev = 3;
        cookie->frmidx = FRMIDX_AP3;
        break;
    default:
        return -EINVAL;
    }
    
    /* check if the ddr is active */
    if (unlikely(!ddr_desc[dev].valid)) {
        printk("%s, DDR%d is not active! \n", __func__, dev);
        return -EINVAL;
    }
    else {
        aval_bytes = ddr_desc[dev].avail_cnt * ddr_desc[dev].granular;
        if (aval_bytes < size) {
            printk("%s, not enough memory for DDR%d! \n", __func__, dev);
            return -ENOMEM;
        }
    }
    
    node = kmem_cache_alloc(cookie->node_cache, GFP_KERNEL);
    if (node == NULL) {
        printk("frmmap_mmap: kmem_cache_alloc fail! \n");
        return -EINVAL;
    }
    memset(node, 0x0, sizeof(ap_node_t));

    /* allocate memory from frammap orginal mechanism 
     */
    node->buf_info.size = size;
    if (frm_get_buf_info(cookie->frmidx, &node->buf_info) < 0) {
        kmem_cache_free(cookie->node_cache, node);
        return -EINVAL;
    }
    
    INIT_LIST_HEAD(&node->list);
    list_add_tail(&node->list, &cookie->node_list);

    /* Get the memory success, start to do mmap 
     */
    vma->vm_page_prot = PAGE_SHARED;  //pgprot_writecombine(vma->vm_page_prot);
    vma->vm_flags |= VM_RESERVED;

    off += node->buf_info.phy_addr;     //points to the offset from node->buf_info.phy_addr
    pfn = off >> PAGE_SHIFT;    //which page number is the start page

    ret = remap_pfn_range(vma, vma->vm_start, pfn, node->buf_info.size, vma->vm_page_prot);

    //printk("Frammap: off=%x, p=%#x, v=%#x, size=%#x, pfn = %d\n", off, node->buf_info.phy_addr, vma->vm_start, size, pfn);

    return ret;
}

static int frmmap_open(struct inode *inode, struct file *filp)
{
    int dev, ret = 0;
    char *name;

    dev = iminor(inode) - FRAMMAP_MINOR;

    if ((dev < 0) || (dev >= MAX_DDR_NR))
        return -1;

    SEMA_LOCK;

    name = ap_devname[dev];
    try_module_get(THIS_MODULE);

    /* The following code processes the multiple open in AP.
     * Note: when multiple open, the filp pointer is also different even the same device node.
     */
    if (filp->private_data != NULL) {
        printk("Error! The file was opened already! \n");
        ret = -1;
        goto exit;
    }

    if (likely(private_data[dev].node_cache == NULL)) {
        memset(name, 0, 10);
        sprintf(name, "frammap_ap%d", dev);
        private_data[dev].node_cache = kmem_cache_create(name, sizeof(ap_node_t), 0, 0, NULL);
        if (!private_data[dev].node_cache) {
            printk("Frammap: AP creates kmem_cache_create fail! \n");
            ret = -1;
            goto exit;
        }

        INIT_LIST_HEAD(&private_data[dev].node_list);

        /* When AP allocates the memory, the information will be kept here 
         */
        private_data[dev].devidx = dev;
    }

    filp->private_data = &private_data[dev];

  exit:
    SEMA_UNLOCK;
    return ret;
}

/* 
 * Close function
 */
static int frmmap_release(struct inode *inode, struct file *filp)
{
    private_data_t *cookie;
    ap_node_t *node, *ne;
    
    SEMA_LOCK;
    
    if (filp->private_data) {
        int dev;

        cookie = (private_data_t *) filp->private_data;
        dev = cookie->devidx;
        
        /* for multiple open in AP */
        if (private_data[dev].node_cache) {
            list_for_each_entry_safe(node, ne, &cookie->node_list, list) {
                SEMA_UNLOCK;
                frm_release_buf_info(cookie->frmidx, &node->buf_info);
                SEMA_LOCK;
                list_del_init(&node->list);
                kmem_cache_free(cookie->node_cache, node);
            }

            /* destroy the ap node cache */
            kmem_cache_destroy(private_data[dev].node_cache);
            private_data[dev].node_cache = NULL;
        }
    }

    filp->private_data = NULL;
    
    SEMA_UNLOCK;
    
    /* need to return the memory */
    module_put(THIS_MODULE);

    return 0;
}

struct file_operations frmmap_fops = {
  owner:THIS_MODULE,
  ioctl:frmmap_ioctl,
  mmap:frmmap_mmap,
  open:frmmap_open,
  release:frmmap_release
};

struct miscdevice misdev[MAX_DDR_NR];

module_init(frammap_init);
module_exit(frammap_exit);

EXPORT_SYMBOL(show_ddr_info);

MODULE_AUTHOR("GM Technology Corp.");
MODULE_DESCRIPTION("GM RAM MMAP Library");
MODULE_LICENSE("GPL");

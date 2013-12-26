#ifndef __FMEM_H
#define __FMEM_H
#include <linux/mm.h>
#include <asm/io.h>
#include <linux/list.h>

#define MAX_DDR_CHUNKS 3   /* include system memory */

typedef struct g_page_info_s
{
    int         nr_node;        /* number of active DDRs */
    int         block_sz;       /* page allocation size once */    
    int         node_sz[MAX_DDR_CHUNKS];    /* the size for the DDR node */
    dma_addr_t  phy_start[MAX_DDR_CHUNKS];  /* the min physical start */
    struct list_head list[MAX_DDR_CHUNKS];  /* page node list */
} g_page_info_t;

/* page node for each pages allocation */
typedef struct {
    struct page     *page;
    dma_addr_t      phy_start;
    unsigned int    size;   /* the real size */
    int             ddr_id; /* belong to which DDR */
    struct list_head list;
} page_node_t;

extern void fmem_get_pageinfo(g_page_info_t **data_ptr);
extern int  fmem_give_pages(int node_id, unsigned int give_sz);
extern void fmem_free_ex(size_t size, void *cpu_addr, dma_addr_t handle);
void *fmem_alloc_ex(size_t size, dma_addr_t * dma_handle, unsigned long flags, int ddr_id);

#endif /* __FMEM_H */

/*
 *  linux/include/asm-armnommu/arch-GM/ahb_dma.h
 *  
 *  GM Platform Clock Tick Rate Definition
 *  
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 * ChangeLog
 *
 *  Peter Liao 09/29/2005  Created.
 */
 
/* ahb_dma.h */
#ifndef __GM__DMAC_FTDMAC020_H_
#define __GM__DMAC_FTDMAC020_H_

#include <asm/scatterlist.h>
#include <mach/platform/ahb_dma.h> 

#define AHBDMA_MAX_DMA_SZ       40960//(0x400000 - 1)

#define AHBDMA_WIDTH_8BIT       0x0
#define AHBDMA_WIDTH_16BIT      0x1
#define AHBDMA_WIDTH_32BIT      0x2
#define AHBDMA_CTL_INC          0x0
#define AHBDMA_CTL_DEC          0x1
#define AHBDMA_CTL_FIX          0x2

#define AHBDMA_MASTER_0         0x0
#define AHBDMA_MASTER_1         0x1

#define AHBDMA_NO_TRIGGER_IRQ   0x0
#define AHBDMA_TRIGGER_IRQ      0x1    //john
#define AHBDMA_ERR_TRIGGER_IRQ  0x2

//john add for interrupt status, error
#define INT_DMA_TRIGGER		0x01
#define INT_DMA_ERROR		0x02
#define INT_DMA_ABORT   	0x04
#define INT_DMA_ALL_DONE	0x80

typedef struct
{
	unsigned int source;
	unsigned int dest;
	unsigned int llp;
	unsigned int control;
	unsigned int tot_size;
} ahb_lld_t;

typedef struct
{
    /* add dma parameter */
    unsigned int    src;        //source address for dma
    unsigned int    dest;       //dest address for dma
    unsigned int    bs;         // source burst size (0/1/2/3/4/5/6/7=>1/4/8/16/32/64/128/256)
    unsigned int    sw;         //source width (0/1/2=>8/16/32)
    unsigned int    dw;         //dest width (0/1/2=>8/16/32)
    unsigned int    sctl;       //source control (0/1/2/3=>inc/dec/fix/x)
    unsigned int    dctl;       // dest coontrol (0/1/2/3=>inc/dec/fix/x)
    unsigned int    size;       // DMA count
    unsigned int    irq;        // (0/1)==>(disable/enable)
    unsigned int    dma_ff_th;  // DMA FIFO threshold    
    unsigned int    chpri;      // Channel Priority Level, 3:Highest Level, 2:2nd high priority, 1: 3nd high priority. 0:lowest priority(default)
} ahb_dma_parm_t;

typedef struct
{
    /* init parameter */
    unsigned int    base;
    unsigned int    channel_base;
    unsigned int    llp_master; //0,1
    unsigned int    src_data_master;//0,1
    unsigned int    dest_data_master;//0,1
    unsigned int    llp_count;      //count of link list number
    unsigned int    channel;
    unsigned int    hw_handshake;
    unsigned int    llp_last_idx;   //0(register),1(first llp),2(second llp),3...
    unsigned int    llp_free_idx;   //0(register),1(first llp),2(second llp),3...
    unsigned int    allocated_dma_lld;
    ahb_lld_t       *ahb_dma_lld;
    ahb_lld_t       *ahb_last_lld;
    ahb_lld_t       *ahb_dma_lld_phys;
    unsigned int    src_he;//disable,enable
    unsigned int    src_rs;//dma channel 0~7
    unsigned int    dst_he;//disable,enable
    unsigned int    dst_rs;//dma channel 0~7    
} ahb_dma_data_t;



extern int fa_ahb_dma_init(int ch, int req_pair);
extern void fa_ahb_dma_add(int ch);
extern void fa_ahb_dma_add_auto(int ch, int total_bytes);
extern void fa_ahb_dma_reset(int ch);
extern unsigned int fa_ahb_dma_get_status(int ch);
//extern void fa_set_ahb_dma_params(int ch, ahb_dma_parm_t *parm);
//extern void fa_set_ahb_dma_ch_params(int ch, ahb_dma_data_t *priv);
extern void fa_set_ahb_dma_transfer_params(int ch, int llp_master, int llp_count, int elem_count,
					   int hw_handshake, int irq_set);
extern void fa_set_ahb_dma_transfer_params2(int ch, int llp_master, int total_bytes, int hw_handshake);

extern void fa_set_ahb_dma_dst_params(int ch, u32 dst_addr, int dest_data_master, int dw, int dctl);
extern void fa_set_ahb_dma_src_params(int ch, u32 src_addr, int src_data_master, int sw, int sctl);
extern void fa_set_ahb_dma_cfg_params(int ch, int src_he, int src_rs, int dst_he, int dst_rs);
extern ahb_dma_parm_t *fa_get_ahb_dma_params(int ch);
extern void fa_set_ahb_dma_usual_params(int ch, u32 src_addr, u32 dest_addr, int irq);
extern void fa_ahb_dma_start(int ch);
extern void fa_ahb_dma_stop(int ch);
extern void fa_ahb_dma_clear_int(int ch);
extern int fa_request_ahb_dma_auto(int dev_id, const char *dev_name,
		     void (* callback)(int ch, u16 int_status, void *data),
		     void *data, int *dma_ch_out, int pio_mode);
extern int fa_request_ahb_dma_ch(int dev_id, const char *dev_name,
		     void (* callback)(int ch, u16 int_status, void *data),
		     void *data, int dma_ch, int pio_mode);
extern void fa_free_ahb_dma(int ch);

extern int fa_ahb_dma_get_irq(int baseaddr);
extern void register_ahbdma_get_irq_func(int (*func)(int base));
extern unsigned int fa_ahb_dma_get_src_addr(int ch);
extern unsigned int fa_ahb_dma_get_dest_addr(int ch);

extern void fa_ahb_free_llp_mem(u32 ch);
extern void fa_ahb_dma_resume(u32 ch);
extern void fa_ahb_dma_pause(u32 ch);
extern void fa_ahb_dma_cloop_llp_init(u32 ch, u32 start, u32 period, u32 size, u32 end, u32 dir, u8 llp_mode);
extern u32 fa_ahb_set_dma_sg(u32 ch, struct scatterlist *sg, int nr_sg, u32 dir, u32 b_irq);
extern void fa_ahb_dma_reset2(int ch);
extern void fa_set_ahb_dma_transfer_bs(int ch, int burst_size);
extern void fa_set_ahb_llp_address (u32 ch, u32 start_addr, u32 end_addr, u32 dir, u32 period);
extern int fa_ahb_dma_set_llp(int ch, unsigned int llp_idx);
extern int fa_ahb_dma_priority_set(int ch, unsigned int priority);

#ifdef DMA_DEBUG
#define dma_dbg(format, arg...) printk(KERN_INFO format , ## arg)
#else
#define dma_dbg(format, arg...) do { ; } while (0)
#endif

static inline unsigned int dma_bs_sz(unsigned int rxth)
{
    unsigned int i;

    i = 0;
    while(rxth > 1) {
       rxth >>= 1;	
       i++;
       if(i > 7)// bs field max value = 7
          break;
    }

    if(i > 1)
       i --;
    return i;
}

#endif /* __GM__DMAC_FTDMAC020_H_ */

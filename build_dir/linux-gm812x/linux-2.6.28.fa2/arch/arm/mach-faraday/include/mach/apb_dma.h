/*
 *  arch/arm/mach-faraday/include/mach/apb_dma.h
 *  
 *  Faraday Platform Clock Tick Rate Definition
 *  
 *  Copyright (C) 2005 Faraday Corp. (http://www.faraday-tech.com)
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
 *  Peter Liao 10/03/2005  Created.
 */
 
#ifndef __FARADAY__APBBRG_FTAPBBRG020S_H_
#define __FARADAY__APBBRG_FTAPBBRG020S_H_

#define APBDMA_WIDTH_8BIT               0x2
#define APBDMA_WIDTH_16BIT              0x1
#define APBDMA_WIDTH_32BIT              0x0

#define APBDMA_TYPE_APB                 0
#define APBDMA_TYPE_AHB                 1

#define APBDMA_NO_TRIGGER_IRQ           0x0
#define APBDMA_TRIGGER_IRQ              0x1
#define APBDMA_ERR_TRIGGER_IRQ		0x2
#define APBDMA_TRIGGER_IRQ_WITH_ERROR   0x3

#define APBDMA_CTL_FIX                  0
#define APBDMA_CTL_INC1                 1
#define APBDMA_CTL_INC2                 2
#define APBDMA_CTL_INC4                 4
#define APBDMA_CTL_INC8                 8
#define APBDMA_CTL_INC16                16
#define APBDMA_CTL_DEC1                 (-1)
#define APBDMA_CTL_DEC2                 (-2)
#define APBDMA_CTL_DEC4                 (-4)

//John add
#define APDDMA_CHANNEL_STEP        0x10
#define APBDMA_INT_FLAG            0x02
#define APBDMA_INT_ERR_FLAG        0x10

//John add for interrupt status, error
#define INT_DMA_TRIGGER 0x01
#define INT_DMA_ERROR   0x02

#define FAIL_INC    100


/*************************************    APB DMA channel assignment    *******************************/
/* This is platform depedent */
#define CFC_APBDMA_CHAL       1
#define SSP1_APBDMA_CHAL      2
#define UART1_TX_APBDMA_CHAL  3
#define UART1_RX_APBDMA_CHAL  3
#define UART2_TX_APBDMA_CHAL  4
#define UART2_RX_APBDMA_CHAL  4
#define SDC_APBDMA_CHAL       5
#define AC97_APBDMA_CHAL      6
#define UART3_TX_APBDMA_CHAL  11
#define UART3_RX_APBDMA_CHAL  11
#define USB220_APBDMA_CHAL    14
#define IRDA_APBDMA_CHAL      11
#define EXTREQ0_APBDMA_CHAL   12
#define EXTREQ1_APBDMA_CHAL   15

typedef void (*dma_handler_func_t)(int, uint, uint);

typedef struct
{
	/* add dma parameter */
	unsigned int    src;        // source address for dma
	unsigned int    dest;       // dest address for dma
	unsigned int    width;      // data width (0/1/2=>8/16/32)
	unsigned int    req_num;    // hardware request/grant number
	int             sctl;       // source incremental
	int             dctl;       // dest incremental
	unsigned int    stype;      // source type (0/1 => APB/AHB)
	unsigned int    dtype;      // dest type (0/1 => APB/AHB)
	unsigned int    burst;      // (0/1)==>(disable/enable)
	unsigned int    size;       // dma count
	unsigned int    irq;        // 0x01: Enable finishing interrupt, 0x02: Enable error response interrupt. 0x03: Enable both.
} apb_dma_parm_t;

extern int fa_apb_dma_get_irq(int base);
extern int fa_apb_dma_init(int ch);
extern void fa_apb_dma_reset(int ch);
extern void fa_apb_dma_clear_int(int ch)	;
extern unsigned int fa_apb_dma_get_status(int ch);
extern void fa_apb_dma_start(int ch);
extern void fa_apb_dma_stop(int ch);
extern void fa_set_apb_dma_dst_params(int ch, u32 dst_addr, int dst_addr_sel, int dctl);
extern void fa_set_apb_dma_src_params(int ch, u32 src_addr, int src_addr_sel, int sctl);
extern void fa_set_apb_dma_transfer_params(int ch, int req_sel, int width, int burst,
						int elem_count, int irq_set);
extern void fa_set_apb_dma_params(int ch, apb_dma_parm_t *parm);
extern int fa_request_apb_dma_auto(int dev_id, const char *dev_name,
		     void (* callback)(int ch, u16 int_status, void *data),
		     void *data, int *dma_ch_out, int pio_mode);
extern int fa_request_apb_dma_ch(int dev_id, const char *dev_name,
		     void (* callback)(int ch, u16 int_status, void *data),
		     void *data, int dma_ch, int pio_mode);
extern void fa_free_apb_dma(int ch);
extern void register_apbdma_get_irq_func(int (*func)(int base));


#ifdef DMA_DEBUG
#define dma_dbg(format, arg...) printk(KERN_INFO format , ## arg)
#else
#define dma_dbg(format, arg...) do { ; } while (0)
#endif

/* Include platform *dependent* APD DMA driver configuration */ 
#include <mach/platform/apb_dma.h>  

#endif /* __FARADAY__APBBRG_FTAPBBRG020S_H_ */

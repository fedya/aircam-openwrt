/*
 *  linux/arch/armnommu/mach-GM/ahb_dma.c
 *
 *  GM FTDMAC020 AHB DMA Controller Device Driver Implementation
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
 * 
 * ChangeLog
 * 
 *  Peter Liao 10/04/2005  Created.
 */

/*
  AHB dma function usage:
        // First, you must request a new DMA channel and call init
	// Request a DMA channel resource via fa_request_ahb_dma_ch or fa_request_ahb_dma_auto
	    if (fa_request_ahb_dma_ch(AHBDMA_TEST_DEVID, "AHBDMA M2M", ahb_dma_test_handler, NULL, ch, 0) != 0) {
		    printk(KERN_ERR "Request AHB DMA Channel resource fail");
		    return;
	    } else
		    printk(KERN_ERR "Request AHB DMA resource success\n");
            
            fa_ahb_dma_init(ch, req_pair);
            
        // Second, add dma entry
        //  Prepare parameter for add dma entry
	    fa_set_ahb_dma_transfer_params( ch, AHBDMA_MASTER_0, LLP_TEST_COUNT, AHBDMA_TEST_SIZE, 0, AHBDMA_NO_TRIGGER_IRQ);
	    parm = fa_get_ahb_dma_params(ch);
	    fa_set_ahb_dma_src_params(ch, src_addr_dma, AHBDMA_MASTER_0, AHBDMA_WIDTH_32BIT, AHBDMA_CTL_INC);
	    fa_set_ahb_dma_dst_params(ch, dest_addr_dma, AHBDMA_MASTER_0, AHBDMA_WIDTH_32BIT, AHBDMA_CTL_INC)
	    fa_set_ahb_dma_cfg_params(ch, src_he, src_rs, dst_he, dst_rs);
            ahb_dma_add(ch);                //call add function
        // Or
	    fa_set_ahb_dma_src_params(ch, src_addr_dma, AHBDMA_MASTER_0, SrcWidth, AHBDMA_CTL_INC);
	    fa_set_ahb_dma_dst_params(ch, dest_addr_dma, AHBDMA_MASTER_0, DstWidth, AHBDMA_CTL_INC);
	    fa_set_ahb_dma_cfg_params(ch, src_he, src_rs, dst_he, dst_rs);
	    fa_set_ahb_dma_transfer_params2( ch, AHBDMA_MASTER_0, DataBytes, 0);
	    ahb_dma_add_auto(ch, Toltal_bytes);


            ....you may use ahb_dma_add (LLP_COUNT+1) times

        // Third, start dma
            ahb_dma_start(ch);

        // Waiting for DMA complete or error

        // Stop and clear interrupt
            fa_ahb_dma_stop(ch);
            fa_ahb_dma_clear_int(ch);
                                
        // Last, Free structure
            fa_free_ahb_dma(ch);
            
        // useful function for you to maintain
            fa_ahb_dma_reset(priv); //reset dma action and stop
            addr=fa_ahb_dma_get_dest_addr(priv); get DMA destinition address
            status=fa_ahb_dma_get_status(priv); get dma status,Bit0:finished IRQ   Bit1:Error IRQ

Note:
****Example to Playing I2S 3 LLP Count dma and its dma size is less 0x800
    (first time)
    +-----------+        +----------+      +----------+       +----------+ 
    |DMA reg    | ---->  | Link 1   | ---> | Link 2   | --->  | Link 3   | ---> NULL
    |NULL data  |        | NULL data|      | NULL data|       | Real data| 
    |No Trig IRQ|        | Trig IRQ |      | Trig IRQ |       | Trig IRQ | 
    +-----------+        +----------+      +----------+       +----------+ 

    (second time,Link 1 Finished and irq triggered)
         +----------+       +----------+       +----------+ 
         | Link 2   | --->  | Link 3   | --->  | Link 1   |  ---> NULL
         | NULL data|       | Real data|       | Real data| 
         | Trig IRQ |       | Trig IRQ |       | Trig IRQ | 
         +----------+       +----------+       +----------+ 
         
    (third time,Link 2 Finished and irq triggered)
         +----------+       +----------+       +----------+ 
         | Link 3   | --->  | Link 1   | --->  | Link 2   |  ---> NULL
         | Real data|       | Real data|       | Real data| 
         | Trig IRQ |       | Trig IRQ |       | Trig IRQ | 
         +----------+       +----------+       +----------+ 

****Example to Playing I2S 3 LLP Count dma and its dma size is 0x1000,you must use

    (first time)
    +-----------+        +----------+      +----------+       +----------+      +----------+     
    |DMA reg    | ---->  | Link 1   | ---> | Link 2   | --->  | Link 3-1 | ---> | Link 3-2 | --->NULL
    |NULL data  |        | NULL data|      | NULL data|       |0x800 size|      |0x800 size|
    |No Trig IRQ|        | Trig IRQ |      | Trig IRQ |       | Real data|      | Real data|    
    +-----------+        +----------+      +----------+       | Trig IRQ |      | Trig IRQ |     
                                                              +----------+      +----------+

****Example to Recording I2S 3 LLP Count dma and its dma size is 0x1000,you must use

    (first time)   
    +-----------+      +----------+      +----------+     +----------+      +----------+     +----------+      +----------+          
    |DMA reg    | ---> | Link 1-1 | ---> | Link 1-2 | --->| Link 2-1 | ---> | Link 2-2 | --->| Link 3-1 | ---> | Link 2-2 | ---> NULL 
    |NULL data  |      |0x800 size|      |0x800 size|     |0x800 size|      |0x800 size|     |0x800 size|      |0x800 size|          
    |No Trig IRQ|      | Real data|      | Real data|     | Real data|      | Real data|     | Real data|      | Real data|          
    +-----------+      | NoTrigIRQ|      | Trig IRQ |     | NoTrigIRQ|      | Trig IRQ |     | NoTrigIRQ|      | Trig IRQ |          
                       +----------+      +----------+     +----------+      +----------+     +----------+      +----------+


*/

#define DMA_DEBUG 1
//Mark following statement if you want to enable DMA debug messages
#undef	DMA_DEBUG

#include <linux/version.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <asm/types.h>
#include <asm/sizes.h>
#include <asm/io.h>
#include <linux/errno.h> 
#include <linux/interrupt.h> 
#include <asm/mach/irq.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27))
#include <mach/ahb_dma.h>
#else
#include <asm/arch/ahb_dma.h>
#endif
#include <linux/dma-mapping.h>
/* register definition */

#define FTAHBC020S_CONTROL_REGISTER_0X88 0x88

#define AHBDMA_INT_0x00      	0x00
#define AHBDMA_INT_TC_0x04      0x04
#define AHBDMA_ISR_0x8          0x8
#define AHBDMA_ERR_CLR_0x10	0x10
#define AHBDMA_INT_ERR_0x0C     0x0C
#define AHBDMA_ENABLE_0x24      0x24

#define AHBDMA_CHANNEL_CSR_0x0          0x0
#define AHBDMA_CHANNEL_CFG_0x4          0x4
#define AHBDMA_CHANNEL_SRC_0x8          0x8
#define AHBDMA_CHANNEL_DST_0xC          0xc
#define AHBDMA_CHANNEL_LLP_0x10         0x10
#define AHBDMA_CHANNEL_TXSZ_0x14        0x14

/* End of Regisgter Definition */

#define IPMODULE DMAC
#define IPNAME   FTDMAC020

/* 0-3 is AHB DMA  */
 
#define FA_AHB_DMA_CHANNEL_MAX	IP_IRQ_COUNT*IP_COUNT
#define AHB_DMA_CHANNELS	IP_IRQ_COUNT

struct fa_ahb_dma_ch {
	int dev_id;
	const char *dev_name;
	void (* callback)(int ch, u16 ch_status, void *data);
	void *data;
	int active;
	ahb_dma_data_t priv;
	ahb_dma_parm_t parm;
	int pio_mode;
	int req_pair;
};

static struct fa_ahb_dma_ch	ahb_dma_chan[FA_AHB_DMA_CHANNEL_MAX];

static spinlock_t ahb_dma_chan_lock; 
static inline int fa_setup_ahb_pmu(int ch, int req_pair)
{
	return 0;
}

static inline void fa_reset_ahb_pmu(int ch, int req_pair)
{
}

static int fa_llp_size(int bs, int sw)
{
        int burst, src_width, i;

        if(bs==0)
                burst = 1;
        else
                burst = 1 << (bs+1);

        src_width = 1 << sw;

        for(i = 0; i < AHBDMA_MAX_DMA_SZ; i++)
                if((burst * src_width * i) >= AHBDMA_MAX_DMA_SZ)
                	if(i > 1)
                        return  (burst * src_width * (i-1));
                                 
        return AHBDMA_MAX_DMA_SZ;	//ok on burst=1, 4, 8, 16 && src_width=4 bytes
}

static void fa_ahb_dma_hw_resource_init(void)
{
	int ch,j;
	
	for (j=0;j<IP_COUNT;j++) {
		for (ch=j*AHB_DMA_CHANNELS;ch<(AHB_DMA_CHANNELS+j*AHB_DMA_CHANNELS);ch++) {            
			ahb_dma_chan[ch].priv.base = IP_va_base[j];
			ahb_dma_chan[ch].priv.channel_base = IP_va_base[j] + 0x100 + (ch-j*AHB_DMA_CHANNELS)*0x20;
			dma_dbg("ahb_dma_chan[%d].priv.base=%08X\n",ch,ahb_dma_chan[ch].priv.base);
			dma_dbg("AHBDMA ch[%d] irq is 0x%08X\n",ch, PLATFORM_AHBDMA_IRQ);
		}
	}
}

#ifdef DMA_DEBUG
static void fa_ahb_dma_show_config(int ch)
{
	int i;
	dma_dbg("===================================================================\n");
	dma_dbg("AHB DMA Channel %d Setting\n",ch);
	dma_dbg("AHB DMA Channe base address is 0x%08X\n",ahb_dma_chan[ch].priv.channel_base);
	dma_dbg("llp_master=%d\n",ahb_dma_chan[ch].priv.llp_master);
	dma_dbg("src_data_master=%d\n",ahb_dma_chan[ch].priv.src_data_master);
	dma_dbg("dest_data_master=%d\n",ahb_dma_chan[ch].priv.dest_data_master);
	dma_dbg("llp_count=%d\n",ahb_dma_chan[ch].priv.llp_count);
	dma_dbg("hw_handshake=%d\n",ahb_dma_chan[ch].priv.hw_handshake);
	dma_dbg("DMA count(size)=%d\n",ahb_dma_chan[ch].parm.size);
	dma_dbg("irq=0x%02X\n",ahb_dma_chan[ch].parm.irq);
	dma_dbg("src_addr=0x%08X\n",ahb_dma_chan[ch].parm.src);
	dma_dbg("sctl=%d\n",ahb_dma_chan[ch].parm.sctl);
	dma_dbg("src width=%d\n",ahb_dma_chan[ch].parm.sw);
	dma_dbg("dst_addr=0x%08X\n",ahb_dma_chan[ch].parm.dest);
	dma_dbg("dctl=%d\n",ahb_dma_chan[ch].parm.dctl);
	dma_dbg("dst width=%d\n",ahb_dma_chan[ch].parm.dw);
	dma_dbg("AHB DMA Register Dump:\n");
	dma_dbg("DMA CSR   is set to 0x%08X\n",inl((void *)(ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_CSR_0x0)));
	dma_dbg("DMA CFG   is set to 0x%08X\n",inl((void *)(ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_CFG_0x4)));
	dma_dbg("DMA SRC   is set to 0x%08X\n",inl((void *)(ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_SRC_0x8)));
	dma_dbg("DMA DST   is set to 0x%08X\n",inl((void *)(ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_DST_0xC)));
	dma_dbg("DMA LLP   is set to 0x%08X\n",inl((void *)(ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_LLP_0x10)));
	dma_dbg("DMA TOT_SIZE is set to 0x%08X\n",inl((void *)(ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_TXSZ_0x14)));
	dma_dbg("AHB DMA Driver Internal Data dump:\n");
	dma_dbg("last llp's base addr %p\n",ahb_dma_chan[ch].priv.ahb_last_lld);
	dma_dbg("llp_free_idx is %d\n",ahb_dma_chan[ch].priv.llp_free_idx);
	dma_dbg("AHB DMA LLP content dump:\n");
	for(i=0;i<ahb_dma_chan[ch].priv.llp_count;i++)
	{
		dma_dbg("LLP[%d]'s base addr is 0x%08x\n", i, (int)&(ahb_dma_chan[ch].priv.ahb_dma_lld[i]));
		dma_dbg("LLP[%d]'s SRC addr is 0x%08x\n", i, ahb_dma_chan[ch].priv.ahb_dma_lld[i].source);
		dma_dbg("LLP[%d]'s DST addr is 0x%08x\n", i, ahb_dma_chan[ch].priv.ahb_dma_lld[i].dest);
		dma_dbg("LLP[%d]'s LLP is 0x%08x\n", i, ahb_dma_chan[ch].priv.ahb_dma_lld[i].llp);
		dma_dbg("LLP[%d]'s Contro is 0x%08x\n", i, ahb_dma_chan[ch].priv.ahb_dma_lld[i].control);
		dma_dbg("LLP[%d]'s TOT_SIZE is 0x%08x\n", i, ahb_dma_chan[ch].priv.ahb_dma_lld[i].tot_size);
	}
	dma_dbg("===================================================================\n");
}
#endif

int fa_ahb_dma_all_done(int ch)
{
	int llp,totsize;
	llp = inl((void *)(ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_LLP_0x10));
	totsize  = inl((void *)(ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_TXSZ_0x14));
	if ( (llp == 0 ) && (totsize == 0) ) {
		dma_dbg("All linked LLP are done\n");
		//printk("All linked LLP are done, llp=%08X, base = %08X\n", llp, ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_LLP_0x10);
		return 1;
	}
	return 0;
}

int fa_ahb_dma_get_irq(int baseaddr)
{
    unsigned int     status;
    status=inl(baseaddr+AHBDMA_INT_0x00);
    dma_dbg("ISR: AHBDMA Status is %08X\n",status);
    if(status&0x1)
        return 0;
    if(status&0x2)
        return 1;
    if(status&0x4)
        return 2;
    if(status&0x8)
        return 3;
    if(status&0x10)
        return 4;
    if(status&0x20)
        return 5;
    if(status&0x40)
        return 6;
    if(status&0x80)
        return 7;
    dma_dbg("ISR: AHBDMA Status no change\n");
    return -1;
}

/**
 *	Get the current destination address of the specific DMA channel.
 *	@ch: DMA chnnel number.
 *
 *	@Returns: the destination address. So people can use this function to
 *       get how many bytes have been transferred.
 *	
 */

unsigned int fa_ahb_dma_get_dest_addr(int ch)
{
	return inl(ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_DST_0xC);
}

unsigned int fa_ahb_dma_get_src_addr(int ch)
{
	return inl(ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_SRC_0x8);
}

static inline int fa_ahb_dma_get_llpcount(int ch, int total_bytes)
{
	int LLPCount;
	int one_llp_size = fa_llp_size(ahb_dma_chan[ch].parm.bs, ahb_dma_chan[ch].parm.sw);
	int DataSize = one_llp_size/*4032*/ << max(ahb_dma_chan[ch].parm.sw, ahb_dma_chan[ch].parm.dw);
	if ( total_bytes <= DataSize )
		return 0;
	else {
		LLPCount = total_bytes/DataSize;
		if ( (total_bytes%DataSize) == 0 )
			return (LLPCount -1);
	}
	return LLPCount;

}

/* ==================================================================== */
/* Exported APB DMA API 						*/
/* ==================================================================== */

/**
 *	Initialize AHB DMA controller.
 *	@ch: DMA chnnel number.
 *	@req_pair: AHB DMA chnnel request pair. (This option depends on IP and platform)
 *	For A320D: 0-MEM to MEM 1-CFC 2-SSP1 3-UART1(Tx) 4-UART1(Rx) 5-UART1(Tx) 6-UART1(Rx)
 *	           7-SDC 8-AC97 9-IrDA(SIR Tx) 10-Reserved 11-USB2.0 Device 12-IrDA(SIR Rx)
 *		   13-IrDA(FIR) 14-EXT REQ0 15-EXT-REQ1
 *
 *	You must given ahb_dma_data_t's parameters  before calling this funciton:
 *		base:   base address of ahb dma
 *		llp_master: LLP master channel number
 *		src_data_master: source data master number
 *		dest_data_master: dest data master number
 *		llp_count:  LLP count
 *		channel:    ahb dma channel
 *		hw      is it hardware handshake?
 *
 *	@Returns zero for success. Non-zero for errors.
 *	
 */
int fa_ahb_dma_init(int ch, int req_pair)
{   
	int             i,size;
	ahb_lld_t       *lld=0;
	ahb_lld_t       *lld_phys=0;

	if(ahb_dma_chan[ch].dev_id == -1)
	{
		printk(KERN_ERR "Unavailable DMA(ch=%d, dev_id=%d)!\n",ch, ahb_dma_chan[ch].dev_id);
		return -ENODEV;
	}
	ahb_dma_chan[ch].req_pair = req_pair;
	//Platform dependent
	if ( (i=fa_setup_ahb_pmu(ch, req_pair)) < 0)
		return i;
	if(ahb_dma_chan[ch].priv.llp_count && 
		 ahb_dma_chan[ch].priv.allocated_dma_lld==0)
	{
		size=sizeof(ahb_lld_t)*(ahb_dma_chan[ch].priv.llp_count);
		dma_dbg("allocate size=0x%x sizeof(ahb_lld_t)=0x%x\n",size,sizeof(ahb_lld_t));
		ahb_dma_chan[ch].priv.ahb_dma_lld = dma_alloc_coherent(NULL , size, (dma_addr_t *)&ahb_dma_chan[ch].priv.ahb_dma_lld_phys, GFP_ATOMIC);
		dma_dbg("priv->ahb_dma_lld=0x%x phys=0x%x size=0x%x\n",(int)ahb_dma_chan[ch].priv.ahb_dma_lld,(int)ahb_dma_chan[ch].priv.ahb_dma_lld_phys,size);
		if(ahb_dma_chan[ch].priv.ahb_dma_lld==0)
		{
			printk(KERN_ERR "ahb_dma_lld allocate memory fail!\n");
			return -ENOMEM;
		}
		ahb_dma_chan[ch].priv.allocated_dma_lld = 1;
	}
	ahb_dma_chan[ch].priv.llp_last_idx=0;
	ahb_dma_chan[ch].priv.llp_free_idx=0;
	ahb_dma_chan[ch].priv.ahb_last_lld=0;

	lld=ahb_dma_chan[ch].priv.ahb_dma_lld;
	lld_phys=ahb_dma_chan[ch].priv.ahb_dma_lld_phys;
	/* initialize value */
	for(i=0;i<ahb_dma_chan[ch].priv.llp_count;i++)
	{
		dma_dbg("lld[%d]=0x%x\n",i,(int)&lld[i]);
		lld[i].source=0;//source
		lld[i].dest=0;	//dest,16bit
		lld[i].control=0;
		lld[i].tot_size=0;

		if(i==ahb_dma_chan[ch].priv.llp_count-1)
			lld[i].llp=0;
		else
			lld[i].llp=(unsigned int)(&lld_phys[i+1])|(ahb_dma_chan[ch].priv.llp_master); //use second channel 
		dma_dbg("ahb_dma_lld[%d].llp is 0x%08x\n",i,ahb_dma_chan[ch].priv.ahb_dma_lld[i].llp);
	}

	outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_LLP_0x10);
	outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CFG_0x4);	//Enabled all interrupt
	outl(0x01, ahb_dma_chan[ch].priv.base+AHBDMA_ENABLE_0x24);		//Enable DMA  controller   
	outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0);
	return 0;
}

/**
 *	Add Linked List Descriptor to the specific AHB DMA channel.
 *	@ch: DMA chnnel number.
 *
 *	LLP count =3 (example)
 *	0 => 1 => 2 => 3
 *	     ^         |
 *	     +---------+
 *	You must prepare ahb_dma_parm_t's parameters before calling ahb_dma_add:
 *		src: source address
 *		dest:dest address
 *		sw: source width (0/1/2=>8/16/32)
 *		dw: dest width (0/1/2=>8/16/32)
 *		sctl: source control (0/1/2/3=>inc/dec/fix/x)
 *		dctl: dest coontrol (0/1/2/3=>inc/dec/fix/x)
 *		size: dma count
 *		irq: (0/1)==>(disable/enable)
 */
void fa_ahb_dma_add(int ch)
{
	unsigned int        val=0;
	volatile ahb_lld_t  *lld;
	ahb_lld_t           *lld_phys;

	dma_dbg("add ahb_dma_chan[%d].priv=0x%p\n",ch, &ahb_dma_chan[ch].priv);
	lld=ahb_dma_chan[ch].priv.ahb_dma_lld;
	lld_phys=ahb_dma_chan[ch].priv.ahb_dma_lld_phys;

	dma_dbg("add lld=0x%p ahb_dma_chan[ch].priv.llp_count=%d ahb_dma_chan[ch].priv.llp_free_idx=%d\n",lld,ahb_dma_chan[ch].priv.llp_count,ahb_dma_chan[ch].priv.llp_free_idx);
	dma_dbg("dest=0x%x dma_count=0x%x\n",ahb_dma_chan[ch].parm.dest,ahb_dma_chan[ch].parm.size);
	if(ahb_dma_chan[ch].priv.llp_free_idx==0) //first to call ahb_dma_add
	{
		/* __asm__("nop");__asm__("nop");__asm__("nop"); */
		outl(ahb_dma_chan[ch].parm.size, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_TXSZ_0x14);
		outl(ahb_dma_chan[ch].parm.src, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_SRC_0x8);
		outl(ahb_dma_chan[ch].parm.dest, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_DST_0xC);

		outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_LLP_0x10);  //john modified, for no LLP
        
		val=(((1-(ahb_dma_chan[ch].parm.irq))<<31)&0x80000000)|
		        ((ahb_dma_chan[ch].parm.bs<<16)&0x70000)|
			((ahb_dma_chan[ch].parm.sw<<11)&0x00003800)|((ahb_dma_chan[ch].parm.dw<<8)&0x00000700)|
			((ahb_dma_chan[ch].priv.hw_handshake<<7)&0x00000080)|
			((ahb_dma_chan[ch].parm.sctl<<5)&0x00000060)|((ahb_dma_chan[ch].parm.dctl<<3)&0x00000018)|
			((ahb_dma_chan[ch].priv.src_data_master<<2)&0x4)|
			((ahb_dma_chan[ch].priv.dest_data_master<<1)&0x2);
		outl(val, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0);
		val=(ahb_dma_chan[ch].priv.llp_count<<16)|
			(ahb_dma_chan[ch].priv.dst_he<<13)|
			(ahb_dma_chan[ch].priv.dst_rs<<9)|
			(ahb_dma_chan[ch].priv.src_he<<7)|
			(ahb_dma_chan[ch].priv.src_rs<<3);		
		outl(val, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CFG_0x4);

	}
	else
	{
		val=(((1-(ahb_dma_chan[ch].parm.irq))<<28)&0x10000000)|
			((ahb_dma_chan[ch].parm.sw<<25)&0x0e000000)|((ahb_dma_chan[ch].parm.dw<<22)&0x01c00000)|
			((ahb_dma_chan[ch].parm.sctl<<20)&0x00300000)|((ahb_dma_chan[ch].parm.dctl<<18)&0x000c0000)|
			((ahb_dma_chan[ch].priv.src_data_master<<17)&0x00020000)|
			((ahb_dma_chan[ch].priv.dest_data_master<<16)&0x00010000)|(ahb_dma_chan[ch].parm.size);

		lld[ahb_dma_chan[ch].priv.llp_free_idx-1].source=ahb_dma_chan[ch].parm.src;
		lld[ahb_dma_chan[ch].priv.llp_free_idx-1].dest=ahb_dma_chan[ch].parm.dest;
		lld[ahb_dma_chan[ch].priv.llp_free_idx-1].control=val;
		lld[ahb_dma_chan[ch].priv.llp_free_idx-1].llp=0;
		lld[ahb_dma_chan[ch].priv.llp_free_idx-1].tot_size = ahb_dma_chan[ch].parm.size;

		if(ahb_dma_chan[ch].priv.ahb_last_lld)
			ahb_dma_chan[ch].priv.ahb_last_lld->llp=(unsigned int)(&lld_phys[ahb_dma_chan[ch].priv.llp_free_idx-1])|ahb_dma_chan[ch].priv.llp_master;
		else {
		    outl((volatile unsigned int)(&lld_phys[0])|ahb_dma_chan[ch].priv.llp_master,
			 ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_LLP_0x10);
		}
		ahb_dma_chan[ch].priv.ahb_last_lld=(ahb_lld_t *)&lld[ahb_dma_chan[ch].priv.llp_free_idx-1];
	}
    
	if (ahb_dma_chan[ch].priv.llp_count)   //john add check
	{
		if(ahb_dma_chan[ch].priv.llp_free_idx==ahb_dma_chan[ch].priv.llp_count)
			ahb_dma_chan[ch].priv.llp_free_idx=1;
		else
			ahb_dma_chan[ch].priv.llp_free_idx++;
	}
}

/**
 *	Add Linked List Descriptor to the specific AHB DMA channel.
 *	@ch: DMA chnnel number.
 *
 *	LLP count =3 (example)
 *	     0 => 1 => 2
 *	     ^         |
 *	     +---------+
 *	You must prepare ahb_dma_parm_t's parameters before calling ahb_dma_add:
 *		src: source address
 *		dest:dest address
 *		sw: source width (0/1/2=>8/16/32)
 *		dw: dest width (0/1/2=>8/16/32)
 *		sctl: source control (0/1/2/3=>inc/dec/fix/x)
 *		dctl: dest coontrol (0/1/2/3=>inc/dec/fix/x)
 *		size: dma count
 *		irq: (0/1)==>(disable/enable)
 */
void fa_ahb_dma_add_no_extrahead(int ch)
{
	unsigned int        val = 0;
	volatile ahb_lld_t  *lld;
	ahb_lld_t           *lld_phys;
        
	dma_dbg("add ahb_dma_chan[%d].priv=0x%p\n",ch, &ahb_dma_chan[ch].priv);
	lld     =ahb_dma_chan[ch].priv.ahb_dma_lld;
	lld_phys=ahb_dma_chan[ch].priv.ahb_dma_lld_phys;

	dma_dbg("add lld=0x%p ahb_dma_chan[ch].priv.llp_count=%d ahb_dma_chan[ch].priv.llp_free_idx=%d\n",lld,ahb_dma_chan[ch].priv.llp_count,ahb_dma_chan[ch].priv.llp_free_idx);
	dma_dbg("dest=0x%x dma_count=0x%x\n",ahb_dma_chan[ch].parm.dest,ahb_dma_chan[ch].parm.size);
	
	/* in the first time, update hardware related registers 
	 */
	if(ahb_dma_chan[ch].priv.llp_free_idx == 0) //first to call ahb_dma_add
	{
		/* __asm__("nop");__asm__("nop");__asm__("nop"); */
		outl(ahb_dma_chan[ch].parm.size, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_TXSZ_0x14);
		outl(ahb_dma_chan[ch].parm.src, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_SRC_0x8);
		outl(ahb_dma_chan[ch].parm.dest, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_DST_0xC);

		outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_LLP_0x10);  //john modified, for no LLP
        
		val=(((1-(ahb_dma_chan[ch].parm.irq))<<31)&0x80000000)|
		    ((ahb_dma_chan[ch].parm.chpri << 22) & 0xC00000) |
		    ((ahb_dma_chan[ch].parm.bs<<16)&0x70000)|
			((ahb_dma_chan[ch].parm.sw<<11)&0x00003800)|((ahb_dma_chan[ch].parm.dw<<8)&0x00000700)|
			((ahb_dma_chan[ch].priv.hw_handshake<<7)&0x00000080)|
			((ahb_dma_chan[ch].parm.sctl<<5)&0x00000060)|((ahb_dma_chan[ch].parm.dctl<<3)&0x00000018)|
			((ahb_dma_chan[ch].priv.src_data_master<<2)&0x4)|
			((ahb_dma_chan[ch].priv.dest_data_master<<1)&0x2);
		outl(val, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0);
		val=(ahb_dma_chan[ch].priv.llp_count<<16)|
			(ahb_dma_chan[ch].priv.dst_he<<13)|
			(ahb_dma_chan[ch].priv.dst_rs<<9)|
			(ahb_dma_chan[ch].priv.src_he<<7)|
			(ahb_dma_chan[ch].priv.src_rs<<3);		
		outl(val, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CFG_0x4);
	}
	
	/* LLP list */
	if (ahb_dma_chan[ch].priv.llp_count > 1)
	{
		val = (((1 - (ahb_dma_chan[ch].parm.irq)) << 28) & 0x10000000) |
			  ((ahb_dma_chan[ch].parm.sw << 25) & 0x0e000000) | ((ahb_dma_chan[ch].parm.dw << 22) & 0x01c00000) |
			  ((ahb_dma_chan[ch].parm.sctl << 20) & 0x00300000) | ((ahb_dma_chan[ch].parm.dctl<<18) & 0x000c0000) |
			  ((ahb_dma_chan[ch].priv.src_data_master << 17) & 0x00020000) |
			  ((ahb_dma_chan[ch].priv.dest_data_master << 16) & 0x00010000) | (ahb_dma_chan[ch].parm.size);

		lld[ahb_dma_chan[ch].priv.llp_free_idx].source  = ahb_dma_chan[ch].parm.src;
		lld[ahb_dma_chan[ch].priv.llp_free_idx].dest    = ahb_dma_chan[ch].parm.dest;
		lld[ahb_dma_chan[ch].priv.llp_free_idx].control = val;
		lld[ahb_dma_chan[ch].priv.llp_free_idx].llp     = 0;
		lld[ahb_dma_chan[ch].priv.llp_free_idx].tot_size= ahb_dma_chan[ch].parm.size;
        
        /* first node */
		if (ahb_dma_chan[ch].priv.ahb_last_lld == NULL)
		{
		    /* first LLP, so update to hardware */
		    outl((volatile unsigned int)(&lld_phys[1]) | ahb_dma_chan[ch].priv.llp_master, ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_LLP_0x10);
		}
		else
		{
		    /* update the last node's LLP */
			ahb_dma_chan[ch].priv.ahb_last_lld->llp = (unsigned int)(&lld_phys[ahb_dma_chan[ch].priv.llp_free_idx]) | ahb_dma_chan[ch].priv.llp_master;
		}
		
		ahb_dma_chan[ch].priv.ahb_last_lld = (ahb_lld_t *)&lld[ahb_dma_chan[ch].priv.llp_free_idx]; /* second time, points to 0 */
	}
    
	if (ahb_dma_chan[ch].priv.llp_count > 1)	
		ahb_dma_chan[ch].priv.llp_free_idx++;
}

/**
 *	Automacticaly allocated enough Linked List Descriptors and link to specific DMA channel.
 *	@ch: DMA chnnel number.
 *
 *	Please prepare ahb_dma_parm_t's parameters by calling fa_set_ahb_dma_transfer_params2, fa_set_ahb_dma_src_params
 *	fa_set_ahb_dma_dst_params functions. And the fa_set_ahb_dma_src_params and fa_set_ahb_dma_dst_params should be
 *	called before calling fa_set_ahb_dma_transfer_params2. Driver automatically allocates enough LLD and link to
 *	specific DMA channel. The interrupt only occurs after the last LLD finishing its transfer.
 *
 */
void fa_ahb_dma_add_auto(int ch, int total_bytes)
{
	int i;
	int LLPCount;
	int src_addr,dest_addr;
	int one_llp_size = fa_llp_size(ahb_dma_chan[ch].parm.bs, ahb_dma_chan[ch].parm.sw);
	int DataSize = one_llp_size/*4032*/ << max(ahb_dma_chan[ch].parm.sw, ahb_dma_chan[ch].parm.dw);
	LLPCount = ahb_dma_chan[ch].priv.llp_count;
	dma_dbg("fa_ahb_dma_add_auto: sw = %d, dw = %d\n",ahb_dma_chan[ch].parm.sw, ahb_dma_chan[ch].parm.dw);
	dma_dbg("fa_ahb_dma_add_auto: LLPCount=%d, Total bytes = %d, DataSize=%d, one_llp_size = %d\n",LLPCount, total_bytes, DataSize, one_llp_size);
	if (LLPCount>0) {
		ahb_dma_chan[ch].parm.size = one_llp_size/*4032*/;
	}
	else {
		ahb_dma_chan[ch].parm.size = total_bytes >> max(ahb_dma_chan[ch].parm.sw, ahb_dma_chan[ch].parm.dw);
	}
	src_addr = ahb_dma_chan[ch].parm.src;
	dest_addr = ahb_dma_chan[ch].parm.dest;
	ahb_dma_chan[ch].priv.llp_count = LLPCount;
	for(i=0;i < LLPCount;i++) {
		switch ( ahb_dma_chan[ch].parm.sctl )
		{
			case AHBDMA_CTL_INC:
				ahb_dma_chan[ch].parm.src = src_addr + DataSize*i;
				break;
			case AHBDMA_CTL_DEC:
				ahb_dma_chan[ch].parm.src = src_addr - DataSize*i;
				break;
			case AHBDMA_CTL_FIX:
				ahb_dma_chan[ch].parm.src = src_addr;
				break;
		}
                switch ( ahb_dma_chan[ch].parm.dctl )
                {
                        case AHBDMA_CTL_INC:
                                ahb_dma_chan[ch].parm.dest = dest_addr + DataSize*i;
                                break;
                        case AHBDMA_CTL_DEC:
                                ahb_dma_chan[ch].parm.dest = dest_addr - DataSize*i;
                                break;
                        case AHBDMA_CTL_FIX:
                                ahb_dma_chan[ch].parm.dest = dest_addr;
                                break;
                }
		ahb_dma_chan[ch].parm.irq = AHBDMA_ERR_TRIGGER_IRQ;
		fa_ahb_dma_add(ch);
	}
	ahb_dma_chan[ch].parm.size = (total_bytes-(DataSize*LLPCount))>>max(ahb_dma_chan[ch].parm.sw, ahb_dma_chan[ch].parm.dw);
	switch ( ahb_dma_chan[ch].parm.sctl )
	{
	        case AHBDMA_CTL_INC:
	                ahb_dma_chan[ch].parm.src = src_addr + DataSize*i;
	                break;
	        case AHBDMA_CTL_DEC:
	                ahb_dma_chan[ch].parm.src = src_addr - DataSize*i;
	                break;
	        case AHBDMA_CTL_FIX:
	                ahb_dma_chan[ch].parm.src = src_addr;
	                break;
	}
	switch ( ahb_dma_chan[ch].parm.dctl )
	{
	        case AHBDMA_CTL_INC:
	                ahb_dma_chan[ch].parm.dest = dest_addr + DataSize*i;
	                break;
	        case AHBDMA_CTL_DEC:
	                ahb_dma_chan[ch].parm.dest = dest_addr - DataSize*i;
	                break;
	        case AHBDMA_CTL_FIX:
	                ahb_dma_chan[ch].parm.dest = dest_addr;
	                break;
	}
	ahb_dma_chan[ch].parm.irq = AHBDMA_TRIGGER_IRQ;
	fa_ahb_dma_add(ch);
}

void fa_ahb_dma_clear_llp_idx(int ch)
{
    ahb_dma_chan[ch].priv.llp_free_idx=0;
    ahb_dma_chan[ch].priv.ahb_last_lld=0;
    //outl(0, ahb_dma_chan[ch].priv.base+AHBDMA_ENABLE_0x24);		//disable DMA controller
    //outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0);	//disable DMA channel 0         
    //outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_LLP_0x10);
    //outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_TXSZ_0x14);	//no transfer size (use LLP)
    //outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CFG_0x4);	//disable all interrupt
}

/**
 *	Reset AHB DMA channel seetings
 *	@ch:	Specific DMA channel.
 *
 *	Please use this funciton to reset AHB DMA channel's settign and status
 */

void fa_ahb_dma_reset(int ch)
{
	int size;
	ahb_dma_chan[ch].priv.llp_last_idx=0;
	ahb_dma_chan[ch].priv.llp_free_idx=0;
	ahb_dma_chan[ch].priv.ahb_last_lld=0;

	dma_dbg("ahb_dma_reset\n");
#if 0   //BugFix: This will influence other channel that is operating. //francis 060914
	outl(0, ahb_dma_chan[ch].priv.base+AHBDMA_ENABLE_0x24);		//disable DMA controller
#endif
	outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0);	//disable DMA channel 0         
	outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_LLP_0x10);
	outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_TXSZ_0x14);	//no transfer size (use LLP)
	outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CFG_0x4);	//disable all interrupt

	//Clean status
	outl(1<<ch, ahb_dma_chan[ch].priv.base+AHBDMA_ISR_0x8);
	outl((1<<(ch+0))|(1<<(ch+16)), ahb_dma_chan[ch].priv.base+AHBDMA_ERR_CLR_0x10);

	size=sizeof(ahb_lld_t)*(ahb_dma_chan[ch].priv.llp_count);
        if(ahb_dma_chan[ch].priv.allocated_dma_lld)
        {
                dma_dbg("free 0x%p with 0x%p size 0x%x\n",ahb_dma_chan[ch].priv.ahb_dma_lld,ahb_dma_chan[ch].priv.ahb_dma_lld_phys,size);
                dma_free_coherent(NULL, size, (void *)(ahb_dma_chan[ch].priv.ahb_dma_lld),(dma_addr_t)(ahb_dma_chan[ch].priv.ahb_dma_lld_phys));
		ahb_dma_chan[ch].priv.allocated_dma_lld = 0;
        }
}

void fa_ahb_dma_reset2(int ch)
{
	ahb_dma_chan[ch].priv.llp_last_idx=0;
	ahb_dma_chan[ch].priv.llp_free_idx=0;
	ahb_dma_chan[ch].priv.ahb_last_lld=0;

	dma_dbg("ahb_dma_reset2\n");
	outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0);	//disable DMA channel 0         
	outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_LLP_0x10);
	outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_TXSZ_0x14);	//no transfer size (use LLP)
	outl(0, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CFG_0x4);	//disable all interrupt
	//Clean status

	fa_ahb_dma_clear_int(ch);
}

/**
 *	Get AHB DMA current status
 *	@ch:	Specific DMA channel.
 *
 *	Please use this funciton to get specific AHB DMA channel's status
 *	0(INT_DMA_TRIGGER) - indicate AHB DMA interrupt complete 
 *	1(INT_DMA_ERROR) - indicate AHB DMA response error interrupt
 */

unsigned int fa_ahb_dma_get_status(int ch)
{
	u32 status=0;
	u32 temp;
	//dma_dbg("%s ==>\n",__func__);
	temp = inl(ahb_dma_chan[ch].priv.base+AHBDMA_INT_ERR_0x0C);
	if ( temp & (1<<(ch)) ) 
		status |= INT_DMA_ERROR;	//error

	if ( temp & (1<<(ch+16)) )
		status |= INT_DMA_ABORT;	//Abort
		
	temp = inl(ahb_dma_chan[ch].priv.base+AHBDMA_INT_TC_0x04);
	if ( temp & (1<<(ch)) )
		status |= INT_DMA_TRIGGER;	//complete

    if (status) {
	    if ( fa_ahb_dma_all_done(ch) ){
		    status |= INT_DMA_ALL_DONE;
        }
    }
    return status;
}

/**
 *	Get the pointer of AHB DMA parameter (ahb_dma_parm_t).
 *	@ch: The DMA chnnel number.
 *
 *	Return the pointer to the data structure ahb_dma_parm_t
 */

ahb_dma_parm_t *fa_get_ahb_dma_params(int ch)
{
	return &(ahb_dma_chan[ch].parm);
}

/**
 *	Setup AHB DMA uaual configurations (src/dest addr and irq).
 *	@ch: The DMA chnnel number.
 *	@src_addr: source address
 *	@dest_addr:destination address
 *	@irq: 0x01: Enable finishing interrupt, 0x02: Enable error response interrupt. 0x3: Enable both. 
 *            If enable, AHB DMA will issue interrutp when DMA finish the link list descriptor or report error 
 *	      So they can be combination by OR.
 *
 *	It is usually used for modifying configurations for the Linked List Descriptor before adding a new LLD.
 */
void fa_set_ahb_dma_usual_params(int ch, u32 src_addr, u32 dest_addr, int irq)
{
	ahb_dma_chan[ch].parm.src = src_addr;
	ahb_dma_chan[ch].parm.dest = dest_addr;
	ahb_dma_chan[ch].parm.irq = irq;
}

#if 0
/**
 *	Setup APB DMA configuration by ahb_dma_parm_t structure.
 *	@ch: The DMA chnnel number.
 *	@parm: The pointer to ahb_dma_parm_t stucture which contains all DMA configuation.
 *	
 *	You must prepared before enable/start AHB DMA operation:
 *	ahb_dma_parm_t data stucture: 
 *	@src: source address
 *	@dest:dest address
 *	@sw: source width (0/1/2=>8/16/32)
 *	@dw: destination width (0/1/2=>8/16/32)
 *	@stype: source type (0/1 => APB/AHB)    
 *	@dtype: dest type (0/1 => APB/AHB)      
 *	@sctl:  source address control. It should be one of [0,1,2,4].
 *              0-Increment 1-Decremetn 2-Fix 3-Reserved
 *	@dctl:  dest address control. It should be one of [0,1,2,4].
 *              0-Increment 1-Decremetn 2-Fix 3-Reserved
 *	@size:	The number of DMA transfer want to be transfered. Each DMA transfer tx/rx
 *		width (1/2/4) data bytes. So each DMA transfer may have 1,2,4 bytes data.
 *	@irq: 0x01: Enable finishing interrupt, 0x02: Enable error response interrupt. 0x3: Enable both. 
 *            If enable, AHB DMA will issue interrutp when DMA finish the link list descriptor or report error 
 *	So they can be combination by OR.
 */
void fa_set_ahb_dma_params(int ch, ahb_dma_parm_t *parm)
{
	memcpy(&ahb_dma_chan[ch].parm, parm, sizeof(ahb_dma_parm_t) );
}

/**
 *	Setup AHB DMA configuration by ahb_dma_data_t structure.
 *	@ch: The DMA chnnel number.
 *	@priv: The pointer to ahb_dma_data_t stucture which contains all DMA configuation.
 *	
 *	You must prepared before enable/start AHB DMA operation:
 *	ahb_dma_parm_t data stucture: 
 *	@base: base address of ahb dma
 *	@llp_master: LLP master channel number
 *	@src_data_master: source data master number
 *	@dest_data_master: dest data master number
 *	@llp_count:  LLP count
 *	@channel:    ahb dma channel
 *	@hw      is it hardware handshake?
 */
 
void fa_set_ahb_dma_ch_params(int ch, ahb_dma_data_t *priv)
{
	ahb_dma_chan[ch].priv.llp_master = priv->llp_master;
	ahb_dma_chan[ch].priv.llp_count = priv->llp_count;
	ahb_dma_chan[ch].priv.hw_handshake = priv->hw_handshake;
	ahb_dma_chan[ch].priv.src_data_master = priv->src_data_master;
	ahb_dma_chan[ch].priv.channel = priv->channel;
}
#endif

/**
 *	Setup AHB DMA transfer mode configurations used for DMA count.
 *	@ch: DMA chnnel number.
 *	@llp_master: Master for loanding the next linked list descriptor. 0/1
 *	@llp_count: LLP number for link list.
 *	@elem_count:	The number of DMA transfer want to be transfered. Each DMA transfer tx/rx
 *			width (1/2/4) data bytes. So each DMA transfer may have 1,2,4 bytes data.

 *	@hw_handshake: Hardware handshake. 0 - Disable, 1 - Enable
 *	@irq_set: AHB DMA interrup setting - INT_DMA_TRIGGER, INT_DMA_ERROR. They can be a combination by OR.
 *	So 0x01: Enable finishing interrupt, 0x02: Enable error response interrupt. 0x3: Enable both.
 *
 *	Pleaes set up the APB DMA tansfer configurations via this funciton before you stating
 *	up a DMA channel.
 */
void fa_set_ahb_dma_transfer_params(int ch, int llp_master, int llp_count, int elem_count,
						 int hw_handshake, int irq_set)
{	
	ahb_dma_chan[ch].priv.llp_master = llp_master;
	ahb_dma_chan[ch].priv.llp_count = llp_count;	
	ahb_dma_chan[ch].parm.size = elem_count;
	ahb_dma_chan[ch].priv.hw_handshake = hw_handshake;
	ahb_dma_chan[ch].parm.irq = irq_set;
}

/**
 *	Setup AHB DMA transfer mode configurations used for total bytes
 *	@ch: DMA chnnel number.
 *	@llp_master: Master for loading the next LLP.
 *	@total_bytes: The total bytes of the transfer data. The demanded LLP count will be automatically
 *		      calculated by driver according src/dest width setting.
 *	@hw_handshake: Hardware handshake. 0 - Disable, 1 - Enable
 *
 *	Pleaes set up the APB DMA tansfer configurations via this funciton if you want to pass total bytes
 *      to driver. Driver automatically calculates the enough LLP count according to the src/dest width.
 */
void fa_set_ahb_dma_transfer_params2(int ch, int llp_master, int total_bytes, int hw_handshake)
{	
	ahb_dma_chan[ch].priv.llp_master = llp_master;
	ahb_dma_chan[ch].priv.llp_count = fa_ahb_dma_get_llpcount(ch, total_bytes);;
	ahb_dma_chan[ch].priv.hw_handshake = hw_handshake;
}

/*
 *  Setup AHB DMA burst size.
 *	@ch: DMA chnnel number.
 *	@burst_size:	source burst size (0/1/2/3/4/5/6/7=>1/4/8/16/32/64/128/256)
 */
void fa_set_ahb_dma_transfer_bs(int ch, int burst_size)
{
	ahb_dma_chan[ch].parm.bs = burst_size;
}

/**
 *	Setup source AHB DMA configurations.
 *	@ch: DMA chnnel number.
 *	@src_addr: The source address.
 *	@src_data_master: Source master seletion - AHBDMA_MASTER_0 or AHBDMA_MASTER_1
 *	@sw: source width (0/1/2=>8/16/32)
 *	@sctl:  source address control. It should be one of [0,1,2,4].
 *              0-Increment 1-Decremetn 2-Fix 3-Reserved
 *
 *	Pleaes set up the detination DMA configuration via this funciton before you stating
 *	up a DMA channel.
 */
void fa_set_ahb_dma_src_params(int ch, u32 src_addr, int src_data_master, int sw, int sctl)
{
	ahb_dma_chan[ch].parm.src = src_addr;
	ahb_dma_chan[ch].priv.src_data_master = src_data_master;
	ahb_dma_chan[ch].parm.sw = sw;
	ahb_dma_chan[ch].parm.sctl = sctl;
}

/**
 *	Setup destination AHB DMA configurations.
 *	@ch: DMA chnnel number.
 *	@dst_addr: The destination address.
 *	@dest_data_master: Destination master seletion - AHBDMA_MASTER_0 or AHBDMA_MASTER_1
 *	@dw: destination width (0/1/2=>8/16/32)
 *	@sctl:  Destination address control. It should be one of [0,1,2,4].
 *              0-Increment 1-Decremetn 2-Fix 3-Reserved
 *	Pleaes set up the detination DMA configuration via this funciton before you stating
 *	up a DMA channel.
 */

void fa_set_ahb_dma_dst_params(int ch, u32 dst_addr, int dest_data_master, int dw, int dctl)
{
	ahb_dma_chan[ch].parm.dest = dst_addr;
	ahb_dma_chan[ch].priv.dest_data_master = dest_data_master;
	ahb_dma_chan[ch].parm.dw = dw;
	ahb_dma_chan[ch].parm.dctl = dctl;
}

void fa_set_ahb_dma_cfg_params(int ch, int src_he, int src_rs, int dst_he, int dst_rs)
{
	ahb_dma_chan[ch].priv.src_he = src_he;
	ahb_dma_chan[ch].priv.src_rs = src_rs;
	ahb_dma_chan[ch].priv.dst_he = dst_he;
	ahb_dma_chan[ch].priv.dst_rs = dst_rs;
}

/**
 *	Start the specific AHB DMA channel
 *	@ch:	Specific DMA channel.
 *
 *	Please use this funciton to start AHB DMA channel
 */

void fa_ahb_dma_start(int ch)
{    
	#ifdef DMA_DEBUG
	fa_ahb_dma_show_config(ch);
	#endif

	ahb_dma_chan[ch].active = 1;
	outl(0x1, ahb_dma_chan[ch].priv.base+AHBDMA_ENABLE_0x24);  //Enable DMA  controller
	outl(inl(ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0)|0x1, ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0);

}

/**
 *	Clear AHB DMA Channel interrupt status
 *	@dma_ch: The APB DMA channel number want to be clear the int status.
 *
 *	Pleaes use this function to clear all interrupt status of specific AHB DMA channel.
 */

void fa_ahb_dma_clear_int(int ch)
{
	outl(1<<ch, ahb_dma_chan[ch].priv.base+AHBDMA_ISR_0x8);
	outl((1<<(ch+0))|(1<<(ch+16)), ahb_dma_chan[ch].priv.base+AHBDMA_ERR_CLR_0x10);
}


/**
 *	Stop the specific APB DMA channel
 *	@ch:	Specific DMA channel.
 *
 */

void fa_ahb_dma_stop(int ch)
{
	dma_dbg("ahb_dma_stop\n");
#if 0
	//Abort/Stop DMA channel
	outl(inl(ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0)|(1<<15), ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0);
#else
    //mask ERR and ABORT int
    outl(inl((void *)(ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_CFG_0x4))|(3<<1), (ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_CFG_0x4)); 
#endif
	//Disable DMA channel
	outl(inl(ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0)&(~0x1), ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0);
#if 0   //BugFix: This will influence other channel that is operating. //francis 060914
	outl(inl(ahb_dma_chan[ch].priv.base+AHBDMA_ENABLE_0x24)&~(0x1), ahb_dma_chan[ch].priv.base+AHBDMA_ENABLE_0x24);
#endif
	fa_ahb_dma_clear_int(ch);
	ahb_dma_chan[ch].active = 0;
    //unmask ERR and ABORT int
    outl(inl((void *)(ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_CFG_0x4)) & (~(3<<1)), (ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_CFG_0x4)); 
}

void fa_ahb_free_llp_mem(u32 ch)
{
        int size;

	size=sizeof(ahb_lld_t)*(ahb_dma_chan[ch].priv.llp_count);
        if(ahb_dma_chan[ch].priv.allocated_dma_lld)
        {
                dma_dbg("free 0x%p with 0x%p size 0x%x\n",ahb_dma_chan[ch].priv.ahb_dma_lld,ahb_dma_chan[ch].priv.ahb_dma_lld_phys,size);
                dma_free_coherent(NULL, size, (void *)(ahb_dma_chan[ch].priv.ahb_dma_lld),(dma_addr_t)(ahb_dma_chan[ch].priv.ahb_dma_lld_phys));
		ahb_dma_chan[ch].priv.allocated_dma_lld = 0;
        }
	fa_ahb_dma_reset2(ch);
}

void fa_ahb_dma_resume(u32 ch)
{
    outl(inl(ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0)|(0x1), 
	 ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0);
}

void fa_ahb_dma_pause(u32 ch)
{
    outl(inl(ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0)&(~0x1),
	 ahb_dma_chan[ch].priv.channel_base+AHBDMA_CHANNEL_CSR_0x0);
}
/* 
 * This function will form a ring and add dma without extra head node. The head node
 * is the head of LLP List. That is llp_list[0].
 *
 * llp_mode = 0, move sequencial data to 2D address
 * llp_mode = 1, last llp will link to first llp and always loop back
 * llp_mode = 2, move 2D data to 2D address, src and dst offset are the same 
 */
void fa_ahb_dma_cloop_llp_init(u32 ch, u32 start, u32 offset_address, u32 size, u32 end, u32 dir, u8 llp_mode)
{
    u32 i, psize, tsize, taddr;
    u32 Max_DMA_SZ;
    
    /* The dma block size (units) of TOT_SIZE depends on the source width. */
    Max_DMA_SZ = (dir == DMA_FROM_DEVICE) ?  (fa_llp_size(ahb_dma_chan[ch].parm.bs, ahb_dma_chan[ch].parm.dw)) : 
                                             (fa_llp_size(ahb_dma_chan[ch].parm.bs, ahb_dma_chan[ch].parm.sw));
    
    for(i = start; i < end; i += offset_address) {
    	psize = size;	
    	taddr = i;	
    	while(psize){
    	    if(unlikely(psize <= Max_DMA_SZ)) {
             tsize = psize;
             ahb_dma_chan[ch].parm.irq = AHBDMA_TRIGGER_IRQ;
    	    }
    	    else {
             tsize = Max_DMA_SZ;
             ahb_dma_chan[ch].parm.irq = AHBDMA_NO_TRIGGER_IRQ;
    	    }
    	    
    	    if(dir == DMA_FROM_DEVICE) {	    
             ahb_dma_chan[ch].parm.dest = taddr;		
             ahb_dma_chan[ch].parm.size = tsize >> ahb_dma_chan[ch].parm.dw;
    	    }
    	    else {
             ahb_dma_chan[ch].parm.src = taddr;
             ahb_dma_chan[ch].parm.size = tsize >> ahb_dma_chan[ch].parm.sw;
    	    }
    	    fa_ahb_dma_add_no_extrahead(ch);
    	    taddr += offset_address;
    	    psize -= tsize;
    	    if(psize){
	    	    if(dir == DMA_FROM_DEVICE)    
			    		ahb_dma_chan[ch].parm.src += tsize;		
	    	    else
			    		ahb_dma_chan[ch].parm.dest += tsize;
    	    }    	    	
    	}
    	if(llp_mode == 0){
		    if(dir == DMA_FROM_DEVICE)    
					ahb_dma_chan[ch].parm.src += size;		
		    else
					ahb_dma_chan[ch].parm.dest += size;
			}  
			else if(llp_mode == 2){
		    if(dir == DMA_FROM_DEVICE)    
					ahb_dma_chan[ch].parm.src += offset_address;		
		    else
					ahb_dma_chan[ch].parm.dest += offset_address;
			}     	
    } 
    /* link from tail to head and form a ring here */
    if ((llp_mode == 1) && (ahb_dma_chan[ch].priv.llp_count > 1))
        ahb_dma_chan[ch].priv.ahb_last_lld->llp = (unsigned int)(&ahb_dma_chan[ch].priv.ahb_dma_lld_phys[0])|
	                                      ahb_dma_chan[ch].priv.llp_master;
}

u32 fa_ahb_set_dma_sg(u32 ch, struct scatterlist *sg, int nr_sg, u32 dir, u32 b_irq)
{
    u32 total_size = 0;
    u32 Max_DMA_SZ;

   if(dir == DMA_FROM_DEVICE) {
       Max_DMA_SZ = fa_llp_size(ahb_dma_chan[ch].parm.bs, ahb_dma_chan[ch].parm.dw);
   }
   else {
       Max_DMA_SZ = fa_llp_size(ahb_dma_chan[ch].parm.bs, ahb_dma_chan[ch].parm.sw);
   }

    fa_ahb_dma_clear_llp_idx(ch);
    while (nr_sg) {
	u32 cur_addr;
	u32 cur_len;

	cur_addr = sg_dma_address(sg);
	cur_len  = sg_dma_len(sg);

	while (cur_len) {
	    u32 bcount;
	    if(unlikely(cur_len <= Max_DMA_SZ)){
		bcount = cur_len;
		if(nr_sg == 1 && b_irq) {
		    ahb_dma_chan[ch].parm.irq = AHBDMA_TRIGGER_IRQ;
		}
		else
		    ahb_dma_chan[ch].parm.irq = AHBDMA_NO_TRIGGER_IRQ;
	    }
	    else { 
		bcount = Max_DMA_SZ;
		ahb_dma_chan[ch].parm.irq = AHBDMA_NO_TRIGGER_IRQ;
	    }
	    
	    if(dir == DMA_FROM_DEVICE) {
		ahb_dma_chan[ch].parm.dest = cur_addr;
		ahb_dma_chan[ch].parm.size = bcount >> ahb_dma_chan[ch].parm.dw;
	    }
	    else{
		ahb_dma_chan[ch].parm.src = cur_addr;
		ahb_dma_chan[ch].parm.size = bcount >> ahb_dma_chan[ch].parm.sw;
	    }
	    fa_ahb_dma_add(ch);
	    
	    cur_addr += bcount;
	    cur_len -= bcount;
	    total_size += bcount;
	}
	sg++;
	nr_sg--;
    }
    return total_size;
}

/*
 *	FA Generic AHB DMA interrupt handler
 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19))
static irqreturn_t fa_ahb_dma_irq_handler(int irq, void *dev_id)
#else 
static irqreturn_t fa_ahb_dma_irq_handler(int irq, void *dev_id, struct pt_regs *regs)
#endif
{
	int ch = ((int) dev_id) - 1;
	u32 dma_int_sts;

	dma_int_sts = fa_ahb_dma_get_status(ch);
	//dma_dbg("APB DMA INT: irq=%d, ch=%d, int_status=%02X\n", irq, ch, dma_int_sts);
	if (!dma_int_sts)
	        return IRQ_NONE;	    
	if (unlikely(ahb_dma_chan[ch].dev_id == -1)) {
		printk(KERN_WARNING "Spurious interrupt from AHB DMA channel %d (CSR %04x)\n",
		       ch, dma_int_sts);
		fa_ahb_dma_clear_int(ch);
		return IRQ_NONE;
	}
	if (unlikely(dma_int_sts & INT_DMA_ERROR))
		printk(KERN_WARNING "AHB DMA response error with device %d\n", ahb_dma_chan[ch].dev_id);
	if (unlikely(dma_int_sts & INT_DMA_ABORT))
		dma_dbg("AHB DMA: DMA abort (device=%d)\n", ahb_dma_chan[ch].dev_id);
	if (likely(dma_int_sts & INT_DMA_TRIGGER))
		//ahb_dma_chan[ch].active = 0;
		;
	if (likely(ahb_dma_chan[ch].callback != NULL))
	{
		ahb_dma_chan[ch].callback(ch, dma_int_sts, ahb_dma_chan[ch].data);
	}
	//Clean DMA interrupt status
	fa_ahb_dma_clear_int(ch);
	return IRQ_HANDLED;
}

/**
 *	Automatically allocate a free DMA channel resourece.
 *	@dev_id: Device id. The dev_id must >= 0.
 *	@dev_name: Device name
 *	@call_back: The callback funcition. It will be called afeter DMA interrup triggered.
 *	@data: The user defined data structure pointer. It will be passed when calling callback function.
 *	@dma_ch_out: The pointer to a interger which is used to store the allocated DMA channel.
 *	@pio_mode: 1 - Using DMA PIO mode, 0 - Using DMA INT mode (needs pass callback funciton)
 *
 *	Pleaes use this function to automatically allocate the AHB DMA channel.
 *	0-3 chnnels are GM AHB DMA channels.
 *	Callback function's prototype: void (* callback)(int ch, u16 int_status, void *data)
 *	Here ch is DMA channel number. int_status is DMA status. data is a pointer to user defined structure.
 *
 *	@Returns: zero for successful. Non-zero for erros.
 */

int fa_request_ahb_dma_auto(int dev_id, const char *dev_name,
		     void (* callback)(int ch, u16 int_status, void *data),
		     void *data, int *dma_ch_out, int pio_mode)
{
	int r;
	int ch, free_ch = -1;
	unsigned long flags;
	struct fa_ahb_dma_ch *chan;
	spin_lock_irqsave(&ahb_dma_chan_lock, flags);
	for (ch = 0; ch < FA_AHB_DMA_CHANNEL_MAX; ch++) {
		if ( (ch%4) == 3 )	//Cannel 3,7 is internal used
			continue;
		if (free_ch == -1 && ahb_dma_chan[ch].dev_id == -1) {
			free_ch = ch;
			break;
		}
	}
	if (free_ch == -1) {
		spin_unlock_irqrestore(&ahb_dma_chan_lock, flags);
		return -EBUSY;
	}
	chan = ahb_dma_chan + free_ch;
	fa_ahb_dma_reset(free_ch);
	chan->dev_id = dev_id;
	spin_unlock_irqrestore(&ahb_dma_chan_lock, flags);
	chan->pio_mode = pio_mode;
	if ( pio_mode ==  0) {
		r = request_irq(PLATFORM_AHBDMA_IRQ, fa_ahb_dma_irq_handler, IRQF_SHARED, "AHB_DMA",
				(void *) (free_ch + 1));
		if (r != 0) {
			printk(KERN_ERR "unable to request IRQ %d for AHB DMA (error %d)\n",
			       PLATFORM_AHBDMA_IRQ, r);
			free_irq(PLATFORM_AHBDMA_IRQ, (void *) (free_ch + 1));
			return r;
		}
		chan->callback = callback;
	}
	else {
		chan->callback = NULL;
	}
	chan->dev_name = dev_name;
	chan->data = data;
	*dma_ch_out = free_ch;
	#ifdef DMA_DEBUG
	dma_dbg("Allocated ch=%d ok, dev_name=%s, dev_id=%d(%d)\n", free_ch, dev_name, dev_id,chan->dev_id);
	#endif

	return 0;
}

/**
 *	Request a assigned DMA channel resourece.
 *	@dev_id: Device id. The dev_id must >= 0.
 *	@dev_name: Device name
 *	@call_back: The callback funcition. It will be called afeter DMA interrup is triggered.
 *	@data: The user defined data structure pointer. It will be passed when calling callback function.
 *	@dma_ch: The assigned AHB DMA channel number.
 *	@pio_mode: 1 - Using DMA PIO mode, 0 - Using DMA INT mode (needs pass callback funciton)
 *
 *	Pleaes use this function to allocate a specific APB DMA channel.
 *	0-3 chnnels are GM AHB DMA channels.
 *	Callback function's prototype: void (* callback)(int ch, u16 int_status, void *data)
 *	Here ch is DMA channel number. int_status is DMA status. data is a pointer to user defined structure.
 *
 *	@Returns: zero for successful. Non-zero for erros.
 */
 
int fa_request_ahb_dma_ch(int dev_id, const char *dev_name,
		     void (* callback)(int ch, u16 int_status, void *data),
		     void *data, int dma_ch, int pio_mode)
{
	int r;
	unsigned long flags;
	struct fa_ahb_dma_ch *chan;
	spin_lock_irqsave(&ahb_dma_chan_lock, flags);

#if 0
	if ( (ahb_dma_chan[dma_ch].dev_id != -1) || ((dma_ch%4) == 3) ) {
		//Cannel 3,7 is internal used
#endif
	if ( (ahb_dma_chan[dma_ch].dev_id != -1)) {
		spin_unlock_irqrestore(&ahb_dma_chan_lock, flags);
		printk(KERN_ERR "Channel %d is not available\n",dma_ch);
		return -EBUSY;
	}
	chan = ahb_dma_chan + dma_ch;
	fa_ahb_dma_reset(dma_ch);
	chan->dev_id = dev_id;
	spin_unlock_irqrestore(&ahb_dma_chan_lock, flags);
	chan->pio_mode = pio_mode;
	if ( pio_mode ==  0) {
		printk(KERN_DEBUG "AHB DMA Interrupt Mode\n");
		r = request_irq(PLATFORM_AHBDMA_IRQ, fa_ahb_dma_irq_handler, IRQF_SHARED, "AHB_DMA",
				(void *) (dma_ch + 1));
		if (r != 0) {
			printk(KERN_ERR "unable to request IRQ %d for AHB DMA (error %d)\n",
			       PLATFORM_AHBDMA_IRQ, r);
			free_irq(PLATFORM_AHBDMA_IRQ, (void *) (dma_ch + 1));
			return r;
		}
		chan->callback = callback;
	}
	else {
		printk(KERN_DEBUG "AHB DMA POLLING Mode\n");
		chan->callback = NULL;
	}
	chan->dev_name = dev_name;
	chan->data = data;
	#ifdef DMA_DEBUG
	dma_dbg("Allocated ch=%d ok, dev_name=%s, dev_id=%d(%d)\n", dma_ch, dev_name, dev_id, chan->dev_id);
	#endif

	return 0;
}

/**
 *	Release a allocated DMA channel resourece.
 *	@dma_ch: The APB DMA channel number want to be released.
 *
 *	Pleaes use this function to release a allocated APB DMA channel.
 */

void fa_free_ahb_dma(int ch)
{
	unsigned long flags;
	int size;

	fa_ahb_dma_stop(ch);
	fa_ahb_dma_reset(ch);
	//Platform dependent
	fa_reset_ahb_pmu(ch, ahb_dma_chan[ch].req_pair);

	spin_lock_irqsave(&ahb_dma_chan_lock, flags);
	if (ahb_dma_chan[ch].dev_id == -1) {
		printk(KERN_ERR "fa_dma: trying to free nonallocated DMA channel %d\n", ch);
		spin_unlock_irqrestore(&ahb_dma_chan_lock, flags);
		return;
	}
	ahb_dma_chan[ch].dev_id = -1;
	if ( ahb_dma_chan[ch].pio_mode == 0 )
		free_irq(PLATFORM_AHBDMA_IRQ, (void *) (ch + 1));
	if ( ahb_dma_chan[ch].priv.allocated_dma_lld ) {
		size=sizeof(ahb_lld_t)*(ahb_dma_chan[ch].priv.llp_count);
		if(ahb_dma_chan[ch].priv.ahb_dma_lld)
		{
			dma_dbg("free 0x%p with 0x%p size 0x%x\n",ahb_dma_chan[ch].priv.ahb_dma_lld,ahb_dma_chan[ch].priv.ahb_dma_lld_phys,size);
			dma_free_coherent(NULL, size, (void *)(ahb_dma_chan[ch].priv.ahb_dma_lld),(dma_addr_t)(ahb_dma_chan[ch].priv.ahb_dma_lld_phys));
			ahb_dma_chan[ch].priv.allocated_dma_lld = 0;
		}
	}
	spin_unlock_irqrestore(&ahb_dma_chan_lock, flags);
#if 0 //Move the code before free_irq
	fa_ahb_dma_stop(ch);
	fa_ahb_dma_reset(ch);
	//Platform dependent
	fa_reset_ahb_pmu(ch, ahb_dma_chan[ch].req_pair);
#endif
	#ifdef DMA_DEBUG
	dma_dbg("Free ch=%d Ok\n", ch);
	#endif
}

void fa_set_ahb_llp_address (u32 ch, u32 start_addr, u32 end_addr, u32 dir, u32 period)
{
    u32 psize, tsize, taddr, Max_DMA_SZ;
    int addr_index, llp_count;

    /* init */
    Max_DMA_SZ = fa_llp_size(ahb_dma_chan[ch].parm.bs, ahb_dma_chan[ch].parm.sw);
    llp_count = 0;

    for (addr_index=start_addr; addr_index<end_addr; addr_index+=period)
    {
        psize = period;
        taddr = addr_index;
        
        while (psize)
        {
            if (unlikely(psize <= Max_DMA_SZ))
	            tsize = psize;
            else
	            tsize = Max_DMA_SZ;
	        
	        if (dir == DMA_TO_DEVICE)
	            ahb_dma_chan[ch].priv.ahb_dma_lld[llp_count++].source = taddr;
	        else
    	        ahb_dma_chan[ch].priv.ahb_dma_lld[llp_count++].dest = taddr;

            taddr += tsize;
            psize -= tsize;
        }
    }
}

/*
 * Reconfigure DMA LLP to head of llp list
 */
int fa_ahb_dma_set_llp(int ch, unsigned int llp_idx)
{
    volatile ahb_lld_t  *lld;
    volatile ahb_lld_t  *lld_phys;
    
    if (ch >= FA_AHB_DMA_CHANNEL_MAX)
    {
        printk("Error in fa_ahb_dma_set_llp() .. \r\n");
        return -1;
    }
    if (llp_idx >= ahb_dma_chan[ch].priv.llp_count)
        return -2;
         
    lld      = ahb_dma_chan[ch].priv.ahb_dma_lld;
	lld_phys = ahb_dma_chan[ch].priv.ahb_dma_lld_phys;

    outl(ahb_dma_chan[ch].parm.size, ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_TXSZ_0x14);
	outl(lld[llp_idx].source, ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_SRC_0x8);
	outl(lld[llp_idx].dest, ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_DST_0xC);	

	if (llp_idx != 0)
	    outl(lld[llp_idx-1].llp, ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_LLP_0x10);
	else
	    outl(lld[ahb_dma_chan[ch].priv.llp_count-1].llp, ahb_dma_chan[ch].priv.channel_base + AHBDMA_CHANNEL_LLP_0x10);

    fa_ahb_dma_clear_int(ch);
            
    return 0;
}

/*
 * Configure channel priority
 */
int fa_ahb_dma_priority_set(int ch, unsigned int priority)
{
    if (ch >= FA_AHB_DMA_CHANNEL_MAX)    
        return -1;
    
    if (priority > 3)
        return -2;
    
    ahb_dma_chan[ch].parm.chpri = priority;
    
    return 0;        
}

static int __init fa_init_ahb_dma(void)
{
	int ch, i;

#ifdef MODULE
	printk(KERN_INFO "GM AHB DMA Driver Module Init");
#else
	dma_dbg("fa_init_ahb_dma\n");
#endif
	spin_lock_init(&ahb_dma_chan_lock);
	memset(&ahb_dma_chan, 0, sizeof(ahb_dma_chan));

	fa_ahb_dma_hw_resource_init();

	for (i=0;i<IP_COUNT;i++) {
		if (!request_region(IP_va_base[i], SZ_1K, "AHB DMAC")) {
			printk(KERN_ERR "Cannot reserve APB DMAC I/O port region\n");
			return -EBUSY;
		}
	}
	for (ch=0;ch<FA_AHB_DMA_CHANNEL_MAX;ch++) {
		ahb_dma_chan[ch].dev_id = -1;
		fa_ahb_dma_reset(ch);
		//dma_dbg("ahb_dma_chan[%d].priv.base=%08X\n",ch,ahb_dma_chan[ch].priv.base);
	}

	printk(KERN_INFO "Attach GM AHB-DMA Driver\n");
	return 0;
}

void __exit fa_cleanup_ahb_dma(void)
{
	int ch,i;

#ifdef MODULE
	printk(KERN_INFO "GM AHB DMA driver Clean-up");
#endif

	for (ch=0;ch<FA_AHB_DMA_CHANNEL_MAX;ch++) {
		ahb_dma_chan[ch].dev_id = -1;
		fa_ahb_dma_reset(ch);
	}

	for (i=0;i<IP_COUNT;i++) {
		release_region(IP_va_base[i], SZ_1K);
	}
}

#ifndef MODULE
arch_initcall(fa_init_ahb_dma);
#else
module_init(fa_init_ahb_dma);
module_exit(fa_cleanup_ahb_dma);
#endif


EXPORT_SYMBOL(fa_ahb_dma_init);
EXPORT_SYMBOL(fa_ahb_dma_add);
EXPORT_SYMBOL(fa_ahb_dma_add_auto);
EXPORT_SYMBOL(fa_ahb_dma_get_status);
//EXPORT_SYMBOL(fa_set_ahb_dma_params);
//EXPORT_SYMBOL(fa_set_ahb_dma_ch_params);
EXPORT_SYMBOL(fa_set_ahb_dma_transfer_params);
EXPORT_SYMBOL(fa_set_ahb_dma_transfer_params2);
EXPORT_SYMBOL(fa_set_ahb_dma_dst_params);
EXPORT_SYMBOL(fa_set_ahb_dma_src_params);
EXPORT_SYMBOL(fa_set_ahb_dma_cfg_params);
EXPORT_SYMBOL(fa_get_ahb_dma_params);
EXPORT_SYMBOL(fa_ahb_dma_start);
EXPORT_SYMBOL(fa_ahb_dma_stop);
EXPORT_SYMBOL(fa_ahb_dma_clear_int);
EXPORT_SYMBOL(fa_set_ahb_dma_usual_params);
EXPORT_SYMBOL(fa_ahb_dma_reset);
EXPORT_SYMBOL(fa_request_ahb_dma_auto);
EXPORT_SYMBOL(fa_request_ahb_dma_ch);
EXPORT_SYMBOL(fa_free_ahb_dma);
EXPORT_SYMBOL(fa_ahb_dma_get_irq);
EXPORT_SYMBOL(fa_ahb_dma_get_src_addr);
EXPORT_SYMBOL(fa_ahb_dma_get_dest_addr);
EXPORT_SYMBOL(fa_ahb_dma_clear_llp_idx);
EXPORT_SYMBOL(fa_ahb_dma_reset2);
EXPORT_SYMBOL(fa_ahb_free_llp_mem);
EXPORT_SYMBOL(fa_ahb_dma_resume);
EXPORT_SYMBOL(fa_ahb_dma_pause);
EXPORT_SYMBOL(fa_ahb_dma_cloop_llp_init);
EXPORT_SYMBOL(fa_ahb_set_dma_sg);
EXPORT_SYMBOL(fa_set_ahb_dma_transfer_bs);
EXPORT_SYMBOL(fa_ahb_dma_all_done);
EXPORT_SYMBOL(fa_set_ahb_llp_address);
EXPORT_SYMBOL(fa_ahb_dma_set_llp);
EXPORT_SYMBOL(fa_ahb_dma_priority_set);

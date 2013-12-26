/*******************************************************************************
 * Module name: fotg2xx_udc.c
 *
 * Copyright 2009 GM for OTG function 
 * All Rights Reserved.
 *
 * The information contained herein is confidential property of Company.
 * The user, copying, transfer or disclosure of such information is 
 * prohibited except by express written agreement with Company.
 *
 * Module Description:
 *  This OTG dirver for GM FOTG200 controller
 *  
 ******************************************************************************/
#undef DEBUG

#ifdef CONFIG_USB_DEBUG
#define DEBUG
#else
#undef DEBUG
#endif

#include <linux/module.h>
#include <linux/pci.h>
#include <linux/dmapool.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/reboot.h>
#include <linux/usb.h>
#include <linux/usb/otg.h>
#include <linux/moduleparam.h>
#include <linux/dma-mapping.h>
#include <linux/kallsyms.h>

#include "../core/hcd.h"
#include "../core/usb.h"
#include "fotg2xx-config.h"
#include "fotg2xx.h"
#include "fotg2xx-ehci-macro.h"
#include "fotg2xx_opt-macro.h"
#include "../gadget/fotg2xx-peri-macro.h"

#include <asm/byteorder.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/unaligned.h>

/* PMU register data */
static int usb_pmu_fd = -1;
int usb_get_pmu_fd(void)
{
	return usb_pmu_fd;
}
EXPORT_SYMBOL(usb_get_pmu_fd);
/* Register the reg_off and bit_masks that will be used in the future */
static pmuReg_t regUSBArray[] = {
	/* reg_off,bit_masks,lock_bits,init_val,init_mask */
#if defined(CONFIG_PLATFORM_GM8180) || defined(CONFIG_PLATFORM_GM8185_v2)
	{0x38, BIT28, BIT28, 0, BIT28},
#elif defined(CONFIG_PLATFORM_GM8181)
	{0x38, BIT16, BIT16, 0, BIT16},
	{0x6C, BIT11|BIT8|BIT9|((BIT0|BIT1|BIT2)<<20)|BIT28|BIT29|BIT30, BIT11|BIT8|BIT9|((BIT0|BIT1|BIT2)<<20)|BIT28|BIT29|BIT30, 0, BIT11|BIT8|BIT9|((BIT0|BIT1|BIT2)<<20)|BIT28|BIT29|BIT30},
#elif defined(CONFIG_PLATFORM_GM8126)
	{0x38, BIT16, BIT16, 0, BIT16},
	{0xA0, ((BIT0|BIT1|BIT2)<<17)|BIT0|BIT3|BIT8|BIT9|BIT11, ((BIT0|BIT1|BIT2)<<17)|BIT0|BIT3|BIT8|BIT9|BIT11, 0, ((BIT0|BIT1|BIT2)<<17)|BIT0|BIT3|BIT8|BIT9|BIT11},
#endif
};
static pmuRegInfo_t usb_clk_info = {
	"usb_clk",
	ARRAY_SIZE(regUSBArray),
	ATTR_TYPE_NONE,             /* no clock source */
	regUSBArray
};

static int is_init_fotg2xx_dev = 0;

static struct otg_transceiver fotg2xx_transc =
{
	.label = "fotg2xx_transc",
	.default_a = 1,
	.state = OTG_STATE_UNDEFINED,
};

#define hcd_to_ehci(hcd) ((struct ehci_hcd *)(hcd->hcd_priv))

#define a_host_to_b_device() { \
	mwOTG20_stop_host(); \
	mdwOTGC_Control_A_BUS_REQ_Clr(); \
	mdwOTGC_Control_A_BUS_DROP_Set(); \
	mdwPort(FOTG200_REGBASE,0x130) &= ~(BIT0|BIT1|BIT2); \
	mUsbGlobIntEnSet(); \
	mUsbUnPLGClr(); \
} 

#define b_device_to_a_host() { \
	mUsbUnPLGSet(); \
	mUsbGlobIntDis(); \
	mdwPort(FOTG200_REGBASE,0x130) |= (BIT0|BIT1|BIT2); \
	mdwOTGC_Control_A_BUS_DROP_Clr() ; \
	mdwOTGC_Control_A_BUS_REQ_Set(); \
	mwOTG20_start_host(); \
}

#if 0 /*move to ehci_irq in fotg2xx-ehci-hcd.c*/
#ifndef CONFIG_GM_FOTG2XX_NOHOST
static char fotg2xx_desc[]="FOTG2XX_Controller";
#endif
#endif

#define QUEUE_LEN 64

u32 fotg2xx_wCurrentInterruptMask;
static int last_fotg2xx_id;
static int last_fotg2xx_role;
static u32 queue[QUEUE_LEN];
static u32 queue_in=0;
static u32 queue_out=0;
static int is_fotg2xx_init = 0;

//=============================================================================
// OTG_RoleChange()
// Description:This function will take care the event about role change.
//             It will close/init some function.
//      
// input:(u8)bRole
//       0 => Change to Host
//       1 => Change to Peripheral
// output: none
//=============================================================================
void OTG_RoleChange(u8 bRole, struct otg_transceiver *xceiver)
{
	printk("Warning OTG_RoleChange not performed yet!!\n");
}

//============================================================================= 
// OTGC_Init()
// Description:1.Init the OTG Structure Variable
//             2.Init the Interrupt register(OTG-Controller layer)
//             3.Call the OTG_RoleChange function to init the Host/Peripheral
// input: none
// output: none
//=============================================================================
void OTGC_Init(struct otg_transceiver *xceiver)
{
	u32 dwTemp;

	//Clear the interrupt status
	dwTemp=mdwOTGC_INT_STS_Rd();
	mdwOTGC_INT_STS_Clr(dwTemp);

#if defined(CONFIG_PLATFORM_GM8126)
	//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0xA0) &= ~BIT3; // set to host mode
	ftpmu010_write_reg(usb_pmu_fd, 0xA0, 0, BIT3);
#endif

	//<1>.Read the ID 
	if (mdwOTGC_Control_ID_Rd() != OTG_ID_A_TYPE){
		//Change to B Type
		//<1.1>.Init Variable
		/*OTG.A_bASRPDET=0;
		  OTG.B_bBSRPDN=0;
		  OTG.CurrentRole=USB_DEVICE_ROLE;*/
		fotg2xx_wCurrentInterruptMask=OTGC_INT_B_TYPE;
		//<1.2>.Init Interrupt
		mdwOTGC_INT_Enable_Clr(OTGC_INT_A_TYPE);
		mdwOTGC_INT_Enable_Set(OTGC_INT_B_TYPE);
		//<1.3>.Init the Peripheral
		// OTG_RoleChange(USB_DEVICE_ROLE,xceiver);
		last_fotg2xx_id = OTG_ID_B_TYPE;
		last_fotg2xx_role =  (OTG_ROLE_HOST | OTG_ROLE_DEVICE) +1;
	}
	else {
		//Changfe to A Type
		//<2.1>.Init Variable
		/*OTG.A_bASRPDET=0;
		  OTG.B_bBSRPDN=0;
		  OTG.CurrentRole=USB_HOST_ROLE;*/
		fotg2xx_wCurrentInterruptMask=OTGC_INT_A_TYPE;
		//<2.2>. Init Interrupt
		mdwOTGC_INT_Enable_Clr(OTGC_INT_B_TYPE);
		mdwOTGC_INT_Enable_Set(OTGC_INT_A_TYPE);                  
		//<2.3>.Init the Host
		// OTG_RoleChange(USB_HOST_ROLE,xceiver);
		last_fotg2xx_id = OTG_ID_A_TYPE;
		last_fotg2xx_role =  (OTG_ROLE_HOST | OTG_ROLE_DEVICE) +1;
	}
	printk("fotg200 int enable = %x\n", mdwFOTGPort(0x08)); 
}

static void Enforce_OTG_to_IDLE(void)
{
	u32 wINTStatus;

	mdwFOTGPort(0x00) = 0; // Clear all value
	wINTStatus = mdwOTGC_INT_STS_Rd(); 
	mdwOTGC_INT_STS_Clr(wINTStatus); 

	if (mdwOTGC_Control_ID_Rd() == OTG_ID_A_TYPE){
		mwOTG20_stop_host();
		mdwOTGC_Control_A_BUS_DROP_Set();
		mdwOTGC_Control_A_BUS_REQ_Clr();
		while(mwOTG20_b_conn_Rd() || mdwOTGC_Control_A_SESS_VLD_Rd() ) 
			udelay(1);
		fotg2xx_transc.state = OTG_STATE_A_IDLE;
		printk("OTG ID A enter idle status b_conn:%x a_sess_vld:%x\n",mwOTG20_b_conn_Rd(),mdwOTGC_Control_A_SESS_VLD_Rd()); 
	}
	else {
		mUsbUnPLGSet();
		mUsbGlobIntDis();
		mdwOTGC_Control_A_BUS_DROP_Set();
		while(mdwOTGC_Control_B_SESS_VLD_Rd()) 
			udelay(1);	
		fotg2xx_transc.state = OTG_STATE_B_IDLE;
		printk("OTG ID B enter idle status b_sess_vld:%x ...\n",mdwOTGC_Control_B_SESS_VLD_Rd());
	}
}

static void Enable_A_SRP_Detection(void)
{
	u32 cnt;
	mdwOTGC_Control_A_BUS_DROP_Clr();
	mdwOTGC_Control_A_SRP_RESP_TYPE_Set();
	mdwOTGC_Control_A_SRP_DET_EN_Set();

	// Wait for SRP signal in 10 second
	for(cnt=0;cnt<1000000 && !(mdwOTGC_INT_STS_Rd()&OTGC_INT_ASRPDET);cnt++)
		udelay(10);

	if (mdwOTGC_INT_STS_Rd() & OTGC_INT_ASRPDET){
		mdwOTGC_INT_STS_Clr(OTGC_INT_ASRPDET);
		mdwOTGC_Control_A_SET_B_HNP_EN_Set();
	}
	else {
		fotg2xx_dbg("No SRP signal is detected %x\n",0);
		mUsbUnPLGSet(); 
		mUsbGlobIntDis(); 
		mdwPort(FOTG200_REGBASE,0x130) |= (BIT0|BIT1|BIT2); 
		mdwOTGC_Control_A_SET_B_HNP_EN_Clr();
	}
	mdwOTGC_Control_A_SRP_DET_EN_Clr();
	mdwOTGC_Control_A_BUS_REQ_Set();
	mwOTG20_start_host();
	while(mdwOTGC_Control_A_VBUS_VLD_Rd()) 
		udelay(1);
	fotg2xx_transc.state = OTG_STATE_A_WAIT_BCON;
	while(mwOTG20_b_conn_Rd()) 
		udelay(1);	
	fotg2xx_transc.state = OTG_STATE_A_HOST;	
}

static void Assert_B_SRP_Signal(void)
{
	int cnt; 

	mUsbUnPLGClr();
	for(cnt=0;cnt<1000000 && !mdwOTGC_Control_B_SESS_END_Rd();cnt++)
		udelay(10);
	if (cnt == 1000000){
		printk("B Session not arrive end ...\n");
		return;
	}

	fotg2xx_transc.state = OTG_STATE_B_SRP_INIT;
	mdwOTGC_Control_B_BUS_REQ_Set();
	for(cnt=0;cnt<1000000 && !(mdwOTGC_INT_STS_Rd()&OTGC_INT_BSRPDN);cnt++)
		udelay(10);
	if(cnt == 1000000){
		printk("B SRP Done timeout  ...\n");
		return;
	}

	for(cnt=0;cnt<1000000 && !(mdwOTGC_Control_B_SESS_VLD_Rd());cnt++)
		udelay(10);
	if(cnt == 1000000){
		printk("SRP Failed --- A-Device No Respond\n");
		printk("Device A does not drive Vbus to VB_SESS_VLD\n");
		return;
	}

	mdwOTGC_INT_STS_Clr(OTGC_INT_BSRPDN);
	// At the first B is peripheral 
	fotg2xx_transc.host->is_b_host = 0;
	printk("B have sent SRP signal...\n");
	mwOTG20_stop_host(); 
	mdwPort(FOTG200_REGBASE,0x130) &= ~(BIT0|BIT1|BIT2); 
	mUsbGlobIntEnSet(); 
	mUsbUnPLGClr(); 
	fotg2xx_transc.state = OTG_STATE_B_PERIPHERAL;
}

void Execute_OTG_Procedure(void)
{
	struct usb_bus *bus = fotg2xx_transc.host; 
	struct usb_hcd *hcd = bus_to_hcd(bus);  //(struct usb_hcd *)bus->hcpriv; 

	Enforce_OTG_to_IDLE();
	if (mdwOTGC_Control_ID_Rd() == OTG_ID_A_TYPE) {
		hcd->driver->stop(hcd);
		Enable_A_SRP_Detection();
		hcd->driver->reset(hcd);  // add for 2.6.21
		hcd->driver->start(hcd);
	}
	else {
		hcd->driver->stop(hcd);
		hcd->driver->reset(hcd);  // add for 2.6.21
		hcd->driver->start(hcd);
		hcd->state = HC_STATE_HALT;
		mwOTG20_stop_host(); 
		printk("Execute_OTG_Procedure call Assert_B_SRP_Signal \n"); 
		Assert_B_SRP_Signal();
		mwOTG20_reset_Set();
	}
	mdwOTGC_INT_Enable_RLCHG_Set();
	fotg2xx_transc.host->otg_port = 1;
}

void Reset_OTG_Host_Controller(void)
{
	struct usb_bus *bus = fotg2xx_transc.host; 
	struct usb_hcd *hcd = bus_to_hcd(bus);  //(struct usb_hcd *)bus->hcpriv;
	hcd->driver->stop(hcd);
	if(mdwOTGC_Control_ID_Rd() == OTG_ID_A_TYPE){
		Enforce_OTG_to_IDLE();
		b_device_to_a_host();
	}
	mdwOTGC_Control_A_SET_B_HNP_EN_Clr();
	mdwOTGC_Control_A_SRP_DET_EN_Clr();
	hcd->driver->reset(hcd);  // add for 2.6.21
	hcd->driver->start(hcd);
	fotg2xx_transc.host->otg_port = 0;
}

static irqreturn_t OTGC_INT_ISR(u32 wINTStatus)
{
	//printk("Warning: OTGC_INT_ISR Not Implement\n");
	printk("OTGC_INT_ISR: re-drive Vbus...\n");
	mdwOTGC_Control_A_BUS_REQ_Clr();
	mdwOTGC_Control_A_BUS_DROP_Set();
	mdwOTGC_Control_A_BUS_DROP_Clr();
	mdwOTGC_Control_A_BUS_REQ_Set();
	return IRQ_HANDLED;
}

void OTG_handler(void);
DECLARE_WORK(my_work_queue, (void *) OTG_handler);

#if 0 /*move to ehci_irq in fotg2xx-ehci-hcd.c*/
static irqreturn_t fotg200_irq(int irq, void *_dev)
#endif
/* fotg200_handle_irq is used for replace fotg200_irq*/
void fotg200_handle_irq(void)
{
	u32 wINTStatus;
	u32 temp;


	wINTStatus = mdwOTGC_INT_STS_Rd();
	//fotg2xx_dbg("fotg200_irq %x\n",wINTStatus); 
	//printk("fotg200_irq %x... status = %x, %x,  enable = %x\n",
	//		    wINTStatus,mdwFOTGPort(0x00),mdwFOTGPort(0x04), mdwFOTGPort(0x08)); 
	mdwOTGC_INT_STS_Clr(OTGC_INT_IDCHG);
	mdwOTGC_INT_STS_Clr(wINTStatus); 
	if (wINTStatus) {  // something happen
		queue[queue_in] = wINTStatus;
		//printk("^^^^ %x..%x ----> %x\n", queue_out, queue_in, queue[queue_in]);  
		if (queue_out != queue_in) {
			// more than one work in the queue, we only do the last one by setting work as "0"
			temp = queue_out;
			while (temp != queue_in) {
				queue[temp] = 0;
				temp = (temp + 1) % QUEUE_LEN; 
			}
			queue_out = queue_in;
		}
		queue_in = (queue_in + 1) % QUEUE_LEN;
		schedule_work(&my_work_queue);
	}

#if 0 /*move to ehci_irq in fotg2xx-ehci-hcd.c*/
	return IRQ_HANDLED;
#endif
}
EXPORT_SYMBOL_GPL(fotg200_handle_irq);

void OTG_handler(void)
{
	u32 wINTStatus;
	u32 current_id;
	unsigned long  flags;
	u32 temp;

	if (queue[queue_out] == 0) {  // happen for multiple work 
		printk(" empty OTG work queue %x\n", queue_out);
		//queue_out = (queue_out + 1) % QUEUE_LEN;
		return;
	}

	// in case host mode with connected device
	// we need to wait host HCD has finished job
	msleep(20);

	// for HCD recover unfinished URB, we need more time 
	temp = mdwPort(FOTG200_REGBASE, 0x10);
	while (temp & 0x20) {   // host async still enable
		msleep(100);
		temp = mdwPort(FOTG200_REGBASE, 0x10);
	}

	local_irq_save(flags);

	//printk("+++++++++ %x:%x\n",queue_out,queue[queue_out]); 
	wINTStatus = queue[queue_out];  
	queue_out = (queue_out + 1 ) % QUEUE_LEN;
	current_id = mdwOTGC_Control_ID_Rd();
	
	/* Change ID */
	if ( (wINTStatus & OTGC_INT_IDCHG) && (last_fotg2xx_id != current_id) ){
		struct usb_bus *bus = fotg2xx_transc.host; 
		struct usb_hcd *hcd = bus_to_hcd(bus);  //(struct usb_hcd *)bus->hcpriv; 
		//struct ehci_hcd *ehci = hcd_to_ehci (hcd);
		//struct usb_device *hdev = hcd->self.root_hub;

		if ( current_id == OTG_ID_A_TYPE){
			printk("%s: FOTG2XX is now in Mini-A type %x\n", __func__, 0);
			last_fotg2xx_id = OTG_ID_A_TYPE;
			if (mdwOTGC_Control_CROLE_Rd() == OTG_ROLE_HOST){
				b_device_to_a_host();
				hcd->driver->reset(hcd);  // add for 2.6.21
				hcd->driver->start(hcd);
			}
		}
		else {
			printk("%s: FOTG2XX is now in Mini-B type %x\n", __func__, 0);
			last_fotg2xx_id = OTG_ID_B_TYPE;
			if (mdwOTGC_Control_CROLE_Rd() == OTG_ROLE_DEVICE){
				//hcd->driver->stop(hcd);
				a_host_to_b_device();
			}
		}
		mdwOTGC_INT_STS_Clr(OTGC_INT_IDCHG);
	}

	/* Enable Device's HNP */
	if (fotg2xx_transc.host->b_hnp_enable == 1 &&  !mdwOTGC_Control_A_SET_B_HNP_EN_Rd()){
		//fotg2xx_dbg("fotg200_irq: enable HNP %x\n",0);
		printk("fotg2oo_irq : enable HNP\n");
		mdwOTGC_Control_A_SET_B_HNP_EN_Set();
	}

	/* Role Change */
	if (wINTStatus & OTGC_INT_RLCHG) {
		if ( (mdwOTGC_Control_ID_Rd() == OTG_ID_A_TYPE)) {
			printk(" ID-A OTG Role change... %x\n",wINTStatus);
			if (mdwOTGC_Control_CROLE_Rd() == OTG_ROLE_HOST){
				mUsbUnPLGSet(); 
				mUsbGlobIntDis(); 
				mdwPort(FOTG200_REGBASE,0x130) |= (BIT0|BIT1|BIT2); 
				mwOTG20_start_host();
				fotg2xx_transc.state = OTG_STATE_A_HOST;
			}
			else {
				mwOTG20_stop_host(); 
				mdwOTGC_Control_A_SET_B_HNP_EN_Clr();
				mdwPort(FOTG200_REGBASE,0x130) &= ~(BIT0|BIT1|BIT2); 
				mUsbGlobIntEnSet(); 
				mUsbUnPLGClr(); 
				fotg2xx_transc.state = OTG_STATE_A_PERIPHERAL;
			}
		} 
		else {   // OTG_ID_D_B_TYPE
			struct usb_bus *bus = fotg2xx_transc.host; 
			struct usb_hcd *hcd = bus_to_hcd(bus); //(struct usb_hcd *)bus->hcpriv; 

			printk(" ID-B OTG Role change... %x\n",wINTStatus);
			if (mdwOTGC_Control_CROLE_Rd() == OTG_ROLE_HOST){
				hcd->state = HC_STATE_RUNNING;

				fotg2xx_transc.host->is_b_host = 1;
				mUsbGlobIntDis(); 
				mdwPort(FOTG200_REGBASE,0x130) |= (BIT0|BIT1|BIT2); 
				mwOTG20_start_host();
				mdwOTGC_Control_B_HNP_EN_Clr();
				fotg2xx_transc.state = OTG_STATE_B_HOST;
				fotg2xx_show_hc_regs();
				fotg2xx_show_otg_regs();
				fotg2xx_show_dev_regs(); 
				printk("enter host mode....\n");
			}
			else {
				//hcd->driver->stop(hcd);
				//for(cnt=0;cnt<5000;cnt++) udelay(10);
				fotg2xx_transc.host->is_b_host = 0;
				//mwOTG20_stop_host(); 
				mdwPort(FOTG200_REGBASE,0x130) &= ~(BIT0|BIT1|BIT2); 
				mUsbGlobIntEnSet(); 
				mUsbUnPLGClr(); 
				fotg2xx_transc.state = OTG_STATE_B_PERIPHERAL;

				//fotg2xx_show_hc_regs();
				//fotg2xx_show_otg_regs();
				printk("enter device mode....\n");
#ifndef CONFIG_USB_GADGET
				// John add for in case gadget driver not enabled while using OTG
				// controller will generate interrupt for suspend, we just to ignore it !!
				printk("WARNING: Your OTG controller don't know how to act as USB device\n");
				printk("         Please enable gadget function\n");
				mdwPort(FOTG200_REGBASE,0x134) = 0xFFFFFFFF;
				mdwPort(FOTG200_REGBASE,0x138) = 0xFFFFFFFF;
				mdwPort(FOTG200_REGBASE,0x13C) = 0xFFFFFFFF;
				mUsbUnPLGSet();
#endif 
			}
		}
	}

	//<1>.Checking the OTG layer interrupt status
	if (wINTStatus>0) {
		if (wINTStatus & OTGC_INT_AVBUSERR)
			printk("Error --- Interrupt OTGC_INT_AVBUSERR=1... \n");
		if (wINTStatus & OTGC_INT_OVC)
			printk("Error --- Attached Device Not Supported: Over Current\n");

		mdwOTGC_INT_STS_Clr(wINTStatus); 
		if ((fotg2xx_wCurrentInterruptMask & wINTStatus)>0)
			OTGC_INT_ISR(wINTStatus);

		mdwOTGC_INT_STS_Clr(wINTStatus); 
		//wFOTGPeri_Port(0xC0)|=BIT1;
	}
	local_irq_restore(flags);
	return; 

	/*
	// The following code to handle host & device, we have moved to seperate module   
	if (mdwOTGC_Control_CROLE_Rd() == OTG_ROLE_HOST){
	// Host ISR
	usb_hcd_irq(irq,xceiver,r);
	//wFOTGPeri_Port(0xC0)|=BIT2;
	}
	else{ // OTG_CurrentRole_Peripheral
	printk("Warning FOTG2XX_CurrentRole_Peripheral Not performed yet\n");
	if ( mUsbGlobIntEnRd() > 0 ){
	usb_interrupt_level1_Save = mUsbIntGroupRegRd();
	usb_interrupt_level1_Mask = mUsbIntGroupMaskRd();
	usb_interrupt_level1 = usb_interrupt_level1_Save & ~usb_interrupt_level1_Mask;
	kdbg("Process Peripheral INT 0x%08X",usb_interrupt_level1);
	}
	else{
	//printk("Nothing but int, 0x%08X\n",wFOTGPeri_Port(0xC0));
	wFOTGPeri_Port(0xC0)|=wFOTGPeri_Port(0xC0);
	mdwPort(FOTG200_REGBASE,0x14) |= (mdwPort(FOTG200_REGBASE,0x14)&0x2F);
	}
	}
	 */   
}


int fotg200_otgd_init(struct device *dev,struct usb_bus *host,struct usb_gadget *gadget)
{
#if 0 /*move to ehci_irq in fotg2xx-ehci-hcd.c*/
#ifndef CONFIG_GM_FOTG2XX_NOHOST	
	int retval;
#endif
#endif
	printk("FOTG200 Controller Initialization\n");
	fotg2xx_transc.dev = dev;
	fotg2xx_transc.host = host;
	fotg2xx_transc.gadget = gadget; 
	mwOTG20_Interrupt_OutPut_High_Set();//For A320

#if 0 /*move to ehci_irq in fotg2xx-ehci-hcd.c*/
#ifndef CONFIG_GM_FOTG2XX_NOHOST	
	if ((retval = request_irq(USB_FOTG2XX_IRQ, fotg200_irq, IRQF_SHARED, fotg2xx_desc, &fotg2xx_transc)) != 0) {
		printk("Error ---> fotg200_otgd_init: request_irq() %x\n",retval);
		return retval;
	}
#endif
#endif
	return 0;
}

int fotg200_init(struct device *dev,struct usb_bus *host,struct usb_gadget *gadget)
{
	u32 retval;

	queue_in =0;
	queue_out=0;
	is_fotg2xx_init = 1;

	//Disable Device's Group interrupt
	mdwPort(FOTG200_REGBASE,0x130) |= (BIT0|BIT1|BIT2);

	//Set unplug to force device to detach to PC
	mdwOTGC_UsbUnPLGSet();
	if ( (retval=fotg200_otgd_init(dev,host,gadget)) < 0 )
		return retval;

	/*fotg200_probe_otgc(&fotg200_driver, &otgd,&cpe_ehci_dev);*/
	//Init OTG module's global variables and registers
	OTGC_Init(&fotg2xx_transc);
	//Turn off all OTG interrutp first. It will be turned on when do role change
	mdwOTGC_INT_Enable_Clr(0xFFFFFFFF);
	//OTG.wCurrentInterruptMask = 0x00000000;
	fotg2xx_wCurrentInterruptMask = OTGC_INT_AVBUSERR|OTGC_INT_OVC;
	mdwOTGC_INT_Enable_Set(OTGC_INT_IDCHG | OTGC_INT_AVBUSERR | OTGC_INT_OVC);
	return 0;
}
EXPORT_SYMBOL(fotg200_init);

int is_fotg200_init(void)
{
	return is_fotg2xx_init;
}
EXPORT_SYMBOL(is_fotg200_init);

void ehci_reset_PHY(void)
{
#ifndef REMOVE_COVERHW
#ifdef CONFIG_GM_FOTG2XX
	//Reset OTG PHY
	mdwOTGC_Control_PHY_Reset_Set();
	udelay(1000);//1000 About 0.2ms
	mdwOTGC_Control_PHY_Reset_Clr();
	printk("Reset Phy %x\n",0);
#endif
#ifdef CONFIG_GM_FUSBH200
	//Reset PHY
	mwH20_Control_Phy_Reset_Set();
	udelay(1000);//1000 About 0.2ms
	mwH20_Control_Phy_Reset_Clr();
#endif
#endif
}

void ehci_reset_OTG(void)
{
#ifndef REMOVE_COVERHW
#ifdef CONFIG_GM_FOTG2XX
	//Reset OTG controller
	mdwOTGC_Control_OTG_Reset_Set();
	mdwOTGC_Control_OTG_Reset_Clr();
#endif
#endif
}

#if defined(CONFIG_GM_FOTG2XX) || defined(CONFIG_USB_GADGET_DUALSPEED)
/* Setup FOTG2XX Registers  */
void init_FOTG2XX_Dev(void)
{
	is_init_fotg2xx_dev = 1;
#if defined(CONFIG_PLATFORM_GM8180) || defined(CONFIG_PLATFORM_GM8185_v2)
	//turn on OTG clock
	//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x38) &= ~BIT28;
	ftpmu010_write_reg(usb_pmu_fd, 0x38, 0, BIT28);
#elif defined(CONFIG_PLATFORM_GM8181)
	if ((ft_get_version() & 0xF0) == 0x20) { /* 8181T/8186 */
/*
		* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) &= ~(0x7<<20);
		* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) |= (0x2<<20); // config reference voltage to 120mV
		* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) |= BIT29; // enable VBUS input
		* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) &= ~BIT28; // set to host mode
*/
		ftpmu010_write_reg(usb_pmu_fd, 0x6C, 0, (BIT0|BIT1|BIT2)<<20);
		ftpmu010_write_reg(usb_pmu_fd, 0x6C, BIT1<<20, BIT1<<20);
		ftpmu010_write_reg(usb_pmu_fd, 0x6C, BIT29, BIT29);
		ftpmu010_write_reg(usb_pmu_fd, 0x6C, 0, BIT28);
	}
/*
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x38) &= ~BIT16; // turn on OTG clock
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) |= BIT11; // select clock source, 0:external, 1:internal
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) |= BIT8|BIT9; // bit8 & bit9 must all be one
*/
	ftpmu010_write_reg(usb_pmu_fd, 0x38, 0, BIT16);
	ftpmu010_write_reg(usb_pmu_fd, 0x6C, BIT11, BIT11);
	ftpmu010_write_reg(usb_pmu_fd, 0x6C, BIT8|BIT9, BIT8|BIT9);
	if ((ft_get_version() & 0xF0) == 0x10) {
		//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) |= BIT30; // for 8181 Ver. C
		ftpmu010_write_reg(usb_pmu_fd, 0x6C, BIT30, BIT30);
		printk("FOTG2XX: disable internal pull-up resistor for IDDIG pin\n");
		mwOTG20Bit_Set(0x80,BIT13); // turn off internal pull-up resistor
		while (mwOTG20Port(0x80) & 0x100000)
			;
	}
	mdelay(10); // waiting for PHY clock be stable, while clock source changed from externel to internel, at lease 5ms
	if ((ft_get_version() & 0xF0) == 0x20) { /* 8181T/8186 */
		mwOTG20Bit_Set(0x40,BIT6); // put PHY into suspend mode
		mdelay(10);
		mwOTG20Bit_Clr(0x40,BIT6);
		mdelay(10);
	}
#elif defined(CONFIG_PLATFORM_GM8126)
/*
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0xA0) &= ~(0x7<<17);
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0xA0) |= (0x2<<17); // config reference voltage to 120mV
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0xA0) |= BIT0; // enable VBUS input
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0xA0) &= ~BIT3; // set to host mode
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x38) &= ~BIT16; // turn on OTG clock
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0xA0) |= BIT8; // select clock source, 0:12 MHz, 1:30 MHz
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0xA0) |= BIT9|BIT11; // bit9 & bit11 must all be one
*/
	ftpmu010_write_reg(usb_pmu_fd, 0xA0, 0, (BIT0|BIT1|BIT2)<<17);
	ftpmu010_write_reg(usb_pmu_fd, 0xA0, BIT1<<17, BIT1<<17);
	ftpmu010_write_reg(usb_pmu_fd, 0xA0, BIT0, BIT0);
	ftpmu010_write_reg(usb_pmu_fd, 0xA0, 0, BIT3);
	ftpmu010_write_reg(usb_pmu_fd, 0x38, 0, BIT16);
	ftpmu010_write_reg(usb_pmu_fd, 0xA0, BIT8, BIT8);
	ftpmu010_write_reg(usb_pmu_fd, 0xA0, BIT9|BIT11, BIT9|BIT11);
	mdelay(10); // waiting for PHY clock be stable, while clock source changed from externel to internel, at lease 5ms
	mwOTG20Bit_Set(0x40,BIT6); // put PHY into suspend mode
	mdelay(10);
	mwOTG20Bit_Clr(0x40,BIT6);
	mdelay(10);
#endif
#ifdef CONFIG_OTG
	//Host controller with OTG fucntion ==> Turn on Vbus
	//Disable OTG interrupt
	mbFOTGPort(0x08)=0x0;

	//Drive Vbus for FOTG100 (Default device A role)]
	if ( mdwOTGC_Control_ID_Rd() == OTG_ID_A_TYPE ) {
		u32 temp;
		udelay(1000);
		printk("Enter Device A\n");
		temp = mdwOTGC_INT_STS_Rd();
		printk("temp = %x\n",temp);
		mdwOTGC_INT_STS_Clr(temp);
		mdwOTGC_Control_A_BUS_DROP_Set();
		mdwOTGC_Control_A_BUS_REQ_Clr();
		udelay(1000);
		printk("Drive Vbus because of ID pin shows Device A\n");
		mdwOTGC_Control_A_BUS_DROP_Clr();
		mdwOTGC_Control_A_BUS_REQ_Set();
		udelay(1000);
	}
#endif

#ifdef CONFIG_711MA_PHY
	//Set cover bit to cover 711MA PHY full speed reset issue
	mwOTG20_Control_COVER_FS_PHY_Reset_Set();
#endif
#ifdef CONFIG_PLATFORM_GM8180
	//Disable device idle interrupt, only enable for role change
	//These interrupt conditions could be ignored
	mUsbIntDevIdleDis();
	mUsbIntSuspDis();
#endif
#ifndef CONFIG_PLATFORM_GM8120
	//Set OTG200 interrupt to high active
	mwOTG20_Interrupt_OutPut_High_Set();  //For A320
#endif
	//Basic initialization for FPFA version IP
	mwPeri20_Control_ChipEnable_Set();
	//Important: If AHB clock is >=15Mhz and <= 30MHz, please remark this line (Enable half speed)).
	//IF > 30Hz, Disable half speed
#ifdef EnableHalfSpeed
	mwPeri20_Control_HALFSPEEDEnable_Set();
#else /* EnableHalfSpeed */
	mwPeri20_Control_HALFSPEEDEnable_Clr();
#endif /* EnableHalfSpeed */

	mwOTG20_Control_ForceHighSpeed_Clr();
	mwOTG20_Control_ForceFullSpeed_Clr();
#if 0 // debug for force otg phy speed
	mwOTG20_Control_ForceFullSpeed_Set(); // force to full-speed (do not ack to device's KJ pattern)
	//mwOTG20_Control_ForceHighSpeed_Set(); // force to high-speed
	if (mwOTG20Bit_Rd(0x80, BIT12))
		printk("!!!! Force Phy to Full-Speed !!!!\n");
	else if (mwOTG20Bit_Rd(0x80, BIT14))
		printk("!!!! Force Phy to High-Speed !!!!\n");
#endif
}
EXPORT_SYMBOL(init_FOTG2XX_Dev);

int is_init_FOTG2XX_Dev(void)
{
	return is_init_fotg2xx_dev;
}
EXPORT_SYMBOL(is_init_FOTG2XX_Dev);
#endif

static int __init ftusb_init(void)
{
	if ((usb_pmu_fd = ftpmu010_register_reg(&usb_clk_info)) < 0)
		printk(KERN_ERR "%s: Register USB to PMU failed\n", __func__);
	return usb_pmu_fd;
}
static void __exit ftusb_exit(void)
{
	if (ftpmu010_deregister_reg(usb_pmu_fd) < 0)
		printk(KERN_ERR "%s: Unregister USB from PMU Failed\n", __func__);
}
module_init(ftusb_init);
module_exit(ftusb_exit);

MODULE_AUTHOR("<GM-tech.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OTG change mode driver");

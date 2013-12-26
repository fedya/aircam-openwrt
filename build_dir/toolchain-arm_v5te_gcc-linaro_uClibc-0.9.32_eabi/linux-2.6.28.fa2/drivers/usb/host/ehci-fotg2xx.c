/*
 * (C) Copyright David Brownell 2000-2002
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef CONFIG_USB_DEBUG
#define DEBUG
#else
#undef DEBUG
#endif

#include <linux/module.h>
//#include <linux/pci.h>
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
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/moduleparam.h>
#include <linux/dma-mapping.h>
#include <linux/kallsyms.h>

#include "fotg2xx-config.h"
#include "fotg2xx.h"
#include "fotg2xx-ehci-macro.h"
#include "fotg2xx_opt-macro.h"

#ifdef CONFIG_GM_FOTG2XX
static  struct usb_hcd  *fotg2xx_hcd;
#endif

#ifdef CONFIG_GM_FUSBH200
static  struct usb_hcd  *fusbh200_hcd;
#endif
static u32 dma_mask = 0xffffffff;
/*
 * hc states include: unknown, halted, ready, running
 * transitional states are messy just now
 * trying to avoid "running" unless urbs are active
 * a "ready" hc can be finishing prefetched work
 */


static int FTC_ehci_init(struct usb_hcd *hcd);

struct hc_driver ehci_driver = {
	.description =		hcd_name,
	.product_desc =		"GM  USB2.0 Host Controller",
	.hcd_priv_size =	sizeof(struct ehci_hcd),

	/*
	 * generic hardware linkage
	 */
	.irq =			ehci_irq,
	.flags =		HCD_MEMORY | HCD_USB2,

	/*
	 * basic lifecycle operations
	 */
	.reset =		FTC_ehci_init,
	.start =		ehci_run,
	.stop =			ehci_stop,
	.shutdown =     ehci_shutdown,

	/*
	 * managing i/o requests and associated device resources
	 */
	.urb_enqueue =		ehci_urb_enqueue,
	.urb_dequeue =		ehci_urb_dequeue,
	.endpoint_disable =	ehci_endpoint_disable,

	/*
	 * scheduling support
	 */
	.get_frame_number =	ehci_get_frame,

	/*
	 * root hub support
	 */
	.hub_status_data =	ehci_hub_status_data,
	.hub_control =		ehci_hub_control,
#if defined(CONFIG_PM)
	.bus_suspend =		ehci_bus_suspend,
	.bus_resume =		ehci_bus_resume,
#endif
	.relinquish_port = ehci_relinquish_port,
	.port_handed_over = ehci_port_handed_over,
};

/* FOTG2XX Controll Registers Setup When Interrupt  occured  */
void fotg_ehci_irq_macro(void)
{
#ifdef CONFIG_GM_FOTG2XX
	if ( mwOTG20_Interrupt_Status_HOST_Rd()){
#else
	if (1){
#endif

#ifdef CONFIG_GM_FOTG2XX
		mwOTG20_Interrupt_Status_HOST_Clr();
#endif
	}
}

/* FUSBH200 hw initialization */
#ifdef CONFIG_GM_FUSBH200
static void init_FUSBH200_Dev(void)
{
#ifdef CONFIG_PLATFORM_GM8185_v2
	//turn on USBH clock
	//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x38) &= ~BIT2;
	ftpmu010_write_reg(usb_get_pmu_fd(), 0x38, 0, BIT2);
#elif defined(CONFIG_PLATFORM_GM8181)
	if ((ft_get_version() & 0xF0) == 0x20) { /* 8181T/8186 */
		//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) &= ~(0x7<<23);
		//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) |= (0x2<<23); // config reference voltage to 120mV
		ftpmu010_write_reg(usb_get_pmu_fd(), 0x6C, 0, (BIT0|BIT1|BIT2)<<23);
		ftpmu010_write_reg(usb_get_pmu_fd(), 0x6C, BIT1<<23, BIT1<<23);
	}
/*
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x38) &= ~BIT17; // turn on USBH clock
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) |= BIT11; // select clock source, 0:external, 1:internal
	* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) |= BIT8|BIT9; // bit8 & bit9 must all be one
*/
	ftpmu010_write_reg(usb_get_pmu_fd(), 0x38, 0, BIT17);
	ftpmu010_write_reg(usb_get_pmu_fd(), 0x6C, BIT11, BIT11);
	ftpmu010_write_reg(usb_get_pmu_fd(), 0x6C, BIT8|BIT9, BIT8|BIT9);
	if ((ft_get_version() & 0xF0) == 0x10) {
		//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) |= BIT30; // for 8181 Ver. C
		ftpmu010_write_reg(usb_get_pmu_fd(), 0x6C, BIT30, BIT30);
	}
	mdelay(10); // waiting for PHY clock be stable, while clock source changed from externel to internel, at lease 5ms
	if ((ft_get_version() & 0xF0) == 0x20) { /* 8181T/8186 */
		unsigned int tmp;

		//tmp = * (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x80);
		tmp = ftpmu010_read_reg(0x80);
		tmp &= ~0x1F0;
		tmp |= 0x3 << 6; /// IREF_ST[2:0], default:0x4
		tmp |= 0x1 << 4; /// PCR[1:0], default:0x1
		//printk(">>>> tmp = %#x\n", tmp);
		//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x80) = tmp;
		ftpmu010_write_reg(usb_get_pmu_fd(), 0x80, tmp, tmp);

		mwH20Bit_Clr(0x34,BIT6); // place PHY in suspend mode
		mdelay(10);
		mwH20Bit_Set(0x34,BIT6);
		mdelay(10);
	}
#endif
#ifdef CONFIG_711MA_PHY
	//Set cover bit to cover 711MA PHY full speed reset issue
	mwH20_Control_COVER_FS_PHY_Reset_Set();
#endif
	//Set OTG200 interrupt to high active
	mwH20_Interrupt_OutPut_High_Set();  //For A320
	//Important: If AHB clock is >=15Mhz and <= 30MHz, please remark this line (Enable half speed)).
	//IF > 30Hz, Disable half speed
	mwH20_Control_VBUS_OFF_Clr();
#ifdef EnableHalfSpeed
	mwH20_Control_HALFSPEEDEnable_Set();
#else /* EnableHalfSpeed */
	mwH20_Control_HALFSPEEDEnable_Clr();
#endif /* EnableHalfSpeed */

	mwH20_Control_ForceHighSpeed_Clr();
	mwH20_Control_ForceFullSpeed_Clr();
#if 0 // debug for force usbh phy speed
	mwH20_Control_ForceFullSpeed_Set(); // force to full-speed (do not ack to device's KJ pattern)
	//mwH20_Control_ForceHighSpeed_Set(); // force to high-speed
	if (mwHost20Bit_Rd(0x40, BIT7))
		printk("!!!! Force Phy to Full-Speed !!!!\n");
	else if (mwHost20Bit_Rd(0x40, BIT6))
		printk("!!!! Force Phy to High-Speed !!!!\n");
#endif
	mwH20_Int_BM_OVC_En();
	mwH20_Int_BM_VBUS_ERR_En();
}
#endif

static int FTC_ehci_init(struct usb_hcd *hcd)
{
	int result;
	struct ehci_hcd *ehci = hcd_to_ehci(hcd);

	fotg2xx_dbg("call FTC_ehci_init.......ehci = %x\n", (u32)ehci);
	ehci->big_endian_mmio = 0;

	ehci->caps = hcd->regs;
	ehci->regs = hcd->regs + HC_LENGTH(ehci_readl(ehci, &ehci->caps->hc_capbase));

	dbg_hcs_params(ehci, "reset");
	dbg_hcc_params(ehci, "reset");

	/* cache this readonly data; minimize chip reads */
	ehci->hcs_params = ehci_readl(ehci, &ehci->caps->hcs_params);

	result = ehci_halt(ehci);
	if (result)
		return result;

	/* data structure init */
	result = ehci_init(hcd);
	if (result)
		return result;

	hcd->has_tt=1; //tt means transaction-translator

	ehci->sbrn = 0x20;

	ehci_reset(ehci);

	ehci_port_power(ehci, 0);

	return result;
}

/*-------------------------------------------------------------------------*/
/* configure so an HC device and id are always provided */
/* always called with process context; sleeping is OK */

/**
 * usb_hcd_fusbh200_probe - initialize HCDs
 * @dev: USB Host Controller being probed
 * Context: !in_interrupt()
 *
 * Allocates basic PCI resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 *
 * Store this function in the HCD's struct pci_driver as probe().
 */
#ifdef CONFIG_GM_FUSBH200
int usb_hcd_fusbh200_probe (struct device *dev)
{
	struct hc_driver	*driver;
	struct usb_hcd		*hcd;
	int			retval;

	fotg2xx_dbg("%s: %x \n", __func__, (u32)dev);

	if (usb_disabled())
		return -ENODEV;

	driver = &ehci_driver;

	dev->power.power_state = PMSG_ON;

	fusbh200_hcd = hcd = usb_create_hcd (driver, dev, "FUSBH200_AMBA");
	if (!hcd) {
		retval = -ENOMEM;
		goto err1;
	}

	if (driver->flags & HCD_MEMORY) {	// EHCI, OHCI
		hcd->rsrc_start = USB_FUSBH200_VA_BASE;
		hcd->rsrc_len = USB_FUSBH200_VA_SIZE;
		if (!request_mem_region (hcd->rsrc_start, hcd->rsrc_len,
					driver->description)) {
			dev_dbg (dev, "controller already in use\n");
			retval = -EBUSY;
			goto err2;
		}
		/*hcd->regs = ioremap_nocache (hcd->rsrc_start, hcd->rsrc_len);*/
		hcd->regs = ioremap_nocache (USB_FUSBH200_PA_BASE, hcd->rsrc_len);
		if (hcd->regs == NULL) {
			dev_dbg (dev, "error mapping memory\n");
			retval = -EFAULT;
			goto err3;
		}
		// fotg2xx_dbg("usb_hcd_fotg2xx_probe ---> hcd->regs:%x con:%x %x\n",*((u32 *)&hcd->regs+4),readl(&hcd->regs), *((u32 *)hcd->rsrc_start));

	} else { 				// UHCI
		printk("Warning %s --- FUSBH200 Not support UHCI Flags \n", __func__);
		retval = -ENOMEM;
		goto err1;
	}

	retval = usb_add_hcd (hcd, USB_FUSBH200_IRQ, IRQF_SHARED);
	/*free_irq(USB_FOTG2XX_IRQ,hcd);  Free host interrupt handler for OTG Controller handler*/
	fotg2xx_dbg("%s: %x \n", __func__, retval);

	if (retval != 0)
		goto err4;
	return retval;

err4:
	if (driver->flags & HCD_MEMORY) {
		iounmap (hcd->regs);
err3:
		release_mem_region (hcd->rsrc_start, hcd->rsrc_len);
	}
	else
		release_region (hcd->rsrc_start, hcd->rsrc_len);
err2:
	usb_put_hcd (hcd);
err1:
	dev_err (dev, "init %s fail, %d\n", "NULL", retval);
	return retval;
}
#endif

/**
 * usb_hcd_fotg2xx_probe - initialize HCDs
 * @dev: USB Host Controller being probed
 * Context: !in_interrupt()
 *
 * Allocates basic PCI resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 *
 * Store this function in the HCD's struct pci_driver as probe().
 */
#ifdef CONFIG_GM_FOTG2XX
int usb_hcd_fotg2xx_probe (struct device *dev)
{
	struct hc_driver	*driver;
	struct usb_hcd		*hcd;
	int			retval;

	fotg2xx_dbg("usb_hcd_fotg2xx_probe: %x \n",(u32)dev);

	if (usb_disabled())
		return -ENODEV;

	driver = &ehci_driver;

	dev->power.power_state = PMSG_ON;

	fotg2xx_hcd = hcd = usb_create_hcd (driver, dev, "FOTG2XX_AMBA");
	if (!hcd) {
		retval = -ENOMEM;
		goto err1;
	}

	if (driver->flags & HCD_MEMORY) {	// EHCI, OHCI
		hcd->rsrc_start = USB_FOTG2XX_VA_BASE;
		hcd->rsrc_len = USB_FOTG2XX_VA_SIZE;
		if (!request_mem_region (hcd->rsrc_start, hcd->rsrc_len,
					driver->description)) {
			dev_dbg (dev, "controller already in use\n");
			retval = -EBUSY;
			goto err2;
		}
		/*hcd->regs = ioremap_nocache (hcd->rsrc_start, hcd->rsrc_len);*/
		hcd->regs = ioremap_nocache (USB_FOTG2XX_PA_BASE, hcd->rsrc_len);
		if (hcd->regs == NULL) {
			dev_dbg (dev, "error mapping memory\n");
			retval = -EFAULT;
			goto err3;
		}
		// fotg2xx_dbg("usb_hcd_fotg2xx_probe ---> hcd->regs:%x con:%x %x\n",*((u32 *)&hcd->regs+4),readl(&hcd->regs), *((u32 *)hcd->rsrc_start));

	} else { 				// UHCI
		printk("Warning usb_hcd_fotg2xx_probe --- FOTG2XX Not support UHCI Flags \n");
		retval = -ENOMEM;
		goto err1;
	}

	retval = usb_add_hcd (hcd, USB_FOTG2XX_IRQ, IRQF_SHARED);
	/*free_irq(USB_FOTG2XX_IRQ,hcd);  Free host interrupt handler for OTG Controller handler*/
	fotg2xx_dbg("usb_hcd_fotg2xx_probe: %x \n",retval);

	if (retval != 0)
		goto err4;
	return retval;

err4:
	if (driver->flags & HCD_MEMORY) {
		iounmap (hcd->regs);
err3:
		release_mem_region (hcd->rsrc_start, hcd->rsrc_len);
	}
	else
		release_region (hcd->rsrc_start, hcd->rsrc_len);
err2:
	usb_put_hcd (hcd);
err1:
	dev_err (dev, "init %s fail, %d\n", "NULL", retval);
	return retval;
}
#endif
//EXPORT_SYMBOL (usb_hcd_fotg2xx_probe);

/* may be called without controller electrically present */
/* may be called with controller, bus, and devices active */

/**
 * usb_hcd_fusbh200_remove - shutdown processing for PCI-based HCDs
 * @dev: USB Host Controller being removed
 * Context: !in_interrupt()
 *
 * Reverses the effect of usb_hcd_fusbh200_probe(), first invoking
 * the HCD's stop() method.  It is always called from a thread
 * context, normally "rmmod", "apmd", or something similar.
 *
 * Store this function in the HCD's struct pci_driver as remove().
 */
#ifdef CONFIG_GM_FUSBH200
int usb_hcd_fusbh200_remove (struct device *dev)
{
	struct usb_hcd		*hcd;

	hcd = fusbh200_hcd;
	if (!hcd)
		return 0;

	usb_remove_hcd (hcd);
	if (hcd->driver->flags & HCD_MEMORY) {
		iounmap (hcd->regs);
		release_mem_region (hcd->rsrc_start, hcd->rsrc_len);
	}
	else {
		release_region (hcd->rsrc_start, hcd->rsrc_len);
	}
	usb_put_hcd (hcd);
	return 0;
}
#endif

/**
 * usb_hcd_fotg2xx_remove - shutdown processing for PCI-based HCDs
 * @dev: USB Host Controller being removed
 * Context: !in_interrupt()
 *
 * Reverses the effect of usb_hcd_fotg2xx_probe(), first invoking
 * the HCD's stop() method.  It is always called from a thread
 * context, normally "rmmod", "apmd", or something similar.
 *
 * Store this function in the HCD's struct pci_driver as remove().
 */
#ifdef CONFIG_GM_FOTG2XX
int usb_hcd_fotg2xx_remove (struct device *dev)
{
	struct usb_hcd		*hcd;

	hcd = fotg2xx_hcd;
	if (!hcd)
		return 0;

	usb_remove_hcd (hcd);
	if (hcd->driver->flags & HCD_MEMORY) {
		iounmap (hcd->regs);
		release_mem_region (hcd->rsrc_start, hcd->rsrc_len);
	}
	else {
		release_region (hcd->rsrc_start, hcd->rsrc_len);
	}
	usb_put_hcd (hcd);
	return 0;
}
#endif
//EXPORT_SYMBOL (usb_hcd_fotg2xx_remove);


#ifdef CONFIG_PM
#ifdef CONFIG_GM_FUSBH200
int usb_hcd_fusbh200_resume (struct device *dev)
{
	struct usb_hcd *hcd;
	int retval;

	hcd = fusbh200_hcd;
	if (hcd->state != HC_STATE_SUSPENDED) {
		dev_dbg (hcd->self.controller,"can't resume, not suspended!\n");
		return 0;
	}

	/* NOTE:  chip docs cover clean "real suspend" cases (what Linux
	 * calls "standby", "suspend to RAM", and so on).  There are also
	 * dirty cases when swsusp fakes a suspend in "shutdown" mode.
	 */

	dev->power.power_state = PMSG_ON;

	hcd->state = HC_STATE_RESUMING;

	retval = hcd->driver->bus_resume(hcd);
	if (!HC_IS_RUNNING (hcd->state)) {
		dev_dbg (hcd->self.controller,"resume fail, retval %d\n", retval);
		usb_hc_died (hcd);
	}

	return retval;
}
#endif
/**
 * usb_hcd_fotg2xx_resume - power management resume of a PCI-based HCD
 * @dev: USB Host Controller being resumed
 *
 * Store this function in the HCD's struct pci_driver as resume().
 */
#ifdef CONFIG_GM_FOTG2XX
int usb_hcd_fotg2xx_resume (struct device *dev)
{
	struct usb_hcd *hcd;
	int retval;

	hcd = fotg2xx_hcd;
	if (hcd->state != HC_STATE_SUSPENDED) {
		dev_dbg (hcd->self.controller,"can't resume, not suspended!\n");
		return 0;
	}

	/* NOTE:  chip docs cover clean "real suspend" cases (what Linux
	 * calls "standby", "suspend to RAM", and so on).  There are also
	 * dirty cases when swsusp fakes a suspend in "shutdown" mode.
	 */

	dev->power.power_state = PMSG_ON;

	hcd->state = HC_STATE_RESUMING;

	retval = hcd->driver->bus_resume(hcd);
	if (!HC_IS_RUNNING (hcd->state)) {
		dev_dbg (hcd->self.controller,"resume fail, retval %d\n", retval);
		usb_hc_died (hcd);
	}

	return retval;
}
#endif
#endif
//EXPORT_SYMBOL (usb_hcd_fotg2xx_resume);

#ifdef CONFIG_PM
#ifdef CONFIG_GM_FUSBH200
int usb_hcd_fusbh200_suspend (struct device *dev, pm_message_t message)
{
	struct usb_hcd *hcd;
	int retval = 0;
	hcd = fusbh200_hcd;

	/* FIXME until the generic PM interfaces change a lot more, this
	 * can't use PCI D1 and D2 states.  For example, the confusion
	 * between messages and states will need to vanish, and messages
	 * will need to provide a target system state again.
	 *
	 * It'll be important to learn characteristics of the target state,
	 * especially on embedded hardware where the HCD will often be in
	 * charge of an external VBUS power supply and one or more clocks.
	 * Some target system states will leave them active; others won't.
	 * (With PCI, that's often handled by platform BIOS code.)
	 */

	/* even when the PCI layer rejects some of the PCI calls
	 * below, HCs can try global suspend and reduce DMA traffic.
	 * PM-sensitive HCDs may already have done this.
	 */

	switch (hcd->state) {

		/* entry if root hub wasn't yet suspended ... from sysfs,
		 * without autosuspend, or if USB_SUSPEND isn't configured.
		 */
		case HC_STATE_RUNNING:
			hcd->state = HC_STATE_QUIESCING;
			retval = hcd->driver->bus_suspend (hcd); //, message);
			if (retval) {
				dev_dbg (hcd->self.controller,
						"suspend fail, retval %d\n",
						retval);
				break;
			}
			hcd->state = HC_STATE_SUSPENDED;
			/* FALLTHROUGH */

			/* entry with CONFIG_USB_SUSPEND, or hcds that autosuspend: the
			 * controller and/or root hub will already have been suspended,
			 * but it won't be ready for a PCI resume call.
			 *
			 * FIXME only CONFIG_USB_SUSPEND guarantees hub_suspend() will
			 * have been called, otherwise root hub timers still run ...
			 */
		case HC_STATE_SUSPENDED:

			/* NOTE:  dev->current_state becomes nonzero only here, and
			 * only for devices that support PCI PM.  Also, exiting
			 * PCI_D3 (but not PCI_D1 or PCI_D2) is allowed to reset
			 * some device state (e.g. as part of clock reinit).
			 */
			if (retval == 0) {
				dev_dbg (hcd->self.controller, "--> PCI D3\n");
			} else if (retval < 0) {
				dev_dbg (dev, "PCI D3 suspend fail, %d\n",
						retval);
				(void) usb_hcd_fusbh200_resume (dev);
				break;
			}
			break;
		default:
			dev_dbg (hcd->self.controller, "hcd state %d; not suspended\n",
					hcd->state);
			WARN_ON(1);
			retval = -EINVAL;
			break;
	}

	/* update power_state **ONLY** to make sysfs happier */
	if (retval == 0)
		dev->power.power_state = message;
	return retval;
}
#endif
/**
 * usb_hcd_fotg2xx_suspend - power management suspend of a PCI-based HCD
 * @dev: USB Host Controller being suspended
 * @message: semantics in flux
 *
 * Store this function in the HCD's struct pci_driver as suspend().
 */
#ifdef CONFIG_GM_FOTG2XX
int usb_hcd_fotg2xx_suspend (struct device *dev, pm_message_t message)
{
	struct usb_hcd *hcd;
	int retval = 0;
	hcd = fotg2xx_hcd;

	/* FIXME until the generic PM interfaces change a lot more, this
	 * can't use PCI D1 and D2 states.  For example, the confusion
	 * between messages and states will need to vanish, and messages
	 * will need to provide a target system state again.
	 *
	 * It'll be important to learn characteristics of the target state,
	 * especially on embedded hardware where the HCD will often be in
	 * charge of an external VBUS power supply and one or more clocks.
	 * Some target system states will leave them active; others won't.
	 * (With PCI, that's often handled by platform BIOS code.)
	 */

	/* even when the PCI layer rejects some of the PCI calls
	 * below, HCs can try global suspend and reduce DMA traffic.
	 * PM-sensitive HCDs may already have done this.
	 */

	switch (hcd->state) {

		/* entry if root hub wasn't yet suspended ... from sysfs,
		 * without autosuspend, or if USB_SUSPEND isn't configured.
		 */
		case HC_STATE_RUNNING:
			hcd->state = HC_STATE_QUIESCING;
			retval = hcd->driver->bus_suspend (hcd); //, message);
			if (retval) {
				dev_dbg (hcd->self.controller,
						"suspend fail, retval %d\n",
						retval);
				break;
			}
			hcd->state = HC_STATE_SUSPENDED;
			/* FALLTHROUGH */

			/* entry with CONFIG_USB_SUSPEND, or hcds that autosuspend: the
			 * controller and/or root hub will already have been suspended,
			 * but it won't be ready for a PCI resume call.
			 *
			 * FIXME only CONFIG_USB_SUSPEND guarantees hub_suspend() will
			 * have been called, otherwise root hub timers still run ...
			 */
		case HC_STATE_SUSPENDED:

			/* NOTE:  dev->current_state becomes nonzero only here, and
			 * only for devices that support PCI PM.  Also, exiting
			 * PCI_D3 (but not PCI_D1 or PCI_D2) is allowed to reset
			 * some device state (e.g. as part of clock reinit).
			 */
			if (retval == 0) {
				dev_dbg (hcd->self.controller, "--> PCI D3\n");
			} else if (retval < 0) {
				dev_dbg (dev, "PCI D3 suspend fail, %d\n",
						retval);
				(void) usb_hcd_fotg2xx_resume (dev);
				break;
			}
			break;
		default:
			dev_dbg (hcd->self.controller, "hcd state %d; not suspended\n",
					hcd->state);
			WARN_ON(1);
			retval = -EINVAL;
			break;
	}

	/* update power_state **ONLY** to make sysfs happier */
	if (retval == 0)
		dev->power.power_state = message;
	return retval;
}
#endif
#endif
//EXPORT_SYMBOL (usb_hcd_fotg2xx_suspend);
int AMBA_bus_match(struct device * dev, struct device_driver * drv);

struct bus_type AMBA_bus_type = {
	.name		= "AMBA_BUS_TYPE",
	.match          = AMBA_bus_match,
	.suspend        = NULL,
	.resume         = NULL,
	.dev_attrs      = NULL
};

#ifdef CONFIG_GM_FOTG2XX
static void release_fotg2xx(struct device *dev)
{
	printk(KERN_WARNING "release fotg2xx device\n");
}

static struct device fotg2xx_device = {
	.bus = &AMBA_bus_type,
	.parent = NULL,
	.bus_id = "fotg2xx_dev",
	.dma_mask = (void *) &dma_mask,  //0xffffffff,
	.coherent_dma_mask =  0xffffffff,
	.release = release_fotg2xx,
};
#if 0
static struct usb_gadget fotg2xx_gadget = {
	.is_otg = 1,
	.is_a_peripheral = 0,
	.b_hnp_enable = 0,
	.a_hnp_support = 0,
	.a_alt_hnp_support = 0,
};
#endif
struct device_driver fotg2xx_driver = {
	.name   = "FOTG2XX_DRV",
	.bus    = &AMBA_bus_type,
	.probe  = usb_hcd_fotg2xx_probe,
	.remove = usb_hcd_fotg2xx_remove,
#if defined(CONFIG_PM)
	.suspend= usb_hcd_fotg2xx_suspend,
	.resume = usb_hcd_fotg2xx_resume
#endif
};

struct device *fotg2xx_get_device(void)
{
	return &fotg2xx_device;
}
EXPORT_SYMBOL_GPL(fotg2xx_get_device);
#endif

#ifdef CONFIG_GM_FUSBH200
static void release_fusbh200(struct device *dev)
{
	printk(KERN_WARNING "release fusbh200 device\n");
}

static struct device fusbh200_device = {
	.bus    = &AMBA_bus_type,
	.parent = NULL,
	.bus_id = "fusbh200_dev",
	.dma_mask = (void *) &dma_mask,  //0xffffffff,
	.coherent_dma_mask = 0xffffffff,
	.release = release_fusbh200,
};

struct device_driver fusbh200_driver = {
	.name   = "FUSBHXXX_DRV",
	.bus    = &AMBA_bus_type,
	.probe  = usb_hcd_fusbh200_probe,
	.remove = usb_hcd_fusbh200_remove,
#if defined(CONFIG_PM)
	.suspend= usb_hcd_fusbh200_suspend,
	.resume = usb_hcd_fusbh200_resume
#endif
};
#endif

int AMBA_bus_match(struct device * dev, struct device_driver * drv)
{
	int ret = 0;
#if defined(CONFIG_GM_FOTG2XX)
	if (dev == &fotg2xx_device && drv == &fotg2xx_driver){
		printk("%s(...) Found Driver FOTG2XX_DRV\n", __func__);
		ret = 1;
	} 
#endif
#if defined(CONFIG_GM_FUSBH200)
	if (dev == &fusbh200_device && drv == &fusbh200_driver){
		printk("%s(...) Found Driver FUSBHXXX_DRV\n", __func__);
		ret = 1;
	} 
#endif
	if (!ret) {
		fotg2xx_dbg("AMBA_bus_match(...) No Driver Found (%p, %p) (%p,%p)\n",dev,&fotg2xx_device,drv,&fotg2xx_driver);
	}

	return ret;
}
///////////////////////////////////////////////////////////////////
#ifdef CONFIG_GM_FUSBH200

static int AMBA_FUSBH200_register_driver(void)
{
	int retval;

	fotg2xx_dbg("%s(...) %x\n", __func__, 0);

	if (device_register(&fusbh200_device)< 0){
		printk("Error --> fotg2xx_register_driver for device_register\n");
		return -1;
	} else {
		printk("H200 device_register ok\n");
	}

	fotg2xx_dbg("Before Driver register... %d\n",0);
	retval = driver_register(&fusbh200_driver);

	fusbh200_show_hc_regs();

#ifdef CONFIG_GM_FUSBH200_LOW_TIMING
	//John add timing adjustment code for fast plug/unplug issue, should set
	//when system boot up
	{
		u32 temp;

		printk("Note : SET lower timing on FPGA, will get low performance\n");
		//Set Timing parameter when system bus too low
		//special on FPGA, Should set as default value (0x01) in real chip
		//With this message, you should check your system bus performance
		temp = readl (FUSBH200_BASE_ADDRESS | 0x34);
		writel (temp | 0x0d,  FUSBH200_BASE_ADDRESS | 0x34 );
		temp = readl (FUSBH200_BASE_ADDRESS | 0x34);
		printk("Timing on misc(0x%08x) = 0x%x\n",FUSBH200_BASE_ADDRESS | 0x34, temp);
	}
#endif

	return 0;
}

static void AMBA_FUSBH200_unregister_driver(void)
{
	driver_unregister(&fusbh200_driver);
	device_unregister(&fusbh200_device);
}

#endif

//////////////////////////////////////////////////////////////////////

#ifdef CONFIG_GM_FOTG2XX

static int AMBA_FOTG210_register_driver(void)
{
	int retval;

	fotg2xx_dbg("AMBA_register_driver(...%d)\n",0);

	if (device_register(&fotg2xx_device)< 0) {
		printk("Error --> fotg2xx_register_driver for device_register\n");
		return -1;
	} else {
		printk("otg2xx device_register ok\n");
	}

	fotg2xx_dbg("Before Driver register... %d\n",0);
	retval = driver_register(&fotg2xx_driver);

#ifdef CONFIG_GM_FOTG2XX_LOW_TIMING
	//John add timing adjustment code for fast plug/unplug issue, should set
	//when system boot up
	{
		u32 temp;

		printk("Note : SET lower timing on FPGA, will get low performance\n");
		//Set Timing parameter when system bus too low
		//special on FPGA, Should set as default value (0x01) in real chip
		//With this message, you should check your system bus performance
		temp = readl (FOTG2XX_BASE_ADDRESS | 0x40);
		writel (temp | 0x0d,  FOTG2XX_BASE_ADDRESS | 0x40 );
		temp = readl (FOTG2XX_BASE_ADDRESS | 0x40);
		printk("Timing on misc(0x%08x) = 0x%x\n",FOTG2XX_BASE_ADDRESS | 0x40, temp);
	}
#endif

	// Init FOTG2XX's OTG Controller Driver
	return(fotg200_init(&fotg2xx_device,&fotg2xx_hcd->self,NULL));
}

static void AMBA_FOTG210_unregister_driver(void)
{
	driver_unregister(&fotg2xx_driver);
	device_unregister(&fotg2xx_device);
}

#endif

int usb_host_register(void)
{
	int ret = 0;
	if (bus_register(&AMBA_bus_type)!=0) {
		printk("Error --> AMBA_register_driver for bus_register\n");
		return -1;
	} else {
		printk("AMBA bus_register ok\n");
	}
#if defined(CONFIG_GM_FOTG2XX)
	init_FOTG2XX_Dev();
	ret = AMBA_FOTG210_register_driver();
#endif
#if defined(CONFIG_GM_FUSBH200)
#if defined(CONFIG_PLATFORM_GM8181) && defined(CONFIG_PCI)
	// only BGA684 package has PCI pin out
	// if PCI device mode enabled, no more init FUSBH200 doing, just exit
/*
	if (((* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE)>>5) & 0x7) == 0)
		if ((* (volatile unsigned int *) (PCIC_FTPCI100_VA_BASE + 0xA0) & BIT4) == 0)
			return -1;
*/
	if (((ftpmu010_read_reg(0x0)>>5) & 0x07) == 0) {
		if ((* (volatile unsigned int *) (PCIC_FTPCI100_VA_BASE + 0xA0) & BIT4) == 0)
			return -1;
	}
#endif
	init_FUSBH200_Dev();
	ret = AMBA_FUSBH200_register_driver();
#endif
	return ret;
}
EXPORT_SYMBOL(usb_host_register);

void usb_host_unregister(void)
{
	// turn off the clcok and unregister driver and device
#if defined(CONFIG_GM_FOTG2XX)
#if defined(CONFIG_PLATFORM_GM8180) || defined(CONFIG_PLATFORM_GM8185_v2)
	//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x38) |= BIT28;
	ftpmu010_write_reg(usb_get_pmu_fd(), 0x38, BIT28, BIT28);
#elif defined(CONFIG_PLATFORM_GM8181) || defined(CONFIG_PLATFORM_GM8126)
	//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x38) |= BIT16;
	ftpmu010_write_reg(usb_get_pmu_fd(), 0x38, BIT16, BIT16);
#endif
	AMBA_FOTG210_unregister_driver();
#endif
#if defined(CONFIG_GM_FUSBH200)
#ifdef CONFIG_PLATFORM_GM8185_v2
	//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x38) |= BIT2;
	ftpmu010_write_reg(usb_get_pmu_fd(), 0x38, BIT2, BIT2);
#elif defined(CONFIG_PLATFORM_GM8181)
	//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x38) |= BIT17;
	ftpmu010_write_reg(usb_get_pmu_fd(), 0x38, BIT17, BIT17);
#endif
	AMBA_FUSBH200_unregister_driver();
#endif
	// unregister the bus
	bus_unregister(&AMBA_bus_type);
}
EXPORT_SYMBOL(usb_host_unregister);
///////////////////////////////////////////////////////////////////


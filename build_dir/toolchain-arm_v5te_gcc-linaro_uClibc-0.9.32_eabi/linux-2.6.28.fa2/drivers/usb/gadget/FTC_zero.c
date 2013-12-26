/*
 * FTC_zero.c -- GM Gadget Zero, for USB development
 *
 * Copyright (C) 2004-2005 John Chiang
 *
 * FTC_zero.c -- GM Gadget Zero v1.1, for FUSB220 verification
 *				compatible with FUSB220_udc driver
 *
 * Copyright (C) 2005_Feb Andrew Feng
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the above-listed copyright holders may not be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * ALTERNATIVELY, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") as published by the Free Software
 * Foundation, either version 2 of that License or (at your option) any
 * later version.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * Gadget Zero only needs two bulk endpoints, and is an example of how you
 * can write a hardware-agnostic gadget driver running inside a USB device.
 *
 * Hardware details are visible (see CONFIG_USB_ZERO_* below) but don't
 * affect most of the driver.
 *
 * Use it with the Linux host/master side "usbtest" driver to get a basic
 * functional test of your device-side usb stack, or with "usb-skeleton".
 */

#define DEBUG 	1
// #define VERBOSE

//#include <linux/config.h>
#include <linux/uts.h>

#include <asm/uaccess.h>

#include <linux/kernel.h>
#include <linux/module.h>
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
#include <linux/uts.h>
#include <linux/version.h>
#include <linux/device.h>

#include <asm/byteorder.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/unaligned.h>
#include <linux/freezer.h>

#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>

#include "FTC_zero.h"
#include "gadget_chips.h"
//11/2/05' AHB_DMA
#define BULK_DMA 0

/*
 * driver assumes self-powered hardware, and
 * has no way for users to trigger remote wakeup.
 */

/*
 * hardware-specific configuration, controlled by which device
 * controller driver was configured.
 *
 * CHIP ... hardware identifier
 * DRIVER_VERSION_NUM ... alerts the host side driver to differences
 *
 * add other defines for other portability issues, like hardware that
 * for some reason doesn't handle full speed bulk maxpacket of 64.
 */

/*
 * config descriptors are also handcrafted.  these must agree with code
 * that sets configurations, and with code managing interfaces and their
 * altsettings.  other complexity may come from:
 *
 *  - high speed support, including "other speed config" rules
 *  - multiple configurations
 *  - interfaces with alternate settings
 *  - embedded class or vendor-specific descriptors
 *
 * this handles high speed, and has a second config that could as easily
 * have been an alternate interface setting (on most hardware).
 *
 * NOTE:  to demonstrate (and test) more USB capabilities, this driver
 * should include an altsetting to test interrupt transfers, including
 * high bandwidth modes at high speed.  (Maybe work like Intel's test
 * device?)
 */

/* if there is only one request in the queue, there'll always be an
 * irq delay between end of one request and start of the next.
 * that prevents using hardware dma queues.
 */

/*---------------------- debug information define ----------------------------------*/
#define xprintk(d,level,fmt,args...) 	printk("%s %s: "fmt ,shortname ,(d)->gadget->dev.bus_id ,## args)
#define wprintk(level,fmt,args...) 	printk(level "%s : " fmt , shortname , ## args)

#ifdef VERBOSE
#define VDEBUG	DEBUG
#define VDBG DBG
#else
#define VDEBUG(dev,fmt,args...) do { } while (0)
#define VDBG(dev,fmt,args...) do { } while (0)
#endif 

#define ERROR(dev,fmt,args...) 	xprintk(dev , KERN_ERR , fmt , ## args)
#define UWARN(dev,fmt,args...)  	xprintk(dev , KERN_WARNING , fmt , ## args)
#define INFO(dev,fmt,args...)   xprintk(dev , KERN_INFO , fmt , ## args)

#define ZERO_DBG_OFF 	0x00
#define ZERO_DBG_CTRL 	0x01
#define ZERO_DBG_BULK 	0x02
#define ZERO_DBG_ISO	0x04
#define ZERO_DBG_INT	0x08
#define ZERO_DBG_FUNC	0x10
#define ZERO_DBG_TMP	0x20
#define ZERO_USB_DBG 	(ZERO_DBG_OFF)//|ZERO_DBG_CTRL|ZERO_DBG_TMP|ZERO_DBG_FUNC)

#if (ZERO_USB_DBG & ZERO_DBG_FUNC)  
#define DBG_FUNCC(fmt,args...) 	wprintk(KERN_INFO , "dbg_fun-" fmt , ## args)
#else
#define DBG_FUNCC(fmt,args...)
#endif

#if (ZERO_USB_DBG & ZERO_DBG_TMP)  
#define DBG_TEMP(fmt,args...) 	wprintk(KERN_INFO , "dbg_tmp-" fmt , ## args)
#else
#define DBG_TEMP(fmt,args...)
#endif


#if (ZERO_USB_DBG & ZERO_DBG_CTRL)  
#define DBG_CTRLL(fmt,args...) 	wprintk(KERN_INFO , "dbg_ctrl-" fmt , ## args)
#else
#define DBG_CTRLL(fmt,args...)
#endif

#if (ZERO_USB_DBG & ZERO_DBG_BULK)  
#define DBG_BULKK(fmt,args...) 	wprintk(KERN_INFO , "dbg_bulk-" fmt , ## args)
#else
#define DBG_BULKK(fmt,args...)
#endif

#if (ZERO_USB_DBG & ZERO_DBG_INT)  
#define DBG_INTT(fmt,args...) 		wprintk(KERN_INFO , "dbg_intr-" fmt , ## args)
#else
#define DBG_INTT(fmt,args...)
#endif

//------------------- String descriptor table --------------
#ifdef	CONFIG_GM_FUSB220 
#define CHIP			"FUSB220"
#define DRIVER_VERSION_NUM		0x0001
#endif
#define DRIVER_VERSION			"Chinese New Year's Day 2005"

static const char shortname [] = "FTC_Zero";
static const char longname [] = "GM Gadget Zero";
static const char *EP_NAME[] = {"ep0", "ep1-bulkin", "ep2-bulkout", "ep3-intin", "ep4-intout"};

static const char ProductStr [] = "GM Gadget Zero";
static const char serial[] = "0123456789ABCDEF";										//  this field filled in driver init
static const char ConfigStr [] = "zero configuration";
static const char InterfaceStr [] =	"zero interface";

static struct usb_string strings[] = 
{
	//{ Dev_iManufacturer, UTS_SYSNAME " " UTS_RELEASE " with " CHIP, },
	{ Dev_iManufacturer, CHIP, },
	{ Dev_iProduct, ProductStr, },
	{ Dev_iSerialNumber, serial, },
	{ Config1_iConfiguration, ConfigStr},
	{ Intf0_iInterface, InterfaceStr},
	{}			// end of list
};

static struct usb_gadget_strings	stringtab =
{
	.language = 0x0409,		// en-us
	.strings	= strings,
};

static struct FTC_zero_dev	*the_zero_dev;
static struct usb_gadget_driver		zero_driver;
static u8	u8CxINVandorDataCount=0;
static u8	u8CxOUTVandorDataCount=0;

//--------------------------------------------------
//-------------------------------------------------
//#define DUMP_MSGS
#ifdef DUMP_MSGS
static void dump_msg(struct FTC_zero_dev *ZeroDev, const char *label,
		const u8 *buf, unsigned int length)
{
	unsigned int	c;

	DBG_FUNCC("+ (FTC_zero)dump_msg(), length=%d\n",length); 

   	printk("%s :\n",label);
	for (c = 0; c < length; c ++)
	{
   		printk(" %02x", buf[c]);
		if((c%16)==15)
		   	printk("\n");			
	}
   	printk("\n");
}

#else

static void inline dump_msg(struct FTC_zero_dev *ZeroDev, const char *label,
		const u8 *buf, unsigned int length)
{}
#endif /* DUMP_MSGS */

/*-------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
//		vUsb_BulkInit(struct FTC_zero_dev *dev)
//		Description: User specified circuit (AP) init
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_BulkInit(struct FTC_zero_dev *dev)
{
	u16 u16_i;
	DBG_FUNCC("+vUsb_BulkInit()\n");

	dev->eUsbMassStorageState = STATE_CBW_SET_QUEUE;	// Init State
	for(u16_i = 0; u16_i < MAX_BULK_SIZE; u16_i ++)
		dev->u8VirtMemory[u16_i] = 0;			// Init Buffer	
}

static void vUsb_IntInit(struct FTC_zero_dev *dev)
{
	// Initial Interrupt and ISO transfer buffer
	u8 u8i = 0;
	u16 u16i;
	DBG_FUNCC("+vUsb_IntInit()\n");

	for (u16i = 0; u16i < MAX_INTR_SIZE; u16i ++)
		dev->u8InterruptArray[u16i] = u8i++;

	dev->u8InterruptINCount = 0;
	dev->u8Interrupt_TX_COUNT = 1;	
	dev->u8InterruptOutCount = 0;
	dev->u8Interrupt_RX_COUNT = 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
static void free_ep_req (struct usb_ep *ep, struct usb_request *req)
{
	DBG_FUNCC("+free_ep_req()\n");

	if (req->buf)
		kfree(req->buf);
		//usb_ep_free_buffer (ep, req->buf, req->dma, USB_ZERO_BUFSIZ);
	usb_ep_free_request (ep, req);
}

static void search_free_ep_req(struct FTC_zero_dev *dev,struct usb_ep *ep)
{
	u8 i;
	DBG_FUNCC("+search_free_ep_req()\n");

	usb_ep_disable(ep);
	for (i=1;i<=ZERO_EP_NUM;i++) 
	{
		if (strcmp (ep->name,EP_NAME[i]) ==0)
			break;
	}

	if (dev->req[i] != 0) 
	{
		if (i<=ZERO_EP_NUM) 
		{
			free_ep_req (ep, dev->req[i]);
			dev->req[i] = 0;
		}
		else 
		{
			ERROR (dev, "Free EP %s:  but cannot find EP num\n",ep->name);
		}
	}
	else 
	{
		INFO(dev,"req %d has removed\n", i);
	}
}

/*-------------------------------------------------------------------------*/

/* These routines may be called in process context or in_irq */
static void wakeup_thread(struct FTC_zero_dev*ZeroDev)
{
	DBG_FUNCC("+ (FTC_zero)wakeup_thread()\n");

	/* Tell the main thread that something has happened */
	udelay(100);

	ZeroDev->thread_wakeup_needed = 1;
	wake_up_all(&ZeroDev->thread_wqh);
}

#if 0
#else
static void interrupt_in_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct FTC_zero_dev *ZeroDev = (struct FTC_zero_dev *) ep->driver_data;
	struct zero_buffhd	*bh = (struct zero_buffhd *) req->context;
	DBG_FUNCC("+ (FTC_zero)interrupt_in_complete()\n");

	if (req->status || req->actual != req->length)
		DBG_TEMP("%s --> %d, %u/%u\n", __FUNCTION__,
				req->status, req->actual, req->length);
	if (req->status == -ECONNRESET)		// Request was cancelled
		usb_ep_fifo_flush(ep);

	/* Hold the lock while we update the request and buffer states */
	spin_lock(&ZeroDev->lock);
	bh->inreq_busy = 0;
	bh->state = BUF_STATE_EMPTY;
	ZeroDev->u8EP_Queue_Complete[INTR_IN_COMPLETE]=TRUE;
	
	spin_unlock(&ZeroDev->lock);
	wakeup_thread(ZeroDev);

}

static void interrupt_out_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct FTC_zero_dev *ZeroDev = (struct FTC_zero_dev *) ep->driver_data;
	struct zero_buffhd	*bh = (struct zero_buffhd *) req->context;
	DBG_FUNCC("+ (FTC_zero)interrupt_out_complete()\n");

	dump_msg(ZeroDev, "intr-out", req->buf, req->actual);
	if (req->status || req->actual != bh->out_intended_length)
		DBG_TEMP( "%s --> %d, %u/%u\n", __FUNCTION__,
				req->status, req->actual, bh->out_intended_length);
	if (req->status == -ECONNRESET)		// Request was cancelled
		usb_ep_fifo_flush(ep);

	/* Hold the lock while we update the request and buffer states */
	spin_lock(&ZeroDev->lock);
	bh->outreq_busy = 0;
	bh->state = BUF_STATE_FULL;
	ZeroDev->u8EP_Queue_Complete[INTR_OUT_COMPLETE]=TRUE;

	spin_unlock(&ZeroDev->lock);
	wakeup_thread(ZeroDev);

}


static void bulk_in_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct FTC_zero_dev *ZeroDev = (struct FTC_zero_dev *) ep->driver_data;
	struct zero_buffhd	*bh = (struct zero_buffhd *) req->context;
	DBG_FUNCC("+ (FTC_zero)bulk_in_complete()\n");

	if (req->status || req->actual != req->length)
		DBG_TEMP("%s --> %d, %u/%u\n", __FUNCTION__,
				req->status, req->actual, req->length);
	if (req->status == -ECONNRESET)		// Request was cancelled
		usb_ep_fifo_flush(ep);

	/* Hold the lock while we update the request and buffer states */
	spin_lock(&ZeroDev->lock);
	bh->inreq_busy = 0;
	bh->state = BUF_STATE_EMPTY;
	spin_unlock(&ZeroDev->lock);
	wakeup_thread(ZeroDev);
}

static void bulk_out_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct FTC_zero_dev *ZeroDev = (struct FTC_zero_dev *) ep->driver_data;
	struct zero_buffhd	*bh = (struct zero_buffhd *) req->context;
	DBG_FUNCC("+ (FTC_zero)bulk_out_complete()\n");

	dump_msg(ZeroDev, "bulk-out", req->buf, req->actual);
	if (req->status || req->actual != bh->out_intended_length)
		DBG_TEMP( "%s --> %d, %u/%u\n", __FUNCTION__,
				req->status, req->actual, bh->out_intended_length);
	if (req->status == -ECONNRESET)		// Request was cancelled
		usb_ep_fifo_flush(ep);

	/* Hold the lock while we update the request and buffer states */
	spin_lock(&ZeroDev->lock);
	bh->outreq_busy = 0;
	bh->state = BUF_STATE_FULL;
	spin_unlock(&ZeroDev->lock);
	wakeup_thread(ZeroDev);
}

#endif
/*-------------------------------------------------------------------------*/
static void reset_config (struct FTC_zero_dev *dev)
{
	//if (dev->u8UsbConfigValue == 0)
	//	return;
    
	DBG_FUNCC("+reset_config()\n");
	//DEBUG (dev, "reset config\n");

	dev->u8UsbConfigValue = 0;
	dev->u8UsbInterfaceValue = 0;
	dev->u8UsbInterfaceAlternateSetting = 0;

	// GM AP initial code
	////// initial INT test ////////////
	vUsb_BulkInit(dev);
	////// initial INT test ////////////
	vUsb_IntInit(dev);

	/* just disable endpoints, forcing completion of pending i/o.
	 * all our completion handlers free their requests in this case.
	 */
	if (dev->Bin_ep) 
	{
		search_free_ep_req(dev,dev->Bin_ep);
		dev->Bin_ep = 0;
	}
	if (dev->Bout_ep)
	{
		search_free_ep_req(dev,dev->Bout_ep);
		dev->Bout_ep = 0;
	}
	if (dev->Iin_ep) 
	{
		search_free_ep_req(dev,dev->Iin_ep);
		dev->Iin_ep = 0;
	}
	if (dev->Iout_ep)
	{
		search_free_ep_req(dev,dev->Iout_ep);
		dev->Iout_ep = 0;
	}
}

/*-------------------------------------------------------------------------*/

static int ep0_queue(struct FTC_zero_dev *ZeroDev)
{
	int	rc;
	DBG_FUNCC("+ (FTC_zero)ep0_queue()\n");

	rc = usb_ep_queue(ZeroDev->ep0, ZeroDev->ep0req, GFP_ATOMIC);
	if (rc != 0 && rc != -ESHUTDOWN)
	{
	}
	return rc;
}


/*
 * Config descriptors are handcrafted.  They must agree with the code
 * that sets configurations and with code managing interfaces and their
 * altsettings.  They must also handle different speeds and other-speed
 * requests.
 */
static int populate_config_buf(enum usb_device_speed speed,
		u8 *buf0, u8 type, unsigned index)
{
	u8	*buf = buf0;
#ifdef HIGHSPEED
	int	hs;
#endif
	DBG_FUNCC("+ (FTC_zero)populate_config_buf()\n");
	if (index > 0)
		return -EINVAL;
	if (config_desc.wTotalLength  > EP0_BUFSIZE)
		return -EDOM;

	/* Config (or other speed config) */
	memcpy(buf, &config_desc, USB_DT_CONFIG_SIZE);
	buf[1] = type;
	buf += USB_DT_CONFIG_SIZE;

	/* Interface */
	memcpy(buf, &intf_desc, USB_DT_INTERFACE_SIZE);
	buf += USB_DT_INTERFACE_SIZE;

	/* The endpoints in the interface (at that speed) */
#ifdef HIGHSPEED
	hs = (speed == USB_SPEED_HIGH);
	if (type == USB_DT_OTHER_SPEED_CONFIG)
		hs = !hs;
	if (hs) 
	{
		memcpy(buf, &hs_bulk_in_desc, USB_DT_ENDPOINT_SIZE);
		buf += USB_DT_ENDPOINT_SIZE;
		
		memcpy(buf, &hs_bulk_out_desc, USB_DT_ENDPOINT_SIZE);
		buf += USB_DT_ENDPOINT_SIZE;

		memcpy(buf, &hs_intr_in_desc, USB_DT_ENDPOINT_SIZE);
		buf += USB_DT_ENDPOINT_SIZE;

		memcpy(buf, &hs_intr_out_desc, USB_DT_ENDPOINT_SIZE);
		buf += USB_DT_ENDPOINT_SIZE;
	} 
	else
#endif
	{
		memcpy(buf, &fs_bulk_in_desc, USB_DT_ENDPOINT_SIZE);
		buf += USB_DT_ENDPOINT_SIZE;
		
		memcpy(buf, &fs_bulk_out_desc, USB_DT_ENDPOINT_SIZE);
		buf += USB_DT_ENDPOINT_SIZE;

		memcpy(buf, &fs_intr_in_desc, USB_DT_ENDPOINT_SIZE);
		buf += USB_DT_ENDPOINT_SIZE;

		memcpy(buf, &fs_intr_out_desc, USB_DT_ENDPOINT_SIZE);
		buf += USB_DT_ENDPOINT_SIZE;
	}

	return buf - buf0;
}

static void raise_exception(struct FTC_zero_dev *ZeroDev, enum zero_state new_state)
{
	unsigned long		flags;
	struct task_struct	*thread_task;
	DBG_FUNCC("+ (FTC_zero)raise_exception()\n");

	/* Do nothing if a higher-priority exception is already in progress.
	* If a lower-or-equal priority exception is in progress, preempt it
	* and notify the main thread by sending it a signal. */
	spin_lock_irqsave(&ZeroDev->lock, flags);
	if (ZeroDev->state <= new_state) 
	{
		ZeroDev->exception_req_tag = ZeroDev->ep0_req_tag;
		ZeroDev->state = new_state;
		thread_task = ZeroDev->thread_task;
		if (thread_task)
			send_sig_info(SIGUSR1, (void *) 1L, thread_task);
	}
	spin_unlock_irqrestore(&ZeroDev->lock, flags);
}

/* Ep0 standard request handlers.  These always run in_irq. */
static int 
standard_setup_req(struct FTC_zero_dev *ZeroDev,
						const struct usb_ctrlrequest *ctrl)
{
	struct usb_request	*req = ZeroDev->ep0req;
	int value = -EOPNOTSUPP;		// if value>=0, value = "data length of Cx data stage"
								// else means Cxcmd not support, return stall				
	DBG_FUNCC("+ (FTC_zero)standard_setup_req()  (ctrl->bRequest = %d)\n",ctrl->bRequest);

	/* Usually this just stores reply data in the pre-allocated ep0 buffer,
	 * but config change events will also reconfigure hardware. */
	switch (ctrl->bRequest)
	{
		case USB_REQ_GET_DESCRIPTOR:
			if (ctrl->bRequestType != (USB_DIR_IN | USB_TYPE_STANDARD |	USB_RECIP_DEVICE))
				break;
			switch (ctrl->wValue >> 8) 
			{
				case USB_DT_DEVICE:
					DBG_TEMP( "get device descriptor\n");
					value = min(ctrl->wLength, (u16) sizeof device_desc);
					memcpy(req->buf, &device_desc, value);
					break;
		#ifdef HIGHSPEED
				case USB_DT_DEVICE_QUALIFIER:
					DBG_TEMP( "get device qualifier\n");
					value = min(ctrl->wLength, (u16) sizeof dev_qualifier);
					memcpy(req->buf, &dev_qualifier, value);
					break;

				case USB_DT_OTHER_SPEED_CONFIG:
					DBG_TEMP( "get other-speed config descriptor\n");
					goto get_config;
		#endif /* HIGHSPEED */		
		
				case USB_DT_CONFIG:
					DBG_TEMP( "get configuration descriptor\n");				
		#ifdef HIGHSPEED
				get_config:
		#endif /* HIGHSPEED */
					// combine config, interface, and endpoint desc andcopy into req->buf,
					// and return total desc table length
					value = populate_config_buf(ZeroDev->gadget->speed,
							req->buf,
							ctrl->wValue >> 8,
							ctrl->wValue & 0xff);
					if (value >= 0)
						value = min(ctrl->wLength, (u16) value);
					break;

				case USB_DT_STRING:
					DBG_TEMP( "get string descriptor(string 0x%x)\n",(u8)ctrl->wValue);

					/* wIndex == language code */
					value = usb_gadget_get_string(&stringtab,
								ctrl->wValue & 0xff, req->buf);
					if (value >= 0)
						value = min(ctrl->wLength, (u16) value);
					break;
			}
			break;
	
		/* One config, two speeds */
		case USB_REQ_SET_CONFIGURATION:
			if (ctrl->bRequestType != (USB_DIR_OUT | USB_TYPE_STANDARD |USB_RECIP_DEVICE))
				break;
			DBG_TEMP( "set configuration\n");
			if (ctrl->wValue == CONFIG_VALUE || ctrl->wValue == 0) 
			{
				u8CxINVandorDataCount=0;
				ZeroDev->new_config = ctrl->wValue;

				/* Raise an exception to wipe out previous transaction
				* state (queued bufs, etc) and set the new config. */
				raise_exception(ZeroDev, ZERO_STATE_CONFIG_CHANGE);
				value = DELAYED_STATUS;
			}
			break;
			
		case USB_REQ_GET_CONFIGURATION:
			if (ctrl->bRequestType != (USB_DIR_IN | USB_TYPE_STANDARD |	USB_RECIP_DEVICE))
				break;
			DBG_TEMP( "get configuration\n");
			*(u8 *) req->buf = ZeroDev->config;
			value = min(ctrl->wLength, (u16) 1);
			break;

		case USB_REQ_SET_INTERFACE:
			if (ctrl->bRequestType != (USB_DIR_OUT| USB_TYPE_STANDARD | USB_RECIP_INTERFACE))
				break;
			if (ZeroDev->config && ctrl->wIndex == 0) 
			{
				/* Raise an exception to wipe out previous transaction
				* state (queued bufs, etc) and install the new
				* interface altsetting. */
				raise_exception(ZeroDev, ZERO_STATE_INTERFACE_CHANGE);
				value = DELAYED_STATUS;
			}
			break;
			
		case USB_REQ_GET_INTERFACE:
			if (ctrl->bRequestType != (USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_INTERFACE))
				break;
			if (!ZeroDev->config)
				break;
			if (ctrl->wIndex != 0) 
			{
				value = -EDOM;
				break;
			}
			DBG_TEMP( "get interface\n");
			*(u8 *) req->buf = 0;
			value = min(ctrl->wLength, (u16) 1);
			break;

		default:
			DBG_TEMP("unknown control req %02x.%02x v%04x i%04x l%u\n",
				ctrl->bRequestType, ctrl->bRequest,
				ctrl->wValue, ctrl->wIndex, ctrl->wLength);
			break;
	}
	return value;
}

/* Ep0 standard request handlers.  These always run in_irq. */
static int 
vendor_setup_req(struct FTC_zero_dev *ZeroDev,
						const struct usb_ctrlrequest *ctrl)
{
	u16 u16i;
	struct usb_request	*req = ZeroDev->ep0req;
	u8 *Ep0ReqBuf=req->buf;
	int value = -EOPNOTSUPP;		// if value>=0, value = "data length of Cx data stage"
								// else means Cxcmd not support, return stall				
	DBG_FUNCC("+ (FTC_zero)vendor_setup_req()  \n");

	/* Usually this just stores reply data in the pre-allocated ep0 buffer,
	 * but config change events will also reconfigure hardware. */
	switch (ctrl->bRequestType)
	{
		case 0x40:	// Vendor command test (for Cx OUT test)
			// Vendor command test (for Cx OUT test)
			// If we OUT Cx data as below, FUSB220 will wait for .
			 if((ctrl->bRequest == 0x00)&&
			   (ctrl->wValue == 0x0000)&&
			   (ctrl->wIndex == 0x0000))
			 {
				value = ctrl->wLength;
			 }
			break;
		case 0xC0:
			 if((ctrl->bRequest == 0x00)&&
			   (ctrl->wValue == 0x0000)&&
			   (ctrl->wIndex == 0x0000))
			 {
				for(u16i = 0; u16i< ctrl->wLength; u16i++)
				{
					Ep0ReqBuf[u16i] = u8CxINVandorDataCount;
					u8CxINVandorDataCount++;			
				}
				value = ctrl->wLength;
			 }
			break;
		default:
			DBG_TEMP("unknown vendor control req %02x.%02x v%04x i%04x l%u\n",
			ctrl->bRequestType, ctrl->bRequest,
			ctrl->wValue, ctrl->wIndex, ctrl->wLength);
			break;
	}
	return value;
}

/*
 * The setup() callback implements all the ep0 functionality that's
 * not handled lower down, in hardware or the hardware driver (like
 * device and endpoint feature flags, and their status).  It's all
 * housekeeping for the gadget function we're implementing.  Most of
 * the work is in config-specific setup.
 */
static int
zero_setup (struct usb_gadget *gadget, const struct usb_ctrlrequest *ctrl)
{
	struct FTC_zero_dev *ZeroDev = get_gadget_data (gadget);
	int rc = -EOPNOTSUPP;
	u8 *Ep0ReqBuf=ZeroDev->ep0req->buf;
	u16 u16i;

	DBG_FUNCC("+zero_setup()\n");
	++ZeroDev->ep0_req_tag;		// Record arrival of a new request
	ZeroDev->ep0req->context = NULL;
	ZeroDev->ep0req->length = 0;
	dump_msg(ZeroDev, "ep0-setup", (u8 *) ctrl, 8);

	if ((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_VENDOR)
	{
		rc = vendor_setup_req(ZeroDev, ctrl);
	}
	else 
	{
		rc = standard_setup_req(ZeroDev, ctrl);
	}
	/* Respond with data/status or defer until later? */
	if (rc >= 0 && rc != DELAYED_STATUS) 
	{
		ZeroDev->ep0req->length = rc;
		ZeroDev->ep0req_name = (ctrl->bRequestType & USB_DIR_IN ? "ep0-in" : "ep0-out");
		rc = ep0_queue(ZeroDev);

		if(rc>=0)
		{
			// if this is Cx vendor command test, must check received data
			if((ZeroDev->ep0req_name == "ep0-out")&&
				((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_VENDOR))
			{
				for(u16i = 0; u16i< ctrl->wLength; u16i++)
				{
					if(Ep0ReqBuf[u16i] != u8CxOUTVandorDataCount)
					{
						rc = -EOPNOTSUPP;
						printk("Cx Vendor OUT compare data fail, (%d) err=0x%x corre=0x%x\n",
									u16i,Ep0ReqBuf[u16i],u8CxOUTVandorDataCount);
					}
					u8CxOUTVandorDataCount++;			
				}
			}
		}
	}

	/* Device either stalls (rc < 0) or reports success */
	return rc;
}


/*-------------------------------------------------------------------------*/

//typedef void (*zero_routine_t)(struct zero_dev *);

static void ep0_complete(struct usb_ep *ep, struct usb_request *req)
{
//	struct zero_dev *ZeroDev = (struct zero_dev *) ep->driver_data;

	DBG_FUNCC("+ (FTC_zero)ep0_complete()\n");

	if (req->actual > 0)
	{}		//		dump_msg(ZeroDev, ZeroDev->ep0req_name, req->buf, req->actual);
	if (req->status || req->actual != req->length)
	{}
	if (req->status == -ECONNRESET)		// Request was cancelled
		usb_ep_fifo_flush(ep);

	if (req->status == 0 && req->context)
	{}
}


/*-------------------------------------------------------------------------*/
static int inline exception_in_progress(struct FTC_zero_dev*ZeroDev)
{
	return (ZeroDev->state > ZERO_STATE_IDLE);
}


static int sleep_thread(struct FTC_zero_dev*ZeroDev)
{
	int	rc;
	DBG_FUNCC("+ (FTC_zero)sleep_thread()\n");

	/* Wait until a signal arrives or we are woken up */
	rc = wait_event_interruptible(ZeroDev->thread_wqh,
			ZeroDev->thread_wakeup_needed);
	ZeroDev->thread_wakeup_needed = 0;
	return (rc ? -EINTR : 0);
}


static int zero_set_halt(struct FTC_zero_dev*ZeroDev, struct usb_ep *ep)
{
	const char	*name;

	if (ep == ZeroDev->Bin_ep)
		name = "bulk-in";
	else if (ep == ZeroDev->Bout_ep)
		name = "bulk-out";
	else
		name = ep->name;
	DBG_TEMP( "%s set halt\n", name);
	return usb_ep_set_halt(ep);
}

static int enable_endpoint(struct FTC_zero_dev*ZeroDev, struct usb_ep *ep,
		const struct usb_endpoint_descriptor *d)
{
	int	rc;
	DBG_FUNCC("+ (FTC_zero)enable_endpoint()\n");

	ep->driver_data = ZeroDev;
	rc = usb_ep_enable(ep, d);
	if (rc)
		ERROR(ZeroDev, "can't enable %s, result %d\n", ep->name, rc);
	return rc;
}

static int alloc_request(struct FTC_zero_dev*ZeroDev, struct usb_ep *ep,
		struct usb_request **preq)
{	
	DBG_FUNCC("+ (FTC_zero)alloc_request()\n");
	
	*preq = usb_ep_alloc_request(ep, GFP_ATOMIC);
	if (*preq)
		return 0;
	DBG_TEMP( "can't allocate request for %s\n", ep->name);
	return -ENOMEM;
}

/*
 * Reset interface setting and re-init endpoint state (toggle etc).
 * Call with altsetting < 0 to disable the interface.  The only other
 * available altsetting is 0, which enables the interface.
 */
static int do_set_interface(struct FTC_zero_dev*ZeroDev, int altsetting)
{
	int	rc = 0;
	int	i;
	const struct usb_endpoint_descriptor	*d;

	DBG_FUNCC("+ (FTC_zero)do_set_interface()\n");

	if (ZeroDev->running)
		DBG_TEMP("reset interface\n");
		
reset:

	DBG_TEMP("FTC_zero =>do_set_interface()=> Deallocate the requests \n");//Bruce
	/* Deallocate the requests */
	for (i = 0; i < NUM_BUFFERS; ++i) 
	{
		struct zero_buffhd *bh = &ZeroDev->buffhds[i];
		if (bh->inreq)
		{
			usb_ep_free_request(ZeroDev->Bin_ep, bh->inreq);
			bh->inreq = NULL;
		}
		if (bh->outreq) 
		{
			usb_ep_free_request(ZeroDev->Bout_ep, bh->outreq);
			bh->outreq = NULL;
		}
	}
	if(ZeroDev->buffhds_intr_in.inreq)
	{
		usb_ep_free_request(ZeroDev->Iin_ep, ZeroDev->buffhds_intr_in.inreq);
		ZeroDev->buffhds_intr_in.inreq = NULL;
	}
	if(ZeroDev->buffhds_intr_out.outreq)
	{
		usb_ep_free_request(ZeroDev->Iout_ep, ZeroDev->buffhds_intr_out.outreq);
		ZeroDev->buffhds_intr_out.outreq = NULL;
	}

	DBG_TEMP("FTC_zero =>do_set_interface()=> Disable the endpoints \n");//Bruce
	/* Disable the endpoints */
	if (ZeroDev->bulk_in_enabled) 
	{
		usb_ep_disable(ZeroDev->Bin_ep);
		ZeroDev->bulk_in_enabled = 0;
	}
	if (ZeroDev->bulk_out_enabled) 
	{
		usb_ep_disable(ZeroDev->Bout_ep);
		ZeroDev->bulk_out_enabled = 0;
	}
	if (ZeroDev->intr_in_enabled) 
	{
		usb_ep_disable(ZeroDev->Iin_ep);
		ZeroDev->intr_in_enabled = 0;
	}
	if (ZeroDev->intr_out_enabled) 
	{
		usb_ep_disable(ZeroDev->Iout_ep);
		ZeroDev->intr_out_enabled = 0;
	}
	
	ZeroDev->running = 0;
	if (altsetting < 0 || rc != 0)
		return rc;

	DBG_TEMP( "set interface %d\n", altsetting);
	DBG_TEMP("FTC_zero =>do_set_interface()=> Enable the endpoints \n");//Bruce

	/* Enable the endpoints */
	d = ep_desc(ZeroDev->gadget, &fs_bulk_in_desc, &hs_bulk_in_desc);
	if ((rc = enable_endpoint(ZeroDev, ZeroDev->Bin_ep, d)) != 0)
		goto reset;
	ZeroDev->bulk_in_enabled = 1;

	d = ep_desc(ZeroDev->gadget, &fs_bulk_out_desc, &hs_bulk_out_desc);
	if ((rc = enable_endpoint(ZeroDev, ZeroDev->Bout_ep, d)) != 0)
		goto reset;
	ZeroDev->bulk_out_enabled = 1;

	d = ep_desc(ZeroDev->gadget, &fs_intr_in_desc, &hs_intr_in_desc);
	if ((rc = enable_endpoint(ZeroDev, ZeroDev->Iin_ep, d)) != 0)
		goto reset;
	ZeroDev->intr_in_enabled = 1;
    
	d = ep_desc(ZeroDev->gadget, &fs_intr_out_desc, &hs_intr_out_desc);
	if ((rc = enable_endpoint(ZeroDev, ZeroDev->Iout_ep, d)) != 0)
		goto reset;
	ZeroDev->intr_out_enabled = 1;
	
	DBG_TEMP("FileStorage =>do_set_interface()=> Allocate the requests \n");//Bruce
	/* Allocate the requests */
	for (i = 0; i < NUM_BUFFERS; ++i)
	{
		struct zero_buffhd	*bh = &ZeroDev->buffhds[i];

		if ((rc = alloc_request(ZeroDev, ZeroDev->Bin_ep, &bh->inreq)) != 0)
			goto reset;
		if ((rc = alloc_request(ZeroDev, ZeroDev->Bout_ep, &bh->outreq)) != 0)
			goto reset;
		bh->inreq->buf = bh->outreq->buf = bh->buf;
		bh->inreq->dma = bh->outreq->dma = bh->dma;
		bh->inreq->context = bh->outreq->context = bh;
		bh->inreq->complete = bulk_in_complete;
		bh->outreq->complete = bulk_out_complete;
	}
	
	// Allocate interrupt request
	if ((rc = alloc_request(ZeroDev, ZeroDev->Iin_ep, &ZeroDev->buffhds_intr_in.inreq)) != 0)
		goto reset;
	ZeroDev->buffhds_intr_in.inreq->buf =  ZeroDev->buffhds_intr_in.buf;
	ZeroDev->buffhds_intr_in.inreq->dma =  ZeroDev->buffhds_intr_in.dma;
	ZeroDev->buffhds_intr_in.inreq->context =  &ZeroDev->buffhds_intr_in;
	ZeroDev->buffhds_intr_in.inreq->complete = interrupt_in_complete;
	
	if ((rc = alloc_request(ZeroDev, ZeroDev->Iout_ep, &ZeroDev->buffhds_intr_out.outreq)) != 0)
		goto reset;
	ZeroDev->buffhds_intr_out.outreq->buf = ZeroDev->buffhds_intr_out.buf;
	ZeroDev->buffhds_intr_out.outreq->dma = ZeroDev->buffhds_intr_out.dma;
	ZeroDev->buffhds_intr_out.outreq->context = &ZeroDev->buffhds_intr_out;
	ZeroDev->buffhds_intr_out.outreq->complete = interrupt_out_complete;
	
	ZeroDev->running = 1;

	// init test parameter for Cx, bulk and interrupt test
	ZeroDev->Mass_stor_state  = STATE_CBW_SET_QUEUE;
	ZeroDev->Intr_IN_State =ZeroDev->Intr_OUT_State = STATE_INTR_SET_QUEUE;

	ZeroDev->u8InterruptINCount = 0;
	ZeroDev->u8Interrupt_TX_COUNT = 1;	
	ZeroDev->u8InterruptOutCount = 0;
	ZeroDev->u8Interrupt_RX_COUNT = 1;

	u8CxINVandorDataCount=0;
	u8CxOUTVandorDataCount=0;	
	
	return rc;
}


/*
 * Change our operational configuration.  This code must agree with the code
 * that returns config descriptors, and with interface altsetting code.
 *
 * It's also responsible for power management interactions.  Some
 * configurations might not work with our current power sources.
 * For now we just assume the gadget is always self-powered.
 */
static int do_set_config(struct FTC_zero_dev*ZeroDev, u8 new_config)
{
	int	rc = 0;
	DBG_FUNCC("+ (FTC_zero)do_set_config()\n");

	/* Disable the single interface */
	if (ZeroDev->config != 0) 
	{
		DBG_TEMP( "reset config\n");
		ZeroDev->config = 0;
		rc = do_set_interface(ZeroDev, -1);
	}

	/* Enable the interface */
	if (new_config != 0) 
	{
		ZeroDev->config = new_config;
		if ((rc = do_set_interface(ZeroDev, 0)) != 0)
			ZeroDev->config = 0;	// Reset on errors
		else
		{
			char *speed;
			switch (ZeroDev->gadget->speed) 
			{
				case USB_SPEED_LOW:		speed = "low";	break;
				case USB_SPEED_FULL:	speed = "full";		break;
				case USB_SPEED_HIGH:	speed = "high";	break;
				default: 		speed = "?";	break;
			}
			DBG_FUNCC("- (FTC_zero)do_set_config(), device is %s speed.\n",speed);
		}
	}
	DBG_FUNCC("- (FTC_zero)do_set_config() finish.\n");
	return rc;
}

static void handle_exception(struct FTC_zero_dev*ZeroDev)
{
	siginfo_t		info;
	int			sig;
	int			i;
	int			num_active;
	struct zero_buffhd	*bh;
	enum zero_state	old_state;
	u8			new_config;
	unsigned int	exception_req_tag;
	int			rc;

	DBG_FUNCC("+ (FTC_zero)handle_exception()\n");
	/* Clear the existing signals.  Anything but SIGUSR1 is converted
	 * into a high-priority EXIT exception. */
	for (;;)
	{
		sig = dequeue_signal_lock(current, &ZeroDev->thread_signal_mask,
				&info);
		if (!sig)
			break;
		if (sig != SIGUSR1) 
		{
			if (ZeroDev->state < ZERO_STATE_EXIT)
				DBG_TEMP( "Main thread exiting on signal\n");
			raise_exception(ZeroDev, ZERO_STATE_EXIT);
		}
	}

	// dequeue bulk endpoint request
	for (i = 0; i < NUM_BUFFERS; i++)
	{
		bh = &ZeroDev->buffhds[i];
		if (bh->inreq_busy)			
			usb_ep_dequeue(ZeroDev->Bin_ep, bh->inreq);
		if (bh->outreq_busy)
			usb_ep_dequeue(ZeroDev->Bout_ep, bh->outreq);
		bh->inreq_busy=bh->outreq_busy=0;
	}
	
	// dequeue interrupt endpoint request
	if(ZeroDev->buffhds_intr_in.inreq_busy)
		usb_ep_dequeue(ZeroDev->Iin_ep, ZeroDev->buffhds_intr_in.inreq);
	
	if(ZeroDev->buffhds_intr_out.outreq_busy)
		usb_ep_dequeue(ZeroDev->Iout_ep, ZeroDev->buffhds_intr_out.outreq);
	ZeroDev->buffhds_intr_in.inreq_busy=ZeroDev->buffhds_intr_out.outreq_busy=0;

	/* Wait until everything is idle */
	for (;;) 
	{
		num_active = ZeroDev->buffhds_intr_in.inreq_busy+
					ZeroDev->buffhds_intr_out.outreq_busy;
		
		for (i = 0; i < NUM_BUFFERS; ++i) 
		{
			bh = &ZeroDev->buffhds[i];
			num_active += bh->inreq_busy + bh->outreq_busy;
		}
		if ((num_active == 0) ||
		     (ZeroDev->state == ZERO_STATE_EXIT) ||
		     (ZeroDev->state == ZERO_STATE_TERMINATED))
			break;
		if (sleep_thread(ZeroDev))
			return;
	}

	/* Clear out the controller's fifos */
	if (ZeroDev->bulk_in_enabled)
		usb_ep_fifo_flush(ZeroDev->Bin_ep);
	if (ZeroDev->bulk_out_enabled)
		usb_ep_fifo_flush(ZeroDev->Bout_ep);
	if (ZeroDev->intr_in_enabled)
		usb_ep_fifo_flush(ZeroDev->Iin_ep);
	if (ZeroDev->intr_out_enabled)
		usb_ep_fifo_flush(ZeroDev->Iout_ep);

	/* Reset the I/O buffer states and pointers, the SCSI
	 * state, and the exception.  Then invoke the handler. */
	spin_lock_irq(&ZeroDev->lock);

	// Set Bulk buffer structure state
	for (i = 0; i < NUM_BUFFERS; ++i) 
	{
		bh = &ZeroDev->buffhds[i];
		bh->state = BUF_STATE_EMPTY;
	}
 	ZeroDev->next_buffhd_to_fill  = &ZeroDev->buffhds[0];
	
	// Set Interrupt buffer structure state
	ZeroDev->buffhds_intr_in.state = BUF_STATE_EMPTY;
	ZeroDev->buffhds_intr_out.state = BUF_STATE_EMPTY;

	exception_req_tag = ZeroDev->exception_req_tag;
	new_config = ZeroDev->new_config;
	old_state = ZeroDev->state;

	ZeroDev->state = ZERO_STATE_IDLE;	
	spin_unlock_irq(&ZeroDev->lock);

	/* Carry out any extra actions required for the exception */
	switch (old_state) 
	{
		default:
			break;

		case ZERO_STATE_RESET:
			/* In case we were forced against our will to halt a
			 * bulk endpoint, clear the halt now.  (The SuperH UDC
			 * requires this.) */
			if (test_and_clear_bit(CLEAR_BULK_HALTS, &ZeroDev->atomic_bitflags))
			{
				usb_ep_clear_halt(ZeroDev->Bin_ep);
				usb_ep_clear_halt(ZeroDev->Bout_ep);
			}
			break;

		case ZERO_STATE_INTERFACE_CHANGE:
			rc = do_set_interface(ZeroDev, 0);
			if (ZeroDev->ep0_req_tag != exception_req_tag)
				break;
			if (rc != 0)			// STALL on errors
				zero_set_halt(ZeroDev, ZeroDev->ep0);
			else				// Complete the status stage
				ep0_queue(ZeroDev);
			break;

		case ZERO_STATE_CONFIG_CHANGE:
			rc = do_set_config(ZeroDev, new_config);
			if (ZeroDev->ep0_req_tag != exception_req_tag)
				break;
			if (rc != 0)			// STALL on errors
				zero_set_halt(ZeroDev, ZeroDev->ep0);
			else				// Complete the status stage
				ep0_queue(ZeroDev);
			break;

		case ZERO_STATE_DISCONNECT:
			do_set_config(ZeroDev, 0);		// Unconfigured state
			break;

		case ZERO_STATE_EXIT:
		case ZERO_STATE_TERMINATED:
			DBG_TEMP("handle_exception() set deiver state to ZERO_STATE_TERMINATED, exit zero driver\n");
			do_set_config(ZeroDev, 0);			// Free resources
			spin_lock_irq(&ZeroDev->lock);
			ZeroDev->state = ZERO_STATE_TERMINATED;	// Stop the thread
			spin_unlock_irq(&ZeroDev->lock);
			break;
	}
}

////////////////////// Bulk test function start ///////////////////////

/* Make bulk-out requests be divisible by the maxpacket size */
static void inline set_out_req_length(struct FTC_zero_dev*ZeroDev,
		struct zero_buffhd *bh, unsigned int length)
{
	unsigned int	rem;
    
	DBG_FUNCC("+ (FTC_zero)set_out_req_length(),len=%d\n",length);
    
	bh->out_intended_length = length;
	rem = length % ZeroDev->bulk_out_maxpacket;
	if (rem > 0)
		length += ZeroDev->bulk_out_maxpacket - rem;
	bh->outreq->length = length;
}

/* Use this for bulk or interrupt transfers, not ep0 */
static void start_transfer(struct FTC_zero_dev*ZeroDev, struct usb_ep *ep,
		struct usb_request *req, volatile int *pbusy, volatile enum zero_buffer_state *state)
{
	int	rc;
	DBG_FUNCC("+ (FTC_zero) start_transfer()\n");
    
	if (ep == ZeroDev->Bin_ep)
		dump_msg(ZeroDev, "bulk-in", req->buf, req->length);
	else if (ep == ZeroDev->Iin_ep)
		dump_msg(ZeroDev, "intr-in", req->buf, req->length);
	*pbusy = 1;
	*state = BUF_STATE_BUSY;
	rc = usb_ep_queue(ep, req, GFP_KERNEL);
	if (rc != 0) 
	{
		*pbusy = 0;
		*state = BUF_STATE_EMPTY;

		/* We can't do much more than wait for a reset */
		/* Note: currently the net2280 driver fails zero-length
		 * submissions if DMA is enabled. */
		if (rc != -ESHUTDOWN && 
		   !(rc == -EOPNOTSUPP && req->length == 0))
		{
			printk("FTC_zero.c: start_transfer()-> error in submission: %s --> %d\n", ep->name, rc);
		}
	}
}

static int halt_bulk_in_endpoint(struct FTC_zero_dev*ZeroDev)
{
	int	rc;
	DBG_FUNCC("+ (FTC_zero)halt_bulk_in_endpoint()\n");

	rc = zero_set_halt(ZeroDev, ZeroDev->Bin_ep);
	if (rc == -EAGAIN)
		DBG_TEMP("delayed bulk-in endpoint halt\n");
	while (rc != 0) 
	{
		if (rc != -EAGAIN) 
		{
			DBG_TEMP("usb_ep_set_halt -> %d\n", rc);
			rc = 0;
			break;
		}

		/* Wait for a short time and then try again */
		set_current_state(TASK_INTERRUPTIBLE);
		if (schedule_timeout(HZ / 10) != 0)
			return -EINTR;
		rc = usb_ep_set_halt(ZeroDev->Bin_ep);
	}
	return rc;
}

static int received_cbw(struct FTC_zero_dev*ZeroDev, struct zero_buffhd *bh)
{
	struct usb_request	*req = bh->outreq;
	struct bulk_cb_wrap	*cbw = (struct bulk_cb_wrap *) req->buf;
	DBG_FUNCC("+ (FTC_zero)received_cbw()\n");

	/* Was this a real packet? */
	if (req->status)
		return -EINVAL;

	/* Is the CBW valid? */
	if (req->actual != USB_BULK_CB_WRAP_LEN ||
			cbw->Signature != __constant_cpu_to_le32(USB_BULK_CB_SIG))
	{
		DBG_BULKK("invalid CBW: len %u sig 0x%x\n",	req->actual,le32_to_cpu(cbw->Signature));

		zero_set_halt(ZeroDev, ZeroDev->Bout_ep);
		halt_bulk_in_endpoint(ZeroDev);
		return -EINVAL;
	}	
	memcpy(&ZeroDev->Bulk_CBW, cbw, USB_BULK_CB_WRAP_LEN);
	// pass u32DataTransferLength to u32DataResidue
	ZeroDev->Bulk_CSW.Residue= cbw->DataTransferLength;
	
	// pass Tag from CBW to CSW
	ZeroDev->Bulk_CSW.Tag= cbw->Tag;
	
	// Assume Status is CMD_PASS
	ZeroDev->Bulk_CSW.Status= USB_STATUS_PASS;
	
	// Get Virtual Memory start address.
	ZeroDev->u32VirtMemoryIndex = (u32)(cbw->CDB[0]) | ((u32)cbw->CDB[1] << 8);
	
	return 0;
}

#if 0
#else
static int CBW_Set_Queue(struct FTC_zero_dev*ZeroDev)
{
	struct zero_buffhd	*bh;
	DBG_FUNCC("+ (FTC_zero)CBW_Set_Queue()-Bulk test, get CBW, set queue to udc\n");

	/* Wait for the next buffer to become available */
	bh = ZeroDev->next_buffhd_to_fill;
	if(bh->state != BUF_STATE_EMPTY) 
		return FALSE;
	
	/* Queue a request to read a Bulk-only CBW */
	set_out_req_length(ZeroDev, bh, USB_BULK_CB_WRAP_LEN);
	start_transfer(ZeroDev, ZeroDev->Bout_ep, bh->outreq,
			&bh->outreq_busy, &bh->state);
	// set mass stor state into next state, and wait bulk out complete
	// to wake thread.
	ZeroDev->Mass_stor_state=STATE_CBW_QUEUE_FINISH;			
	return TRUE;
}

static int CBW_Queue_Finish(struct FTC_zero_dev*ZeroDev)
{
	struct zero_buffhd	*bh;

	/* Wait for the next buffer to become available */
	bh = ZeroDev->next_buffhd_to_fill;
	if (bh->state != BUF_STATE_FULL) 
	{
		DBG_FUNCC("+ (FTC_zero)CBW_Queue_Finish()-- haven't get data, exit this function. \n");
		return FALSE;
	}
	DBG_FUNCC("+ (FTC_zero)CBW_Queue_Finish()--Bulk test, already get CBW, go to next state. \n");
	if( received_cbw(ZeroDev, bh) != 0)
	{
		bh->state = BUF_STATE_EMPTY;
		ZeroDev->Mass_stor_state=STATE_CBW_SET_QUEUE;			
		return TRUE;
	}
	else
	{
		bh->state = BUF_STATE_EMPTY;
		ZeroDev->next_buffhd_to_fill = bh->next;

		// judge what's next state, and go to next state
		if (ZeroDev->Bulk_CSW.Residue == 0)
			ZeroDev->Mass_stor_state  = STATE_CSW_SET_QUEUE;
		else if (ZeroDev->Bulk_CBW.Flags== 0)
			ZeroDev->Mass_stor_state = STATE_CB_DATA_OUT_SET_QUEUE;
		else
			ZeroDev->Mass_stor_state = STATE_CB_DATA_IN_SET_QUEUE;		
		return TRUE;
	}				
}

#endif

#if 0
#else
static int CB_Data_IN_Set_Queue(struct FTC_zero_dev*ZeroDev)
{
	struct zero_buffhd	*bh;

	DBG_FUNCC("+ (FTC_zero)CB_Data_IN_Set_Queue()-Bulk test, do Bulk IN data stage.\n");
	
	/* Wait for the next buffer to become available */
	bh = ZeroDev->next_buffhd_to_fill;
	if(bh->state != BUF_STATE_EMPTY) 
		return FALSE;

	// copy data from receive buffer to Bulk test buffer.
	memcpy(bh->buf,ZeroDev->u8VirtMemory+ZeroDev->u32VirtMemoryIndex,
			ZeroDev->Bulk_CBW.DataTransferLength);
	/* Queue a request to read a Bulk-only CBW */
	bh->inreq->length=ZeroDev->Bulk_CBW.DataTransferLength;
	start_transfer(ZeroDev, ZeroDev->Bin_ep, bh->inreq,
			&bh->inreq_busy, &bh->state);
	
	ZeroDev->Mass_stor_state=STATE_CB_DATA_IN_QUEUE_FINISH;		
	return TRUE;
}


static int CB_Data_IN_Queue_Finish(struct FTC_zero_dev*ZeroDev)
{
	struct zero_buffhd	*bh;

	DBG_FUNCC("+ (FTC_zero)CB_Data_IN_Queue_Finish()-Bulk test, finish Bulk IN data stage.\n");
	
	/* Wait for the next buffer to become available */
	bh = ZeroDev->next_buffhd_to_fill;
	if(bh->state != BUF_STATE_EMPTY) 
		return FALSE;

	ZeroDev->next_buffhd_to_fill = bh->next;
	ZeroDev->Mass_stor_state=STATE_CSW_SET_QUEUE;	
	return TRUE;
}

static int CB_Data_OUT_Set_Queue(struct FTC_zero_dev*ZeroDev)
{
	struct zero_buffhd	*bh;

	DBG_FUNCC("+ (FTC_zero)CB_Data_OUT_Set_Queue()-Bulk test, do OUT data stage.\n");

	/* Wait for the next buffer to become available */
	bh = ZeroDev->next_buffhd_to_fill;
	if(bh->state != BUF_STATE_EMPTY) 
		return FALSE;

	/* Queue a request to read a Bulk-only CBW */
	#if 0
	set_out_req_length(ZeroDev, bh, ZeroDev->Bulk_CBW.DataTransferLength);
	#else
	bh->outreq->length=ZeroDev->Bulk_CBW.DataTransferLength;
	#endif
	
	start_transfer(ZeroDev, ZeroDev->Bout_ep, bh->outreq, &bh->outreq_busy, &bh->state);

	ZeroDev->Mass_stor_state=STATE_CB_DATA_OUT_QUEUE_FINISH;		
	return TRUE;
}


static int CB_Data_OUT_Queue_Finish(struct FTC_zero_dev*ZeroDev)
{
	struct zero_buffhd	*bh;

	DBG_FUNCC("+ (FTC_zero)CB_Data_OUT_Queue_Finish()-Bulk test, finish OUT data stage.\n");

	/* Wait for the next buffer to become available */
	bh = ZeroDev->next_buffhd_to_fill;
	if(bh->state != BUF_STATE_FULL) 
		return FALSE;
	
	// copy data from receive buffer to Bulk test buffer.
	memcpy(ZeroDev->u8VirtMemory+ZeroDev->u32VirtMemoryIndex,bh->buf,
			ZeroDev->Bulk_CBW.DataTransferLength);
	bh->state = BUF_STATE_EMPTY;

	ZeroDev->next_buffhd_to_fill = bh->next;
	ZeroDev->Mass_stor_state=STATE_CSW_SET_QUEUE;		

	return TRUE;
}
#endif

#if 0
#else
static int CSW_Set_Queue(struct FTC_zero_dev*ZeroDev)
{
	struct zero_buffhd	*bh;

	DBG_FUNCC("+ (FTC_zero)CSW_Set_Queue()\n");
   
	/* Wait for the next buffer to become available */
	bh = ZeroDev->next_buffhd_to_fill;
	if(bh->state != BUF_STATE_EMPTY) 
		return FALSE;

	// copy data from receive buffer to Bulk test buffer.
	memcpy(bh->buf,&ZeroDev->Bulk_CSW, USB_BULK_CS_WRAP_LEN);
	
	/* Queue a request to read a Bulk-only CBW */
	bh->inreq->length=USB_BULK_CS_WRAP_LEN;
	start_transfer(ZeroDev, ZeroDev->Bin_ep, bh->inreq,
			&bh->inreq_busy, &bh->state);

	ZeroDev->Mass_stor_state=STATE_CSW_QUEUE_FINISH;		
	return TRUE;
}

static int CSW_Queue_Finish(struct FTC_zero_dev*ZeroDev)
{
	struct zero_buffhd	*bh;
	DBG_FUNCC("+ (FTC_zero)CSW_Set_Queue()\n");
   
	/* Wait for the next buffer to become available */
	bh = ZeroDev->next_buffhd_to_fill;
	if(bh->state != BUF_STATE_EMPTY) 
		return FALSE;

	ZeroDev->next_buffhd_to_fill = bh->next;
	ZeroDev->Mass_stor_state=STATE_CBW_SET_QUEUE;		
	return TRUE;
}
#endif
////////////////////// Bulk test function finish ///////////////////////

////////////////////// Interrupt test function start ////////////////////
static int do_Intr_IN_Set_Queue(struct FTC_zero_dev*ZeroDev)
{
	struct zero_buffhd	*bh;
	u8 u8i;
	DBG_FUNCC("+ (FTC_zero)do_Intr_IN_Set_Queue()-Interrupt IN test.\n");
	
	/* Wait for the next buffer to become available */
	bh = &ZeroDev->buffhds_intr_in;
	if (bh->state != BUF_STATE_EMPTY)
		return FALSE;
	
	// fill data into interrupt test buffer
	for (u8i = 0; u8i < ZeroDev->u8Interrupt_TX_COUNT; u8i++)
	{
		ZeroDev->u8InterruptArray[u8i] = ZeroDev->u8InterruptINCount;
		ZeroDev->u8InterruptINCount ++;
	}
	
	// copy data from receive buffer to Bulk test buffer.
	memcpy(bh->buf,ZeroDev->u8InterruptArray,	ZeroDev->u8Interrupt_TX_COUNT);
	
	/* Queue a request to read a Bulk-only CBW */
	bh->inreq->length=ZeroDev->u8Interrupt_TX_COUNT;
	start_transfer(ZeroDev, ZeroDev->Iin_ep, bh->inreq,
			&bh->inreq_busy, &bh->state);
	
	ZeroDev->Intr_IN_State=STATE_INTR_QUEUE_FINISH;
	return TRUE;
}

static int do_Intr_IN_Queue_Finish(struct FTC_zero_dev*ZeroDev)
{
	struct zero_buffhd	*bh;
	u32 u32PacketSize ;

#ifdef HIGHSPEED
	int	hs;
	hs = (ZeroDev->gadget->speed== USB_SPEED_HIGH);
	if (hs) 
		u32PacketSize = hs_intr_in_desc.wMaxPacketSize ;	
	else
#endif
		u32PacketSize = fs_intr_in_desc.wMaxPacketSize ;	
	
	/* Wait for the next buffer to become available */
	bh = &ZeroDev->buffhds_intr_in;
	if(bh->state != BUF_STATE_EMPTY)
	{
		DBG_FUNCC("+ (FTC_zero)do_Intr_IN_Queue_Finish()- haven't send data yet, exit this function.\n");
		return FALSE;
	}

	DBG_FUNCC("+ (FTC_zero)do_Intr_IN_Queue_Finish()-Interrupt IN test.\n");
	ZeroDev->u8Interrupt_TX_COUNT++;
	if (ZeroDev->u8Interrupt_TX_COUNT > u32PacketSize)
		ZeroDev->u8Interrupt_TX_COUNT = 1;
	
	ZeroDev->Intr_IN_State=STATE_INTR_SET_QUEUE;
	return TRUE;
}

static int do_Intr_OUT_Set_Queue(struct FTC_zero_dev*ZeroDev)
{
	struct zero_buffhd	*bh;
	DBG_FUNCC("+ (FTC_zero)do_Intr_OUT_Set_Queue()-Interrupt OUT test,len=%d\n",
			ZeroDev->u8Interrupt_RX_COUNT);
	
	/* Wait for the next buffer to become available */
	bh = &ZeroDev->buffhds_intr_out;
	if(bh->state != BUF_STATE_EMPTY)
		return FALSE;
	
	/* Queue a request to read a Bulk-only CBW */
	set_out_req_length(ZeroDev, bh, ZeroDev->u8Interrupt_RX_COUNT);
	start_transfer(ZeroDev, ZeroDev->Iout_ep, bh->outreq,
			&bh->outreq_busy, &bh->state);
	
	ZeroDev->Intr_OUT_State=STATE_INTR_QUEUE_FINISH;
	return TRUE;
}

static int do_Intr_OUT_Queue_Finish(struct FTC_zero_dev*ZeroDev)
{
	struct zero_buffhd	*bh;
	u32 u32PacketSize,u32i ;
	u8 *pu8Buffer;

#ifdef HIGHSPEED
	int	hs;
	hs = (ZeroDev->gadget->speed== USB_SPEED_HIGH);
	if (hs) 
		u32PacketSize = hs_intr_in_desc.wMaxPacketSize ;	
	else
#endif
		u32PacketSize = fs_intr_in_desc.wMaxPacketSize ;	

	/* Wait for the next buffer to become available */
	bh = &ZeroDev->buffhds_intr_out;
	if(bh->state != BUF_STATE_FULL)
	{
		DBG_FUNCC("+ (FTC_zero)do_Intr_OUT_Queue_Finish()- haven't get data, exit this function.\n");
		return FALSE;	
	}

	DBG_FUNCC("+ (FTC_zero)do_Intr_OUT_Queue_Finish()-Interrupt OUT test(len=%d).\n",
				ZeroDev->u8Interrupt_RX_COUNT);
	// Compare Interrupt OUT data
	pu8Buffer = bh->buf;
	for(u32i=0;u32i<ZeroDev->u8Interrupt_RX_COUNT;u32i++)
	{
		if(pu8Buffer[u32i]!= ZeroDev->u8InterruptOutCount)
		{
			printk("FTC_zero: do_Intr_OUT() got wrong data.(%d) Err=0x%x, Corre=0x%x\n",
				u32i,pu8Buffer[u32i],ZeroDev->u8InterruptOutCount);
		}
		ZeroDev->u8InterruptOutCount++;
	}
	
	ZeroDev->u8Interrupt_RX_COUNT++;
	if(ZeroDev->u8Interrupt_RX_COUNT > u32PacketSize)
		ZeroDev->u8Interrupt_RX_COUNT = 1;
	
	ZeroDev->u8IntrOUT_queue_set = TRUE;
	ZeroDev->Intr_OUT_State=STATE_INTR_SET_QUEUE;
	bh->state = BUF_STATE_EMPTY;
	
	return TRUE;
}

////////////////////// Interrupt test function finish ///////////////////

static int zero_main_thread(void *ZeroDev_)
{
	struct FTC_zero_dev *ZeroDev = (struct FTC_zero_dev *) ZeroDev_;

	DBG_FUNCC("+ (FTC_zero)zero_main_thread()\n");

	ZeroDev->thread_task = current;

	/* Release all our userspace resources */
	daemonize("FTC-zero-gadget"); 
	//reparent_to_init();   in 2.6 , daemonize has called reparent_to_init()
	strncpy(current->comm, "FTC-zero-gadget",
			sizeof(current->comm) - 1);

	/* Allow the thread to be killed by a signal, but set the signal mask
	 * to block everything but INT, TERM, KILL, and USR1. */
	//siginitsetinv(&ZeroDev->thread_signal_mask, sigmask(SIGINT) |
	//		sigmask(SIGTERM) | sigmask(SIGKILL) |
	//		sigmask(SIGUSR1));
	//sigprocmask(SIG_SETMASK, &ZeroDev->thread_signal_mask, &old);

	/* Allow the thread to be killed by a signal, but set the signal mask
	 * 	 * to block everything but INT, TERM, KILL, and USR1. */
	allow_signal(SIGINT);
	allow_signal(SIGTERM);
	allow_signal(SIGKILL);
	allow_signal(SIGUSR1);

	/* Allow the thread to be frozen */
	set_freezable();
						
	/* Arrange for userspace references to be interpreted as kernel
	 * pointers.  That way we can pass a kernel pointer to a routine
	 * that expects a __user pointer and it will work okay. */
	set_fs(get_ds());
	DBG_TEMP("+ (FTC_zero)ZeroDev->thread_notifier = 0x%x\n",(u32)&ZeroDev->thread_notifier);

	/* Wait for the gadget registration to finish up */
	wait_for_completion(&ZeroDev->thread_notifier);
	
	ZeroDev->Mass_stor_state  = STATE_CBW_SET_QUEUE;
	ZeroDev->Intr_IN_State =ZeroDev->Intr_OUT_State = STATE_INTR_SET_QUEUE;
	ZeroDev->u8Interrupt_RX_COUNT=1;

	/* The main loop */
	while (ZeroDev->state != ZERO_STATE_TERMINATED) 
	{
		if (exception_in_progress(ZeroDev) || signal_pending(current)) 
		{
			handle_exception(ZeroDev);
			continue;
		}

		if (!ZeroDev->running) 
		{
			sleep_thread(ZeroDev);
			continue;
		}

	INTR_IN_TEST_START:	
		switch(ZeroDev->Intr_IN_State)
		{
			case STATE_INTR_SET_QUEUE:
				do_Intr_IN_Set_Queue(ZeroDev);
				break;
			case STATE_INTR_QUEUE_FINISH:
				if(do_Intr_IN_Queue_Finish(ZeroDev)==TRUE)
					goto INTR_IN_TEST_START;
				break;
		}
				
	INTR_OUT_TEST_START:	
		switch(ZeroDev->Intr_OUT_State)
		{
			case STATE_INTR_SET_QUEUE:
				do_Intr_OUT_Set_Queue(ZeroDev);
				break;
			case STATE_INTR_QUEUE_FINISH:
				if(do_Intr_OUT_Queue_Finish(ZeroDev)==TRUE)
					goto INTR_OUT_TEST_START;
				break;
		}

	BULK_TEST_START:
		switch(ZeroDev->Mass_stor_state)
		{
			case STATE_CBW_SET_QUEUE:
				// after set queue, must goto sleep, and wait bulk out complete to wake up.
				CBW_Set_Queue(ZeroDev);
				break;
				
			case STATE_CBW_QUEUE_FINISH:
				// after receive CBW, must check if CBW ok, just go to next state.
				// else go to reset queue. If buffer still full, just goto sleep and wait next wakeup.
				if(CBW_Queue_Finish( ZeroDev)==TRUE)
					goto BULK_TEST_START;
				else
					break;
					
			case STATE_CB_DATA_IN_SET_QUEUE:
				// after set queue, must goto sleep, and wait bulk in complete to wake up.
				CB_Data_IN_Set_Queue(ZeroDev);
				break;
				
			case STATE_CB_DATA_IN_QUEUE_FINISH:
				// after send data, goto to CSW state.
				// If buffer still full, just goto sleep and wait next wakeup.
				if(CB_Data_IN_Queue_Finish( ZeroDev)==TRUE)
					goto BULK_TEST_START;
				else
					break;
								
			case STATE_CB_DATA_OUT_SET_QUEUE:
				// after set queue, must goto sleep, and wait bulk out complete to wake up.
				CB_Data_OUT_Set_Queue(ZeroDev);
				break;
				
			case STATE_CB_DATA_OUT_QUEUE_FINISH:
				// after receive data, goto to CSW state.
				// If buffer still empty, just goto sleep and wait next wakeup.
				if(CB_Data_OUT_Queue_Finish( ZeroDev)==TRUE)
					goto BULK_TEST_START;
				else
					break;				
				
			case STATE_CSW_SET_QUEUE:
				// after set queue, must goto sleep, and wait bulk in complete to wake up.
				CSW_Set_Queue(ZeroDev);
				break;
				
			case STATE_CSW_QUEUE_FINISH:
				// after send data, goto to CBW state.
				// If buffer still full, just goto sleep and wait next wakeup.
				if(CSW_Queue_Finish( ZeroDev)==TRUE)
					goto BULK_TEST_START;
				else
					break;
				
			default:
				break;
		}

		sleep_thread(ZeroDev);
	}
	
	ZeroDev->thread_task = NULL;
	flush_signals(current);

	/* In case we are exiting because of a signal, unregister the
	 * gadget driver and close the backing file. */
	if (test_and_clear_bit(REGISTERED, &ZeroDev->atomic_bitflags)) 
	{
		usb_gadget_unregister_driver(&zero_driver);
	}

	/* Let the unbind and cleanup routines know the thread has exited */
	complete_and_exit(&ZeroDev->thread_notifier, 0);
}

////////////////////////////////////////////////////////////////////////////

static void
zero_disconnect (struct usb_gadget *gadget)
{
	struct FTC_zero_dev	*dev = get_gadget_data (gadget);
	unsigned long		flags;

	DBG_FUNCC("+zero_disconnect()\n");
	INFO(dev,"now Disconnect device\n");

	spin_lock_irqsave (&dev->lock, flags);
	reset_config (dev);

	/* a more significant application might have some non-usb
	 * activities to quiesce here, saving resources like power
	 * or pushing the notification up a network stack.
	 */
	spin_unlock_irqrestore (&dev->lock, flags);

	/* next we may get setup() calls to enumerate new connections;
	 * or an unbind() during shutdown (including removing module).
	 */
}

/*-------------------------------------------------------------------------*/
static void
zero_unbind (struct usb_gadget *gadget)
{
	struct FTC_zero_dev *ZeroDev = get_gadget_data (gadget);
	int	i;
	struct usb_request *req = ZeroDev->ep0req;
	//struct zero_buffhd	*bh;

	DBG_FUNCC("+zero_unbind()\n");
	
	clear_bit(REGISTERED, &ZeroDev->atomic_bitflags);
	/* If the thread isn't already dead, tell it to exit now */
	if (ZeroDev->state != ZERO_STATE_TERMINATED) 
	{
		raise_exception(ZeroDev, ZERO_STATE_EXIT);
		wait_for_completion(&ZeroDev->thread_notifier);
		/* The cleanup routine waits for this completion also */
		complete(&ZeroDev->thread_notifier);
	}
	
	/* Free the data buffers */
	for (i = 0; i < NUM_BUFFERS; ++i) 
	{
		struct zero_buffhd	*bh = &ZeroDev->buffhds[i];

		if (bh->buf)
		   kfree(bh->buf);
		   //usb_ep_free_buffer(ZeroDev->Bin_ep, bh->buf, bh->dma,USB_ZERO_BUFSIZ);			
	}
	
	if(ZeroDev->buffhds_intr_in.buf)
	   kfree(ZeroDev->buffhds_intr_in.buf);
		//usb_ep_free_buffer(ZeroDev->Iin_ep, ZeroDev->buffhds_intr_in.buf, 
		//					ZeroDev->buffhds_intr_in.dma, USB_ZERO_BUFSIZ);
	
	if(ZeroDev->buffhds_intr_out.buf)
	   kfree(ZeroDev->buffhds_intr_out.buf);
		//usb_ep_free_buffer(ZeroDev->Iout_ep, ZeroDev->buffhds_intr_out.buf,
		//					ZeroDev->buffhds_intr_out.dma, USB_ZERO_BUFSIZ);
	
	/* Free the request and buffer for endpoint 0 */
	if (req) 
	{
		if (req->buf)
		   kfree(req->buf);
			//usb_ep_free_buffer(ZeroDev->ep0, req->buf,
		        //			req->dma, EP0_BUFSIZE);
		usb_ep_free_request(ZeroDev->ep0, req);
	}

	/* we've already been disconnected ... no i/o is active */
	kfree (ZeroDev);
	set_gadget_data (gadget, 0);
}


static int
zero_bind (struct usb_gadget *gadget)
{
	struct FTC_zero_dev *ZeroDev=the_zero_dev;
	int i,rc;
	struct usb_ep		*ep;
	struct usb_request	*req;	
	DBG_FUNCC("+zero_bind() for FTC_zero\n");

	ZeroDev->gadget = gadget;
	set_gadget_data(gadget, ZeroDev);
	ZeroDev->ep0 = gadget->ep0;
	ZeroDev->ep0->driver_data = ZeroDev;

	/* Fix up the descriptors */
	// some descriptor table did not fill when descriptor table init,
	// must fill these data in here
	// 1) for device descriptor
	device_desc.bMaxPacketSize0 = ZeroDev->ep0->maxpacket;
#ifdef HIGHSPEED
	dev_qualifier.bMaxPacketSize0 = ZeroDev->ep0->maxpacket;		// ???
#endif
	// YPING add
	device_desc.bDeviceClass = device_desc.bDeviceSubClass = device_desc.bDeviceProtocol =0;

	// 2) for configuration descriptor
	i = 4;	// Number of endpoints
	config_desc.wTotalLength = USB_DT_CONFIG_SIZE + USB_DT_INTERFACE_SIZE + USB_DT_ENDPOINT_SIZE * i;

	// 3) for interface descriptor

	/* Find all the endpoints we will use */
	i=0;	
	gadget_for_each_ep(ep, gadget) 
	{
		DBG_TEMP( "zero_bind=>gadget_for_each_ep (ep->name=%s) \n",ep->name);
		i++;
		if (strcmp(ep->name, EP_BULK_IN_NAME) == 0)
		{
			ZeroDev->Bin_ep = ep;
			DBG_TEMP( "zero_bind=> Found BULK_IN ED \n");//Bruce
		}
		else if (strcmp(ep->name, EP_BULK_OUT_NAME) == 0)
		{
			ZeroDev->Bout_ep= ep;
			DBG_TEMP("zero_bind=> Found BULK_OUT ED \n");//Bruce
		}
		else if (strcmp(ep->name, EP_INTR_IN_NAME) == 0)
		{
			ZeroDev->Iin_ep= ep;
			DBG_TEMP( "zero_bind=> Found INTR_IN ED \n");//Bruce
		}
		else if (strcmp(ep->name, EP_INTR_OUT_NAME) == 0)
		{
			ZeroDev->Iout_ep= ep;
			DBG_TEMP( "zero_bind=> Found INTR_OUT ED \n");//Bruce
		}
		if (i>5)  //Bruce;;Modify
			break; //Bruce;;Modify	 
	}
	
	ZeroDev->bulk_out_maxpacket = (gadget->speed == USB_SPEED_HIGH ? 512 :
			FS_BULK_OUT_MAXPACKET);
			
	/* Allocate the request and buffer for endpoint 0 */
	ZeroDev->ep0req = req = usb_ep_alloc_request(ZeroDev->ep0, GFP_KERNEL);
	if (!req)
		goto out;
	
	//{
	//   struct FTC_ep        *my_ep;
	//   struct FTC_request	*my_req;   	
        //    
	//   my_ep = container_of(ep, struct FTC_ep, ZeroDev->ep0);
	//   my_req = container_of(req, struct FTC_request, req);	   
	//   my_req->req.buf = dma_alloc_coherent(&my_ep->dev->gadget.dev,EP0_BUFSIZE,&(my_req->req.dma),GFP_KERNEL);
	//   if (!my_req->req.buf) {
	//      printk("cannot allocate DMA buffer for EP:%x\n", (u32) ep);
	//   }
	//   else {  
	//      my_req->req.length = length;  
	//      my_ep->dma=1;
	//      //printk("EP %x== alloc buffer = 0x%x\n", (u32) ep, (u32) my_req->req.buf);	      
 	//   }
        // }
	
	//req->buf = usb_ep_alloc_buffer(ZeroDev->ep0, EP0_BUFSIZE,
	//		&req->dma, GFP_KERNEL);
	req->buf = kmalloc(EP0_BUFSIZE,GFP_KERNEL);
	if (!req->buf)
		goto out;
	req->complete = ep0_complete;

	/* Allocate the data buffers */
	DBG_TEMP("Allocate the data buffers\n");//Bruce
	for (i = 0; i < NUM_BUFFERS; ++i) 
	{
		struct zero_buffhd	*bh = &ZeroDev->buffhds[i];

	        bh->buf = kmalloc(USB_ZERO_BUFSIZ,GFP_KERNEL);
		//bh->buf = usb_ep_alloc_buffer(ZeroDev->Bin_ep, USB_ZERO_BUFSIZ,
		//		&bh->dma, GFP_KERNEL);
		if (!bh->buf)
			goto out;
		bh->next = bh + 1;
	}
	ZeroDev->buffhds[NUM_BUFFERS - 1].next = &ZeroDev->buffhds[0];

	ZeroDev->buffhds_intr_in.buf = kmalloc(USB_ZERO_BUFSIZ,GFP_KERNEL);
	//ZeroDev->buffhds_intr_in.buf=usb_ep_alloc_buffer(ZeroDev->Iin_ep, USB_ZERO_BUFSIZ,
	//						&ZeroDev->buffhds_intr_in.dma, GFP_KERNEL);
	ZeroDev->buffhds_intr_in.next=&ZeroDev->buffhds_intr_in;
	
	ZeroDev->buffhds_intr_out.buf = kmalloc(USB_ZERO_BUFSIZ,GFP_KERNEL);
	//ZeroDev->buffhds_intr_out.buf=usb_ep_alloc_buffer(ZeroDev->Iout_ep, USB_ZERO_BUFSIZ,
	//						&ZeroDev->buffhds_intr_out.dma, GFP_KERNEL);
	ZeroDev->buffhds_intr_out.next=&ZeroDev->buffhds_intr_out;
	
	if ((!ZeroDev->buffhds_intr_in.buf)||(!ZeroDev->buffhds_intr_out.buf))
		goto out;
	
	/* This should reflect the actual gadget power source */
	DBG_TEMP("This should reflect the actual gadget power source\n");//Bruce
	usb_gadget_set_selfpowered(gadget);
	
	DBG_TEMP("register kernel_thread(zero_main_thread...\n");//Bruce
	if ((rc = kernel_thread(zero_main_thread, ZeroDev, (CLONE_VM | CLONE_FS | CLONE_FILES))) < 0)
		goto out;
	ZeroDev->thread_pid = rc;
	
	ZeroDev->u8LineCount = 0;
	INFO (ZeroDev, "%s, version: " DRIVER_VERSION "\n", longname);
	INFO (ZeroDev, "using %s, %s %s %s %s\n", gadget->name,
		 EP_NAME[1],EP_NAME[2],EP_NAME[3],EP_NAME[4]);

	return 0;

out:
    DBG_FUNCC(  "zero_bind Fail(out)...\n");//Bruce

	ZeroDev->state = ZERO_STATE_TERMINATED;	// The thread is dead
	zero_unbind(gadget);
	return -ENOMEM;
}

/*-------------------------------------------------------------------------*/

static struct usb_gadget_driver zero_driver = 
{
	.speed		= USB_SPEED_HIGH,
	.function	= (char *) longname,
	.bind		= zero_bind,
	.unbind		= zero_unbind,

	.setup		= zero_setup,
	.disconnect	= zero_disconnect,
	.driver 	= 
	{
		.name		= (char *) shortname,
		// .shutdown = ...
		// .suspend = ...
		// .resume = ...
	},
};

MODULE_AUTHOR ("Andrew in FTC");
MODULE_LICENSE ("Dual BSD/GPL");

static int __init zero_alloc(void)
{
	struct FTC_zero_dev *ZeroDev;

	DBG_FUNCC("+ (FTC_zero)zero_alloc()\n");

	ZeroDev = kmalloc(sizeof *ZeroDev, GFP_KERNEL);
	if (!ZeroDev)
	{
		DBG_TEMP("+ (FTC_zero)zero_alloc() kmalloc fail\n");
		return -ENOMEM;
	}
	memset(ZeroDev, 0, sizeof *ZeroDev);
	spin_lock_init(&ZeroDev->lock);
	//init_rwsem(&ZeroDev->filesem);
	init_waitqueue_head(&ZeroDev->thread_wqh);
	init_completion(&ZeroDev->thread_notifier);

	// YPING add, for bulk test buffer
	ZeroDev->Bulk_CSW.Signature=USB_BULK_CS_SIG;

	the_zero_dev = ZeroDev;
	return 0;
}

static void zero_free(struct FTC_zero_dev*ZeroDev)
{
	DBG_FUNCC("+ (FTC_zero)zero_free()\n");
	
	kfree(ZeroDev);
}

static int __init init (void)
{
	int		rc;
	struct FTC_zero_dev	*ZeroDev;

	if ((rc = zero_alloc()) != 0)
		return rc;
	ZeroDev = the_zero_dev;
	
	printk("%s call usb_gadget_register_driver to register USB device\n",shortname);
	if ((rc = usb_gadget_register_driver(&zero_driver)) != 0)
	{
		zero_free(ZeroDev);
		return rc;
	}
	set_bit(REGISTERED, &ZeroDev->atomic_bitflags);

	/* Tell the thread to start working */
	DBG_TEMP("FTC_zero init -1\n");//Bruce
	complete(&ZeroDev->thread_notifier);
	DBG_TEMP("FTC_zero init -2\n");//Bruce
	return 0;
}
module_init (init);

static void __exit cleanup (void)
{
	struct FTC_zero_dev *ZeroDev = the_zero_dev;

	DBG_FUNCC("+ (FTC_zero)cleanup()\n");

	/* Unregister the driver iff the thread hasn't already done so */
	if (test_and_clear_bit(REGISTERED, &ZeroDev->atomic_bitflags))
		usb_gadget_unregister_driver(&zero_driver);

	/* Wait for the thread to finish up */
	wait_for_completion(&ZeroDev->thread_notifier);

}
module_exit (cleanup);


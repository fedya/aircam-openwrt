/*******************************************************************************
 * Module name: fotg2xx_udc.c
 *
 * Copyright 2006 GM as an unpublished work.
 * All Rights Reserved.
 *
 * The information contained herein is confidential property of Company.
 * The user, copying, transfer or disclosure of such information is
 * prohibited except by express written agreement with Company.
 *
 * Written on 2006/6 by Elliot Hou Au-ping.
 *
 * Module Description:
 *  This OTG UDC dirver for GM FOTG220 controller
 *
 ******************************************************************************/
//#include <linux/config.h>

#ifdef CONFIG_USB_DEBUG
        #define DEBUG
#else
        #undef DEBUG
#endif
        #define DEBUG
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
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>
#include <linux/moduleparam.h>
#include <linux/dma-mapping.h>

#ifndef CONFIG_GM_FUSB220
#include "../host/fotg2xx-config.h"
#include "../host/fotg2xx.h"
#include "../host/fotg2xx-ehci-macro.h"
#include "../host/fotg2xx_opt-macro.h"
struct device *fotg2xx_get_device(void);
int fotg200_init(struct device *dev,struct usb_bus *host,struct usb_gadget *gadget);
int is_fotg200_init(void);
void init_FOTG2XX_Dev(void);
int is_init_FOTG2XX_Dev(void);
void fotg200_handle_irq(void);
#endif

#include "GM_udc.h"
//#include <asm/arch-GM/ahb_dma.h>

#define	DRIVER_DESC		"FUSB220 USB Device Controller"
#define	DRIVER_VERSION	"04-Oct 2004"

/* static const char driver_name [] = "FTC_FUSB220_udc";*/
#ifdef CONFIG_GM_FUSB220
static const char driver_name [] = "fusb_udc";
#else
static const char driver_name [] = "fotg_udc";
#endif
static const char driver_desc [] = DRIVER_DESC;

static char *names [] = {"ep0","ep1-bulkin","ep2-bulkout","ep3-intin","ep4-intout","ep5","ep6","ep7","ep8","ep9","ep10" };
#define BULK_IN_EP_NUM		1
#define BULK_OUT_EP_NUM		2
#define INTR_IN_EP_NUM		3
#define INTR_OUT_EP_NUM		4

static struct FTC_udc	*the_controller=NULL;
static struct usb_bus	*the_host=NULL;


#define DBG_TEMP_udc(fmt,args...) printk(KERN_INFO "%s : " fmt , "fsg" , ## args) //Bruce;;Remove

/*-------------------------------------------------------------------------*/

static void nuke(struct FTC_ep *, int status);

#ifdef CONFIG_GM_FUSB220
//*******************************************************************
// Name:FIFO_Int
// Description:FUSB220 to enable/disable FIFO interrupt of dedicated EP
//*******************************************************************
static int FIFO_Int(struct FTC_ep *ep,unsigned int val)
{
	u8  u8fifo_n;  
	u32 offset; 
	u8  mval;

	DBG_FUNCC("+FIFO_Int()(ed=%d INT=%d)\n",ep->num,val);

	//<1>.Get the fifo number
	u8fifo_n = mUsbEPMapRd(ep->num);		// get the relatived FIFO number
	if (ep->is_in)
		u8fifo_n &= 0x0F;
	else
		u8fifo_n >>= 4;
	if (u8fifo_n >= FUSB220_MAX_FIFO)	// over the Max. fifo count ?
		return -EINVAL;

	if (ep->is_in) {  // IN 
#ifdef CONFIG_GM_FUSB220
		offset = 0x16;
#else
		offset = 0x140;
#endif
		while (u8fifo_n > 8) {
			offset++;
			u8fifo_n -=8;
		}
		mval = 1<<u8fifo_n;
	}
	else {
#ifdef CONFIG_GM_FUSB220
		offset = 0x12;
#else
		offset = 0x138;
#endif
		while (u8fifo_n > 4) {
			offset++;
			u8fifo_n -=4;
		}
		mval = 1<<(u8fifo_n*2) | 1<<((u8fifo_n*2)+1);
	}

	if (val) { // enable FIFO interrupt
		mUsbIntFIFOEn(offset, mval);
	}
	else { 
		mUsbIntFIFODis(offset, mval);
	}

	return 0;
}
#endif

///////////////////////////////////////////////////////////////////////
// Enable endpoint 
// EP0 : has been enabled while driver booting up
// Need to give this EP's descriptor
static int
FTC_ep_enable(struct usb_ep *_ep, const struct usb_endpoint_descriptor *desc)
{
	struct FTC_udc	*dev;
	struct FTC_ep	*ep;
	u16	max;
	unsigned long flags;

	ep = container_of(_ep, struct FTC_ep, ep);

	DBG_FUNCC("+FTC_ep_enable() : _ep = %x desc = %x ep->desc= %x\n",(u32) _ep, (u32) desc, (u32) ep->desc);

	// check input variable, if there ia any variable undefined, return false
	if (!_ep || !desc || ep->desc
			 || desc->bDescriptorType != USB_DT_ENDPOINT) {
		return -EINVAL;
	}

	// if this is used to enable ep0, return false
	dev = ep->dev; 
	if (ep == &dev->ep[0]) {  //no EP0 need to be enabled
		return -EINVAL;
	}

	// if upper level driver not ready or device speed unknown, return false
	if (!dev->driver || dev->gadget.speed == USB_SPEED_UNKNOWN) {
		return -EINVAL;
	}

	if (ep->num != (desc->bEndpointAddress & 0x0f)) {
		return -EINVAL;
	}

	// EP should be Bulk or intr
	switch (desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) {
		case USB_ENDPOINT_XFER_BULK:
		case USB_ENDPOINT_XFER_INT:
			break;
		default:
			return -EINVAL;
	}

	/* enabling the no-toggle interrupt mode would need an api hook */
	// max = le16_to_cpu(get_unaligned(&desc->wMaxPacketSize));
	 max = le16_to_cpu(desc->wMaxPacketSize);

	// 11/2/05' AHB_DMA
	// Only bulk use AHB_DMA, and not always use DMA, so change while running
	//if ((desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_BULK)
	//   ep->dma = 1;
	//else
	ep->dma = 0;		// right now we choice not to use dma

	ep->is_in = (USB_DIR_IN & desc->bEndpointAddress) != 0;

	spin_lock_irqsave(&ep->dev->lock, flags);

#ifdef CONFIG_GM_FUSB220	
	//Do not enable the Interrupt for this ep, enable interrupt when upper level set queue for this endpoint 
	FIFO_Int(ep,0);
#endif	
	
	ep->ep.maxpacket = max;
	ep->stopped = 0;
	ep->dma_running= FALSE;
	ep->desc = desc;
	spin_unlock_irqrestore(&ep->dev->lock, flags);

	printk("enable %s %s maxpacket %u\n", ep->ep.name,
		   ep->is_in ? "IN" : "OUT",
		   // ep->dma ? "dma" : "pio",
		   max);

	return 0;
}

static void ep_reset(struct FTC_ep *ep)
{
	//struct FTC_udc		*dev = ep->dev;
	DBG_FUNCC("+ep_reset\n");

	ep->ep.maxpacket = MAX_FIFO_SIZE;
	ep->desc = 0;
	ep->stopped = 1;
	ep->irqs = 0;
	ep->dma = 0;
}

static int FTC_ep_disable(struct usb_ep *_ep)
{
	struct FTC_ep	*ep;
	struct FTC_udc	*dev;
	unsigned long	flags;

	DBG_FUNCC("+FTC_ep_disable()\n");

	ep = container_of(_ep, struct FTC_ep, ep);
	if (!_ep || !ep->desc)
		return -ENODEV;

	//printk("+FTC_ep_disable() : _ep = 0x%x ep->desc = 0x%x\n", _ep , ep->desc);
	dev = ep->dev;

	//John mark for in suspend will reset system
	//john if (dev->ep0state == EP0_SUSPEND)
	//john 	return -EBUSY;

	if (ep == &dev->ep[0])  //john no EP0 need to be enabled
		return -EINVAL;

	VDBG(dev, "disable %s\n", _ep->name);

	spin_lock_irqsave(&dev->lock, flags);
	nuke(ep, -ESHUTDOWN);
	ep_reset(ep);

#ifdef CONFIG_GM_FUSB220	
	////John FUSB220, disable FIFO interrupt
	FIFO_Int(ep,0);
#endif
	spin_unlock_irqrestore(&dev->lock, flags);

	return 0;
}

/*-------------------------------------------------------------------------*/

static struct usb_request *
FTC_alloc_request(struct usb_ep *_ep, unsigned gfp_flags)
{
	struct FTC_request	*req;

	DBG_FUNCC("+FTC_alloc_request\n");
	if (!_ep)
		return 0;

	req = kmalloc(sizeof *req, gfp_flags);
	if (!req)
		return 0;

	memset(req, 0, sizeof *req);
        req->req.dma = DMA_ADDR_INVALID;  
	
	INIT_LIST_HEAD(&req->queue);

	DBG_FUNCC("-FTC_alloc_request\n");
	return &req->req;
}

static void
FTC_free_request(struct usb_ep *_ep, struct usb_request *_req)
{
	struct FTC_request	*req;
	
	DBG_FUNCC("+FTC_free_request()\n");

	if (!_ep || !_req)
		return;

	req = container_of(_req, struct FTC_request, req);
	WARN_ON(!list_empty(&req->queue));
	kfree(req);
}

/*-------------------------------------------------------------------------*/

/* many common platforms have dma-coherent caches, which means that it's
 * safe to use kmalloc() memory for all i/o buffers without using any
 * cache flushing calls.  (unless you're trying to share cache lines
 * between dma and non-dma activities, which is a slow idea in any case.)
 *
 * other platforms need more care, with 2.6 having a moderately general
 * solution except for the common "buffer is smaller than a page" case.
 */
//#undef USE_KMALLOC		// right now we chose not to use kmalloc() to get memory buffer

/* allocating buffers this way eliminates dma mapping overhead, which
 * on some platforms will mean eliminating a per-io buffer copy.  with
 * some kinds of system caches, further tweaks may still be needed.
 */
/*
static void *
FTC_alloc_buffer(struct usb_ep *_ep, unsigned bytes,
			dma_addr_t *dma, unsigned  gfp_flags)
{
	void		*retval;
	struct FTC_ep	*ep;

	DBG_FUNCC("+FTC_alloc_buffer():ep = 0x%x\n",(u32) _ep);

	ep = container_of(_ep, struct FTC_ep, ep);
	if (!_ep)
		return 0;

	*dma = DMA_ADDR_INVALID;

#if defined(USE_KMALLOC)
	retval = kmalloc(bytes, gfp_flags);

	if (retval)
		*dma = virt_to_phys(retval);
#else
		// one problem with this call is that it wastes memory on
		// * typical 1/N page allocations: it allocates 1-N pages.
		// * another is that it always uses GFP_ATOMIC.
		// * linux 2.4  --> retval = pci_alloc_consistent((struct pci_dev *)ep->dev, bytes, dma);
		// 
#warning Using pci_alloc_consistent even with buffers smaller than a page.
		retval = dma_alloc_coherent(&ep->dev->gadget.dev,
                                bytes, dma, gfp_flags);
#endif
	return retval;
}

static void
FTC_free_buffer(struct usb_ep *_ep, void *buf, dma_addr_t dma, unsigned bytes)
{

#ifndef	USE_KMALLOC
   	struct FTC_ep	*ep;
	// free memory into the right allocator 
	DBG_FUNCC("+FTC_free_buffer()\n");
	ep = container_of(_ep, struct FTC_ep, ep);
	if (!_ep)
		return;
	// one problem with this call is that some platforms
	// * don't allow it to be used in_irq().
	// * linux 2.4 ---> pci_unmap_single((void *)dev, req->req.dma, temp*16, //USB_EPX_BUFSIZ,  //req->req.length+32,
        //			ep->is_in ? PCI_DMA_TODEVICE : PCI_DMA_FROMDEVICE);
	// 
	dma_free_coherent(&ep->dev->gadget.dev, bytes, buf, dma);
#else
	DBG_FUNCC("+FTC_free_buffer()\n");
	kfree (buf);
#endif
}
*/

/*-------------------------------------------------------------------------*/
// finish/abort one request
static void
done(struct FTC_ep *ep, struct FTC_request *req, int status)
{
	struct FTC_udc		*dev;
	unsigned stopped = ep->stopped;

	DBG_FUNCC("+done()\n");

	list_del_init(&req->queue);

	if (likely(req->req.status == -EINPROGRESS))   // still ongoing
		req->req.status = status;
	else // has finished
		status = req->req.status;

	dev = ep->dev;

	if (req->mapped)  // DMA mapped
	{
           u32 temp;
           DBG_CTRLL("....dma_unmap_single len = %d dma =%x, dir=%x dev=%x\n",
	   		   req->req.length,req->req.dma,ep->is_in,dev);   

	   // important : DMA length will set as 16*n bytes
	   temp = (req->req.length + 15) / 16;
	   temp = temp *16;
	   dma_unmap_single((void *)dev, req->req.dma, temp,  //USB_EPX_BUFSIZ,  
	   	ep->is_in ? DMA_TO_DEVICE : DMA_FROM_DEVICE);
	   req->req.dma = DMA_ADDR_INVALID;
	   req->mapped = 0;
	}
	
#ifndef USB_TRACE
	if (status && status != -ESHUTDOWN)
#endif
	VDBG(dev, "complete %s req %p stat %d len %u/%u\n",
		ep->ep.name, &req->req, status,
		req->req.actual, req->req.length);

	/* don't modify queue heads during completion callback */
	if (status == -ESHUTDOWN)
	   ep->stopped = 0;
	else
	   ep->stopped = 1;

#ifdef CONFIG_GM_FUSB220	
	if (ep->num>0)	
		FIFO_Int(ep,0); //Bruce;;Disable FIFO interrupt
#endif	
	spin_unlock(&dev->lock);
	
	req->req.complete(&ep->ep, &req->req);
	spin_lock(&dev->lock);

	if (ep->num==0)	
		mUsbEP0DoneSet();

	ep->stopped = stopped;  //recover 

#ifdef CONFIG_GM_FUSB220	
	if (!list_empty (&ep->queue))
		FIFO_Int(ep,1);//Bruce;;Enable FIFO interrupt
#endif

	DBG_FUNCC("-done() stopped=%d\n",stopped);
}


/* dequeue ALL requests */
static void nuke(struct FTC_ep *ep, int status)
{
	struct FTC_request	*req;
	DBG_FUNCC("+nuke() ep addr= 0x%x\n", (u32) ep);

	ep->stopped = 1;
	if (list_empty(&ep->queue))
		return;
	while (!list_empty(&ep->queue)) 
	{
		req = list_entry(ep->queue.next, struct FTC_request, queue);
		printk("release req = %x\n", (u32) req);
		done(ep, req, status);
	}
}


static int FTC_queue(struct usb_ep *_ep, struct usb_request *_req, unsigned gfp_flags);

/* dequeue JUST ONE request */
static int FTC_dequeue(struct usb_ep *_ep, struct usb_request *_req)
{
	struct FTC_request	*req;
	struct FTC_ep		*ep;
	struct FTC_udc		*dev;
	unsigned long		flags;

	DBG_FUNCC("+FTC_dequeue()\n");

	ep = container_of(_ep, struct FTC_ep, ep);
	if (!_ep || !_req || (!ep->desc && ep->num != 0))
		return -EINVAL;
	dev = ep->dev;
	if (!dev->driver)
		return -ESHUTDOWN;

	/* we can't touch (dma) registers when suspended */
	if (dev->ep0state == EP0_SUSPEND)
		return -EBUSY;

	VDBG(dev, "%s %s %s %s %p\n", __FUNCTION__, _ep->name,
		ep->is_in ? "IN" : "OUT",
		ep->dma ? "dma" : "pio",
		_req);

	spin_lock_irqsave(&dev->lock, flags);

	/* make sure it's actually queued on this endpoint */
	list_for_each_entry (req, &ep->queue, queue) 
	{
		if (&req->req == _req)
			break;
	}
	if (&req->req != _req) 
	{
		spin_unlock_irqrestore (&dev->lock, flags);
		return -EINVAL;
	}

	//Remove;;if (ep->dma && ep->queue.next == &req->queue && !ep->stopped) {
	//Remove;;	abort_dma(ep, -ECONNRESET);
	//Remove;;	done(ep, req, -ECONNRESET);
	//Remove;;	dma_advance(dev, ep);
	//Remove;;} 
	//Remove;;else
	 if (!list_empty(&req->queue))
	 	{
		done(ep, req, -ECONNRESET);
	 	}
	else
		req = 0;
	spin_unlock_irqrestore(&dev->lock, flags);

	return req ? 0 : -EOPNOTSUPP;
}
extern inline void
pio_advance(struct FTC_ep *ep);
/*-------------------------------------------------------------------------*/

static void FTC_clear_halt(struct FTC_ep *ep)
{
	DBG_FUNCC("+FTC_clear_halt()(ep->num=%d)\n",ep->num);

	// assert (ep->num !=0)
	VDBG(ep->dev, "%s clear halt\n", ep->ep.name);
	if (ep->num == 0) 
	{
		ep->dev->ep0state = EP0_IDLE;
		ep->dev->ep[0].stopped = 0;
	}
	else
	{
		if (ep->is_in)	// IN direction ?
		{
			DBG_CTRLL("FTC_udc==>FTC_clear_halt()==>IN direction, EP%d \n",ep->num);
			mUsbEPinRsTgSet(ep->num);		// Set Rst_Toggle Bit
			mUsbEPinRsTgClr(ep->num);		// Clear Rst_Toggle Bit
			mUsbEPinStallClr(ep->num);	// Clear Stall Bit
		}
		else
		{
			DBG_CTRLL("FTC_udc==>FTC_clear_halt()==>OUT direction, EP%d \n",ep->num);
			mUsbEPoutRsTgSet(ep->num);	// Set Rst_Toggle Bit
			mUsbEPoutRsTgClr(ep->num);	// Clear Rst_Toggle Bit
			mUsbEPoutStallClr(ep->num);	// Clear Stall Bit
		}
	}
	DBG_CTRLL("FTC_udc==>FTC_clear_halt()==>ep->stopped = %d\n",ep->stopped);

	if (ep->stopped) 
	{
		ep->stopped = 0;
	//	pio_advance(ep);
	}
}

static int FTC_set_halt(struct usb_ep *_ep, int value)
{
	struct FTC_ep	*ep;
	unsigned long	flags;
	int		retval = 0;

	DBG_FUNCC("+FTC_set_halt()\n");
	if (!_ep)
		return -ENODEV;
	ep = container_of (_ep, struct FTC_ep, ep);
	
	DBG_BULKK("FTC_set_halt()===> (ep->num=%d)(Value=%d)\n",ep->num,value);
  //*********** Process the EP-0 SetHalt *******************
	if (ep->num == 0) 
	{
		if (value==1)
		{    // protocol stall, need H/W to reset
			mUsbEP0StallSet();
		} 
		else if (value==2) 
		{   // function stall, SW to set/clear, nad EP0 work normally 
			ep->dev->ep0state = EP0_STALL;
			ep->dev->ep[0].stopped = 1;
		} 
		else if (value==0) 
		{    // clear function stall, SW to set/clear, nad EP0 work normally 
			ep->dev->ep0state = EP0_IDLE;
			ep->dev->ep[0].stopped = 0;
		} 		
		return retval; //EP0 Set Halt will return here		
	} /* don't change EPxSTATUS_EP_INVALID to READY */
	else if (!ep->desc) 
	{
		DBG(ep->dev, "%s %s inactive?\n", __FUNCTION__, ep->ep.name);
		return -EINVAL;
	}

	//*********** Process the EP-X SetHalt *******************
  
	spin_lock_irqsave(&ep->dev->lock, flags);
	if (!list_empty(&ep->queue))   // something in queue 
		retval = -EAGAIN;
	else if (!value)
		FTC_clear_halt(ep);
	else 
	{
		ep->stopped = 1;
		VDBG(ep->dev, "%s set halt\n", ep->ep.name);

		if (ep->is_in)	// IN direction ?
			{
			printk("set in stall bit\n");
			mUsbEPinStallSet(ep->num);		// Set in Stall Bit
			}
		else
			{
			printk("set out stall bit\n");			
			mUsbEPoutStallSet(ep->num);		// Set out Stall Bit
			}
	}
	spin_unlock_irqrestore(&ep->dev->lock, flags);
	return retval;
}



//********************************************************
//Name: FTC_fifo_status 
//Description: 
//
//********************************************************
static int FTC_fifo_status(struct usb_ep *_ep)
{
	struct FTC_ep *ep;
	u8     u8fifo_n;   //john
	u32	   size;

	DBG_FUNCC("+FTC_fifo_status()\n");

	if (!_ep)
		return -ENODEV;
	ep = container_of(_ep, struct FTC_ep, ep);

	DBG_BULKK("FTC_udc-->FTC_fifo_status-->Check (size is only reported sanely for OUT)");
	/* size is only reported sanely for OUT */
	if (ep->is_in)
	{
		DBG_BULKK("FTC_udc-->FTC_fifo_status-->return -EOPNOTSUPP (ep->is_in)");
		return -EOPNOTSUPP;
	}

	//John for FUSB220
	if (ep->num ==0) 
	{   	//EP0 
		// note : for EP0, only know empty or not
		size = !mUsbEP0EMPFIFO();   
	}
	else 
	{
		DBG_BULKK("FTC_udc-->FTC_fifo_status-->ep->num >0 ");

		u8fifo_n = mUsbEPMapRd(ep->num);		// get the relatived FIFO number
		if (ep->is_in)
			u8fifo_n &= 0x0F;
		else
			u8fifo_n >>= 4;
		if (u8fifo_n >= FUSB220_MAX_FIFO)			// over the Max. fifo count ?
			return -ENOBUFS;

		// Check the FIFO had been enable ?
		if ((mUsbFIFOConfigRd(u8fifo_n) & 0x80) == 0)
			return -ENOBUFS;

		size = mUsbFIFOOutByteCount(u8fifo_n);
		VDBG(ep->dev, "%s %s %u\n", __FUNCTION__, ep->ep.name, size);
	}
	return size;
}

static void FTC_fifo_flush(struct usb_ep *_ep)
{
	struct FTC_ep *ep;
	u8     u8fifo_n;   //john

	DBG_FUNCC("+FTC_fifo_flush()\n");

	if (!_ep)
		return;
	ep = container_of(_ep, struct FTC_ep, ep);
	VDBG(ep->dev, "%s %s\n", __FUNCTION__, ep->ep.name);

	/* don't change EPxSTATUS_EP_INVALID to READY */
	if (!ep->desc && ep->num != 0) 
	{
		DBG(ep->dev, "%s %s inactive?\n", __FUNCTION__, ep->ep.name);
		return;
	}

	//John for FUSB220
	if (ep->num ==0) 
	{   //EP0 
	       mUsbEP0ClearFIFO();
	}
	else 
	{
		u8fifo_n = mUsbEPMapRd(ep->num);		// get the relatived FIFO number
		if (ep->is_in)
			u8fifo_n &= 0x0F;
		else
			u8fifo_n >>= 4;
		if (u8fifo_n >= FUSB220_MAX_FIFO)	// over the Max. fifo count ?
			return;

		// Check the FIFO had been enable ?
		if ((mUsbFIFOConfigRd(u8fifo_n) & 0x80) == 0)
			return;

		mUsbFIFOReset(u8fifo_n);   	//reset FIFO
		udelay(10);
		mUsbFIFOResetOK(u8fifo_n);   //reset FIFO finish
	}
	return;
}

static struct usb_ep_ops FTC_ep_ops = 
{
	.enable	= FTC_ep_enable,
	.disable	= FTC_ep_disable,

	.alloc_request	= FTC_alloc_request,
	.free_request	= FTC_free_request,

	//.alloc_buffer	= FTC_alloc_buffer,
	//.free_buffer	= FTC_free_buffer,

	.queue	= FTC_queue,
	.dequeue	= FTC_dequeue,

	.set_halt	 = FTC_set_halt,
	.fifo_status = FTC_fifo_status,
	.fifo_flush	 = FTC_fifo_flush,
};

/*-------------------------------------------------------------------------*/

static int FTC_get_frame(struct usb_gadget *_gadget)
{
	struct FTC_udc	*dev;
	u16 retval;
	unsigned long	flags;

	DBG_FUNCC("+FTC_get_frame()\n");

	if (!_gadget)
		return -ENODEV;
	dev = container_of (_gadget, struct FTC_udc, gadget);
	spin_lock_irqsave (&dev->lock, flags);
	retval = ( (mUsbFrameNoHigh() & 0x07) <<8) | mUsbFrameNoLow();
	spin_unlock_irqrestore (&dev->lock, flags);

	return retval;
}

static int FTC_wakeup(struct usb_gadget *_gadget)
{
	struct FTC_udc	*dev;
	unsigned long	flags;
	DBG_FUNCC("+FTC_wakeup()\n");

	if (!_gadget)
		return -ENODEV;
	dev = container_of (_gadget, struct FTC_udc, gadget);
	spin_lock_irqsave (&dev->lock, flags);

	// Set "Device_Remote_Wakeup", Turn on the"RMWKUP" bit in Mode Register
	mUsbRmWkupSet();
	spin_unlock_irqrestore (&dev->lock, flags);
	return 0;
}

static int FTC_set_selfpowered(struct usb_gadget *_gadget, int value)
{
	DBG_FUNCC("+FTC_set_selfpowered()\n");
	return -EOPNOTSUPP;
}

static int FTC_ioctl(struct usb_gadget *_gadget, unsigned code, unsigned long param)
{
	unsigned long	flags;
	struct FTC_udc	*dev;
	struct FTC_ep	*ep;
	struct usb_ep   *_ep;

	DBG_FUNCC("+FTC_ioctl()\n");

	if (!_gadget)
		return -ENODEV;
	dev = container_of (_gadget, struct FTC_udc, gadget);
	spin_lock_irqsave (&dev->lock, flags);

	switch (code) 
	{
		case 1:   //DMA enable from others
			_ep = (struct usb_ep *)param;       
			ep = container_of(_ep, struct FTC_ep, ep);
			ep->dma=1;
			break;
		case 2:   //DMA disable from others
			_ep = (struct usb_ep *)param;       
			ep = container_of(_ep, struct FTC_ep, ep);
			ep->dma=0;
			break;
		default:
			break;
	}

	spin_unlock_irqrestore (&dev->lock, flags);
	return -EOPNOTSUPP;
}

static const struct usb_gadget_ops FTC_ops = 
{
	.get_frame	     	= FTC_get_frame,
	.wakeup		     	= FTC_wakeup,
	.set_selfpowered 	= FTC_set_selfpowered,
	.ioctl          	= FTC_ioctl,
};

#ifndef CONFIG_GM_FUSB220
#include "fotg2xx_udc.c"
#else
#include "fusb220_udc.c"
#endif

/*-------------------------------------------------------------------------*/
static void udc_reinit (struct FTC_udc *dev)
{
	unsigned i;

	DBG_FUNCC("+udc_reinit()\n");

	INIT_LIST_HEAD (&dev->gadget.ep_list);
	dev->gadget.ep0 = &dev->ep[0].ep;
	dev->gadget.speed = USB_SPEED_UNKNOWN;
	dev->ep0state = EP0_DISCONNECT;
	dev->irqs = 0;

	for (i = 0; i < FUSB220_CURRENT_SUPPORT_EP; i++) 
	{
		struct FTC_ep	*ep = &dev->ep[i];
		
		ep->num = i;
		ep->ep.name = names[i];
		DBG_CTRLL("EP%d Name = %s \n",i, ep->ep.name);
		
		ep->ep.ops = &FTC_ep_ops;
		list_add_tail (&ep->ep.ep_list, &dev->gadget.ep_list);
		ep->dev = dev;
		INIT_LIST_HEAD (&ep->queue);
		ep_reset(ep);
	}
	for (i = 0; i < FUSB220_CURRENT_SUPPORT_EP; i++) 
		dev->ep[i].irqs = 0;

	dev->ep[0].ep.maxpacket = MAX_EP0_SIZE;
	list_del_init (&dev->ep[0].ep.ep_list);
	INFO(dev,"L%x: GM UDC reinit finish...\n", dev->u8LineCount ++);
}

static void udc_reset(struct FTC_udc *dev)
{
	DBG_FUNCC("+udc_reset()\n");

	//default value
	dev->Dma_Status = PIO_Mode;
	dev->u8LineCount = 0;
#ifdef CONFIG_GM_FUSB220	
	INFO(dev,"***** FTC USB Device 2.0 (FUSB220) Linux Lower Driver *****\n");
#else
	INFO(dev,"***** FTC OTG controller Device Linux Lower Driver *****\n");
#endif
	INFO(dev,"L%x: System initial, Please wait...\n", dev->u8LineCount ++);

	// initial Reg setup
#ifdef CONFIG_GM_FUSB220
	mUsbIntRdBufErrDis();		// Disable Read buffer error interrupt (for AXD memory table) 0x11 BIT5
	mUsbIntBufEmptyDis();		// 0x18 BIT0
#endif
	mUsbTstHalfSpeedDis();		// Set for FPGA Testing 0x02 BIT7 
	mUsbUnPLGClr();				// 0x08 BIT0
 	vUsbInit(dev);

	INFO(dev,"L%x: System reset finish...\n", dev->u8LineCount ++);
	//INFO(dev,"\nInterrupt Mask:0x%x\n",bFUSBPort(0x10));
}

static void udc_enable(struct FTC_udc *dev)
{
	DBG_FUNCC("+udc_enable()\n");

	// Enable usb200 global interrupt
	mUsbGlobIntEnSet();
	mUsbChipEnSet();
}

static irqreturn_t FUSBD_irq(int irq, void *_dev)  //, struct pt_regs *r)
{
	struct FTC_udc	*dev = _dev;
	u32	   handled = 0;

#ifndef CONFIG_GM_FUSB220
	u32 status;
	status = mdwOTGC_INT_STS_Rd();

	if (status & (OTGC_INT_A_TYPE | OTGC_INT_B_TYPE)) {
		fotg200_handle_irq();
	}
#endif

	spin_lock(&dev->lock);
	dev->usb_interrupt_level1_Save = mUsbIntGroupRegRd();
	dev->usb_interrupt_level1_Mask = mUsbIntGroupMaskRd();
	dev->usb_interrupt_level1 = dev->usb_interrupt_level1_Save & ~dev->usb_interrupt_level1_Mask;

	
#ifdef CONFIG_GM_FUSB220
	if (dev->usb_interrupt_level1 != 0 ){
#else
        handled = 1;
	if (dev->usb_interrupt_level1 != 0 && mdwOTGC_Control_CROLE_Rd() != OTG_ROLE_HOST){
#endif 
		/* fotg2xx_dbg("FTC_irq %x \n",dev->usb_interrupt_level1);*/
		dev->irqs++;
		handled = 1;
		//ib_DisableInt(IRQ_USBDEV);

		//INFO(dev,"\nInterrupt Source:0x%x\n",bFUSBPort(0x20));
		vUsbHandler(dev);
		// Clear usb interrupt flags
		dev->usb_interrupt_level1 = 0;
		//ib_EnableInt(IRQ_USBDEV);
	}

	spin_unlock(&dev->lock);
	return IRQ_RETVAL(handled);
}

/* tear down the binding between this driver and the pci device */
//Trace ok 12212004
static void FTC_usb_remove(void)
{
	DBG_FUNCC("+FTC_usb_remove()\n");
	DBG(the_controller, "%s\n", __FUNCTION__);

	/* start with the driver above us */
	if (the_controller->driver) 
	{
		/* should have been done already by driver model core */
		UWARN(dev,"remove driver '%s' is still registered\n",
				the_controller->driver->driver.name);
		usb_gadget_unregister_driver(the_controller->driver);
	}
	udc_reset(the_controller);

	if (the_controller->got_irq) {//Andrew update
		free_irq(IRQ_USBDEV, the_controller);		//Andrew update
		the_controller->got_irq = 0;
	}

	device_unregister (&(the_controller->gadget.dev));
#if 0	//Andrew update
	// free EP0 req, buffer
	FTC_free_buffer (&the_controller->ep[0].ep, 
					the_controller->EP0req->buf, 
					the_controller->EP0req->dma, the_controller->EP0req->length);
	FTC_free_request (&the_controller->ep[0].ep, the_controller->EP0req);
#endif

	kfree(the_controller);	//Andrew update
	kfree(the_host);
	
	the_controller = NULL;
	the_host = NULL;
#if defined(CONFIG_PLATFORM_GM8126)
	//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0xA0) &= ~BIT3; // set to host mode
	ftpmu010_write_reg(usb_get_pmu_fd(), 0xA0, 0, BIT3);
#endif
	INFO(dev,"USB device unbind\n");
}

/*-------------------------------------------------------------------------*/

/* keeping it simple:
 * - one bus driver, initted first;
 * - one function driver, initted second
 */

/* when a driver is successfully registered, it will receive
 * control requests including set_configuration(), which enables
 * non-control requests.  then usb traffic follows until a
 * disconnect is reported.  then a host may connect again, or
 * the driver might get unbound.
 */
int usb_gadget_register_driver(struct usb_gadget_driver *driver)
{
	struct FTC_udc	*dev = the_controller;
	int	   retval; 
	DBG_FUNCC("+usb_gadget_register_driver()\n");

#ifndef CONFIG_GM_FUSB220
	fotg2xx_dbg("usb_gadget_register_driver %p==>  drv:%p\n",dev,driver); 
#endif

	if (!dev)
		return -ENODEV;
	if (driver->speed != USB_SPEED_HIGH || 
			!driver->bind ||
			!driver->unbind ||
			!driver->disconnect ||
			!driver->setup){
		printk(" Error ==> spd:%x(%x) bnd:%p unbnd:%p dis:%p set:%p \n",driver->speed,USB_SPEED_HIGH, \
				driver->bind,driver->unbind,driver->disconnect,driver->setup);
		return -EINVAL;
	}
	if (dev->driver)
		return -EBUSY;

	/* Disable Global Interrupt to prevent useless irq */
	mUsbGlobIntDis();

	/* hook up the driver */
	dev->driver = driver;
	retval = driver->bind(&dev->gadget);
	if (retval) {
		printk("bind to driver %s --> error %d\n",
				driver->driver.name, retval);
		dev->driver = 0;
		return retval;
	}

	/* then enable host detection and ep0; and we're ready
	 * for set_configuration as well as eventual disconnect.
	 */

#if defined (CONFIG_GM_FOTG2XX)
	mUsbUnPLGClr();
#ifndef CONFIG_USB_GADGET
	*((volatile u32 *) ( FOTG200_REGBASE | 0x134)) = 0;
	*((volatile u32 *) ( FOTG200_REGBASE | 0x138)) = 0;
	*((volatile u32 *) ( FOTG200_REGBASE | 0x13C)) = 0;
#endif
#endif
#ifndef CONFIG_GM_FUSB220	
	vUsbInit(dev);
#endif
	udc_enable(dev);

	DBG(dev, "registered gadget driver '%s'\n", driver->driver.name);
	DBG_FUNCC("-usb_gadget_register_driver()\n");
	return 0;
}
EXPORT_SYMBOL(usb_gadget_register_driver);


static void
stop_activity(struct FTC_udc *dev, struct usb_gadget_driver *driver)
{
	unsigned	i;

	DBG (dev, "%s\n", __FUNCTION__);

	if (dev->gadget.speed == USB_SPEED_UNKNOWN)
		driver = 0;

	// disconnect gadget driver after quiesceing hw and the driver 
	udc_reset (dev);
	for (i = 0; i < FUSB220_CURRENT_SUPPORT_EP; i++)
		nuke(&dev->ep [i], -ESHUTDOWN);
	if (driver) {
		spin_unlock(&dev->lock);
		driver->disconnect(&dev->gadget);
		spin_lock(&dev->lock);
	}

	if (dev->driver)
		udc_enable(dev);
}


int usb_gadget_unregister_driver(struct usb_gadget_driver *driver)
{
	struct FTC_udc	*dev = the_controller;
	unsigned long	flags;
	DBG_FUNCC("+usb_gadget_unregister_driver()\n");

	if (!dev)
		return -ENODEV;
	if (!driver || driver != dev->driver)
		return -EINVAL;
	spin_lock_irqsave(&dev->lock, flags);
	stop_activity(dev, driver);
	spin_unlock_irqrestore(&dev->lock, flags);
	driver->unbind(&dev->gadget);
	dev->gadget.dev.driver = NULL;
	dev->driver = NULL;

	DBG(dev, "unregistered driver '%s'\n", driver->driver.name);
	return 0;
}
EXPORT_SYMBOL(usb_gadget_unregister_driver);

void release_dummy(struct device *dev)
{
   return;
}
/* wrap this driver around the specified pci device, but
 * don't respond over USB until a gadget driver binds to us.
 */
//Trace ok 12212004
static int FTC_usb_probe(void)
{
	int	retval=0;
	DBG_FUNCC("+FTC_usb_probe()\n");

	//<1>.Init "the_controller" structure
	/* if you want to support more than one controller in a system,
 	 * usb_gadget_driver_{register,unregister}() must change.
	 */
	/*if (the_controller) 
	{
		UWARN(dev,"ignoring : more than one device\n");
		return -EBUSY;
	}*/

	/* alloc, and start init */
	spin_lock_init(&the_controller->lock);
	the_controller->gadget.ops = &FTC_ops;

	/* the "gadget" abstracts/virtualizes the controller */
	the_controller->gadget.name = driver_name;
	the_controller->enabled = 1;
	the_controller->EPUseDMA = FUSB220_DMA_IS_IDLE_NOW;   //reset
	the_controller->ReqForDMA= 0;   //reset
#ifdef CONFIG_GM_FUSB220

#ifdef CONFIG_GM_FUSB220_DMA
	/* allocate one channel for DMA fa_request_ahb_dma_auto*/
	if (fa_request_ahb_dma_auto(USB_GADGET_DEVICE_ID, driver_name,
		 vCheckDMA, the_controller, &(the_controller->chForDMA), 0) != 0)
	   printk(KERN_ERR "Request AHB DMA Channel resource fail");
	else
	   printk(KERN_ERR "Request AHB DMA resource success\n");
	//fa_init_ahb_dma
       fa_ahb_dma_init(the_controller->chForDMA, 11); //req_pair : 11 for USB 2.0 device
#endif
	
	the_controller->gadget.dev.parent = NULL;
        strcpy (the_controller->gadget.dev.bus_id, "fusb220");
	the_controller->gadget.dev.dma_mask = (void *)0xffffffff;
	the_controller->gadget.dev.coherent_dma_mask = 0xffffffff;
#else //not use FUSB220
	the_controller->gadget.dev.parent = NULL;
      	strcpy (the_controller->gadget.dev.bus_id, "FOTG200-Device");
	the_controller->gadget.dev.dma_mask = (void *)0xffffffff;
	the_controller->gadget.dev.coherent_dma_mask = 0xffffffff;
	retval = device_register (&(the_controller->gadget.dev));
	the_controller->gadget.dev.release = &release_dummy;
#endif 

#ifdef CONFIG_USB_GADGET_DUALSPEED
	the_controller->gadget.is_dualspeed		= 1;
#else
	the_controller->gadget.is_dualspeed		= 0;
#endif

#if defined(CONFIG_PLATFORM_GM8181)
	if ((ft_get_version() & 0xF0) == 0x20) { /* 8181T/8186 */
/*
		* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) &= ~(0x7<<20);
		* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) |= (0x2<<20); // config reference voltage to 120mV
		* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) |= BIT29; // enable VBUS input
		* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0x6C) |= BIT28; // set to device mode
*/
		ftpmu010_write_reg(usb_get_pmu_fd(), 0x6C, 0, (BIT0|BIT1|BIT2)<<20);
		ftpmu010_write_reg(usb_get_pmu_fd(), 0x6C, BIT1<<20, BIT1<<20);
		ftpmu010_write_reg(usb_get_pmu_fd(), 0x6C, BIT29, BIT29);
		ftpmu010_write_reg(usb_get_pmu_fd(), 0x6C, BIT28, BIT28);
	}
#elif defined(CONFIG_PLATFORM_GM8126)
	//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0xA0) &= ~(0x7<<17);
	//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0xA0) |= (0x2<<17); // config reference voltage to 120mV
	//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0xA0) |= BIT0; // enable VBUS input
	//* (volatile unsigned int *) (PMU_FTPMU010_VA_BASE + 0xA0) |= BIT3; // set to device mode
	ftpmu010_write_reg(usb_get_pmu_fd(), 0xA0, 0, (BIT0|BIT1|BIT2)<<17);
	ftpmu010_write_reg(usb_get_pmu_fd(), 0xA0, BIT1<<17, BIT1<<17);
	ftpmu010_write_reg(usb_get_pmu_fd(), 0xA0, BIT0, BIT0);
	ftpmu010_write_reg(usb_get_pmu_fd(), 0xA0, BIT3, BIT3);
#endif
	//<2>. udc Reset/udc reinit
	/* init to known state, then setup irqs */
	udc_reset(the_controller);
	udc_reinit(the_controller);
	
	//<3>.Init USB DEV ISR
	if (!the_controller->got_irq) {
		if (request_irq(IRQ_USBDEV, FUSBD_irq, IRQF_SHARED, driver_name, the_controller) != 0) {
			UWARN(dev, "request interrupt failed\n");
			retval = -EBUSY;
			goto done;
		}
		the_controller->got_irq = 1;
	}
	printk("Init GM UDC ISR finished\n");
	
#if defined(CONFIG_GM_FUSB220)
	retval = device_register (&the_controller->gadget.dev);
#endif

	mUsbUnPLGSet();

	/* done */
	return 0;

done:
	DBG_TEMP("FTC_usb_probe() failed\n");
	if (the_controller)
		FTC_usb_remove();
	return retval;
}

void FTC_set_USB_DevCtrl(struct FTC_udc *ctrl)
{
	the_controller = ctrl;
#ifndef CONFIG_GM_FUSB220
	fotg2xx_dbg("fusb220_set_controller => ptr:%p\n",the_controller);
#endif
}	


/*-------------------------------------------------------------------------*/
//Trace ok 12212004
static int __init init (void)
{
#ifndef CONFIG_GM_FUSB220
	INFO(dev,"device init ... %x\n",0);
#else
#ifdef CONFIG_GM_FUSB220_DMA
	INFO(dev,"init USB device Lower driver (DMA mode)\n");
#else
	INFO(dev,"init USB device Lower driver (PIO mode)\n");
#endif
#endif
	
#ifdef CONFIG_GM_FUSB220
	INFO(dev,"FUSB220_BASE_ADDRESS = 0x%x\n", FUSB220_BASE_ADDRESS);
	INFO(dev,"FUSSB220 USB Device Hardware controller\n");
	the_controller = kzalloc (sizeof *the_controller, GFP_KERNEL);
	if (the_controller == NULL)
	{
		printk("Error ==> FUSB2XX Device Initiation.\n");
		return -ENOMEM;
	}
#else //not use FUSB220
	mwOTG20_Interrupt_OutPut_High_Set();
	the_controller = kzalloc (sizeof *the_controller, GFP_KERNEL);
	the_host = kzalloc (sizeof(struct usb_bus), GFP_KERNEL);
	if (the_controller == NULL) {
		printk("Error ==> FOTG2XX Device part Initiation.\n");
		return -ENOMEM;
	}
	if (the_host == NULL) {
		printk("Error ==> FOTG2XX Host part Initiation.\n");
		return -ENOMEM;
	}
	if (!is_init_FOTG2XX_Dev()) {
		init_FOTG2XX_Dev();
	}
	if (!is_fotg200_init()) {
		fotg200_init(&(the_controller->gadget.dev),the_host,NULL);
	}
#endif
	return FTC_usb_probe();
}

module_init (init);

//Trace ok 12212004
static void __exit cleanup (void)
{
	INFO(dev,"remove USB device Lower driver\n");
	return FTC_usb_remove();
}

module_exit (cleanup);

MODULE_AUTHOR("<GM-tech.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("USB device Driver");
//EXPORT_SYMBOL(FTC_alloc_buffer);
//EXPORT_SYMBOL(FTC_free_buffer);



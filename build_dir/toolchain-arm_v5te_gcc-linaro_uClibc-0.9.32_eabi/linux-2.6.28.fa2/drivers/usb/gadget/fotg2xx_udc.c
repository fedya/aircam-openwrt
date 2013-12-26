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

#define DEV_MAJOR           41	//major device for Device
#define AP_CMD_ENABLE_DEV   5

extern int devfs_register_chrdev(unsigned int major, const char *name,
				 struct file_operations *fops);
//static int DEVC_AP_ioctl(struct inode * inode, struct file * file,unsigned int cmd,unsigned long arg);

#if 0
static struct file_operations dev_fops = {
owner:	THIS_MODULE,
ioctl:	DEVC_AP_ioctl,
};

//*********************************************************
// Name: DEVC_AP_ioctl
// Description: Request from user mode AP
// Input:struct inode * inode, struct file * file
//      ,unsigned int cmd,unsigned long arg
// Output: int
//*********************************************************
static int DEVC_AP_ioctl(struct inode *inode, struct file *file, u32 cmd,
			 unsigned long arg)
{
	DBG_FUNCC("+DEVC_AP_ioctl()\n");

	switch (cmd) {
	case AP_CMD_ENABLE_DEV:
		mUsbChipEnSet();
		mUsbGlobIntEnSet();
		return (0);
		break;
	default:
		break;
	}
	return 1;
}
#endif

/*-------------------------------------------------------------------------*/
//*******************************************************************
// Name:Get_FIFO_Num
// Description:get the FIFO number from ep
//             FIFO0=0
//             FIFO1=1
//             FIFO2=2
//             FIFO3=3
//             FIFO-CX = 0xFF
//*******************************************************************
static u8 Get_FIFO_Num(struct FTC_ep *ep)
{
	u8 u8fifo_n;

	if (ep->num == 0) {
		u8fifo_n = FIFOCX;
		return (u8fifo_n);
	}

	u8fifo_n = mUsbEPMapRd(ep->num);	// get the relatived FIFO number
	if (ep->is_in)
		u8fifo_n &= 0x0F;
	else
		u8fifo_n >>= 4;

	if (u8fifo_n >= MAX_FIFO_NUM)	// over the Max. fifo count ?
		printk("??? Error ep > FUSB200_MAX_FIFO \n");

	return (u8fifo_n);
}

/*-------------------------------------------------------------------------*/
//****************************************************
// Name:CX_dma_Directly
// Description: Start DMA directly
//              <1>.For Control Command - Get Stayus (Only for ep0)
// Input:<1>.For Control Command - Get Stayus
//       <2>.status
// Output:none
//****************************************************
static int CX_dma_Directly(u8 * pu8Buffer, u32 u8Num, u8 bdir)
{
	u32 FIFO_Sel, wTemp, wDMABuffer, temp;
	u8 u8fifo_n;
	struct FTC_udc *dev = the_controller;

	DBG_FUNCC("+CX_dma_Directly, start addr = 0x%x len=0x%x, dir =%x\n",
		  (u32) pu8Buffer, u8Num, bdir);

	//<1>.Get the FIFO Select
	u8fifo_n = 0;
	FIFO_Sel = FOTG200_DMA2CxFIFO;

	//<2>.Map the DMA Buffer
	temp = (u8Num + 15) / 16;
	temp = temp * 16;
	wDMABuffer = dma_map_single((void *)dev, pu8Buffer, temp,	//USB_EPX_BUFSIZ
				    bdir ? DMA_TO_DEVICE : DMA_FROM_DEVICE);

	//<3>.Init DMA start register
	// EP=0,1,2,3,4
	/*if (bdir)
	   mUsbDmaConfig(u8Num,DIRECTION_IN);
	   else
	   mUsbDmaConfig(u8Num,DIRECTION_OUT); */
	if (bdir)
		mUsbDmaConfig(u8Num, DIRECTION_OUT);
	else
		mUsbDmaConfig(u8Num, DIRECTION_IN);

	mUsbDMA2FIFOSel(FIFO_Sel);
	mUsbDmaAddr((u32) wDMABuffer);

	//<4>.Enable the DMA
	mUsbDmaStart();

	//<5>.Waiting for DMA complete
	while (1) {
		wTemp = mUsbIntSrc2Rd();
		if (wTemp & BIT8) {
			mUsbIntDmaErrClr();
			printk("??? Cx IN DMA error..");
			break;
		}
		if (wTemp & BIT7) {
			mUsbIntDmaFinishClr();
			break;
		}
		if ((wTemp & 0x00000007) > 0) {	//If (Resume/Suspend/Reset) exit
			mUsbIntDmaFinishClr();
			printk
			    ("???? Cx IN DMA stop because USB Resume/Suspend/Reset..");
			break;
		}
	}
	mUsbDMA2FIFOSel(FOTG200_DMA2FIFO_Non);

	//<6>.Unmap the DMA
	dma_unmap_single((void *)dev, wDMABuffer, temp,	//USB_EPX_BUFSIZ,  //req->req.length+32,
			 bdir ? DMA_TO_DEVICE : DMA_FROM_DEVICE);
	return 0;
}

//****************************************************
// Name:start_dma
// Description: Start the DMA
//              For FOTG200-Peripheral HW:
//              <1>.Control-8 byte command => Only PIO
//              <2>.Others => Only DMA
//
// Input:<1>.ep structure point
//       <2>.status
// Output:none
//****************************************************
// return:  0 = q running, 1 = q stopped, negative = errno
static int start_dma(struct FTC_ep *ep, struct FTC_request *req)
{
	struct FTC_udc *dev = ep->dev;
	u32 start = req->req.dma;
	u32 FIFO_Sel = 0;
	u8 u8fifo_n;

	DBG_FUNCC("+start_dma, start addr = 0x%x len = 0x%x\n", start,
		  req->req.length);

	//<0>. 
	//<1>.Get the FIFO Select
	u8fifo_n = Get_FIFO_Num(ep);
	if (u8fifo_n == FIFOCX)
		FIFO_Sel = FOTG200_DMA2CxFIFO;
	else
		FIFO_Sel = 1 << (u8fifo_n);

	//<2>.Init DMA start register
	// EP=0,1,2,3,4
	if (likely(ep->is_in)) {
		if (req->req.length == 0) {
			if ((ep->num) == 0)
				mUsbEP0DoneSet();
			else
				mUsbEPinZeroSet(ep->num);
			done(ep, req, 0);
			return 0;
		} else {
			if (req->req.length <= 64) {
				ep->wDMA_Set_Length = req->req.length;
				mUsbDmaConfig(req->req.length, DIRECTION_IN);
			} else {
				if (ep->num == 0) {
					ep->wDMA_Set_Length = 64;
					mUsbDmaConfig(64, DIRECTION_IN);
				} else {
					if (req->req.length >= 0x01FFFF)
						req->req.length = (0x01FFFF-1);
					ep->wDMA_Set_Length = req->req.length;
					mUsbDmaConfig(req->req.length, DIRECTION_IN);

				}
			}
		}
	} else {
		if ((ep->num) == 0)
			mUsbDmaConfig(req->req.length, DIRECTION_OUT);
		else {		//For EP1~EP4
			//Read the Byte Counter
			if (mUsbFIFOOutByteCount(((ep->num) - 1)) == 0)
				return 0;
			DBG_CTRLL
			    (">>>start_dma ==>(mUsbFIFOOutByteCount=0x%x(ep->num=0x%x)(MaxPackSize=0x%x))\n",
			     mUsbFIFOOutByteCount(((ep->num) - 1)), ep->num,
			     mUsbEPMxPtSzRd(((ep->num)), DIRECTION_OUT));

			if (mUsbFIFOOutByteCount(((ep->num) - 1)) <
			    (mUsbEPMxPtSzRd(((ep->num)), DIRECTION_OUT))) {
				ep->wDMA_Set_Length =
				    mUsbFIFOOutByteCount(((ep->num) - 1));
				mUsbDmaConfig((ep->wDMA_Set_Length),
					      DIRECTION_OUT);
			} else {
				ep->wDMA_Set_Length = req->req.length;
				mUsbDmaConfig(ep->wDMA_Set_Length,
					      DIRECTION_OUT);
			}
		}
	}

	mUsbDMA2FIFOSel(FIFO_Sel);
	mUsbDmaAddr(start);
	DBG_CTRLL(">>>(mUsbDmaConfigRd=0x%x(Request Length=0x%x))\n",
		  mUsbDmaConfigRd(), req->req.length);
	DBG_CTRLL(">>>(mUsbDMA2FIFOSel=0x%x)\n", mUsbDMA2FIFORd());
	DBG_CTRLL(">>>(mUsbDmaAddr=0x%x)\n", mUsbDmaAddrRd());

	//<3>.Record who use the DMA chanel(In use)
	dev->EPUseDMA = ep->num;

	//<4>.Disable FIFO-Interrupt
	// If use DMA, no FIFO interrupt for FIFO
	//FIFO_Interrupt(ep,0);

	//<5>.Enable the DMA Interrupt
	mUsbIntDmaErrEn();
	mUsbIntDmaFinishEn();

	//<6>.Enable the DMA
	mUsbDmaStart();

	DBG_FUNCC("-start_dma...\n");
	return 0;
}

//****************************************************
// Name:dma_advance
// Description: After finish DMA
// Input:<1>.dev structure pointer
//       <2>.ep structure pointer
// Output:none
//****************************************************
static void dma_advance(struct FTC_udc *dev, struct FTC_ep *ep)
{
	struct FTC_request *req;

	DBG_FUNCC("+dma_advance\n");

	if (unlikely(list_empty(&ep->queue))) {

stop:				//Disable DMA)
		mUsbDmaStop();
		mUsbIntDmaErrDis();
		mUsbIntDmaFinishDis();
		mUsbDMA2FIFOSel(FOTG200_DMA2FIFO_Non);
		dev->EPUseDMA = DMA_CHANEL_FREE;

		if (unlikely(ep->num)) {	//ep->num>0
			//Disable the FIFO-Interrupt (ep->Num>0)
			if (likely(ep->is_in))
				mUsbIntFXINDis(((ep->num) - 1));
			else
				mUsbIntFXOUTDis(((ep->num) - 1));
		}
		return;
	}

	req = list_entry(ep->queue.next, struct FTC_request, queue);

	//<2>.Get length
	/* normal hw dma completion (not abort) */
	if (mUsbIntDmaErrRd() == 0) {
		req->req.actual += ep->wDMA_Set_Length;
		DBG_CTRLL(">>> dma_advance=>req->req.actual=0x%x  \n",
			  req->req.actual);

		// only control EP & IN will have such condition
		if (likely(ep->is_in)) {
			if (req->req.length > req->req.actual) {
				ep->wDMA_Set_Length =
					req->req.length - req->req.actual;
				if (ep->num == 0) {
					if (ep->wDMA_Set_Length > 64)
						ep->wDMA_Set_Length = 64;
				} else {
					if (ep->wDMA_Set_Length >= 0x01FFFF)
						ep->wDMA_Set_Length = (0x01FFFF-1);//Max. DMA length = 128KB-1
				}
				mUsbDmaConfig(ep->wDMA_Set_Length,
						DIRECTION_IN);
				mUsbDmaAddr(req->req.dma + req->req.actual);
				dev->EPUseDMA = ep->num;

				mUsbIntDmaErrEn();
				mUsbIntDmaFinishEn();

				mUsbDmaStart();
				return;
			}
		}
	} else {
		printk("??? DMA Error...\n");
		req->req.actual = 0;
	}

#ifdef USB_TRACE
	VDBG(dev, "done %s %s dma, %u/%u bytes, req %p\n",
	     ep->ep.name, ep->is_in ? "IN" : "OUT",
	     req->req.actual, req->req.length, req);
#endif

	//<3>.Done the request
	done(ep, req, 0);

	if (list_empty(&ep->queue))
		goto stop;

	//<4>.Start another req DMA
	if (ep->num == 0) {
		req = list_entry(ep->queue.next, struct FTC_request, queue);
		(void)start_dma(ep, req);
	} else {
		//<1>.Free the DMA resource => Waiting for next DMA-Start
		mUsbDmaStop();
		mUsbIntDmaErrDis();
		mUsbIntDmaFinishDis();
		mUsbDMA2FIFOSel(FOTG200_DMA2FIFO_Non);
		dev->EPUseDMA = DMA_CHANEL_FREE;

		//<2>.open the interrupt
		if (likely(ep->is_in))
			mUsbIntFXINEn(((ep->num) - 1));	//Enable the Bulk-In
		else
			mUsbIntFXOUTEn(((ep->num) - 1));	//Enable the Bulk-Out
	}
}

#if 0
//****************************************************
// Name:abort_dma
// Description: In FOTG200 abort_dma = reset dma
// Input:<1>.ep structure pointer
//       <2>.Status
// Output:none
//****************************************************
static void abort_dma(struct FTC_ep *ep, int status)
{
	struct FTC_request *req;
	struct FTC_udc *dev;
	u8 u8fifo_n;

	DBG_FUNCC("+abort_dma\n");

	req = list_entry(ep->queue.next, struct FTC_request, queue);

	/* FIXME using these resets isn't usably documented. this may
	 * not work unless it's followed by disabling the endpoint.
	 *
	 * FIXME the OUT reset path doesn't even behave consistently.
	 */

	//<1>.Checking => Finish
	if (mUsbIntDmaFinishRd() > 0)
		goto finished;

	req->req.status = status;

	VDBG(ep->dev, "%s %s %s %d/%d\n", __FUNCTION__, ep->ep.name,
	     ep->is_in ? "IN" : "OUT", req->req.actual, req->req.length);

	mUsbDMAResetSet();

	u8fifo_n = Get_FIFO_Num(ep);
	if (u8fifo_n == FIFOCX)
		mUsbCxFClr();
	else
		mUsbFIFOReset(u8fifo_n);
	return;

finished:
	///* dma already completed; no abort needed */
	//command(regs, COMMAND_FIFO_ENABLE, ep->num);
	req->req.actual = req->req.length;
	req->req.status = 0;

	mUsbDmaStop();
	mUsbIntDmaErrDis();
	mUsbIntDmaFinishDis();
	mUsbDMA2FIFOSel(FOTG200_DMA2FIFO_Non);

	dev = ep->dev;
	dev->EPUseDMA = DMA_CHANEL_FREE;
}
#endif
//****************************************************
// Name:FTC_queue
// Description:
// Input:<1>.ep structure point
//       <2>.status
//       <3>.flag
// Output:none
//****************************************************
static int
FTC_queue(struct usb_ep *_ep, struct usb_request *_req, unsigned gfp_flags)
{
	struct FTC_request *req;
	struct FTC_ep *ep;
	struct FTC_udc *dev;
	unsigned long flags;
	int status;
	u32 temp;

	DBG_FUNCC("+FTC_queue()\n");

	//<1>.Check request & ep & dev

	/* always require a cpu-view buffer so pio works */
	req = container_of(_req, struct FTC_request, req);
	if (unlikely(!_req || !_req->complete
		     || !_req->buf || !list_empty(&req->queue))) {
		DBG_CTRLL("??? FTC_queue => return -EINVAL\n");
		return -EINVAL;
	}

	ep = container_of(_ep, struct FTC_ep, ep);
	if (unlikely(!_ep || (!ep->desc && ep->num != 0))) {
		DBG_CTRLL("??? FTC_queue => return -EINVAL\n");
		return -EINVAL;
	}
	//Check CX 0 bytes
	if (req->req.length == 0) {
		if (ep->num == 0) {	//request Control Transfer 0 byte
			mUsbEP0DoneSet();
			mUsbCfgSet();	//Temp Solution for Set Configuration
			DBG_CTRLL
			    (">>> FTC_queue => return (set configuration)\n");
			return 0;
		}
		//else => Other ED 0 bytes
	}

	dev = ep->dev;
	if (unlikely(!dev->driver || dev->gadget.speed == USB_SPEED_UNKNOWN)) {
		DBG_CTRLL("??? FTC_queue => return -ESHUTDOWN\n");
		return -ESHUTDOWN;
	}

	/* can't touch registers when suspended */
	if (dev->ep0state == EP0_SUSPEND) {
		DBG_CTRLL("??? FTC_queue => return -EBUSY\n");
		return -EBUSY;
	}
	//if (ep->dma && _req->dma == DMA_ADDR_INVALID) {
	if (_req->dma == DMA_ADDR_INVALID) {
		DBG_CTRLL("....dma_map_single len = %x, dev = %x, buf = %x\n",
			  _req->length, dev, _req->buf);

		//important : DMA length will set as 16*n bytes
		temp = _req->length / 16;
		if (_req->length % 16)
			temp++;
		temp = temp * 16;
		_req->dma = dma_map_single((void *)dev, _req->buf, temp,	//USB_EPX_BUFSIZ,
					   ep->
					   is_in ? DMA_TO_DEVICE :
					   DMA_FROM_DEVICE);
		req->mapped = 1;
	}
#ifdef USB_TRACE
	VDBG(dev, "%s queue req %p, len %u buf %p\n",
	     _ep->name, _req, _req->length, _req->buf);
#endif

	//<2>.Set the req's status ...
	spin_lock_irqsave(&dev->lock, flags);

	_req->status = -EINPROGRESS;
	_req->actual = 0;

	//<3>.For control-in => Fource short packet

	/* for ep0 IN without premature status, zlp is required and
	 * writing EOP starts the status stage (OUT).
	 */
	if (unlikely(ep->num == 0 && ep->is_in))
		_req->zero = 1;

	/* kickstart this i/o queue? */
	status = 0;

	//Bruce;;if (list_empty(&ep->queue) && likely(!ep->stopped)) {
	//In  => Write data to the FIFO directly
	//Out => Only Enable the FIFO-Read interrupt
	if (list_empty(&ep->queue) && likely(!ep->stopped)) {
		//ep->num>0 ==> will add to queue until ed-FIFO-Interrupt be issue
		/* dma:  done after dma completion IRQ (or error)
		 * pio:  FOTG200 do not support
		 */
		DBG_CTRLL(">>> dev->EPUseDMA = 0x%x (ep->num=%x)\n",
			  dev->EPUseDMA, ep->num);

		//if ((dev->EPUseDMA) ==DMA_CHANEL_FREE)
		if ((ep->num) == 0)
			status = start_dma(ep, req);

		if (unlikely(status != 0)) {
			if (status > 0)
				status = 0;
			req = 0;
		}
	}
	/* else pio or dma irq handler advances the queue. */
	//Add request to queue
	if (likely(req != 0)) {
		DBG_CTRLL(">>> add request to queue ...\n");
		list_add_tail(&req->queue, &ep->queue);
	}
	//Enable the FIFO Interrupt
	if (likely((ep->num) > 0)) {	//Open the FIFO interrupt
		if (likely((ep->is_in) == 1))	//For In-Ep
			mUsbIntFXINEn(((ep->num) - 1));
		else		//For Out-Ep
			mUsbIntFXOUTEn(((ep->num) - 1));

		DBG_CTRLL
		    (">>> Enable EP-%x Interrupt (Register=0x%x)(Length=0x%x)...\n",
		     ep->num, mUsbIntSrc1MaskRd(), req->req.length);
	}

	spin_unlock_irqrestore(&dev->lock, flags);
	return status;
}

//****************************************************
// Name:Ep_ISR
// Description: For Ep-1 In
//             <1>.if queue have data start dma
//
//
// Input:dev
// Output:none
//****************************************************
void Ep_ISR(struct FTC_udc *dev, u8 epNum)
{
	struct FTC_request *req;
	struct FTC_ep *ep;

	DBG_FUNCC("+Ep_ISR(epNum=0x%x)\n", epNum);

	if (dev->EPUseDMA != DMA_CHANEL_FREE)
		return;

	//<1>.Checking data in queue ?
	ep = &(dev->ep[epNum]);

	if (list_empty(&ep->queue)) {
		if (likely(ep->is_in))
			mUsbIntFXINDis(((ep->num) - 1));	//Disable the Bulk--In
		else
			mUsbIntFXOUTDis(((ep->num) - 1));	//Disable the Bulk-Out
		printk("Error Ep_ISR ==> LIST is empty\n");
	} else {		//data in queue
		if (dev->EPUseDMA == DMA_CHANEL_FREE) {
			//Start the DMA
			req =
			    list_entry(ep->queue.next, struct FTC_request,
				       queue);
			(void)start_dma(ep, req);
		} else {
			printk
			    ("Error Ep_ISR ==> LIST is full and DMA used %x\n",
			     dev->EPUseDMA);
			//During DMA => Wait for done & disable DMA
			/*if (likely(ep->is_in))
			   mUsbIntFXINDis(((ep->num)-1));//Disable the interrupt-In
			   else
			   mUsbIntFXOUTDis(((ep->num)-1));//Disable the interrupt-Out
			 */
		}
	}
}

/////////////////////////////////////////////////////
//              clrFIFORegister(void)
//              Description:
//              input: none
//              output: none
/////////////////////////////////////////////////////
static void vUsbClrFIFORegister(void)
{
	u32 u8ep;

	mUsbEPMapAllClr();
	mUsbFIFOMapAllClr();
	mUsbFIFOConfigAllClr();

	for (u8ep = 1; u8ep <= MAX_FIFO_NUM; u8ep++) {
		mUsbEPMxPtSzClr(u8ep, DIRECTION_IN);
		mUsbEPMxPtSzClr(u8ep, DIRECTION_OUT);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// config FIFO
//-----------------------------------------------------------------------
/////////////////////////////////////////////////////
//              vUsbFIFO_EPxCfg_HS(void)
//              Description:
//                      1. Configure the FIFO and EPx map
//              input: none
//              output: none
/////////////////////////////////////////////////////
static void vUsbFIFO_EPxCfg_FS(void)
{
	u32 i;

	DBG_FUNCC("+vUsbFIFO_EPxCfg_FS()\n");

	DBG_CTRLL("FIFO-Start:0~3\n");
	DBG_CTRLL("Dir:Out=>1 / In =>0\n");
	DBG_CTRLL("BLKSize:1=>64bytes / 2 =>128 bytes\n");
	DBG_CTRLL("MaxPackSize:Max=64 bytes\n");
	DBG_CTRLL("IFO-Use-Num:1=>Single / 2=>Double / 3=>TRIBLE\n");
	DBG_CTRLL
	    ("FULL_ED4_bTYPE:0=>Control / 1=>ISO / 2=>Bulk / 3=>Interrupt\n");

	vUsbClrFIFORegister();

	//EP4
	mUsbEPMap(EP4, FULL_EP4_Map);
	mUsbFIFOMap(FULL_ED4_FIFO_START, FULL_EP4_FIFO_Map);
	mUsbFIFOConfig(FULL_ED4_FIFO_START, FULL_EP4_FIFO_Config);

	for (i = (FULL_ED4_FIFO_START + 1);
	     i < (FULL_ED4_FIFO_START + (FULL_ED4_bBLKNO * FULL_ED4_bBLKSIZE));
	     i++) {
		mUsbFIFOConfig(i, (FULL_EP4_FIFO_Config & (~BIT7)));
	}

	mUsbEPMxPtSz(EP4, FULL_ED4_bDIRECTION, (FULL_ED4_MAXPACKET & 0x7ff));
	mUsbEPinHighBandSet(EP4, FULL_ED4_bDIRECTION, FULL_ED4_MAXPACKET);
	DBG_CTRLL
	    ("EP4 Config = (FIFO-Start=0x%x) (Dir=0x%x)(BLKSize=0x%x)(MaxPackSize=0x%x)\n",
	     FULL_ED4_FIFO_START, FULL_ED4_bDIRECTION, FULL_ED4_bBLKSIZE,
	     FULL_ED4_MAXPACKET);
	DBG_CTRLL("             (FIFO-Use-Num=0x%x) (Type=0x%x)\n",
		  FULL_ED4_bBLKNO, FULL_ED4_bTYPE);
	DBG_CTRLL
	    ("Register Dump (mUsbEPMap=0x%x) (mUsbFIFOMap=0x%x)(mUsbFIFOConfig=0x%x)\n",
	     mUsbEPMap1_4Rd(), mUsbFIFOMapAllRd(), mUsbFIFOConfigAllRd());

	//EP3
	mUsbEPMap(EP3, FULL_EP3_Map);
	mUsbFIFOMap(FULL_ED3_FIFO_START, FULL_EP3_FIFO_Map);
	mUsbFIFOConfig(FULL_ED3_FIFO_START, FULL_EP3_FIFO_Config);

	for (i = FULL_ED3_FIFO_START + 1;
	     i < FULL_ED3_FIFO_START + (FULL_ED3_bBLKNO * FULL_ED3_bBLKSIZE);
	     i++) {
		mUsbFIFOConfig(i, (FULL_EP3_FIFO_Config & (~BIT7)));
	}

	mUsbEPMxPtSz(EP3, FULL_ED3_bDIRECTION, (FULL_ED3_MAXPACKET & 0x7ff));
	mUsbEPinHighBandSet(EP3, FULL_ED3_bDIRECTION, FULL_ED3_MAXPACKET);
	DBG_CTRLL
	    ("EP3 Config = (FIFO-Start=0x%x) (Dir=0x%x)(BLKSize=0x%x)(MaxPackSize=0x%x)\n",
	     FULL_ED3_FIFO_START, FULL_ED3_bDIRECTION, FULL_ED3_bBLKSIZE,
	     FULL_ED3_MAXPACKET);
	DBG_CTRLL("             (FIFO-Use-Num=0x%x) (Type=0x%x)\n",
		  FULL_ED3_bBLKNO, FULL_ED3_bTYPE);
	DBG_CTRLL
	    ("Register Dump (mUsbEPMap=0x%x) (mUsbFIFOMap=0x%x)(mUsbFIFOConfig=0x%x)\n",
	     mUsbEPMap1_4Rd(), mUsbFIFOMapAllRd(), mUsbFIFOConfigAllRd());

	//EP2
	mUsbEPMap(EP2, FULL_EP2_Map);
	mUsbFIFOMap(FULL_ED2_FIFO_START, FULL_EP2_FIFO_Map);
	mUsbFIFOConfig(FULL_ED2_FIFO_START, FULL_EP2_FIFO_Config);
	for (i = FULL_ED2_FIFO_START + 1;
	     i < FULL_ED2_FIFO_START + (FULL_ED2_bBLKNO * FULL_ED2_bBLKSIZE);
	     i++) {
		mUsbFIFOConfig(i, (FULL_EP2_FIFO_Config & (~BIT7)));
	}

	mUsbEPMxPtSz(EP2, FULL_ED2_bDIRECTION, (FULL_ED2_MAXPACKET & 0x7ff));
	mUsbEPinHighBandSet(EP2, FULL_ED2_bDIRECTION, FULL_ED2_MAXPACKET);
	DBG_CTRLL
	    ("EP2 Config = (FIFO-Start=0x%x) (Dir=0x%x)(BLKSize=0x%x)(MaxPackSize=0x%x)\n",
	     FULL_ED2_FIFO_START, FULL_ED2_bDIRECTION, FULL_ED2_bBLKSIZE,
	     FULL_ED2_MAXPACKET);
	DBG_CTRLL("             (FIFO-Use-Num=0x%x) (Type=0x%x)\n",
		  FULL_ED2_bBLKNO, FULL_ED2_bTYPE);
	DBG_CTRLL
	    ("Register Dump (mUsbEPMap=0x%x) (mUsbFIFOMap=0x%x)(mUsbFIFOConfig=0x%x)\n",
	     mUsbEPMap1_4Rd(), mUsbFIFOMapAllRd(), mUsbFIFOConfigAllRd());

	//EP1
	mUsbEPMap(EP1, FULL_EP1_Map);
	mUsbFIFOMap(FULL_ED1_FIFO_START, FULL_EP1_FIFO_Map);
	mUsbFIFOConfig(FULL_ED1_FIFO_START, FULL_EP1_FIFO_Config);

	for (i = FULL_ED1_FIFO_START + 1;
	     i < FULL_ED1_FIFO_START + (FULL_ED1_bBLKNO * FULL_ED1_bBLKSIZE);
	     i++) {
		mUsbFIFOConfig(i, (FULL_EP1_FIFO_Config & (~BIT7)));
	}

	mUsbEPMxPtSz(EP1, FULL_ED1_bDIRECTION, (FULL_ED1_MAXPACKET & 0x7ff));
	mUsbEPinHighBandSet(EP1, FULL_ED1_bDIRECTION, FULL_ED1_MAXPACKET);
	DBG_CTRLL
	    ("EP1 Config = (FIFO-Start=0x%x) (Dir=0x%x)(BLKSize=0x%x)(MaxPackSize=0x%x)\n",
	     FULL_ED1_FIFO_START, FULL_ED1_bDIRECTION, FULL_ED1_bBLKSIZE,
	     FULL_ED1_MAXPACKET);
	DBG_CTRLL("             (FIFO-Use-Num=0x%x) (Type=0x%x)\n",
		  FULL_ED1_bBLKNO, FULL_ED1_bTYPE);
	DBG_CTRLL
	    ("Register Dump (mUsbEPMap=0x%x) (mUsbFIFOMap=0x%x)(mUsbFIFOConfig=0x%x)\n",
	     mUsbEPMap1_4Rd(), mUsbFIFOMapAllRd(), mUsbFIFOConfigAllRd());
}

static void vUsbFIFO_EPxCfg_HS(void)
{
	u32 i;

	DBG_FUNCC("+vUsbFIFO_EPxCfg_HS()\n");

	DBG_CTRLL("FIFO-Start:0~3\n");
	DBG_CTRLL("Dir:Out=>1 / In =>0\n");
	DBG_CTRLL("BLKSize:1=>512bytes / 2 =>1024 bytes\n");
	DBG_CTRLL("MaxPackSize:Max=1023 bytes\n");
	DBG_CTRLL("IFO-Use-Num:1=>Single / 2=>Double / 3=>TRIBLE\n");
	DBG_CTRLL
	    ("FULL_ED4_bTYPE:0=>Control / 1=>ISO / 2=>Bulk / 3=>Interrupt\n");

	vUsbClrFIFORegister();

	//EP4
	mUsbEPMap(EP4, HIGH_EP4_Map);
	mUsbFIFOMap(HIGH_ED4_FIFO_START, HIGH_EP4_FIFO_Map);
	mUsbFIFOConfig(HIGH_ED4_FIFO_START, HIGH_EP4_FIFO_Config);

	for (i = HIGH_ED4_FIFO_START + 1;
	     i < HIGH_ED4_FIFO_START + (HIGH_ED4_bBLKNO * HIGH_ED4_bBLKSIZE);
	     i++) {
		mUsbFIFOConfig(i, (HIGH_EP4_FIFO_Config & (~BIT7)));
	}

	mUsbEPMxPtSz(EP4, HIGH_ED4_bDIRECTION, (HIGH_ED4_MAXPACKET & 0x7ff));
	mUsbEPinHighBandSet(EP4, HIGH_ED4_bDIRECTION, HIGH_ED4_MAXPACKET);
	DBG_CTRLL
	    ("EP4 Config = (FIFO-Start=0x%x) (Dir=0x%x)(BLKSize=0x%x)(MaxPackSize=0x%x)\n",
	     HIGH_ED4_FIFO_START, HIGH_ED4_bDIRECTION, HIGH_ED4_bBLKSIZE,
	     HIGH_ED4_MAXPACKET);
	DBG_CTRLL("             (FIFO-Use-Num=0x%x) (Type=0x%x)\n",
		  HIGH_ED4_bBLKNO, HIGH_ED4_bTYPE);

	//EP3
	mUsbEPMap(EP3, HIGH_EP3_Map);
	mUsbFIFOMap(HIGH_ED3_FIFO_START, HIGH_EP3_FIFO_Map);
	mUsbFIFOConfig(HIGH_ED3_FIFO_START, HIGH_EP3_FIFO_Config);

	for (i = HIGH_ED3_FIFO_START + 1;
	     i < HIGH_ED3_FIFO_START + (HIGH_ED3_bBLKNO * HIGH_ED3_bBLKSIZE);
	     i++) {
		mUsbFIFOConfig(i, (HIGH_EP3_FIFO_Config & (~BIT7)));
	}

	mUsbEPMxPtSz(EP3, HIGH_ED3_bDIRECTION, (HIGH_ED3_MAXPACKET & 0x7ff));
	mUsbEPinHighBandSet(EP3, HIGH_ED3_bDIRECTION, HIGH_ED3_MAXPACKET);
	DBG_CTRLL
	    ("EP3 Config = (FIFO-Start=0x%x) (Dir=0x%x)(BLKSize=0x%x)(MaxPackSize=0x%x)\n",
	     HIGH_ED3_FIFO_START, HIGH_ED3_bDIRECTION, HIGH_ED3_bBLKSIZE,
	     HIGH_ED3_MAXPACKET);
	DBG_CTRLL("             (FIFO-Use-Num=0x%x) (Type=0x%x)\n",
		  HIGH_ED3_bBLKNO, HIGH_ED3_bTYPE);

	//EP2
	mUsbEPMap(EP2, HIGH_EP2_Map);
	mUsbFIFOMap(HIGH_ED2_FIFO_START, HIGH_EP2_FIFO_Map);
	mUsbFIFOConfig(HIGH_ED2_FIFO_START, HIGH_EP2_FIFO_Config);

	for (i = HIGH_ED2_FIFO_START + 1;
	     i < HIGH_ED2_FIFO_START + (HIGH_ED2_bBLKNO * HIGH_ED2_bBLKSIZE);
	     i++) {
		mUsbFIFOConfig(i, (HIGH_EP2_FIFO_Config & (~BIT7)));
	}

	mUsbEPMxPtSz(EP2, HIGH_ED2_bDIRECTION, (HIGH_ED2_MAXPACKET & 0x7ff));
	mUsbEPinHighBandSet(EP2, HIGH_ED2_bDIRECTION, HIGH_ED2_MAXPACKET);
	DBG_CTRLL
	    ("EP2 Config = (FIFO-Start=0x%x) (Dir=0x%x)(BLKSize=0x%x)(MaxPackSize=0x%x)\n",
	     HIGH_ED2_FIFO_START, HIGH_ED2_bDIRECTION, HIGH_ED2_bBLKSIZE,
	     HIGH_ED2_MAXPACKET);
	DBG_CTRLL("             (FIFO-Use-Num=0x%x) (Type=0x%x)\n",
		  HIGH_ED2_bBLKNO, HIGH_ED2_bTYPE);

	//EP1
	mUsbEPMap(EP1, HIGH_EP1_Map);
	mUsbFIFOMap(HIGH_ED1_FIFO_START, HIGH_EP1_FIFO_Map);
	mUsbFIFOConfig(HIGH_ED1_FIFO_START, HIGH_EP1_FIFO_Config);

	for (i = HIGH_ED1_FIFO_START + 1;
	     i < HIGH_ED1_FIFO_START + (HIGH_ED1_bBLKNO * HIGH_ED1_bBLKSIZE);
	     i++) {
		mUsbFIFOConfig(i, (HIGH_EP1_FIFO_Config & (~BIT7)));
	}

	mUsbEPMxPtSz(EP1, HIGH_ED1_bDIRECTION, (HIGH_ED1_MAXPACKET & 0x7ff));
	mUsbEPinHighBandSet(EP1, HIGH_ED1_bDIRECTION, HIGH_ED1_MAXPACKET);
	DBG_CTRLL
	    ("EP1 Config = (FIFO-Start=0x%x) (Dir=0x%x)(BLKSize=0x%x)(MaxPackSize=0x%x)\n",
	     HIGH_ED1_FIFO_START, HIGH_ED1_bDIRECTION, HIGH_ED1_bBLKSIZE,
	     HIGH_ED1_MAXPACKET);
	DBG_CTRLL("             (FIFO-Use-Num=0x%x) (Type=0x%x)\n",
		  HIGH_ED1_bBLKNO, HIGH_ED1_bTYPE);
	DBG_CTRLL
	    ("Register Dump (mUsbEPMap=0x%x) (mUsbFIFOMap=0x%x)(mUsbFIFOConfig=0x%x)\n",
	     mUsbEPMap1_4Rd(), mUsbFIFOMapAllRd(), mUsbFIFOConfigAllRd());
}

/*-------------------------------------------------------------------------*/
// GM USB initial code

///////////////////////////////////////////////////////////////////////////////
//              vFOTG200_Dev_Init()
//              Description:
//                      1. Turn on the "Global Interrupt Enable" bit of FOTG200-P
//                      2. Turn on the "Chip Enable" bit of FOTG200
//              input: none
//              output: none
///////////////////////////////////////////////////////////////////////////////
static void vFOTG200_Dev_Init(void)
{
	DBG_FUNCC("+vFOTG200_Dev_Init()\n");

	// suspend counter
	mUsbIdleCnt(7);

	// Clear interrupt
	mUsbIntBusRstClr();
	mUsbIntSuspClr();
	mUsbIntResmClr();

	// Disable all fifo interrupt
	mUsbIntFIFO0_3OUTDis();
	mUsbIntFIFO0_3INDis();

	// Soft Reset
	mUsbSoftRstSet();	// All circuit change to which state after Soft Reset?
	mUsbSoftRstClr();

	// Clear all fifo
	mUsbClrAllFIFOSet();	// will be cleared after one cycle.

	//Bruce;;Clear mUsbIntEP0EndDis
	mUsbIntEP0EndDis();
	mUsbIntEP0InDis();	//We will use DMA-finish to instead of it
	mUsbIntEP0OutDis();	//We will use DMA-finish to instead of it

	if (mdwOTGC_Control_ID_Rd() != OTG_ID_A_TYPE) {
		if (mdwOTGC_Control_CROLE_Rd() == OTG_ROLE_DEVICE) {
			printk("FOTG2XX is now in Mini-B type %x\n", 0);
			mwOTG20_stop_host();
			//msleep(30);
			mdwOTGC_Control_A_BUS_REQ_Clr();
			mdwOTGC_Control_A_BUS_DROP_Set();
			//msleep(20);
			mdwPort(FOTG200_REGBASE, 0x130) &=
			    ~(BIT0 | BIT1 | BIT2);

			// on device mode, Dev_Idle on A360 always set, so we disable interrupt
			// on FPGA we have no such issue
			// we also disable Dev_Wakeup_byVBUS
			mdwPort(FOTG200_REGBASE, 0x13C) = 0x600;
			//john, should not enable now
			//mUsbGlobIntEnSet();
			mUsbUnPLGClr();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//              vUsbInit(struct FTC_udc *dev)
//              Description:
//                      1. Configure the FIFO and EPx map.
//                      2. Init FOTG200-Peripheral.
//                      3. Set the usb interrupt source as edge-trigger.
//                      4. Enable Usb interrupt.
//              input: none
//              output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsbInit(struct FTC_udc *dev)
{
	DBG_FUNCC("+vUsbInit()\n");

	// init variables
	dev->u16TxRxCounter = 0;
	dev->eUsbCxCommand = CMD_VOID;
	dev->u8UsbConfigValue = 0;
	dev->u8UsbInterfaceValue = 0;
	dev->u8UsbInterfaceAlternateSetting = 0;

	// init hardware
	vFOTG200_Dev_Init();
}

///////////////////////////////////////////////////////////////////////////////
//              vUsb_rst(struct FTC_udc *dev)
//              Description:
//                      1. Change descriptor table (High or Full speed).
//              input: none
//              output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_rst(struct FTC_udc *dev)
{
	DBG_FUNCC("+vUsb_rst()\n");

	INFO(dev, "L%x, Bus reset\n", dev->u8LineCount++);

	mUsbIntBusRstClr();
	dev->gadget.speed = USB_SPEED_UNKNOWN;
}

///////////////////////////////////////////////////////////////////////////////
//              vUsb_suspend(dev)
//              Description:
//                      1. .
//              input: none
//              output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_suspend(struct FTC_udc *dev)
{
	DBG_FUNCC("+vUsb_suspend()\n");

	INFO(dev, "L%x, Bus suspend\n", dev->u8LineCount++);
	// uP must do-over everything it should handle and do before into the suspend mode
	// Go Suspend status
	mUsbIntSuspClr();
	if (dev->gadget.b_hnp_enable) {
		mUsbGlobIntDis();
		mdwPort(FOTG200_REGBASE, 0x130) |= (BIT0 | BIT1 | BIT2);
		mUsb_OTGC_Control_B_HNP_EN_Set();
	}
	//john, clear FIFO for suspend to avoid extra interrupt
	mUsbFIFODone(0);
	mUsbFIFODone(1);
	mUsbFIFODone(2);
	mUsbFIFODone(3);

	//Bruce;;mUsbGoSuspend();
	dev->gadget.b_hnp_enable = 0;
	dev->ep0state = EP0_SUSPEND;
}

///////////////////////////////////////////////////////////////////////////////
//              vUsb_resm(struct FTC_udc        *dev)
//              Description:
//                      1. Change descriptor table (High or Full speed).
//              input: none
//              output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_resm(struct FTC_udc *dev)
{
	DBG_FUNCC("+vUsb_resm()\n");

	INFO(dev, "L%x, Bus resume\n", dev->u8LineCount++);
	// uP must do-over everything it should handle and do before into the suspend mode
	// uP must wakeup immediately
	mUsbIntResmClr();

	dev->ep0state = EP0_IDLE;
}

void vUsbClrEPx(void)
{
	u32 u8ep;

	DBG_FUNCC("+vUsbClrEPx()\n");

	// Clear All EPx Toggle Bit
	for (u8ep = 1; u8ep <= MAX_EP_NUM; u8ep++) {
		mUsbEPinRsTgSet(u8ep);
		mUsbEPinRsTgClr(u8ep);
	}

	for (u8ep = 1; u8ep <= MAX_EP_NUM; u8ep++) {
		mUsbEPoutRsTgSet(u8ep);
		mUsbEPoutRsTgClr(u8ep);
	}
}

///////////////////////////////////////////////////////////////////////////////
//              bGet_status(struct FTC_udc *dev)
//              Description:
//                      1. Send 2 bytes status to host.
//              input: none
//              output: TRUE or FALSE (u8)
///////////////////////////////////////////////////////////////////////////////
static u8 bGet_status(struct FTC_udc *dev, const struct usb_ctrlrequest *ctrl)
{
	u8 u8ep_n, u8fifo_n, RecipientStatusLow, RecipientStatusHigh;
	u8 u8Tmp[2];
	u8 bdir;

	DBG_FUNCC("+bGet_status()  \n");

	RecipientStatusLow = 0;
	RecipientStatusHigh = 0;
	switch ((ctrl->bRequestType) & 0x3)	// Judge which recipient type is at first
	{
	case 0:		// Device
		// Return 2-byte's Device status (Bit1:Remote_Wakeup, Bit0:Self_Powered) to Host
		// Notice that the programe sequence of RecipientStatus
		RecipientStatusLow = mUsbRmWkupST() << 1;
		// Bit0: Self_Powered--> DescriptorTable[0x23], D6(Bit 6)
		// Now we force device return data as self power. (Andrew)
		RecipientStatusLow |= ((USB_CONFIG_ATT_SELFPOWER >> 6) & 0x01);
		break;
	case 1:		// Interface
		// Return 2-byte ZEROs Interface status to Host
		break;
	case 2:		// Endpoint
		if (ctrl->wIndex == 0x00) {
			if (dev->ep0state == EP0_STALL)
				RecipientStatusLow = TRUE;
		} else {
			u8ep_n = (u8) ctrl->wIndex & 0x7F;	// which ep will be clear
			bdir = (u8) ctrl->wIndex >> 7;	// the direction of this ep
			if (u8ep_n > MAX_EP_NUM)	// over the Max. ep count ?
				return FALSE;
			else {
				u8fifo_n = mUsbEPMapRd(u8ep_n);	// get the relatived FIFO number
				if (bdir == 1)
					u8fifo_n &= 0x0F;
				else
					u8fifo_n >>= 4;
				if (u8fifo_n >= MAX_FIFO_NUM)	// over the Max. fifo count ?
					return FALSE;
				// Check the FIFO had been enable ?
				if (bdir == 1)	// IN direction ?
					RecipientStatusLow =
					    mUsbEPinStallST(u8ep_n);
				else
					RecipientStatusLow =
					    mUsbEPoutStallST(u8ep_n);
			}
		}
		break;
	default:
		return FALSE;
	}

	// return RecipientStatus;
	u8Tmp[0] = RecipientStatusLow;
	u8Tmp[1] = RecipientStatusHigh;

	//Use DMA to transfer data
	CX_dma_Directly(u8Tmp, 2, 1);

	mUsbEP0DoneSet();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//              bClear_feature(struct FTC_udc *dev)
//              Description:
//                      1. Send 2 bytes status to host.
//              input: none
//              output: TRUE or FALSE (u8)
///////////////////////////////////////////////////////////////////////////////
static u8 bClear_feature(struct FTC_udc *dev,
			 const struct usb_ctrlrequest *ctrl)
{
	u8 u8ep_n;
	u8 u8fifo_n;
	u8 bdir;

	DBG_FUNCC("+bClear_feature()\n");

	switch (ctrl->wValue)	// FeatureSelector
	{
	case 0:		// ENDPOINT_HALE
		// Clear "Endpoint_Halt", Turn off the "STALL" bit in Endpoint Control Function Register
		if (ctrl->wIndex == 0x00)
			u8ep_n = 0;	//Sp0 ed clear feature
		else {
			u8ep_n = ctrl->wIndex & 0x7F;	// which ep will be clear
			bdir = ctrl->wIndex >> 7;	// the direction of this ep
			if (u8ep_n > MAX_EP_NUM)	// over the Max. ep count ?
				return FALSE;
			else {
				u8fifo_n = Get_FIFO_Num(&dev->ep[u8ep_n]);	// get the relatived FIFO number
				if (u8fifo_n < MAX_FIFO_NUM)
					if ((mUsbFIFOConfigRd(u8fifo_n) & FIFOEnBit) == 0)	// Check the FIFO had been enable ?
						return FALSE;
			}
		}
		FTC_clear_halt(&dev->ep[u8ep_n]);
		break;
	case 1:		// Device Remote Wakeup
		// Clear "Device_Remote_Wakeup", Turn off the"RMWKUP" bit in Main Control Register
		mUsbRmWkupClr();
		break;
	case 2:		// Test Mode
		return FALSE;
	default:
		return FALSE;
	}

	mUsbEP0DoneSet();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//              bSet_feature(struct FTC_udc *dev)
//              Description:
//                      1. Send 2 bytes status to host.
//              input: none
//              output: TRUE or FALSE (u8)
///////////////////////////////////////////////////////////////////////////////
static u8 bSet_feature(struct FTC_udc *dev, const struct usb_ctrlrequest *ctrl)
{
	u8 i;
	u8 u8ep_n;
	u8 u8fifo_n;
	u8 u8Tmp[55];
	u8 *pp;
	u8 bdir;

	DBG_FUNCC("+bSet_feature()\n");

	switch (ctrl->wValue)	// FeatureSelector
	{
	case 0:		// ENDPOINT_HALT
		// Set "Endpoint_Halt", Turn on the "STALL" bit in Endpoint Control Function Register
		if (ctrl->wIndex == 0x00)
			FTC_set_halt(dev->gadget.ep0, 1);	// Return EP0_Stall
		else {
			u8ep_n = ctrl->wIndex & 0x7F;	// which ep will be clear
			bdir = ctrl->wIndex >> 7;	// the direction of this ep
			u8fifo_n = Get_FIFO_Num(&dev->ep[u8ep_n]);	// get the relatived FIFO number
			if (u8fifo_n < MAX_FIFO_NUM)	// Check the FIFO had been enable ?
				if ((mUsbFIFOConfigRd(u8fifo_n) & FIFOEnBit) ==
				    0)
					return FALSE;

			if (bdir == 1)	// IN direction ?
				mUsbEPinStallSet(u8ep_n);	// Clear Stall Bit
			else
				mUsbEPoutStallSet(u8ep_n);	// Set Stall Bit
		}
		break;
	case 1:		// Device Remote Wakeup
		// Set "Device_Remote_Wakeup", Turn on the"RMWKUP" bit in Mode Register
		mUsbRmWkupSet();
		break;

	case 2:		// Test Mode
		switch ((ctrl->wIndex >> 8))	// TestSelector
		{
		case 0x1:	// Test_J
			mUsbTsMdWr(TEST_J);
			break;
		case 0x2:	// Test_K
			mUsbTsMdWr(TEST_K);
			break;
		case 0x3:	// TEST_SE0_NAK
			mUsbTsMdWr(TEST_SE0_NAK);
			break;
		case 0x4:	// Test_Packet
			mUsbTsMdWr(TEST_PKY);
			mUsbEP0DoneSet();	// special case: follow the test sequence
			pp = u8Tmp;
			for (i = 0; i < 9; i++)	// JKJKJKJK x 9
				(*pp++) = (0x00);

			//(*pp++) = (0xAA);
			//(*pp++) = (0x00);

			for (i = 0; i < 8; i++)	// JJKKJJKK *8, 8*AA
				(*pp++) = (0xAA);

			for (i = 0; i < 8; i++)	//JJJJKKKK *8, 8*EE
				(*pp++) = (0xEE);

			(*pp++) = (0xFE);	//JJJJJJJKKKKKKK *8,                                                                                                                                                             
			for (i = 0; i < 11; i++)	// 11*FF
				(*pp++) = (0xFF);

			(*pp++) = (0x7F);	// JJJJJJJK * 8
			(*pp++) = (0xBF);
			(*pp++) = (0xDF);
			(*pp++) = (0xEF);
			(*pp++) = (0xF7);
			(*pp++) = (0xFB);
			(*pp++) = (0xFD);
			(*pp++) = (0xFC);
			(*pp++) = (0x7E);	// {JKKKKKKK * 10}, JK
			(*pp++) = (0xBF);
			(*pp++) = (0xDF);
			(*pp++) = (0xEF);
			(*pp++) = (0xF7);
			(*pp++) = (0xFB);
			(*pp++) = (0xFD);
			(*pp) = (0x7E);
			CX_dma_Directly(u8Tmp, 53, 1);
			mUsbEP0DoneSet();

			// Turn on "r_test_packet_done" bit(flag) (Bit 5)
			mUsbTsPkDoneSet();
			return TRUE;
			break;
		case 0x5:	// Test_Force_Enable
			//FUSBPort[0x08] = 0x20; //Start Test_Force_Enable
			break;
		default:
			return FALSE;
		}
		break;
	case 3:		//For OTG => b_hnp_enable
		dev->gadget.b_hnp_enable = 1;
		//<1>.Set b_Bus_Request
		mUsb_OTGC_Control_B_BUS_REQ_Set();

		//<2>.Set the HNP enable
		mUsb_OTGC_Control_B_HNP_EN_Set();
		fotg2xx_dbg("FOTG2XX Device set HNP ... HNP:%x B_BUS_REQ:%x\n",
			    mUsb_OTGC_Control_B_HNP_EN_Rd(),
			    mUsb_OTGC_Control_B_BUS_REQ_Rd());
		break;
	case 4:		//For OTG => b_hnp_enable
		dev->gadget.a_hnp_support = 1;
		break;
	case 5:		//For OTG => b_hnp_enable
		dev->gadget.a_alt_hnp_support = 1;
		printk
		    (">>> Please Connect to an alternate port on the A-device for HNP...\n");
		break;
	default:
		return FALSE;
	}

	mUsbEP0DoneSet();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//              bSynch_frame(struct FTC_udc *dev)
//              Description:
//                      1. If the EP is a Iso EP, then return the 2 bytes Frame number.
//                               else stall this command
//              input: none
//              output: TRUE or FALSE
///////////////////////////////////////////////////////////////////////////////
static u8 bSynch_frame(struct FTC_udc *dev, const struct usb_ctrlrequest *ctrl)
{
	DBG_FUNCC("+bSynch_frame()  ==> add by Andrew\n");

	if ((ctrl->wIndex == 0) || (ctrl->wIndex > 4))
		return FALSE;

	// Does the Endpoint support Isochronous transfer type?
	mUsbEP0DoneSet();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//              bSet_address(struct FTC_udc *dev)
//              Description:
//                      1. Set addr to FUSB200 register.
//              input: none
//              output: TRUE or FALSE (u8)
///////////////////////////////////////////////////////////////////////////////
static u8 bSet_address(struct FTC_udc *dev, const struct usb_ctrlrequest *ctrl)
{
	DBG_FUNCC("+bSet_address() = %d\n", ctrl->wValue);

	if (ctrl->wValue >= 0x0100)
		return FALSE;
	else {
		mUsbDevAddrSet(ctrl->wValue);
		mUsbEP0DoneSet();
		return TRUE;
	}
}

///////////////////////////////////////////////////////////////////////////////
//              vUsb_ep0setup(struct FTC_udc *dev)
//              Description:
//                      1. Read the speed
//                      2. Read 8-byte setup packet.
//          3. Process the standard command:
//             <1>.bSet_address
//
//
//              input: none
//              output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_ep0setup(struct FTC_udc *dev)
{
	u8 u8UsbCmd[8];
	struct usb_ctrlrequest ctrl;
	int tmp;
	u32 u32UsbCmd[2];

	DBG_FUNCC("+vUsb_ep0setup()\n");

	//<1>.Read the speed
	if (dev->gadget.speed == USB_SPEED_UNKNOWN) {
		// first ep0 command after usb reset, means we can check usb speed right now.
		if (mUsbHighSpeedST())	// First we should judge HS or FS
		{
			INFO(dev, "L%x, high speed mode\n", dev->u8LineCount++);
			dev->gadget.speed = USB_SPEED_HIGH;
			vUsbFIFO_EPxCfg_HS();	//Set the FIFO information
		} else {
			INFO(dev, "L%x, full speed mode\n", dev->u8LineCount++);
			dev->gadget.speed = USB_SPEED_FULL;
			vUsbFIFO_EPxCfg_FS();	//Set the FIFO information
		}
		dev->ep0state = EP0_IDLE;
	}
	//<2>.Dequeue ALL requests
	nuke(&dev->ep[0], 0);
	dev->ep[0].stopped = 0;

	//<3>.Read 8-byte setup packet from FIFO

	// Read 8-byte setup packet from FIFO
	mUsbDMA2FIFOSel(FOTG200_DMA2CxFIFO);
	u32UsbCmd[0] = mUsbEP0CmdDataRdDWord();
	u32UsbCmd[1] = mUsbEP0CmdDataRdDWord();
	mUsbDMA2FIFOSel(FOTG200_DMA2FIFO_Non);
	memcpy(u8UsbCmd, u32UsbCmd, 8);

	DBG_CTRLL("L%x, EP0Cmd:%02x %02x %02x %02x %02x %02x %02x %02x\n",
		  dev->u8LineCount++, u8UsbCmd[0], u8UsbCmd[1], u8UsbCmd[2],
		  u8UsbCmd[3], u8UsbCmd[4], u8UsbCmd[5], u8UsbCmd[6],
		  u8UsbCmd[7]);

	/* read SETUP packet and enter DATA stage */
	ctrl.bRequestType = u8UsbCmd[0];
	ctrl.bRequest = u8UsbCmd[1];
	ctrl.wValue = (u8UsbCmd[3] << 8) | u8UsbCmd[2];
	ctrl.wIndex = (u8UsbCmd[5] << 8) | u8UsbCmd[4];
	ctrl.wLength = (u8UsbCmd[7] << 8) | u8UsbCmd[6];

	if (likely(ctrl.bRequestType & USB_DIR_IN)) {
		dev->ep[0].is_in = 1;
		dev->ep0state = EP0_IN;
	} else {
		dev->ep[0].is_in = 0;
		dev->ep0state = EP0_OUT;
	}

	//Parsing the Standard Command
	switch (ctrl.bRequest)	// by Standard Request codes
	{
	case USB_REQ_CLEAR_FEATURE:	// clear feature
		if (bClear_feature(dev, &(ctrl)) == FALSE)
			goto stall;
		break;
	case USB_REQ_SET_ADDRESS:	// set address
		if (dev->ep0state == EP0_STALL)
			goto stall;
		if (bSet_address(dev, &(ctrl)) == FALSE)
			goto stall;
		break;
	case USB_REQ_SET_FEATURE:	// clear feature
		if (bSet_feature(dev, &(ctrl)) == FALSE)
			goto stall;
		break;
	case USB_REQ_GET_STATUS:	// clear feature
		if (bGet_status(dev, &(ctrl)) == FALSE)
			goto stall;
		break;
	case USB_REQ_SYNCH_FRAME:	// clear feature
		if (dev->ep0state == EP0_STALL)
			goto stall;
		if (bSynch_frame(dev, &(ctrl)) == FALSE)
			goto stall;
		break;
	default:		/* pass to gadget driver */
		if (dev->ep0state == EP0_STALL)
			goto stall;

		spin_unlock(&dev->lock);
		tmp = dev->driver->setup(&dev->gadget, &(ctrl));
		spin_lock(&dev->lock);
		DBG_CTRLL(">>>Exit Driver call back setup function...\n");
		if (unlikely(tmp < 0))
			goto stall;
		break;
	}
	//Normal Exit
	return;

	//Stall the command
stall:
#ifdef USB_TRACE
	VDBG(dev, "req %02x.%02x protocol STALL; err %d\n", ctrl.bRequestType,
	     ctrl.bRequest, tmp);
#endif
	INFO(dev, "Set STALL in vUsb_ep0setup\n");
	FTC_set_halt(dev->gadget.ep0, 1);	// Return EP0_Stall
	dev->ep[0].stopped = 1;
	dev->ep0state = EP0_STALL;
}

///////////////////////////////////////////////////////////////////////////////
//              vUsb_ep0end(struct FTC_udc *dev)
//              Description:
//                      1. End this transfer.
//              input: none
//              output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_ep0end(struct FTC_udc *dev)
{
	DBG_FUNCC("+vUsb_ep0end()\n");

	dev->eUsbCxCommand = CMD_VOID;
}

///////////////////////////////////////////////////////////////////////////////
//              vUsb_ep0fail(struct FTC_udc *dev)
//              Description:
//                      1. Stall this transfer.
//              input: none
//              output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_ep0fail(struct FTC_udc *dev)
{
	DBG_FUNCC("+vUsb_ep0fail()\n");
	INFO(dev, "L%x, EP0 fail\n", dev->u8LineCount++);

	FTC_set_halt(dev->gadget.ep0, 1);	// Return EP0_Stall
}

///////////////////////////////////////////////////////////////////////////////
//              vUsbHandler(struct FTC_udc      *dev)
//              Description:
//                      1. Service all Usb events
//                      2. ReEnable Usb interrupt.
//              input: none
//              output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsbHandler(struct FTC_udc *dev)	//FOTG200.ok
{
	u32 usb_interrupt_level2;
	u32 usb_interrupt_Mask;
	u32 usb_interrupt_Origan;

	DBG_FUNCC("+vUsbHandler()\n");
	DBG_CTRLL("usb_interrupt_level1:0x%x\n", dev->usb_interrupt_level1);

	//----- Group Byte 2 ---------
	if (dev->usb_interrupt_level1 & BIT2) {
		usb_interrupt_Origan = mUsbIntSrc2Rd();
		usb_interrupt_Mask = mUsbIntSrc2MaskRd();
		usb_interrupt_level2 =
		    usb_interrupt_Origan & ~usb_interrupt_Mask;
		DBG_CTRLL("usb_interrupt_Origan:0x%x\n", usb_interrupt_Origan);
		DBG_CTRLL("usb_interrupt_Mask:0x%x\n", usb_interrupt_Mask);
		DBG_CTRLL("usb_interrupt_level2:0x%x\n", usb_interrupt_level2);

		if (usb_interrupt_level2 & BIT0)
			vUsb_rst(dev);
		if (usb_interrupt_level2 & BIT1)
			vUsb_suspend(dev);
		if (usb_interrupt_level2 & BIT2)
			vUsb_resm(dev);
		if (usb_interrupt_level2 & BIT3) {
			mUsbIntIsoSeqErrClr();
			printk("??? ISO sequence error...\n");
		}
		if (usb_interrupt_level2 & BIT4) {
			mUsbIntIsoSeqAbortClr();
			printk("??? ISO sequence error...\n");
		}
		if (usb_interrupt_level2 & BIT5) {
			mUsbIntTX0ByteClr();
			mUsbIntTX0ByteSetClr(0x9);	// Clear all zero-legnth 
		}

		if (usb_interrupt_level2 & BIT6)
			mUsbIntRX0ByteClr();

		if (usb_interrupt_level2 & BIT7) {
			mUsbIntDmaFinishClr();
			dma_advance(dev, &(dev->ep[dev->EPUseDMA]));
		}
		if (usb_interrupt_level2 & BIT8) {
			mUsbIntDmaErrClr();
			printk("??? DMA error Interrupt \n");
		}
	}
	//----- Group Byte 0 ---------
	if (dev->usb_interrupt_level1 & BIT0) {
		usb_interrupt_Origan = mUsbIntSrc0Rd();
		usb_interrupt_Mask = mUsbIntSrc0MaskRd();
		usb_interrupt_level2 =
		    usb_interrupt_Origan & ~usb_interrupt_Mask;

		DBG_CTRLL("IntSCR0:0x%x\n", usb_interrupt_level2);
		dev->ep[0].irqs++;
		//        Stop APB DMA if DMA is still running
		//        record buffer counter, and clear buffer. Later
		//        will re-input data use DMA.
		if (usb_interrupt_level2 & BIT0) {
			DBG_CTRLL("USB ep0 Setup\n");
			vUsb_ep0setup(dev);
		} else if (usb_interrupt_level2 & BIT3) {
			DBG_CTRLL("USB ep0 end\n");
			vUsb_ep0end(dev);
		}
		if (usb_interrupt_level2 & BIT1) {
			DBG_CTRLL("USB ep0 TX\n");
		}
		if (usb_interrupt_level2 & BIT2) {
			DBG_CTRLL("USB ep0 RX\n");
		}
		if (usb_interrupt_level2 & BIT4) {
			UWARN(dev, "USB ep0 fail\n");
			vUsb_ep0fail(dev);
		}
		if (usb_interrupt_level2 & BIT5) {
			mUsbIntEP0AbortClr();
			printk("??? Command Abort Interrupt ...\n");
		}
	}
	//----- Group Byte 1 ---------
	if (dev->usb_interrupt_level1 & BIT1) {
		usb_interrupt_Origan = mUsbIntSrc1Rd();
		usb_interrupt_Mask = mUsbIntSrc1MaskRd();
		usb_interrupt_level2 =
		    usb_interrupt_Origan & (~usb_interrupt_Mask);

		DBG_CTRLL
		    ("(IntSCR1:0x%x)(Mask1:0x%x)(usb_interrupt_level2=0x%x)\n",
		     usb_interrupt_Origan, mUsbIntSrc1MaskRd(),
		     usb_interrupt_level2);

		//FIFO0 => Ep-1 In  Bulk
		//FIFO1 => Ep-2   Out     Bulk
		//FIFO2 => Ep-3   In      Interrupt
		//FIFO3 => Ep-4   Out     Interrupt
		// use FIFO1 for ep2( bulk out)
		if (usb_interrupt_level2 & BIT3)	// short packet
			Ep_ISR(dev, 2);
		else if (usb_interrupt_level2 & BIT2)	// full packet
			Ep_ISR(dev, 2);

		// use FIFO0 for ep1( bulk in)
		if (usb_interrupt_level2 & BIT16)
			Ep_ISR(dev, 1);

		// use FIFO3 for ep4( Interrupt out)
		if (usb_interrupt_level2 & BIT7)	// short packet
			Ep_ISR(dev, 4);
		else if (usb_interrupt_level2 & BIT6)	// full packet
			Ep_ISR(dev, 4);

		// use FIFO2 for ep3( Interrupt in)
		if (usb_interrupt_level2 & BIT18)
			Ep_ISR(dev, 3);
	}
}

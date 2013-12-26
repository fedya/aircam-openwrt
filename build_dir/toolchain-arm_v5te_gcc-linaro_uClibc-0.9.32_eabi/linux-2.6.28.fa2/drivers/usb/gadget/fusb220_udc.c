/*
 * GM USB220 ("FUSB220") USB Device Controller driver
 *
 * Copyright (C) 2004-2005 Lineo
 *      by John Chiang
 * Copyright (C) 2004 GM tech corp.
 *      porting to 2.6 by Brad Lu
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

/*
 * This device has ep0 and four bulk/interrupt endpoints.
 *
 *  - Endpoint numbering is fixed: EP1-bulk in, EP2-bulk out, EP3-interrupt in, EP4 interrupt out 
 *  - EP maxpacket (if full speed:64, if high speed:512)
 *  - no DMA supporting in the first version
 *  - support AHB_DMA in the 2nd version
 */
/*-------------------------------------------------------------------------*/

static void nuke(struct FTC_ep *, int status);
void vCheckDMA(int ch, u16 int_status, void *data); //(struct FTC_udc *dev)

/*-------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
//		vUsbDxFWr()
//		Description: Write the buffer content to USB220 PAM through data port
//		input: Buffer pointer, byte number to write, Change Endian type or NOT
//		output: this time write data length 
///////////////////////////////////////////////////////////////////////////////
static int vUsbDxFWr(u8 FIFONum, u8 *pu8Buffer, u16 u16Num)
{
	u16  u16_i;
	u8   *pp;
	u8 	*pBufPtrOrg;
	
	DBG_FUNCC("+vUsbDxFWr()(u16Num=%d)\n",u16Num);

	if (u16Num<=0)
		return u16Num;


	pBufPtrOrg=NULL;


	//Check if start address is 4 bytes alignment
	// if it's not, must malloc a new buffer, and set to 4 byte alignment.
	if ((((u32) pu8Buffer) % 4)!=0)
	{
		DBG_BULKK("Data Ptr not alignment, malloc a temp buf.\n");
		pBufPtrOrg  = kmalloc(u16Num+4, GFP_KERNEL);
		pp=pBufPtrOrg;
		while((((u32) pp) % 4)!=0)
			pp=pp+1;
		memcpy(pp , pu8Buffer, u16Num);		
	}
	else
		pp = pu8Buffer;	
	DBG_BULKK("[Brad]FIFO write DATA=");
	for (u16_i = (u16Num >> 2); u16_i > 0; u16_i --) {				
		mUsbWrDWord(FIFONum, *((u32 *)(pp)));
		DBG_BULKK("0x%08x,",*((u32 *)(pp)));		
		pp = pp + 4;
	}

	switch (u16Num % 4)
	{
		case 1:
			mUsbWrByte0(FIFONum, *((u8 *)pp));
			break;
		case 2:
			mUsbWrWord(FIFONum, *((u16 *)(pp)));
			break;
		case 3:
#if (USB_DataPort == NoFixPort)			
			// for this version of data port, if want to write data length= 3 bytes,
			// we must write first 2 bytes in offset 0, and last byte write to offset 2.
			mUsbWrWord(FIFONum, *((u16 *)(pp)));
			pp = pp + 2;

			mUsbWrByte2(FIFONum, *((u8 *)pp));
#elif(USB_DataPort == FixPort)
			// for newest version of data port, if want to write data length= 3 bytes,
			// we must write first 2 bytes in offset 0, and last byte still write to offset 0.
			mUsbWrWord(FIFONum, *((u16 *)(pp)));
			pp = pp + 2;

			mUsbWrByte0(FIFONum, *((u8 *)pp));
#endif
			break;
		default:
			break;
	}
	mUsbFIFODone(FIFONum);

	// if create a buffer, free it now.
	//if((((u32) pu8Buffer) % 4)!=0)

	if (pBufPtrOrg !=NULL)
	   kfree(pBufPtrOrg);
	
	return u16Num;
}


///////////////////////////////////////////////////////////////////////////////
//		vUsbDxFRd()
//		Description: Read FUSB220 FIFO data using PIO mode
//		input: Buffer pointer, byte number to write, Change Endian type or NOT
//		output: this time read data length 
///////////////////////////////////////////////////////////////////////////////
static int vUsbDxFRd(u8 FIFONum, u8 * pu8Buffer, u16 u16Num)
{
	u16  u16_i;
	u8   *pp;
	u8	*pBufPtrOrg;
	u8 	*pBufPtrSave;
	u32  dwTemp;
	u32  *dwpp;


	DBG_FUNCC("+vUsbDxFRd() : byte = %d\n", u16Num);


	if (u16Num<=0)
		return u16Num;


	pBufPtrOrg=NULL;

	pBufPtrSave=NULL;


	//Check if start address is 4 bytes alignment ?
	// if it's not, must malloc a new buffer, and set to 4 byte alignment.
	if ((((u32) pu8Buffer) % 4)!=0)
	{
		DBG_BULKK("Data Ptr not alignment, malloc a temp buf.\n");
		pBufPtrOrg  = kmalloc(u16Num+4, GFP_KERNEL);
		pp=pBufPtrOrg;
		while((((u32) pp) % 4)!=0)
			pp=pp+1;
		pBufPtrSave=pp;
	}
	else
		pp = pu8Buffer;	

	DBG_BULKK("FIFO DATA=");

	// read data use 4 bytes each time.
	dwpp = (u32 *) pp;
	for (u16_i = (u16Num >> 2); u16_i > 0; u16_i --) {
		dwTemp = mUsbRdDWord(FIFONum);
		DBG_BULKK("0x%08x,",dwTemp);
		*dwpp++ = dwTemp;
	}

	// translate data pointer record from 4 bytes to 1 byte, and read left data from FIFO	
	pp = (u8 *)dwpp;		
	switch (u16Num & 0x3)
	{
		case 1:
			dwTemp = mUsbRdDWord(FIFONum);
			*(pp) = (u8)(dwTemp);
			DBG_BULKK("0x%08x,",dwTemp);
			break;
		case 2:
			dwTemp = mUsbRdDWord(FIFONum);
			*(pp) = (u8)(dwTemp);
			*(pp + 1)= (u8)(dwTemp >> 8);
			DBG_BULKK("0x%08x,",dwTemp);
			break;
		case 3:
			dwTemp = mUsbRdDWord(FIFONum);
			*(pp) = (u8)(dwTemp);
			*(pp + 1)= (u8)(dwTemp >> 8);
			*(pp + 2)= (u8)(dwTemp >> 16);
			DBG_BULKK("0x%08x,",dwTemp);				
			break;
		default:
			break;
	}
	DBG_BULKK("\n");

	// if create a buffer, free it now.
	//if((((u32) pu8Buffer) % 4)!=0)

	if (pBufPtrOrg != NULL) {
		DBG_BULKK("Copy temp buf data to real buf.\n");
		memcpy( pu8Buffer, pBufPtrSave, u16Num);			
		kfree(pBufPtrOrg);
	}
	
	return u16Num;
}


///////////////////////////////////////////////////////////////////////////////
//		vUsbCxFWr()
//		Description: Write the buffer content to USB220 CxF using PIO mode
//		input: Buffer pointer, byte number to write
//		output: this time write data length 
///////////////////////////////////////////////////////////////////////////////
static int vUsbCxFWr( u8 *pu8Buffer, u16 u16Num)
{
	u16  u16_i;
	u8   *pp;
	u8 	*pBufPtrOrg;
	u16  wTemp;
	u8	 bTemp;
	u32  *dwpp;

	DBG_FUNCC("+vUsbCxFWr()(Len=%d)\n",u16Num);

	if(u16Num==0)
		return 0;
	pBufPtrOrg=NULL;

	
	while( !mUsbIntBufEmptyRd() );
	DBG_CTRLL("Cx config and status:0x%x\n",bFUSBPort(0x0B));
	//Check if start address is 4 bytes alignment
	// if it's not, must malloc a new buffer, and set to 4 byte alignment.
	if((((u32) pu8Buffer) % 4)!=0)
	{
		DBG_CTRLL("Data Ptr not alignment, malloc a temp buf.\n");
		pBufPtrOrg  = kmalloc(u16Num+4, GFP_KERNEL);
		pp=pBufPtrOrg;
		while((((u32) pp) % 4)!=0)
			pp=pp+1;
		memcpy(pp , pu8Buffer, u16Num);		
	}
	else
		pp = pu8Buffer;	
	
	DBG_CTRLL("Write data into Cx FIFO.\n");
	dwpp = (u32 *) pp;
	for (u16_i = (u16Num >> 2); u16_i > 0; u16_i --) {
        //while((bFUSBPort(0x0B)&0x10));// CX FIFO is not full
		mUsbEP0DataWrDWord(*dwpp);
		dwpp++;
	}

	pp = (u8 *)dwpp;

    //while((bFUSBPort(0x0B)&0x10)); // CX FIFO is not full
	switch (u16Num % 4)
	{
		case 1:
			bTemp = *(pp);
			mUsbEP0DataWrByte0(bTemp);
			break;
		case 2:
			wTemp =((u16)(*(pp + 1)));
			wTemp = (wTemp << 8)|((u16)(*(pp)));			
			mUsbEP0DataWrWord(wTemp);
			break;
		case 3:
			wTemp = (u16)(*(pp + 1));
			wTemp = (wTemp << 8)|(u16)(*(pp));			
			mUsbEP0DataWrWord(wTemp);
			
			pp = pp + 2;
#if (USB_DataPort == NoFixPort)				
			mUsbEP0DataWrByte2((*(pp)));
#elif(USB_DataPort == FixPort)
			mUsbEP0DataWrByte0((*(pp)));
#endif					
			break;
		default:
			break;
	}
	
	// if create a buffer, free it now.
	//if((((u32) pu8Buffer) % 4)!=0)

	if (pBufPtrOrg!=NULL)
	   kfree(pBufPtrOrg);
	DBG_CTRLL("Exit Cx Write.\n");
	return u16Num;	
}


///////////////////////////////////////////////////////////////////////////////
//		vUsbCxFRd()
//		Description: Fill the buffer from USB200 via Dbus
//		input: Buffer pointer, byte number to write, Change Endian type or NOT
//		output: this time read data length 
///////////////////////////////////////////////////////////////////////////////
static int vUsbCxFRd(u8 *pu8Buffer, u16 u16Num)
{
	u16  u16_i;
	u8   *pp;
	u8 	*pBufPtrOrg;
	u8 	*pBufPtrSave;
	u32  dwTemp;
	u16  wTemp;
	u32  *dwpp;

	DBG_FUNCC("+vUsbCxFRd()(Len=%d)\n",u16Num);


	pBufPtrOrg=NULL;

	pBufPtrSave=NULL;


	// Check if start address is 4 bytes alignment ?
	// if it's not, must malloc a new buffer, and set to 4 byte alignment.
	if((((u32) pu8Buffer) % 4)!=0)
	{
		DBG_CTRLL("Data Ptr not alignment, malloc a temp buf.\n");
		pBufPtrOrg  = kmalloc(u16Num+4, GFP_KERNEL);
		pp=pBufPtrOrg;
		while((((u32) pp) % 4)!=0)
			pp=pp+1;
		pBufPtrSave = pp;
	}
	else
		pp = pu8Buffer;
	
	DBG_CTRLL("Read data from Cx FIFO.\n");
	dwpp = (u32 *) pp;
	for (u16_i = (u16Num >> 2); u16_i > 0; u16_i --) {
        //while((bFUSBPort(0x21)& BIT2));		// Check CX FIFO is not empty, if it's empty, wait host send data.
		dwTemp = mUsbEP0DataRdDWord();
		*dwpp = dwTemp;
		dwpp++;
	}
	pp = (u8 *) dwpp;
	
    //while((bFUSBPort(0x21)& BIT2));		// Check CX FIFO is not empty, if it's empty, wait host send data.
	switch (u16Num % 4)
	{
		case 1:
			*(pp) = mUsbEP0DataRdByte0();
			break;
		case 2:
			wTemp = mUsbEP0DataRdWord();
			*pp++ = (u8)wTemp;
			*pp= (u8)(wTemp >> 8);	
			break;
		case 3:
			wTemp = mUsbEP0DataRdWord();
			*pp 		= (u8)wTemp;
			*(pp + 1)= (u8)(wTemp >> 8);	
			pp = pp + 2;

#if (USB_DataPort == NoFixPort)
			*(pp) 	= mUsbEP0DataRdByte2();
#elif(USB_DataPort == FixPort)
			*(pp) 	= mUsbEP0DataRdByte0();
#endif
			break;
		default:
			break;
	}


	// if create a buffer, save data into real buffer and free it now.
	//if((((u32) pu8Buffer) % 4)!=0)

	if (pBufPtrOrg!=NULL) {
		DBG_CTRLL("Copy temp buf data to real buf.\n");
		memcpy( pu8Buffer, pBufPtrSave, u16Num);			
		kfree(pBufPtrOrg);
	}


	DBG_CTRLL("Exit Cx Read.\n");
	return u16Num;
}


///////////////////////////////////////////////////////////////////////////////
//		vUsbCxFRd()
//		Description: Fill the buffer from USB200 via Dbus
//		input: Buffer pointer, byte number to write, Change Endian type or NOT
//		output: this time read data length 
///////////////////////////////////////////////////////////////////////////////
static int vUsbCxFRd8ByteCmd(u8 *pu8Buffer)
{
	u16  u16_i;
	u8   *pp;
	u8 	*pBufPtrOrg;
	u8 	*pBufPtrSave;
	u32  dwTemp;
	u16  wTemp;
	u32  *dwpp;
	u16 u16Num=8;
	
	DBG_FUNCC("+vUsbCxFRd8ByteCmd()\n");


	pBufPtrOrg=NULL;

	pBufPtrSave=NULL;


	// Check if start address is 4 bytes alignment ?
	// if it's not, must malloc a new buffer, and set to 4 byte alignment.
	if((((u32) pu8Buffer) % 4)!=0)
	{
		DBG_CTRLL("Data Ptr not alignment, malloc a temp buf.\n");
		pBufPtrOrg  = kmalloc(u16Num+4, GFP_KERNEL);
		pp=pBufPtrOrg;
		while((((u32) pp) % 4)!=0)
			pp=pp+1;
		pBufPtrSave = pp;
	}
	else
		pp = pu8Buffer;
	
	DBG_CTRLL("Read data from Cx FIFO.\n");
	dwpp = (u32 *) pp;
	for (u16_i = (u16Num >> 2); u16_i > 0; u16_i --) {
        //while((bFUSBPort(0x0B)& BIT5));		// Check CX FIFO is not empty, if it's empty, wait host send data.
		dwTemp = mUsbEP0DataRdDWord();
		*dwpp = dwTemp;
		dwpp++;
	}
	pp = (u8 *) dwpp;
	
    //while((bFUSBPort(0x0B)& BIT5));		// Check CX FIFO is not empty, if it's empty, wait host send data.
	switch (u16Num % 4)
	{
		case 1:
			*(pp) = mUsbEP0DataRdByte0();
			break;
		case 2:
			wTemp = mUsbEP0DataRdWord();
			*pp++ = (u8)wTemp;
			*pp= (u8)(wTemp >> 8);	
			break;
		case 3:
			wTemp = mUsbEP0DataRdWord();
			*pp 		= (u8)wTemp;
			*(pp + 1)= (u8)(wTemp >> 8);	
			pp = pp + 2;

#if (USB_DataPort == NoFixPort)
			*(pp) 	= mUsbEP0DataRdByte2();
#elif(USB_DataPort == FixPort)
			*(pp) 	= mUsbEP0DataRdByte0();
#endif
			break;
		default:
			break;
	}

	// if create a buffer, save data into real buffer and free it now.
	//if((((u32) pu8Buffer) % 4)!=0)

    if (pBufPtrOrg!=NULL) {
		DBG_CTRLL("Copy temp buf data to real buf.\n");
		memcpy( pu8Buffer, pBufPtrSave, u16Num);			
		kfree(pBufPtrOrg);
	}
	DBG_CTRLL("Exit Cx Read.\n");
	return u16Num;
}

// return:  0 = still running, 1 = completed, negative = errno
static int write_fifo(struct FTC_ep *ep, struct FTC_request *req)
{
	struct FTC_udc	*dev = ep->dev;
	u8		*buf;
	int		is_last;
	u8      u8fifo_n,usb_interrupt_Source;
	unsigned	length, count;
	u8 usb_interrupt_reg;

	DBG_FUNCC("+write_fifo() : actual = 0x%x\n",req->req.actual);
write_fifo_top:
	buf = req->req.buf + req->req.actual;  // current location
	prefetch(buf);

	dev = ep->dev;
	if (unlikely(ep->num == 0 && dev->ep0state != EP0_IN))
		return -EL2HLT;
	if (ep->num > 0) 
	{  
		// EP1,2,3...
		u8fifo_n = mUsbEPMapRd(ep->num);		// get the relatived FIFO number
		if (ep->is_in)
			u8fifo_n &= 0x0F;
		else
			u8fifo_n >>= 4;
		if (u8fifo_n >= FUSB220_MAX_FIFO)	// over the Max. fifo count ?
			return -EINVAL;

		// Check the FIFO had been enable ?
		if ((mUsbFIFOConfigRd(u8fifo_n) & 0x80) == 0)
			return -EINVAL;
	}
	else
		u8fifo_n = 0; //useless

	//length = min(req->req.length - req->req.actual, ep->ep.maxpacket); maked for porting to 2.6
//	printk("[write_fifo]req->req.length = %d\n",req->req.length);
//	printk("[write_fifo]ep->ep.maxpacket = %d\n",ep->ep.maxpacket);
	if((length = req->req.length - req->req.actual) > ep->ep.maxpacket)
		length = ep->ep.maxpacket;
	req->req.actual += length;
	DBG_FUNCC("+write_fifo() : actual = 0x%x\n",req->req.actual);

	if (ep->num == 0)    //EP0
		count = vUsbCxFWr( buf, length);	// return this time real transfer length
	else
	{
		//For Bulk-in Interrupt Enable/Disable
		if (req->req.length<=(req->req.actual)) 
			FIFO_Int(ep,0); //Disable the Bulk-In, Before write last packet
		else 
			FIFO_Int(ep,1); //Enable the Bulk-In, Because Remain Size>0
		count = vUsbDxFWr(u8fifo_n, buf, length);
		if (unlikely(count!=length))
			ERROR(dev,"Write FIFO Fail(count!=length)...\n");
	}

	/* last packet often short (sometimes a zlp, especially on ep0) */
	// If this time real transfer length!= max packet size, this time is a last packet.
	if ((unlikely(count != ep->ep.maxpacket)) ) {
		if (ep->num == 0) {
//			dev->ep[0].stopped = 1;
			dev->ep0state = EP0_STATUS;
		}
		is_last = 1;
	} 
	else {
		if (likely(req->req.length != req->req.actual))  //|| req->req.zero)
			is_last = 0;
		else
			is_last = 1;
	}
	/* requests complete when all IN data is in the FIFO,
	 * or sometimes later, if a zlp was needed.
	 */
	 // In here we also need to check if there is a reset or suspend int, must stop this transmit.
	usb_interrupt_reg = mUsbIntByte7Rd();
	if ((is_last) || (usb_interrupt_reg & (BIT1|BIT2))) {
		done(ep, req, 0);
		if(usb_interrupt_reg & (BIT1|BIT2))
			printk("There is a Rst/Susp int when write_fifo(), exit this function.\n");
		return 1;
	}
	else { //Bruce
		//For Host-In ==> Write FIFO
		if (ep->num > 0) {
			usb_interrupt_Source = mUsbIntByte5Rd(); //For Host-In 
			if (usb_interrupt_Source & (BIT0))//For Host-In (FIFO-Empty)
				goto write_fifo_top;
		}
		if(ep->num == 0) {
			while(!(bFUSBPort(0x21)&BIT1));		// if this is not the last cx packet and CX IN interrupt not rising,
												// wait for CX IN interrupt rising
			goto write_fifo_top;			
		}
	}

	return 0;
}

static int read_fifo(struct FTC_ep *ep, struct FTC_request *req)
{
	u32		 size;
	u8		*buf;
	unsigned is_short;
	u8      u8fifo_n,usb_interrupt_Source;
	int count;
	unsigned bufferspace;
	u8 usb_interrupt_reg;
	DBG_FUNCC("+read_fifo(): len = 0x%x, actual = 0x%x\n",req->req.length,req->req.actual);
read_fifo_top:
	
	buf = req->req.buf + req->req.actual;
	prefetchw(buf);

	//EP0 should be OUT stage
	if (unlikely(ep->num == 0 && ep->dev->ep0state != EP0_OUT))
		return -EL2HLT;

	if (ep->num > 0) {  
		// EP1,2,3...
		u8fifo_n = mUsbEPMapRd(ep->num);		// get the relatived FIFO number
		if (ep->is_in)
			u8fifo_n &= 0x0F;
		else
			u8fifo_n >>= 4;
		if (u8fifo_n >= FUSB220_MAX_FIFO)	// over the Max. fifo count ?
			return -EINVAL;

		// Check the FIFO had been enable ?
		if ((mUsbFIFOConfigRd(u8fifo_n) & 0x80) == 0)
			return -EINVAL;
	}
	else
		u8fifo_n = 0;  //useless

	bufferspace = req->req.length - req->req.actual;
	if (likely(ep->num != 0 )) {
		// For non-ep0 endpoint, we must read FIFO byte Cnt to deceide
		// data length we are going to read
		if( bufferspace != 0) 
			size = mUsbFIFOOutByteCount(u8fifo_n);
		else
			size=0;
	}
	else {
		// because there is no byte counter for cx fifo, so we must count 
		// this time transmit data length
		//	size = min(req->req.length - req->req.actual, ep->ep.maxpacket);
		if((size = req->req.length - req->req.actual) > ep->ep.maxpacket)
			size = ep->ep.maxpacket;
	}

	req->req.actual += size;
	is_short = (size < ep->ep.maxpacket);
	
#ifdef USB_TRACE
	VDBG(ep->dev, "read %s %u bytes%s OUT req %p %u/%u\n",
		ep->ep.name, size, is_short ? "/S" : "",
		req, req->req.actual, req->req.length);
#endif
	if (ep->num == 0) {    //EP0
		if(size>0) {
			while(!(bFUSBPort(0x21)&BIT2));		// wait for CX OUT interrupt rising	
			count = vUsbCxFRd( buf, size);
		}
	}
	else {
		//For Bulk-Out Interrupt Enable/Disable
		count = vUsbDxFRd(u8fifo_n, buf, size);
		if (req->req.length  > req->req.actual)
			FIFO_Int(ep,1);	//Enable the Bulk-Out, Because Remain Size>0
		else
			FIFO_Int(ep,0);	//Disable the Bulk-Out, Because Remain Size=0
	}

	/* completion */
	// In here we also need to check if there is a reset or suspend int, must stop this transmit.
	usb_interrupt_reg = mUsbIntByte7Rd();
	if ((unlikely(is_short || (req->req.actual == req->req.length))) || (usb_interrupt_reg & (BIT1|BIT2)))
	{
		if (unlikely(ep->num == 0)) {
			/* ep0out status stage */
			ep->stopped = 1;
			ep->dev->ep0state = EP0_STATUS;
		}	
		
 		done(ep, req, 0);

		if (usb_interrupt_reg & (BIT1|BIT2)) {
			printk("There is a Rst/Susp int when read_fifo(), exit this function.\n");
			return 1;
		}
		//Accelerate Bulk Out EP=> Check the Bulk OUT FIFO Full
		// and read Bulk OUT FIFO again (only for Bulk transfer)
		// If next queue already existand BULK OUT interrupt rising,
		// just go to read FIFO again.
		if (ep->num == BULK_OUT_EP_NUM) {
			usb_interrupt_Source = mUsbIntByte1Rd();	//Check BULK-Out Interrupt 
			if (usb_interrupt_Source & (BIT5|BIT4)) {	//Check BULK-Out Interrupt
				if (!list_empty(&ep->queue)) {
					req = list_entry(ep->queue.next,struct FTC_request, queue);
					goto read_fifo_top;
				}
			}
		}
		return 1;
	}
	else {
		//Accelerate Bulk Out EP=> Check the Bulk OUT FIFO Full
		// and read Bulk OUT FIFO again (only for Bulk transfer)
		if (ep->num == BULK_OUT_EP_NUM) {
			usb_interrupt_Source = mUsbIntByte1Rd();
			if (usb_interrupt_Source & (BIT5|BIT4))	//Check BULK-Out Interrupt 
				goto read_fifo_top;
		}
		if(ep->num == 0)
			goto read_fifo_top;
	}	
	return 0;
}

 inline void
pio_advance(struct FTC_ep *ep)
{
	struct FTC_request	*req;

	DBG_FUNCC("+pio_advance()\n");

	if (unlikely(list_empty (&ep->queue)))
		return;
	DBG_BULKK("FTC_udc => pio_advance() ==> list_entry(ep->queue.next, struct FTC_request, queue)");    		
	req = list_entry(ep->queue.next, struct FTC_request, queue);
	DBG_BULKK("FTC_udc => pio_advance() ==> ep->is_in ?...");    		
	(ep->is_in ? write_fifo : read_fifo)(ep, req);
}

/*-------------------------------------------------------------------------*/
//============= AHB_DMA function start ==================
// FUSB220 to enable/disable DMA of dedicated EP 
#ifdef CONFIG_GM_FUSB220_DMA
static int USB_DMA_Enable(struct FTC_ep *ep,unsigned int val)
{
	u8  u8fifo_n;  
	u32 offset; 
	u8  mval;
	DBG_FUNCC("+USB_DMA_Enable()\n");

	u8fifo_n = mUsbEPMapRd(ep->num);		// get the relatived FIFO number
	if (ep->is_in)
		u8fifo_n &= 0x0F;
	else
		u8fifo_n >>= 4;
	if (u8fifo_n >= FUSB220_MAX_FIFO)	// over the Max. fifo count ?
		return -EINVAL;
	
	
	offset = 0x7e;
	if (u8fifo_n >= 8) {
		offset++;
		mval = 1 << (u8fifo_n-8);
	}
	else {
		mval = 1 << u8fifo_n;
	}

	if (val)  //enable DMA
		mUsbFIFODMAEn(offset,mval);
	else
		mUsbFIFODMADis(offset,mval);
	return 0;
}

// return:  0 = q running, 1 = q stopped, negative = errno

static int start_dma(struct FTC_udc *dev,struct FTC_ep *ep, struct FTC_request *req)
{
//	struct FTC_udc *dev = ep->dev;
	u32		       start ;
	//u32			   end = start + req->req.length - 1;
	ahb_dma_parm_t parm;
	u8 u8fifo_n;
	u32 temp;
	
	DBG_FUNCC("+start_dma()\n");


	/* set up dma mapping in case the caller didn't set*/
	if (req->req.dma == DMA_ADDR_INVALID)
	{
		DBG_TEMP("....pci_map_single len = %d\n",req->req.length);

		// important : DMA length will set as 16*n bytes
		temp = req->req.length / 16;
		if (req->req.length % 16)
		   temp++;
		req->req.dma = pci_map_single((void *)dev, req->req.buf, temp*16, //USB_EPX_BUFSIZ,  
						ep->is_in ? PCI_DMA_TODEVICE : PCI_DMA_FROMDEVICE);
		req->mapped = 1;
	}
	start = req->CurDmaStartAddr;
	
	// EP1,2,3...
	u8fifo_n = mUsbEPMapRd(ep->num);		// get the relatived FIFO number
	if (ep->is_in)
		u8fifo_n &= 0x0F;
	else
		u8fifo_n >>= 4;
	if (u8fifo_n >= FUSB220_MAX_FIFO)	// over the Max. fifo count ?
		return -EINVAL;

	// Check the FIFO had been enable ?
	if ((mUsbFIFOConfigRd(u8fifo_n) & 0x80) == 0)
		return -EINVAL;

	dev->EPUseDMA = ep->num;
	dev->ReqForDMA = req;
	// If use DMA, no FIFO interrupt for FIFO
	FIFO_Int(ep,0);

//	printk("[Brad] transfer length = %d\n",req->req.length);
	// We must seperate into several transmit times.
	// It's because AHB_DMA max tx length is only 8k bytes.
	if(req->req.actual < req->req.length)
	{
		if ((req->req.length-req->req.actual)>AHB_DMA_MAX_LEN)
			req->u32DMACurrTxLen=AHB_DMA_MAX_LEN;
		else
			req->u32DMACurrTxLen=req->req.length-req->req.actual;
	}
	DBG_FUNCC("+start_dma, start addr= 0x%x,total len=%d, Cur len=%d, Dir=%s\n",
		start,req->req.length,req->u32DMACurrTxLen,(ep->is_in)?"IN":"OUT");
	
	/* re-init the bits affecting IN dma; careful with zlps */
	DBG_TEMP("start_dma()-fill AHB dma structure.\n");


	if (likely(ep->is_in)) 
	{
		parm.src= start;
		parm.dest= (u32) (&(mDMAUsbRdDWord(u8fifo_n)));    
		parm.sw=AHBDMA_WIDTH_32BIT;
		parm.dw=AHBDMA_WIDTH_32BIT;
		parm.sctl=AHBDMA_CTL_INC;
		parm.dctl=AHBDMA_CTL_FIX;
		parm.size= req->u32DMACurrTxLen >> 2;   //beacuse we xfr 4 bytes/each time
		parm.irq=AHBDMA_TRIGGER_IRQ;
	
	} 
	else 
	{
		parm.src= (u32) (&(mDMAUsbRdDWord(u8fifo_n)));
		parm.dest= start;
		parm.sw=AHBDMA_WIDTH_32BIT;
		parm.dw=AHBDMA_WIDTH_32BIT;
		parm.sctl=AHBDMA_CTL_FIX;
		parm.dctl=AHBDMA_CTL_INC;
		parm.size= req->u32DMACurrTxLen >> 2;   //beacuse we xfr 4 bytes/each time
		parm.irq=AHBDMA_TRIGGER_IRQ;
	}

      fa_set_ahb_dma_src_params(dev->chForDMA, parm.src, AHBDMA_MASTER_0, parm.sw, parm.sctl);
      fa_set_ahb_dma_dst_params(dev->chForDMA, parm.dest, AHBDMA_MASTER_0, parm.dw, parm.dctl);

      fa_set_ahb_dma_transfer_params2( dev->chForDMA, AHBDMA_MASTER_0, req->u32DMACurrTxLen, 0);
      fa_ahb_dma_add_auto(dev->chForDMA,req->u32DMACurrTxLen);


//	DBG_TEMP("start_dma()-add AHB dma structure to AHB link.(ahb_dma=0x%x)\n",(u32)dev->ahb_dma);
	//ahb_dma_add(dev->ahb_dma,&parm);   2.4 dma 
	
	DBG_TEMP("start_dma()-enable FUSB220 DMA reg.\n");
	USB_DMA_Enable(ep,1);
	
	DBG_TEMP("start_dma()-start AHB dma.\n");
	//ahb_dma_start(dev->ahb_dma);	2.4 dma
      fa_ahb_dma_start(dev->chForDMA);

	ep->dma_running=TRUE;

	// important : Some A320d platform quality is not good,
	// if use DMA mode to transmit data fail, please add mdelay() as bellow.
	// Then transmit fail problem maybe fixed.
 //  	mdelay(20);

	DBG_FUNCC("-start_dma finish\n");
	return 0;
}
///////////////////////////////////////////////////////////////////////////////
//		vCheckDMA()
//		Description: This function is just like the end of read_fifo() or write_fifo()
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
void vCheckDMA(int ch, u16 int_status, void *data) //(struct FTC_udc *dev)
{
	u8 bDMA_Finish = FALSE;
	//u32 IntStatus;
	struct FTC_udc *dev = data; 
	unsigned long flags;
	struct FTC_ep *ep;
	struct FTC_request *req;
	u8  u8fifo_n;  

	DBG_TEMP("vCheckDMA()-AHB dma prcess finish.\n");
	// IntStatus = ahbdma_get_status(dev->ahb_dma); 2.6 fa_ahb_dma_get_status
	//fa_ahb_dma_get_status(ch);  // 2.6 DMA function , already get status in fa_ahb_dma_irq_handler
	
	    
	if (int_status & INT_DMA_ERROR) {     // 2.4 use INT_ERROR
		printk("FUSB220 FIFO%d use AHB DMA Error\n",dev->EPUseDMA);	
		bDMA_Finish = TRUE;
	}
	else if ((int_status & INT_DMA_TRIGGER)){ // 2.4 use INT_TRIGGER
		bDMA_Finish = TRUE;
	}
	else
		DBG_TEMP_udc("Error: Rising DMA int, but no finish or error status\n");
	
	spin_lock_irqsave(&dev->lock, flags);
	if (bDMA_Finish) {
		// Get endpoint information
		ep = &dev->ep[dev->EPUseDMA];				
		// Get endpoint request for DMA
		req = dev->ReqForDMA;	
		
		req->req.actual +=  req->u32DMACurrTxLen;   		//just set "actual transmit length"="request length"
		req->CurDmaStartAddr += req->u32DMACurrTxLen;
//		printk("[Brad]req->req.actual =%d, req->req.length =%d\n",
//			    req->req.actual,req->req.length);
		if(req->req.actual < req->req.length)
		{
			// if DMA not yet finish the job, just re-open ep int, and wait 
			// interrupt to enable dma
			FIFO_Int(ep,1);
		}
		else
		{
			// Use DMA EP NUM to get relatived FIFO number
			u8fifo_n = mUsbEPMapRd(dev->EPUseDMA);		
			if (ep->is_in)
				u8fifo_n &= 0x0F;
			else
				u8fifo_n >>= 4;

			if (u8fifo_n >= FUSB220_MAX_FIFO)	
			{   // over the Max. fifo count ?
				printk("....OOP: vCheckDMA() FIFO out of range\n");
				return;
			}

			if (ep->is_in) {
				// if it's IN EP, must set done to finish this transfer.
				mUsbFIFODone(u8fifo_n);
			}
			else  {
				// out endpoint 
				if ((mUsbIntByte1Rd() & (BIT5 | BIT4)))   //check HW error
					DBG_BULKK("FUSB220 Bulk OUT use DMA finish, but still have %d bytes in FIFO\n", mUsbFIFOOutByteCount(u8fifo_n)); 
			}

			dev->EPUseDMA = FUSB220_DMA_IS_IDLE_NOW;	//reset
			USB_DMA_Enable(ep,0);
			done(ep, req, 0);
			dev->ReqForDMA=0;
            		//fa_ahb_dma_stop(ch);

		}
		ep->dma_running=FALSE;
	}
	
	spin_unlock_irqrestore(&dev->lock, flags);

}
// For 2.6 DMA

static irqreturn_t dma_interrupt_handler(int irq, void *_dev, struct pt_regs *dummy)
{
	struct FTC_udc	*dev = _dev;
	unsigned long flags;

	spin_lock_irqsave(&dev->lock, flags);

	//printk("dev addr = 0x%x\n", (u32) dev);
	//printk("struc = 0x%x\n", (u32) dev->ahb_dma);
	//printk("DMA base = 0x%x\n", dev->ahb_dma->base);
	//printk("DMA channel base = 0x%x\n", dev->ahb_dma->channel_base);
	
	//vCheckDMA(dev); 

	// clear interrupt only after processing 
	//ahbdma_clear_int(dev->ahb_dma);  // fa_ahb_dma_clear_int(dev->ahb_dma->channel) in 2.6
	
	spin_unlock_irqrestore(&dev->lock, flags);

	return IRQ_RETVAL(1);
}
//==================== AHB DMA function finish ==============

#endif
static int
FTC_queue(struct usb_ep *_ep, struct usb_request *_req, unsigned gfp_flags)
{
	struct FTC_request	*req;
	struct FTC_ep		*ep;
	struct FTC_udc		*dev;
	unsigned long		 flags;
	int			status;
	int temp;
    //Remove;;u32 temp;

	DBG_FUNCC("+FTC_queue()\n");

	/* always require a cpu-view buffer so pio works */
	req = container_of(_req, struct FTC_request, req);
	if (unlikely(!_req || !_req->complete || !_req->buf || !list_empty(&req->queue)))
	{
		if(!_req)
			printk("FTC_queue() return fail-1-1,_req=0x%x \n",(u32)_req);
		if(!_req->complete)
			printk("FTC_queue() return fail-1-2,_req->complete=0x%x \n",(u32)_req->complete);
		if(!_req->buf)
			printk("FTC_queue() return fail-1-3,_req->buf=0x%x \n",(u32)_req->buf);
		if(!list_empty(&req->queue))
			printk("FTC_queue() return fail-1-4\n");
			
		printk("FTC_queue() return fail-1\n");
		return -EINVAL;
	}
	ep = container_of(_ep, struct FTC_ep, ep);
	if (unlikely(!_ep || (!ep->desc && ep->num != 0)))
	{
		printk("FTC_queue() return fail-2\n");
		return -EINVAL;
	}
	dev = ep->dev;
	if (unlikely(!dev->driver || dev->gadget.speed == USB_SPEED_UNKNOWN))
		return -ESHUTDOWN;

	/* can't touch registers when suspended */
	if (dev->ep0state == EP0_SUSPEND)
	{
		printk("FTC_queue() return fail (ep0state == EP0_SUSPEND) \n");
		return -EBUSY;
	}
	
#ifdef USB_TRACE
	VDBG(dev, "%s queue req %p, len %u buf %p\n",
			_ep->name, _req, _req->length, _req->buf);
#endif

	spin_lock_irqsave(&dev->lock, flags);

	_req->status = -EINPROGRESS;
	_req->actual = 0;

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

	temp=list_empty(&ep->queue);		
	if (list_empty(&ep->queue) && likely(!ep->stopped)) 
	{		
		/* dma:  done after dma completion IRQ (or error)
		* pio:  done after last fifo operation
		*/
		if (ep->num>0)
			FIFO_Int(ep,1);//Enable the Bulk-Out, Because Remain Size>0
		else
		{
			//For Bulk-In or ep->num=0
			status = (ep->is_in ? write_fifo : read_fifo)(ep, req);
			if (unlikely(status != 0)) 
			{
				if (status > 0)
					status = 0;
				req = 0;
			}  	   
		}
	} /* else pio or dma irq handler advances the queue. */

	if (likely(req != 0)) 
	{
		DBG_CTRLL("add request into queue(ep->stopped=%d)(list_empty(&ep->queue)=%d)\n",(int)(ep->stopped),temp);
		list_add_tail(&req->queue, &ep->queue);
	}

	spin_unlock_irqrestore(&dev->lock, flags);
	return status;
}


/*-------------------------------------------------------------------------*/

static inline char *dmastr(struct FTC_udc *dev)
{
	DBG_FUNCC("+dmastr()\n");

	if (dev->Dma_Status == PIO_Mode)
		return "(dma disabled)";
	else if (dev->Dma_Status == AHB_DMA)
		return "(AHB DMA)";
	else
		return "(APB DMA)";
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// config FIFO
//-----------------------------------------------------------------------
/////////////////////////////////////////////////////
//		vUsbFIFO_EPxCfg_HS(void)
//		Description:
//			1. Configure the FIFO and EPx map
//		input: none
//		output: none
/////////////////////////////////////////////////////
static void vUsbFIFO_EPxCfg_FS(void)
{
	int i;
	DBG_FUNCC("+vUsbFIFO_EPxCfg_FS()\n");

	//EP4
	mUsbEPMap(EP4, FULL_EP4_Map);
	mUsbFIFOMap(FULL_ED4_FIFO_START, FULL_EP4_FIFO_Map);
	mUsbFIFOConfig(FULL_ED4_FIFO_START,FULL_EP4_FIFO_Config);
							
	for(i = (FULL_ED4_FIFO_START + 1) ;
	i < (FULL_ED4_FIFO_START + (FULL_ED4_bBLKNO *FULL_ED4_bBLKSIZE)) ;
	i ++)
	{
		mUsbFIFOConfig(i, (FULL_EP4_FIFO_Config & (~BIT7)) );
	}
	mUsbEPMxPtSzHigh(EP4, FULL_ED4_bDIRECTION, (FULL_ED4_MAXPACKET & 0x7ff) );
	mUsbEPMxPtSzLow(EP4, FULL_ED4_bDIRECTION, (FULL_ED4_MAXPACKET & 0x7ff) );
	mUsbEPinHighBandSet(EP4 , FULL_ED4_bDIRECTION ,FULL_ED4_MAXPACKET);

	//EP3
	mUsbEPMap(EP3, FULL_EP3_Map);
	mUsbFIFOMap(FULL_ED3_FIFO_START, FULL_EP3_FIFO_Map);
	mUsbFIFOConfig(FULL_ED3_FIFO_START, FULL_EP3_FIFO_Config);

	for(i = FULL_ED3_FIFO_START + 1 ;
	    i < FULL_ED3_FIFO_START + (FULL_ED3_bBLKNO *FULL_ED3_bBLKSIZE) ;
	    i ++)
	{
		mUsbFIFOConfig(i, (FULL_EP3_FIFO_Config & (~BIT7)) );
	}
	mUsbEPMxPtSzHigh(EP3, FULL_ED3_bDIRECTION, (FULL_ED3_MAXPACKET & 0x7ff) );
	mUsbEPMxPtSzLow(EP3, FULL_ED3_bDIRECTION, (FULL_ED3_MAXPACKET & 0x7ff) );
	mUsbEPinHighBandSet(EP3 , FULL_ED3_bDIRECTION , FULL_ED3_MAXPACKET);

	
	//EP2
	mUsbEPMap(EP2, FULL_EP2_Map);
	mUsbFIFOMap(FULL_ED2_FIFO_START, FULL_EP2_FIFO_Map);
	mUsbFIFOConfig(FULL_ED2_FIFO_START, FULL_EP2_FIFO_Config);

	for(i = FULL_ED2_FIFO_START + 1 ;
	    i < FULL_ED2_FIFO_START + (FULL_ED2_bBLKNO *FULL_ED2_bBLKSIZE) ;
	    i ++)
	{
		mUsbFIFOConfig(i, (FULL_EP2_FIFO_Config & (~BIT7)) );
	}
	mUsbEPMxPtSzHigh(EP2, FULL_ED2_bDIRECTION, (FULL_ED2_MAXPACKET & 0x7ff) );
	mUsbEPMxPtSzLow(EP2, FULL_ED2_bDIRECTION,(FULL_ED2_MAXPACKET & 0x7ff) );
	mUsbEPinHighBandSet(EP2 , FULL_ED2_bDIRECTION , FULL_ED2_MAXPACKET);

	//EP1
	mUsbEPMap(EP1, FULL_EP1_Map);
	mUsbFIFOMap(FULL_ED1_FIFO_START, FULL_EP1_FIFO_Map);
	mUsbFIFOConfig(FULL_ED1_FIFO_START, FULL_EP1_FIFO_Config);

	for(i = FULL_ED1_FIFO_START + 1 ;
	    i < FULL_ED1_FIFO_START + (FULL_ED1_bBLKNO *FULL_ED1_bBLKSIZE) ;
	    i ++)
	{
		mUsbFIFOConfig(i, (FULL_EP1_FIFO_Config & (~BIT7)) );
	}
	
	mUsbEPMxPtSzHigh(EP1, FULL_ED1_bDIRECTION, (FULL_ED1_MAXPACKET & 0x7ff) );
	mUsbEPMxPtSzLow(EP1, FULL_ED1_bDIRECTION,  (FULL_ED1_MAXPACKET & 0x7ff) );
	
	mUsbEPinHighBandSet(EP1 , FULL_ED1_bDIRECTION , FULL_ED1_MAXPACKET);
}


static void vUsbFIFO_EPxCfg_HS(void)
{
	int i;
	DBG_FUNCC("+vUsbFIFO_EPxCfg_HS()\n");
				
	//EP4
	mUsbEPMap(EP4, HIGH_EP4_Map);
	mUsbFIFOMap(HIGH_ED4_FIFO_START, HIGH_EP4_FIFO_Map);
	mUsbFIFOConfig(HIGH_ED4_FIFO_START,HIGH_EP4_FIFO_Config);
							
	for(i = HIGH_ED4_FIFO_START + 1 ;
	    i < HIGH_ED4_FIFO_START + (HIGH_ED4_bBLKNO *HIGH_ED4_bBLKSIZE) ;
	    i ++)
	{
		mUsbFIFOConfig(i, (HIGH_EP4_FIFO_Config & (~BIT7)) );
	}
	mUsbEPMxPtSzHigh(EP4, HIGH_ED4_bDIRECTION, (HIGH_ED4_MAXPACKET & 0x7ff) );
	mUsbEPMxPtSzLow(EP4, HIGH_ED4_bDIRECTION, (HIGH_ED4_MAXPACKET & 0x7ff) );
	mUsbEPinHighBandSet(EP4 , HIGH_ED4_bDIRECTION , HIGH_ED4_MAXPACKET);

	//EP3
	mUsbEPMap(EP3, HIGH_EP3_Map);
	mUsbFIFOMap(HIGH_ED3_FIFO_START, HIGH_EP3_FIFO_Map);
	mUsbFIFOConfig(HIGH_ED3_FIFO_START, HIGH_EP3_FIFO_Config);

	for(i = HIGH_ED3_FIFO_START + 1 ;
	    i < HIGH_ED3_FIFO_START + (HIGH_ED3_bBLKNO *HIGH_ED3_bBLKSIZE) ;
	    i ++)
	{
		mUsbFIFOConfig(i, (HIGH_EP3_FIFO_Config & (~BIT7)) );
	}
	mUsbEPMxPtSzHigh(EP3, HIGH_ED3_bDIRECTION, (HIGH_ED3_MAXPACKET & 0x7ff) );
	mUsbEPMxPtSzLow(EP3, HIGH_ED3_bDIRECTION,(HIGH_ED3_MAXPACKET & 0x7ff) );
	mUsbEPinHighBandSet(EP3 , HIGH_ED3_bDIRECTION ,HIGH_ED3_MAXPACKET);
	
	//EP2
	mUsbEPMap(EP2, HIGH_EP2_Map);
	mUsbFIFOMap(HIGH_ED2_FIFO_START, HIGH_EP2_FIFO_Map);
	mUsbFIFOConfig(HIGH_ED2_FIFO_START, HIGH_EP2_FIFO_Config);

	for(i = HIGH_ED2_FIFO_START + 1 ;
	    i < HIGH_ED2_FIFO_START + (HIGH_ED2_bBLKNO *HIGH_ED2_bBLKSIZE) ;
	    i ++)
	{
		mUsbFIFOConfig(i, (HIGH_EP2_FIFO_Config & (~BIT7)) );
	}
	mUsbEPMxPtSzHigh(EP2, HIGH_ED2_bDIRECTION, (HIGH_ED2_MAXPACKET & 0x7ff) );
	mUsbEPMxPtSzLow(EP2, HIGH_ED2_bDIRECTION, (HIGH_ED2_MAXPACKET & 0x7ff) );
	mUsbEPinHighBandSet(EP2 , HIGH_ED2_bDIRECTION , HIGH_ED2_MAXPACKET);

	//EP1
	mUsbEPMap(EP1, HIGH_EP1_Map);
	mUsbFIFOMap(HIGH_ED1_FIFO_START, HIGH_EP1_FIFO_Map);
	mUsbFIFOConfig(HIGH_ED1_FIFO_START, HIGH_EP1_FIFO_Config);

	for(i = HIGH_ED1_FIFO_START + 1 ;
	    i < HIGH_ED1_FIFO_START + (HIGH_ED1_bBLKNO *HIGH_ED1_bBLKSIZE) ;
	    i ++)
	{
		mUsbFIFOConfig(i, (HIGH_EP1_FIFO_Config & (~BIT7)) );
	}
	mUsbEPMxPtSzHigh(EP1, HIGH_ED1_bDIRECTION, (HIGH_ED1_MAXPACKET & 0x7ff) );
	mUsbEPMxPtSzLow(EP1, HIGH_ED1_bDIRECTION,(HIGH_ED1_MAXPACKET & 0x7ff) );
	mUsbEPinHighBandSet(EP1 , HIGH_ED1_bDIRECTION , HIGH_ED1_MAXPACKET);				
}

////////////////////////////////////////////////
// check the endpoint to Read/Write Data 
static void
EPX_check_advance(struct FTC_udc *dev,struct FTC_ep *ep)
{
	struct FTC_request	*req;
#ifdef	CONFIG_GM_FUSB220_DMA
	int dma_status;
#endif
	// u8 ep_num;
	// struct zero_dev *pzero_dev;
	DBG_FUNCC("+EPX_check_advance()\n");
	
	if (unlikely(list_empty (&ep->queue))) {
		//queue is empty => Disable Interrupt enable
		FIFO_Int(ep,0); //Disable Interrupt
		
	}
	else {
		
		if( dev->EPUseDMA == FUSB220_DMA_IS_IDLE_NOW) {
			
			req = list_entry(ep->queue.next, struct FTC_request, queue);
			req->CurDmaStartAddr=req->req.dma;
		}
#ifdef CONFIG_GM_FUSB220_DMA
		else if ( (dev->EPUseDMA == ep->num) && (dev->ReqForDMA !=0) ) {
			// DMA does not finish it's last work.
			req = dev->ReqForDMA;  //get the request			
		}
#endif
		else
			{
			goto TO_DO_NOTHING;
			}
		
		DBG_CTRLL("ep **NON queue empty EP num = %d\n", ep->num);


//		printk("[EPX_check_advance] transfer length = %d\n",req->req.length);

		if ((u32)req->req.length==0){
			DBG_TEMP_udc("Error: Transfer length in request = 0\n");
			}	
			
#ifdef CONFIG_GM_FUSB220_DMA
		if(ep->dma_running == TRUE)
		{
			dma_status=fa_ahb_dma_get_status(0);
			DBG_TEMP_udc("dma_status = %d\n",dma_status);
			DBG_TEMP_udc("Error: DMA running but rising EP%d interrupt, bFUSBPort(0x12)=0x%x\n",
				ep->num,bFUSBPort(0x12));
			return;
		}
		else {
			if (ep->num ==BULK_IN_EP_NUM) {
				// If this is an Bulk IN endpoint and the length we want to transmit is 4 byte alignment, 
				// we can use DMA to transmit this data.
				if (((u32)req->req.length&0xf)==0) {
					start_dma(dev,ep, req);
					return;
				}
			}
			if (ep->num ==BULK_OUT_EP_NUM) {
				// If this is an Bulk OUT endpoint and the length we want to receive is 4 byte alignment, 
				// we can use DMA to receive this data.
				// ( in here we use byte count to check receive data length and request data length to check)
				if( (((u32)mUsbFIFOOutByteCount(((mUsbEPMapRd(ep->num))>>4)) & 0xf) == 0)&&
					(((u32)req->req.length&0xf)==0)) {
					start_dma(dev,ep, req);
					return;
				}
			}		
		}
#endif
	
		(ep->is_in ? write_fifo : read_fifo)(ep, req);
		return;

		TO_DO_NOTHING:
		DBG_TEMP("DMA is enable now, can't service any other data transfer for ep%d, dma is busy for ep%d\n",
			ep->num,dev->EPUseDMA);

	}
}

/*-------------------------------------------------------------------------*/
// GM USB initial code
///////////////////////////////////////////////////////////////////////////////
//		vFUSB220Init()
//		Description:
//			1. Turn on the "Global Interrupt Enable" bit of FUSB220
//			2. Turn on the "Chip Enable" bit of FUSB200
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vFUSB220Init(void)
{
	DBG_FUNCC("+vFUSB220Init()\n");

	// suspend counter
	mUsbIdleCnt(7);
	
	// Clear interrupt
	mUsbIntBusRstClr();
	mUsbIntSuspClr();
	mUsbIntResmClr();
	
	// Disable all fifo interrupt
	mUsbIntFIFO0_3OUTDis();
	mUsbIntFIFO4_7OUTDis();
	mUsbIntFIFO8_9OUTDis();
	mUsbIntFIFO0_7INDis();
	mUsbIntFIFO8_9INDis();
	
	// Soft Reset
	mUsbSoftRstSet(); 			// All circuit change to which state after Soft Reset?
	mUsbSoftRstClr();
	
	// Clear all fifo
	mUsbClrAllFIFOSet();		// will be cleared after one cycle.
	
	// move to udc_enable
	//// Enable usb200 global interrupt
	//mUsbGlobIntEnSet();
	//mUsbChipEnSet();
	
	//Bruce;;Clear mUsbIntEP0EndDis
	mUsbIntEP0EndDis();
}

///////////////////////////////////////////////////////////////////////////////
//		vUsbInit(struct FTC_udc *dev)
//		Description:
//			1. Configure the FIFO and EPx map.
//			2. Init FUSB220.
//			3. Set the usb interrupt source as edge-trigger.
//			4. Enable Usb interrupt.
//		input: none
//		output: none
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
	vFUSB220Init();
}

/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/

///////////////////////////////////////////////////////////////////////////////
//		vUsb_rst(struct FTC_udc	*dev)
//		Description:
//			1. Change descriptor table (High or Full speed).
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_rst(struct FTC_udc	*dev)
{
	//struct zero_dev *pzero_dev = (struct zero_dev *) dev->gadget.ep0->driver_data; 

	//Move to higher Init AP
	//vUsb_BulkInit();
	//vUsb_IntInit();
	
	DBG_FUNCC("+vUsb_rst()\n");		

	// stop
	INFO(dev,"L%x, Bus reset\n", dev->u8LineCount ++);

	//Bruce;;Mark;;12282004;;dev->driver->disconnect(&dev->gadget);
	mUsbIntBusRstClr();
	dev->gadget.speed = USB_SPEED_UNKNOWN;
}

///////////////////////////////////////////////////////////////////////////////
//		vUsb_suspend(dev)
//		Description:
//			1. .
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_suspend(struct FTC_udc	*dev)
{
	DBG_FUNCC("+vUsb_suspend()\n");
	INFO(dev,"L%x, Bus suspend\n", dev->u8LineCount ++);
	// uP must do-over everything it should handle and do before into the suspend mode
	// Go Suspend status
	mUsbIntSuspClr();
	
	//Bruce;;mUsbGoSuspend();
	dev->ep0state = EP0_SUSPEND;
}

///////////////////////////////////////////////////////////////////////////////
//		vUsb_resm(struct FTC_udc	*dev)
//		Description:
//			1. Change descriptor table (High or Full speed).
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_resm(struct FTC_udc	*dev)
{
	DBG_FUNCC("+vUsb_resm()\n");

	INFO(dev,"L%x, Bus resume\n", dev->u8LineCount ++);
	// uP must do-over everything it should handle and do before into the suspend mode
	// uP must wakeup immediately
	mUsbIntResmClr();

	dev->ep0state = EP0_IDLE;
}

///////////////////////////////////////////////////////////////////////////////
//		vUsb_Iso_SeqErr(struct FTC_udc *dev)
//		Description:
//			1. FUSB200 Detects High bandwidth isochronous Data PID sequential error.
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_Iso_SeqErr(struct FTC_udc *dev)
{
	u8 u8Tmp0 = mUsbIntIsoSeqErr0Rd();
	u8 u8Tmp1 = mUsbIntIsoSeqErr1Rd();
	u8 i;

	DBG_FUNCC("+vUsb_Iso_SeqErr()\n");

	for (i = 1; i < 8; i ++) {
	   if (u8Tmp0 & (BIT0 << i))
	      UWARN(dev,"L%x, EP%x Isochronous Sequential Error\n", dev->u8LineCount ++, i);
	}


	for (i = 0; i < 8; i ++) {
	   if(u8Tmp1 & (BIT0 << i))
	      UWARN(dev,"L%x, EP%x Isochronous Sequential Error\n", dev->u8LineCount ++, i+8);
	}

	mUsbIntIsoSeqErrClr();
	mUsbIntIsoSeqErr0Clr();
	mUsbIntIsoSeqErr1Clr();
}

///////////////////////////////////////////////////////////////////////////////
//		vUsb_Iso_SeqAbort(struct FTC_udc *dev)
//		Description:
//			1. FUSB200 Detects High bandwidth isochronous  Data PID sequential abort.
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_Iso_SeqAbort(struct FTC_udc *dev)
{
	u8 u8Tmp0 = mUsbIntIsoSeqAbort0Rd();		
	u8 u8Tmp1 = mUsbIntIsoSeqAbort1Rd();
	u8 i;

	DBG_FUNCC("+vUsb_Iso_SeqAbort()\n");

	for(i = 1; i < 8; i ++) {
		if (u8Tmp0 & (BIT0 << i))
		   UWARN(dev,"L%x, EP%x Isochronous Sequential Abort\n", dev->u8LineCount ++, i);
	}


	for(i = 0; i < 8; i ++) {
		if (u8Tmp1 & (BIT0 << i))
		   UWARN(dev,"L%x, EP%x Isochronous Sequential Abort\n", dev->u8LineCount ++, i+8);
	}

	 mUsbIntIsoSeqAbortClr();
	 mUsbIntIsoSeqAbort0Clr();		
	 mUsbIntIsoSeqAbort1Clr();		
}

///////////////////////////////////////////////////////////////////////////////
//		vUsb_TX0Byte(struct FTC_udc *dev)
//		Description:
//			1. Send 0 byte data to host.
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_TX0Byte(struct FTC_udc *dev)
{
	u8 u8Tmp0 = mUsbIntTX0Byte0Rd();
	u8 u8Tmp1 = mUsbIntTX0Byte1Rd();
	u8 i;

	DBG_FUNCC("+vUsb_TX0Byte()\n");

	for (i = 1; i < 8; i ++) {	
		if (u8Tmp0 & (BIT0 << i)) 
		   DBG_CTRLL("L%x, EP%x IN data 0 byte to host\n", dev->u8LineCount ++, i); 	   	   
	}


	for (i = 0; i < 8; i ++) {	
		if (u8Tmp1 & (BIT0 << i))
		   DBG_CTRLL("L%x, EP%x IN data 0 byte to host\n", dev->u8LineCount ++, i+8);  	   
	} 	 

	mUsbIntTX0ByteClr();
	mUsbIntTX0Byte0Clr();
	mUsbIntTX0Byte1Clr();
}

///////////////////////////////////////////////////////////////////////////////
//		vUsb_RX0Byte(struct FTC_udc *dev)
//		Description:
//			1. Receive 0 byte data from host.
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_RX0Byte(struct FTC_udc *dev)
{
	u8 u8Tmp0 = mUsbIntRX0Byte0Rd();
	u8 u8Tmp1 = mUsbIntRX0Byte1Rd();
	u8 i;

	DBG_FUNCC("+vUsb_RX0Byte()\n");

	for (i = 1; i < 8; i ++) {
	   if (u8Tmp0 & (BIT0 << i)) 
	      DBG_CTRLL("L%x, EP%x OUT data 0 byte to Device\n", dev->u8LineCount ++, i);        
	}


	for (i = 0; i < 8; i ++) {
	   if (u8Tmp1 & (BIT0 << i)) 
	      DBG_CTRLL("L%x, EP%x OUT data 0 byte to Device\n", dev->u8LineCount ++, i+8);    
	}	   

	mUsbIntRX0ByteClr();
	mUsbIntRX0Byte0Clr();
	mUsbIntRX0Byte1Clr();
}


void vUsbClrEPx(void)
{
	u8 u8ep;

	DBG_FUNCC("+vUsbClrEPx()\n");

	// Clear All EPx Toggle Bit
	for (u8ep = 1; u8ep <= FUSB220_MAX_EP; u8ep ++)
	{
		mUsbEPinRsTgSet(u8ep);
		mUsbEPinRsTgClr(u8ep);
	}
	for (u8ep = 1; u8ep <= FUSB220_MAX_EP; u8ep ++)
	{
		mUsbEPoutRsTgSet(u8ep);
		mUsbEPoutRsTgClr(u8ep);
	}
}

///////////////////////////////////////////////////////////////////////////////
//		bGet_status(struct FTC_udc *dev,const struct usb_ctrlrequest *ctrl)
//		Description: (add by Andrew)
//			1. Send 2 bytes status to host.
//		input: none
//		output: TRUE or FALSE (u8)
///////////////////////////////////////////////////////////////////////////////
static u8 bGet_status(struct FTC_udc *dev,const struct usb_ctrlrequest *ctrl)
{  
	u8 u8ep_n,u8fifo_n,RecipientStatusLow, RecipientStatusHigh;
	u8 u8Tmp[2];
	u8 bdir;
	
	DBG_FUNCC("+bGet_status()  ==> Add by Andrew\n");
	
	RecipientStatusLow = 0;
	RecipientStatusHigh = 0;
	switch((ctrl->bRequestType)&0x3)  // Judge which recipient type is at first
	{
   		case 0:					// Device
        	// Return 2-byte's Device status (Bit1:Remote_Wakeup, Bit0:Self_Powered) to Host
        	// Notice that the programe sequence of RecipientStatus
			RecipientStatusLow = mUsbRmWkupST() << 1;
			// Bit0: Self_Powered--> DescriptorTable[0x23], D6(Bit 6)
			// Now we force device return data as self power. (Andrew)
			RecipientStatusLow |= ((USB_CONFIG_ATT_SELFPOWER >> 6) & 0x01);
        	break;
 		case 1:					// Interface
			// Return 2-byte ZEROs Interface status to Host
    		break;

		case 2:					// Endpoint
			if(ctrl->wIndex == 0x00)
			{
				if(dev->ep0state == EP0_STALL)
	       			RecipientStatusLow = TRUE;
			}
			else
			{
				u8ep_n = (u8)ctrl->wIndex  & 0x7F;		// which ep will be clear
				bdir = (u8)ctrl->wIndex >> 7;			// the direction of this ep
				if (u8ep_n > FUSB220_MAX_EP)			// over the Max. ep count ?
					return FALSE;
				else
				{
					u8fifo_n = mUsbEPMapRd(u8ep_n);		// get the relatived FIFO number
					if (bdir == 1)
						u8fifo_n &= 0x0F;
					else
						u8fifo_n >>= 4;
					if (u8fifo_n >= FUSB220_MAX_FIFO)	// over the Max. fifo count ?
						return FALSE;

				    // Check the FIFO had been enable ?
					if ((mUsbFIFOConfigRd(u8fifo_n) & 0x80) == 0)
						return FALSE;
					if (bdir == 1)						// IN direction ?
						RecipientStatusLow = mUsbEPinStallST(u8ep_n);
					else
						RecipientStatusLow = mUsbEPoutStallST(u8ep_n);
					DBG_TEMP("+bGet_status()  ==> EP=0x%x, Status=%d\n",
							(u8)ctrl->wIndex,RecipientStatusLow);					
				}
			}
        	break;
		default :
			return FALSE;
	}

	// return RecipientStatus;
	u8Tmp[0] = RecipientStatusLow;
	u8Tmp[1] = RecipientStatusHigh;
	vUsbCxFWr( u8Tmp, 2);
	mUsbEP0DoneSet();	
	
	return TRUE; 
}

///////////////////////////////////////////////////////////////////////////////
//		bSet_feature(struct FTC_udc *dev,const struct usb_ctrlrequest *ctrl)
//		Description: (add by Andrew)
//			1. Process Cx Set feature command.
//		input: none
//		output: TRUE or FALSE (u8)
///////////////////////////////////////////////////////////////////////////////
static u8 bSet_feature(struct FTC_udc *dev,const struct usb_ctrlrequest *ctrl)
{
	u8 i,u8ep_n,u8fifo_n;
	u8 u8Tmp[52];
	u8 * pp;
	u8 bdir;
	
	DBG_FUNCC("+bSet_feature()  ==> Add by Andrew\n");
	
	switch (ctrl->wValue)		// FeatureSelector
	{
		case 0:	// ENDPOINT_HALE
			// Set "Endpoint_Halt", Turn on the "STALL" bit in Endpoint Control Function Register
			if(ctrl->wIndex == 0x00)
				FTC_set_halt(dev->gadget.ep0, 2);  // Set EP0 to functional stall
			else
			{
				u8ep_n = (u8)ctrl->wIndex & 0x7F;		// which ep will be clear
				bdir = (u8)ctrl->wIndex >> 7;			// the direction of this ep
				if (u8ep_n > FUSB220_MAX_EP)			// over the Max. ep count ?
					return FALSE;
				else
				{
					u8fifo_n = mUsbEPMapRd(u8ep_n);		// get the relatived FIFO number
					if (bdir == 1)
						u8fifo_n &= 0x0F;
					else
						u8fifo_n >>= 4;
					if (u8fifo_n >= FUSB220_MAX_FIFO)	// over the Max. fifo count ?
						return FALSE;

	  			    // Check the FIFO had been enable ?
					if ((mUsbFIFOConfigRd(u8fifo_n) & 0x80) == 0)
						return FALSE;
					if (bdir == 1)						// IN direction ?
						mUsbEPinStallSet(u8ep_n);		// Clear Stall Bit
					else
						mUsbEPoutStallSet(u8ep_n);		// Set Stall Bit
					DBG_TEMP("+bSet_feature()  ==> EP=0x%x\n", (u8)ctrl->wIndex);					
				}
			}
			mUsbEP0DoneSet();	
			break;
 		case 1 :   		// Device Remote Wakeup
 			// Set "Device_Remote_Wakeup", Turn on the"RMWKUP" bit in Mode Register
			mUsbRmWkupSet();
			mUsbEP0DoneSet();	
	            break;

		case 2 :   		// Test Mode
			switch (ctrl->wIndex >> 8)	// TestSelector
			{
				case 0x1:	// Test_J
					mUsbTsMdWr(TEST_J);
					mUsbEP0DoneSet();	
					break;
				
				case 0x2:	// Test_K
					mUsbTsMdWr(TEST_K);
					mUsbEP0DoneSet();	
					break;
						
				case 0x3:	// TEST_SE0_NAK
					mUsbTsMdWr(TEST_SE0_NAK);
					mUsbEP0DoneSet();	
					break;
						
				case 0x4:	// Test_Packet
					mUsbTsMdWr(TEST_PKY);
					mUsbEP0DoneSet();			// special case: follow the test sequence
					//////////////////////////////////////////////
					// Jay ask to modify, 91-6-5 (Begin)		//
					//////////////////////////////////////////////
					pp = u8Tmp;
					for (i=0; i<9; i++) 		// JKJKJKJK x 9
					{
						(*pp) = (0x00);
						pp ++;
					}

					for (i=0; i<8; i++) 		// JJKKJJKK*8, 8*AA
					{
						(*pp) = (0xAA);
						pp ++;
					}

					for (i=0; i<8; i++) 		// JJJJKKKK*8, 8*EE
					{
						(*pp) = (0xEE);
						pp ++;
					}
					(*pp) = (0xFE); 			// JJJJJJJKKKKKKK *8
					pp ++;

					for (i=0; i<11; i++)		// 11*FF
					{
						(*pp) = (0xFF);
						pp ++;
					}

					(*pp) = (0x7F); 			// JJJJJJJK *8
					pp ++;
					(*pp) = (0xBF);
					pp ++;
					(*pp) = (0xDF);
					pp ++;
					(*pp) = (0xEF);
					pp ++;
					(*pp) = (0xF7);
					pp ++;
					(*pp) = (0xFB);
					pp ++;
					(*pp) = (0xFD);
					pp ++;
					(*pp) = (0xFC);
					pp ++;
					(*pp) = (0x7E); 			// {JKKKKKKK * 10}, JK
					pp ++;
					(*pp) = (0xBF);
					pp ++;
					(*pp) = (0xDF);
					pp ++;
					(*pp) = (0xEF);
					pp ++;
					(*pp) = (0xF7);
					pp ++;
					(*pp) = (0xFB);
					pp ++;
					(*pp) = (0xFD);
					pp ++;
					(*pp) = (0x7E);
					vUsbCxFWr( u8Tmp, 53);

					//////////////////////////////////////////////
					// Jay ask to modify, 91-6-5 (End)			//
					//////////////////////////////////////////////

					// Turn on "r_test_packet_done" bit(flag) (Bit 5)
					mUsbTsPkDoneSet();
					break;
				
				case 0x5:	// Test_Force_Enable
					//FUSBPort[0x08] = 0x20;	//Start Test_Force_Enable
					break;
			
				default:
					return FALSE;		
			}
	         	break;
		default :
			return FALSE;
	}
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//		bSet_Descriptor(struct FTC_udc *dev,const struct usb_ctrlrequest *ctrl)
//		Description: (add by Andrew)
//			1. Send 2 bytes status to host.
//		input: none
//		output: TRUE or FALSE (u8)
///////////////////////////////////////////////////////////////////////////////
static u8 bSet_Descriptor(struct FTC_udc *dev,const struct usb_ctrlrequest *ctrl)
{
	DBG_FUNCC("+bSet_Descriptor()  ==> Not Implement\n");
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
//		bSynch_frame(struct FTC_udc *dev,const struct usb_ctrlrequest *ctrl)
//		Description: (add by Andrew)
//			1. If the EP is a Iso EP, then return the 2 bytes Frame number.
//				 else stall this command
//		input: none
//		output: TRUE or FALSE
///////////////////////////////////////////////////////////////////////////////
static u8 bSynch_frame(struct FTC_udc *dev,const struct usb_ctrlrequest *ctrl)
{
	u8 TransferType;
	u8 u8Tmp[2];
	DBG_FUNCC("+bSynch_frame()  ==> add by Andrew\n");

	if((ctrl->wIndex==0)||(ctrl->wIndex>4))
		return FALSE;
	
	// Does the Endpoint support Isochronous transfer type? 
	TransferType = (dev->ep[ctrl->wIndex].desc->bmAttributes)&0x03;
	if (TransferType == 1)	// Isochronous
	{
		u8Tmp[0] = mUsbFrameNoLow();
		u8Tmp[1] = mUsbFrameNoHigh();

		vUsbCxFWr( u8Tmp, 2);
		mUsbEP0DoneSet();
		return TRUE;
	}
	else
		return FALSE;	
}

///////////////////////////////////////////////////////////////////////////////
//		bSet_address(struct FTC_udc *dev,const struct usb_ctrlrequest *ctrl)
//		Description: (add by Bruce)
//			1. Set addr to FUSB200 register.
//		input: none
//		output: TRUE or FALSE (u8)
///////////////////////////////////////////////////////////////////////////////
static u8 bSet_address(struct FTC_udc *dev,const struct usb_ctrlrequest *ctrl)
{
	DBG_FUNCC("+bSet_address() = %d\n", ctrl->wValue);

	if (ctrl->wValue >= 0x0100)
		return FALSE;
	else
	{
		mUsbDevAddrSet(ctrl->wValue);
		mUsbEP0DoneSet();
		return TRUE;
	}
}

///////////////////////////////////////////////////////////////////////////////
//		bClear_feature(struct FTC_udc *dev)
//		Description: (add by Bruce)
//			1. Send 2 bytes status to host.
//		input: none
//		output: TRUE or FALSE (u8)
///////////////////////////////////////////////////////////////////////////////
static u8 bClear_feature(struct FTC_udc *dev,const struct usb_ctrlrequest *ctrl)
{
	u8 u8ep_n;
	u8 u8fifo_n;
	u8 bdir;

	DBG_FUNCC("+bClear_feature()\n");

	switch (ctrl->wValue)		// FeatureSelector
	{
		case 0:		// ENDPOINT_HALE
			// Clear "Endpoint_Halt", Turn off the "STALL" bit in Endpoint Control Function Register
			if(ctrl->wIndex == 0x00)
				FTC_set_halt(dev->gadget.ep0, 0);  
			else {
				u8ep_n = ctrl->wIndex & 0x7F;		// which ep will be clear
				bdir = ctrl->wIndex >> 7;			// the direction of this ep
				if (u8ep_n > FUSB220_MAX_EP)			// over the Max. ep count ?
					return FALSE;
				else {
					u8fifo_n = mUsbEPMapRd(u8ep_n);		// get the relatived FIFO number
					if (bdir == 1)
						u8fifo_n &= 0x0F;
					else
						u8fifo_n >>= 4;
					if (u8fifo_n >= FUSB220_MAX_FIFO)	// over the Max. fifo count ?
						return FALSE;

					// Check the FIFO had been enable ?
					if ((mUsbFIFOConfigRd(u8fifo_n) & 0x80) == 0)
						return FALSE;
					// Clear ep stall
					FTC_clear_halt(&dev->ep[u8ep_n]);//Bruce
				}
				mUsbEP0DoneSet();	
				//   FTC_clear_halt(&dev->ep[u8ep_n]);//Bruce
				//	pio_advance(&dev->ep[u8ep_n]);
				return TRUE; 
			}
			break;
		case 1 :   		// Device Remote Wakeup
			// Clear "Device_Remote_Wakeup", Turn off the"RMWKUP" bit in Main Control Register
			mUsbRmWkupClr();
			break;
		case 2 :   		// Test Mode
			return FALSE;
		default :
			return FALSE;
	}
	mUsbEP0DoneSet();	
	return TRUE; 
}

#define Cmd_Service_Fail	 		0
#define Cmd_Already_Service 		1
#define Cmd_Let_Gadget_To_Service 	2
///////////////////////////////////////////////////////////////////////////////
//		u8StandardCommand(struct FTC_udc *dev, struct usb_ctrlrequest	*ctrl)
//		Description:
//		input: none
//		output: TRUE or FALSE
///////////////////////////////////////////////////////////////////////////////
static u8 u8StandardCommand(struct FTC_udc *dev, struct usb_ctrlrequest	*ctrl)
{
	DBG_FUNCC("+bStandardCommand()\n");

	switch (ctrl->bRequest) 
	{
		case USB_REQ_GET_STATUS:			// get statue, add by Andrew
			if (bGet_status(dev, (ctrl))==FALSE)
				return Cmd_Service_Fail;
			break;
			
		case USB_REQ_SET_FEATURE:			// set feature, add by Andrew
			if (bSet_feature(dev, (ctrl))==FALSE)
				return Cmd_Service_Fail;
			break;
			
		case USB_REQ_SET_DESCRIPTOR:		// set descriptor, add by Andrew
			if (dev->ep0state == EP0_STALL)
				return Cmd_Service_Fail;
			if (bSet_Descriptor(dev, (ctrl))==FALSE)
				return Cmd_Service_Fail;
			break;
			
		case USB_REQ_SYNCH_FRAME:			// Synch frame, add by Andrew
			if (dev->ep0state == EP0_STALL)
				return Cmd_Service_Fail;
			if (bSynch_frame(dev, (ctrl))==FALSE)
				return Cmd_Service_Fail;
			break;
			
		case USB_REQ_CLEAR_FEATURE:		// clear feature, add by Bruce
			if (bClear_feature(dev, (ctrl))==FALSE)
				return Cmd_Service_Fail;
			break;

   		case USB_REQ_SET_ADDRESS:		// set address, add by Bruce
			if (dev->ep0state == EP0_STALL)
				return Cmd_Service_Fail;
			if (bSet_address(dev,(ctrl))==FALSE)		
				return Cmd_Service_Fail;
   			break;

		// for set configuration and set interface, some setting need to be set in lower driver	
		// and other information must set in upper driver
		case USB_REQ_SET_CONFIGURATION:		
			if (ctrl->wValue==0)
			   mUsbCfgClr();
			else {
			   if (dev->gadget.speed == USB_SPEED_HIGH)
			      vUsbFIFO_EPxCfg_HS();
			   else if(dev->gadget.speed == USB_SPEED_FULL)
				  vUsbFIFO_EPxCfg_FS();

			   mUsbCfgSet();
			}	
			// reset dma status
			dev->EPUseDMA = FUSB220_DMA_IS_IDLE_NOW;	//reset
			
			return Cmd_Let_Gadget_To_Service;
			break;
			
		default:		/* left some commands and pass to gadget driver to service*/
			return Cmd_Let_Gadget_To_Service;
   			break;			
	}
	return Cmd_Already_Service;
}

///////////////////////////////////////////////////////////////////////////////
//		vUsb_ep0setup(struct FTC_udc *dev)
//		Description:
//			1. Read the speed
//			2. Read 8-byte setup packet.
//         		3. Process the standard command:
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_ep0setup(struct FTC_udc *dev)
{
	u8  u8UsbCmd[8],u8Tmp;
	struct usb_ctrlrequest	ctrl;
	int	tmp;

	DBG_FUNCC("+vUsb_ep0setup()\n");

	//<1>.Read the speed
	if (dev->gadget.speed == USB_SPEED_UNKNOWN) {
	   // first ep0 command after usb reset, means we can check usb speed right now.
	   if (mUsbHighSpeedST()) {					// First we should judge HS or FS
	 	  INFO(dev,"L%x, high speed mode\n", dev->u8LineCount ++);
		  dev->gadget.speed = USB_SPEED_HIGH;
//		  vUsbFIFO_EPxCfg_HS();//Set the FIFO information
	   }
	   else {
		  INFO(dev,"L%x, full speed mode\n", dev->u8LineCount ++);
		  dev->gadget.speed = USB_SPEED_FULL;
//		  vUsbFIFO_EPxCfg_FS();//Set the FIFO information
	   }
	   dev->ep0state = EP0_IDLE;
	}

	//<2>.Dequeue ALL requests
	nuke(&dev->ep[0], 0);
	dev->ep[0].stopped = 0;

	//<3>.Read 8-byte setup packet from FIFO
	vUsbCxFRd8ByteCmd(u8UsbCmd);
   	DBG_CTRLL("L%x, EP0Cmd:%02x %02x %02x %02x %02x %02x %02x %02x\n", dev->u8LineCount ++,
		      u8UsbCmd[0],u8UsbCmd[1],u8UsbCmd[2],u8UsbCmd[3],u8UsbCmd[4],u8UsbCmd[5],u8UsbCmd[6],u8UsbCmd[7]);

	/* read SETUP packet and enter DATA stage */
	ctrl.bRequestType = u8UsbCmd[0];  
	ctrl.bRequest = u8UsbCmd[1];
	ctrl.wValue  =(u8UsbCmd[3]  << 8) | u8UsbCmd[2];
	ctrl.wIndex  = (u8UsbCmd[5]  << 8) | u8UsbCmd[4]; 
	ctrl.wLength = (u8UsbCmd[7]  << 8) | u8UsbCmd[6];

	if (likely(ctrl.bRequestType & USB_DIR_IN)) {
	   dev->ep[0].is_in = 1;
	   dev->ep0state = EP0_IN;
	} 
	else {
	   dev->ep[0].is_in = 0;
	   dev->ep0state = EP0_OUT;
	}

	// Check if 
	if (dev->ep0state == EP0_STALL) {
	   if( ((ctrl.bRequestType&0x60) != 0)||
	      !((ctrl.bRequest==USB_REQ_GET_STATUS)||
		   (ctrl.bRequest==USB_REQ_SET_FEATURE)||
		   (ctrl.bRequest==USB_REQ_CLEAR_FEATURE))) {
          goto stall;	
	   }
	}
	
	//Parsing some Standard Commands 
	if ((ctrl.bRequestType&0x60) == 0) { // Standard Request codes for USB  lower driver
	   u8Tmp = u8StandardCommand(dev,&ctrl);
	   if (u8Tmp== Cmd_Service_Fail)
	      goto stall;	
	   else if(u8Tmp == Cmd_Let_Gadget_To_Service)
	 	  goto PassToGadget;	
	   else if(u8Tmp == Cmd_Already_Service)
        	  return;		// exit this function
	}
			
PassToGadget:/* pass to gadget driver */
	spin_unlock (&dev->lock);
	tmp = dev->driver->setup(&dev->gadget, &(ctrl));
	spin_lock (&dev->lock);	
	if (unlikely(tmp < 0)) 	
		goto stall;			
	return ;		//Normal Exit

//Stall the command
stall:
#ifdef USB_TRACE
	VDBG(dev, "req %02x.%02x protocol STALL; err %d\n",
		ctrl.bRequestType, ctrl.bRequest, tmp);
#endif
	INFO(dev,"Set STALL in vUsb_ep0setup\n");
	DBG_TEMP("+vUsb_ep0setup() fail, set stall\n");
	FTC_set_halt(dev->gadget.ep0, 1);  // Return EP0_Stall
}


///////////////////////////////////////////////////////////////////////////////
//		vUsb_ep0tx()
//		Description:
//			1. Transmit data to EP0 FIFO.
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_ep0tx(struct FTC_udc *dev)
{
	struct FTC_request	*req;
	DBG_FUNCC("+vUsb_ep0tx()\n");

	req = container_of(dev->EP0req, struct FTC_request, req);
	if (!req) 
	{
		UWARN(dev,"vUsb_ep0tx(): No req allocated for EP0, but rising ep0tx int, just skip int\n");
		return;
	}

	switch(dev->eUsbCxCommand)
	{
		case CMD_GET_DESCRIPTOR:
			write_fifo( &(dev->ep[0]), req );
			//vUsbEP0TxData();
			break;
		default:
			DBG_TEMP("+vUsb_ep0tx() fail, set stall\n");

			// ****** Caution *********
			// Normally, in here we should set ep0 stall(halt) without any condition,
			// that means "if(dev->gadget.speed == USB_SPEED_HIGH)" is unnecessary.
			// But in here, because FUSB220 in A320D still have some bugs. So i must add
			// this condition to let FUSB220 work well.
			if(dev->gadget.speed == USB_SPEED_HIGH)
				FTC_set_halt(dev->gadget.ep0, 1);  // Return EP0_Stall
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//		vUsb_ep0rx()
//		Description:
//			1. Receive data from EP0 FIFO.
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_ep0rx(struct FTC_udc *dev)
{ 
	DBG_FUNCC("+vUsb_ep0rx()\n");

	switch(dev->eUsbCxCommand)
	{
		case CMD_SET_DESCRIPTOR:
			FTC_queue(dev->gadget.ep0, dev->EP0req, GFP_ATOMIC);
			//vUsbEP0RxData(dev);
			break;
		default:
			DBG_TEMP("+vUsb_ep0rx() fail, set stall\n");
			FTC_set_halt(dev->gadget.ep0, 1);  // Return EP0_Stall
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//		vUsb_ep0end(struct FTC_udc *dev)
//		Description:
//			1. End this transfer.
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_ep0end(struct FTC_udc *dev)
{ 
	DBG_FUNCC("+vUsb_ep0end()\n");

	dev->eUsbCxCommand = CMD_VOID;
	mUsbEP0DoneSet();	
}

///////////////////////////////////////////////////////////////////////////////
//		vUsb_ep0fail(struct FTC_udc *dev)
//		Description:
//			1. Stall this transfer.
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsb_ep0fail(struct FTC_udc *dev)
{ 
	DBG_FUNCC("+vUsb_ep0fail()\n");
	DBG_TEMP("+vUsb_ep0fail() fail, set stall\n");
	FTC_set_halt(dev->gadget.ep0, 1);  // Return EP0_Stall
}

///////////////////////////////////////////////////////////////////////////////
//		vUsbHandler(struct FTC_udc	*dev)
//		Description:
//			1. Service all Usb events
//			2. ReEnable Usb interrupt.
//		input: none
//		output: none
///////////////////////////////////////////////////////////////////////////////
static void vUsbHandler(struct FTC_udc	*dev)
{
	u8 usb_interrupt_level2;
	u8 usb_interrupt_Mask;
	u8 usb_interrupt_Origan;
	 
	DBG_FUNCC("+vUsbHandler()\n");

	DBG_CTRLL("usb_interrupt_level1:0x%x\n",dev->usb_interrupt_level1);

	if (dev->usb_interrupt_level1 & BIT7)				//Group Byte 7
	{
		usb_interrupt_Origan = mUsbIntByte7Rd();
		usb_interrupt_Mask = mUsbIntByte7MaskRd();
		usb_interrupt_level2 = usb_interrupt_Origan & ~usb_interrupt_Mask;
		
		DBG_CTRLL("IntSCR7:0x%x\n",usb_interrupt_level2);
		
		if (usb_interrupt_level2 & BIT0) {
			DBG_CTRLL("HBF is Empty\n");
			dev->bUsbBufferEmpty = TRUE;
		}
		else {
			DBG_CTRLL("HBF is not Empty\n");
			dev->bUsbBufferEmpty = FALSE;
		}	
		
		if (usb_interrupt_level2 & BIT1)
			vUsb_rst(dev);


		if (usb_interrupt_level2 & BIT2) {
			vUsb_suspend(dev);  
			return;    // once enter suspend, exit immediately
		}
		if (usb_interrupt_level2 & BIT3)
			vUsb_resm(dev);      
		if (usb_interrupt_level2 & BIT4)
			vUsb_Iso_SeqErr(dev);
		if (usb_interrupt_level2 & BIT5)
			vUsb_Iso_SeqAbort(dev);
		if (usb_interrupt_level2 & BIT6)
			vUsb_TX0Byte(dev);
		if (usb_interrupt_level2 & BIT7)
			vUsb_RX0Byte(dev);	
	}

	if (dev->usb_interrupt_level1 & BIT0)				//Group Byte 0
	{
		usb_interrupt_Origan = mUsbIntByte0Rd();
		usb_interrupt_Mask = mUsbIntByte0MaskRd();
		usb_interrupt_level2 = usb_interrupt_Origan & ~usb_interrupt_Mask;

		DBG_CTRLL("IntSCR0:0x%x\n",usb_interrupt_level2);
		dev->ep[0].irqs++;
		//	Stop APB DMA if DMA is still running 
		//	record buffer counter, and clear buffer. Later  
		//	will re-input data use DMA.	
		if (usb_interrupt_level2 & BIT0) {
			DBG_CTRLL("USB ep0 Setup\n");			
			vUsb_ep0setup(dev);
		}	
		else if (usb_interrupt_level2 & BIT3) {
			DBG_CTRLL("USB ep0 end\n");
			vUsb_ep0end(dev);
		}	
		if (usb_interrupt_level2 & BIT1) {
			DBG_CTRLL("USB ep0 TX\n");
			vUsb_ep0tx(dev);
		}	
		if (usb_interrupt_level2 & BIT2) {
			DBG_CTRLL("USB ep0 RX\n");
			vUsb_ep0rx(dev);
		}	
		if (usb_interrupt_level2 & BIT4) {
			UWARN(dev,"USB ep0 fail\n");	
			vUsb_ep0fail(dev);
		}
		if (usb_interrupt_level2 & BIT5) {
			UWARN(dev,"RBUF error\n");	
			mUsbIntRdBufErrClr();
		}
	}

	if (dev->usb_interrupt_level1 & BIT1)				//Group Byte 1
	{
		usb_interrupt_Origan = mUsbIntByte1Rd();
		usb_interrupt_Mask = mUsbIntByte1MaskRd();
		usb_interrupt_level2 = usb_interrupt_Origan & ~usb_interrupt_Mask;

		DBG_CTRLL("IntSCR1:0x%x\n",usb_interrupt_level2);

		dev->ep[2].irqs++;
		// YPING : use FIFO2 for ep2( bulk out)
		if (usb_interrupt_level2 & BIT5)
		{			// short packet
			DBG_BULKK("Bulk Out <EP2> Short Packet\n");
			EPX_check_advance(dev,&dev->ep[2]);   // EP2 bulk out,receive from Host
		}		 
		else if (usb_interrupt_level2 & BIT4)
		{		// full packet
			DBG_BULKK("Bulk Out <EP2> Full Packet\n");
			EPX_check_advance(dev,&dev->ep[2]);   // EP2 bulk out
		}	
	}

	if (dev->usb_interrupt_level1 & BIT2)				//Group Byte 2
	{
		usb_interrupt_Origan = mUsbIntByte2Rd();
		usb_interrupt_Mask = mUsbIntByte2MaskRd();
		usb_interrupt_level2 = usb_interrupt_Origan & ~usb_interrupt_Mask;

		DBG_CTRLL("IntSCR2:0x%x\n",usb_interrupt_level2);
		dev->ep[4].irqs++;
		// YPING : use FIFO5 for ep4( interrupt out)
		if (usb_interrupt_level2 & BIT3)
		{			// short packet
			DBG_CTRLL("Interrupt OUT <EP4> Short Packet\n");
			EPX_check_advance(dev,&dev->ep[4]);   // EP4 interrupt out
		}	
		else if (usb_interrupt_level2 & BIT2)
		{		// full packet
			DBG_CTRLL("Interrupt OUT <EP4> Full Packet\n");
			EPX_check_advance(dev,&dev->ep[4]);   // EP4 interrupt out
		}	
	}
	
	if (dev->usb_interrupt_level1 & BIT5)				//Group Byte 5
	{
		usb_interrupt_Origan = mUsbIntByte5Rd();
		usb_interrupt_Mask = mUsbIntByte5MaskRd();
		usb_interrupt_level2 = usb_interrupt_Origan & ~usb_interrupt_Mask;

		DBG_CTRLL("IntSCR5:0x%x\n",usb_interrupt_level2);
		if (usb_interrupt_level2 & BIT0)
		{
			DBG_CTRLL("Bulk IN <EP1> \n");
			dev->ep[1].irqs++;
			EPX_check_advance(dev,&dev->ep[1]);   // EP1 bulk in
		}			
		// YPING : use FIFO4 for ep3( Interrupt In)
		if (usb_interrupt_level2 & BIT4)
		{
			DBG_CTRLL("Interrupt IN <EP3> \n");
			dev->ep[3].irqs++;
			EPX_check_advance(dev,&dev->ep[3]);   // EP1 interrupt in
		}		
	}

	if (dev->usb_interrupt_level1 & BIT6)				//Group Byte 6
	{
		usb_interrupt_Origan = mUsbIntByte6Rd();
		usb_interrupt_Mask = mUsbIntByte6MaskRd();
		usb_interrupt_level2 = usb_interrupt_Origan & ~usb_interrupt_Mask;

		DBG_CTRLL("IntSCR6:0x%x\n",usb_interrupt_level2);
	}
}


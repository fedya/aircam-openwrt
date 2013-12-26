#define mLowByte(u16)	((u8)(u16	 ))
#define mHighByte(u16)	((u8)(u16 >> 8))
#define DMA_CHANEL_FREE         0XFF
#define FUSB220_DMA_IS_IDLE_NOW	DMA_CHANEL_FREE
#define USB_GADGET_DEVICE_ID	0x01
//#define FUSB220_DMA_IS_IDLE_NOW	(FUSB220_MAX_EP+1)	// 1..10
//(FUSB220_MAX_EP+1)	// 1..10
//**************************************************
/*
#define FULL_ED1_FIFO_START  	FIFO0
#define FULL_ED2_FIFO_START  	(FULL_ED1_FIFO_START+(FULL_ED1_bBLKNO *FULL_ED1_bBLKSIZE))
#define FULL_ED3_FIFO_START  	(FULL_ED2_FIFO_START+(FULL_ED2_bBLKNO *FULL_ED2_bBLKSIZE))
#define FULL_ED4_FIFO_START  	(FULL_ED3_FIFO_START+(FULL_ED3_bBLKNO *FULL_ED3_bBLKSIZE))

#define FULL_EP1_Map			(FULL_ED1_FIFO_START |(FULL_ED1_FIFO_START << 4)|(0xF0 >> (4*(1-FULL_ED1_bDIRECTION))))
#define FULL_EP1_FIFO_Map    	((FULL_ED1_bDIRECTION << 4) | EP1)
#define FULL_EP1_FIFO_Config 	(0x80 | ((FULL_ED1_bBLKSIZE - 1) << 4) | ((FULL_ED1_bBLKNO - 1) << 2) | FULL_ED1_bTYPE)
#define FULL_EP2_Map        		(FULL_ED2_FIFO_START |(FULL_ED2_FIFO_START << 4)|(0xF0 >> (4*(1-FULL_ED2_bDIRECTION))))
#define FULL_EP2_FIFO_Map    	((FULL_ED2_bDIRECTION << 4) | EP2)
#define FULL_EP2_FIFO_Config 	(0x80 | ((FULL_ED2_bBLKSIZE - 1) << 4) | ((FULL_ED2_bBLKNO - 1) << 2) | FULL_ED2_bTYPE)
#define FULL_EP3_Map        		(FULL_ED3_FIFO_START |(FULL_ED3_FIFO_START << 4)|(0xF0 >> (4*(1-FULL_ED3_bDIRECTION))))
#define FULL_EP3_FIFO_Map    	((FULL_ED3_bDIRECTION << 4) | EP3)
#define FULL_EP3_FIFO_Config 	(0x80 | ((FULL_ED3_bBLKSIZE - 1) << 4) | ((FULL_ED3_bBLKNO - 1) << 2) | FULL_ED3_bTYPE)
#define FULL_EP4_Map        		(FULL_ED4_FIFO_START |(FULL_ED4_FIFO_START << 4)|(0xF0 >> (4*(1-FULL_ED4_bDIRECTION))))
#define FULL_EP4_FIFO_Map    	((FULL_ED4_bDIRECTION << 4) | EP4)
#define FULL_EP4_FIFO_Config 	(0x80 | ((FULL_ED4_bBLKSIZE - 1) << 4) | ((FULL_ED4_bBLKNO - 1) << 2) | FULL_ED4_bTYPE)


#define HIGH_ED1_FIFO_START  	FIFO0
#define HIGH_ED2_FIFO_START  	(HIGH_ED1_FIFO_START+(HIGH_ED1_bBLKNO *HIGH_ED1_bBLKSIZE))
#define HIGH_ED3_FIFO_START  	(HIGH_ED2_FIFO_START+(HIGH_ED2_bBLKNO *HIGH_ED2_bBLKSIZE))
#define HIGH_ED4_FIFO_START  	(HIGH_ED3_FIFO_START+(HIGH_ED3_bBLKNO *HIGH_ED3_bBLKSIZE))

#define HIGH_EP1_Map        		(HIGH_ED1_FIFO_START |(HIGH_ED1_FIFO_START << 4)|(0xF0 >> (4*(1-HIGH_ED1_bDIRECTION))))
#define HIGH_EP1_FIFO_Map    	((HIGH_ED1_bDIRECTION << 4) | EP1)
#define HIGH_EP1_FIFO_Config 	(0x80 | ((HIGH_ED1_bBLKSIZE - 1) << 4) | ((HIGH_ED1_bBLKNO - 1) << 2) | HIGH_ED1_bTYPE)
#define HIGH_EP2_Map        		(HIGH_ED2_FIFO_START |(HIGH_ED2_FIFO_START << 4)|(0xF0 >> (4*(1-HIGH_ED2_bDIRECTION))))
#define HIGH_EP2_FIFO_Map    	((HIGH_ED2_bDIRECTION << 4) | EP2)
#define HIGH_EP2_FIFO_Config 	(0x80 | ((HIGH_ED2_bBLKSIZE - 1) << 4) | ((HIGH_ED2_bBLKNO - 1) << 2) | HIGH_ED2_bTYPE)
#define HIGH_EP3_Map        		(HIGH_ED3_FIFO_START |(HIGH_ED3_FIFO_START << 4)|(0xF0 >> (4*(1-HIGH_ED3_bDIRECTION))))
#define HIGH_EP3_FIFO_Map    	((HIGH_ED3_bDIRECTION << 4) | EP3)
#define HIGH_EP3_FIFO_Config 	(0x80 | ((HIGH_ED3_bBLKSIZE - 1) << 4) | ((HIGH_ED3_bBLKNO - 1) << 2) | HIGH_ED3_bTYPE)
#define HIGH_EP4_Map        		(HIGH_ED4_FIFO_START |(HIGH_ED4_FIFO_START << 4)|(0xF0 >> (4*(1-HIGH_ED4_bDIRECTION))))
#define HIGH_EP4_FIFO_Map    	((HIGH_ED4_bDIRECTION << 4) | EP4)
#define HIGH_EP4_FIFO_Config 	(0x80 | ((HIGH_ED4_bBLKSIZE - 1) << 4) | ((HIGH_ED4_bBLKNO - 1) << 2) | HIGH_ED4_bTYPE)
*/

#ifndef __FUSB220_M_H
	#define __FUSB220_M_H
							    
	#define CPE_USBDEV_BASE  (USB_FUSB220_VA_BASE)
	#define IRQ_USBDEV 	USB_FUSB220_IRQ 

// for debug begin	
#define IO_ADDRESS(x)                   (((x>>4)&0xffff0000)+(x&0xffff)+IO_BASE) 
// for debug end

	// Macro
	#define FUSB220_BASE_ADDRESS    (CPE_USBDEV_BASE) //0x96700000//  0x90600000
	#define bFUSBPort(bOffset)				*((volatile u8 *) ( FUSB220_BASE_ADDRESS | (u32)(bOffset)))
	#define wFUSBPort(bOffset)				*((volatile u16 *) ( FUSB220_BASE_ADDRESS | (u32)(bOffset)))
	#define dwFUSBPort(bOffset)				*((volatile u32 *) ( FUSB220_BASE_ADDRESS | (u32)(bOffset)))
	//john CPE_USBDEV_BASE USB_FUSB220_PA_BASE
	#define DMAbFUSBPort(bOffset)   *((volatile u8 *) ( USB_FUSB220_PA_BASE  | (u32)(bOffset)))
	#define DMAwFUSBPort(bOffset)   *((volatile u16 *) ( USB_FUSB220_PA_BASE | (u32)(bOffset)))
	#define DMAdwFUSBPort(bOffset)  *((volatile u32 *) ( USB_FUSB220_PA_BASE | (u32)(bOffset)))
	// Macro
	#define mUsbGoSuspend()					(bFUSBPort(0x00) |=  (u8)BIT3)
	
	#define mUsbSoftRstSet()					(bFUSBPort(0x00) |=  (u8)BIT4)
	#define mUsbSoftRstClr()					(bFUSBPort(0x00) &= ~(u8)BIT4)

	#define mUsbHighSpeedST()				(bFUSBPort(0x00) & (u8)BIT6)
	#define mUsbRmWkupST()					(bFUSBPort(0x00) & (u8)BIT0)
	#define mUsbRmWkupClr()					(bFUSBPort(0x00) &= ~(u8)BIT0)
	#define mUsbRmWkupSet()				(bFUSBPort(0x00) |= (u8)BIT0)
	#define mUsbGlobIntEnSet()				(bFUSBPort(0x00) |= (u8)BIT2)
	#define mUsbChipEnSet()					(bFUSBPort(0x00) |= (u8)BIT5)

	#define mUsbHbfFlush()					(bFUSBPort(0x00) |= (u8)BIT1)
	#define mUsbHbfClr()						(bFUSBPort(0x00) |= (u8)BIT7)	

	#define mUsbDevAddrSet(Value)			(bFUSBPort(0x01) = Value)
	#define mUsbCfgST()						(bFUSBPort(0x01) & (u8)BIT7)
	#define mUsbCfgSet()						(bFUSBPort(0x01) |= (u8)BIT7)
	#define mUsbCfgClr()						(bFUSBPort(0x01) &= ~(u8)BIT7)
	#define mUsbClrAllFIFOSet()				(bFUSBPort(0x02) |= (u8)BIT0)
	#define mUsbClrAllFIFOClr()				(bFUSBPort(0x02) &= ~(u8)BIT0)
	
	#define mUsbTstHalfSpeedEn()				(bFUSBPort(0x02) |= (u8)BIT7)
	#define mUsbTstHalfSpeedDis()			(bFUSBPort(0x02) &= ~(u8)BIT7)

	#define mUsbFrameNoLow()				(bFUSBPort(0x04))
	#define mUsbFrameNoHigh()				(bFUSBPort(0x05))

	#define mUsbSOFMaskHS()				(bFUSBPort(0x06) = 0x4c); (bFUSBPort(0x07) = 0x4)
	#define mUsbSOFMaskFS()				(bFUSBPort(0x06) = 0x10); (bFUSBPort(0x07) = 0x27)
	
	#define mUsbTsMdWr(item)				(bFUSBPort(0x08) = item)
	#define mUsbUnPLGClr()					(bFUSBPort(0x08) &= ~(u8)BIT0)

	#define mUsbEP0DoneSet()				(bFUSBPort(0x0B) |= (u8)BIT0)
	#define mUsbTsPkDoneSet()				(bFUSBPort(0x0B) |= (u8)BIT1)
	#define mUsbEP0StallSet()					(bFUSBPort(0x0B) |= (u8)BIT2)
	#define mUsbEP0ClearFIFO()				(bFUSBPort(0x0B) |= (u8)BIT3)  //john
	#define mUsbEP0EMPFIFO()				(bFUSBPort(0x0B) &= (u8)BIT5)  //john

	/////// Read CxF data ////////
	#define mUsbEP0DataRdByte0()			(bFUSBPort(0x0C))
	#define mUsbEP0DataRdByte2()			(bFUSBPort(0x0E))
	#define mUsbEP0DataRdWord()			(wFUSBPort(0x0C))	
	#define mUsbEP0DataRdDWord()			(dwFUSBPort(0x0C))

	/////// Write CxF data ////////
	#define mUsbEP0DataWrByte0(data)		(bFUSBPort(0x0C) = data)
	#define mUsbEP0DataWrByte2(data)		(bFUSBPort(0x0E) = data)	
	#define mUsbEP0DataWrWord(data)		(wFUSBPort(0x0C) = data)
	#define mUsbEP0DataWrDWord(data)		(dwFUSBPort(0x0C) = (u32)data)
	
	#define mUsbIntGrp1Dis()			(bFUSBPort(0x10) |= (u8)BIT1)
	#define mUsbIntGrp2Dis()			(bFUSBPort(0x10) |= (u8)BIT2)
	#define mUsbIntGrp3Dis()			(bFUSBPort(0x10) |= (u8)BIT3)
	#define mUsbIntGrp4Dis()			(bFUSBPort(0x10) |= (u8)BIT4)
	#define mUsbIntGrp5Dis()			(bFUSBPort(0x10) |= (u8)BIT5)
	#define mUsbIntGrp6Dis()			(bFUSBPort(0x10) |= (u8)BIT6)
	#define mUsbIntGrp7Dis()			(bFUSBPort(0x10) |= (u8)BIT7)
	
	#define mUsbIntEP0SetupDis()		(bFUSBPort(0x11) |= (u8)BIT0)
	#define mUsbIntEP0InDis()		(bFUSBPort(0x11) |= (u8)BIT1)
	#define mUsbIntEP0OutDis()		(bFUSBPort(0x11) |= (u8)BIT2)
	#define mUsbIntEP0EndDis()		(bFUSBPort(0x11) |= (u8)BIT3)
	#define mUsbIntEP0FailDis()		(bFUSBPort(0x11) |= (u8)BIT4)
	#define mUsbIntRdBufErrDis()		(bFUSBPort(0x11) |= (u8)BIT5)

	#define mUsbIntEP0SetupEn()		(bFUSBPort(0x11) &= ~((u8)BIT0))
	#define mUsbIntEP0InEn()			(bFUSBPort(0x11) &= ~((u8)BIT1))
	#define mUsbIntEP0OutEn()		(bFUSBPort(0x11) &= ~((u8)BIT2))
	#define mUsbIntEP0EndEn()		(bFUSBPort(0x11) &= ~((u8)BIT3))
	#define mUsbIntEP0FailEn()		(bFUSBPort(0x11) &= ~((u8)BIT4))
	#define mUsbIntRdBufErrEn()		(bFUSBPort(0x11) &= ~((u8)BIT5))
	
	#define mUsbIntFIFO0_3OUTDis()	(bFUSBPort(0x12) = 0xFF)
	#define mUsbIntFIFO4_7OUTDis()	(bFUSBPort(0x13) = 0xFF)
	#define mUsbIntFIFO8_9OUTDis()	(bFUSBPort(0x14) = 0xFF)
	#define mUsbIntFIFO0_7INDis()	(bFUSBPort(0x16) = 0xFF)
	#define mUsbIntFIFO8_9INDis()	(bFUSBPort(0x17) = 0xFF)

	#define mUsbIntF0OUTEn()		(bFUSBPort(0x12) &= ~((u8)BIT1 | (u8)BIT0))	
	#define mUsbIntF2OUTEn()		(bFUSBPort(0x12) &= ~((u8)BIT5 | (u8)BIT4))
	#define mUsbIntF5OUTEn()		(bFUSBPort(0x13) &= ~((u8)BIT3 | (u8)BIT2))	
	#define mUsbIntF0INEn()			(bFUSBPort(0x16) &= ~((u8)BIT0))
	#define mUsbIntF1INEn()			(bFUSBPort(0x16) &= ~((u8)BIT1))
	#define mUsbIntF4INEn()			(bFUSBPort(0x16) &= ~((u8)BIT4))
	#define mUsbIntF8INEn()			(bFUSBPort(0x17) &= ~((u8)BIT0))

	#define mUsbIntF0OUTDis()		(bFUSBPort(0x12) |= ((u8)BIT1 | (u8)BIT0))	
	#define mUsbIntF2OUTDis()		(bFUSBPort(0x12) |= ((u8)BIT5 | (u8)BIT4))
	#define mUsbIntF5OUTDis()		(bFUSBPort(0x13) |= ((u8)BIT3 | (u8)BIT2))		
	#define mUsbIntF0INDis()			(bFUSBPort(0x16) |= (u8)BIT0)
	#define mUsbIntF4INDis()			(bFUSBPort(0x16) |= (u8)BIT4)
	#define mUsbIntF8INDis()			(bFUSBPort(0x17) |= (u8)BIT0)

	#define mUsbIntFIFOEn(off,val)	(bFUSBPort(off) &= ~(val))	  //john
	#define mUsbIntFIFODis(off,val)	(bFUSBPort(off) |=  (val))	  //john

	#define mUsbIntBufEmptyDis()		(bFUSBPort(0x18) |= 0x01)//|= BIT0)

	#define mUsbIntRX0Byte0Rd()		(bFUSBPort(0x19))
	#define mUsbIntRX0Byte1Rd()		(bFUSBPort(0x1A))
	#define mUsbIntRX0Byte0Clr()		(bFUSBPort(0x19) = 0)
	#define mUsbIntRX0Byte1Clr()		(bFUSBPort(0x1A) = 0)

	#define mUsbIntGroupMaskRd()	(bFUSBPort(0x10))
	#define mUsbIntByte0MaskRd()	(bFUSBPort(0x11))
	#define mUsbIntByte1MaskRd()	(bFUSBPort(0x12))
	#define mUsbIntByte2MaskRd()	(bFUSBPort(0x13))
	#define mUsbIntByte3MaskRd()	(bFUSBPort(0x14))
	#define mUsbIntByte4MaskRd()	(bFUSBPort(0x15))
	#define mUsbIntByte5MaskRd()	(bFUSBPort(0x16))
	#define mUsbIntByte6MaskRd()	(bFUSBPort(0x17))
	#define mUsbIntByte7MaskRd()	(bFUSBPort(0x18))

	#define mUsbIntRdBufErrClr()		(bFUSBPort(0x21) &= ~((u8)BIT5))
	#define mUsbIntGroupRegRd()		(bFUSBPort(0x20))
	#define mUsbIntByte0Rd()			(bFUSBPort(0x21))
	#define mUsbIntByte1Rd()			(bFUSBPort(0x22))
	#define mUsbIntByte2Rd()			(bFUSBPort(0x23))
	#define mUsbIntByte3Rd()			(bFUSBPort(0x24))
	#define mUsbIntByte4Rd()			(bFUSBPort(0x25))
	#define mUsbIntByte5Rd()			(bFUSBPort(0x26))
	#define mUsbIntByte6Rd()			(bFUSBPort(0x27))
	#define mUsbIntByte7Rd()			(bFUSBPort(0x28))

	#define mUsbIntBusRstClr()		(bFUSBPort(0x28) = 0x00)		// so weird?
	#define mUsbIntSuspClr()			(bFUSBPort(0x28) = 0x00)		// so weird?
	#define mUsbIntResmClr()			(bFUSBPort(0x28) = 0x00)		// so weird?
	#define mUsbIntIsoSeqErrClr()		(bFUSBPort(0x28) &= ~BIT4)			//
	#define mUsbIntIsoSeqAbortClr()	(bFUSBPort(0x28) &= ~BIT5)			//
	#define mUsbIntTX0ByteClr()		(bFUSBPort(0x28) &= ~BIT6)			//
	#define mUsbIntRX0ByteClr()		(bFUSBPort(0x28) &= ~BIT7)			//
	#define mUsbIntBufEmptyRd()		(bFUSBPort(0x28) & BIT0)			//

	#define mUsbIntIsoSeqErr0Rd()	(bFUSBPort(0x29))
	#define mUsbIntIsoSeqErr1Rd()	(bFUSBPort(0x2A))
	#define mUsbIntIsoSeqErr0Clr()	(bFUSBPort(0x29) = 0)
	#define mUsbIntIsoSeqErr1Clr()	(bFUSBPort(0x2A) = 0)

	#define mUsbIntIsoSeqAbort0Rd()	(bFUSBPort(0x2B))
	#define mUsbIntIsoSeqAbort1Rd()	(bFUSBPort(0x2C))
	#define mUsbIntIsoSeqAbort0Clr()	(bFUSBPort(0x2B) = 0)
	#define mUsbIntIsoSeqAbort1Clr()	(bFUSBPort(0x2C) = 0)

	#define mUsbIntTX0Byte0Rd()		(bFUSBPort(0x2D))
	#define mUsbIntTX0Byte1Rd()		(bFUSBPort(0x2E))
	#define mUsbIntTX0Byte0Clr()		(bFUSBPort(0x2D) = 0)
	#define mUsbIntTX0Byte1Clr()		(bFUSBPort(0x2E) = 0)

	#define mUsbIdleCnt(time)		(bFUSBPort(0x2F) = time)
	#define mUsbHbfCountRd()		(bFUSBPort(0x3F) & (0x1F))

	// Endpoint & FIFO Configuration
	#define mUsbEPinHighBandSet(EPn, dir , size )	(bFUSBPort(0x41 + ((EPn - 1) << 1)) &= ~(BIT6 |BIT5));  (bFUSBPort(0x41 + ((EPn - 1) << 1)) |= ((((u8)(size >> 11)+1) << 5)*(dir)) )
	
	#define mUsbEPMxPtSzLow(EPn, dir, size)		(bFUSBPort(0x40 + ((1-dir) * 0x20) + ((EPn - 1) << 1)) = (u8)(size))
	#define mUsbEPMxPtSzHigh(EPn, dir, size)		(bFUSBPort(0x41 + ((1-dir) * 0x20) + ((EPn - 1) << 1)) = (u8)(size >> 8))
	#define mUsbEPinMxPtSz(EPn)		((((bFUSBPort(0x41 + ((EPn - 1) << 1))) & 0x07) << 8) | (bFUSBPort(0x40 + ((EPn - 1) << 1))))
	#define mUsbEPinStallST(EPn)		((bFUSBPort(0x41 + ((EPn - 1) << 1)) & BIT3) >> 3)
	#define mUsbEPinStallClr(EPn)		(bFUSBPort(0x41 + ((EPn - 1) << 1)) &= ~BIT3)
	#define mUsbEPinStallSet(EPn)		(bFUSBPort(0x41 + ((EPn - 1) << 1)) |=  BIT3)
	#define mUsbEPinRsTgClr(EPn)		(bFUSBPort(0x41 + ((EPn - 1) << 1)) &= ~BIT4)
	#define mUsbEPinRsTgSet(EPn)	(bFUSBPort(0x41 + ((EPn - 1) << 1)) |=  BIT4)

	#define mUsbEPoutMxPtSz(EPn)	((((bFUSBPort(0x61 + ((EPn - 1) << 1))) & 0x07) << 8) | (bFUSBPort(0x60 + ((EPn - 1) << 1))))
	#define mUsbEPoutStallST(EPn)	((bFUSBPort(0x61 + ((EPn - 1) << 1)) & BIT3) >> 3)
	#define mUsbEPoutStallClr(EPn)	(bFUSBPort(0x61 + ((EPn - 1) << 1)) &= ~BIT3)
	#define mUsbEPoutStallSet(EPn)	(bFUSBPort(0x61 + ((EPn - 1) << 1)) |=  BIT3)
	#define mUsbEPoutRsTgClr(EPn)	(bFUSBPort(0x61 + ((EPn - 1) << 1)) &= ~BIT4)
	#define mUsbEPoutRsTgSet(EPn)	(bFUSBPort(0x61 + ((EPn - 1) << 1)) |=  BIT4)

	#define mUsbFIFO0DMAEn()	(bFUSBPort(0x7e ) |=  BIT0)
	#define mUsbFIFO1DMAEn()	(bFUSBPort(0x7e ) |=  BIT1)
	#define mUsbFIFO2DMAEn()	(bFUSBPort(0x7e ) |=  BIT2)
	#define mUsbFIFO3DMAEn()	(bFUSBPort(0x7e ) |=  BIT3)
	#define mUsbFIFO4DMAEn()	(bFUSBPort(0x7e ) |=  BIT4)
	#define mUsbFIFO5DMAEn()	(bFUSBPort(0x7e ) |=  BIT5)
	#define mUsbFIFO6DMAEn()	(bFUSBPort(0x7e ) |=  BIT6)
	#define mUsbFIFO7DMAEn()	(bFUSBPort(0x7e ) |=  BIT7)
	#define mUsbFIFO8DMAEn()	(bFUSBPort(0x7f ) |=  BIT0)
	#define mUsbFIFO9DMAEn()	(bFUSBPort(0x7f ) |=  BIT1)
	#define mUsbFIFO10DMAEn()	(bFUSBPort(0x7f ) |=  BIT2)
	#define mUsbFIFO11DMAEn()	(bFUSBPort(0x7f ) |=  BIT3)
	#define mUsbFIFO12DMAEn()	(bFUSBPort(0x7f ) |=  BIT4)
	#define mUsbFIFO13DMAEn()	(bFUSBPort(0x7f ) |=  BIT5)
	#define mUsbFIFO14DMAEn()	(bFUSBPort(0x7f ) |=  BIT6)
	#define mUsbFIFO15DMAEn()	(bFUSBPort(0x7f ) |=  BIT7)

	#define mUsbFIFO0DMADis()	(bFUSBPort(0x7e ) &=  ~BIT0)
	#define mUsbFIFO1DMADis()	(bFUSBPort(0x7e ) &=  ~BIT1)
	#define mUsbFIFO2DMADis()	(bFUSBPort(0x7e ) &=  ~BIT2)
	#define mUsbFIFO3DMADis()	(bFUSBPort(0x7e ) &=  ~BIT3)
	#define mUsbFIFO4DMADis()	(bFUSBPort(0x7e ) &=  ~BIT4)
	#define mUsbFIFO5DMADis()	(bFUSBPort(0x7e ) &=  ~BIT5)
	#define mUsbFIFO6DMADis()	(bFUSBPort(0x7e ) &=  ~BIT6)
	#define mUsbFIFO7DMADis()	(bFUSBPort(0x7e ) &=  ~BIT7)

	#define mUsbFIFODMAEn(off,val)		(bFUSBPort(off) |= (val))	  //john
	#define mUsbFIFODMADis(off,val)		(bFUSBPort(off) &= ~(val))	  //john
	
	#define mUsbEPMap(EPn, MAP)		(bFUSBPort(0x30 + (EPn-1)) = MAP)
	#define mUsbEPMapRd(EPn)			(bFUSBPort(0x30 + (EPn-1)))

	#define mUsbFIFOMap(FIFOn, MAP)	(bFUSBPort(0x80 + FIFOn) = MAP)
	#define mUsbFIFOMapRd(FIFOn)		(bFUSBPort(0x80 + FIFOn))

	#define mUsbFIFOConfig(FIFOn, CONFIG)	(bFUSBPort(0x90 + FIFOn) = CONFIG)
	#define mUsbFIFOConfigRd(FIFOn)			(bFUSBPort(0x90 + FIFOn))

	#define mUsbFIFOOutByteCount(fifo_num)	(u16)(((((u16)bFUSBPort(0xA0+fifo_num))&((u16)0x07))<<8) | ((u16)bFUSBPort(0xB0+fifo_num)))
	#define mUsbFIFODone(fifo_num)			(bFUSBPort(0xA0+fifo_num) |= ((u8)BIT3))
	#define mUsbFIFOReset(fifo_num)			(bFUSBPort(0xA0+fifo_num) |=  BIT4) //john
	#define mUsbFIFOResetOK(fifo_num)		(bFUSBPort(0xA0+fifo_num) &= ~BIT4) //john
	
	/////// Read Data FIFO ////////
	#define mUsbRdByte0(FIFOn)				(bFUSBPort(0xC0 + (FIFOn<<2)))
	#define mUsbRdByte2(FIFOn)				(bFUSBPort(0xC2 + (FIFOn<<2)))
	#define mUsbRdWord(FIFOn)				(wFUSBPort(0xC0 + (FIFOn<<2)))
	#define mUsbRdDWord(FIFOn)				(dwFUSBPort(0xC0 + (FIFOn<<2)))

	//john
	#define mDMAUsbRdDWord(FIFOn)			(DMAdwFUSBPort(0xC0 + (FIFOn<<2)))

	/////// Write Data FIFO ////////
	#define mUsbWrByte0(FIFOn, value)		(bFUSBPort(0xC0 + (FIFOn<<2)) = value)
	#define mUsbWrByte2(FIFOn, value)		(bFUSBPort(0xC2 + (FIFOn<<2)) = value)
	#define mUsbWrWord(FIFOn, value)		(wFUSBPort(0xC0 + (FIFOn<<2)) = value)
	#define mUsbWrDWord(FIFOn, value)		(dwFUSBPort(0xC0 + (FIFOn<<2)) = value)
	
#endif /* __FUSB220_M_H  */

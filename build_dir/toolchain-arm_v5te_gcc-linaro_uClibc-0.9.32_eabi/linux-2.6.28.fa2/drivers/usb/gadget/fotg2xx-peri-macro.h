
#ifndef __FOTG200_M_H
#define __FOTG200_M_H

#define FIFOCX	         0xFF
#define DMA_CHANEL_FREE  0XFF

#define MAX_EP_NUM	8
#define MAX_FIFO_NUM	4

#define CPE_FOTG200_BASE (USB_FOTG2XX_0_VA_BASE+0x100)
#define IRQ_USBDEV 	USB_FOTG2XX_IRQ
#define FUSB220_DMA_IS_IDLE_NOW DMA_CHANEL_FREE

#define FOTG200_BASE_ADDRESS		USB_FOTG2XX_0_VA_BASE
#define FUSB220_BASE_ADDRESS		CPE_FOTG200_BASE
#define FOTG200_FIFO_BASE(bOffset)	((CPE_FOTG200_BASE)+0xC0+(bOffset<<2))

#define bFOTGPeri_Port(bOffset)		*((volatile u8 *) ( FOTG200_BASE_ADDRESS | (u32)(bOffset)))
#define hwFOTGPeri_Port(bOffset)	*((volatile u16 *) ( FOTG200_BASE_ADDRESS | (u32)(bOffset)))
#define wFOTGPeri_Port(bOffset)		*((volatile u32 *) ( FOTG200_BASE_ADDRESS | (u32)(bOffset)))

// Main control register(0x100)
#define mUsbRmWkupST()			(wFOTGPeri_Port(0x100) & BIT0)
#define mUsbRmWkupSet()		        (wFOTGPeri_Port(0x100) |= BIT0)
#define mUsbRmWkupClr()			(wFOTGPeri_Port(0x100) &= ~BIT0)

#define mUsbTstHalfSpeedEn()		(wFOTGPeri_Port(0x100) |= BIT1)
#define mUsbTstHalfSpeedDis()	        (wFOTGPeri_Port(0x100) &= ~BIT1)

#define mUsbGlobIntEnRd()		(wFOTGPeri_Port(0x100) & BIT2)
#define mUsbGlobIntEnSet()		(wFOTGPeri_Port(0x100) |= BIT2)
#define mUsbGlobIntDis()		(wFOTGPeri_Port(0x100) &= ~BIT2)

#define mUsbGoSuspend()			(wFOTGPeri_Port(0x100) |=  BIT3)

#define mUsbSoftRstSet()		(wFOTGPeri_Port(0x100) |=  BIT4)
#define mUsbSoftRstClr()		(wFOTGPeri_Port(0x100) &= ~BIT4)

#define mUsbChipEnSet()			(wFOTGPeri_Port(0x100) |= BIT5)
#define mUsbHighSpeedST()		(wFOTGPeri_Port(0x100) & BIT6)
#define mUsbDMAResetSet()		(wFOTGPeri_Port(0x100) |= BIT8)

// Device address register(0x104)
#define mUsbDevAddrSet(Value)	        (wFOTGPeri_Port(0x104) = (u32)Value)
#define mUsbCfgST()			(wFOTGPeri_Port(0x104) & BIT7)
#define mUsbCfgSet()			(wFOTGPeri_Port(0x104) |= BIT7)
#define mUsbCfgClr()			(wFOTGPeri_Port(0x104) &= ~BIT7)

// Test register(0x108)
#define mUsbClrAllFIFOSet()		(wFOTGPeri_Port(0x108) |= BIT0)
#define mUsbClrAllFIFOClr()		(wFOTGPeri_Port(0x108) &= ~BIT0)

// SOF Frame Number register(0x10C)
#define mUsbFrameNo()			(u16)(wFOTGPeri_Port(0x10C) & 0x7FF)
#define mUsbMicroFrameNo()		(u8)((wFOTGPeri_Port(0x10C) & 0x3800)>>11)
#define mUsbFrameNoLow()		(mUsbFrameNo() & 0xff)
#define mUsbFrameNoHigh()		(mUsbFrameNo() >> 8)	

// SOF Mask register(0x110)
#define mUsbSOFMaskHS()		        (wFOTGPeri_Port(0x110) = 0x44c)
#define mUsbSOFMaskFS()		        (wFOTGPeri_Port(0x110) = 0x2710)

// PHY Test Mode Selector register(0x114)
#define mUsbTsMdWr(item)		(wFOTGPeri_Port(0x114) = (u32)item)
#define mUsbUnPLGClr()			(wFOTGPeri_Port(0x114) &= ~BIT0)
#define mUsbUnPLGSet()			(wFOTGPeri_Port(0x114) |= BIT0)
// Vendor Specific IO Control register(0x118)

// Cx configuration and status register(0x11C)

// Cx configuration and FIFO Empty Status register(0x120)
#define mUsbEP0DoneSet()		(wFOTGPeri_Port(0x120) |= BIT0)
#define mUsbTsPkDoneSet()		(wFOTGPeri_Port(0x120) |= BIT1)
#define mUsbEP0StallSet()		(wFOTGPeri_Port(0x120) |= BIT2)
#define mUsbCxFClr()			(wFOTGPeri_Port(0x120) |= BIT3)

#define mUsbCxFFull()			(wFOTGPeri_Port(0x120) & BIT4)
#define mUsbCxFEmpty()			(wFOTGPeri_Port(0x120) & BIT5)
#define mUsbCxFByteCnt()		(u8)((wFOTGPeri_Port(0x120) & 0x7F000000)>>24)
#define mUsbEP0ClearFIFO()              (bFOTGPeri_Port(0x0120) |= (u8)BIT3)  //john
#define mUsbEP0EMPFIFO()                (bFOTGPeri_Port(0x0120) &= (u8)BIT5)  //john

// IDLE Counter register(0x124)
#define mUsbIdleCnt(time)		(wFOTGPeri_Port(0x124) = (u32)time)

// Mask of interrupt group(0x130)
#define mUsbIntGrp0Dis()		(wFOTGPeri_Port(0x130) |= BIT0)
#define mUsbIntGrp1Dis()		(wFOTGPeri_Port(0x130) |= BIT1)
#define mUsbIntGrp2Dis()		(wFOTGPeri_Port(0x130) |= BIT2)

#define mUsbIntGroupMaskRd()	        (wFOTGPeri_Port(0x130))

// Mask of interrupt source group 0(0x134)
#define mUsbIntEP0SetupDis()		(wFOTGPeri_Port(0x134) |= BIT0)
#define mUsbIntEP0InDis()		(wFOTGPeri_Port(0x134) |= BIT1)
#define mUsbIntEP0OutDis()		(wFOTGPeri_Port(0x134) |= BIT2)
#define mUsbIntEP0EndDis()		(wFOTGPeri_Port(0x134) |= BIT3)
#define mUsbIntEP0FailDis()		(wFOTGPeri_Port(0x134) |= BIT4)

#define mUsbIntEP0SetupEn()		(wFOTGPeri_Port(0x134) &= ~(BIT0))
#define mUsbIntEP0InEn()		(wFOTGPeri_Port(0x134) &= ~(BIT1))
#define mUsbIntEP0OutEn()		(wFOTGPeri_Port(0x134) &= ~(BIT2))
#define mUsbIntEP0EndEn()		(wFOTGPeri_Port(0x134) &= ~(BIT3))
#define mUsbIntEP0FailEn()		(wFOTGPeri_Port(0x134) &= ~(BIT4))

#define mUsbIntSrc0MaskRd()		(wFOTGPeri_Port(0x134))

// Mask of interrupt source group 1(0x138)
#define mUsbIntFIFO0_3OUTDis()	        (wFOTGPeri_Port(0x138) |= 0xFF)
#define mUsbIntFIFO0_3INDis()	        (wFOTGPeri_Port(0x138) |= 0xF0000)
#define mUsbIntFIFO0_3Set(wTemp)	(wFOTGPeri_Port(0x138)|= wTemp)
#define mUsbIntFIFO0_3Dis(wTemp)	(wFOTGPeri_Port(0x138)&= ~wTemp)

#define mUsbIntF0OUTEn()		(wFOTGPeri_Port(0x138) &= ~(BIT1 | BIT0))
#define mUsbIntF0OUTDis()		(wFOTGPeri_Port(0x138) |= (BIT1 | BIT0))
#define mUsbIntF1OUTEn()		(wFOTGPeri_Port(0x138) &= ~(BIT3 | BIT2))
#define mUsbIntF1OUTDis()		(wFOTGPeri_Port(0x138) |= (BIT3 | BIT2))
#define mUsbIntF2OUTEn()		(wFOTGPeri_Port(0x138) &= ~(BIT5 | BIT4))
#define mUsbIntF2OUTDis()		(wFOTGPeri_Port(0x138) |= (BIT5 | BIT4))
#define mUsbIntF3OUTEn()		(wFOTGPeri_Port(0x138) &= ~(BIT7 | BIT6))
#define mUsbIntF3OUTDis()		(wFOTGPeri_Port(0x138) |= (BIT7 | BIT6))

#define mUsbIntFXOUTEn(bnum)	        (wFOTGPeri_Port(0x138) &= ~((BIT0<<((bnum)*2+1)) | (BIT0<<((bnum)*2))))
#define mUsbIntFXOUTDis(bnum)     	(wFOTGPeri_Port(0x138) |= ((BIT0<<((bnum)*2+1)) | (BIT0<<((bnum)*2))))

#define mUsbIntF0INEn()			(wFOTGPeri_Port(0x138) &= ~BIT16)
#define mUsbIntF0INDis()		(wFOTGPeri_Port(0x138) |= BIT16)
#define mUsbIntF1INEn()			(wFOTGPeri_Port(0x138) &= ~BIT17)
#define mUsbIntF1INDis()		(wFOTGPeri_Port(0x138) |= BIT17)
#define mUsbIntF2INEn()			(wFOTGPeri_Port(0x138) &= ~BIT18)
#define mUsbIntF2INDis()		(wFOTGPeri_Port(0x138) |= BIT18)
#define mUsbIntF3INEn()			(wFOTGPeri_Port(0x138) &= ~BIT19)
#define mUsbIntF3INDis()		(wFOTGPeri_Port(0x138) |= BIT19)

#define mUsbIntFXINEn(bnum)		(wFOTGPeri_Port(0x138) &= ~(BIT0<<(bnum+16)))
#define mUsbIntFXINDis(bnum)	        (wFOTGPeri_Port(0x138) |= (BIT0<<(bnum+16)))

#define mUsbIntSrc1MaskRd()		(wFOTGPeri_Port(0x138))

// Mask of interrupt source group 2(DMA int mask)(0x13C)
#define mUsbIntSuspDis()		(wFOTGPeri_Port(0x13C) |= BIT1)
#define mUsbIntDmaErrDis()		(wFOTGPeri_Port(0x13C) |= BIT8)
#define mUsbIntDmaFinishDis()	        (wFOTGPeri_Port(0x13C) |= BIT7)

#define mUsbIntSuspEn()			(wFOTGPeri_Port(0x13C) &= ~(BIT1))
#define mUsbIntDmaErrEn()		(wFOTGPeri_Port(0x13C) &= ~(BIT8))
#define mUsbIntDmaFinishEn()	        (wFOTGPeri_Port(0x13C) &= ~(BIT7))

#define mUsbIntSrc2MaskRd()		(wFOTGPeri_Port(0x13C))

// Interrupt group (0x140)
#define mUsbIntGroupRegRd()		(wFOTGPeri_Port(0x140))
#define mUsbIntGroupRegSet(wValue)	(wFOTGPeri_Port(0x140) |= wValue)

// Interrupt source group 0(0x144)
#define mUsbIntSrc0Rd()			(wFOTGPeri_Port(0x144))
#define mUsbIntEP0AbortClr()	        (wFOTGPeri_Port(0x144) &= ~(BIT5))
#define mUsbIntSrc0Clr()		(wFOTGPeri_Port(0x144) = 0)
#define mUsbIntSrc0Set(wValue)	        (wFOTGPeri_Port(0x144) |= wValue)

// Interrupt source group 1(0x148)
#define mUsbIntSrc1Rd()			(wFOTGPeri_Port(0x148))
#define mUsbIntSrc1Set(wValue)	        (wFOTGPeri_Port(0x148) |= wValue)

// Interrupt source group 2(0x14C)
#define mUsbIntSrc2Rd()			(wFOTGPeri_Port(0x14C))
#define mUsbIntSrc2Set(wValue)	        (wFOTGPeri_Port(0x14C) |= wValue)


#define mUsbIntBusRstClr()		(wFOTGPeri_Port(0x14C) &= ~BIT0)
#define mUsbIntSuspClr()		(wFOTGPeri_Port(0x14C) &= ~BIT1)
#define mUsbIntResmClr()		(wFOTGPeri_Port(0x14C) &= ~BIT2)
#define mUsbIntIsoSeqErrClr()	        (wFOTGPeri_Port(0x14C) &= ~BIT3)
#define mUsbIntIsoSeqAbortClr()	        (wFOTGPeri_Port(0x14C) &= ~BIT4)
#define mUsbIntTX0ByteClr()		(wFOTGPeri_Port(0x14C) &= ~BIT5)
#define mUsbIntRX0ByteClr()		(wFOTGPeri_Port(0x14C) &= ~BIT6)
#define mUsbIntDmaFinishClr()	        (wFOTGPeri_Port(0x14C) &= ~BIT7)
#define mUsbIntDmaErrClr()		(wFOTGPeri_Port(0x14C) &= ~BIT8)

#define mUsbIntDmaFinishRd()		(wFOTGPeri_Port(0x14C) & BIT7)
#define mUsbIntDmaErrRd()		(wFOTGPeri_Port(0x14C) & BIT8)

// Rx 0 byte packet register(0x150)
#define mUsbIntRX0ByteRd()		(u8)(wFOTGPeri_Port(0x150))
#define mUsbIntRX0ByteSetClr(set)	(wFOTGPeri_Port(0x150) &= ~((u32)set))

// Tx 0 byte packet register(0x154)
#define mUsbIntTX0ByteRd()		(u8)(wFOTGPeri_Port(0x154))
#define mUsbIntTX0ByteSetClr(data)	(wFOTGPeri_Port(0x154) &= ~((u32)data))

// ISO sequential Error/Abort register(0x158)
#define mUsbIntIsoSeqErrRd()		(u8)((wFOTGPeri_Port(0x158) & 0xff0000)>>16)
#define mUsbIntIsoSeqErrSetClr(data)	(wFOTGPeri_Port(0x158) &= ~(((u32)data)<<16))

#define mUsbIntIsoSeqAbortRd()	        (u8)(wFOTGPeri_Port(0x158) & 0xff)
#define mUsbIntIsoSeqAbortSetClr(data)	(wFOTGPeri_Port(0x158) &= ~((u32)data))

// IN Endpoint MaxPacketSize register(0x160,0x164,...,0x17C)
#define mUsbEPinHighBandSet(EPn, dir , size )	(wFOTGPeri_Port(0x160 + ((EPn - 1) << 2)) &= ~(BIT14 |BIT13));  (wFOTGPeri_Port(0x160 + ((EPn - 1) << 2)) |= ((((u8)(size >> 11)+1) << 13)* dir) )

#define mUsbEPMxPtSz(EPn, dir, size) (wFOTGPeri_Port(0x160 + ((1-dir) * 0x20) + ((EPn - 1) << 2)) = (u16)(size))

#define mUsbEPMxPtSzClr(EPn, dir)    (wFOTGPeri_Port(0x160 + ((1-dir) * 0x20) + ((EPn - 1) << 2)) = 0)

#define mUsbEPMxPtSzRd(EPn, dir)     (wFOTGPeri_Port(0x160 + ((1-dir) * 0x20) + ((EPn - 1) << 2)))

#define mUsbEPinMxPtSz(EPn)		(wFOTGPeri_Port(0x160 + ((EPn - 1) << 2)) & 0x7ff)
#define mUsbEPinStallST(EPn)		((wFOTGPeri_Port(0x160 + ((EPn - 1) << 2)) & BIT11) >> 11)
#define mUsbEPinStallClr(EPn)		(wFOTGPeri_Port(0x160 + ((EPn - 1) << 2)) &= ~BIT11)
#define mUsbEPinStallSet(EPn)		(wFOTGPeri_Port(0x160 + ((EPn - 1) << 2)) |=  BIT11)
#define mUsbEPinRsTgClr(EPn)		(wFOTGPeri_Port(0x160 + ((EPn - 1) << 2)) &= ~BIT12)
#define mUsbEPinRsTgSet(EPn)	        (wFOTGPeri_Port(0x160 + ((EPn - 1) << 2)) |=  BIT12)
#define mUsbEPinZeroSet(EPn)	        (wFOTGPeri_Port(0x160 + ((EPn - 1) << 2)) |=  BIT15)

// OUT Endpoint MaxPacketSize register(0x180,0x164,...,0x19C)
#define mUsbEPoutMxPtSz(EPn)	        ((wFOTGPeri_Port(0x180 + ((EPn - 1) << 2))) & 0x7ff)
#define mUsbEPoutStallST(EPn)	        ((wFOTGPeri_Port(0x180 + ((EPn - 1) << 2)) & BIT11) >> 11)
#define mUsbEPoutStallClr(EPn)	        (wFOTGPeri_Port(0x180 + ((EPn - 1) << 2)) &= ~BIT11)
#define mUsbEPoutStallSet(EPn)	        (wFOTGPeri_Port(0x180 + ((EPn - 1) << 2)) |=  BIT11)
#define mUsbEPoutRsTgClr(EPn)	        (wFOTGPeri_Port(0x180 + ((EPn - 1) << 2)) &= ~BIT12)
#define mUsbEPoutRsTgSet(EPn)	        (wFOTGPeri_Port(0x180 + ((EPn - 1) << 2)) |=  BIT12)

// Endpoint & FIFO Configuration
// Endpoint 1~4 Map register(0x1a0), Endpoint 5~8 Map register(0x1a4)
#define mUsbEPMap(EPn, MAP)	        (bFOTGPeri_Port(0x1a0 + (EPn-1)) = MAP)
#define mUsbEPMapRd(EPn)		(bFOTGPeri_Port(0x1a0+ (EPn-1)))
#define mUsbEPMapAllClr()		(wFOTGPeri_Port(0x1a0) = 0);(wFOTGPeri_Port(0x1a4) = 0)
#define mUsbEPMap1_4Rd()                (wFOTGPeri_Port(0x1a0))

// FIFO Map register(0x1a8)
#define mUsbFIFOMap(FIFOn, MAP)	        (bFOTGPeri_Port(0x1a8 + FIFOn) = MAP)
#define mUsbFIFOMapRd(FIFOn)	        (bFOTGPeri_Port(0x1a8 + FIFOn))
#define mUsbFIFOMapAllClr()		(wFOTGPeri_Port(0x1a8) = 0)
#define mUsbFIFOMapAllRd()              (wFOTGPeri_Port(0x1a8))
// FIFO Configuration register(0x1ac)
#define mUsbFIFOConfig(FIFOn, CONFIG)	(bFOTGPeri_Port(0x1ac + FIFOn) = CONFIG)
#define mUsbFIFOConfigRd(FIFOn)		(bFOTGPeri_Port(0x1ac + FIFOn))
#define mUsbFIFOConfigAllClr()		(bFOTGPeri_Port(0x1ac) = 0)
#define FIFOEnBit			0x20
#define mUsbFIFOConfigAllRd()           (wFOTGPeri_Port(0x1ac))
// FIFO byte count register(0x1b0)
#define mUsbFIFOOutByteCount(fifo_num)	(((wFOTGPeri_Port(0x1b0+(fifo_num)*4)&0x7ff)))
#define mUsbFIFODone(fifo_num)		(wFOTGPeri_Port(0x1b0+(fifo_num)*4) |= BIT11)
#define mUsbFIFOReset(fifo_num)		(wFOTGPeri_Port(0x1b0+(fifo_num)*4) |=  BIT12) //john
#define mUsbFIFOResetOK(fifo_num)	(wFOTGPeri_Port(0x1b0+(fifo_num)*4) &= ~BIT12) //john

// DMA target FIFO register(0x1c0)
#define FOTG200_DMA2FIFO_Non 		0
#define FOTG200_DMA2FIFO0 		BIT0
#define FOTG200_DMA2FIFO1 		BIT1
#define FOTG200_DMA2FIFO2 		BIT2
#define FOTG200_DMA2FIFO3 		BIT3
#define FOTG200_DMA2CxFIFO 		BIT4

#define mUsbDMA2FIFOSel(sel)		(wFOTGPeri_Port(0x1c0) = sel)
#define mUsbDMA2FIFORd()		(wFOTGPeri_Port(0x1c0))

// DMA parameter set 1 (0x1c8)
#define mUsbDmaConfig(len,Dir)		(wFOTGPeri_Port(0x1c8) = (((u32)len)<<8)|(Dir<<1))
#define mUsbDmaLenRd()			((wFOTGPeri_Port(0x1c8) & 0x1fff0000) >> 8)
#define mUsbDmaConfigRd()		(wFOTGPeri_Port(0x1c8))
#define mUsbDmaConfigSet(set)		(wFOTGPeri_Port(0x1c8) = set)

#define mUsbDmaStart()			(wFOTGPeri_Port(0x1c8) |= BIT0)
#define mUsbDmaStop()			(wFOTGPeri_Port(0x1c8) &= ~BIT0)

// DMA parameter set 2 (0x1cc)
#define mUsbDmaAddr(addr)		(wFOTGPeri_Port(0x1cc) = addr)
#define mUsbDmaAddrRd()			(wFOTGPeri_Port(0x1cc))

// 8 byte command data port(0x1d0)
#define mUsbEP0CmdDataRdDWord()	        (wFOTGPeri_Port(0x1d0))

//For OTG Definition;;0x80
#define mUsb_OTGC_Control_B_BUS_REQ_Rd()	(wFOTGPeri_Port(0x80)& BIT0)
#define mUsb_OTGC_Control_B_BUS_REQ_Set()  	(wFOTGPeri_Port(0x80) |=  BIT0)
#define mUsb_OTGC_Control_B_BUS_REQ_Clr()  	(wFOTGPeri_Port(0x80) &=  (~BIT0))

#define mUsb_OTGC_Control_B_HNP_EN_Rd()		(wFOTGPeri_Port(0x80)& BIT1)
#define mUsb_OTGC_Control_B_HNP_EN_Set()	(wFOTGPeri_Port(0x80) |=  BIT1)
#define mUsb_OTGC_Control_B_HNP_EN_Clr()  	(wFOTGPeri_Port(0x80) &=  (~BIT1))

#define mUsb_OTGC_Control_B_DSCHG_VBUS_Rd()	(wFOTGPeri_Port(0x80)& BIT2)
#define mUsb_OTGC_Control_B_DSCHG_VBUS_Set()	(wFOTGPeri_Port(0x80) |=  BIT2)
#define mUsb_OTGC_Control_B_DSCHG_VBUS_Clr() 	(wFOTGPeri_Port(0x80) &=  (~BIT2))

#define mUsb_OTGC_Control_B_SESS_END_Rd()	(wFOTGPeri_Port(0x80)& BIT16)
#define mUsb_TGC_Control_B_SESS_VLD_Rd()	(wFOTGPeri_Port(0x80)& BIT17)
#define mUsb_TGC_Control_A_SESS_VLD_Rd()	(wFOTGPeri_Port(0x80)& BIT18)
#define mUsb_TGC_Control_A_VBUS_VLD_Rd()	(wFOTGPeri_Port(0x80)& BIT19)

#define mUsb_OTGC_Control_CROLE_Rd()	        (wFOTGPeri_Port(0x80)& BIT20) //0:Host 1:Peripheral

#define mUsb_dwOTGC_INT_STS_ROLE_CHG_Rd()       (wFOTGPeri_Port(0x84)& BIT8)

//For Host Port Reset setting (Timing Critical)
#define mUsb_mwHost20_PORTSC_PortReset_Rd()	(wFOTGPeri_Port(0x30) &=  BIT8)
#define mUsb_mwHost20_PORTSC_PortReset_Set()	(wFOTGPeri_Port(0x30) |=  BIT8)
#define mUsb_mwHost20_PORTSC_PortReset_Clr()	(wFOTGPeri_Port(0x30) &=  ~BIT8)

#define mUsbIntFIFOEn(off,val)  (bFOTGPeri_Port(CPE_FOTG200_BASE | off) &= ~(val))        //john
#define mUsbIntFIFODis(off,val) (bFOTGPeri_Port(CPE_FOTG200_BASE | off) |=  (val))        //john

#endif /* __FOTG200_M_H  */

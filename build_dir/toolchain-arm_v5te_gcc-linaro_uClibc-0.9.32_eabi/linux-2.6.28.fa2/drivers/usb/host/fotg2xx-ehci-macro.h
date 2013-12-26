#ifndef FOTG2XX_EHCI_MACRO_H_CHK
#define FOTG2XX_EHCI_MACRO_H_CHK

#include "fotg2xx-config.h"
#include <asm/io.h>
/*-------------------------------------------------------------------------*/


static inline int ft_get_version(void)
{
	static int version = 0;
	int inc = 0;

	/*
	 * Version ID:
	 *     818110: A/B version
	 *     818111: C version
	 *     818112: D version
	 */
	if (!version) {
		version = ftpmu010_read_reg(0x0) >> 8;
		switch ((ftpmu010_read_reg(0x0)>>5) & 0x7) {
			case 0 : inc = 0; break;
			case 2 : inc = 1; break;
			case 6 : inc = 2; break;
			case 7 : inc = 3; break;
			default: inc = 0; break;
		}
		if ((version & 0xf0) == 0x10) { /* 8181 series */
			printk("IC: GM%04x(%c)\n", ((version >> 8) & 0xffff) + inc, (version & 0xff) + 'B' - 0x10);
		} else {
			if ((ftpmu010_read_reg(0x0)>>4) & 0x1) { /* 8181T series */
				printk("IC: GM%04xT(%c)\n", ((version >> 8) & 0xffff) + inc, (version & 0xff) + 'A' - 0x20);
			} else { /* 8186 */
				printk("IC: GM%04x(%c)\n", ((version >> 8) & 0xffff) + 5 + inc, (version & 0xff) + 'A' - 0x20);
			}
		}
	}

	return version;
}

#if defined(CONFIG_GM_FOTG2XX) || defined(CONFIG_USB_GADGET_DUALSPEED)
  /* FOTG2XX Global Controller Register Macro Definition */
  //<1>.Macro volatile
  #define FOTG2XX_BASE_ADDRESS	            FOTG200_REGBASE
  #define mwOTG20Port(bOffset)	            *((volatile u32 *)(FOTG2XX_BASE_ADDRESS | bOffset))
  #define mwOTG20Bit_Rd(bByte,wBitNum)            (mwOTG20Port(bByte)&wBitNum)
  #define mwOTG20Bit_Set(bByte,wBitNum)           (mwOTG20Port(bByte)|=wBitNum)
  #define mwOTG20Bit_Clr(bByte,wBitNum)           (mwOTG20Port(bByte)&=~wBitNum)
  // 0x0C0(Interrupt Status)
  #define mwOTG20_Interrupt_Status_HOST_Rd()       (mwOTG20Bit_Rd(0xC0,0x00000004))
  #define mwOTG20_Interrupt_Status_HOST_Clr()	   (mwOTG20Port(0xC0)= 0x00000004)

  // 0x0C4(Interrupt Mask)
  #define mwOTG20_Interrupt_Mask_HOST_Clr()	    mwOTG20Bit_Clr(0xC4,0x00000004)

  //0x100(Device Controller Registers(Address = 100h~1FFh) )
  #define mwPeri20_Control_ChipEnable_Set()         mwOTG20Bit_Set(0x100,BIT5)
  #define mwPeri20_Control_HALFSPEEDEnable_Set()    mwOTG20Bit_Set(0x100,BIT1)
  #define mwPeri20_Control_HALFSPEEDEnable_Clr()    mwOTG20Bit_Clr(0x100,BIT1)
  //0x80 <OTG Controller Register>
  #define mwOTG20_Control_ForceFullSpeed_Set()      mwOTG20Bit_Set(0x80,BIT12)
  #define mwOTG20_Control_ForceHighSpeed_Set()      (mwOTG20Bit_Set(0x80,BIT14))  
  #define mwOTG20_Control_ForceFullSpeed_Clr()      mwOTG20Bit_Clr(0x80,BIT12)
  #define mwOTG20_Control_ForceHighSpeed_Clr()      (mwOTG20Bit_Clr(0x80,BIT14))
  
  #ifdef CONFIG_711MA_PHY
     // Cover 711MA PHY (Full speed reset issue)
     #define mwOTG20_Control_COVER_FS_PHY_Reset_Set()  mwOTG20Bit_Set(0x80,BIT28)
     #define mwOTG20_Control_COVER_FS_PHY_Reset_Clr()  mwOTG20Bit_Clr(0x80,BIT28)
  #endif
#endif /* CONFIG_GM_FOTG2XX */

#ifdef CONFIG_GM_FUSBH200
  /* FOTG2XX Global Controller Register Macro Definition */
  //<1>.Macro volatile
  #define FUSBH200_BASE_ADDRESS                 FUSBH200_REGBASE
  #define mwH20Port(bOffset)               *((volatile u32 *)(FUSBH200_BASE_ADDRESS | bOffset))
  #define mwH20Bit_Rd(bByte,wBitNum)            (mwH20Port(bByte)&wBitNum)
  #define mwH20Bit_Set(bByte,wBitNum)           (mwH20Port(bByte)|=wBitNum)
  #define mwH20Bit_Clr(bByte,wBitNum)           (mwH20Port(bByte)&=~wBitNum)

  // 0x40(Bus Monitor Control/Status)
  #define mwH20_Control_VBUS_FLT_SEL_Set()          mwH20Bit_Set(0x40,BIT0)
  #define mwH20_Control_VBUS_FLT_SEL_Clr()          mwH20Bit_Clr(0x40,BIT0)
  #define mwH20_Control_HDISCON_FLT_SEL_Set()       mwH20Bit_Set(0x40,BIT1)
  #define mwH20_Control_HDISCON_FLT_SEL_Clr()       mwH20Bit_Clr(0x40,BIT1)
  #define mwH20_Control_HALFSPEEDEnable_Set()       mwH20Bit_Set(0x40,BIT2)
  #define mwH20_Control_HALFSPEEDEnable_Clr()       mwH20Bit_Clr(0x40,BIT2)
  #define mwH20_Interrupt_OutPut_High_Set()         mwH20Bit_Set(0x40,BIT3)
  #define mwH20_Interrupt_OutPut_High_Clr()         mwH20Bit_Clr(0x40,BIT3)
  #define mwH20_Control_VBUS_OFF_Set()              mwH20Bit_Set(0x40,BIT4)
  #define mwH20_Control_VBUS_OFF_Clr()              mwH20Bit_Clr(0x40,BIT4)
  #define mwH20_Control_Phy_Reset_Set()             mwH20Bit_Set(0x40,BIT5)
  #define mwH20_Control_Phy_Reset_Clr()             mwH20Bit_Clr(0x40,BIT5)
  #define mwH20_Control_ForceFullSpeed_Set()        mwH20Bit_Set(0x40,BIT7)
  #define mwH20_Control_ForceHighSpeed_Set()        mwH20Bit_Set(0x40,BIT6)  
  #define mwH20_Control_ForceFullSpeed_Clr()        mwH20Bit_Clr(0x40,BIT7)
  #define mwH20_Control_ForceHighSpeed_Clr()        mwH20Bit_Clr(0x40,BIT6)
  #define mwH20_Control_VBUS_VLD_Rd()               mwH20Bit_Rd(0x40,BIT8)
  #define mwH20_Control_HOST_SPD_TYP_Rd()           mwH20Bit_Rd(0x40,(BIT9|BIT10))
  #ifdef CONFIG_711MA_PHY
     // Cover 711MA PHY (Full speed reset issue)
     #define mwH20_Control_COVER_FS_PHY_Reset_Set()    mwH20Bit_Set(0x40,BIT12)
     #define mwH20_Control_COVER_FS_PHY_Reset_Clr()    mwH20Bit_Clr(0x40,BIT12)
  #endif
  
  // 0x044(Bus Monitor Interrupt Status)
  #define mwH20_Int_Status_BM_DMA_ERROR_Rd()       mwH20Bit_Rd(0x44,0x00000010)
  #define mwH20_Int_Status_BM_DMA_CMPLT_Rd()       mwH20Bit_Rd(0x44,0x00000008)
  #define mwH20_Int_Status_BM_DPLGRMV_Rd()         mwH20Bit_Rd(0x44,0x00000004)
  #define mwH20_Int_Status_BM_OVC_Rd()             mwH20Bit_Rd(0x44,0x00000002)
  #define mwH20_Int_Status_BM_VBUS_ERR_Rd()        mwH20Bit_Rd(0x44,0x00000001)
  #define mwH20_Int_Status_BM_DMA_ERROR_Clr()      (mwH20Port(0x44)= 0x00000010)
  #define mwH20_Int_Status_BM_DMA_CMPLT_Clr()      (mwH20Port(0x44)= 0x00000008)
  #define mwH20_Int_Status_BM_DPLGRMV_Clr()        (mwH20Port(0x44)= 0x00000004)
  #define mwH20_Int_Status_BM_OVC_Clr()            (mwH20Port(0x44)= 0x00000002)
  #define mwH20_Int_Status_BM_VBUS_ERR_Clr()       (mwH20Port(0x44)= 0x00000001)
  // 0x048(Bus Monitor Interrupt Enable)
  #define mwH20_Int_BM_DMA_ERROR_En()              mwH20Bit_Set(0x48,0x00000010)
  #define mwH20_Int_BM_DMA_CMPLT_En()              mwH20Bit_Set(0x48,0x00000008)
  #define mwH20_Int_BM_BPLGRMV_En()                mwH20Bit_Set(0x48,0x00000004)
  #define mwH20_Int_BM_OVC_En()                    mwH20Bit_Set(0x48,0x00000002)
  #define mwH20_Int_BM_VBUS_ERR_En()               mwH20Bit_Set(0x48,0x00000001)
  #define mwH20_Int_BM_DMA_ERROR_Dis()             mwH20Bit_Clr(0x48,0x00000010)
  #define mwH20_Int_BM_DMA_CMPLT_Dis()             mwH20Bit_Clr(0x48,0x00000008)
  #define mwH20_Int_BM_DPLGRMV_Dis()               mwH20Bit_Clr(0x48,0x00000004)
  #define mwH20_Int_BM_OVC_Dis()                   mwH20Bit_Clr(0x48,0x00000002)
  #define mwH20_Int_BM_VBUS_ERR_Dis()              mwH20Bit_Clr(0x48,0x00000001)
  #define mwH20_Int_BM_DMA_ERROR_Rd()              mwH20Bit_Rd(0x48,0x00000010)
  #define mwH20_Int_BM_DMA_CMPLT_Rd()              mwH20Bit_Rd(0x48,0x00000008)
  #define mwH20_Int_BM_BPLGRMV_Rd()                mwH20Bit_Rd(0x48,0x00000004)
  #define mwH20_Int_BM_OVC_Rd()                    mwH20Bit_Rd(0x48,0x00000002)
  #define mwH20_Int_BM_VBUS_ERR_Rd()               mwH20Bit_Rd(0x48,0x00000001)
	// 0x50(Test)
  #define mwH20_Test_TST_JSTA_Set()                mwH20Bit_Set(0x50,BIT0)
  #define mwH20_Test_TST_JSTA_Clr()                mwH20Bit_Clr(0x50,BIT0)
  #define mwH20_Test_TST_KSTA_Set()                mwH20Bit_Set(0x50,BIT1)
  #define mwH20_Test_TST_KSTA_Clr()                mwH20Bit_Clr(0x50,BIT1)
  #define mwH20_Test_TST_PKT_Set()                 mwH20Bit_Set(0x50,BIT2)
  #define mwH20_Test_TST_PKT_Clr()                 mwH20Bit_Clr(0x50,BIT2)
  #define mwH20_Test_TST_MOD_Set()                 mwH20Bit_Set(0x50,BIT3)
  #define mwH20_Test_TST_MOD_Clr()                 mwH20Bit_Clr(0x50,BIT3)
  #define mwH20_Test_TST_LOOPBK_Set()              mwH20Bit_Set(0x50,BIT4)
  #define mwH20_Test_TST_LOOPBK_Clr()              mwH20Bit_Clr(0x50,BIT4)
  // 0x54(Vendor Specific I/O Control Register)
  #define mwH20_Test_VCTL_Rd()                     mwH20Bit_Rd(0x50,0x0000001F)
  #define mwH20_Test_VCTL_Set(Value)               mwH20Bit_Set(0x50,Value)
  #define mwH20_Test_VCTLOAD_N_Set()               mwH20Bit_Set(0x50,BIT5)
  #define mwH20_Test_VCTLOAD_N_Clr()               mwH20Bit_Clr(0x50,BIT5)

#endif /* CONFIG_GM_FUSBH200 */

#endif  

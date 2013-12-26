//////////////////////////////////////////////////////////////////////////////
//  File name: OTGController.h
//  Company: GM Tech. Corp.
//  Description: 1.Define Data Type
//               2.Define for Bulk
//               3.Define Macro
///////////////////////////////////////////////////////////////////////////////

#include "fotg2xx-config.h"
#include "fotg2xx.h"
#include <linux/usb.h>
#include <linux/usb/gadget.h>

#ifndef FOTG2XX_GADGET_MACRO_H_CHK
#define FOTG2XX_GADGET_MACRO_H_CHK

#if defined(CONFIG_GM_FOTG2XX) || defined(CONFIG_USB_GADGET_DUALSPEED)
#define OTG_BASE_ADDRESS	    (FOTG200_REGBASE+0x80)
#define mbFOTGPort(bOffset)	    *((volatile u8 *) ( OTG_BASE_ADDRESS | bOffset))
#define mwFOTGPort(bOffset)	    *((volatile u16 *) ( OTG_BASE_ADDRESS | bOffset))
#define mdwFOTGPort(bOffset)	    *((volatile u32 *) ( OTG_BASE_ADDRESS | bOffset))
#define mdwPort(portbase,bOffset)   *((volatile u32 *) ( portbase | bOffset))
#endif


#define BIT0	                 0x00000001
#define BIT1	                 0x00000002
#define BIT2	                 0x00000004
#define BIT3	                 0x00000008
#define BIT4	                 0x00000010
#define BIT5	                 0x00000020
#define BIT6	                 0x00000040
#define BIT7	                 0x00000080
#define BIT8	                 0x00000100
#define BIT9	                 0x00000200
#define BIT10	                 0x00000400
#define BIT11	                 0x00000800
#define BIT12	                 0x00001000
#define BIT13	                 0x00002000
#define BIT14	                 0x00004000
#define BIT15	                 0x00008000	
#define BIT16	                 0x00010000
#define BIT17	                 0x00020000
#define BIT18	                 0x00040000
#define BIT19	                 0x00080000
#define BIT20	                 0x00100000
#define BIT21	                 0x00200000
#define BIT22	                 0x00400000
#define BIT23	                 0x00800000	
#define BIT24	                 0x01000000
#define BIT25	                 0x02000000
#define BIT26	                 0x04000000
#define BIT27	                 0x08000000
#define BIT28	                 0x10000000
#define BIT29	                 0x20000000
#define BIT30	                 0x40000000
#define BIT31	                 0x80000000	

//=================== 1.OTG Condition Setting ==========================================
#define   OTGC_ENABLE     //If Only peripheral => Please disable this item
#define   OTGC_A_SRP_RESPONSE_TYPE  A_SRP_RESP_TYPE_VBUS  //A_SRP_RESP_TYPE_DATA_LINE
#define   OTGC_Device_Not_Support_Then_Return 1

//=================== 2.Define Macro ==================================================
#define mwOTG20_stop_host()    mwOTG20Bit_Clr(0x10,BIT0)
#define mwOTG20_start_host()   mwOTG20Bit_Set(0x10,BIT0)
#define mwOTG20_b_conn_Rd()    mwOTG20Bit_Rd(0x30,BIT0)
#define mwOTG20_reset_Set()    mwOTG20Bit_Set(0x30,BIT8)
#define mwOTG20_reset_Clr()    mwOTG20Bit_Clr(0x30,BIT8)
#define mwOTG20_porten_Set()   mwOTG20Bit_Set(0x30,BIT2)
#define mwOTG20_porten_Clr()   mwOTG20Bit_Clr(0x30,BIT2)
#define mwOTG20_suspend_Set()  mwOTG20Bit_Set(0x30,BIT7)
#define mwOTG20_suspend_Clr()  mwOTG20Bit_Clr(0x30,BIT7)

//Offset:0x000(OTG Control/Status Register) => Suppose Word-Read & Word-Write
//~B
#define mdwOTGC_Control_B_BUS_REQ_Rd()         (mdwFOTGPort(0x00)& BIT0)  
#define mdwOTGC_Control_B_BUS_REQ_Set()        (mdwFOTGPort(0x00) |=  BIT0)  
#define mdwOTGC_Control_B_BUS_REQ_Clr()        (mdwFOTGPort(0x00) &=  (~BIT0))  
   
#define mdwOTGC_Control_B_HNP_EN_Rd()          (mdwFOTGPort(0x00)& BIT1)  
#define mdwOTGC_Control_B_HNP_EN_Set()         (mdwFOTGPort(0x00) |=  BIT1)   
#define mdwOTGC_Control_B_HNP_EN_Clr()         (mdwFOTGPort(0x00) &=  (~BIT1))  
    
#define mdwOTGC_Control_B_DSCHG_VBUS_Rd()      (mdwFOTGPort(0x00)& BIT2)  
#define mdwOTGC_Control_B_DSCHG_VBUS_Set()     (mdwFOTGPort(0x00) |=  BIT2)    
#define mdwOTGC_Control_B_DSCHG_VBUS_Clr()     (mdwFOTGPort(0x00) &=  (~BIT2))  

#if defined(CONFIG_GM_FOTG2XX) || defined(CONFIG_USB_GADGET_DUALSPEED)
#define mdwOTGC_Control_A_SRP_RESP_TYPE_Rd()    (mdwFOTGPort(0x00)& BIT8)
#define mdwOTGC_Control_A_SRP_RESP_TYPE_Set()   (mdwFOTGPort(0x00) |=  BIT8)
#define mdwOTGC_Control_A_SRP_RESP_TYPE_Clr()   (mdwFOTGPort(0x00) &=  (~BIT8))

#define mdwOTGC_Control_A_SRP_DET_EN_Rd()        (mdwFOTGPort(0x00)& BIT7)
#define mdwOTGC_Control_A_SRP_DET_EN_Set()       (mdwFOTGPort(0x00) |=  BIT7)
#define mdwOTGC_Control_A_SRP_DET_EN_Clr()       (mdwFOTGPort(0x00) &=  (~BIT7))
#endif
 
#define mdwOTGC_Control_A_SET_B_HNP_EN_Rd()     (mdwFOTGPort(0x00)& BIT6)  
#define mdwOTGC_Control_A_SET_B_HNP_EN_Set()    (mdwFOTGPort(0x00) |=  BIT6)    
#define mdwOTGC_Control_A_SET_B_HNP_EN_Clr()    (mdwFOTGPort(0x00) &=  (~BIT6))    

#if defined(CONFIG_GM_FOTG2XX) || defined(CONFIG_USB_GADGET_DUALSPEED)
#define mdwOTGC_Control_A_BUS_DROP_Rd()     (mdwFOTGPort(0x00)& BIT5)
#define mdwOTGC_Control_A_BUS_DROP_Set()    (mdwFOTGPort(0x00) |=  BIT5)
#define mdwOTGC_Control_A_BUS_DROP_Clr()    (mdwFOTGPort(0x00) &=  (~BIT5))
#define mdwOTGC_Control_A_BUS_REQ_Rd()      (mdwFOTGPort(0x00)& BIT4)
#define mdwOTGC_Control_A_BUS_REQ_Set()     (mdwFOTGPort(0x00) |=  BIT4)
#define mdwOTGC_Control_A_BUS_REQ_Clr()     (mdwFOTGPort(0x00) &=  (~BIT4))
#endif

#define mdwOTGC_Control_OTG_Reset_Set()     (mdwFOTGPort(0x00) |=  BIT24)
#define mdwOTGC_Control_OTG_Reset_Clr()     (mdwFOTGPort(0x00) &=  (~BIT24))
#define mdwOTGC_Control_PHY_Reset_Set()     (mdwFOTGPort(0x00) |=  BIT15)
#define mdwOTGC_Control_PHY_Reset_Clr()     (mdwFOTGPort(0x00) &=  (~BIT15))
#define mdwOTGC_Control_B_SESS_END_Rd()     (mdwFOTGPort(0x00)& BIT16)    
#define mdwOTGC_Control_B_SESS_VLD_Rd()     (mdwFOTGPort(0x00)& BIT17)    
#define mdwOTGC_Control_A_SESS_VLD_Rd()     (mdwFOTGPort(0x00)& BIT18)    
#define mdwOTGC_Control_A_VBUS_VLD_Rd()     (mdwFOTGPort(0x00)& BIT19)  
#define mdwOTGC_Control_CROLE_Rd()          (mdwFOTGPort(0x00)& BIT20)    
#define mdwOTGC_Control_ID_Rd()             (mdwFOTGPort(0x00)& BIT21)    
#define mdwOTGC_Control_Rd()                (mdwFOTGPort(0x00))    
  
#define A_SRP_RESP_TYPE_VBUS                0x00    
#define A_SRP_RESP_TYPE_DATA_LINE           0x10
  
//Offset:0x004(OTG Interrupt Status Register) 
#define mdwOTGC_INT_STS_Rd()                (mdwFOTGPort(0x04))
#define mdwOTGC_INT_STS_Clr(wValue)         (mdwFOTGPort(0x04) |= wValue)    
    
#define OTGC_INT_BSRPDN                     BIT0  
#define OTGC_INT_ASRPDET                    BIT4
#define OTGC_INT_AVBUSERR                   BIT5
#define OTGC_INT_RLCHG                      BIT8
#define OTGC_INT_IDCHG                      BIT9
#define OTGC_INT_OVC                        BIT10
#define OTGC_INT_BPLGRMV                    BIT11
#define OTGC_INT_APLGRMV                    BIT12

#define OTGC_INT_A_TYPE   (OTGC_INT_ASRPDET|OTGC_INT_AVBUSERR|OTGC_INT_OVC|OTGC_INT_RLCHG|OTGC_INT_IDCHG|OTGC_INT_BPLGRMV|OTGC_INT_APLGRMV)
#define OTGC_INT_B_TYPE   (OTGC_INT_BSRPDN|OTGC_INT_AVBUSERR|OTGC_INT_OVC|OTGC_INT_RLCHG|OTGC_INT_IDCHG)

//Offset:0x008(OTG Interrupt Enable Register) 
#define mdwOTGC_INT_Enable_Rd()              (mdwFOTGPort(0x08))
#define mdwOTGC_INT_Enable_Set(wValue)       (mdwFOTGPort(0x08)|= wValue)
#define mdwOTGC_INT_Enable_Clr(wValue)       (mdwFOTGPort(0x08)&= (~wValue))

#define mdwOTGC_INT_Enable_BSRPDN_Set()      (mdwFOTGPort(0x08) |=  BIT0)   
#define mdwOTGC_INT_Enable_ASRPDET_Set()     (mdwFOTGPort(0x08) |=  BIT4)   
#define mdwOTGC_INT_Enable_AVBUSERR_Set()    (mdwFOTGPort(0x08) |=  BIT5)   
#define mdwOTGC_INT_Enable_RLCHG_Set()       (mdwFOTGPort(0x08) |=  BIT8)   
#define mdwOTGC_INT_Enable_IDCHG_Set()       (mdwFOTGPort(0x08) |=  BIT9)   
#define mdwOTGC_INT_Enable_OVC_Set()         (mdwFOTGPort(0x08) |=  BIT10)   
#define mdwOTGC_INT_Enable_BPLGRMV_Set()     (mdwFOTGPort(0x08) |=  BIT11)   
#define mdwOTGC_INT_Enable_APLGRMV_Set()     (mdwFOTGPort(0x08) |=  BIT12)   

#define mdwOTGC_INT_Enable_BSRPDN_Clr()      (mdwFOTGPort(0x08) &= ~BIT0)        
#define mdwOTGC_INT_Enable_ASRPDET_Clr()     (mdwFOTGPort(0x08) &= ~BIT4)        
#define mdwOTGC_INT_Enable_AVBUSERR_Clr()    (mdwFOTGPort(0x08) &= ~BIT5)        
#define mdwOTGC_INT_Enable_RLCHG_Clr()       (mdwFOTGPort(0x08) &= ~BIT8)        
#define mdwOTGC_INT_Enable_IDCHG_Clr()       (mdwFOTGPort(0x08) &= ~BIT9)        
#define mdwOTGC_INT_Enable_OVC_Clr()         (mdwFOTGPort(0x08) &= ~BIT10)        
#define mdwOTGC_INT_Enable_BPLGRMV_Clr()     (mdwFOTGPort(0x08) &= ~BIT11)       
#define mdwOTGC_INT_Enable_APLGRMV_Clr()     (mdwFOTGPort(0x08) &= ~BIT12)       
    
#if defined(CONFIG_GM_FOTG2XX) || defined(CONFIG_USB_GADGET_DUALSPEED)
// PHY Test Mode Selector register(0x114)
#define mdwOTGC_UsbUnPLGClr()	             (mdwPort(FOTG200_REGBASE,0x114) &= ~BIT0)
#define mdwOTGC_UsbUnPLGSet()                (mdwPort(FOTG200_REGBASE,0x114) |=  BIT0)
#define mwOTG20_Interrupt_OutPut_High_Set()  (mdwPort(FOTG200_REGBASE,0xC4) |= BIT3)
#define mwOTG20_Interrupt_OutPut_High_Clr()  (mdwPort(FOTG200_REGBASE,0xC4) &= ~BIT3)  
#endif

#if defined(CONFIG_GM_FOTG2XX) || defined(CONFIG_USB_GADGET_DUALSPEED)
//===================3.Define Stricture ==================================================
#define OTG_ID_A_TYPE 0
#define OTG_ID_B_TYPE ((u32)1 << 21)

#define OTG_ROLE_HOST 	0
#define OTG_ROLE_DEVICE ((u32)1 << 20)

/*typedef struct OTGC_STS
  {
    volatile int     CurrentRole;            // 0:Host  1:Peripheral
    u32              wCurrentInterruptMask;
    volatile u8      bVBUS_Vaild;
    volatile u8      bDevice_RESET_Received;
    volatile u8      bDevice_RemoteWakeUp_Received;
    volatile u8      A_bASRPDET;
    volatile u8      B_bBSRPDN;
    volatile u8      A_BPLGRMV;
    volatile u8      A_APLGRMV;
    volatile u8      IDCHG;
    struct usb_otgd  *otgd;
} OTGC_STS;*/ 

#define HC_SUSPEND  3
#define HC_OPER     2
#define HC_RESUME   1
#define HC_RESET    0

#define USB_HOST_ROLE     0
#define USB_DEVICE_ROLE   1
#define USB_OTG_TYPEA     0
#define USB_OTG_TYPEB     1
//Used by power management related funciton (ex. suspend, wake up, etc.)
#define USB_PWM_SUSPEND   0
#define USB_OTG_SUSPEND   1

extern u8 OTGH_SET_FEATURE_OTG[];
//===================4.Define Extern Function ===========================================
int fotg200_init(struct device *dev,struct usb_bus *host,struct usb_gadget *gadget);
int fotg200_otgd_init(struct device *dev,struct usb_bus *host,struct usb_gadget *gadget);
void init_FOTG2XX_Dev(void);
int is_init_FOTG2XX_Dev(void);
int usb_get_pmu_fd(void);
#endif
  
#endif /* FOTG2XX_GADGET_MACRO_H_CHK */

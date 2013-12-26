
/*******************************************************************************
 * Module name: config.h
 *
 * The information contained herein is confidential property of Company.
 * The user, copying, transfer or disclosure of such information is 
 * prohibited except by express written agreement with Company.
 *
 * Module Description:
 *  Port from kernel_tree/include/Linux/config.h
 *  macro definition for configuration of platform and
 *  compiler (DEBUG macros)
 *
 ******************************************************************************/

#ifndef FOTG2XX_CONFIG_H_CHK
#define FOTG2XX_CONFIG_H_CHK

#ifdef CONFIG_GM_FOTG2XX_MODULE
#define CONFIG_GM_FOTG2XX
#endif

/* Define this const macro will disable any cod0e which is used to recover HW issue */
//#define REMOVE_COVERHW	1

/* =======================================================*/
/* OTG application related macro definitions              */
/* =======================================================*/
#define CONFIG_OTG
#define CONFIG_OPT_TESTING

#define OTG_SRP_DATA_PULSING
//#define OTG_SRP_VBUS_PULSING

/* =======================================================*/
//Please only define following macros if you need to cover PHY issues
//#define CONFIG_711MA_PHY		/* For 711MA to enable hc to cover full speed reset issue */
/* =======================================================*/
#undef  CONFIG_GM_USB
#define CONFIG_GM_USB

// The following define is moved to menuconfig
//Please only define one of four types depenedinn on which type of controller
//#define CONFIG_GM_FOTG2XX		/* FOTG2XX or FOTG210 */
//#define CONFIG_GM_FUSBH200

////////////////////////////////////////
//#ifdef CONFIG_GM_FOTG2XX
#undef CONFIG_OTG
#undef CONFIG_GM_USB20
#define CONFIG_OTG
#define CONFIG_GM_USB20
#define FOTG200_REGBASE   USB_FOTG2XX_0_VA_BASE 
#define FOTG200_IRQ       USB_FOTG2XX_IRQ
//#endif

////////////////////////////////////////
#ifdef CONFIG_GM_FUSBH200
#undef  CONFIG_OTG
#undef CONFIG_GM_USB20
#define CONFIG_OTG
#define CONFIG_GM_USB20
#define FUSBH200_REGBASE   USB_FUSBH200_VA_BASE 
#define FUSBH200_IRQ       USB_FUSBH200_IRQ
#else
#define USB_FUSBH200_VA_BASE	0
#endif

/* ======================================================*/
/* DEBUG fucniton macro definitions                      */
/* ======================================================*/
//#define SHOW_MSG
//#define DEBUG
//#define CONFIG_DEBUG
//#define CONFIG_USB_DEBUG
//#define VERBOSE_DEBUG
//#define KERNEL_DEBUG
#ifndef CONFIG_GM_FOTG2XX
#ifdef VERBOSE_DEBUG
#define OHCI_VERBOSE_DEBUG  //Enable Verbose debug, dump more details
#endif
#else
#ifdef VERBOSE_DEBUG
#define EHCI_VERBOSE_DEBUG  //Enable Verbose debug, dump more details
#endif
#endif

#endif  /* FOTG2XX-CONFIG_H_CHK */

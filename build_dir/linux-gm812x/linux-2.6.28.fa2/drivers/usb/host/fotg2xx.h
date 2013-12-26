#ifndef FOTG2XX_H_CHK
#define FOTG2XX_H_CHK

#ifdef SHOW_MSG
#define msg	printk
#else
#define msg(format,...) do {} while (0)
#endif

#if defined(CONFIG_GM_FOTG2XX_INFO) || defined(CONFIG_GM_FUSBH200_INFO)
#define fotg2xx_dbg(format, ...) printk(KERN_INFO " "format, __VA_ARGS__ )
#else
#define fotg2xx_dbg(format, ...) do {} while (0)
#endif

#ifdef CONFIG_GM_FOTG2XX_INFO
#define fotg2xx_show_hc_regs() { \
      printk(" FOTG2XX host controller registers\n"); \
      printk(" Cap:%08x Cmd:%08x Sts:%08x HIE:%08x\n",\
	     mwOTG20Port(0x8),mwOTG20Port(0x10),mwOTG20Port(0x14),mwOTG20Port(0x18)); \
      printk(" FrI:%08x PBA:%08x ABA:%08x PSC:%08x Mis:%08x\n",\
             mwOTG20Port(0x1c),mwOTG20Port(0x24),mwOTG20Port(0x28),mwOTG20Port(0x30),mwOTG20Port(0x40)); \
}	

#define fotg2xx_show_otg_regs() { \
      printk(" FOTG2XXX On-The-Go controller and global registers\n"); \
      printk(" OCS:%08x OIS:%08x OIE:%08x GIS:%08x GIM:%08x\n",\
             mwOTG20Port(0x80),mwOTG20Port(0x84),mwOTG20Port(0x88),mwOTG20Port(0xc0),mwOTG20Port(0xc4));\
}

#define fotg2xx_show_dev_regs() { \
      printk(" FOTG2XXX USB 2.0 device controller registers\n"); \
      printk(" Ctl:%08x Adr:%08x Tst:%08x %08x\n",\
             mwOTG20Port(0x100),mwOTG20Port(0x104),mwOTG20Port(0x108),mwOTG20Port(0x114)); \
}

#else
#define fotg2xx_show_hc_regs() { } 
#define fotg2xx_show_otg_regs() { }
#define fotg2xx_show_dev_regs() { }
#endif
#ifdef CONFIG_GM_FUSBH200_INFO
#define fusbh200_show_hc_regs() { \
      printk(" FUSBH200 host controller registers\n"); \
      printk(" Cap:%08x Cmd:%08x Sts:%08x HIE:%08x\n",\
             mwH20Port(0x8),mwH20Port(0x10),mwH20Port(0x14),mwH20Port(0x18)); \
      printk(" FrI:%08x PBA:%08x ABA:%08x PSC:%08x Mis:%08x\n",\
             mwH20Port(0x1c),mwH20Port(0x24),mwH20Port(0x28),mwH20Port(0x30),mwH20Port(0x40)); \
}	
#else
#define fusbh200_show_hc_regs() { } 
#endif

u32 ehci_is_ehci_irq(void);
void fotg200_handle_irq(void);

#ifdef KERNEL_DEBUG
#define kdbg dbg
#else
#define kdbg(format, ...) do {} while (0)
#endif

#endif /* FOTG2XX_H_CHK */


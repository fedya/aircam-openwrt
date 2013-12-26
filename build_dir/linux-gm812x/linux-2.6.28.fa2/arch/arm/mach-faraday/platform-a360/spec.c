/*
 *  linux/arch/arm/mach-faraday/platform-a360/spec.c
 *
 *  Faraday Platform Specification  (Automatically generated by "autospec", do not edit)
 *
 *  Copyright (C) 2009 Faraday Corp. (http://www.faraday-tech.com)
 *
 *  Platform Hierarchy :
 *
 *    A360
 *     +- CPU_FA626TE  (0)
 *     +- ADC_FTTSC010  (0)
 *     +- AES_FTAES020  (0)
 *     +- AHBC_FTAHBC020  (0)
 *     +- DDRC_FTDDRII020  (0)
 *     +- EBC_FTLVBC020  (0)
 *     +- EBCIO_FTLVBC020  (0)
 *     +- EBCMEM_FTLVBC020  (0)
 *     +- GPIO_FTGPIO010  (0)
 *     +- GPIO_FTGPIO010  (1)
 *     +- IIC_FTIIC010  (0)
 *     +- IIC_FTIIC010  (1)
 *     +- INTC_FTINTC010  (0)
 *     +- LCDC_FTLCDC200  (0)
 *     +- MAC_FTMAC110  (0)
 *     +- NANDC_FTNANDC020  (0)
 *     +- PCI1
 *     |   +- PCI_FTPCIE3914  (0)
 *     +- PCIIO_FTPCIE3914  (0)
 *     +- PCIMEM_FTPCIE3914  (0)
 *     +- PMU_FTPMU010  (0)
 *     +- RAM_FTDDRII020  (0)
 *     +- RTC_FTRTC010  (0)
 *     +- SCU_FTSCU000  (0)
 *     +- SDC_FTSDC010  (0)
 *     +- SMC_FTSMC020  (0)
 *     +- SSP_FTSSP010  (0)
 *     +- SSP_FTSSP010  (1)
 *     +- TIMER_FTTMR010  (0)
 *     +- UART_FTUART010  (0)
 *     +- UART_FTUART010  (1)
 *     +- USB_FOTG2XX  (0)
 *     +- USB_FUSBH200  (1)
 *     +- WDT_FTWDT010  (0)
 *     +- APBDMA
 *     |   +- APBBRG_FTAPBBRG020  (0)
 *     +- AHBDMA
 *         +- DMAC_FTDMAC020  (0)
 */


#include <linux/init.h>
#include <linux/module.h>

#include <asm/mach/map.h>
#include <asm/sizes.h>

#include <mach/platform/spec.h>

/*
 * Interrrupt numbers
 */


/* AES */
const unsigned char AES_FTAES020_irq[AES_FTAES020_COUNT] = {
     AES_FTAES020_0_IRQ
};

/* AHBC */
const unsigned char AHBC_FTAHBC020_irq[AHBC_FTAHBC020_COUNT] = {
     AHBC_FTAHBC020_0_IRQ
};

/* EBC */
const unsigned char EBC_FTLVBC020_irq[EBC_FTLVBC020_COUNT][EBC_FTLVBC020_IRQ_COUNT] = {
    { EBC_FTLVBC020_0_IRQ0, EBC_FTLVBC020_0_IRQ1 }
};

/* GPIO */
const unsigned char GPIO_FTGPIO010_irq[GPIO_FTGPIO010_COUNT] = {
     GPIO_FTGPIO010_0_IRQ,
     GPIO_FTGPIO010_1_IRQ
};

/* IIC */
const unsigned char IIC_FTIIC010_irq[IIC_FTIIC010_COUNT] = {
     IIC_FTIIC010_0_IRQ,
     IIC_FTIIC010_1_IRQ
};

/* LCDC */
const unsigned char LCDC_FTLCDC200_irq[LCDC_FTLCDC200_COUNT] = {
     LCDC_FTLCDC200_0_IRQ
};

/* MAC */
const unsigned char MAC_FTMAC110_irq[MAC_FTMAC110_COUNT] = {
     MAC_FTMAC110_0_IRQ
};

/* NANDC */
const unsigned char NANDC_FTNANDC020_irq[NANDC_FTNANDC020_COUNT] = {
     NANDC_FTNANDC020_0_IRQ
};

/* PMU */
const unsigned char PMU_FTPMU010_irq[PMU_FTPMU010_COUNT] = {
     PMU_FTPMU010_0_IRQ
};

/* RTC */
const unsigned char RTC_FTRTC010_irq[RTC_FTRTC010_COUNT][RTC_FTRTC010_IRQ_COUNT] = {
    { RTC_FTRTC010_0_IRQ0, RTC_FTRTC010_0_IRQ1 }
};

/* SDC */
const unsigned char SDC_FTSDC010_irq[SDC_FTSDC010_COUNT] = {
     SDC_FTSDC010_0_IRQ
};

/* SSP */
const unsigned char SSP_FTSSP010_irq[SSP_FTSSP010_COUNT] = {
     SSP_FTSSP010_0_IRQ,
     SSP_FTSSP010_1_IRQ
};

/* TIMER */
const unsigned char TIMER_FTTMR010_irq[TIMER_FTTMR010_COUNT][TIMER_FTTMR010_IRQ_COUNT] = {
    { TIMER_FTTMR010_0_IRQ0, TIMER_FTTMR010_0_IRQ1, TIMER_FTTMR010_0_IRQ2 }
};

/* UART */
const unsigned char UART_FTUART010_irq[UART_FTUART010_COUNT] = {
     UART_FTUART010_0_IRQ,
     UART_FTUART010_1_IRQ
};

/* USB */
const unsigned char USB_FOTG2XX_irq[USB_FOTG2XX_COUNT] = {
     USB_FOTG2XX_0_IRQ
};
const unsigned char USB_FUSBH200_irq[USB_FUSBH200_COUNT] = {
     USB_FUSBH200_0_IRQ
};

/* WDT */
const unsigned char WDT_FTWDT010_irq[WDT_FTWDT010_COUNT] = {
     WDT_FTWDT010_0_IRQ
};

/* PCI */
const unsigned char PCI_FTPCIE3914_irq[PCI_FTPCIE3914_COUNT][PCI_FTPCIE3914_IRQ_COUNT] = {
    { PCI_FTPCIE3914_0_IRQ0, PCI_FTPCIE3914_0_IRQ1, PCI_FTPCIE3914_0_IRQ2, PCI_FTPCIE3914_0_IRQ3 }
};

/* APBBRG */
const unsigned char APBBRG_FTAPBBRG020_irq[APBBRG_FTAPBBRG020_COUNT][APBBRG_FTAPBBRG020_IRQ_COUNT] = {
    { APBBRG_FTAPBBRG020_0_IRQ0, APBBRG_FTAPBBRG020_0_IRQ1, APBBRG_FTAPBBRG020_0_IRQ2, APBBRG_FTAPBBRG020_0_IRQ3 }
};

/* DMAC */
const unsigned char DMAC_FTDMAC020_irq[DMAC_FTDMAC020_COUNT][DMAC_FTDMAC020_IRQ_COUNT] = {
    { DMAC_FTDMAC020_0_IRQ0, DMAC_FTDMAC020_0_IRQ1, DMAC_FTDMAC020_0_IRQ2, DMAC_FTDMAC020_0_IRQ3, DMAC_FTDMAC020_0_IRQ4, DMAC_FTDMAC020_0_IRQ5, DMAC_FTDMAC020_0_IRQ6, DMAC_FTDMAC020_0_IRQ7 }
};

/*
 * Base addresses
 */


/* CPU */
const unsigned CPU_MEM_pa_base[CPU_MEM_PA_COUNT] = {
     CPU_MEM_0_PA_BASE
};
const unsigned CPU_MEM_pa_limit[CPU_MEM_PA_COUNT] = {
     CPU_MEM_0_PA_LIMIT
};
const unsigned CPU_MEM_pa_size[CPU_MEM_PA_COUNT] = {
     CPU_MEM_0_PA_SIZE
};

/* ADC */
const unsigned ADC_FTTSC010_pa_base[ADC_FTTSC010_COUNT] = {
     ADC_FTTSC010_0_PA_BASE
};
const unsigned ADC_FTTSC010_pa_limit[ADC_FTTSC010_COUNT] = {
     ADC_FTTSC010_0_PA_LIMIT
};
const unsigned ADC_FTTSC010_pa_size[ADC_FTTSC010_COUNT] = {
     ADC_FTTSC010_0_PA_SIZE
};
const unsigned ADC_FTTSC010_va_base[ADC_FTTSC010_COUNT] = {
     ADC_FTTSC010_0_VA_BASE
};
const unsigned ADC_FTTSC010_va_limit[ADC_FTTSC010_COUNT] = {
     ADC_FTTSC010_0_VA_LIMIT
};
const unsigned ADC_FTTSC010_va_size[ADC_FTTSC010_COUNT] = {
     ADC_FTTSC010_0_VA_SIZE
};

/* AES */
const unsigned AES_FTAES020_pa_base[AES_FTAES020_COUNT] = {
     AES_FTAES020_0_PA_BASE
};
const unsigned AES_FTAES020_pa_limit[AES_FTAES020_COUNT] = {
     AES_FTAES020_0_PA_LIMIT
};
const unsigned AES_FTAES020_pa_size[AES_FTAES020_COUNT] = {
     AES_FTAES020_0_PA_SIZE
};
const unsigned AES_FTAES020_va_base[AES_FTAES020_COUNT] = {
     AES_FTAES020_0_VA_BASE
};
const unsigned AES_FTAES020_va_limit[AES_FTAES020_COUNT] = {
     AES_FTAES020_0_VA_LIMIT
};
const unsigned AES_FTAES020_va_size[AES_FTAES020_COUNT] = {
     AES_FTAES020_0_VA_SIZE
};

/* AHBC */
const unsigned AHBC_FTAHBC020_pa_base[AHBC_FTAHBC020_COUNT] = {
     AHBC_FTAHBC020_0_PA_BASE
};
const unsigned AHBC_FTAHBC020_pa_limit[AHBC_FTAHBC020_COUNT] = {
     AHBC_FTAHBC020_0_PA_LIMIT
};
const unsigned AHBC_FTAHBC020_pa_size[AHBC_FTAHBC020_COUNT] = {
     AHBC_FTAHBC020_0_PA_SIZE
};
const unsigned AHBC_FTAHBC020_va_base[AHBC_FTAHBC020_COUNT] = {
     AHBC_FTAHBC020_0_VA_BASE
};
const unsigned AHBC_FTAHBC020_va_limit[AHBC_FTAHBC020_COUNT] = {
     AHBC_FTAHBC020_0_VA_LIMIT
};
const unsigned AHBC_FTAHBC020_va_size[AHBC_FTAHBC020_COUNT] = {
     AHBC_FTAHBC020_0_VA_SIZE
};

/* DDRC */
const unsigned DDRC_FTDDRII020_pa_base[DDRC_FTDDRII020_COUNT] = {
     DDRC_FTDDRII020_0_PA_BASE
};
const unsigned DDRC_FTDDRII020_pa_limit[DDRC_FTDDRII020_COUNT] = {
     DDRC_FTDDRII020_0_PA_LIMIT
};
const unsigned DDRC_FTDDRII020_pa_size[DDRC_FTDDRII020_COUNT] = {
     DDRC_FTDDRII020_0_PA_SIZE
};
const unsigned DDRC_FTDDRII020_va_base[DDRC_FTDDRII020_COUNT] = {
     DDRC_FTDDRII020_0_VA_BASE
};
const unsigned DDRC_FTDDRII020_va_limit[DDRC_FTDDRII020_COUNT] = {
     DDRC_FTDDRII020_0_VA_LIMIT
};
const unsigned DDRC_FTDDRII020_va_size[DDRC_FTDDRII020_COUNT] = {
     DDRC_FTDDRII020_0_VA_SIZE
};

/* EBC */
const unsigned EBC_FTLVBC020_pa_base[EBC_FTLVBC020_COUNT] = {
     EBC_FTLVBC020_0_PA_BASE
};
const unsigned EBC_FTLVBC020_pa_limit[EBC_FTLVBC020_COUNT] = {
     EBC_FTLVBC020_0_PA_LIMIT
};
const unsigned EBC_FTLVBC020_pa_size[EBC_FTLVBC020_COUNT] = {
     EBC_FTLVBC020_0_PA_SIZE
};
const unsigned EBC_FTLVBC020_va_base[EBC_FTLVBC020_COUNT] = {
     EBC_FTLVBC020_0_VA_BASE
};
const unsigned EBC_FTLVBC020_va_limit[EBC_FTLVBC020_COUNT] = {
     EBC_FTLVBC020_0_VA_LIMIT
};
const unsigned EBC_FTLVBC020_va_size[EBC_FTLVBC020_COUNT] = {
     EBC_FTLVBC020_0_VA_SIZE
};

/* EBCIO */
const unsigned EBCIO_FTLVBC020_pa_base[EBCIO_FTLVBC020_COUNT] = {
     EBCIO_FTLVBC020_0_PA_BASE
};
const unsigned EBCIO_FTLVBC020_pa_limit[EBCIO_FTLVBC020_COUNT] = {
     EBCIO_FTLVBC020_0_PA_LIMIT
};
const unsigned EBCIO_FTLVBC020_pa_size[EBCIO_FTLVBC020_COUNT] = {
     EBCIO_FTLVBC020_0_PA_SIZE
};
const unsigned EBCIO_FTLVBC020_va_base[EBCIO_FTLVBC020_COUNT] = {
     EBCIO_FTLVBC020_0_VA_BASE
};
const unsigned EBCIO_FTLVBC020_va_limit[EBCIO_FTLVBC020_COUNT] = {
     EBCIO_FTLVBC020_0_VA_LIMIT
};
const unsigned EBCIO_FTLVBC020_va_size[EBCIO_FTLVBC020_COUNT] = {
     EBCIO_FTLVBC020_0_VA_SIZE
};

/* EBCMEM */
const unsigned EBCMEM_FTLVBC020_pa_base[EBCMEM_FTLVBC020_COUNT] = {
     EBCMEM_FTLVBC020_0_PA_BASE
};
const unsigned EBCMEM_FTLVBC020_pa_limit[EBCMEM_FTLVBC020_COUNT] = {
     EBCMEM_FTLVBC020_0_PA_LIMIT
};
const unsigned EBCMEM_FTLVBC020_pa_size[EBCMEM_FTLVBC020_COUNT] = {
     EBCMEM_FTLVBC020_0_PA_SIZE
};
const unsigned EBCMEM_FTLVBC020_va_base[EBCMEM_FTLVBC020_COUNT] = {
     EBCMEM_FTLVBC020_0_VA_BASE
};
const unsigned EBCMEM_FTLVBC020_va_limit[EBCMEM_FTLVBC020_COUNT] = {
     EBCMEM_FTLVBC020_0_VA_LIMIT
};
const unsigned EBCMEM_FTLVBC020_va_size[EBCMEM_FTLVBC020_COUNT] = {
     EBCMEM_FTLVBC020_0_VA_SIZE
};

/* GPIO */
const unsigned GPIO_FTGPIO010_pa_base[GPIO_FTGPIO010_COUNT] = {
     GPIO_FTGPIO010_0_PA_BASE,
     GPIO_FTGPIO010_1_PA_BASE
};
const unsigned GPIO_FTGPIO010_pa_limit[GPIO_FTGPIO010_COUNT] = {
     GPIO_FTGPIO010_0_PA_LIMIT,
     GPIO_FTGPIO010_1_PA_LIMIT
};
const unsigned GPIO_FTGPIO010_pa_size[GPIO_FTGPIO010_COUNT] = {
     GPIO_FTGPIO010_0_PA_SIZE,
     GPIO_FTGPIO010_1_PA_SIZE
};
const unsigned GPIO_FTGPIO010_va_base[GPIO_FTGPIO010_COUNT] = {
     GPIO_FTGPIO010_0_VA_BASE,
     GPIO_FTGPIO010_1_VA_BASE
};
const unsigned GPIO_FTGPIO010_va_limit[GPIO_FTGPIO010_COUNT] = {
     GPIO_FTGPIO010_0_VA_LIMIT,
     GPIO_FTGPIO010_1_VA_LIMIT
};
const unsigned GPIO_FTGPIO010_va_size[GPIO_FTGPIO010_COUNT] = {
     GPIO_FTGPIO010_0_VA_SIZE,
     GPIO_FTGPIO010_1_VA_SIZE
};

/* IIC */
const unsigned IIC_FTIIC010_pa_base[IIC_FTIIC010_COUNT] = {
     IIC_FTIIC010_0_PA_BASE,
     IIC_FTIIC010_1_PA_BASE
};
const unsigned IIC_FTIIC010_pa_limit[IIC_FTIIC010_COUNT] = {
     IIC_FTIIC010_0_PA_LIMIT,
     IIC_FTIIC010_1_PA_LIMIT
};
const unsigned IIC_FTIIC010_pa_size[IIC_FTIIC010_COUNT] = {
     IIC_FTIIC010_0_PA_SIZE,
     IIC_FTIIC010_1_PA_SIZE
};
const unsigned IIC_FTIIC010_va_base[IIC_FTIIC010_COUNT] = {
     IIC_FTIIC010_0_VA_BASE,
     IIC_FTIIC010_1_VA_BASE
};
const unsigned IIC_FTIIC010_va_limit[IIC_FTIIC010_COUNT] = {
     IIC_FTIIC010_0_VA_LIMIT,
     IIC_FTIIC010_1_VA_LIMIT
};
const unsigned IIC_FTIIC010_va_size[IIC_FTIIC010_COUNT] = {
     IIC_FTIIC010_0_VA_SIZE,
     IIC_FTIIC010_1_VA_SIZE
};

/* INTC */
const unsigned INTC_FTINTC010_pa_base[INTC_FTINTC010_COUNT] = {
     INTC_FTINTC010_0_PA_BASE
};
const unsigned INTC_FTINTC010_pa_limit[INTC_FTINTC010_COUNT] = {
     INTC_FTINTC010_0_PA_LIMIT
};
const unsigned INTC_FTINTC010_pa_size[INTC_FTINTC010_COUNT] = {
     INTC_FTINTC010_0_PA_SIZE
};
const unsigned INTC_FTINTC010_va_base[INTC_FTINTC010_COUNT] = {
     INTC_FTINTC010_0_VA_BASE
};
const unsigned INTC_FTINTC010_va_limit[INTC_FTINTC010_COUNT] = {
     INTC_FTINTC010_0_VA_LIMIT
};
const unsigned INTC_FTINTC010_va_size[INTC_FTINTC010_COUNT] = {
     INTC_FTINTC010_0_VA_SIZE
};

/* LCDC */
const unsigned LCDC_FTLCDC200_pa_base[LCDC_FTLCDC200_COUNT] = {
     LCDC_FTLCDC200_0_PA_BASE
};
const unsigned LCDC_FTLCDC200_pa_limit[LCDC_FTLCDC200_COUNT] = {
     LCDC_FTLCDC200_0_PA_LIMIT
};
const unsigned LCDC_FTLCDC200_pa_size[LCDC_FTLCDC200_COUNT] = {
     LCDC_FTLCDC200_0_PA_SIZE
};
const unsigned LCDC_FTLCDC200_va_base[LCDC_FTLCDC200_COUNT] = {
     LCDC_FTLCDC200_0_VA_BASE
};
const unsigned LCDC_FTLCDC200_va_limit[LCDC_FTLCDC200_COUNT] = {
     LCDC_FTLCDC200_0_VA_LIMIT
};
const unsigned LCDC_FTLCDC200_va_size[LCDC_FTLCDC200_COUNT] = {
     LCDC_FTLCDC200_0_VA_SIZE
};

/* MAC */
const unsigned MAC_FTMAC110_pa_base[MAC_FTMAC110_COUNT] = {
     MAC_FTMAC110_0_PA_BASE
};
const unsigned MAC_FTMAC110_pa_limit[MAC_FTMAC110_COUNT] = {
     MAC_FTMAC110_0_PA_LIMIT
};
const unsigned MAC_FTMAC110_pa_size[MAC_FTMAC110_COUNT] = {
     MAC_FTMAC110_0_PA_SIZE
};
const unsigned MAC_FTMAC110_va_base[MAC_FTMAC110_COUNT] = {
     MAC_FTMAC110_0_VA_BASE
};
const unsigned MAC_FTMAC110_va_limit[MAC_FTMAC110_COUNT] = {
     MAC_FTMAC110_0_VA_LIMIT
};
const unsigned MAC_FTMAC110_va_size[MAC_FTMAC110_COUNT] = {
     MAC_FTMAC110_0_VA_SIZE
};

/* NANDC */
const unsigned NANDC_FTNANDC020_pa_base[NANDC_FTNANDC020_COUNT] = {
     NANDC_FTNANDC020_0_PA_BASE
};
const unsigned NANDC_FTNANDC020_pa_limit[NANDC_FTNANDC020_COUNT] = {
     NANDC_FTNANDC020_0_PA_LIMIT
};
const unsigned NANDC_FTNANDC020_pa_size[NANDC_FTNANDC020_COUNT] = {
     NANDC_FTNANDC020_0_PA_SIZE
};
const unsigned NANDC_FTNANDC020_va_base[NANDC_FTNANDC020_COUNT] = {
     NANDC_FTNANDC020_0_VA_BASE
};
const unsigned NANDC_FTNANDC020_va_limit[NANDC_FTNANDC020_COUNT] = {
     NANDC_FTNANDC020_0_VA_LIMIT
};
const unsigned NANDC_FTNANDC020_va_size[NANDC_FTNANDC020_COUNT] = {
     NANDC_FTNANDC020_0_VA_SIZE
};

/* PCIIO */
const unsigned PCIIO_FTPCIE3914_pa_base[PCIIO_FTPCIE3914_COUNT] = {
     PCIIO_FTPCIE3914_0_PA_BASE
};
const unsigned PCIIO_FTPCIE3914_pa_limit[PCIIO_FTPCIE3914_COUNT] = {
     PCIIO_FTPCIE3914_0_PA_LIMIT
};
const unsigned PCIIO_FTPCIE3914_pa_size[PCIIO_FTPCIE3914_COUNT] = {
     PCIIO_FTPCIE3914_0_PA_SIZE
};
const unsigned PCIIO_FTPCIE3914_va_base[PCIIO_FTPCIE3914_COUNT] = {
     PCIIO_FTPCIE3914_0_VA_BASE
};
const unsigned PCIIO_FTPCIE3914_va_limit[PCIIO_FTPCIE3914_COUNT] = {
     PCIIO_FTPCIE3914_0_VA_LIMIT
};
const unsigned PCIIO_FTPCIE3914_va_size[PCIIO_FTPCIE3914_COUNT] = {
     PCIIO_FTPCIE3914_0_VA_SIZE
};

/* PCIMEM */
const unsigned PCIMEM_FTPCIE3914_pa_base[PCIMEM_FTPCIE3914_COUNT] = {
     PCIMEM_FTPCIE3914_0_PA_BASE
};
const unsigned PCIMEM_FTPCIE3914_pa_limit[PCIMEM_FTPCIE3914_COUNT] = {
     PCIMEM_FTPCIE3914_0_PA_LIMIT
};
const unsigned PCIMEM_FTPCIE3914_pa_size[PCIMEM_FTPCIE3914_COUNT] = {
     PCIMEM_FTPCIE3914_0_PA_SIZE
};
const unsigned PCIMEM_FTPCIE3914_va_base[PCIMEM_FTPCIE3914_COUNT] = {
     PCIMEM_FTPCIE3914_0_VA_BASE
};
const unsigned PCIMEM_FTPCIE3914_va_limit[PCIMEM_FTPCIE3914_COUNT] = {
     PCIMEM_FTPCIE3914_0_VA_LIMIT
};
const unsigned PCIMEM_FTPCIE3914_va_size[PCIMEM_FTPCIE3914_COUNT] = {
     PCIMEM_FTPCIE3914_0_VA_SIZE
};

/* PMU */
const unsigned PMU_FTPMU010_pa_base[PMU_FTPMU010_COUNT] = {
     PMU_FTPMU010_0_PA_BASE
};
const unsigned PMU_FTPMU010_pa_limit[PMU_FTPMU010_COUNT] = {
     PMU_FTPMU010_0_PA_LIMIT
};
const unsigned PMU_FTPMU010_pa_size[PMU_FTPMU010_COUNT] = {
     PMU_FTPMU010_0_PA_SIZE
};
const unsigned PMU_FTPMU010_va_base[PMU_FTPMU010_COUNT] = {
     PMU_FTPMU010_0_VA_BASE
};
const unsigned PMU_FTPMU010_va_limit[PMU_FTPMU010_COUNT] = {
     PMU_FTPMU010_0_VA_LIMIT
};
const unsigned PMU_FTPMU010_va_size[PMU_FTPMU010_COUNT] = {
     PMU_FTPMU010_0_VA_SIZE
};

/* RAM */
const unsigned RAM_FTDDRII020_pa_base[RAM_FTDDRII020_COUNT] = {
     RAM_FTDDRII020_0_PA_BASE
};
const unsigned RAM_FTDDRII020_pa_limit[RAM_FTDDRII020_COUNT] = {
     RAM_FTDDRII020_0_PA_LIMIT
};
const unsigned RAM_FTDDRII020_pa_size[RAM_FTDDRII020_COUNT] = {
     RAM_FTDDRII020_0_PA_SIZE
};

/* RTC */
const unsigned RTC_FTRTC010_pa_base[RTC_FTRTC010_COUNT] = {
     RTC_FTRTC010_0_PA_BASE
};
const unsigned RTC_FTRTC010_pa_limit[RTC_FTRTC010_COUNT] = {
     RTC_FTRTC010_0_PA_LIMIT
};
const unsigned RTC_FTRTC010_pa_size[RTC_FTRTC010_COUNT] = {
     RTC_FTRTC010_0_PA_SIZE
};
const unsigned RTC_FTRTC010_va_base[RTC_FTRTC010_COUNT] = {
     RTC_FTRTC010_0_VA_BASE
};
const unsigned RTC_FTRTC010_va_limit[RTC_FTRTC010_COUNT] = {
     RTC_FTRTC010_0_VA_LIMIT
};
const unsigned RTC_FTRTC010_va_size[RTC_FTRTC010_COUNT] = {
     RTC_FTRTC010_0_VA_SIZE
};

/* SCU */
const unsigned SCU_FTSCU000_pa_base[SCU_FTSCU000_COUNT] = {
     SCU_FTSCU000_0_PA_BASE
};
const unsigned SCU_FTSCU000_pa_limit[SCU_FTSCU000_COUNT] = {
     SCU_FTSCU000_0_PA_LIMIT
};
const unsigned SCU_FTSCU000_pa_size[SCU_FTSCU000_COUNT] = {
     SCU_FTSCU000_0_PA_SIZE
};
const unsigned SCU_FTSCU000_va_base[SCU_FTSCU000_COUNT] = {
     SCU_FTSCU000_0_VA_BASE
};
const unsigned SCU_FTSCU000_va_limit[SCU_FTSCU000_COUNT] = {
     SCU_FTSCU000_0_VA_LIMIT
};
const unsigned SCU_FTSCU000_va_size[SCU_FTSCU000_COUNT] = {
     SCU_FTSCU000_0_VA_SIZE
};

/* SDC */
const unsigned SDC_FTSDC010_pa_base[SDC_FTSDC010_COUNT] = {
     SDC_FTSDC010_0_PA_BASE
};
const unsigned SDC_FTSDC010_pa_limit[SDC_FTSDC010_COUNT] = {
     SDC_FTSDC010_0_PA_LIMIT
};
const unsigned SDC_FTSDC010_pa_size[SDC_FTSDC010_COUNT] = {
     SDC_FTSDC010_0_PA_SIZE
};
const unsigned SDC_FTSDC010_va_base[SDC_FTSDC010_COUNT] = {
     SDC_FTSDC010_0_VA_BASE
};
const unsigned SDC_FTSDC010_va_limit[SDC_FTSDC010_COUNT] = {
     SDC_FTSDC010_0_VA_LIMIT
};
const unsigned SDC_FTSDC010_va_size[SDC_FTSDC010_COUNT] = {
     SDC_FTSDC010_0_VA_SIZE
};

/* SMC */
const unsigned SMC_FTSMC020_pa_base[SMC_FTSMC020_COUNT] = {
     SMC_FTSMC020_0_PA_BASE
};
const unsigned SMC_FTSMC020_pa_limit[SMC_FTSMC020_COUNT] = {
     SMC_FTSMC020_0_PA_LIMIT
};
const unsigned SMC_FTSMC020_pa_size[SMC_FTSMC020_COUNT] = {
     SMC_FTSMC020_0_PA_SIZE
};
const unsigned SMC_FTSMC020_va_base[SMC_FTSMC020_COUNT] = {
     SMC_FTSMC020_0_VA_BASE
};
const unsigned SMC_FTSMC020_va_limit[SMC_FTSMC020_COUNT] = {
     SMC_FTSMC020_0_VA_LIMIT
};
const unsigned SMC_FTSMC020_va_size[SMC_FTSMC020_COUNT] = {
     SMC_FTSMC020_0_VA_SIZE
};

/* SSP */
const unsigned SSP_FTSSP010_pa_base[SSP_FTSSP010_COUNT] = {
     SSP_FTSSP010_0_PA_BASE,
     SSP_FTSSP010_1_PA_BASE
};
const unsigned SSP_FTSSP010_pa_limit[SSP_FTSSP010_COUNT] = {
     SSP_FTSSP010_0_PA_LIMIT,
     SSP_FTSSP010_1_PA_LIMIT
};
const unsigned SSP_FTSSP010_pa_size[SSP_FTSSP010_COUNT] = {
     SSP_FTSSP010_0_PA_SIZE,
     SSP_FTSSP010_1_PA_SIZE
};
const unsigned SSP_FTSSP010_va_base[SSP_FTSSP010_COUNT] = {
     SSP_FTSSP010_0_VA_BASE,
     SSP_FTSSP010_1_VA_BASE
};
const unsigned SSP_FTSSP010_va_limit[SSP_FTSSP010_COUNT] = {
     SSP_FTSSP010_0_VA_LIMIT,
     SSP_FTSSP010_1_VA_LIMIT
};
const unsigned SSP_FTSSP010_va_size[SSP_FTSSP010_COUNT] = {
     SSP_FTSSP010_0_VA_SIZE,
     SSP_FTSSP010_1_VA_SIZE
};

/* TIMER */
const unsigned TIMER_FTTMR010_pa_base[TIMER_FTTMR010_COUNT] = {
     TIMER_FTTMR010_0_PA_BASE
};
const unsigned TIMER_FTTMR010_pa_limit[TIMER_FTTMR010_COUNT] = {
     TIMER_FTTMR010_0_PA_LIMIT
};
const unsigned TIMER_FTTMR010_pa_size[TIMER_FTTMR010_COUNT] = {
     TIMER_FTTMR010_0_PA_SIZE
};
const unsigned TIMER_FTTMR010_va_base[TIMER_FTTMR010_COUNT] = {
     TIMER_FTTMR010_0_VA_BASE
};
const unsigned TIMER_FTTMR010_va_limit[TIMER_FTTMR010_COUNT] = {
     TIMER_FTTMR010_0_VA_LIMIT
};
const unsigned TIMER_FTTMR010_va_size[TIMER_FTTMR010_COUNT] = {
     TIMER_FTTMR010_0_VA_SIZE
};

/* UART */
const unsigned UART_FTUART010_pa_base[UART_FTUART010_COUNT] = {
     UART_FTUART010_0_PA_BASE,
     UART_FTUART010_1_PA_BASE
};
const unsigned UART_FTUART010_pa_limit[UART_FTUART010_COUNT] = {
     UART_FTUART010_0_PA_LIMIT,
     UART_FTUART010_1_PA_LIMIT
};
const unsigned UART_FTUART010_pa_size[UART_FTUART010_COUNT] = {
     UART_FTUART010_0_PA_SIZE,
     UART_FTUART010_1_PA_SIZE
};
const unsigned UART_FTUART010_va_base[UART_FTUART010_COUNT] = {
     UART_FTUART010_0_VA_BASE,
     UART_FTUART010_1_VA_BASE
};
const unsigned UART_FTUART010_va_limit[UART_FTUART010_COUNT] = {
     UART_FTUART010_0_VA_LIMIT,
     UART_FTUART010_1_VA_LIMIT
};
const unsigned UART_FTUART010_va_size[UART_FTUART010_COUNT] = {
     UART_FTUART010_0_VA_SIZE,
     UART_FTUART010_1_VA_SIZE
};

/* USB */
const unsigned USB_FOTG2XX_pa_base[USB_FOTG2XX_COUNT] = {
     USB_FOTG2XX_0_PA_BASE
};
const unsigned USB_FOTG2XX_pa_limit[USB_FOTG2XX_COUNT] = {
     USB_FOTG2XX_0_PA_LIMIT
};
const unsigned USB_FOTG2XX_pa_size[USB_FOTG2XX_COUNT] = {
     USB_FOTG2XX_0_PA_SIZE
};
const unsigned USB_FOTG2XX_va_base[USB_FOTG2XX_COUNT] = {
     USB_FOTG2XX_0_VA_BASE
};
const unsigned USB_FOTG2XX_va_limit[USB_FOTG2XX_COUNT] = {
     USB_FOTG2XX_0_VA_LIMIT
};
const unsigned USB_FOTG2XX_va_size[USB_FOTG2XX_COUNT] = {
     USB_FOTG2XX_0_VA_SIZE
};
const unsigned USB_FUSBH200_pa_base[USB_FUSBH200_COUNT] = {
     USB_FUSBH200_0_PA_BASE
};
const unsigned USB_FUSBH200_pa_limit[USB_FUSBH200_COUNT] = {
     USB_FUSBH200_0_PA_LIMIT
};
const unsigned USB_FUSBH200_pa_size[USB_FUSBH200_COUNT] = {
     USB_FUSBH200_0_PA_SIZE
};
const unsigned USB_FUSBH200_va_base[USB_FUSBH200_COUNT] = {
     USB_FUSBH200_0_VA_BASE
};
const unsigned USB_FUSBH200_va_limit[USB_FUSBH200_COUNT] = {
     USB_FUSBH200_0_VA_LIMIT
};
const unsigned USB_FUSBH200_va_size[USB_FUSBH200_COUNT] = {
     USB_FUSBH200_0_VA_SIZE
};

/* WDT */
const unsigned WDT_FTWDT010_pa_base[WDT_FTWDT010_COUNT] = {
     WDT_FTWDT010_0_PA_BASE
};
const unsigned WDT_FTWDT010_pa_limit[WDT_FTWDT010_COUNT] = {
     WDT_FTWDT010_0_PA_LIMIT
};
const unsigned WDT_FTWDT010_pa_size[WDT_FTWDT010_COUNT] = {
     WDT_FTWDT010_0_PA_SIZE
};
const unsigned WDT_FTWDT010_va_base[WDT_FTWDT010_COUNT] = {
     WDT_FTWDT010_0_VA_BASE
};
const unsigned WDT_FTWDT010_va_limit[WDT_FTWDT010_COUNT] = {
     WDT_FTWDT010_0_VA_LIMIT
};
const unsigned WDT_FTWDT010_va_size[WDT_FTWDT010_COUNT] = {
     WDT_FTWDT010_0_VA_SIZE
};

/* PCI */
const unsigned PCI_FTPCIE3914_pa_base[PCI_FTPCIE3914_COUNT] = {
     PCI_FTPCIE3914_0_PA_BASE
};
const unsigned PCI_FTPCIE3914_pa_limit[PCI_FTPCIE3914_COUNT] = {
     PCI_FTPCIE3914_0_PA_LIMIT
};
const unsigned PCI_FTPCIE3914_pa_size[PCI_FTPCIE3914_COUNT] = {
     PCI_FTPCIE3914_0_PA_SIZE
};
const unsigned PCI_FTPCIE3914_va_base[PCI_FTPCIE3914_COUNT] = {
     PCI_FTPCIE3914_0_VA_BASE
};
const unsigned PCI_FTPCIE3914_va_limit[PCI_FTPCIE3914_COUNT] = {
     PCI_FTPCIE3914_0_VA_LIMIT
};
const unsigned PCI_FTPCIE3914_va_size[PCI_FTPCIE3914_COUNT] = {
     PCI_FTPCIE3914_0_VA_SIZE
};

/* APBBRG */
const unsigned APBBRG_FTAPBBRG020_pa_base[APBBRG_FTAPBBRG020_COUNT] = {
     APBBRG_FTAPBBRG020_0_PA_BASE
};
const unsigned APBBRG_FTAPBBRG020_pa_limit[APBBRG_FTAPBBRG020_COUNT] = {
     APBBRG_FTAPBBRG020_0_PA_LIMIT
};
const unsigned APBBRG_FTAPBBRG020_pa_size[APBBRG_FTAPBBRG020_COUNT] = {
     APBBRG_FTAPBBRG020_0_PA_SIZE
};
const unsigned APBBRG_FTAPBBRG020_va_base[APBBRG_FTAPBBRG020_COUNT] = {
     APBBRG_FTAPBBRG020_0_VA_BASE
};
const unsigned APBBRG_FTAPBBRG020_va_limit[APBBRG_FTAPBBRG020_COUNT] = {
     APBBRG_FTAPBBRG020_0_VA_LIMIT
};
const unsigned APBBRG_FTAPBBRG020_va_size[APBBRG_FTAPBBRG020_COUNT] = {
     APBBRG_FTAPBBRG020_0_VA_SIZE
};

/* DMAC */
const unsigned DMAC_FTDMAC020_pa_base[DMAC_FTDMAC020_COUNT] = {
     DMAC_FTDMAC020_0_PA_BASE
};
const unsigned DMAC_FTDMAC020_pa_limit[DMAC_FTDMAC020_COUNT] = {
     DMAC_FTDMAC020_0_PA_LIMIT
};
const unsigned DMAC_FTDMAC020_pa_size[DMAC_FTDMAC020_COUNT] = {
     DMAC_FTDMAC020_0_PA_SIZE
};
const unsigned DMAC_FTDMAC020_va_base[DMAC_FTDMAC020_COUNT] = {
     DMAC_FTDMAC020_0_VA_BASE
};
const unsigned DMAC_FTDMAC020_va_limit[DMAC_FTDMAC020_COUNT] = {
     DMAC_FTDMAC020_0_VA_LIMIT
};
const unsigned DMAC_FTDMAC020_va_size[DMAC_FTDMAC020_COUNT] = {
     DMAC_FTDMAC020_0_VA_SIZE
};

/*
 * Platform Initialization Information
 */

const struct map_desc platform_io_desc[PLATFORM_IO_DESC_NR] __initdata = {
    { ADC_FTTSC010_0_VA_BASE,	__phys_to_pfn(ADC_FTTSC010_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { AES_FTAES020_0_VA_BASE,	__phys_to_pfn(AES_FTAES020_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { AHBC_FTAHBC020_0_VA_BASE,	__phys_to_pfn(AHBC_FTAHBC020_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { DDRC_FTDDRII020_0_VA_BASE,	__phys_to_pfn(DDRC_FTDDRII020_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { EBC_FTLVBC020_0_VA_BASE,	__phys_to_pfn(EBC_FTLVBC020_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { EBCIO_FTLVBC020_0_VA_BASE,	__phys_to_pfn(EBCIO_FTLVBC020_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { EBCMEM_FTLVBC020_0_VA_BASE,	__phys_to_pfn(EBCMEM_FTLVBC020_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { GPIO_FTGPIO010_0_VA_BASE,	__phys_to_pfn(GPIO_FTGPIO010_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { GPIO_FTGPIO010_1_VA_BASE,	__phys_to_pfn(GPIO_FTGPIO010_1_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { IIC_FTIIC010_0_VA_BASE,	__phys_to_pfn(IIC_FTIIC010_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { IIC_FTIIC010_1_VA_BASE,	__phys_to_pfn(IIC_FTIIC010_1_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { INTC_FTINTC010_0_VA_BASE,	__phys_to_pfn(INTC_FTINTC010_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { LCDC_FTLCDC200_0_VA_BASE,	__phys_to_pfn(LCDC_FTLCDC200_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { MAC_FTMAC110_0_VA_BASE,	__phys_to_pfn(MAC_FTMAC110_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { NANDC_FTNANDC020_0_VA_BASE,	__phys_to_pfn(NANDC_FTNANDC020_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { PCIIO_FTPCIE3914_0_VA_BASE,	__phys_to_pfn(PCIIO_FTPCIE3914_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { PCIMEM_FTPCIE3914_0_VA_BASE,	__phys_to_pfn(PCIMEM_FTPCIE3914_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { PMU_FTPMU010_0_VA_BASE,	__phys_to_pfn(PMU_FTPMU010_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { RTC_FTRTC010_0_VA_BASE,	__phys_to_pfn(RTC_FTRTC010_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { SCU_FTSCU000_0_VA_BASE,	__phys_to_pfn(SCU_FTSCU000_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { SDC_FTSDC010_0_VA_BASE,	__phys_to_pfn(SDC_FTSDC010_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { SMC_FTSMC020_0_VA_BASE,	__phys_to_pfn(SMC_FTSMC020_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { SSP_FTSSP010_0_VA_BASE,	__phys_to_pfn(SSP_FTSSP010_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { SSP_FTSSP010_1_VA_BASE,	__phys_to_pfn(SSP_FTSSP010_1_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { TIMER_FTTMR010_0_VA_BASE,	__phys_to_pfn(TIMER_FTTMR010_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { UART_FTUART010_0_VA_BASE,	__phys_to_pfn(UART_FTUART010_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { UART_FTUART010_1_VA_BASE,	__phys_to_pfn(UART_FTUART010_1_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { USB_FOTG2XX_0_VA_BASE,	__phys_to_pfn(USB_FOTG2XX_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { USB_FUSBH200_0_VA_BASE,	__phys_to_pfn(USB_FUSBH200_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { WDT_FTWDT010_0_VA_BASE,	__phys_to_pfn(WDT_FTWDT010_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { PCI_FTPCIE3914_0_VA_BASE,	__phys_to_pfn(PCI_FTPCIE3914_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { APBBRG_FTAPBBRG020_0_VA_BASE,	__phys_to_pfn(APBBRG_FTAPBBRG020_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB },
    { DMAC_FTDMAC020_0_VA_BASE,	__phys_to_pfn(DMAC_FTDMAC020_0_PA_BASE),	0x00001000,	MT_DEVICE_NCNB }
};

/*
 * Interrupt Dispatch Tables, for both FIQ and IRQ
 */

/* const interrupt_dispatch_function *fiq_dispatch_table[32]; */


/*
 * Export All Symbols
 */

/*
 * Interrrupt numbers
 */

/* AES */
EXPORT_SYMBOL( AES_FTAES020_irq );
/* AHBC */
EXPORT_SYMBOL( AHBC_FTAHBC020_irq );
/* EBC */
EXPORT_SYMBOL( EBC_FTLVBC020_irq );
/* GPIO */
EXPORT_SYMBOL( GPIO_FTGPIO010_irq );
/* IIC */
EXPORT_SYMBOL( IIC_FTIIC010_irq );
/* LCDC */
EXPORT_SYMBOL( LCDC_FTLCDC200_irq );
/* MAC */
EXPORT_SYMBOL( MAC_FTMAC110_irq );
/* NANDC */
EXPORT_SYMBOL( NANDC_FTNANDC020_irq );
/* PMU */
EXPORT_SYMBOL( PMU_FTPMU010_irq );
/* RTC */
EXPORT_SYMBOL( RTC_FTRTC010_irq );
/* SDC */
EXPORT_SYMBOL( SDC_FTSDC010_irq );
/* SSP */
EXPORT_SYMBOL( SSP_FTSSP010_irq );
/* TIMER */
EXPORT_SYMBOL( TIMER_FTTMR010_irq );
/* UART */
EXPORT_SYMBOL( UART_FTUART010_irq );
/* USB */
EXPORT_SYMBOL( USB_FOTG2XX_irq );
EXPORT_SYMBOL( USB_FUSBH200_irq );
/* WDT */
EXPORT_SYMBOL( WDT_FTWDT010_irq );
/* PCI */
EXPORT_SYMBOL( PCI_FTPCIE3914_irq );
/* APBBRG */
EXPORT_SYMBOL( APBBRG_FTAPBBRG020_irq );
/* DMAC */
EXPORT_SYMBOL( DMAC_FTDMAC020_irq );

/*
 * Base addresses
 */

/* CPU */
EXPORT_SYMBOL( CPU_MEM_pa_base );
EXPORT_SYMBOL( CPU_MEM_pa_limit );
EXPORT_SYMBOL( CPU_MEM_pa_size );
/* ADC */
EXPORT_SYMBOL( ADC_FTTSC010_pa_base );
EXPORT_SYMBOL( ADC_FTTSC010_pa_limit );
EXPORT_SYMBOL( ADC_FTTSC010_pa_size );
EXPORT_SYMBOL( ADC_FTTSC010_va_base );
EXPORT_SYMBOL( ADC_FTTSC010_va_limit );
EXPORT_SYMBOL( ADC_FTTSC010_va_size );
/* AES */
EXPORT_SYMBOL( AES_FTAES020_pa_base );
EXPORT_SYMBOL( AES_FTAES020_pa_limit );
EXPORT_SYMBOL( AES_FTAES020_pa_size );
EXPORT_SYMBOL( AES_FTAES020_va_base );
EXPORT_SYMBOL( AES_FTAES020_va_limit );
EXPORT_SYMBOL( AES_FTAES020_va_size );
/* AHBC */
EXPORT_SYMBOL( AHBC_FTAHBC020_pa_base );
EXPORT_SYMBOL( AHBC_FTAHBC020_pa_limit );
EXPORT_SYMBOL( AHBC_FTAHBC020_pa_size );
EXPORT_SYMBOL( AHBC_FTAHBC020_va_base );
EXPORT_SYMBOL( AHBC_FTAHBC020_va_limit );
EXPORT_SYMBOL( AHBC_FTAHBC020_va_size );
/* DDRC */
EXPORT_SYMBOL( DDRC_FTDDRII020_pa_base );
EXPORT_SYMBOL( DDRC_FTDDRII020_pa_limit );
EXPORT_SYMBOL( DDRC_FTDDRII020_pa_size );
EXPORT_SYMBOL( DDRC_FTDDRII020_va_base );
EXPORT_SYMBOL( DDRC_FTDDRII020_va_limit );
EXPORT_SYMBOL( DDRC_FTDDRII020_va_size );
/* EBC */
EXPORT_SYMBOL( EBC_FTLVBC020_pa_base );
EXPORT_SYMBOL( EBC_FTLVBC020_pa_limit );
EXPORT_SYMBOL( EBC_FTLVBC020_pa_size );
EXPORT_SYMBOL( EBC_FTLVBC020_va_base );
EXPORT_SYMBOL( EBC_FTLVBC020_va_limit );
EXPORT_SYMBOL( EBC_FTLVBC020_va_size );
/* EBCIO */
EXPORT_SYMBOL( EBCIO_FTLVBC020_pa_base );
EXPORT_SYMBOL( EBCIO_FTLVBC020_pa_limit );
EXPORT_SYMBOL( EBCIO_FTLVBC020_pa_size );
EXPORT_SYMBOL( EBCIO_FTLVBC020_va_base );
EXPORT_SYMBOL( EBCIO_FTLVBC020_va_limit );
EXPORT_SYMBOL( EBCIO_FTLVBC020_va_size );
/* EBCMEM */
EXPORT_SYMBOL( EBCMEM_FTLVBC020_pa_base );
EXPORT_SYMBOL( EBCMEM_FTLVBC020_pa_limit );
EXPORT_SYMBOL( EBCMEM_FTLVBC020_pa_size );
EXPORT_SYMBOL( EBCMEM_FTLVBC020_va_base );
EXPORT_SYMBOL( EBCMEM_FTLVBC020_va_limit );
EXPORT_SYMBOL( EBCMEM_FTLVBC020_va_size );
/* GPIO */
EXPORT_SYMBOL( GPIO_FTGPIO010_pa_base );
EXPORT_SYMBOL( GPIO_FTGPIO010_pa_limit );
EXPORT_SYMBOL( GPIO_FTGPIO010_pa_size );
EXPORT_SYMBOL( GPIO_FTGPIO010_va_base );
EXPORT_SYMBOL( GPIO_FTGPIO010_va_limit );
EXPORT_SYMBOL( GPIO_FTGPIO010_va_size );
/* IIC */
EXPORT_SYMBOL( IIC_FTIIC010_pa_base );
EXPORT_SYMBOL( IIC_FTIIC010_pa_limit );
EXPORT_SYMBOL( IIC_FTIIC010_pa_size );
EXPORT_SYMBOL( IIC_FTIIC010_va_base );
EXPORT_SYMBOL( IIC_FTIIC010_va_limit );
EXPORT_SYMBOL( IIC_FTIIC010_va_size );
/* INTC */
EXPORT_SYMBOL( INTC_FTINTC010_pa_base );
EXPORT_SYMBOL( INTC_FTINTC010_pa_limit );
EXPORT_SYMBOL( INTC_FTINTC010_pa_size );
EXPORT_SYMBOL( INTC_FTINTC010_va_base );
EXPORT_SYMBOL( INTC_FTINTC010_va_limit );
EXPORT_SYMBOL( INTC_FTINTC010_va_size );
/* LCDC */
EXPORT_SYMBOL( LCDC_FTLCDC200_pa_base );
EXPORT_SYMBOL( LCDC_FTLCDC200_pa_limit );
EXPORT_SYMBOL( LCDC_FTLCDC200_pa_size );
EXPORT_SYMBOL( LCDC_FTLCDC200_va_base );
EXPORT_SYMBOL( LCDC_FTLCDC200_va_limit );
EXPORT_SYMBOL( LCDC_FTLCDC200_va_size );
/* MAC */
EXPORT_SYMBOL( MAC_FTMAC110_pa_base );
EXPORT_SYMBOL( MAC_FTMAC110_pa_limit );
EXPORT_SYMBOL( MAC_FTMAC110_pa_size );
EXPORT_SYMBOL( MAC_FTMAC110_va_base );
EXPORT_SYMBOL( MAC_FTMAC110_va_limit );
EXPORT_SYMBOL( MAC_FTMAC110_va_size );
/* NANDC */
EXPORT_SYMBOL( NANDC_FTNANDC020_pa_base );
EXPORT_SYMBOL( NANDC_FTNANDC020_pa_limit );
EXPORT_SYMBOL( NANDC_FTNANDC020_pa_size );
EXPORT_SYMBOL( NANDC_FTNANDC020_va_base );
EXPORT_SYMBOL( NANDC_FTNANDC020_va_limit );
EXPORT_SYMBOL( NANDC_FTNANDC020_va_size );
/* PCIIO */
EXPORT_SYMBOL( PCIIO_FTPCIE3914_pa_base );
EXPORT_SYMBOL( PCIIO_FTPCIE3914_pa_limit );
EXPORT_SYMBOL( PCIIO_FTPCIE3914_pa_size );
EXPORT_SYMBOL( PCIIO_FTPCIE3914_va_base );
EXPORT_SYMBOL( PCIIO_FTPCIE3914_va_limit );
EXPORT_SYMBOL( PCIIO_FTPCIE3914_va_size );
/* PCIMEM */
EXPORT_SYMBOL( PCIMEM_FTPCIE3914_pa_base );
EXPORT_SYMBOL( PCIMEM_FTPCIE3914_pa_limit );
EXPORT_SYMBOL( PCIMEM_FTPCIE3914_pa_size );
EXPORT_SYMBOL( PCIMEM_FTPCIE3914_va_base );
EXPORT_SYMBOL( PCIMEM_FTPCIE3914_va_limit );
EXPORT_SYMBOL( PCIMEM_FTPCIE3914_va_size );
/* PMU */
EXPORT_SYMBOL( PMU_FTPMU010_pa_base );
EXPORT_SYMBOL( PMU_FTPMU010_pa_limit );
EXPORT_SYMBOL( PMU_FTPMU010_pa_size );
EXPORT_SYMBOL( PMU_FTPMU010_va_base );
EXPORT_SYMBOL( PMU_FTPMU010_va_limit );
EXPORT_SYMBOL( PMU_FTPMU010_va_size );
/* RAM */
EXPORT_SYMBOL( RAM_FTDDRII020_pa_base );
EXPORT_SYMBOL( RAM_FTDDRII020_pa_limit );
EXPORT_SYMBOL( RAM_FTDDRII020_pa_size );
/* RTC */
EXPORT_SYMBOL( RTC_FTRTC010_pa_base );
EXPORT_SYMBOL( RTC_FTRTC010_pa_limit );
EXPORT_SYMBOL( RTC_FTRTC010_pa_size );
EXPORT_SYMBOL( RTC_FTRTC010_va_base );
EXPORT_SYMBOL( RTC_FTRTC010_va_limit );
EXPORT_SYMBOL( RTC_FTRTC010_va_size );
/* SCU */
EXPORT_SYMBOL( SCU_FTSCU000_pa_base );
EXPORT_SYMBOL( SCU_FTSCU000_pa_limit );
EXPORT_SYMBOL( SCU_FTSCU000_pa_size );
EXPORT_SYMBOL( SCU_FTSCU000_va_base );
EXPORT_SYMBOL( SCU_FTSCU000_va_limit );
EXPORT_SYMBOL( SCU_FTSCU000_va_size );
/* SDC */
EXPORT_SYMBOL( SDC_FTSDC010_pa_base );
EXPORT_SYMBOL( SDC_FTSDC010_pa_limit );
EXPORT_SYMBOL( SDC_FTSDC010_pa_size );
EXPORT_SYMBOL( SDC_FTSDC010_va_base );
EXPORT_SYMBOL( SDC_FTSDC010_va_limit );
EXPORT_SYMBOL( SDC_FTSDC010_va_size );
/* SMC */
EXPORT_SYMBOL( SMC_FTSMC020_pa_base );
EXPORT_SYMBOL( SMC_FTSMC020_pa_limit );
EXPORT_SYMBOL( SMC_FTSMC020_pa_size );
EXPORT_SYMBOL( SMC_FTSMC020_va_base );
EXPORT_SYMBOL( SMC_FTSMC020_va_limit );
EXPORT_SYMBOL( SMC_FTSMC020_va_size );
/* SSP */
EXPORT_SYMBOL( SSP_FTSSP010_pa_base );
EXPORT_SYMBOL( SSP_FTSSP010_pa_limit );
EXPORT_SYMBOL( SSP_FTSSP010_pa_size );
EXPORT_SYMBOL( SSP_FTSSP010_va_base );
EXPORT_SYMBOL( SSP_FTSSP010_va_limit );
EXPORT_SYMBOL( SSP_FTSSP010_va_size );
/* TIMER */
EXPORT_SYMBOL( TIMER_FTTMR010_pa_base );
EXPORT_SYMBOL( TIMER_FTTMR010_pa_limit );
EXPORT_SYMBOL( TIMER_FTTMR010_pa_size );
EXPORT_SYMBOL( TIMER_FTTMR010_va_base );
EXPORT_SYMBOL( TIMER_FTTMR010_va_limit );
EXPORT_SYMBOL( TIMER_FTTMR010_va_size );
/* UART */
EXPORT_SYMBOL( UART_FTUART010_pa_base );
EXPORT_SYMBOL( UART_FTUART010_pa_limit );
EXPORT_SYMBOL( UART_FTUART010_pa_size );
EXPORT_SYMBOL( UART_FTUART010_va_base );
EXPORT_SYMBOL( UART_FTUART010_va_limit );
EXPORT_SYMBOL( UART_FTUART010_va_size );
/* USB */
EXPORT_SYMBOL( USB_FOTG2XX_pa_base );
EXPORT_SYMBOL( USB_FOTG2XX_pa_limit );
EXPORT_SYMBOL( USB_FOTG2XX_pa_size );
EXPORT_SYMBOL( USB_FOTG2XX_va_base );
EXPORT_SYMBOL( USB_FOTG2XX_va_limit );
EXPORT_SYMBOL( USB_FOTG2XX_va_size );
EXPORT_SYMBOL( USB_FUSBH200_pa_base );
EXPORT_SYMBOL( USB_FUSBH200_pa_limit );
EXPORT_SYMBOL( USB_FUSBH200_pa_size );
EXPORT_SYMBOL( USB_FUSBH200_va_base );
EXPORT_SYMBOL( USB_FUSBH200_va_limit );
EXPORT_SYMBOL( USB_FUSBH200_va_size );
/* WDT */
EXPORT_SYMBOL( WDT_FTWDT010_pa_base );
EXPORT_SYMBOL( WDT_FTWDT010_pa_limit );
EXPORT_SYMBOL( WDT_FTWDT010_pa_size );
EXPORT_SYMBOL( WDT_FTWDT010_va_base );
EXPORT_SYMBOL( WDT_FTWDT010_va_limit );
EXPORT_SYMBOL( WDT_FTWDT010_va_size );
/* PCI */
EXPORT_SYMBOL( PCI_FTPCIE3914_pa_base );
EXPORT_SYMBOL( PCI_FTPCIE3914_pa_limit );
EXPORT_SYMBOL( PCI_FTPCIE3914_pa_size );
EXPORT_SYMBOL( PCI_FTPCIE3914_va_base );
EXPORT_SYMBOL( PCI_FTPCIE3914_va_limit );
EXPORT_SYMBOL( PCI_FTPCIE3914_va_size );
/* APBBRG */
EXPORT_SYMBOL( APBBRG_FTAPBBRG020_pa_base );
EXPORT_SYMBOL( APBBRG_FTAPBBRG020_pa_limit );
EXPORT_SYMBOL( APBBRG_FTAPBBRG020_pa_size );
EXPORT_SYMBOL( APBBRG_FTAPBBRG020_va_base );
EXPORT_SYMBOL( APBBRG_FTAPBBRG020_va_limit );
EXPORT_SYMBOL( APBBRG_FTAPBBRG020_va_size );
/* DMAC */
EXPORT_SYMBOL( DMAC_FTDMAC020_pa_base );
EXPORT_SYMBOL( DMAC_FTDMAC020_pa_limit );
EXPORT_SYMBOL( DMAC_FTDMAC020_pa_size );
EXPORT_SYMBOL( DMAC_FTDMAC020_va_base );
EXPORT_SYMBOL( DMAC_FTDMAC020_va_limit );
EXPORT_SYMBOL( DMAC_FTDMAC020_va_size );

/*
 * Interrupt Dispatch Tables, for both FIQ and IRQ
 */

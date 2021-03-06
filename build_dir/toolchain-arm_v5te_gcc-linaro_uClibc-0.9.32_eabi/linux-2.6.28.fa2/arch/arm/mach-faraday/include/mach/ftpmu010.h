/*
 *  arch/arm/mach-faraday/include/mach/ftpmu010.h
 *
 *  Copyright (C) 2009 Faraday Technology.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __FTPMU010_H
#define __FTPMU010_H

#define FTPMU010_OFFSET_IDNMBR0		0x00
#define FTPMU010_OFFSET_OSCC		0x08
#define FTPMU010_OFFSET_PMODE		0x0c
#define FTPMU010_OFFSET_PMCR		0x10
#define FTPMU010_OFFSET_PED		0x14
#define FTPMU010_OFFSET_PEDSR		0x18
#define FTPMU010_OFFSET_PMSR		0x20
#define FTPMU010_OFFSET_PGSR		0x24

#ifdef CONFIG_PLATFORM_A320
#define FTPMU010_OFFSET_MFPSR		0x28
#endif	/* CONFIG_PLATFORM_A320 */

#define FTPMU010_OFFSET_MISC		0x2c

#ifdef CONFIG_PLATFORM_A320
#define FTPMU010_OFFSET_PDLLCR0		0x30
#define FTPMU010_OFFSET_PDLLCR1		0x34
#else	/* CONFIG_PLATFORM_A320 */
#define FTPMU010_OFFSET_PDLLCR		0x30
#define FTPMU010_OFFSET_AHBMCLKOFF	0x38
#define FTPMU010_OFFSET_APBMCLKOFF	0x3c
#define FTPMU010_OFFSET_DCSRCR0		0x40
#define FTPMU010_OFFSET_DCSRCR1		0x44
#define FTPMU010_OFFSET_DCSRCR2		0x48
#define FTPMU010_OFFSET_SDRAMHTC	0x4c
#endif	/* CONFIG_PLATFORM_A320 */

#define FTPMU010_OFFSET_PSPR0		0x50
#define FTPMU010_OFFSET_PSPR1		0x54
#define FTPMU010_OFFSET_PSPR2		0x58
#define FTPMU010_OFFSET_PSPR3		0x5c
#define FTPMU010_OFFSET_PSPR4		0x60
#define FTPMU010_OFFSET_PSPR5		0x64
#define FTPMU010_OFFSET_PSPR6		0x68
#define FTPMU010_OFFSET_PSPR7		0x6c
#define FTPMU010_OFFSET_PSPR8		0x70
#define FTPMU010_OFFSET_PSPR9		0x74
#define FTPMU010_OFFSET_PSPR10		0x78
#define FTPMU010_OFFSET_PSPR11		0x7c
#define FTPMU010_OFFSET_PSPR12		0x80
#define FTPMU010_OFFSET_PSPR13		0x84
#define FTPMU010_OFFSET_PSPR14		0x88
#define FTPMU010_OFFSET_PSPR15		0x8c

#ifdef CONFIG_PLATFORM_A320
#define FTPMU010_OFFSET_AHBDMA_RACCS	0x90
#endif	/* CONFIG_PLATFORM_A320 */

#define FTPMU010_OFFSET_JSS		0x9c

#ifdef CONFIG_PLATFORM_A320
#define FTPMU010_OFFSET_CFC_RACC	0xa0
#define FTPMU010_OFFSET_SSP1_RACC	0xa4
#define FTPMU010_OFFSET_UART1TX_RACC	0xa8
#define FTPMU010_OFFSET_UART1RX_RACC	0xac
#define FTPMU010_OFFSET_UART2TX_RACC	0xb0
#define FTPMU010_OFFSET_UART2RX_RACC	0xb4
#define FTPMU010_OFFSET_SDC_RACC	0xb8
#define FTPMU010_OFFSET_I2SAC97_RACC	0xbc
#define FTPMU010_OFFSET_IRDATX_RACC	0xc0
#define FTPMU010_OFFSET_USBD_RACC	0xc8
#define FTPMU010_OFFSET_IRDARX_RACC	0xcc
#define FTPMU010_OFFSET_IRDA_RACC	0xd0
#define FTPMU010_OFFSET_ED0_RACC	0xd4
#define FTPMU010_OFFSET_ED1_RACC	0xd8
#endif	/* CONFIG_PLATFORM_A320 */

/*
 * ID Number 0 Register
 */
#define FTPMU010_ID_A320A	0x03200000
#define FTPMU010_ID_A320C	0x03200010
#define FTPMU010_ID_A320D	0x03200030
#define FTPMU010_ID_A360	0x00003360

/*
 * OSC Control Register
 */
#ifdef CONFIG_PLATFORM_A320

#define FTPMU010_OSCC_OSCH_TRI		(1 << 11)
#define FTPMU010_OSCC_OSCH_STABLE	(1 << 9)
#define FTPMU010_OSCC_OSCH_OFF		(1 << 8)
#define FTPMU010_OSCC_OSCL_TRI		(1 << 3)
#define FTPMU010_OSCC_OSCL_RTCLSEL	(1 << 2)
#define FTPMU010_OSCC_OSCL_STABLE	(1 << 1)
#define FTPMU010_OSCC_OSCL_OFF		(1 << 0)

#else	/* CONFIG_PLATFORM_A320 */

#define FTPMU010_OSCC_SOSC_STABLE	(1 << 9)
#define FTPMU010_OSCC_SOSC_ENABLE	(1 << 8)
#define FTPMU010_OSCC_ROSC_STABLE	(1 << 1)

#endif	/* CONFIG_PLATFORM_A320 */

/*
 * Power Mode Register
 */
#ifdef CONFIG_PLATFORM_A320

#define FTPMU010_PMODE_DIVAHBCLK_MASK	(0x7 << 4)
#define FTPMU010_PMODE_DIVAHBCLK_2	(0x0 << 4)
#define FTPMU010_PMODE_DIVAHBCLK_3	(0x1 << 4)
#define FTPMU010_PMODE_DIVAHBCLK_4	(0x2 << 4)
#define FTPMU010_PMODE_DIVAHBCLK_6	(0x3 << 4)
#define FTPMU010_PMODE_DIVAHBCLK_8	(0x4 << 4)
#define FTPMU010_PMODE_DIVAHBCLK(pmode)	(((pmode) >> 4) & 0x7)

#endif	/* CONFIG_PLATFORM_A320 */

#define FTPMU010_PMODE_FCS		(1 << 2)
#define FTPMU010_PMODE_TURBO		(1 << 1)
#define FTPMU010_PMODE_SLEEP		(1 << 0)

/*
 * Power Manager Status Register
 */
#define FTPMU010_PMSR_SMR	(1 << 10)

#define FTPMU010_PMSR_RDH	(1 << 2)
#define FTPMU010_PMSR_PH	(1 << 1)
#define FTPMU010_PMSR_CKEHLOW	(1 << 0)

#ifdef CONFIG_PLATFORM_A320

/*
 * Multi-Function Port Setting Register
 */
#define FTPMU010_MFPSR_MODEMPINSEL	(1 << 14)
#define FTPMU010_MFPSR_AC97CLKOUTSEL	(1 << 13)
#define FTPMU010_MFPSR_AC97PINSEL	(1 << 3)

/*
 * PLL/DLL Control Register 0
 */
#define FTPMU010_PDLLCR0_HCLKOUTDIS(cr0)	(((cr0) >> 20) & 0xf)
#define FTPMU010_PDLLCR0_DLLFRAG		(1 << 19)
#define FTPMU010_PDLLCR0_DLLSTSEL		(1 << 18)
#define FTPMU010_PDLLCR0_DLLSTABLE		(1 << 17)
#define FTPMU010_PDLLCR0_DLLDIS			(1 << 16)
#define FTPMU010_PDLLCR0_PLL1NS(cr0)		(((cr0) >> 3) & 0x1ff)
#define FTPMU010_PDLLCR0_PLL1STSEL		(1 << 2)
#define FTPMU010_PDLLCR0_PLL1STABLE		(1 << 1)
#define FTPMU010_PDLLCR0_PLL1DIS		(1 << 0)

#else	/* CONFIG_PLATFORM_A320 */

/*
 * PLL/DLL Control Register
 */
#define FTPMU010_PDLLCR_DLLSTABLE		(1 << 2)
#define FTPMU010_PDLLCR_DLLDIS			(1 << 21)
#define FTPMU010_PDLLCR_PLLMS(cr)		(((cr) >> 11) & 0xf)
#define FTPMU010_PDLLCR_PLLNS(cr)		(((cr) >> 3) & 0x3f)
#define FTPMU010_PDLLCR_PLLSTABLE		(1 << 1)
#define FTPMU010_PDLLCR_PLLDIS			(1 << 0)

#endif	/* CONFIG_PLATFORM_A320 */

unsigned int ftpmu010_get_hclk(void __iomem *base);
unsigned int ftpmu010_get_pclk(void __iomem *base);
unsigned int ftpmu010_get_cpuclk(void __iomem *base);
int ftpmu010_in_turbo_mode(void __iomem *base);

#endif	/* __FTPMU010_H */

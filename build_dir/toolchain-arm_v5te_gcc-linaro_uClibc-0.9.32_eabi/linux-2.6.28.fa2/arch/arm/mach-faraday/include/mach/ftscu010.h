/*
 *  arch/arm/mach-faraday/include/mach/ftscu010.h
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

#ifndef __FTSCU010_H
#define __FTSCU010_H

#define	FTSCU010_OFFSET_ID		0x00
#define	FTSCU010_OFFSET_VER		0x04
#define	FTSCU010_OFFSET_CSR		0x08
#define	FTSCU010_OFFSET_MFCR		0x0c
#define	FTSCU010_OFFSET_CR		0x10
#define	FTSCU010_OFFSET_SR		0x14
#define	FTSCU010_OFFSET_GPIO_SLEEP	0x18
#define	FTSCU010_OFFSET_OSCC		0x1c
#define	FTSCU010_OFFSET_PLL1CR		0x20
#define	FTSCU010_OFFSET_DLL		0x24
#define	FTSCU010_OFFSET_HCLKGATE	0x28
#define	FTSCU010_OFFSET_PCLKGATE	0x2c
#define	FTSCU010_OFFSET_SPR0		0x100
#define	FTSCU010_OFFSET_SPR1		0x104
#define	FTSCU010_OFFSET_SPR2		0x108
#define	FTSCU010_OFFSET_SPR3		0x10c
#define	FTSCU010_OFFSET_SPR4		0x110
#define	FTSCU010_OFFSET_SPR5		0x114
#define	FTSCU010_OFFSET_SPR6		0x118
#define	FTSCU010_OFFSET_SPR7		0x11c
#define	FTSCU010_OFFSET_SPR8		0x120
#define	FTSCU010_OFFSET_SPR9		0x124
#define	FTSCU010_OFFSET_SPR10		0x128
#define	FTSCU010_OFFSET_SPR11		0x12c
#define	FTSCU010_OFFSET_SPR12		0x130
#define	FTSCU010_OFFSET_SPR13		0x134
#define	FTSCU010_OFFSET_SPR14		0x138
#define	FTSCU010_OFFSET_SPR15		0x13c
#define	FTSCU010_OFFSET_GC		0x200
#define	FTSCU010_OFFSET_EXT_CS		0x204
#define	FTSCU010_OFFSET_DMA0_REQ_CFG0	0x208
#define	FTSCU010_OFFSET_DMA0_REQ_CFG1	0x20c
#define	FTSCU010_OFFSET_DMA1_REQ_CFG0	0x210
#define	FTSCU010_OFFSET_DMA1_REQ_CFG1	0x214
#define	FTSCU010_OFFSET_DMA_ACK_CFG0	0x218
#define	FTSCU010_OFFSET_DMA_ACK_CFG1	0x21c
#define	FTSCU010_OFFSET_DMA_ACK_CFG2	0x220
#define	FTSCU010_OFFSET_DMA_ACK_CFG3	0x224
#define	FTSCU010_OFFSET_SCLK_CFG0	0x228
#define	FTSCU010_OFFSET_SCLK_CFG1	0x22c
#define	FTSCU010_OFFSET_SCLK_EN		0x230
#define	FTSCU010_OFFSET_EDG_SYN_CTRL	0x234
#define	FTSCU010_OFFSET_MFPSR0		0x238
#define	FTSCU010_OFFSET_MFPSR1		0x23c
#define	FTSCU010_OFFSET_DCSRCR0		0x240
#define	FTSCU010_OFFSET_DCSRCR1		0x244
#define	FTSCU010_OFFSET_DBG_CFG		0x248
#define	FTSCU010_OFFSET_DLY_CTRL	0x254
#define	FTSCU010_OFFSET_PWR_CTRL	0x258
#define	FTSCU010_OFFSET_SATA_DBG	0x25c

/*
 * Configuration Strap Register
 */
#define FTSCU010_CSR_OSCH_CNT_DIS	(1 << 0)
#define FTSCU010_CSR_PLL_DIS		(1 << 1)
#define FTSCU010_CSR_DLL_DIS		(1 << 2)
#define FTSCU010_CSR_CPU_M_FCLK(csr)	(((csr) >> 3) & 0x3)
#define FTSCU010_CSR_PLL_NS(csr)	(((csr) >> 5) & 0x3f)

/*
 * PLL1 Control Register
 */
#define FTSCU010_PLL1CR_PLL_DIS		(1 << 0)
#define FTSCU010_PLL1CR_PLL_STABLE	(1 << 1)
#define FTSCU010_PLL1CR_PLL_NS(cr)	(((cr) >> 24) & 0x3f)

/*
 * Multi-Function Pin Setting Register 0
 */
#define FTSCU010_MFPSR0_FGEXTAHB_EXTAHB		(0x0 << 0)
#define FTSCU010_MFPSR0_FGEXTAHB_PERIPHERALS	(0x1 << 0)
#define FTSCU010_MFPSR0_FGSATA_EXTAHB		(0x0 << 2)
#define FTSCU010_MFPSR0_FGSATA_SATA		(0x1 << 2)
#define FTSCU010_MFPSR0_FGSATA_EXTDMA		(0x2 << 2)
#define FTSCU010_MFPSR0_FGISP_EXTAHB		(0x0 << 4)
#define FTSCU010_MFPSR0_FGISP_GPIO1		(0x1 << 4)
#define FTSCU010_MFPSR0_FGISP_ISP		(0x2 << 4)
#define FTSCU010_MFPSR0_FGTS_EXTAHB		(0x0 << 6)
#define FTSCU010_MFPSR0_FGTS_GPIO0		(0x1 << 6)
#define FTSCU010_MFPSR0_FGTS_TS			(0x2 << 6)
#define FTSCU010_MFPSR0_FGLCD_LCD		(0x0 << 8)
#define FTSCU010_MFPSR0_FGLCD_TV_MICE		(0x1 << 8)
#define FTSCU010_MFPSR0_FGLCD_LCD_MICE		(0x2 << 8)
#define FTSCU010_MFPSR0_FGEBI_EBI		(0x0 << 10)
#define FTSCU010_MFPSR0_FGEBI_IDE		(0x1 << 10)

/*
 * Multi-Function Pin Setting Register 1
 */
#define FTSCU010_MFPSR1_FGUART1_EXTAHB		(0x0 << 0)
#define FTSCU010_MFPSR1_FGUART1_GPIO0		(0x1 << 0)
#define FTSCU010_MFPSR1_FGUART1_UART1		(0x2 << 0)
#define FTSCU010_MFPSR1_FGUART2_GPIO1		(0x0 << 2)
#define FTSCU010_MFPSR1_FGUART2_PWM		(0x1 << 2)
#define FTSCU010_MFPSR1_FGUART2_UART2		(0x2 << 2)
#define FTSCU010_MFPSR1_FGUART3_GPIO1		(0x0 << 4)
#define FTSCU010_MFPSR1_FGUART3_UART3		(0x2 << 4)
#define FTSCU010_MFPSR1_FGSSP0_SSP0		(0x0 << 6)
#define FTSCU010_MFPSR1_FGSSP0_SATA		(0x1 << 6)
#define FTSCU010_MFPSR1_FGSSP0_SICE		(0x2 << 6)
#define FTSCU010_MFPSR1_FGSSP1_GPIO1		(0x0 << 8)
#define FTSCU010_MFPSR1_FGSSP1_I2C1		(0x1 << 8)
#define FTSCU010_MFPSR1_FGSSP1_SSP1		(0x2 << 8)
#define FTSCU010_MFPSR1_FGGMAC_GMAC		(0x0 << 10)
#define FTSCU010_MFPSR1_FGGMAC_MCP_ICE		(0x1 << 10)
#define FTSCU010_MFPSR1_FGPWM0_EXTAHB		(0x0 << 12)
#define FTSCU010_MFPSR1_FGPWM0_PWM		(0x1 << 12)
#define FTSCU010_MFPSR1_FGPWM0_EXTDMA		(0x2 << 12)
#define FTSCU010_MFPSR1_FGPWM1_EXTAHB		(0x0 << 14)
#define FTSCU010_MFPSR1_FGPWM1_GPIO1		(0x1 << 14)
#define FTSCU010_MFPSR1_FGPWM1_PWM		(0x2 << 14)
#define FTSCU010_MFPSR1_FGI2C1_EXTAHB		(0x0 << 16)
#define FTSCU010_MFPSR1_FGI2C1_GPIO1		(0x1 << 16)
#define FTSCU010_MFPSR1_FGI2C1_I2C1		(0x2 << 16)
#define FTSCU010_MFPSR1_FGGPIO0_EXTAHB		(0x0 << 18)
#define FTSCU010_MFPSR1_FGGPIO0_GPIO0		(0x1 << 18)
#define FTSCU010_MFPSR1_FGGPIO0_EXTDMA		(0x2 << 18)
#define FTSCU010_MFPSR1_FGKBC_EXTAHB		(0x0 << 20)
#define FTSCU010_MFPSR1_FGKBC_GPIO0		(0x1 << 20)
#define FTSCU010_MFPSR1_FGKBC_KBC		(0x2 << 20)

unsigned int ftscu010_get_dclk(void __iomem *base);
unsigned int ftscu010_get_mclk(void __iomem *base);
unsigned int ftscu010_get_hclk(void __iomem *base);
unsigned int ftscu010_get_pclk(void __iomem *base);
unsigned int ftscu010_get_mcpuclk(void __iomem *base);

void ftscu010_pinmux_setup(void __iomem *base);

#endif	/* __FTSCU010_H */

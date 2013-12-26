/*
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
#include <linux/kernel.h>
#include <asm/io.h>

#include <mach/ftscu010.h>

/******************************************************************************
 * clock functions
 *****************************************************************************/
static unsigned int ftscu010_get_pllout_g(void __iomem *base)
{
	unsigned int pll1cr = readl(base + FTSCU010_OFFSET_PLL1CR);
	unsigned int ns = FTSCU010_PLL1CR_PLL_NS(pll1cr);

	return MAIN_CLK * ns / 2;
}

unsigned int ftscu010_get_dclk(void __iomem *base)
{
	return ftscu010_get_pllout_g(base) / 2;
}

unsigned int ftscu010_get_mclk(void __iomem *base)
{
	return ftscu010_get_pllout_g(base) / 3;
}

unsigned int ftscu010_get_hclk(void __iomem *base)
{
	return ftscu010_get_pllout_g(base) / 4;
}

unsigned int ftscu010_get_pclk(void __iomem *base)
{
	return ftscu010_get_pllout_g(base) / 8;
}

unsigned int ftscu010_get_mcpuclk(void __iomem *base)
{
	unsigned int csr = readl(base + FTSCU010_OFFSET_CSR);
	int select = FTSCU010_CSR_CPU_M_FCLK(csr);

	switch(select) {
	case 0:
		return ftscu010_get_hclk(base);

	case 1:
		return ftscu010_get_pclk(base);

	default:
		return ftscu010_get_pllout_g(base);
	}
}

/******************************************************************************
 * pin mux setup
 *
 * This shit is used to handle the brain-dead pin mux design.
 *****************************************************************************/
void ftscu010_pinmux_setup(void __iomem *base)
{
	unsigned int mfpsr0 = 0;
	unsigned int mfpsr1 = 0;

	/*
	 * Function group EXTAHB
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGEXTAHB_EXTAHB)
	mfpsr0 |= FTSCU010_MFPSR0_FGEXTAHB_EXTAHB;
#elif defined(CONFIG_FTSCU010_PINMUX_FGEXTAHB_PERIPHERALS)
	mfpsr0 |= FTSCU010_MFPSR0_FGEXTAHB_PERIPHERALS;
#else
#error Missing function group EXTAHB setting
#endif

	/*
	 * Function group SATA
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGSATA_EXTAHB)
	mfpsr0 |= FTSCU010_MFPSR0_FGSATA_EXTAHB;
#elif defined(CONFIG_FTSCU010_PINMUX_FGSATA_SATA)
	mfpsr0 |= FTSCU010_MFPSR0_FGSATA_SATA;
#elif defined(CONFIG_FTSCU010_PINMUX_FGSATA_EXTDMA)
	mfpsr0 |= FTSCU010_MFPSR0_FGSATA_EXTDMA;
#else
#error Missing function group SATA setting
#endif

	/*
	 * Function group ISP
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGISP_EXTAHB)
	mfpsr0 |= FTSCU010_MFPSR0_FGISP_EXTAHB;
#elif defined(CONFIG_FTSCU010_PINMUX_FGISP_GPIO1)
	mfpsr0 |= FTSCU010_MFPSR0_FGISP_GPIO1;
#elif defined(CONFIG_FTSCU010_PINMUX_FGISP_ISP)
	mfpsr0 |= FTSCU010_MFPSR0_FGISP_ISP;
#else
#error Missing function group ISP setting
#endif

	/*
	 * Function group TS
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGTS_EXTAHB)
	mfpsr0 |= FTSCU010_MFPSR0_FGTS_EXTAHB;
#elif defined(CONFIG_FTSCU010_PINMUX_FGTS_GPIO0)
	mfpsr0 |= FTSCU010_MFPSR0_FGTS_GPIO0;
#elif defined(CONFIG_FTSCU010_PINMUX_FGTS_TS)
	mfpsr0 |= FTSCU010_MFPSR0_FGTS_TS;
#else
#error Missing function group TS setting
#endif

	/*
	 * Function group LCD
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGLCD_LCD)
	mfpsr0 |= FTSCU010_MFPSR0_FGLCD_LCD;
#elif defined(CONFIG_FTSCU010_PINMUX_FGLCD_TV_MICE)
	mfpsr0 |= FTSCU010_MFPSR0_FGLCD_TV_MICE;
#elif defined(CONFIG_FTSCU010_PINMUX_FGLCD_LCD_MICE)
	mfpsr0 |= FTSCU010_MFPSR0_FGLCD_LCD_MICE;
#else
#error Missing function group LCD setting
#endif

	/*
	 * Function group EBI
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGEBI_EBI)
	mfpsr0 |= FTSCU010_MFPSR0_FGEBI_EBI;
#elif defined(CONFIG_FTSCU010_PINMUX_FGEBI_IDE)
	/* NOR Flash disabled */
	mfpsr0 |= FTSCU010_MFPSR0_FGEBI_IDE;
#else
#error Missing function group EBI setting
#endif

	/*
	 * Function group UART1
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGUART1_EXTAHB)
	mfpsr1 |= FTSCU010_MFPSR1_FGUART1_EXTAHB;
#elif defined(CONFIG_FTSCU010_PINMUX_FGUART1_GPIO0)
	mfpsr1 |= FTSCU010_MFPSR1_FGUART1_GPIO0;
#elif defined (CONFIG_FTSCU010_PINMUX_FGUART1_UART1)
	mfpsr1 |= FTSCU010_MFPSR1_FGUART1_UART1;
#else
#error Missing function group UART1 setting
#endif

	/*
	 * Function group UART2
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGUART2_GPIO1)
	mfpsr1 |= FTSCU010_MFPSR1_FGUART2_GPIO1;
#elif defined(CONFIG_FTSCU010_PINMUX_FGUART2_PWM2)
	mfpsr1 |= FTSCU010_MFPSR1_FGUART2_PWM;
#elif defined(CONFIG_FTSCU010_PINMUX_FGUART2_UART2)
	mfpsr1 |= FTSCU010_MFPSR1_FGUART2_UART2;
#else
#error Missing function group UART2 setting
#endif

	/*
	 * Function group UART3
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGUART3_GPIO1)
	mfpsr1 |= FTSCU010_MFPSR1_FGUART3_GPIO1;
#elif defined(CONFIG_FTSCU010_PINMUX_FGUART3_UART3)
	mfpsr1 |= FTSCU010_MFPSR1_FGUART3_UART3;
#else
#error Missing function group UART3 setting
#endif

	/*
	 * Function group SSP0
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGSSP0_SSP0)
	mfpsr1 |= FTSCU010_MFPSR1_FGSSP0_SSP0;
#elif defined(CONFIG_FTSCU010_PINMUX_FGSSP0_SATA)
	mfpsr1 |= FTSCU010_MFPSR1_FGSSP0_SATA;
#elif defined(CONFIG_FTSCU010_PINMUX_FGSSP0_SICE)
	mfpsr1 |= FTSCU010_MFPSR1_FGSSP0_SICE;
#else
#error Missing function group SSP0 setting
#endif

	/*
	 * Function group SSP1
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGSSP1_GPIO1)
	mfpsr1 |= FTSCU010_MFPSR1_FGSSP1_GPIO1;
#elif defined(CONFIG_FTSCU010_PINMUX_FGSSP1_I2C1)
	mfpsr1 |= FTSCU010_MFPSR1_FGSSP1_I2C1;
#elif defined(CONFIG_FTSCU010_PINMUX_FGSSP1_SSP1)
	mfpsr1 |= FTSCU010_MFPSR1_FGSSP1_SSP1;
#else
#error Missing function group setting
#endif

	/*
	 * Function group GMAC
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGGMAC_GMAC)
	mfpsr1 |= FTSCU010_MFPSR1_FGGMAC_GMAC;
#elif defined(CONFIG_FTSCU010_PINMUX_FGGMAC_MCP_ICE)
	mfpsr1 |= FTSCU010_MFPSR1_FGGMAC_MCP_ICE;
#else
#error Missing function group setting
#endif

	/*
	 * Function group PWM0
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGPWM0_EXTAHB)
	mfpsr1 |= FTSCU010_MFPSR1_FGPWM0_EXTAHB;
#elif defined(CONFIG_FTSCU010_PINMUX_FGPWM0_PWM0)
	mfpsr1 |= FTSCU010_MFPSR1_FGPWM0_PWM;
#elif defined(CONFIG_FTSCU010_PINMUX_FGPWM0_EXTDMA)
	mfpsr1 |= FTSCU010_MFPSR1_FGPWM0_EXTDMA;
#else
#error Missing function group setting
#endif

	/*
	 * Function group PWM1
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGPWM1_EXTAHB)
	mfpsr1 |= FTSCU010_MFPSR1_FGPWM1_EXTAHB;
#elif defined(CONFIG_FTSCU010_PINMUX_FGPWM1_GPIO1)
	mfpsr1 |= FTSCU010_MFPSR1_FGPWM1_GPIO1;
#elif defined(CONFIG_FTSCU010_PINMUX_FGPWM1_PWM1)
	mfpsr1 |= FTSCU010_MFPSR1_FGPWM1_PWM;
#else
#error Missing function group setting
#endif

	/*
	 * Function group I2C1
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGI2C1_EXTAHB)
	mfpsr1 |= FTSCU010_MFPSR1_FGI2C1_EXTAHB;
#elif defined(CONFIG_FTSCU010_PINMUX_FGI2C1_GPIO1)
	mfpsr1 |= FTSCU010_MFPSR1_FGI2C1_GPIO1;
#elif defined(CONFIG_FTSCU010_PINMUX_FGI2C1_I2C1)
	mfpsr1 |= FTSCU010_MFPSR1_FGI2C1_I2C1;
#else
#error Missing function group setting
#endif

	/*
	 * Function group GPIO0
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGGPIO0_EXTAHB)
	mfpsr1 |= FTSCU010_MFPSR1_FGGPIO0_EXTAHB;
#elif defined(CONFIG_FTSCU010_PINMUX_FGGPIO0_GPIO0)
	mfpsr1 |= FTSCU010_MFPSR1_FGGPIO0_GPIO0;
#elif defined(CONFIG_FTSCU010_PINMUX_FGGPIO0_EXTDMA)
	mfpsr1 |= FTSCU010_MFPSR1_FGGPIO0_EXTDMA;
#else
#error Missing function group setting
#endif

	/*
	 * Function group KBC
	 */
#if defined(CONFIG_FTSCU010_PINMUX_FGKBC_EXTAHB)
	mfpsr1 |= FTSCU010_MFPSR1_FGKBC_EXTAHB;
#elif defined(CONFIG_FTSCU010_PINMUX_FGKBC_GPIO0)
	mfpsr1 |= FTSCU010_MFPSR1_FGKBC_GPIO0;
#elif defined(CONFIG_FTSCU010_PINMUX_FGKBC_KBC)
	mfpsr1 |= FTSCU010_MFPSR1_FGKBC_KBC;
#else
#error Missing function group setting
#endif

	printk(KERN_INFO "FTSCU010: [MFPSR0] = %08x, [MFPSR1] = %08x\n", mfpsr0, mfpsr1);
	writel(mfpsr0, base + FTSCU010_OFFSET_MFPSR0);
	writel(mfpsr1, base + FTSCU010_OFFSET_MFPSR1);
}


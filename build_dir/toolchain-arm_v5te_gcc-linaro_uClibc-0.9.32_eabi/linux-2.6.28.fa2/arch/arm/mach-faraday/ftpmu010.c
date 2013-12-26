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

#include <mach/ftpmu010.h>

int ftpmu010_in_turbo_mode(void __iomem *base)
{
	unsigned int pmode = readl(base + FTPMU010_OFFSET_PMODE);

	return pmode & FTPMU010_PMODE_TURBO;
}

#ifdef CONFIG_PLATFORM_A320
static const unsigned int bits2div[] = { 2, 3, 4, 6, 8, };

static unsigned int ftpmu010_get_divahbclk(void __iomem *base)
{
	unsigned int pmode = readl(base + FTPMU010_OFFSET_PMODE);
	unsigned int divbits = FTPMU010_PMODE_DIVAHBCLK(pmode);

	if (divbits > 4) {
		printk(KERN_ERR "unknown DIVAHBCLK %d\n", divbits);
		return 8;
	}

	return bits2div[divbits];
}

static unsigned int ftpmu010_get_pll1_freq(void __iomem *base)
{
	unsigned int freq = MAIN_CLK;
	unsigned int pdllcr0 = readl(base + FTPMU010_OFFSET_PDLLCR0);

	if (!(pdllcr0 & FTPMU010_PDLLCR0_PLL1DIS)) {
		unsigned int mul = FTPMU010_PDLLCR0_PLL1NS(pdllcr0);

		freq *= mul;
	}

	return freq;
}

unsigned int ftpmu010_get_hclk(void __iomem *base)
{
	unsigned int freq = ftpmu010_get_pll1_freq(base);
	unsigned int div = ftpmu010_get_divahbclk(base);

	return freq / div;
}

unsigned int ftpmu010_get_pclk(void __iomem *base)
{
	return ftpmu010_get_hclk(base) / 2;
}

unsigned int ftpmu010_get_cpuclk(void __iomem *base)
{
	if (ftpmu010_in_turbo_mode(base))
		return ftpmu010_get_pll1_freq(base);

	return ftpmu010_get_hclk(base);
}
#else	/* CONFIG_PLATFORM_A320 */
static unsigned int ftpmu010_get_pll_freq(void __iomem *base)
{
	unsigned int pdllcr = readl(base + FTPMU010_OFFSET_PDLLCR);
	unsigned int mul = FTPMU010_PDLLCR_PLLNS(pdllcr);
	unsigned int div = FTPMU010_PDLLCR_PLLMS(pdllcr);

	return MAIN_CLK * mul / div;
}

unsigned int ftpmu010_get_hclk(void __iomem *base)
{
	return ftpmu010_get_pll_freq(base) / 8;
}

unsigned int ftpmu010_get_pclk(void __iomem *base)
{
	return ftpmu010_get_pll_freq(base) / 16;
}
#endif	/* CONFIG_PLATFORM_A320 */

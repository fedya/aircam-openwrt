/*
 *  Platform depdenent initialization
 *
 *  Copyright (C) 2009 Faraday Corp. (http://www.faraday-tech.com)
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

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach-types.h>
#include <asm/mach/time.h>

#include <linux/platform_device.h>

#include <mach/spec.h>
#include <mach/ftintc010.h>
#include <mach/ftpwmtmr010.h>
#include <mach/ftscu010.h>

/******************************************************************************
 * platform devices
 *****************************************************************************/

/*
 * RTC011
 */
static struct resource ftrtc011_resources[] = {
	{
		.start	= RTC_FTRTC011_0_PA_BASE,
		.end	= RTC_FTRTC011_0_PA_LIMIT,
		.flags	= IORESOURCE_MEM,
	}, {	/* alarm interrupt */
		.start	= RTC_FTRTC011_0_IRQ0,
		.end	= RTC_FTRTC011_0_IRQ0,
		.flags	= IORESOURCE_IRQ,
	}, {	/* periodic interrupt */
		.start	= RTC_FTRTC011_0_IRQ1,
		.end	= RTC_FTRTC011_0_IRQ1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ftrtc011_device = {
	.name		= "ftrtc011",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(ftrtc011_resources),
	.resource	= ftrtc011_resources,
};

/*
 * GMAC100
 */
static struct resource ftgmac100_resources[] = {
	{
		.start	= MAC_FTGMAC100_0_PA_BASE,
		.end	= MAC_FTGMAC100_0_PA_LIMIT,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= MAC_FTGMAC100_0_IRQ,
		.end	= MAC_FTGMAC100_0_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ftgmac100_device = {
	.name		= "ftgmac100",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(ftgmac100_resources),
	.resource	= ftgmac100_resources,
};

/*
 * LCDC200
 */
static struct resource ftlcdc200_resources[] = {
	{
		.start	= LCDC_FTLCDC200_0_PA_BASE,
		.end	= LCDC_FTLCDC200_0_PA_LIMIT,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= LCDC_FTLCDC200_0_IRQ0,
		.end	= LCDC_FTLCDC200_0_IRQ0,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ftlcdc200_device = {
	.name		= "ftlcdc200",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(ftlcdc200_resources),
	.resource	= ftlcdc200_resources,
};

/*
 * FTTSC010
 *
 * Note: Sytlus interrupt of FTTSC010 on A369 is broken.
 */
static struct resource fttsc010_resources[] = {
	{
		.start  = ADC_FTTSC010_0_PA_BASE,
		.end    = ADC_FTTSC010_0_PA_LIMIT,
		.flags  = IORESOURCE_MEM,
	}, {	/* ADC interrupt */
		.start	= ADC_FTTSC010_0_IRQ0,
		.end	= ADC_FTTSC010_0_IRQ0,
		.flags	= IORESOURCE_IRQ,
	}, {	/* stylus interrupt */
		.start	= ADC_FTTSC010_0_IRQ1,
		.end	= ADC_FTTSC010_0_IRQ1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device fttsc010_device = {
	.name		= "fttsc010",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(fttsc010_resources),
	.resource	= fttsc010_resources,
};

/*
 * FTKBC010
 */
static struct resource ftkbc010_resources[] = {
	{
		.start  = KBC_FTKBC010_0_PA_BASE,
		.end    = KBC_FTKBC010_0_PA_LIMIT,
		.flags  = IORESOURCE_MEM,
	}, {
		.start	= KBC_FTKBC010_0_IRQ,
		.end	= KBC_FTKBC010_0_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ftkbc010_device = {
	.name		= "ftkbc010",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(ftkbc010_resources),
	.resource	= ftkbc010_resources,
};

static struct platform_device *a369_devices[] __initdata = {
	&ftrtc011_device,
	&ftgmac100_device,
	&ftlcdc200_device,
	&fttsc010_device,
	&ftkbc010_device,
};

/******************************************************************************
 * clock functions
 *****************************************************************************/
unsigned int a369_get_ahb_clk(void)
{
	void __iomem *base = (void __iomem *)SCU_FTSCU010_VA_BASE;

	return ftscu010_get_hclk(base);
}
EXPORT_SYMBOL(a369_get_ahb_clk);

unsigned int a369_get_apb_clk(void)
{
	void __iomem *base = (void __iomem *)SCU_FTSCU010_VA_BASE;

	return ftscu010_get_pclk(base);
}
EXPORT_SYMBOL(a369_get_apb_clk);

static unsigned int a369_get_cpu_clk(void)
{
	void __iomem *base = (void __iomem *)SCU_FTSCU010_VA_BASE;

	return ftscu010_get_mcpuclk(base);
}

/******************************************************************************
 * timer - clockevent and clocksource
 *****************************************************************************/
static struct ftpwmtmr010_clockevent ftpwmtmr010_0_clockevent = {
	.clockevent = {
		.name	= "ftpwmtmr010:0",
		.irq	= TIMER_FTPWMTMR010_0_IRQ0,
	},
	.base	= (void __iomem *)TIMER_FTPWMTMR010_0_VA_BASE,
	.id	= 0,
};

static struct ftpwmtmr010_clocksource ftpwmtmr010_1_clocksource = {
	.clocksource = {
		.name	= "ftpwmtmr010:1",
	},
	.base	= (void __iomem *)TIMER_FTPWMTMR010_0_VA_BASE,
	.id	= 1,
};

static void __init a369_sys_timer_init(void)
{
	unsigned int cpuclk = a369_get_cpu_clk();
	unsigned int hclk = a369_get_ahb_clk();
	unsigned int pclk = a369_get_apb_clk();

	printk(KERN_INFO "CPU: %d Hz, HCLK: %d Hz, PCLK: %d Hz\n", cpuclk, hclk, pclk);

	ftpwmtmr010_0_clockevent.freq = pclk;
	ftpwmtmr010_clockevent_init(&ftpwmtmr010_0_clockevent);

	ftpwmtmr010_1_clocksource.freq = pclk;
	ftpwmtmr010_clocksource_init(&ftpwmtmr010_1_clocksource);
}

struct sys_timer a369_sys_timer = {
	.init	= a369_sys_timer_init,
};

/******************************************************************************
 * platform dependent functions
 *****************************************************************************/
extern const struct map_desc platform_io_desc[PLATFORM_IO_DESC_NR];

static void __init platform_map_io(void)
{
	iotable_init((struct map_desc*)platform_io_desc, ARRAY_SIZE(platform_io_desc));
}

void __iomem *ftintc010_base_addr;

static void __init a369_init_irq(void)
{
	struct ftintc010_trigger_type master_trigger_type = {
		.irqmode	= PLATFORM_IRQ_TRIGGER_MODE & 0xffffffff,
		.irqlevel	= ~(PLATFORM_IRQ_TRIGGER_LEVEL & 0xffffffff),
		.fiqmode	= PLATFORM_FIQ_TRIGGER_MODE,
		.fiqlevel	= ~PLATFORM_FIQ_TRIGGER_LEVEL,
		.irqmodeex	= PLATFORM_IRQ_TRIGGER_MODE >> 32,
		.irqlevelex	= ~(PLATFORM_IRQ_TRIGGER_LEVEL >> 32),
	};

	/*
	 * initialize primary interrupt controller
	 */
	ftintc010_base_addr = __io(INTC_FTINTC020_0_VA_BASE);

	ftintc010_init(0, ftintc010_base_addr, 0, &master_trigger_type);
}

static void __init a369_board_init(void)
{
	void __iomem *base = (void __iomem *)SCU_FTSCU010_VA_BASE;

	ftscu010_pinmux_setup(base);

	platform_add_devices(a369_devices, ARRAY_SIZE(a369_devices));
}

MACHINE_START(FARADAY, PLATFORM_NAME)
	.phys_io	= PLATFORM_BOOTTIME_IO_PA_BASE,
	.io_pg_offst	= ((PLATFORM_BOOTTIME_IO_VA_BASE) >> 20) << 2,
	.boot_params	= BOOT_PARAMETER_PA_BASE,
	.map_io		= platform_map_io,
	.init_irq	= a369_init_irq,
	.timer		= &a369_sys_timer,
	.init_machine	= a369_board_init,
MACHINE_END

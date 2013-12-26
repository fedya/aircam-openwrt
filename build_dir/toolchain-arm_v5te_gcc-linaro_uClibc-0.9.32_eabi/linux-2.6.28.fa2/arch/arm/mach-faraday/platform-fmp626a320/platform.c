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

#include <asm/hardware/gic.h>

#include <mach/spec.h>
#include <mach/ftintc010.h>
#include <mach/fttmr010.h>
#include <mach/ftpci100.h>
#include <mach/ftpmu010.h>

/******************************************************************************
 * platform devices
 *****************************************************************************/

/*
 * RTC010
 */
static struct resource ftrtc010_resources[] = {
	{
		.start	= RTC_FTRTC010_0_PA_BASE,
		.end	= RTC_FTRTC010_0_PA_LIMIT,
		.flags	= IORESOURCE_MEM,
	}, {		/* alarm interrupt */
		.start	= RTC_FTRTC010_0_IRQ0,
		.end	= RTC_FTRTC010_0_IRQ0,
		.flags	= IORESOURCE_IRQ,
	}, {		/* periodic interrupt */
		.start	= RTC_FTRTC010_0_IRQ1,
		.end	= RTC_FTRTC010_0_IRQ1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ftrtc010_device = {
	.name		= "ftrtc010",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(ftrtc010_resources),
	.resource	= ftrtc010_resources,
};

/*
 * MAC100
 */
static struct resource ftmac100_0_resources[] = {
	{
		.start	= MAC_FTMAC100_0_PA_BASE,
		.end	= MAC_FTMAC100_0_PA_LIMIT,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= MAC_FTMAC100_0_IRQ,
		.end	= MAC_FTMAC100_0_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ftmac100_0_device = {
	.name		= "ftmac100",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(ftmac100_0_resources),
	.resource	= ftmac100_0_resources,
};

#ifdef CONFIG_PLATFORM_FIA321
static struct resource ftmac100_1_resources[] = {
	{
		.start	= MAC_FTMAC100_1_PA_BASE,
		.end	= MAC_FTMAC100_1_PA_LIMIT,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= MAC_FTMAC100_1_IRQ,
		.end	= MAC_FTMAC100_1_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ftmac100_1_device = {
	.name		= "ftmac100",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(ftmac100_1_resources),
	.resource	= ftmac100_1_resources,
};
#endif

static struct platform_device *a320_devices[] __initdata = {
	&ftrtc010_device,
	&ftmac100_0_device,
#ifdef CONFIG_PLATFORM_FIA321
	&ftmac100_1_device,
#endif
};

/******************************************************************************
 * clock functions
 *****************************************************************************/
unsigned int a320_get_ahb_clk(void)
{
	void __iomem *base = (void __iomem *)PMU_FTPMU010_VA_BASE;

	return ftpmu010_get_hclk(base);
}
EXPORT_SYMBOL(a320_get_ahb_clk);

unsigned int a320_get_apb_clk(void)
{
	void __iomem *base = (void __iomem *)PMU_FTPMU010_VA_BASE;

	return ftpmu010_get_pclk(base);
}
EXPORT_SYMBOL(a320_get_apb_clk);

/******************************************************************************
 * timer - clockevent and clocksource
 *****************************************************************************/
static struct fttmr010_clockevent fttmr010_0_clockevent = {
	.clockevent = {
		.name	= "fttmr010:0",
		.irq	= TIMER_FTTMR010_0_IRQ0,
	},
	.base	= (void __iomem *)TIMER_FTTMR010_0_VA_BASE,
	.id	= 0,
};

static struct fttmr010_clocksource fttmr010_1_clocksource = {
	.clocksource = {	
		.name	= "fttmr010:1",
	},
	.base	= (void __iomem *)TIMER_FTTMR010_0_VA_BASE,
	.id	= 1,
};

#ifdef CONFIG_LOCAL_TIMERS
extern void __iomem *twd_base_addr;
extern unsigned int twd_size;
#endif

static void __init a320_sys_timer_init(void)
{
#ifdef CONFIG_LOCAL_TIMERS
	twd_base_addr = __io(PLATFORM_TWD_VA_BASE);
	twd_size = PLATFORM_TWD_SIZE;
#endif

	fttmr010_0_clockevent.freq = a320_get_apb_clk();
	fttmr010_clockevent_init(&fttmr010_0_clockevent);

	fttmr010_1_clocksource.freq = a320_get_apb_clk();
	fttmr010_clocksource_init(&fttmr010_1_clocksource);
}

struct sys_timer a320_sys_timer = {
	.init	= a320_sys_timer_init,
};

/******************************************************************************
 * platform dependent functions
 *****************************************************************************/
extern const struct map_desc platform_io_desc[PLATFORM_IO_DESC_NR];

static void __init platform_map_io(void)
{
        iotable_init((struct map_desc*)platform_io_desc, ARRAY_SIZE(platform_io_desc));
#ifdef CONFIG_FTPCI100
	ftpci100_init(0, __io(PCIC_FTPCI100_0_VA_BASE));
#endif
}

void __iomem *gic_cpu_base_addr;

static void __init a320_init_irq(void)
{
	struct ftintc010_trigger_type master_trigger_type = {
		.irqmode	= PLATFORM_IRQ_TRIGGER_MODE,
		.irqlevel	= ~PLATFORM_IRQ_TRIGGER_LEVEL,
		.fiqmode	= PLATFORM_FIQ_TRIGGER_MODE,
		.fiqlevel	= ~PLATFORM_FIQ_TRIGGER_LEVEL,
	};
#ifdef CONFIG_PLATFORM_FIA321
	struct ftintc010_trigger_type slave_trigger_type = {
		.irqmode	= PLATFORM_FIA321_IRQ_TRIGGER_MODE,
		.irqlevel	= ~PLATFORM_FIA321_IRQ_TRIGGER_LEVEL,
		.fiqmode	= PLATFORM_FIA321_FIQ_TRIGGER_MODE,
		.fiqlevel	= ~PLATFORM_FIA321_FIQ_TRIGGER_LEVEL,
	};
#endif

	/*
	 * initialize primary interrupt controller
	 */
	gic_dist_init(0, __io(PLATFORM_GIC_DIST_VA_BASE), IRQ_LOCALTIMER);

	gic_cpu_base_addr = __io(PLATFORM_GIC_CPU_VA_BASE);
	gic_cpu_init(0, gic_cpu_base_addr);

	/*
	 * initialize second tier interrupt controller
	 */
	ftintc010_init(0, __io(INTC_FTINTC010_0_VA_BASE), 0, &master_trigger_type);
	ftintc010_cascade_irq(0, PLATFORM_LEGACY_IRQ);

#ifdef CONFIG_PLATFORM_FIA321
	/*
	 * initialize third tier interrupt controller
	 */
	ftintc010_init(1, __io(INTC_FTINTC010_1_VA_BASE),
			PLATFORM_FIA321_IRQ_BASE, &slave_trigger_type);

	ftintc010_cascade_irq(1, PLATFORM_FIA321_IRQ);

#ifdef CONFIG_FTPCI100
	/*
	 * initialize fourth tier interrupt controller
	 * because all 4 irqs of FTPCI100 are connected to the same pin of A321
	 * we are compelled to make FTPCI100 be a third tier interrupt controller
	 */
	ftpci100_int_init(0, PLATFORM_FIA321_PCI_IRQ_BASE);

	ftpci100_int_cascade_irq(0, PLATFORM_FIA321_IRQ_BASE + PLATFORM_FIA321_PCI_IRQ);
#endif
#endif
}

static void __init a320_board_init(void)
{
	platform_add_devices(a320_devices, ARRAY_SIZE(a320_devices));
}

MACHINE_START(FARADAY, PLATFORM_NAME)
	.phys_io	= PLATFORM_BOOTTIME_IO_PA_BASE,
	.io_pg_offst	= ((PLATFORM_BOOTTIME_IO_VA_BASE) >> 20) << 2,
	.boot_params	= BOOT_PARAMETER_PA_BASE,
	.map_io		= platform_map_io,
	.init_irq	= a320_init_irq,
	.timer		= &a320_sys_timer,
	.init_machine	= a320_board_init,
MACHINE_END

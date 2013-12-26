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
#include <mach/fttmr010.h>
#include <mach/ftintc010.h>

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

static void __init axi_sys_timer_init(void)
{
	fttmr010_0_clockevent.freq = CLOCK_TICK_RATE;
	fttmr010_clockevent_init(&fttmr010_0_clockevent);

	fttmr010_1_clocksource.freq = CLOCK_TICK_RATE;
	fttmr010_clocksource_init(&fttmr010_1_clocksource);
}

struct sys_timer axi_sys_timer = {
	.init	= axi_sys_timer_init,
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

static void __init axi_init_irq(void)
{
	struct ftintc010_trigger_type master_trigger_type = {
		.irqmode	= PLATFORM_IRQ_TRIGGER_MODE,
		.irqlevel	= ~PLATFORM_IRQ_TRIGGER_LEVEL,
		.fiqmode	= PLATFORM_FIQ_TRIGGER_MODE,
		.fiqlevel	= ~PLATFORM_FIQ_TRIGGER_LEVEL,
	};

	/*
	 * initialize primary interrupt controller
	 */
	ftintc010_base_addr = __io(INTC_FTINTC010_0_VA_BASE);

	ftintc010_init(0, ftintc010_base_addr, 0, &master_trigger_type);
}


MACHINE_START(FARADAY, PLATFORM_NAME)
	.phys_io	= PLATFORM_BOOTTIME_IO_PA_BASE,
	.io_pg_offst	= ((PLATFORM_BOOTTIME_IO_VA_BASE) >> 20) << 2,
	.boot_params	= BOOT_PARAMETER_PA_BASE,
	.map_io		= platform_map_io,
	.init_irq	= axi_init_irq,
	.timer		= &axi_sys_timer,
MACHINE_END

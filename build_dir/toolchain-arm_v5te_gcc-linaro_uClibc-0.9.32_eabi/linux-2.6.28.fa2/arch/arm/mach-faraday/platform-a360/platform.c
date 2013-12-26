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
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>

#include <mach/spec.h>
#include <mach/ftintc010.h>
#include <mach/fttmr010.h>
#include <mach/ftpmu010.h>
#include <mach/ftscu000.h>

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
 * SSP010 SPI controller
 */
static struct resource ftssp010_spi_resources[] = {
	{
		.start	= SSP_FTSSP010_1_PA_BASE,
		.end	= SSP_FTSSP010_1_PA_LIMIT,
		.flags	= IORESOURCE_MEM,
	}, {
		.start	= SSP_FTSSP010_1_IRQ,
		.end	= SSP_FTSSP010_1_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device ftssp010_spi_controller = {
	.name		= "ftssp010_spi",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(ftssp010_spi_resources),
	.resource	= ftssp010_spi_resources,
};

static struct platform_device *a360_devices[] __initdata = {
	&ftrtc010_device,
	&ftssp010_spi_controller,
};

/******************************************************************************
 * SPI devices
 *****************************************************************************/
static const struct flash_platform_data a360_spi_slave_data;

static struct spi_board_info a360_spi_info[] = {
	{
		.modalias	= "w25p16",
		.platform_data	= &a360_spi_slave_data,
		.max_speed_hz	= 50 * 1000 * 1000,
		.bus_num	= 0,
		.chip_select	= 0,
		.mode		= SPI_MODE_0,
	}
};

/******************************************************************************
 * clock functions
 *****************************************************************************/
unsigned int a360_get_ahb_clk(void)
{
	void __iomem *base = (void __iomem *)PMU_FTPMU010_VA_BASE;

	return ftpmu010_get_hclk(base);
}
EXPORT_SYMBOL(a360_get_ahb_clk);

unsigned int a360_get_apb_clk(void)
{
	void __iomem *base = (void __iomem *)PMU_FTPMU010_VA_BASE;

	return ftpmu010_get_pclk(base);
}
EXPORT_SYMBOL(a360_get_apb_clk);

static unsigned int a360_get_cpu_clk(void)
{
	void __iomem *pmu = (void __iomem *)PMU_FTPMU010_VA_BASE;
	void __iomem *scu = (void __iomem *)SCU_FTSCU000_VA_BASE;
	unsigned int hclk;

	hclk = ftpmu010_get_hclk(pmu);

	if (ftpmu010_in_turbo_mode(pmu)) {
		if (ftscu000_in_half_turbo_mode(scu)) {
			return hclk * 2;
		} else {
			return hclk * 4;
		}
	}

	return hclk;
}

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

static void __init a360_sys_timer_init(void)
{
	unsigned int cpuclk = a360_get_cpu_clk();
	unsigned int hclk = a360_get_ahb_clk();
	unsigned int pclk = a360_get_apb_clk();

	printk(KERN_INFO "CPU: %d Hz, HCLK: %d Hz, PCLK: %d Hz\n", cpuclk, hclk, pclk);

	fttmr010_0_clockevent.freq = pclk;
	fttmr010_clockevent_init(&fttmr010_0_clockevent);

	fttmr010_1_clocksource.freq = pclk;
	fttmr010_clocksource_init(&fttmr010_1_clocksource);
}

struct sys_timer a360_sys_timer = {
	.init	= a360_sys_timer_init,
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

static void __init a360_init_irq(void)
{
	struct ftintc010_trigger_type master_trigger_type = {
		.irqmode	= PLATFORM_IRQ_TRIGGER_MODE,
		.irqlevel	= ~PLATFORM_IRQ_TRIGGER_LEVEL,
		.fiqmode	= PLATFORM_FIQ_TRIGGER_MODE,
		.fiqlevel	= ~PLATFORM_FIQ_TRIGGER_LEVEL,
		.irqmodeex	= PLATFORM_IRQ_TRIGGER_MODE >> 32,
		.irqlevelex	= ~(PLATFORM_IRQ_TRIGGER_LEVEL >> 32),
	};

	/*
	 * initialize primary interrupt controller
	 */
	ftintc010_base_addr = __io(INTC_FTINTC010_0_VA_BASE);

	ftintc010_init(0, ftintc010_base_addr, 0, &master_trigger_type);
}

static void __init a360_board_init(void)
{
	/*
	 * spi_register_board_info() must be called before
	 * platform_device_register() / platform_add_devices()
	 */
	spi_register_board_info(a360_spi_info, ARRAY_SIZE(a360_spi_info));

	platform_add_devices(a360_devices, ARRAY_SIZE(a360_devices));
}

MACHINE_START(FARADAY, PLATFORM_NAME)
	.phys_io	= PLATFORM_BOOTTIME_IO_PA_BASE,
	.io_pg_offst	= ((PLATFORM_BOOTTIME_IO_VA_BASE) >> 20) << 2,
	.boot_params	= BOOT_PARAMETER_PA_BASE,
	.map_io		= platform_map_io,
	.init_irq	= a360_init_irq,
	.timer		= &a360_sys_timer,
	.init_machine	= a360_board_init,
MACHINE_END

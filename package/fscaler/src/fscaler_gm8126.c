/**
 * @file fscaler_gm8126.c
 * @brief GM8181 series scaler platform depenent routines' implementations
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#include "fscaler_module.h"
#include <linux/bitops.h>
#include <asm/io.h>
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,16))
#include <linux/io.h>
#include <mach/ftpmu010.h>
#endif /* LINUX_VERSION_CODE */
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,14))
#include <mach/spec.h>
#else
#include <asm/arch/platform/spec.h>
#include <asm/arch/spec.h>
#endif /* LINUX_VERSION_CODE */
#include "fscaler_platform.h"

#define PMU_VA_BASE PMU_FTPMU010_0_VA_BASE
#define SCAL_PMU_OFS    0x38
#define SCAL_CLK_PMU_OFS 0x74

typedef struct ip_info {
    int nr;
} ip_info_t;

ip_info_t fscal_ipinfo[] = {
    [0] = {.nr = 14,},
};

int fscal_max = sizeof(fscal_ipinfo) / sizeof(ip_info_t);

#define SCAL_PLL_DIV_MASK  0x0000F000

static int pmu_fd = -1;

static pmuReg_t pmu_reg_8126[] = {
    {0x38, (0x1 << 14), (0x1 << 14), 0x0, 0x0},
    {0x74, (0xF << 12), (0xF << 12), 0x0, 0x0},
};

static pmuRegInfo_t pmu_reg_info_8126 = {
    "SCALER",
    ARRAY_SIZE(pmu_reg_8126),
    ATTR_TYPE_PLL2,
    &pmu_reg_8126[0]
};

static int platform_clock_divide_value(int clk)
{
    //int div, pll = PLL2_CLK_IN;
    int div = 0, pll = ftpmu010_get_attr(ATTR_TYPE_PLL2);
    div = pll / clk / 1000000;
    if (div * clk * 1000000 < pll) {
        ++div;
    }
    if (div <= 0) {
        printk("###### Warning: div is negative, force set 1\n");
        div = 1;
    }
    if (div > 16) {
        printk("###### Warning: div is greater than 16, force set 16\n");
        div = 16;
    }
    return div;
}

/**
 * @function void platform_power(int ip, int b_power)
 * @brief Trun on/off driver's clock from PMU
 * @param id Scaler device index
 * @param b_power Power on/off
 * @return none
 */
void platform_power(int ip, int b_power)
{
    //unsigned long val;
    int div = 2;
    if (ip >= fscal_max) {
        return;
    }

    pmu_fd = ftpmu010_register_reg(&pmu_reg_info_8126);
    if (pmu_fd < 0)
        panic("scaler register pmu fail!");

    div = platform_clock_divide_value(pmu_clock);
    printk("SCL: div:%d\n", div);

    //val = ioread32(PMU_VA_BASE + SCAL_CLK_PMU_OFS);
    //val = ftpmu010_read_reg(SCAL_CLK_PMU_OFS);

    //val &= ~SCAL_PLL_DIV_MASK;
    //val |= (--div) << 12;

    //iowrite32(val, PMU_VA_BASE + SCAL_CLK_PMU_OFS);
    ftpmu010_write_reg(pmu_fd, SCAL_CLK_PMU_OFS, ((--div) << 12), (0xF << 12));

    //val = ioread32(PMU_VA_BASE + SCAL_PMU_OFS);
    if (b_power) {
        //clear_bit(fscal_ipinfo[ip].nr, &val);
        ftpmu010_write_reg(pmu_fd, SCAL_PMU_OFS, 0, (0x1 << 14));
    } else {
        //set_bit(fscal_ipinfo[ip].nr, &val);
        ftpmu010_write_reg(pmu_fd, SCAL_PMU_OFS, (0x1 << 14), (0x1 << 14));
    }
    //iowrite32(val, PMU_VA_BASE + SCAL_PMU_OFS);
}

EXPORT_SYMBOL(platform_power);

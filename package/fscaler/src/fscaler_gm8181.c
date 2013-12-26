/**
 * @file fscaler_gm8181.c
 * @brief GM8181 series scaler platform depenent routines' implementations
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/bitops.h>
#include <asm/io.h>
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,16))
#include <linux/io.h>
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
    [0] = {.nr = 22,},
};

int fscal_max = sizeof(fscal_ipinfo) / sizeof(ip_info_t);

#define GM_PMU_MASK         0x0000f000
#define GM8181_PMU_VAL      0x00001000
#define GM8181T_PMU_VAL     0x00002000

#define SCAL_PLL3_DIV_MASK  0x0000F000

#define GMUNKNOW (int)(0)
#define GM8181  (int)(8181)
#define GM8181T (int)(8181 + (int)'T')

static int platform_check(int ip)
{
    unsigned long val = ioread32(PMU_VA_BASE);
    if ((val & GM_PMU_MASK) == GM8181_PMU_VAL) {
        return GM8181;
    }
    if ((val & GM_PMU_MASK) == GM8181T_PMU_VAL) {
        return GM8181T;
    }
    return GMUNKNOW;
}

static int platform_clock_divide_value(int clk)
{
    int div, pll3 = PLL3_CLK_IN;
    div = pll3 / clk / 1000000;
    if (div * clk * 1000000 < pll3) {
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
    unsigned long val;
    int type, div = 2;
    if (ip >= fscal_max) {
        return;
    }
    type = platform_check(ip);
    switch (type) {
    case GM8181:
        div = platform_clock_divide_value(162);
        break;
    default:
    case GM8181T:
        div = platform_clock_divide_value(216);
        break;
    }
    val = ioread32(PMU_VA_BASE + SCAL_CLK_PMU_OFS);
    val &= ~SCAL_PLL3_DIV_MASK;
    val |= (--div) << 12;
    iowrite32(val, PMU_VA_BASE + SCAL_CLK_PMU_OFS);
    val = ioread32(PMU_VA_BASE + SCAL_PMU_OFS);
    if (b_power)
        clear_bit(fscal_ipinfo[ip].nr, &val);
    else
        set_bit(fscal_ipinfo[ip].nr, &val);
    iowrite32(val, PMU_VA_BASE + SCAL_PMU_OFS);
}

EXPORT_SYMBOL(platform_power);

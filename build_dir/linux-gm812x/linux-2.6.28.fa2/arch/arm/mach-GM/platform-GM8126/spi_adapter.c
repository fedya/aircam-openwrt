#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <asm/io.h>
#include <mach/spec.h>
#include <mach/ftpmu010.h>
#include <mach/platform/gpio.h>
#include <mach/ftssp010_spi.h>
#include <mach/hardware.h>

// SPI0 PMU handler
int spi_0_fd = -1;

// SPI1 PMU handler
int spi_1_fd = -1;

/**
 * SPI0 PMU init table
*/
static pmuReg_t reg_SPI_0_array[] = {
 /* CLK init */
 /* reg_off  bit_masks  lock_bits     init_val    init_mask */
    {0x78,  (0x3F << 0), (0x3F << 0), (0x06 << 0), (0x3F << 0)},
    {0x3C,  (0x1 << 5),  (0x1 << 5),  (0x0 << 5),  (0x1 << 5)},
    {0x50,  (0x3F << 26),(0x3F << 26),(0x15 << 26),(0x3F << 26)}, 
#if defined(CONFIG_SPI_FTSSP010_CS0) || defined(CONFIG_SPI_FTSSP010_CS1) 
    {0x54,           0,         0,          0,          0},
#endif
#if defined(CONFIG_SPI_FTSSP010_CS2) || defined(CONFIG_SPI_FTSSP010_CS3) || defined(CONFIG_SPI_FTSSP010_CS4) || defined(CONFIG_SPI_FTSSP010_CS5)
    {0x60,           0,         0,          0,          0},
#endif
};

static pmuRegInfo_t spi_0_clk_pinmux_info = {
    "spi_0_clk_pinmux", 
    ARRAY_SIZE(reg_SPI_0_array),
    ATTR_TYPE_PLL2, /* use PLL2 source */
    reg_SPI_0_array
};

/**
 * SPI1 PMU init table
*/
static pmuReg_t reg_SPI_1_array[] = {
 /* CLK init */
 /* reg_off  bit_masks  lock_bits     init_val    init_mask */
    {0x78,  (0x3F << 6), (0x3F << 6), (0x06 << 6), (0x3F << 6)},
    {0x3C,  (0x01 << 6), (0x1  << 6), (0x0  << 6), (0x1  << 6)},
    //{0x60,  (0x1FF << 0), (0x1FF << 0), (0x48 << 0), (0x1FF << 0)}, 
    {0x60,  (0xFFF << 0), (0xFFF << 0), (0x248 << 0), (0xFFF << 0)}, 
};

static pmuRegInfo_t spi_1_clk_pinmux_info = {
    "spi_1_clk_pinmux", 
    ARRAY_SIZE(reg_SPI_1_array),
    ATTR_TYPE_PLL2, /* use PLL2 source */
    reg_SPI_1_array
};

// chip select descriptors of SSP0 
static struct ftssp010_spi_cs_descriptor cs_descriptors_0[FTSSP010_SPI_CS_NR_0];

// chip select descriptors of SSP1 
static struct ftssp010_spi_cs_descriptor cs_descriptors_1[FTSSP010_SPI_CS_NR_1];

/**
 * start to implement SPI0 fixup callback functions
 */

static void spi_0_pmu_init(struct ftssp010_spi_hw_platform *hw_platform)
{
    u16 i = 0;
    u32 ssp0clk_pvalue = 0;

    /* register SPI to pmu core */
    for (i = 0; i < ARRAY_SIZE(reg_SPI_0_array) ; ++i) {
        if (spi_0_clk_pinmux_info.pRegArray[i].reg_off == 0x54) {
#ifdef CONFIG_SPI_FTSSP010_CS0
            spi_0_clk_pinmux_info.pRegArray[i].bits_mask |= (0x3 << 4);
            spi_0_clk_pinmux_info.pRegArray[i].lock_bits |= (0x3 << 4); 
            spi_0_clk_pinmux_info.pRegArray[i].init_val  |= (0x0 << 4);
            spi_0_clk_pinmux_info.pRegArray[i].init_mask |= (0x3 << 4);
#endif
#ifdef CONFIG_SPI_FTSSP010_CS1
            spi_0_clk_pinmux_info.pRegArray[i].bits_mask |= (0x3 << 6);
            spi_0_clk_pinmux_info.pRegArray[i].lock_bits |= (0x3 << 6); 
            spi_0_clk_pinmux_info.pRegArray[i].init_val  |= (0x0 << 6);
            spi_0_clk_pinmux_info.pRegArray[i].init_mask |= (0x3 << 6);
#endif
        }

        if (spi_0_clk_pinmux_info.pRegArray[i].reg_off == 0x60) {
#ifdef CONFIG_SPI_FTSSP010_CS2
            spi_0_clk_pinmux_info.pRegArray[i].bits_mask |= (0x7 << 6);
            spi_0_clk_pinmux_info.pRegArray[i].lock_bits |= (0x7 << 6); 
            spi_0_clk_pinmux_info.pRegArray[i].init_val  |= (0x0 << 0);
            spi_0_clk_pinmux_info.pRegArray[i].init_mask |= (0x7 << 0);
#endif
#ifdef CONFIG_SPI_FTSSP010_CS3
            spi_0_clk_pinmux_info.pRegArray[i].bits_mask |= (0x7 << 3);
            spi_0_clk_pinmux_info.pRegArray[i].lock_bits |= (0x7 << 3); 
            spi_0_clk_pinmux_info.pRegArray[i].init_val  |= (0x0 << 3);
            spi_0_clk_pinmux_info.pRegArray[i].init_mask |= (0x7 << 3);
#endif
#ifdef CONFIG_SPI_FTSSP010_CS4
            spi_0_clk_pinmux_info.pRegArray[i].bits_mask |= (0x7 << 6);
            spi_0_clk_pinmux_info.pRegArray[i].lock_bits |= (0x7 << 6); 
            spi_0_clk_pinmux_info.pRegArray[i].init_val  |= (0x0 << 6);
            spi_0_clk_pinmux_info.pRegArray[i].init_mask |= (0x7 << 6);
#endif
#ifdef CONFIG_SPI_FTSSP010_CS5
            spi_0_clk_pinmux_info.pRegArray[i].bits_mask |= (0x7 << 9);
            spi_0_clk_pinmux_info.pRegArray[i].lock_bits |= (0x7 << 9); 
            spi_0_clk_pinmux_info.pRegArray[i].init_val  |= (0x0 << 9);
            spi_0_clk_pinmux_info.pRegArray[i].init_mask |= (0x7 << 9);
#endif
        }
    }
    
    spi_0_fd = ftpmu010_register_reg(&spi_0_clk_pinmux_info);
    if (unlikely(spi_0_fd < 0)){
        panic("In %s: SPI 0 registers to PMU fail! \n", __func__);
    } 
    // read current SPI0 working clock, NOTE: the working of SSP on 8126 can not be over 81MHz due to HW limitation
    ssp0clk_pvalue = (ftpmu010_read_reg(0x78) & 0x3F);
    hw_platform->working_clk = (PLL2_CLK_IN/(ssp0clk_pvalue + 1));

    //printk("PLL2_CLK_IN = %d\n", PLL2_CLK_IN);
    //printk("ssp0clk_pvalue = %d\n", ssp0clk_pvalue);
    //printk("SPI0 working clock = %d\n", hw_platform->working_clk);
}

static void spi_1_pmu_init(struct ftssp010_spi_hw_platform *hw_platform)
{
    u32 ssp1clk_pvalue = 0;

    spi_1_fd = ftpmu010_register_reg(&spi_1_clk_pinmux_info);
    if (unlikely(spi_1_fd < 0)){
        panic("In %s: SPI 1 registers to PMU fail! \n", __func__);
    } 
    
    // read current SPI1 working clock, NOTE: the working of SSP on 8126 can not be over 81MHz due to HW limitation
    ssp1clk_pvalue = (ftpmu010_read_reg(0x78) & (0x3F<<6)) >> 6;
    hw_platform->working_clk = (PLL2_CLK_IN/(ssp1clk_pvalue + 1));

    //printk("ssp1clk_pvalue = %d\n", ssp1clk_pvalue);
    //printk("SPI1 working clock = %d\n", hw_platform->working_clk);
}

static int spi_cs_preconfig_0(void)
{
#ifdef CONFIG_SPI_FTSSP010_CS0
    cs_descriptors_0[0].cs = gm_gpio_pin_index(0, 14);
    cs_descriptors_0[0].enable = 1;
#endif
#ifdef CONFIG_SPI_FTSSP010_CS1
    cs_descriptors_0[1].cs = gm_gpio_pin_index(0, 15);
    cs_descriptors_0[1].enable = 1;
#endif
#ifdef CONFIG_SPI_FTSSP010_CS2
    cs_descriptors_0[2].cs = gm_gpio_pin_index(0, 19);
    cs_descriptors_0[2].enable = 1;
#endif
#ifdef CONFIG_SPI_FTSSP010_CS3
    cs_descriptors_0[3].cs = gm_gpio_pin_index(0, 20);
    cs_descriptors_0[3].enable = 1;
#endif
#ifdef CONFIG_SPI_FTSSP010_CS4
    cs_descriptors_0[4].cs = gm_gpio_pin_index(0, 21);
    cs_descriptors_0[4].enable = 1;
#endif
#ifdef CONFIG_SPI_FTSSP010_CS5
    cs_descriptors_0[5].cs = gm_gpio_pin_index(0, 22);
    cs_descriptors_0[5].enable = 1;
#endif

    return 0;
}

static int spi_cs_preconfig_1(void)
{
    /* SSP1 only has one chip select, note!! SSP1's CS is the same as SSP0's CS2, don't use them at the same time */
    cs_descriptors_1[0].cs = gm_gpio_pin_index(0, 19);
    cs_descriptors_1[0].enable = 1;

    return 0;
}

static int spi_cs_request_0(void)
{
    int ret = -1;
    int spi_cs = -1;
    int i = 0;

    for (i = 0; i < FTSSP010_SPI_CS_NR_0; ++i) {
        if (cs_descriptors_0[i].enable) {
            spi_cs = cs_descriptors_0[i].cs;

            ret = gpio_request(spi_cs, "");
            if (unlikely(ret < 0)) {
                FTSSP010_SPI_PRINT("%s fails: gpio_request %d not OK\n", __func__, spi_cs);
                return -1;
            }

            ret = gpio_direction_output(spi_cs, 1);
            if (unlikely(ret < 0)) {
                FTSSP010_SPI_PRINT
                    ("%s fails: gpio_direction_output %d not OK\n", __func__, spi_cs);
                return -1;
            }
        }
    }

    return 0;
}

static int spi_cs_request_1(void)
{
    int ret = -1;
    int spi_cs = -1;

    if (cs_descriptors_1[0].enable) {
        spi_cs = cs_descriptors_1[0].cs;

        ret = gpio_request(spi_cs, "");
        if (unlikely(ret < 0)) {
            FTSSP010_SPI_PRINT("%s fails: gpio_request %d not OK\n", __func__, spi_cs);
            return -1;
        }

        ret = gpio_direction_output(spi_cs, 1);
        if (unlikely(ret < 0)) {
            FTSSP010_SPI_PRINT
                ("%s fails: gpio_direction_output %d not OK\n", __func__, spi_cs);
            return -1;
        }
    }

    return 0;
}

static int spi_cs_high_0(enum FTSSP010_SPI_CS i)
{
    if (unlikely(cs_descriptors_0[i].enable == 0)) {
        FTSSP010_SPI_PRINT("%s fail: this CS(%d) not enabled\n", __func__, i);
        return -1;
    }

    gpio_set_value(cs_descriptors_0[i].cs, 1);

    return 0;
}

static int spi_cs_low_0(enum FTSSP010_SPI_CS i)
{
    if (unlikely(cs_descriptors_0[i].enable == 0)) {
        FTSSP010_SPI_PRINT("%s fail: this CS(%d) not enabled\n", __func__, i);
        return -1;
    }

    gpio_set_value(cs_descriptors_0[i].cs, 0);

    return 0;
}

static int spi_cs_high_all_0(void)
{
    int i = 0;

    for (i = 0; i < FTSSP010_SPI_CS_NR_0; ++i) {
        if (cs_descriptors_0[i].enable) {
            if (unlikely(spi_cs_high_0(i) < 0)) {
                FTSSP010_SPI_PRINT("%s fails\n", __func__);
                return -1;
            }
        }
    }

    return 0;
}

static int spi_cs_init_0(void)
{
    int ret = -1;

    ret = spi_cs_preconfig_0();
    if (unlikely(ret < 0)) {
        FTSSP010_SPI_PRINT("%s fails: spi_cs_preconfig not OK\n", __func__);
        return ret;
    }

    ret = spi_cs_request_0();
    if (unlikely(ret < 0)) {
        FTSSP010_SPI_PRINT("%s fails: spi_cs_request not OK\n", __func__);
        return ret;
    }

    ret = spi_cs_high_all_0();
    if (unlikely(ret < 0)) {
        FTSSP010_SPI_PRINT("%s fails: spi_cs_high_all not OK\n", __func__);
        return ret;
    }

    return 0;
}

static int spi_hw_init_0(struct ftssp010_spi_hw_platform *hw_platform)
{
    int ret = -1;

    hw_platform->nr_chip_select = FTSSP010_SPI_CS_NR_0;
    hw_platform->cs_high = spi_cs_high_0;
    hw_platform->cs_low = spi_cs_low_0;
   
    spi_0_pmu_init(hw_platform);

    ret = spi_cs_init_0();
    if (unlikely(ret < 0)) {
        FTSSP010_SPI_PRINT("%s fails: spi_cs_init_0 not OK\n", __func__);
        return -1;
    }

    return 0;
}


static int spi_cs_high_1(enum FTSSP010_SPI_CS i)
{
    if (unlikely(cs_descriptors_1[i].enable == 0)) {
        FTSSP010_SPI_PRINT("%s fail: this CS(%d) not enabled\n", __func__, i);
        return -1;
    }

    gpio_set_value(cs_descriptors_1[i].cs, 1);

    return 0;
}

static int spi_cs_low_1(enum FTSSP010_SPI_CS i)
{
    if (unlikely(cs_descriptors_1[i].enable == 0)) {
        FTSSP010_SPI_PRINT("%s fail: this CS(%d) not enabled\n", __func__, i);
        return -1;
    }

    gpio_set_value(cs_descriptors_1[i].cs, 0);

    return 0;
}

static int spi_cs_high_all_1(void)
{
    if (cs_descriptors_1[0].enable) {
        if (unlikely(spi_cs_high_1(0) < 0)) {
            FTSSP010_SPI_PRINT("%s fails\n", __func__);
            return -1;
        }
    }

    return 0;
}

static int spi_cs_init_1(void)
{
    int ret = -1;

    ret = spi_cs_preconfig_1();
    if (unlikely(ret < 0)) {
        FTSSP010_SPI_PRINT("%s fails: spi_cs_preconfig not OK\n", __func__);
        return ret;
    }

    ret = spi_cs_request_1();
    if (unlikely(ret < 0)) {
        FTSSP010_SPI_PRINT("%s fails: spi_cs_request not OK\n", __func__);
        return ret;
    }

    ret = spi_cs_high_all_1();
    if (unlikely(ret < 0)) {
        FTSSP010_SPI_PRINT("%s fails: spi_cs_high_all not OK\n", __func__);
        return ret;
    }

    return 0;
}

static int spi_hw_init_1(struct ftssp010_spi_hw_platform *hw_platform)
{
    int ret = -1;

    hw_platform->nr_chip_select = FTSSP010_SPI_CS_NR_1;
    hw_platform->cs_high = spi_cs_high_1;
    hw_platform->cs_low = spi_cs_low_1;
   
    spi_1_pmu_init(hw_platform);

    ret = spi_cs_init_1();
    if (unlikely(ret < 0)) {
        FTSSP010_SPI_PRINT("%s fails: spi_cs_init_0 not OK\n", __func__);
        return -1;
    }

    return 0;
}

/**
 * this function can only be called once for one controller_id, prevent double init!!
 */
struct ftssp010_spi_hw_platform *ftssp010_spi_get_hw_platform(u8 controller_id)
{
    struct ftssp010_spi_hw_platform *hw_platform = NULL;
    
    hw_platform = kzalloc(sizeof(struct ftssp010_spi_hw_platform), GFP_KERNEL);
    if (!hw_platform) {
        panic("In %s: alloc fail.\n", __func__);    
    }
    
    // currently, only SSP0 and SSP1 can use SPI driver
    if (controller_id == 0) {
        spi_hw_init_0(hw_platform);        
    } else if (controller_id == 1) {
        spi_hw_init_1(hw_platform);        
    } 

    return hw_platform;
}

void ftssp010_spi_free_hw_platform(struct ftssp010_spi_hw_platform *hw_platform)
{
    if (!hw_platform) {
        kfree(hw_platform);
    }
    
    hw_platform = NULL;
}

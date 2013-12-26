#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <mach/spec.h>
#include <linux/platform_device.h>
#include <mach/ftpmu010.h>

#define SYS_CLK 12000000

static void __iomem    *pmu_base_addr = (void __iomem *)PMU_FTPMU010_VA_BASE;

/* PMU register data */
static int i2c_fd = -1;
static int gpio_fd = -1;
static int dmac_fd = -1;

static pmuReg_t  regI2cArray[] = {
 /* reg_off  bit_masks  lock_bits     init_val    init_mask */
    {0x3C, (0x1 << 22), (0x1 << 22), (0x0 << 22), (0x1 << 22)},
};

static pmuReg_t regGPIOArray[] = {
 /* reg_off  bit_masks  lock_bits     init_val    init_mask */
    {0x3C, (0x1 << 20), (0x1 << 20), (0x0 << 20), (0x1 << 20)},
};

static pmuRegInfo_t i2c_clk_info = {
    "i2c_clk", 
    ARRAY_SIZE(regI2cArray),
    ATTR_TYPE_NONE, /* no clock source */
    regI2cArray};
    
static pmuRegInfo_t gpio_clk_info = {
    "gpio_clk", 
    ARRAY_SIZE(regGPIOArray),
    ATTR_TYPE_NONE, /* no clock source */
    regGPIOArray};

/* DMAC 
 */
static pmuReg_t regDMACArray[] = {
 /* reg_off  bit_masks  lock_bits     init_val    init_mask */
    {0x38, (0x1 << 11), (0x1 << 11), (0x0 << 11), (0x1 << 11)},
};
static pmuRegInfo_t dmac_clk_info = {
    "DMAC_CLK", 
    ARRAY_SIZE(regDMACArray),
    ATTR_TYPE_NONE, /* no clock source */
    regDMACArray};
    
static inline u32 pmu_read_cpumode(void)
{       
    return ((ioread32(pmu_base_addr + 0x30) >> 24) & 0xFF);
}

static unsigned int pmu_read_pll1out(void)
{
	u32 mul, div;
    
    mul = (ioread32(pmu_base_addr + 0x30) >> 4) & 0xFF;
    div = (ioread32(pmu_base_addr + 0x30) >> 12) & 0xFF;
    
	return (SYS_CLK * mul / div);
}

static unsigned int pmu_read_pll2out(void)
{
	u32 mul, div;
    
    mul = (ioread32(pmu_base_addr + 0x34) >> 20) & 0xFF;
    div = (ioread32(pmu_base_addr + 0x34) >> 28) & 0x0F;
    
    return (SYS_CLK * mul / div);
}

static unsigned int pmu_read_pll3out(void)
{
	u32 mul, div;
    
    mul = (ioread32(pmu_base_addr + 0x34) >> 4) & 0xFF;
    div = (ioread32(pmu_base_addr + 0x34) >> 12) & 0x0F;
    
	return (SYS_CLK * mul / div);
}

static unsigned int pmu_get_ahb_clk(void)
{
	u32 fclk_mode, hclk_mode;
	u32 pll1_out, hclk = 0, fclk = 0;
	static u32 print_info_1st = 0;

	pll1_out = pmu_read_pll1out();
	fclk_mode = (pmu_read_cpumode() >> 4) & 0x3;
	hclk_mode = (pmu_read_cpumode() >> 2) & 0x3;

	if (fclk_mode == 0)
		fclk = pll1_out / 1;
	else if (fclk_mode == 1)
		fclk = pll1_out / 2;
	else if (fclk_mode == 2)
		fclk = pll1_out / 3;
	else
		fclk = pll1_out / 6;

	if (hclk_mode == 0)
		hclk = pll1_out / 1;
	else if (hclk_mode == 1)
		hclk = pll1_out / 2;
	else if (hclk_mode == 2)
		hclk = pll1_out / 3;
	else
		hclk = pll1_out / 6;

	if (!print_info_1st++)
		printk("GM Clock: CPU = %d MHz, AHBCLK = %d MHz\n", fclk / 1000000, hclk / 1000000);

	return hclk;
}

unsigned int pmu_get_apb_clk(void)
{
    return pmu_get_ahb_clk() / 2;
}

static unsigned int pmu_get_version(void)
{
	static unsigned int version = 0;
	int inc = 0;

	/*
	 * Version ID:
	 *     818110: A/B version
	 *     818111: C version
	 *     818112: D version
	 */
	if (!version) {
	    
	    version = ioread32(pmu_base_addr) >> 8;
	    
	    switch ((ioread32(pmu_base_addr) >> 5) & 0x7) 
	    {		
			case 0 : inc = 0; break;
			case 2 : inc = 1; break;
			case 6 : inc = 2; break;
			case 7 : inc = 3; break;
			default: inc = 0; break;
		}
		if ((version & 0xf0) == 0x10) {
			/* 8181 series */
			printk("IC: GM%04x(%c)\n", ((version >> 8) & 0xffff) + inc, (version & 0xff) + 'B' - 0x10);
		} else {
			if ((inl(PMU_FTPMU010_0_VA_BASE)>>4) & 0x1) {
				/* 8181T series */
				printk("IC: GM%04xT(%c)\n", ((version >> 8) & 0xffff) + inc, (version & 0xff) + 'A' - 0x20);
			} else {
				/* 8186 series */
				printk("IC: GM%04x(%c)\n", ((version >> 8) & 0xffff) + 5 + inc, (version & 0xff) + 'A' - 0x20);
			}
		}
	}

	return version;
}

static unsigned int pmu_get_cpu_clk(void)
{
    u32 fclk_mode;
	u32 pll1_out, fclk = 0;
	
	pll1_out = pmu_read_pll1out();
	fclk_mode = (pmu_read_cpumode() >> 4) & 0x3;
	
	if (fclk_mode == 0)
		fclk = pll1_out / 1;
	else if (fclk_mode == 1)
		fclk = pll1_out / 2;
	else if (fclk_mode == 2)
		fclk = pll1_out / 3;
	else
		fclk = pll1_out / 6;

	return fclk;
}

struct clock_s
{
    attrInfo_t   clock;
    u32         (*clock_fun)(void);
} clock_info[] = {
    {{"hclk",   ATTR_TYPE_AHB,    0}, pmu_get_ahb_clk}, 
    {{"pclk",   ATTR_TYPE_APB,    0}, pmu_get_apb_clk},
    {{"pll1",   ATTR_TYPE_PLL1,   0}, pmu_read_pll1out},
    {{"pll2",   ATTR_TYPE_PLL2,   0}, pmu_read_pll2out},
    {{"pll3",   ATTR_TYPE_PLL3,   0}, pmu_read_pll3out},
    {{"cpuclk", ATTR_TYPE_CPU,    0}, pmu_get_cpu_clk},
    {{"pmuver", ATTR_TYPE_PMUVER, 0}, pmu_get_version},
};

static int __init pmu_init(void)
{    
    int i;
    
    printk("PMU: Mapped at 0x%x \n", (unsigned int)pmu_base_addr);
        
    /* calls init function */
    ftpmu010_init(pmu_base_addr);
    
    /* register clock */
    for (i = 0; i < ARRAY_SIZE(clock_info); i ++)
    {
        if (clock_info[i].clock_fun)
            clock_info[i].clock.value = clock_info[i].clock_fun();
        
        ftpmu010_register_attr(&clock_info[i].clock);
    }
    
    /* register I2C to pmu core */
    i2c_fd = ftpmu010_register_reg(&i2c_clk_info);
    if (unlikely(i2c_fd < 0)){
        printk("I2C registers to PMU fail! \n");
    }

    /* register GPIO to pmu core */
    gpio_fd = ftpmu010_register_reg(&gpio_clk_info);
    if (unlikely(gpio_fd < 0)){
        printk("GPIO registers to PMU fail! \n");
    }
    
    /* register DMAC to pmu core */
    dmac_fd = ftpmu010_register_reg(&dmac_clk_info);
    if (unlikely(dmac_fd < 0)){
        printk("DMAC registers to PMU fail! \n");
    }
    
    return 0;
}

arch_initcall(pmu_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GM Technology Corp.");
MODULE_DESCRIPTION("PMU driver");

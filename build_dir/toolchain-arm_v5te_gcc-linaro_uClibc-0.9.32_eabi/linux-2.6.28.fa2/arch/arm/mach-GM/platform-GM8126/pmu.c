#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <asm/io.h>
#include <mach/spec.h>
#include <mach/ftpmu010.h>

#define SYS_CLK 30000000    /* 30Mhz */

static void __iomem    *pmu_base_addr = (void __iomem *)PMU_FTPMU010_VA_BASE;

/* PMU register data */
static int i2c_fd = -1;
static int gpio_fd = -1;
static int dmac_fd = -1;
int ftpmu010_h264e_fd = -1;
int ftpmu010_mcp100_fd = -1;
/* -----------------------------------------------------------------------
 * Clock GATE table. Note: this table is necessary
 * -----------------------------------------------------------------------
 */
ftpmu010_gate_clk_t gate_tbl[] = {
    /* moduleID,    count,  register (ofs, val, mask) */
    {FTPMU_H264E_0, 1,      {{0x38, (0x0 << 4),  (0x1 << 4)}}},   /* mcp210off */
    {FTPMU_MCP100_0,1,      {{0x38, (0x0 << 6),  (0x1 << 6)}}},   /* mcp100off */
    {FTPMU_3DI_0,   1,      {{0x38, (0x0 << 20), (0x1 << 20)}}},  /* di3d0off */
    {FTPMU_SCALER_0,1,      {{0x38, (0x0 << 14), (0x1 << 14)}}},  /* scaroff */
    {FTPMU_NONE,    0,      {{0x0,   0x0,        0x0}}} /* END, this row is necessary */
};

/* I2C
 */
static pmuReg_t  regI2cArray[] = {
 /* reg_off  bit_masks  lock_bits     init_val    init_mask */
    {0x3C, (0x1 << 13), (0x1 << 13), (0x0 << 13), (0x1 << 13)},
    {0x44, (0xF << 12), (0xF << 12), (0x1 << 12), (0x1 << 12)},
};

static pmuRegInfo_t i2c_clk_info = {
    "i2c_clk", 
    ARRAY_SIZE(regI2cArray),
    ATTR_TYPE_NONE, /* no clock source */
    regI2cArray
};

/* GPIO
 */ 
static pmuReg_t regGPIOArray[] = {
 /* reg_off  bit_masks  lock_bits     init_val    init_mask */
    {0x3C, (0x7 << 9), (0x7 << 9), (0x0 << 9), (0x7 << 9)},
};

static pmuRegInfo_t gpio_clk_info = {
    "gpio_clk", 
    ARRAY_SIZE(regGPIOArray),
    ATTR_TYPE_NONE, /* no clock source */
    regGPIOArray
};

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
    regDMACArray
};

/* H264E
 */
static pmuReg_t regH264EArray[] = {
    /* reg_off  bit_masks  lock_bits     init_val    init_mask */
    {0x38,      (0x1 << 4), (0x1 << 4),  (0x0 << 4), (0x0 << 4)},
    {0x6C,      (0x1 << 8), (0x1 << 8),  (0x0 << 8), (0x0 << 8)},
};
static pmuRegInfo_t h264e_clk_info = {
    "H264E_CLK", 
    ARRAY_SIZE(regH264EArray),
    ATTR_TYPE_PLL1,
    regH264EArray
};

/* MCP100
 */
static pmuReg_t regMCP100Array[] = {
    /* reg_off  bit_masks  lock_bits     init_val    init_mask */
    {0x38,      (0x1 << 6), (0x1 << 6),  (0x0 << 6), (0x0 << 6)},
};
static pmuRegInfo_t mcp100_clk_info = {
    "MCP100_CLK", 
    ARRAY_SIZE(regMCP100Array),
    ATTR_TYPE_PLL1,
    regMCP100Array
};

static unsigned int pmu_get_version(void)
{
	static unsigned int version = 0;

	/*
	 * Version ID:
	 *     812610: 8126
	 *     812620: 8126 MP
     *     812621: 8128
	 */
	if (!version) {	    
	    version = ioread32(pmu_base_addr) >> 8;
        switch (version)
	    {
	      case GM8126_TEST_CHIP_ID:
	        printk("IC: GM8126 \n");
	        break;
	      case GM8126_MP2_CHIP_ID:
	        printk("IC: GM8126 MP\n");
	        break;
      	case GM8128_MP_CHIP_ID:
      	case GM8128_MP1_CHIP_ID:
            printk("IC: GM8128 MP\n");
            break;  
	      default:
	        printk("IC: Uknown!!!!!! %x\n", version);
	        break;
        }
	}

	return version;
}

/*
 * Local functions
 */
static inline u32 pmu_read_cpumode(void)
{
    if(pmu_get_version() == GM8126_TEST_CHIP_ID) {
	    return ((inl(PMU_FTPMU010_0_VA_BASE + 0x30)) >> 24) & 0xff;
    } else {
        return ((inl(PMU_FTPMU010_0_VA_BASE + 0x30)) >> 25) & 0xff;
    }
}

static inline u32 pmu_read_pll1out(void)
{
	u32 mul, div;

    if(pmu_get_version() == GM8126_TEST_CHIP_ID) {
        mul = (inl(PMU_FTPMU010_0_VA_BASE + 0x30) >>  3) & 0x1ff;
        div = (inl(PMU_FTPMU010_0_VA_BASE + 0x30) >> 12) & 0x1f;
    } else {
        mul = (inl(PMU_FTPMU010_0_VA_BASE + 0x30) >>  4) & 0x1ff;
        div = (inl(PMU_FTPMU010_0_VA_BASE + 0x30) >> 13) & 0x1f;
    }

	return (SYS_CLK / div * mul);
}

static inline u32 pmu_read_pll2out(void)
{
	u32 mul, div;

	mul = (inl(PMU_FTPMU010_0_VA_BASE + 0x34) >> 3) & 0x1ff;
	div = (inl(PMU_FTPMU010_0_VA_BASE + 0x34) >> 12) & 0x1f;

	return (SYS_CLK / div * mul);
}

static unsigned int pmu_get_ahb_clk(void)
{
	u32 fclk_mode, hclk_mode;
	u32 pll1_out, pll2_out, hclk = 0, fclk = 0;
	static u32 print_info_1st = 0;

	pll1_out = pmu_read_pll1out();	
    pll2_out = pmu_read_pll2out();
	fclk_mode = (pmu_read_cpumode() >> 2) & 0x3;
	hclk_mode = (pmu_read_cpumode() >> 0) & 0x3;

	if (fclk_mode == 0)
		fclk = pll1_out / 1;
	else if (fclk_mode == 1)
		fclk = pll1_out / 2;
	else if (fclk_mode == 2)
		fclk = (pll1_out * 2) / 3;
	else
		fclk = pll2_out;

	if (hclk_mode == 0)
		hclk = pll1_out / 1;
	else if (hclk_mode == 1)
		hclk = pll1_out / 2;
	else if (hclk_mode == 2)
		hclk = pll1_out / 3;
	else
		hclk = pll2_out / 2;

	if (!print_info_1st++) {
		printk("GM Clock: CPU = %d MHz, AHBCLK = %d MHz, PLL1CLK = %d MHz, PLL2CLK = %d MHz\n", 
		    fclk / 1000000, hclk / 1000000, pll1_out/1000000, pmu_read_pll2out()/1000000);
    }
    
	return hclk;
}

unsigned int pmu_get_apb_clk(void)
{
    return pmu_get_ahb_clk() / 2;
}

static unsigned int pmu_get_cpu_clk(void)
{
    u32 fclk_mode;
	u32 pll1_out, pll2_out, fclk = 0;
	
	pll1_out = pmu_read_pll1out();
    pll2_out = pmu_read_pll2out();
    fclk_mode = (pmu_read_cpumode() >> 4) & 0x3;
printk("%s:%d <fclk_mode=%d, pll2_out=%d>\n",__FUNCTION__,__LINE__, fclk_mode, pll2_out);
	
	if (fclk_mode == 0)
		fclk = pll1_out / 1;
	else if (fclk_mode == 1)
		fclk = pll1_out / 2;
	else if (fclk_mode == 2)
		fclk = (pll1_out * 2) / 3;
	else
		fclk = pll2_out;

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
    {{"cpuclk", ATTR_TYPE_CPU,    0}, pmu_get_cpu_clk},
    {{"pmuver", ATTR_TYPE_PMUVER, 0}, pmu_get_version},
};

static int __init pmu_init(void)
{    
    int i;
    
    printk("PMU: Mapped at 0x%x \n", (unsigned int)pmu_base_addr);
        
    /* calls init function */
    ftpmu010_init(pmu_base_addr, &gate_tbl[0]);
    
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
    
    ftpmu010_h264e_fd = ftpmu010_register_reg(&h264e_clk_info);
    if (unlikely(ftpmu010_h264e_fd < 0)){
        printk("H264 registers to PMU fail! \n");
    }
    
    ftpmu010_mcp100_fd = ftpmu010_register_reg(&mcp100_clk_info);
    if (unlikely(ftpmu010_mcp100_fd < 0)){
        printk("MCP100 registers to PMU fail! \n");
    }
    return 0;
}

EXPORT_SYMBOL(ftpmu010_h264e_fd);
EXPORT_SYMBOL(ftpmu010_mcp100_fd);

arch_initcall(pmu_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GM Technology Corp.");
MODULE_DESCRIPTION("PMU driver");

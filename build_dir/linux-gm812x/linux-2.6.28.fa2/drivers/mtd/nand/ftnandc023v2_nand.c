/*
 * FTNANDC023 NAND driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/io.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/vmalloc.h>
#include <linux/dma-mapping.h>
#include <linux/jiffies.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <mach/platform/spec.h>
#include <mach/platform/platform_io.h>
#include <mach/platform/ahb_dma.h>
#include "platform.c"
#include <linux/gpio.h>
/*
 * Local function or variables declaration
 */
static int nand_fd;
static void ftnandc023_dma_callback(int ch, u16 int_status, void *data);
static int ftnandc023v2_block_markbad(struct mtd_info *mtd, loff_t ofs);
int ftnandc023v2_read_bbt(struct mtd_info *mtd, loff_t offs);
static unsigned int ftnandc023v2_get_blockbase(struct mtd_info *mtd, unsigned int base_addr);
static int avail_oob_sz = 0;
static wait_queue_head_t nand023_queue;
static unsigned int trigger_flag = 0;

/*
 * Macro definitions
 */
#define AHB_Memory_8KByte   (1 << 4)
#define IMAGE_MAGIC         0x805A474D
#define MTD_CFG_SZ          (3*256 << 10)
#ifdef CONFIG_PLATFORM_GM8210
#define MTD_LOGO_SZ         (4 << 20)  //4M bytes
#define MTD_LINUX_SZ        (30 << 20)  //30M bytes
#else
#define MTD_LINUX_SZ        (20 << 20)  //20M bytes
#endif
#define CONFIG_FTNANDC023_START_CHANNEL     0

#define BLOCK_ALIGN(base, blk_shift) ((((base) + (0x1 << (blk_shift)) - 1) >> (blk_shift)) << (blk_shift))

#if defined (MODULE) || defined (CONFIG_FTNANDC023_USE_AHBDMA)
#include <mach/ahb_dma.h>
#endif

#ifndef CONFIG_FTNANDC023_USE_AHBDMA
#error "Please configure FTNANDC023_USE_AHBDMA in menuconfig"
#endif
#define PORTING
#include "ftnandc023v2_nand.h"

#ifdef CONFIG_PLATFORM_GM8126
#define AHBDMA_NANDC_REQ        14 
#endif

#ifdef CONFIG_GPIO_WP
//#define GPIO_PIN	28
//#define PIN_NAME	"gpio28"
#define GPIO_PIN        ((32*2)+30) 
#define PIN_NAME        "gpio2_30" 
#endif
//=============================================================================
// System Header, size = 512 bytes
//=============================================================================
typedef struct sys_header {
	
    char signature[8];          /* Signature is "GM8126" */
    unsigned int bootm_addr;    /* default Image offset to load by nandboot */
    unsigned int burnin_addr;   /* burn-in image address */
    unsigned int uboot_addr;    /* uboot address */
    unsigned int linux_addr;    /* linux image address */
#ifdef CONFIG_PLATFORM_GM8210   
    unsigned int slave_linux_addr;    /* slave linux image address */
    unsigned int audio_addr;    /* audio image address */  
    unsigned int logo_addr;    /* logo image address */  
    unsigned int reserved1[4];  /* unused */ 
#else   
    unsigned int reserved1[7];  /* unused */
#endif    
    struct {
        unsigned int nand_numblks;      //number of blocks in chip
        unsigned int nand_numpgs_blk;   //how many pages in a block
        unsigned int nand_pagesz;       //real size in bytes                        
        unsigned int nand_sparesz_inpage;       //64bytes for 2k, ...... needed for NANDC023
        unsigned int nand_numce;        //how many CE in chip
        unsigned int nand_status_bit;
        unsigned int nand_cmmand_bit;
        unsigned int nand_ecc_capability;
        unsigned int nand_ecc_base;     //0/1 indicates 512/1024 bytes      
        unsigned int reserved_1;
        unsigned int reserved_2;
        unsigned int nand_row_cycle;    //1 for 1 cycle ...
        unsigned int nand_col_cycle;    //1 for 1 cycle ...
        unsigned int reserved[1];
    } nandfixup;
    unsigned int reserved2[100];        // unused
    unsigned char last_511[4];  // byte510:0x55, byte511:0xAA
} sys_header_t;

//=============================================================================
// BI table, size = 1024 bytes
//=============================================================================
typedef struct bi_table {
    /* This array size is related to USB_BUFF_SZ defined in usb_scsi.h */
    unsigned int bi_status[256];        //each bit indicates a block. 1 for good, 0 for bad
} bi_table_t;

/* Image header , 512 bytes */
typedef struct nand_img_header {
    unsigned int magic;         /* Image header magic number (0x805A474D) */
    unsigned int chksum;        /* Image CRC checksum */
    unsigned int size;          /* Image size */
    unsigned int unused;
    unsigned char name[80];     /* Image name */
    unsigned char reserved[160 + 256];  /* Reserved for future */
} img_hdr_t;

struct ftnandc023_nand_data {
    struct nand_chip chip;
    struct mtd_info mtd;
    void __iomem *io_base;
    int sel_chip;
    int cur_cmd;
    int page_addr;
    int column;
    int byte_ofs;
    u32 *buf;
    struct device *dev;
    unsigned char *dma_buf;
    dma_addr_t dmaaddr;
    int cur_chan;
    int valid_chip[MAX_CHANNEL];
    int scan_state;
    int flash_type;
    int large_page;
    sys_header_t *sys_hdr;      /* system header */
    bi_table_t *bi_table;       /* bad block table next to sys_hdr */
    int (*write_oob) (struct mtd_info * mtd, const u_char * buf, int len);
    int (*read_oob) (struct mtd_info * mtd, u_char * buf);
    int (*write_page) (struct mtd_info * mtd, const uint8_t * buf);
    int (*read_page) (struct mtd_info * mtd, u_char * buf);
};

#ifdef CONFIG_MTD_NAND_FTNANDC023
static int startchn = CONFIG_FTNANDC023_START_CHANNEL;
#else
static int startchn = 0;
module_param(startchn, int, 0644);
#endif

static int eccbasft;
static int spare;
static int usecrc;
static int useecc;
static int legacy;

#define	max_chip_size	(2048 * 16)     //2048MB NAND
static unsigned int page_map[max_chip_size] = { };

#ifdef CONFIG_MTD_CMDLINE_PARTS
static const char *part_probes[] = { "cmdlinepart", NULL };
#endif

/* tWH, tWP, tREH, tREA, tRP, tWB, tRB, tWHR, tWHR2, tRHW, tRR, tAR,
 * tADL, tRHZ, tCCS, tCS, tCLS, tCLR, tALS, tCALS2, tCWAW, tWPRE,
 * tRPRE, tWPST, tRPST, tWPSTH, tRPSTH, tDQSHZ, tCAD, tDSL
 * tDSH, tDQSL, tDQSH, tCKWR, tWRCK
 */
static struct ftnandc023_chip_timing PORTING chip_timing =
    { 10, 12, 10, 20, 12, 100, 0, 60, 0, 100, 20, 10,
    100, 100, 0, 20, 12, 10, 12, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};

#ifdef CONFIG_MTD_PARTITIONS
static struct mtd_partition PORTING ftnandc023_partition_info[] = {
    {.name = "Linux Section"},
    {.name = "User Section",.size = MTDPART_SIZ_FULL},
    {.name = "Loader Section"},
    {.name = "Burin Section"},
    {.name = "UBoot Section"},
    {.name = "CFG Section"},
};
#endif

/* define the MTD paratition index */
#ifdef CONFIG_PLATFORM_GM8210
enum { MTD_PART_LINUX, MTD_PART_USER, MTD_PART_LOADER, MTD_PART_UBOOT, MTD_PART_SLAVE, MTD_PART_AUDIO, 
        MTD_PART_CFG, MTD_PART_LOGO};
#else
enum { MTD_PART_LINUX, MTD_PART_USER, MTD_PART_LOADER, MTD_PART_BURNIN, MTD_PART_UBOOT,
        MTD_PART_CFG };
#endif

static struct resource ftnandc023_resource[] = {
    [0] = {
           .start = NAND_FTNAND023_PA_BASE,     /* Register Base address */
           .end = NAND_FTNAND023_PA_LIMIT,
           .flags = IORESOURCE_MEM,
           },
    [1] = {
           .start = NANDDP_FTNAND023_PA_BASE,   /* BMC buffer or Data Port access */
           .end = NANDDP_FTNAND023_PA_LIMIT,
           .flags = IORESOURCE_MEM,
           },
    [2] = {
           .start = NAND_FTNAND023_IRQ,
           .end = NAND_FTNAND023_IRQ,
           .flags = IORESOURCE_IRQ,
           }
};

static struct nand_ecclayout nand_hw_eccoob = {
    .eccbytes = 0,
    .eccpos = {0},
    .oobfree = {
                {.offset = 0,
                 .length = 64}}
};

static uint8_t ftnandc023_bbt_pattern[] = { 'B', 'b', 't', '0' };
static uint8_t ftnandc023_mirror_pattern[] = { '1', 't', 'b', 'B' };

static struct nand_bbt_descr ftnandc023_bbt_mirror_descr = {
    .options = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
        | NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
    .offs = 0,
    .len = 4,
    .veroffs = 4,
    .maxblocks = 4,
    .pattern = ftnandc023_mirror_pattern
};

static struct nand_bbt_descr ftnandc023_bbt_main_descr = {
    .options = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
        | NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
    .offs = 0,
    .len = 4,
    .veroffs = 4,
    .maxblocks = 4,
    .pattern = ftnandc023_bbt_pattern
};

static uint8_t ftnandc023_scan_ff_pattern[] = { 0xff, 0xff, 0xff, 0xff };

static struct nand_bbt_descr ftnandc023_largepage_flashbased = {
    .options = NAND_BBT_SCAN2NDPAGE,
    .offs = 0,
    .len = 4,
    .pattern = ftnandc023_scan_ff_pattern
};

static void ftnandc023_regdump(struct mtd_info *mtd)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    u32 val;

    printk(KERN_INFO "===================================\n");
    val = readl(data->io_base + ECC_INTR_STATUS);
    printk(KERN_INFO "ECC intr sts(0x%2x): 0x%08x\n", ECC_INTR_STATUS, val);
    val = readl(data->io_base + DEV_BUSY);
    printk(KERN_INFO "Device busy(0x%2x): 0x%08x\n", DEV_BUSY, val);
    val = readl(data->io_base + GENERAL_SETTING);
    printk(KERN_INFO "General setting(0x%2x): 0x%08x\n", GENERAL_SETTING, val);
    val = readl(data->io_base + MEM_ATTR_SET);
    printk(KERN_INFO "Mem attr.(0x%2x): 0x%08x\n", MEM_ATTR_SET, val);
    val = readl(data->io_base + INTR_STATUS);
    printk(KERN_INFO "Intr sts(0x%2x): 0x%08x\n", INTR_STATUS, val);
    val = readl(data->io_base + BMC_REGION_STATUS);
    printk(KERN_INFO "BMC region sts(0x%2x): 0x%08x\n", BMC_REGION_STATUS, val);
    printk(KERN_INFO "===================================\n");
}

static inline void ftnandc023_set_row_col_addr(struct ftnandc023_nand_data *data, int row, int col)
{
    int val;

    val = readl(data->io_base + MEM_ATTR_SET);
    val &= ~(0x7 << 12);
    val |= (ATTR_ROW_CYCLE(row) | ATTR_COL_CYCLE(col));

    writel(val, data->io_base + MEM_ATTR_SET);
}

/* low enable write protect, high disable write protect */
void write_protect(int mode)
{
#ifdef CONFIG_GPIO_WP
		if(mode)
			gpio_direction_output(GPIO_PIN, 0);
		else
			gpio_direction_output(GPIO_PIN, 1);
#endif
}

static void ftnandc023_soft_reset(struct mtd_info *mtd)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;

    platform_select_pinmux(nand_fd, 0);

    writel(1, data->io_base + GLOBAL_RESET);
    while (readl(data->io_base + GLOBAL_RESET)) ;

    platform_select_pinmux(nand_fd, -1);
}

static int ftnandc023_nand_check_cmdq(struct mtd_info *mtd)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    unsigned long timeo = jiffies;
    u32 status;
    int ret;

    platform_select_pinmux(nand_fd, 0);

    ret = -EIO;
    timeo += HZ;
    while (time_before(jiffies, timeo)) {
        status = readl(data->io_base + CMDQUEUE_STATUS);
        if ((status & CMDQUEUE_STATUS_FULL(data->cur_chan)) == 0) {
            ret = 0;
            break;
        }
        cond_resched();
    }
    
		platform_select_pinmux(nand_fd, -1);
    return ret;
}

#ifdef CONFIG_FTNANDC023_USE_AHBDMA
static int ftnandc023_dma_wait(struct mtd_info *mtd)
{
#if 1
		int rc = 0;
    wait_event_interruptible_timeout(nand023_queue, trigger_flag == 1, 500 * HZ);
		if (rc < 0) {
    	printk("nand023 queue wake up timeout signal arrived\n");
    	return -1;
    }

    trigger_flag = 0;
    return 0;
#else
    unsigned long timeo = jiffies;
    u32 val = 0;
    int ret = -EIO;

    timeo += HZ;

    while (time_before(jiffies, timeo)) {
        val = fa_ahb_dma_get_status(AHBDMA_NANDC);
        if ((val & INT_DMA_ALL_DONE) != 0) {
            ret = 0;
            goto out;
        }

        cond_resched();
    }

    DBGLEVEL1(ftnandc023_dbg("dma timeout, status = 0x%x\n", val));
    ftnandc023_regdump(mtd);
  out:
    return ret;
#endif
}
#endif

static int ftnandc023_nand_wait(struct mtd_info *mtd, struct nand_chip *chip)
{
    struct ftnandc023_nand_data *data = chip->priv;

    unsigned long timeo = jiffies;
    int ret, state = chip->state;
    u32 status;
    int i;

    platform_select_pinmux(nand_fd, 0);

    ret = NAND_STATUS_FAIL;
    timeo += HZ;

    while (time_before(jiffies, timeo)) {
        status = readl(data->io_base + INTR_STATUS);
        /* Clear interrupt instantly, since we already keep IS to status */
        writel(status, data->io_base + INTR_STATUS);

        if ((data->cur_cmd == LARGE_FIXFLOW_PAGEWRITE) || (data->cur_cmd == LARGE_FIXFLOW_ERASE)) {
            if (unlikely(status & STATUS_FAIL(data->cur_chan))) {
                printk(KERN_ERR "NAND: STATUS FAIL(0x%x, off 0x178 = 0x%x)!\n", status,
                       readl(data->io_base + READ_STATUS0));
                ftnandc023_soft_reset(mtd);
                goto out;
            }
        }

        if (status & STATUS_CMD_COMPLETE(data->cur_chan)) {
            if (state == FL_READING) {
                if (unlikely(status & STATUS_CRC_FAIL(data->cur_chan))) {
                    writel(STATUS_CRC_FAIL(data->cur_chan), data->io_base + INTR_STATUS);
                }

                status = readl(data->io_base + ECC_INTR_STATUS);
                if (unlikely(status & ECC_ERR_FAIL(data->cur_chan))) {
                    writel(ECC_ERR_FAIL(data->cur_chan), data->io_base + ECC_INTR_STATUS);
                    for (i = 0; i < (mtd->writesize >> 2); i++) {
                        if (*(data->buf + i) != 0xFFFFFFFF) {
                            ret = NAND_STATUS_FAIL;
                            printk(KERN_ERR "ECC err\n");
                            mtd->ecc_stats.failed++;
                            goto out;
                        }
                    }
                }
            }
            ret = NAND_STATUS_READY;
            goto out;

        }

        cond_resched();
    }

    DBGLEVEL1(ftnandc023_dbg("nand wait time out\n"));
    ftnandc023_regdump(mtd);
  out:
  	write_protect(1);
  	platform_select_pinmux(nand_fd, -1);
    return ret;
}

/* The unit of Hclk is MHz, and the unit of Time is ns.
 * We desire to calculate N to satisfy N*(1/Hclk) > Time given Hclk and Time
 * ==> N > Time * Hclk
 * ==> N > Time * 10e(-9) * Hclk *10e(6)        --> take the order out
 * ==> N > Time * Hclk * 10e(-3)
 * ==> N > Time * Hclk / 1000
 * ==> N = (Time * Hclk + 999) / 1000
 */
static void ftnandc023_calc_timing(struct mtd_info *mtd)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    uint16_t tWH, tWP, tREH, tRES, tBSY, tBUF1;
    uint16_t tBUF2, tBUF3, tBUF4, t1, tPRE, tRLAT;
    uint16_t tPST, tPSTH, tCAD, tCS, tCKWR;
    int i;
    struct ftnandc023_chip_timing *p;
    u32 HCLK, timing[4];

    /*
     * The default value is slow and it should be enough for use.
     */
    return;

    //HCLK = a320_get_ahb_clk() * 2 / 1000000;
    HCLK = ftpmu010_get_attr(ATTR_TYPE_AHB) / 1000000;
#if defined (CONFIG_FTNANDC023_MICRON_29F32G08CBABB)
    if (data->flash_type == ONFI)
        p = &sync_timing;
    else
#endif
        p = &chip_timing;

    tWH = (p->tWH * HCLK + 999) / 1000;
    if (tWH >= 1)
        tWH--;

    tWP = (p->tWP * HCLK + 999) / 1000;
    if (tWP >= 1)
        tWP--;

    tREH = (p->tREH * HCLK + 999) / 1000;
    if (tREH >= 1)
        tREH--;

    tRES = max(p->tREA, p->tRP);
    tRES = (tRES * HCLK + 999) / 1000;
    if (tRES >= 1)
        tRES--;

    tRLAT = tREH + tRES + 1;

    tBSY = max(p->tWB, p->tRB);
    tBSY = (tBSY * HCLK + 999) / 1000;
    if (tBSY >= 2)
        tBSY--;

    tBUF1 = max(p->tADL, p->tCCS);
    tBUF1 = (tBUF1 * HCLK + 999) / 1000;
    if (tBUF1 >= 2)
        tBUF1--;

    tBUF2 = max(p->tAR, p->tRR);
    tBUF2 = max(tBUF2, p->tCLR);
    tBUF2 = (tBUF2 * HCLK + 999) / 1000;
    if (tBUF2 >= 2)
        tBUF2--;

    tBUF3 = max(p->tRHW, p->tRHZ);
    tBUF3 = max(tBUF3, p->tDQSHZ);
    tBUF3 = (tBUF3 * HCLK + 999) / 1000;
    if (tBUF3 >= 2)
        tBUF3--;

    tBUF4 = p->tWHR;
    tBUF4 = (tBUF4 * HCLK + 999) / 1000;
    if (tBUF4 >= 2)
        tBUF4--;

    if (data->flash_type == TOGGLE) {
        t1 = max(p->tCLS, p->tALS);
        t1 = max(t1, p->tCS);
        t1 -= p->tWP;
    } else {
        t1 = max((uint16_t) (p->tCALS2 - p->tWP), p->tCWAW);
    }
    t1 = (t1 * HCLK + 999) / 1000;
    if (t1 >= 1)
        t1--;

    tPRE = max(p->tWPRE, p->tRPRE);
    tPRE = (tPRE * HCLK + 999) / 1000;
    if (tPRE >= 1)
        tPRE--;

    tPST = max(p->tWPST, p->tRPST);
    tPST = (tPST * HCLK + 999) / 1000;
    if (tPST >= 1)
        tPST--;

    if (data->flash_type == TOGGLE)
        tPSTH = max(p->tWPSTH, p->tRPSTH);
    else
        tPSTH = p->tDQSHZ;
    tPSTH = (tPSTH * HCLK + 999) / 1000;
    if (tPSTH >= 1)
        tPSTH--;

    tCAD = p->tCAD;
    tCAD = (tCAD * HCLK + 999) / 1000;
    if (tCAD >= 1)
        tCAD--;

    tCS = p->tCS;
    tCS = (tCS * HCLK + 999) / 1000;
    if (tCS >= 1)
        tCS--;

    if (data->flash_type == TOGGLE) {
        tCKWR = max(p->tDSL, p->tDSH);
        tCKWR = max(tCKWR, p->tDQSL);
        tCKWR = max(tCKWR, p->tDQSH);
    } else {
        tCKWR = max(p->tCKWR, p->tWRCK);
    }
    tCKWR = (tCKWR * HCLK + 999) / 1000;
    if (tCKWR >= 1)
        tCKWR--;

    timing[0] = (tWH << 24) | (tWP << 16) | (tREH << 8) | tRES;
    timing[1] = (tRLAT << 16) | (tBSY << 8) | t1;
    timing[2] = (tBUF4 << 24) | (tBUF3 << 16) | (tBUF2 << 8) | tBUF1;
    timing[3] = (tPRE << 28) | (tPST << 24) | (tPSTH << 16) | (tCAD << 8) | (tCS << 5) | tCKWR;

    printk("AC Timing 0:0x%08x\n", timing[0]);
    printk("AC Timing 1:0x%08x\n", timing[1]);
    printk("AC Timing 2:0x%08x\n", timing[2]);
    printk("AC Timing 3:0x%08x\n", timing[3]);

    for (i = 0; i < MAX_CHANNEL; i++) {
        writel(timing[0], data->io_base + FL_AC_TIMING0(i));
        writel(timing[1], data->io_base + FL_AC_TIMING1(i));
        writel(timing[2], data->io_base + FL_AC_TIMING2(i));
        writel(timing[3], data->io_base + FL_AC_TIMING3(i));
    }
}

static uint32_t ftnandc023_onfi_get_feature(struct mtd_info *mtd, int chn, int ce, int type)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    u32 cq6, val;

    platform_select_pinmux(nand_fd, 0);

    ftnandc023_set_row_col_addr(data, 1, 0);

    /* 0x1 is Timing mode feature address */
    writel(0x1, data->io_base + CMDQUEUE1(chn));

    cq6 = CMD_COMPLETE_EN | CMD_FLASH_TYPE(type);
    cq6 |= CMD_START_CE(ce) | CMD_BYTE_MODE | CMD_SPARE_NUM(3);
    cq6 |= CMD_INDEX(ONFI_FIXFLOW_GETFEATURE);
    ftnandc023_nand_check_cmdq(mtd);
    writel(cq6, data->io_base + CMDQUEUE6(chn));
    ftnandc023_nand_wait(mtd, chip);
    val = readl(data->io_base + SPARE_SRAM + (chn << 10)
                + (ce << 6));

		platform_select_pinmux(nand_fd, -1);

    return val;
}

static void ftnandc023_onfi_set_feature(struct mtd_info *mtd, int chn, int ce)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    u32 cq6;

    platform_select_pinmux(nand_fd, 0);
    write_protect(0);

    ftnandc023_set_row_col_addr(data, 1, 0);

    /* 0x11 means Timing mode 1 and Synchronous DDR */
    writel(0x11, data->io_base + SPARE_SRAM + (chn << 10) + (ce << 6));

    /* 0x1 is Timing mode feature address */
    writel(0x1, data->io_base + CMDQUEUE1(chn));

    cq6 = CMD_COMPLETE_EN | CMD_FLASH_TYPE(0);
    cq6 |= CMD_START_CE(ce) | CMD_BYTE_MODE | CMD_SPARE_NUM(3);
    cq6 |= CMD_INDEX(ONFI_FIXFLOW_SETFEATURE);
    ftnandc023_nand_check_cmdq(mtd);
    writel(cq6, data->io_base + CMDQUEUE6(chn));
    ftnandc023_nand_wait(mtd, chip);

    platform_select_pinmux(nand_fd, -1);
}

static int ftnandc023_onfi_sync(struct mtd_info *mtd)
{
    struct nand_chip *p = mtd->priv;
    struct ftnandc023_nand_data *data = p->priv;
    u32 val;
    int i, j, ret = -1;

    platform_select_pinmux(nand_fd, 0);

    for (i = startchn; i < MAX_CHANNEL; i++) {
        for (j = 0; j < data->valid_chip[i]; j++) {
            val = ftnandc023_onfi_get_feature(mtd, i, j, 0);
            printk("onfi feature for channel %d, CE %d: 0x%x\n", i, j, val);
        }
    }
    for (i = startchn; i < MAX_CHANNEL; i++) {
        for (j = 0; j < data->valid_chip[i]; j++) {
            ftnandc023_onfi_set_feature(mtd, i, j);
        }
    }
    for (i = startchn; i < MAX_CHANNEL; i++) {
        for (j = 0; j < data->valid_chip[i]; j++) {
            val = ftnandc023_onfi_get_feature(mtd, i, j, 3);
            printk("onfi feature for channel %d, CE %d: 0x%x\n", i, j, val);
            if (val != 0x1111) {
                goto out;
            }
        }
    }
    ret = 0;

  out:
  	platform_select_pinmux(nand_fd, -1);
    return ret;
}

static int ftnandc023_available_oob(struct mtd_info *mtd)
{
    int ret = 0;
    int tmp, eccbyte;

    if (useecc < 0)
        goto out;
    if (usecrc != 0)
        usecrc = 1;
    else
        usecrc = 0;

    eccbyte = (useecc * 14) / 8;
    if (((useecc * 14) % 8) != 0)
        eccbyte++;
    tmp = (eccbyte * (mtd->writesize >> eccbasft)) + (usecrc << 1) * (mtd->writesize >> eccbasft);

        /*----------------------------------------------------------
	 * YAFFS require 16 bytes OOB without ECC, 28 bytes with
	 * ECC enable.
	 * BBT require 5 bytes for Bad Block Table marker.
	 */
#ifdef CONFIG_YAFFS_FS
    if (spare - tmp >= 16) {
        ret = spare - tmp;
        printk(KERN_INFO "NAND(YAFFS): avaliable OOB is %d byte.\n", ret);
    } else {
        printk(KERN_INFO
               "NAND: Not enough OOB:%d bytes(YAFFS requires 16 bytes without software ECC, "
               "28 bytes with ECC enable), try to reduce ECC correction bits.\n", spare - tmp);
    }
#else
    ret = spare - tmp;
    printk(KERN_INFO "Avaliable OOB is %d byte(%d per sector).\n", ret,
           ret / (mtd->writesize >> eccbasft));
#endif
  out:
    return ret;
}

static uint8_t ftnandc023_read_byte(struct mtd_info *mtd)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    uint32_t lv;
    uint8_t b = 0;

    platform_select_pinmux(nand_fd, 0);

    switch (data->cur_cmd) {
    case NAND_CMD_READID:
        lv = readl(data->io_base + SPARE_SRAM + (data->cur_chan << 10)
                   + (data->sel_chip << 6));
        b = (lv >> data->byte_ofs) & 0xFF;
        data->byte_ofs += 8;
        if (data->byte_ofs == 32)
            data->byte_ofs = 0;
        break;
    case NAND_CMD_STATUS:
        lv = readl(data->io_base + READ_STATUS0);
        lv = lv >> (data->cur_chan * 8);
        b = (lv & 0xFF);
        /* FIXME: status seems has problem, workaround here */
        b |= NAND_STATUS_WP;
        break;
    }
		platform_select_pinmux(nand_fd, -1);
    return b;
}

static int ftnandc023_nand_read_oob_lp(struct mtd_info *mtd, u_char * buf)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    u32 cq6;
    int status = 0, i, j;
    u8 *p;

    p = buf;

    platform_select_pinmux(nand_fd, 0);
    ftnandc023_set_row_col_addr(data, 2, 1);

    writel(data->page_addr, data->io_base + CMDQUEUE1(data->cur_chan));
    writel(CMD_COUNT(mtd->writesize >> eccbasft), data->io_base + CMDQUEUE5(data->cur_chan));

    cq6 = CMD_COMPLETE_EN | CMD_FLASH_TYPE(data->flash_type);
    cq6 |= CMD_START_CE(data->sel_chip) | CMD_SPARE_NUM(spare) | CMD_INDEX(LARGE_FIXFLOW_READOOB);
    status = ftnandc023_nand_check_cmdq(mtd);
    if (status < 0)
        goto out;
    writel(cq6, data->io_base + CMDQUEUE6(data->cur_chan));
    if (ftnandc023_nand_wait(mtd, chip) == NAND_STATUS_FAIL) {
        status = -1;
        goto out;
    }

    for (i = 0; i < (mtd->writesize >> eccbasft); i++) {
        for (j = 0; j < (spare + 1); j++) {
            *(p++) =
                readb(data->io_base + SPARE_SRAM + (data->cur_chan << 10) +
                      (data->sel_chip << 6) + i * (64 / (mtd->writesize >> eccbasft)) + j);
        }
    }

    for (i = avail_oob_sz; i < mtd->oobsize; i++)
        *(p++) = 0xFF;          /* clear */
  out:
  	platform_select_pinmux(nand_fd, -1);
    return status;
}

static int ftnandc023_nand_read_oob_sp(struct mtd_info *mtd, u_char * buf)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    u32 cq6;
    int i, status = 0;

    platform_select_pinmux(nand_fd, 0);
    ftnandc023_set_row_col_addr(data, 1, 0);

    writel(data->page_addr, data->io_base + CMDQUEUE1(data->cur_chan));
    writel(CMD_COUNT(1), data->io_base + CMDQUEUE5(data->cur_chan));

    cq6 = CMD_COMPLETE_EN | CMD_FLASH_TYPE(data->flash_type);
    cq6 |= CMD_START_CE(data->sel_chip) | CMD_SPARE_NUM(spare) | CMD_INDEX(SMALL_FIXFLOW_READOOB);
    status = ftnandc023_nand_check_cmdq(mtd);
    if (status < 0)
        goto out;
    writel(cq6, data->io_base + CMDQUEUE6(data->cur_chan));
    if (ftnandc023_nand_wait(mtd, chip) == NAND_STATUS_FAIL) {
        status = -1;
        goto out;
    }

    for (i = 0; i < mtd->oobsize; i++) {
        *(buf + i) = readb(data->io_base + SPARE_SRAM + (data->cur_chan << 10) +
                           (data->sel_chip << 6) + i);
    }
  out:
  	platform_select_pinmux(nand_fd, -1);
    return status;
}

static int ftnandc023_nand_read_oob_std(struct mtd_info *mtd, struct nand_chip *chip, int page,
                                        int sndcmd)
{
    struct ftnandc023_nand_data *data = chip->priv;

    data->page_addr = page;

    return data->read_oob(mtd, chip->oob_poi);
}

static int ftnandc023_nand_read_page_lp(struct mtd_info *mtd, u_char * buf)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    u32 cq6;
    u32 *lbuf;
    int status = 0;
    int i, j, t = 0;
    u8 *p;

    DBGLEVEL2(ftnandc023_dbg
              ("r:page = 0x%x, size = %d, data->column = %d\n", data->page_addr, mtd->writesize,
               data->column));
    lbuf = (u32 *) buf;

    platform_select_pinmux(nand_fd, 0);
    ftnandc023_set_row_col_addr(data, 2, 1);
    writel(data->page_addr, data->io_base + CMDQUEUE1(data->cur_chan));
    writel(CMD_COUNT(mtd->writesize >> eccbasft) | (data->column / mtd->writesize),
           data->io_base + CMDQUEUE5(data->cur_chan));
    cq6 = CMD_COMPLETE_EN | CMD_FLASH_TYPE(data->flash_type);
    cq6 |= CMD_START_CE(data->sel_chip) | CMD_SPARE_NUM(spare) | CMD_INDEX(LARGE_FIXFLOW_PAGEREAD);
#ifdef CONFIG_FTNANDC023_USE_AHBDMA
    cq6 |= (1 << 4);
#endif
    status = ftnandc023_nand_check_cmdq(mtd);
    if (status < 0)
        goto out;

    writel(cq6, data->io_base + CMDQUEUE6(data->cur_chan));
#ifdef CONFIG_FTNANDC023_USE_AHBDMA
    fa_set_ahb_dma_src_params(AHBDMA_NANDC,
                              (u32) data->chip.IO_ADDR_R, AHBDMA_MASTER_0, AHBDMA_WIDTH_32BIT,
                              AHBDMA_CTL_FIX);
    fa_set_ahb_dma_dst_params(AHBDMA_NANDC,
                              (u32) data->dmaaddr, AHBDMA_MASTER_0, AHBDMA_WIDTH_32BIT,
                              AHBDMA_CTL_INC);
    fa_set_ahb_dma_transfer_params2(AHBDMA_NANDC, AHBDMA_MASTER_0, mtd->writesize, 1);
    fa_ahb_dma_init(AHBDMA_NANDC, AHBDMA_NANDC_REQ);
    fa_ahb_dma_add_auto(AHBDMA_NANDC, mtd->writesize);
    fa_ahb_dma_start(AHBDMA_NANDC);
    status = ftnandc023_dma_wait(mtd);
    fa_ahb_dma_reset(AHBDMA_NANDC);
    if (status < 0) {
        ftnandc023_soft_reset(mtd);
        goto out;
    }
    memcpy(buf, data->dma_buf, mtd->writesize);

#else
    for (i = 0; i < mtd->writesize; i += 4)
        *lbuf++ = *(volatile unsigned *)(data->chip.IO_ADDR_R);
#endif
    if (ftnandc023_nand_wait(mtd, chip) == NAND_STATUS_FAIL) {
        status = -1;
        goto out;
    }

    p = chip->oob_poi;
    if (p) {
        for (i = 0; i < (mtd->writesize >> eccbasft); i++) {
            for (j = 0; j < (spare + 1); j++) {
                t++;
                *(p++) =
                    readb(data->io_base + SPARE_SRAM + (data->cur_chan << 10) +
                          (data->sel_chip << 6) + i * (64 / (mtd->writesize >> eccbasft)) + j);
            }
        }
    }

    /* clear to default value */
    if (p) {
        for (i = avail_oob_sz; i < mtd->oobsize; i++) {
            *(p++) = 0xFF;
        }
    }
  out:
  	platform_select_pinmux(nand_fd, -1);
    return status;
}

static int ftnandc023_nand_read_page_sp(struct mtd_info *mtd, u_char * buf)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    u32 cq6;
    u32 *lbuf;
    int status = 0;
    int i;

    lbuf = (u32 *) buf;

    platform_select_pinmux(nand_fd, 0);
    ftnandc023_set_row_col_addr(data, 1, 0);

    writel(data->page_addr, data->io_base + CMDQUEUE1(data->cur_chan));
    writel(CMD_COUNT(1) | (data->column / mtd->writesize),
           data->io_base + CMDQUEUE5(data->cur_chan));
    cq6 = CMD_COMPLETE_EN | CMD_FLASH_TYPE(data->flash_type);
    cq6 |= CMD_START_CE(data->sel_chip) | CMD_SPARE_NUM(spare) | CMD_INDEX(SMALL_FIXFLOW_PAGEREAD);
#ifdef CONFIG_FTNANDC023_USE_AHBDMA
    cq6 |= (1 << 4);
#endif
    status = ftnandc023_nand_check_cmdq(mtd);
    if (status < 0)
        goto out;
    writel(cq6, data->io_base + CMDQUEUE6(data->cur_chan));
#ifdef CONFIG_FTNANDC023_USE_AHBDMA
    fa_set_ahb_dma_src_params(AHBDMA_NANDC,
                              (u32) data->chip.IO_ADDR_R, AHBDMA_MASTER_0, AHBDMA_WIDTH_32BIT,
                              AHBDMA_CTL_FIX);
    fa_set_ahb_dma_dst_params(AHBDMA_NANDC,
                              (u32) data->dmaaddr, AHBDMA_MASTER_0, AHBDMA_WIDTH_32BIT,
                              AHBDMA_CTL_INC);
    fa_set_ahb_dma_transfer_params2(AHBDMA_NANDC, AHBDMA_MASTER_0, mtd->writesize, 1);
    fa_ahb_dma_init(AHBDMA_NANDC, AHBDMA_NANDC_REQ);
    fa_ahb_dma_add_auto(AHBDMA_NANDC, mtd->writesize);
    fa_ahb_dma_start(AHBDMA_NANDC);
    status = ftnandc023_dma_wait(mtd);
    fa_ahb_dma_reset(AHBDMA_NANDC);
    if (status < 0) {
        ftnandc023_soft_reset(mtd);
        goto out;
    }
    memcpy(buf, data->dma_buf, mtd->writesize);
#else
    for (i = 0; i < mtd->writesize; i += 4)
        *lbuf++ = *(volatile unsigned *)(data->chip.IO_ADDR_R);
#endif
    if (ftnandc023_nand_wait(mtd, chip) == NAND_STATUS_FAIL) {
        status = -1;
        goto out;
    }

    for (i = 0; i < mtd->oobsize; i++) {
        *(chip->oob_poi + i) = readb(data->io_base + SPARE_SRAM + (data->cur_chan << 10) +
                                     (data->sel_chip << 6) + i);
    }

  out:
    platform_select_pinmux(nand_fd, -1);
    return status;
}

static int ftnandc023_nand_read_page(struct mtd_info *mtd, struct nand_chip *chip, uint8_t * buf)
{
    struct ftnandc023_nand_data *data = chip->priv;

    data->buf = (u32 *) buf;

    return data->read_page(mtd, buf);
}

static int ftnandc023_nand_write_oob_lp(struct mtd_info *mtd, const u_char * buf, int len)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    u32 cq6;
    int status = 0, i, j;
    u8 *p;

#if 0
    {
        u8 oob_buf[64] = { 0xAB };
        if (ftnandc023_nand_read_oob_lp(mtd, oob_buf) < 0)
            return -1;

        for (i = 0; i < avail_oob_sz; i++)
            printk("0x%x ", oob_buf[i]);
        printk("\n");

        platform_select_pinmux(nand_fd, 0);
        write_protect(0);
        goto out;
    }
#else

    platform_select_pinmux(nand_fd, 0);
    write_protect(0);

    if (page_map[data->page_addr / 32] & (1 << (data->page_addr % 32))) //have be write 
        goto out;
#endif
    ftnandc023_set_row_col_addr(data, 2, 1);

    p = (u8 *) buf;

    for (i = 0; i < (mtd->writesize >> eccbasft); i++) {
        for (j = 0; j < (spare + 1); j++) {
            writeb(*(p++),
                   data->io_base + SPARE_SRAM + (data->cur_chan << 10) +
                   (data->sel_chip << 6) + i * (64 / (mtd->writesize >> eccbasft)) + j);
        }
    }

    writel(data->page_addr, data->io_base + CMDQUEUE1(data->cur_chan));
    writel(CMD_COUNT(mtd->writesize >> eccbasft), data->io_base + CMDQUEUE5(data->cur_chan));
    cq6 = CMD_COMPLETE_EN | CMD_FLASH_TYPE(data->flash_type);
    cq6 |= CMD_START_CE(data->sel_chip) | CMD_SPARE_NUM(spare) | CMD_INDEX(LARGE_FIXFLOW_WRITEOOB);
    status = ftnandc023_nand_check_cmdq(mtd);
    if (status < 0) {
        printk("status error\n");
        goto out;
    }
    writel(cq6, data->io_base + CMDQUEUE6(data->cur_chan));

    if (ftnandc023_nand_wait(mtd, chip) == NAND_STATUS_FAIL) {
        status = -1;
        printk("wait error\n");
        goto out;
    }
  out:
    platform_select_pinmux(nand_fd, -1);
    return status;
}

static int ftnandc023_nand_write_oob_sp(struct mtd_info *mtd, const u_char * buf, int len)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    u32 cq6;
    int i, status = 0;

    platform_select_pinmux(nand_fd, 0);
    write_protect(0);
    ftnandc023_set_row_col_addr(data, 1, 0);

    for (i = 0; i < len; i++)
        writeb(*(buf + i),
               data->io_base + SPARE_SRAM + (data->cur_chan << 10) + (data->sel_chip << 6) + i);

    writel(data->page_addr, data->io_base + CMDQUEUE1(data->cur_chan));
    writel(CMD_COUNT(1), data->io_base + CMDQUEUE5(data->cur_chan));
    cq6 = CMD_COMPLETE_EN | CMD_FLASH_TYPE(data->flash_type);
    cq6 |= CMD_START_CE(data->sel_chip) | CMD_SPARE_NUM(spare) | CMD_INDEX(SMALL_FIXFLOW_WRITEOOB);
    status = ftnandc023_nand_check_cmdq(mtd);
    if (status < 0)
        goto out;
    writel(cq6, data->io_base + CMDQUEUE6(data->cur_chan));

    if (ftnandc023_nand_wait(mtd, chip) == NAND_STATUS_FAIL) {
        status = -1;
        goto out;
    }
  out:
    platform_select_pinmux(nand_fd, -1);
    return status;
}

static int ftnandc023_nand_write_oob_std(struct mtd_info *mtd, struct nand_chip *chip, int page)
{
    struct ftnandc023_nand_data *data = chip->priv;

    DBGLEVEL2(ftnandc023_dbg("write oob only to page = 0x%x\n", page));
    data->page_addr = page;

    return data->write_oob(mtd, chip->oob_poi, mtd->oobsize);
}

static int ftnandc023_nand_write_page_lp(struct mtd_info *mtd, const uint8_t * buf)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    u32 cq6;
    int i, j;
    u32 *lbuf;
    int status = 0;
    u8 *p;

    DBGLEVEL2(ftnandc023_dbg
              ("w:page = 0x%x, size = %d, data->column = %d\n", data->page_addr, mtd->writesize,
               data->column));

    platform_select_pinmux(nand_fd, 0);
    write_protect(0);
    p = chip->oob_poi;

    if (p != NULL) {
        for (i = 0; i < (mtd->writesize >> eccbasft); i++) {
            for (j = 0; j < (spare + 1); j++) {
                writeb(*(p++),
                       data->io_base + SPARE_SRAM + (data->cur_chan << 10) +
                       (data->sel_chip << 6) + i * (64 / (mtd->writesize >> eccbasft)) + j);
            }
        }
    }

    lbuf = (u32 *) buf;

    ftnandc023_set_row_col_addr(data, 2, 1);

    writel(data->page_addr, data->io_base + CMDQUEUE1(data->cur_chan));
    writel(CMD_COUNT(mtd->writesize >> eccbasft) | (data->column / mtd->writesize),
           data->io_base + CMDQUEUE5(data->cur_chan));
    cq6 = CMD_COMPLETE_EN | CMD_FLASH_TYPE(data->flash_type);
    cq6 |= CMD_START_CE(data->sel_chip) | CMD_SPARE_NUM(spare) | CMD_INDEX(LARGE_FIXFLOW_PAGEWRITE);

#ifdef CONFIG_FTNANDC023_USE_AHBDMA
    cq6 |= (1 << 4);
#endif
    status = ftnandc023_nand_check_cmdq(mtd);
    if (status < 0)
        goto out;
    writel(cq6, data->io_base + CMDQUEUE6(data->cur_chan));
#ifdef CONFIG_FTNANDC023_USE_AHBDMA
    memcpy(data->dma_buf, buf, mtd->writesize);
    fa_set_ahb_dma_src_params(AHBDMA_NANDC,
                              (u32) data->dmaaddr, AHBDMA_MASTER_0, AHBDMA_WIDTH_32BIT,
                              AHBDMA_CTL_INC);
    fa_set_ahb_dma_dst_params(AHBDMA_NANDC,
                              (u32) data->chip.IO_ADDR_R, AHBDMA_MASTER_0, AHBDMA_WIDTH_32BIT,
                              AHBDMA_CTL_FIX);
    fa_set_ahb_dma_transfer_params2(AHBDMA_NANDC, AHBDMA_MASTER_0, mtd->writesize, 1);
    fa_ahb_dma_init(AHBDMA_NANDC, AHBDMA_NANDC_REQ);
    fa_ahb_dma_add_auto(AHBDMA_NANDC, mtd->writesize);
    fa_ahb_dma_start(AHBDMA_NANDC);
    status = ftnandc023_dma_wait(mtd);
    fa_ahb_dma_reset(AHBDMA_NANDC);
    if (status < 0) {
        ftnandc023_soft_reset(mtd);
        goto out;
    }
#else
    for (i = 0; i < mtd->writesize; i += 4)
        *(volatile unsigned *)(data->chip.IO_ADDR_R) = *lbuf++;
#endif
    page_map[data->page_addr / 32] |= (1 << (data->page_addr % 32));

    if (ftnandc023_nand_wait(mtd, chip) == NAND_STATUS_FAIL) {
        status = -1;
        goto out;
    }
  out:
    platform_select_pinmux(nand_fd, -1);
    return status;
}

static int ftnandc023_nand_write_page_sp(struct mtd_info *mtd, const uint8_t * buf)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    u32 cq6;
    int i;
    int status = 0;
    u32 *lbuf;

    platform_select_pinmux(nand_fd, 0);
    write_protect(0);

    lbuf = (u32 *) buf;
    for (i = 0; i < mtd->oobsize; i++)
        writeb(*(chip->oob_poi + i),
               data->io_base + SPARE_SRAM + (data->cur_chan << 10) + (data->sel_chip << 6) + i);

    ftnandc023_set_row_col_addr(data, 1, 0);

    writel(data->page_addr, data->io_base + CMDQUEUE1(data->cur_chan));
    writel(CMD_COUNT(1) | (data->column / mtd->writesize),
           data->io_base + CMDQUEUE5(data->cur_chan));
    cq6 = CMD_COMPLETE_EN | CMD_FLASH_TYPE(data->flash_type);
    cq6 |= CMD_START_CE(data->sel_chip) | CMD_SPARE_NUM(spare) | CMD_INDEX(SMALL_FIXFLOW_PAGEWRITE);
#ifdef CONFIG_FTNANDC023_USE_AHBDMA
    cq6 |= (1 << 4);
#endif
    status = ftnandc023_nand_check_cmdq(mtd);
    if (status < 0)
        goto out;
    writel(cq6, data->io_base + CMDQUEUE6(data->cur_chan));
#ifdef CONFIG_FTNANDC023_USE_AHBDMA
    memcpy(data->dma_buf, buf, mtd->writesize);
    fa_set_ahb_dma_src_params(AHBDMA_NANDC,
                              (u32) data->dmaaddr, AHBDMA_MASTER_0, AHBDMA_WIDTH_32BIT,
                              AHBDMA_CTL_INC);
    fa_set_ahb_dma_dst_params(AHBDMA_NANDC,
                              (u32) data->chip.IO_ADDR_R, AHBDMA_MASTER_0, AHBDMA_WIDTH_32BIT,
                              AHBDMA_CTL_FIX);
    fa_set_ahb_dma_transfer_params2(AHBDMA_NANDC, AHBDMA_MASTER_0, mtd->writesize, 1);
    fa_ahb_dma_init(AHBDMA_NANDC, AHBDMA_NANDC_REQ);
    fa_ahb_dma_add_auto(AHBDMA_NANDC, mtd->writesize);
    fa_ahb_dma_start(AHBDMA_NANDC);
    status = ftnandc023_dma_wait(mtd);
    fa_ahb_dma_reset(AHBDMA_NANDC);
    if (status < 0) {
        ftnandc023_soft_reset(mtd);
        goto out;
    }
#else
    for (i = 0; i < mtd->writesize; i += 4)
        *(volatile unsigned *)(data->chip.IO_ADDR_R) = *lbuf++;
#endif
    if (ftnandc023_nand_wait(mtd, chip) == NAND_STATUS_FAIL) {
        status = -1;
        goto out;
    }
  out:
    platform_select_pinmux(nand_fd, -1);
    return status;
}

static int ftnandc023_nand_write_page(struct mtd_info *mtd, struct nand_chip *chip,
                                      const uint8_t * buf, int page, int cached, int raw)
{
    struct nand_chip *p = mtd->priv;
    struct ftnandc023_nand_data *data = p->priv;
    int status = 0;
#ifdef CONFIG_MTD_NAND_VERIFY_WRITE
    u8 *vbuf;
    int i;
#endif
    data->page_addr = page;

    platform_select_pinmux(nand_fd, 0);
    write_protect(0);

    status = data->write_page(mtd, buf);
    if (status < 0)
        goto out;

#ifdef CONFIG_MTD_NAND_VERIFY_WRITE
    vbuf = vmalloc(mtd->writesize + mtd->oobsize);
    memcpy(vbuf + mtd->writesize, chip->oob_poi, mtd->oobsize);

    status = data->read_page(mtd, vbuf);
    if (status < 0)
        goto out;
    for (i = 0; i < mtd->writesize; i++) {
        if (*(buf + i) != *(vbuf + i)) {
            printk(KERN_ERR "write verify failed at normal region..");
            goto out_free;
        }
    }
    for (i = 0; i < mtd->oobsize; i++) {
        if (*(chip->oob_poi + i) != *(vbuf + mtd->writesize + i)) {
            printk(KERN_ERR "write verify failed at oob region..\n");
            goto out_free;
        }
    }

  out_free:
    vfree(vbuf);
#endif
  out:
    platform_select_pinmux(nand_fd, -1);
    return status;
}

static void ftnandc023_nand_write_page_lowlevel(struct mtd_info *mtd,
                                                struct nand_chip *chip, const uint8_t * buf)
{
}

static void ftnandc023_nand_cmdfunc(struct mtd_info *mtd, unsigned command,
                                    int column, int page_addr)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    u32 cq6, tmp, i;

    platform_select_pinmux(nand_fd, 0);

    cq6 = CMD_COMPLETE_EN | CMD_FLASH_TYPE(data->flash_type);
    data->cur_cmd = command;
    if (page_addr != -1)
        data->page_addr = page_addr;
    if (column != -1)
        data->column = column;
    switch (command) {
    case NAND_CMD_READID:
        data->byte_ofs = 0;
        cq6 |= CMD_START_CE(data->sel_chip) | CMD_BYTE_MODE | CMD_SPARE_NUM(4);
        if (data->large_page)
            cq6 |= CMD_INDEX(LARGE_FIXFLOW_READID);
        else
            cq6 |= CMD_INDEX(SMALL_FIXFLOW_READID);
        writel(CMD_COUNT(1), data->io_base + CMDQUEUE5(data->cur_chan));
        ftnandc023_nand_check_cmdq(mtd);
        writel(cq6, data->io_base + CMDQUEUE6(data->cur_chan));
        ftnandc023_nand_wait(mtd, chip);
        break;
    case NAND_CMD_RESET:
        cq6 |= CMD_START_CE(data->sel_chip);
        if (data->large_page) {
            if (data->flash_type == 3)
                cq6 |= CMD_INDEX(ONFI_FIXFLOW_SYNCRESET);
            else
                cq6 |= CMD_INDEX(LARGE_FIXFLOW_RESET);
        } else {
            cq6 |= CMD_INDEX(SMALL_FIXFLOW_RESET);
        }
        ftnandc023_nand_check_cmdq(mtd);
        writel(cq6, data->io_base + CMDQUEUE6(data->cur_chan));
        ftnandc023_nand_wait(mtd, chip);
        break;
    case NAND_CMD_STATUS:
        writel(CMD_COUNT(1), data->io_base + CMDQUEUE5(data->cur_chan));
        cq6 |= CMD_START_CE(data->sel_chip);
        if (data->large_page)
            cq6 |= CMD_INDEX(LARGE_FIXFLOW_READSTATUS);
        else
            cq6 |= CMD_INDEX(SMALL_FIXFLOW_READSTATUS);
        ftnandc023_nand_check_cmdq(mtd);
        writel(cq6, data->io_base + CMDQUEUE6(data->cur_chan));
        ftnandc023_nand_wait(mtd, chip);
        break;
    case NAND_CMD_ERASE1:
    		write_protect(0);
        DBGLEVEL2(ftnandc023_dbg("erase page: 0x%x\n", data->page_addr));
        if ((data->page_addr / 32) >= max_chip_size)
            printk("=========== Please improve max_chip_size ==========\n");
        //erase one block = N pages
        tmp = (data->mtd.erasesize / data->mtd.writesize);
        if (tmp <= 32) {
            for (i = 0; i < tmp; i++)
                page_map[data->page_addr / 32] &= ~(1 << (i % 32));
        } else
            for (i = 0; i < tmp / 32; i++)
                page_map[(data->page_addr / 32) + i] = 0;

        writel(data->page_addr, data->io_base + CMDQUEUE1(data->cur_chan));
        writel(CMD_COUNT(1), data->io_base + CMDQUEUE5(data->cur_chan));
        if (data->large_page) {
            ftnandc023_set_row_col_addr(data, 2, 1);
            cq6 |= CMD_INDEX(LARGE_FIXFLOW_ERASE);
        } else {
            ftnandc023_set_row_col_addr(data, 1, 0);
            cq6 |= CMD_INDEX(SMALL_FIXFLOW_ERASE);
        }
        cq6 |= CMD_START_CE(data->sel_chip) | CMD_SCALE(1);
        ftnandc023_nand_check_cmdq(mtd);
        writel(cq6, data->io_base + CMDQUEUE6(data->cur_chan));
        break;
    case NAND_CMD_ERASE2:
    case NAND_CMD_PAGEPROG:
    case NAND_CMD_SEQIN:
        break;
    }
		
		platform_select_pinmux(nand_fd, -1);
}

/*
 * Currently, we have pin mux with SD card
 */
static void ftnandc023_nand_select_chip(struct mtd_info *mtd, int chip)
{
    struct nand_chip *p = mtd->priv;
    struct ftnandc023_nand_data *data = p->priv;
    int chn = 0;

    //DBGLEVEL2(ftnandc023_dbg("chip = %d, ", chip));
    if (data->scan_state != 1) {
        while (chip != -1) {
            if (chip < data->valid_chip[chn]) {
                break;
            } else {
                chip = chip - data->valid_chip[chn];
                chn++;
            }
        }
        data->cur_chan = chn;
    }
#ifdef CONFIG_FTNANDC023_HYNIX_HY27US08561A
    if (chip == 1)
        data->sel_chip = 2;
    else if (chip == 2)
        data->sel_chip = 1;
    else
        data->sel_chip = chip;
#else
    data->sel_chip = chip;
#endif

    //DBGLEVEL2(ftnandc023_dbg("==>chan = %d, ce = %d\n", data->cur_chan, data->sel_chip));
}

/*
 * Probe for the NAND device.
 */
static int __init ftnandc023_nand_probe(struct platform_device *pdev)
{
    struct ftnandc023_nand_data *data;
    int res, chipnum, size;
    int i, type, free_oob_sz;
    u32 val, tmp;

#ifdef CONFIG_MTD_PARTITIONS
    struct mtd_partition *partitions = NULL;
    int num_partitions = 0;
#endif

    res = chipnum = size = type = 0;
    /* Allocate memory for the device structure (and zero it) */
    data = kzalloc(sizeof(struct ftnandc023_nand_data), GFP_KERNEL);
    if (!data) {
        dev_err(&pdev->dev, "failed to allocate device structure.\n");
        res = -ENOMEM;
        goto out;
    }

    data->io_base = ioremap(pdev->resource[0].start,
                            pdev->resource[0].end - pdev->resource[0].start + 1);
    if (data->io_base == NULL) {
        dev_err(&pdev->dev, "ioremap failed for register.\n");
        res = -EIO;
        goto out_free_data;
    }
#ifdef CONFIG_FTNANDC023_USE_AHBDMA
    data->chip.IO_ADDR_R = (void *)pdev->resource[1].start;
#else
    data->chip.IO_ADDR_R = ioremap(pdev->resource[1].start,
                                   pdev->resource[1].end - pdev->resource[1].start + 1);
    if (data->chip.IO_ADDR_R == NULL) {
        dev_err(&pdev->dev, "ioremap failed for data port.\n");
        res = -EIO;
        goto out_no_ior;
    }
#endif

    /* The following setting was done in nsboot already. Actually it don't need to be 
     * configured again. 
     */
    writel(REPORT_ADDR_EN | BUSY_RDY_LOC(6) | CMD_STS_LOC(0) | CE_NUM(0),
           data->io_base + GENERAL_SETTING);

    /* Currently, it is fixed in LEGACY_LARGE
     */
    legacy = LEGACY_LARGE;
    val = readl(data->io_base + MEM_ATTR_SET);
    spare = (val >> 16) & 0x3FF;
    val = readl(data->io_base + ECC_CORRECT_BITREG1);
#ifdef CONFIG_PLATFORM_GM8126
    useecc = (val & 0x1F) + 1;
#else
    useecc = (val & 0x3F) + 1;
#endif 
    eccbasft = ((val >> 16) & 0x1) ? 10 : 9;
    val = readl(data->io_base + GENERAL_SETTING);
    usecrc = ((val >> 16) & 0x1) ? 0 : 1;
    data->flash_type = 0;
    data->large_page = 1;

        /*-----------------------------------------------------------
	 * For ONFI or Toggle High Speed Flash, need to adjust delay.
	 */
    if (data->flash_type > LEGACY_LARGE) {
        val = readl(data->io_base + DQS_DELAY);
        val &= ~0x1F;
        val |= 10;
        writel(val, data->io_base + DQS_DELAY);
    }

    data->chip.priv = data;
    data->mtd.priv = &data->chip;
    data->mtd.owner = THIS_MODULE;
    data->mtd.name = pdev->dev.bus_id;
    data->dev = &pdev->dev;
    data->chip.IO_ADDR_W = data->chip.IO_ADDR_R;
    data->chip.select_chip = ftnandc023_nand_select_chip;
    data->chip.cmdfunc = ftnandc023_nand_cmdfunc;
    data->chip.read_byte = ftnandc023_read_byte;
    data->chip.write_page = ftnandc023_nand_write_page;
    data->chip.waitfunc = ftnandc023_nand_wait;
    data->chip.block_markbad = ftnandc023v2_block_markbad;
    data->chip.chip_delay = 0;
    //data->chip.options = NAND_USE_FLASH_BBT | NAND_NO_SUBPAGE_WRITE | NAND_OWN_BUFFERS;
    data->chip.options = NAND_NO_SUBPAGE_WRITE | NAND_OWN_BUFFERS;      /* remove NAND_USE_FLASH_BBT */

    platform_set_drvdata(pdev, data);

    data->scan_state = 1;

        /*--------------------------------------------------------
	 * ONFI flash must work in Asynch mode for READ ID command.
	 * Switch it back to Legacy.
	 */
    if (data->flash_type == ONFI) {
        type = data->flash_type;
        data->flash_type = 0;
    }
    /* Scan to find existance of the device */
    for (i = startchn; i < MAX_CHANNEL; i++) {
        //printk(KERN_INFO "NAND: Scan Channel %d...\n", i);

        data->cur_chan = i;
        if (!nand_scan_ident(&data->mtd, MAX_CE)) {
            if ((0xFFFFFFFF - size) > (data->mtd.size)
                && ((chipnum + data->chip.numchips) <= NAND_MAX_CHIPS)) {
                data->valid_chip[i] = data->chip.numchips;
                chipnum += data->chip.numchips;
                size += (chipnum * data->chip.chipsize);
            } else {
                printk(KERN_INFO "Can not accept more flash chips.\n");
                break;
            }
        }
    }

    if (chipnum == 0) {
        res = -ENXIO;
        goto out_unset_drv;
    }

    data->chip.numchips = chipnum;
    data->mtd.size = size;
    data->scan_state = 0;

    data->chip.ecc.layout = &nand_hw_eccoob;
    data->chip.bbt_td = &ftnandc023_bbt_main_descr;
    data->chip.bbt_md = &ftnandc023_bbt_mirror_descr;
    data->chip.badblock_pattern = &ftnandc023_largepage_flashbased;

    /* check spare size */
    if (spare != (data->mtd.writesize >> 5))
        panic("NAND: the spare size %d(%d) is wrong! \n", data->mtd.writesize >> 5, spare);

    if (data->mtd.writesize == 2048)
        tmp = 1;
    else if (data->mtd.writesize == 4096)
        tmp = 2;
    else if (data->mtd.writesize == 8192)
        tmp = 3;
    else
        tmp = -1;

    /* check page size */
    val = readl(data->io_base + MEM_ATTR_SET);
    if (tmp != (val & 0x3))
        panic("NAND: the page size %d is wrong! \n", data->mtd.writesize);

    /* check block size */
    tmp = (data->mtd.erasesize / data->mtd.writesize) - 1;
    if (tmp != ((val >> 2) & 0x3FF))
        panic("NAND: the block size %d is wrong! \n", data->mtd.erasesize);

    data->chip.buffers = kmalloc(data->mtd.writesize + (spare << 2), GFP_KERNEL);
    if (!data->chip.buffers) {
        dev_err(&pdev->dev, "failed to allocate chip buffers.\n");
        res = -ENOMEM;
        goto out_unset_drv;
    }

    data->chip.ecc.mode = NAND_ECC_HW;
    data->chip.ecc.size = data->mtd.writesize;
    data->chip.ecc.bytes = 0;
    data->chip.ecc.read_page = ftnandc023_nand_read_page;
    data->chip.ecc.write_page = ftnandc023_nand_write_page_lowlevel;
    data->chip.ecc.read_oob = ftnandc023_nand_read_oob_std;
    data->chip.ecc.write_oob = ftnandc023_nand_write_oob_std;
    data->chip.ecc.read_page_raw = ftnandc023_nand_read_page;

    if (data->large_page) {
        data->read_page = ftnandc023_nand_read_page_lp;
        data->write_page = ftnandc023_nand_write_page_lp;
        data->read_oob = ftnandc023_nand_read_oob_lp;
        data->write_oob = ftnandc023_nand_write_oob_lp;
    } else {
        data->read_page = ftnandc023_nand_read_page_sp;
        data->write_page = ftnandc023_nand_write_page_sp;
        data->read_oob = ftnandc023_nand_read_oob_sp;
        data->write_oob = ftnandc023_nand_write_oob_sp;
    }

#ifdef CONFIG_FTNANDC023_USE_AHBDMA
    /* The following method is changed to use interrupt mode instead of PIO mode 
     */
    if (fa_request_ahb_dma_ch(AHBDMA_NANDC, "ftnandc023", ftnandc023_dma_callback, data,
                              AHBDMA_NANDC, 0 /* DMA */ )) {
        dev_err(&pdev->dev, "AHBDMA channel allocation failed\n");
        res = -ENODEV;
        goto out_free_buf;
    }
    if (eccbasft > 9)
        fa_set_ahb_dma_transfer_bs(AHBDMA_NANDC, 7); //burst 1024 bytes
    else
        fa_set_ahb_dma_transfer_bs(AHBDMA_NANDC, 6); //burst 512 bytes

    writel((1 << AHBDMA_NANDC), DMAC_FTDMAC020_VA_BASE + 0x28);
    data->dma_buf = dma_alloc_coherent(&pdev->dev, data->mtd.writesize, &data->dmaaddr, GFP_KERNEL);
    if (!data->dma_buf) {
        res = -ENOMEM;
        goto out_free_dma;
    }
#endif

    /* read the system header first 
     */
    if (1) {
        /* first disble ecc due to potential different ecc capability 
         */
        unsigned int blk_base, usr_base, cfg_base, sz;
        int free_space;
        img_hdr_t *img_hdr;
#ifdef CONFIG_MTD_PARTITIONS
        struct mtd_partition *partitions;
#endif
        /* read system header 
         */
        data->sys_hdr = kzalloc(data->mtd.writesize, GFP_KERNEL);
        if (data->sys_hdr == NULL)
            panic("NAND: no memory");
        data->page_addr = 0;
        data->chip.oob_poi = data->chip.buffers->databuf + data->mtd.writesize;
        if (data->read_page(&data->mtd, (u_char *) data->sys_hdr) < 0)
            panic("NAND: read system header fail!");

        /* read bad block table 
         */
        data->bi_table = kzalloc(data->mtd.writesize, GFP_KERNEL);
        if (data->bi_table == NULL)
            panic("NAND: no memory");
        /* the bi table is next to system header */
        data->page_addr += 1;
        if (data->read_page(&data->mtd, (u_char *) data->bi_table) < 0)
            panic("NAND: read bad block table fail!");

#ifdef CONFIG_MTD_PARTITIONS
        img_hdr = kmalloc(data->mtd.writesize, GFP_KERNEL);
        if (img_hdr == NULL)
            panic("NAND: allocate memory fail!");
        blk_base = ftnandc023v2_get_blockbase(&data->mtd, data->sys_hdr->linux_addr);
        data->page_addr = blk_base >> data->chip.page_shift;
        if (data->read_page(&data->mtd, (u_char *) img_hdr) < 0)
            panic("NAND: read bad block table fail!");
        if (img_hdr->magic != IMAGE_MAGIC) {
            printk("NAND:The expected magic number is 0x%x, but we got 0x%x \n",
                   IMAGE_MAGIC, img_hdr->magic);
            panic("wrong magic number!");
        }

        partitions = ftnandc023_partition_info;
        /* 
         * arrange the MTD partition, PORTING 
         */
        /* loader, nsboot.bin */
        partitions[MTD_PART_LOADER].offset =
            ftnandc023v2_get_blockbase(&data->mtd, (1 << data->chip.bbt_erase_shift));
            
        /* uboot */
        partitions[MTD_PART_UBOOT].offset =
            ftnandc023v2_get_blockbase(&data->mtd, data->sys_hdr->uboot_addr);  
                      
#ifdef CONFIG_PLATFORM_GM8210            
        /* slave */
        partitions[MTD_PART_SLAVE].offset =
            ftnandc023v2_get_blockbase(&data->mtd, data->sys_hdr->slave_linux_addr);
            
        /* audio */
        partitions[MTD_PART_AUDIO].offset =
            ftnandc023v2_get_blockbase(&data->mtd, data->sys_hdr->audio_addr);      

        /* logo */
        partitions[MTD_PART_LOGO].offset =
            ftnandc023v2_get_blockbase(&data->mtd, data->sys_hdr->logo_addr); 
                        
        /* cfg */
        cfg_base = data->sys_hdr->audio_addr - MTD_CFG_SZ;
        partitions[MTD_PART_CFG].offset = ftnandc023v2_get_blockbase(&data->mtd, cfg_base);

        if (partitions[MTD_PART_CFG].offset <= partitions[MTD_PART_UBOOT].offset + (256 << 10))
            panic("NAND: uboot space %#x is too small!\n",
                  data->sys_hdr->audio_addr - data->sys_hdr->uboot_addr);                    
#else
        /* burn-in */
        partitions[MTD_PART_BURNIN].offset =
            ftnandc023v2_get_blockbase(&data->mtd, data->sys_hdr->burnin_addr);
            
        /* cfg */
        cfg_base = data->sys_hdr->linux_addr - MTD_CFG_SZ;
        partitions[MTD_PART_CFG].offset = ftnandc023v2_get_blockbase(&data->mtd, cfg_base);

        if (partitions[MTD_PART_CFG].offset <= partitions[MTD_PART_UBOOT].offset + (256 << 10))
            panic("NAND: uboot space %#x is too small!\n",
                  data->sys_hdr->linux_addr - data->sys_hdr->uboot_addr);            
#endif   
        /* linux */
        partitions[MTD_PART_LINUX].offset =
            ftnandc023v2_get_blockbase(&data->mtd, data->sys_hdr->linux_addr);

        /* 
         * calculate every partition's MTD size
         */
#ifdef CONFIG_PLATFORM_GM8210 
        /* loader */
        partitions[MTD_PART_LOADER].size =
            partitions[MTD_PART_UBOOT].offset - partitions[MTD_PART_LOADER].offset;
        /* uboot */
        partitions[MTD_PART_UBOOT].size =
            partitions[MTD_PART_AUDIO].offset - partitions[MTD_PART_UBOOT].offset;
        /* audio */
        partitions[MTD_PART_UBOOT].size =
            partitions[MTD_PART_SLAVE].offset - partitions[MTD_PART_AUDIO].offset;            
        /* slave */
        partitions[MTD_PART_SLAVE].size =
            partitions[MTD_PART_LINUX].offset - partitions[MTD_PART_SLAVE].offset;            
        /* cfg */
        partitions[MTD_PART_CFG].size =
            partitions[MTD_PART_AUDIO].offset - partitions[MTD_PART_CFG].offset;
#else          
        /* loader */
        partitions[MTD_PART_LOADER].size =
            partitions[MTD_PART_BURNIN].offset - partitions[MTD_PART_LOADER].offset;
        /* burn-in */
        partitions[MTD_PART_BURNIN].size =
            partitions[MTD_PART_UBOOT].offset - partitions[MTD_PART_BURNIN].offset;
        /* uboot */
        partitions[MTD_PART_UBOOT].size =
            partitions[MTD_PART_CFG].offset - partitions[MTD_PART_UBOOT].offset;
        /* cfg */
        partitions[MTD_PART_CFG].size =
            partitions[MTD_PART_LINUX].offset - partitions[MTD_PART_CFG].offset;
#endif
        /* sanity check to see if the space is big enough (including header) */
        if ((img_hdr->size + data->mtd.writesize + ((256 * 4) << 10)) > MTD_LINUX_SZ)
            panic("NAND: please redefine MTD_LINUX_SZ: %#x \n", MTD_LINUX_SZ);

        /* linux, image header + image size */
        partitions[MTD_PART_LINUX].size = BLOCK_ALIGN(MTD_LINUX_SZ, data->chip.bbt_erase_shift);

        /* 
         * calculate the mtd of user configuration base
         */
        /* user partition */         
#ifdef CONFIG_PLATFORM_GM8210 
        usr_base = data->sys_hdr->logo_addr + MTD_LOGO_SZ;
        partitions[MTD_PART_USER].offset = ftnandc023v2_get_blockbase(&data->mtd, usr_base);
#else         
        usr_base = partitions[MTD_PART_LINUX].offset + partitions[MTD_PART_LINUX].size;
        partitions[MTD_PART_USER].offset = ftnandc023v2_get_blockbase(&data->mtd, usr_base);
#endif        
        if (free_space || sz) {
        }

        kfree(img_hdr);
#endif /* CONFIG_MTD_PARTITIONS */

        /* restore the orginal setting */
        data->page_addr = 0;
    }

    avail_oob_sz = i = ftnandc023_available_oob(&data->mtd);
    /* default value */
    free_oob_sz = 3 /* bytes */  * (data->mtd.writesize >> eccbasft);
#ifdef CONFIG_YAFFS_FS
    free_oob_sz = 16;
#endif
    if (likely(i >= free_oob_sz)) {
        if (i > 64)
            data->mtd.oobsize = 64;
        else
            data->mtd.oobsize = i;
        data->chip.ecc.layout->oobfree[0].length = data->mtd.oobsize;
    } else {
        printk("NAND: oob size is too small! \n");
        res = -ENXIO;
        goto out_unset_drv;
    }

    printk("NAND Chip: oobsize:%#x, pagesize:%#x, blocksize:%#x, chipsize:%#x, "
           "ECC capbility is %d bits, CRC protection is %s\n",
           (int)spare, (int)data->mtd.writesize, (int)data->mtd.erasesize, (int)data->chip.chipsize,
           useecc, usecrc ? "enabled" : "disabled");

#if 1                           /* Harry, in MTD utility, the oobsize should be multiple of 64. We should be on the way. */
    data->mtd.oobsize = spare;
    spare = (avail_oob_sz / (data->mtd.writesize >> eccbasft)) - 1;
#else
    printk("NAND: total oobsize: %d\n", data->mtd.oobsize);
    spare = (data->mtd.oobsize / (data->mtd.writesize >> eccbasft)) - 1;
    printk("NAND: oobsize per sector: %d\n", spare + 1);
#endif
    /* Scan bad block and create bbt table
     */
    nand_scan_tail(&data->mtd);

        /*----------------------------------------------------------
	 * ONFI synch mode means High Speed. If fails to change to
	 * Synch mode, then use flash as Async mode(Normal speed) and
	 * use LEGACY_LARGE fix flow.
	 */
    if (type == ONFI) {
        if (ftnandc023_onfi_sync(&data->mtd) == 0)
            data->flash_type = ONFI;
        else
            data->flash_type = 0;
    }

    ftnandc023_calc_timing(&data->mtd);

#ifdef CONFIG_MTD_PARTITIONS
#ifdef CONFIG_MTD_CMDLINE_PARTS
    num_partitions = parse_mtd_partitions(&data->mtd, part_probes, &partitions, 0);
#endif
    if (num_partitions <= 0) {
        partitions = ftnandc023_partition_info;
        num_partitions = ARRAY_SIZE(ftnandc023_partition_info);
    }
    res = add_mtd_partitions(&data->mtd, partitions, num_partitions);
#else
    res = add_mtd_device(&data->mtd);
#endif
    if (!res)
        return res;

    nand_release(&data->mtd);
#ifdef CONFIG_FTNANDC023_USE_AHBDMA
  out_free_dma:
    fa_free_ahb_dma(AHBDMA_NANDC);
  out_free_buf:
#endif
    kfree(data->chip.buffers);

  out_unset_drv:
    platform_set_drvdata(pdev, NULL);

#ifndef CONFIG_FTNANDC023_USE_AHBDMA
    iounmap(data->chip.IO_ADDR_R);
#endif
    iounmap(data->io_base);
  out_free_data:
    if (data->sys_hdr)
        kfree(data->sys_hdr);
    if (data->bi_table)
        kfree(data->bi_table);
    kfree(data);
  out:
    return res;
}

/*
 * @consult with bad block table about this block is good or bad.
 *
 * @ftnandc023v2_read_bbt(struct mtd_info *mtd, loff_t offs)
 * @param mtd: MTD device structure
 * @param offs: block base address
 * @return: 1 for bad block, 0 for good block
*/
int ftnandc023v2_read_bbt(struct mtd_info *mtd, loff_t offs)
{
    struct nand_chip *chip = mtd->priv;
    struct ftnandc023_nand_data *data = chip->priv;
    unsigned int *bi_table = (unsigned int *)data->bi_table;
    int quotient, remainder, blk_id, result;

    blk_id = offs >> chip->bbt_erase_shift;
    quotient = blk_id >> 5;
    remainder = blk_id & 0x1F;
    result = (bi_table[quotient] >> remainder) & 0x1;

    if (result == 1)
        return 0;               /* good */

    return 1;                   /* bad */
}

/**
 * ftnandc023v2_default_block_markbad - mark a block bad
 * @mtd:	MTD device structure
 * @ofs:	offset from device start
 *
 * This is the default implementation, which can be overridden by
 * a hardware specific driver.
*/
static int ftnandc023v2_block_markbad(struct mtd_info *mtd, loff_t ofs)
{
    /* the purpose of function should change bi byte in Spare area. But in nandc023, it was
     * destroyed. So here we left nothing.
     */
    if (mtd || ofs) {
    }

    printk("NAND: This function is not implemented! Please have a call if you need it! \n");

    return 0;
}

/*
 * @get block baseaddr. It skips the bad block as well.
 *
 * @ftnandc023v2_get_blockbase(struct mtd_info *mtd, unsigned int base_addr)
 * @param mtd: MTD device structure
 * @param base_addr: liner base address
 * @return: good block address
*/
unsigned int ftnandc023v2_get_blockbase(struct mtd_info *mtd, unsigned int base_addr)
{
    struct nand_chip *chip = mtd->priv;
    unsigned int block_base;

    block_base = BLOCK_ALIGN(base_addr, chip->bbt_erase_shift);
    while (ftnandc023v2_read_bbt(mtd, (loff_t) block_base) != 0)        /* find good block */
        block_base += (0x1 << chip->bbt_erase_shift);   //move to next block

    return block_base;
}

/*
 * Remove a NAND device.
 */
static int __devexit ftnandc023_nand_remove(struct platform_device *pdev)
{
    struct ftnandc023_nand_data *data = platform_get_drvdata(pdev);

#ifdef CONFIG_FTNANDC023_USE_AHBDMA
    dma_free_coherent(&pdev->dev, data->mtd.writesize, data->dma_buf, data->dmaaddr);
    fa_free_ahb_dma(AHBDMA_NANDC);
#endif
    nand_release(&data->mtd);
    iounmap(data->io_base);
    kfree(data->chip.buffers);
    kfree(data);

    return 0;
}

static void ftnandc023_release(struct device *dev)
{
}

static u64 ftnandc023_dmamask = DMA_BIT_MASK(32);

static struct platform_device ftnandc023_device = {
    .name = "ftnandc023_nand",
    .id = -1,
    .num_resources = ARRAY_SIZE(ftnandc023_resource),
    .resource = ftnandc023_resource,
    .dev = {
            .dma_mask = &ftnandc023_dmamask,
            .coherent_dma_mask = DMA_BIT_MASK(32),
            .release = ftnandc023_release,
            },
};

static struct platform_driver ftnandc023_nand_driver = {
    .probe = ftnandc023_nand_probe,
    .remove = ftnandc023_nand_remove,
    .driver = {
               .name = "ftnandc023_nand",
               .owner = THIS_MODULE,
               },
};

static int __init ftnandc023_nand_init(void)
{
    int ret = 0;

    /* check if the system is running NAND system 
     */
#ifdef CONFIG_PLATFORM_GM8210
    if (platform_check_flash_type() != 0)
#else     
    if (platform_check_nandc() < 0)
#endif    	
        return 0;

    init_waitqueue_head(&nand023_queue);

    /* Register PMU and turn on gate clock
     */
    nand_fd = ftpmu010_register_reg(&pmu_reg_info);
    if (nand_fd < 0)
        panic("NANDC: register PMU fail");

#ifdef CONFIG_GPIO_WP
    if ((ret = gpio_request(GPIO_PIN, PIN_NAME)) != 0) {
        printk("gpio request fail\n");
        return ret;
    }
    printk("register GPIO for NAND write protect\n");
#endif

    if (platform_device_register(&ftnandc023_device)) {
        printk(KERN_ERR "device register failed\n");
        ret = -ENODEV;
    }
    if (platform_driver_register(&ftnandc023_nand_driver)) {
        printk(KERN_ERR "driver register failed\n");
        ret = -ENODEV;
    }
    return ret;
}

static void __exit ftnandc023_nand_exit(void)
{
    /* check if the system is running NAND system 
     */
#ifdef CONFIG_PLATFORM_GM8210
    if (platform_check_flash_type() != 0)
#else
    if (platform_check_nandc() < 0)
#endif    	
        return;

    /* Deregister PMU
     */
    ftpmu010_deregister_reg(nand_fd);

    platform_driver_unregister(&ftnandc023_nand_driver);
    platform_device_unregister(&ftnandc023_device);
}

/*
 * @callback function from DMAC module
 *
 * @ftnandc023_dma_callback int func(int ch, u16 int_status, void *data)
 * @param ch is used to indicate DMA channel
 * @param int_status indicates the interrupt status of DMA controller
 * @param data indicates the private data
 * @return: none
*/
void ftnandc023_dma_callback(int ch, u16 int_status, void *data)
{
    if (ch != AHBDMA_NANDC) {
        printk("NAND: error dma channel... \n");
        return;
    }

    trigger_flag = 1;
    wake_up_interruptible(&nand023_queue);

    return;
}

module_init(ftnandc023_nand_init);
module_exit(ftnandc023_nand_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Te-Chen Ying");
MODULE_DESCRIPTION("FTNANDC023 V2.0 NAND driver");
MODULE_ALIAS("platform:ftnandc023_nand");

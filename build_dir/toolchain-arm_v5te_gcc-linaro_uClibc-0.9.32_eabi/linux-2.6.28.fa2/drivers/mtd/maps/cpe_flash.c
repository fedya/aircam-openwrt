/*
 * Flash on CPE mx29lv
 *
 * $Id: cpe-flash.c,v
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <mach/platform/spec.h>
#include <asm/io.h>

static struct mtd_info *mymtd = NULL;

#define BUSWIDTH_8BIT   1
#define BUSWIDTH_16BIT  2

#if defined(CONFIG_PLATFORM_GM8181)
struct map_info physmap_map = {
		.name =		"GM 8-bit flash device",
		.bankwidth = BUSWIDTH_8BIT,
};
#else
struct map_info physmap_map = {
		.name =		"GM 16-bit flash device",
		.bankwidth = BUSWIDTH_16BIT,
};
#endif

#if defined(CONFIG_PLATFORM_GM8181)
struct mtd_partition flash_parts[] = {

	{
		.name =		"BurnIn",	// for system usage
		.offset	=	0x0,		// Burn-in ,start addr MTD_PA_BASE
		.size =		0x100000	// 1MB
	},	
	{
		.name =		"User",		// free for use
		.offset =	0x100000,	// start addr MTD_PA_BASE + 0x100000
		.size =		0x100000	// 1MB
	},
	{
		.name =		"UBoot",	// for system usage
		.offset	=	0x200000,	// start addr MTD_PA_BASE + 0x200000
		.size =		0x40000	    // 256K
	},
	{
		.name =		"Linux",	// for system usage
		.offset	=	0x240000,	// Kernel/Root FS ,start addr MTD_PA_BASE + 0x240000
		.size =		0xDC0000	// 13.75MB
	}
};
#endif /* mtd_partition */

#define PARTITION_COUNT (sizeof(flash_parts)/sizeof(struct mtd_partition))


int __init init_flagadm(void)
{
	int i = 0, mode = 0;
	unsigned int value = 0, FLASH_SIZE = 0;

#if defined(CONFIG_PLATFORM_GM8181)
	//calculate flash size, if you use 2 chip or 1 chip is not flash, you can define the size directly
	value = *(volatile unsigned int *)(SRAM_FTSMC010_VA_BASE);	
	i = (value & 0xF0) >> 4;    //read BNK_SIZE
	if (i >= 11)
		FLASH_SIZE = 1 << (i + 4);
	else
		FLASH_SIZE = 1 << (i + 20);

    FLASH_SIZE = FLASH_SIZE * (1 << (value & 0x3));
    
    /* 00: GPIO[7:6]/SMC pin-out, 10: NAND pin out */
    *(volatile unsigned int *)(PMU_FTPMU010_VA_BASE + 0x50) &= 0xFFFFFFFC;
#endif /* CONFIG_PLATFORM_GM8181 */

	printk(KERN_NOTICE "Flash device: SIZE 0x%x at ADDR 0x%x\n",
			FLASH_SIZE, MTD_PA_BASE);

	physmap_map.phys = MTD_PA_BASE;
	physmap_map.size = FLASH_SIZE;

	mode = (*((unsigned int *)SRAM_FTSMC010_VA_BASE) & 0x3);
    	if((mode & 0x3) == 0)
       	   printk("Flash is 8 bit mode\n");
    	else if ((mode & 0x3) == 1)
    	   printk("Flash is 16 bit mode\n");
    	else
    	   printk("This Flash mode not support\n");

       	printk("MTD: Init %s: (size=%dMB) start at address=0x%x\n", physmap_map.name, (FLASH_SIZE/1024)/1024, MTD_VA_BASE);
	physmap_map.virt = (void __iomem *)MTD_VA_BASE;

	if (!physmap_map.virt) {
		printk("MTD: ioremap fail\n");
		return -EIO;
	}

	simple_map_init(&physmap_map);
	
	mymtd = do_map_probe("cfi_probe", &physmap_map);
	if (mymtd) {
		mymtd->owner = THIS_MODULE;
		add_mtd_partitions(mymtd, flash_parts, PARTITION_COUNT);
		printk("MTD flash device initialized\n");
		return 0;
	}
	else
		printk("MTD flash device initialized fail\n");

	return -ENXIO;
}

static void __exit cleanup_flagadm(void)
{
	if (mymtd) {
		del_mtd_partitions(mymtd);
		map_destroy(mymtd);
	}
	if (physmap_map.virt) {
		physmap_map.virt = 0;
	}
}

module_init(init_flagadm);
module_exit(cleanup_flagadm);

MODULE_AUTHOR("GM Corp.");
MODULE_LICENSE("GM License");

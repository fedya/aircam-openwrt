/**
 * @file platform.c
 *  Introduction:
 *      The file is a platform dependent porting file. It configures the 
 *      pmu setting for NAND such as pinmux, or gate clock...
 *
 * Copyright (C) 2010 GM Corp. (http://www.grain-media.com)
 *
 * $Revision: 1.10 $ 
 * $Date: 2011/08/30 02:32:54 $ 2010/8/26 03:22
 *
 * ChangeLog:
 *  $Log: platform.c,v $
 *  Revision 1.10  2011/08/30 02:32:54  easonli
 *  *:[nand] modify pinmux protection by ftpmu010
 *
 *  Revision 1.9  2011/08/30 02:00:08  wdshih
 *  add GM8210
 *
 *  Revision 1.8  2011/01/14 01:56:12  harry_hs
 *  add delay to prevent data loss
 *
 *  Revision 1.7  2011/01/13 13:42:27  harry_hs
 *  pinmux
 *
 *  Revision 1.6  2011/01/13 13:41:14  harry_hs
 *  pinmux
 *
 *  Revision 1.5  2011/01/12 01:42:24  harry_hs
 *  modify pinmux with SD
 *
 *  Revision 1.4  2011/01/11 05:06:38  harry_hs
 *  pinmux with SD
 *
 *  Revision 1.3  2011/01/10 13:47:39  harry_hs
 *  wrong pinmux bug fix
 *
 *  Revision 1.2  2011/01/10 08:25:45  harry_hs
 *  add pinmux with SD card
 *
 *  Revision 1.1  2010/09/08 02:04:18  harry_hs
 *  first version
 *
 */
#include <linux/interrupt.h> 
#include <mach/ftpmu010.h>

#ifdef CONFIG_PLATFORM_GM8126
static pmuReg_t  pmu_reg[] = {
    /* off, bitmask,   lockbit,    init val,   init mask */
    {0x38, (0x1 << 5), (0x1 << 5), (0x0 << 5), (0x1 << 5)},   /* clock gate */
    {0x50, (0x3 << 14),(0x3 << 14),(0x3 << 14),(0x3 << 14)},  /* NAND_BUSY pin with UART2 */
    {0x5C, 0x0FFFFFFF, 0x0FFFFF00, 0x05555555, 0x0FFFFFFF},   /* pinMux with GPIO/SD */
    {0x6C, (0x3 << 21),(0x0 << 21),(0x0 << 21),(0x1 << 21)},  /* pinMux with SD */
    //{0x6C, (0x7 << 20),(0x0 << 21),(0x3 << 20),(0x3 << 20)},  /* pinMux with SD */
};

static pmuRegInfo_t	pmu_reg_info = {
    "NANDC023",
    ARRAY_SIZE(pmu_reg),
    ATTR_TYPE_NONE,
    pmu_reg
};

/*
 * @the purpose of this function is used to check if the system is running NAND
 *
 * @platform_check_nandc(void)
 * @param : none 
 * @return: 0 for system running NAND, -1 for not
 */
int platform_check_nandc(void)
{
    volatile unsigned int value;
    
    value = ftpmu010_read_reg(0x04) & 0xFF;
    if (value & (0x1 << 5))    /* 0: SPI, 1: NAND */
        return 0;
    
    return -1;        
}

static int  ref_count = 0;
/*
 * @the purpose of this function is used to switch the pinmux
 *
 * @int platform_select_pinmux(int fd)
 * @param : fd 
 * @return: 0 for success, < 0 for fail.
 */
int platform_select_pinmux(int fd, int chip)
{
    int ret = 0;
    volatile int i;
    
    if (chip >= 0) {
        if (ref_count == 0) {
#if 0
            /* 0x6C
             */
            for(;;){//while (1) {
                if (ftpmu010_pimnux_request(fd, 0x6C, (0x1 << 22)) == 0)
                    break;
                set_current_state(TASK_UNINTERRUPTIBLE);
                schedule_timeout(HZ / 100);
            }
#endif
#if defined(CONFIG_PLATFORM_GM8126)
		if (ftpmu010_request_pins(fd, 0x6C, (0x1 << 22), 1) < 0) {
			panic("%s: request pin failed\n", __func__);
		}
#endif
            
            /* the delay is used to prevent data in the bus from incomplete */
            for (i = 0; i < 1000; i++) {}            
            
            ftpmu010_write_reg(fd, 0x6C, (0x0 << 22), (0x1 << 22));
        }
        ref_count ++;
    }
    else {
        ref_count -- ;
        if (ref_count == 0) {
            /* chip unselect, the delay is used to prevent data in the bus from incomplete */
            for (i = 0; i < 1000; i++) {}
            //ftpmu010_del_lockbits(fd, 0x6C, (0x1 << 22));
#if defined(CONFIG_PLATFORM_GM8126)
            ftpmu010_write_reg(fd, 0x6C, (0x1 << 22), (0x1 << 22)); //to SD
	    ftpmu010_release_pins(fd, 0x6C, (0x1 << 22));
#endif
        }
    }
    
    return ret;
}
#elif defined(CONFIG_PLATFORM_GM8210_M)
static pmuReg_t  pmu_reg[] = {
    /* off, bitmask,   lockbit,    init val,   init mask */
    {0x38, (0x1 << 5), (0x1 << 5), (0x0 << 5), (0x1 << 5)},   /* clock gate */
    {0x50, (0x3 << 14),(0x3 << 14),(0x3 << 14),(0x3 << 14)},  /* NAND_BUSY pin with UART2 */
    {0x5C, 0x0FFFFFFF, 0x0FFFFF00, 0x05555555, 0x0FFFFFFF},   /* pinMux with GPIO/SD */
    {0x6C, (0x3 << 21),(0x0 << 21),(0x0 << 21),(0x1 << 21)},  /* pinMux with SD */
    //{0x6C, (0x7 << 20),(0x0 << 21),(0x3 << 20),(0x3 << 20)},  /* pinMux with SD */
};

static pmuRegInfo_t	pmu_reg_info = {
    "NANDC023",
    ARRAY_SIZE(pmu_reg),
    ATTR_TYPE_NONE,
    pmu_reg
};

/*
 * @the purpose of this function is used to check if the system is running NAND
 *
 * @platform_check_nandc(void)
 * @param : none 
 * @return: 0 for system running NAND, -1 for not
 */
int platform_check_nandc(void)
{
    volatile unsigned int value;

    value = (ftpmu010_read_reg(0x04) >> 21) & 0x1;
    if (value)    /* 0: NOR, 1: SPI/NAND */
        return 0;
   
    return -1;        
}

static int  ref_count = 0;
/*
 * @the purpose of this function is used to switch the pinmux
 *
 * @int platform_select_pinmux(int fd)
 * @param : fd 
 * @return: 0 for success, < 0 for fail.
 */
int platform_select_pinmux(int fd, int chip)
{
    int ret = 0;
    volatile int i;
    
    if (chip >= 0) {
        if (ref_count == 0) {
            /* 0x6C
             */
            while (1) {
                if (ftpmu010_bits_is_locked(fd, 0x6C, (0x1 << 22)) < 0)
                    break;
                set_current_state(TASK_UNINTERRUPTIBLE);
                schedule_timeout(HZ / 100);
            }
            /* chip select */
            ret = ftpmu010_add_lockbits(fd, 0x6C, (0x1 << 22));
            if (ret < 0)
                panic("NAND err: Bits in 0x6C conflict with other module! \n");
            
            /* the delay is used to prevent data in the bus from incomplete */
            for (i = 0; i < 1000; i++) {}            
            
            ftpmu010_write_reg(fd, 0x6C, (0x0 << 22), (0x1 << 22));
        }
        ref_count ++;
    }
    else {
        ref_count -- ;
        if (ref_count == 0) {
            /* chip unselect, the delay is used to prevent data in the bus from incomplete */
            for (i = 0; i < 1000; i++) {}
            ftpmu010_del_lockbits(fd, 0x6C, (0x1 << 22));
            ftpmu010_write_reg(fd, 0x6C, (0x1 << 22), (0x1 << 22)); //to SD
            
        }
    }
    
    return ret;
}
#else
#error "NANDC023: platform is not support!"
#endif

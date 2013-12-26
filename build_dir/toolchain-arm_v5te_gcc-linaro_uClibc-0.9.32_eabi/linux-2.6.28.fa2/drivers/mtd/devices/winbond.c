/** 
 * @file winbond.c
 * @brief this file complte winbond compatible SPI FLASH driver
 * @author Grain Media Corp (C)
 * @date 2010-08-26
 *
 * $Revisio$
 * $Date: 2012/04/24 08:58:04 $
 *
 * ChangeLog
 *   $Log: winbond.c,v $
 *   Revision 1.25  2012/04/24 08:58:04  wdshih
 *   *** empty log message ***
 *
 *   Revision 1.24  2012/04/13 06:10:49  wdshih
 *   add ESMT SPI flash
 *
 *   Revision 1.23  2012/02/10 07:10:00  wdshih
 *   add GD spi flash
 *
 *   Revision 1.22  2011/11/23 06:07:51  wdshih
 *   add 8210 partation
 *
 *   Revision 1.21  2011/09/14 09:33:31  wdshih
 *   *** empty log message ***
 *
 *   Revision 1.20  2011/09/14 03:36:20  wdshih
 *   fix 4 byte mode reboot issue and modify CFG partation address
 *
 *   Revision 1.19  2011/09/13 09:52:44  wdshih
 *   fix spansion mount MTD issue
 *
 *   Revision 1.18  2011/09/08 09:21:45  wdshih
 *   add spansion flash
 *
 *   Revision 1.17  2011/08/24 01:23:18  wdshih
 *   add EN25QH256 flash
 *
 *   Revision 1.16  2011/07/08 09:14:10  wdshih
 *   fix wait ready issue and add MX25L256 spi flash function
 *
 *   Revision 1.15  2011/06/27 01:14:33  wdshih
 *   fix SPI flash write busy issue
 *
 *   Revision 1.14  2011/04/18 05:55:53  harry_hs
 *   remove PMU_FTPMU010_VA_BASE
 *
 *   Revision 1.13  2011/03/24 05:54:30  wdshih
 *   add spi flash
 *
 *   Revision 1.12  2011/03/22 06:00:31  wdshih
 *   add EN25Q128 flash
 *
 *   Revision 1.11  2011/01/13 03:32:27  mars_ch
 *   *: recover jumper setting check to enable SPI flash
 *
 *   Revision 1.10  2010/12/27 09:04:19  mars_ch
 *   *: add MXIC SPI flash and a new winbond 8MB flash
 *
 *   Revision 1.9  2010/11/12 08:43:00  mars_ch
 *   *: support the combined SPI flash, means two pyhsical ones into a logical bigger one
 *
 *   Revision 1.8  2010/11/08 12:56:38  mars_ch
 *   *: add 8126 with second SPI flash support
 *
 *   Revision 1.7  2010/09/24 11:27:15  mars_ch
 *   *: move spi_board_info from driver to arch to fix dirty hack of module init order issues
 *
 *   Revision 1.6  2010/09/17 08:34:36  mars_ch
 *   *: remove useless MTD flash size flag
 *
 *   Revision 1.5  2010/09/15 11:24:06  mars_ch
 *   *: add loader section
 *
 *   Revision 1.4  2010/09/14 04:29:25  mars_ch
 *   *: add MTD_FLASH_TOTAL_SIZE and clean up some dirty hard code
 *
 *   Revision 1.3  2010/09/01 02:39:31  mars_ch
 *   *: change MTD user setting parameter
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/io.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <mach/ftpmu010.h>

#define DEBUG_WINBOND_SPI_FLASH		0

#if DEBUG_WINBOND_SPI_FLASH
#define WINBOND_DEG_PRINT(FMT, ARGS...)    printk(FMT, ##ARGS)
#else
#define WINBOND_DEG_PRINT(FMT, ARGS...)
#endif

#define WINBOND_SPI_FLASH_JEDEC_M_ID 	 0xEF
#define MXIC_SPI_FLASH_JEDEC_M_ID       0xC2
#define NUMONYX_SPI_FLASH_JEDEC_M_ID    0x20
#define EON_SPI_FLASH_JEDEC_M_ID        0x1C
#define ESMT_SPI_FLASH_JEDEC_M_ID       0x8C
#define SELF_SPI_FLASH_JEDEC_M_ID       0x68
#define GD_SPI_FLASH_JEDEC_M_ID    		 0xC8
#define SPANSION_SPI_FLASH_JEDEC_M_ID   0x01

#define CMD_W25_WREN            	0x06    /* Write Enable */
#define CMD_W25_WRDI            	0x04    /* Write Disable */
#define CMD_W25_RDSR            	0x05    /* Read Status Register */
#define CMD_W25_WRSR            	0x01    /* Write Status Register */
#define CMD_W25_READ            	0x03    /* Read Data Bytes */
#define CMD_W25_FAST_READ       	0x0B    /* Read Data Bytes at Higher Speed */
#define CMD_W25_PP              	0x02    /* Page Program */
#define CMD_W25_SE4K              	0x20    /* Sector (4K) Erase */
#define CMD_W25_BE32K             	0x52    /* Block (32K) Erase */
#define CMD_W25_BE64K             	0xD8    /* Block (64K) Erase */
#define CMD_W25_CE              	0xC7    /* Chip Erase */
#define CMD_W25_DP              	0xB9    /* Deep Power-down */
#define CMD_W25_RES             	0xAB    /* Release from DP, and Read Signature */
#define CMD_W25_RD_JEDEC_ID		    0x9F

#define WINBOND_ID_NOCHECK						 0x7878
#define WINBOND_ID_W25P16               0x2015
#define WINBOND_ID_W25X16               0x3015
#define WINBOND_ID_W25X32               0x3016
#define WINBOND_ID_W25X64               0x3017
#define WINBOND_ID_W25Q64CV             0x4017
#define WINBOND_ID_W25Q128BV            0x4018

#define MXIC_ID_MX25L12845E             0x2018
#define MXIC_ID_MX25L25635E             0x2019
#define NUMONYX_ID_N25Q128              0xBA18
#define EON_ID_EN25Q128                 0x3018
#define EON_ID_EN25QH256                0x7019

#define GD_ID_GD25Q64B									0x4017
#define GD_ID_GD25Q128B								0x4018

#define ESMT_ID_F25L32QA								0x4116
#define ESMT_ID_F25L64QA								0x4117

#define SELF_ID                 				0x1234

#define SPSN_ID_S25FL128P	0x2018
#define SPANSION_ID_s25sl12801     		0x0301
#define SPANSION_ID_s25sl12800     		0x0300

#define WINBOND_SR_WIP          (1 << 0)        /* busy state */
#define WINBOND_SR_WEL          (1 << 1)        /* write-enable state */

#define MAX_READY_WAIT_COUNT    10000000

//#define CMD_W25_SIZE	        4
//#define FAST_READ_DUMMY_BYTE    0
#define GM_SPI_FLASH_PAGESIZE	256
//#define GM_SPI_FLASH_TOTALSIZE  (CONFIG_MTD_FLASH_TOTAL_SIZE*1024*1024)

#ifdef CONFIG_MTD_PARTITIONS
#define	mtd_has_partitions()	(1)
#else
#define	mtd_has_partitions()	(0)
#endif

//=============================================================================
// System Header, size = 256 bytes
//=============================================================================
struct flash_system_header {
    char signature[8];          /* Signature is "GM8126" */
    unsigned int bootm_addr;    /* Image offset to load by spiboot */
    unsigned int burnin_addr;   /* burn-in image address */
    unsigned int uboot_addr;    /* uboot address */
    unsigned int linux_addr;    /* linux image address */
#ifdef CONFIG_PLATFORM_GM8210
    unsigned int slave_linux_addr;          /* slave linux image address */
    unsigned int audio_addr;                /* audio image address */
    unsigned int logo_addr;                /* logo image address */
    unsigned int reserved1[4];  /* unused */
#else    
    unsigned int reserved1[7];  /* unused */
#endif    

    struct {
        unsigned int pagesz_log2;       // page size with bytes in log2
        unsigned int secsz_log2;        // sector size with bytes in log2
        unsigned int chipsz_log2;       // chip size with bytes in log2
        unsigned int reserved[10];      // unused
    } norfixup;

    unsigned int reserved2[37]; // unused
    unsigned char mark[4];      // byte254:0x55, byte255:0xAA
};

struct flash_image_header {
    unsigned int magic;         /* Image header magic number (0x805A474D) */
    unsigned int chksum;        /* Image CRC checksum */
    unsigned int size;          /* Image size */
    unsigned int unused;
    unsigned char name[80];     /* Image name */
    unsigned char reserved[160];        /* Reserved for future */
};

struct winbond_spi_flash_info {
    const char *name;
    u32 id;
    u8 byte_mode;	/* 3: 3 byte mode, 4:4 byte mode(over 128Mb flash) */
    u8 log2_page_size;
    u16 pages_per_sector;
    u16 sectors_per_block;
    u16 nr_blocks;
    u32 nr_bytes;
};

/*
 * logical flash's alias in kernel memory, it might contain an auxiliary flash
 * A logical SPI flash means that a structure of spi_board_info in arch/arm/mach-GM/platform-GMxxxx/platformxxx.c
 */
struct winbond_spi_flash {
    ///physical flash info
    struct winbond_spi_flash_info *info;
    ///send cmd by spi
    struct spi_device *spi;
    ///lock for this logical flash
    struct mutex lock;
    ///mtd info for kernel, including partion table
    struct mtd_info mtd;
    ///usually partioned for logical flash, not partioned for auxiliary flash
    u8 partitioned:1;
    ///might be different erase size so that we need to know the corresponding command
    u8 erase_opcode;
    ///spi command container
    //u8 command[CMD_W25_SIZE + FAST_READ_DUMMY_BYTE];
    u8 command[5];
    ///ID for multiple logical flash
    u8 device_no;
    ///auxiliary flash pointer
    struct winbond_spi_flash *aux;
};

struct winbond_spi_flash_read_cxt {
    size_t *retlen;
    u_char *buf;
};

struct winbond_spi_flash_write_cxt {
    size_t *retlen;
    const u_char *buf;
};

/*
 * global counter to indicate the number of logical flash probed
 */
static u8 flash_count = 0;

static struct winbond_spi_flash_info winbond_spi_flash_info_table[] = {
	{
		.id			= WINBOND_ID_NOCHECK,
		.byte_mode			= 3,
		.log2_page_size		= 8, //256bytes
		.pages_per_sector	= 16,//1//16,
		.sectors_per_block	= 16,//256//16,//1,
		.nr_blocks		= 16,//d1
		.name			= "WB_NOCHECK",
	},
    {	
     .name = "W25GENERIC",
     .id = 0,
     .byte_mode			= 3,
     .log2_page_size = 8,       ///256bytes
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks      = 128,
     .nr_bytes = 0x800000 ///8 MB
     },
    {
     .name = "W25X16",
     .id = WINBOND_ID_W25X16,
     .byte_mode			= 3,
     .log2_page_size = 8,
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks = 32,
     .nr_bytes = 0x200000///2 MB
     },
    {
     .name = "W25X32",
     .id = WINBOND_ID_W25X32,
     .byte_mode			= 3,
     .log2_page_size = 8,
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks = 64,
     .nr_bytes = 0x400000///4 MB
     },
    {
     .name = "W25X64",
     .id = WINBOND_ID_W25X64,
     .byte_mode			= 3,
     .log2_page_size = 8,
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks = 128,
     .nr_bytes = 0x800000///8 MB
    },
    { //Mars Cheng 20101125
     .name       = "W25Q64CV",
     .id         = WINBOND_ID_W25Q64CV,
     .byte_mode			= 3,
     .log2_page_size       = 8, ///256bytes
     .pages_per_sector   = 16,
     .sectors_per_block  = 16,
     .nr_blocks      = 128,
     .nr_bytes = 0x800000 ///8 MB
    },
    {
     .name = "W25Q128BV",
     .id = WINBOND_ID_W25Q128BV,
     .byte_mode			= 3,
     .log2_page_size = 8,       ///256bytes
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks = 256,
     .nr_bytes = 0x1000000///16 MB
     }
};///end of winbond_spi_flash_info_table

static struct winbond_spi_flash_info mxic_spi_flash_info_table[] = {
    {
     .name = "MX25L12845E",
     .id = MXIC_ID_MX25L12845E,
     .byte_mode			= 3,
     .log2_page_size = 8,       ///256bytes
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks      = 256,
     .nr_bytes = 0x1000000 ///16 MB
     },
    {
     .name = "MX25L25635E",
     .id = MXIC_ID_MX25L25635E,
     .byte_mode			= 4,
     .log2_page_size = 8,      
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks      = 512,
     .nr_bytes = 0x2000000 ///32 MB
    }     
};

static struct winbond_spi_flash_info numonyx_spi_flash_info_table[] = {
    {
     .name = "N25Q128",
     .id = NUMONYX_ID_N25Q128,
     .byte_mode			= 3,
     .log2_page_size = 8,       ///256bytes
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks      = 256,
     .nr_bytes = 0x1000000 ///16 MB
     },
};

static struct winbond_spi_flash_info eon_spi_flash_info_table[] = {
    {
     .name = "EN25Q128",
     .id = EON_ID_EN25Q128,
     .byte_mode			= 3,
     .log2_page_size = 8,       ///256bytes
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks      = 256,
     .nr_bytes = 0x1000000 ///16 MB
     },
    {
     .name = "EN25QH256",
     .id = EON_ID_EN25QH256,
     .byte_mode			= 4,
     .log2_page_size = 8,       
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks      = 512,
     .nr_bytes = 0x2000000 ///32 MB
    }          
};

static struct winbond_spi_flash_info spansion_spi_flash_info_table[] = {
    {
		
	 .name = "s25sl12800",
	 .id = SPSN_ID_S25FL128P,
	 .byte_mode		   = 3,
	 .log2_page_size = 8, //(256B)
	 .pages_per_sector = 256,//1026,
	 .sectors_per_block = 1,
	 .nr_blocks		= 256,//16,//(1M)
	 .nr_bytes = 0x1000000 ///16 MB
	
#if 0		
     .name = "s25sl12800",
     .id = SPANSION_ID_s25sl12800,
     .byte_mode			= 3,
     .log2_page_size = 8, //(256B)
     .pages_per_sector = 1024,//1026,
     .sectors_per_block = 16,
     .nr_blocks      = 16,//16,//(1M)
     .nr_bytes = 0x1000000 ///16 MB

},
    {
     .name = "s25sl12801",
     .id = SPSN_ID_S25FL128P,
     .byte_mode			= 3,
     .log2_page_size = 8,       ///256bytes
     .pages_per_sector = 256,
     .sectors_per_block = 1,
     .nr_blocks      = 256,
     .nr_bytes = 0x1000000 ///16 MB
#endif     
    }     
};

static struct winbond_spi_flash_info esmt_spi_flash_info_table[] = {
    {
     .name = "F25L32QA",
     .id = ESMT_ID_F25L32QA,
     .byte_mode			= 3,
     .log2_page_size = 8,       ///256bytes
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks      = 64,
     .nr_bytes = 0x400000 ///4 MB
     },
    {     
     .name = "F25L64QA",
     .id = ESMT_ID_F25L64QA,
     .byte_mode			= 3,
     .log2_page_size = 8,       ///256bytes
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks      = 128,
     .nr_bytes = 0x800000 ///8 MB
     },     
};

static struct winbond_spi_flash_info gd_spi_flash_info_table[] = {
    {
     .name = "GD25Q64B",
     .id = GD_ID_GD25Q64B,
     .byte_mode			= 3,
     .log2_page_size = 8,       ///256bytes
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks      = 128,
     .nr_bytes = 0x800000 ///8 MB
     },
    {     
     .name = "GD25Q128B",
     .id = GD_ID_GD25Q128B,
     .byte_mode			= 3,
     .log2_page_size = 8,       ///256bytes
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks      = 256,
     .nr_bytes = 0x1000000 ///16 MB
     },     
};

static struct winbond_spi_flash_info SELF_spi_flash_info_table[] = {
    {
     .name = "SELF",
     .id = SELF_ID,
     .byte_mode			= 3,
     .log2_page_size = 8,       ///256bytes
     .pages_per_sector = 16,
     .sectors_per_block = 16,
     .nr_blocks      = 256,
     .nr_bytes = 0x1000000 ///16 MB
     },
};

/*
 * Internal helper functions
 */

static inline struct winbond_spi_flash_info *default_winbond_spi_flash_info(void)
{
    return &winbond_spi_flash_info_table[0];
}

static inline u32 byte_swap(u32 * v)
{
    return *v = (((*v & 0xFF) << 24) |
                 ((*v & 0xFF00) << 8) | ((*v & 0xFF0000) >> 8) | ((*v & 0xFF000000) >> 24));
}

static inline struct winbond_spi_flash *mtd_to_winbond_spi_flash(struct mtd_info *mtd)
{
    return container_of(mtd, struct winbond_spi_flash, mtd);
}

/** 
* @brief Assemble the address part of a command for Winbond devices in non-power-of-two page size mode.
* 
* @param flash alias of the flash
* @param cmd spi command
* @param offset start address
*/
static void winbond_build_address(const struct winbond_spi_flash *flash, u8 * cmd, const u32 offset)
{
  	if(flash->info->byte_mode == 4){
  		cmd[0] = (offset >> 24) & 0xFF;
  		cmd[1] = (offset >> 16) & 0xFF;
  		cmd[2] = (offset >> 8) & 0xFF;
  		cmd[3] = offset & 0xFF;		
  	}else {
  		cmd[0] = (offset >> 16) & 0xFF;
  		cmd[1] = (offset >> 8) & 0xFF;
  		cmd[2] = offset & 0xFF;
  	}    
}

/** 
* @brief Read the status register, returning its value in the location
* 
* @param flash alias of the flash
* 
* @return the status register value or negative if error occurred.
*/
static u8 read_sr(struct winbond_spi_flash *flash)
{
    int retval = 0;
    u8 code = CMD_W25_RDSR;
    u8 val = 0;

    retval = spi_write_then_read(flash->spi, &code, 1, &val, 1);

    if (retval < 0) {
        return retval;
    }

    return val;
}

/*
 * Write status register 1 byte
 * Returns negative if error occurred.
 */
static int write_sr(struct winbond_spi_flash *flash, u8 val)
{
    u8 cmd[2] = { 0 };

    cmd[0] = CMD_W25_WRSR;
    cmd[1] = val;

    return spi_write(flash->spi, cmd, 2);
}

/*
 * Set write enable latch with Write Enable command.
 * Returns negative if error occurred.
 */
static inline int write_enable(struct winbond_spi_flash *flash)
{
    u8 code = CMD_W25_WREN;

    return spi_write_then_read(flash->spi, &code, 1, NULL, 0);
}

/*
 * Service routine to read status register until ready, or timeout occurs.
 * Returns non-zero if error.
 */
static int wait_till_ready(struct winbond_spi_flash *flash, u32 check_state)
{
    int count = 0;
    int sr = 0;

    if ((check_state != WINBOND_SR_WIP) && (check_state != WINBOND_SR_WEL)) {
        panic("In %s: wrong state check bit\n", __func__);
    }

    /* one chip guarantees max 5 msec wait here after page writes,
     * but potentially three seconds (!) after page erase.
     */
    for (count = 0; count < MAX_READY_WAIT_COUNT; count++) {
        if ((sr = read_sr(flash)) < 0) {
            break;
        } else if (!(sr & check_state)) {
            return 0;
        }
        /* REVISIT sometimes sleeping would be best */
    }

    return 1;
}

int spi_4byte_mode_one_time = 1;

static int spi_4byte_mode(struct winbond_spi_flash *flash, int enable) 
{ 
	int ret = -1;
  u8 code; 

	if (wait_till_ready(flash, WINBOND_SR_WIP)) {
		goto exit;
	}
    
	if((flash->info->byte_mode == 4) && (spi_4byte_mode_one_time)){
	  if (enable) 
	  		code = 0xB7; /* EN4B, enter 4-byte mode */ 
	  else 
	 		code = 0xE9; /* EX4B, exit 4-byte mode */ 
    
#ifndef CONFIG_PLATFORM_GM8126 
		spi_4byte_mode_one_time = 0;
#endif

		ret = spi_write_then_read(flash->spi, &code, 1, NULL, 0);
		if (ret < 0) {
			printk("SF: Set 4 byte mode failed\n");
			goto exit;
		}  
	}
	ret = 0;
exit:	
  return ret; 
}

/*
 * Erase one sector of flash memory at offset ``offset'' which is any
 * address within the sector which should be erased.
 *
 * Returns 0 if successful, non-zero otherwise.
 */
static int erase_sector(struct winbond_spi_flash *flash, u32 offset)
{
    u8 cmd[5] = { 0 };

    /* Send write enable, then erase commands. */
    write_enable(flash);

    /* Set up command buffer. */
    cmd[0] = flash->erase_opcode;
    winbond_build_address(flash, cmd + 1, offset);

    spi_write(flash->spi, cmd, flash->info->byte_mode + 1);

    /* Wait until finished previous write command. */
    if (wait_till_ready(flash, WINBOND_SR_WEL)) {
        return 1;
    }
    return 0;
}

/****************************************************************************/

/*
 * MTD implementation starts from now on
 */

/*
 * common control flow to deal with a logical flash, which includes an auxiliary flash
 */
static int do_with_combined_spi_flash(struct winbond_spi_flash *flash, u32 addr, u32 len, 
                                      int (*f)(struct winbond_spi_flash *flash, u32 addr, u32 len, void *cxt), 
                                      void *cxt)
{
    struct winbond_spi_flash *aux = flash->aux;
    int ret = 0;

    if (addr < flash->info->nr_bytes) {
        if (addr + len > flash->info->nr_bytes) {
            u32 primary_len = flash->info->nr_bytes - addr;
            len -= primary_len;
            if ((ret = f(flash, addr, primary_len, cxt)) != 0) {
                goto err;
            } 
        } else {
            if ((ret = f(flash, addr, len, cxt)) != 0) {
                goto err;
            } else {
                goto done;
            }
        }
        
        if ((ret = f(aux, 0, len, cxt)) != 0) {
            goto err;
        } 
    } else {
        if ((ret = f(aux, (addr - flash->info->nr_bytes), len, cxt)) != 0) {
            goto err;
        } 
    }

done:
    return 0;
err:
    return ret;
}

static int winbond_spi_flash_erase_imp(struct winbond_spi_flash *flash, u32 addr, u32 len, void *cxt)
{
    struct mtd_info *mtd = &flash->mtd;
    struct erase_info *instr = (struct erase_info *)cxt;

    while (len) {
        if (erase_sector(flash, addr)) {
            instr->state = MTD_ERASE_FAILED;
            goto err;
        }

        addr += mtd->erasesize;
        len -= mtd->erasesize;
    }

    return 0;
err:            
    return -EIO;
}

/*
 * Erase an address range on the flash chip.  The address range may extend
 * one or more erase sectors.  Return an error is there is a problem erasing.
 */
static int winbond_spi_flash_erase(struct mtd_info *mtd, struct erase_info *instr)
{
    struct winbond_spi_flash *flash = mtd_to_winbond_spi_flash(mtd);
    struct winbond_spi_flash *aux = flash->aux;
    u32 addr = 0, len = 0;
    int ret = 0;

    /* sanity checks */
    if (instr->addr + instr->len > flash->mtd.size) {
        return -EINVAL;
    }

    if (((instr->addr % mtd->erasesize) != 0) || ((instr->len % mtd->erasesize) != 0)) {
        return -EINVAL;
    }

    addr = instr->addr;
    len = instr->len;

    mutex_lock(&flash->lock);
    spi_4byte_mode(flash, 1);

    if (aux == NULL) {
        if ((ret = winbond_spi_flash_erase_imp(flash, addr, len, instr)) != 0) {
            goto err;
        }
    } else {
        ret = do_with_combined_spi_flash(flash, addr, len, winbond_spi_flash_erase_imp, instr);
        if (ret < 0) {
            goto err;
        } 
    }
    spi_4byte_mode(flash, 0);	
    mutex_unlock(&flash->lock);
    instr->state = MTD_ERASE_DONE;
    mtd_erase_callback(instr);

    return 0;

err:
	  spi_4byte_mode(flash, 0);	
    mutex_unlock(&flash->lock);
    return ret;
}

static int winbond_spi_flash_read_imp(struct winbond_spi_flash *flash, u32 from, u32 len, void *cxt)
{
    struct spi_transfer t[2];
    struct spi_message m;
    size_t *retlen = ((struct winbond_spi_flash_read_cxt *)cxt)->retlen;
    u_char *buf = ((struct winbond_spi_flash_read_cxt *)cxt)->buf;


    spi_message_init(&m);
    memset(t, 0, (sizeof t));

    /* NOTE:
     * OPCODE_FAST_READ (if available) is faster.
     * Should add 1 byte DUMMY_BYTE.
     */
    t[0].tx_buf = flash->command;
    t[0].len = flash->info->byte_mode + 1;
    spi_message_add_tail(&t[0], &m);

    t[1].rx_buf = buf;
    t[1].len = len;
    spi_message_add_tail(&t[1], &m);

    /* Byte count starts at zero. */
    if (retlen) {
        *retlen = 0;
    }
    
    /* FIXME switch to OPCODE_FAST_READ.  It's required for higher
     * clocks; and at this writing, every chip this driver handles
     * supports that opcode.
     */

    /* Set up the write data buffer. */
    flash->command[0] = CMD_W25_READ;
    if(flash->info->byte_mode == 4){
    	flash->command[1] = (from >> 24) & 0xFF;
    	flash->command[2] = (from >> 16) & 0xFF;
    	flash->command[3] = (from >> 8) & 0xFF; 
    	flash->command[4] = from & 0xFF;    	
    }else{
    flash->command[1] = (from >> 16) & 0xFF;
    flash->command[2] = (from >> 8) & 0xFF;
    flash->command[3] = from & 0xFF;
		}
    spi_sync(flash->spi, &m);

    /* Wait till previous write/erase is done. */
    if (wait_till_ready(flash, WINBOND_SR_WIP)) {
        return 1;
    }   
    *retlen = m.actual_length - (flash->info->byte_mode + 1);

    return 0;
}

/*
 * Read an address range from the flash chip.  The address range
 * may be any size provided it is within the physical boundaries.
 */
static int winbond_spi_flash_read(struct mtd_info *mtd, loff_t from, size_t len, size_t * retlen,
                                  u_char * buf)
{
    struct winbond_spi_flash *flash = mtd_to_winbond_spi_flash(mtd);
    struct winbond_spi_flash *aux = flash->aux;
    struct winbond_spi_flash_read_cxt cxt = { retlen, buf };
    int ret = 0;
//printk("<from 0x%08x,len %zd>", (u32) from, len);
    WINBOND_DEG_PRINT("%s: %s from 0x%08x, len %zd\n", flash->spi->dev.bus_id,
                      __FUNCTION__, (u32) from, len);

    /* sanity checks */
    if (!len) {
        return 0;
    }

    if (from + len > flash->mtd.size) {
        return -EINVAL;
    }

    mutex_lock(&flash->lock);
    spi_4byte_mode(flash, 1);

    if (aux == NULL) {
        if ((ret = winbond_spi_flash_read_imp(flash, from, len, &cxt)) != 0) {
            goto err;
        }
    } else {
        ret = do_with_combined_spi_flash(flash, from, len, winbond_spi_flash_read_imp, &cxt);
        if (ret < 0) {
            goto err;
        } 
    }
err:
	  spi_4byte_mode(flash, 0);
    mutex_unlock(&flash->lock);
    return ret;
}

static int winbond_spi_flash_write_imp(struct winbond_spi_flash *flash, u32 to, u32 len, void *cxt)
{
    u32 page_offset, page_size;
    struct spi_transfer t[2];
    struct spi_message m;
    size_t *retlen = ((struct winbond_spi_flash_write_cxt *)cxt)->retlen;
    const u_char *buf = ((struct winbond_spi_flash_write_cxt *)cxt)->buf;
    
    if (retlen) {
        *retlen = 0;
    }

    /* sanity checks */
    if (!len) {
        return (0);
    }

    spi_message_init(&m);
    memset(t, 0, (sizeof t));

    t[0].tx_buf = flash->command;
    t[0].len = flash->info->byte_mode + 1;
    spi_message_add_tail(&t[0], &m);

    t[1].tx_buf = buf;
    spi_message_add_tail(&t[1], &m);

    write_enable(flash);

    /* Set up the opcode in the write buffer. */
    flash->command[0] = CMD_W25_PP;
    if(flash->info->byte_mode == 4){
    	flash->command[1] = (to >> 24) & 0xFF;
    	flash->command[2] = (to >> 16) & 0xFF;
    	flash->command[3] = (to >> 8) & 0xFF; 
    	flash->command[4] = to & 0xFF;   	
  	}else{
    	flash->command[1] = (to >> 16) & 0xFF;
    	flash->command[2] = (to >> 8) & 0xFF;
    	flash->command[3] = to & 0xFF;
		}
    /* what page do we start with? */
    page_offset = to % GM_SPI_FLASH_PAGESIZE;

    /* do all the bytes fit onto one page? */
    if (page_offset + len <= GM_SPI_FLASH_PAGESIZE) {
        t[1].len = len;
        spi_sync(flash->spi, &m);
        
		    /* Wait until finished previous write command. */
		    if (wait_till_ready(flash, WINBOND_SR_WEL)) {
		        return 1;
		    }        
        *retlen = m.actual_length - (flash->info->byte_mode + 1);
    } else {
        u32 i;

        /* the size of data remaining on the first page */
        page_size = GM_SPI_FLASH_PAGESIZE - page_offset;

        t[1].len = page_size;
        spi_sync(flash->spi, &m);

		    /* Wait until finished previous write command. */
		    if (wait_till_ready(flash, WINBOND_SR_WEL)) {
		        return 1;
		    }
        *retlen = m.actual_length - (flash->info->byte_mode + 1);

        /* write everything in PAGESIZE chunks */
        for (i = page_size; i < len; i += page_size) {
            page_size = len - i;
            if (page_size > GM_SPI_FLASH_PAGESIZE)
                page_size = GM_SPI_FLASH_PAGESIZE;

            /* write the next page to flash */
            if(flash->info->byte_mode == 4){
            	flash->command[1] = ((to + i) >> 24) & 0xFF;
            	flash->command[2] = ((to + i) >> 16) & 0xFF;
            	flash->command[3] = ((to + i) >> 8) & 0xFF; 
            	flash->command[4] = (to + i);           	
          	}else{
            flash->command[1] = ((to + i) >> 16) & 0xFF;
            flash->command[2] = ((to + i) >> 8) & 0xFF;
            flash->command[3] = (to + i) & 0xFF;
						}
            t[1].tx_buf = buf + i;
            t[1].len = page_size;

            write_enable(flash);

            spi_sync(flash->spi, &m);

				    /* Wait until finished previous write command. */
				    if (wait_till_ready(flash, WINBOND_SR_WEL)) {
				        return 1;
				    }
            if (retlen)
                *retlen += m.actual_length - (flash->info->byte_mode + 1);
        }
    }

    return 0;
}

/*
 * Write an address range to the flash chip.  Data must be written in
 * GM_SPI_FLASH_PAGESIZE chunks.  The address range may be any size provided
 * it is within the physical boundaries.
 */
static int winbond_spi_flash_write(struct mtd_info *mtd, loff_t to, size_t len, size_t * retlen,
                                   const u_char * buf)
{
    struct winbond_spi_flash *flash = mtd_to_winbond_spi_flash(mtd);
    struct winbond_spi_flash *aux = flash->aux;
    struct winbond_spi_flash_write_cxt cxt = { retlen, buf };
    int ret = 0;

    WINBOND_DEG_PRINT("%s: %s to 0x%08x, len %zd\n", flash->spi->dev.bus_id, __FUNCTION__, (u32) to,
                      len);

    if (to + len > flash->mtd.size) {
        return -EINVAL;
    }
    
    mutex_lock(&flash->lock);
    spi_4byte_mode(flash, 1);
    
    if (aux == NULL) {
        if ((ret = winbond_spi_flash_write_imp(flash, to, len, &cxt)) != 0) {
            goto err;
        }
    } else {
        ret = do_with_combined_spi_flash(flash, to, len, winbond_spi_flash_write_imp, &cxt);
        if (ret < 0) {
            goto err;
        } 
    }
err:
	  spi_4byte_mode(flash, 0);	
    mutex_unlock(&flash->lock);
    return ret;
}

static struct winbond_spi_flash_info *__devinit jedec_probe(struct spi_device *spi)
{
    struct winbond_spi_flash_info *info = NULL;
    u8 code = CMD_W25_RD_JEDEC_ID;
    u8 id[5] = { 0 };
    u32 jedec_id = 0;
    u16 i = 0, table_size = 0;
    int tmp = -1;

    tmp = spi_write_then_read(spi, &code, 1, id, ARRAY_SIZE(id));
    if (unlikely(tmp < 0)) {
        WINBOND_DEG_PRINT("%s: error %d reading JEDEC ID\n", spi->dev.bus_id, tmp);
        printk("error %d reading JEDEC ID\n", tmp);
        return NULL;
    }

    jedec_id = ((id[1] << 8) | id[2]);

    WINBOND_DEG_PRINT("id[0] = %x, id[1] = %x, id[2] = %x, id[3] = %x, id[4] = %x\n", id[0], id[1], id[2],id[3],id[4]);

    if (likely(WINBOND_SPI_FLASH_JEDEC_M_ID == id[0])) {
        info = winbond_spi_flash_info_table;
        table_size = ARRAY_SIZE(winbond_spi_flash_info_table);
    } else if (MXIC_SPI_FLASH_JEDEC_M_ID == id[0]) {
        info = mxic_spi_flash_info_table;
        table_size = ARRAY_SIZE(mxic_spi_flash_info_table);
    } else if (NUMONYX_SPI_FLASH_JEDEC_M_ID == id[0]) {
        info = numonyx_spi_flash_info_table;
        table_size = ARRAY_SIZE(numonyx_spi_flash_info_table);
    } else if (EON_SPI_FLASH_JEDEC_M_ID == id[0]) {
        info = eon_spi_flash_info_table;
        table_size = ARRAY_SIZE(eon_spi_flash_info_table);
    } else if (GD_SPI_FLASH_JEDEC_M_ID == id[0]) {
        info = gd_spi_flash_info_table;
        table_size = ARRAY_SIZE(gd_spi_flash_info_table); 
    } else if (ESMT_SPI_FLASH_JEDEC_M_ID == id[0]) {
        info = esmt_spi_flash_info_table;
        table_size = ARRAY_SIZE(esmt_spi_flash_info_table);         
    } else if (SELF_SPI_FLASH_JEDEC_M_ID == id[0]) {
        info = SELF_spi_flash_info_table;
        table_size = ARRAY_SIZE(SELF_spi_flash_info_table);  
    } else if (SPANSION_SPI_FLASH_JEDEC_M_ID== id[0]) {
        info = spansion_spi_flash_info_table;
        table_size = ARRAY_SIZE(spansion_spi_flash_info_table); 
			 //printk("SPANSION_SPI_FLASH_JEDEC_M_ID \n");              
    } else {
        WINBOND_DEG_PRINT("%s fails: ID(0x%02x%02x%02x) no matched.\n", __func__, id[0], id[1], id[2]);
        return NULL;
    }
    
    for (i = 0; i < table_size; i++) {
        if ((info + i)->id == jedec_id) {
            return (info + i);
        }
    }

    printk("Can't identify flash type, use default setting\n");

    return default_winbond_spi_flash_info();
}

static struct winbond_spi_flash_info *specific_chip_probe(struct spi_device *spi)
{
    struct flash_platform_data *data = NULL;
    struct winbond_spi_flash_info *info = NULL;

    /* Platform data helps sort out which chip type we have, as
     * well as how this board partitions it.  If we don't have
     * a chip ID, try the JEDEC id commands; they'll work for most
     * newer chips, even if we don't recognize the particular chip.
     */
    data = spi->dev.platform_data;
    if (data && data->type) {
#if 0
        for (i = 0, info = m25p_data; i < ARRAY_SIZE(m25p_data); i++, info++) {
            if (strcmp(data->type, info->name) == 0)
                break;
        }

        /* unrecognized chip? */
        if (i == ARRAY_SIZE(m25p_data)) {
            DEBUG(MTD_DEBUG_LEVEL0, "%s: unrecognized id %s\n", spi->dev.bus_id, data->type);
            info = NULL;

            /* recognized; is that chip really what's there? */
        } else if (info->jedec_id) {
            struct flash_info *chip = jedec_probe(spi);

            if (!chip || chip != info) {
                dev_warn(&spi->dev, "found %s, expected %s\n",
                         chip ? chip->name : "UNKNOWN", info->name);
                info = NULL;
            }
        }
#endif
    } else {
        WINBOND_DEG_PRINT("start to probe dev id\n");
        info = jedec_probe(spi);
    }

    return info;
}

static void _xget_image_header(struct mtd_info *mtd, loff_t from, size_t len, struct flash_image_header *header)
{
    size_t retlen = 0;

    winbond_spi_flash_read(mtd, from, sizeof(struct flash_image_header), &retlen, (u_char *)header);

    byte_swap(&header->magic);
    byte_swap(&header->size);

    WINBOND_DEG_PRINT("header magic = %x\n", header->magic);
    WINBOND_DEG_PRINT("image size = %d\n", header->size);
}

/* header variable only used in get_flash_partition_info to init MTD partion */
static struct flash_system_header __devinitdata system_header;
static struct flash_image_header __devinitdata linux_header;
static struct flash_image_header __devinitdata uboot_header;
static struct flash_image_header __devinitdata burnin_header;
#ifdef CONFIG_PLATFORM_GM8210
static struct flash_image_header __devinitdata slave_linux_header;
static struct flash_image_header __devinitdata audio_header;
static struct flash_image_header __devinitdata logo_header;
#endif

static int __devinit get_flash_partition_info(struct mtd_info *mtd, struct mtd_partition *parts)
{
    size_t user_offset = 0, cfg_offset = 0, retlen = 0;
#if 0
    size_t new_user_offset = 0;
#endif

    winbond_spi_flash_read(mtd, 0, sizeof(struct flash_system_header), &retlen,
                           (u_char *) & system_header);

    byte_swap((u32 *) system_header.signature);
    byte_swap((u32 *) (system_header.signature + 4));
    byte_swap(&system_header.burnin_addr);
    byte_swap(&system_header.uboot_addr);
    byte_swap(&system_header.linux_addr);
    byte_swap(&system_header.bootm_addr);
#ifdef CONFIG_PLATFORM_GM8210
    byte_swap(&system_header.slave_linux_addr);
    byte_swap(&system_header.audio_addr);
    byte_swap(&system_header.logo_addr);
#endif

    WINBOND_DEG_PRINT("len =  %d\n", retlen);
    WINBOND_DEG_PRINT("IC = %s\n", system_header.signature);
    WINBOND_DEG_PRINT("Loader image address = %x\n", system_header.bootm_addr);
    WINBOND_DEG_PRINT("Burn-in image address = %x\n", system_header.burnin_addr);
    WINBOND_DEG_PRINT("U-Boot image address = %x\n", system_header.uboot_addr);
    WINBOND_DEG_PRINT("Linux image address = %x\n", system_header.linux_addr);
#ifdef CONFIG_PLATFORM_GM8210
    WINBOND_DEG_PRINT("Linux slave image address = %x\n", system_header.slave_linux_addr);
    WINBOND_DEG_PRINT("Linux audio image address = %x\n", system_header.audio_addr);
    WINBOND_DEG_PRINT("Logo image address = %x\n", system_header.logo_addr);
#endif 
    
    _xget_image_header(mtd, system_header.linux_addr, sizeof(struct flash_image_header), &linux_header);
    _xget_image_header(mtd, system_header.uboot_addr, sizeof(struct flash_image_header), &uboot_header);
    _xget_image_header(mtd, system_header.burnin_addr, sizeof(struct flash_image_header), &burnin_header);
#ifdef CONFIG_PLATFORM_GM8210
    _xget_image_header(mtd, system_header.slave_linux_addr, sizeof(struct flash_image_header), &slave_linux_header);
    _xget_image_header(mtd, system_header.audio_addr, sizeof(struct flash_image_header), &audio_header);
    _xget_image_header(mtd, system_header.logo_addr, sizeof(struct flash_image_header), &logo_header);
    
    /* MTD CFG PARTATION */
    cfg_offset = system_header.audio_addr - (2 * mtd->erasesize);

    if (cfg_offset < mtd->size) {
        WINBOND_DEG_PRINT("MTD CFG address = %x\n", cfg_offset);
    } else {
        printk("%s fails: flash size is not enough to save user's space\n", __FUNCTION__);
        goto wrong_offset;
    }

    if (cfg_offset < (system_header.uboot_addr + uboot_header.size)) {
        printk("%s fails: user's partion address overlays the uboot region\n", __FUNCTION__);
        goto wrong_offset;
    } 
    
    //printk("CONFIG_MTD_USER_PARTION_SIZE = %d KB\n", CONFIG_MTD_USER_PARTION_SIZE);
    user_offset = mtd->size - (1024 * CONFIG_MTD_USER_PARTION_SIZE);

    if (user_offset < mtd->size) {
        WINBOND_DEG_PRINT("MTD User address = %x\n", user_offset);
    } else {
        printk("%s fails: flash size is not enough to save user's space\n", __FUNCTION__);
        goto wrong_offset;
    }

    if (user_offset < (system_header.logo_addr + linux_header.size)) {
        printk("%s fails: user's partion address overlays the linux region\n", __FUNCTION__);
        goto wrong_offset;
    }    
#else
    /* MTD CFG PARTATION */
    cfg_offset = system_header.linux_addr - (2*mtd->erasesize);

    if (cfg_offset < mtd->size) {
        WINBOND_DEG_PRINT("MTD CFG address = %x\n", cfg_offset);
    } else {
        printk("%s fails: flash size is not enough to save user's space\n", __FUNCTION__);
        goto wrong_offset;
    }

    if (cfg_offset < (system_header.uboot_addr + uboot_header.size)) {
        printk("%s fails: user's partion address overlays the uboot region\n", __FUNCTION__);
        goto wrong_offset;
    } 
    
    //printk("CONFIG_MTD_USER_PARTION_SIZE = %d KB\n", CONFIG_MTD_USER_PARTION_SIZE);
    user_offset = mtd->size - (1024*CONFIG_MTD_USER_PARTION_SIZE);

    if (user_offset < mtd->size) {
        WINBOND_DEG_PRINT("MTD User address = %x\n", user_offset);
    } else {
        printk("%s fails: flash size is not enough to save user's space\n", __FUNCTION__);
        goto wrong_offset;
    }

    if (user_offset < (system_header.linux_addr + linux_header.size)) {
        printk("%s fails: user's partion address overlays the linux region\n", __FUNCTION__);
        goto wrong_offset;
    }
#endif    
#ifdef CONFIG_PLATFORM_GM8210
    //linux setting
    parts[0].offset = system_header.linux_addr;
    parts[0].size = system_header.logo_addr - system_header.linux_addr;
    //user setting
    parts[1].offset = user_offset;
    parts[1].size = 1024 * CONFIG_MTD_USER_PARTION_SIZE;       
    //Loader setting
    parts[2].offset = mtd->erasesize;//loader must be just after the first erase block
    parts[2].size = system_header.uboot_addr - mtd->erasesize;
    //uboot setting
    parts[3].offset = system_header.uboot_addr;
    parts[3].size = system_header.audio_addr - system_header.uboot_addr - (2 * mtd->erasesize);    
    //slave setting
    parts[4].offset = system_header.uboot_addr;
    parts[4].size = system_header.linux_addr - system_header.slave_linux_addr; 
    //audio setting
    parts[5].offset = system_header.audio_addr;
    parts[5].size = system_header.slave_linux_addr - system_header.audio_addr;
    //cfg setting
    parts[6].offset = cfg_offset;
    parts[6].size = 2 * mtd->erasesize;         
#else
    //linux setting
    parts[0].offset = system_header.linux_addr;
    parts[0].size = user_offset - system_header.linux_addr - mtd->erasesize;
    //user setting
    parts[1].offset = user_offset;
    parts[1].size = 1024 * CONFIG_MTD_USER_PARTION_SIZE;
    //spi boot setting
    parts[2].offset = mtd->erasesize;//loader must be just after the first erase block
    parts[2].size = system_header.burnin_addr - mtd->erasesize;
    //burnin setting
    parts[3].offset = system_header.burnin_addr;
    parts[3].size = system_header.uboot_addr - system_header.burnin_addr;
    //uboot setting
    parts[4].offset = system_header.uboot_addr;
    parts[4].size = system_header.linux_addr - system_header.uboot_addr - (2 * mtd->erasesize);
    //cfg setting
    parts[5].offset = cfg_offset;
    parts[5].size = 2 * mtd->erasesize;    
#endif

    return 0;

  wrong_offset:
    return -1;
}

static int __devinit winbond_spi_flash_normal_probe(struct spi_device *spi)
{
    struct flash_platform_data *data = NULL;
    struct winbond_spi_flash_info *info = NULL;
    struct winbond_spi_flash *flash = NULL;

    data = spi->dev.platform_data;

    info = specific_chip_probe(spi);

    if (!info) {
        printk("Can't identify flash type, use default setting\n");
        info = default_winbond_spi_flash_info();
    }

    flash = kzalloc(sizeof *flash, GFP_KERNEL);
    if (!flash) {
        return -ENOMEM;
    }

    flash->spi = spi;
    flash->info = info;
    mutex_init(&flash->lock);

    dev_set_drvdata(&spi->dev, flash);

    //reset flash's status register
    write_enable(flash);
    write_sr(flash, 0);

    if (data && data->name) {
        flash->mtd.name = data->name;
    } else {
        flash->mtd.name = spi->dev.bus_id;
    }

    flash->mtd.type = MTD_NORFLASH;
    //TODO:make sure this
    flash->mtd.writesize = 1;
    flash->mtd.flags = MTD_CAP_NORFLASH;
    flash->mtd.size = info->nr_bytes;
    flash->mtd.erase = winbond_spi_flash_erase;
    flash->mtd.read = winbond_spi_flash_read;
    flash->mtd.write = winbond_spi_flash_write;

    /* current prefer "small sector" erase */
    //TODO:this WILL be changed to 64K BLOCK erase
    flash->erase_opcode = CMD_W25_SE4K;
    flash->mtd.erasesize = (1 << info->log2_page_size) * info->pages_per_sector;//4096;
    flash->device_no = flash_count++;
#if 0
    WINBOND_DEG_PRINT("mtd .name = %s, .size = 0x%.8x (%uMiB) "
                      ".erasesize = 0x%.8x (%uKiB) .numeraseregions = %d\n",
                      flash->mtd.name,
                      flash->mtd.size, flash->mtd.size / (1024 * 1024),
                      flash->mtd.erasesize, flash->mtd.erasesize / 1024,
                      flash->mtd.numeraseregions);
    if (flash->mtd.numeraseregions) {
        for (i = 0; i < flash->mtd.numeraseregions; i++) {
            WINBOND_DEG_PRINT("mtd.eraseregions[%d] = { .offset = 0x%.8x, "
                              ".erasesize = 0x%.8x (%uKiB), "
                              ".numblocks = %d }\n",
                              i, flash->mtd.eraseregions[i].offset,
                              flash->mtd.eraseregions[i].erasesize,
                              flash->mtd.eraseregions[i].erasesize / 1024,
                              flash->mtd.eraseregions[i].numblocks);
        }
    }
#endif

    if (flash->device_no == 0) {
        if (get_flash_partition_info(&flash->mtd, data->parts) != 0) {
            printk("Error: get_flash_partition_info not OK\n");
            goto err_flash_parts;
        }
    }

    /* partitions should match sector boundaries; and it may be good to
     * use readonly partitions for writeprotected sectors (BP2..BP0).
     */
    if (likely(mtd_has_partitions())) {
        struct mtd_partition *parts = NULL;
        int nr_parts = 0;

        if (nr_parts <= 0 && data && data->parts) {
            parts = data->parts;
            nr_parts = data->nr_parts;
        }

        if (nr_parts > 0) {
            flash->partitioned = 1;
            return add_mtd_partitions(&flash->mtd, parts, nr_parts);
        }
    } else if (data->nr_parts) {
        WINBOND_DEG_PRINT("ignoring %d default partitions on %s\n", data->nr_parts, data->name);
    }

    return add_mtd_device(&flash->mtd) == 1 ? -ENODEV : 0;

  err_flash_parts:
    return -ENODEV;
}

/*
 * not standard way to make a spi_device, we ignore the spi.dev filed so that 
 * you won't find the device info from sysfs 
 */
static struct spi_device* make_spi_device(struct spi_device *primary_spi)
{
    struct spi_device *aux_spi = kzalloc(sizeof(struct spi_device), GFP_KERNEL);
    struct spi_board_info *aux_info = primary_spi->controller_data;   

    if (unlikely(aux_spi == NULL)) {
        printk("%s fails: kzalloc fail.\n", __func__);
        return NULL;
    }
 
    aux_spi->master = primary_spi->master;
    aux_spi->bits_per_word = primary_spi->bits_per_word;
    aux_spi->max_speed_hz = aux_info->max_speed_hz;
    aux_spi->chip_select = aux_info->chip_select;
    aux_spi->mode = aux_info->mode;
    
    return aux_spi;
}  

static int __devinit winbond_spi_flash_combined_probe(struct spi_device *spi)
{
    struct flash_platform_data *primary_data = spi->dev.platform_data;
    struct winbond_spi_flash_info *primary_info = specific_chip_probe(spi);
    struct winbond_spi_flash *primary_flash = NULL;
    struct spi_device *aux_spi = NULL;
    struct winbond_spi_flash_info *aux_info = NULL;
    struct winbond_spi_flash *aux_flash = NULL;

    aux_spi = make_spi_device(spi);
    if (unlikely(aux_spi == NULL)) {
        goto mk_aux_spi_dev_err;
    }

    aux_info = specific_chip_probe(aux_spi);
    if (unlikely(!aux_info)) {
        printk("Can't identify aux_flash type, use default setting\n");
        aux_info = default_winbond_spi_flash_info();
    }

    aux_flash = kzalloc(sizeof(*aux_flash), GFP_KERNEL);
    if (!aux_flash) {
        printk("%s fails: malloc memory not OK\n", __func__);
        return -ENOMEM;
    }

    aux_flash->spi = aux_spi;
    aux_flash->info = aux_info;
    aux_flash->mtd.erasesize = (1 << aux_info->log2_page_size) * aux_info->pages_per_sector;//4096;
    aux_flash->erase_opcode = CMD_W25_SE4K;
    mutex_init(&aux_flash->lock);

    dev_set_drvdata(&aux_spi->dev, aux_flash);

    //reset flash's status register
    write_enable(aux_flash);
    write_sr(aux_flash, 0);

    primary_info = specific_chip_probe(spi);

    if (!primary_info) {
        printk("Can't identify primary_flash type, use default setting\n");
        primary_info = default_winbond_spi_flash_info();
    }

    primary_info->nr_blocks += aux_flash->info->nr_blocks;

    primary_flash = kzalloc(sizeof(*primary_flash), GFP_KERNEL);
    if (!primary_flash) {
        return -ENOMEM;
    }

    primary_flash->aux = aux_flash;
    primary_flash->spi = spi;
    primary_flash->info = primary_info;
    mutex_init(&primary_flash->lock);

    dev_set_drvdata(&spi->dev, primary_flash);

    //reset flash's status register
    write_enable(primary_flash);
    write_sr(primary_flash, 0);

    if (primary_data && primary_data->name) {
        primary_flash->mtd.name = primary_data->name;
    } else {
        primary_flash->mtd.name = spi->dev.bus_id;
    }

    primary_flash->mtd.type = MTD_NORFLASH;
    //TODO:make sure this
    primary_flash->mtd.writesize = 1;
    primary_flash->mtd.flags = MTD_CAP_NORFLASH;
    primary_flash->mtd.size = (1 << primary_info->log2_page_size) * primary_info->pages_per_sector *
                               primary_info->sectors_per_block * primary_info->nr_bytes;
    primary_flash->mtd.erase = winbond_spi_flash_erase;
    primary_flash->mtd.read = winbond_spi_flash_read;
    primary_flash->mtd.write = winbond_spi_flash_write;

    /* current prefer "small sector" erase */
    //TODO:this WILL be changed to 64K BLOCK erase
    primary_flash->erase_opcode = CMD_W25_SE4K;
    primary_flash->mtd.erasesize = (1 << primary_info->log2_page_size) * primary_info->pages_per_sector;//4096;
    primary_flash->device_no = flash_count++;

    if (primary_flash->device_no == 0) {
        if (get_flash_partition_info(&primary_flash->mtd, primary_data->parts) != 0) {
            printk("Error: get_flash_partition_info not OK\n");
            goto err_flash_parts;
        }
    }
    
    /* partitions should match sector boundaries; and it may be good to
     * use readonly partitions for writeprotected sectors (BP2..BP0).
     */
    if (likely(mtd_has_partitions())) {
        struct mtd_partition *parts = NULL;
        int nr_parts = 0;

        if (nr_parts <= 0 && primary_data && primary_data->parts) {
            parts = primary_data->parts;
            nr_parts = primary_data->nr_parts;
        }

        if (nr_parts > 0) {
            primary_flash->partitioned = 1;
            return add_mtd_partitions(&primary_flash->mtd, parts, nr_parts);
        }
    } else if (primary_data->nr_parts) {
        WINBOND_DEG_PRINT("ignoring %d default partitions on %s\n", primary_data->nr_parts, primary_data->name);
    }

    return add_mtd_device(&primary_flash->mtd) == 1 ? -ENODEV : 0;

err_flash_parts:
    return -ENODEV;
mk_aux_spi_dev_err:
    return -ENODEV;
}

/*
 * board specific setup should have ensured the SPI clock used here
 * matches what the READ command supports, at least until this driver
 * understands FAST_READ (for clocks over 25 MHz).
 */
static int __devinit winbond_spi_flash_probe(struct spi_device *spi)
{
    if (spi->controller_data != NULL) { //has combined flash to probe
        return winbond_spi_flash_combined_probe(spi);
    } else { 
        return winbond_spi_flash_normal_probe(spi);
    }
}

static int __devexit winbond_spi_flash_remove(struct spi_device *spi)
{
    struct winbond_spi_flash *flash = dev_get_drvdata(&spi->dev);
    int status = 0;

    /* Clean up MTD stuff. */
    if (mtd_has_partitions() && flash->partitioned) {
        status = del_mtd_partitions(&flash->mtd);
    } else {
        status = del_mtd_device(&flash->mtd);
    }

    if (status == 0) {
        kfree(flash);
    }

    return 0;
}

static struct spi_driver winbond_spi_flash_driver = {
    .driver = {
               .name = "WINBOND_SPI_FLASH",
               .bus = &spi_bus_type,
               .owner = THIS_MODULE,
               },
    .probe = winbond_spi_flash_probe,
    .remove = __devexit_p(winbond_spi_flash_remove),
};

static int __init winbond_spi_flash_init(void)
{
    u32 value;
    
    value = ftpmu010_read_reg(0x04);
    
    //check jumper setting to confirm that we are boot from spi flash
    if (value & (0x01 << 5)) {
        return 0;
    }

    return spi_register_driver(&winbond_spi_flash_driver);
}

static void winbond_spi_flash_exit(void)
{
    spi_unregister_driver(&winbond_spi_flash_driver);
}

module_init(winbond_spi_flash_init);
module_exit(winbond_spi_flash_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Grain Media Corp.");
MODULE_DESCRIPTION("MTD SPI driver for Winbond flash chips");

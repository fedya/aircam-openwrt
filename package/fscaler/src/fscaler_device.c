/**
 * @file fscaler_device.c
 * @brief Scaler device table
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.04
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_device.h"
#include "fscaler_driver.h"
#include "fscaler_module.h"
#include "fscaler_property.h"
#include "fscaler_hw.h"
#include "fscaler_job.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER
#include <linux/bitmap.h>
#include <linux/bitops.h>
#include <linux/string.h>
#include <linux/spinlock.h>

///////////////////////////////////////////////////////////////////////////////
// LOCAL DEFINE AND MACRO
#define IRQ_MAX_NAMELEN 32
#define IRQ_NAME "fscal_irq"
#define IRQ_HANDLER(ret,name,irq_name,dev_name) \
    ret name(int irq_name, void * dev_name, struct pt_regs *pt_regs)
#ifndef SA_INTERRUPT
#define SA_INTERRUPT IRQF_DISABLED
#undef IRQ_HANDLER
#define IRQ_HANDLER(ret,name,irq_name,dev_name) \
    ret name(int irq_name, void * dev_name)
#endif /* SA_INTERRUPT */

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION DECALRATION
static int fscaler_device_cache(struct fscaler_table *table);
static void fscaler_device_uncache(struct fscaler_table *table);
static void *fscaler_device_request(struct fscaler_table *table, int index);
static void fscaler_device_remove(struct fscaler_table *table, void *data);
static int fscaler_device_compare(void *c0, void *c1);
static void fscaler_device_timer_handler(unsigned long data);
static void fscaler_device_tasklet_handler(unsigned long data);
static int fscaler_osd_get_index(struct fscaler_device *dev, char data);
static void fscaler_osd_disable(struct fscaler_device *dev, struct fscaler_job *job);
static void fscaler_osd_enable(struct fscaler_device *dev, struct fscaler_job *job);
static void fscaler_osd_set_display(struct fscaler_device *dev, struct fscaler_job *job);
static IRQ_HANDLER(irqreturn_t, fscaler_device_irq_handler, irq, dev_id);

///////////////////////////////////////////////////////////////////////////////
// STATIC VARIABLE DEFINITION
static char FSCALER_DEVICE_TABLE_NAME[] = "fscdev";
struct fiosd_char FSCALER_OSD_FONTS[] = {
    [0] = {
           .font = 0x30,        //Char:[0]
           .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x31, 0x80, 0x31,
                       0x80, 0x31, 0x80, 0x31, 0x80, 0x31, 0x80,
                       0x31, 0x80, 0x31, 0x80, 0x31, 0x80, 0x1b, 0x00, 0x0e, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
           },
    [1] = {
           .font = 0x31,        //Char:[1]
           .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x06, 0x00, 0x06, 0x00, 0x06,
                       0x00, 0x06, 0x00, 0x06, 0x00, 0x06, 0x00,
                       0x06, 0x00, 0x06, 0x00, 0x06, 0x00, 0x06, 0x00, 0x1f, 0x80, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
           },
    [2] = {
           .font = 0x32,        //Char:[2]
           .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x31, 0x80, 0x31, 0x80, 0x31,
                       0x80, 0x03, 0x00, 0x06, 0x00, 0x0c, 0x00,
                       0x18, 0x00, 0x10, 0x00, 0x39, 0x80, 0x35, 0x80, 0x33, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
           },
    [3] = {
           .font = 0x33,        //Char:[3]
           .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x31, 0x80, 0x33, 0x00, 0x33,
                       0x00, 0x06, 0x00, 0x0e, 0x00, 0x03, 0x00,
                       0x01, 0x80, 0x01, 0x80, 0x31, 0x80, 0x33, 0x00, 0x1e, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
           },
    [4] = {
           .font = 0x34,        //Char:[4]
           .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x07, 0x00, 0x07, 0x00, 0x0f,
                       0x00, 0x0b, 0x00, 0x1b, 0x00, 0x13, 0x00,
                       0x33, 0x00, 0x3f, 0x80, 0x03, 0x00, 0x03, 0x00, 0x0f, 0x80, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
           },
    [5] = {
           .font = 0x35,        //Char:[5]
           .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x31, 0x80, 0x31, 0x80, 0x30,
                       0x00, 0x3e, 0x00, 0x33, 0x00, 0x31, 0x80,
                       0x01, 0x80, 0x01, 0x80, 0x31, 0x80, 0x33, 0x00, 0x1e, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
           },
    [6] = {
           .font = 0x36,        //Char:[6]
           .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x19, 0x80, 0x31, 0x80, 0x30,
                       0x00, 0x3e, 0x00, 0x3b, 0x00, 0x31, 0x80,
                       0x31, 0x80, 0x31, 0x80, 0x31, 0x80, 0x1b, 0x00, 0x0e, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
           },
    [7] = {
           .font = 0x37,        //Char:[7]
           .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3d, 0x80, 0x33, 0x80, 0x31, 0x80, 0x31,
                       0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00,
                       0x06, 0x00, 0x06, 0x00, 0x06, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
           },
    [8] = {
           .font = 0x38,        //Char:[8]
           .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x31, 0x80, 0x31,
                       0x80, 0x1b, 0x00, 0x0e, 0x00, 0x1b, 0x00,
                       0x31, 0x80, 0x31, 0x80, 0x31, 0x80, 0x1b, 0x00, 0x0e, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
           },
    [9] = {
           .font = 0x39,        //Char:[9]
           .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x31, 0x80, 0x31,
                       0x80, 0x31, 0x80, 0x31, 0x80, 0x1b, 0x80,
                       0x0d, 0x80, 0x01, 0x80, 0x31, 0x80, 0x33, 0x00, 0x1e, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
           },
    [10] = {
            .font = 0x41,       //Char:[A]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x0e,
                        0x00, 0x0a, 0x00, 0x0a, 0x00, 0x1b, 0x00,
                        0x1f, 0x00, 0x1b, 0x00, 0x1b, 0x00, 0x1b, 0x00, 0x3b, 0x80, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [11] = {
            .font = 0x42,       //Char:[B]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x19, 0x80, 0x19, 0x80, 0x19,
                        0x80, 0x19, 0x80, 0x1f, 0x00, 0x19, 0x80,
                        0x19, 0x80, 0x19, 0x80, 0x19, 0x80, 0x19, 0x80, 0x3f, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [12] = {
            .font = 0x43,       //Char:[C]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x0e, 0x80, 0x1b, 0x80, 0x31, 0x80, 0x30,
                        0x80, 0x30, 0x80, 0x30, 0x00, 0x30, 0x00,
                        0x30, 0x00, 0x30, 0x00, 0x30, 0x80, 0x19, 0x80, 0x0f, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [13] = {
            .font = 0x44,       //Char:[D]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x1b, 0x00, 0x19, 0x80, 0x19,
                        0x80, 0x19, 0x80, 0x19, 0x80, 0x19, 0x80,
                        0x19, 0x80, 0x19, 0x80, 0x19, 0x80, 0x1b, 0x00, 0x3e, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [14] = {
            .font = 0x45,       //Char:[E]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x19, 0x80, 0x19, 0x80, 0x18,
                        0x00, 0x19, 0x00, 0x1f, 0x00, 0x19, 0x00,
                        0x19, 0x00, 0x18, 0x00, 0x19, 0x80, 0x19, 0x80, 0x3f, 0x80, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [15] = {
            .font = 0x46,       //Char:[F]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x19, 0x80, 0x19, 0x80, 0x18,
                        0x00, 0x19, 0x00, 0x1f, 0x00, 0x19, 0x00,
                        0x19, 0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0x00, 0x3e, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [16] = {
            .font = 0x47,       //Char:[G]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x0d, 0x80, 0x1b, 0x80, 0x31, 0x80, 0x31,
                        0x80, 0x30, 0x00, 0x30, 0x00, 0x37, 0x80,
                        0x31, 0x80, 0x31, 0x80, 0x31, 0x80, 0x1b, 0x80, 0x0d, 0x80, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [17] = {
            .font = 0x48,       //Char:[H]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x31, 0x80, 0x31, 0x80, 0x31, 0x80, 0x31,
                        0x80, 0x31, 0x80, 0x3f, 0x80, 0x31, 0x80,
                        0x31, 0x80, 0x31, 0x80, 0x31, 0x80, 0x31, 0x80, 0x31, 0x80, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [18] = {
            .font = 0x49,       //Char:[I]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x0c,
                        0x00, 0x0c, 0x00, 0x0c, 0x00, 0x0c, 0x00,
                        0x0c, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x3f, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [19] = {
            .font = 0x4a,       //Char:[J]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x06, 0x00, 0x06, 0x00, 0x06,
                        0x00, 0x06, 0x00, 0x06, 0x00, 0x06, 0x00,
                        0x06, 0x00, 0x36, 0x00, 0x36, 0x00, 0x36, 0x00, 0x3c, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [20] = {
            .font = 0x4b,       //Char:[K]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3b, 0x80, 0x19, 0x80, 0x1b, 0x00, 0x1b,
                        0x00, 0x1e, 0x00, 0x1c, 0x00, 0x1e, 0x00,
                        0x1b, 0x00, 0x1b, 0x00, 0x19, 0x80, 0x19, 0x80, 0x3b, 0x80, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [21] = {
            .font = 0x4c,       //Char:[L]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x18, 0x00, 0x18, 0x00, 0x18,
                        0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0x00,
                        0x18, 0x00, 0x19, 0x80, 0x19, 0x80, 0x19, 0x80, 0x3f, 0x80, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [22] = {
            .font = 0x4d,       //Char:[M]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x31, 0x80, 0x31, 0x80, 0x3b, 0x80, 0x3b,
                        0x80, 0x3b, 0x80, 0x3f, 0x80, 0x35, 0x80,
                        0x35, 0x80, 0x35, 0x80, 0x31, 0x80, 0x31, 0x80, 0x31, 0x80, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [23] = {
            .font = 0x4e,       //Char:[N]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3b, 0x80, 0x19, 0x80, 0x19, 0x80, 0x1d,
                        0x80, 0x1d, 0x80, 0x1d, 0x80, 0x1b, 0x80,
                        0x1b, 0x80, 0x1b, 0x80, 0x19, 0x80, 0x19, 0x80, 0x3d, 0x80, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [24] = {
            .font = 0x4f,       //Char:[O]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x31, 0x80, 0x31,
                        0x80, 0x31, 0x80, 0x31, 0x80, 0x31, 0x80,
                        0x31, 0x80, 0x31, 0x80, 0x31, 0x80, 0x1b, 0x00, 0x0e, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [25] = {
            .font = 0x50,       //Char:[P]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x1b, 0x00, 0x19, 0x80, 0x19,
                        0x80, 0x19, 0x80, 0x19, 0x80, 0x1b, 0x00,
                        0x1e, 0x00, 0x18, 0x00, 0x18, 0x00, 0x18, 0x00, 0x3e, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [26] = {
            .font = 0x51,       //Char:[Q]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x1b, 0x00, 0x31, 0x80, 0x31,
                        0x80, 0x31, 0x80, 0x31, 0x80, 0x31, 0x80,
                        0x31, 0x80, 0x35, 0x80, 0x3b, 0x80, 0x1b, 0x00, 0x0f, 0x00, 0x01,
                        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [27] = {
            .font = 0x52,       //Char:[R]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x19, 0x80, 0x19, 0x80, 0x19,
                        0x80, 0x19, 0x80, 0x1b, 0x00, 0x1e, 0x00,
                        0x1b, 0x00, 0x19, 0x80, 0x19, 0x80, 0x19, 0x80, 0x3d, 0x80, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [28] = {
            .font = 0x53,       //Char:[S]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x0e, 0x80, 0x1b, 0x80, 0x31, 0x80, 0x30,
                        0x80, 0x30, 0x80, 0x1c, 0x00, 0x07, 0x00,
                        0x21, 0x80, 0x21, 0x80, 0x31, 0x80, 0x3b, 0x00, 0x2e, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [29] = {
            .font = 0x54,       //Char:[T]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x16, 0x80, 0x16, 0x80, 0x16,
                        0x80, 0x06, 0x00, 0x06, 0x00, 0x06, 0x00,
                        0x06, 0x00, 0x06, 0x00, 0x06, 0x00, 0x06, 0x00, 0x0f, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [30] = {
            .font = 0x55,       //Char:[U]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3b, 0x80, 0x19, 0x80, 0x19, 0x80, 0x19,
                        0x80, 0x19, 0x80, 0x19, 0x80, 0x19, 0x80,
                        0x19, 0x80, 0x19, 0x80, 0x19, 0x80, 0x19, 0x80, 0x0f, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [31] = {
            .font = 0x56,       //Char:[V]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3b, 0x80, 0x1b, 0x00, 0x1b, 0x00, 0x1b,
                        0x00, 0x1b, 0x00, 0x1b, 0x00, 0x1b, 0x00,
                        0x0e, 0x00, 0x0e, 0x00, 0x0e, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [32] = {
            .font = 0x57,       //Char:[W]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x31, 0x80, 0x31, 0x80, 0x31, 0x80, 0x31,
                        0x80, 0x35, 0x80, 0x35, 0x80, 0x35, 0x80,
                        0x3f, 0x80, 0x1f, 0x00, 0x1b, 0x00, 0x1b, 0x00, 0x1b, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [33] = {
            .font = 0x58,       //Char:[X]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3b, 0x80, 0x3b, 0x80, 0x11, 0x00, 0x1b,
                        0x00, 0x1b, 0x00, 0x0e, 0x00, 0x0e, 0x00,
                        0x1b, 0x00, 0x1b, 0x00, 0x11, 0x00, 0x3b, 0x80, 0x3b, 0x80, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [34] = {
            .font = 0x59,       //Char:[Y]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3b, 0x80, 0x19, 0x80, 0x19, 0x80, 0x19,
                        0x80, 0x19, 0x80, 0x0f, 0x00, 0x06, 0x00,
                        0x06, 0x00, 0x06, 0x00, 0x06, 0x00, 0x06, 0x00, 0x0f, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [35] = {
            .font = 0x5a,       //Char:[Z]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x33, 0x00, 0x33, 0x00, 0x36,
                        0x00, 0x06, 0x00, 0x0c, 0x00, 0x0c, 0x00,
                        0x18, 0x00, 0x1b, 0x00, 0x33, 0x00, 0x33, 0x00, 0x3f, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [36] = {
            .font = 0x20,       //Char:[ ]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [37] = {
            .font = 0x2c,       //Char:[,]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x04,
                        0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [38] = {
            .font = 0x2d,       //Char:[-]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [39] = {
            .font = 0x2f,       //Char:[/]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x08,
                        0x00, 0x08, 0x00, 0x08, 0x00, 0x10, 0x00,
                        0x10, 0x00, 0x10, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [40] = {
            .font = 0x3a,       //Char:[:]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
    [41] = {
            .font = 0x3d,       //Char:[=]
            .fbitmap = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00,
                        0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,},
            },
};

///////////////////////////////////////////////////////////////////////////////
// INLINE FUNCTION DEFINITION
static inline void dump_reg(u32 vbs)
{
#define REG_SIZE 0x400
    volatile u32 *preg = (u32 *) vbs;
    u32 i = 0, j = 0;
    for (i = 0, j = 0; i < (REG_SIZE >> 2); i++, j++) {
        if (j == 0 || j >= 4) {
            printk(KERN_DEBUG "\n");
            printk(KERN_DEBUG "0x%04x:", i << 2);
            j = 0;
        }
        printk(KERN_DEBUG "  %08x", *preg);
        preg++;
    }
    printk(KERN_DEBUG "\n");
}

STATIC_INLINE void fscaler_device_timer_init(struct fscaler_device *dev)
{
    if (dev) {
        spin_lock_init(&dev->timer_lcok);
        dev->timer.function = fscaler_device_timer_handler;
        dev->timer.data = (unsigned long)dev;
        init_timer(&dev->timer);
        dev->timer_base = video_gettime_us() / 1000;
    }
}

STATIC_INLINE void fscaler_device_timer_trigger(struct fscaler_device *dev)
{
    if (dev) {
        if (timer_pending(&dev->timer))
            mod_timer(&dev->timer, jiffies + msecs_to_jiffies(timeout));
        else {
            dev->timer.expires = jiffies + msecs_to_jiffies(timeout);
            add_timer(&dev->timer);
        }
        dev->timer_base = video_gettime_us() / 1000;
    }
}

STATIC_INLINE void fscaler_device_timer_remove(struct fscaler_device *dev)
{
    if (dev) {
        if (timer_pending(&dev->timer))
            del_timer(&dev->timer);
    }
}

STATIC_INLINE void fscaler_device_tasklet_init(struct fscaler_device *dev)
{
    if (dev) {
        tasklet_init(&dev->irq_tasklet, fscaler_device_tasklet_handler, (unsigned long)dev);
    }
}

STATIC_INLINE void fscaler_device_tasklet_run(struct fscaler_device *dev)
{
    if (dev) {
        tasklet_hi_schedule(&dev->irq_tasklet);
    }
}

STATIC_INLINE void fscaler_device_tasklet_kill(struct fscaler_device *dev)
{
    if (dev) {
        tasklet_kill(&dev->irq_tasklet);
    }
}

STATIC_INLINE void fscaler_device_enable(struct fscaler_device *dev)
{
    unsigned int n = 30000, delay_ns = 1;
    if (dev && dev->base) {
        fscaler_hw_select_source(dev->base);
        fscaler_hw_caps(dev->base);
        fscaler_hw_enable(dev->base, 1);
        /* Delay and make sure device is enable */
        while (fscaler_reg_read(dev->base, IOAT_SCARSTS, msk1(24), 0) && --n) {
            ndelay(delay_ns);
        }
        if (unlikely(0 >= n)) {
            M_ERROR("SCL: enable: force cu time out!\n");
        }
        /* Dummy 5 times read for cu counter safe.
         * CU causes post counter launch and it is dangerous to fire scaler immediately */
        for (n = 0; n < 5; ++n) {
            fscaler_reg_read(dev->base, IOAT_SCARSTS, msk1(24), 0);
        }
    }
}

STATIC_INLINE void fscaler_device_disable(struct fscaler_device *dev, int b_soft)
{
    if (dev && dev->base) {
        fscaler_hw_enable(dev->base, 0);
        fscaler_hw_update(dev->base, 0);
        if (b_soft) {
            fscaler_hw_software_reset(dev->base, dev->irq_ret == FSCALER_HW_OK);
        }
        dev->irq_ret = 0;
    }
}

STATIC_INLINE void fscaler_device_init_status(struct fscaler_device *dev)
{
    if (dev && dev->base) {
        dev->irq_mask = (unsigned int)fscaler_hw_init_irqmask(dev->base);
        fscaler_hw_init_status(dev->base, (unsigned long)dev->irq_mask);
    }
}

STATIC_INLINE int fscaler_device_test_status(struct fscaler_device *dev)
{
    if (dev && dev->base) {
        return fscaler_hw_test_status(dev->base, (unsigned long)dev->irq_mask);
    }
    return FSCALER_OK;
}

STATIC_INLINE void fscaler_device_set_status(struct fscaler_device *dev, int status)
{
    struct fscaler_job *job = NULL;
    if (dev) {
        M_DEVELOPP("[setsts]<%d:%p>{%d,%d}\n", dev->id, dev, (int)dev->status, (int)status);
        job = fscaler_device_fetch_job(dev);
        dev->status = status;
        /* Record the device status to the job on process */
        switch (status) {
        case FSCALER_DEVICE_STATUS_IDLE:
            /* While scaler device is idle, there is no scaler job is on process.
             * It is not neccessary to keep the job status as other cases.
             */
            break;
        case FSCALER_DEVICE_STATUS_ONPROC:
            if (job) {
                job->status = FSCALER_JOB_STATUS_ONPROC;
            }
            break;
        case FSCALER_DEVICE_STATUS_ABORT:
            if (job) {
                job->status = FSCALER_JOB_STATUS_ABORT;
            }
            break;
        case FSCALER_DEVICE_STATUS_ERROR:
            if (job) {
                job->status = FSCALER_JOB_STATUS_ERROR;
            }
            break;
        case FSCALER_DEVICE_STATUS_DONE:
            if (job) {
                job->status = FSCALER_JOB_STATUS_DONE;
            }
            break;
        default:
            break;
        }
    }
}

STATIC_INLINE void fscaler_device_putjob(struct fscaler_device *dev, struct fscaler_job *job)
{
    unsigned long flags;
    if (dev) {
        spin_lock_irqsave(&dev->lock, flags);
        fscaler_job_set_magic(job, __func__);
        klist_add_tail(&job->knode_dev, &dev->klist_job);
        spin_unlock_irqrestore(&dev->lock, flags);
    }
}

STATIC_INLINE struct fscaler_job *fscaler_device_getjob(struct fscaler_device *dev)
{
    struct fscaler_job *job = NULL;
    unsigned long flags;
    if (dev) {
        spin_lock_irqsave(&dev->lock, flags);
        job = klist_entry(klist_pop(&dev->klist_job, 0), struct fscaler_job, knode_dev);
        fscaler_job_set_magic(job, __func__);
        spin_unlock_irqrestore(&dev->lock, flags);
    }
    return job;
}

STATIC_INLINE void fscaler_device_release_and_output_job(struct fscaler_device *dev, int status)
{
    struct fscaler_job *job = NULL;
    if (dev) {
        fscaler_device_set_status(dev, status);
        job = fscaler_device_release_job(dev);
        fscaler_job_set_magic(job, __func__);
        fscaler_device_putjob(dev, job);
        //fscaler_device_set_status(dev, FSCALER_DEVICE_STATUS_IDLE);
    }
}

STATIC_INLINE void fscaler_device_do_really_output(struct fscaler_device *dev)
{
    //unsigned long flags;
    struct fscaler_job *job = NULL;
    if (dev) {
        //spin_lock_irqsave(&dev->lock, flags);
        /* Pop front scaler jobs and render out by callbacks in the loop */
        job = fscaler_device_getjob(dev);
        fscaler_job_set_magic(job, __func__);
        while (job) {
            switch (job->status) {
            case FSCALER_JOB_STATUS_ONPROC:
                M_PANIC("SCL: Job status should not be on-proc in tasklet handler\n");
                break;
            case FSCALER_JOB_STATUS_ERROR:
                if (job->error) {
                    job->error(job);
                }
                break;
            case FSCALER_JOB_STATUS_ABORT:
                if (job->abort) {
                    job->abort(job);
                }
                break;

            case FSCALER_JOB_STATUS_DONE:
                if (job->done) {
                    job->done(job);
                }
                break;
            default:
                M_PANIC("SCL: Unexpected status of scaler job\n");
                if (job->error) {
                    job->error(job);
                }
                break;
            }
            fscaler_job_remove(fscaler_job_to_table(job), job);
            job = fscaler_device_getjob(dev);
        }
        if (dev->kpi_up) {
            fscaler_kpi_show(&dev->show);
            dev->kpi_up = 0;
        }
        //spin_unlock_irqrestore(&dev->lock, flags);
    }
}

STATIC_INLINE void fscaler_device_grab_job(struct fscaler_device *dev)
{
    if (dev && dev->grab) {
        if (unlikely(!dev->opened)) {
            M_PANIC("SCL: Device should be open\n");
            return;
        }
        dev->grab(dev);
    }
}

STATIC_INLINE void fscaler_device_do_job(struct fscaler_device *dev)
{
    struct fscaler_job *job = NULL;
    struct fscaler_subjob *sjob = NULL;
    if (dev) {
        if (unlikely(!dev->opened)) {
            M_PANIC("SCL: Device should be open\n");
            return;
        }
        if (unlikely(!fscaler_device_locked(dev))) {
            M_PANIC("SCL: Device should be locked\n");
            return;
        }
        job = fscaler_device_fetch_job(dev);
        if (job) {
            fscaler_job_set_magic(job, __func__);
            sjob = fscaler_job_get_subjob(job, job->curr);
            if (sjob) {
                M_DEVELOPP("##dojob##curr:%d type:%d\n", job->curr, job->type);
                /* Setup scaler hardware by job types */
                fscaler_device_set_status(dev, FSCALER_DEVICE_STATUS_ONPROC);
                switch (job->type) {
                case FSCALER_JOB_TYPE_SINGLE:
                case FSCALER_JOB_TYPE_TWICE:
                case FSCALER_JOB_TYPE_TRIPLE:
                    /* Single and twice type of scaler jobs do the same scale operation
                     * as the individual event for the scaler. The process is a
                     * disable-setup-enable process to reduce one calculation. Because
                     * "enable" will force to do a calculation and it is not neccessarry
                     * trigger the scaler to calculate again explicitly. */
                    fscaler_device_disable(dev, BUG_FORCE_RETRIGER_ON());
                    fscaler_osd_disable(dev, job);
                    fscaler_osd_set_display(dev, job);
                    fscaler_device_init_status(dev);
                    fscaler_hw_set_target_background(dev->base, sjob->out_w, sjob->out_h);
                    fscaler_hw_set_target_window(dev->base, sjob->out_w, sjob->out_h);
                    fscaler_hw_set_target_frame(dev->base, sjob->out_x, sjob->out_y,
                                                sjob->out_w, sjob->out_bg_w, &sjob->out_buf);
                    fscaler_hw_set_source_background(dev->base, sjob->in_bg_w, sjob->in_bg_h);
                    fscaler_hw_set_source_window(dev->base, sjob->in_x, sjob->in_y, sjob->in_w,
                                                 sjob->in_h);
                    fscaler_hw_set_source_frame(dev->base, sjob->in_x, sjob->in_y, sjob->in_bg_w,
                                                &sjob->in_buf);
                    fscaler_kpi_time_start(&(dev->kpi.time));
                    fscaler_kpi_value_add(&(dev->kpi.value),
                                          sjob->out_w * sjob->out_h + sjob->in_w * sjob->in_h);
                    fscaler_osd_enable(dev, job);
                    fscaler_device_enable(dev);
                    fscaler_hw_frame_trigger(dev->base, 1);
                    M_DEVELOPP("SCL: fire job:%d\n", job->type);
                    /* Fire the timer to handler the timeout situation */
                    fscaler_device_timer_trigger(dev);
                    break;
                case FSCALER_JOB_TYPE_PASS:
                    /* Pass job type is consifered as fake finish */
                    M_DEVELOPP("SCL: do pass job\n");
                    fscaler_device_release_and_output_job(dev, FSCALER_DEVICE_STATUS_DONE);
                    fscaler_device_tasklet_run(dev);
                    break;
                default:
                    /* Unsupport job type is considered as abort */
                    printk("SCL: do unsupported job\n");
                    fscaler_device_release_and_output_job(dev, FSCALER_DEVICE_STATUS_ABORT);
                    fscaler_device_tasklet_run(dev);
                    break;
                }
            }
        }
    }
}

STATIC_INLINE void fscaler_device_reboot(struct fscaler_device *dev)
{
    unsigned long flags;
    if (dev) {
        spin_lock_irqsave(&dev->timer_lcok, flags);
        fscaler_job_set_magic(dev->job, __func__);
        //printk("SCL: dev->retry:%d\n", dev->retry);
        if (dev->job && dev->job->retry < max_retry) {
            //printk("SCL: reboot, retry:%d, max_retry:%d\n", dev->job->retry, max_retry);
            ++dev->job->retry;
            ++dev->retry;
            dev->job->curr = 0;
            fscaler_device_do_job(dev);
            spin_unlock_irqrestore(&dev->timer_lcok, flags);
        } else {
            //printk("SCL: reboot, max_retry:%d\n", max_retry);
            fscaler_device_release_and_output_job(dev, FSCALER_DEVICE_STATUS_ERROR);
            fscaler_device_disable(dev, 1);
            spin_unlock_irqrestore(&dev->timer_lcok, flags);
            fscaler_device_grab_job(dev);
            fscaler_device_tasklet_run(dev);
        }
    }
}

STATIC_INLINE int fscaler_osd_init(struct fscaler_driver *drv)
{
    int i = 0, j = 0, max = ARRAY_SIZE(FSCALER_OSD_FONTS);
    struct fscaler_device *dev = NULL;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (drv && table) {
        for (i = 0; i < table->max; ++i) {
            dev = fscaler_device_find(table, i);
            if (dev) {
                bitmap_zero((unsigned long *)&dev->osd.map.bitmap[0],
                            ARRAY_SIZE(dev->osd.map.index));
                bitmap_zero((unsigned long *)&dev->osd.map.fbitmap[0], OSD_MAX_CHAR_NUM);
                memset(dev->osd.map.index, -1, sizeof(dev->osd.map.index));
                dev->osd.map.count = 0;
                dev->osd.pluged = 1;
                dev->osd.check = 0xFFFFFFFF;
                memset(dev->osd.palette, -1, sizeof(dev->osd.palette));
                dev->osd.trans[0].level = FOSD_TRANSPARENT_0PERCENT;
                memset(&dev->osd.disp[0], -1, sizeof(dev->osd.disp[0]));
#ifdef DYNAMIC_OSD_RESOURCE
                dev->osd.active = FSCALER_ALLOC(sizeof(int), max_entity);
                dev->osd.dispram_update = FSCALER_ALLOC(sizeof(int), max_entity);
                dev->osd.win = FSCALER_ALLOC(sizeof(struct fiosd_win), max_entity);
                dev->osd.str = FSCALER_ALLOC(sizeof(struct fiosd_string), max_entity);
                dev->osd.font = FSCALER_ALLOC(sizeof(struct fiosd_font_info), max_entity);
                if (!dev->osd.active || !dev->osd.dispram_update || !dev->osd.win || !dev->osd.str
                    || !dev->osd.font) {
                    break;
                }
#endif
                for (j = 0; j < max_entity; ++j) {
                    dev->osd.active[j] = 0;
                    dev->osd.dispram_update[j] = 0;
                    memset(&dev->osd.win[j], 0, sizeof(dev->osd.win[j]));
                    dev->osd.win[j].channel = j;
                    memset(&dev->osd.str[j], 0, sizeof(dev->osd.str[j]));
                    dev->osd.str[j].channel = j;
                    dev->osd.font[j].width = FSCALER_OSD_MAX_FONT_W;
                    dev->osd.font[j].height = FSCALER_OSD_MAX_FONT_H;
                    dev->osd.font[j].row_space = 0;
                    dev->osd.font[j].col_space = 0;
                    dev->osd.font[j].channel = j;
                }
            }
        }
        if (i >= table->max) {
            if (max > OSD_MAX_CHAR_NUM) {
                M_WARN("SCL: out of osd font max\n");
                max = OSD_MAX_CHAR_NUM;
            }
            for (i = 0; i < max; ++i) {
                fscaler_osd_set_font(drv, &FSCALER_OSD_FONTS[i], 1);
            }
            return FSCALER_OK;
        }
#ifdef DYNAMIC_OSD_RESOURCE
        if (dev->osd.font) {
            FSCALER_FREE(dev->osd.font);
            dev->osd.font = NULL;
        }
        if (dev->osd.str) {
            FSCALER_FREE(dev->osd.str);
            dev->osd.str = NULL;
        }
        if (dev->osd.win) {
            FSCALER_FREE(dev->osd.win);
            dev->osd.win = NULL;
        }
        if (dev->osd.dispram_update) {
            FSCALER_FREE(dev->osd.dispram_update);
            dev->osd.dispram_update = NULL;
        }
        if (dev->osd.active) {
            FSCALER_FREE(dev->osd.active);
            dev->osd.active = NULL;
        }
#endif /* DYNAMIC_OSD_RESOURCE */
    }
    return FSCALER_ERR_CREATE;
}

STATIC_INLINE void fscaler_osd_deinit(struct fscaler_driver *drv)
{
#ifdef DYNAMIC_OSD_RESOURCE
    int i = 0;
    struct fscaler_device *dev = NULL;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (drv && table) {
        for (i = 0; i < table->max; ++i) {
            dev = fscaler_device_find(table, i);
            if (dev) {
                if (dev->osd.font) {
                    FSCALER_FREE(dev->osd.font);
                    dev->osd.font = NULL;
                }
                if (dev->osd.str) {
                    FSCALER_FREE(dev->osd.str);
                    dev->osd.str = NULL;
                }
                if (dev->osd.win) {
                    FSCALER_FREE(dev->osd.win);
                    dev->osd.win = NULL;
                }
                if (dev->osd.dispram_update) {
                    FSCALER_FREE(dev->osd.dispram_update);
                    dev->osd.dispram_update = NULL;
                }
                if (dev->osd.active) {
                    FSCALER_FREE(dev->osd.active);
                    dev->osd.active = NULL;
                }
            }
        }
    }
#endif
}

STATIC_INLINE void fscaler_osd_setup_palette(struct fscaler_device *dev, int index, unsigned char y,
                                             unsigned char cb, unsigned char cr)
{
    unsigned int palette = 0;
    if (dev) {
        palette = (((y >> 2) & 0x3F) << 16) | (((cb >> 3) & 0x1F) << 8) | ((cr >> 3) & 0x1F);
        fscaler_hw_set_osd_palette(dev->base, index, palette);
    }
}

STATIC_INLINE struct fscaler_device *fscaler_device_allocate(struct fscaler_table *table, int index)
{
    struct fscaler_device *dev = NULL;
    if (table && table->map && table->kmem && 0 <= index && table->max > index) {
        dev = FSCALER_KMEM_ALLOC(table->kmem);
        if (dev) {
            dev->parent = table;
            dev->base = 0;
            dev->id = index;
            dev->status = FSCALER_DEVICE_STATUS_IDLE;
            dev->opened = 0;
            dev->irq = -1;
            dev->irq_mask = 0;
            dev->irq_ret = 0;
            dev->job = NULL;
            dev->retry = 0;
            spin_lock_init(&dev->lock);
            fscaler_device_timer_init(dev);
            dev->kpi_up = 0;
            fscaler_kpi_init(&dev->kpi, index);
            klist_init(&dev->klist_job, 0, 0);
            dev->osd.pluged = 0;
#ifdef DYNAMIC_OSD_RESOURCE
            dev->osd.active = NULL;
            dev->osd.win = NULL;
            dev->osd.str = NULL;
            dev->osd.font = NULL;
#endif
            M_DEVELOPP("SCL: dev:%p, id:%d\n", dev, dev->id);
            return dev;
        }
    }
    return NULL;
}

STATIC_INLINE void fscaler_device_free(struct fscaler_table *table, struct fscaler_device *dev)
{
    if (table && table->kmem && dev) {
        dev->parent = NULL;
        FSCALER_KMEM_FREE(table->kmem, (void *)dev);
        dev = NULL;
    }
}

STATIC_INLINE int fscaler_device_irq_request(struct fscaler_device *dev)
{
    int ret = 0;
    if (dev && dev->irq >= 0) {
        ret = request_irq(dev->irq, fscaler_device_irq_handler, SA_INTERRUPT, IRQ_NAME, dev);
        if (0 <= ret) {
            fscaler_device_tasklet_init(dev);
            return FSCALER_OK;
        }
        dev->irq = -1;
    }
    return FSCALER_ERR_OPEN;
}

STATIC_INLINE void fscaler_device_irq_free(struct fscaler_device *dev)
{
    if (dev && dev->irq >= 0) {
        fscaler_device_tasklet_kill(dev);
        free_irq(dev->irq, dev);
        dev->irq = -1;
    }
}

///////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION IMPLAMENTATION
int fscaler_device_cache(struct fscaler_table *table)
{
    if (table) {
        table->kmem = FSCALER_KMEM_CACHE(fscaler_device, SLAB_HWCACHE_ALIGN);
        if (table->kmem) {
            return FSCALER_OK;
        }
    }
    return FSCALER_ERR_CREATE;
}

void fscaler_device_uncache(struct fscaler_table *table)
{
    if (table && table->kmem) {
        FSCALER_KMEM_UNCACHE(table->kmem);
        table->kmem = NULL;
    }
}

void *fscaler_device_request(struct fscaler_table *table, int index)
{
    struct fscaler_device *dev = fscaler_device_allocate(table, index);
    unsigned long flags;
    if (dev) {
        fscaler_device_callback_register(dev, 0);
        spin_lock_irqsave(&table->lock, flags);
        klist_add_tail(&dev->knode_table, &table->klist_item);
        spin_unlock_irqrestore(&table->lock, flags);
        return dev;
    }
    return NULL;
}

void fscaler_device_remove(struct fscaler_table *table, void *data)
{
    struct fscaler_device *dev = (struct fscaler_device *)data;
    unsigned long flags;
    if (table && dev) {
        spin_lock_irqsave(&table->lock, flags);
        klist_del(&dev->knode_table);
        spin_unlock_irqrestore(&table->lock, flags);
        fscaler_device_free(table, dev);
        dev = NULL;
    }
}

int fscaler_device_compare(void *c0, void *c1)
{
    struct fscaler_device *dev = (struct fscaler_device *)c0;
    struct fscaler_device *found = (struct fscaler_device *)c1;
    if (dev && !found) {
        return LKP_CODE(0, LKP_ACT_EXCHANGE);
    }
    return LKP_CODE(0, 0);
}

void fscaler_device_timer_handler(unsigned long data)
{
    unsigned int elapse = 0, curr = video_gettime_us() / 1000;
    struct fscaler_device *dev = (struct fscaler_device *)data;
    if (dev) {
        /* Calculate elapsed time from last trigger of this device */
        if (curr >= dev->timer_base) {
            elapse = curr - dev->timer_base;
        } else {
            elapse = video_gettime_max_ms() - dev->timer_base + curr;
        }
        if (elapse >= timeout) {
            /* Timout. Dump somthing, restore device, grab a new job
             * to proceed and run the render tasklet */
            fscaler_job_print_verbos("timeout", dev->job);
            dump_reg(dev->base);
            M_DEVELOPP("SCL: timeout\n");
            fscaler_device_reboot(dev);
        } else {
            M_WARN("SCL: Fake device timer timeout:%d(%d) ms\n", elapse, timeout);
            mod_timer(&dev->timer, jiffies + msecs_to_jiffies(timeout - elapse));
        }
    }
}

void fscaler_device_tasklet_handler(unsigned long data)
{
    fscaler_device_do_really_output((struct fscaler_device *)data);
}

int fscaler_osd_get_index(struct fscaler_device *dev, char data)
{
    if (test_bit(data, (const volatile void *)&dev->osd.map.bitmap[0])) {
        return (int)(dev->osd.map.index[(u32) data]);
    }
    return 0;
}

void fscaler_osd_disable(struct fscaler_device *dev, struct fscaler_job *job)
{
    if (supp_osd) {
        if (dev && dev->osd.pluged && job) {
            M_DEVELOPP("SCL: osd active:%d, fd:%d\n", dev->osd.active[job->fd], job->fd);
            if (dev->osd.active[job->fd]) {
                dev->osd.check = job->start_time;
                fscaler_hw_osd_enable(dev->base, 0, 0);
                M_DEVELOPP("SCL: osd disable\n");
            }
        }
    }
}

void fscaler_osd_enable(struct fscaler_device *dev, struct fscaler_job *job)
{
    if (supp_osd) {
        if (dev && dev->osd.pluged && job) {
            M_DEVELOPP("SCL: osd active:%d, fd:%d\n", dev->osd.active[job->fd], job->fd);
            if (dev->osd.active[job->fd] && dev->osd.check == job->start_time) {
                fscaler_hw_osd_enable(dev->base, 0, 1);
                M_DEVELOPP("SCL: osd enable\n");
            } else {
                fscaler_hw_osd_enable(dev->base, 0, 0);
                M_DEVELOPP("SCL: osd disable\n");
            }
        }
    }
}

void fscaler_osd_set_display(struct fscaler_device *dev, struct fscaler_job *job)
{
    int i = 0;
    struct fscaler_subjob *sjob = NULL;
    struct fiosd_string *str = NULL;
    struct fiosd_font_info *font = NULL;
    struct fiosd_transparent *trans = NULL;
    struct fiosd_win *win = NULL;
    struct fiosd_palette *pal = NULL;
    int x = 0, y = 0, w = 0, h = 0, ofs_x = 0, ofs_y = 0;
    if (supp_osd) {
        if (dev && dev->osd.pluged && job) {
            if (dev->osd.active[job->fd] && dev->osd.check == job->start_time) {
                M_DEVELOPP("SCL: osd job fd:%d, curr:%d\n", job->fd, job->curr);
                str = &dev->osd.str[job->fd];
                font = &dev->osd.font[job->fd];
                trans = &dev->osd.trans[0];
                win = &dev->osd.win[job->fd];
                sjob = fscaler_job_get_subjob(job, job->curr);
                /* control dispaly ram */
                M_DEVELOPP("SCL: dev->osd.dispram_update[%d]:%d\n", job->fd,
                           dev->osd.dispram_update[job->fd]);
                if (dev->osd.dispram_update[job->fd]) {
                    for (i = 0; i < strlen(str->string); ++i) {
                        dev->osd.disp[0].font[str->start + i] = str->string[i];
                        dev->osd.disp[0].index[str->start + i] =
                            fscaler_osd_get_index(dev, str->string[i]);
                        fscaler_hw_set_osd_display_ram(dev->base, str->start + i,
                                                       dev->osd.disp[0].index[str->start + i]);
                    }
                    dev->osd.dispram_update[job->fd] = 0;
                }
                /* set palette */
                for (i = 0; i < ARRAY_SIZE(dev->osd.palette); ++i) {
                    pal = &dev->osd.palette[i];
                    fscaler_osd_setup_palette(dev, pal->index, pal->y, pal->cb, pal->cr);
                }
                /* set colr */
                fscaler_hw_set_osd_color(dev->base, 0, trans->level, str->bg_color, str->fg_color);
                /* set window */
                w = (int)(win->hdim);   ///< dim = real size, need not to be transformed
                h = (int)(win->vdim);   ///< dim = real size, need not to be transformed
                ofs_x = (int)OFFSET2X(sjob->out_bg_w, win->x);
                ofs_y = (int)OFFSET2Y(sjob->out_bg_h, win->y);
#if 0
                if (ofs_x >= sjob->out_x) {
                    x = ofs_x - sjob->out_x;
                } else {
                    x = 0;
                }
                if (ofs_y >= sjob->out_y) {
                    y = ofs_y - sjob->out_y;
                } else {
                    y = 0;
                }
#else
                x = 0;
                if (ofs_x >= sjob->out_x) {
                    sjob->in_x = ofs_x;
                    sjob->out_x = ofs_x;
                } else {
                    sjob->in_x = sjob->out_x;
                }
                sjob->in_w = w * (font->width + font->row_space);
                sjob->out_w = w * (font->width + font->row_space);
                y = 0;
                if (ofs_y >= sjob->out_y) {
                    sjob->in_y = ofs_y;
                    sjob->out_y = ofs_y;
                } else {
                    sjob->in_y = sjob->out_y;
                }
                sjob->in_h = h * (font->height + font->col_space);
                sjob->out_h = h * (font->height + font->col_space);
#endif
                M_DEVELOPP("SCL: osd size:%d %d %d %d\n", x, y, w, h);
                fscaler_hw_set_osd_win(dev->base, 0, str->start, x, y, w, h);
                /* set font font */
                fscaler_hw_set_osd_font_size(dev->base, 0, font->row_space, font->col_space,
                                             font->width, font->height);
            }
        }
    }
}

IRQ_HANDLER(irqreturn_t, fscaler_device_irq_handler, irq, dev_id)
{
    int ret = 0;
    struct fscaler_device *dev = (struct fscaler_device *)dev_id;
    M_DEVELOPP("SCL: #I# wakeup, %p\n", dev);
    if (dev) {
        fscaler_job_set_magic(dev->job, __func__);
        fscaler_device_timer_remove(dev);       ///< Remove the timeout handler since IRQ is raised.
        if (fscaler_kpi_time_end(&(dev->kpi), &(dev->show))) {
            //memcpy(&dev->show, &dev->kpi, sizeof(dev->kpi));
            //fscaler_kpi_value_init(&(dev->kpi.value));
            dev->kpi_up = 1;
        }
        /* Test the scaler hardware by detect the bits of status register and do something */
        ret = fscaler_device_test_status(dev);
        if (FSCALER_FAILED(ret)) {
            dev->irq_ret = ret;
            printk("SCL: #I#, reboot\n");
            /* Failure. Set status error and push to the renderred list */
            fscaler_device_reboot(dev);
        } else {
            if (FSCALER_HW_OK == ret) {
                dev->irq_ret = FSCALER_HW_OK;
                if (fscaler_job_check_finished(dev->job)) {
                    M_DEVELOPP("SCL: #I#, finish\n");
                    /* Finsih. Set status done and push it to renderred list */
                    fscaler_device_release_and_output_job(dev, FSCALER_DEVICE_STATUS_DONE);
                    fscaler_device_grab_job(dev);
                    fscaler_device_tasklet_run(dev);
                } else {
                    M_DEVELOPP("SCL: #I#, next\n");
                    /* Do not finish yet. Shift to next subjob and proceed with the same device */
                    fscaler_job_to_next_subjob(dev->job);
                    fscaler_device_do_job(dev);
                    goto EXIT;
                }
            } else if (FSCALER_HW_OK_FAKE_DMAOVF == ret) {
                /* Fake overflow case, skip it */
                M_DEVELOPP("SCL: #I#, fake\n");
                goto EXIT;
            }
        }
    }
  EXIT:
    M_DEVELOPP("SCL: #I# leave\n");
    return IRQ_HANDLED;
}

///////////////////////////////////////////////////////////////////////////////
// EXPORTED FUCNTION IMPLAMENTATION
/**
 * @function int fscaler_device_table_request(struct fscaler_driver *drv)
 * @brief To request device table
 * @param drv Scaler driver pointer
 * @return int Return code
 * @retval FSCALER_OK Success
 * @retval FSCALER_ERR_CREATE Failed to request device table
 */
int fscaler_device_table_request(struct fscaler_driver *drv)
{
    int hr = 0;
    hr = fscaler_table_request(drv, &FSCALER_DEVICE_TABLE_NAME[0],
                               max_device, 0, 0,
                               fscaler_device_cache, fscaler_device_uncache,
                               fscaler_device_request, fscaler_device_remove,
                               fscaler_device_compare);
    if (FSCALER_SUCCEED(hr)) {
        return FSCALER_OK;
    }
    return FSCALER_ERR_CREATE;
}

/**
 * @function void fscaler_device_table_remove(struct fscaler_driver *drv)
 * @brief To remove device table
 * @param drv Scaler driver pointer
 * @return None
 */
void fscaler_device_table_remove(struct fscaler_driver *drv)
{
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (table) {
        fscaler_table_remove(table);
        table = NULL;
    }
}

/**
 * @function const char *fscaler_device_table_name(void)
 * @brief To get the device table name
 * @param None
 * @return const char* C-string of device table name
 */
const char *fscaler_device_table_name(void)
{
    return &FSCALER_DEVICE_TABLE_NAME[0];
}

/**
 * @function int fscaler_device_open(struct fscaler_device *dev)
 * @brief To open scaler device
 * @param dev Scaler device pointer
 * @return int Return code
 * @retval FSCALER_OK Success
 * @retval FSCALER_ERR_OPEN Failed to open scaler device
 */
int fscaler_device_open(struct fscaler_device *dev)
{
    int hr = 0;
    if (dev && dev->irq >= 0) {
        if (!dev->opened) {
            dev->opened = 1;
            hr = fscaler_device_irq_request(dev);
            if (FSCALER_FAILED(hr)) {
                M_DEVELOPP("Failed to request irq:%d\n", dev->irq);
                goto ERROR_EXIT;
            }
            fscaler_hw_power(dev->id, 1);
            fscaler_device_unlock(dev);
        }
        return FSCALER_OK;
    }
  ERROR_EXIT:
    return FSCALER_ERR_OPEN;
}

/**
 * @function void fscaler_device_close(struct fscaler_device *dev)
 * @brief To close scaler device
 * @param dev Scaler device pointer
 * @return None
 */
void fscaler_device_close(struct fscaler_device *dev)
{
    if (dev && dev->opened) {
        fscaler_device_lock(dev);
        fscaler_device_disable(dev, 0);
        fscaler_hw_power(dev->id, 0);
        fscaler_device_irq_free(dev);
        fscaler_device_release_and_output_job(dev, FSCALER_DEVICE_STATUS_ABORT);
        fscaler_device_do_really_output(dev);
        dev->opened = 0;
    }
}

/**
 * @function fscaler_device_process(struct fscaler_device *dev)
 * @brief To proceed scaler device locked with job
 * @param dev Scaler device pointer
 * @return None
 */
void fscaler_device_process(struct fscaler_device *dev)
{
    fscaler_device_do_job(dev);
}

/**
 * @function void fscaler_device_receive_job(struct fscaler_device *dev, struct fscaler_job *job)
 * @brief To put the job into scaler device
 * @param dev Scaler device pointer
 * @param job to put
 * @return None
 */
void fscaler_device_receive_job(struct fscaler_device *dev, struct fscaler_job *job)
{
    if (dev && job) {
        fscaler_job_set_magic(job, __func__);
        if (dev->opened) {
            fscaler_device_lock(dev);
            job->id = dev->id;
            dev->job = job;
            job = NULL;
        }
    }
}

/**
 * @function struct fscaler_job *fscaler_device_fetch_job(struct fscaler_device *dev)
 * @brief To fetch job hold by the scaler device
 * @param dev Scaler device pointer
 * @return struct fscaler_job* Job pointer hold by the device
 * @retval NULL Failed to fetch job
 */
struct fscaler_job *fscaler_device_fetch_job(struct fscaler_device *dev)
{
    if (dev && dev->opened) {
        if (fscaler_device_locked(dev)) {
            if (!dev->job) {
                M_DEVELOPP("SCL: Device should not hold null job\n");
            }
            fscaler_job_set_magic(dev->job, __func__);
            return dev->job;
        }
    }
    return NULL;
}

/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
struct fscaler_job *fscaler_device_release_job(struct fscaler_device *dev)
{
    struct fscaler_job *job = NULL;
    if (dev && dev->opened) {
        fscaler_job_set_magic(dev->job, __func__);
        job = dev->job;
        dev->job = NULL;
        fscaler_device_unlock(dev);
    }
    return job;
}

/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
int fscaler_osd_request(struct fscaler_driver *drv)
{
    return fscaler_osd_init(drv);
}

/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
void fscaler_osd_remove(struct fscaler_driver *drv)
{
    return fscaler_osd_deinit(drv);
}

/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
int fscaler_osd_set_enable(struct fscaler_driver *drv, struct fiosd_ch *data, int enable)
{
    int i = 0;
    struct fscaler_device *dev = NULL;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (table && data) {
        for (i = 0; i < table->max; ++i) {
            dev = fscaler_device_find(table, i);
            if (dev) {
                dev->osd.active[data->channel] = enable;
            }
        }
        return FSCALER_OK;
    }
    return FSCALER_ERR_FAIL;
}

/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
int fscaler_osd_set_font(struct fscaler_driver *drv, struct fiosd_char *data, int set)
{
    int i = 0, findex = -1;
    struct fscaler_device *dev = NULL;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (table && data) {
        for (i = 0; i < table->max; ++i) {
            dev = fscaler_device_find(table, i);
            if (dev) {
                if (set) {
                    if (test_bit(data->font, (const volatile void *)&dev->osd.map.bitmap[0])) {
                        findex = dev->osd.map.index[(u32) data->font];
                    } else {
                        if (dev->osd.map.count < OSD_MAX_CHAR_NUM) {
                            findex = find_first_zero_bit(dev->osd.map.fbitmap, OSD_MAX_CHAR_NUM);
                            if (findex >= 0 && findex < OSD_MAX_CHAR_NUM) {
                                set_bit(findex, (volatile void *)&dev->osd.map.fbitmap[0]);
                                set_bit(data->font, (volatile void *)&dev->osd.map.bitmap[0]);
                                dev->osd.map.index[(u32) data->font] = findex;
                                dev->osd.map.count++;
                            }
                        }
                    }
                    if (findex >= 0 && findex < OSD_MAX_CHAR_NUM) {
                        fscaler_hw_set_font_data(dev->base, findex, &data->fbitmap[0],
                                                 FSCALER_OSD_MAX_FONT_H);
                    }
                } else {
                    if (0 == data->font) {
                        bitmap_zero((unsigned long *)&dev->osd.map.fbitmap[0], OSD_MAX_CHAR_NUM);
                        bitmap_zero((unsigned long *)&dev->osd.map.bitmap[0],
                                    ARRAY_SIZE(dev->osd.map.index));
                        memset(dev->osd.map.index, 0, sizeof(dev->osd.map.index));
                        dev->osd.map.count = 0;
                    } else if (test_bit(data->font, (const volatile void *)&dev->osd.map.bitmap[0])) {
                        findex = dev->osd.map.index[(u32) data->font];
                        clear_bit(findex, (volatile void *)&dev->osd.map.fbitmap[0]);
                        clear_bit(data->font, (volatile void *)&dev->osd.map.bitmap[0]);
                        dev->osd.map.count--;
                    }
                }
            }
        }
        return FSCALER_OK;
    }
    return FSCALER_ERR_FAIL;
}

/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
int fscaler_osd_set_palette(struct fscaler_driver *drv, struct fiosd_palette *data)
{
    int i = 0;
    struct fscaler_device *dev = NULL;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (table && data && data->index < 7) {
        for (i = 0; i < table->max; ++i) {
            dev = fscaler_device_find(table, i);
            if (dev) {
                memcpy(&dev->osd.palette[(u32) data->index], data, sizeof(*data));
                //fscaler_osd_setup_palette(dev, data->index, data->y, data->cb, data->cr);
            }
        }
        return FSCALER_OK;
    }
    return FSCALER_ERR_FAIL;
}

/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
int fscaler_osd_set_win(struct fscaler_driver *drv, struct fiosd_win *data)
{
    int i = 0;
    struct fscaler_device *dev = NULL;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (table && data) {
        for (i = 0; i < table->max; ++i) {
            dev = fscaler_device_find(table, i);
            if (dev) {
                memcpy(&dev->osd.win[(u32) data->channel], data, sizeof(*data));
            }
        }
        return FSCALER_OK;
    }
    return FSCALER_ERR_FAIL;
}

/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
int fscaler_osd_set_font_size(struct fscaler_driver *drv, struct fiosd_font_info *data)
{
    int i = 0;
    struct fscaler_device *dev = NULL;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (table && data) {
        for (i = 0; i < table->max; ++i) {
            dev = fscaler_device_find(table, i);
            if (dev) {
                memcpy(&dev->osd.font[(u32) data->channel], data, sizeof(*data));
                if (BUG_FIXED_OSD_FONT_SIZE_ON()) {
                    dev->osd.font[(u32) data->channel].width = FSCALER_OSD_MAX_FONT_W;
                    dev->osd.font[(u32) data->channel].height = FSCALER_OSD_MAX_FONT_H;
                }
                M_DEVELOPP("SCL: row:%d col:%d w:%d h:%d\n",
                           dev->osd.font[(u32) data->channel].row_space,
                           dev->osd.font[(u32) data->channel].col_space,
                           dev->osd.font[(u32) data->channel].width,
                           dev->osd.font[(u32) data->channel].height);
            }
        }
        return FSCALER_OK;
    }
    return FSCALER_ERR_FAIL;
}

/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
int fscaler_osd_set_transparent(struct fscaler_driver *drv, struct fiosd_transparent *data)
{
    int i = 0;
    struct fscaler_device *dev = NULL;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (table && data && data->level < FSCOSD_TRANS_MAX) {
        for (i = 0; i < table->max; ++i) {
            dev = fscaler_device_find(table, i);
            if (dev) {
                memcpy(&dev->osd.trans[0], data, sizeof(dev->osd.trans[0]));
                dev->osd.trans[0] = *data;
            }
        }
        return FSCALER_OK;
    }
    return FSCALER_ERR_FAIL;
}

/**
 * @function
 * @brief
 * @param
 * @param
 * @param
 * @return
 * @retval
 */
int fscaler_osd_set_string(struct fscaler_driver *drv, struct fiosd_string *data)
{
    int i = 0;
    struct fscaler_device *dev = NULL;
    struct fscaler_table *table = fscaler_driver_table_find(drv, fscaler_device_table_name());
    if (table && data) {
        for (i = 0; i < table->max; ++i) {
            dev = fscaler_device_find(table, i);
            if (dev) {
                memcpy(&dev->osd.str[(u32) data->channel], data, sizeof(*data));
                dev->osd.dispram_update[(u32) data->channel] = 1;
                M_DEVELOPP("SCL: str:%s\n", dev->osd.str[(u32) data->channel].string);
            }
        }
        return FSCALER_OK;
    }
    return FSCALER_ERR_FAIL;
}

///////////////////////////////////////////////////////////////////////////////
// EXPORT SYMOBL
EXPORT_SYMBOL(fscaler_device_table_request);
EXPORT_SYMBOL(fscaler_device_table_remove);
EXPORT_SYMBOL(fscaler_device_open);
EXPORT_SYMBOL(fscaler_device_close);
EXPORT_SYMBOL(fscaler_device_process);
EXPORT_SYMBOL(fscaler_device_receive_job);
EXPORT_SYMBOL(fscaler_device_fetch_job);
EXPORT_SYMBOL(fscaler_device_release_job);
EXPORT_SYMBOL(fscaler_osd_request);
EXPORT_SYMBOL(fscaler_osd_remove);
EXPORT_SYMBOL(fscaler_osd_set_enable);
EXPORT_SYMBOL(fscaler_osd_set_font);
EXPORT_SYMBOL(fscaler_osd_set_palette);
EXPORT_SYMBOL(fscaler_osd_set_win);
EXPORT_SYMBOL(fscaler_osd_set_font_size);
EXPORT_SYMBOL(fscaler_osd_set_transparent);
EXPORT_SYMBOL(fscaler_osd_set_string);

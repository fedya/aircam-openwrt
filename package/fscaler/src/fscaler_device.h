/**
 * @file fscaler_device.h
 * @brief Scaler device table
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */
#ifndef _FSCALER_DEVICE_H_
#define _FSCALER_DEVICE_H_

///////////////////////////////////////////////////////////////////////////////
// TYPE FORWARD DECLARTION
struct fscaler_device;
struct fscaler_driver;
struct fscaler_job;
struct fscaler_table;
struct fscaler_kpi;
struct fscaler_osd;

///////////////////////////////////////////////////////////////////////////////
// LOCAL INCLUDE HEADER
#include "fscaler_table.h"
#include "fscaler_kpi.h"
#include "fscaler_osdapi.h"

///////////////////////////////////////////////////////////////////////////////
// LINUX INCLUDE HEADER
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>

///////////////////////////////////////////////////////////////////////////////
// DEFINE AND MACRO
#define FSCALER_DEVICE_STATUS_IDLE        0x0001        ///< Scaler device is idle
#define FSCALER_DEVICE_STATUS_ONPROC      0x0002        ///< Scaler device is on process
#define FSCALER_DEVICE_STATUS_DONE        0x0003        ///< Scaler device is done
#define FSCALER_DEVICE_STATUS_ERROR       0x0004        ///< Scaler device is failed to process job
#define FSCALER_DEVICE_STATUS_ABORT       0x0005        ///< Scaler device is abort to do job (Unsupported job or cancelled job)

#define FSCALER_OSD_MAX_FONT              64
#define FSCALER_OSD_MAX_CODE              256
#define FSCALER_OSD_MAX_DISPLAY           128
#define FSCALER_OSD_MAJOR                 10
#define FSCALER_OSD_MINOR                 (30<<8)
#define FSCALER_OSD_CH2FD(minor)          ((minor)-FSCALER_OSD_MINOR)
#define FSCALER_OSD_MAX_FONT_W            12
#define FSCALER_OSD_MAX_FONT_H            18

#define FSCALER_OSD_STATUS_RESET          0x0001
#define FSCALER_OSD_STATUS_DISABLE        0x0002
#define FSCALER_OSD_STATUS_ENABLE         0x0003

///////////////////////////////////////////////////////////////////////////////
// EXTERN VARIABLE AND FUNCTION

///////////////////////////////////////////////////////////////////////////////
// TYPE DECLARTION AND DEFINITION
struct fscaler_osd_disp_ram {
    char font[FSCOSD_DISP_MAX];
    int index[FSCOSD_DISP_MAX];
};

struct fscaler_osd {
    int pluged;
    unsigned int check;
    struct fiosd_osd_font_data map;
    struct fiosd_palette palette[FSCOSD_PAL_MAX];
    struct fiosd_transparent trans[1];
    struct fscaler_osd_disp_ram disp[1];
#define DYNAMIC_OSD_RESOURCE
#ifdef DYNAMIC_OSD_RESOURCE
    int *active;
    int *dispram_update;
    struct fiosd_win *win;
    struct fiosd_string *str;
    struct fiosd_font_info *font;
#else
    int active[FSCFD_MAX];
    int dispram_update[FSCFD_MAX];
    struct fiosd_win win[FSCFD_MAX];
    struct fiosd_string str[FSCFD_MAX];
    struct fiosd_font_info font[FSCFD_MAX];
#endif
};

struct fscaler_device {
    struct fscaler_table *parent;
    struct klist_node knode_table;
    struct klist klist_job;
    void (*grab) (struct fscaler_device *);
    unsigned int base;
    int id;
    int status;
    int opened;
    int irq;
    spinlock_t lock;
    struct tasklet_struct irq_tasklet;
    unsigned int irq_mask;
    int irq_ret;
    struct fscaler_job *job;
    spinlock_t timer_lcok;
    struct timer_list timer;
    unsigned int timer_base;
    int kpi_up;
    struct fscaler_kpi kpi;
    struct fscaler_kpi show;
    struct fscaler_osd osd;
    int retry;
};

extern int fscaler_device_table_request(struct fscaler_driver *drv);
extern void fscaler_device_table_remove(struct fscaler_driver *drv);
extern const char *fscaler_device_table_name(void);
extern int fscaler_device_open(struct fscaler_device *dev);
extern void fscaler_device_close(struct fscaler_device *dev);
extern void fscaler_device_process(struct fscaler_device *dev);
extern void fscaler_device_receive_job(struct fscaler_device *dev, struct fscaler_job *job);
extern struct fscaler_job *fscaler_device_fetch_job(struct fscaler_device *dev);
extern struct fscaler_job *fscaler_device_release_job(struct fscaler_device *dev);
extern int fscaler_osd_request(struct fscaler_driver *drv);
extern void fscaler_osd_remove(struct fscaler_driver *drv);
extern int fscaler_osd_set_enable(struct fscaler_driver *drv, struct fiosd_ch *data, int enable);
extern int fscaler_osd_set_font(struct fscaler_driver *drv, struct fiosd_char *data, int set);
extern int fscaler_osd_set_palette(struct fscaler_driver *drv, struct fiosd_palette *data);
extern int fscaler_osd_set_win(struct fscaler_driver *drv, struct fiosd_win *data);
extern int fscaler_osd_set_font_size(struct fscaler_driver *drv, struct fiosd_font_info *data);
extern int fscaler_osd_set_transparent(struct fscaler_driver *drv, struct fiosd_transparent *data);
extern int fscaler_osd_set_string(struct fscaler_driver *drv, struct fiosd_string *data);

static inline struct fscaler_table *fscaler_device_to_table(struct fscaler_device *dev)
{
    return dev ? dev->parent : NULL;
}

static inline void fscaler_device_callback_register(struct fscaler_device *dev,
                                                    void (*grab) (struct fscaler_device *))
{
    if (dev) {
        dev->grab = grab;
    }
}

static inline struct fscaler_device *fscaler_device_find(struct fscaler_table *table, int index)
{
    return (struct fscaler_device *)fscaler_table_item_find(table, index);
}

static inline void fscaler_device_lock(struct fscaler_device *dev)
{
    struct fscaler_table *table = fscaler_device_to_table(dev);
    if (table && dev->opened) {
        fscaler_table_item_lock(table, dev->id);
    }
}

static inline void fscaler_device_unlock(struct fscaler_device *dev)
{
    struct fscaler_table *table = fscaler_device_to_table(dev);
    if (table && dev->opened) {
        fscaler_table_item_unlock(table, dev->id);
    }
}

static inline int fscaler_device_locked(struct fscaler_device *dev)
{
    return dev ? fscaler_table_item_locked(fscaler_device_to_table(dev), dev->id) : 1;
}

static inline struct fscaler_device *fscaler_device_sched(struct fscaler_table *table)
{
    return (struct fscaler_device *)fscaler_table_item_lookup(table);
}

#endif /* _FSCALER_DEVICE_H_ */

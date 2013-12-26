/**
 * @file fscaler_module.h
 * @brief scaler module header
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#ifndef _FSCALER_MODULE_H_
#define _FSCALER_MODULE_H_

struct device;
struct fscaler_module;
struct fscaler_driver;

#include "fscaler_list.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>

#define __m2cstr(m)         #m
#define __m2name(m)         __m2cstr(m)
#define MPFX_NAME           __m2name(MPFX)
#define MPFX_VERS           __m2name(MVER)

extern int dbg_type;
extern int supp_proc;
extern int supp_kpi;
extern int supp_vg;
extern int supp_io;
extern int supp_osd;
extern int pmu_clock;
extern unsigned int timeout;
extern unsigned int max_retry;
extern unsigned int max_width;
extern unsigned int max_height;
extern unsigned int max_entity;
extern unsigned int max_device;

struct fscaler_module {
    struct device *dev;
    struct klist klist_drv;
};

extern struct fscaler_driver *fscaler_module_driver_find(struct fscaler_module *mod,
                                                         const char *name);

#endif /* _FSCALER_MODULE_H_ */

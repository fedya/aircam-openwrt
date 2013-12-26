/**
 * @file fscaler_proc.h
 * @brief Scaler /proc system header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#ifndef _FSCALER_PROC_H_
#define _FSCALER_PROC_H_

struct fscaler_driver;

#include "fscaler_interface.h"

extern int fscaler_proc_interface_request(struct fscaler_driver *drv);
extern void fscaler_proc_interface_remove(struct fscaler_driver *drv);
extern const char *fscaler_proc_interface_name(void);

#endif /* _FSCALER_PROC_H_ */

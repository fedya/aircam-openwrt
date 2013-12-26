/**
 * @file fscaler_osd.h
 * @brief Scaler driver OSD control interface header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 25
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#ifndef _FSCALER_OSD_H_
#define _FSCALER_OSD_H_

struct fscaler_driver;

#include "fscaler_interface.h"

extern int fscaler_osd_interface_request(struct fscaler_driver *drv);
extern void fscaler_osd_interface_remove(struct fscaler_driver *drv);
extern const char *fscaler_osd_interface_name(void);

#endif /* _FSCALER_IO_H_ */

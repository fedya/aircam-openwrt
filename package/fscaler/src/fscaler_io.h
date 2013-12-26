/**
 * @file fscaler_io.h
 * @brief Scaler driver ioctl interface header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#ifndef _FSCALER_IO_H_
#define _FSCALER_IO_H_

#include "fscaler_driver.h"

#define FSCALER_IO_OK               (0) ///< Success
#define FSCALER_IO_ERR_OPEN         (-256)      ///< Failed to open scaler entity
#define FSCALER_IO_ERR_INSTALL      (-257)      ///< Failed to install scaler driver ioctl interface
#define FSCALER_IO_ERR_UNINSTALL     (-258)     ///< Failed to uninstall scaler driver ioctl interface
#define FSCALER_IO_SUCCEED(code)    (0 <= (code))
#define FSCALER_IO_FAILED(code)     (0 > (code))

extern int fscaler_io_uninstall_interface(void);
extern int fscaler_io_install_interface(struct device *p_dev, struct platform_device *p_plat_dev);

#endif /* _FSCALER_IO_H_ */

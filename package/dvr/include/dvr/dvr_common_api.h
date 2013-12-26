#ifndef __DVR_COMMON_API_H__
#define __DVR_COMMON_API_H__

#include <linux/ioctl.h>
#include "dvr_type_define.h"

#define DVR_IOC_MAGIC 'D'

/**
 * \b ioctl(dvr_fd, DVR_COMMON_APPLY, &tag)
 *
 * \arg get function tag from user space, and set to videograph level
 * \arg parameter :
 * \n \b \e pointer \b \e tag : argument from user space ioctl parameter, it means structure FuncTag  
 */
#define DVR_COMMON_APPLY                _IOW(DVR_IOC_MAGIC, 1, FuncTag) 
#define DVR_COMMON_DEBUG                _IOW(DVR_IOC_MAGIC, 2, FuncTag) 
#define DVR_COMMON_DEBUG_WITH_PANIC     _IOW(DVR_IOC_MAGIC, 3, FuncTag) 

#endif /* __DVR_COMMON_API_H__ */



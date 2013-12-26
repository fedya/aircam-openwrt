/**
 * @file videograph_device.h
 * @brief Videograph device routines header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */

#ifndef _VIDEOGRAPH_DEVICE_H_
#define _VIDEOGRAPH_DEVICE_H_

#include "videograph_api.h"

#define VIDEOGRAPH_DYNAMIC_MAJOR            10
#define VIDEOGRAPH_DYNAMIC_MINOR            (30<<8)
#define VIDEOGRAPH_MINOR_TO_FD(minor)       ((minor)-VIDEOGRAPH_DYNAMIC_MINOR)
#define VIDEOGRAPH_ENTITY_TO_FD(entity)     VIDEOGRAPH_MINOR_TO_FD((entity)->minor)

typedef struct videograph_device {
    char *name;
    int max;
    struct v_operations *fops;
    int first;
} videograph_device_t;

static inline void videograph_device_init(struct videograph_device *p_vg_dev, int max)
{
    if (p_vg_dev) {
        p_vg_dev->max = max;
    }
}

extern int videograph_device_register(struct videograph_device *p_dev);
extern int videograph_device_deregister(struct videograph_device *p_dev);

#endif /* _VIDEOGRAPH_DEVICE_H_ */

/**
 * @file fscaler_debug.h
 * @brief Scaler debug routines' header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.1.04
 * @date 2010 Oct 5
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */
#ifndef _FSCALER_DEBUG_H_
#define _FSCALER_DEBUG_H_

#include "fscaler_module.h"

/* Unused argument macro, mark the argument to prevent using it */
#define UNUSED_ARG(arg)     (void)(arg)

/* Simple breakpoint macro for ICE debugging */
#define breakpoint()	\
({ \
    volatile int __breakpoint = 1, __a = 0; \
    do { \
        ++__a; \
    } while (__breakpoint); \
    __breakpoint; \
})

#define DBG_INFO(format, arg...)  printk(KERN_INFO "" format , ## arg)
#define DBG_WARN(format, arg...)  printk(KERN_WARNING "%s:" format , __func__ , ## arg)
#define DBG_ERROR(format, arg...)  printk(KERN_ERR "%s:" format , __func__ , ## arg)

/* To fix the bug the scaler ip need to be forced to reset */
#define BUG_FORCE_RETRIGER
#ifdef BUG_FORCE_RETRIGER
#define BUG_FORCE_RETRIGER_ON()     1
#define BUG_FORCE_RETRIGER_OFF()    0
#else
#define BUG_FORCE_RETRIGER_ON()     0
#define BUG_FORCE_RETRIGER_OFF()    1
#endif /* BUG_FORCE_RETRIGER */

/* To make font size of OSD fixed */
#define BUG_FIXED_OSD_FONT_SIZE
#ifdef BUG_FIXED_OSD_FONT_SIZE
#define BUG_FIXED_OSD_FONT_SIZE_ON()        1
#define BUG_FIXED_OSD_FONT_SIZE_OFF()       0
#else
#define BUG_FIXED_OSD_FONT_SIZE_ON()        0
#define BUG_FIXED_OSD_FONT_SIZE_OFF()       1
#endif /* BUG_FIXED_OSD_FONT_SIZE */

/* To ignore the incorrect DMA overflow status */
#define BUG_FAKE_DMAOVF
#ifdef BUG_FAKE_DMAOVF
#define BUG_FAKE_DMAOVF_ON()        1
#define BUG_FAKE_DMAOVF_OFF()       0
#else
#define BUG_FAKE_DMAOVF_ON()        0
#define BUG_FAKE_DMAOVF_OFF()       1
#endif /* BUG_FORCE_RETRIGER */

/* If _DEVELOP_DEBUG_ is defined, the verbos trace information will output to the terminal */
//#define _DEVELOP_DEBUG_
#ifdef _DEVELOP_DEBUG_
#define M_SPF(args...)       do { DBG_INFO(args); } while (0)
#define M_DEVELOP(args...)   do { DBG_INFO(args); } while (0)
#else
#define M_SPF(args...)       (void)(0)
#define M_DEVELOP(args...)   (void)(0)
#endif /* _DEVELOP_DEBUG_ */
#define M_DEVELOPP(args...)  (void)(0)

#if 0 /*_DEVELOP_DEBUG_*/
#define STATIC_INLINE static
#else
#define STATIC_INLINE static inline
#endif

/* Module debug macro, enable by dbg_type */
#include "videograph_api.h"
#define M_DEBUG(args...) do { \
    if (dbg_type) { \
        if (supp_vg) { \
            printm(args); \
        } else { \
            DBG_INFO(args); \
        } \
    } \
} while (0)

#define M_WARN(args...) do { \
    if (dbg_type) { \
        if (supp_vg) { \
            printm(args); \
        } else { \
            DBG_WARN(args); \
        } \
    } \
} while (0)

#define M_ERROR(args...) do { \
    if (dbg_type) { \
        if (supp_vg) { \
            printm(args); \
        } else { \
            DBG_ERROR(args); \
        } \
    } \
} while (0)

#define M_PANIC(args...) do { \
    if (dbg_type) { \
        if (supp_vg) { \
            printm_with_panic(args); \
        } else { \
            panic(args); \
        } \
    } \
} while (0)

#endif /* _FSCALER_DEBUG_H_ */

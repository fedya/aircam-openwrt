/** @file drv_api.h
 *
 *  @brief This file provides the capture driver related function calls or structures
 *
 *  @author Francis Huang
 *  @version 0.1
 *  @date 2010/09/28
 *
 */

#ifndef __DRV_API_H__
#define __DRV_API_H__
#include <linux/types.h>

#define VIDEO_MAJOR                 81
#define MINOR_ID(ch,ph)             ((ch)<<4|(ph&0x03))

#define MAG_NUM                     0x168
#define FCAP_DRV_TAG                ((MAG_NUM<<16)|0x1234)
#define FCAP_CHANNEL_NUM(c, p, s)   ((c)<<2|((p)&0x01)|((s)&0x01)<<1)
#define FRAMERATIO(n,d)             ((n)<<16|(d))

#define FCAP_BUF_INFO(i,p)          ((i)<<8|(p))
#define FCAP_BUF_MODE(info)         (((info)>>8)&0xff)  //0:frame mode, 1:field mode
#define BI_FRAME_MODE               0
#define BI_FIELD_MODE               1
#define FCAP_BUF_TYPE(info)         ((info)&0xff)       //0:top field, 1:bottom field
#define BI_TOP_FIELD                0
#define BI_BOTTOM_FIELD             1

/**
 * @brief This structure defines for capture driver buffer information
 */ 
typedef struct drv_buf_header{
    u32 Tag;            ///< magic number => FCAP_DRV_TAG
    u32 size;           ///< header size
    u32 VBI[2];         ///< VBI enable
    u32 Y[2];           ///< buffer offset of Y
    u32 U[2];           ///< buffer offset of U
    u32 V[2];           ///< buffer offset of V
    u16 width;          ///< output target width
    u16 height;         ///< output target height
    u16 bg_x;           ///< left position of background buffer
    u16 bg_y;           ///< top position of background buffer
    u16 bg_w;           ///< width of background buffer
    u16 bg_h;           ///< height of background buffer
    int buf_info;       ///< buffer information => FCAP_BUF_INFO(i,p) for record frame/field mode, top/bottom field
    u8  ch_num;         ///< channel number => FCAP_CHANNEL_NUM(c, p, s)
} drv_buf_header_t;
#endif /* __DRV_API_H__ */

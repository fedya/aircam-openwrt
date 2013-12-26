/** @file cap_v4l_api.h
 *
 *  @brief This file provides the capture V4L related function calls or IOCTLs.
 *
 *  @author Francis Huang
 *  @version 0.1
 *  @date 2010/09/28
 *
 */

#ifndef _FCAP_API_H_
#define _FCAP_API_H_
#include <linux/ioctl.h>
#include <linux/videodev.h>

#ifndef __KERNEL__
#include <stdint.h>
#endif

/**
 * @brief This structure defines for capture external background buffer setting
 */ 
struct fcap_ext_buf_info {
    uint32_t bg_width;                      ///< width of background buffer
    uint32_t bg_height;                     ///< height of background buffer
    uint32_t phy_addr[VIDEO_MAX_FRAME];     ///< physical address of background buffer
    uint32_t buf_size;                      ///< size of background buffer
    uint8_t nbuf;                           ///< number of buffer
    uint8_t npending;                       ///< number of pending buffer
};  

/**
 * @brief This structure defines for enhance capture external background buffer setting
 */ 
struct fcap_ext2_buf_info {
    uint32_t bg_width;                      ///< width of background buffer
    uint32_t bg_height;                     ///< height of background buffer
    uint32_t phy_addr[VIDEO_MAX_FRAME];     ///< physical address of background buffer
    uint32_t buf_size;                      ///< size of background buffer
    uint8_t nbuf;                           ///< number of buffer
    uint8_t npending;                       ///< number of pending buffer
    uint32_t video_x;                       ///< left position of background buffer
    uint32_t video_y;                       ///< top position of background buffer
};

/**
 * @brief This structure defines for setting background buffer left/top postion
 */ 
struct fcap_ext2_buf_offset {
    uint32_t x;
    uint32_t y;
};

#define FCAP_DEINTERLACE_OFF  0
#define FCAP_DEINTERLACE_ON   1
#define FCAP_DI_ON_DMA_2D     2
#define FCAP_DI_ON_3D         3
#define FCAP_DI_MAX           3

/**
 * @brief This structure defines for Y, U, V offset
 */ 
struct fcap_buf_offset{
    uint32_t y;         ///< buffer offset of Y
    uint32_t u;         ///< buffer offset of U
    uint32_t v;         ///< buffer offset of V
};

/**
 * @brief This structure defines for getting Y, U, V offset of buffer
 */ 
struct fcap_offset_info {
    struct fcap_buf_offset offset[2];       ///< Y, U, V offset
};

/**
 * @brief This structure defines for capture crop setting.
 */ 
struct fcap_crop_info {
    uint16_t x;         ///< left position of cropping
    uint16_t y;         ///< top position of cropping
    uint16_t w;         ///< width of cropping
    uint16_t h;         ///< height of cropping
};

/**
 * @brief This structure defines for capture frame rate control
 */ 
struct fcap_frame_rate {
    int enable;         ///< frame rate control enable
    int numerator;      ///< frame rate numerator
    int denominator;    ///< frame rate denominator
};

/**
 * @brief This structure defines for capture flip control
 */ 
struct fcap_flip_param{
    int h_flip;         ///< horizontal flip, 0: off 1:on
    int v_flip;         ///< vertical flip, 0: off 1: on
};

#define FCAP_MACH_PAUSE 0
#define FCAP_MACH_RESUME 1

/* Use 'f' as magic number */
#define FCAP_IOC_MAGIC  'f'

/*
 * IOCTL
 */
#define FCAP_IOCSEXTBUF         _IOW(FCAP_IOC_MAGIC, 1, struct fcap_ext_buf_info)
#define FCAP_IOSDEINTERLACE     _IOW(FCAP_IOC_MAGIC, 2, int)
#define FCAP_IOGDEINTERLACE     _IOR(FCAP_IOC_MAGIC, 3, int)
#define FCAP_IOGBUFOFFSET       _IOR(FCAP_IOC_MAGIC, 4, struct fcap_offset_info)
#define FCAP_IOSCROP            _IOW(FCAP_IOC_MAGIC, 5, struct fcap_crop_info)
//#define FCAP_VBI_IOCGDATA     _IOR(FCAP_IOC_MAGIC, 7, struct fcap_vbi_ext_info)
//#define FCAP_IOGGRABBUF       _IOR(FCAP_IOC_MAGIC, 8, int)
//#define FCAP_IOSGRABBUF       _IOWR(FCAP_IOC_MAGIC, 9, struct fcap_grabbuf_mode)
//#define FCAP_IOGRABBUFINFO    _IOR(FCAP_IOC_MAGIC, 10, struct fcap_grabbuf_info)
#define FCAP_IOSSCALEAUTORATIO  _IOR(FCAP_IOC_MAGIC, 11, int)
#define FCAP_IOCSEXTBUF2        _IOW(FCAP_IOC_MAGIC, 12, struct fcap_ext2_buf_info)
//#define FCAP_IOCSRESETBUF     _IOW(FCAP_IOC_MAGIC, 13, int)
#define FCAP_IOSMODE            _IOW(FCAP_IOC_MAGIC, 14, int)
#define FCAP_IOSFRAMERATE       _IOW(FCAP_IOC_MAGIC, 15, struct fcap_frame_rate)
#define FCAP_IOCSEXTBUF2OFFSET  _IOW(FCAP_IOC_MAGIC, 16, struct fcap_ext2_buf_offset)
#define FCAP_IOCMACHINE         _IOW(FCAP_IOC_MAGIC, 17, int)
#define FCAP_IOCSFLIP           _IOW(FCAP_IOC_MAGIC, 18, struct fcap_flip_param)
#define FCAP_IOCSSHARPNESS      _IOW(FCAP_IOC_MAGIC, 19, int)
#endif  /* _FCAP_API_H_ */

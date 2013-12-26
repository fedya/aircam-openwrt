/**
 * @file ioctl_isp210.h
 * ISP210 IOCTLs
 *
 * Copyright (C) 2010 GM Corp. (http://www.grain-media.com)
 *
 * $Revision: 1.2 $
 * $Date: 2012/03/13 13:39:42 $
 *
 * ChangeLog:
 *  $Log: ioctl_isp210.h,v $
 *  Revision 1.2  2012/03/13 13:39:42  ricktsao
 *  1.add following IOCTL
 *       ISP_IOC_SET_GAMMA_LUT/ISP_IOC_GET_GAMMA_LUT
 *
 *  Revision 1.1  2012/02/15 06:36:57  ricktsao
 *  no message
 *
 *  Revision 1.20  2011/12/20 05:31:31  ricktsao
 *  1. add a member "shift" into shdc_regs struct
 *  2. modify MD result definition
 *  3. add following IOCTL
 *      ISP_IOC_GET_KNEE_EN / ISP_IOC_SET_KNEE_EN
 *      ISP_IOC_GET_KNEE_POINT_X / ISP_IOC_SET_KNEE_POINT_X
 *      ISP_IOC_GET_KNEE_POINT_Y / ISP_IOC_SET_KNEE_POINT_Y
 *
 *  Revision 1.19  2011/11/14 09:47:26  kenny_ch
 *  add manual level setting when auto_adj_en be disabled
 *
 *  Revision 1.18  2011/11/08 12:13:41  kenny_ch
 *  add dynamic loading cv matrix in different color temperature and let it compatible previous version
 *
 *  Revision 1.17  2011/10/26 11:55:35  ricktsao
 *  add ISP_IOC_GET_RGB_RESULT
 *
 *  Revision 1.16  2011/08/24 10:11:16  ricktsao
 *  add ISP_IOC_SET_DPC_LEV/ISP_IOC_GET_DPC_LEV
 *         ISP_IOC_SET_CTK_LEV/ISP_IOC_GET_CTK_LEV
 *         ISP_IOC_SET_AUTO_ADJ_EN/ISP_IOC_GET_AUTO_ADJ_EN
 *
 *  Revision 1.15  2011/08/03 12:27:14  ricktsao
 *  add ISP_IOC_GET_FRAME_RATE
 *
 *  Revision 1.14  2011/07/06 06:09:30  hwchih
 *  1.Define IOCTL command for set sensor size.
 *
 *  Revision 1.13  2011/06/06 08:54:07  ricktsao
 *  1.modify af_result struct
 *
 *  Revision 1.12  2011/05/25 04:55:43  ricktsao
 *  Add following IOCTL
 *      ISP_IOC_SET_R_GAIN / ISP_IOC_GET_R_GAIN
 *      ISP_IOC_SET_G_GAIN / ISP_IOC_GET_G_GAIN
 *      ISP_IOC_SET_B_GAIN / ISP_IOC_GET_B_GAIN
 *
 *  Revision 1.11  2011/05/23 08:37:32  ricktsao
 *  Add IOCTL to get AF statistic
 *
 *  Revision 1.10  2011/04/25 06:03:11  ricktsao
 *  1. remove following IOCTL
 *          ISP_IOC_GET_SHUTTER_WIDTH
 *  2. add following IOCTL
 *          ISP_IOC_GET_Y_RESULT
 *          ISP_IOC_READ_SENSOR_REG /  ISP_IOC_WRITE_SENSOR_REG
 *          ISP_IOC_SET_SENSOR_EXPOSURE / ISP_IOC_GET_SENSOR_EXPOSURE
 *          ISP_IOC_SET_SENSOR_FPS / ISP_IOC_GET_SENSOR_FPS
 *          ISP_IOC_SET_SENSOR_AE_EN / ISP_IOC_GET_SENSOR_AE_EN
 *          ISP_IOC_SET_SENSOR_AWB_EN / ISP_IOC_GET_SENSOR_AWB_EN
 *
 *  Revision 1.9  2011/03/10 02:29:49  ricktsao
 *  1. remove following IOCTL
 *       ISP_IOC_SET_IMGSIZE
 *  2. add following IOCTL
 *       ISP_IOC_SET_SENSOR_FPS / ISP_IOC_GET_SENSOR_FPS
 *       ISP_IOC_GET_SENSOR_SIZE
 *       ISP_IOC_SET_ACT_WIN / ISP_IOC_GET_ACT_WIN
 *       ISP_IOC_SET_TARGET_SIZE / ISP_IOC_GET_TARGET_SIZE
 *
 *  Revision 1.8  2011/02/25 02:03:08  ricktsao
 *  Add following IOCTL
 *      ISP_IOC_GET_SHUTTER_WIDTH
 *      ISP_IOC_GET_SENSOR_GAIN
 *
 *  Revision 1.7  2011/02/21 02:19:29  ricktsao
 *  1. Add version define
 *  2. remove following IOCTL
 *      ISP_IOC_SET_MONOLEVEL/ISP_IOC_GET_MONOLEVEL
 *      ISP_IOC_SET_AUTOMONO_EN/ISP_IOC_GET_AUTOMONO_EN
 *  3. add following IOCTL
 *      ISP_IOC_SET_AUTO_CS_TH/ISP_IOC_GET_AUTO_CS_TH
 *      ISP_IOC_SET_AUTO_CS_EN/ISP_IOC_GET_AUTO_CS_EN
 *      ISP_IOC_SET_AUTO_CS_GAIN/ISP_IOC_GET_AUTO_CS_GAIN
 *
 *  Revision 1.6  2011/02/15 08:32:13  ricktsao
 *  Add Mirror & Flip IOCTL
 *
 *  Revision 1.5  2011/02/09 02:07:13  ricktsao
 *  1. modify struct shdc_regs
 *  2. add motion detection releated IOCTL define
 *
 *  Revision 1.4  2011/01/26 07:04:48  ricktsao
 *  update IOCTL define
 *
 *  Revision 1.3  2011/01/20 10:36:07  ricktsao
 *  Add IOCTL key and paramrter struct definition
 *
 *  Revision 1.2  2011/01/19 08:35:03  ricktsao
 *  1. Add IOCTL command define
 *
 *  Revision 1.1  2011/01/12 02:48:09  ricktsao
 *  isp210_v2 driver initial version
 *
 */

#ifndef __IOCTL_ISP210_H__
#define __IOCTL_ISP210_H__

#include <linux/ioctl.h>

#ifndef __KERNEL__
#ifndef u32
typedef unsigned int u32;
#endif

#ifndef s32
typedef int s32;
#endif

#ifndef u16
typedef unsigned short u16;
#endif

#ifndef s16
typedef short s16;
#endif

#ifndef u8
typedef unsigned char u8;
#endif

#ifndef s8
typedef char s8;
#endif

#endif

//============================================================================
// IOCTL version
//============================================================================
#define ISP_IOCTL_VER   ((2<<16)|(0<<8)|(0<<4)|0)  /* 2.0.00 */

//============================================================================
// struct & flag definition
//============================================================================
typedef struct win_rect {
    int x;
    int y;
    int width;
    int height;
} win_rect_t;

typedef struct win_pos {
    int x;
    int y;
} win_pos_t;

typedef struct win_size {
    int width;
    int height;
} win_size_t;

// flags of ISP_IOC_ENABLE_FUNC/ISP_IOC_DISABLE_FUNC
#define ISP_SHDC    (1UL <<  0)
#define ISP_RMC     (1UL <<  1)
#define ISP_DPC     (1UL <<  3)
#define ISP_CTK     (1UL <<  4)
#define ISP_GM      (1UL <<  5)
#define ISP_YCC     (1UL <<  6)
#define ISP_CS      (1UL <<  7)
#define ISP_IE      (1UL <<  8)
#define ISP_FCS     (1UL <<  9)
#define ISP_CE      (1UL << 10)
#define ISP_SP      (1UL << 11)
#define ISP_HR      (1UL << 12)
#define ISP_BCS     (1UL << 13)
#define ISP_RDN     (1UL << 20)
#define ISP_CDN     (1UL << 21)
#define ISP_SDN     (1UL << 22)

typedef struct ob_offset {
    u32 ob[4];
} ob_offset_t;

typedef struct shdc_regs {
    u32 center_xy;
    u32 rb_kxu;
    u32 grb_kxu;
    u32 rb_kxd;
    u32 grb_kxd;
    u32 rb_kxr;
    u32 grb_kxr;
    u32 rb_kxl;
    u32 grb_kxl;
    u32 shift;
    u32 offset;
} shdc_regs_t;

typedef struct cc_matrix {
    s32 coe[9];
} cc_matrix_t;

typedef struct cc_offset {
    s32 coe[3];
} cc_offset_t;

typedef struct gamma_lut {
    u32 coe[1408];
} gamma_lut_t;

typedef struct cv_matrix {
    s32 coe[9];
} cv_matrix_t;

typedef struct auto_adj {
    int en;
    int level;
} auto_adj_t;

#ifndef MD_WIN_MAX_XNUM
#define MD_WIN_MAX_XNUM     128
#endif

#ifndef MD_WIN_MAX_YNUM
#define MD_WIN_MAX_YNUM     128
#endif

#define MD_RESULT_BUFSIZE   ((MD_WIN_MAX_XNUM*MD_WIN_MAX_YNUM+7)/8)

typedef struct md_result {
    u32 fmno;
    u32 x_num;
    u32 y_num;
    u8  data[MD_RESULT_BUFSIZE];
} md_result_t;

#ifndef AE_WIN_MAX_XNUM
#define AE_WIN_MAX_XNUM     32
#endif

#ifndef AE_WIN_MAX_YNUM
#define AE_WIN_MAX_YNUM     32
#endif

typedef struct y_result {
    u32 fmno;
    u32 x_num;
    u32 y_num;
    u8  data[AE_WIN_MAX_XNUM*AE_WIN_MAX_YNUM];
} y_result_t;

#ifndef AWB_WIN_MAX_XNUM
#define AWB_WIN_MAX_XNUM    128
#endif

#ifndef AWB_WIN_MAX_YNUM
#define AWB_WIN_MAX_YNUM    64
#endif

typedef struct rgb_result {
    u32 fmno;
    u32 x_num;
    u32 y_num;
    u32 data[AWB_WIN_MAX_XNUM*AWB_WIN_MAX_YNUM];    // RGB888 format
} rgb_result_t;

#ifndef AF_WIN_MAX_XNUM
#define AF_WIN_MAX_XNUM     5
#endif

#ifndef AF_WIN_MAX_YNUM
#define AF_WIN_MAX_YNUM     5
#endif

typedef struct af_result {
    u32 fmno;
    u32 x_num;
    u32 y_num;
    u32 fv[AF_WIN_MAX_XNUM*AF_WIN_MAX_YNUM];
} af_result_t;

// parameter of ISP_IOC_SET_GAMMA
#define ISP_GAMMA_16    0
#define ISP_GAMMA_18    1
#define ISP_GAMMA_20    2
#define ISP_GAMMA_22    3
#define ISP_GAMMA_USER  4

// IOCTL parameter of extend algorithm module
typedef struct alg_arg {
    u32      cmd;
    int      direction;
    void    *data;
    ssize_t  size;
} alg_arg_t;

#define DATA_APP_TO_DRIVER  0
#define DATA_DRIVER_TO_APP  1

typedef struct sen_reg {
    u32 addr;
    u32 value;
} sen_reg_t;

//============================================================================
// IOCTL command definition
//============================================================================
#define ISP_IOC_ISP   's'
#define ISP_IOC_START               _IO(ISP_IOC_ISP, 1)
#define ISP_IOC_STOP                _IO(ISP_IOC_ISP, 2)
#define ISP_IOC_ENABLE_FUNC         _IOW(ISP_IOC_ISP, 4, u32)
#define ISP_IOC_DISABLE_FUNC        _IOW(ISP_IOC_ISP, 5, u32)
#define ISP_IOC_SET_CC_MATRIX       _IOW(ISP_IOC_ISP, 6, struct cc_matrix)
#define ISP_IOC_GET_CC_MATRIX       _IOR(ISP_IOC_ISP, 6, struct cc_matrix)
#define ISP_IOC_SET_CC_OFFSET       _IOW(ISP_IOC_ISP, 7, struct cc_offset)
#define ISP_IOC_GET_CC_OFFSET       _IOR(ISP_IOC_ISP, 7, struct cc_offset)
#define ISP_IOC_SET_CE_INTENSITY    _IOW(ISP_IOC_ISP, 8, int)
#define ISP_IOC_GET_CE_INTENSITY    _IOR(ISP_IOC_ISP, 8, int)
#define ISP_IOC_SET_OB_OFFSET       _IOW(ISP_IOC_ISP, 9, struct ob_offset)
#define ISP_IOC_GET_OB_OFFSET       _IOR(ISP_IOC_ISP, 9, struct ob_offset)
#define ISP_IOC_SET_SHDC_REGS       _IOW(ISP_IOC_ISP, 10, struct shdc_regs)
#define ISP_IOC_GET_SHDC_REGS       _IOR(ISP_IOC_ISP, 10, struct shdc_regs)
#define ISP_IOC_GET_MD_RESULT       _IOR(ISP_IOC_ISP, 11, struct md_result *)
#define ISP_IOC_SET_MD_SHAD_EN      _IOW(ISP_IOC_ISP, 12, int)
#define ISP_IOC_GET_MD_SHAD_EN      _IOR(ISP_IOC_ISP, 12, int)
#define ISP_IOC_SET_MD_SHAD_TH      _IOW(ISP_IOC_ISP, 13, int)
#define ISP_IOC_GET_MD_SHAD_TH      _IOR(ISP_IOC_ISP, 13, int)
#define ISP_IOC_SET_MD_A_ALPHA      _IOW(ISP_IOC_ISP, 14, int)
#define ISP_IOC_GET_MD_A_ALPHA      _IOR(ISP_IOC_ISP, 14, int)
#define ISP_IOC_SET_MD_SMB_TH       _IOW(ISP_IOC_ISP, 15, int)
#define ISP_IOC_GET_MD_SMB_TH       _IOR(ISP_IOC_ISP, 15, int)
#define ISP_IOC_SET_MD_TBG_TH       _IOW(ISP_IOC_ISP, 16, int)
#define ISP_IOC_GET_MD_TBG_TH       _IOR(ISP_IOC_ISP, 16, int)
#define ISP_IOC_GET_Y_RESULT        _IOR(ISP_IOC_ISP, 17, struct y_result *)
#define ISP_IOC_GET_RGB_RESULT      _IOR(ISP_IOC_ISP, 18, struct rgb_result *)
#define ISP_IOC_GET_AF_RESULT       _IOR(ISP_IOC_ISP, 19, struct af_result *)
#define ISP_IOC_GET_FRAME_RATE      _IOR(ISP_IOC_ISP, 20, int)
#define ISP_IOC_SET_KNEE_EN         _IOW(ISP_IOC_ISP, 21, int)
#define ISP_IOC_GET_KNEE_EN         _IOR(ISP_IOC_ISP, 21, int)
#define ISP_IOC_SET_KNEE_POINT_X    _IOW(ISP_IOC_ISP, 22, int)
#define ISP_IOC_GET_KNEE_POINT_X    _IOR(ISP_IOC_ISP, 22, int)
#define ISP_IOC_SET_KNEE_POINT_Y    _IOW(ISP_IOC_ISP, 23, int)
#define ISP_IOC_GET_KNEE_POINT_Y    _IOR(ISP_IOC_ISP, 23, int)
#define ISP_IOC_GET_SENSOR_SIZE     _IOR(ISP_IOC_ISP, 24, struct win_size)
#define ISP_IOC_SET_SENSOR_SIZE     _IOW(ISP_IOC_ISP, 24, struct win_size)
#define ISP_IOC_SET_ACT_WIN         _IOW(ISP_IOC_ISP, 25, struct win_rect)
#define ISP_IOC_GET_ACT_WIN         _IOR(ISP_IOC_ISP, 25, struct win_rect)
#define ISP_IOC_SET_TARGET_SIZE     _IOW(ISP_IOC_ISP, 26, struct win_size)
#define ISP_IOC_GET_TARGET_SIZE     _IOR(ISP_IOC_ISP, 26, struct win_size)
#define ISP_IOC_SET_CV_MATRIX       _IOW(ISP_IOC_ISP, 27, struct cv_matrix)
#define ISP_IOC_GET_CV_MATRIX       _IOR(ISP_IOC_ISP, 27, struct cv_matrix)
#define ISP_IOC_SET_GAMMA_LUT       _IOW(ISP_IOC_ISP, 28, struct gamma_lut *)
#define ISP_IOC_GET_GAMMA_LUT       _IOR(ISP_IOC_ISP, 28, struct gamma_lut *)

#define ISP_IOC_ENABLE_AE           _IO(ISP_IOC_ISP, 32)
#define ISP_IOC_DISABLE_AE          _IO(ISP_IOC_ISP, 33)
#define ISP_IOC_ENABLE_AWB          _IO(ISP_IOC_ISP, 34)
#define ISP_IOC_DISABLE_AWB         _IO(ISP_IOC_ISP, 35)
#define ISP_IOC_ENABLE_AF           _IO(ISP_IOC_ISP, 36)
#define ISP_IOC_DISABLE_AF          _IO(ISP_IOC_ISP, 37)
#define ISP_IOC_ENABLE_MD           _IO(ISP_IOC_ISP, 38)
#define ISP_IOC_DISABLE_MD          _IO(ISP_IOC_ISP, 39)

#define ISP_IOC_SET_R_GAIN          _IOW(ISP_IOC_ISP, 40, u32)
#define ISP_IOC_GET_R_GAIN          _IOR(ISP_IOC_ISP, 40, u32)
#define ISP_IOC_SET_G_GAIN          _IOW(ISP_IOC_ISP, 41, u32)
#define ISP_IOC_GET_G_GAIN          _IOR(ISP_IOC_ISP, 41, u32)
#define ISP_IOC_SET_B_GAIN          _IOW(ISP_IOC_ISP, 42, u32)
#define ISP_IOC_GET_B_GAIN          _IOR(ISP_IOC_ISP, 42, u32)

// image enhancement
#define ISP_IOC_SET_BRIGHTNESS      _IOW(ISP_IOC_ISP, 64, int)
#define ISP_IOC_GET_BRIGHTNESS      _IOR(ISP_IOC_ISP, 64, int)
#define ISP_IOC_SET_CONTRAST        _IOW(ISP_IOC_ISP, 65, int)
#define ISP_IOC_GET_CONTRAST        _IOR(ISP_IOC_ISP, 65, int)
#define ISP_IOC_SET_HUE             _IOW(ISP_IOC_ISP, 66, int)
#define ISP_IOC_GET_HUE             _IOR(ISP_IOC_ISP, 66, int)
#define ISP_IOC_SET_SATURATION      _IOW(ISP_IOC_ISP, 67, int)
#define ISP_IOC_GET_SATURATION      _IOR(ISP_IOC_ISP, 67, int)
#define ISP_IOC_SET_SHARPNESS       _IOW(ISP_IOC_ISP, 68, int)
#define ISP_IOC_GET_SHARPNESS       _IOR(ISP_IOC_ISP, 68, int)
#define ISP_IOC_SET_DENOISE         _IOW(ISP_IOC_ISP, 69, int)
#define ISP_IOC_GET_DENOISE         _IOR(ISP_IOC_ISP, 69, int)
#define ISP_IOC_SET_AUTO_CS_TH      _IOW(ISP_IOC_ISP, 70, int)
#define ISP_IOC_GET_AUTO_CS_TH      _IOR(ISP_IOC_ISP, 70, int)
#define ISP_IOC_SET_GAMMA           _IOW(ISP_IOC_ISP, 71, int)
#define ISP_IOC_GET_GAMMA           _IOR(ISP_IOC_ISP, 71, int)
#define ISP_IOC_SET_AUTO_CS_EN      _IOW(ISP_IOC_ISP, 72, int)
#define ISP_IOC_GET_AUTO_CS_EN      _IOR(ISP_IOC_ISP, 72, int)
#define ISP_IOC_SET_AUTO_CS_GAIN    _IOW(ISP_IOC_ISP, 73, int)
#define ISP_IOC_GET_AUTO_CS_GAIN    _IOR(ISP_IOC_ISP, 73, int)
#define ISP_IOC_SET_DPC_LEV         _IOW(ISP_IOC_ISP, 74, int)
#define ISP_IOC_GET_DPC_LEV         _IOR(ISP_IOC_ISP, 74, int)
#define ISP_IOC_SET_CTK_LEV         _IOW(ISP_IOC_ISP, 75, int)
#define ISP_IOC_GET_CTK_LEV         _IOR(ISP_IOC_ISP, 75, int)
#define ISP_IOC_SET_CTK_LEV         _IOW(ISP_IOC_ISP, 75, int)
#define ISP_IOC_GET_AUTO_ADJ_EN     _IOR(ISP_IOC_ISP, 76, int)
#define ISP_IOC_SET_AUTO_ADJ_EN     _IOW(ISP_IOC_ISP, 76, struct auto_adj)

// algorithm module IOCTL
#define ISP_IOC_AE_IOCTL            _IOWR(ISP_IOC_ISP, 128, struct alg_arg)
#define ISP_IOC_AWB_IOCTL           _IOWR(ISP_IOC_ISP, 129, struct alg_arg)
#define ISP_IOC_AF_IOCTL            _IOWR(ISP_IOC_ISP, 130, struct alg_arg)

//----------------------------------------------------------------------------
#define ISP_IOC_SENSOR  'e'
#define ISP_IOC_READ_SENSOR_REG     _IOWR(ISP_IOC_SENSOR, 1, struct sen_reg)
#define ISP_IOC_WRITE_SENSOR_REG    _IOWR(ISP_IOC_SENSOR, 2, struct sen_reg)
#define ISP_IOC_SET_SENSOR_MIRROR   _IOW(ISP_IOC_SENSOR, 3, int)
#define ISP_IOC_GET_SENSOR_MIRROR   _IOR(ISP_IOC_SENSOR, 3, int)
#define ISP_IOC_SET_SENSOR_FLIP     _IOW(ISP_IOC_SENSOR, 4, int)
#define ISP_IOC_GET_SENSOR_FLIP     _IOR(ISP_IOC_SENSOR, 4, int)
#define ISP_IOC_SET_SENSOR_EXPOSURE _IOW(ISP_IOC_SENSOR, 5, u32)
#define ISP_IOC_GET_SENSOR_EXPOSURE _IOR(ISP_IOC_SENSOR, 5, u32)
#define ISP_IOC_SET_SENSOR_GAIN     _IOW(ISP_IOC_SENSOR, 6, u32)
#define ISP_IOC_GET_SENSOR_GAIN     _IOR(ISP_IOC_SENSOR, 6, u32)
#define ISP_IOC_SET_SENSOR_FPS      _IOW(ISP_IOC_SENSOR, 7, int)
#define ISP_IOC_GET_SENSOR_FPS      _IOR(ISP_IOC_SENSOR, 7, int)
#define ISP_IOC_SET_SENSOR_AE_EN    _IOW(ISP_IOC_SENSOR, 8, int)
#define ISP_IOC_GET_SENSOR_AE_EN    _IOR(ISP_IOC_SENSOR, 8, int)
#define ISP_IOC_SET_SENSOR_AWB_EN   _IOW(ISP_IOC_SENSOR, 9, int)
#define ISP_IOC_GET_SENSOR_AWB_EN   _IOR(ISP_IOC_SENSOR, 9, int)

#endif /*__IOCTL_ISP210_H__*/

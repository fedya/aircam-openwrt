/**
 * @file ioctl_gm_awb.h
 * Grain-Media AWB IOCTLs
 *
 * Copyright (C) 2010 GM Corp. (http://www.grain-media.com)
 *
 * $Revision: 1.1 $
 * $Date: 2012/02/15 06:36:57 $
 *
 * ChangeLog:
 *  $Log: ioctl_gm_awb.h,v $
 *  Revision 1.1  2012/02/15 06:36:57  ricktsao
 *  no message
 *
 *  Revision 1.7  2011/07/14 13:19:52  ricktsao
 *  1.add following IOCTL
 *     AWB_GET_TARGET_RG_RATIO/AWB_SET_TARGET_RG_RATIO
 *     AWB_GET_TARGET_BG_RATIO/AWB_SET_TARGET_BG_RATIO
 *
 *  Revision 1.6  2011/07/01 06:07:26  ricktsao
 *  1. Add following option
 *      AWB_GET_SPEED/AWB_SET_SPEED
 *      AWB_GET_INTERVAL/AWB_SET_INTERVAL
 *      AWB_GET_FREEZE_SEG/AWB_SET_FREEZE_SEG
 *      AWB_GET_WP_THRESHOLD
 *      AWB_SET_MIN_RsB2G/AWB_SET_MAX_RsB2G
 *      AWB_SET_MIN_R2G/AWB_SET_MAX_R2G
 *      AWB_SET_MIN_B2G/AWB_SET_MAX_B2G
 *      AWB_SET_MIN_YTH/AWB_SET_MAX_YTH
 *      AWB_SET_MIN_RmB2GG/AWB_SET_MAX_RmB2GG
 *
 *  Revision 1.5  2011/05/11 02:06:22  ricktsao
 *  no message
 *
 *  Revision 1.4  2011/01/26 07:04:48  ricktsao
 *  update IOCTL define
 *
 *  Revision 1.3  2011/01/19 08:35:03  ricktsao
 *  1. Add IOCTL command define
 *
 *  Revision 1.2  2011/01/13 13:01:19  ricktsao
 *  Add IOCTL definition
 *
 *  Revision 1.1  2011/01/12 02:48:09  ricktsao
 *  isp210_v2 driver initial version
 *
 */

#ifndef __IOCTL_GM_AWB_H__
#define __IOCTL_GM_AWB_H__

//============================================================================
// struct & flag definition
//============================================================================
typedef struct wp_thres {
    s32 MIN_RsB2G;      //(R-B)/G threshold minimum, fixed point format S.21.10
    s32 MAX_RsB2G;      //(R-B)/G threshold maximum
    s32 MIN_R2G;        //R/G threshold minimum
    s32 MAX_R2G;        //R/G threshold maximum
    s32 MIN_B2G;        //B/G threshold minimum
    s32 MAX_B2G;        //B/G threshold maximum
    s32 MIN_YTH;        //Y Threshold minimum  0~255
    s32 MAX_YTH;        //Y Threshold maximum  0~255
    s32 MIN_RmB2GG;     //R/G * B/G threshold minimum, fixed point format S.11.20
    s32 MAX_RmB2GG;     //R/G * B/G threshold maximum, fixed point format S.11.20
} wp_thres_t;

// AWB scene mode
#define AWB_SCENE_AUTO                  0
#define AWB_SCENE_INCANDESCENT_LIGHT    1
#define AWB_SCENE_COOL_LIGHT            2
#define AWB_SCENE_SUN_LIGHT             3
#define AWB_SCENE_CLOUDY                4
#define AWB_SCENE_SUN_SHADE             5
#define AWB_NUM_OF_SCENE_MODE           6

//============================================================================
// IOCTL sub-command definition
//============================================================================
#define AWB_SET_SCENE_MODE      1       // argument int*
#define AWB_GET_SCENE_MODE      2       // argument int*
#define AWB_GET_SPEED           3       // argument u32*
#define AWB_SET_SPEED           4       // argument u32*
#define AWB_GET_INTERVAL        5       // argument u32*
#define AWB_SET_INTERVAL        6       // argument u32*
#define AWB_GET_FREEZE_SEG      7       // argument u32*
#define AWB_SET_FREEZE_SEG      8       // argument u32*

#define AWB_GET_WP_THRESHOLD    9       // argument wp_thres_t*
#define AWB_SET_MIN_RsB2G       10      // argument s32*
#define AWB_SET_MAX_RsB2G       11      // argument s32*
#define AWB_SET_MIN_R2G         12      // argument s32*
#define AWB_SET_MAX_R2G         13      // argument s32*
#define AWB_SET_MIN_B2G         14      // argument s32*
#define AWB_SET_MAX_B2G         15      // argument s32*
#define AWB_SET_MIN_YTH         16      // argument s32*
#define AWB_SET_MAX_YTH         17      // argument s32*
#define AWB_SET_MIN_RmB2GG      18      // argument s32*
#define AWB_SET_MAX_RmB2GG      19      // argument s32*

#define AWB_GET_TARGET_RG_RATIO 20      // argument u32*
#define AWB_SET_TARGET_RG_RATIO 21      // argument s32*
#define AWB_GET_TARGET_BG_RATIO 22      // argument u32*
#define AWB_SET_TARGET_BG_RATIO 23      // argument u32*

#endif /*__IOCTL_GM_AWB_H__*/

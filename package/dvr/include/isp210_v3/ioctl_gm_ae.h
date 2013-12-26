/**
 * @file ioctl_gm_ae.h
 * Grain-Media AE IOCTLs
 *
 * Copyright (C) 2010 GM Corp. (http://www.grain-media.com)
 *
 * $Revision: 1.1 $
 * $Date: 2012/02/15 06:36:57 $
 *
 * ChangeLog:
 *  $Log: ioctl_gm_ae.h,v $
 *  Revision 1.1  2012/02/15 06:36:57  ricktsao
 *  no message
 *
 *  Revision 1.12  2011/12/30 10:56:45  ricktsao
 *  no message
 *
 *  Revision 1.11  2011/12/20 05:28:50  ricktsao
 *  1. add AE IRIS mode setting
 *  2. add following IOCTL
 *      AE_GET_IRIS_MIN_EXP / AE_GET_IRIS_MIN_EXP
 *      AE_GET_MODE / AE_SET_MODE
 *      AE_GET_CONST_EXP / AE_SET_CONST_EXP
 *      AE_GET_CONST_GAIN / AE_SET_CONST_GAIN
 *
 *  Revision 1.10  2011/10/03 12:16:41  kenny_ch
 *  add AE_SET_IRIS_LIGHT_SCALE/AE_GET_IRIS_LIGHT_SCALE/AE_SET_IRIS_DARK_SCALE/AE_GET_IRIS_DARK_SCALE
 *
 *  Revision 1.9  2011/08/02 07:50:07  kenny_ch
 *  dd sensitivity function
 *   1.1 ioctl command "AE_SET_SENSITIVITY" and "AE_GET_SENSITIVITY" be added
 *   1.2 proc /proc/isp0/ae/sensitivity be added
 *  2. split ae debug message off
 *
 *  Revision 1.8  2011/08/01 11:36:39  kenny_ch
 *  1.Add a new function to control IRIS open and let it last in the maximum aperture.
 *  2.increase threshold of AE convergence to 5
 *
 *  Revision 1.7  2011/07/06 13:50:57  ricktsao
 *  1. Add AE_GET_BOUNDARY_GAIN/AE_SET_BOUNDARY_GAIN
 *
 *  Revision 1.6  2011/06/07 03:30:30  ricktsao
 *  1.Add AE_GET_WIN_WEIGHT and AE_SET_WIN_WEIGHT
 *
 *  Revision 1.5  2011/05/16 03:39:56  ricktsao
 *  no message
 *
 *  Revision 1.4  2011/05/03 13:06:37  ricktsao
 *  no message
 *
 *  Revision 1.3  2011/04/25 06:05:30  ricktsao
 *  1. add following command
 *          AE_SET_MIN_GAIN / AE_GET_MIN_GAIN
 *          AE_SET_MAX_EXPOSURE / AE_GET_MAX_EXPOSURE
 *          AE_SET_MIN_EXPOSURE / AE_GET_MIN_EXPOSURE
 *
 *  Revision 1.2  2011/01/19 08:35:03  ricktsao
 *  1. Add IOCTL command define
 *
 *  Revision 1.1  2011/01/12 02:48:09  ricktsao
 *  isp210_v2 driver initial version
 *
 */

#ifndef __IOCTL_GM_AE_H__
#define __IOCTL_GM_AE_H__

#include "ioctl_isp210.h"

//============================================================================
// struct & flag definition
//============================================================================
// static allocation
#define AE_WIN_XNUM         8
#define AE_WIN_YNUM         8

// AE ev mode
#define AE_EV_MODE_VIDEO    0
#define AE_EV_MODE_CAMERA   1
#define AE_EV_MODE_USER     2
#define AE_NUM_OF_EV_MODE   3

// AE power frequency
#define AE_PWR_FREQ_60      0
#define AE_PWR_FREQ_50      1

// AE IRIS mode
#define AE_MODE_AUTO                0
#define AE_MODE_APERTURE_PRIORITY   1
#define AE_MODE_SHUTTER_PRIORITY    2

//============================================================================
// IOCTL command definition
//============================================================================
#define AE_GET_CURRENTY             0       // argument int*
#define AE_SET_TARGETY              1       // argument int*
#define AE_GET_TARGETY              2       // argument int*
#define AE_SET_EV_MODE              3       // argument int*
#define AE_GET_EV_MODE              4       // argument int*
#define AE_SET_PWR_FREQ             5       // argument int*
#define AE_GET_PWR_FREQ             6       // argument int*
#define AE_SET_MAX_GAIN             7       // argument u32*
#define AE_GET_MAX_GAIN             8       // argument u32*
#define AE_SET_MIN_GAIN             9       // argument u32*
#define AE_GET_MIN_GAIN             10      // argument u32*
#define AE_SET_MAX_EXPOSURE         11      // argument u32*
#define AE_GET_MAX_EXPOSURE         12      // argument u32*
#define AE_SET_MIN_EXPOSURE         13      // argument u32*
#define AE_GET_MIN_EXPOSURE         14      // argument u32*
#define AE_GET_SPEED                15      // argument u32*
#define AE_SET_SPEED                16      // argument u32*
#define AE_GET_WIN_WEIGHT           17      // argument int[AE_WIN_XNUM*AE_WIN_YNUM]
#define AE_SET_WIN_WEIGHT           18      // argument int[AE_WIN_XNUM*AE_WIN_YNUM]
#define AE_GET_BOUNDARY_GAIN        19      // argument u32*
#define AE_SET_BOUNDARY_GAIN        20      // argument u32*
#define AE_SET_IRIS_ENABLE          21      // argument u32*
#define AE_GET_IRIS_ENABLE          22      // argument u32*
#define AE_SET_SENSITIVITY          23      // argument u32*
#define AE_GET_SENSITIVITY          24      // argument u32*
#define AE_SET_IRIS_LIGHT_SCALE     25      // argument u32*
#define AE_GET_IRIS_LIGHT_SCALE     26      // argument u32*
#define AE_SET_IRIS_DARK_SCALE      27      // argument u32*
#define AE_GET_IRIS_DARK_SCALE      28      // argument u32*
#define AE_GET_IRIS_MIN_EXP         29      // argument u32*
#define AE_SET_IRIS_MIN_EXP         30      // argument u32*
#define AE_GET_MODE                 31      // argument int*
#define AE_SET_MODE                 32      // argument int*
#define AE_GET_CONST_EXP            33      // argument u32*
#define AE_SET_CONST_EXP            34      // argument u32*
#define AE_GET_CONST_GAIN           35      // argument u32*
#define AE_SET_CONST_GAIN           36      // argument u32*

#endif /*__IOCTL_GM_AE_H__*/

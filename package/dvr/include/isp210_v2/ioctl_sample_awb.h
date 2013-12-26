/**
 * @file sample_awb.c
 * Sample AWB IOCTLs
 *
 * Copyright (C) 2010 GM Corp. (http://www.grain-media.com)
 *
 * $Revision: 1.1 $
 * $Date: 2011/12/30 10:59:07 $
 *
 * ChangeLog:
 *  $Log: ioctl_sample_awb.h,v $
 *  Revision 1.1  2011/12/30 10:59:07  ricktsao
 *  add sample AWB
 *
 */

#ifndef __IOCTL_SAMPLE_AWB_H__
#define __IOCTL_SAMPLE_AWB_H__

#include "ioctl_isp210.h"

//============================================================================
// struct & flag definition
//============================================================================


//============================================================================
// IOCTL command definition
//============================================================================
#define AWB_GET_SPEED           1       // argument u32*
#define AWB_SET_SPEED           2       // argument u32*

#endif /*__IOCTL_SAMPLE_AWB_H__*/

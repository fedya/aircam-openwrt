/**
 * @file sample_ae.c
 * Sample AE IOCTLs
 *
 * Copyright (C) 2010 GM Corp. (http://www.grain-media.com)
 *
 * $Revision: 1.1 $
 * $Date: 2012/02/15 06:36:57 $
 *
 * ChangeLog:
 *  $Log: ioctl_sample_ae.h,v $
 *  Revision 1.1  2012/02/15 06:36:57  ricktsao
 *  no message
 *
 *  Revision 1.1  2011/12/30 10:58:38  ricktsao
 *  add sample AE
 *
 */

#ifndef __IOCTL_SAMPLE_AE_H__
#define __IOCTL_SAMPLE_AE_H__

#include "ioctl_isp210.h"

//============================================================================
// struct & flag definition
//============================================================================


//============================================================================
// IOCTL command definition
//============================================================================
#define AE_SET_TARGETY              1       // argument int*
#define AE_GET_TARGETY              2       // argument int*
#define AE_GET_SPEED                3       // argument u32*
#define AE_SET_SPEED                4       // argument u32*

#endif /*__IOCTL_SAMPLE_AE_H__*/

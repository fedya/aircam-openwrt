#ifndef __ISP_API_H__
#define __ISP_API_H__

#include <linux/ioctl.h>

#ifndef __KERNEL__                                                                                                           
#include <stdint.h>                                                                                                          
#endif
//define for ISP_IO_SET_AE_EV_MODE
#define AE_EV_MODE_VIDEO            0
#define AE_EV_MODE_CAMERA           1
//define for ISP_IO_SET_GAMMA
#define GAMMA_CURVE_22    0 
#define GAMMA_CURVE_18    1
#define GAMMA_CURVE_16    2
#define GAMMA_CURVE_14    3

/* Use 's' as magic number */                                                                                                
#define ISP_IOC_MAGIC  's'                                                                                                  
                                                                                                                             
#define ISP_IO_ENABLE_AE             _IOW(ISP_IOC_MAGIC, 1, int)    //enable/disable AutoExposure
#define ISP_IO_ENABLE_AWB          _IOW(ISP_IOC_MAGIC, 2, int)    //enable/disable AutoWhiteBalance
#define ISP_IO_ENABLE_AF             _IOW(ISP_IOC_MAGIC, 3, int)    //enable/disable AutoFocus
#define ISP_IO_SET_GAMMA            _IOW(ISP_IOC_MAGIC, 4, int)    //Select Gamma Curve
//define for AE parameters
#define ISP_IO_SET_AE_EV_MODE   _IOW(ISP_IOC_MAGIC, 5, int)    //Set AE to Video/Camera mode
#define ISP_IO_SET_AE_FREQ         _IOW(ISP_IOC_MAGIC, 6, int)    //Set AE power frequency 50/60
#define ISP_IO_SET_AE_TARGET      _IOW(ISP_IOC_MAGIC, 7, int)    //Set AE target brightness
//define for AWB parameters
#define ISP_IO_SET_AWB_SPEED    _IOW(ISP_IOC_MAGIC, 11, int)   //adjust AWB speed

//define for MD parameters
#define ISP_IO_ENABLE_MD             _IOW(ISP_IOC_MAGIC, 12, int)    //enable/disable Motion Detection
#define ISP_IO_SET_SHAD_EN         _IOW(ISP_IOC_MAGIC, 13, int)    
#define ISP_IO_SET_SHAD_TH         _IOW(ISP_IOC_MAGIC, 14, int)   
#define ISP_IO_SET_A_ALPHA         _IOW(ISP_IOC_MAGIC, 15, int)   
#define ISP_IO_SET_SMB_TH           _IOW(ISP_IOC_MAGIC, 16, int) 
#define ISP_IO_SET_TBG_TH           _IOW(ISP_IOC_MAGIC, 17, int)   

#define ISP_IO_GET_AE_FREQ            _IOR(ISP_IOC_MAGIC, 100+1, int)    //Get current AE power frequency
#define ISP_IO_GET_RES                   _IOR(ISP_IOC_MAGIC, 100+2, int)   //Get Image width, high
#define ISP_IO_GET_MD_INFO            _IOR(ISP_IOC_MAGIC, 100+3, int)   //Get Motion Detection result

#endif /*__ISP_API_H__*/

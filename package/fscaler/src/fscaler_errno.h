/**
 * @file fscaler_driver.h
 * @brief Scaler driver header file
 * @author slchen <slchen@grain-media.com>
 * @version 0.9.0
 * @date 2010 Nov 1
 * @copyright (C) 2010 Grain Media Corp. < http://www.grain-media.com >
 */
#ifndef _FSCALER_ERRNO_H_
#define _FSCALER_ERRNO_H_

#define FSCALER_FAC_COMMON                  0x0000
#define FSCALER_FAC_PARAM                   0x0001
#define FSCALER_FAC_DRIVER                  0x0002
#define FSCALER_FAC_VG                      0x0003
#define FSCALER_FAC_IOCTL                   0x0004
#define FSCALER_FAC_OSD                     0x0005
#define FSCALER_FAC_ENTITY                  0x0006
#define FSCALER_FAC_DEVICE                  0x0007
#define FSCALER_FAC_JOB                     0x0008
#define FSCALER_FAC_HW                      0x0009

#define FSCALER_SEV_OK	                    0
#define FSCALER_SEV_ERR		                1

#define FSCALER_CODE(sev,fac,code) \
    ((int)(((unsigned int)(sev)<<31)|((unsigned int)(fac)<<16)|((unsigned int)(code))))

#define FSCALER_FAILED(hr)                  ((unsigned int)(hr) >> 31)
#define FSCALER_SUCCEED(hr)                 (!FSCALER_FAILED(hr))

#define FSCALER_OK                          FSCALER_CODE(FSCALER_SEV_OK, FSCALER_FAC_COMMON, 0) ///< Success
#define FSCALER_ERR_FAIL                    FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 1)        ///< Normal failure
#define FSCALER_ERR_CREATE                  FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 2)        ///< Failed to create resource
#define FSCALER_ERR_DESTROY                 FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 3)        ///< Failed to destroy resource
#define FSCALER_ERR_INSTALL                 FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 4)        ///< Failed to install/register
#define FSCALER_ERR_UNINSTALL               FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 5)        ///< Failed to uninstall/deregister
#define FSCALER_ERR_PREPARE                 FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 6)        ///< Failed to prepare or pre-process
#define FSCALER_ERR_UNPREPARE               FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 7)        ///< Failed to unprepare or post-process
#define FSCALER_ERR_OPEN                    FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 8)        ///< Failed to open
#define FSCALER_ERR_CLOSE                   FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 9)        ///< Failed to close
#define FSCALER_ERR_SET                     FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 10)       ///< Failed to set resource/property
#define FSCALER_ERR_GET                     FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 11)       ///< Failed to get resource/property
#define FSCALER_ERR_DROPPED                 FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 12)       ///< Dropped, drop(reject to pass through) and return directly without success or failure
#define FSCALER_ERR_ABORTED                 FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 13)       ///< Aborted, pass through and return failure
#define FSCALER_ERR_REJECTED                FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 14)       ///< Rejected, reject(reject to pass through) and return failure
#define FSCALER_ERR_UNSUPPORTED             FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_COMMON, 15)       ///< Unsupported

// Scaler hardware return code
#define FSCALER_HW_OK_DAHB_IDLE             FSCALER_CODE(FSCALER_SEV_OK, FSCALER_FAC_HW, 3)     ///< Success DAHB idle
#define FSCALER_HW_OK_STS                   FSCALER_CODE(FSCALER_SEV_OK, FSCALER_FAC_HW, 2)     ///< Success status done
#define FSCALER_HW_OK_FAKE_DMAOVF           FSCALER_CODE(FSCALER_SEV_OK, FSCALER_FAC_HW, 1)     ///< Success with fake DMA overflow
#define FSCALER_HW_OK                       FSCALER_CODE(FSCALER_SEV_OK, FSCALER_FAC_HW, 0)     ///< Sucess if DAHB idle and status done(at the same time or sequentailly)
#define FSCALER_HW_ERR_STSTEST              FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_HW, 1)    ///< Scaler hardware test status error
#define FSCALER_HW_ERR_DMAOVF               FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_HW, 2)    ///< Scaler hardware DMA overflow error
#define FSCALER_HW_ERR_UAHB                 FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_HW, 3)    ///< Scaler hardware UAHB error
#define FSCALER_HW_ERR_NULL                 FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_HW, 4)    ///< Scaler hardware NULL error
#define FSCALER_HW_ERR_DAHB                 FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_HW, 5)    ///< Scaler hardware DAHB error
#define FSCALER_HW_ERR_BUSY                 FSCALER_CODE(FSCALER_SEV_ERR, FSCALER_FAC_HW, 6)    ///< Scaler hardware status busy timeout

#endif /* _FSCALER_ERRNO_H_ */

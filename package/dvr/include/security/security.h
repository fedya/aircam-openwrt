/*! \mainpage GM8126/GM812X AES/DES/Triple-DES Security API Reference
 *
 * \n security.h describe AES/DES/Triple-DES Security ioctl functions behavior
 */

/**
 *  \example security_test.c
*/

#ifndef _SECURITY_H_
#define _SECURITY_H_
#include <linux/ioctl.h>
#include <linux/sched.h>

#ifndef __KERNEL__
#include <stdint.h>
#endif

/**
 * @brief security algorithm argument and address
 */
typedef struct esreq_tag {
    int algorithm;///< security algorithm, such as Algorithm_DES/Algorithm_Triple_DES/Algorithm_AES_128/Algorithm_AES_192/Algorithm_AES_256
    int mode;			///< security mode, such as ECB_mode/CBC_mode/CTR_mode/CFB_mode/OFB_mode

    unsigned int data_length;   ///< size of data, max value = 4096, include 16 bytes IV
    unsigned int key_length;    ///< size of key
    unsigned int IV_length;     ///< size of Initial Vector

    unsigned int key_addr[8];   ///< key array, it maybe be modify by security algorithm
    unsigned int IV_addr[4];    ///< Initial Vector array, it maybe be modify by security algorithm

    unsigned int DataIn_addr;   ///< input data address point
    unsigned int DataOut_addr;  ///< output data address point

    unsigned int Key_backup;    ///< backup key
    unsigned int IV_backup;     ///< backup Initial Vector
} esreq;


/* Use 'e' as magic number */
#define IOC_MAGIC  'e'

/**
 * \b ioctl(security_fd, ES_GETKEY, &tag)
 *
 * \arg get random key and Initial Vector
 * \arg parameter :
 * \n \b \e pointer \b \e tag : argument from user space ioctl parameter, it means structure esreq  
 */
#define ES_GETKEY           _IOWR(IOC_MAGIC, 8, esreq)
/**
 * \b ioctl(security_fd, ES_GETKEY, &tag)
 *
 * \arg encrypt data, must set key and Initial Vector first
 * \arg parameter :
 * \n \b \e pointer \b \e tag : argument from user space ioctl parameter, it means structure esreq  
 */
#define ES_ENCRYPT          _IOWR(IOC_MAGIC, 9, esreq)
/**
 * \b ioctl(security_fd, ES_GETKEY, &tag)
 *
 * \arg decrypt data, must set key and Initial Vector first
 * \arg parameter :
 * \n \b \e pointer \b \e tag : argument from user space ioctl parameter, it means structure esreq  
 */
#define ES_DECRYPT          _IOWR(IOC_MAGIC, 10, esreq)
/**
 * \b ioctl(security_fd, ES_GETKEY, &tag)
 *
 * \arg auto encrypt data, key and Initial Vector will be auto generated
 * \arg parameter :
 * \n \b \e pointer \b \e tag : argument from user space ioctl parameter, it means structure esreq  
 */
#define ES_AUTO_ENCRYPT    	_IOWR(IOC_MAGIC, 11, esreq)
/**
 * \b ioctl(security_fd, ES_GETKEY, &tag)
 *
 * \arg auto decrypt data, key and Initial Vector will be auto generated
 * \arg parameter :
 * \n \b \e pointer \b \e tag : argument from user space ioctl parameter, it means structure esreq  
 */
#define ES_AUTO_DECRYPT    	_IOWR(IOC_MAGIC, 12, esreq)

/* security mode */
#define ECB_mode			0x00
#define CBC_mode			0x10
#define CTR_mode			0x20
#define CFB_mode			0x40
#define OFB_mode			0x50

/* security algorithm */
#define Algorithm_DES			0x0
#define Algorithm_Triple_DES		0x2
#define Algorithm_AES_128		0x8
#define Algorithm_AES_192		0xA
#define Algorithm_AES_256		0xC

#endif /* _SECURITY_H_ */

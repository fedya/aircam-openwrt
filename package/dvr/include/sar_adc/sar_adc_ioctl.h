#ifndef __SAR_ADC_IOCTL_H__
#define __SAR_ADC_IOCTL_H__

#include <linux/ioctl.h>

#define SAR_ADC_MAGIC 'S'
#define SAR_ADC_KEY_ADC_DIRECT_READ                _IOR(SAR_ADC_MAGIC, 1, int) 

#endif //end of __SAR_ADC_IOCTL_H__



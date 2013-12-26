#ifndef _FTGPIO_API_H_
#define _FTGPIO_API_H_

#include <linux/ioctl.h>

#define GPIO_NUM 3

typedef struct _GPIO_PIN {
	unsigned int port[GPIO_NUM];
	unsigned int value[GPIO_NUM];
} gpio_pin;

//============================================================================
// I/O control ID
//============================================================================
/* Use 'g' as magic number */
#define GPIO_IOC_MAGIC		'g'
#define GPIO_SET_MULTI_PINS_OUT		_IOW(GPIO_IOC_MAGIC, 1, gpio_pin)
#define GPIO_SET_MULTI_PINS_IN		_IOW(GPIO_IOC_MAGIC, 2, gpio_pin)
#define GPIO_GET_MULTI_PINS_VALUE	_IOW(GPIO_IOC_MAGIC, 3, gpio_pin)
#define GPIO_IOC_MAXNR 3

#endif /*_FTGPIO_API_H_*/

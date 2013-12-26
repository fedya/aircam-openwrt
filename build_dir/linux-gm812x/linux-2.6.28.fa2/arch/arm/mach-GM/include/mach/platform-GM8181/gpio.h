/*
 *  $Name:  $
 *  $Author: mars_ch $
 *  $Date: 2010/08/03 03:36:16 $
 *  $Log: gpio.h,v $
 *  Revision 1.5  2010/08/03 03:36:16  mars_ch
 *  *: rename gpio api
 *
 *  Revision 1.4  2010/08/02 08:05:11  mars_ch
 *  *: clean gm gpio driver code again
 *
 *  Revision 1.3  2010/08/02 07:56:07  mars_ch
 *  *: clean gm gpio driver code
 *
 *  Revision 1.2  2010/08/02 07:03:54  mars_ch
 *  *: rename gpio define for GM controller
 *
 *  Revision 1.1  2010/06/21 07:42:15  mars_ch
 *  *: add gpio platform support
 *
 *  $Revision: 1.5 $ 
 */

#ifndef __GM8181_GPIO_H__
#define __GM8181_GPIO_H__

#define GM_GPIO010_NR_PORT		1
#define GM_GPIO_NR_PIN_PER_PORT		32
#define ARCH_NR_GPIOS			(GM_GPIO_NR_PIN_PER_PORT*GM_GPIO010_NR_PORT)

static inline int gm_gpio_pin_index(unsigned port, unsigned pin)
{
	int ret = port*GM_GPIO_NR_PIN_PER_PORT + pin;
	
	return ret < ARCH_NR_GPIOS ? ret : -1;
} 

#endif//end of __GM8181_GPIO_H__

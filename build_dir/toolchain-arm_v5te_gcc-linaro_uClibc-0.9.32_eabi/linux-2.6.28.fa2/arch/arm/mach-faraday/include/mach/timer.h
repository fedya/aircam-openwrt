/*
 *  arch/arm/mach-faraday/include/mach/timer.h
 *
 *  Faraday FTTMR010 Timer Device Driver Interface
 *
 *  Copyright (C) 2005 Faraday Corp. (http://www.faraday-tech.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Note
 * 
 *  As IP_COUNT might be greater than one, timer ID is computed as follows:
 *  id=0~2 : Timer 1~3 of the first FTTMR010 IP
 *  id=3~5 : Timer 1~3 of the second FTTMR010 IP
 *  ...
 *  Therefore:
 *    (id / 3) : Compute which IP
 *    (id % 3) : Compute which timer in this IP
 *  Notice:
 *    For simplicity's sake, all code does not check for invalid timer id
 * 
 * ChangeLog
 * 
 *  Luke Lee  09/14/2005  Created, heavily modified from Faraday CPE platform code.
 */


#ifndef __FARADAY_TIMER_FTTMR010_HEADER__
#define __FARADAY_TIMER_FTTMR010_HEADER__

/*
 * Definition of register offsets
 */

#define TIMER1_COUNT                0x0
#define TIMER1_LOAD                 0x4
#define TIMER1_MATCH1               0x8
#define TIMER1_MATCH2               0xC

#define TIMER2_COUNT                0x10
#define TIMER2_LOAD                 0x14
#define TIMER2_MATCH1               0x18
#define TIMER2_MATCH2               0x1C

#define TIMER3_COUNT                0x20
#define TIMER3_LOAD                 0x24
#define TIMER3_MATCH1               0x28
#define TIMER3_MATCH2               0x2C

#define TIMER_TMCR                  0x30
#define TIMER_INTRSTATE             0x34
#define TIMER_INTRMASK              0x38

/* Each timer's register address is offset by 0x10 */
#define TIMER_OFFSET                0x10

/*
 * Definition of TMCR bits
 */

#define TM1ENABLE                   1
#define TM1CLOCK                    (1<<1)
#define TM1OFENABLE                 (1<<2)

#define TM2ENABLE                   (1<<3)
#define TM2CLOCK                    (1<<4)
#define TM2OFENABLE                 (1<<5)

#define TM3ENABLE                   (1<<6)
#define TM3CLOCK                    (1<<7)
#define TM3OFENABLE                 (1<<8)

#define TM1UPDOWN                   (1<<9)
#define TM2UPDOWN                   (1<<10)
#define TM3UPDOWN                   (1<<11)

struct sys_timer;
extern struct sys_timer platform_timer;

/*
 * Enable a specific timer
 */
void timer_fttmr010_enable(unsigned int id);

/*
 * Disable a specific timer
 */
void timer_fttmr010_disable(unsigned int id);

/*
 * Set current counter value
 */
void timer_fttmr010_set_counter(unsigned int id, unsigned int value);

/*
 * Get current counter value
 */
unsigned int timer_fttmr010_get_counter(unsigned int id);

/*
 * Set match1 value
 */
void timer_fttmr010_set_match1(unsigned int id, unsigned int value);

/*
 * Set match2 value
 */
void timer_fttmr010_set_match2(unsigned int id, unsigned int value);

/*
 * Set counter expiration reload value
 */
void timer_fttmr010_set_reload(unsigned int id, unsigned int value);

/*
 * Get counter expiration reload value
 */
unsigned int timer_fttmr010_get_reload(unsigned int id);

/*
 * Map timer ID to the associated irq number.
 */
unsigned int timer_fttmr010_id_to_irq(unsigned int id);

#endif // __FARADAY_TIMER_FTTMR010_HEADER__

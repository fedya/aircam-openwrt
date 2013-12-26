/*
 *  linux/arch/arm/mach-faraday/timer.c
 *
 *  Faraday FTTMR010 Timer Device Driver Implementation
 *
 *  Copyright (C) 2005 Faraday Corp. (http://www.faraday-tech.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
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
 *  Luke Lee  09/14/2005  Created
 */

#include <linux/time.h>
#include <linux/timex.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <asm/io.h>

#include <asm/mach/time.h>
#include <asm/irq.h>
#include <asm/param.h>

#include <mach/timer.h>
#include <mach/spec.h>

#define IPMODULE TIMER
#define IPNAME   FTTMR010
/* used to protect the timer update */
static spinlock_t timerlock;

static unsigned setup_flag[3] = {
        TM1UPDOWN | TM1CLOCK, TM2UPDOWN | TM2CLOCK, TM3UPDOWN | TM3CLOCK
};
static unsigned enable_flag[3] = {
        TM1ENABLE | TM1OFENABLE, TM2ENABLE | TM2OFENABLE,
        TM3ENABLE | TM3OFENABLE,
};

static unsigned long timer_fttmr010_base(unsigned int id)
{
        return IP_va_base[id / 3];
}

static void timer_fttmr010_write(unsigned int id, unsigned int reg, int val)
{
        outl(val, timer_fttmr010_base(id) + (id % 3) * TIMER_OFFSET + reg);
}

static unsigned int timer_fttmr010_read(unsigned int id, unsigned int reg)
{
        return inl(timer_fttmr010_base(id) + (id % 3) * TIMER_OFFSET + reg);
}

/*
 * Enable a specific timer in the system
 */
void timer_fttmr010_enable(unsigned int id)
{
        unsigned int cr = timer_fttmr010_read(id, TIMER_TMCR);

        cr &= ~setup_flag[id % 3];
        cr |= enable_flag[id % 3];

        timer_fttmr010_write(id, TIMER_TMCR, cr);
}

/*
 * Disable a specific timer
 */
void timer_fttmr010_disable(unsigned int id)
{
        unsigned int cr = timer_fttmr010_read(id, TIMER_TMCR);

        cr &= ~enable_flag[id % 3];

        timer_fttmr010_write(id, TIMER_TMCR, cr);
}

/*
 * Set current counter value
 */
void timer_fttmr010_set_counter(unsigned int id, unsigned int value)
{
        timer_fttmr010_write(id, TIMER1_COUNT, value);
}

/*
 * Get current counter value
 */
unsigned int timer_fttmr010_get_counter(unsigned int id)
{
        return timer_fttmr010_read(id, TIMER1_COUNT);
}

/*
 * Set match1 value
 */
void timer_fttmr010_set_match1(unsigned int id, unsigned int value)
{
        timer_fttmr010_write(id, TIMER1_MATCH1, value);
}

/*
 * Set match2 value
 */
void timer_fttmr010_set_match2(unsigned int id, unsigned int value)
{
        timer_fttmr010_write(id, TIMER1_MATCH2, value);
}
/*
 * Set counter expiration reload value
 */
void timer_fttmr010_set_reload(unsigned int id, unsigned int value)
{
        timer_fttmr010_write(id, TIMER1_LOAD, value);
}

/*
 * Set counter expiration reload value
 */
unsigned int timer_fttmr010_get_reload(unsigned int id)
{
        return timer_fttmr010_read(id, TIMER1_LOAD);
}

/*
 * Map timer ID to the associated irq number.
 */
unsigned int timer_fttmr010_id_to_irq(unsigned int id)
{
        return IP_irq[id / 3][id % 3];
}

/*
 * The first timer in the system is used as the system clock tick
 */
static irqreturn_t timer0_interrupt(int irq, void *dev_id)
{
    unsigned long flags;
    
    spin_lock_irqsave(&timerlock, flags);
	timer_tick();
	spin_unlock_irqrestore(&timerlock, flags);
	
	return IRQ_HANDLED;
}

static struct irqaction timer0_irq = {
	.name = "Timer Tick",
	.flags = IRQF_DISABLED,
	.handler = timer0_interrupt,
};

static struct resource timer_resource = {
	.name = "Timer 0~2",
	.start = IP_VA_BASE(0),
	.end = IP_VA_BASE(0) + IP_VA_SIZE(0),
};

/*
 * Timer initialization, used by platform.c, called only once
 */
extern int setup_irq(unsigned int irq, struct irqaction *new);
static void __init timer_fttmr010_init(void)
{
    spin_lock_init(&timerlock);
    
	/*
	 * Here we initialize only the first timer in system,
	 * To use any other timer, the initilization steps are similar to the following.
	 */
	/* reload value = AHB_CLK_IN/2/HZ - 1, as suggested by MBU/VS/Justine */
	timer_fttmr010_set_reload(0, APB_CLK_IN / HZ - 1);
	timer_fttmr010_set_counter(0, APB_CLK_IN / HZ - 1);
	timer_fttmr010_set_match1(0, 0);
	timer_fttmr010_set_match2(0, 0);
	timer_fttmr010_enable(0);

	printk(KERN_INFO
	       "FTTMR010 timer 0 installed on IRQ %d, with clock %d at %d HZ.\r\n",
	       timer_fttmr010_id_to_irq(0), APB_CLK_IN, HZ);

	setup_irq(timer_fttmr010_id_to_irq(0), &timer0_irq);

	/* Register I/O address range of this timer */
	request_resource(&ioport_resource, &timer_resource);
}

/*
 * This data structure cannot be __initdata as the .offset field keep accessed by
 * arm/kernel/time.c function do_gettimeofday(struct timeval *tv)
 */
struct sys_timer platform_timer = {
	.init = timer_fttmr010_init,
};

/*
 * Timer Export API
 */
EXPORT_SYMBOL(timer_fttmr010_enable);
EXPORT_SYMBOL(timer_fttmr010_disable);
EXPORT_SYMBOL(timer_fttmr010_set_counter);
EXPORT_SYMBOL(timer_fttmr010_get_counter);
EXPORT_SYMBOL(timer_fttmr010_set_match1);
EXPORT_SYMBOL(timer_fttmr010_set_match2);
EXPORT_SYMBOL(timer_fttmr010_set_reload);
EXPORT_SYMBOL(timer_fttmr010_get_reload);
EXPORT_SYMBOL(timer_fttmr010_id_to_irq);


/*
 *  Copyright (C) 2009 Faraday Corp. (http://www.faraday-tech.com)
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
 */

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/pm.h>

#include <mach/system.h>

/*
 * FA526/FA626TE requires irq enabled before idle
 */
static void fa_idle(void)
{
	if (!need_resched())
		arch_idle();
}

static int __init fa_idle_init(void)
{
        pm_idle = fa_idle;
        return 0;
}

arch_initcall(fa_idle_init);


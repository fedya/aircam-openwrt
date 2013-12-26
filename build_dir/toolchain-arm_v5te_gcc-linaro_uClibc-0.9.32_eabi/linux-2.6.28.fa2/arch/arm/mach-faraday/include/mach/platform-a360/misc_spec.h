/*
 *  arch/arm/mach-faraday/include/mach/platform-A360/misc_spec.h
 *
 *  Faraday A360 platform dependent definitions
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
 * ChangeLog
 * 
 *  Luke Lee  09/14/2005  Created
 *  Luke Lee  10/06/2005  Modified for automatic system clock rate
 */

#ifndef __MISC_SPEC_H__
#define __MISC_SPEC_H__

#define BOOT_PARAMETER_PA_BASE 0x100

#define PLATFORM_BOOTTIME_IO_PA_BASE   UART_FTUART010_0_PA_BASE
#define PLATFORM_BOOTTIME_IO_VA_BASE   UART_FTUART010_0_VA_BASE

#ifndef __ASSEMBLY__
extern unsigned int a360_get_ahb_clk(void);
extern unsigned int a360_get_apb_clk(void);
#define AHB_CLK_IN    a360_get_ahb_clk()
#define APB_CLK_IN    a360_get_apb_clk()
#endif //__ASSEMBLY__

/*
 * The "Main CLK" Oscillator on the board which is used by the PLL to generate
 * CPU/AHB/APB clocks.
 */
#define MAIN_CLK	40000000

#endif /*__MISC_SPEC_H__ */


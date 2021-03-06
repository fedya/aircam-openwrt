/*
 *  arch/arm/mach-faraday/include/mach/platform-a320/serial.h
 *  
 *  Serial port definition
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
 * Note
 *
 *  There are 3 UARTs (FTUART010) in A320D platform
 * 
 * ChangeLog
 *
 *  Luke Lee  09/15/2005  Created.
 *  Luke Lee  11/16/2005  Add conditional compilation.
 */

     /* UART CLK        PORT                      IRQ                   FLAGS       hub6 io_type iomem_base reg_shift*/
#ifdef UART_FTUART010_1_VA_BASE
#define EXTENDED_UART_1 \
     { 0, BASE_BAUD, UART_FTUART010_1_VA_BASE, UART_FTUART010_1_IRQ, STD_COM_FLAGS,   0,    0,     NULL,       2 }, /* ttyS1 */
#else
#define EXTENDED_UART_1
#endif

#ifdef UART_FTUART010_2_VA_BASE
#define EXTENDED_UART_2 \
     { 0, BASE_BAUD, UART_FTUART010_2_VA_BASE, UART_FTUART010_2_IRQ, STD_COM_FLAGS,   0,    0,     NULL,       2 }, /* ttyS2 */
#else
#define EXTENDED_UART_2
#endif


#define PLATFORM_MORE_SERIAL_PORTS \
        EXTENDED_UART_1            \
        EXTENDED_UART_2

/*
 *  arch/arm/mach-faraday/include/mach/platform-a320/apb_dma.h
 *  
 *  Faraday Platform Clock Tick Rate Definition
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
 *  Peter Liao 10/04/2005  Created.
 */
 
#ifndef __FARADAY_PLATFORM_A320_APBBRG_FTAPBBRG020S_H_ 
#define __FARADAY_PLATFORM_A320_APBBRG_FTAPBBRG020S_H_ 

#include <asm/io.h>

static inline void fa_setup_apb_pmu(int ch, int req_num)
{
	/*
	 * We must to set up PMU to enable APB DMA. If not, IP may be configurated to AHB support by other drivers
	 * before. In this case, IP cannot be used with APB DMA anymore.
	 */

	switch (req_num)
	{
		case 1://CFC
			outl(ch&~0x8, PMU_FTPMU010_VA_BASE + 0xA0);
			break;
		case 2://SSP
			outl(ch&~0x8, PMU_FTPMU010_VA_BASE + 0xA4);
			break;
		case 3://FFUART(UART1)
			outl(ch&~0x8, PMU_FTPMU010_VA_BASE + 0xA8);
			outl(ch&~0x8, PMU_FTPMU010_VA_BASE + 0xAC);
			break;
		case 4://BTUART(UART2)
			outl(ch&~0x8, PMU_FTPMU010_VA_BASE + 0xB0);
			outl(ch&~0x8, PMU_FTPMU010_VA_BASE + 0xB4);
			break;
		case 5://SDC
			outl(ch&~0x8, PMU_FTPMU010_VA_BASE + 0xB8);
			break;
		case 6://I2S/AC97
			outl(ch&~0x8, PMU_FTPMU010_VA_BASE + 0xBC);
			break;
		case 8://STUART(UART4)
			//outl(ch&~0x8, PMU_FTPMU010_VA_BASE + 0xA0);
			break;
		case 11://IrDA
			outl(ch&~0x8, PMU_FTPMU010_VA_BASE + 0xc0);
			outl(ch&~0x8, PMU_FTPMU010_VA_BASE + 0xcc);
			outl(ch&~0x8, PMU_FTPMU010_VA_BASE + 0xd0);
			break;
	}
}

static inline void fa_reset_apb_pmu(int ch, int req_num)
{
	switch (req_num)
	{
		case 1://CFC
			outl(0x0, PMU_FTPMU010_VA_BASE + 0xA0);
			break;
		case 2://SSP
			outl(0x0, PMU_FTPMU010_VA_BASE + 0xA4);
			break;
		case 3://FFUART(UART1)
			outl(0x0, PMU_FTPMU010_VA_BASE + 0xA8);
			outl(0x0, PMU_FTPMU010_VA_BASE + 0xAC);
			break;
		case 4://BTUART(UART2)
			outl(0x0, PMU_FTPMU010_VA_BASE + 0xB0);
			outl(0x0, PMU_FTPMU010_VA_BASE + 0xB4);
			break;
		case 5://SDC
			outl(0x0, PMU_FTPMU010_VA_BASE + 0xB8);
			break;
		case 6://I2S/AC97
			outl(0x0, PMU_FTPMU010_VA_BASE + 0xBC);
			break;
		case 8://STUART(UART4)
			//outl(0x0, PMU_FTPMU010_VA_BASE + 0xA0);
			break;
		case 11://IrDA
			outl(0x0, PMU_FTPMU010_VA_BASE + 0xc0);
			outl(0x0, PMU_FTPMU010_VA_BASE + 0xcc);
			outl(0x0, PMU_FTPMU010_VA_BASE + 0xd0);
			break;
	}
}

#endif /* __FARADAY_PLATFORM_A320_APBBRG_FTAPBBRG020S_H_  */

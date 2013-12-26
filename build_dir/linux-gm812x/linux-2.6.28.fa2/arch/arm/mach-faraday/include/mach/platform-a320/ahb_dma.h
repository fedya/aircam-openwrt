/*
 *  arch/arm/mach-faraday/include/mach/platform-a320/ahb_dma.h
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
 
/* ahb_dma.h */
#ifndef __FARADAY_PLATFORM_A320_DMAC_FTDMAC020_H_
#define __FARADAY_PLATFORM_A320_DMAC_FTDMAC020_H_

/* Platform depedent inline funcitoin used by AHB DMA driver */

#include <asm/io.h>

static inline int fa_setup_ahb_pmu(int ch, int req_pair)
{
	// req_pair = 0 used for mem to mem testing
	if ( (req_pair > 16) ||  (req_pair < 0) ) {
		printk(KERN_ERR "Unsupport request pair!\n");
		return -ENODEV;
	} else if ( (req_pair < 16) && (req_pair > 0) ) {
		outl(ch|0x8, PMU_FTPMU010_VA_BASE + 0xA0+(req_pair-1)*4);
		dma_dbg("Routing reg 0x%08X set to %08X\n", PMU_FTPMU010_VA_BASE + 0xA0+(req_pair-1)*4, inl(PMU_FTPMU010_VA_BASE+0xA0+(req_pair-1)*4) );
		dma_dbg("REQ/ACK is 0x%08X \n", inl(PMU_FTPMU010_VA_BASE + 0x90) );
	}
	return 0;
}

static inline void fa_reset_ahb_pmu(int ch, int req_pair)
{
	if ( req_pair != 0 )
		outl(0x0, PMU_FTPMU010_VA_BASE + 0xA0+(req_pair-1)*4);
}


#endif /* __FARADAY__DMAC_FTDMAC020_H_ */

/*
 *  arch/arm/mach-GM/include/mach/platform-a320/ahb_dma.h
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
#ifndef __GM_PLATFORM_DMAC_FTDMAC020_H_
#define __GM_PLATFORM_DMAC_FTDMAC020_H_

/* Platform depedent inline funcitoin used by AHB DMA driver */

#include <asm/io.h>

/*
 * define DMA channel resource 
 */
#define AHBDMA_SSP0_RX      0
#define AHBDMA_SSP0_TX      1
#define AHBDMA_SSP1_RX      2
#define AHBDMA_SSP1_TX      3
#define AHBDMA_SSP2_RX      4
#define AHBDMA_SSP2_TX      5
#define AHBDMA_SDC          6
#define AHBDMA_NANDC        0   /* share with AHBDMA_SSP0_RX */


#endif /* __GM_PLATFORM_DMAC_FTDMAC020_H_ */

/*
 *  arch/arm/mach-faraday/include/mach/memory.h
 *  
 *  Faraday Platform Independent Memory Configuration
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
 *  Luke Lee  09/16/2005  Created.
 */

#ifndef __FARADAY_PLATFORM_INDEPENDENT_MEMORY_HEADER__
#define __FARADAY_PLATFORM_INDEPENDENT_MEMORY_HEADER__

#include <asm/sizes.h>
#include <mach/spec.h>

#ifndef PHYS_OFFSET
#define PHYS_OFFSET     CPU_MEM_PA_BASE
#endif

#ifndef END_MEM
#define END_MEM		(CPU_MEM_PA_LIMIT + 1)
#endif

#ifndef MEM_SIZE
#define MEM_SIZE	(CPU_MEM_PA_SIZE)
#endif

#ifndef __virt_to_bus
#define __virt_to_bus(x)	__virt_to_phys(x)
#endif

#ifndef __bus_to_virt
#define __bus_to_virt(x)	__phys_to_virt(x)
#endif

/*
 * Size of DMA-consistent memory region.  Must be multiple of 2M,
 * between 2MB and 14MB inclusive.
 *
 * Increase default in arch/arm/include/asm/memory.h
 */
#define CONSISTENT_DMA_SIZE SZ_8M

#endif /* __FARADAY_PLATFORM_INDEPENDENT_MEMORY_HEADER__ */

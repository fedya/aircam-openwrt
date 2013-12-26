/*
 *  arch/arm/mach-GM/include/mach/memory.h
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
 *  Harry Hsu 04/06/2010 add "isolate high memory" function
 */

#ifndef __GM_PLATFORM_INDEPENDENT_MEMORY_HEADER__
#define __GM_PLATFORM_INDEPENDENT_MEMORY_HEADER__

#include <asm/sizes.h>
#include <mach/spec.h>
#include <mach/platform/memory.h>


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
#if 0
#define CONSISTENT_DMA_SIZE SZ_8M
#endif

#define arch_adjust_zones(node, size, holes)	__arch_adjust_zones(node, size, holes)

/* memory size is 128M, #define NODE_MEM_SIZE_BITS  27 is removed, otherwise arch/arm/include/memory.h
 * will use NODE_MEM_SIZE_BITS marco
 */
#define SECTION_SIZE_BITS   27
#define MAX_PHYSMEM_BITS    32

#ifndef __ASSEMBLY__

/*
 * Because of the wide memory address space between physical RAM banks on the 
 * SA1100, it's much convenient to use Linux's NUMA support to implement our 
 * memory map representation.  Assuming all memory nodes have equal access 
 * characteristics, we then have generic discontiguous memory support.
 *
 * Of course, all this isn't mandatory for SA1100 implementations with only
 * one used memory bank.  For those, simply undefine CONFIG_DISCONTIGMEM.
 *
 * The nodes are matched with the physical memory bank addresses which are 
 * incidentally the same as virtual addresses.
 * 
 * 	node 0:  0xc0000000 - 0xc7ffffff
 * 	node 1:  0xc8000000 - 0xcfffffff
 * 	node 2:  0xd0000000 - 0xd7ffffff
 * 	node 3:  0xd8000000 - 0xdfffffff
 * 
 */
 
extern unsigned int fmem_kvaddr_to_nid(unsigned long addr);
extern unsigned int fmem_pfn_to_nid(unsigned int pfn);
extern unsigned int fmem_local_map_nr(unsigned long addr);
extern unsigned int fmem_get_ddr0_mask(void);

#define NODE_MEM_SIZE_MASK	fmem_get_ddr0_mask //((1 << NODE_MEM_SIZE_BITS) - 1)

/*
 * Given a kernel address, find the home node of the underlying memory.
 */
#define KVADDR_TO_NID(addr) fmem_kvaddr_to_nid((unsigned long)addr)    //(((unsigned long)(addr) - PAGE_OFFSET) >> 27)

/*
 * Given a page frame number, convert it to a node id.
 */
#define PFN_TO_NID(pfn)		fmem_pfn_to_nid(pfn) //(((pfn) - PHYS_PFN_OFFSET) >> (27 - PAGE_SHIFT))

/*
 * Given a kaddr, LOCAL_MEM_MAP finds the owning node of the memory
 * and returns the index corresponding to the appropriate page in the
 * node's mem_map.
 */
#define LOCAL_MAP_NR(addr) \
	fmem_local_map_nr((unsigned long)addr) //(((unsigned long)(addr) & 0x07ffffff) >> PAGE_SHIFT)
#endif /* __ASSEMBLY__ */

#endif /* __GM_PLATFORM_INDEPENDENT_MEMORY_HEADER__ */

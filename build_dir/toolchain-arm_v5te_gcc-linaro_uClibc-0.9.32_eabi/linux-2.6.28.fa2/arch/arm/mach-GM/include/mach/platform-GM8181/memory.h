/*
 *  linux/include/asm-armnommu/arch-GM/platform-GM8181/memory.h
 *  
 *  GM Platform Dependent Memory Configuration
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
 */
#ifndef __MEMORY_H__
#define __MEMORY_H__

#ifndef __ASSEMBLY__


/* The memory size for HIGMMEM_ZONE or the size for Framamp management.
 * When CONFIG_ISOLATE_HIGHMEM is defined, HIGHMEM zone is created and  managed by linux kernel. 
 * If it is not defined, HIGHMEM zone is not created by linux kernel but a memory region
 * with size HIGH_MEM_SIZE is allocated from the kernel and managed by Frammap.
 * Without CONFIG_ISOLATE_HIGHMEM, when the users call fmem_alloc() function to allocate memory, 
 * the memory will be from DMA zone instead of this region.
 *
 * Users can adjust the memory size based on the real memory size requirement. 
 * The size should be alignment to 8M, otherwise it will auto be aligned to 8M in fmem_init().
 * This definition is applied to DDR0 only.
 */
#define HIGH_MEM_SIZE 0x3600000

/* The memory size for Framamp management. Users can adjust the memory size based on the real
 * memory size requirement. The size should be alignment to 8M, otherwise it will auto be aligned
 * to 8M in fmem_init().
 * This definition is applied to any DDRs except DDR0. -1 means using the largest memory size
 */
#define FRAMMAP_MEM_SIZE -1

extern void __arch_adjust_zones(int node, unsigned long *size, unsigned long *holes);

#endif /* __ASSEMBLY__ */
#endif /* __MEMORY_H__ */


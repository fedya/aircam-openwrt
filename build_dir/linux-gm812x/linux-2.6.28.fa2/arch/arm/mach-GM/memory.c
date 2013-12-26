/*
 *  Copyright (C) 2009 Faraday Technology.
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
 * Change Log:
 *      Isolate a high memory block from Bank 0 
 */
#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/mmzone.h>
#include <linux/bootmem.h>
#include <linux/mman.h>
#include <linux/pfn.h>
#include <linux/nodemask.h>
#include <mach/memory.h>
#include <mach/fmem.h>

/* 0: ZONE_DMA
 * 1: ZONE_NORMAL
 * 2: ZONE_HIGH
 */
void __init __arch_adjust_zones(int node, unsigned long *size, unsigned long *holes)
{    
    /* there is only memory zone which is NORMAL ZONE */
}
/*
 *  arch/arm/mach-faraday/include/mach/io.h
 *  
 *  Faraday Platform Independent IO Definition
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
 *  ChangeLog
 *  
 *   Luke Lee 09/16/2005 Created.
 */

#ifndef __FARADAY_PLATFORM_IO_HEADER__
#define __FARADAY_PLATFORM_IO_HEADER__

#include <linux/compiler.h>

#ifndef IO_SPACE_LIMIT
#define IO_SPACE_LIMIT      0xffffffff
#endif

#ifndef __io
#define __io(a)             ((void __iomem *)(a))
#endif

#ifndef __mem_pci
#define __mem_pci(a)        (a)
#endif

#endif

/*
 *  arch/arm/mach-faraday/include/mach/hardware.h
 *  
 *  Faraday Platform Independent Hardware Configuration
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
 *  Peter Liao 10/04/2005 Modified for uClinux
 */

#ifndef __FARADAY_PLATFORM_HARDWARE_HEADER__
#define __FARADAY_PLATFORM_HARDWARE_HEADER__

#include <mach/spec.h>

#ifndef PCIBIOS_MIN_IO
/* the mini io address is 0x6000,that is IO will allocate from 0-0x6000 offset*/
#define PCIBIOS_MIN_IO	0x0
#endif

#ifndef PCIBIOS_MIN_MEM
/* the mini MEM address is 0x100000,that is MEM will allocate from 0-0x100000 offset*/
#define PCIBIOS_MIN_MEM	0x0
#endif

#define pcibios_assign_all_busses()	1 

#endif /* __FARADAY_PLATFORM_HARDWARE_HEADER__ */

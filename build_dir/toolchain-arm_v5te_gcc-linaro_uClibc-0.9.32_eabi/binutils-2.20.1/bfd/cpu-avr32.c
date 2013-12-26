/* BFD library support routines for AVR32.
   Copyright 2003,2004,2005,2006,2007,2008,2009 Atmel Corporation.

   Written by Haavard Skinnemoen, Atmel Norway, <hskinnemoen@atmel.com>

   This is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "bfd.h"
#include "sysdep.h"
#include "libbfd.h"

#define N(machine, print, default, next)			\
  {								\
    32,				/* 32 bits in a word */		\
    32,				/* 32 bits in an address */	\
    8,				/* 8 bits in a byte */		\
    bfd_arch_avr32,		/* architecture */		\
    machine,			/* machine */			\
    "avr32",			/* arch name */			\
    print,			/* printable name */		\
    1,				/* section align power */	\
    default,			/* the default machine? */	\
    bfd_default_compatible,					\
    bfd_default_scan,						\
    next,							\
  }

static const bfd_arch_info_type cpu_info[] =
{
  N(bfd_mach_avr32_ap, "avr32:ap", FALSE, &cpu_info[1]),
  N(bfd_mach_avr32_uc, "avr32:uc", FALSE, &cpu_info[2]),
  N(bfd_mach_avr32_ucr1, "avr32:ucr1", FALSE, &cpu_info[3]),
  N(bfd_mach_avr32_ucr2, "avr32:ucr2", FALSE, &cpu_info[4]),
  N(bfd_mach_avr32_ucr3, "avr32:ucr3", FALSE, NULL),
};

const bfd_arch_info_type bfd_avr32_arch =
  N(bfd_mach_avr32_ap, "avr32", TRUE, &cpu_info[0]);

/* AVR32 ELF support for BFD.
   Copyright 2003,2004,2005,2006,2007,2008,2009 Atmel Corporation.

   Written by Haavard Skinnemoen, Atmel Norway, <hskinnemoen@atmel.com>

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#include "elf/reloc-macros.h"

/* CPU-specific flags for the ELF header e_flags field */
#define EF_AVR32_LINKRELAX		0x01
#define EF_AVR32_PIC			0x02

START_RELOC_NUMBERS (elf_avr32_reloc_type)
    RELOC_NUMBER (R_AVR32_NONE,			0)

    /* Data Relocations */
    RELOC_NUMBER (R_AVR32_32,			1)
    RELOC_NUMBER (R_AVR32_16,			2)
    RELOC_NUMBER (R_AVR32_8,			3)
    RELOC_NUMBER (R_AVR32_32_PCREL,		4)
    RELOC_NUMBER (R_AVR32_16_PCREL,		5)
    RELOC_NUMBER (R_AVR32_8_PCREL,		6)
    RELOC_NUMBER (R_AVR32_DIFF32,		7)
    RELOC_NUMBER (R_AVR32_DIFF16,		8)
    RELOC_NUMBER (R_AVR32_DIFF8,		9)
    RELOC_NUMBER (R_AVR32_GOT32,		10)
    RELOC_NUMBER (R_AVR32_GOT16,		11)
    RELOC_NUMBER (R_AVR32_GOT8,			12)

    /* Normal Code Relocations */
    RELOC_NUMBER (R_AVR32_21S,			13)
    RELOC_NUMBER (R_AVR32_16U,			14)
    RELOC_NUMBER (R_AVR32_16S,			15)
    RELOC_NUMBER (R_AVR32_8S,			16)
    RELOC_NUMBER (R_AVR32_8S_EXT,		17)

    /* PC-Relative Code Relocations */
    RELOC_NUMBER (R_AVR32_22H_PCREL,		18)
    RELOC_NUMBER (R_AVR32_18W_PCREL,		19)
    RELOC_NUMBER (R_AVR32_16B_PCREL,		20)
    RELOC_NUMBER (R_AVR32_16N_PCREL,		21)
    RELOC_NUMBER (R_AVR32_14UW_PCREL,		22)
    RELOC_NUMBER (R_AVR32_11H_PCREL,		23)
    RELOC_NUMBER (R_AVR32_10UW_PCREL,		24)
    RELOC_NUMBER (R_AVR32_9H_PCREL,		25)
    RELOC_NUMBER (R_AVR32_9UW_PCREL,		26)

    /* Special Code Relocations */
    RELOC_NUMBER (R_AVR32_HI16,			27)
    RELOC_NUMBER (R_AVR32_LO16,			28)

    /* PIC Relocations */
    RELOC_NUMBER (R_AVR32_GOTPC,		29)
    RELOC_NUMBER (R_AVR32_GOTCALL,		30)
    RELOC_NUMBER (R_AVR32_LDA_GOT,		31)
    RELOC_NUMBER (R_AVR32_GOT21S,		32)
    RELOC_NUMBER (R_AVR32_GOT18SW,		33)
    RELOC_NUMBER (R_AVR32_GOT16S,		34)
    RELOC_NUMBER (R_AVR32_GOT7UW,		35)

    /* Constant Pool Relocations */
    RELOC_NUMBER (R_AVR32_32_CPENT,		36)
    RELOC_NUMBER (R_AVR32_CPCALL,		37)
    RELOC_NUMBER (R_AVR32_16_CP,		38)
    RELOC_NUMBER (R_AVR32_9W_CP,		39)

    /* Dynamic Relocations */
    RELOC_NUMBER (R_AVR32_RELATIVE,		40)
    RELOC_NUMBER (R_AVR32_GLOB_DAT,		41)
    RELOC_NUMBER (R_AVR32_JMP_SLOT,		42)

    /* Linkrelax Information */
    RELOC_NUMBER (R_AVR32_ALIGN,		43)

    RELOC_NUMBER (R_AVR32_15S,		        44)

END_RELOC_NUMBERS (R_AVR32_max)

/* Processor specific dynamic array tags.  */

/* The total size in bytes of the Global Offset Table */
#define DT_AVR32_GOTSZ			0x70000001

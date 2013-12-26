/* Opcode tables for AVR32.
   Copyright 2005,2006,2007,2008,2009 Atmel Corporation.

   Written by Haavard Skinnemoen, Atmel Norway, <hskinnemoen@atmel.com>

   This file is part of libopcodes.

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

#include <stdlib.h>
#include <assert.h>

#include "avr32-opc.h"

#define PICO_CPNO	1

void
avr32_insert_simple(const struct avr32_ifield *field,
		    void *buf, unsigned long value)
{
  bfd_vma word;

  word = bfd_getb32(buf);
  word &= ~field->mask;
  word |= (value << field->shift) & field->mask;
  bfd_putb32(word, buf);
}

void
avr32_insert_bit5c(const struct avr32_ifield *field ATTRIBUTE_UNUSED,
		   void *buf, unsigned long value)
{
  char *opcode = buf;

  opcode[0] = (opcode[0] & 0xe1) | (value & 0x1e);
  opcode[1] = (opcode[1] & 0xef) | ((value & 1) << 4);
}

void
avr32_insert_k10(const struct avr32_ifield *field ATTRIBUTE_UNUSED,
		 void *buf, unsigned long value)
{
  char *opcode = buf;

  opcode[0] = (opcode[0] & 0xf0) | ((value & 0xf0) >> 4);
  opcode[1] = ((opcode[1] & 0x0c) | ((value & 0x0f) << 4)
	       | ((value & 0x300) >> 8));
}


void
avr32_insert_k21(const struct avr32_ifield *field,
		 void *buf, unsigned long value)
{
  bfd_vma word;
  bfd_vma k21;

  word = bfd_getb32(buf);
  word &= ~field->mask;
  k21 = ((value & 0xffff) | ((value & 0x10000) << 4)
	 | ((value & 0x1e0000) << 8));
  assert(!(k21 & ~field->mask));
  word |= k21;
  bfd_putb32(word, buf);
}

void
avr32_insert_cpop(const struct avr32_ifield *field,
		  void *buf, unsigned long value)
{
  bfd_vma word;

  word = bfd_getb32(buf);
  word &= ~field->mask;
  word |= (((value & 0x1e) << 15) | ((value & 0x60) << 20)
	   | ((value & 0x01) << 12));
  bfd_putb32(word, buf);
}

void
avr32_insert_k12cp(const struct avr32_ifield *field,
		   void *buf, unsigned long value)
{
  bfd_vma word;

  word = bfd_getb32(buf);
  word &= ~field->mask;
  word |= ((value & 0xf00) << 4) | (value & 0xff);
  bfd_putb32(word, buf);
}

void avr32_extract_simple(const struct avr32_ifield *field,
			  void *buf, unsigned long *value)
{
  /* XXX: The disassembler has done any necessary byteswapping already */
  bfd_vma word = *(bfd_vma *)buf;

  *value = (word & field->mask) >> field->shift;
}

void avr32_extract_bit5c(const struct avr32_ifield *field ATTRIBUTE_UNUSED,
			 void *buf, unsigned long *value)
{
  bfd_vma word = *(bfd_vma *)buf;

  *value = ((word >> 20) & 1) | ((word >> 24) & 0x1e);
}

void avr32_extract_k10(const struct avr32_ifield *field ATTRIBUTE_UNUSED,
		       void *buf, unsigned long *value)
{
  bfd_vma word = *(bfd_vma *)buf;

  *value = ((word >> 8) & 0x300) | ((word >> 20) & 0xff);
}

void avr32_extract_k21(const struct avr32_ifield *field ATTRIBUTE_UNUSED,
		       void *buf, unsigned long *value)
{
  bfd_vma word = *(bfd_vma *)buf;

  *value = ((word & 0xffff) | ((word >> 4) & 0x10000)
	    | ((word >> 8) & 0x1e0000));
}

void avr32_extract_cpop(const struct avr32_ifield *field ATTRIBUTE_UNUSED,
			void *buf, unsigned long *value)
{
  bfd_vma word = *(bfd_vma *)buf;

  *value = (((word >> 12) & 1) | ((word >> 15) & 0x1e)
	    | ((word >> 20) & 0x60));
}

void avr32_extract_k12cp(const struct avr32_ifield *field ATTRIBUTE_UNUSED,
			 void *buf, unsigned long *value)
{
  bfd_vma word = *(bfd_vma *)buf;

  *value = ((word >> 4) & 0xf00) | (word & 0xff);
}


#define IFLD(id, bitsz, shift, mask, func) \
  { AVR32_IFIELD_##id, bitsz, shift, mask, \
    avr32_insert_##func, avr32_extract_##func }

const struct avr32_ifield avr32_ifield_table[] =
  {
    IFLD(RX, 4, 25, 0x1e000000, simple),
    IFLD(RY, 4, 16, 0x000f0000, simple),
    IFLD(COND4C, 4, 20, 0x00f00000, simple),
    IFLD(K8C, 8, 20, 0x0ff00000, simple),
    IFLD(K7C, 7, 20, 0x07f00000, simple),
    IFLD(K5C, 5, 20, 0x01f00000, simple),
    IFLD(K3, 3, 20, 0x00700000, simple),
    IFLD(RY_DW, 3, 17, 0x000e0000, simple),
    IFLD(COND4E, 4, 8, 0x00000f00, simple),
    IFLD(K8E, 8, 0, 0x000000ff, simple),
    IFLD(BIT5C, 5, 20, 0x1e100000, bit5c),
    IFLD(COND3, 3, 16, 0x00070000, simple),
    IFLD(K10, 10, 16, 0x0ff30000, k10),
    IFLD(POPM, 9, 19, 0x0ff80000, simple),
    IFLD(K2, 2, 4, 0x00000030, simple),
    IFLD(RD_E, 4, 0, 0x0000000f, simple),
    IFLD(RD_DW, 3, 1, 0x0000000e, simple),
    IFLD(X, 1, 5, 0x00000020, simple),
    IFLD(Y, 1, 4, 0x00000010, simple),
    IFLD(X2, 1, 13, 0x00002000, simple),
    IFLD(Y2, 1, 12, 0x00001000, simple),
    IFLD(K5E, 5, 0, 0x0000001f, simple),
    IFLD(PART2, 2, 0, 0x00000003, simple),
    IFLD(PART1, 1, 0, 0x00000001, simple),
    IFLD(K16, 16, 0, 0x0000ffff, simple),
    IFLD(CACHEOP, 5, 11, 0x0000f800, simple),
    IFLD(K11, 11, 0, 0x000007ff, simple),
    IFLD(K21, 21, 0, 0x1e10ffff, k21),
    IFLD(CPOP, 7, 12, 0x060f1000, cpop),
    IFLD(CPNO, 3, 13, 0x0000e000, simple),
    IFLD(CRD_RI, 4, 8, 0x00000f00, simple),
    IFLD(CRX, 4, 4, 0x000000f0, simple),
    IFLD(CRY, 4, 0, 0x0000000f, simple),
    IFLD(K7E, 7, 0, 0x0000007f, simple),
    IFLD(CRD_DW, 3, 9, 0x00000e00, simple),
    IFLD(PART1_K12, 1, 12, 0x00001000, simple),
    IFLD(PART2_K12, 2, 12, 0x00003000, simple),
    IFLD(K12, 12, 0, 0x00000fff, simple),
    IFLD(S5, 5, 5, 0x000003e0, simple),
    IFLD(K5E2, 5, 4, 0x000001f0, simple),
    IFLD(K4, 4, 20, 0x00f00000, simple),
    IFLD(COND4E2, 4, 4, 0x000000f0, simple),
    IFLD(K8E2, 8, 4, 0x00000ff0, simple),
    IFLD(K6, 6, 20, 0x03f00000, simple),
    IFLD(MEM15, 15, 0, 0x00007fff, simple),
    IFLD(MEMB5, 5, 15, 0x000f8000, simple),
    IFLD(W, 1, 25, 0x02000000, simple),
    /* Coprocessor Multiple High/Low */
    IFLD(CM_HL, 1, 8, 0x00000100, simple),
    IFLD(K12CP, 12 ,0, 0x0000f0ff, k12cp),
    IFLD(K9E, 9 ,0, 0x000001ff, simple),
    IFLD (FP_RX, 4,  4, 0x000000F0, simple),
    IFLD (FP_RY, 4,  0, 0x0000000F, simple),
    IFLD (FP_RD, 4,  8, 0x00000F00, simple),
    IFLD (FP_RA, 4, 16, 0x000F0000, simple) 
  };
#undef IFLD


struct avr32_opcode avr32_opc_table[] =
  {
    {
      AVR32_OPC_ABS, 2, 0x5c400000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_ABS],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      }
    },
    {
      AVR32_OPC_ACALL, 2, 0xd0000000, 0xf00f0000,
      &avr32_syntax_table[AVR32_SYNTAX_ACALL],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_ACR, 2, 0x5c000000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_ACR],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ADC, 4, 0xe0000040, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_ADC],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ADD1, 2, 0x00000000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_ADD1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_ADD2, 4, 0xe0000000, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_ADD2],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
      },
    },
    {
      AVR32_OPC_ADDABS, 4, 0xe0000e40, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_ADDABS],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ADDHH_W, 4, 0xe0000e00, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_ADDHH_W],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_AND1, 2, 0x00600000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_AND1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_AND2, 4, 0xe1e00000, 0xe1f0fe00,
      &avr32_syntax_table[AVR32_SYNTAX_AND2],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K5E2],
      },
    },
    {
      AVR32_OPC_AND3, 4, 0xe1e00200, 0xe1f0fe00,
      &avr32_syntax_table[AVR32_SYNTAX_AND3],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K5E2],
      },
    },
    {
      AVR32_OPC_ANDH, 4, 0xe4100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_ANDH],
      BFD_RELOC_AVR32_16U, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_ANDH_COH, 4, 0xe6100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_ANDH_COH],
      BFD_RELOC_AVR32_16U, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_ANDL, 4, 0xe0100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_ANDL],
      BFD_RELOC_AVR32_16U, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_ANDL_COH, 4, 0xe2100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_ANDL_COH],
      BFD_RELOC_AVR32_16U, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_ANDN, 2, 0x00800000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_ANDN],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_ASR1, 4, 0xe0000840, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_ASR1],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ASR3, 4, 0xe0001400, 0xe1f0ffe0,
      &avr32_syntax_table[AVR32_SYNTAX_ASR3],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K5E],
      },
    },
    {
      AVR32_OPC_ASR2, 2, 0xa1400000, 0xe1e00000,
      &avr32_syntax_table[AVR32_SYNTAX_ASR2],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_BIT5C],
      },
    },
    {
      AVR32_OPC_BLD, 4, 0xedb00000, 0xfff0ffe0,
      &avr32_syntax_table[AVR32_SYNTAX_BLD],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K5E],
      },
    },
    {
      AVR32_OPC_BREQ1, 2, 0xc0000000, 0xf00f0000,
      &avr32_syntax_table[AVR32_SYNTAX_BREQ1],
      BFD_RELOC_AVR32_9H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_BRNE1, 2, 0xc0010000, 0xf00f0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRNE1],
      BFD_RELOC_AVR32_9H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_BRCC1, 2, 0xc0020000, 0xf00f0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRCC1],
      BFD_RELOC_AVR32_9H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_BRCS1, 2, 0xc0030000, 0xf00f0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRCS1],
      BFD_RELOC_AVR32_9H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_BRGE1, 2, 0xc0040000, 0xf00f0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRGE1],
      BFD_RELOC_AVR32_9H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_BRLT1, 2, 0xc0050000, 0xf00f0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRLT1],
      BFD_RELOC_AVR32_9H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_BRMI1, 2, 0xc0060000, 0xf00f0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRMI1],
      BFD_RELOC_AVR32_9H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_BRPL1, 2, 0xc0070000, 0xf00f0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRPL1],
      BFD_RELOC_AVR32_9H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_BREQ2, 4, 0xe0800000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BREQ2],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRNE2, 4, 0xe0810000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRNE2],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRCC2, 4, 0xe0820000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRHS2],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRCS2, 4, 0xe0830000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRLO2],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRGE2, 4, 0xe0840000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRGE2],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRLT2, 4, 0xe0850000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRLT2],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRMI2, 4, 0xe0860000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRMI2],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRPL2, 4, 0xe0870000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRPL2],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRLS, 4, 0xe0880000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRLS],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRGT, 4, 0xe0890000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRGT],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRLE, 4, 0xe08a0000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRLE],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRHI, 4, 0xe08b0000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRHI],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRVS, 4, 0xe08c0000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRVS],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRVC, 4, 0xe08d0000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRVC],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRQS, 4, 0xe08e0000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRQS],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BRAL, 4, 0xe08f0000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_BRAL],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_BREAKPOINT, 2, 0xd6730000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_BREAKPOINT],
      BFD_RELOC_UNUSED, 0, -1, { NULL },
    },
    {
      AVR32_OPC_BREV, 2, 0x5c900000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_BREV],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_BST, 4, 0xefb00000, 0xfff0ffe0,
      &avr32_syntax_table[AVR32_SYNTAX_BST],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K5E],
      },
    },
    {
      AVR32_OPC_CACHE, 4, 0xf4100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_CACHE],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K11],
	&avr32_ifield_table[AVR32_IFIELD_CACHEOP],
      },
    },
    {
      AVR32_OPC_CASTS_B, 2, 0x5c600000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_CASTS_B],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_CASTS_H, 2, 0x5c800000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_CASTS_H],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_CASTU_B, 2, 0x5c500000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_CASTU_B],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_CASTU_H, 2, 0x5c700000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_CASTU_H],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_CBR, 2, 0xa1c00000, 0xe1e00000,
      &avr32_syntax_table[AVR32_SYNTAX_CBR],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_BIT5C],
      },
    },
    {
      AVR32_OPC_CLZ, 4, 0xe0001200, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_CLZ],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_COM, 2, 0x5cd00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_COM],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_COP, 4, 0xe1a00000, 0xf9f00000,
      &avr32_syntax_table[AVR32_SYNTAX_COP],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_CRD_RI],
	&avr32_ifield_table[AVR32_IFIELD_CRX],
	&avr32_ifield_table[AVR32_IFIELD_CRY],
	&avr32_ifield_table[AVR32_IFIELD_CPOP],
      },
    },
    {
      AVR32_OPC_CP_B, 4, 0xe0001800, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_CP_B],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_CP_H, 4, 0xe0001900, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_CP_H],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_CP_W1, 2, 0x00300000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_CP_W1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_CP_W2, 2, 0x58000000, 0xfc000000,
      &avr32_syntax_table[AVR32_SYNTAX_CP_W2],
      BFD_RELOC_AVR32_6S, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K6],
      },
    },
    {
      AVR32_OPC_CP_W3, 4, 0xe0400000, 0xe1e00000,
      &avr32_syntax_table[AVR32_SYNTAX_CP_W3],
      BFD_RELOC_AVR32_21S, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_CPC1, 4, 0xe0001300, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_CPC1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_CPC2, 2, 0x5c200000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_CPC2],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_CSRF, 2, 0xd4030000, 0xfe0f0000,
      &avr32_syntax_table[AVR32_SYNTAX_CSRF],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_K5C],
      },
    },
    {
      AVR32_OPC_CSRFCZ, 2, 0xd0030000, 0xfe0f0000,
      &avr32_syntax_table[AVR32_SYNTAX_CSRFCZ],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_K5C],
      },
    },
    {
      AVR32_OPC_DIVS, 4, 0xe0000c00, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_DIVS],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_DIVU, 4, 0xe0000d00, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_DIVU],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_EOR1, 2, 0x00500000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_EOR1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_EOR2, 4, 0xe1e02000, 0xe1f0fe00,
      &avr32_syntax_table[AVR32_SYNTAX_EOR2],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K5E2],
      }
    },
    {
      AVR32_OPC_EOR3, 4, 0xe1e02200, 0xe1f0fe00,
      &avr32_syntax_table[AVR32_SYNTAX_EOR3],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K5E2],
      }
    },
    {
      AVR32_OPC_EORL, 4, 0xec100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_EORL],
      BFD_RELOC_AVR32_16U, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_EORH, 4, 0xee100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_EORH],
      BFD_RELOC_AVR32_16U, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_FRS, 2, 0xd7430000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_FRS],
      BFD_RELOC_UNUSED, 0, -1, { NULL },
    },
    {
      AVR32_OPC_ICALL, 2, 0x5d100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_ICALL],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_INCJOSP, 2, 0xd6830000, 0xff8f0000,
      &avr32_syntax_table[AVR32_SYNTAX_INCJOSP],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_K3],
      },
    },
    {
      AVR32_OPC_LD_D1, 2, 0xa1010000, 0xe1f10000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_D1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY_DW],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_LD_D2, 2, 0xa1100000, 0xe1f10000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_D2],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY_DW],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_LD_D3, 2, 0xa1000000, 0xe1f10000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_D3],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY_DW],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_LD_D5, 4, 0xe0000200, 0xe1f0ffc1,
      &avr32_syntax_table[AVR32_SYNTAX_LD_D5],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_DW],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
      },
    },
    {
      AVR32_OPC_LD_D4, 4, 0xe0e00000, 0xe1f10000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_D4],
      BFD_RELOC_AVR32_16S, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY_DW],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_LD_SB2, 4, 0xe0000600, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_LD_SB2],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
      },
    },
    {
      AVR32_OPC_LD_SB1, 4, 0xe1200000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_SB1],
      BFD_RELOC_AVR32_16S, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_LD_UB1, 2, 0x01300000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_UB1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_LD_UB2, 2, 0x01700000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_UB2],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_LD_UB5, 4, 0xe0000700, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_LD_UB5],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
      },
    },
    {
      AVR32_OPC_LD_UB3, 2, 0x01800000, 0xe1800000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_UB3],
      BFD_RELOC_AVR32_3U, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K3],
      },
    },
    {
      AVR32_OPC_LD_UB4, 4, 0xe1300000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_UB4],
      BFD_RELOC_AVR32_16S, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_LD_SH1, 2, 0x01100000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_SH1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_LD_SH2, 2, 0x01500000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_SH2],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_LD_SH5, 4, 0xe0000400, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_LD_SH5],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
      },
    },
    {
      AVR32_OPC_LD_SH3, 2, 0x80000000, 0xe1800000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_SH3],
      BFD_RELOC_AVR32_4UH, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K3],
      },
    },
    {
      AVR32_OPC_LD_SH4, 4, 0xe1000000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_SH4],
      BFD_RELOC_AVR32_16S, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_LD_UH1, 2, 0x01200000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_UH1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_LD_UH2, 2, 0x01600000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_UH2],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_LD_UH5, 4, 0xe0000500, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_LD_UH5],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
      },
    },
    {
      AVR32_OPC_LD_UH3, 2, 0x80800000, 0xe1800000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_UH3],
      BFD_RELOC_AVR32_4UH, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K3],
      },
    },
    {
      AVR32_OPC_LD_UH4, 4, 0xe1100000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_UH4],
      BFD_RELOC_AVR32_16S, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_LD_W1, 2, 0x01000000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_W1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_LD_W2, 2, 0x01400000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_W2],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_LD_W5, 4, 0xe0000300, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_LD_W5],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
      },
    },
    {
      AVR32_OPC_LD_W6, 4, 0xe0000f80, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_LD_W6],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
      },
    },
    {
      AVR32_OPC_LD_W3, 2, 0x60000000, 0xe0000000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_W3],
      BFD_RELOC_AVR32_7UW, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K5C],
      },
    },
    {
      AVR32_OPC_LD_W4, 4, 0xe0f00000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_LD_W4],
      BFD_RELOC_AVR32_16S, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_LDC_D1, 4, 0xe9a01000, 0xfff01100,
      &avr32_syntax_table[AVR32_SYNTAX_LDC_D1],
      BFD_RELOC_AVR32_10UW, 4, 3,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_CRD_DW],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_LDC_D2, 4, 0xefa00050, 0xfff011ff,
      &avr32_syntax_table[AVR32_SYNTAX_LDC_D2],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_CRD_DW],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_LDC_D3, 4, 0xefa01040, 0xfff011c0,
      &avr32_syntax_table[AVR32_SYNTAX_LDC_D3],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_CRD_DW],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_K2],
      },
    },
    {
      AVR32_OPC_LDC_W1, 4, 0xe9a00000, 0xfff01000,
      &avr32_syntax_table[AVR32_SYNTAX_LDC_W1],
      BFD_RELOC_AVR32_10UW, 4, 3,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_CRD_RI],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_LDC_W2, 4, 0xefa00040, 0xfff010ff,
      &avr32_syntax_table[AVR32_SYNTAX_LDC_W2],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_CRD_RI],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_LDC_W3, 4, 0xefa01000, 0xfff010c0,
      &avr32_syntax_table[AVR32_SYNTAX_LDC_W3],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_CRD_RI],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_K2],
      },
    },
    {
      AVR32_OPC_LDC0_D, 4, 0xf3a00000, 0xfff00100,
      &avr32_syntax_table[AVR32_SYNTAX_LDC0_D],
      BFD_RELOC_AVR32_14UW, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_CRD_DW],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K12CP],
      },
    },
    {
      AVR32_OPC_LDC0_W, 4, 0xf1a00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_LDC0_W],
      BFD_RELOC_AVR32_14UW, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_CRD_RI],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K12CP],
      },
    },
    {
      AVR32_OPC_LDCM_D, 4, 0xeda00400, 0xfff01f00,
      &avr32_syntax_table[AVR32_SYNTAX_LDCM_D],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_LDCM_D_PU, 4, 0xeda01400, 0xfff01f00,
      &avr32_syntax_table[AVR32_SYNTAX_LDCM_D_PU],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_LDCM_W, 4, 0xeda00000, 0xfff01e00,
      &avr32_syntax_table[AVR32_SYNTAX_LDCM_W],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
	&avr32_ifield_table[AVR32_IFIELD_CM_HL],
      },
    },
    {
      AVR32_OPC_LDCM_W_PU, 4, 0xeda01000, 0xfff01e00,
      &avr32_syntax_table[AVR32_SYNTAX_LDCM_W_PU],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
	&avr32_ifield_table[AVR32_IFIELD_CM_HL],
      },
    },
    {
      AVR32_OPC_LDDPC, 2, 0x48000000, 0xf8000000,
      &avr32_syntax_table[AVR32_SYNTAX_LDDPC],
      BFD_RELOC_AVR32_9UW_PCREL, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K7C],
      },
    },
    {
      AVR32_OPC_LDDPC_EXT, 4, 0xfef00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_LDDPC_EXT],
      BFD_RELOC_AVR32_16B_PCREL, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_LDDSP, 2, 0x40000000, 0xf8000000,
      &avr32_syntax_table[AVR32_SYNTAX_LDDSP],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K7C],
      },
    },
    {
      AVR32_OPC_LDINS_B, 4, 0xe1d04000, 0xe1f0c000,
      &avr32_syntax_table[AVR32_SYNTAX_LDINS_B],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_PART2_K12],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K12],
      },
    },
    {
      AVR32_OPC_LDINS_H, 4, 0xe1d00000, 0xe1f0e000,
      &avr32_syntax_table[AVR32_SYNTAX_LDINS_H],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_PART1_K12],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K12],
      },
    },
    {
      AVR32_OPC_LDM, 4, 0xe1c00000, 0xfdf00000,
      &avr32_syntax_table[AVR32_SYNTAX_LDM],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_W],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_LDMTS, 4, 0xe5c00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_LDMTS],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_LDMTS_PU, 4, 0xe7c00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_LDMTS_PU],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_LDSWP_SH, 4, 0xe1d02000, 0xe1f0f000,
      &avr32_syntax_table[AVR32_SYNTAX_LDSWP_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K12],
      },
    },
    {
      AVR32_OPC_LDSWP_UH, 4, 0xe1d03000, 0xe1f0f000,
      &avr32_syntax_table[AVR32_SYNTAX_LDSWP_UH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K12],
      },
    },
    {
      AVR32_OPC_LDSWP_W, 4, 0xe1d08000, 0xe1f0f000,
      &avr32_syntax_table[AVR32_SYNTAX_LDSWP_W],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K12],
      },
    },
    {
      AVR32_OPC_LSL1, 4, 0xe0000940, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_LSL1],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_LSL3, 4, 0xe0001500, 0xe1f0ffe0,
      &avr32_syntax_table[AVR32_SYNTAX_LSL3],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K5E],
      },
    },
    {
      AVR32_OPC_LSL2, 2, 0xa1600000, 0xe1e00000,
      &avr32_syntax_table[AVR32_SYNTAX_LSL2],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_BIT5C],
      },
    },
    {
      AVR32_OPC_LSR1, 4, 0xe0000a40, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_LSR1],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_LSR3, 4, 0xe0001600, 0xe1f0ffe0,
      &avr32_syntax_table[AVR32_SYNTAX_LSR3],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K5E],
      },
    },
    {
      AVR32_OPC_LSR2, 2, 0xa1800000, 0xe1e00000,
      &avr32_syntax_table[AVR32_SYNTAX_LSR2],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_BIT5C],
      },
    },
    {
      AVR32_OPC_MAC, 4, 0xe0000340, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_MAC],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_MACHH_D, 4, 0xe0000580, 0xe1f0ffc1,
      &avr32_syntax_table[AVR32_SYNTAX_MACHH_D],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MACHH_W, 4, 0xe0000480, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_MACHH_W],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MACS_D, 4, 0xe0000540, 0xe1f0fff1,
      &avr32_syntax_table[AVR32_SYNTAX_MACS_D],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_MACSATHH_W, 4, 0xe0000680, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_MACSATHH_W],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MACUD, 4, 0xe0000740, 0xe1f0fff1,
      &avr32_syntax_table[AVR32_SYNTAX_MACUD],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_MACWH_D, 4, 0xe0000c80, 0xe1f0ffe1,
      &avr32_syntax_table[AVR32_SYNTAX_MACWH_D],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MAX, 4, 0xe0000c40, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_MAX],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_MCALL, 4, 0xf0100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_MCALL],
      BFD_RELOC_AVR32_18W_PCREL, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_MFDR, 4, 0xe5b00000, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MFDR],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MFSR, 4, 0xe1b00000, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MFSR],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MIN, 4, 0xe0000d40, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_MIN],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_MOV3, 2, 0x00900000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_MOV3],
      BFD_RELOC_NONE, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOV1, 2, 0x30000000, 0xf0000000,
      &avr32_syntax_table[AVR32_SYNTAX_MOV1],
      BFD_RELOC_AVR32_8S, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_MOV2, 4, 0xe0600000, 0xe1e00000,
      &avr32_syntax_table[AVR32_SYNTAX_MOV2],
      BFD_RELOC_AVR32_21S, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_MOVEQ1, 4, 0xe0001700, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVEQ1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVNE1, 4, 0xe0001710, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVNE1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVCC1, 4, 0xe0001720, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVHS1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVCS1, 4, 0xe0001730, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVLO1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVGE1, 4, 0xe0001740, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVGE1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVLT1, 4, 0xe0001750, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVLT1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVMI1, 4, 0xe0001760, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVMI1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVPL1, 4, 0xe0001770, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVPL1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVLS1, 4, 0xe0001780, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVLS1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVGT1, 4, 0xe0001790, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVGT1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVLE1, 4, 0xe00017a0, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVLE1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVHI1, 4, 0xe00017b0, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVHI1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVVS1, 4, 0xe00017c0, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVVS1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVVC1, 4, 0xe00017d0, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVVC1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVQS1, 4, 0xe00017e0, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVQS1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVAL1, 4, 0xe00017f0, 0xe1f0ffff,
      &avr32_syntax_table[AVR32_SYNTAX_MOVAL1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MOVEQ2, 4, 0xf9b00000, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVEQ2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVNE2, 4, 0xf9b00100, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVNE2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVCC2, 4, 0xf9b00200, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVHS2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVCS2, 4, 0xf9b00300, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVLO2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVGE2, 4, 0xf9b00400, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVGE2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVLT2, 4, 0xf9b00500, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVLT2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVMI2, 4, 0xf9b00600, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVMI2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVPL2, 4, 0xf9b00700, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVPL2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVLS2, 4, 0xf9b00800, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVLS2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVGT2, 4, 0xf9b00900, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVGT2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVLE2, 4, 0xf9b00a00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVLE2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVHI2, 4, 0xf9b00b00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVHI2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVVS2, 4, 0xf9b00c00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVVS2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVVC2, 4, 0xf9b00d00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVVC2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVQS2, 4, 0xf9b00e00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVQS2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MOVAL2, 4, 0xf9b00f00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MOVAL2],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MTDR, 4, 0xe7b00000, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MTDR],
      BFD_RELOC_AVR32_8S_EXT, 2, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8E],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_MTSR, 4, 0xe3b00000, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MTSR],
      BFD_RELOC_AVR32_8S_EXT, 2, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8E],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_MUL1, 2, 0xa1300000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_MUL1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_MUL2, 4, 0xe0000240, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_MUL2],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_MUL3, 4, 0xe0001000, 0xe1f0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_MUL3],
      BFD_RELOC_AVR32_8S_EXT, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_MULHH_W, 4, 0xe0000780, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_MULHH_W],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MULNHH_W, 4, 0xe0000180, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_MULNHH_W],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MULNWH_D, 4, 0xe0000280, 0xe1f0ffe1,
      &avr32_syntax_table[AVR32_SYNTAX_MULNWH_D],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MULSD, 4, 0xe0000440, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_MULSD],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_MULSATHH_H, 4, 0xe0000880, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_MULSATHH_H],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MULSATHH_W, 4, 0xe0000980, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_MULSATHH_W],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MULSATRNDHH_H, 4, 0xe0000a80, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_MULSATRNDHH_H],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MULSATRNDWH_W, 4, 0xe0000b80, 0xe1f0ffe0,
      &avr32_syntax_table[AVR32_SYNTAX_MULSATRNDWH_W],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MULSATWH_W, 4, 0xe0000e80, 0xe1f0ffe0,
      &avr32_syntax_table[AVR32_SYNTAX_MULSATWH_W],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MULU_D, 4, 0xe0000640, 0xe1f0fff1,
      &avr32_syntax_table[AVR32_SYNTAX_MULU_D],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_MULWH_D, 4, 0xe0000d80, 0xe1f0ffe1,
      &avr32_syntax_table[AVR32_SYNTAX_MULWH_D],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_MUSFR, 2, 0x5d300000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_MUSFR],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      }
    },
    {
      AVR32_OPC_MUSTR, 2, 0x5d200000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_MUSTR],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      }
    },
    {
      AVR32_OPC_MVCR_D, 4, 0xefa00010, 0xfff111ff,
      &avr32_syntax_table[AVR32_SYNTAX_MVCR_D],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY_DW],
	&avr32_ifield_table[AVR32_IFIELD_CRD_DW],
      },
    },
    {
      AVR32_OPC_MVCR_W, 4, 0xefa00000, 0xfff010ff,
      &avr32_syntax_table[AVR32_SYNTAX_MVCR_W],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_CRD_RI],
      },
    },
    {
      AVR32_OPC_MVRC_D, 4, 0xefa00030, 0xfff111ff,
      &avr32_syntax_table[AVR32_SYNTAX_MVRC_D],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_CRD_DW],
	&avr32_ifield_table[AVR32_IFIELD_RY_DW],
      },
    },
    {
      AVR32_OPC_MVRC_W, 4, 0xefa00020, 0xfff010ff,
      &avr32_syntax_table[AVR32_SYNTAX_MVRC_W],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_CRD_RI],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_NEG, 2, 0x5c300000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_NEG],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      }
    },
    {
      AVR32_OPC_NOP, 2, 0xd7030000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_NOP],
      BFD_RELOC_UNUSED, 0, -1, { NULL },
    },
    {
      AVR32_OPC_OR1, 2, 0x00400000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_OR1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_OR2, 4, 0xe1e01000, 0xe1f0fe00,
      &avr32_syntax_table[AVR32_SYNTAX_OR2],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K5E2],
      },
    },
    {
      AVR32_OPC_OR3, 4, 0xe1e01200, 0xe1f0fe00,
      &avr32_syntax_table[AVR32_SYNTAX_OR3],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K5E2],
      },
    },
    {
      AVR32_OPC_ORH, 4, 0xea100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_ORH],
      BFD_RELOC_AVR32_16U, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_ORL, 4, 0xe8100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_ORL],
      BFD_RELOC_AVR32_16U, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_PABS_SB, 4, 0xe00023e0, 0xfff0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PABS_SB],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PABS_SH, 4, 0xe00023f0, 0xfff0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PABS_SH],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PACKSH_SB, 4, 0xe00024d0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PACKSH_SB],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PACKSH_UB, 4, 0xe00024c0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PACKSH_UB],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PACKW_SH, 4, 0xe0002470, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PACKW_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PADD_B, 4, 0xe0002300, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PADD_B],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PADD_H, 4, 0xe0002000, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PADD_H],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PADDH_SH, 4, 0xe00020c0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDH_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PADDH_UB, 4, 0xe0002360, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDH_UB],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PADDS_SB, 4, 0xe0002320, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDS_SB],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PADDS_SH, 4, 0xe0002040, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDS_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PADDS_UB, 4, 0xe0002340, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDS_UB],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PADDS_UH, 4, 0xe0002080, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDS_UH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PADDSUB_H, 4, 0xe0002100, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDSUB_H],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_PADDSUBH_SH, 4, 0xe0002280, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDSUBH_SH],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_PADDSUBS_SH, 4, 0xe0002180, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDSUBS_SH],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_PADDSUBS_UH, 4, 0xe0002200, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDSUBS_UH],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_PADDX_H, 4, 0xe0002020, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDX_H],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PADDXH_SH, 4, 0xe00020e0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDXH_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PADDXS_SH, 4, 0xe0002060, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDXS_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PADDXS_UH, 4, 0xe00020a0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PADDXS_UH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PASR_B, 4, 0xe0002410, 0xe1f8fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PASR_B],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_COND3],
      },
    },
    {
      AVR32_OPC_PASR_H, 4, 0xe0002440, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PASR_H],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PAVG_SH, 4, 0xe00023d0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PAVG_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PAVG_UB, 4, 0xe00023c0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PAVG_UB],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PLSL_B, 4, 0xe0002420, 0xe1f8fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PLSL_B],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_COND3],
      },
    },
    {
      AVR32_OPC_PLSL_H, 4, 0xe0002450, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PLSL_H],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PLSR_B, 4, 0xe0002430, 0xe1f8fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PLSR_B],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_COND3],
      },
    },
    {
      AVR32_OPC_PLSR_H, 4, 0xe0002460, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PLSR_H],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PMAX_SH, 4, 0xe0002390, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PMAX_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PMAX_UB, 4, 0xe0002380, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PMAX_UB],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PMIN_SH, 4, 0xe00023b0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PMIN_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PMIN_UB, 4, 0xe00023a0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PMIN_UB],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_POPJC, 2, 0xd7130000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_POPJC],
      BFD_RELOC_UNUSED, 0, -1, { NULL },
    },
    {
      AVR32_OPC_POPM, 2, 0xd0020000, 0xf0070000,
      &avr32_syntax_table[AVR32_SYNTAX_POPM],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_POPM],
      },
    },
    {
      AVR32_OPC_POPM_E, 4, 0xe3cd0000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_POPM_E],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_PREF, 4, 0xf2100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_PREF],
      BFD_RELOC_AVR32_16S, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_PSAD, 4, 0xe0002400, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSAD],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PSUB_B, 4, 0xe0002310, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUB_B],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PSUB_H, 4, 0xe0002010, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUB_H],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PSUBADD_H, 4, 0xe0002140, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBADD_H],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_PSUBADDH_SH, 4, 0xe00022c0, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBADDH_SH],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_PSUBADDS_SH, 4, 0xe00021c0, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBADDS_SH],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_PSUBADDS_UH, 4, 0xe0002240, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBADDS_UH],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_PSUBH_SH, 4, 0xe00020d0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBH_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PSUBH_UB, 4, 0xe0002370, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBH_UB],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PSUBS_SB, 4, 0xe0002330, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBS_SB],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PSUBS_SH, 4, 0xe0002050, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBS_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PSUBS_UB, 4, 0xe0002350, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBS_UB],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PSUBS_UH, 4, 0xe0002090, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBS_UH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PSUBX_H, 4, 0xe0002030, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBX_H],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PSUBXH_SH, 4, 0xe00020f0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBXH_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PSUBXS_SH, 4, 0xe0002070, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBXS_SH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PSUBXS_UH, 4, 0xe00020b0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_PSUBXS_UH],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_PUNPCKSB_H, 4, 0xe00024a0, 0xe1ffffe0,
      &avr32_syntax_table[AVR32_SYNTAX_PUNPCKSB_H],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_PUNPCKUB_H, 4, 0xe0002480, 0xe1ffffe0,
      &avr32_syntax_table[AVR32_SYNTAX_PUNPCKUB_H],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_PUSHJC, 2, 0xd7230000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_PUSHJC],
      BFD_RELOC_UNUSED, 0, -1, { NULL },
    },
    {
      AVR32_OPC_PUSHM, 2, 0xd0010000, 0xf00f0000,
      &avr32_syntax_table[AVR32_SYNTAX_PUSHM],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_PUSHM_E, 4, 0xebcd0000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_PUSHM_E],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_RCALL1, 2, 0xc00c0000, 0xf00c0000,
      &avr32_syntax_table[AVR32_SYNTAX_RCALL1],
      BFD_RELOC_AVR32_11H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K10],
      },
    },
    {
      AVR32_OPC_RCALL2, 4, 0xe0a00000, 0xe1ef0000,
      &avr32_syntax_table[AVR32_SYNTAX_RCALL2],
      BFD_RELOC_AVR32_22H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_RETEQ, 2, 0x5e000000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETEQ],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETNE, 2, 0x5e100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETNE],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETCC, 2, 0x5e200000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETHS],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETCS, 2, 0x5e300000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETLO],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETGE, 2, 0x5e400000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETGE],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETLT, 2, 0x5e500000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETLT],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETMI, 2, 0x5e600000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETMI],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETPL, 2, 0x5e700000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETPL],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETLS, 2, 0x5e800000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETLS],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETGT, 2, 0x5e900000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETGT],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETLE, 2, 0x5ea00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETLE],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETHI, 2, 0x5eb00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETHI],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETVS, 2, 0x5ec00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETVS],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETVC, 2, 0x5ed00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETVC],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETQS, 2, 0x5ee00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETQS],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETAL, 2, 0x5ef00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_RETAL],
      BFD_RELOC_NONE, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_RETD, 2, 0xd6230000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_RETD],
      BFD_RELOC_NONE, 0, -1, { NULL },
    },
    {
      AVR32_OPC_RETE, 2, 0xd6030000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_RETE],
      BFD_RELOC_NONE, 0, -1, { NULL },
    },
    {
      AVR32_OPC_RETJ, 2, 0xd6330000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_RETJ],
      BFD_RELOC_NONE, 0, -1, { NULL },
    },
    {
      AVR32_OPC_RETS, 2, 0xd6130000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_RETS],
      BFD_RELOC_NONE, 0, -1, { NULL },
    },
    {
      AVR32_OPC_RJMP, 2, 0xc0080000, 0xf00c0000,
      &avr32_syntax_table[AVR32_SYNTAX_RJMP],
      BFD_RELOC_AVR32_11H_PCREL, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K10],
      },
    },
    {
      AVR32_OPC_ROL, 2, 0x5cf00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_ROL],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      }
    },
    {
      AVR32_OPC_ROR, 2, 0x5d000000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_ROR],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      }
    },
    {
      AVR32_OPC_RSUB1, 2, 0x00200000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_RSUB1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_RSUB2, 4, 0xe0001100, 0xe1f0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_RSUB2],
      BFD_RELOC_AVR32_8S_EXT, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SATADD_H, 4, 0xe00002c0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_SATADD_H],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SATADD_W, 4, 0xe00000c0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_SATADD_W],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SATRNDS, 4, 0xf3b00000, 0xfff0fc00,
      &avr32_syntax_table[AVR32_SYNTAX_SATRNDS],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K5E],
	&avr32_ifield_table[AVR32_IFIELD_S5],
      },
    },
    {
      AVR32_OPC_SATRNDU, 4, 0xf3b00400, 0xfff0fc00,
      &avr32_syntax_table[AVR32_SYNTAX_SATRNDU],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K5E],
	&avr32_ifield_table[AVR32_IFIELD_S5],
      },
    },
    {
      AVR32_OPC_SATS, 4, 0xf1b00000, 0xfff0fc00,
      &avr32_syntax_table[AVR32_SYNTAX_SATS],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K5E],
	&avr32_ifield_table[AVR32_IFIELD_S5],
      },
    },
    {
      AVR32_OPC_SATSUB_H, 4, 0xe00003c0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_SATSUB_H],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SATSUB_W1, 4, 0xe00001c0, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_SATSUB_W1],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SATSUB_W2, 4, 0xe0d00000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_SATSUB_W2],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_SATU, 4, 0xf1b00400, 0xfff0fc00,
      &avr32_syntax_table[AVR32_SYNTAX_SATU],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K5E],
	&avr32_ifield_table[AVR32_IFIELD_S5],
      },
    },
    {
      AVR32_OPC_SBC, 4, 0xe0000140, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_SBC],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SBR, 2, 0xa1a00000, 0xe1e00000,
      &avr32_syntax_table[AVR32_SYNTAX_SBR],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_BIT5C],
      },
    },
    {
      AVR32_OPC_SCALL, 2, 0xd7330000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_SCALL],
      BFD_RELOC_UNUSED, 0, -1, { NULL },
    },
    {
      AVR32_OPC_SCR, 2, 0x5c100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SCR],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SLEEP, 4, 0xe9b00000, 0xffffff00,
      &avr32_syntax_table[AVR32_SYNTAX_SLEEP],
      BFD_RELOC_AVR32_8S_EXT, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SREQ, 2, 0x5f000000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SREQ],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRNE, 2, 0x5f100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRNE],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRCC, 2, 0x5f200000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRHS],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRCS, 2, 0x5f300000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRLO],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRGE, 2, 0x5f400000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRGE],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRLT, 2, 0x5f500000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRLT],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRMI, 2, 0x5f600000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRMI],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRPL, 2, 0x5f700000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRPL],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRLS, 2, 0x5f800000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRLS],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRGT, 2, 0x5f900000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRGT],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRLE, 2, 0x5fa00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRLE],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRHI, 2, 0x5fb00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRHI],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRVS, 2, 0x5fc00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRVS],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRVC, 2, 0x5fd00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRVC],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRQS, 2, 0x5fe00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRQS],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SRAL, 2, 0x5ff00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SRAL],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SSRF, 2, 0xd2030000, 0xfe0f0000,
      &avr32_syntax_table[AVR32_SYNTAX_SSRF],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_K5C],
      },
    },
    {
      AVR32_OPC_ST_B1, 2, 0x00c00000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_B1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ST_B2, 2, 0x00f00000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_B2],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ST_B5, 4, 0xe0000b00, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_ST_B5],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
      },
    },
    {
      AVR32_OPC_ST_B3, 2, 0xa0800000, 0xe1800000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_B3],
      BFD_RELOC_AVR32_3U, 3, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K3],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ST_B4, 4, 0xe1600000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_B4],
      BFD_RELOC_AVR32_16S, 3, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ST_D1, 2, 0xa1200000, 0xe1f10000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_D1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY_DW],
      },
    },
    {
      AVR32_OPC_ST_D2, 2, 0xa1210000, 0xe1f10000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_D2],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY_DW],
      },
    },
    {
      AVR32_OPC_ST_D3, 2, 0xa1110000, 0xe1f10000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_D3],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY_DW],
      },
    },
    {
      AVR32_OPC_ST_D5, 4, 0xe0000800, 0xe1f0ffc1,
      &avr32_syntax_table[AVR32_SYNTAX_ST_D5],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
	&avr32_ifield_table[AVR32_IFIELD_RD_DW],
      },
    },
    {
      AVR32_OPC_ST_D4, 4, 0xe0e10000, 0xe1f10000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_D4],
      BFD_RELOC_AVR32_16S, 3, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
	&avr32_ifield_table[AVR32_IFIELD_RY_DW],
      },
    },
    {
      AVR32_OPC_ST_H1, 2, 0x00b00000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_H1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ST_H2, 2, 0x00e00000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_H2],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ST_H5, 4, 0xe0000a00, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_ST_H5],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
      },
    },
    {
      AVR32_OPC_ST_H3, 2, 0xa0000000, 0xe1800000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_H3],
      BFD_RELOC_AVR32_4UH, 3, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K3],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ST_H4, 4, 0xe1500000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_H4],
      BFD_RELOC_AVR32_16S, 3, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ST_W1, 2, 0x00a00000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_W1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ST_W2, 2, 0x00d00000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_W2],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ST_W5, 4, 0xe0000900, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_ST_W5],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
      },
    },
    {
      AVR32_OPC_ST_W3, 2, 0x81000000, 0xe1000000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_W3],
      BFD_RELOC_AVR32_6UW, 3, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K4],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_ST_W4, 4, 0xe1400000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_ST_W4],
      BFD_RELOC_AVR32_16S, 3, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_STC_D1, 4, 0xeba01000, 0xfff01100,
      &avr32_syntax_table[AVR32_SYNTAX_STC_D1],
      BFD_RELOC_AVR32_10UW, 4, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
	&avr32_ifield_table[AVR32_IFIELD_CRD_DW],
      },
    },
    {
      AVR32_OPC_STC_D2, 4, 0xefa00070, 0xfff011f0,
      &avr32_syntax_table[AVR32_SYNTAX_STC_D2],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_CRD_DW],
      },
    },
    {
      AVR32_OPC_STC_D3, 4, 0xefa010c0, 0xfff011c0,
      &avr32_syntax_table[AVR32_SYNTAX_STC_D3],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_K2],
	&avr32_ifield_table[AVR32_IFIELD_CRD_DW],
      },
    },
    {
      AVR32_OPC_STC_W1, 4, 0xeba00000, 0xfff01000,
      &avr32_syntax_table[AVR32_SYNTAX_STC_W1],
      BFD_RELOC_AVR32_10UW, 4, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
	&avr32_ifield_table[AVR32_IFIELD_CRD_RI],
      },
    },
    {
      AVR32_OPC_STC_W2, 4, 0xefa00060, 0xfff010ff,
      &avr32_syntax_table[AVR32_SYNTAX_STC_W2],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_CRD_RI],
      },
    },
    {
      AVR32_OPC_STC_W3, 4, 0xefa01080, 0xfff010c0,
      &avr32_syntax_table[AVR32_SYNTAX_STC_W3],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_K2],
	&avr32_ifield_table[AVR32_IFIELD_CRD_RI],
      },
    },
    {
      AVR32_OPC_STC0_D, 4, 0xf7a00000, 0xfff00100,
      &avr32_syntax_table[AVR32_SYNTAX_STC0_D],
      BFD_RELOC_AVR32_14UW, 3, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K12CP],
	&avr32_ifield_table[AVR32_IFIELD_CRD_DW],
      },
    },
    {
      AVR32_OPC_STC0_W, 4, 0xf5a00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_STC0_W],
      BFD_RELOC_AVR32_14UW, 3, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K12CP],
	&avr32_ifield_table[AVR32_IFIELD_CRD_RI],
      },
    },
    {
      AVR32_OPC_STCM_D, 4, 0xeda00500, 0xfff01f00,
      &avr32_syntax_table[AVR32_SYNTAX_STCM_D],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_STCM_D_PU, 4, 0xeda01500, 0xfff01f00,
      &avr32_syntax_table[AVR32_SYNTAX_STCM_D_PU],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_STCM_W, 4, 0xeda00200, 0xfff01e00,
      &avr32_syntax_table[AVR32_SYNTAX_STCM_W],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
	&avr32_ifield_table[AVR32_IFIELD_CM_HL],
      },
    },
    {
      AVR32_OPC_STCM_W_PU, 4, 0xeda01200, 0xfff01e00,
      &avr32_syntax_table[AVR32_SYNTAX_STCM_W_PU],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_CPNO],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
	&avr32_ifield_table[AVR32_IFIELD_CM_HL],
      },
    },
    {
      AVR32_OPC_STCOND, 4, 0xe1700000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_STCOND],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_STDSP, 2, 0x50000000, 0xf8000000,
      &avr32_syntax_table[AVR32_SYNTAX_STDSP],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_K7C],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_STHH_W2, 4, 0xe1e08000, 0xe1f0c0c0,
      &avr32_syntax_table[AVR32_SYNTAX_STHH_W2],
      BFD_RELOC_UNUSED, 7, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_CRD_RI],
	&avr32_ifield_table[AVR32_IFIELD_K2],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X2],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y2],
      },
    },
    {
      AVR32_OPC_STHH_W1, 4, 0xe1e0c000, 0xe1f0c000,
      &avr32_syntax_table[AVR32_SYNTAX_STHH_W1],
      BFD_RELOC_AVR32_STHH_W, 6, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_K8E2],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X2],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y2],
      },
    },
    {
      AVR32_OPC_STM, 4, 0xe9c00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_STM],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_STM_PU, 4, 0xebc00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_STM_PU],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_STMTS, 4, 0xedc00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_STMTS],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_STMTS_PU, 4, 0xefc00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_STMTS_PU],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_STSWP_H, 4, 0xe1d09000, 0xe1f0f000,
      &avr32_syntax_table[AVR32_SYNTAX_STSWP_H],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K12],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_STSWP_W, 4, 0xe1d0a000, 0xe1f0f000,
      &avr32_syntax_table[AVR32_SYNTAX_STSWP_W],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K12],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_SUB1, 2, 0x00100000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_SUB1],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_SUB2, 4, 0xe0000100, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_SUB2],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K2],
      },
    },
    {
      AVR32_OPC_SUB5, 4, 0xe0c00000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_SUB5],
      BFD_RELOC_AVR32_SUB5, 3, 2,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_SUB3_SP, 2, 0x200d0000, 0xf00f0000,
      &avr32_syntax_table[AVR32_SYNTAX_SUB3_SP],
      BFD_RELOC_AVR32_10SW, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_SUB3, 2, 0x20000000, 0xf0000000,
      &avr32_syntax_table[AVR32_SYNTAX_SUB3],
      BFD_RELOC_AVR32_8S, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8C],
      },
    },
    {
      AVR32_OPC_SUB4, 4, 0xe0200000, 0xe1e00000,
      &avr32_syntax_table[AVR32_SYNTAX_SUB4],
      BFD_RELOC_AVR32_21S, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K21],
      },
    },
    {
      AVR32_OPC_SUBEQ, 4, 0xf7b00000, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBEQ],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBNE, 4, 0xf7b00100, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBNE],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBCC, 4, 0xf7b00200, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBHS],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBCS, 4, 0xf7b00300, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBLO],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBGE, 4, 0xf7b00400, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBGE],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBLT, 4, 0xf7b00500, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBLT],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBMI, 4, 0xf7b00600, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBMI],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBPL, 4, 0xf7b00700, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBPL],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBLS, 4, 0xf7b00800, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBLS],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBGT, 4, 0xf7b00900, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBGT],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBLE, 4, 0xf7b00a00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBLE],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBHI, 4, 0xf7b00b00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBHI],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBVS, 4, 0xf7b00c00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBVS],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBVC, 4, 0xf7b00d00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBVC],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBQS, 4, 0xf7b00e00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBQS],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBAL, 4, 0xf7b00f00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBAL],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFEQ, 4, 0xf5b00000, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFEQ],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFNE, 4, 0xf5b00100, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFNE],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFCC, 4, 0xf5b00200, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFHS],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFCS, 4, 0xf5b00300, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFLO],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFGE, 4, 0xf5b00400, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFGE],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFLT, 4, 0xf5b00500, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFLT],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFMI, 4, 0xf5b00600, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFMI],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFPL, 4, 0xf5b00700, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFPL],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFLS, 4, 0xf5b00800, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFLS],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFGT, 4, 0xf5b00900, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFGT],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFLE, 4, 0xf5b00a00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFLE],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFHI, 4, 0xf5b00b00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFHI],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFVS, 4, 0xf5b00c00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFVS],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFVC, 4, 0xf5b00d00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFVC],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFQS, 4, 0xf5b00e00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFQS],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBFAL, 4, 0xf5b00f00, 0xfff0ff00,
      &avr32_syntax_table[AVR32_SYNTAX_SUBFAL],
      BFD_RELOC_AVR32_8S_EXT, 2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      },
    },
    {
      AVR32_OPC_SUBHH_W, 4, 0xe0000f00, 0xe1f0ffc0,
      &avr32_syntax_table[AVR32_SYNTAX_SUBHH_W],
      BFD_RELOC_UNUSED, 5, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_X],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_Y],
      },
    },
    {
      AVR32_OPC_SWAP_B, 2, 0x5cb00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SWAP_B],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      }
    },
    {
      AVR32_OPC_SWAP_BH, 2, 0x5cc00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SWAP_BH],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      }
    },
    {
      AVR32_OPC_SWAP_H, 2, 0x5ca00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_SWAP_H],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      }
    },
    {
      AVR32_OPC_SYNC, 4, 0xebb00000, 0xffffff00,
      &avr32_syntax_table[AVR32_SYNTAX_SYNC],
      BFD_RELOC_AVR32_8S_EXT, 1, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_K8E],
      }
    },
    {
      AVR32_OPC_TLBR, 2, 0xd6430000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_TLBR],
      BFD_RELOC_UNUSED, 0, -1, { NULL },
    },
    {
      AVR32_OPC_TLBS, 2, 0xd6530000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_TLBS],
      BFD_RELOC_UNUSED, 0, -1, { NULL },
    },
    {
      AVR32_OPC_TLBW, 2, 0xd6630000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_TLBW],
      BFD_RELOC_UNUSED, 0, -1, { NULL },
    },
    {
      AVR32_OPC_TNBZ, 2, 0x5ce00000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_TNBZ],
      BFD_RELOC_UNUSED, 1, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
      }
    },
    {
      AVR32_OPC_TST, 2, 0x00700000, 0xe1f00000,
      &avr32_syntax_table[AVR32_SYNTAX_TST],
      BFD_RELOC_UNUSED, 2, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_RX],
      },
    },
    {
      AVR32_OPC_XCHG, 4, 0xe0000b40, 0xe1f0fff0,
      &avr32_syntax_table[AVR32_SYNTAX_XCHG],
      BFD_RELOC_UNUSED, 3, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RD_E],
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
      },
    },
    {
      AVR32_OPC_MEMC, 4, 0xf6100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_MEMC],
      BFD_RELOC_AVR32_15S, 2, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_MEM15],
	&avr32_ifield_table[AVR32_IFIELD_MEMB5],
      },
    },
    {
      AVR32_OPC_MEMS, 4, 0xf8100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_MEMS],
      BFD_RELOC_AVR32_15S, 2, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_MEM15],
	&avr32_ifield_table[AVR32_IFIELD_MEMB5],
      },
    },
    {
      AVR32_OPC_MEMT, 4, 0xfa100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_MEMT],
      BFD_RELOC_AVR32_15S, 2, 0,
      {
	&avr32_ifield_table[AVR32_IFIELD_MEM15],
	&avr32_ifield_table[AVR32_IFIELD_MEMB5],
      },
    },
    {
      AVR32_OPC_BFEXTS, 4, 0xe1d0b000, 0xe1f0fc00,
      &avr32_syntax_table[AVR32_SYNTAX_BFEXTS],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_S5],
	&avr32_ifield_table[AVR32_IFIELD_K5E],
      },
    },
    {
      AVR32_OPC_BFEXTU, 4, 0xe1d0c000, 0xe1f0fc00,
      &avr32_syntax_table[AVR32_SYNTAX_BFEXTU],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_S5],
	&avr32_ifield_table[AVR32_IFIELD_K5E],
      },
    },
    {
      AVR32_OPC_BFINS, 4, 0xe1d0d000, 0xe1f0fc00,
      &avr32_syntax_table[AVR32_SYNTAX_BFINS],
      BFD_RELOC_UNUSED, 4, -1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RX],
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_S5],
	&avr32_ifield_table[AVR32_IFIELD_K5E],
      },
    },
#define AVR32_OPCODE_RSUBCOND(cond_name, cond_field)                    \
    {                                                                   \
      AVR32_OPC_RSUB ## cond_name , 4,                                  \
      0xfbb00000 | (cond_field << 8), 0xfff0ff00,                       \
      &avr32_syntax_table[AVR32_SYNTAX_RSUB ## cond_name ],             \
      BFD_RELOC_AVR32_8S_EXT, 2, 1,                                     \
      {                                                                 \
	&avr32_ifield_table[AVR32_IFIELD_RY],                           \
	&avr32_ifield_table[AVR32_IFIELD_K8E],                          \
      },                                                                \
    },

    AVR32_OPCODE_RSUBCOND (EQ, 0) 
    AVR32_OPCODE_RSUBCOND (NE, 1) 
    AVR32_OPCODE_RSUBCOND (CC, 2) 
    AVR32_OPCODE_RSUBCOND (CS, 3) 
    AVR32_OPCODE_RSUBCOND (GE, 4) 
    AVR32_OPCODE_RSUBCOND (LT, 5) 
    AVR32_OPCODE_RSUBCOND (MI, 6) 
    AVR32_OPCODE_RSUBCOND (PL, 7) 
    AVR32_OPCODE_RSUBCOND (LS, 8) 
    AVR32_OPCODE_RSUBCOND (GT, 9) 
    AVR32_OPCODE_RSUBCOND (LE, 10) 
    AVR32_OPCODE_RSUBCOND (HI, 11) 
    AVR32_OPCODE_RSUBCOND (VS, 12) 
    AVR32_OPCODE_RSUBCOND (VC, 13) 
    AVR32_OPCODE_RSUBCOND (QS, 14) 
    AVR32_OPCODE_RSUBCOND (AL, 15) 

#define AVR32_OPCODE_OP3_COND(op_name, op_field, cond_name, cond_field) \
    {                                                                   \
      AVR32_OPC_ ## op_name ## cond_name , 4,                           \
      0xe1d0e000 | (cond_field << 8) | (op_field << 4), 0xe1f0fff0,     \
      &avr32_syntax_table[AVR32_SYNTAX_ ## op_name ## cond_name ],      \
      BFD_RELOC_UNUSED, 3, -1,                                          \
      {                                                                 \
	&avr32_ifield_table[AVR32_IFIELD_RD_E],                         \
	&avr32_ifield_table[AVR32_IFIELD_RX],                           \
	&avr32_ifield_table[AVR32_IFIELD_RY],                           \
      },                                                                \
    },

    AVR32_OPCODE_OP3_COND (ADD, 0, EQ, 0)
    AVR32_OPCODE_OP3_COND (ADD, 0, NE, 1)
    AVR32_OPCODE_OP3_COND (ADD, 0, CC, 2)
    AVR32_OPCODE_OP3_COND (ADD, 0, CS, 3)
    AVR32_OPCODE_OP3_COND (ADD, 0, GE, 4)
    AVR32_OPCODE_OP3_COND (ADD, 0, LT, 5)
    AVR32_OPCODE_OP3_COND (ADD, 0, MI, 6)
    AVR32_OPCODE_OP3_COND (ADD, 0, PL, 7)
    AVR32_OPCODE_OP3_COND (ADD, 0, LS, 8)
    AVR32_OPCODE_OP3_COND (ADD, 0, GT, 9)
    AVR32_OPCODE_OP3_COND (ADD, 0, LE, 10)
    AVR32_OPCODE_OP3_COND (ADD, 0, HI, 11)
    AVR32_OPCODE_OP3_COND (ADD, 0, VS, 12)
    AVR32_OPCODE_OP3_COND (ADD, 0, VC, 13)
    AVR32_OPCODE_OP3_COND (ADD, 0, QS, 14)
    AVR32_OPCODE_OP3_COND (ADD, 0, AL, 15)

    AVR32_OPCODE_OP3_COND (SUB2, 1, EQ, 0)
    AVR32_OPCODE_OP3_COND (SUB2, 1, NE, 1)
    AVR32_OPCODE_OP3_COND (SUB2, 1, CC, 2)
    AVR32_OPCODE_OP3_COND (SUB2, 1, CS, 3)
    AVR32_OPCODE_OP3_COND (SUB2, 1, GE, 4)
    AVR32_OPCODE_OP3_COND (SUB2, 1, LT, 5)
    AVR32_OPCODE_OP3_COND (SUB2, 1, MI, 6)
    AVR32_OPCODE_OP3_COND (SUB2, 1, PL, 7)
    AVR32_OPCODE_OP3_COND (SUB2, 1, LS, 8)
    AVR32_OPCODE_OP3_COND (SUB2, 1, GT, 9)
    AVR32_OPCODE_OP3_COND (SUB2, 1, LE, 10)
    AVR32_OPCODE_OP3_COND (SUB2, 1, HI, 11)
    AVR32_OPCODE_OP3_COND (SUB2, 1, VS, 12)
    AVR32_OPCODE_OP3_COND (SUB2, 1, VC, 13)
    AVR32_OPCODE_OP3_COND (SUB2, 1, QS, 14)
    AVR32_OPCODE_OP3_COND (SUB2, 1, AL, 15)

    AVR32_OPCODE_OP3_COND (AND, 2, EQ, 0)
    AVR32_OPCODE_OP3_COND (AND, 2, NE, 1)
    AVR32_OPCODE_OP3_COND (AND, 2, CC, 2)
    AVR32_OPCODE_OP3_COND (AND, 2, CS, 3)
    AVR32_OPCODE_OP3_COND (AND, 2, GE, 4)
    AVR32_OPCODE_OP3_COND (AND, 2, LT, 5)
    AVR32_OPCODE_OP3_COND (AND, 2, MI, 6)
    AVR32_OPCODE_OP3_COND (AND, 2, PL, 7)
    AVR32_OPCODE_OP3_COND (AND, 2, LS, 8)
    AVR32_OPCODE_OP3_COND (AND, 2, GT, 9)
    AVR32_OPCODE_OP3_COND (AND, 2, LE, 10)
    AVR32_OPCODE_OP3_COND (AND, 2, HI, 11)
    AVR32_OPCODE_OP3_COND (AND, 2, VS, 12)
    AVR32_OPCODE_OP3_COND (AND, 2, VC, 13)
    AVR32_OPCODE_OP3_COND (AND, 2, QS, 14)
    AVR32_OPCODE_OP3_COND (AND, 2, AL, 15)

    AVR32_OPCODE_OP3_COND (OR, 3, EQ, 0)
    AVR32_OPCODE_OP3_COND (OR, 3, NE, 1)
    AVR32_OPCODE_OP3_COND (OR, 3, CC, 2)
    AVR32_OPCODE_OP3_COND (OR, 3, CS, 3)
    AVR32_OPCODE_OP3_COND (OR, 3, GE, 4)
    AVR32_OPCODE_OP3_COND (OR, 3, LT, 5)
    AVR32_OPCODE_OP3_COND (OR, 3, MI, 6)
    AVR32_OPCODE_OP3_COND (OR, 3, PL, 7)
    AVR32_OPCODE_OP3_COND (OR, 3, LS, 8)
    AVR32_OPCODE_OP3_COND (OR, 3, GT, 9)
    AVR32_OPCODE_OP3_COND (OR, 3, LE, 10)
    AVR32_OPCODE_OP3_COND (OR, 3, HI, 11)
    AVR32_OPCODE_OP3_COND (OR, 3, VS, 12)
    AVR32_OPCODE_OP3_COND (OR, 3, VC, 13)
    AVR32_OPCODE_OP3_COND (OR, 3, QS, 14)
    AVR32_OPCODE_OP3_COND (OR, 3, AL, 15)

    AVR32_OPCODE_OP3_COND (EOR, 4, EQ, 0)
    AVR32_OPCODE_OP3_COND (EOR, 4, NE, 1)
    AVR32_OPCODE_OP3_COND (EOR, 4, CC, 2)
    AVR32_OPCODE_OP3_COND (EOR, 4, CS, 3)
    AVR32_OPCODE_OP3_COND (EOR, 4, GE, 4)
    AVR32_OPCODE_OP3_COND (EOR, 4, LT, 5)
    AVR32_OPCODE_OP3_COND (EOR, 4, MI, 6)
    AVR32_OPCODE_OP3_COND (EOR, 4, PL, 7)
    AVR32_OPCODE_OP3_COND (EOR, 4, LS, 8)
    AVR32_OPCODE_OP3_COND (EOR, 4, GT, 9)
    AVR32_OPCODE_OP3_COND (EOR, 4, LE, 10)
    AVR32_OPCODE_OP3_COND (EOR, 4, HI, 11)
    AVR32_OPCODE_OP3_COND (EOR, 4, VS, 12)
    AVR32_OPCODE_OP3_COND (EOR, 4, VC, 13)
    AVR32_OPCODE_OP3_COND (EOR, 4, QS, 14)
    AVR32_OPCODE_OP3_COND (EOR, 4, AL, 15) 

#define AVR32_OPCODE_LD_COND(op_name, op_field, cond_name, cond_field)  \
    {                                                                   \
      AVR32_OPC_ ## op_name ## cond_name , 4,                           \
      0xe1f00000 | (cond_field << 12) | (op_field  << 9), 0xe1f0fe00,   \
      &avr32_syntax_table[AVR32_SYNTAX_ ## op_name ## cond_name ],      \
      BFD_RELOC_UNUSED, 3, -1,                                          \
      {                                                                 \
	&avr32_ifield_table[AVR32_IFIELD_RY],                           \
	&avr32_ifield_table[AVR32_IFIELD_RX],                           \
	&avr32_ifield_table[AVR32_IFIELD_K9E],                          \
      },                                                                \
    },
    
#define AVR32_OPCODE_ST_COND(op_name, op_field, cond_name, cond_field)  \
    {                                                                   \
      AVR32_OPC_ ## op_name ## cond_name , 4,                           \
      0xe1f00000 | (cond_field << 12) | (op_field  << 9), 0xe1f0fe00,   \
      &avr32_syntax_table[AVR32_SYNTAX_ ## op_name ## cond_name ],      \
      BFD_RELOC_UNUSED, 3, -1,                                          \
      {                                                                 \
	&avr32_ifield_table[AVR32_IFIELD_RX],                           \
	&avr32_ifield_table[AVR32_IFIELD_K9E],                          \
	&avr32_ifield_table[AVR32_IFIELD_RY],                           \
      },                                                                \
    },

    AVR32_OPCODE_LD_COND (LD_W, 0, EQ, 0) 
    AVR32_OPCODE_LD_COND (LD_W, 0, NE, 1) 
    AVR32_OPCODE_LD_COND (LD_W, 0, CC, 2) 
    AVR32_OPCODE_LD_COND (LD_W, 0, CS, 3) 
    AVR32_OPCODE_LD_COND (LD_W, 0, GE, 4) 
    AVR32_OPCODE_LD_COND (LD_W, 0, LT, 5) 
    AVR32_OPCODE_LD_COND (LD_W, 0, MI, 6) 
    AVR32_OPCODE_LD_COND (LD_W, 0, PL, 7) 
    AVR32_OPCODE_LD_COND (LD_W, 0, LS, 8) 
    AVR32_OPCODE_LD_COND (LD_W, 0, GT, 9) 
    AVR32_OPCODE_LD_COND (LD_W, 0, LE, 10) 
    AVR32_OPCODE_LD_COND (LD_W, 0, HI, 11) 
    AVR32_OPCODE_LD_COND (LD_W, 0, VS, 12) 
    AVR32_OPCODE_LD_COND (LD_W, 0, VC, 13) 
    AVR32_OPCODE_LD_COND (LD_W, 0, QS, 14) 
    AVR32_OPCODE_LD_COND (LD_W, 0, AL, 15) 

    AVR32_OPCODE_LD_COND (LD_SH, 1, EQ, 0) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, NE, 1) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, CC, 2) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, CS, 3) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, GE, 4) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, LT, 5) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, MI, 6) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, PL, 7) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, LS, 8) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, GT, 9) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, LE, 10) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, HI, 11) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, VS, 12) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, VC, 13) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, QS, 14) 
    AVR32_OPCODE_LD_COND (LD_SH, 1, AL, 15) 

    AVR32_OPCODE_LD_COND (LD_UH, 2, EQ, 0) 
    AVR32_OPCODE_LD_COND (LD_UH, 2, NE, 1) 
    AVR32_OPCODE_LD_COND (LD_UH, 2, CC, 2) 
    AVR32_OPCODE_LD_COND (LD_UH, 2, CS, 3) 
    AVR32_OPCODE_LD_COND (LD_UH, 2, GE, 4) 
    AVR32_OPCODE_LD_COND (LD_UH, 2, LT, 5) 
    AVR32_OPCODE_LD_COND (LD_UH, 2, MI, 6) 
    AVR32_OPCODE_LD_COND (LD_UH, 2, PL, 7) 
    AVR32_OPCODE_LD_COND (LD_SH, 2, LS, 8) 
    AVR32_OPCODE_LD_COND (LD_SH, 2, GT, 9) 
    AVR32_OPCODE_LD_COND (LD_SH, 2, LE, 10) 
    AVR32_OPCODE_LD_COND (LD_SH, 2, HI, 11) 
    AVR32_OPCODE_LD_COND (LD_SH, 2, VS, 12) 
    AVR32_OPCODE_LD_COND (LD_SH, 2, VC, 13) 
    AVR32_OPCODE_LD_COND (LD_SH, 2, QS, 14) 
    AVR32_OPCODE_LD_COND (LD_SH, 2, AL, 15) 

    AVR32_OPCODE_LD_COND (LD_SB, 3, EQ, 0) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, NE, 1) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, CC, 2) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, CS, 3) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, GE, 4) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, LT, 5) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, MI, 6) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, PL, 7) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, LS, 8) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, GT, 9) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, LE, 10) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, HI, 11) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, VS, 12) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, VC, 13) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, QS, 14) 
    AVR32_OPCODE_LD_COND (LD_SB, 3, AL, 15) 

    AVR32_OPCODE_LD_COND (LD_UB, 4, EQ, 0) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, NE, 1) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, CC, 2) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, CS, 3) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, GE, 4) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, LT, 5) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, MI, 6) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, PL, 7) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, LS, 8) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, GT, 9) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, LE, 10) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, HI, 11) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, VS, 12) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, VC, 13) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, QS, 14) 
    AVR32_OPCODE_LD_COND (LD_UB, 4, AL, 15) 

    AVR32_OPCODE_ST_COND (ST_W, 5, EQ, 0) 
    AVR32_OPCODE_ST_COND (ST_W, 5, NE, 1) 
    AVR32_OPCODE_ST_COND (ST_W, 5, CC, 2) 
    AVR32_OPCODE_ST_COND (ST_W, 5, CS, 3) 
    AVR32_OPCODE_ST_COND (ST_W, 5, GE, 4) 
    AVR32_OPCODE_ST_COND (ST_W, 5, LT, 5) 
    AVR32_OPCODE_ST_COND (ST_W, 5, MI, 6) 
    AVR32_OPCODE_ST_COND (ST_W, 5, PL, 7) 
    AVR32_OPCODE_ST_COND (ST_W, 5, LS, 8) 
    AVR32_OPCODE_ST_COND (ST_W, 5, GT, 9) 
    AVR32_OPCODE_ST_COND (ST_W, 5, LE, 10) 
    AVR32_OPCODE_ST_COND (ST_W, 5, HI, 11) 
    AVR32_OPCODE_ST_COND (ST_W, 5, VS, 12) 
    AVR32_OPCODE_ST_COND (ST_W, 5, VC, 13) 
    AVR32_OPCODE_ST_COND (ST_W, 5, QS, 14) 
    AVR32_OPCODE_ST_COND (ST_W, 5, AL, 15) 

    AVR32_OPCODE_ST_COND (ST_H, 6, EQ, 0) 
    AVR32_OPCODE_ST_COND (ST_H, 6, NE, 1) 
    AVR32_OPCODE_ST_COND (ST_H, 6, CC, 2) 
    AVR32_OPCODE_ST_COND (ST_H, 6, CS, 3) 
    AVR32_OPCODE_ST_COND (ST_H, 6, GE, 4) 
    AVR32_OPCODE_ST_COND (ST_H, 6, LT, 5) 
    AVR32_OPCODE_ST_COND (ST_H, 6, MI, 6) 
    AVR32_OPCODE_ST_COND (ST_H, 6, PL, 7) 
    AVR32_OPCODE_ST_COND (ST_H, 6, LS, 8) 
    AVR32_OPCODE_ST_COND (ST_H, 6, GT, 9) 
    AVR32_OPCODE_ST_COND (ST_H, 6, LE, 10) 
    AVR32_OPCODE_ST_COND (ST_H, 6, HI, 11) 
    AVR32_OPCODE_ST_COND (ST_H, 6, VS, 12) 
    AVR32_OPCODE_ST_COND (ST_H, 6, VC, 13) 
    AVR32_OPCODE_ST_COND (ST_H, 6, QS, 14) 
    AVR32_OPCODE_ST_COND (ST_H, 6, AL, 15) 

    AVR32_OPCODE_ST_COND (ST_B, 7, EQ, 0) 
    AVR32_OPCODE_ST_COND (ST_B, 7, NE, 1) 
    AVR32_OPCODE_ST_COND (ST_B, 7, CC, 2) 
    AVR32_OPCODE_ST_COND (ST_B, 7, CS, 3) 
    AVR32_OPCODE_ST_COND (ST_B, 7, GE, 4) 
    AVR32_OPCODE_ST_COND (ST_B, 7, LT, 5) 
    AVR32_OPCODE_ST_COND (ST_B, 7, MI, 6) 
    AVR32_OPCODE_ST_COND (ST_B, 7, PL, 7) 
    AVR32_OPCODE_ST_COND (ST_B, 7, LS, 8) 
    AVR32_OPCODE_ST_COND (ST_B, 7, GT, 9) 
    AVR32_OPCODE_ST_COND (ST_B, 7, LE, 10) 
    AVR32_OPCODE_ST_COND (ST_B, 7, HI, 11) 
    AVR32_OPCODE_ST_COND (ST_B, 7, VS, 12) 
    AVR32_OPCODE_ST_COND (ST_B, 7, VC, 13) 
    AVR32_OPCODE_ST_COND (ST_B, 7, QS, 14) 
    AVR32_OPCODE_ST_COND (ST_B, 7, AL, 15) 

    {
      AVR32_OPC_MOVH, 4, 0xfc100000, 0xfff00000,
      &avr32_syntax_table[AVR32_SYNTAX_MOVH],
      BFD_RELOC_AVR32_16U,  2, 1,
      {
	&avr32_ifield_table[AVR32_IFIELD_RY],
	&avr32_ifield_table[AVR32_IFIELD_K16],
      },
    },
    {
      AVR32_OPC_SSCALL, 2, 0xd7530000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_SSCALL],
      BFD_RELOC_UNUSED, 0, -1, { NULL },
    },
    {
      AVR32_OPC_RETSS, 2, 0xd7630000, 0xffff0000,
      &avr32_syntax_table[AVR32_SYNTAX_RETSS],
      BFD_RELOC_UNUSED, 0, -1, { NULL },
    },

  {
    AVR32_OPC_FMAC_S, 4, 0xE1A00000, 0xFFF0F000,
    &avr32_syntax_table[AVR32_SYNTAX_FMAC_S],
    BFD_RELOC_UNUSED, 4, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RA],
      &avr32_ifield_table[AVR32_IFIELD_FP_RX],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FNMAC_S, 4, 0xE1A01000, 0xFFF0F000,
    &avr32_syntax_table[AVR32_SYNTAX_FNMAC_S],
    BFD_RELOC_UNUSED, 4, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RA],
      &avr32_ifield_table[AVR32_IFIELD_FP_RX],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FMSC_S, 4, 0xE3A00000, 0xFFF0F000,
    &avr32_syntax_table[AVR32_SYNTAX_FMSC_S],
    BFD_RELOC_UNUSED, 4, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RA],
      &avr32_ifield_table[AVR32_IFIELD_FP_RX],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FNMSC_S, 4, 0xE3A01000, 0xFFF0F000,
    &avr32_syntax_table[AVR32_SYNTAX_FNMSC_S],
    BFD_RELOC_UNUSED, 4, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RA],
      &avr32_ifield_table[AVR32_IFIELD_FP_RX],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FMUL_S, 4, 0xE5A20000, 0xFFFFF000,
    &avr32_syntax_table[AVR32_SYNTAX_FMUL_S],
    BFD_RELOC_UNUSED, 3, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RX],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FNMUL_S, 4, 0xE5A30000, 0xFFFFF000,
    &avr32_syntax_table[AVR32_SYNTAX_FNMUL_S],
    BFD_RELOC_UNUSED, 3, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RX],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FADD_S, 4, 0xE5A00000, 0xFFFFF000,
    &avr32_syntax_table[AVR32_SYNTAX_FADD_S],
    BFD_RELOC_UNUSED, 3, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RX],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FSUB_S, 4, 0xE5A10000, 0xFFFFF000,
    &avr32_syntax_table[AVR32_SYNTAX_FSUB_S],
    BFD_RELOC_UNUSED, 3, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RX],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FCASTRS_SW, 4, 0xE5AB0000, 0xFFFFF0F0,
    &avr32_syntax_table[AVR32_SYNTAX_FCASTRS_SW],
    BFD_RELOC_UNUSED, 2, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FCASTRS_UW, 4, 0xE5A90000, 0xFFFFF0F0,
    &avr32_syntax_table[AVR32_SYNTAX_FCASTRS_UW],
    BFD_RELOC_UNUSED, 2, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FCASTSW_S, 4, 0xE5A60000, 0xFFFFF0F0,
    &avr32_syntax_table[AVR32_SYNTAX_FCASTSW_S],
    BFD_RELOC_UNUSED, 2, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FCASTUW_S, 4, 0xE5A40000, 0xFFFFF0F0,
    &avr32_syntax_table[AVR32_SYNTAX_FCASTUW_S],
    BFD_RELOC_UNUSED, 2, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FCMP_S, 4, 0xE5AC0000, 0xFFFFFF00,
    &avr32_syntax_table[AVR32_SYNTAX_FCMP_S],
    BFD_RELOC_UNUSED, 2, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RX],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FCHK_S, 4, 0xE5AD0000, 0xFFFFFFF0,
    &avr32_syntax_table[AVR32_SYNTAX_FCHK_S],
    BFD_RELOC_UNUSED, 1, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FRCPA_S, 4, 0xE5AE0000, 0xFFFFF0F0,
    &avr32_syntax_table[AVR32_SYNTAX_FRCPA_S],
    BFD_RELOC_UNUSED, 2, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  },
  {
    AVR32_OPC_FRSQRTA_S, 4, 0xE5AF0000, 0xFFFFF0F0,
    &avr32_syntax_table[AVR32_SYNTAX_FRSQRTA_S],
    BFD_RELOC_UNUSED, 2, -1,
    {
      &avr32_ifield_table[AVR32_IFIELD_FP_RD],
      &avr32_ifield_table[AVR32_IFIELD_FP_RY]
    }
  }

};


const struct avr32_alias avr32_alias_table[] =
  {
    {
      AVR32_ALIAS_PICOSVMAC0,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x0c },
      },
    },
    {
      AVR32_ALIAS_PICOSVMAC1,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x0d },
      },
    },
    {
      AVR32_ALIAS_PICOSVMAC2,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x0e },
      },
    },
    {
      AVR32_ALIAS_PICOSVMAC3,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x0f },
      },
    },
    {
      AVR32_ALIAS_PICOSVMUL0,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x08 },
      },
    },
    {
      AVR32_ALIAS_PICOSVMUL1,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x09 },
      },
    },
    {
      AVR32_ALIAS_PICOSVMUL2,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x0a },
      },
    },
    {
      AVR32_ALIAS_PICOSVMUL3,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x0b },
      },
    },
    {
      AVR32_ALIAS_PICOVMAC0,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x04 },
      },
    },
    {
      AVR32_ALIAS_PICOVMAC1,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x05 },
      },
    },
    {
      AVR32_ALIAS_PICOVMAC2,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x06 },
      },
    },
    {
      AVR32_ALIAS_PICOVMAC3,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x07 },
      },
    },
    {
      AVR32_ALIAS_PICOVMUL0,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x00 },
      },
    },
    {
      AVR32_ALIAS_PICOVMUL1,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x01 },
      },
    },
    {
      AVR32_ALIAS_PICOVMUL2,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x02 },
      },
    },
    {
      AVR32_ALIAS_PICOVMUL3,
      &avr32_opc_table[AVR32_OPC_COP],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
	{ 0, 0x03 },
      },
    },
    {
      AVR32_ALIAS_PICOLD_D1,
      &avr32_opc_table[AVR32_OPC_LDC_D1],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOLD_D2,
      &avr32_opc_table[AVR32_OPC_LDC_D2],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOLD_D3,
      &avr32_opc_table[AVR32_OPC_LDC_D3],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 },
      },
    },
    {
      AVR32_ALIAS_PICOLD_W1,
      &avr32_opc_table[AVR32_OPC_LDC_W1],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOLD_W2,
      &avr32_opc_table[AVR32_OPC_LDC_W2],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOLD_W3,
      &avr32_opc_table[AVR32_OPC_LDC_W3],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 },
      },
    },
    {
      AVR32_ALIAS_PICOLDM_D,
      &avr32_opc_table[AVR32_OPC_LDCM_D],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOLDM_D_PU,
      &avr32_opc_table[AVR32_OPC_LDCM_D_PU],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOLDM_W,
      &avr32_opc_table[AVR32_OPC_LDCM_W],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
      },
    },
    {
      AVR32_ALIAS_PICOLDM_W_PU,
      &avr32_opc_table[AVR32_OPC_LDCM_W_PU],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
      },
    },
    {
      AVR32_ALIAS_PICOMV_D1,
      &avr32_opc_table[AVR32_OPC_MVCR_D],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOMV_D2,
      &avr32_opc_table[AVR32_OPC_MVRC_D],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOMV_W1,
      &avr32_opc_table[AVR32_OPC_MVCR_W],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOMV_W2,
      &avr32_opc_table[AVR32_OPC_MVRC_W],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOST_D1,
      &avr32_opc_table[AVR32_OPC_STC_D1],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
      },
    },
    {
      AVR32_ALIAS_PICOST_D2,
      &avr32_opc_table[AVR32_OPC_STC_D2],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOST_D3,
      &avr32_opc_table[AVR32_OPC_STC_D3],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 },
      },
    },
    {
      AVR32_ALIAS_PICOST_W1,
      &avr32_opc_table[AVR32_OPC_STC_W1],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
      },
    },
    {
      AVR32_ALIAS_PICOST_W2,
      &avr32_opc_table[AVR32_OPC_STC_W2],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOST_W3,
      &avr32_opc_table[AVR32_OPC_STC_W3],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 },
      },
    },
    {
      AVR32_ALIAS_PICOSTM_D,
      &avr32_opc_table[AVR32_OPC_STCM_D],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOSTM_D_PU,
      &avr32_opc_table[AVR32_OPC_STCM_D_PU],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 },
      },
    },
    {
      AVR32_ALIAS_PICOSTM_W,
      &avr32_opc_table[AVR32_OPC_STCM_W],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
      },
    },
    {
      AVR32_ALIAS_PICOSTM_W_PU,
      &avr32_opc_table[AVR32_OPC_STCM_W_PU],
      {
	{ 0, PICO_CPNO },
	{ 1, 0 }, { 1, 1 }, { 1, 2 },
      },
    },
  };


#define SYNTAX_NORMAL0(id, mne, opc, arch)			\
  {							\
    AVR32_SYNTAX_##id, arch,			\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],	\
    AVR32_PARSER_NORMAL,					\
    { &avr32_opc_table[AVR32_OPC_##opc], },		\
    NULL, 0, { }					\
  }
#define SYNTAX_NORMAL1(id, mne, opc, op0, arch)		\
  {							\
    AVR32_SYNTAX_##id, arch,			\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],	\
    AVR32_PARSER_NORMAL,					\
    { &avr32_opc_table[AVR32_OPC_##opc], },		\
    NULL, 1,						\
    {							\
      AVR32_OPERAND_##op0,				\
    }							\
  }
#define SYNTAX_NORMALM1(id, mne, opc, op0, arch)		\
  {							\
    AVR32_SYNTAX_##id, arch,			\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],	\
    AVR32_PARSER_NORMAL,					\
    { &avr32_opc_table[AVR32_OPC_##opc], },			\
    NULL, -1,						\
    {							\
      AVR32_OPERAND_##op0,				\
    }							\
  }
#define SYNTAX_NORMAL2(id, mne, opc, op0, op1, arch)		\
  {							\
    AVR32_SYNTAX_##id, arch,			\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],	\
    AVR32_PARSER_NORMAL,					\
    { &avr32_opc_table[AVR32_OPC_##opc], },			\
    NULL, 2,						\
    {							\
      AVR32_OPERAND_##op0, AVR32_OPERAND_##op1,		\
    }							\
  }
#define SYNTAX_NORMALM2(id, mne, opc, op0, op1, arch)		\
  {							\
    AVR32_SYNTAX_##id, arch,			\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],	\
    AVR32_PARSER_NORMAL,					\
    { &avr32_opc_table[AVR32_OPC_##opc], },			\
    NULL, -2,						\
    {							\
      AVR32_OPERAND_##op0, AVR32_OPERAND_##op1,		\
    }							\
  }
#define SYNTAX_NORMAL3(id, mne, opc, op0, op1, op2, arch)	\
  {							\
    AVR32_SYNTAX_##id, arch,			\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],	\
    AVR32_PARSER_NORMAL,					\
    { &avr32_opc_table[AVR32_OPC_##opc], },			\
    NULL, 3,						\
    {							\
      AVR32_OPERAND_##op0, AVR32_OPERAND_##op1,		\
      AVR32_OPERAND_##op2,				\
    }							\
  }
#define SYNTAX_NORMALM3(id, mne, opc, op0, op1, op2, arch)	\
  {							\
    AVR32_SYNTAX_##id, arch,			\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],	\
    AVR32_PARSER_NORMAL,					\
    { &avr32_opc_table[AVR32_OPC_##opc], },			\
    NULL, -3,						\
    {							\
      AVR32_OPERAND_##op0, AVR32_OPERAND_##op1,		\
      AVR32_OPERAND_##op2,				\
    }							\
  }
#define SYNTAX_NORMAL4(id, mne, opc, op0, op1, op2, op3, arch)\
  {							\
    AVR32_SYNTAX_##id, arch,			\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],	\
    AVR32_PARSER_NORMAL,					\
    { &avr32_opc_table[AVR32_OPC_##opc], },			\
    NULL, 4,						\
    {							\
      AVR32_OPERAND_##op0, AVR32_OPERAND_##op1,		\
      AVR32_OPERAND_##op2, AVR32_OPERAND_##op3,		\
    }							\
  }
#define SYNTAX_NORMAL5(id, mne, opc, op0, op1, op2, op3, op4, arch)	\
  {								\
    AVR32_SYNTAX_##id, arch,				\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],		\
    AVR32_PARSER_NORMAL,						\
    { &avr32_opc_table[AVR32_OPC_##opc], },				\
    NULL, 5,							\
    {								\
      AVR32_OPERAND_##op0, AVR32_OPERAND_##op1,			\
      AVR32_OPERAND_##op2, AVR32_OPERAND_##op3,			\
      AVR32_OPERAND_##op4,					\
    }								\
  }

#define SYNTAX_NORMAL_C1(id, mne, opc, nxt, op0, arch)	\
  {							\
    AVR32_SYNTAX_##id, arch,			\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],	\
    AVR32_PARSER_NORMAL,					\
    { &avr32_opc_table[AVR32_OPC_##opc], },			\
    &avr32_syntax_table[AVR32_SYNTAX_##nxt], 1,		\
    {							\
      AVR32_OPERAND_##op0,				\
    }							\
  }
#define SYNTAX_NORMAL_CM1(id, mne, opc, nxt, op0, arch)	\
  {							\
    AVR32_SYNTAX_##id, arch,			\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],	\
    AVR32_PARSER_NORMAL,					\
    { &avr32_opc_table[AVR32_OPC_##opc], },			\
    &avr32_syntax_table[AVR32_SYNTAX_##nxt], -1,	\
    {							\
      AVR32_OPERAND_##op0,				\
    }							\
  }
#define SYNTAX_NORMAL_C2(id, mne, opc, nxt, op0, op1, arch)	\
  {							\
    AVR32_SYNTAX_##id, arch,			\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],	\
    AVR32_PARSER_NORMAL,					\
    { &avr32_opc_table[AVR32_OPC_##opc], },			\
    &avr32_syntax_table[AVR32_SYNTAX_##nxt], 2,		\
    {							\
      AVR32_OPERAND_##op0, AVR32_OPERAND_##op1,		\
    }							\
  }
#define SYNTAX_NORMAL_CM2(id, mne, opc, nxt, op0, op1, arch)	\
  {							\
    AVR32_SYNTAX_##id, arch,			\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],	\
    AVR32_PARSER_NORMAL,					\
    { &avr32_opc_table[AVR32_OPC_##opc], },			\
    &avr32_syntax_table[AVR32_SYNTAX_##nxt], -2,	\
    {							\
      AVR32_OPERAND_##op0, AVR32_OPERAND_##op1,		\
    }							\
  }
#define SYNTAX_NORMAL_C3(id, mne, opc, nxt, op0, op1, op2, arch)	\
  {								\
    AVR32_SYNTAX_##id, arch,				\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],		\
    AVR32_PARSER_NORMAL,						\
    { &avr32_opc_table[AVR32_OPC_##opc], },				\
    &avr32_syntax_table[AVR32_SYNTAX_##nxt], 3,			\
    {								\
      AVR32_OPERAND_##op0, AVR32_OPERAND_##op1,			\
      AVR32_OPERAND_##op2,					\
    }								\
  }
#define SYNTAX_NORMAL_CM3(id, mne, opc, nxt, op0, op1, op2, arch)	\
  {								\
    AVR32_SYNTAX_##id, arch,				\
    &avr32_mnemonic_table[AVR32_MNEMONIC_##mne],		\
    AVR32_PARSER_NORMAL,						\
    { &avr32_opc_table[AVR32_OPC_##opc], },				\
    &avr32_syntax_table[AVR32_SYNTAX_##nxt], -3,		\
    {								\
      AVR32_OPERAND_##op0, AVR32_OPERAND_##op1,			\
      AVR32_OPERAND_##op2,					\
    }								\
  }


const struct avr32_syntax avr32_syntax_table[] =
  {
    SYNTAX_NORMAL1(ABS, ABS, ABS, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(ACALL, ACALL, ACALL, UNSIGNED_CONST_W, AVR32_V1),
    SYNTAX_NORMAL1(ACR, ACR, ACR, INTREG,AVR32_V1),
    SYNTAX_NORMAL3(ADC, ADC, ADC, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ADD1, ADD, ADD1, ADD2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(ADD2, ADD, ADD2, INTREG, INTREG, INTREG_LSL, AVR32_V1),
    SYNTAX_NORMAL3(ADDABS, ADDABS, ADDABS, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(ADDHH_W, ADDHH_W, ADDHH_W, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL_C2(AND1, AND, AND1, AND2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C3(AND2, AND, AND2, AND3, INTREG, INTREG, INTREG_LSL, AVR32_V1),
    SYNTAX_NORMAL3(AND3, AND, AND3, INTREG, INTREG, INTREG_LSR, AVR32_V1),
    SYNTAX_NORMAL_C2(ANDH, ANDH, ANDH, ANDH_COH, INTREG, UNSIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL3(ANDH_COH, ANDH, ANDH_COH, INTREG, UNSIGNED_CONST, COH, AVR32_V1),
    SYNTAX_NORMAL_C2(ANDL, ANDL, ANDL, ANDL_COH, INTREG, UNSIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL3(ANDL_COH, ANDL, ANDL_COH, INTREG, UNSIGNED_CONST, COH, AVR32_V1),
    SYNTAX_NORMAL2(ANDN, ANDN, ANDN, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C3(ASR1, ASR, ASR1, ASR3, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C3(ASR3, ASR, ASR3, ASR2, INTREG, INTREG, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL2(ASR2, ASR, ASR2, INTREG, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL4(BFEXTS, BFEXTS, BFEXTS, INTREG, INTREG, UNSIGNED_NUMBER, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL4(BFEXTU, BFEXTU, BFEXTU, INTREG, INTREG, UNSIGNED_NUMBER, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL4(BFINS, BFINS, BFINS, INTREG, INTREG, UNSIGNED_NUMBER, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL2(BLD, BLD, BLD, INTREG, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL_C1(BREQ1, BREQ, BREQ1, BREQ2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL_C1(BRNE1, BRNE, BRNE1, BRNE2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL_C1(BRCC1, BRCC, BRCC1, BRCC2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL_C1(BRCS1, BRCS, BRCS1, BRCS2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL_C1(BRGE1, BRGE, BRGE1, BRGE2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL_C1(BRLT1, BRLT, BRLT1, BRLT2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL_C1(BRMI1, BRMI, BRMI1, BRMI2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL_C1(BRPL1, BRPL, BRPL1, BRPL2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL_C1(BRHS1, BRHS, BRCC1, BRHS2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL_C1(BRLO1, BRLO, BRCS1, BRLO2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BREQ2, BREQ, BREQ2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRNE2, BRNE, BRNE2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRCC2, BRCC, BRCC2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRCS2, BRCS, BRCS2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRGE2, BRGE, BRGE2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRLT2, BRLT, BRLT2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRMI2, BRMI, BRMI2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRPL2, BRPL, BRPL2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRLS, BRLS, BRLS, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRGT, BRGT, BRGT, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRLE, BRLE, BRLE, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRHI, BRHI, BRHI, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRVS, BRVS, BRVS, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRVC, BRVC, BRVC, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRQS, BRQS, BRQS, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRAL, BRAL, BRAL, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRHS2, BRHS, BRCC2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(BRLO2, BRLO, BRCS2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL0(BREAKPOINT, BREAKPOINT, BREAKPOINT, AVR32_V1),
    SYNTAX_NORMAL1(BREV, BREV, BREV, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(BST, BST, BST, INTREG, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL2(CACHE, CACHE, CACHE, INTREG_SDISP, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL1(CASTS_B, CASTS_B, CASTS_B, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(CASTS_H, CASTS_H, CASTS_H, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(CASTU_B, CASTU_B, CASTU_B, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(CASTU_H, CASTU_H, CASTU_H, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(CBR, CBR, CBR, INTREG, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL2(CLZ, CLZ, CLZ, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(COM, COM, COM, INTREG, AVR32_V1),
    SYNTAX_NORMAL5(COP, COP, COP, CPNO, CPREG, CPREG, CPREG, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL2(CP_B, CP_B, CP_B, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(CP_H, CP_H, CP_H, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(CP_W1, CP_W, CP_W1, CP_W2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(CP_W2, CP_W, CP_W2, CP_W3, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(CP_W3, CP_W, CP_W3, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(CPC1, CPC, CPC1, CPC2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(CPC2, CPC, CPC2, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(CSRF, CSRF, CSRF, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL1(CSRFCZ, CSRFCZ, CSRFCZ, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL3(DIVS, DIVS, DIVS, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(DIVU, DIVU, DIVU, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(EOR1, EOR, EOR1, EOR2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C3(EOR2, EOR, EOR2, EOR3, INTREG, INTREG, INTREG_LSL, AVR32_V1),
    SYNTAX_NORMAL3(EOR3, EOR, EOR3, INTREG, INTREG, INTREG_LSR, AVR32_V1),
    SYNTAX_NORMAL2(EORL, EORL, EORL, INTREG, UNSIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(EORH, EORH, EORH, INTREG, UNSIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL0(FRS, FRS, FRS, AVR32_V1),
    SYNTAX_NORMAL0(SSCALL, SSCALL, SSCALL, AVR32_V3),
    SYNTAX_NORMAL0(RETSS, RETSS, RETSS, AVR32_V3),
    SYNTAX_NORMAL1(ICALL, ICALL, ICALL, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(INCJOSP, INCJOSP, INCJOSP, JOSPINC, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_D1, LD_D, LD_D1, LD_D2, DWREG, INTREG_POSTINC, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_D2, LD_D, LD_D2, LD_D3, DWREG, INTREG_PREDEC, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_D3, LD_D, LD_D3, LD_D5, DWREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_D5, LD_D, LD_D5, LD_D4, DWREG, INTREG_INDEX, AVR32_V1),
    SYNTAX_NORMAL2(LD_D4, LD_D, LD_D4, DWREG, INTREG_SDISP, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_SB2, LD_SB, LD_SB2, LD_SB1, INTREG, INTREG_INDEX, AVR32_V1),
    SYNTAX_NORMAL2(LD_SB1, LD_SB, LD_SB1, INTREG, INTREG_SDISP, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_UB1, LD_UB, LD_UB1, LD_UB2, INTREG, INTREG_POSTINC, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_UB2, LD_UB, LD_UB2, LD_UB5, INTREG, INTREG_PREDEC, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_UB5, LD_UB, LD_UB5, LD_UB3, INTREG, INTREG_INDEX, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_UB3, LD_UB, LD_UB3, LD_UB4, INTREG, INTREG_UDISP, AVR32_V1),
    SYNTAX_NORMAL2(LD_UB4, LD_UB, LD_UB4, INTREG, INTREG_SDISP, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_SH1, LD_SH, LD_SH1, LD_SH2, INTREG, INTREG_POSTINC, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_SH2, LD_SH, LD_SH2, LD_SH5, INTREG, INTREG_PREDEC, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_SH5, LD_SH, LD_SH5, LD_SH3, INTREG, INTREG_INDEX, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_SH3, LD_SH, LD_SH3, LD_SH4, INTREG, INTREG_UDISP_H, AVR32_V1),
    SYNTAX_NORMAL2(LD_SH4, LD_SH, LD_SH4, INTREG, INTREG_SDISP, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_UH1, LD_UH, LD_UH1, LD_UH2, INTREG, INTREG_POSTINC, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_UH2, LD_UH, LD_UH2, LD_UH5, INTREG, INTREG_PREDEC, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_UH5, LD_UH, LD_UH5, LD_UH3, INTREG, INTREG_INDEX, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_UH3, LD_UH, LD_UH3, LD_UH4, INTREG, INTREG_UDISP_H, AVR32_V1),
    SYNTAX_NORMAL2(LD_UH4, LD_UH, LD_UH4, INTREG, INTREG_SDISP, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_W1, LD_W, LD_W1, LD_W2, INTREG, INTREG_POSTINC, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_W2, LD_W, LD_W2, LD_W5, INTREG, INTREG_PREDEC, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_W5, LD_W, LD_W5, LD_W6, INTREG, INTREG_INDEX, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_W6, LD_W, LD_W6, LD_W3, INTREG, INTREG_XINDEX, AVR32_V1),
    SYNTAX_NORMAL_C2(LD_W3, LD_W, LD_W3, LD_W4, INTREG, INTREG_UDISP_W, AVR32_V1),
    SYNTAX_NORMAL2(LD_W4, LD_W, LD_W4, INTREG, INTREG_SDISP, AVR32_V1),
    SYNTAX_NORMAL3(LDC_D1, LDC_D, LDC_D1, CPNO, CPREG_D, INTREG_UDISP_W, AVR32_V1),
    SYNTAX_NORMAL_C3(LDC_D2, LDC_D, LDC_D2, LDC_D1, CPNO, CPREG_D, INTREG_PREDEC, AVR32_V1),
    SYNTAX_NORMAL_C3(LDC_D3, LDC_D, LDC_D3, LDC_D2, CPNO, CPREG_D, INTREG_INDEX, AVR32_V1),
    SYNTAX_NORMAL3(LDC_W1, LDC_W, LDC_W1, CPNO, CPREG, INTREG_UDISP_W, AVR32_V1),
    SYNTAX_NORMAL_C3(LDC_W2, LDC_W, LDC_W2, LDC_W1, CPNO, CPREG, INTREG_PREDEC, AVR32_V1),
    SYNTAX_NORMAL_C3(LDC_W3, LDC_W, LDC_W3, LDC_W2, CPNO, CPREG, INTREG_INDEX, AVR32_V1),
    SYNTAX_NORMAL2(LDC0_D, LDC0_D, LDC0_D, CPREG_D, INTREG_UDISP_W, AVR32_V1),
    SYNTAX_NORMAL2(LDC0_W, LDC0_W, LDC0_W, CPREG, INTREG_UDISP_W, AVR32_V1),
    SYNTAX_NORMAL_CM3(LDCM_D, LDCM_D, LDCM_D, LDCM_D_PU, CPNO, INTREG, REGLIST_CPD8, AVR32_V1),
    SYNTAX_NORMALM3(LDCM_D_PU, LDCM_D, LDCM_D_PU, CPNO, INTREG_POSTINC, REGLIST_CPD8, AVR32_V1),
    SYNTAX_NORMAL_CM3(LDCM_W, LDCM_W, LDCM_W, LDCM_W_PU, CPNO, INTREG, REGLIST_CP8, AVR32_V1),
    SYNTAX_NORMALM3(LDCM_W_PU, LDCM_W, LDCM_W_PU, CPNO, INTREG_POSTINC, REGLIST_CP8, AVR32_V1),
    SYNTAX_NORMAL2(LDDPC, LDDPC, LDDPC, INTREG, PC_UDISP_W, AVR32_V1),
    SYNTAX_NORMAL2(LDDPC_EXT, LDDPC, LDDPC_EXT, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(LDDSP, LDDSP, LDDSP, INTREG, SP_UDISP_W, AVR32_V1),
    SYNTAX_NORMAL2(LDINS_B, LDINS_B, LDINS_B, INTREG_BSEL, INTREG_SDISP, AVR32_V1),
    SYNTAX_NORMAL2(LDINS_H, LDINS_H, LDINS_H, INTREG_HSEL, INTREG_SDISP_H, AVR32_V1),
    SYNTAX_NORMALM1(LDM, LDM, LDM, REGLIST_LDM, AVR32_V1),
    SYNTAX_NORMAL_CM2(LDMTS, LDMTS, LDMTS, LDMTS_PU, INTREG, REGLIST16, AVR32_V1),
    SYNTAX_NORMALM2(LDMTS_PU, LDMTS, LDMTS_PU, INTREG_POSTINC, REGLIST16, AVR32_V1),
    SYNTAX_NORMAL2(LDSWP_SH, LDSWP_SH, LDSWP_SH, INTREG, INTREG_SDISP_H, AVR32_V1),
    SYNTAX_NORMAL2(LDSWP_UH, LDSWP_UH, LDSWP_UH, INTREG, INTREG_SDISP_H, AVR32_V1),
    SYNTAX_NORMAL2(LDSWP_W, LDSWP_W, LDSWP_W, INTREG, INTREG_SDISP_W, AVR32_V1),
    SYNTAX_NORMAL_C3(LSL1, LSL, LSL1, LSL3, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C3(LSL3, LSL, LSL3, LSL2, INTREG, INTREG, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL2(LSL2, LSL, LSL2, INTREG, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL_C3(LSR1, LSR, LSR1, LSR3, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C3(LSR3, LSR, LSR3, LSR2, INTREG, INTREG, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL2(LSR2, LSR, LSR2, INTREG, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL3(MAC, MAC, MAC, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(MACHH_D, MACHH_D, MACHH_D, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL3(MACHH_W, MACHH_W, MACHH_W, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL3(MACS_D, MACS_D, MACS_D, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(MACSATHH_W, MACSATHH_W, MACSATHH_W, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL3(MACUD, MACU_D, MACUD, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(MACWH_D, MACWH_D, MACWH_D, INTREG, INTREG, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL3(MAX, MAX, MAX, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(MCALL, MCALL, MCALL, MCALL, AVR32_V1),
    SYNTAX_NORMAL2(MFDR, MFDR, MFDR, INTREG, UNSIGNED_CONST_W, AVR32_V1),
    SYNTAX_NORMAL2(MFSR, MFSR, MFSR, INTREG, UNSIGNED_CONST_W, AVR32_V1),
    SYNTAX_NORMAL3(MIN, MIN, MIN, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOV3, MOV, MOV3, MOV1, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOV1, MOV, MOV1, MOV2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOV2, MOV, MOV2,INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVEQ1, MOVEQ, MOVEQ1, MOVEQ2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVNE1, MOVNE, MOVNE1, MOVNE2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVCC1, MOVCC, MOVCC1, MOVCC2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVCS1, MOVCS, MOVCS1, MOVCS2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVGE1, MOVGE, MOVGE1, MOVGE2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVLT1, MOVLT, MOVLT1, MOVLT2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVMI1, MOVMI, MOVMI1, MOVMI2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVPL1, MOVPL, MOVPL1, MOVPL2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVLS1, MOVLS, MOVLS1, MOVLS2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVGT1, MOVGT, MOVGT1, MOVGT2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVLE1, MOVLE, MOVLE1, MOVLE2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVHI1, MOVHI, MOVHI1, MOVHI2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVVS1, MOVVS, MOVVS1, MOVVS2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVVC1, MOVVC, MOVVC1, MOVVC2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVQS1, MOVQS, MOVQS1, MOVQS2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVAL1, MOVAL, MOVAL1, MOVAL2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVHS1, MOVHS, MOVCC1, MOVHS2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MOVLO1, MOVLO, MOVCS1, MOVLO2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(MOVEQ2, MOVEQ, MOVEQ2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVNE2, MOVNE, MOVNE2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVCC2, MOVCC, MOVCC2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVCS2, MOVCS, MOVCS2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVGE2, MOVGE, MOVGE2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVLT2, MOVLT, MOVLT2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVMI2, MOVMI, MOVMI2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVPL2, MOVPL, MOVPL2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVLS2, MOVLS, MOVLS2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVGT2, MOVGT, MOVGT2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVLE2, MOVLE, MOVLE2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVHI2, MOVHI, MOVHI2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVVS2, MOVVS, MOVVS2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVVC2, MOVVC, MOVVC2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVQS2, MOVQS, MOVQS2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVAL2, MOVAL, MOVAL2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVHS2, MOVHS, MOVCC2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MOVLO2, MOVLO, MOVCS2, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(MTDR, MTDR, MTDR, UNSIGNED_CONST_W, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(MTSR, MTSR, MTSR, UNSIGNED_CONST_W, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(MUL1, MUL, MUL1, MUL2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C3(MUL2, MUL, MUL2, MUL3, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(MUL3, MUL, MUL3, INTREG, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL3(MULHH_W, MULHH_W, MULHH_W, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL3(MULNHH_W, MULNHH_W, MULNHH_W, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL3(MULNWH_D, MULNWH_D, MULNWH_D, INTREG, INTREG, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL3(MULSD, MULS_D, MULSD, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(MULSATHH_H, MULSATHH_H, MULSATHH_H, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL3(MULSATHH_W, MULSATHH_W, MULSATHH_W, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL3(MULSATRNDHH_H, MULSATRNDHH_H, MULSATRNDHH_H, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL3(MULSATRNDWH_W, MULSATRNDWH_W, MULSATRNDWH_W, INTREG, INTREG, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL3(MULSATWH_W, MULSATWH_W, MULSATWH_W, INTREG, INTREG, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL3(MULU_D, MULU_D, MULU_D, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(MULWH_D, MULWH_D, MULWH_D, INTREG, INTREG, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL1(MUSFR, MUSFR, MUSFR, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(MUSTR, MUSTR, MUSTR, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(MVCR_D, MVCR_D, MVCR_D, CPNO, DWREG, CPREG_D, AVR32_V1),
    SYNTAX_NORMAL3(MVCR_W, MVCR_W, MVCR_W, CPNO, INTREG, CPREG, AVR32_V1),
    SYNTAX_NORMAL3(MVRC_D, MVRC_D, MVRC_D, CPNO, CPREG_D, DWREG, AVR32_V1),
    SYNTAX_NORMAL3(MVRC_W, MVRC_W, MVRC_W, CPNO, CPREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(NEG, NEG, NEG, INTREG, AVR32_V1),
    SYNTAX_NORMAL0(NOP, NOP, NOP, AVR32_V1),
    SYNTAX_NORMAL_C2(OR1, OR, OR1, OR2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C3(OR2, OR, OR2, OR3, INTREG, INTREG, INTREG_LSL, AVR32_V1),
    SYNTAX_NORMAL3(OR3, OR, OR3, INTREG, INTREG, INTREG_LSR, AVR32_V1),
    SYNTAX_NORMAL2(ORH, ORH, ORH, INTREG, UNSIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(ORL, ORL, ORL, INTREG, UNSIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(PABS_SB, PABS_SB, PABS_SB, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL2(PABS_SH, PABS_SH, PABS_SH, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PACKSH_SB, PACKSH_SB, PACKSH_SB, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PACKSH_UB, PACKSH_UB, PACKSH_UB, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PACKW_SH, PACKW_SH, PACKW_SH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PADD_B, PADD_B, PADD_B, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PADD_H, PADD_H, PADD_H, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDH_SH, PADDH_SH, PADDH_SH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDH_UB, PADDH_UB, PADDH_UB, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDS_SB, PADDS_SB, PADDS_SB, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDS_SH, PADDS_SH, PADDS_SH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDS_UB, PADDS_UB, PADDS_UB, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDS_UH, PADDS_UH, PADDS_UH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDSUB_H, PADDSUB_H, PADDSUB_H, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDSUBH_SH, PADDSUBH_SH, PADDSUBH_SH, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDSUBS_SH, PADDSUBS_SH, PADDSUBS_SH, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDSUBS_UH, PADDSUBS_UH, PADDSUBS_UH, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDX_H, PADDX_H, PADDX_H, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDXH_SH, PADDXH_SH, PADDXH_SH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDXS_SH, PADDXS_SH, PADDXS_SH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PADDXS_UH, PADDXS_UH, PADDXS_UH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PASR_B, PASR_B, PASR_B, INTREG, INTREG, UNSIGNED_NUMBER, AVR32_SIMD),
    SYNTAX_NORMAL3(PASR_H, PASR_H, PASR_H, INTREG, INTREG, UNSIGNED_NUMBER, AVR32_SIMD),
    SYNTAX_NORMAL3(PAVG_SH, PAVG_SH, PAVG_SH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PAVG_UB, PAVG_UB, PAVG_UB, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PLSL_B, PLSL_B, PLSL_B, INTREG, INTREG, UNSIGNED_NUMBER, AVR32_SIMD),
    SYNTAX_NORMAL3(PLSL_H, PLSL_H, PLSL_H, INTREG, INTREG, UNSIGNED_NUMBER, AVR32_SIMD),
    SYNTAX_NORMAL3(PLSR_B, PLSR_B, PLSR_B, INTREG, INTREG, UNSIGNED_NUMBER, AVR32_SIMD),
    SYNTAX_NORMAL3(PLSR_H, PLSR_H, PLSR_H, INTREG, INTREG, UNSIGNED_NUMBER, AVR32_SIMD),
    SYNTAX_NORMAL3(PMAX_SH, PMAX_SH, PMAX_SH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PMAX_UB, PMAX_UB, PMAX_UB, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PMIN_SH, PMIN_SH, PMIN_SH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PMIN_UB, PMIN_UB, PMIN_UB, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL0(POPJC, POPJC, POPJC, AVR32_V1),
    SYNTAX_NORMAL_CM1(POPM, POPM, POPM, POPM_E, REGLIST9, AVR32_V1),
    SYNTAX_NORMALM1(POPM_E, POPM, POPM_E, REGLIST16, AVR32_V1),
    SYNTAX_NORMAL1(PREF, PREF, PREF, INTREG_SDISP, AVR32_V1),
    SYNTAX_NORMAL3(PSAD, PSAD, PSAD, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUB_B, PSUB_B, PSUB_B, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUB_H, PSUB_H, PSUB_H, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBADD_H, PSUBADD_H, PSUBADD_H, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBADDH_SH, PSUBADDH_SH, PSUBADDH_SH, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBADDS_SH, PSUBADDS_SH, PSUBADDS_SH, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBADDS_UH, PSUBADDS_UH, PSUBADDS_UH, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBH_SH, PSUBH_SH, PSUBH_SH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBH_UB, PSUBH_UB, PSUBH_UB, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBS_SB, PSUBS_SB, PSUBS_SB, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBS_SH, PSUBS_SH, PSUBS_SH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBS_UB, PSUBS_UB, PSUBS_UB, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBS_UH, PSUBS_UH, PSUBS_UH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBX_H, PSUBX_H, PSUBX_H, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBXH_SH, PSUBXH_SH, PSUBXH_SH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBXS_SH, PSUBXS_SH, PSUBXS_SH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL3(PSUBXS_UH, PSUBXS_UH, PSUBXS_UH, INTREG, INTREG, INTREG, AVR32_SIMD),
    SYNTAX_NORMAL2(PUNPCKSB_H, PUNPCKSB_H, PUNPCKSB_H, INTREG, INTREG_HSEL, AVR32_SIMD),
    SYNTAX_NORMAL2(PUNPCKUB_H, PUNPCKUB_H, PUNPCKUB_H, INTREG, INTREG_HSEL, AVR32_SIMD),
    SYNTAX_NORMAL0(PUSHJC, PUSHJC, PUSHJC, AVR32_V1),
    SYNTAX_NORMAL_CM1(PUSHM, PUSHM, PUSHM, PUSHM_E, REGLIST8, AVR32_V1),
    SYNTAX_NORMALM1(PUSHM_E, PUSHM, PUSHM_E, REGLIST16, AVR32_V1),
    SYNTAX_NORMAL_C1(RCALL1, RCALL, RCALL1, RCALL2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(RCALL2, RCALL, RCALL2, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(RETEQ, RETEQ, RETEQ, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETNE, RETNE, RETNE, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETCC, RETCC, RETCC, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETCS, RETCS, RETCS, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETGE, RETGE, RETGE, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETLT, RETLT, RETLT, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETMI, RETMI, RETMI, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETPL, RETPL, RETPL, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETLS, RETLS, RETLS, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETGT, RETGT, RETGT, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETLE, RETLE, RETLE, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETHI, RETHI, RETHI, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETVS, RETVS, RETVS, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETVC, RETVC, RETVC, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETQS, RETQS, RETQS, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETAL, RETAL, RETAL, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETHS, RETHS, RETCC, RETVAL, AVR32_V1),
    SYNTAX_NORMAL1(RETLO, RETLO, RETCS, RETVAL, AVR32_V1),
    SYNTAX_NORMAL0(RETD, RETD, RETD, AVR32_V1),
    SYNTAX_NORMAL0(RETE, RETE, RETE, AVR32_V1),
    SYNTAX_NORMAL0(RETJ, RETJ, RETJ, AVR32_V1),
    SYNTAX_NORMAL0(RETS, RETS, RETS, AVR32_V1),
    SYNTAX_NORMAL1(RJMP, RJMP, RJMP, JMPLABEL, AVR32_V1),
    SYNTAX_NORMAL1(ROL, ROL, ROL, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(ROR, ROR, ROR, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(RSUB1, RSUB, RSUB1, RSUB2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(RSUB2, RSUB, RSUB2, INTREG, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL3(SATADD_H, SATADD_H, SATADD_H, INTREG, INTREG, INTREG,  AVR32_DSP),
    SYNTAX_NORMAL3(SATADD_W, SATADD_W, SATADD_W, INTREG, INTREG, INTREG, AVR32_DSP),
    SYNTAX_NORMAL2(SATRNDS, SATRNDS, SATRNDS, INTREG_LSR, UNSIGNED_NUMBER, AVR32_DSP),
    SYNTAX_NORMAL2(SATRNDU, SATRNDU, SATRNDU, INTREG_LSR, UNSIGNED_NUMBER, AVR32_DSP),
    SYNTAX_NORMAL2(SATS, SATS, SATS, INTREG_LSR, UNSIGNED_NUMBER, AVR32_DSP),
    SYNTAX_NORMAL3(SATSUB_H, SATSUB_H, SATSUB_H, INTREG, INTREG, INTREG, AVR32_DSP),
    SYNTAX_NORMAL_C3(SATSUB_W1, SATSUB_W, SATSUB_W1, SATSUB_W2, INTREG, INTREG, INTREG, AVR32_DSP),
    SYNTAX_NORMAL3(SATSUB_W2, SATSUB_W, SATSUB_W2, INTREG, INTREG, SIGNED_CONST, AVR32_DSP),
    SYNTAX_NORMAL2(SATU, SATU, SATU, INTREG_LSR, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL3(SBC, SBC, SBC, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(SBR, SBR, SBR, INTREG, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL0(SCALL, SCALL, SCALL, AVR32_V1),
    SYNTAX_NORMAL1(SCR, SCR, SCR, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SLEEP, SLEEP, SLEEP, UNSIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL1(SREQ, SREQ, SREQ, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRNE, SRNE, SRNE, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRCC, SRCC, SRCC, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRCS, SRCS, SRCS, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRGE, SRGE, SRGE, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRLT, SRLT, SRLT, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRMI, SRMI, SRMI, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRPL, SRPL, SRPL, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRLS, SRLS, SRLS, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRGT, SRGT, SRGT, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRLE, SRLE, SRLE, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRHI, SRHI, SRHI, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRVS, SRVS, SRVS, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRVC, SRVC, SRVC, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRQS, SRQS, SRQS, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRAL, SRAL, SRAL, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRHS, SRHS, SRCC, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SRLO, SRLO, SRCS, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SSRF, SSRF, SSRF, UNSIGNED_NUMBER, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_B1, ST_B, ST_B1, ST_B2, INTREG_POSTINC, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_B2, ST_B, ST_B2, ST_B5, INTREG_PREDEC, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_B5, ST_B, ST_B5, ST_B3, INTREG_INDEX, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_B3, ST_B, ST_B3, ST_B4, INTREG_UDISP, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(ST_B4, ST_B, ST_B4, INTREG_SDISP, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_D1, ST_D, ST_D1, ST_D2, INTREG_POSTINC, DWREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_D2, ST_D, ST_D2, ST_D3, INTREG_PREDEC, DWREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_D3, ST_D, ST_D3, ST_D5, INTREG, DWREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_D5, ST_D, ST_D5, ST_D4, INTREG_INDEX, DWREG, AVR32_V1),
    SYNTAX_NORMAL2(ST_D4, ST_D, ST_D4, INTREG_SDISP, DWREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_H1, ST_H, ST_H1, ST_H2, INTREG_POSTINC, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_H2, ST_H, ST_H2, ST_H5, INTREG_PREDEC, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_H5, ST_H, ST_H5, ST_H3, INTREG_INDEX, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_H3, ST_H, ST_H3, ST_H4, INTREG_UDISP_H, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(ST_H4, ST_H, ST_H4, INTREG_SDISP, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_W1, ST_W, ST_W1, ST_W2, INTREG_POSTINC, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_W2, ST_W, ST_W2, ST_W5, INTREG_PREDEC, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_W5, ST_W, ST_W5, ST_W3, INTREG_INDEX, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(ST_W3, ST_W, ST_W3, ST_W4, INTREG_UDISP_W, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(ST_W4, ST_W, ST_W4, INTREG_SDISP, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(STC_D1, STC_D, STC_D1, CPNO, INTREG_UDISP_W, CPREG_D, AVR32_V1),
    SYNTAX_NORMAL_C3(STC_D2, STC_D, STC_D2, STC_D1, CPNO, INTREG_POSTINC, CPREG_D, AVR32_V1),
    SYNTAX_NORMAL_C3(STC_D3, STC_D, STC_D3, STC_D2, CPNO, INTREG_INDEX, CPREG_D, AVR32_V1),
    SYNTAX_NORMAL3(STC_W1, STC_W, STC_W1, CPNO, INTREG_UDISP_W, CPREG, AVR32_V1),
    SYNTAX_NORMAL_C3(STC_W2, STC_W, STC_W2, STC_W1, CPNO, INTREG_POSTINC, CPREG, AVR32_V1),
    SYNTAX_NORMAL_C3(STC_W3, STC_W, STC_W3, STC_W2, CPNO, INTREG_INDEX, CPREG, AVR32_V1),
    SYNTAX_NORMAL2(STC0_D, STC0_D, STC0_D, INTREG_UDISP_W, CPREG_D, AVR32_V1),
    SYNTAX_NORMAL2(STC0_W, STC0_W, STC0_W, INTREG_UDISP_W, CPREG, AVR32_V1),
    SYNTAX_NORMAL_CM3(STCM_D, STCM_D, STCM_D, STCM_D_PU, CPNO, INTREG, REGLIST_CPD8, AVR32_V1),
    SYNTAX_NORMALM3(STCM_D_PU, STCM_D, STCM_D_PU, CPNO, INTREG_PREDEC, REGLIST_CPD8, AVR32_V1),
    SYNTAX_NORMAL_CM3(STCM_W, STCM_W, STCM_W, STCM_W_PU, CPNO, INTREG, REGLIST_CP8, AVR32_V1),
    SYNTAX_NORMALM3(STCM_W_PU, STCM_W, STCM_W_PU, CPNO, INTREG_PREDEC, REGLIST_CP8, AVR32_V1),
    SYNTAX_NORMAL2(STCOND, STCOND, STCOND, INTREG_SDISP, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(STDSP, STDSP, STDSP, SP_UDISP_W, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C3(STHH_W2, STHH_W, STHH_W2, STHH_W1, INTREG_INDEX, INTREG_HSEL, INTREG_HSEL, AVR32_V1),
    SYNTAX_NORMAL3(STHH_W1, STHH_W, STHH_W1, INTREG_UDISP_W, INTREG_HSEL, INTREG_HSEL, AVR32_V1),
    SYNTAX_NORMAL_CM2(STM, STM, STM, STM_PU, INTREG, REGLIST16, AVR32_V1),
    SYNTAX_NORMALM2(STM_PU, STM, STM_PU, INTREG_PREDEC, REGLIST16, AVR32_V1),
    SYNTAX_NORMAL_CM2(STMTS, STMTS, STMTS, STMTS_PU, INTREG, REGLIST16, AVR32_V1),
    SYNTAX_NORMALM2(STMTS_PU, STMTS, STMTS_PU, INTREG_PREDEC, REGLIST16, AVR32_V1),
    SYNTAX_NORMAL2(STSWP_H, STSWP_H, STSWP_H, INTREG_SDISP_H, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(STSWP_W, STSWP_W, STSWP_W, INTREG_SDISP_W, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C2(SUB1, SUB, SUB1, SUB2, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL_C3(SUB2, SUB, SUB2, SUB5, INTREG, INTREG, INTREG_LSL, AVR32_V1),
    SYNTAX_NORMAL_C3(SUB5, SUB, SUB5, SUB3_SP, INTREG, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUB3_SP, SUB, SUB3_SP, SUB3, SP, SIGNED_CONST_W, AVR32_V1),
    SYNTAX_NORMAL_C2(SUB3, SUB, SUB3, SUB4, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUB4, SUB, SUB4, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBEQ, SUBEQ, SUBEQ, SUB2EQ, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBNE, SUBNE, SUBNE, SUB2NE, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBCC, SUBCC, SUBCC, SUB2CC, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBCS, SUBCS, SUBCS, SUB2CS, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBGE, SUBGE, SUBGE, SUB2GE, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBLT, SUBLT, SUBLT, SUB2LT, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBMI, SUBMI, SUBMI, SUB2MI, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBPL, SUBPL, SUBPL, SUB2PL, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBLS, SUBLS, SUBLS, SUB2LS, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBGT, SUBGT, SUBGT, SUB2GT, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBLE, SUBLE, SUBLE, SUB2LE, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBHI, SUBHI, SUBHI, SUB2HI, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBVS, SUBVS, SUBVS, SUB2VS, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBVC, SUBVC, SUBVC, SUB2VC, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBQS, SUBQS, SUBQS, SUB2QS, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBAL, SUBAL, SUBAL, SUB2AL, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBHS, SUBHS, SUBCC, SUB2CC, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL_C2(SUBLO, SUBLO, SUBCS, SUB2CS, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFEQ, SUBFEQ, SUBFEQ, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFNE, SUBFNE, SUBFNE, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFCC, SUBFCC, SUBFCC, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFCS, SUBFCS, SUBFCS, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFGE, SUBFGE, SUBFGE, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFLT, SUBFLT, SUBFLT, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFMI, SUBFMI, SUBFMI, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFPL, SUBFPL, SUBFPL, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFLS, SUBFLS, SUBFLS, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFGT, SUBFGT, SUBFGT, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFLE, SUBFLE, SUBFLE, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFHI, SUBFHI, SUBFHI, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFVS, SUBFVS, SUBFVS, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFVC, SUBFVC, SUBFVC, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFQS, SUBFQS, SUBFQS, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFAL, SUBFAL, SUBFAL, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFHS, SUBFHS, SUBFCC, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(SUBFLO, SUBFLO, SUBFCS, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL3(SUBHH_W, SUBHH_W, SUBHH_W, INTREG, INTREG_HSEL, INTREG_HSEL, AVR32_DSP),
    SYNTAX_NORMAL1(SWAP_B, SWAP_B, SWAP_B, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SWAP_BH, SWAP_BH, SWAP_BH, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SWAP_H, SWAP_H, SWAP_H, INTREG, AVR32_V1),
    SYNTAX_NORMAL1(SYNC, SYNC, SYNC, UNSIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL0(TLBR, TLBR, TLBR, AVR32_V1),
    SYNTAX_NORMAL0(TLBS, TLBS, TLBS, AVR32_V1),
    SYNTAX_NORMAL0(TLBW, TLBW, TLBW, AVR32_V1),
    SYNTAX_NORMAL1(TNBZ, TNBZ, TNBZ, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(TST, TST, TST, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL3(XCHG, XCHG, XCHG, INTREG, INTREG, INTREG, AVR32_V1),
    SYNTAX_NORMAL2(MEMC, MEMC, MEMC, SIGNED_CONST_W, UNSIGNED_NUMBER, AVR32_RMW),
    SYNTAX_NORMAL2(MEMS, MEMS, MEMS, SIGNED_CONST_W, UNSIGNED_NUMBER, AVR32_RMW),
    SYNTAX_NORMAL2(MEMT, MEMT, MEMT, SIGNED_CONST_W, UNSIGNED_NUMBER, AVR32_RMW),
  SYNTAX_NORMAL4 (FMAC_S,  FMAC_S,  FMAC_S,  INTREG, INTREG, INTREG, INTREG,
                  AVR32_V3FP),
  SYNTAX_NORMAL4 (FNMAC_S, FNMAC_S, FNMAC_S, INTREG, INTREG, INTREG, INTREG, 
                  AVR32_V3FP),
  SYNTAX_NORMAL4 (FMSC_S,  FMSC_S,  FMSC_S,  INTREG, INTREG, INTREG, INTREG, 
                  AVR32_V3FP),
  SYNTAX_NORMAL4 (FNMSC_S, FNMSC_S, FNMSC_S, INTREG, INTREG, INTREG, INTREG, 
                  AVR32_V3FP),
  SYNTAX_NORMAL3 (FMUL_S,  FMUL_S,  FMUL_S,  INTREG, INTREG, INTREG, AVR32_V3FP),
  SYNTAX_NORMAL3 (FNMUL_S, FNMUL_S, FNMUL_S, INTREG, INTREG, INTREG, AVR32_V3FP),
  SYNTAX_NORMAL3 (FADD_S,  FADD_S,  FADD_S,  INTREG, INTREG, INTREG, AVR32_V3FP),
  SYNTAX_NORMAL3 (FSUB_S,  FSUB_S,  FSUB_S,  INTREG, INTREG, INTREG, AVR32_V3FP),
  SYNTAX_NORMAL2 (FCASTRS_SW, FCASTRS_SW, FCASTRS_SW, INTREG, INTREG, AVR32_V3FP),
  SYNTAX_NORMAL2 (FCASTRS_UW, FCASTRS_UW, FCASTRS_UW, INTREG, INTREG, AVR32_V3FP),
  SYNTAX_NORMAL2 (FCASTSW_S,  FCASTSW_S,  FCASTSW_S,  INTREG, INTREG, AVR32_V3FP),
  SYNTAX_NORMAL2 (FCASTUW_S,  FCASTUW_S,  FCASTUW_S,  INTREG, INTREG, AVR32_V3FP),
  SYNTAX_NORMAL2 (FCMP_S, FCMP_S, FCMP_S, INTREG, INTREG, AVR32_V3FP),
  SYNTAX_NORMAL1 (FCHK_S, FCHK_S, FCHK_S, INTREG, AVR32_V3FP),
  SYNTAX_NORMAL2 (FRCPA_S,   FRCPA_S,   FRCPA_S,   INTREG, INTREG, AVR32_V3FP),
  SYNTAX_NORMAL2 (FRSQRTA_S, FRSQRTA_S, FRSQRTA_S, INTREG, INTREG, AVR32_V3FP),
    {
      AVR32_SYNTAX_LDA_W,
      AVR32_V1, NULL, AVR32_PARSER_LDA,
      { NULL }, NULL,
      2,
      {
	AVR32_OPERAND_INTREG,
	AVR32_OPERAND_SIGNED_CONST,
      },
    },
    {
      AVR32_SYNTAX_CALL,
      AVR32_V1, NULL, AVR32_PARSER_CALL,
      { NULL }, NULL,
      1,
      {
	AVR32_OPERAND_JMPLABEL,
      },
    },
    {
      AVR32_SYNTAX_PICOSVMAC0,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOSVMAC], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOSVMAC0] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOSVMAC1], 4,
      {
	AVR32_OPERAND_PICO_OUT0,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOSVMAC1,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOSVMAC], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOSVMAC1] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOSVMAC2], 4,
      {
	AVR32_OPERAND_PICO_OUT1,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOSVMAC2,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOSVMAC], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOSVMAC2] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOSVMAC3], 4,
      {
	AVR32_OPERAND_PICO_OUT2,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOSVMAC3,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOSVMAC], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOSVMAC3] },
      NULL, 4,
      {
	AVR32_OPERAND_PICO_OUT3,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOSVMUL0,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOSVMUL], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOSVMUL0] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOSVMUL1], 4,
      {
	AVR32_OPERAND_PICO_OUT0,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOSVMUL1,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOSVMUL], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOSVMUL1] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOSVMUL2], 4,
      {
	AVR32_OPERAND_PICO_OUT1,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOSVMUL2,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOSVMUL], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOSVMUL2] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOSVMUL3], 4,
      {
	AVR32_OPERAND_PICO_OUT2,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOSVMUL3,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOSVMUL], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOSVMUL3] },
      NULL, 4,
      {
	AVR32_OPERAND_PICO_OUT3,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOVMAC0,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOVMAC], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOVMAC0] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOVMAC1], 4,
      {
	AVR32_OPERAND_PICO_OUT0,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOVMAC1,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOVMAC], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOVMAC1] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOVMAC2], 4,
      {
	AVR32_OPERAND_PICO_OUT1,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOVMAC2,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOVMAC], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOVMAC2] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOVMAC3], 4,
      {
	AVR32_OPERAND_PICO_OUT2,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOVMAC3,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOVMAC], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOVMAC3] },
      NULL, 4,
      {
	AVR32_OPERAND_PICO_OUT3,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOVMUL0,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOVMUL], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOVMUL0] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOVMUL1], 4,
      {
	AVR32_OPERAND_PICO_OUT0,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOVMUL1,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOVMUL], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOVMUL1] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOVMUL2], 4,
      {
	AVR32_OPERAND_PICO_OUT1,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOVMUL2,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOVMUL], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOVMUL2] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOVMUL3], 4,
      {
	AVR32_OPERAND_PICO_OUT2,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOVMUL3,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOVMUL], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOVMUL3] },
      NULL, 4,
      {
	AVR32_OPERAND_PICO_OUT3,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
	AVR32_OPERAND_PICO_IN,
      },
    },
    {
      AVR32_SYNTAX_PICOLD_D2,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOLD_D], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOLD_D2] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOLD_D3], 2,
      {
	AVR32_OPERAND_PICO_REG_D,
	AVR32_OPERAND_INTREG_PREDEC,
      },
    },
    {
      AVR32_SYNTAX_PICOLD_D3,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOLD_D], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOLD_D3] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOLD_D1], 2,
      {
	AVR32_OPERAND_PICO_REG_D,
	AVR32_OPERAND_INTREG_INDEX,
      },
    },
    {
      AVR32_SYNTAX_PICOLD_D1,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOLD_D], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOLD_D1] },
      NULL, 2,
      {
	AVR32_OPERAND_PICO_REG_D,
	AVR32_OPERAND_INTREG_UDISP_W,
      },
    },
    {
      AVR32_SYNTAX_PICOLD_W2,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOLD_W], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOLD_W2] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOLD_W3], 2,
      {
	AVR32_OPERAND_PICO_REG_W,
	AVR32_OPERAND_INTREG_PREDEC,
      },
    },
    {
      AVR32_SYNTAX_PICOLD_W3,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOLD_W], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOLD_W3] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOLD_W1], 2,
      {
	AVR32_OPERAND_PICO_REG_W,
	AVR32_OPERAND_INTREG_INDEX,
      },
    },
    {
      AVR32_SYNTAX_PICOLD_W1,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOLD_W], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOLD_W1] },
      NULL, 2,
      {
	AVR32_OPERAND_PICO_REG_W,
	AVR32_OPERAND_INTREG_UDISP_W,
      },
    },
    {
      AVR32_SYNTAX_PICOLDM_D,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOLDM_D], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOLDM_D] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOLDM_D_PU], -2,
      {
	AVR32_OPERAND_INTREG,
	AVR32_OPERAND_PICO_REGLIST_D,
      },
    },
    {
      AVR32_SYNTAX_PICOLDM_D_PU,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOLDM_D], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOLDM_D_PU] },
      NULL, -2,
      {
	AVR32_OPERAND_INTREG_POSTINC,
	AVR32_OPERAND_PICO_REGLIST_D,
      },
    },
    {
      AVR32_SYNTAX_PICOLDM_W,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOLDM_W], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOLDM_W] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOLDM_W_PU], -2,
      {
	AVR32_OPERAND_INTREG,
	AVR32_OPERAND_PICO_REGLIST_W,
      },
    },
    {
      AVR32_SYNTAX_PICOLDM_W_PU,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOLDM_W], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOLDM_W_PU] },
      NULL, -2,
      {
	AVR32_OPERAND_INTREG_POSTINC,
	AVR32_OPERAND_PICO_REGLIST_W,
      },
    },
    {
      AVR32_SYNTAX_PICOMV_D1,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOMV_D], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOMV_D1] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOMV_D2], 2,
      {
	AVR32_OPERAND_DWREG,
	AVR32_OPERAND_PICO_REG_D,
      },
    },
    {
      AVR32_SYNTAX_PICOMV_D2,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOMV_D], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOMV_D2] },
      NULL, 2,
      {
	AVR32_OPERAND_PICO_REG_D,
	AVR32_OPERAND_DWREG,
      },
    },
    {
      AVR32_SYNTAX_PICOMV_W1,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOMV_W], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOMV_W1] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOMV_W2], 2,
      {
	AVR32_OPERAND_INTREG,
	AVR32_OPERAND_PICO_REG_W,
      },
    },
    {
      AVR32_SYNTAX_PICOMV_W2,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOMV_W], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOMV_W2] },
      NULL, 2,
      {
	AVR32_OPERAND_PICO_REG_W,
	AVR32_OPERAND_INTREG,
      },
    },
    {
      AVR32_SYNTAX_PICOST_D2,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOST_D], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOST_D2] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOST_D3], 2,
      {
	AVR32_OPERAND_INTREG_POSTINC,
	AVR32_OPERAND_PICO_REG_D,
      },
    },
    {
      AVR32_SYNTAX_PICOST_D3,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOST_D], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOST_D3] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOST_D1], 2,
      {
	AVR32_OPERAND_INTREG_INDEX,
	AVR32_OPERAND_PICO_REG_D,
      },
    },
    {
      AVR32_SYNTAX_PICOST_D1,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOST_D], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOST_D1] },
      NULL, 2,
      {
	AVR32_OPERAND_INTREG_UDISP_W,
	AVR32_OPERAND_PICO_REG_D,
      },
    },
    {
      AVR32_SYNTAX_PICOST_W2,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOST_W], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOST_W2] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOST_W3], 2,
      {
	AVR32_OPERAND_INTREG_POSTINC,
	AVR32_OPERAND_PICO_REG_W,
      },
    },
    {
      AVR32_SYNTAX_PICOST_W3,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOST_W], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOST_W3] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOST_W1], 2,
      {
	AVR32_OPERAND_INTREG_INDEX,
	AVR32_OPERAND_PICO_REG_W,
      },
    },
    {
      AVR32_SYNTAX_PICOST_W1,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOST_W], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOST_W1] },
      NULL, 2,
      {
	AVR32_OPERAND_INTREG_UDISP_W,
	AVR32_OPERAND_PICO_REG_W,
      },
    },
    {
      AVR32_SYNTAX_PICOSTM_D,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOSTM_D], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOSTM_D] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOSTM_D_PU], -2,
      {
	AVR32_OPERAND_INTREG,
	AVR32_OPERAND_PICO_REGLIST_D,
      },
    },
    {
      AVR32_SYNTAX_PICOSTM_D_PU,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOSTM_D], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOSTM_D_PU] },
      NULL, -2,
      {
	AVR32_OPERAND_INTREG_PREDEC,
	AVR32_OPERAND_PICO_REGLIST_D,
      },
    },
    {
      AVR32_SYNTAX_PICOSTM_W,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOSTM_W], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOSTM_W] },
      &avr32_syntax_table[AVR32_SYNTAX_PICOSTM_W_PU], -2,
      {
	AVR32_OPERAND_INTREG,
	AVR32_OPERAND_PICO_REGLIST_W,
      },
    },
    {
      AVR32_SYNTAX_PICOSTM_W_PU,
      AVR32_PICO, &avr32_mnemonic_table[AVR32_MNEMONIC_PICOSTM_W], AVR32_PARSER_ALIAS,
      { .alias = &avr32_alias_table[AVR32_ALIAS_PICOSTM_W_PU] },
      NULL, -2,
      {
	AVR32_OPERAND_INTREG_PREDEC,
	AVR32_OPERAND_PICO_REGLIST_W,
      },
    },
    SYNTAX_NORMAL2(RSUBEQ, RSUBEQ, RSUBEQ, INTREG, SIGNED_CONST, AVR32_V1),
    SYNTAX_NORMAL2(RSUBNE, RSUBNE, RSUBNE, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBCC, RSUBCC, RSUBCC, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBCS, RSUBCS, RSUBCS, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBGE, RSUBGE, RSUBGE, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBLT, RSUBLT, RSUBLT, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBMI, RSUBMI, RSUBMI, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBPL, RSUBPL, RSUBPL, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBLS, RSUBLS, RSUBLS, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBGT, RSUBGT, RSUBGT, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBLE, RSUBLE, RSUBLE, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBHI, RSUBHI, RSUBHI, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBVS, RSUBVS, RSUBVS, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBVC, RSUBVC, RSUBVC, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBQS, RSUBQS, RSUBQS, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBAL, RSUBAL, RSUBAL, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBHS, RSUBHS, RSUBCC, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL2(RSUBLO, RSUBLO, RSUBCS, INTREG, SIGNED_CONST, AVR32_V2),
    SYNTAX_NORMAL3(ADDEQ, ADDEQ, ADDEQ, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDNE, ADDNE, ADDNE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDCC, ADDCC, ADDCC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDCS, ADDCS, ADDCS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDGE, ADDGE, ADDGE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDLT, ADDLT, ADDLT, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDMI, ADDMI, ADDMI, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDPL, ADDPL, ADDPL, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDLS, ADDLS, ADDLS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDGT, ADDGT, ADDGT, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDLE, ADDLE, ADDLE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDHI, ADDHI, ADDHI, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDVS, ADDVS, ADDVS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDVC, ADDVC, ADDVC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDQS, ADDQS, ADDQS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDAL, ADDAL, ADDAL, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDHS, ADDHS, ADDCC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ADDLO, ADDLO, ADDCS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2EQ, SUBEQ, SUB2EQ, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2NE, SUBNE, SUB2NE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2CC, SUBCC, SUB2CC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2CS, SUBCS, SUB2CS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2GE, SUBGE, SUB2GE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2LT, SUBLT, SUB2LT, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2MI, SUBMI, SUB2MI, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2PL, SUBPL, SUB2PL, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2LS, SUBLS, SUB2LS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2GT, SUBGT, SUB2GT, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2LE, SUBLE, SUB2LE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2HI, SUBHI, SUB2HI, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2VS, SUBVS, SUB2VS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2VC, SUBVC, SUB2VC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2QS, SUBQS, SUB2QS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2AL, SUBAL, SUB2AL, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2HS, SUBHS, SUB2CC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(SUB2LO, SUBLO, SUB2CS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDEQ, ANDEQ, ANDEQ, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDNE, ANDNE, ANDNE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDCC, ANDCC, ANDCC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDCS, ANDCS, ANDCS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDGE, ANDGE, ANDGE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDLT, ANDLT, ANDLT, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDMI, ANDMI, ANDMI, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDPL, ANDPL, ANDPL, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDLS, ANDLS, ANDLS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDGT, ANDGT, ANDGT, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDLE, ANDLE, ANDLE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDHI, ANDHI, ANDHI, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDVS, ANDVS, ANDVS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDVC, ANDVC, ANDVC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDQS, ANDQS, ANDQS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDAL, ANDAL, ANDAL, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDHS, ANDHS, ANDCC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ANDLO, ANDLO, ANDCS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(OREQ, OREQ, OREQ, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORNE, ORNE, ORNE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORCC, ORCC, ORCC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORCS, ORCS, ORCS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORGE, ORGE, ORGE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORLT, ORLT, ORLT, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORMI, ORMI, ORMI, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORPL, ORPL, ORPL, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORLS, ORLS, ORLS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORGT, ORGT, ORGT, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORLE, ORLE, ORLE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORHI, ORHI, ORHI, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORVS, ORVS, ORVS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORVC, ORVC, ORVC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORQS, ORQS, ORQS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORAL, ORAL, ORAL, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORHS, ORHS, ORCC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(ORLO, ORLO, ORCS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EOREQ, EOREQ, EOREQ, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORNE, EORNE, EORNE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORCC, EORCC, EORCC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORCS, EORCS, EORCS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORGE, EORGE, EORGE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORLT, EORLT, EORLT, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORMI, EORMI, EORMI, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORPL, EORPL, EORPL, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORLS, EORLS, EORLS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORGT, EORGT, EORGT, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORLE, EORLE, EORLE, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORHI, EORHI, EORHI, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORVS, EORVS, EORVS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORVC, EORVC, EORVC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORQS, EORQS, EORQS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORAL, EORAL, EORAL, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORHS, EORHS, EORCC, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL3(EORLO, EORLO, EORCS, INTREG, INTREG, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(LD_WEQ, LD_WEQ, LD_WEQ, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WNE, LD_WNE, LD_WNE, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WCC, LD_WCC, LD_WCC, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WCS, LD_WCS, LD_WCS, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WGE, LD_WGE, LD_WGE, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WLT, LD_WLT, LD_WLT, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WMI, LD_WMI, LD_WMI, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WPL, LD_WPL, LD_WPL, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WLS, LD_WLS, LD_WLS, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WGT, LD_WGT, LD_WGT, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WLE, LD_WLE, LD_WLE, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WHI, LD_WHI, LD_WHI, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WVS, LD_WVS, LD_WVS, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WVC, LD_WVC, LD_WVC, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WQS, LD_WQS, LD_WQS, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WAL, LD_WAL, LD_WAL, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WHS, LD_WHS, LD_WCC, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_WLO, LD_WLO, LD_WCS, INTREG, INTREG_UDISP_W, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHEQ, LD_SHEQ, LD_SHEQ, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHNE, LD_SHNE, LD_SHNE, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHCC, LD_SHCC, LD_SHCC, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHCS, LD_SHCS, LD_SHCS, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHGE, LD_SHGE, LD_SHGE, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHLT, LD_SHLT, LD_SHLT, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHMI, LD_SHMI, LD_SHMI, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHPL, LD_SHPL, LD_SHPL, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHLS, LD_SHLS, LD_SHLS, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHGT, LD_SHGT, LD_SHGT, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHLE, LD_SHLE, LD_SHLE, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHHI, LD_SHHI, LD_SHHI, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHVS, LD_SHVS, LD_SHVS, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHVC, LD_SHVC, LD_SHVC, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHQS, LD_SHQS, LD_SHQS, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHAL, LD_SHAL, LD_SHAL, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHHS, LD_SHHS, LD_SHCC, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SHLO, LD_SHLO, LD_SHCS, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHEQ, LD_UHEQ, LD_UHEQ, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHNE, LD_UHNE, LD_UHNE, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHCC, LD_UHCC, LD_UHCC, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHCS, LD_UHCS, LD_UHCS, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHGE, LD_UHGE, LD_UHGE, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHLT, LD_UHLT, LD_UHLT, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHMI, LD_UHMI, LD_UHMI, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHPL, LD_UHPL, LD_UHPL, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHLS, LD_UHLS, LD_UHLS, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHGT, LD_UHGT, LD_UHGT, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHLE, LD_UHLE, LD_UHLE, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHHI, LD_UHHI, LD_UHHI, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHVS, LD_UHVS, LD_UHVS, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHVC, LD_UHVC, LD_UHVC, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHQS, LD_UHQS, LD_UHQS, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHAL, LD_UHAL, LD_UHAL, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHHS, LD_UHHS, LD_UHCC, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_UHLO, LD_UHLO, LD_UHCS, INTREG, INTREG_UDISP_H, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBEQ, LD_SBEQ, LD_SBEQ, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBNE, LD_SBNE, LD_SBNE, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBCC, LD_SBCC, LD_SBCC, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBCS, LD_SBCS, LD_SBCS, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBGE, LD_SBGE, LD_SBGE, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBLT, LD_SBLT, LD_SBLT, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBMI, LD_SBMI, LD_SBMI, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBPL, LD_SBPL, LD_SBPL, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBLS, LD_SBLS, LD_SBLS, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBGT, LD_SBGT, LD_SBGT, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBLE, LD_SBLE, LD_SBLE, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBHI, LD_SBHI, LD_SBHI, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBVS, LD_SBVS, LD_SBVS, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBVC, LD_SBVC, LD_SBVC, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBQS, LD_SBQS, LD_SBQS, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBAL, LD_SBAL, LD_SBAL, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBHS, LD_SBHS, LD_SBCC, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_SBLO, LD_SBLO, LD_SBCS, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBEQ, LD_UBEQ, LD_UBEQ, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBNE, LD_UBNE, LD_UBNE, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBCC, LD_UBCC, LD_UBCC, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBCS, LD_UBCS, LD_UBCS, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBGE, LD_UBGE, LD_UBGE, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBLT, LD_UBLT, LD_UBLT, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBMI, LD_UBMI, LD_UBMI, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBPL, LD_UBPL, LD_UBPL, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBLS, LD_UBLS, LD_UBLS, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBGT, LD_UBGT, LD_UBGT, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBLE, LD_UBLE, LD_UBLE, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBHI, LD_UBHI, LD_UBHI, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBVS, LD_UBVS, LD_UBVS, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBVC, LD_UBVC, LD_UBVC, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBQS, LD_UBQS, LD_UBQS, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBAL, LD_UBAL, LD_UBAL, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBHS, LD_UBHS, LD_UBCC, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(LD_UBLO, LD_UBLO, LD_UBCS, INTREG, INTREG_UDISP, AVR32_V2),
    SYNTAX_NORMAL2(ST_WEQ, ST_WEQ, ST_WEQ, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WNE, ST_WNE, ST_WNE, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WCC, ST_WCC, ST_WCC, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WCS, ST_WCS, ST_WCS, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WGE, ST_WGE, ST_WGE, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WLT, ST_WLT, ST_WLT, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WMI, ST_WMI, ST_WMI, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WPL, ST_WPL, ST_WPL, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WLS, ST_WLS, ST_WLS, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WGT, ST_WGT, ST_WGT, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WLE, ST_WLE, ST_WLE, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WHI, ST_WHI, ST_WHI, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WVS, ST_WVS, ST_WVS, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WVC, ST_WVC, ST_WVC, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WQS, ST_WQS, ST_WQS, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WAL, ST_WAL, ST_WAL, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WHS, ST_WHS, ST_WCC, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_WLO, ST_WLO, ST_WCS, INTREG_UDISP_W, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HEQ, ST_HEQ, ST_HEQ, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HNE, ST_HNE, ST_HNE, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HCC, ST_HCC, ST_HCC, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HCS, ST_HCS, ST_HCS, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HGE, ST_HGE, ST_HGE, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HLT, ST_HLT, ST_HLT, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HMI, ST_HMI, ST_HMI, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HPL, ST_HPL, ST_HPL, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HLS, ST_HLS, ST_HLS, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HGT, ST_HGT, ST_HGT, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HLE, ST_HLE, ST_HLE, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HHI, ST_HHI, ST_HHI, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HVS, ST_HVS, ST_HVS, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HVC, ST_HVC, ST_HVC, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HQS, ST_HQS, ST_HQS, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HAL, ST_HAL, ST_HAL, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HHS, ST_HHS, ST_HCC, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_HLO, ST_HLO, ST_HCS, INTREG_UDISP_H, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BEQ, ST_BEQ, ST_BEQ, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BNE, ST_BNE, ST_BNE, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BCC, ST_BCC, ST_BCC, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BCS, ST_BCS, ST_BCS, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BGE, ST_BGE, ST_BGE, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BLT, ST_BLT, ST_BLT, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BMI, ST_BMI, ST_BMI, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BPL, ST_BPL, ST_BPL, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BLS, ST_BLS, ST_BLS, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BGT, ST_BGT, ST_BGT, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BLE, ST_BLE, ST_BLE, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BHI, ST_BHI, ST_BHI, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BVS, ST_BVS, ST_BVS, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BVC, ST_BVC, ST_BVC, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BQS, ST_BQS, ST_BQS, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BAL, ST_BAL, ST_BAL, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BHS, ST_BHS, ST_BCC, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(ST_BLO, ST_BLO, ST_BCS, INTREG_UDISP, INTREG, AVR32_V2),
    SYNTAX_NORMAL2(MOVH, MOVH, MOVH, INTREG, UNSIGNED_CONST, AVR32_V2),

  };

#define NORMAL_MNEMONIC(name, syntax, str)		\
  {							\
    AVR32_MNEMONIC_##name, str,				\
    &avr32_syntax_table[AVR32_SYNTAX_##syntax],		\
  }
#define FP_MNEMONIC(name, syntax, str)			\
  NORMAL_MNEMONIC(name##_S, syntax##_S, str ".s"),	\
  NORMAL_MNEMONIC(name##_D, syntax##_D, str ".d")

const struct avr32_mnemonic avr32_mnemonic_table[] =
  {
    NORMAL_MNEMONIC(ABS, ABS, "abs"),
    NORMAL_MNEMONIC(ACALL, ACALL, "acall"),
    NORMAL_MNEMONIC(ACR, ACR, "acr"),
    NORMAL_MNEMONIC(ADC, ADC, "adc"),
    NORMAL_MNEMONIC(ADD, ADD1, "add"),
    NORMAL_MNEMONIC(ADDABS, ADDABS, "addabs"),
    NORMAL_MNEMONIC(ADDHH_W, ADDHH_W, "addhh.w"),
    NORMAL_MNEMONIC(AND, AND1, "and"),
    NORMAL_MNEMONIC(ANDH, ANDH, "andh"),
    NORMAL_MNEMONIC(ANDL, ANDL, "andl"),
    NORMAL_MNEMONIC(ANDN, ANDN, "andn"),
    NORMAL_MNEMONIC(ASR, ASR1, "asr"),
    NORMAL_MNEMONIC(BFEXTS, BFEXTS, "bfexts"),
    NORMAL_MNEMONIC(BFEXTU, BFEXTU, "bfextu"),
    NORMAL_MNEMONIC(BFINS, BFINS, "bfins"),
    NORMAL_MNEMONIC(BLD, BLD, "bld"),
    NORMAL_MNEMONIC(BREQ, BREQ1, "breq"),
    NORMAL_MNEMONIC(BRNE, BRNE1, "brne"),
    NORMAL_MNEMONIC(BRCC, BRCC1, "brcc"),
    NORMAL_MNEMONIC(BRCS, BRCS1, "brcs"),
    NORMAL_MNEMONIC(BRGE, BRGE1, "brge"),
    NORMAL_MNEMONIC(BRLT, BRLT1, "brlt"),
    NORMAL_MNEMONIC(BRMI, BRMI1, "brmi"),
    NORMAL_MNEMONIC(BRPL, BRPL1, "brpl"),
    NORMAL_MNEMONIC(BRHS, BRHS1, "brhs"),
    NORMAL_MNEMONIC(BRLO, BRLO1, "brlo"),
    NORMAL_MNEMONIC(BRLS, BRLS, "brls"),
    NORMAL_MNEMONIC(BRGT, BRGT, "brgt"),
    NORMAL_MNEMONIC(BRLE, BRLE, "brle"),
    NORMAL_MNEMONIC(BRHI, BRHI, "brhi"),
    NORMAL_MNEMONIC(BRVS, BRVS, "brvs"),
    NORMAL_MNEMONIC(BRVC, BRVC, "brvc"),
    NORMAL_MNEMONIC(BRQS, BRQS, "brqs"),
    NORMAL_MNEMONIC(BRAL, BRAL, "bral"),
    NORMAL_MNEMONIC(BREAKPOINT, BREAKPOINT, "breakpoint"),
    NORMAL_MNEMONIC(BREV, BREV, "brev"),
    NORMAL_MNEMONIC(BST, BST, "bst"),
    NORMAL_MNEMONIC(CACHE, CACHE, "cache"),
    NORMAL_MNEMONIC(CASTS_B, CASTS_B, "casts.b"),
    NORMAL_MNEMONIC(CASTS_H, CASTS_H, "casts.h"),
    NORMAL_MNEMONIC(CASTU_B, CASTU_B, "castu.b"),
    NORMAL_MNEMONIC(CASTU_H, CASTU_H, "castu.h"),
    NORMAL_MNEMONIC(CBR, CBR, "cbr"),
    NORMAL_MNEMONIC(CLZ, CLZ, "clz"),
    NORMAL_MNEMONIC(COM, COM, "com"),
    NORMAL_MNEMONIC(COP, COP, "cop"),
    NORMAL_MNEMONIC(CP_B, CP_B, "cp.b"),
    NORMAL_MNEMONIC(CP_H, CP_H, "cp.h"),
    NORMAL_MNEMONIC(CP_W, CP_W1, "cp.w"),
    NORMAL_MNEMONIC(CP, CP_W1, "cp"),
    NORMAL_MNEMONIC(CPC, CPC1, "cpc"),
    NORMAL_MNEMONIC(CSRF, CSRF, "csrf"),
    NORMAL_MNEMONIC(CSRFCZ, CSRFCZ, "csrfcz"),
    NORMAL_MNEMONIC(DIVS, DIVS, "divs"),
    NORMAL_MNEMONIC(DIVU, DIVU, "divu"),
    NORMAL_MNEMONIC(EOR, EOR1, "eor"),
    NORMAL_MNEMONIC(EORL, EORL, "eorl"),
    NORMAL_MNEMONIC(EORH, EORH, "eorh"),
    NORMAL_MNEMONIC(FRS, FRS, "frs"),
    NORMAL_MNEMONIC(SSCALL, SSCALL, "sscall"),
    NORMAL_MNEMONIC(RETSS, RETSS, "retss"),
    NORMAL_MNEMONIC(ICALL, ICALL, "icall"),
    NORMAL_MNEMONIC(INCJOSP, INCJOSP, "incjosp"),
    NORMAL_MNEMONIC(LD_D, LD_D1, "ld.d"),
    NORMAL_MNEMONIC(LD_SB, LD_SB2, "ld.sb"),
    NORMAL_MNEMONIC(LD_UB, LD_UB1, "ld.ub"),
    NORMAL_MNEMONIC(LD_SH, LD_SH1, "ld.sh"),
    NORMAL_MNEMONIC(LD_UH, LD_UH1, "ld.uh"),
    NORMAL_MNEMONIC(LD_W, LD_W1, "ld.w"),
    NORMAL_MNEMONIC(LDC_D, LDC_D3, "ldc.d"),
    NORMAL_MNEMONIC(LDC_W, LDC_W3, "ldc.w"),
    NORMAL_MNEMONIC(LDC0_D, LDC0_D, "ldc0.d"),
    NORMAL_MNEMONIC(LDC0_W, LDC0_W, "ldc0.w"),
    NORMAL_MNEMONIC(LDCM_D, LDCM_D, "ldcm.d"),
    NORMAL_MNEMONIC(LDCM_W, LDCM_W, "ldcm.w"),
    NORMAL_MNEMONIC(LDDPC, LDDPC, "lddpc"),
    NORMAL_MNEMONIC(LDDSP, LDDSP, "lddsp"),
    NORMAL_MNEMONIC(LDINS_B, LDINS_B, "ldins.b"),
    NORMAL_MNEMONIC(LDINS_H, LDINS_H, "ldins.h"),
    NORMAL_MNEMONIC(LDM, LDM, "ldm"),
    NORMAL_MNEMONIC(LDMTS, LDMTS, "ldmts"),
    NORMAL_MNEMONIC(LDSWP_SH, LDSWP_SH, "ldswp.sh"),
    NORMAL_MNEMONIC(LDSWP_UH, LDSWP_UH, "ldswp.uh"),
    NORMAL_MNEMONIC(LDSWP_W, LDSWP_W, "ldswp.w"),
    NORMAL_MNEMONIC(LSL, LSL1, "lsl"),
    NORMAL_MNEMONIC(LSR, LSR1, "lsr"),
    NORMAL_MNEMONIC(MAC, MAC, "mac"),
    NORMAL_MNEMONIC(MACHH_D, MACHH_D, "machh.d"),
    NORMAL_MNEMONIC(MACHH_W, MACHH_W, "machh.w"),
    NORMAL_MNEMONIC(MACS_D, MACS_D, "macs.d"),
    NORMAL_MNEMONIC(MACSATHH_W, MACSATHH_W, "macsathh.w"),
    NORMAL_MNEMONIC(MACU_D, MACUD, "macu.d"),
    NORMAL_MNEMONIC(MACWH_D, MACWH_D, "macwh.d"),
    NORMAL_MNEMONIC(MAX, MAX, "max"),
    NORMAL_MNEMONIC(MCALL, MCALL, "mcall"),
    NORMAL_MNEMONIC(MFDR, MFDR, "mfdr"),
    NORMAL_MNEMONIC(MFSR, MFSR, "mfsr"),
    NORMAL_MNEMONIC(MIN, MIN, "min"),
    NORMAL_MNEMONIC(MOV, MOV3, "mov"),
    NORMAL_MNEMONIC(MOVEQ, MOVEQ1, "moveq"),
    NORMAL_MNEMONIC(MOVNE, MOVNE1, "movne"),
    NORMAL_MNEMONIC(MOVCC, MOVCC1, "movcc"),
    NORMAL_MNEMONIC(MOVCS, MOVCS1, "movcs"),
    NORMAL_MNEMONIC(MOVGE, MOVGE1, "movge"),
    NORMAL_MNEMONIC(MOVLT, MOVLT1, "movlt"),
    NORMAL_MNEMONIC(MOVMI, MOVMI1, "movmi"),
    NORMAL_MNEMONIC(MOVPL, MOVPL1, "movpl"),
    NORMAL_MNEMONIC(MOVLS, MOVLS1, "movls"),
    NORMAL_MNEMONIC(MOVGT, MOVGT1, "movgt"),
    NORMAL_MNEMONIC(MOVLE, MOVLE1, "movle"),
    NORMAL_MNEMONIC(MOVHI, MOVHI1, "movhi"),
    NORMAL_MNEMONIC(MOVVS, MOVVS1, "movvs"),
    NORMAL_MNEMONIC(MOVVC, MOVVC1, "movvc"),
    NORMAL_MNEMONIC(MOVQS, MOVQS1, "movqs"),
    NORMAL_MNEMONIC(MOVAL, MOVAL1, "moval"),
    NORMAL_MNEMONIC(MOVHS, MOVHS1, "movhs"),
    NORMAL_MNEMONIC(MOVLO, MOVLO1, "movlo"),
    NORMAL_MNEMONIC(MTDR, MTDR, "mtdr"),
    NORMAL_MNEMONIC(MTSR, MTSR, "mtsr"),
    NORMAL_MNEMONIC(MUL, MUL1, "mul"),
    NORMAL_MNEMONIC(MULHH_W, MULHH_W, "mulhh.w"),
    NORMAL_MNEMONIC(MULNHH_W, MULNHH_W, "mulnhh.w"),
    NORMAL_MNEMONIC(MULNWH_D, MULNWH_D, "mulnwh.d"),
    NORMAL_MNEMONIC(MULS_D, MULSD, "muls.d"),
    NORMAL_MNEMONIC(MULSATHH_H, MULSATHH_H, "mulsathh.h"),
    NORMAL_MNEMONIC(MULSATHH_W, MULSATHH_W, "mulsathh.w"),
    NORMAL_MNEMONIC(MULSATRNDHH_H, MULSATRNDHH_H, "mulsatrndhh.h"),
    NORMAL_MNEMONIC(MULSATRNDWH_W, MULSATRNDWH_W, "mulsatrndwh.w"),
    NORMAL_MNEMONIC(MULSATWH_W, MULSATWH_W, "mulsatwh.w"),
    NORMAL_MNEMONIC(MULU_D, MULU_D, "mulu.d"),
    NORMAL_MNEMONIC(MULWH_D, MULWH_D, "mulwh.d"),
    NORMAL_MNEMONIC(MUSFR, MUSFR, "musfr"),
    NORMAL_MNEMONIC(MUSTR, MUSTR, "mustr"),
    NORMAL_MNEMONIC(MVCR_D, MVCR_D, "mvcr.d"),
    NORMAL_MNEMONIC(MVCR_W, MVCR_W, "mvcr.w"),
    NORMAL_MNEMONIC(MVRC_D, MVRC_D, "mvrc.d"),
    NORMAL_MNEMONIC(MVRC_W, MVRC_W, "mvrc.w"),
    NORMAL_MNEMONIC(NEG, NEG, "neg"),
    NORMAL_MNEMONIC(NOP, NOP, "nop"),
    NORMAL_MNEMONIC(OR, OR1, "or"),
    NORMAL_MNEMONIC(ORH, ORH, "orh"),
    NORMAL_MNEMONIC(ORL, ORL, "orl"),
    NORMAL_MNEMONIC(PABS_SB, PABS_SB, "pabs.sb"),
    NORMAL_MNEMONIC(PABS_SH, PABS_SH, "pabs.sh"),
    NORMAL_MNEMONIC(PACKSH_SB, PACKSH_SB, "packsh.sb"),
    NORMAL_MNEMONIC(PACKSH_UB, PACKSH_UB, "packsh.ub"),
    NORMAL_MNEMONIC(PACKW_SH, PACKW_SH, "packw.sh"),
    NORMAL_MNEMONIC(PADD_B, PADD_B, "padd.b"),
    NORMAL_MNEMONIC(PADD_H, PADD_H, "padd.h"),
    NORMAL_MNEMONIC(PADDH_SH, PADDH_SH, "paddh.sh"),
    NORMAL_MNEMONIC(PADDH_UB, PADDH_UB, "paddh.ub"),
    NORMAL_MNEMONIC(PADDS_SB, PADDS_SB, "padds.sb"),
    NORMAL_MNEMONIC(PADDS_SH, PADDS_SH, "padds.sh"),
    NORMAL_MNEMONIC(PADDS_UB, PADDS_UB, "padds.ub"),
    NORMAL_MNEMONIC(PADDS_UH, PADDS_UH, "padds.uh"),
    NORMAL_MNEMONIC(PADDSUB_H, PADDSUB_H, "paddsub.h"),
    NORMAL_MNEMONIC(PADDSUBH_SH, PADDSUBH_SH, "paddsubh.sh"),
    NORMAL_MNEMONIC(PADDSUBS_SH, PADDSUBS_SH, "paddsubs.sh"),
    NORMAL_MNEMONIC(PADDSUBS_UH, PADDSUBS_UH, "paddsubs.uh"),
    NORMAL_MNEMONIC(PADDX_H, PADDX_H, "paddx.h"),
    NORMAL_MNEMONIC(PADDXH_SH, PADDXH_SH, "paddxh.sh"),
    NORMAL_MNEMONIC(PADDXS_SH, PADDXS_SH, "paddxs.sh"),
    NORMAL_MNEMONIC(PADDXS_UH, PADDXS_UH, "paddxs.uh"),
    NORMAL_MNEMONIC(PASR_B, PASR_B, "pasr.b"),
    NORMAL_MNEMONIC(PASR_H, PASR_H, "pasr.h"),
    NORMAL_MNEMONIC(PAVG_SH, PAVG_SH, "pavg.sh"),
    NORMAL_MNEMONIC(PAVG_UB, PAVG_UB, "pavg.ub"),
    NORMAL_MNEMONIC(PLSL_B, PLSL_B, "plsl.b"),
    NORMAL_MNEMONIC(PLSL_H, PLSL_H, "plsl.h"),
    NORMAL_MNEMONIC(PLSR_B, PLSR_B, "plsr.b"),
    NORMAL_MNEMONIC(PLSR_H, PLSR_H, "plsr.h"),
    NORMAL_MNEMONIC(PMAX_SH, PMAX_SH, "pmax.sh"),
    NORMAL_MNEMONIC(PMAX_UB, PMAX_UB, "pmax.ub"),
    NORMAL_MNEMONIC(PMIN_SH, PMIN_SH, "pmin.sh"),
    NORMAL_MNEMONIC(PMIN_UB, PMIN_UB, "pmin.ub"),
    NORMAL_MNEMONIC(POPJC, POPJC, "popjc"),
    NORMAL_MNEMONIC(POPM, POPM, "popm"),
    NORMAL_MNEMONIC(PREF, PREF, "pref"),
    NORMAL_MNEMONIC(PSAD, PSAD, "psad"),
    NORMAL_MNEMONIC(PSUB_B, PSUB_B, "psub.b"),
    NORMAL_MNEMONIC(PSUB_H, PSUB_H, "psub.h"),
    NORMAL_MNEMONIC(PSUBADD_H, PSUBADD_H, "psubadd.h"),
    NORMAL_MNEMONIC(PSUBADDH_SH, PSUBADDH_SH, "psubaddh.sh"),
    NORMAL_MNEMONIC(PSUBADDS_SH, PSUBADDS_SH, "psubadds.sh"),
    NORMAL_MNEMONIC(PSUBADDS_UH, PSUBADDS_UH, "psubadds.uh"),
    NORMAL_MNEMONIC(PSUBH_SH, PSUBH_SH, "psubh.sh"),
    NORMAL_MNEMONIC(PSUBH_UB, PSUBH_UB, "psubh.ub"),
    NORMAL_MNEMONIC(PSUBS_SB, PSUBS_SB, "psubs.sb"),
    NORMAL_MNEMONIC(PSUBS_SH, PSUBS_SH, "psubs.sh"),
    NORMAL_MNEMONIC(PSUBS_UB, PSUBS_UB, "psubs.ub"),
    NORMAL_MNEMONIC(PSUBS_UH, PSUBS_UH, "psubs.uh"),
    NORMAL_MNEMONIC(PSUBX_H, PSUBX_H, "psubx.h"),
    NORMAL_MNEMONIC(PSUBXH_SH, PSUBXH_SH, "psubxh.sh"),
    NORMAL_MNEMONIC(PSUBXS_SH, PSUBXS_SH, "psubxs.sh"),
    NORMAL_MNEMONIC(PSUBXS_UH, PSUBXS_UH, "psubxs.uh"),
    NORMAL_MNEMONIC(PUNPCKSB_H, PUNPCKSB_H, "punpcksb.h"),
    NORMAL_MNEMONIC(PUNPCKUB_H, PUNPCKUB_H, "punpckub.h"),
    NORMAL_MNEMONIC(PUSHJC, PUSHJC, "pushjc"),
    NORMAL_MNEMONIC(PUSHM, PUSHM, "pushm"),
    NORMAL_MNEMONIC(RCALL, RCALL1, "rcall"),
    NORMAL_MNEMONIC(RETEQ, RETEQ, "reteq"),
    NORMAL_MNEMONIC(RETNE, RETNE, "retne"),
    NORMAL_MNEMONIC(RETCC, RETCC, "retcc"),
    NORMAL_MNEMONIC(RETCS, RETCS, "retcs"),
    NORMAL_MNEMONIC(RETGE, RETGE, "retge"),
    NORMAL_MNEMONIC(RETLT, RETLT, "retlt"),
    NORMAL_MNEMONIC(RETMI, RETMI, "retmi"),
    NORMAL_MNEMONIC(RETPL, RETPL, "retpl"),
    NORMAL_MNEMONIC(RETLS, RETLS, "retls"),
    NORMAL_MNEMONIC(RETGT, RETGT, "retgt"),
    NORMAL_MNEMONIC(RETLE, RETLE, "retle"),
    NORMAL_MNEMONIC(RETHI, RETHI, "rethi"),
    NORMAL_MNEMONIC(RETVS, RETVS, "retvs"),
    NORMAL_MNEMONIC(RETVC, RETVC, "retvc"),
    NORMAL_MNEMONIC(RETQS, RETQS, "retqs"),
    NORMAL_MNEMONIC(RETAL, RETAL, "retal"),
    NORMAL_MNEMONIC(RETHS, RETHS, "reths"),
    NORMAL_MNEMONIC(RETLO, RETLO, "retlo"),
    NORMAL_MNEMONIC(RET, RETAL, "ret"),
    NORMAL_MNEMONIC(RETD, RETD, "retd"),
    NORMAL_MNEMONIC(RETE, RETE, "rete"),
    NORMAL_MNEMONIC(RETJ, RETJ, "retj"),
    NORMAL_MNEMONIC(RETS, RETS, "rets"),
    NORMAL_MNEMONIC(RJMP, RJMP, "rjmp"),
    NORMAL_MNEMONIC(ROL, ROL, "rol"),
    NORMAL_MNEMONIC(ROR, ROR, "ror"),
    NORMAL_MNEMONIC(RSUB, RSUB1, "rsub"),
    NORMAL_MNEMONIC(SATADD_H, SATADD_H, "satadd.h"),
    NORMAL_MNEMONIC(SATADD_W, SATADD_W, "satadd.w"),
    NORMAL_MNEMONIC(SATRNDS, SATRNDS, "satrnds"),
    NORMAL_MNEMONIC(SATRNDU, SATRNDU, "satrndu"),
    NORMAL_MNEMONIC(SATS, SATS, "sats"),
    NORMAL_MNEMONIC(SATSUB_H, SATSUB_H, "satsub.h"),
    NORMAL_MNEMONIC(SATSUB_W, SATSUB_W1, "satsub.w"),
    NORMAL_MNEMONIC(SATU, SATU, "satu"),
    NORMAL_MNEMONIC(SBC, SBC, "sbc"),
    NORMAL_MNEMONIC(SBR, SBR, "sbr"),
    NORMAL_MNEMONIC(SCALL, SCALL, "scall"),
    NORMAL_MNEMONIC(SCR, SCR, "scr"),
    NORMAL_MNEMONIC(SLEEP, SLEEP, "sleep"),
    NORMAL_MNEMONIC(SREQ, SREQ, "sreq"),
    NORMAL_MNEMONIC(SRNE, SRNE, "srne"),
    NORMAL_MNEMONIC(SRCC, SRCC, "srcc"),
    NORMAL_MNEMONIC(SRCS, SRCS, "srcs"),
    NORMAL_MNEMONIC(SRGE, SRGE, "srge"),
    NORMAL_MNEMONIC(SRLT, SRLT, "srlt"),
    NORMAL_MNEMONIC(SRMI, SRMI, "srmi"),
    NORMAL_MNEMONIC(SRPL, SRPL, "srpl"),
    NORMAL_MNEMONIC(SRLS, SRLS, "srls"),
    NORMAL_MNEMONIC(SRGT, SRGT, "srgt"),
    NORMAL_MNEMONIC(SRLE, SRLE, "srle"),
    NORMAL_MNEMONIC(SRHI, SRHI, "srhi"),
    NORMAL_MNEMONIC(SRVS, SRVS, "srvs"),
    NORMAL_MNEMONIC(SRVC, SRVC, "srvc"),
    NORMAL_MNEMONIC(SRQS, SRQS, "srqs"),
    NORMAL_MNEMONIC(SRAL, SRAL, "sral"),
    NORMAL_MNEMONIC(SRHS, SRHS, "srhs"),
    NORMAL_MNEMONIC(SRLO, SRLO, "srlo"),
    NORMAL_MNEMONIC(SSRF, SSRF, "ssrf"),
    NORMAL_MNEMONIC(ST_B, ST_B1, "st.b"),
    NORMAL_MNEMONIC(ST_D, ST_D1, "st.d"),
    NORMAL_MNEMONIC(ST_H, ST_H1, "st.h"),
    NORMAL_MNEMONIC(ST_W, ST_W1, "st.w"),
    NORMAL_MNEMONIC(STC_D, STC_D3, "stc.d"),
    NORMAL_MNEMONIC(STC_W, STC_W3, "stc.w"),
    NORMAL_MNEMONIC(STC0_D, STC0_D, "stc0.d"),
    NORMAL_MNEMONIC(STC0_W, STC0_W, "stc0.w"),
    NORMAL_MNEMONIC(STCM_D, STCM_D, "stcm.d"),
    NORMAL_MNEMONIC(STCM_W, STCM_W, "stcm.w"),
    NORMAL_MNEMONIC(STCOND, STCOND, "stcond"),
    NORMAL_MNEMONIC(STDSP, STDSP, "stdsp"),
    NORMAL_MNEMONIC(STHH_W, STHH_W2, "sthh.w"),
    NORMAL_MNEMONIC(STM, STM, "stm"),
    NORMAL_MNEMONIC(STMTS, STMTS, "stmts"),
    NORMAL_MNEMONIC(STSWP_H, STSWP_H, "stswp.h"),
    NORMAL_MNEMONIC(STSWP_W, STSWP_W, "stswp.w"),
    NORMAL_MNEMONIC(SUB, SUB1, "sub"),
    NORMAL_MNEMONIC(SUBEQ, SUBEQ, "subeq"),
    NORMAL_MNEMONIC(SUBNE, SUBNE, "subne"),
    NORMAL_MNEMONIC(SUBCC, SUBCC, "subcc"),
    NORMAL_MNEMONIC(SUBCS, SUBCS, "subcs"),
    NORMAL_MNEMONIC(SUBGE, SUBGE, "subge"),
    NORMAL_MNEMONIC(SUBLT, SUBLT, "sublt"),
    NORMAL_MNEMONIC(SUBMI, SUBMI, "submi"),
    NORMAL_MNEMONIC(SUBPL, SUBPL, "subpl"),
    NORMAL_MNEMONIC(SUBLS, SUBLS, "subls"),
    NORMAL_MNEMONIC(SUBGT, SUBGT, "subgt"),
    NORMAL_MNEMONIC(SUBLE, SUBLE, "suble"),
    NORMAL_MNEMONIC(SUBHI, SUBHI, "subhi"),
    NORMAL_MNEMONIC(SUBVS, SUBVS, "subvs"),
    NORMAL_MNEMONIC(SUBVC, SUBVC, "subvc"),
    NORMAL_MNEMONIC(SUBQS, SUBQS, "subqs"),
    NORMAL_MNEMONIC(SUBAL, SUBAL, "subal"),
    NORMAL_MNEMONIC(SUBHS, SUBHS, "subhs"),
    NORMAL_MNEMONIC(SUBLO, SUBLO, "sublo"),
    NORMAL_MNEMONIC(SUBFEQ, SUBFEQ, "subfeq"),
    NORMAL_MNEMONIC(SUBFNE, SUBFNE, "subfne"),
    NORMAL_MNEMONIC(SUBFCC, SUBFCC, "subfcc"),
    NORMAL_MNEMONIC(SUBFCS, SUBFCS, "subfcs"),
    NORMAL_MNEMONIC(SUBFGE, SUBFGE, "subfge"),
    NORMAL_MNEMONIC(SUBFLT, SUBFLT, "subflt"),
    NORMAL_MNEMONIC(SUBFMI, SUBFMI, "subfmi"),
    NORMAL_MNEMONIC(SUBFPL, SUBFPL, "subfpl"),
    NORMAL_MNEMONIC(SUBFLS, SUBFLS, "subfls"),
    NORMAL_MNEMONIC(SUBFGT, SUBFGT, "subfgt"),
    NORMAL_MNEMONIC(SUBFLE, SUBFLE, "subfle"),
    NORMAL_MNEMONIC(SUBFHI, SUBFHI, "subfhi"),
    NORMAL_MNEMONIC(SUBFVS, SUBFVS, "subfvs"),
    NORMAL_MNEMONIC(SUBFVC, SUBFVC, "subfvc"),
    NORMAL_MNEMONIC(SUBFQS, SUBFQS, "subfqs"),
    NORMAL_MNEMONIC(SUBFAL, SUBFAL, "subfal"),
    NORMAL_MNEMONIC(SUBFHS, SUBFHS, "subfhs"),
    NORMAL_MNEMONIC(SUBFLO, SUBFLO, "subflo"),
    NORMAL_MNEMONIC(SUBHH_W, SUBHH_W, "subhh.w"),
    NORMAL_MNEMONIC(SWAP_B, SWAP_B, "swap.b"),
    NORMAL_MNEMONIC(SWAP_BH, SWAP_BH, "swap.bh"),
    NORMAL_MNEMONIC(SWAP_H, SWAP_H, "swap.h"),
    NORMAL_MNEMONIC(SYNC, SYNC, "sync"),
    NORMAL_MNEMONIC(TLBR, TLBR, "tlbr"),
    NORMAL_MNEMONIC(TLBS, TLBS, "tlbs"),
    NORMAL_MNEMONIC(TLBW, TLBW, "tlbw"),
    NORMAL_MNEMONIC(TNBZ, TNBZ, "tnbz"),
    NORMAL_MNEMONIC(TST, TST, "tst"),
    NORMAL_MNEMONIC(XCHG, XCHG, "xchg"),
    NORMAL_MNEMONIC(MEMC, MEMC, "memc"),
    NORMAL_MNEMONIC(MEMS, MEMS, "mems"),
    NORMAL_MNEMONIC(MEMT, MEMT, "memt"),
  NORMAL_MNEMONIC (FMAC_S,     FMAC_S,     "fmac.s"),
  NORMAL_MNEMONIC (FNMAC_S,    FNMAC_S,    "fnmac.s"),
  NORMAL_MNEMONIC (FMSC_S,     FMSC_S,     "fmsc.s"), 
  NORMAL_MNEMONIC (FNMSC_S,    FNMSC_S,    "fnmsc.s"), 
  NORMAL_MNEMONIC (FMUL_S,     FMUL_S,     "fmul.s"),
  NORMAL_MNEMONIC (FNMUL_S,    FNMUL_S,    "fnmul.s"),
  NORMAL_MNEMONIC (FADD_S,     FADD_S,     "fadd.s"),
  NORMAL_MNEMONIC (FSUB_S,     FSUB_S,     "fsub.s"),
  NORMAL_MNEMONIC (FCASTRS_SW, FCASTRS_SW, "fcastrs.sw"),
  NORMAL_MNEMONIC (FCASTRS_UW, FCASTRS_UW, "fcastrs.uw"),
  NORMAL_MNEMONIC (FCASTSW_S,  FCASTSW_S,  "fcastsw.s"),
  NORMAL_MNEMONIC (FCASTUW_S,  FCASTUW_S,  "fcastuw.s"),
  NORMAL_MNEMONIC (FCMP_S,     FCMP_S,     "fcmp.s"),
  NORMAL_MNEMONIC (FCHK_S,     FCHK_S,     "fchk.s"),
  NORMAL_MNEMONIC (FRCPA_S,    FRCPA_S,    "frcpa.s"),
  NORMAL_MNEMONIC (FRSQRTA_S,  FRSQRTA_S,  "frsqrta.s"),
    NORMAL_MNEMONIC(LDA_W, LDA_W, "lda.w"),
    NORMAL_MNEMONIC(CALL, CALL, "call"),
    NORMAL_MNEMONIC(PICOSVMAC, PICOSVMAC0, "picosvmac"),
    NORMAL_MNEMONIC(PICOSVMUL, PICOSVMUL0, "picosvmul"),
    NORMAL_MNEMONIC(PICOVMAC, PICOVMAC0, "picovmac"),
    NORMAL_MNEMONIC(PICOVMUL, PICOVMUL0, "picovmul"),
    NORMAL_MNEMONIC(PICOLD_D, PICOLD_D2, "picold.d"),
    NORMAL_MNEMONIC(PICOLD_W, PICOLD_W2, "picold.w"),
    NORMAL_MNEMONIC(PICOLDM_D, PICOLDM_D, "picoldm.d"),
    NORMAL_MNEMONIC(PICOLDM_W, PICOLDM_W, "picoldm.w"),
    NORMAL_MNEMONIC(PICOMV_D, PICOMV_D1, "picomv.d"),
    NORMAL_MNEMONIC(PICOMV_W, PICOMV_W1, "picomv.w"),
    NORMAL_MNEMONIC(PICOST_D, PICOST_D2, "picost.d"),
    NORMAL_MNEMONIC(PICOST_W, PICOST_W2, "picost.w"),
    NORMAL_MNEMONIC(PICOSTM_D, PICOSTM_D, "picostm.d"),
    NORMAL_MNEMONIC(PICOSTM_W, PICOSTM_W, "picostm.w"),
    NORMAL_MNEMONIC(RSUBEQ, RSUBEQ, "rsubeq"),
    NORMAL_MNEMONIC(RSUBNE, RSUBNE, "rsubne"),
    NORMAL_MNEMONIC(RSUBCC, RSUBCC, "rsubcc"),
    NORMAL_MNEMONIC(RSUBCS, RSUBCS, "rsubcs"),
    NORMAL_MNEMONIC(RSUBGE, RSUBGE, "rsubge"),
    NORMAL_MNEMONIC(RSUBLT, RSUBLT, "rsublt"),
    NORMAL_MNEMONIC(RSUBMI, RSUBMI, "rsubmi"),
    NORMAL_MNEMONIC(RSUBPL, RSUBPL, "rsubpl"),
    NORMAL_MNEMONIC(RSUBLS, RSUBLS, "rsubls"),
    NORMAL_MNEMONIC(RSUBGT, RSUBGT, "rsubgt"),
    NORMAL_MNEMONIC(RSUBLE, RSUBLE, "rsuble"),
    NORMAL_MNEMONIC(RSUBHI, RSUBHI, "rsubhi"),
    NORMAL_MNEMONIC(RSUBVS, RSUBVS, "rsubvs"),
    NORMAL_MNEMONIC(RSUBVC, RSUBVC, "rsubvc"),
    NORMAL_MNEMONIC(RSUBQS, RSUBQS, "rsubqs"),
    NORMAL_MNEMONIC(RSUBAL, RSUBAL, "rsubal"),
    NORMAL_MNEMONIC(RSUBHS, RSUBHS, "rsubhs"),
    NORMAL_MNEMONIC(RSUBLO, RSUBLO, "rsublo"),
    NORMAL_MNEMONIC(ADDEQ, ADDEQ, "addeq"),
    NORMAL_MNEMONIC(ADDNE, ADDNE, "addne"),
    NORMAL_MNEMONIC(ADDCC, ADDCC, "addcc"),
    NORMAL_MNEMONIC(ADDCS, ADDCS, "addcs"),
    NORMAL_MNEMONIC(ADDGE, ADDGE, "addge"),
    NORMAL_MNEMONIC(ADDLT, ADDLT, "addlt"),
    NORMAL_MNEMONIC(ADDMI, ADDMI, "addmi"),
    NORMAL_MNEMONIC(ADDPL, ADDPL, "addpl"),
    NORMAL_MNEMONIC(ADDLS, ADDLS, "addls"),
    NORMAL_MNEMONIC(ADDGT, ADDGT, "addgt"),
    NORMAL_MNEMONIC(ADDLE, ADDLE, "addle"),
    NORMAL_MNEMONIC(ADDHI, ADDHI, "addhi"),
    NORMAL_MNEMONIC(ADDVS, ADDVS, "addvs"),
    NORMAL_MNEMONIC(ADDVC, ADDVC, "addvc"),
    NORMAL_MNEMONIC(ADDQS, ADDQS, "addqs"),
    NORMAL_MNEMONIC(ADDAL, ADDAL, "addal"),
    NORMAL_MNEMONIC(ADDHS, ADDHS, "addhs"),
    NORMAL_MNEMONIC(ADDLO, ADDLO, "addlo"),
    NORMAL_MNEMONIC(ANDEQ, ANDEQ, "andeq"),
    NORMAL_MNEMONIC(ANDNE, ANDNE, "andne"),
    NORMAL_MNEMONIC(ANDCC, ANDCC, "andcc"),
    NORMAL_MNEMONIC(ANDCS, ANDCS, "andcs"),
    NORMAL_MNEMONIC(ANDGE, ANDGE, "andge"),
    NORMAL_MNEMONIC(ANDLT, ANDLT, "andlt"),
    NORMAL_MNEMONIC(ANDMI, ANDMI, "andmi"),
    NORMAL_MNEMONIC(ANDPL, ANDPL, "andpl"),
    NORMAL_MNEMONIC(ANDLS, ANDLS, "andls"),
    NORMAL_MNEMONIC(ANDGT, ANDGT, "andgt"),
    NORMAL_MNEMONIC(ANDLE, ANDLE, "andle"),
    NORMAL_MNEMONIC(ANDHI, ANDHI, "andhi"),
    NORMAL_MNEMONIC(ANDVS, ANDVS, "andvs"),
    NORMAL_MNEMONIC(ANDVC, ANDVC, "andvc"),
    NORMAL_MNEMONIC(ANDQS, ANDQS, "andqs"),
    NORMAL_MNEMONIC(ANDAL, ANDAL, "andal"),
    NORMAL_MNEMONIC(ANDHS, ANDHS, "andhs"),
    NORMAL_MNEMONIC(ANDLO, ANDLO, "andlo"),
    NORMAL_MNEMONIC(OREQ, OREQ, "oreq"),
    NORMAL_MNEMONIC(ORNE, ORNE, "orne"),
    NORMAL_MNEMONIC(ORCC, ORCC, "orcc"),
    NORMAL_MNEMONIC(ORCS, ORCS, "orcs"),
    NORMAL_MNEMONIC(ORGE, ORGE, "orge"),
    NORMAL_MNEMONIC(ORLT, ORLT, "orlt"),
    NORMAL_MNEMONIC(ORMI, ORMI, "ormi"),
    NORMAL_MNEMONIC(ORPL, ORPL, "orpl"),
    NORMAL_MNEMONIC(ORLS, ORLS, "orls"),
    NORMAL_MNEMONIC(ORGT, ORGT, "orgt"),
    NORMAL_MNEMONIC(ORLE, ORLE, "orle"),
    NORMAL_MNEMONIC(ORHI, ORHI, "orhi"),
    NORMAL_MNEMONIC(ORVS, ORVS, "orvs"),
    NORMAL_MNEMONIC(ORVC, ORVC, "orvc"),
    NORMAL_MNEMONIC(ORQS, ORQS, "orqs"),
    NORMAL_MNEMONIC(ORAL, ORAL, "oral"),
    NORMAL_MNEMONIC(ORHS, ORHS, "orhs"),
    NORMAL_MNEMONIC(ORLO, ORLO, "orlo"),
    NORMAL_MNEMONIC(EOREQ, EOREQ, "eoreq"),
    NORMAL_MNEMONIC(EORNE, EORNE, "eorne"),
    NORMAL_MNEMONIC(EORCC, EORCC, "eorcc"),
    NORMAL_MNEMONIC(EORCS, EORCS, "eorcs"),
    NORMAL_MNEMONIC(EORGE, EORGE, "eorge"),
    NORMAL_MNEMONIC(EORLT, EORLT, "eorlt"),
    NORMAL_MNEMONIC(EORMI, EORMI, "eormi"),
    NORMAL_MNEMONIC(EORPL, EORPL, "eorpl"),
    NORMAL_MNEMONIC(EORLS, EORLS, "eorls"),
    NORMAL_MNEMONIC(EORGT, EORGT, "eorgt"),
    NORMAL_MNEMONIC(EORLE, EORLE, "eorle"),
    NORMAL_MNEMONIC(EORHI, EORHI, "eorhi"),
    NORMAL_MNEMONIC(EORVS, EORVS, "eorvs"),
    NORMAL_MNEMONIC(EORVC, EORVC, "eorvc"),
    NORMAL_MNEMONIC(EORQS, EORQS, "eorqs"),
    NORMAL_MNEMONIC(EORAL, EORAL, "eoral"),
    NORMAL_MNEMONIC(EORHS, EORHS, "eorhs"),
    NORMAL_MNEMONIC(EORLO, EORLO, "eorlo"),
    NORMAL_MNEMONIC(LD_WEQ, LD_WEQ, "ld.weq"),
    NORMAL_MNEMONIC(LD_WNE, LD_WNE, "ld.wne"),
    NORMAL_MNEMONIC(LD_WCC, LD_WCC, "ld.wcc"),
    NORMAL_MNEMONIC(LD_WCS, LD_WCS, "ld.wcs"),
    NORMAL_MNEMONIC(LD_WGE, LD_WGE, "ld.wge"),
    NORMAL_MNEMONIC(LD_WLT, LD_WLT, "ld.wlt"),
    NORMAL_MNEMONIC(LD_WMI, LD_WMI, "ld.wmi"),
    NORMAL_MNEMONIC(LD_WPL, LD_WPL, "ld.wpl"),
    NORMAL_MNEMONIC(LD_WLS, LD_WLS, "ld.wls"),
    NORMAL_MNEMONIC(LD_WGT, LD_WGT, "ld.wgt"),
    NORMAL_MNEMONIC(LD_WLE, LD_WLE, "ld.wle"),
    NORMAL_MNEMONIC(LD_WHI, LD_WHI, "ld.whi"),
    NORMAL_MNEMONIC(LD_WVS, LD_WVS, "ld.wvs"),
    NORMAL_MNEMONIC(LD_WVC, LD_WVC, "ld.wvc"),
    NORMAL_MNEMONIC(LD_WQS, LD_WQS, "ld.wqs"),
    NORMAL_MNEMONIC(LD_WAL, LD_WAL, "ld.wal"),
    NORMAL_MNEMONIC(LD_WHS, LD_WHS, "ld.whs"),
    NORMAL_MNEMONIC(LD_WLO, LD_WLO, "ld.wlo"),
    NORMAL_MNEMONIC(LD_SHEQ, LD_SHEQ, "ld.sheq"),
    NORMAL_MNEMONIC(LD_SHNE, LD_SHNE, "ld.shne"),
    NORMAL_MNEMONIC(LD_SHCC, LD_SHCC, "ld.shcc"),
    NORMAL_MNEMONIC(LD_SHCS, LD_SHCS, "ld.shcs"),
    NORMAL_MNEMONIC(LD_SHGE, LD_SHGE, "ld.shge"),
    NORMAL_MNEMONIC(LD_SHLT, LD_SHLT, "ld.shlt"),
    NORMAL_MNEMONIC(LD_SHMI, LD_SHMI, "ld.shmi"),
    NORMAL_MNEMONIC(LD_SHPL, LD_SHPL, "ld.shpl"),
    NORMAL_MNEMONIC(LD_SHLS, LD_SHLS, "ld.shls"),
    NORMAL_MNEMONIC(LD_SHGT, LD_SHGT, "ld.shgt"),
    NORMAL_MNEMONIC(LD_SHLE, LD_SHLE, "ld.shle"),
    NORMAL_MNEMONIC(LD_SHHI, LD_SHHI, "ld.shhi"),
    NORMAL_MNEMONIC(LD_SHVS, LD_SHVS, "ld.shvs"),
    NORMAL_MNEMONIC(LD_SHVC, LD_SHVC, "ld.shvc"),
    NORMAL_MNEMONIC(LD_SHQS, LD_SHQS, "ld.shqs"),
    NORMAL_MNEMONIC(LD_SHAL, LD_SHAL, "ld.shal"),
    NORMAL_MNEMONIC(LD_SHHS, LD_SHHS, "ld.shhs"),
    NORMAL_MNEMONIC(LD_SHLO, LD_SHLO, "ld.shlo"),
    NORMAL_MNEMONIC(LD_UHEQ, LD_UHEQ, "ld.uheq"),
    NORMAL_MNEMONIC(LD_UHNE, LD_UHNE, "ld.uhne"),
    NORMAL_MNEMONIC(LD_UHCC, LD_UHCC, "ld.uhcc"),
    NORMAL_MNEMONIC(LD_UHCS, LD_UHCS, "ld.uhcs"),
    NORMAL_MNEMONIC(LD_UHGE, LD_UHGE, "ld.uhge"),
    NORMAL_MNEMONIC(LD_UHLT, LD_UHLT, "ld.uhlt"),
    NORMAL_MNEMONIC(LD_UHMI, LD_UHMI, "ld.uhmi"),
    NORMAL_MNEMONIC(LD_UHPL, LD_UHPL, "ld.uhpl"),
    NORMAL_MNEMONIC(LD_UHLS, LD_UHLS, "ld.uhls"),
    NORMAL_MNEMONIC(LD_UHGT, LD_UHGT, "ld.uhgt"),
    NORMAL_MNEMONIC(LD_UHLE, LD_UHLE, "ld.uhle"),
    NORMAL_MNEMONIC(LD_UHHI, LD_UHHI, "ld.uhhi"),
    NORMAL_MNEMONIC(LD_UHVS, LD_UHVS, "ld.uhvs"),
    NORMAL_MNEMONIC(LD_UHVC, LD_UHVC, "ld.uhvc"),
    NORMAL_MNEMONIC(LD_UHQS, LD_UHQS, "ld.uhqs"),
    NORMAL_MNEMONIC(LD_UHAL, LD_UHAL, "ld.uhal"),
    NORMAL_MNEMONIC(LD_UHHS, LD_UHHS, "ld.uhhs"),
    NORMAL_MNEMONIC(LD_UHLO, LD_UHLO, "ld.uhlo"),
    NORMAL_MNEMONIC(LD_SBEQ, LD_SBEQ, "ld.sbeq"),
    NORMAL_MNEMONIC(LD_SBNE, LD_SBNE, "ld.sbne"),
    NORMAL_MNEMONIC(LD_SBCC, LD_SBCC, "ld.sbcc"),
    NORMAL_MNEMONIC(LD_SBCS, LD_SBCS, "ld.sbcs"),
    NORMAL_MNEMONIC(LD_SBGE, LD_SBGE, "ld.sbge"),
    NORMAL_MNEMONIC(LD_SBLT, LD_SBLT, "ld.sblt"),
    NORMAL_MNEMONIC(LD_SBMI, LD_SBMI, "ld.sbmi"),
    NORMAL_MNEMONIC(LD_SBPL, LD_SBPL, "ld.sbpl"),
    NORMAL_MNEMONIC(LD_SBLS, LD_SBLS, "ld.sbls"),
    NORMAL_MNEMONIC(LD_SBGT, LD_SBGT, "ld.sbgt"),
    NORMAL_MNEMONIC(LD_SBLE, LD_SBLE, "ld.sble"),
    NORMAL_MNEMONIC(LD_SBHI, LD_SBHI, "ld.sbhi"),
    NORMAL_MNEMONIC(LD_SBVS, LD_SBVS, "ld.sbvs"),
    NORMAL_MNEMONIC(LD_SBVC, LD_SBVC, "ld.sbvc"),
    NORMAL_MNEMONIC(LD_SBQS, LD_SBQS, "ld.sbqs"),
    NORMAL_MNEMONIC(LD_SBAL, LD_SBAL, "ld.sbal"),
    NORMAL_MNEMONIC(LD_SBHS, LD_SBHS, "ld.sbhs"),
    NORMAL_MNEMONIC(LD_SBLO, LD_SBLO, "ld.sblo"),
    NORMAL_MNEMONIC(LD_UBEQ, LD_UBEQ, "ld.ubeq"),
    NORMAL_MNEMONIC(LD_UBNE, LD_UBNE, "ld.ubne"),
    NORMAL_MNEMONIC(LD_UBCC, LD_UBCC, "ld.ubcc"),
    NORMAL_MNEMONIC(LD_UBCS, LD_UBCS, "ld.ubcs"),
    NORMAL_MNEMONIC(LD_UBGE, LD_UBGE, "ld.ubge"),
    NORMAL_MNEMONIC(LD_UBLT, LD_UBLT, "ld.ublt"),
    NORMAL_MNEMONIC(LD_UBMI, LD_UBMI, "ld.ubmi"),
    NORMAL_MNEMONIC(LD_UBPL, LD_UBPL, "ld.ubpl"),
    NORMAL_MNEMONIC(LD_UBLS, LD_UBLS, "ld.ubls"),
    NORMAL_MNEMONIC(LD_UBGT, LD_UBGT, "ld.ubgt"),
    NORMAL_MNEMONIC(LD_UBLE, LD_UBLE, "ld.uble"),
    NORMAL_MNEMONIC(LD_UBHI, LD_UBHI, "ld.ubhi"),
    NORMAL_MNEMONIC(LD_UBVS, LD_UBVS, "ld.ubvs"),
    NORMAL_MNEMONIC(LD_UBVC, LD_UBVC, "ld.ubvc"),
    NORMAL_MNEMONIC(LD_UBQS, LD_UBQS, "ld.ubqs"),
    NORMAL_MNEMONIC(LD_UBAL, LD_UBAL, "ld.ubal"),
    NORMAL_MNEMONIC(LD_UBHS, LD_UBHS, "ld.ubhs"),
    NORMAL_MNEMONIC(LD_UBLO, LD_UBLO, "ld.ublo"),
    NORMAL_MNEMONIC(ST_WEQ, ST_WEQ, "st.weq"),
    NORMAL_MNEMONIC(ST_WNE, ST_WNE, "st.wne"),
    NORMAL_MNEMONIC(ST_WCC, ST_WCC, "st.wcc"),
    NORMAL_MNEMONIC(ST_WCS, ST_WCS, "st.wcs"),
    NORMAL_MNEMONIC(ST_WGE, ST_WGE, "st.wge"),
    NORMAL_MNEMONIC(ST_WLT, ST_WLT, "st.wlt"),
    NORMAL_MNEMONIC(ST_WMI, ST_WMI, "st.wmi"),
    NORMAL_MNEMONIC(ST_WPL, ST_WPL, "st.wpl"),
    NORMAL_MNEMONIC(ST_WLS, ST_WLS, "st.wls"),
    NORMAL_MNEMONIC(ST_WGT, ST_WGT, "st.wgt"),
    NORMAL_MNEMONIC(ST_WLE, ST_WLE, "st.wle"),
    NORMAL_MNEMONIC(ST_WHI, ST_WHI, "st.whi"),
    NORMAL_MNEMONIC(ST_WVS, ST_WVS, "st.wvs"),
    NORMAL_MNEMONIC(ST_WVC, ST_WVC, "st.wvc"),
    NORMAL_MNEMONIC(ST_WQS, ST_WQS, "st.wqs"),
    NORMAL_MNEMONIC(ST_WAL, ST_WAL, "st.wal"),
    NORMAL_MNEMONIC(ST_WHS, ST_WHS, "st.whs"),
    NORMAL_MNEMONIC(ST_WLO, ST_WLO, "st.wlo"),
    NORMAL_MNEMONIC(ST_HEQ, ST_HEQ, "st.heq"),
    NORMAL_MNEMONIC(ST_HNE, ST_HNE, "st.hne"),
    NORMAL_MNEMONIC(ST_HCC, ST_HCC, "st.hcc"),
    NORMAL_MNEMONIC(ST_HCS, ST_HCS, "st.hcs"),
    NORMAL_MNEMONIC(ST_HGE, ST_HGE, "st.hge"),
    NORMAL_MNEMONIC(ST_HLT, ST_HLT, "st.hlt"),
    NORMAL_MNEMONIC(ST_HMI, ST_HMI, "st.hmi"),
    NORMAL_MNEMONIC(ST_HPL, ST_HPL, "st.hpl"),
    NORMAL_MNEMONIC(ST_HLS, ST_HLS, "st.hls"),
    NORMAL_MNEMONIC(ST_HGT, ST_HGT, "st.hgt"),
    NORMAL_MNEMONIC(ST_HLE, ST_HLE, "st.hle"),
    NORMAL_MNEMONIC(ST_HHI, ST_HHI, "st.hhi"),
    NORMAL_MNEMONIC(ST_HVS, ST_HVS, "st.hvs"),
    NORMAL_MNEMONIC(ST_HVC, ST_HVC, "st.hvc"),
    NORMAL_MNEMONIC(ST_HQS, ST_HQS, "st.hqs"),
    NORMAL_MNEMONIC(ST_HAL, ST_HAL, "st.hal"),
    NORMAL_MNEMONIC(ST_HHS, ST_HHS, "st.hhs"),
    NORMAL_MNEMONIC(ST_HLO, ST_HLO, "st.hlo"),
    NORMAL_MNEMONIC(ST_BEQ, ST_BEQ, "st.beq"),
    NORMAL_MNEMONIC(ST_BNE, ST_BNE, "st.bne"),
    NORMAL_MNEMONIC(ST_BCC, ST_BCC, "st.bcc"),
    NORMAL_MNEMONIC(ST_BCS, ST_BCS, "st.bcs"),
    NORMAL_MNEMONIC(ST_BGE, ST_BGE, "st.bge"),
    NORMAL_MNEMONIC(ST_BLT, ST_BLT, "st.blt"),
    NORMAL_MNEMONIC(ST_BMI, ST_BMI, "st.bmi"),
    NORMAL_MNEMONIC(ST_BPL, ST_BPL, "st.bpl"),
    NORMAL_MNEMONIC(ST_BLS, ST_BLS, "st.bls"),
    NORMAL_MNEMONIC(ST_BGT, ST_BGT, "st.bgt"),
    NORMAL_MNEMONIC(ST_BLE, ST_BLE, "st.ble"),
    NORMAL_MNEMONIC(ST_BHI, ST_BHI, "st.bhi"),
    NORMAL_MNEMONIC(ST_BVS, ST_BVS, "st.bvs"),
    NORMAL_MNEMONIC(ST_BVC, ST_BVC, "st.bvc"),
    NORMAL_MNEMONIC(ST_BQS, ST_BQS, "st.bqs"),
    NORMAL_MNEMONIC(ST_BAL, ST_BAL, "st.bal"),
    NORMAL_MNEMONIC(ST_BHS, ST_BHS, "st.bhs"),
    NORMAL_MNEMONIC(ST_BLO, ST_BLO, "st.blo"),
    NORMAL_MNEMONIC(MOVH, MOVH, "movh"),

  };
#undef NORMAL_MNEMONIC
#undef ALIAS_MNEMONIC
#undef FP_MNEMONIC

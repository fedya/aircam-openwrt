/* Assembler interface for AVR32.
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

#include <string.h>

#include "avr32-opc.h"
#include "avr32-asm.h"

/* Structure for a register hash table entry.  */
struct reg_entry
{
  const char	*name;
  int		number;
};

/* Integer Registers.  */
static const struct reg_entry reg_table[] =
  {
    /* Primary names (used by the disassembler) */
    { "r0",   0 }, { "r1",   1 }, { "r2",   2 }, { "r3",   3 },
    { "r4",   4 }, { "r5",   5 }, { "r6",   6 }, { "r7",   7 },
    { "r8",   8 }, { "r9",   9 }, { "r10", 10 }, { "r11", 11 },
    { "r12", 12 }, { "sp",  13 }, { "lr",  14 }, { "pc",  15 },
    /* Alternatives to sp, lr and pc.  */
    { "r13", 13 }, { "r14", 14 }, { "r15", 15 },
  };
#define AVR32_NR_INTREGS (sizeof(reg_table)/sizeof(reg_table[0]))

/* Coprocessor Registers.  */
static const struct reg_entry cr_table[] =
  {
    { "cr0",   0 }, { "cr1",   1 }, { "cr2",   2 }, { "cr3",   3 },
    { "cr4",   4 }, { "cr5",   5 }, { "cr6",   6 }, { "cr7",   7 },
    { "cr8",   8 }, { "cr9",   9 }, { "cr10", 10 }, { "cr11", 11 },
    { "cr12", 12 }, { "cr13", 13 }, { "cr14", 14 }, { "cr15", 15 },
  };
#define AVR32_NR_CPREGS (sizeof(cr_table)/sizeof(cr_table[0]))

#define AVR32_NR_FPREGS (sizeof(fr_table)/sizeof(fr_table[0]))

/* PiCo Registers.  */
static const struct reg_entry pico_table[] =
  {
    { "inpix2",    0 }, { "inpix1",    1 }, { "inpix0",    2 },
    { "outpix2",   3 }, { "outpix1",   4 }, { "outpix0",   5 },
    { "coeff0_a",  6 }, { "coeff0_b",  7 }, { "coeff1_a",  8 },
    { "coeff1_b",  9 }, { "coeff2_a", 10 }, { "coeff2_b", 11 },
    { "vmu0_out", 12 }, { "vmu1_out", 13 }, { "vmu2_out", 14 },
    { "config",   15 },
  };
#define AVR32_NR_PICOREGS (sizeof(pico_table)/sizeof(pico_table[0]))

int
avr32_parse_intreg(const char *str)
{
  unsigned int i;

  for (i = 0; i < AVR32_NR_INTREGS; i++)
    {
      if (strcasecmp(reg_table[i].name, str) == 0)
	return reg_table[i].number;
    }

  return -1;
}

int
avr32_parse_cpreg(const char *str)
{
  unsigned int i;

  for (i = 0; i < AVR32_NR_CPREGS; i++)
    {
      if (strcasecmp(cr_table[i].name, str) == 0)
	return cr_table[i].number;
    }

  return -1;
}


int avr32_parse_picoreg(const char *str)
{
  unsigned int i;

  for (i = 0; i < AVR32_NR_PICOREGS; i++)
    {
      if (strcasecmp(pico_table[i].name, str) == 0)
	return pico_table[i].number;
    }

  return -1;
}

static unsigned long
parse_reglist(char *str, char **endptr, int (*parse_reg)(const char *))
{
  int reg_from, reg_to;
  unsigned long result = 0;
  char *p1, *p2, c;

  while (*str)
    {
      for (p1 = str; *p1; p1++)
	if (*p1 == ',' || *p1 == '-')
	  break;

      c = *p1, *p1 = 0;
      reg_from = parse_reg(str);
      *p1 = c;

      if (reg_from < 0)
	break;

      if (*p1 == '-')
	{
	  for (p2 = ++p1; *p2; p2++)
	    if (*p2 == ',')
	      break;

	  c = *p2, *p2 = 0;
	  /* printf("going to parse reg_to from `%s'\n", p1); */
	  reg_to = parse_reg(p1);
	  *p2 = c;

	  if (reg_to < 0)
	    break;

	  while (reg_from <= reg_to)
	    result |= (1 << reg_from++);
	  p1 = p2;
	}
      else
	result |= (1 << reg_from);

      str = p1;
      if (*str) ++str;
    }

  if (endptr)
    *endptr = str;

  return result;
}

unsigned long
avr32_parse_reglist(char *str, char **endptr)
{
  return parse_reglist(str, endptr, avr32_parse_intreg);
}

unsigned long
avr32_parse_cpreglist(char *str, char **endptr)
{
  return parse_reglist(str, endptr, avr32_parse_cpreg);
}

unsigned long
avr32_parse_pico_reglist(char *str, char **endptr)
{
  return parse_reglist(str, endptr, avr32_parse_picoreg);
}

int
avr32_make_regmask8(unsigned long regmask16, unsigned long *regmask8)
{
  unsigned long result = 0;

  /* printf("convert regmask16 0x%04lx\n", regmask16); */

  if (regmask16 & 0xf)
    {
      if ((regmask16 & 0xf) == 0xf)
	result |= 1 << 0;
      else
	return -1;
    }
  if (regmask16 & 0xf0)
    {
      if ((regmask16 & 0xf0) == 0xf0)
	result |= 1 << 1;
      else
	return -1;
    }
  if (regmask16 & 0x300)
    {
      if ((regmask16 & 0x300) == 0x300)
	result |= 1 << 2;
      else
	return -1;
    }
  if (regmask16 & (1 << 13))
    return -1;

  if (regmask16 & (1 << 10))
    result |= 1 << 3;
  if (regmask16 & (1 << 11))
    result |= 1 << 4;
  if (regmask16 & (1 << 12))
    result |= 1 << 5;
  if (regmask16 & (1 << 14))
    result |= 1 << 6;
  if (regmask16 & (1 << 15))
    result |= 1 << 7;

  *regmask8 = result;

  return 0;
}

#if 0
struct reg_map
{
  const struct reg_entry	*names;
  int				nr_regs;
  struct hash_control		*htab;
  const char			*errmsg;
};

struct reg_map all_reg_maps[] =
  {
    { reg_table, AVR32_NR_INTREGS, NULL, N_("integral register expected") },
    { cr_table,  AVR32_NR_CPREGS,  NULL, N_("coprocessor register expected") },
  };
#endif

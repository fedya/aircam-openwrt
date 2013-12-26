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
#ifndef __OPCODES_AVR32_ASM_H
#define __OPCODES_AVR32_ASM_H

extern int
avr32_parse_intreg(const char *str);
extern int
avr32_parse_cpreg(const char *str);
extern int
avr32_parse_picoreg(const char *str);
extern unsigned long
avr32_parse_reglist(char *str, char **endptr);
extern unsigned long
avr32_parse_cpreglist(char *str, char **endptr);
extern unsigned long
avr32_parse_pico_reglist(char *str, char **endptr);
extern int
avr32_make_regmask8(unsigned long regmask16, unsigned long *regmask8);

#endif /* __OPCODES_AVR32_ASM_H */

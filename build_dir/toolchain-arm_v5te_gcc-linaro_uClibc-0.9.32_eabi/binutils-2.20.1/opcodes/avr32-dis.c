/* Print AVR32 instructions for GDB and objdump.
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

#include "sysdep.h"
#include "dis-asm.h"
#include "avr32-opc.h"
#include "opintl.h"
#include "safe-ctype.h"

/* TODO: Share this with -asm */

/* Structure for a register hash table entry.  */
struct reg_entry
{
  const char	*name;
  int		number;
};

#ifndef strneq
#define strneq(a,b,n)	(strncmp ((a), (b), (n)) == 0)
#endif

static char avr32_opt_decode_fpu = 0;

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

static const char bparts[4] = { 'b', 'l', 'u', 't' };
static bfd_vma current_pc;

struct avr32_field_value
{
  const struct avr32_ifield *ifield;
  unsigned long value;
};

struct avr32_operand
{
  int id;
  int is_pcrel;
  int align_order;
  int (*print)(struct avr32_operand *op, struct disassemble_info *info,
	       struct avr32_field_value *ifields);
};

static signed long
get_signed_value(const struct avr32_field_value *fv)
{
  signed long value = fv->value;

  if (fv->value & (1 << (fv->ifield->bitsize - 1)))
    value |= (~0UL << fv->ifield->bitsize);

  return value;
}

static void
print_reglist_range(unsigned int first, unsigned int last,
		    const struct reg_entry *reg_names,
		    int need_comma,
		    struct disassemble_info *info)
{
  if (need_comma)
    info->fprintf_func(info->stream, ",");

  if (first == last)
    info->fprintf_func(info->stream, "%s",
		       reg_names[first].name);
  else
    info->fprintf_func(info->stream, "%s-%s",
		       reg_names[first].name, reg_names[last].name);
}

static int
print_intreg(struct avr32_operand *op,
	     struct disassemble_info *info,
	     struct avr32_field_value *ifields)
{
  unsigned long regid = ifields[0].value << op->align_order;

  info->fprintf_func(info->stream, "%s",
		     reg_table[regid].name);
  return 1;
}

static int
print_intreg_predec(struct avr32_operand *op ATTRIBUTE_UNUSED,
		    struct disassemble_info *info,
		    struct avr32_field_value *ifields)
{
  info->fprintf_func(info->stream, "--%s",
		     reg_table[ifields[0].value].name);
  return 1;
}

static int
print_intreg_postinc(struct avr32_operand *op ATTRIBUTE_UNUSED,
		     struct disassemble_info *info,
		     struct avr32_field_value *ifields)
{
  info->fprintf_func(info->stream, "%s++",
		     reg_table[ifields[0].value].name);
  return 1;
}

static int
print_intreg_lsl(struct avr32_operand *op ATTRIBUTE_UNUSED,
		 struct disassemble_info *info,
		 struct avr32_field_value *ifields)
{
  const char *rp = reg_table[ifields[0].value].name;
  unsigned long sa = ifields[1].value;

  if (sa)
    info->fprintf_func(info->stream, "%s<<0x%lx", rp, sa);
  else
    info->fprintf_func(info->stream, "%s", rp);

  return 2;
}

static int
print_intreg_lsr(struct avr32_operand *op ATTRIBUTE_UNUSED,
		 struct disassemble_info *info,
		 struct avr32_field_value *ifields)
{
  const char *rp = reg_table[ifields[0].value].name;
  unsigned long sa = ifields[1].value;

  if (sa)
    info->fprintf_func(info->stream, "%s>>0x%lx", rp, sa);
  else
    info->fprintf_func(info->stream, "%s", rp);

  return 2;
}

static int
print_intreg_bpart(struct avr32_operand *op ATTRIBUTE_UNUSED,
		   struct disassemble_info *info,
		   struct avr32_field_value *ifields)
{
  info->fprintf_func(info->stream, "%s:%c",
		     reg_table[ifields[0].value].name,
		     bparts[ifields[1].value]);
  return 2;
}

static int
print_intreg_hpart(struct avr32_operand *op ATTRIBUTE_UNUSED,
		   struct disassemble_info *info,
		   struct avr32_field_value *ifields)
{
  info->fprintf_func(info->stream, "%s:%c",
		     reg_table[ifields[0].value].name,
		     ifields[1].value ? 't' : 'b');
  return 2;
}

static int
print_intreg_sdisp(struct avr32_operand *op,
		  struct disassemble_info *info,
		  struct avr32_field_value *ifields)
{
  signed long disp;

  disp = get_signed_value(&ifields[1]) << op->align_order;

  info->fprintf_func(info->stream, "%s[%ld]",
		     reg_table[ifields[0].value].name, disp);
  return 2;
}

static int
print_intreg_udisp(struct avr32_operand *op,
		   struct disassemble_info *info,
		   struct avr32_field_value *ifields)
{
  info->fprintf_func(info->stream, "%s[0x%lx]",
		     reg_table[ifields[0].value].name,
		     ifields[1].value << op->align_order);
  return 2;
}

static int
print_intreg_index(struct avr32_operand *op ATTRIBUTE_UNUSED,
		   struct disassemble_info *info,
		   struct avr32_field_value *ifields)
{
  const char *rb, *ri;
  unsigned long sa = ifields[2].value;

  rb = reg_table[ifields[0].value].name;
  ri = reg_table[ifields[1].value].name;

  if (sa)
    info->fprintf_func(info->stream, "%s[%s<<0x%lx]", rb, ri, sa);
  else
    info->fprintf_func(info->stream, "%s[%s]", rb, ri);

  return 3;
}

static int
print_intreg_xindex(struct avr32_operand *op ATTRIBUTE_UNUSED,
		    struct disassemble_info *info,
		    struct avr32_field_value *ifields)
{
  info->fprintf_func(info->stream, "%s[%s:%c<<2]",
		     reg_table[ifields[0].value].name,
		     reg_table[ifields[1].value].name,
		     bparts[ifields[2].value]);
  return 3;
}

static int
print_jmplabel(struct avr32_operand *op,
	       struct disassemble_info *info,
	       struct avr32_field_value *ifields)
{
  bfd_vma address, offset;

  offset = get_signed_value(ifields) << op->align_order;
  address = (current_pc & (~0UL << op->align_order)) + offset;

  info->print_address_func(address, info);

  return 1;
}

static int
print_pc_disp(struct avr32_operand *op,
	      struct disassemble_info *info,
	      struct avr32_field_value *ifields)
{
  bfd_vma address, offset;

  offset = ifields[0].value << op->align_order;
  address = (current_pc & (~0UL << op->align_order)) + offset;

  info->print_address_func(address, info);

  return 1;
}

static int
print_sp(struct avr32_operand *op ATTRIBUTE_UNUSED,
	 struct disassemble_info *info,
	 struct avr32_field_value *ifields ATTRIBUTE_UNUSED)
{
  info->fprintf_func(info->stream, "sp");
  return 1;
}

static int
print_sp_disp(struct avr32_operand *op,
	      struct disassemble_info *info,
	      struct avr32_field_value *ifields)
{
  info->fprintf_func(info->stream, "sp[0x%lx]",
		     ifields[0].value << op->align_order);
  return 1;
}

static int
print_cpno(struct avr32_operand *op ATTRIBUTE_UNUSED,
	   struct disassemble_info *info,
	   struct avr32_field_value *ifields)
{
  info->fprintf_func(info->stream, "cp%lu", ifields[0].value);
  return 1;
}

static int
print_cpreg(struct avr32_operand *op,
	    struct disassemble_info *info,
	    struct avr32_field_value *ifields)
{
  info->fprintf_func(info->stream, "cr%lu",
		     ifields[0].value << op->align_order);
  return 1;
}

static int
print_uconst(struct avr32_operand *op,
	     struct disassemble_info *info,
	     struct avr32_field_value *ifields)
{
  info->fprintf_func(info->stream, "0x%lx",
		     ifields[0].value << op->align_order);
  return 1;
}

static int
print_sconst(struct avr32_operand *op,
	     struct disassemble_info *info,
	     struct avr32_field_value *ifields)
{
  info->fprintf_func(info->stream, "%ld",
		     get_signed_value(ifields) << op->align_order);
  return 1;
}

static int
print_reglist8_head(unsigned long regmask, int *commap,
		    struct disassemble_info *info)
{
  int first = -1, last, i = 0;
  int need_comma = 0;

  while (i < 12)
    {
      if (first == -1 && (regmask & 1))
	{
	  first = i;
	}
      else if (first != -1 && !(regmask & 1))
	{
	  last = i - 1;

	  print_reglist_range(first, last, reg_table, need_comma, info);
	  need_comma = 1;
	  first = -1;
	}

      if (i < 8)
	i += 4;
      else if (i < 10)
	i += 2;
      else
	i++;
      regmask >>= 1;
    }

  *commap = need_comma;
  return first;
}

static void
print_reglist8_tail(unsigned long regmask, int first, int need_comma,
		    struct disassemble_info *info)
{
  int last = 11;

  if (regmask & 0x20)
    {
      if (first == -1)
	first = 12;
      last = 12;
    }

  if (first != -1)
    {
      print_reglist_range(first, last, reg_table, need_comma, info);
      need_comma = 1;
      first = -1;
    }

  if (regmask & 0x40)
    {
      if (first == -1)
	first = 14;
      last = 14;
    }

  if (regmask & 0x80)
    {
      if (first == -1)
	first = 15;
      last = 15;
    }

  if (first != -1)
    print_reglist_range(first, last, reg_table, need_comma, info);
}

static int
print_reglist8(struct avr32_operand *op ATTRIBUTE_UNUSED,
	       struct disassemble_info *info,
	       struct avr32_field_value *ifields)
{
  unsigned long regmask = ifields[0].value;
  int first, need_comma;

  first = print_reglist8_head(regmask, &need_comma, info);
  print_reglist8_tail(regmask, first, need_comma, info);

  return 1;
}

static int
print_reglist9(struct avr32_operand *op ATTRIBUTE_UNUSED,
	       struct disassemble_info *info,
	       struct avr32_field_value *ifields)
{
  unsigned long regmask = ifields[0].value >> 1;
  int first, last, need_comma;

  first = print_reglist8_head(regmask, &need_comma, info);

  if ((ifields[0].value & 0x101) == 0x101)
    {
      if (first != -1)
	{
	  last = 11;

	  print_reglist_range(first, last, reg_table, need_comma, info);
	  need_comma = 1;
	  first = -1;
	}

      print_reglist_range(15, 15, reg_table, need_comma, info);

      regmask >>= 5;

      if ((regmask & 3) == 0)
	info->fprintf_func(info->stream, ",r12=0");
      else if ((regmask & 3) == 1)
	info->fprintf_func(info->stream, ",r12=1");
      else
	info->fprintf_func(info->stream, ",r12=-1");
    }
  else
      print_reglist8_tail(regmask, first, need_comma, info);

  return 1;
}

static int
print_reglist16(struct avr32_operand *op ATTRIBUTE_UNUSED,
		struct disassemble_info *info,
		struct avr32_field_value *ifields)
{
  unsigned long regmask = ifields[0].value;
  unsigned int i = 0, first, last;
  int need_comma = 0;

  while (i < 16)
    {
      if (regmask & 1)
	{
	  first = i;
	  while (i < 16)
	    {
	      i++;
	      regmask >>= 1;
	      if (!(regmask & 1))
		break;
	    }
	  last = i - 1;
	  print_reglist_range(first, last, reg_table, need_comma, info);
	  need_comma = 1;
	}
      else
	{
	  i++;
	  regmask >>= 1;
	}
    }

  return 1;
}

static int
print_reglist_ldm(struct avr32_operand *op,
		  struct disassemble_info *info,
		  struct avr32_field_value *ifields)
{
  int rp, w_bit;
  int i, first, last;
  unsigned long regmask;

  rp = ifields[0].value;
  w_bit = ifields[1].value;
  regmask = ifields[2].value;

  if (regmask & (1 << AVR32_REG_PC) && rp == AVR32_REG_PC)
    {
      if (w_bit)
	info->fprintf_func(info->stream, "sp++");
      else
	info->fprintf_func(info->stream, "sp");

      for (i = 0; i < 12; )
	{
	  if (regmask & (1 << i))
	    {
	      first = i;
	      while (i < 12)
		{
		  i++;
		  if (!(regmask & (1 << i)))
		    break;
		}
	      last = i - 1;
	      print_reglist_range(first, last, reg_table, 1, info);
	    }
	  else
	    i++;
	}

      info->fprintf_func(info->stream, ",pc");
      if (regmask & (1 << AVR32_REG_LR))
	info->fprintf_func(info->stream, ",r12=-1");
      else if (regmask & (1 << AVR32_REG_R12))
	info->fprintf_func(info->stream, ",r12=1");
      else
	info->fprintf_func(info->stream, ",r12=0");
    }
  else
    {
      if (w_bit)
	info->fprintf_func(info->stream, "%s++,", reg_table[rp].name);
      else
	info->fprintf_func(info->stream, "%s,", reg_table[rp].name);

      print_reglist16(op, info, ifields + 2);
    }

  return 3;
}

static int
print_reglist_cp8(struct avr32_operand *op ATTRIBUTE_UNUSED,
		  struct disassemble_info *info,
		  struct avr32_field_value *ifields)
{
  unsigned long regmask = ifields[0].value;
  unsigned int i = 0, first, last, offset = 0;
  int need_comma = 0;

  if (ifields[1].value)
    offset = 8;

  while (i < 8)
    {
      if (regmask & 1)
	{
	  first = i;
	  while (i < 8)
	    {
	      i++;
	      regmask >>= 1;
	      if (!(regmask & 1))
		break;
	    }
	  last = i - 1;
	  print_reglist_range(offset + first, offset + last,
			      cr_table, need_comma, info);
	  need_comma = 1;
	}
      else
	{
	  i++;
	  regmask >>= 1;
	}
    }

  return 2;
}

static int
print_reglist_cpd8(struct avr32_operand *op ATTRIBUTE_UNUSED,
		   struct disassemble_info *info,
		   struct avr32_field_value *ifields)
{
  unsigned long regmask = ifields[0].value;
  unsigned int i = 0, first, last;
  int need_comma = 0;

  while (i < 8)
    {
      if (regmask & 1)
	{
	  first = 2 * i;
	  while (i < 8)
	    {
	      i++;
	      regmask >>= 1;
	      if (!(regmask & 1))
		break;
	    }
	  last = 2 * (i - 1) + 1;
	  print_reglist_range(first, last, cr_table, need_comma, info);
	  need_comma = 1;
	}
      else
	{
	  i++;
	  regmask >>= 1;
	}
    }

  return 1;
}

static int
print_retval(struct avr32_operand *op ATTRIBUTE_UNUSED,
	     struct disassemble_info *info,
	     struct avr32_field_value *ifields)
{
  unsigned long regid = ifields[0].value;
  const char *retval;

  if (regid < AVR32_REG_SP)
    retval = reg_table[regid].name;
  else if (regid == AVR32_REG_SP)
    retval = "0";
  else if (regid == AVR32_REG_LR)
    retval = "-1";
  else
    retval = "1";

  info->fprintf_func(info->stream, "%s", retval);

  return 1;
}

static int
print_mcall(struct avr32_operand *op,
	    struct disassemble_info *info,
	    struct avr32_field_value *ifields)
{
  unsigned long regid = ifields[0].value;

  if (regid == AVR32_REG_PC)
    print_jmplabel(op, info, ifields + 1);
  else
    print_intreg_sdisp(op, info, ifields);

  return 2;
}

static int
print_jospinc(struct avr32_operand *op ATTRIBUTE_UNUSED,
	      struct disassemble_info *info,
	      struct avr32_field_value *ifields)
{
  signed long value = ifields[0].value;

  if (value >= 4)
    value -= 8;
  else
    value += 1;

  info->fprintf_func(info->stream, "%ld", value);

  return 1;
}

static int
print_coh(struct avr32_operand *op ATTRIBUTE_UNUSED,
	  struct disassemble_info *info,
	  struct avr32_field_value *ifields ATTRIBUTE_UNUSED)
{
  info->fprintf_func(info->stream, "COH");
  return 0;
}

#define OP(name, sgn, pcrel, align, func) \
  { AVR32_OPERAND_##name, pcrel, align, print_##func }

struct avr32_operand operand[AVR32_NR_OPERANDS] =
  {
    OP(INTREG, 0, 0, 0, intreg),
    OP(INTREG_PREDEC, 0, 0, 0, intreg_predec),
    OP(INTREG_POSTINC, 0, 0, 0, intreg_postinc),
    OP(INTREG_LSL, 0, 0, 0, intreg_lsl),
    OP(INTREG_LSR, 0, 0, 0, intreg_lsr),
    OP(INTREG_BSEL, 0, 0, 0, intreg_bpart),
    OP(INTREG_HSEL, 0, 0, 1, intreg_hpart),
    OP(INTREG_SDISP, 1, 0, 0, intreg_sdisp),
    OP(INTREG_SDISP_H, 1, 0, 1, intreg_sdisp),
    OP(INTREG_SDISP_W, 1, 0, 2, intreg_sdisp),
    OP(INTREG_UDISP, 0, 0, 0, intreg_udisp),
    OP(INTREG_UDISP_H, 0, 0, 1, intreg_udisp),
    OP(INTREG_UDISP_W, 0, 0, 2, intreg_udisp),
    OP(INTREG_INDEX, 0, 0, 0, intreg_index),
    OP(INTREG_XINDEX, 0, 0, 0, intreg_xindex),
    OP(DWREG, 0, 0, 1, intreg),
    OP(PC_UDISP_W, 0, 1, 2, pc_disp),
    OP(SP, 0, 0, 0, sp),
    OP(SP_UDISP_W, 0, 0, 2, sp_disp),
    OP(CPNO, 0, 0, 0, cpno),
    OP(CPREG, 0, 0, 0, cpreg),
    OP(CPREG_D, 0, 0, 1, cpreg),
    OP(UNSIGNED_CONST, 0, 0, 0, uconst),
    OP(UNSIGNED_CONST_W, 0, 0, 2, uconst),
    OP(SIGNED_CONST, 1, 0, 0, sconst),
    OP(SIGNED_CONST_W, 1, 0, 2, sconst),
    OP(JMPLABEL, 1, 1, 1, jmplabel),
    OP(UNSIGNED_NUMBER, 0, 0, 0, uconst),
    OP(UNSIGNED_NUMBER_W, 0, 0, 2, uconst),
    OP(REGLIST8, 0, 0, 0, reglist8),
    OP(REGLIST9, 0, 0, 0, reglist9),
    OP(REGLIST16, 0, 0, 0, reglist16),
    OP(REGLIST_LDM, 0, 0, 0, reglist_ldm),
    OP(REGLIST_CP8, 0, 0, 0, reglist_cp8),
    OP(REGLIST_CPD8, 0, 0, 0, reglist_cpd8),
    OP(RETVAL, 0, 0, 0, retval),
    OP(MCALL, 1, 0, 2, mcall),
    OP(JOSPINC, 0, 0, 0, jospinc),
    OP(COH, 0, 0, 0, coh),
  };

static void
print_opcode(bfd_vma insn_word, const struct avr32_opcode *opc,
	     bfd_vma pc, struct disassemble_info *info)
{
  const struct avr32_syntax *syntax = opc->syntax;
  struct avr32_field_value fields[AVR32_MAX_FIELDS];
  unsigned int i, next_field = 0, nr_operands;

  for (i = 0; i < opc->nr_fields; i++)
    {
      opc->fields[i]->extract(opc->fields[i], &insn_word, &fields[i].value);
      fields[i].ifield = opc->fields[i];
    }

  current_pc = pc;
  info->fprintf_func(info->stream, "%s", syntax->mnemonic->name);

  if (syntax->nr_operands < 0)
    nr_operands = (unsigned int) -syntax->nr_operands;
  else
    nr_operands = (unsigned int) syntax->nr_operands;

  for (i = 0; i < nr_operands; i++)
    {
      struct avr32_operand *op = &operand[syntax->operand[i]];

      if (i)
	info->fprintf_func(info->stream, ",");
      else
	info->fprintf_func(info->stream, "\t");
      next_field += op->print(op, info, &fields[next_field]);
    }
}

#define is_fpu_insn(iw) ((iw&0xf9f0e000)==0xe1a00000) 

static const struct avr32_opcode *
find_opcode(bfd_vma insn_word)
{
  int i;

  for (i = 0; i < AVR32_NR_OPCODES; i++)
    {
      const struct avr32_opcode *opc = &avr32_opc_table[i];

      if ((insn_word & opc->mask) == opc->value)
      {
        if (avr32_opt_decode_fpu)
        {
          if (is_fpu_insn(insn_word))
          {
            if (opc->id != AVR32_OPC_COP)
	       return opc;
          }
          else
            return opc;
        }
        else
	return opc;
    }
   }

  return NULL;
}

static int
read_insn_word(bfd_vma pc, bfd_vma *valuep,
	       struct disassemble_info *info)
{
  bfd_byte b[4];
  int status;

  status = info->read_memory_func(pc, b, 4, info);
  if (status)
    {
      status = info->read_memory_func(pc, b, 2, info);
      if (status)
	{
	  info->memory_error_func(status, pc, info);
	  return -1;
	}
      b[3] = b[2] = 0;
    }

  *valuep =  (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | b[3];
  return 0;
}

/* Parse an individual disassembler option.  */

void
parse_avr32_disassembler_option (option)
     char * option;
{
  if (option == NULL)
    return;

  if (!strcmp(option,"decode-fpu"))
  {
    avr32_opt_decode_fpu = 1;
    return;
  }

  printf("\n%s--",option);
  /* XXX - should break 'option' at following delimiter.  */
  fprintf (stderr, _("Unrecognised disassembler option: %s\n"), option);

  return;
}

/* Parse the string of disassembler options, spliting it at whitespaces
   or commas.  (Whitespace separators supported for backwards compatibility).  */

static void
parse_disassembler_options (char *options)
{
  if (options == NULL)
    return;

  while (*options)
    {
      parse_avr32_disassembler_option (options);

      /* Skip forward to next seperator.  */
      while ((*options) && (! ISSPACE (*options)) && (*options != ','))
	++ options;
      /* Skip forward past seperators.  */
      while (ISSPACE (*options) || (*options == ','))
	++ options;
    }
}

int
print_insn_avr32(bfd_vma pc, struct disassemble_info *info)
{
  bfd_vma insn_word;
  const struct avr32_opcode *opc;

  if (info->disassembler_options)
    {
      parse_disassembler_options (info->disassembler_options);

      /* To avoid repeated parsing of these options, we remove them here.  */
      info->disassembler_options = NULL;
    }

  info->bytes_per_chunk = 1;
  info->display_endian = BFD_ENDIAN_BIG;

  if (read_insn_word(pc, &insn_word, info))
    return -1;

  opc = find_opcode(insn_word);
  if (opc)
    {
      print_opcode(insn_word, opc, pc, info);
      return opc->size;
    }
  else
    {
      info->fprintf_func(info->stream, _("*unknown*"));
      return 2;
    }

}

void
print_avr32_disassembler_options (FILE *stream ATTRIBUTE_UNUSED)
{
  fprintf(stream, "\n AVR32 Specific Disassembler Options:\n");
  fprintf(stream, "  -M decode-fpu                  Print FPU instructions instead of 'cop' \n");
}


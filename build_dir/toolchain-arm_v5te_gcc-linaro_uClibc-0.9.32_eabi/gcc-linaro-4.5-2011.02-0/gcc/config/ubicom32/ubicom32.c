/* Subroutines for insn-output.c for Ubicom32

   Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
   2009 Free Software Foundation, Inc.
   Contributed by Ubicom, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "insn-flags.h"
#include "output.h"
#include "insn-attr.h"
#include "insn-codes.h"
#include "flags.h"
#include "recog.h"
#include "expr.h"
#include "function.h"
#include "obstack.h"
#include "toplev.h"
#include "tm_p.h"
#include "tm-constrs.h"
#include "basic-block.h"
#include "integrate.h"
#include "target.h"
#include "target-def.h"
#include "reload.h"
#include "df.h"
#include "langhooks.h"
#include "optabs.h"

static tree ubicom32_handle_fndecl_attribute (tree *, tree, tree, int, bool *);
static void ubicom32_layout_frame (void);
static void ubicom32_function_prologue (FILE *, HOST_WIDE_INT);
static void ubicom32_function_epilogue (FILE *, HOST_WIDE_INT);
static bool ubicom32_rtx_costs (rtx, int, int, int *, bool speed);
static bool ubicom32_fixed_condition_code_regs (unsigned int *,
						unsigned int *);
static enum machine_mode ubicom32_cc_modes_compatible (enum machine_mode,
						       enum machine_mode);
static int ubicom32_naked_function_p (void);
static void ubicom32_machine_dependent_reorg (void);
static bool ubicom32_assemble_integer (rtx, unsigned int, int);
static void ubicom32_asm_init_sections (void);
static int ubicom32_arg_partial_bytes (CUMULATIVE_ARGS *, enum machine_mode,tree, 
				       bool);
static bool ubicom32_pass_by_reference (CUMULATIVE_ARGS *ca ATTRIBUTE_UNUSED,
					enum machine_mode mode, const_tree type,
					bool named ATTRIBUTE_UNUSED);
static bool ubicom32_callee_copies (CUMULATIVE_ARGS *ca ATTRIBUTE_UNUSED,
				    enum machine_mode mode, const_tree type,
				    bool named ATTRIBUTE_UNUSED);

static bool ubicom32_return_in_memory (const_tree type, 
				       const_tree fntype ATTRIBUTE_UNUSED);
static bool ubicom32_is_base_reg (rtx, int);
static void ubicom32_init_builtins (void);
static rtx ubicom32_expand_builtin (tree, rtx, rtx, enum machine_mode, int);
static tree ubicom32_fold_builtin (tree, tree, bool);
static int ubicom32_get_valid_offset_mask (enum machine_mode);
static bool ubicom32_cannot_force_const_mem (rtx);

/* Case values threshold */
int ubicom32_case_values_threshold = 6;

/* Nonzero if this chip supports the Ubicom32 v3 ISA.  */
int ubicom32_v3 = 1;

/* Nonzero if this chip supports the Ubicom32 v4 ISA.  */
int ubicom32_v4 = 1;

/* Valid attributes:
   naked - don't generate function prologue/epilogue and `ret' command.  */
const struct attribute_spec ubicom32_attribute_table[] =
{
  /* { name, min_len, max_len, decl_req, type_req, fn_type_req, handler } */
  { "naked", 0, 0, true,  false, false, ubicom32_handle_fndecl_attribute },
  { NULL,    0, 0, false, false, false, NULL }
};

#undef TARGET_ASM_FUNCTION_PROLOGUE
#define TARGET_ASM_FUNCTION_PROLOGUE ubicom32_function_prologue

#undef TARGET_ASM_FUNCTION_EPILOGUE
#define TARGET_ASM_FUNCTION_EPILOGUE ubicom32_function_epilogue

#undef TARGET_ATTRIBUTE_TABLE
#define TARGET_ATTRIBUTE_TABLE ubicom32_attribute_table

/* All addresses cost the same amount.  */
#undef TARGET_ADDRESS_COST
#define TARGET_ADDRESS_COST hook_int_rtx_bool_0

#undef TARGET_RTX_COSTS
#define TARGET_RTX_COSTS ubicom32_rtx_costs

#undef TARGET_FIXED_CONDITION_CODE_REGS
#define TARGET_FIXED_CONDITION_CODE_REGS ubicom32_fixed_condition_code_regs

#undef TARGET_CC_MODES_COMPATIBLE
#define TARGET_CC_MODES_COMPATIBLE ubicom32_cc_modes_compatible

#undef TARGET_MACHINE_DEPENDENT_REORG
#define TARGET_MACHINE_DEPENDENT_REORG ubicom32_machine_dependent_reorg

#undef  TARGET_ASM_INTEGER
#define TARGET_ASM_INTEGER ubicom32_assemble_integer

#undef TARGET_ASM_INIT_SECTIONS
#define TARGET_ASM_INIT_SECTIONS ubicom32_asm_init_sections

#undef TARGET_ARG_PARTIAL_BYTES
#define TARGET_ARG_PARTIAL_BYTES ubicom32_arg_partial_bytes

#undef TARGET_PASS_BY_REFERENCE
#define TARGET_PASS_BY_REFERENCE ubicom32_pass_by_reference

#undef TARGET_CALLEE_COPIES
#define TARGET_CALLEE_COPIES ubicom32_callee_copies

#undef TARGET_RETURN_IN_MEMORY
#define TARGET_RETURN_IN_MEMORY ubicom32_return_in_memory

#undef TARGET_INIT_BUILTINS
#define TARGET_INIT_BUILTINS ubicom32_init_builtins

#undef TARGET_EXPAND_BUILTIN
#define TARGET_EXPAND_BUILTIN ubicom32_expand_builtin

#undef TARGET_FOLD_BUILTIN
#define TARGET_FOLD_BUILTIN ubicom32_fold_builtin

#undef TARGET_CANNOT_FORCE_CONST_MEM
#define TARGET_CANNOT_FORCE_CONST_MEM ubicom32_cannot_force_const_mem

struct gcc_target targetm = TARGET_INITIALIZER;

static char save_regs[FIRST_PSEUDO_REGISTER];
static int nregs;
static int frame_size;
int ubicom32_stack_size = 0;	/* size of allocated stack (including frame) */
int ubicom32_can_use_calli_to_ret;

#define STACK_UNIT_BOUNDARY (STACK_BOUNDARY / BITS_PER_UNIT)
#define ROUND_CALL_BLOCK_SIZE(BYTES) \
  (((BYTES) + (STACK_UNIT_BOUNDARY - 1)) & ~(STACK_UNIT_BOUNDARY - 1))

/* In case of a PRE_INC, POST_INC, PRE_DEC, POST_DEC memory reference, we
   must report the mode of the memory reference from PRINT_OPERAND to
   PRINT_OPERAND_ADDRESS.  */
enum machine_mode output_memory_reference_mode;

/* Flag for some split insns from the ubicom32.md.  */
int ubicom32_reorg_completed;

enum reg_class const ubicom32_regclass_map[FIRST_PSEUDO_REGISTER] =
{
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  DATA_REGS, 
  FDPIC_REG, 
  ADDRESS_REGS, 
  ADDRESS_REGS, 
  ADDRESS_REGS, 
  ADDRESS_REGS, 
  ADDRESS_REGS, 
  ADDRESS_REGS, 
  ADDRESS_REGS, 
  ACC_REGS,
  ACC_LO_REGS,
  ACC_REGS,
  ACC_LO_REGS,
  SOURCE3_REG,
  ADDRESS_REGS,
  NO_REGS,			/* CC_REG must be NO_REGS */
  SPECIAL_REGS,
  SPECIAL_REGS,
  SPECIAL_REGS,
  SPECIAL_REGS,
  SPECIAL_REGS,
  SPECIAL_REGS,
  SPECIAL_REGS,
  SPECIAL_REGS
};

rtx ubicom32_compare_op0;
rtx ubicom32_compare_op1;

/* Handle command line option overrides.  */

void
ubicom32_override_options (void)
{
  flag_pic = 0;

  if (strcmp (ubicom32_arch_name, "ubicom32v1") == 0) {
    /* If we have a version 1 architecture then we want to avoid using jump
       tables.  */
    ubicom32_case_values_threshold = 30000;
    ubicom32_v3 = 0;
    ubicom32_v4 = 0;
  } else if (strcmp (ubicom32_arch_name, "ubicom32v2") == 0) {
    ubicom32_v3 = 0;
    ubicom32_v4 = 0;
  } else if (strcmp (ubicom32_arch_name, "ubicom32v3") == 0) {
    ubicom32_v3 = 1;
    ubicom32_v4 = 0;
  } else if (strcmp (ubicom32_arch_name, "ubicom32v4") == 0) {
    ubicom32_v3 = 1;
    ubicom32_v4 = 1;
  }

  /* There is no single unaligned SI op for PIC code.  Sometimes we
     need to use ".4byte" and sometimes we need to use ".picptr".
     See ubicom32_assemble_integer for details.  */
  if (TARGET_FDPIC)
    targetm.asm_out.unaligned_op.si = 0;
}

void
ubicom32_conditional_register_usage (void)
{
  /* If we're using the old ipOS ABI we need to make D10 through D13
     caller-clobbered.  */
  if (TARGET_IPOS_ABI)
    {
      call_used_regs[D10_REGNUM] = 1;
      call_used_regs[D11_REGNUM] = 1;
      call_used_regs[D12_REGNUM] = 1;
      call_used_regs[D13_REGNUM] = 1;
    }
}

/* We have some number of optimizations that don't really work for the Ubicom32
   architecture so we deal with them here.  */

void
ubicom32_optimization_options (int level ATTRIBUTE_UNUSED,
			       int size ATTRIBUTE_UNUSED)
{
  /* The tree IVOPTs pass seems to do really bad things for the Ubicom32
     architecture - it tends to turn things that would happily use pre/post
     increment/decrement into operations involving unecessary loop
     indicies.  */
  flag_ivopts = 0;

  /* We have problems where DSE at the RTL level misses partial stores
     to the stack.  For now we disable it to avoid this.  */
  flag_dse = 0;
}

/* Print operand X using operand code CODE to assembly language output file
   FILE.  */

void
ubicom32_print_operand (FILE *file, rtx x, int code)
{
  switch (code)
    {
    case 'A':
      /* Identify the correct accumulator to use.  */
      if (REGNO (x) == ACC0_HI_REGNUM || REGNO (x) == ACC0_LO_REGNUM)
	fprintf (file, "acc0");
      else if (REGNO (x) == ACC1_HI_REGNUM || REGNO (x) == ACC1_LO_REGNUM)
	fprintf (file, "acc1");
      else
	abort ();
      break;

    case 'b':
    case 'B':
      {
	enum machine_mode mode;

	mode = GET_MODE (XEXP (x, 0));

	/* These are normal and reversed branches.  */
	switch (code == 'b' ? GET_CODE (x) : reverse_condition (GET_CODE (x)))
	  {
	  case NE:
	    fprintf (file, "ne");
	    break;

	  case EQ:
	    fprintf (file, "eq");
	    break;

	  case GE:
	    if (mode == CCSZNmode || mode == CCWZNmode)
	      fprintf (file, "pl");
	    else
	      fprintf (file, "ge");
	    break;

	  case GT:
	    fprintf (file, "gt");
	    break;

	  case LE:
	    fprintf (file, "le");
	    break;

	  case LT:
	    if (mode == CCSZNmode || mode == CCWZNmode)
	      fprintf (file, "mi");
	    else
	      fprintf (file, "lt");
	    break;

	  case GEU:
	    fprintf (file, "cs");
	    break;

	  case GTU:
	    fprintf (file, "hi");
	    break;

	  case LEU:
	    fprintf (file, "ls");
	    break;

	  case LTU:
	    fprintf (file, "cc");
	    break;

	  default:
	    abort ();
	  }
      }
      break;

    case 'C':
      /* This is used for the operand to a call instruction;
	 if it's a REG, enclose it in parens, else output
	 the operand normally.  */
      if (REG_P (x))
	{
	  fputc ('(', file);
	  ubicom32_print_operand (file, x, 0);
	  fputc (')', file);
	}
      else
	ubicom32_print_operand (file, x, 0);
      break;

    case 'd':
      /* Bit operations we need bit numbers. */
      fprintf (file, "%d", exact_log2 (INTVAL (x)));
      break;

    case 'D':
      /* Bit operations we need bit numbers. */
      fprintf (file, "%d", exact_log2 (~ INTVAL (x)));
      break;

    case 'E':
      /* For lea, which we use to add address registers.
	 We don't want the '#' on a constant. */
      if (CONST_INT_P (x))
	{
	  fprintf (file, "%ld", INTVAL (x));
	  break;
	}
      /* FALL THROUGH */

    default:
      switch (GET_CODE (x))
	{
	case MEM:
	  output_memory_reference_mode = GET_MODE (x);
	  output_address (XEXP (x, 0));
	  break;

	case PLUS:
	  output_address (x);
	  break;

	case REG:
	  fprintf (file, "%s", reg_names[REGNO (x)]);
	  break;

	case SUBREG:
	  fprintf (file, "%s", reg_names[subreg_regno (x)]);
	  break;

	/* This will only be single precision....  */
	case CONST_DOUBLE:
	  {
	    unsigned long val;
	    REAL_VALUE_TYPE rv;

	    REAL_VALUE_FROM_CONST_DOUBLE (rv, x);
	    REAL_VALUE_TO_TARGET_SINGLE (rv, val);
	    fprintf (file, "0x%lx", val);
	    break;
	  }

	case CONST_INT:
	case SYMBOL_REF:
	case CONST:
	case LABEL_REF:
	case CODE_LABEL:
	case LO_SUM:
	  ubicom32_print_operand_address (file, x);
	  break;

	case HIGH:
	  fprintf (file, "#%%hi(");
	  ubicom32_print_operand_address (file, XEXP (x, 0));
	  fprintf (file, ")");
	  break;

	case UNSPEC:
	  switch (XINT (x, 1))
	    {
	    case UNSPEC_FDPIC_GOT:
	      fprintf (file, "#%%got_lo(");
	      ubicom32_print_operand_address (file, XVECEXP (x, 0, 0));
	      fprintf (file, ")");
	      break;

	    case UNSPEC_FDPIC_GOT_FUNCDESC:
	      fprintf (file, "#%%got_funcdesc_lo(");
	      ubicom32_print_operand_address (file, XVECEXP (x, 0, 0));
	      fprintf (file, ")");
	      break;

	    default:
	      abort ();
	    }
          break;

	default:
	  abort ();
	}
      break;
   }
}

/* Output assembly language output for the address ADDR to FILE.  */

void
ubicom32_print_operand_address (FILE *file, rtx addr)
{
  switch (GET_CODE (addr))
    {
    case POST_INC:
      ubicom32_print_operand_address (file, XEXP (addr, 0));
      fprintf (file, "%d++", GET_MODE_SIZE (output_memory_reference_mode));
      break;

    case PRE_INC:
      fprintf (file, "%d", GET_MODE_SIZE (output_memory_reference_mode));
      ubicom32_print_operand_address (file, XEXP (addr, 0));
      fprintf (file, "++");
      break;

    case POST_DEC:
      ubicom32_print_operand_address (file, XEXP (addr, 0));
      fprintf (file, "%d++", -GET_MODE_SIZE (output_memory_reference_mode));
      break;

    case PRE_DEC:
      fprintf (file, "%d", -GET_MODE_SIZE (output_memory_reference_mode));
      ubicom32_print_operand_address (file, XEXP (addr, 0));
      fprintf (file, "++");
      break;

    case POST_MODIFY:
      ubicom32_print_operand_address (file, XEXP (addr, 0));
      fprintf (file, "%ld++", INTVAL (XEXP (XEXP (addr,1), 1)));
      break;

    case PRE_MODIFY:
      fprintf (file, "%ld", INTVAL (XEXP (XEXP (addr,1), 1)));
      ubicom32_print_operand_address (file, XEXP (addr, 0));
      fprintf (file, "++");
      break;

    case REG:
      fputc ('(', file);
      fprintf (file, "%s", reg_names[REGNO (addr)]); 
      fputc (')', file);
      break;

    case PLUS:
      {
	rtx base = XEXP (addr, 0);
	rtx index = XEXP (addr, 1); 

 	/* Switch around addresses of the form index * scaling + base.  */
 	if (! ubicom32_is_base_reg (base, 1))
 	  {
 	    rtx tmp = base;
 	    base = index;
 	    index = tmp;
 	  }

	if (CONST_INT_P (index)) 
	  {
	    fprintf (file, "%ld", INTVAL (index)); 
	    fputc ('(', file);
	    fputs (reg_names[REGNO (base)], file); 
	  }
 	else if (GET_CODE (index) == MULT
 	         || REG_P (index))
	  {
 	    if (GET_CODE (index) == MULT)
 	      index = XEXP (index, 0);
	    fputc ('(', file);
	    fputs (reg_names[REGNO (base)], file); 
	    fputc (',', file);
	    fputs (reg_names[REGNO (index)], file); 
	  }
	else 
	  abort (); 

	fputc (')', file);
	break;
      }

    case LO_SUM:
      fprintf (file, "%%lo(");
      ubicom32_print_operand (file, XEXP (addr, 1), 'L');
      fprintf (file, ")(");
      ubicom32_print_operand (file, XEXP (addr, 0), 0);
      fprintf (file, ")");
      break;

    case CONST_INT:
      fputc ('#', file);
      output_addr_const (file, addr); 
      break;

    default:
      output_addr_const (file, addr);
      break;
    }
}

/* X and Y are two things to compare using CODE.  Emit the compare insn and
   return the rtx for the cc reg in the proper mode.  */

rtx
ubicom32_gen_compare_reg (enum rtx_code code, rtx x, rtx y)
{
  enum machine_mode mode = SELECT_CC_MODE (code, x, y);
  rtx cc_reg;

  cc_reg = gen_rtx_REG (mode, CC_REGNUM);

  emit_insn (gen_rtx_SET (VOIDmode, cc_reg,
			  gen_rtx_COMPARE (mode, x, y)));

  return cc_reg;
}

/* Given a comparison code (EQ, NE, etc.) and the first operand of a COMPARE,
   return the mode to be used for the comparison.  */

enum machine_mode
ubicom32_select_cc_mode (enum rtx_code op, rtx x, rtx y)
{
  /* Is this a short compare?  */
  if (GET_MODE (x) == QImode
      || GET_MODE (x) == HImode
      || GET_MODE (y) == QImode
      || GET_MODE (y) == HImode)
    {
      switch (op)
	{
	case EQ :
	case NE :
	  return CCSZmode;

	case GE:
	case LT:
	  if (y == const0_rtx)
	    return CCSZNmode;

	default :
	  return CCSmode;
	}
    }

  /* We have a word compare.  */
  switch (op)
    {
    case EQ :
    case NE :
      return CCWZmode;

    case GE :
    case LT :
      if (y == const0_rtx)
	return CCWZNmode;

    default :
      return CCWmode;
    }
}

/* Return TRUE or FALSE depending on whether the first SET in INSN
   has source and destination with matching CC modes, and that the
   CC mode is at least as constrained as REQ_MODE.  */
bool
ubicom32_match_cc_mode (rtx insn, enum machine_mode req_mode)
{
  rtx set;
  enum machine_mode set_mode;

  set = PATTERN (insn);
  if (GET_CODE (set) == PARALLEL)
    set = XVECEXP (set, 0, 0);
  gcc_assert (GET_CODE (set) == SET);
  gcc_assert (GET_CODE (SET_SRC (set)) == COMPARE);

  /* SET_MODE is the mode we have in the instruction.  This must either
     be the same or less restrictive that the required mode REQ_MODE.  */
  set_mode = GET_MODE (SET_DEST (set));

  switch (req_mode)
    {
    case CCSZmode:
      if (set_mode != CCSZmode)
	return 0;
      break;

    case CCSZNmode:
      if (set_mode != CCSZmode
	  && set_mode != CCSZNmode)
	return 0;
      break;

    case CCSmode:
      if (set_mode != CCSmode
	  && set_mode != CCSZmode
	  && set_mode != CCSZNmode)
	return 0;
      break;

    case CCWZmode:
      if (set_mode != CCWZmode)
	return 0;
      break;

    case CCWZNmode:
      if (set_mode != CCWZmode
	  && set_mode != CCWZNmode)
	return 0;
      break;

    case CCWmode:
      if (set_mode != CCWmode
	  && set_mode != CCWZmode
	  && set_mode != CCWZNmode)
	return 0;
      break;

    default:
      gcc_unreachable ();
    }

  return (GET_MODE (SET_SRC (set)) == set_mode);
}

/* Replace the comparison OP0 CODE OP1 by a semantically equivalent one
   that we can implement more efficiently.  */

void
ubicom32_canonicalize_comparison (enum rtx_code *code, rtx *op0, rtx *op1)
{
  /* If we have a REG and a MEM then compare the MEM with the REG and not
     the other way round.  */
  if (REG_P (*op0) && MEM_P (*op1))
    {
      rtx tem = *op0;
      *op0 = *op1;
      *op1 = tem;
      *code = swap_condition (*code);
      return;
    }

  /* If we have a REG and a CONST_INT then we may want to reverse things
     if the constant can be represented as an "I" constraint.  */
  if (REG_P (*op0) && CONST_INT_P (*op1) && satisfies_constraint_I (*op1))
    {
      rtx tem = *op0;
      *op0 = *op1;
      *op1 = tem;
      *code = swap_condition (*code);
      return;
    }
}

/* Return the fixed registers used for condition codes.  */

static bool
ubicom32_fixed_condition_code_regs (unsigned int *p1, unsigned int *p2)
{
  *p1 = CC_REGNUM;
  *p2 = INVALID_REGNUM;
 
  return true;
}

/* If two condition code modes are compatible, return a condition code
   mode which is compatible with both.  Otherwise, return
   VOIDmode.  */

static enum machine_mode
ubicom32_cc_modes_compatible (enum machine_mode m1, enum machine_mode m2)
{
  if (m1 == m2)
    return m1;

  if (GET_MODE_CLASS (m1) != MODE_CC || GET_MODE_CLASS (m2) != MODE_CC)
    return VOIDmode;

  switch (m1)
    {
    case CCWmode:
      if (m2 == CCWZNmode || m2 == CCWZmode)
	return m1;

      return VOIDmode;

    case CCWZNmode:
      if (m2 == CCWmode)
	return m2;

      if (m2 == CCWZmode)
	return m1;

      return VOIDmode;

    case CCWZmode:
      if (m2 == CCWmode || m2 == CCWZNmode)
	return m2;

      return VOIDmode;

    case CCSmode:
      if (m2 == CCSZNmode || m2 == CCSZmode)
	return m1;

      return VOIDmode;

    case CCSZNmode:
      if (m2 == CCSmode)
	return m2;

      if (m2 == CCSZmode)
	return m1;

      return VOIDmode;

    case CCSZmode:
      if (m2 == CCSmode || m2 == CCSZNmode)
	return m2;

      return VOIDmode;

    default:
      gcc_unreachable ();
    }
}

static rtx
ubicom32_legitimize_fdpic_address_symbol (rtx orig, rtx reg, rtx fdpic_reg)
{
  int unspec;
  rtx got_offs;
  rtx got_offs_scaled;
  rtx plus_scaled;
  rtx tmp;
  rtx new_rtx;

  gcc_assert (reg != 0);

  if (GET_CODE (orig) == SYMBOL_REF
      && SYMBOL_REF_FUNCTION_P (orig))
    unspec = UNSPEC_FDPIC_GOT_FUNCDESC;
  else
    unspec = UNSPEC_FDPIC_GOT;

  got_offs = gen_reg_rtx (SImode);
  tmp = gen_rtx_UNSPEC (Pmode, gen_rtvec (1, orig), unspec);
  emit_move_insn (got_offs, tmp);

  got_offs_scaled = gen_rtx_MULT (SImode, got_offs, GEN_INT (4));
  plus_scaled = gen_rtx_PLUS (Pmode, fdpic_reg, got_offs_scaled);
  new_rtx = gen_const_mem (Pmode, plus_scaled);
  emit_move_insn (reg, new_rtx);

  return reg;
}

static rtx
ubicom32_legitimize_fdpic_address (rtx orig, rtx reg, rtx fdpic_reg)
{
  rtx addr = orig;
  rtx new_rtx = orig;

  if (GET_CODE (addr) == CONST || GET_CODE (addr) == PLUS)
    {
      rtx base;

      if (GET_CODE (addr) == CONST)
	{
	  addr = XEXP (addr, 0);
	  gcc_assert (GET_CODE (addr) == PLUS);
	}

      base = ubicom32_legitimize_fdpic_address_symbol (XEXP (addr, 0), reg, fdpic_reg);
      return gen_rtx_PLUS (Pmode, base, XEXP (addr, 1));
    }

  return new_rtx;
}

/* Code generation.  */

void
ubicom32_expand_movsi (rtx *operands)
{
  if (GET_CODE (operands[1]) == SYMBOL_REF
      || (GET_CODE (operands[1]) == CONST
	  && GET_CODE (XEXP (operands[1], 0)) == PLUS
	  && GET_CODE (XEXP (XEXP (operands[1], 0), 0)) == SYMBOL_REF)
      || CONSTANT_ADDRESS_P (operands[1]))
    {
      if (TARGET_FDPIC)
	{
	  rtx tmp;
	  rtx fdpic_reg;

	  gcc_assert (can_create_pseudo_p ());
	  tmp = gen_reg_rtx (Pmode);
	  fdpic_reg = get_hard_reg_initial_val (SImode, FDPIC_REGNUM);
	  if (GET_CODE (operands[1]) == SYMBOL_REF
	      || GET_CODE (operands[1]) == LABEL_REF)
	    operands[1] = ubicom32_legitimize_fdpic_address_symbol (operands[1], tmp, fdpic_reg);
	  else
	    operands[1] = ubicom32_legitimize_fdpic_address (operands[1], tmp, fdpic_reg);
	}
      else
	{
	  rtx tmp;
	  enum machine_mode mode;

	  /* We want to avoid reusing operand 0 if we can because it limits
	     our ability to optimize later.  */
	  tmp = ! can_create_pseudo_p () ? operands[0] : gen_reg_rtx (Pmode);

	  mode = GET_MODE (operands[0]);
	  emit_insn (gen_rtx_SET (VOIDmode, tmp,
				  gen_rtx_HIGH (mode, operands[1])));
	  operands[1] = gen_rtx_LO_SUM (mode, tmp, operands[1]);
	  if (can_create_pseudo_p() && ! REG_P (operands[0]))
	    {
	      tmp = gen_reg_rtx (mode);
	      emit_insn (gen_rtx_SET (VOIDmode, tmp, operands[1]));
	      operands[1] = tmp;
	    }
	}
    }
}

/* Emit code for addsi3.  */

void
ubicom32_expand_addsi3 (rtx *operands)
{
  rtx op, clob;

  if (can_create_pseudo_p ())
    {
      /* If we have a non-data reg for operand 1 then prefer that over
         a CONST_INT in operand 2.  */
      if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	  && CONST_INT_P (operands[2]))
	operands[2] = copy_to_mode_reg (SImode, operands[2]);

      if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
	operands[2] = copy_to_mode_reg (SImode, operands[2]);
    }

  /* Emit the instruction.  */

  op = gen_rtx_SET (VOIDmode, operands[0],
		    gen_rtx_PLUS (SImode, operands[1], operands[2]));

  if (! can_create_pseudo_p ())
    {
      /* Reload doesn't know about the flags register, and doesn't know that
         it doesn't want to clobber it.  We can only do this with PLUS.  */
      emit_insn (op);
    }
  else
    {
      clob = gen_rtx_CLOBBER (VOIDmode, gen_rtx_REG (CCmode, CC_REGNUM));
      emit_insn (gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2, op, clob)));
    }
}

/* Emit code for mulsi3.  Return 1 if we have generated all the code
   necessary to do the multiplication.  */

int
ubicom32_emit_mult_sequence (rtx *operands)
{
  if (! ubicom32_v4)
    {
      rtx a1, a1_1, a2;
      rtx b1, b1_1, b2;
      rtx mac_lo_rtx;
      rtx t1, t2, t3;

      /* Give up if we cannot create new pseudos.  */
      if (!can_create_pseudo_p())
	return 0;

      /* Synthesize 32-bit multiplication using 16-bit operations:
     
	 a1 = highpart (a)
	 a2 = lowpart (a)

	 b1 = highpart (b)
	 b2 = lowpart (b)

	 c = (a1 * b1) << 32 + (a1 * b2) << 16 + (a2 * b1) << 16 + a2 * b2
	   =        0        + (a1 * b2) << 16 + (a2 * b1) << 16 + a2 * b2
	                       ^^^^^^^^^^^^^^^   ^^^^^^^^^^^^^^^   ^^^^^^^
			           Signed             Signed      Unsigned  */

      if (!ubicom32_data_register_operand (operands[1], GET_MODE (operands[1])))
	{
	  rtx op1;

	  op1 = gen_reg_rtx (SImode);
	  emit_move_insn (op1, operands[1]);
	  operands[1] = op1;
	}

      if (!ubicom32_data_register_operand (operands[2], GET_MODE (operands[2])))
	{
	  rtx op2;

	  op2 = gen_reg_rtx (SImode);
	  emit_move_insn (op2, operands[2]);
	  operands[2] = op2;
	}

      /* a1 = highpart (a)  */
      a1 = gen_reg_rtx (HImode);
      a1_1 = gen_reg_rtx (SImode);
      emit_insn (gen_ashrsi3 (a1_1, operands[1], GEN_INT (16)));
      emit_move_insn (a1, gen_lowpart (HImode, a1_1));

      /* a2 = lowpart (a)  */
      a2 = gen_reg_rtx (HImode);
      emit_move_insn (a2, gen_lowpart (HImode, operands[1]));

      /* b1 = highpart (b)  */
      b1 = gen_reg_rtx (HImode);
      b1_1 = gen_reg_rtx (SImode);
      emit_insn (gen_ashrsi3 (b1_1, operands[2], GEN_INT (16)));
      emit_move_insn (b1, gen_lowpart (HImode, b1_1));

      /* b2 = lowpart (b)  */
      b2 = gen_reg_rtx (HImode);
      emit_move_insn (b2, gen_lowpart (HImode, operands[2]));

      /* t1 = (a1 * b2) << 16  */
      t1 = gen_reg_rtx (SImode);
      mac_lo_rtx = gen_rtx_REG (SImode, ACC0_LO_REGNUM);
      emit_insn (gen_mulhisi3 (mac_lo_rtx, a1, b2));
      emit_insn (gen_ashlsi3 (t1, mac_lo_rtx, GEN_INT (16)));

      /* t2 = (a2 * b1) << 16  */
      t2 = gen_reg_rtx (SImode);
      emit_insn (gen_mulhisi3 (mac_lo_rtx, a2, b1));
      emit_insn (gen_ashlsi3 (t2, mac_lo_rtx, GEN_INT (16)));

      /* mac_lo = a2 * b2  */
      emit_insn (gen_umulhisi3 (mac_lo_rtx, a2, b2));

      /* t3 = t1 + t2  */
      t3 = gen_reg_rtx (SImode);
      emit_insn (gen_addsi3 (t3, t1, t2));

      /* c = t3 + mac_lo_rtx  */
      emit_insn (gen_addsi3 (operands[0], mac_lo_rtx, t3));

      return 1;
    }
  else
    {
      rtx acc_rtx;

      /* Give up if we cannot create new pseudos.  */
      if (!can_create_pseudo_p())
	return 0;

      if (!ubicom32_data_register_operand (operands[1], GET_MODE (operands[1])))
        {
	  rtx op1;

	  op1 = gen_reg_rtx (SImode);
	  emit_move_insn (op1, operands[1]);
	  operands[1] = op1;
	}

      if (!ubicom32_data_register_operand (operands[2], GET_MODE (operands[2])))
	{
	  rtx op2;

	  op2 = gen_reg_rtx (SImode);
	  emit_move_insn (op2, operands[2]);
	  operands[2] = op2;
	}

      acc_rtx = gen_reg_rtx (DImode);
      emit_insn (gen_umulsidi3 (acc_rtx, operands[1], operands[2]));
      emit_move_insn (operands[0], gen_lowpart (SImode, acc_rtx));

      return 1;
    }
}

/* Move the integer value VAL into OPERANDS[0].  */

void
ubicom32_emit_move_const_int (rtx dest, rtx imm)
{
  rtx xoperands[2];
  
  xoperands[0] = dest;
  xoperands[1] = imm;

  /* Treat mem destinations separately.  Values must be explicitly sign
     extended.  */
  if (MEM_P (dest))
    {
      rtx low_hword_mem;
      rtx low_hword_addr;

      /* Emit shorter sequence for signed 7-bit quantities.  */
      if (satisfies_constraint_I (imm))
	{
          output_asm_insn ("move.4\t%0, %1", xoperands);
          return;
	}

      /* Special case for pushing constants.  */
      if (GET_CODE (XEXP (dest, 0)) == PRE_DEC
	  && XEXP (XEXP (dest, 0), 0) == stack_pointer_rtx)
	{
	  output_asm_insn ("movei\t-4(sp)++, #%%hi(%E1)", xoperands);
	  output_asm_insn ("movei\t2(sp), #%%lo(%E1)", xoperands);
	  return;
	}

      /* See if we can add 2 to the original address.  This is only
	 possible if the original address is of the form REG or
	 REG+const.  */
      low_hword_addr = plus_constant (XEXP (dest, 0), 2);
      if (ubicom32_legitimate_address_p (HImode, low_hword_addr, 1))
	{
	  low_hword_mem = gen_rtx_MEM (HImode, low_hword_addr);
	  MEM_COPY_ATTRIBUTES (low_hword_mem, dest);
	  output_asm_insn ("movei\t%0, #%%hi(%E1)", xoperands);
	  xoperands[0] = low_hword_mem;
	  output_asm_insn ("movei\t%0, #%%lo(%E1)", xoperands);
	  return;
	}

      /* The original address is too complex.  We need to use a
	 scratch memory by (sp) and move that to the original
	 destination.  */
      if (! reg_mentioned_p (stack_pointer_rtx, dest))
	{
	  output_asm_insn ("movei\t-4(sp)++, #%%hi(%E1)", xoperands);
	  output_asm_insn ("movei\t2(sp), #%%lo(%E1)", xoperands);
	  output_asm_insn ("move.4\t%0, (sp)4++", xoperands);
	  return;
	}

      /* Our address mentions the stack pointer so we need to
	 use our scratch data register here as well as scratch
	 memory.  */
      output_asm_insn ("movei\t-4(sp)++, #%%hi(%E1)", xoperands);
      output_asm_insn ("movei\t2(sp), #%%lo(%E1)", xoperands);
      output_asm_insn ("move.4\td15, (sp)4++", xoperands);
      output_asm_insn ("move.4\t%0, d15", xoperands);
      return;
    }

  /* Move into registers are zero extended by default.  */
  if (! REG_P (dest))
    abort ();

  if (satisfies_constraint_N (imm))
    {
      output_asm_insn ("movei\t%0, %1", xoperands);
      return;
    }

  if (INTVAL (xoperands[1]) >= 0xff80
      && INTVAL (xoperands[1]) < 0x10000)
    {
      xoperands[1] = GEN_INT (INTVAL (xoperands[1]) - 0x10000);
      output_asm_insn ("move.2\t%0, %1", xoperands);
      return;
    }

  if ((REGNO_REG_CLASS (REGNO (xoperands[0])) == ADDRESS_REGS
       || REGNO_REG_CLASS (REGNO (xoperands[0])) == FDPIC_REG)
      && ((INTVAL (xoperands[1]) & 0x80000000) == 0))
    {
      output_asm_insn ("moveai\t%0, #%%hi(%E1)", xoperands);
      if ((INTVAL (xoperands[1]) & 0x7f) != 0)
	output_asm_insn ("lea.1\t%0, %%lo(%E1)(%0)", xoperands);
      return;
    }

  if ((INTVAL (xoperands[1]) & 0xffff0000) == 0)
    {
      output_asm_insn ("movei\t%0, #%%lo(%E1)", xoperands);
      output_asm_insn ("move.2\t%0, %0", xoperands);
      return;
    }

  /* This is very expensive.  The constant is so large that we
     need to use the stack to do the load.  */
  output_asm_insn ("movei\t-4(sp)++, #%%hi(%E1)", xoperands);
  output_asm_insn ("movei\t2(sp), #%%lo(%E1)", xoperands);
  output_asm_insn ("move.4\t%0, (sp)4++", xoperands);
}

/* Stack layout. Prologue/Epilogue.  */

static int save_regs_size;

static void 
ubicom32_layout_frame (void)
{
  int regno;
  
  memset ((char *) &save_regs[0], 0, sizeof (save_regs));
  nregs = 0;
  frame_size = get_frame_size ();

  if (frame_pointer_needed || df_regs_ever_live_p (FRAME_POINTER_REGNUM))
    {
      save_regs[FRAME_POINTER_REGNUM] = 1;
      ++nregs;
    }

  if (current_function_is_leaf && ! df_regs_ever_live_p (LINK_REGNO))
    ubicom32_can_use_calli_to_ret = 1;
  else
    {
      ubicom32_can_use_calli_to_ret = 0;
      save_regs[LINK_REGNO] = 1;
      ++nregs;
    }

  /* Figure out which register(s) needs to be saved.  */
  for (regno = 0; regno <= LAST_ADDRESS_REGNUM; regno++)
	if (df_regs_ever_live_p(regno)
	&& ! call_used_regs[regno]
	&& ! fixed_regs[regno]
	&& ! save_regs[regno])
    {
      save_regs[regno] = 1;
      ++nregs;
    }

  save_regs_size = 4 * nregs;
}

static void
ubicom32_emit_add_movsi (int regno, int adj)
{
  rtx x;
  rtx reg = gen_rtx_REG (SImode, regno);

  adj += 4;
  if (adj > 8 * 4) 
    {
      x = emit_insn (gen_addsi3 (stack_pointer_rtx, stack_pointer_rtx,
				 GEN_INT (-adj)));
      RTX_FRAME_RELATED_P (x) = 1;
      x = emit_move_insn (gen_rtx_MEM (SImode, stack_pointer_rtx), reg);
    }
  else
    {
      rtx addr = gen_rtx_PRE_MODIFY (Pmode, stack_pointer_rtx,
				     gen_rtx_PLUS (Pmode, stack_pointer_rtx,
						   GEN_INT (-adj)));
      x = emit_move_insn (gen_rtx_MEM (SImode, addr), reg);
    }
  RTX_FRAME_RELATED_P (x) = 1;      
}

void
ubicom32_expand_prologue (void)
{
  rtx x;
  int regno;
  int outgoing_args_size = crtl->outgoing_args_size;
  int adj;

  if (ubicom32_naked_function_p ())
    return;

  ubicom32_builtin_saveregs ();
  
  ubicom32_layout_frame ();
  adj = (outgoing_args_size + get_frame_size () + save_regs_size
	 + crtl->args.pretend_args_size);
  
  if (!adj)
    ;
  else if (outgoing_args_size + save_regs_size < 508
	   && get_frame_size () + save_regs_size > 508)
    {
      int i = 0;
      x = gen_addsi3 (stack_pointer_rtx, stack_pointer_rtx,
		      GEN_INT (-adj));
      x = emit_insn (x);
      RTX_FRAME_RELATED_P (x) = 1;

      for (regno = LAST_ADDRESS_REGNUM; regno >= 0; --regno)
	if (save_regs[regno] && regno != LINK_REGNO)
	  {
	    x = gen_rtx_MEM (SImode,
			     gen_rtx_PLUS (Pmode,
					   stack_pointer_rtx,
					   GEN_INT (i * 4 + outgoing_args_size)));
	    x = emit_move_insn (x, gen_rtx_REG (SImode, regno));
	    RTX_FRAME_RELATED_P (x) = 1;
	    ++i;
	  }
      if (save_regs[LINK_REGNO])
	{
	  x = gen_rtx_MEM (SImode,
			   gen_rtx_PLUS (Pmode,
					 stack_pointer_rtx,
					 GEN_INT (i * 4 + outgoing_args_size)));
	  x = emit_move_insn (x, gen_rtx_REG (SImode, LINK_REGNO));
	  RTX_FRAME_RELATED_P (x) = 1;
	}
    }
  else
    {
      int regno;
      int adj = get_frame_size () + crtl->args.pretend_args_size;
      int i = 0;

      if (save_regs[LINK_REGNO])
	{
	  ubicom32_emit_add_movsi (LINK_REGNO, adj);
	  ++i;
	}
      
      for (regno = 0; regno <= LAST_ADDRESS_REGNUM; ++regno)
	if (save_regs[regno] && regno != LINK_REGNO)
	  {
	    if (i)
	      {
		rtx mem = gen_rtx_MEM (SImode,
				       gen_rtx_PRE_DEC (Pmode,
							stack_pointer_rtx));
		x = emit_move_insn (mem, gen_rtx_REG (SImode, regno));
		RTX_FRAME_RELATED_P (x) = 1;
	      }
	    else
	      ubicom32_emit_add_movsi (regno, adj);
	    ++i;
	  }
      
      if (outgoing_args_size || (!i && adj))
	{
	  x = gen_addsi3 (stack_pointer_rtx, stack_pointer_rtx,
			  GEN_INT (-outgoing_args_size - (i ? 0 : adj)));
	  x = emit_insn (x);
	  RTX_FRAME_RELATED_P (x) = 1;
	}
    }

  if (frame_pointer_needed)
    {
      int fp_adj = save_regs_size + outgoing_args_size;
      x = gen_addsi3 (frame_pointer_rtx, stack_pointer_rtx,
		      GEN_INT (fp_adj));
      x = emit_insn (x);
      RTX_FRAME_RELATED_P (x) = 1;
    }
}

void
ubicom32_expand_epilogue (void)
{
  rtx x;
  int regno;
  int outgoing_args_size = crtl->outgoing_args_size;
  int adj;
  int i;

  if (ubicom32_naked_function_p ())
    {
      emit_jump_insn (gen_return_internal (gen_rtx_REG (SImode,
						        LINK_REGNO)));
      return;
    }

  if (cfun->calls_alloca)
    {
      x = gen_addsi3 (stack_pointer_rtx, frame_pointer_rtx,
		      GEN_INT (-save_regs_size));
      emit_insn (x);
      outgoing_args_size = 0;
    }
  
  if (outgoing_args_size)
    {
      x = gen_addsi3 (stack_pointer_rtx, stack_pointer_rtx,
		      GEN_INT (outgoing_args_size));
      emit_insn (x);
    }

  i = 0;
  for (regno = LAST_ADDRESS_REGNUM; regno >= 0; --regno)
    if (save_regs[regno] && regno != LINK_REGNO)
      {
	x = gen_rtx_MEM (SImode, gen_rtx_POST_INC (Pmode, stack_pointer_rtx));
	emit_move_insn (gen_rtx_REG (SImode, regno), x);
	++i;
      }

  /* Do we have to adjust the stack after we've finished restoring regs?  */
  adj = get_frame_size() + crtl->args.pretend_args_size;
  if (cfun->stdarg)
    adj += UBICOM32_FUNCTION_ARG_REGS * UNITS_PER_WORD;
 
#if 0
  if (crtl->calls_eh_return && 0)
    {
      if (save_regs[LINK_REGNO])
        {
          x = gen_rtx_MEM (SImode, gen_rtx_POST_INC (Pmode, stack_pointer_rtx));
          emit_move_insn (gen_rtx_REG (SImode, LINK_REGNO), x);
        }

      if (adj)
        {
          x = gen_addsi3 (stack_pointer_rtx, stack_pointer_rtx,
			  GEN_INT (adj));
          x = emit_insn (x);
        }

      /* Perform the additional bump for __throw.  */
      emit_insn (gen_addsi3 (stack_pointer_rtx, stack_pointer_rtx,
			     EH_RETURN_STACKADJ_RTX));
      emit_jump_insn (gen_eh_return_internal ());
      return;
    }
#endif

  if (save_regs[LINK_REGNO])
    {
      if (adj >= 4 && adj <= (6 * 4))
        {
	  x = GEN_INT (adj + 4);
          emit_jump_insn (gen_return_from_post_modify_sp (x));
	  return;
        }

      if (adj == 0)
	{
          x = gen_rtx_MEM (SImode, gen_rtx_POST_INC (Pmode, stack_pointer_rtx));
          emit_jump_insn (gen_return_internal (x));
          return;
	}

      x = gen_rtx_MEM (SImode, gen_rtx_POST_INC (Pmode, stack_pointer_rtx));
      emit_move_insn (gen_rtx_REG (SImode, LINK_REGNO), x);
    }

  if (adj)
    {
      x = gen_addsi3 (stack_pointer_rtx, stack_pointer_rtx,
		      GEN_INT (adj));
      x = emit_insn (x);
      adj = 0;
    }

  /* Given that we've just done all the hard work here we may as well use
     a calli to return.  */
  ubicom32_can_use_calli_to_ret = 1;
  emit_jump_insn (gen_return_internal (gen_rtx_REG (SImode, LINK_REGNO)));
}

void
ubicom32_expand_call_fdpic (rtx *operands)
{
  rtx c;
  rtx addr;
  rtx fdpic_reg = get_hard_reg_initial_val (SImode, FDPIC_REGNUM);

  addr = XEXP (operands[0], 0);

  c = gen_call_fdpic (addr, operands[1], fdpic_reg);
  emit_call_insn (c);
}

void
ubicom32_expand_call_value_fdpic (rtx *operands)
{
  rtx c;
  rtx addr;
  rtx fdpic_reg = get_hard_reg_initial_val (SImode, FDPIC_REGNUM);

  addr = XEXP (operands[1], 0);

  c = gen_call_value_fdpic (operands[0], addr, operands[2], fdpic_reg);
  emit_call_insn (c);
}

void
ubicom32_expand_eh_return (rtx *operands)
{
  if (REG_P (operands[0])
      || REGNO (operands[0]) != EH_RETURN_STACKADJ_REGNO)
    {
      rtx sp = EH_RETURN_STACKADJ_RTX;
      emit_move_insn (sp, operands[0]);
      operands[0] = sp;
    }

  if (REG_P (operands[1])
      || REGNO (operands[1]) != EH_RETURN_HANDLER_REGNO)
    {
      rtx ra = EH_RETURN_HANDLER_RTX;
      emit_move_insn (ra, operands[1]);
      operands[1] = ra;
    }
}

/* Compute the offsets between eliminable registers.  */

int
ubicom32_initial_elimination_offset (int from, int to)
{
  ubicom32_layout_frame ();
  if (from == FRAME_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
    return save_regs_size + crtl->outgoing_args_size;

  if (from == ARG_POINTER_REGNUM && to == FRAME_POINTER_REGNUM)
    return get_frame_size ()/* + save_regs_size */;

  if (from == ARG_POINTER_REGNUM && to == STACK_POINTER_REGNUM)
    return get_frame_size ()
	   + crtl->outgoing_args_size
	   + save_regs_size;

  return 0;
}

/* Return 1 if it is appropriate to emit `ret' instructions in the
   body of a function.  Do this only if the epilogue is simple, needing a
   couple of insns.  Prior to reloading, we can't tell how many registers
   must be saved, so return 0 then.  Return 0 if there is no frame
   marker to de-allocate.

   If NON_SAVING_SETJMP is defined and true, then it is not possible
   for the epilogue to be simple, so return 0.  This is a special case
   since NON_SAVING_SETJMP will not cause regs_ever_live to change
   until final, but jump_optimize may need to know sooner if a
   `return' is OK.  */

int
ubicom32_can_use_return_insn_p (void)
{
  if (! reload_completed || frame_pointer_needed)
    return 0;

  return 1;
}

/* Attributes and CC handling.  */

/* Handle an attribute requiring a FUNCTION_DECL; arguments as in
   struct attribute_spec.handler.  */
static tree
ubicom32_handle_fndecl_attribute (tree *node, tree name,
			      tree args ATTRIBUTE_UNUSED,
			      int flags ATTRIBUTE_UNUSED,
			      bool *no_add_attrs)
{
  if (TREE_CODE (*node) != FUNCTION_DECL)
    {
      warning ("'%s' attribute only applies to functions",
	       IDENTIFIER_POINTER (name));
      *no_add_attrs = true;
    }

  return NULL_TREE;
}

/* A C expression that places additional restrictions on the register class to
   use when it is necessary to copy value X into a register in class CLASS.
   The value is a register class; perhaps CLASS, or perhaps another, smaller
   class.  On many machines, the following definition is safe:

        #define PREFERRED_RELOAD_CLASS(X,CLASS) CLASS

   Sometimes returning a more restrictive class makes better code.  For
   example, on the 68000, when X is an integer constant that is in range for a
   `moveq' instruction, the value of this macro is always `DATA_REGS' as long
   as CLASS includes the data registers.  Requiring a data register guarantees
   that a `moveq' will be used.

   If X is a `const_double', by returning `NO_REGS' you can force X into a
   memory constant.  This is useful on certain machines where immediate
   floating values cannot be loaded into certain kinds of registers.  */

enum reg_class
ubicom32_preferred_reload_class (rtx x, enum reg_class class)
{
  /* If a symbolic constant, HIGH or a PLUS is reloaded,
     it is most likely being used as an address, so
     prefer ADDRESS_REGS.  If 'class' is not a superset
     of ADDRESS_REGS, e.g. DATA_REGS, then reject this reload.  */
  if (GET_CODE (x) == PLUS
      || GET_CODE (x) == HIGH
      || GET_CODE (x) == LABEL_REF
      || GET_CODE (x) == SYMBOL_REF
      || GET_CODE (x) == CONST)
    {
      if (reg_class_subset_p (ALL_ADDRESS_REGS, class))
	return ALL_ADDRESS_REGS;

      return NO_REGS;
    }

  return class;
}

/* Function arguments and varargs.  */

int
ubicom32_reg_parm_stack_space (tree fndecl)
{
  return 0;
  
  if (fndecl
      && TYPE_ARG_TYPES (TREE_TYPE (fndecl)) != 0
      && (TREE_VALUE (tree_last (TYPE_ARG_TYPES (TREE_TYPE (fndecl)))) 
	  != void_type_node))
    return UBICOM32_FUNCTION_ARG_REGS * UNITS_PER_WORD;

  return 0;
}

/* Flush the argument registers to the stack for a stdarg function;
   return the new argument pointer.  */

rtx
ubicom32_builtin_saveregs (void)
{
  int regno;
  
  if (! cfun->stdarg)
    return 0;
  
  for (regno = UBICOM32_FUNCTION_ARG_REGS - 1; regno >= 0; --regno)
    emit_move_insn (gen_rtx_MEM (SImode,
				 gen_rtx_PRE_DEC (SImode,
						  stack_pointer_rtx)),
		    gen_rtx_REG (SImode, regno));
  
  return stack_pointer_rtx;
}

void
ubicom32_va_start (tree valist, rtx nextarg)
{
  std_expand_builtin_va_start (valist, nextarg);
}

rtx
ubicom32_va_arg (tree valist, tree type)
{
  HOST_WIDE_INT size, rsize;
  tree addr, incr, tmp;
  rtx addr_rtx;
  int indirect = 0;

  /* Round up sizeof(type) to a word.  */
  size = int_size_in_bytes (type);
  rsize = (size + UNITS_PER_WORD - 1) & -UNITS_PER_WORD;

  /* Large types are passed by reference.  */
  if (size > 8)
    {
      indirect = 1;
      size = rsize = UNITS_PER_WORD;
    }

  incr = valist;
  addr = incr = save_expr (incr);

  /* FIXME Nat's version - is it correct?  */
  tmp = fold_convert (ptr_type_node, size_int (rsize));
  tmp = build2 (PLUS_EXPR, ptr_type_node, incr, tmp);
  incr = fold (tmp);

  /* FIXME Nat's version - is it correct? */
  incr = build2 (MODIFY_EXPR, ptr_type_node, valist, incr);

  TREE_SIDE_EFFECTS (incr) = 1;
  expand_expr (incr, const0_rtx, VOIDmode, EXPAND_NORMAL);

  addr_rtx = expand_expr (addr, NULL, Pmode, EXPAND_NORMAL);

  if (size < UNITS_PER_WORD)
    emit_insn (gen_addsi3 (addr_rtx, addr_rtx,
			   GEN_INT (UNITS_PER_WORD - size)));

  if (indirect)
    {
      addr_rtx = force_reg (Pmode, addr_rtx);
      addr_rtx = gen_rtx_MEM (Pmode, addr_rtx);
      set_mem_alias_set (addr_rtx, get_varargs_alias_set ());
    }

  return addr_rtx;
}

void
init_cumulative_args (CUMULATIVE_ARGS *cum, tree fntype, rtx libname,
		      int indirect ATTRIBUTE_UNUSED)
{
  cum->nbytes = 0;

  if (!libname)
    {
      cum->stdarg = (TYPE_ARG_TYPES (fntype) != 0
		     && (TREE_VALUE (tree_last (TYPE_ARG_TYPES (fntype)))
			 != void_type_node));
    }
}

/* Return an RTX to represent where a value in mode MODE will be passed
   to a function.  If the result is 0, the argument will be pushed.  */

rtx
function_arg (CUMULATIVE_ARGS *cum, enum machine_mode mode, tree type,
	      int named ATTRIBUTE_UNUSED)
{
  rtx result = 0;
  int size, align;
  int nregs = UBICOM32_FUNCTION_ARG_REGS;
  
  /* Figure out the size of the object to be passed.  */
  if (mode == BLKmode)
    size = int_size_in_bytes (type);
  else
    size = GET_MODE_SIZE (mode);

  /* Figure out the alignment of the object to be passed.  */
  align = size;

  cum->nbytes = (cum->nbytes + 3) & ~3;

  /* Don't pass this arg via a register if all the argument registers
     are used up.  */
  if (cum->nbytes >= nregs * UNITS_PER_WORD)
    return 0;

  /* Don't pass this arg via a register if it would be split between
     registers and memory.  */
  result = gen_rtx_REG (mode, cum->nbytes / UNITS_PER_WORD);

  return result;
}

rtx
function_incoming_arg (CUMULATIVE_ARGS *cum, enum machine_mode mode, tree type,
		       int named ATTRIBUTE_UNUSED)
{
  if (cfun->stdarg)
    return 0;

  return function_arg (cum, mode, type, named);
}


/* Implement hook TARGET_ARG_PARTIAL_BYTES.

   Returns the number of bytes at the beginning of an argument that
   must be put in registers.  The value must be zero for arguments
   that are passed entirely in registers or that are entirely pushed
   on the stack.  */
static int
ubicom32_arg_partial_bytes (CUMULATIVE_ARGS *cum, enum machine_mode mode,
			    tree type, bool named ATTRIBUTE_UNUSED)
{
  int size, diff;

  int nregs = UBICOM32_FUNCTION_ARG_REGS;

  /* round up to full word */
  cum->nbytes = (cum->nbytes + 3) & ~3;

  if (targetm.calls.pass_by_reference (cum, mode, type, named))
      return 0;

  /* number of bytes left in registers */
  diff = nregs*UNITS_PER_WORD - cum->nbytes;

  /* regs all used up */
  if (diff <= 0)
    return 0;

  /* Figure out the size of the object to be passed.  */
  if (mode == BLKmode)
    size = int_size_in_bytes (type);
  else
    size = GET_MODE_SIZE (mode);

  /* enough space left in regs for size */
  if (size <= diff)
    return 0;

  /* put diff bytes in regs and rest on stack */
  return diff;

}

static bool
ubicom32_pass_by_reference (CUMULATIVE_ARGS *ca ATTRIBUTE_UNUSED,
			    enum machine_mode mode, const_tree type,
			    bool named ATTRIBUTE_UNUSED)
{
  int size;

  if (type)
    size = int_size_in_bytes (type);
  else
    size = GET_MODE_SIZE (mode);

  return size <= 0 || size > 8;
}

static bool
ubicom32_callee_copies (CUMULATIVE_ARGS *ca ATTRIBUTE_UNUSED,
			enum machine_mode mode, const_tree type,
			bool named ATTRIBUTE_UNUSED)
{
  int size;

  if (type)
    size = int_size_in_bytes (type);
  else
    size = GET_MODE_SIZE (mode);

  return size <= 0 || size > 8;
}
 
static bool
ubicom32_return_in_memory (const_tree type, const_tree fntype ATTRIBUTE_UNUSED)
{
  int size, mode;

  if (!type)
    return true;

  size = int_size_in_bytes(type);
  if (size > 8) 
    return true;

  mode = TYPE_MODE(type);
  if (mode == BLKmode)
    return true;

  return false;
}

/* Return true if a given register number REGNO is acceptable for machine
   mode MODE.  */
bool
ubicom32_hard_regno_mode_ok (unsigned int regno, enum machine_mode mode)
{
  /* If we're not at least a v3 ISA then ACC0_HI is only 16 bits.  */
  if (! ubicom32_v3)
    {
      if (regno == ACC0_HI_REGNUM)
	return (mode == QImode || mode == HImode);
    }

  /* Only the flags reg can hold CCmode.  */
  if (GET_MODE_CLASS (mode) == MODE_CC)
    return regno == CC_REGNUM;

  /* We restrict the choice of DImode registers to only being address,
     data or accumulator regs.  We also restrict them to only start on
     even register numbers so we never have to worry about partial
     overlaps between operands in instructions.  */
  if (GET_MODE_SIZE (mode) > 4)
    {
      switch (REGNO_REG_CLASS (regno))
	{
	case ADDRESS_REGS:
	case DATA_REGS:
	case ACC_REGS:
	  return (regno & 1) == 0;

        default:
	  return false;
	}
    }

  return true;
}

/* The macros REG_OK_FOR..._P assume that the arg is a REG rtx
   and check its validity for a certain class.
   We have two alternate definitions for each of them.
   The usual definition accepts all pseudo regs; the other rejects
   them unless they have been allocated suitable hard regs.
   The symbol REG_OK_STRICT causes the latter definition to be used.

   Most source files want to accept pseudo regs in the hope that
   they will get allocated to the class that the insn wants them to be in.
   Source files for reload pass need to be strict.
   After reload, it makes no difference, since pseudo regs have
   been eliminated by then.  

   These assume that REGNO is a hard or pseudo reg number.
   They give nonzero only if REGNO is a hard reg of the suitable class
   or a pseudo reg currently allocated to a suitable hard reg.
   Since they use reg_renumber, they are safe only once reg_renumber
   has been allocated, which happens in local-alloc.c.  */

int
ubicom32_regno_ok_for_base_p (int regno, int strict)
{
  if ((regno >= FIRST_ADDRESS_REGNUM && regno <= STACK_POINTER_REGNUM) 
      || (!strict
	  && (regno >= FIRST_PSEUDO_REGISTER
	      || regno == ARG_POINTER_REGNUM))
      || (strict && (reg_renumber 
		     && reg_renumber[regno] >= FIRST_ADDRESS_REGNUM
		     && reg_renumber[regno] <= STACK_POINTER_REGNUM)))
    return 1;

  return 0;
}

int
ubicom32_regno_ok_for_index_p (int regno, int strict)
{
  if ((regno >= FIRST_DATA_REGNUM && regno <= LAST_DATA_REGNUM)
      || (!strict && regno >= FIRST_PSEUDO_REGISTER)
      || (strict && (reg_renumber 
		     && reg_renumber[regno] >= FIRST_DATA_REGNUM
		     && reg_renumber[regno] <= LAST_DATA_REGNUM)))
    return 1;

  return 0;
}

/* Returns 1 if X is a valid index register.  STRICT is 1 if only hard
   registers should be accepted.  Accept either REG or SUBREG where a
   register is valid.  */

static bool
ubicom32_is_index_reg (rtx x, int strict)
{
  if ((REG_P (x) && ubicom32_regno_ok_for_index_p (REGNO (x), strict))
      || (GET_CODE (x) == SUBREG && REG_P (SUBREG_REG (x))
	  && ubicom32_regno_ok_for_index_p (REGNO (SUBREG_REG (x)), strict)))
    return true;

  return false;
}

/* Return 1 if X is a valid index for a memory address.  */

static bool
ubicom32_is_index_expr (enum machine_mode mode, rtx x, int strict)
{
  /* Immediate index must be an unsigned 7-bit offset multiple of 1, 2
     or 4 depending on mode.  */
  if (CONST_INT_P (x))
    {
      switch (mode)
	{
	case QImode:
	  return satisfies_constraint_J (x);
	  
	case HImode:
	  return satisfies_constraint_K (x);

	case SImode:
	case SFmode:
	  return satisfies_constraint_L (x);

	case DImode:
	  return satisfies_constraint_L (x)
		 && satisfies_constraint_L (GEN_INT (INTVAL (x) + 4));
	  
	default:	  
	  return false;
	}
    }

  if (mode != SImode && mode != HImode && mode != QImode)
    return false;

  /* Register index scaled by mode of operand: REG + REG * modesize.
     Valid scaled index registers are:

     SImode	(mult (dreg) 4))
     HImode	(mult (dreg) 2))
     QImode	(mult (dreg) 1))  */
  if (GET_CODE (x) == MULT
      && ubicom32_is_index_reg (XEXP (x, 0), strict)
      && CONST_INT_P (XEXP (x, 1))
      && INTVAL (XEXP (x, 1)) == (HOST_WIDE_INT)GET_MODE_SIZE (mode))
    return true;

  /* REG + REG addressing is allowed for QImode.  */
  if (ubicom32_is_index_reg (x, strict) && mode == QImode)
    return true;

  return false;
}

static bool
ubicom32_is_valid_offset (enum machine_mode mode, HOST_WIDE_INT offs)
{
  if (offs < 0)
    return false;

  switch (mode)
    {
    case QImode:
      return offs <= 127;

    case HImode:
      return offs <= 254;

    case SImode:
    case SFmode:
      return offs <= 508;

    case DImode:
      return offs <= 504;

    default:
      return false;
    }
}

static int
ubicom32_get_valid_offset_mask (enum machine_mode mode)
{
  switch (mode)
    {
    case QImode:
      return 127;

    case HImode:
      return 255;

    case SImode:
    case SFmode:
      return 511;

    case DImode:
      return 255;

    default:
      return 0;
    }
}

/* Returns 1 if X is a valid base register.  STRICT is 1 if only hard
   registers should be accepted.  Accept either REG or SUBREG where a
   register is valid.  */

static bool
ubicom32_is_base_reg (rtx x, int strict)
{
  if ((REG_P (x) && ubicom32_regno_ok_for_base_p (REGNO (x), strict))
      || (GET_CODE (x) == SUBREG && REG_P (SUBREG_REG (x))
	  && ubicom32_regno_ok_for_base_p (REGNO (SUBREG_REG (x)), strict)))
    return true;

  return false;
}

static bool
ubicom32_cannot_force_const_mem (rtx x ATTRIBUTE_UNUSED)
{
  return TARGET_FDPIC;
}

/* Determine if X is a legitimate constant.  */

bool
ubicom32_legitimate_constant_p (rtx x)
{
  /* Among its other duties, LEGITIMATE_CONSTANT_P decides whether
     a constant can be entered into reg_equiv_constant[].  If we return true,
     reload can create new instances of the constant whenever it likes.

     The idea is therefore to accept as many constants as possible (to give
     reload more freedom) while rejecting constants that can only be created
     at certain times.  In particular, anything with a symbolic component will
     require use of the pseudo FDPIC register, which is only available before
     reload.  */
  if (TARGET_FDPIC)
    {
      if (GET_CODE (x) == SYMBOL_REF
	  || (GET_CODE (x) == CONST
	      && GET_CODE (XEXP (x, 0)) == PLUS
	      && GET_CODE (XEXP (XEXP (x, 0), 0)) == SYMBOL_REF)
	  || CONSTANT_ADDRESS_P (x))
	return false;

      return true;
    }

  /* For non-PIC code anything goes!  */
  return true;
}

/* Address validation.  */

bool
ubicom32_legitimate_address_p (enum machine_mode mode, rtx x, int strict)
{
  if (TARGET_DEBUG_ADDRESS)
    {									
      fprintf (stderr, "\n==> GO_IF_LEGITIMATE_ADDRESS%s\n",
	       (strict) ? " (STRICT)" : "");
      debug_rtx (x);
    }									

  if (CONSTANT_ADDRESS_P (x))
    return false;

  if (ubicom32_is_base_reg (x, strict)) 
    return true;

  if ((GET_CODE (x) == POST_INC 
       || GET_CODE (x) == PRE_INC 
       || GET_CODE (x) == POST_DEC 
       || GET_CODE (x) == PRE_DEC)
      && REG_P (XEXP (x, 0))
      && ubicom32_is_base_reg (XEXP (x, 0), strict)
      && mode != DImode)
    return true;

  if ((GET_CODE (x) == PRE_MODIFY || GET_CODE (x) == POST_MODIFY)
      && ubicom32_is_base_reg (XEXP (x, 0), strict)
      && GET_CODE (XEXP (x, 1)) == PLUS
      && rtx_equal_p (XEXP (x, 0), XEXP (XEXP (x, 1), 0))
      && CONST_INT_P (XEXP (XEXP (x, 1), 1))
      && mode != DImode)
    {
      HOST_WIDE_INT disp = INTVAL (XEXP (XEXP (x, 1), 1));
      switch (mode)
	{
	case QImode:
	  return disp >= -8 && disp <= 7;
	  
	case HImode:
	  return disp >= -16 && disp <= 14 && ! (disp & 1);
	  
	case SImode:
	  return disp >= -32 && disp <= 28 && ! (disp & 3);
	  
	default:
	  return false;
	}
    }
  
  /* Accept base + index * scale.  */
  if (GET_CODE (x) == PLUS
      && ubicom32_is_base_reg (XEXP (x, 0), strict)
      && ubicom32_is_index_expr (mode, XEXP (x, 1), strict))
    return true;

  /* Accept index * scale + base.  */
  if (GET_CODE (x) == PLUS
      && ubicom32_is_base_reg (XEXP (x, 1), strict)
      && ubicom32_is_index_expr (mode, XEXP (x, 0), strict))
    return true;

  if (! TARGET_FDPIC)
    {
      /* Accept (lo_sum (reg) (symbol_ref)) that can be used as a mem+7bits
	 displacement operand:

	 moveai a1, #%hi(SYM)
	 move.4 d3, %lo(SYM)(a1)  */
      if (GET_CODE (x) == LO_SUM
	  && ubicom32_is_base_reg (XEXP (x, 0), strict)
	  && (GET_CODE (XEXP (x, 1)) == SYMBOL_REF
	      || GET_CODE (XEXP (x, 1)) == LABEL_REF /* FIXME: wrong */)
	  && mode != DImode)
	return true;
    }

  if (TARGET_DEBUG_ADDRESS)
    fprintf (stderr, "\nNot a legitimate address.\n");

  return false;
}

rtx
ubicom32_legitimize_address (rtx x, rtx oldx ATTRIBUTE_UNUSED,
			     enum machine_mode mode)
{
  if (mode == BLKmode)
    return NULL_RTX;

  if (GET_CODE (x) == PLUS
      && REG_P (XEXP (x, 0))
      && ! REGNO_PTR_FRAME_P (REGNO (XEXP (x, 0))) 
      && CONST_INT_P (XEXP (x, 1))
      && ! ubicom32_is_valid_offset (mode, INTVAL (XEXP (x, 1))))
    {
      rtx base;
      rtx plus;
      rtx new_rtx;
      HOST_WIDE_INT val = INTVAL (XEXP (x, 1));
      HOST_WIDE_INT low = val & ubicom32_get_valid_offset_mask (mode);
      HOST_WIDE_INT high = val ^ low;

      if (val < 0)
	return NULL_RTX;

      if (! low)
	return NULL_RTX;

      /* Reload the high part into a base reg; leave the low part
	 in the mem directly.  */
      base = XEXP (x, 0);
      if (! ubicom32_is_base_reg (base, 0))
	base = copy_to_mode_reg (Pmode, base);

      plus = expand_simple_binop (Pmode, PLUS,
				  gen_int_mode (high, Pmode),
				  base, NULL, 0, OPTAB_WIDEN);
      new_rtx = plus_constant (plus, low);

      return new_rtx;
    }

  return NULL_RTX;
}

/* Try a machine-dependent way of reloading an illegitimate address AD
   operand.  If we find one, push the reload and and return the new address.

   MODE is the mode of the enclosing MEM.  OPNUM is the operand number
   and TYPE is the reload type of the current reload.  */

rtx 
ubicom32_legitimize_reload_address (rtx ad, enum machine_mode mode,
				    int opnum, int type)
{
  /* Is this an address that we've already fixed up?  If it is then
     recognize it and move on.  */
  if (GET_CODE (ad) == PLUS
      && GET_CODE (XEXP (ad, 0)) == PLUS
      && REG_P (XEXP (XEXP (ad, 0), 0))
      && CONST_INT_P (XEXP (XEXP (ad, 0), 1))
      && CONST_INT_P (XEXP (ad, 1)))
    {
      push_reload (XEXP (ad, 0), NULL_RTX, &XEXP (ad, 0), NULL,
		   BASE_REG_CLASS, Pmode, VOIDmode, 0, 0,
		   opnum, (enum reload_type) type);
      return ad;
    }

  /* Have we got an address where the offset is simply out of range?  If
     yes then reload the range as a high part and smaller offset.  */
  if (GET_CODE (ad) == PLUS
      && REG_P (XEXP (ad, 0))
      && REGNO (XEXP (ad, 0)) < FIRST_PSEUDO_REGISTER
      && REGNO_OK_FOR_BASE_P (REGNO (XEXP (ad, 0)))
      && CONST_INT_P (XEXP (ad, 1))
      && ! ubicom32_is_valid_offset (mode, INTVAL (XEXP (ad, 1))))
    {
      rtx temp;
      rtx new_rtx;

      HOST_WIDE_INT val = INTVAL (XEXP (ad, 1));
      HOST_WIDE_INT low = val & ubicom32_get_valid_offset_mask (mode);
      HOST_WIDE_INT high = val ^ low;

      /* Reload the high part into a base reg; leave the low part
	 in the mem directly.  */
      temp = gen_rtx_PLUS (Pmode, XEXP (ad, 0), GEN_INT (high));
      new_rtx = gen_rtx_PLUS (Pmode, temp, GEN_INT (low));

      push_reload (XEXP (new_rtx, 0), NULL_RTX, &XEXP (new_rtx, 0), NULL,
		   BASE_REG_CLASS, Pmode, VOIDmode, 0, 0,
		   opnum, (enum reload_type) type);
      return new_rtx;
    }

  /* If we're presented with an pre/post inc/dec then we must force this
     to be done in an address register.  The register allocator should
     work this out for itself but at times ends up trying to use the wrong
     class.  If we get the wrong class then reload will end up generating
     at least 3 instructions whereas this way we can hopefully keep it to
     just 2.  */
  if ((GET_CODE (ad) == POST_INC 
       || GET_CODE (ad) == PRE_INC 
       || GET_CODE (ad) == POST_DEC 
       || GET_CODE (ad) == PRE_DEC)
      && REG_P (XEXP (ad, 0))
      && REGNO (XEXP (ad, 0)) < FIRST_PSEUDO_REGISTER
      && ! REGNO_OK_FOR_BASE_P (REGNO (XEXP (ad, 0))))
    {
      push_reload (XEXP (ad, 0), XEXP (ad, 0), &XEXP (ad, 0), &XEXP (ad, 0),
		   BASE_REG_CLASS, GET_MODE (XEXP (ad, 0)), GET_MODE (XEXP (ad, 0)), 0, 0,
		   opnum, RELOAD_OTHER);
      return ad;
    }

  return NULL_RTX;
}

/* Compute a (partial) cost for rtx X.  Return true if the complete
   cost has been computed, and false if subexpressions should be
   scanned.  In either case, *TOTAL contains the cost result.  */

static bool
ubicom32_rtx_costs (rtx x, int code, int outer_code, int *total,
		    bool speed ATTRIBUTE_UNUSED)
{
  enum machine_mode mode = GET_MODE (x);

  switch (code)
    {
    case CONST_INT:
      /* Very short constants often fold into instructions so
         we pretend that they don't cost anything!  This is
	 really important as regards zero values as otherwise
	 the compiler has a nasty habit of wanting to reuse
	 zeroes that are in regs but that tends to pessimize
	 the code.  */
      if (satisfies_constraint_I (x))
	{
	  *total = 0;
	  return true;
	}

      /* Bit clearing costs nothing  */
      if (outer_code == AND
	  && exact_log2 (~INTVAL (x)) != -1)
	{
	  *total = 0;
	  return true;
	}

      /* Masking the lower set of bits costs nothing.  */
      if (outer_code == AND
	  && exact_log2 (INTVAL (x) + 1) != -1)
	{
	  *total = 0;
	  return true;
	}

      /* Bit setting costs nothing.  */
      if (outer_code == IOR
	  && exact_log2 (INTVAL (x)) != -1)
	{
	  *total = 0;
	  return true;
	}

      /* Larger constants that can be loaded via movei aren't too
         bad.  If we're just doing a set they cost nothing extra.  */
      if (satisfies_constraint_N (x))
	{
	  if (mode == DImode)
	    *total = COSTS_N_INSNS (2);
	  else 
	    *total = COSTS_N_INSNS (1);
	  return true;
	}

      if (mode == DImode)
	*total = COSTS_N_INSNS (5);
      else
        *total = COSTS_N_INSNS (3);
      return true;

    case CONST_DOUBLE:
      /* We don't optimize CONST_DOUBLEs well nor do we relax them well,
	 so their cost is very high.  */
      *total = COSTS_N_INSNS (6);
      return true;

    case CONST:
    case SYMBOL_REF:
    case MEM:
      *total = 0;
      return true;

    case IF_THEN_ELSE:
      *total = COSTS_N_INSNS (1);
      return true;

    case LABEL_REF:
    case HIGH:
    case LO_SUM:
    case BSWAP:
    case PLUS:
    case MINUS:
    case AND:
    case IOR:
    case XOR:
    case ASHIFT:
    case ASHIFTRT:
    case LSHIFTRT:
    case NEG:
    case NOT:
    case SIGN_EXTEND:
    case ZERO_EXTEND:
    case ZERO_EXTRACT:
      if (outer_code == SET)
	{
	  if (mode == DImode)
	    *total = COSTS_N_INSNS (2);
	  else
	    *total = COSTS_N_INSNS (1);
	}
      return true;

    case COMPARE:
      if (outer_code == SET)
	{
	  if (GET_MODE (XEXP (x, 0)) == DImode
	      || GET_MODE (XEXP (x, 1)) == DImode)
	    *total = COSTS_N_INSNS (2);
	  else
	    *total = COSTS_N_INSNS (1);
	}
      return true;

    case UMOD:
    case UDIV:
    case MOD:
    case DIV:
      if (outer_code == SET)
	{
	  if (mode == DImode)
	    *total = COSTS_N_INSNS (600);
	  else
	    *total = COSTS_N_INSNS (200);
	}
      return true;

    case MULT:
      if (outer_code == SET)
	{
	  if (! ubicom32_v4)
	    {
	      if (mode == DImode)
		*total = COSTS_N_INSNS (15);
	      else
		*total = COSTS_N_INSNS (5);
	    }
	  else
	    {
	      if (mode == DImode)
		*total = COSTS_N_INSNS (6);
	      else
		*total = COSTS_N_INSNS (2);
	    }
	}
      return true;

    case UNSPEC:
      if (XINT (x, 1) == UNSPEC_FDPIC_GOT
	  || XINT (x, 1) == UNSPEC_FDPIC_GOT_FUNCDESC)
	*total = 0;
      return true;

    default:
      return false;
    }
}

/* Return 1 if ADDR can have different meanings depending on the machine
   mode of the memory reference it is used for or if the address is
   valid for some modes but not others.

   Autoincrement and autodecrement addresses typically have
   mode-dependent effects because the amount of the increment or
   decrement is the size of the operand being addressed.  Some machines
   have other mode-dependent addresses. Many RISC machines have no
   mode-dependent addresses.

   You may assume that ADDR is a valid address for the machine.  */

int
ubicom32_mode_dependent_address_p (rtx addr)
{
  if (GET_CODE (addr) == POST_INC 
      || GET_CODE (addr) == PRE_INC 
      || GET_CODE (addr) == POST_DEC 
      || GET_CODE (addr) == PRE_DEC 
      || GET_CODE (addr) == POST_MODIFY 
      || GET_CODE (addr) == PRE_MODIFY)
    return 1;

  return 0;
}

static void
ubicom32_function_prologue (FILE *file, HOST_WIDE_INT size ATTRIBUTE_UNUSED)
{
  fprintf (file, "/* frame/pretend: %ld/%d save_regs: %d out_args: %d  %s */\n",
	   get_frame_size (), crtl->args.pretend_args_size,
	   save_regs_size, crtl->outgoing_args_size,
	   current_function_is_leaf ? "leaf" : "nonleaf");
}

static void
ubicom32_function_epilogue (FILE *file ATTRIBUTE_UNUSED,
			    HOST_WIDE_INT size ATTRIBUTE_UNUSED)
{
  ubicom32_reorg_completed = 0;  
}

static void
ubicom32_machine_dependent_reorg (void)
{
#if 0 /* Commenting out this optimization until it is fixed */
  if (optimize)
    {
      compute_bb_for_insn ();

      /* Do a very simple CSE pass over just the hard registers.  */
      reload_cse_regs (get_insns ());

      /* Reload_cse_regs can eliminate potentially-trapping MEMs.
	 Remove any EH edges associated with them.  */
      if (flag_non_call_exceptions)
	purge_all_dead_edges ();
    }
#endif
  ubicom32_reorg_completed = 1;
}

void
ubicom32_output_cond_jump (rtx insn, rtx cond, rtx target)
{
  rtx note;
  int mostly_false_jump;
  rtx xoperands[2];
  rtx cc_reg;

  note = find_reg_note (insn, REG_BR_PROB, 0);
  mostly_false_jump = !note || (INTVAL (XEXP (note, 0))
				<= REG_BR_PROB_BASE / 2);

  xoperands[0] = target;
  xoperands[1] = cond;
  cc_reg = XEXP (cond, 0);

  if (GET_MODE (cc_reg) == CCWmode
      || GET_MODE (cc_reg) == CCWZmode
      || GET_MODE (cc_reg) == CCWZNmode)
    {
      if (mostly_false_jump)
        output_asm_insn ("jmp%b1.w.f\t%0", xoperands);
      else
        output_asm_insn ("jmp%b1.w.t\t%0", xoperands);
      return;
    }

  if (GET_MODE (cc_reg) == CCSmode
      || GET_MODE (cc_reg) == CCSZmode
      || GET_MODE (cc_reg) == CCSZNmode)
    {
      if (mostly_false_jump)
        output_asm_insn ("jmp%b1.s.f\t%0", xoperands);
      else
        output_asm_insn ("jmp%b1.s.t\t%0", xoperands);
      return;
    }

  abort ();
}

/* Return non-zero if FUNC is a naked function.  */

static int
ubicom32_naked_function_p (void)
{
  return lookup_attribute ("naked", DECL_ATTRIBUTES (current_function_decl)) != NULL_TREE;
}

/* Return an RTX indicating where the return address to the
   calling function can be found.  */
rtx
ubicom32_return_addr_rtx (int count, rtx frame ATTRIBUTE_UNUSED)
{
  if (count != 0)
    return NULL_RTX;

  return get_hard_reg_initial_val (Pmode, LINK_REGNO);
}

/*
 * ubicom32_readonly_data_section: This routtine handles code
 * at the start of readonly data sections
 */
static void
ubicom32_readonly_data_section (const void *data ATTRIBUTE_UNUSED)
{
  static int num = 0;
  if (in_section == readonly_data_section){
    fprintf (asm_out_file, "%s", DATA_SECTION_ASM_OP);
    if (flag_data_sections){
      fprintf (asm_out_file, ".rodata%d", num);
      fprintf (asm_out_file, ",\"a\"");
    }
    fprintf (asm_out_file, "\n");
  }
  num++;
}

/*
 * ubicom32_text_section: not in readonly section
 */
static void
ubicom32_text_section(const void *data ATTRIBUTE_UNUSED)
{
  fprintf (asm_out_file, "%s\n", TEXT_SECTION_ASM_OP);
}

/*
 * ubicom32_data_section: not in readonly section
 */
static void
ubicom32_data_section(const void *data ATTRIBUTE_UNUSED)
{
  fprintf (asm_out_file, "%s\n", DATA_SECTION_ASM_OP);
}

/*
 * ubicom32_asm_init_sections: This routine implements special
 * section handling
 */
static void
ubicom32_asm_init_sections(void)
{
  text_section = get_unnamed_section(SECTION_CODE, ubicom32_text_section, NULL);

  data_section = get_unnamed_section(SECTION_WRITE, ubicom32_data_section, NULL);

  readonly_data_section = get_unnamed_section(0, ubicom32_readonly_data_section, NULL);
}

/*
 * ubicom32_profiler:  This routine would call
 * mcount to support prof and gprof if mcount
 * was supported. Currently, do nothing.
 */
void
ubicom32_profiler(void)
{
}

/* Initialise the builtin functions.  Start by initialising
   descriptions of different types of functions (e.g., void fn(int),
   int fn(void)), and then use these to define the builtins. */
static void
ubicom32_init_builtins (void)
{
  tree endlink;
  tree short_unsigned_endlink;
  tree unsigned_endlink;
  tree short_unsigned_ftype_short_unsigned;
  tree unsigned_ftype_unsigned;

  endlink = void_list_node;

  short_unsigned_endlink
    = tree_cons (NULL_TREE, short_unsigned_type_node, endlink);

  unsigned_endlink
    = tree_cons (NULL_TREE, unsigned_type_node, endlink);

  short_unsigned_ftype_short_unsigned
    = build_function_type (short_unsigned_type_node, short_unsigned_endlink);

  unsigned_ftype_unsigned
    = build_function_type (unsigned_type_node, unsigned_endlink);

  /* Initialise the byte swap function. */
  add_builtin_function ("__builtin_ubicom32_swapb_2",
  			short_unsigned_ftype_short_unsigned,
			UBICOM32_BUILTIN_UBICOM32_SWAPB_2,
			BUILT_IN_MD, NULL,
			NULL_TREE);

  /* Initialise the byte swap function. */
  add_builtin_function ("__builtin_ubicom32_swapb_4",
  			unsigned_ftype_unsigned,
			UBICOM32_BUILTIN_UBICOM32_SWAPB_4,
			BUILT_IN_MD, NULL,
			NULL_TREE);
}

/* Given a builtin function taking 2 operands (i.e., target + source),
   emit the RTL for the underlying instruction. */
static rtx
ubicom32_expand_builtin_2op (enum insn_code icode, tree arglist, rtx target)
{
  tree arg0;
  rtx op0, pat;
  enum machine_mode tmode, mode0;

  /* Grab the incoming argument and emit its RTL. */
  arg0 = TREE_VALUE (arglist);
  op0 = expand_expr (arg0, NULL_RTX, VOIDmode, 0);

  /* Determine the modes of the instruction operands. */
  tmode = insn_data[icode].operand[0].mode;
  mode0 = insn_data[icode].operand[1].mode;

  /* Ensure that the incoming argument RTL is in a register of the
     correct mode. */
  if (!(*insn_data[icode].operand[1].predicate) (op0, mode0))
    op0 = copy_to_mode_reg (mode0, op0);

  /* If there isn't a suitable target, emit a target register. */
  if (target == 0
      || GET_MODE (target) != tmode
      || !(*insn_data[icode].operand[0].predicate) (target, tmode))
    target = gen_reg_rtx (tmode);

  /* Emit and return the new instruction. */
  pat = GEN_FCN (icode) (target, op0);
  if (!pat)
    return 0;
  emit_insn (pat);

  return target;
}

/* Expand a call to a builtin function. */
static rtx
ubicom32_expand_builtin (tree exp, rtx target, rtx subtarget ATTRIBUTE_UNUSED,
			 enum machine_mode mode ATTRIBUTE_UNUSED,
			 int ignore ATTRIBUTE_UNUSED)
{
  tree fndecl = TREE_OPERAND (CALL_EXPR_FN (exp), 0);
  tree arglist = CALL_EXPR_ARGS(exp);
  int fcode = DECL_FUNCTION_CODE (fndecl);

  switch (fcode)
    {
    case UBICOM32_BUILTIN_UBICOM32_SWAPB_2:
      return ubicom32_expand_builtin_2op (CODE_FOR_bswaphi, arglist, target);

    case UBICOM32_BUILTIN_UBICOM32_SWAPB_4:
      return ubicom32_expand_builtin_2op (CODE_FOR_bswapsi, arglist, target);

    default:
      gcc_unreachable();
    }

  /* Should really do something sensible here.  */
  return NULL_RTX;
}

/* Fold any constant argument for a swapb.2 instruction.  */
static tree
ubicom32_fold_builtin_ubicom32_swapb_2 (tree fndecl, tree arglist)
{
  tree arg0;

  arg0 = TREE_VALUE (arglist);

  /* Optimize constant value.  */
  if (TREE_CODE (arg0) == INTEGER_CST)
    {
      HOST_WIDE_INT v;
      HOST_WIDE_INT res;

      v = TREE_INT_CST_LOW (arg0);
      res = ((v >> 8) & 0xff)
	     | ((v & 0xff) << 8);

      return build_int_cst (TREE_TYPE (TREE_TYPE (fndecl)), res);
    }

  return NULL_TREE;
}

/* Fold any constant argument for a swapb.4 instruction.  */
static tree
ubicom32_fold_builtin_ubicom32_swapb_4 (tree fndecl, tree arglist)
{
  tree arg0;

  arg0 = TREE_VALUE (arglist);

  /* Optimize constant value.  */
  if (TREE_CODE (arg0) == INTEGER_CST)
    {
      unsigned HOST_WIDE_INT v;
      unsigned HOST_WIDE_INT res;

      v = TREE_INT_CST_LOW (arg0);
      res = ((v >> 24) & 0xff)
	     | (((v >> 16) & 0xff) << 8)
	     | (((v >> 8) & 0xff) << 16)
	     | ((v & 0xff) << 24);

      return build_int_cst_wide (TREE_TYPE (TREE_TYPE (fndecl)), res, 0);
    }

  return NULL_TREE;
}

/* Fold any constant arguments for builtin functions.  */
static tree
ubicom32_fold_builtin (tree fndecl, tree arglist, bool ignore ATTRIBUTE_UNUSED)
{
  switch (DECL_FUNCTION_CODE (fndecl))
    {
    case UBICOM32_BUILTIN_UBICOM32_SWAPB_2:
      return ubicom32_fold_builtin_ubicom32_swapb_2 (fndecl, arglist);

    case UBICOM32_BUILTIN_UBICOM32_SWAPB_4:
      return ubicom32_fold_builtin_ubicom32_swapb_4 (fndecl, arglist);

    default:
      return NULL;
    }
}

/* Implementation of TARGET_ASM_INTEGER.  When using FD-PIC, we need to
   tell the assembler to generate pointers to function descriptors in
   some cases.  */
static bool
ubicom32_assemble_integer (rtx value, unsigned int size, int aligned_p)
{
  if (TARGET_FDPIC && size == UNITS_PER_WORD)
    {
      if (GET_CODE (value) == SYMBOL_REF
	  && SYMBOL_REF_FUNCTION_P (value))
	{
	  fputs ("\t.picptr\t%funcdesc(", asm_out_file);
	  output_addr_const (asm_out_file, value);
	  fputs (")\n", asm_out_file);
	  return true;
	}

      if (!aligned_p)
	{
	  /* We've set the unaligned SI op to NULL, so we always have to
	     handle the unaligned case here.  */
	  assemble_integer_with_op ("\t.4byte\t", value);
	  return true;
	}
    }

  return default_assemble_integer (value, size, aligned_p);
}

/* If the constant I can be constructed by shifting a source-1 immediate
   by a constant number of bits then return the bit count.  If not
   return 0.  */

int
ubicom32_shiftable_const_int (int i)
{
  int shift = 0;

  /* Note that any constant that can be represented as an immediate to
     a movei instruction is automatically ignored here in the interests
     of the clarity of the output asm code.  */
  if (i >= -32768 && i <= 32767)
    return 0;

  /* Find the number of trailing zeroes.  We could use __builtin_ctz
     here but it's not obvious if this is supported on all build
     compilers so we err on the side of caution.  */
  if ((i & 0xffff) == 0)
    {
      shift += 16;
      i >>= 16;
    }

  if ((i & 0xff) == 0)
    {
      shift += 8;
      i >>= 8;
    }

  if ((i & 0xf) == 0)
    {
      shift += 4;
      i >>= 4;
    }

  if ((i & 0x3) == 0)
    {
      shift += 2;
      i >>= 2;
    }

  if ((i & 0x1) == 0)
    {
      shift += 1;
      i >>= 1;
    }

  if (i >= -128 && i <= 127)
    return shift;

  return 0;
}


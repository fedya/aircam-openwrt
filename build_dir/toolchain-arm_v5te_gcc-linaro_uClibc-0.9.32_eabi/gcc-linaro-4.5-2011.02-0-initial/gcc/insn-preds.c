/* Generated automatically by the program 'build/genpreds'
   from the machine description file '/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md'.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "tm_p.h"
#include "function.h"
#include "insn-config.h"
#include "recog.h"
#include "real.h"
#include "output.h"
#include "flags.h"
#include "hard-reg-set.h"
#include "resource.h"
#include "toplev.h"
#include "reload.h"
#include "regs.h"
#include "tm-constrs.h"

static inline int
s_register_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 23 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);
  /* We don't consider registers whose class is NO_REGS
     to be a register operand.  */
  /* XXX might have to check for lo regs only for thumb ??? */
  return (GET_CODE (op) == REG
	  && (REGNO (op) >= FIRST_PSEUDO_REGISTER
	      || REGNO_REG_CLASS (REGNO (op)) != NO_REGS));
}

int
s_register_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case REG:
    case SUBREG:
      break;
    default:
      return false;
    }
  return (
(s_register_operand_1 (op, mode))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

static inline int
arm_hard_register_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 37 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  return REGNO (op) < FIRST_PSEUDO_REGISTER;
}

int
arm_hard_register_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return ((GET_CODE (op) == REG) && (
(arm_hard_register_operand_1 (op, mode)))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

int
low_register_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return ((GET_CODE (op) == REG) && (
#line 44 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(REGNO (op) <= LAST_LO_REGNUM))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

int
low_reg_or_int_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT) || (low_register_operand (op, mode));
}

static inline int
arm_general_register_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 54 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);

  return (GET_CODE (op) == REG
	  && (REGNO (op) <= LAST_ARM_REGNUM
	      || REGNO (op) >= FIRST_PSEUDO_REGISTER));
}

int
arm_general_register_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case REG:
    case SUBREG:
      break;
    default:
      return false;
    }
  return (
(arm_general_register_operand_1 (op, mode))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

static inline int
f_register_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 65 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);

  /* We don't consider registers whose class is NO_REGS
     to be a register operand.  */
  return (GET_CODE (op) == REG
	  && (REGNO (op) >= FIRST_PSEUDO_REGISTER
	      || REGNO_REG_CLASS (REGNO (op)) == FPA_REGS));
}

int
f_register_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case REG:
    case SUBREG:
      break;
    default:
      return false;
    }
  return (
(f_register_operand_1 (op, mode))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

static inline int
vfp_register_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 78 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);

  /* We don't consider registers whose class is NO_REGS
     to be a register operand.  */
  return (GET_CODE (op) == REG
	  && (REGNO (op) >= FIRST_PSEUDO_REGISTER
	      || REGNO_REG_CLASS (REGNO (op)) == VFP_LO_REGS
	      || (TARGET_VFPD32
		  && REGNO_REG_CLASS (REGNO (op)) == VFP_REGS)));
}

int
vfp_register_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case REG:
    case SUBREG:
      break;
    default:
      return false;
    }
  return (
(vfp_register_operand_1 (op, mode))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

int
subreg_lowpart_operator (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == SUBREG) && (
#line 93 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(subreg_lowpart_p (op)));
}

int
reg_or_int_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT) || (s_register_operand (op, mode));
}

int
arm_immediate_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT) && (
#line 102 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(const_ok_for_arm (INTVAL (op))));
}

int
arm_immediate_di_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
      break;
    default:
      return false;
    }
  return 
#line 108 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(arm_const_double_by_immediates (op));
}

int
arm_neg_immediate_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT) && (
#line 112 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(const_ok_for_arm (-INTVAL (op))));
}

int
arm_not_immediate_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT) && (
#line 116 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(const_ok_for_arm (~INTVAL (op))));
}

int
const0_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT) && (
#line 120 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(INTVAL (op) == 0));
}

int
arm_rhs_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (s_register_operand (op, mode)) || (arm_immediate_operand (op, mode));
}

int
arm_rhsm_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (arm_rhs_operand (op, mode)) || (memory_operand (op, mode));
}

int
arm_add_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (arm_rhs_operand (op, mode)) || (arm_neg_immediate_operand (op, mode));
}

int
arm_addimm_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (arm_immediate_operand (op, mode)) || (arm_neg_immediate_operand (op, mode));
}

int
arm_not_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (arm_rhs_operand (op, mode)) || (arm_not_immediate_operand (op, mode));
}

int
arm_di_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (s_register_operand (op, mode)) || (arm_immediate_di_operand (op, mode));
}

int
offsettable_memory_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return ((GET_CODE (op) == MEM) && (
#line 152 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(offsettable_address_p (reload_completed | reload_in_progress,
				mode, XEXP (op, 0))))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

int
call_memory_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == MEM) && ((
#line 159 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(GET_RTX_CLASS (GET_CODE (XEXP (op, 0)))
			 != RTX_AUTOINC)) && (memory_operand (op, mode)));
}

int
arm_reload_memory_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case MEM:
    case REG:
    case SUBREG:
      break;
    default:
      return false;
    }
  return (
#line 165 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
((!CONSTANT_P (op)
		     && (true_regnum(op) == -1
			 || (GET_CODE (op) == REG
			     && REGNO (op) >= FIRST_PSEUDO_REGISTER))))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

int
arm_float_rhs_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (s_register_operand (op, mode)) || ((GET_CODE (op) == CONST_DOUBLE) && (
#line 175 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(TARGET_FPA && arm_const_double_rtx (op))));
}

int
arm_float_add_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (arm_float_rhs_operand (op, mode)) || ((GET_CODE (op) == CONST_DOUBLE) && (
#line 180 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(TARGET_FPA && neg_const_double_rtx_ok_for_fpa (op))));
}

int
vfp_compare_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (s_register_operand (op, mode)) || ((GET_CODE (op) == CONST_DOUBLE) && (
#line 185 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(arm_const_double_rtx (op))));
}

int
arm_float_compare_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (
#line 188 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(TARGET_VFP)) ? (vfp_compare_operand (op, mode)) : (arm_float_rhs_operand (op, mode));
}

int
index_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (s_register_operand (op, mode)) || ((immediate_operand (op, mode)) && (
#line 196 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
((GET_CODE (op) != CONST_INT
			  || (INTVAL (op) < 4096 && INTVAL (op) > -4096)))));
}

int
shiftable_operator (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case PLUS:
    case MINUS:
    case IOR:
    case XOR:
    case AND:
      break;
    default:
      return false;
    }
  return 
#line 202 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(mode == GET_MODE (op));
}

int
plusminus_operator (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case PLUS:
    case MINUS:
      break;
    default:
      return false;
    }
  return 
#line 207 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(mode == GET_MODE (op));
}

int
logical_binary_operator (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case IOR:
    case XOR:
    case AND:
      break;
    default:
      return false;
    }
  return 
#line 212 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(mode == GET_MODE (op));
}

int
shift_operator (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return ((((GET_CODE (op) == MULT) && (
#line 217 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(power_of_two_operand (XEXP (op, 1), mode)))) || ((GET_CODE (op) == ROTATE) && (
#line 219 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(GET_CODE (XEXP (op, 1)) == CONST_INT
				   && ((unsigned HOST_WIDE_INT) INTVAL (XEXP (op, 1))) < 32)))) || (GET_CODE (op) == ASHIFT || GET_CODE (op) == ASHIFTRT || GET_CODE (op) == LSHIFTRT || GET_CODE (op) == ROTATERT)) && (
#line 222 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(mode == GET_MODE (op)));
}

int
thumb_16bit_operator (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case PLUS:
    case MINUS:
    case AND:
    case IOR:
    case XOR:
      return true;
    default:
      break;
    }
  return false;
}

int
equality_operator (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case EQ:
    case NE:
      return true;
    default:
      break;
    }
  return false;
}

int
arm_comparison_operator (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case EQ:
    case NE:
    case LE:
    case LT:
    case GE:
    case GT:
    case GEU:
    case GTU:
    case LEU:
    case LTU:
      return true;
    default:
      break;
    }
  return (
#line 236 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(TARGET_32BIT && TARGET_HARD_FLOAT
			 && (TARGET_FPA || TARGET_VFP))) && (GET_CODE (op) == UNORDERED || GET_CODE (op) == ORDERED || GET_CODE (op) == UNLT || GET_CODE (op) == UNLE || GET_CODE (op) == UNGE || GET_CODE (op) == UNGT);
}

int
lt_ge_comparison_operator (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case LT:
    case GE:
      return true;
    default:
      break;
    }
  return false;
}

int
minmax_operator (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case SMIN:
    case SMAX:
    case UMIN:
    case UMAX:
      break;
    default:
      return false;
    }
  return 
#line 245 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(mode == GET_MODE (op));
}

int
cc_register (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == REG) && ((
#line 249 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(REGNO (op) == CC_REGNUM)) && ((
#line 250 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(mode == GET_MODE (op))) || (
#line 251 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(mode == VOIDmode && GET_MODE_CLASS (GET_MODE (op)) == MODE_CC))));
}

static inline int
dominant_cc_register_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 255 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  if (mode == VOIDmode)
    {
      mode = GET_MODE (op);
      
      if (GET_MODE_CLASS (mode) != MODE_CC)
	return false;
    }

  return (cc_register (op, mode)
	  && (mode == CC_DNEmode
	     || mode == CC_DEQmode
	     || mode == CC_DLEmode
	     || mode == CC_DLTmode
	     || mode == CC_DGEmode
	     || mode == CC_DGTmode
	     || mode == CC_DLEUmode
	     || mode == CC_DLTUmode
	     || mode == CC_DGEUmode
	     || mode == CC_DGTUmode));
}

int
dominant_cc_register (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == REG) && (
(dominant_cc_register_1 (op, mode)));
}

int
arm_extendqisi_mem_op (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (memory_operand (op, mode)) && (
#line 279 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(arm_legitimate_address_outer_p (mode, XEXP (op, 0),
						    SIGN_EXTEND, 0)));
}

int
arm_reg_or_extendqisi_mem_op (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (arm_extendqisi_mem_op (op, mode)) || (s_register_operand (op, mode));
}

static inline int
power_of_two_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 288 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  HOST_WIDE_INT value = INTVAL (op);

  return value != 0 && (value & (value - 1)) == 0;
}

int
power_of_two_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT) && (
(power_of_two_operand_1 (op, mode)));
}

static inline int
nonimmediate_di_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 296 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
   if (s_register_operand (op, mode))
     return true;

   if (GET_CODE (op) == SUBREG)
     op = SUBREG_REG (op);

   return GET_CODE (op) == MEM && memory_address_p (DImode, XEXP (op, 0));
}

int
nonimmediate_di_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case REG:
    case SUBREG:
    case MEM:
      break;
    default:
      return false;
    }
  return (
(nonimmediate_di_operand_1 (op, mode))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

int
di_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
      return true;
    default:
      break;
    }
  return (GET_CODE (op) == REG || GET_CODE (op) == SUBREG || GET_CODE (op) == MEM) && (nonimmediate_di_operand (op, mode));
}

static inline int
nonimmediate_soft_df_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 313 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  if (s_register_operand (op, mode))
    return true;

  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);

  return GET_CODE (op) == MEM && memory_address_p (DFmode, XEXP (op, 0));
}

int
nonimmediate_soft_df_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case REG:
    case SUBREG:
    case MEM:
      break;
    default:
      return false;
    }
  return (
(nonimmediate_soft_df_operand_1 (op, mode))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

int
soft_df_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_DOUBLE) || ((GET_CODE (op) == REG || GET_CODE (op) == SUBREG || GET_CODE (op) == MEM) && (nonimmediate_soft_df_operand (op, mode)));
}

int
const_shift_count (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT) && (
#line 330 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(((unsigned HOST_WIDE_INT) INTVAL (op)) < 32));
}

static inline int
load_multiple_operation_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 335 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  HOST_WIDE_INT count = XVECLEN (op, 0);
  int dest_regno;
  rtx src_addr;
  HOST_WIDE_INT i = 1, base = 0;
  rtx elt;

  if (low_irq_latency)
    return false;

  if (count <= 1
      || GET_CODE (XVECEXP (op, 0, 0)) != SET)
    return false;

  /* Check to see if this might be a write-back.  */
  if (GET_CODE (SET_SRC (elt = XVECEXP (op, 0, 0))) == PLUS)
    {
      i++;
      base = 1;

      /* Now check it more carefully.  */
      if (GET_CODE (SET_DEST (elt)) != REG
          || GET_CODE (XEXP (SET_SRC (elt), 0)) != REG
          || GET_CODE (XEXP (SET_SRC (elt), 1)) != CONST_INT
          || INTVAL (XEXP (SET_SRC (elt), 1)) != (count - 1) * 4)
        return false;
    }

  /* Perform a quick check so we don't blow up below.  */
  if (count <= i
      || GET_CODE (XVECEXP (op, 0, i - 1)) != SET
      || GET_CODE (SET_DEST (XVECEXP (op, 0, i - 1))) != REG
      || GET_CODE (SET_SRC (XVECEXP (op, 0, i - 1))) != MEM)
    return false;

  dest_regno = REGNO (SET_DEST (XVECEXP (op, 0, i - 1)));
  src_addr = XEXP (SET_SRC (XVECEXP (op, 0, i - 1)), 0);

  for (; i < count; i++)
    {
      elt = XVECEXP (op, 0, i);

      if (GET_CODE (elt) != SET
          || GET_CODE (SET_DEST (elt)) != REG
          || GET_MODE (SET_DEST (elt)) != SImode
          || REGNO (SET_DEST (elt)) != (unsigned int)(dest_regno + i - base)
          || GET_CODE (SET_SRC (elt)) != MEM
          || GET_MODE (SET_SRC (elt)) != SImode
          || GET_CODE (XEXP (SET_SRC (elt), 0)) != PLUS
          || !rtx_equal_p (XEXP (XEXP (SET_SRC (elt), 0), 0), src_addr)
          || GET_CODE (XEXP (XEXP (SET_SRC (elt), 0), 1)) != CONST_INT
          || INTVAL (XEXP (XEXP (SET_SRC (elt), 0), 1)) != (i - base) * 4)
        return false;
    }

  return true;
}

int
load_multiple_operation (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == PARALLEL) && (
(load_multiple_operation_1 (op, mode)));
}

static inline int
store_multiple_operation_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 395 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  HOST_WIDE_INT count = XVECLEN (op, 0);
  int src_regno;
  rtx dest_addr;
  HOST_WIDE_INT i = 1, base = 0;
  rtx elt;

  if (low_irq_latency)
    return false;

  if (count <= 1
      || GET_CODE (XVECEXP (op, 0, 0)) != SET)
    return false;

  /* Check to see if this might be a write-back.  */
  if (GET_CODE (SET_SRC (elt = XVECEXP (op, 0, 0))) == PLUS)
    {
      i++;
      base = 1;

      /* Now check it more carefully.  */
      if (GET_CODE (SET_DEST (elt)) != REG
          || GET_CODE (XEXP (SET_SRC (elt), 0)) != REG
          || GET_CODE (XEXP (SET_SRC (elt), 1)) != CONST_INT
          || INTVAL (XEXP (SET_SRC (elt), 1)) != (count - 1) * 4)
        return false;
    }

  /* Perform a quick check so we don't blow up below.  */
  if (count <= i
      || GET_CODE (XVECEXP (op, 0, i - 1)) != SET
      || GET_CODE (SET_DEST (XVECEXP (op, 0, i - 1))) != MEM
      || GET_CODE (SET_SRC (XVECEXP (op, 0, i - 1))) != REG)
    return false;

  src_regno = REGNO (SET_SRC (XVECEXP (op, 0, i - 1)));
  dest_addr = XEXP (SET_DEST (XVECEXP (op, 0, i - 1)), 0);

  for (; i < count; i++)
    {
      elt = XVECEXP (op, 0, i);

      if (GET_CODE (elt) != SET
          || GET_CODE (SET_SRC (elt)) != REG
          || GET_MODE (SET_SRC (elt)) != SImode
          || REGNO (SET_SRC (elt)) != (unsigned int)(src_regno + i - base)
          || GET_CODE (SET_DEST (elt)) != MEM
          || GET_MODE (SET_DEST (elt)) != SImode
          || GET_CODE (XEXP (SET_DEST (elt), 0)) != PLUS
          || !rtx_equal_p (XEXP (XEXP (SET_DEST (elt), 0), 0), dest_addr)
          || GET_CODE (XEXP (XEXP (SET_DEST (elt), 0), 1)) != CONST_INT
          || INTVAL (XEXP (XEXP (SET_DEST (elt), 0), 1)) != (i - base) * 4)
        return false;
    }

  return true;
}

int
store_multiple_operation (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == PARALLEL) && (
(store_multiple_operation_1 (op, mode)));
}

static inline int
multi_register_push_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 455 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  if ((GET_CODE (XVECEXP (op, 0, 0)) != SET)
      || (GET_CODE (SET_SRC (XVECEXP (op, 0, 0))) != UNSPEC)
      || (XINT (SET_SRC (XVECEXP (op, 0, 0)), 1) != UNSPEC_PUSH_MULT))
    return false;

  return true;
}

int
multi_register_push (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == PARALLEL) && (
(multi_register_push_1 (op, mode)));
}

int
thumb1_cmp_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return ((GET_CODE (op) == REG || GET_CODE (op) == SUBREG) && (s_register_operand (op, mode))) || ((GET_CODE (op) == CONST_INT) && (
#line 473 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(((unsigned HOST_WIDE_INT) INTVAL (op)) < 256)));
}

int
thumb1_cmpneg_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT) && (
#line 477 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(INTVAL (op) < 0 && INTVAL (op) > -256));
}

int
thumb_cbrch_target_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case REG:
    case SUBREG:
    case MEM:
      break;
    default:
      return false;
    }
  return (s_register_operand (op, mode)) || ((
#line 491 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(reload_in_progress || reload_completed)) && (memory_operand (op, mode)));
}

static inline int
cirrus_register_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 501 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);

  return (GET_CODE (op) == REG
	  && (REGNO_REG_CLASS (REGNO (op)) == CIRRUS_REGS
	      || REGNO_REG_CLASS (REGNO (op)) == GENERAL_REGS));
}

int
cirrus_register_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case REG:
    case SUBREG:
      break;
    default:
      return false;
    }
  return (
(cirrus_register_operand_1 (op, mode))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

static inline int
cirrus_fp_register_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);

  return (GET_CODE (op) == REG
	  && (REGNO (op) >= FIRST_PSEUDO_REGISTER
	      || REGNO_REG_CLASS (REGNO (op)) == CIRRUS_REGS));
}

int
cirrus_fp_register (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case REG:
    case SUBREG:
      break;
    default:
      return false;
    }
  return (
(cirrus_fp_register_1 (op, mode))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

int
cirrus_shift_const (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT) && (
#line 523 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(((unsigned HOST_WIDE_INT) INTVAL (op)) < 64));
}

static inline int
const_multiple_of_8_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 530 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  unsigned HOST_WIDE_INT val = INTVAL (op);
  return (val & 7) == 0;
}

int
const_multiple_of_8_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT) && (
(const_multiple_of_8_operand_1 (op, mode)));
}

static inline int
imm_for_neon_mov_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 537 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  return neon_immediate_valid_for_move (op, mode, NULL, NULL);
}

int
imm_for_neon_mov_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return ((GET_CODE (op) == CONST_VECTOR) && (
(imm_for_neon_mov_operand_1 (op, mode)))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

static inline int
imm_for_neon_logic_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 543 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  return (TARGET_NEON
          && neon_immediate_valid_for_logic (op, mode, 0, NULL, NULL));
}

int
imm_for_neon_logic_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return ((GET_CODE (op) == CONST_VECTOR) && (
(imm_for_neon_logic_operand_1 (op, mode)))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

static inline int
imm_for_neon_inv_logic_operand_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 550 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  return (TARGET_NEON
          && neon_immediate_valid_for_logic (op, mode, 1, NULL, NULL));
}

int
imm_for_neon_inv_logic_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return ((GET_CODE (op) == CONST_VECTOR) && (
(imm_for_neon_inv_logic_operand_1 (op, mode)))) && (
(mode == VOIDmode || GET_MODE (op) == mode));
}

int
neon_logic_op2 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (imm_for_neon_logic_operand (op, mode)) || (s_register_operand (op, mode));
}

int
neon_inv_logic_op2 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (imm_for_neon_inv_logic_operand (op, mode)) || (s_register_operand (op, mode));
}

int
neon_lane_number (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT) && (
#line 566 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(INTVAL (op) >= 0 && INTVAL (op) <= 7));
}

int
cmpdi_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (
#line 570 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(TARGET_HARD_FLOAT && TARGET_MAVERICK)) ? ((
#line 571 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(TARGET_ARM)) && (cirrus_fp_register (op, mode))) : ((
#line 573 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
(TARGET_32BIT)) && (arm_di_operand (op, mode)));
}

int
arm_sync_memory_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (memory_operand (op, mode)) && (GET_CODE (XEXP (op, 0)) == REG);
}

static inline int
vect_par_constant_high_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 584 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  HOST_WIDE_INT count = XVECLEN (op, 0);
  int i;
  int base = GET_MODE_NUNITS (mode);

  if ((count < 1)
      || (count != base/2))
    return false;
    
  if (!VECTOR_MODE_P (mode))
    return false;

  for (i = 0; i < count; i++)
   {
     rtx elt = XVECEXP (op, 0, i);
     int val;

     if (GET_CODE (elt) != CONST_INT)
       return false;

     val = INTVAL (elt);
     if (val != (base/2) + i)
       return false;
   }
  return true; 
}

int
vect_par_constant_high (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == PARALLEL) && (
(vect_par_constant_high_1 (op, mode)));
}

static inline int
vect_par_constant_low_1 (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
#line 613 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/predicates.md"
{
  HOST_WIDE_INT count = XVECLEN (op, 0);
  int i;
  int base = GET_MODE_NUNITS (mode);

  if ((count < 1)
      || (count != base/2))
    return false;
    
  if (!VECTOR_MODE_P (mode))
    return false;

  for (i = 0; i < count; i++)
   {
     rtx elt = XVECEXP (op, 0, i);
     int val;

     if (GET_CODE (elt) != CONST_INT)
       return false;

     val = INTVAL (elt);
     if (val != i)
       return false;
   } 
  return true; 
}

int
vect_par_constant_low (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == PARALLEL) && (
(vect_par_constant_low_1 (op, mode)));
}

enum constraint_num
lookup_constraint (const char *str)
{
  switch (str[0])
    {
    case 'D':
      if (!strncmp (str, "Dz", 2))
        return CONSTRAINT_Dz;
      if (!strncmp (str, "Da", 2))
        return CONSTRAINT_Da;
      if (!strncmp (str, "Db", 2))
        return CONSTRAINT_Db;
      if (!strncmp (str, "Dc", 2))
        return CONSTRAINT_Dc;
      if (!strncmp (str, "Di", 2))
        return CONSTRAINT_Di;
      if (!strncmp (str, "Dn", 2))
        return CONSTRAINT_Dn;
      if (!strncmp (str, "Dl", 2))
        return CONSTRAINT_Dl;
      if (!strncmp (str, "DL", 2))
        return CONSTRAINT_DL;
      if (!strncmp (str, "Dv", 2))
        return CONSTRAINT_Dv;
      if (!strncmp (str, "Dy", 2))
        return CONSTRAINT_Dy;
      if (!strncmp (str, "D0", 2))
        return CONSTRAINT_D0;
      break;
    case 'G':
      return CONSTRAINT_G;
    case 'H':
      return CONSTRAINT_H;
    case 'I':
      return CONSTRAINT_I;
    case 'J':
      return CONSTRAINT_J;
    case 'K':
      return CONSTRAINT_K;
    case 'L':
      return CONSTRAINT_L;
    case 'M':
      return CONSTRAINT_M;
    case 'N':
      return CONSTRAINT_N;
    case 'O':
      return CONSTRAINT_O;
    case 'P':
      if (!strncmp (str, "Pb", 2))
        return CONSTRAINT_Pb;
      if (!strncmp (str, "Ps", 2))
        return CONSTRAINT_Ps;
      if (!strncmp (str, "Pt", 2))
        return CONSTRAINT_Pt;
      if (!strncmp (str, "Pv", 2))
        return CONSTRAINT_Pv;
      if (!strncmp (str, "Pa", 2))
        return CONSTRAINT_Pa;
      break;
    case 'Q':
      return CONSTRAINT_Q;
    case 'U':
      if (!strncmp (str, "Uv", 2))
        return CONSTRAINT_Uv;
      if (!strncmp (str, "Uy", 2))
        return CONSTRAINT_Uy;
      if (!strncmp (str, "Un", 2))
        return CONSTRAINT_Un;
      if (!strncmp (str, "Um", 2))
        return CONSTRAINT_Um;
      if (!strncmp (str, "Us", 2))
        return CONSTRAINT_Us;
      if (!strncmp (str, "Uq", 2))
        return CONSTRAINT_Uq;
      if (!strncmp (str, "Ut", 2))
        return CONSTRAINT_Ut;
      break;
    case 'b':
      return CONSTRAINT_b;
    case 'c':
      return CONSTRAINT_c;
    case 'f':
      return CONSTRAINT_f;
    case 'h':
      return CONSTRAINT_h;
    case 'j':
      return CONSTRAINT_j;
    case 'k':
      return CONSTRAINT_k;
    case 'l':
      return CONSTRAINT_l;
    case 't':
      return CONSTRAINT_t;
    case 'v':
      return CONSTRAINT_v;
    case 'w':
      return CONSTRAINT_w;
    case 'x':
      return CONSTRAINT_x;
    case 'y':
      return CONSTRAINT_y;
    case 'z':
      return CONSTRAINT_z;
    default: break;
    }
  return CONSTRAINT__UNKNOWN;
}

enum reg_class
regclass_for_constraint (enum constraint_num c)
{
  switch (c)
    {
    case CONSTRAINT_f: return TARGET_ARM ? FPA_REGS : NO_REGS;
    case CONSTRAINT_t: return TARGET_32BIT ? VFP_LO_REGS : NO_REGS;
    case CONSTRAINT_v: return TARGET_ARM ? CIRRUS_REGS : NO_REGS;
    case CONSTRAINT_w: return TARGET_32BIT ? (TARGET_VFPD32 ? VFP_REGS : VFP_LO_REGS) : NO_REGS;
    case CONSTRAINT_x: return TARGET_32BIT ? VFP_D0_D7_REGS : NO_REGS;
    case CONSTRAINT_y: return TARGET_REALLY_IWMMXT ? IWMMXT_REGS : NO_REGS;
    case CONSTRAINT_z: return TARGET_REALLY_IWMMXT ? IWMMXT_GR_REGS : NO_REGS;
    case CONSTRAINT_l: return TARGET_THUMB ? LO_REGS : GENERAL_REGS;
    case CONSTRAINT_h: return TARGET_THUMB ? HI_REGS : NO_REGS;
    case CONSTRAINT_k: return STACK_REG;
    case CONSTRAINT_b: return TARGET_THUMB ? BASE_REGS : NO_REGS;
    case CONSTRAINT_c: return CC_REG;
    default: break;
    }
  return NO_REGS;
}

bool
constraint_satisfied_p (rtx op, enum constraint_num c)
{
  switch (c)
    {
    case CONSTRAINT_j: return satisfies_constraint_j (op);
    case CONSTRAINT_I: return satisfies_constraint_I (op);
    case CONSTRAINT_J: return satisfies_constraint_J (op);
    case CONSTRAINT_K: return satisfies_constraint_K (op);
    case CONSTRAINT_L: return satisfies_constraint_L (op);
    case CONSTRAINT_M: return satisfies_constraint_M (op);
    case CONSTRAINT_N: return satisfies_constraint_N (op);
    case CONSTRAINT_O: return satisfies_constraint_O (op);
    case CONSTRAINT_Pa: return satisfies_constraint_Pa (op);
    case CONSTRAINT_Pb: return satisfies_constraint_Pb (op);
    case CONSTRAINT_Ps: return satisfies_constraint_Ps (op);
    case CONSTRAINT_Pt: return satisfies_constraint_Pt (op);
    case CONSTRAINT_Pv: return satisfies_constraint_Pv (op);
    case CONSTRAINT_G: return satisfies_constraint_G (op);
    case CONSTRAINT_H: return satisfies_constraint_H (op);
    case CONSTRAINT_D0: return satisfies_constraint_D0 (op);
    case CONSTRAINT_Dz: return satisfies_constraint_Dz (op);
    case CONSTRAINT_Da: return satisfies_constraint_Da (op);
    case CONSTRAINT_Db: return satisfies_constraint_Db (op);
    case CONSTRAINT_Dc: return satisfies_constraint_Dc (op);
    case CONSTRAINT_Di: return satisfies_constraint_Di (op);
    case CONSTRAINT_Dn: return satisfies_constraint_Dn (op);
    case CONSTRAINT_Dl: return satisfies_constraint_Dl (op);
    case CONSTRAINT_DL: return satisfies_constraint_DL (op);
    case CONSTRAINT_Dv: return satisfies_constraint_Dv (op);
    case CONSTRAINT_Dy: return satisfies_constraint_Dy (op);
    case CONSTRAINT_Ut: return satisfies_constraint_Ut (op);
    case CONSTRAINT_Uv: return satisfies_constraint_Uv (op);
    case CONSTRAINT_Uy: return satisfies_constraint_Uy (op);
    case CONSTRAINT_Un: return satisfies_constraint_Un (op);
    case CONSTRAINT_Um: return satisfies_constraint_Um (op);
    case CONSTRAINT_Us: return satisfies_constraint_Us (op);
    case CONSTRAINT_Uq: return satisfies_constraint_Uq (op);
    case CONSTRAINT_Q: return satisfies_constraint_Q (op);
    default: break;
    }
  return false;
}

bool
insn_const_int_ok_for_constraint (HOST_WIDE_INT ival, enum constraint_num c)
{
  switch (c)
    {
    case CONSTRAINT_I:
      return 
#line 92 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT ? const_ok_for_arm (ival)
		   : ival >= 0 && ival <= 255);

    case CONSTRAINT_J:
      return 
#line 99 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT ? (ival >= -4095 && ival <= 4095)
		   : (ival >= -255 && ival <= -1));

    case CONSTRAINT_K:
      return 
#line 107 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT ? const_ok_for_arm (~ival)
		   : thumb_shiftable_const (ival));

    case CONSTRAINT_L:
      return 
#line 114 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT ? const_ok_for_arm (-ival)
		   : (ival >= -7 && ival <= 7));

    case CONSTRAINT_M:
      return 
#line 123 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT ? ((ival >= 0 && ival <= 32)
				 || ((ival & (ival - 1)) == 0))
		   : ((ival >= 0 && ival <= 1020) && ((ival & 3) == 0)));

    case CONSTRAINT_N:
      return 
#line 130 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(!TARGET_32BIT && (ival >= 0 && ival <= 31));

    case CONSTRAINT_O:
      return 
#line 136 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_THUMB1 && ival >= -508 && ival <= 508
		   && ((ival & 3) == 0));

    case CONSTRAINT_Pa:
      return 
#line 142 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_THUMB1 && ival >= -510 && ival <= 510
		    && (ival > 255 || ival < -255));

    case CONSTRAINT_Pb:
      return 
#line 148 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_THUMB1 && ival >= -262 && ival <= 262
		    && (ival > 255 || ival < -255));

    case CONSTRAINT_Ps:
      return 
#line 154 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_THUMB2 && ival >= -255 && ival <= 255);

    case CONSTRAINT_Pt:
      return 
#line 159 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_THUMB2 && ival >= -7 && ival <= 7);

    case CONSTRAINT_Pv:
      return 
#line 164 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_THUMB2 && ival >= -255 && ival <= 0);

    default: break;
    }
  return false;
}

bool
insn_extra_memory_constraint (enum constraint_num c)
{
  switch (c)
    {
    case CONSTRAINT_Ut:
      return true;

    case CONSTRAINT_Uv:
      return true;

    case CONSTRAINT_Uy:
      return true;

    case CONSTRAINT_Un:
      return true;

    case CONSTRAINT_Um:
      return true;

    case CONSTRAINT_Us:
      return true;

    case CONSTRAINT_Uq:
      return true;

    case CONSTRAINT_Q:
      return true;

    default: break;
    }
  return false;
}


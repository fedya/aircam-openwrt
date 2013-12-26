/* Generated automatically by the program 'build/genpreds'
   from the machine description file '/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md'.  */

#ifndef GCC_TM_CONSTRS_H
#define GCC_TM_CONSTRS_H

static inline bool
satisfies_constraint_j (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (
#line 72 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && arm_arch_thumb2)) && ((GET_CODE (op) == HIGH) || ((GET_CODE (op) == CONST_INT) && (
#line 75 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
((ival & 0xffff0000) == 0))));
}
static inline bool
satisfies_constraint_I (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 92 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT ? const_ok_for_arm (ival)
		   : ival >= 0 && ival <= 255));
}
static inline bool
satisfies_constraint_J (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 99 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT ? (ival >= -4095 && ival <= 4095)
		   : (ival >= -255 && ival <= -1)));
}
static inline bool
satisfies_constraint_K (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 107 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT ? const_ok_for_arm (~ival)
		   : thumb_shiftable_const (ival)));
}
static inline bool
satisfies_constraint_L (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 114 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT ? const_ok_for_arm (-ival)
		   : (ival >= -7 && ival <= 7)));
}
static inline bool
satisfies_constraint_M (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 123 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT ? ((ival >= 0 && ival <= 32)
				 || ((ival & (ival - 1)) == 0))
		   : ((ival >= 0 && ival <= 1020) && ((ival & 3) == 0))));
}
static inline bool
satisfies_constraint_N (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 130 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(!TARGET_32BIT && (ival >= 0 && ival <= 31)));
}
static inline bool
satisfies_constraint_O (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 136 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_THUMB1 && ival >= -508 && ival <= 508
		   && ((ival & 3) == 0)));
}
static inline bool
satisfies_constraint_Pa (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 142 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_THUMB1 && ival >= -510 && ival <= 510
		    && (ival > 255 || ival < -255)));
}
static inline bool
satisfies_constraint_Pb (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 148 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_THUMB1 && ival >= -262 && ival <= 262
		    && (ival > 255 || ival < -255)));
}
static inline bool
satisfies_constraint_Ps (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 154 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_THUMB2 && ival >= -255 && ival <= 255));
}
static inline bool
satisfies_constraint_Pt (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 159 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_THUMB2 && ival >= -7 && ival <= 7));
}
static inline bool
satisfies_constraint_Pv (rtx op)
{
  HOST_WIDE_INT ival = 0;
  if (CONST_INT_P (op))
    ival = INTVAL (op);
  return (GET_CODE (op) == CONST_INT) && (
#line 164 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_THUMB2 && ival >= -255 && ival <= 0));
}
static inline bool
satisfies_constraint_G (rtx op)
{
  return (GET_CODE (op) == CONST_DOUBLE) && (
#line 169 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && arm_const_double_rtx (op)));
}
static inline bool
satisfies_constraint_H (rtx op)
{
  return (GET_CODE (op) == CONST_DOUBLE) && (
#line 174 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && neg_const_double_rtx_ok_for_fpa (op)));
}
static inline bool
satisfies_constraint_D0 (rtx op)
{
  enum machine_mode mode = GET_MODE (op);
  return (GET_CODE (op) == CONST_DOUBLE) && (
#line 181 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_NEON && op == CONST0_RTX (mode)));
}
static inline bool
satisfies_constraint_Dz (rtx op)
{
  enum machine_mode mode = GET_MODE (op);
  return (GET_CODE (op) == CONST_VECTOR) && (
#line 187 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_NEON && op == CONST0_RTX (mode)));
}
static inline bool
satisfies_constraint_Da (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_DOUBLE:
    case CONST_INT:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return 
#line 194 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && arm_const_double_inline_cost (op) == 2);
}
static inline bool
satisfies_constraint_Db (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_DOUBLE:
    case CONST_INT:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return 
#line 201 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && arm_const_double_inline_cost (op) == 3);
}
static inline bool
satisfies_constraint_Dc (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_DOUBLE:
    case CONST_INT:
    case CONST_VECTOR:
      break;
    default:
      return false;
    }
  return 
#line 209 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && arm_const_double_inline_cost (op) == 4
		   && !(optimize_size || arm_ld_sched));
}
static inline bool
satisfies_constraint_Di (rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST_DOUBLE:
    case CONST_INT:
      break;
    default:
      return false;
    }
  return 
#line 217 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && arm_const_double_by_immediates (op));
}
static inline bool
satisfies_constraint_Dn (rtx op)
{
  return (GET_CODE (op) == CONST_VECTOR) && (
#line 224 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT
		   && imm_for_neon_mov_operand (op, GET_MODE (op))));
}
static inline bool
satisfies_constraint_Dl (rtx op)
{
  return (GET_CODE (op) == CONST_VECTOR) && (
#line 232 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT
		   && imm_for_neon_logic_operand (op, GET_MODE (op))));
}
static inline bool
satisfies_constraint_DL (rtx op)
{
  return (GET_CODE (op) == CONST_VECTOR) && (
#line 240 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT
		   && imm_for_neon_inv_logic_operand (op, GET_MODE (op))));
}
static inline bool
satisfies_constraint_Dv (rtx op)
{
  return (GET_CODE (op) == CONST_DOUBLE) && (
#line 248 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && vfp3_const_double_rtx (op)));
}
static inline bool
satisfies_constraint_Dy (rtx op)
{
  return (GET_CODE (op) == CONST_DOUBLE) && (
#line 255 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && TARGET_VFP_DOUBLE && vfp3_const_double_rtx (op)));
}
static inline bool
satisfies_constraint_Ut (rtx op)
{
  return (GET_CODE (op) == MEM) && (
#line 262 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && neon_struct_mem_operand (op)));
}
static inline bool
satisfies_constraint_Uv (rtx op)
{
  return (GET_CODE (op) == MEM) && (
#line 268 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && arm_coproc_mem_operand (op, FALSE)));
}
static inline bool
satisfies_constraint_Uy (rtx op)
{
  return (GET_CODE (op) == MEM) && (
#line 274 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && arm_coproc_mem_operand (op, TRUE)));
}
static inline bool
satisfies_constraint_Un (rtx op)
{
  return (GET_CODE (op) == MEM) && (
#line 281 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && neon_vector_mem_operand (op, 0)));
}
static inline bool
satisfies_constraint_Um (rtx op)
{
  return (GET_CODE (op) == MEM) && (
#line 288 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && neon_vector_mem_operand (op, 2)));
}
static inline bool
satisfies_constraint_Us (rtx op)
{
  return (GET_CODE (op) == MEM) && (
#line 295 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_32BIT && neon_vector_mem_operand (op, 1)));
}
static inline bool
satisfies_constraint_Uq (rtx op)
{
  return (GET_CODE (op) == MEM) && (
#line 301 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(TARGET_ARM
		   && arm_legitimate_address_outer_p (GET_MODE (op), XEXP (op, 0),
						      SIGN_EXTEND, 0)));
}
static inline bool
satisfies_constraint_Q (rtx op)
{
  return (GET_CODE (op) == MEM) && (
#line 309 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/constraints.md"
(REG_P (XEXP (op, 0))));
}
#endif /* tm-constrs.h */

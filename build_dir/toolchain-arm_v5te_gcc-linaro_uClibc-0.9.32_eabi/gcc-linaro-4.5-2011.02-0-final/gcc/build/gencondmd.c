/* Generated automatically by the program `genconditions' from the target
   machine description file.  */

#include "bconfig.h"
#include "system.h"

/* It is necessary, but not entirely safe, to include the headers below
   in a generator program.  As a defensive measure, don't do so when the
   table isn't going to have anything in it.  */
#if GCC_VERSION >= 3001

/* Do not allow checking to confuse the issue.  */
#undef ENABLE_CHECKING
#undef ENABLE_TREE_CHECKING
#undef ENABLE_RTL_CHECKING
#undef ENABLE_RTL_FLAG_CHECKING
#undef ENABLE_GC_CHECKING
#undef ENABLE_GC_ALWAYS_COLLECT

#include "coretypes.h"
#include "tm.h"
#include "insn-constants.h"
#include "rtl.h"
#include "tm_p.h"
#include "function.h"

/* Fake - insn-config.h doesn't exist yet.  */
#define MAX_RECOG_OPERANDS 10
#define MAX_DUP_OPERANDS 10
#define MAX_INSNS_PER_SPLIT 5

#include "regs.h"
#include "recog.h"
#include "real.h"
#include "output.h"
#include "flags.h"
#include "hard-reg-set.h"
#include "resource.h"
#include "toplev.h"
#include "reload.h"
#include "tm-constrs.h"

#define HAVE_eh_return 1
#include "except.h"

/* Dummy external declarations.  */
extern rtx insn;
extern rtx ins1;
extern rtx operands[];

#endif /* gcc >= 3.0.1 */

/* Structure definition duplicated from gensupport.h rather than
   drag in that file and its dependencies.  */
struct c_test
{
  const char *expr;
  int value;
};

/* This table lists each condition found in the machine description.
   Each condition is mapped to its truth value (0 or 1), or -1 if that
   cannot be calculated at compile time.
   If we don't have __builtin_constant_p, or it's not acceptable in array
   initializers, fall back to assuming that all conditions potentially
   vary at run time.  It works in 3.0.1 and later; 3.0 only when not
   optimizing.  */

#if GCC_VERSION >= 3001
static const struct c_test insn_conditions[] = {

  { "(TARGET_32BIT) && (TARGET_32BIT\n\
   && (   register_operand (operands[0], QImode)\n\
       || register_operand (operands[1], QImode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 5937 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 5937 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode))))
    : -1 },
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V2SFmode != V2SFmode && V2SFmode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SFmode))",
    __builtin_constant_p 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V2SFmode != V2SFmode && V2SFmode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SFmode)))
    ? (int) 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V2SFmode != V2SFmode && V2SFmode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SFmode)))
    : -1 },
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V4HImode != V2SFmode && V4HImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (V4HImode == V4HImode && TARGET_REALLY_IWMMXT)",
    __builtin_constant_p 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V4HImode != V2SFmode && V4HImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V4HImode == V4HImode && TARGET_REALLY_IWMMXT))
    ? (int) 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V4HImode != V2SFmode && V4HImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V4HImode == V4HImode && TARGET_REALLY_IWMMXT))
    : -1 },
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "TARGET_NEON\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SImode))",
    __builtin_constant_p 
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SImode)))
    ? (int) 
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SImode)))
    : -1 },
#line 964 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FP16",
    __builtin_constant_p 
#line 964 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FP16)
    ? (int) 
#line 964 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FP16)
    : -1 },
#line 875 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
  { "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_FPA",
    __builtin_constant_p 
#line 875 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_FPA)
    ? (int) 
#line 875 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_FPA)
    : -1 },
#line 409 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP\n\
   && (   s_register_operand (operands[0], SFmode)\n\
       || s_register_operand (operands[1], SFmode))",
    __builtin_constant_p 
#line 409 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP
   && (   s_register_operand (operands[0], SFmode)
       || s_register_operand (operands[1], SFmode)))
    ? (int) 
#line 409 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP
   && (   s_register_operand (operands[0], SFmode)
       || s_register_operand (operands[1], SFmode)))
    : -1 },
#line 4328 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && (GET_CODE (operands[1]) != MEM) && BYTES_BIG_ENDIAN",
    __builtin_constant_p 
#line 4328 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && (GET_CODE (operands[1]) != MEM) && BYTES_BIG_ENDIAN)
    ? (int) 
#line 4328 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && (GET_CODE (operands[1]) != MEM) && BYTES_BIG_ENDIAN)
    : -1 },
#line 6560 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && XVECLEN (operands[0], 0) == 2",
    __builtin_constant_p 
#line 6560 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 2)
    ? (int) 
#line 6560 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 2)
    : -1 },
#line 3613 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1 || arm_arch_thumb2",
    __builtin_constant_p 
#line 3613 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 || arm_arch_thumb2)
    ? (int) 
#line 3613 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 || arm_arch_thumb2)
    : -1 },
#line 6548 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && XVECLEN (operands[0], 0) == 3",
    __builtin_constant_p 
#line 6548 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 3)
    ? (int) 
#line 6548 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 3)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP\n\
   && (   s_register_operand (operands[0], SFmode)\n\
       || s_register_operand (operands[1], SFmode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 409 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP
   && (   s_register_operand (operands[0], SFmode)
       || s_register_operand (operands[1], SFmode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 409 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP
   && (   s_register_operand (operands[0], SFmode)
       || s_register_operand (operands[1], SFmode))))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_THUMB2 && reload_completed\n\
   && GET_CODE(operands[3]) != PLUS\n\
   && GET_CODE(operands[3]) != MINUS)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1055 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && reload_completed
   && GET_CODE(operands[3]) != PLUS
   && GET_CODE(operands[3]) != MINUS))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1055 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && reload_completed
   && GET_CODE(operands[3]) != PLUS
   && GET_CODE(operands[3]) != MINUS))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_THUMB2 && reload_completed\n\
   && ((GET_CODE(operands[3]) != ROTATE && GET_CODE(operands[3]) != ROTATERT)\n\
       || REG_P(operands[2])))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1089 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && reload_completed
   && ((GET_CODE(operands[3]) != ROTATE && GET_CODE(operands[3]) != ROTATERT)
       || REG_P(operands[2]))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1089 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && reload_completed
   && ((GET_CODE(operands[3]) != ROTATE && GET_CODE(operands[3]) != ROTATERT)
       || REG_P(operands[2]))))
    : -1 },
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V8HImode != V2SFmode && V8HImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (V8HImode == V4HImode && TARGET_REALLY_IWMMXT)",
    __builtin_constant_p 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V8HImode != V2SFmode && V8HImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V8HImode == V4HImode && TARGET_REALLY_IWMMXT))
    ? (int) 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V8HImode != V2SFmode && V8HImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V8HImode == V4HImode && TARGET_REALLY_IWMMXT))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && arm_arch5)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 11318 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch5))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 11318 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch5))
    : -1 },
#line 463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], V2SFmode)\n\
       || register_operand (operands[1], V2SFmode))",
    __builtin_constant_p 
#line 463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V2SFmode)
       || register_operand (operands[1], V2SFmode)))
    ? (int) 
#line 463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V2SFmode)
       || register_operand (operands[1], V2SFmode)))
    : -1 },
#line 152 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/iwmmxt.md"
  { "TARGET_REALLY_IWMMXT\n\
   && (   register_operand (operands[0], SImode)\n\
       || register_operand (operands[1], SImode))",
    __builtin_constant_p 
#line 152 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/iwmmxt.md"
(TARGET_REALLY_IWMMXT
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode)))
    ? (int) 
#line 152 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/iwmmxt.md"
(TARGET_REALLY_IWMMXT
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode)))
    : -1 },
#line 1336 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/iwmmxt.md"
  { "TARGET_REALLY_IWMMXT",
    __builtin_constant_p 
#line 1336 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/iwmmxt.md"
(TARGET_REALLY_IWMMXT)
    ? (int) 
#line 1336 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/iwmmxt.md"
(TARGET_REALLY_IWMMXT)
    : -1 },
#line 1837 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON && (!true || flag_unsafe_math_optimizations)",
    __builtin_constant_p 
#line 1837 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON && (!true || flag_unsafe_math_optimizations))
    ? (int) 
#line 1837 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON && (!true || flag_unsafe_math_optimizations))
    : -1 },
#line 2147 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
  && (INTVAL (operands[2]) >= 0 && INTVAL (operands[2]) < 32\n\
      && INTVAL (operands[1]) > 0 \n\
      && INTVAL (operands[1]) + (INTVAL (operands[2]) & 1) <= 8\n\
      && INTVAL (operands[1]) + INTVAL (operands[2]) <= 32)",
    __builtin_constant_p 
#line 2147 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
  && (INTVAL (operands[2]) >= 0 && INTVAL (operands[2]) < 32
      && INTVAL (operands[1]) > 0 
      && INTVAL (operands[1]) + (INTVAL (operands[2]) & 1) <= 8
      && INTVAL (operands[1]) + INTVAL (operands[2]) <= 32))
    ? (int) 
#line 2147 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
  && (INTVAL (operands[2]) >= 0 && INTVAL (operands[2]) < 32
      && INTVAL (operands[1]) > 0 
      && INTVAL (operands[1]) + (INTVAL (operands[2]) & 1) <= 8
      && INTVAL (operands[1]) + INTVAL (operands[2]) <= 32))
    : -1 },
#line 234 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && ! TARGET_IWMMXT\n\
   && !(TARGET_HARD_FLOAT && TARGET_VFP)\n\
   && (   register_operand (operands[0], SImode)\n\
       || register_operand (operands[1], SImode))",
    __builtin_constant_p 
#line 234 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode)))
    ? (int) 
#line 234 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode)))
    : -1 },
#line 1181 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && arm_arch_hwdiv",
    __builtin_constant_p 
#line 1181 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && arm_arch_hwdiv)
    ? (int) 
#line 1181 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && arm_arch_hwdiv)
    : -1 },
#line 6485 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1 && XVECLEN (operands[0], 0) == 5",
    __builtin_constant_p 
#line 6485 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && XVECLEN (operands[0], 0) == 5)
    ? (int) 
#line 6485 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && XVECLEN (operands[0], 0) == 5)
    : -1 },
#line 536 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_MAVERICK) && !TARGET_NEON",
    __builtin_constant_p 
#line 536 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_MAVERICK) && !TARGET_NEON)
    ? (int) 
#line 536 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_MAVERICK) && !TARGET_NEON)
    : -1 },
#line 11445 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "arm_arch_thumb2",
    __builtin_constant_p 
#line 11445 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(arm_arch_thumb2)
    ? (int) 
#line 11445 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(arm_arch_thumb2)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_THUMB2 && arm_arch6)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 976 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && arm_arch6))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 976 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && arm_arch6))
    : -1 },
#line 8888 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && USE_RETURN_INSN (FALSE)",
    __builtin_constant_p 
#line 8888 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && USE_RETURN_INSN (FALSE))
    ? (int) 
#line 8888 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && USE_RETURN_INSN (FALSE))
    : -1 },
#line 8894 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && USE_RETURN_INSN (FALSE)",
    __builtin_constant_p 
#line 8894 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && USE_RETURN_INSN (FALSE))
    ? (int) 
#line 8894 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && USE_RETURN_INSN (FALSE))
    : -1 },
#line 5187 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1 && satisfies_constraint_J (operands[1])",
    __builtin_constant_p 
#line 5187 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && satisfies_constraint_J (operands[1]))
    ? (int) 
#line 5187 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && satisfies_constraint_J (operands[1]))
    : -1 },
#line 1161 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
   && !const_ok_for_arm (INTVAL (operands[1]))\n\
   && const_ok_for_arm (~INTVAL (operands[1]))",
    __builtin_constant_p 
#line 1161 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && !const_ok_for_arm (INTVAL (operands[1]))
   && const_ok_for_arm (~INTVAL (operands[1])))
    ? (int) 
#line 1161 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && !const_ok_for_arm (INTVAL (operands[1]))
   && const_ok_for_arm (~INTVAL (operands[1])))
    : -1 },
#line 656 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON && reload_completed",
    __builtin_constant_p 
#line 656 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON && reload_completed)
    ? (int) 
#line 656 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON && reload_completed)
    : -1 },
#line 5992 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_FP16)\n\
   && (	  s_register_operand (operands[0], HFmode)\n\
       || s_register_operand (operands[1], HFmode))",
    __builtin_constant_p 
#line 5992 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_FP16)
   && (	  s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode)))
    ? (int) 
#line 5992 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_FP16)
   && (	  s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode)))
    : -1 },
#line 8780 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
   && (GET_CODE (operands[0]) == SYMBOL_REF)\n\
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[0]))",
    __builtin_constant_p 
#line 8780 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && (GET_CODE (operands[0]) == SYMBOL_REF)
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[0])))
    ? (int) 
#line 8780 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && (GET_CODE (operands[0]) == SYMBOL_REF)
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[0])))
    : -1 },
#line 9115 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT || optimize_size || flag_pic",
    __builtin_constant_p 
#line 9115 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT || optimize_size || flag_pic)
    ? (int) 
#line 9115 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT || optimize_size || flag_pic)
    : -1 },
#line 4665 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1 && reload_completed",
    __builtin_constant_p 
#line 4665 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && reload_completed)
    ? (int) 
#line 4665 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && reload_completed)
    : -1 },
#line 2175 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32\n\
       && INTVAL (operands[2]) > 0 \n\
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8\n\
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)",
    __builtin_constant_p 
#line 2175 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32))
    ? (int) 
#line 2175 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32))
    : -1 },
#line 1130 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2\n\
   && peep2_regno_dead_p(0, CC_REGNUM)\n\
   && ((rtx_equal_p(operands[0], operands[1])\n\
	&& INTVAL(operands[2]) > -256 && INTVAL(operands[2]) < 256)\n\
       || (INTVAL(operands[2]) > -8 && INTVAL(operands[2]) < 8))",
    __builtin_constant_p 
#line 1130 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && peep2_regno_dead_p(0, CC_REGNUM)
   && ((rtx_equal_p(operands[0], operands[1])
	&& INTVAL(operands[2]) > -256 && INTVAL(operands[2]) < 256)
       || (INTVAL(operands[2]) > -8 && INTVAL(operands[2]) < 8)))
    ? (int) 
#line 1130 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && peep2_regno_dead_p(0, CC_REGNUM)
   && ((rtx_equal_p(operands[0], operands[1])
	&& INTVAL(operands[2]) > -256 && INTVAL(operands[2]) < 256)
       || (INTVAL(operands[2]) > -8 && INTVAL(operands[2]) < 8)))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM\n\
   && arm_arch4\n\
   && (GET_CODE (operands[1]) != CONST_INT\n\
       || const_ok_for_arm (INTVAL (operands[1]))\n\
       || const_ok_for_arm (~INTVAL (operands[1]))))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 5787 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && arm_arch4
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1])))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 5787 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && arm_arch4
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1])))))
    : -1 },
#line 95 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_THUMB2 && TARGET_VFP && TARGET_HARD_FLOAT\n\
   && (   s_register_operand (operands[0], SImode)\n\
       || s_register_operand (operands[1], SImode))",
    __builtin_constant_p 
#line 95 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_VFP && TARGET_HARD_FLOAT
   && (   s_register_operand (operands[0], SImode)
       || s_register_operand (operands[1], SImode)))
    ? (int) 
#line 95 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_VFP && TARGET_HARD_FLOAT
   && (   s_register_operand (operands[0], SImode)
       || s_register_operand (operands[1], SImode)))
    : -1 },
#line 5338 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && peep2_reg_dead_p (2, operands[0])",
    __builtin_constant_p 
#line 5338 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && peep2_reg_dead_p (2, operands[0]))
    ? (int) 
#line 5338 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && peep2_reg_dead_p (2, operands[0]))
    : -1 },
#line 4718 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && GET_CODE (XEXP (operands[4], 0)) == PLUS\n\
   && rtx_equal_p (operands[0], XEXP (XEXP (operands[4], 0), 0))\n\
   && rtx_equal_p (operands[2], XEXP (XEXP (operands[4], 0), 1))\n\
   && (peep2_reg_dead_p (3, operands[0])\n\
       || rtx_equal_p (operands[0], operands[3]))\n\
   && (peep2_reg_dead_p (3, operands[2])\n\
       || rtx_equal_p (operands[2], operands[3]))",
    __builtin_constant_p 
#line 4718 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && GET_CODE (XEXP (operands[4], 0)) == PLUS
   && rtx_equal_p (operands[0], XEXP (XEXP (operands[4], 0), 0))
   && rtx_equal_p (operands[2], XEXP (XEXP (operands[4], 0), 1))
   && (peep2_reg_dead_p (3, operands[0])
       || rtx_equal_p (operands[0], operands[3]))
   && (peep2_reg_dead_p (3, operands[2])
       || rtx_equal_p (operands[2], operands[3])))
    ? (int) 
#line 4718 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && GET_CODE (XEXP (operands[4], 0)) == PLUS
   && rtx_equal_p (operands[0], XEXP (XEXP (operands[4], 0), 0))
   && rtx_equal_p (operands[2], XEXP (XEXP (operands[4], 0), 1))
   && (peep2_reg_dead_p (3, operands[0])
       || rtx_equal_p (operands[0], operands[3]))
   && (peep2_reg_dead_p (3, operands[2])
       || rtx_equal_p (operands[2], operands[3])))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_THUMB2\n\
   && TARGET_HARD_FLOAT && TARGET_FPA\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], SFmode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 620 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_THUMB2
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 620 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_THUMB2
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && arm_arch3m && !arm_arch6)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1720 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch3m && !arm_arch6))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1720 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch3m && !arm_arch6))
    : -1 },
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V2SFmode != V2SFmode && V2SFmode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (V2SFmode == V4HImode && TARGET_REALLY_IWMMXT)",
    __builtin_constant_p 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V2SFmode != V2SFmode && V2SFmode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V2SFmode == V4HImode && TARGET_REALLY_IWMMXT))
    ? (int) 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V2SFmode != V2SFmode && V2SFmode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V2SFmode == V4HImode && TARGET_REALLY_IWMMXT))
    : -1 },
#line 5200 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1 && satisfies_constraint_K (operands[1])",
    __builtin_constant_p 
#line 5200 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && satisfies_constraint_K (operands[1]))
    ? (int) 
#line 5200 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && satisfies_constraint_K (operands[1]))
    : -1 },
#line 6575 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_EITHER && !low_irq_latency",
    __builtin_constant_p 
#line 6575 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_EITHER && !low_irq_latency)
    ? (int) 
#line 6575 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_EITHER && !low_irq_latency)
    : -1 },
#line 638 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT &&\n\
   !(const_ok_for_arm (INTVAL (operands[2]))\n\
     || const_ok_for_arm (-INTVAL (operands[2])))\n\
    && const_ok_for_arm (~INTVAL (operands[2]))",
    __builtin_constant_p 
#line 638 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT &&
   !(const_ok_for_arm (INTVAL (operands[2]))
     || const_ok_for_arm (-INTVAL (operands[2])))
    && const_ok_for_arm (~INTVAL (operands[2])))
    ? (int) 
#line 638 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT &&
   !(const_ok_for_arm (INTVAL (operands[2]))
     || const_ok_for_arm (-INTVAL (operands[2])))
    && const_ok_for_arm (~INTVAL (operands[2])))
    : -1 },
#line 5153 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
   && TARGET_USE_MOVT && GET_CODE (operands[1]) == SYMBOL_REF\n\
   && !flag_pic && !target_word_relocations\n\
   && !arm_tls_referenced_p (operands[1])",
    __builtin_constant_p 
#line 5153 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && TARGET_USE_MOVT && GET_CODE (operands[1]) == SYMBOL_REF
   && !flag_pic && !target_word_relocations
   && !arm_tls_referenced_p (operands[1]))
    ? (int) 
#line 5153 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && TARGET_USE_MOVT && GET_CODE (operands[1]) == SYMBOL_REF
   && !flag_pic && !target_word_relocations
   && !arm_tls_referenced_p (operands[1]))
    : -1 },
#line 55 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_ARM && TARGET_VFP && TARGET_HARD_FLOAT\n\
   && (   s_register_operand (operands[0], SImode)\n\
       || s_register_operand (operands[1], SImode))",
    __builtin_constant_p 
#line 55 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_VFP && TARGET_HARD_FLOAT
   && (   s_register_operand (operands[0], SImode)
       || s_register_operand (operands[1], SImode)))
    ? (int) 
#line 55 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_VFP && TARGET_HARD_FLOAT
   && (   s_register_operand (operands[0], SImode)
       || s_register_operand (operands[1], SImode)))
    : -1 },
#line 1399 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && reload_completed",
    __builtin_constant_p 
#line 1399 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && reload_completed)
    ? (int) 
#line 1399 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && reload_completed)
    : -1 },
#line 8132 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT || TARGET_THUMB1",
    __builtin_constant_p 
#line 8132 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT || TARGET_THUMB1)
    ? (int) 
#line 8132 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT || TARGET_THUMB1)
    : -1 },
#line 684 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE && reload_completed\n\
   && arm_general_register_operand (operands[0], DFmode)",
    __builtin_constant_p 
#line 684 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE && reload_completed
   && arm_general_register_operand (operands[0], DFmode))
    ? (int) 
#line 684 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE && reload_completed
   && arm_general_register_operand (operands[0], DFmode))
    : -1 },
#line 1015 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && flag_pic",
    __builtin_constant_p 
#line 1015 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && flag_pic)
    ? (int) 
#line 1015 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && flag_pic)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_THUMB2 && ! TARGET_IWMMXT\n\
   && !(TARGET_HARD_FLOAT && TARGET_VFP)\n\
   && (   register_operand (operands[0], SImode)\n\
       || register_operand (operands[1], SImode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 234 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 234 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode))))
    : -1 },
#line 7768 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && (GET_CODE (operands[4]) == EQ\n\
       || GET_CODE (operands[4]) == NE\n\
       || GET_CODE (operands[4]) == GE\n\
       || GET_CODE (operands[4]) == LT)",
    __builtin_constant_p 
#line 7768 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (GET_CODE (operands[4]) == EQ
       || GET_CODE (operands[4]) == NE
       || GET_CODE (operands[4]) == GE
       || GET_CODE (operands[4]) == LT))
    ? (int) 
#line 7768 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (GET_CODE (operands[4]) == EQ
       || GET_CODE (operands[4]) == NE
       || GET_CODE (operands[4]) == GE
       || GET_CODE (operands[4]) == LT))
    : -1 },
#line 3065 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
   && GET_CODE (operands[1]) == GET_CODE (operands[9])\n\
   && INTVAL (operands[3]) == 32 - INTVAL (operands[6])",
    __builtin_constant_p 
#line 3065 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && GET_CODE (operands[1]) == GET_CODE (operands[9])
   && INTVAL (operands[3]) == 32 - INTVAL (operands[6]))
    ? (int) 
#line 3065 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && GET_CODE (operands[1]) == GET_CODE (operands[9])
   && INTVAL (operands[3]) == 32 - INTVAL (operands[6]))
    : -1 },
#line 831 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
  { "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_FPA",
    __builtin_constant_p 
#line 831 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_FPA)
    ? (int) 
#line 831 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_FPA)
    : -1 },
#line 515 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
  { "TARGET_THUMB2\n\
   && TARGET_HARD_FLOAT && TARGET_MAVERICK\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], DFmode))",
    __builtin_constant_p 
#line 515 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_THUMB2
   && TARGET_HARD_FLOAT && TARGET_MAVERICK
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode)))
    ? (int) 
#line 515 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_THUMB2
   && TARGET_HARD_FLOAT && TARGET_MAVERICK
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode)))
    : -1 },
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V16QImode != V2SFmode && V16QImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V16QImode))",
    __builtin_constant_p 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V16QImode != V2SFmode && V16QImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V16QImode)))
    ? (int) 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V16QImode != V2SFmode && V16QImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V16QImode)))
    : -1 },
#line 11318 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && arm_arch5",
    __builtin_constant_p 
#line 11318 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch5)
    ? (int) 
#line 11318 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch5)
    : -1 },
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "TARGET_NEON\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8HImode))",
    __builtin_constant_p 
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8HImode)))
    ? (int) 
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8HImode)))
    : -1 },
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP",
    __builtin_constant_p 
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP)
    ? (int) 
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP)
    : -1 },
#line 1495 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && arm_arch6 && optimize_size",
    __builtin_constant_p 
#line 1495 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch6 && optimize_size)
    ? (int) 
#line 1495 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch6 && optimize_size)
    : -1 },
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], V2DImode)\n\
       || register_operand (operands[1], V2DImode))",
    __builtin_constant_p 
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V2DImode)
       || register_operand (operands[1], V2DImode)))
    ? (int) 
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V2DImode)
       || register_operand (operands[1], V2DImode)))
    : -1 },
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], TImode)\n\
       || register_operand (operands[1], TImode))",
    __builtin_constant_p 
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], TImode)
       || register_operand (operands[1], TImode)))
    ? (int) 
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], TImode)
       || register_operand (operands[1], TImode)))
    : -1 },
#line 3255 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && !arm_eliminable_register (operands[1])",
    __builtin_constant_p 
#line 3255 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !arm_eliminable_register (operands[1]))
    ? (int) 
#line 3255 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !arm_eliminable_register (operands[1]))
    : -1 },
#line 538 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && reload_completed\n\
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))",
    __builtin_constant_p 
#line 538 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0]))))
    ? (int) 
#line 538 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0]))))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM && !arm_arch6)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 4279 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && !arm_arch6))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 4279 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && !arm_arch6))
    : -1 },
#line 1814 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_DSP_MULTIPLY",
    __builtin_constant_p 
#line 1814 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_DSP_MULTIPLY)
    ? (int) 
#line 1814 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_DSP_MULTIPLY)
    : -1 },
#line 1129 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE",
    __builtin_constant_p 
#line 1129 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE)
    ? (int) 
#line 1129 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE)
    : -1 },
#line 5952 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && (   register_operand (operands[0], QImode)\n\
       || register_operand (operands[1], QImode))",
    __builtin_constant_p 
#line 5952 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode)))
    ? (int) 
#line 5952 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode)))
    : -1 },
#line 206 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2\n\
  && !(TARGET_HARD_FLOAT && (TARGET_MAVERICK || TARGET_VFP))\n\
  && !TARGET_IWMMXT",
    __builtin_constant_p 
#line 206 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
  && !(TARGET_HARD_FLOAT && (TARGET_MAVERICK || TARGET_VFP))
  && !TARGET_IWMMXT)
    ? (int) 
#line 206 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
  && !(TARGET_HARD_FLOAT && (TARGET_MAVERICK || TARGET_VFP))
  && !TARGET_IWMMXT)
    : -1 },
#line 1025 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && USE_RETURN_INSN (FALSE)",
    __builtin_constant_p 
#line 1025 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && USE_RETURN_INSN (FALSE))
    ? (int) 
#line 1025 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && USE_RETURN_INSN (FALSE))
    : -1 },
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "TARGET_NEON\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SImode))",
    __builtin_constant_p 
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SImode)))
    ? (int) 
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SImode)))
    : -1 },
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], CImode)\n\
       || register_operand (operands[1], CImode))",
    __builtin_constant_p 
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], CImode)
       || register_operand (operands[1], CImode)))
    ? (int) 
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], CImode)
       || register_operand (operands[1], CImode)))
    : -1 },
#line 2847 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON",
    __builtin_constant_p 
#line 2847 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON)
    ? (int) 
#line 2847 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON)
    : -1 },
#line 8731 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && !arm_arch5 && (!CONSTANT_ADDRESS_P (XEXP (operands[1], 0)))",
    __builtin_constant_p 
#line 8731 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && !arm_arch5 && (!CONSTANT_ADDRESS_P (XEXP (operands[1], 0))))
    ? (int) 
#line 8731 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && !arm_arch5 && (!CONSTANT_ADDRESS_P (XEXP (operands[1], 0))))
    : -1 },
#line 594 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
  { "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_FPA\n\
   && (register_operand (operands[0], XFmode)\n\
       || register_operand (operands[1], XFmode))",
    __builtin_constant_p 
#line 594 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_FPA
   && (register_operand (operands[0], XFmode)
       || register_operand (operands[1], XFmode)))
    ? (int) 
#line 594 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_FPA
   && (register_operand (operands[0], XFmode)
       || register_operand (operands[1], XFmode)))
    : -1 },
#line 10706 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && load_multiple_sequence (operands, 2, NULL, NULL, NULL)",
    __builtin_constant_p 
#line 10706 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && load_multiple_sequence (operands, 2, NULL, NULL, NULL))
    ? (int) 
#line 10706 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && load_multiple_sequence (operands, 2, NULL, NULL, NULL))
    : -1 },
#line 11435 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_SOFT_TP",
    __builtin_constant_p 
#line 11435 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_SOFT_TP)
    ? (int) 
#line 11435 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_SOFT_TP)
    : -1 },
#line 372 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP\n\
   && (   s_register_operand (operands[0], SFmode)\n\
       || s_register_operand (operands[1], SFmode))",
    __builtin_constant_p 
#line 372 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP
   && (   s_register_operand (operands[0], SFmode)
       || s_register_operand (operands[1], SFmode)))
    ? (int) 
#line 372 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP
   && (   s_register_operand (operands[0], SFmode)
       || s_register_operand (operands[1], SFmode)))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM && arm_arch6)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 4641 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch6))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 4641 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch6))
    : -1 },
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], EImode)\n\
       || register_operand (operands[1], EImode))",
    __builtin_constant_p 
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], EImode)
       || register_operand (operands[1], EImode)))
    ? (int) 
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], EImode)
       || register_operand (operands[1], EImode)))
    : -1 },
#line 463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], V2SImode)\n\
       || register_operand (operands[1], V2SImode))",
    __builtin_constant_p 
#line 463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V2SImode)
       || register_operand (operands[1], V2SImode)))
    ? (int) 
#line 463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V2SImode)
       || register_operand (operands[1], V2SImode)))
    : -1 },
#line 4120 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "!TARGET_THUMB2 && !arm_arch6",
    __builtin_constant_p 
#line 4120 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(!TARGET_THUMB2 && !arm_arch6)
    ? (int) 
#line 4120 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(!TARGET_THUMB2 && !arm_arch6)
    : -1 },
#line 1386 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2\n\
   && peep2_regno_dead_p(0, CC_REGNUM)",
    __builtin_constant_p 
#line 1386 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && peep2_regno_dead_p(0, CC_REGNUM))
    ? (int) 
#line 1386 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && peep2_regno_dead_p(0, CC_REGNUM))
    : -1 },
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V8HImode != V2SFmode && V8HImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8HImode))",
    __builtin_constant_p 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V8HImode != V2SFmode && V8HImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8HImode)))
    ? (int) 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V8HImode != V2SFmode && V8HImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8HImode)))
    : -1 },
#line 5139 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
  && (!(const_ok_for_arm (INTVAL (operands[1]))\n\
        || const_ok_for_arm (~INTVAL (operands[1]))))",
    __builtin_constant_p 
#line 5139 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
  && (!(const_ok_for_arm (INTVAL (operands[1]))
        || const_ok_for_arm (~INTVAL (operands[1])))))
    ? (int) 
#line 5139 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
  && (!(const_ok_for_arm (INTVAL (operands[1]))
        || const_ok_for_arm (~INTVAL (operands[1])))))
    : -1 },
#line 558 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
  { "TARGET_ARM\n\
   && TARGET_HARD_FLOAT && TARGET_FPA\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], DFmode))",
    __builtin_constant_p 
#line 558 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode)))
    ? (int) 
#line 558 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode)))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP\n\
   && (   register_operand (operands[0], DFmode)\n\
       || register_operand (operands[1], DFmode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 449 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 449 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode))))
    : -1 },
#line 813 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && peep2_reg_dead_p (3, operands[2])",
    __builtin_constant_p 
#line 813 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && peep2_reg_dead_p (3, operands[2]))
    ? (int) 
#line 813 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && peep2_reg_dead_p (3, operands[2]))
    : -1 },
#line 2777 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM\n\
   && GET_CODE (operands[2]) == CONST_INT\n\
   && !const_ok_for_arm (INTVAL (operands[2]))",
    __builtin_constant_p 
#line 2777 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && GET_CODE (operands[2]) == CONST_INT
   && !const_ok_for_arm (INTVAL (operands[2])))
    ? (int) 
#line 2777 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && GET_CODE (operands[2]) == CONST_INT
   && !const_ok_for_arm (INTVAL (operands[2])))
    : -1 },
#line 8757 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1 && !arm_arch5",
    __builtin_constant_p 
#line 8757 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && !arm_arch5)
    ? (int) 
#line 8757 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && !arm_arch5)
    : -1 },
#line 10721 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && store_multiple_sequence (operands, 4, NULL, NULL, NULL)",
    __builtin_constant_p 
#line 10721 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && store_multiple_sequence (operands, 4, NULL, NULL, NULL))
    ? (int) 
#line 10721 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && store_multiple_sequence (operands, 4, NULL, NULL, NULL))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_HARD_TP)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 11424 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_HARD_TP))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 11424 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_HARD_TP))
    : -1 },
#line 10734 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && store_multiple_sequence (operands, 3, NULL, NULL, NULL)",
    __builtin_constant_p 
#line 10734 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && store_multiple_sequence (operands, 3, NULL, NULL, NULL))
    ? (int) 
#line 10734 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && store_multiple_sequence (operands, 3, NULL, NULL, NULL))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM && adjacent_mem_locations (operands[2], operands[3]))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 10559 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && adjacent_mem_locations (operands[2], operands[3])))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 10559 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && adjacent_mem_locations (operands[2], operands[3])))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM && arm_arch4 && !arm_arch6)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 4626 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch4 && !arm_arch6))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 4626 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch4 && !arm_arch6))
    : -1 },
#line 1431 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2",
    __builtin_constant_p 
#line 1431 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2)
    ? (int) 
#line 1431 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2)
    : -1 },
#line 8866 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && GET_CODE (operands[0]) == SYMBOL_REF",
    __builtin_constant_p 
#line 8866 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && GET_CODE (operands[0]) == SYMBOL_REF)
    ? (int) 
#line 8866 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && GET_CODE (operands[0]) == SYMBOL_REF)
    : -1 },
#line 1023 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && !TARGET_NEON",
    __builtin_constant_p 
#line 1023 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !TARGET_NEON)
    ? (int) 
#line 1023 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !TARGET_NEON)
    : -1 },
#line 8936 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && USE_RETURN_INSN (TRUE)",
    __builtin_constant_p 
#line 8936 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && USE_RETURN_INSN (TRUE))
    ? (int) 
#line 8936 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && USE_RETURN_INSN (TRUE))
    : -1 },
#line 682 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON && !BYTES_BIG_ENDIAN",
    __builtin_constant_p 
#line 682 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON && !BYTES_BIG_ENDIAN)
    ? (int) 
#line 682 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON && !BYTES_BIG_ENDIAN)
    : -1 },
#line 2273 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && !reg_overlap_mentioned_p (operands[0], operands[3])",
    __builtin_constant_p 
#line 2273 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && !reg_overlap_mentioned_p (operands[0], operands[3]))
    ? (int) 
#line 2273 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && !reg_overlap_mentioned_p (operands[0], operands[3]))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_THUMB2 && arm_arch_hwdiv)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1181 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && arm_arch_hwdiv))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1181 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && arm_arch_hwdiv))
    : -1 },
#line 6094 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_EITHER\n\
   && reload_completed\n\
   && GET_CODE (operands[1]) == CONST_DOUBLE",
    __builtin_constant_p 
#line 6094 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_EITHER
   && reload_completed
   && GET_CODE (operands[1]) == CONST_DOUBLE)
    ? (int) 
#line 6094 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_EITHER
   && reload_completed
   && GET_CODE (operands[1]) == CONST_DOUBLE)
    : -1 },
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V4SFmode != V2SFmode && V4SFmode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (V4SFmode == V4HImode && TARGET_REALLY_IWMMXT)",
    __builtin_constant_p 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V4SFmode != V2SFmode && V4SFmode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V4SFmode == V4HImode && TARGET_REALLY_IWMMXT))
    ? (int) 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V4SFmode != V2SFmode && V4SFmode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V4SFmode == V4HImode && TARGET_REALLY_IWMMXT))
    : -1 },
#line 11527 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_EITHER && (arm_arch6 || !optimize_size)",
    __builtin_constant_p 
#line 11527 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_EITHER && (arm_arch6 || !optimize_size))
    ? (int) 
#line 11527 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_EITHER && (arm_arch6 || !optimize_size))
    : -1 },
#line 4980 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && !(TARGET_HARD_FLOAT && TARGET_MAVERICK)\n\
   && (   register_operand (operands[0], DImode)\n\
       || register_operand (operands[1], DImode))",
    __builtin_constant_p 
#line 4980 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && !(TARGET_HARD_FLOAT && TARGET_MAVERICK)
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode)))
    ? (int) 
#line 4980 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && !(TARGET_HARD_FLOAT && TARGET_MAVERICK)
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode)))
    : -1 },
#line 1105 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2\n\
   && peep2_regno_dead_p(0, CC_REGNUM)\n\
   && (unsigned HOST_WIDE_INT) INTVAL(operands[1]) < 256",
    __builtin_constant_p 
#line 1105 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && peep2_regno_dead_p(0, CC_REGNUM)
   && (unsigned HOST_WIDE_INT) INTVAL(operands[1]) < 256)
    ? (int) 
#line 1105 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && peep2_regno_dead_p(0, CC_REGNUM)
   && (unsigned HOST_WIDE_INT) INTVAL(operands[1]) < 256)
    : -1 },
#line 6275 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FPA",
    __builtin_constant_p 
#line 6275 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FPA)
    ? (int) 
#line 6275 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FPA)
    : -1 },
#line 11463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1 && arm_arch6",
    __builtin_constant_p 
#line 11463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && arm_arch6)
    ? (int) 
#line 11463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && arm_arch6)
    : -1 },
#line 422 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && TARGET_SOFT_FLOAT",
    __builtin_constant_p 
#line 422 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && TARGET_SOFT_FLOAT)
    ? (int) 
#line 422 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && TARGET_SOFT_FLOAT)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM && arm_arch4)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 4587 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch4))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 4587 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch4))
    : -1 },
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V2SImode != V2SFmode && V2SImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (V2SImode == V4HImode && TARGET_REALLY_IWMMXT)",
    __builtin_constant_p 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V2SImode != V2SFmode && V2SImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V2SImode == V4HImode && TARGET_REALLY_IWMMXT))
    ? (int) 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V2SImode != V2SFmode && V2SImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V2SImode == V4HImode && TARGET_REALLY_IWMMXT))
    : -1 },
#line 11351 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && arm_arch5e",
    __builtin_constant_p 
#line 11351 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch5e)
    ? (int) 
#line 11351 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch5e)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM\n\
   && TARGET_HARD_FLOAT && TARGET_FPA\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], DFmode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 558 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 558 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode))))
    : -1 },
#line 8704 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && arm_arch5",
    __builtin_constant_p 
#line 8704 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch5)
    ? (int) 
#line 8704 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch5)
    : -1 },
  { "(TARGET_ARM) && ( reload_completed)",
    __builtin_constant_p (
#line 11392 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM) && 
#line 11394 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
( reload_completed))
    ? (int) (
#line 11392 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM) && 
#line 11394 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
( reload_completed))
    : -1 },
#line 5166 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && (   register_operand (operands[0], SImode) \n\
       || register_operand (operands[1], SImode))",
    __builtin_constant_p 
#line 5166 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (   register_operand (operands[0], SImode) 
       || register_operand (operands[1], SImode)))
    ? (int) 
#line 5166 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (   register_operand (operands[0], SImode) 
       || register_operand (operands[1], SImode)))
    : -1 },
#line 500 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP\n\
   && (   register_operand (operands[0], DFmode)\n\
       || register_operand (operands[1], DFmode))",
    __builtin_constant_p 
#line 500 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))
    ? (int) 
#line 500 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP\n\
   && (   s_register_operand (operands[0], SFmode)\n\
       || s_register_operand (operands[1], SFmode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 372 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP
   && (   s_register_operand (operands[0], SFmode)
       || s_register_operand (operands[1], SFmode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 372 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP
   && (   s_register_operand (operands[0], SFmode)
       || s_register_operand (operands[1], SFmode))))
    : -1 },
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], V16QImode)\n\
       || register_operand (operands[1], V16QImode))",
    __builtin_constant_p 
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V16QImode)
       || register_operand (operands[1], V16QImode)))
    ? (int) 
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V16QImode)
       || register_operand (operands[1], V16QImode)))
    : -1 },
#line 5498 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && arm_arch4",
    __builtin_constant_p 
#line 5498 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch4)
    ? (int) 
#line 5498 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch4)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM && TARGET_VFP && TARGET_HARD_FLOAT\n\
   && (   s_register_operand (operands[0], SImode)\n\
       || s_register_operand (operands[1], SImode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 55 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_VFP && TARGET_HARD_FLOAT
   && (   s_register_operand (operands[0], SImode)
       || s_register_operand (operands[1], SImode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 55 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_VFP && TARGET_HARD_FLOAT
   && (   s_register_operand (operands[0], SImode)
       || s_register_operand (operands[1], SImode))))
    : -1 },
#line 11376 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_EITHER",
    __builtin_constant_p 
#line 11376 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_EITHER)
    ? (int) 
#line 11376 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_EITHER)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM && ! TARGET_IWMMXT\n\
   && !(TARGET_HARD_FLOAT && TARGET_VFP)\n\
   && (   register_operand (operands[0], SImode)\n\
       || register_operand (operands[1], SImode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 5119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 5119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode))))
    : -1 },
#line 30 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/sync.md"
  { "TARGET_HAVE_MEMORY_BARRIER",
    __builtin_constant_p 
#line 30 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/sync.md"
(TARGET_HAVE_MEMORY_BARRIER)
    ? (int) 
#line 30 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/sync.md"
(TARGET_HAVE_MEMORY_BARRIER)
    : -1 },
#line 7943 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_MAVERICK)",
    __builtin_constant_p 
#line 7943 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_MAVERICK))
    ? (int) 
#line 7943 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_MAVERICK))
    : -1 },
#line 2571 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && reload_completed\n\
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))\n\
   && ! IS_IWMMXT_REGNUM (REGNO (operands[0]))",
    __builtin_constant_p 
#line 2571 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))
   && ! IS_IWMMXT_REGNUM (REGNO (operands[0])))
    ? (int) 
#line 2571 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))
   && ! IS_IWMMXT_REGNUM (REGNO (operands[0])))
    : -1 },
#line 4930 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_EITHER && reload_completed",
    __builtin_constant_p 
#line 4930 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_EITHER && reload_completed)
    ? (int) 
#line 4930 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_EITHER && reload_completed)
    : -1 },
#line 4885 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
   && reload_completed\n\
   && (arm_const_double_inline_cost (operands[1])\n\
       <= ((optimize_size || arm_ld_sched) ? 3 : 4))",
    __builtin_constant_p 
#line 4885 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && reload_completed
   && (arm_const_double_inline_cost (operands[1])
       <= ((optimize_size || arm_ld_sched) ? 3 : 4)))
    ? (int) 
#line 4885 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && reload_completed
   && (arm_const_double_inline_cost (operands[1])
       <= ((optimize_size || arm_ld_sched) ? 3 : 4)))
    : -1 },
#line 282 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/sync.md"
  { "TARGET_HAVE_LDREXBHD && TARGET_HAVE_MEMORY_BARRIER",
    __builtin_constant_p 
#line 282 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/sync.md"
(TARGET_HAVE_LDREXBHD && TARGET_HAVE_MEMORY_BARRIER)
    ? (int) 
#line 282 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/sync.md"
(TARGET_HAVE_LDREXBHD && TARGET_HAVE_MEMORY_BARRIER)
    : -1 },
#line 603 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE",
    __builtin_constant_p 
#line 603 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE)
    ? (int) 
#line 603 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE)
    : -1 },
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V2SImode != V2SFmode && V2SImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SImode))",
    __builtin_constant_p 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V2SImode != V2SFmode && V2SImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SImode)))
    ? (int) 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V2SImode != V2SFmode && V2SImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SImode)))
    : -1 },
#line 1269 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && optimize_size && peep2_regno_dead_p (0, CC_REGNUM)",
    __builtin_constant_p 
#line 1269 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && optimize_size && peep2_regno_dead_p (0, CC_REGNUM))
    ? (int) 
#line 1269 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && optimize_size && peep2_regno_dead_p (0, CC_REGNUM))
    : -1 },
#line 289 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2\n\
   && TARGET_SOFT_FLOAT\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], SFmode))",
    __builtin_constant_p 
#line 289 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))
    ? (int) 
#line 289 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))
    : -1 },
#line 4616 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "!arm_arch6",
    __builtin_constant_p 
#line 4616 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(!arm_arch6)
    ? (int) 
#line 4616 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(!arm_arch6)
    : -1 },
#line 4626 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && arm_arch4 && !arm_arch6",
    __builtin_constant_p 
#line 4626 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch4 && !arm_arch6)
    ? (int) 
#line 4626 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch4 && !arm_arch6)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 9346 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 9346 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM))
    : -1 },
#line 10980 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && reload_completed",
    __builtin_constant_p 
#line 10980 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && reload_completed)
    ? (int) 
#line 10980 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && reload_completed)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FPA)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 522 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FPA))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 522 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FPA))
    : -1 },
#line 4279 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && !arm_arch6",
    __builtin_constant_p 
#line 4279 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && !arm_arch6)
    ? (int) 
#line 4279 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && !arm_arch6)
    : -1 },
#line 431 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
  { "TARGET_ARM\n\
   && TARGET_HARD_FLOAT && TARGET_MAVERICK\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], DFmode))",
    __builtin_constant_p 
#line 431 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_MAVERICK
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode)))
    ? (int) 
#line 431 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_MAVERICK
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode)))
    : -1 },
#line 8278 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE",
    __builtin_constant_p 
#line 8278 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE)
    ? (int) 
#line 8278 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE)
    : -1 },
  { "(TARGET_THUMB1) && ( reload_completed && CONST_INT_P (operands[2])\n\
   && operands[1] != stack_pointer_rtx\n\
   && (INTVAL (operands[2]) > 255 || INTVAL (operands[2]) < -255))",
    __builtin_constant_p (
#line 686 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1) && 
#line 706 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
( reload_completed && CONST_INT_P (operands[2])
   && operands[1] != stack_pointer_rtx
   && (INTVAL (operands[2]) > 255 || INTVAL (operands[2]) < -255)))
    ? (int) (
#line 686 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1) && 
#line 706 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
( reload_completed && CONST_INT_P (operands[2])
   && operands[1] != stack_pointer_rtx
   && (INTVAL (operands[2]) > 255 || INTVAL (operands[2]) < -255)))
    : -1 },
#line 7698 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && (GET_CODE (operands[3]) == EQ\n\
       || GET_CODE (operands[3]) == NE\n\
       || GET_CODE (operands[3]) == GE\n\
       || GET_CODE (operands[3]) == LT)",
    __builtin_constant_p 
#line 7698 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (GET_CODE (operands[3]) == EQ
       || GET_CODE (operands[3]) == NE
       || GET_CODE (operands[3]) == GE
       || GET_CODE (operands[3]) == LT))
    ? (int) 
#line 7698 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (GET_CODE (operands[3]) == EQ
       || GET_CODE (operands[3]) == NE
       || GET_CODE (operands[3]) == GE
       || GET_CODE (operands[3]) == LT))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && XVECLEN (operands[0], 0) == 5)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 6466 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 5))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 6466 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 5))
    : -1 },
#line 1055 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && reload_completed\n\
   && GET_CODE(operands[3]) != PLUS\n\
   && GET_CODE(operands[3]) != MINUS",
    __builtin_constant_p 
#line 1055 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && reload_completed
   && GET_CODE(operands[3]) != PLUS
   && GET_CODE(operands[3]) != MINUS)
    ? (int) 
#line 1055 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && reload_completed
   && GET_CODE(operands[3]) != PLUS
   && GET_CODE(operands[3]) != MINUS)
    : -1 },
#line 5359 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "flag_pic",
    __builtin_constant_p 
#line 5359 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(flag_pic)
    ? (int) 
#line 5359 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(flag_pic)
    : -1 },
#line 3825 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT && (TARGET_FPA || TARGET_VFP)",
    __builtin_constant_p 
#line 3825 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && (TARGET_FPA || TARGET_VFP))
    ? (int) 
#line 3825 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && (TARGET_FPA || TARGET_VFP))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_INT_SIMD)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 4655 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_INT_SIMD))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 4655 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_INT_SIMD))
    : -1 },
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V4SImode != V2SFmode && V4SImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SImode))",
    __builtin_constant_p 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V4SImode != V2SFmode && V4SImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SImode)))
    ? (int) 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V4SImode != V2SFmode && V4SImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SImode)))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && XVECLEN (operands[0], 0) == 3)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 6548 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 3))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 6548 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 3))
    : -1 },
#line 11334 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && arm_arch_thumb2",
    __builtin_constant_p 
#line 11334 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch_thumb2)
    ? (int) 
#line 11334 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch_thumb2)
    : -1 },
#line 1436 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2\n\
   && GET_CODE (operands[2]) == CONST_INT\n\
   && !(const_ok_for_arm (INTVAL (operands[2]))\n\
	|| const_ok_for_arm (~INTVAL (operands[2])))",
    __builtin_constant_p 
#line 1436 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && GET_CODE (operands[2]) == CONST_INT
   && !(const_ok_for_arm (INTVAL (operands[2]))
	|| const_ok_for_arm (~INTVAL (operands[2]))))
    ? (int) 
#line 1436 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && GET_CODE (operands[2]) == CONST_INT
   && !(const_ok_for_arm (INTVAL (operands[2]))
	|| const_ok_for_arm (~INTVAL (operands[2]))))
    : -1 },
#line 4587 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && arm_arch4",
    __builtin_constant_p 
#line 4587 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch4)
    ? (int) 
#line 4587 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch4)
    : -1 },
#line 8825 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && GET_CODE (operands[1]) == SYMBOL_REF\n\
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[1]))",
    __builtin_constant_p 
#line 8825 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && GET_CODE (operands[1]) == SYMBOL_REF
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[1])))
    ? (int) 
#line 8825 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && GET_CODE (operands[1]) == SYMBOL_REF
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[1])))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && arm_arch6)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 11454 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch6))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 11454 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch6))
    : -1 },
#line 6534 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && XVECLEN (operands[0], 0) == 4",
    __builtin_constant_p 
#line 6534 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 4)
    ? (int) 
#line 6534 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 4)
    : -1 },
#line 493 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
  { "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_MAVERICK\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], SFmode))",
    __builtin_constant_p 
#line 493 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_MAVERICK
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))
    ? (int) 
#line 493 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_MAVERICK
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))
    : -1 },
#line 5352 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_VXWORKS_RTP && TARGET_ARM && flag_pic",
    __builtin_constant_p 
#line 5352 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_VXWORKS_RTP && TARGET_ARM && flag_pic)
    ? (int) 
#line 5352 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_VXWORKS_RTP && TARGET_ARM && flag_pic)
    : -1 },
#line 78 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/iwmmxt.md"
  { "TARGET_REALLY_IWMMXT\n\
   && (   register_operand (operands[0], DImode)\n\
       || register_operand (operands[1], DImode))",
    __builtin_constant_p 
#line 78 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/iwmmxt.md"
(TARGET_REALLY_IWMMXT
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode)))
    ? (int) 
#line 78 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/iwmmxt.md"
(TARGET_REALLY_IWMMXT
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode)))
    : -1 },
  { "(TARGET_THUMB1) && ( reload_completed)",
    __builtin_constant_p (
#line 11407 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1) && 
#line 11409 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
( reload_completed))
    ? (int) (
#line 11407 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1) && 
#line 11409 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
( reload_completed))
    : -1 },
#line 997 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && !flag_pic",
    __builtin_constant_p 
#line 997 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && !flag_pic)
    ? (int) 
#line 997 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && !flag_pic)
    : -1 },
#line 5119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && ! TARGET_IWMMXT\n\
   && !(TARGET_HARD_FLOAT && TARGET_VFP)\n\
   && (   register_operand (operands[0], SImode)\n\
       || register_operand (operands[1], SImode))",
    __builtin_constant_p 
#line 5119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode)))
    ? (int) 
#line 5119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode)))
    : -1 },
  { "(TARGET_32BIT) && ( reload_completed)",
    __builtin_constant_p (
#line 9488 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 9490 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
( reload_completed))
    ? (int) (
#line 9488 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 9490 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
( reload_completed))
    : -1 },
#line 2088 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
   && GET_CODE (operands[2]) == CONST_INT\n\
   && !(const_ok_for_arm (INTVAL (operands[2]))\n\
	|| const_ok_for_arm (~INTVAL (operands[2])))",
    __builtin_constant_p 
#line 2088 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && GET_CODE (operands[2]) == CONST_INT
   && !(const_ok_for_arm (INTVAL (operands[2]))
	|| const_ok_for_arm (~INTVAL (operands[2]))))
    ? (int) 
#line 2088 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && GET_CODE (operands[2]) == CONST_INT
   && !(const_ok_for_arm (INTVAL (operands[2]))
	|| const_ok_for_arm (~INTVAL (operands[2]))))
    : -1 },
#line 1308 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && optimize_size",
    __builtin_constant_p 
#line 1308 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && optimize_size)
    ? (int) 
#line 1308 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && optimize_size)
    : -1 },
#line 1376 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && !arm_arch6",
    __builtin_constant_p 
#line 1376 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !arm_arch6)
    ? (int) 
#line 1376 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !arm_arch6)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_THUMB2 && TARGET_VFP && TARGET_HARD_FLOAT\n\
   && (   s_register_operand (operands[0], SImode)\n\
       || s_register_operand (operands[1], SImode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 95 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_VFP && TARGET_HARD_FLOAT
   && (   s_register_operand (operands[0], SImode)
       || s_register_operand (operands[1], SImode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 95 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_VFP && TARGET_HARD_FLOAT
   && (   s_register_operand (operands[0], SImode)
       || s_register_operand (operands[1], SImode))))
    : -1 },
#line 5699 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && (   register_operand (operands[0], HImode)\n\
       || register_operand (operands[1], HImode))",
    __builtin_constant_p 
#line 5699 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (   register_operand (operands[0], HImode)
       || register_operand (operands[1], HImode)))
    ? (int) 
#line 5699 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (   register_operand (operands[0], HImode)
       || register_operand (operands[1], HImode)))
    : -1 },
#line 2804 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM\n\
   && !const_ok_for_arm (INTVAL (operands[2]))\n\
   && const_ok_for_arm (~INTVAL (operands[2]))",
    __builtin_constant_p 
#line 2804 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && !const_ok_for_arm (INTVAL (operands[2]))
   && const_ok_for_arm (~INTVAL (operands[2])))
    ? (int) 
#line 2804 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && !const_ok_for_arm (INTVAL (operands[2]))
   && const_ok_for_arm (~INTVAL (operands[2])))
    : -1 },
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT",
    __builtin_constant_p 
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT)
    ? (int) 
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT)
    : -1 },
#line 11454 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && arm_arch6",
    __builtin_constant_p 
#line 11454 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch6)
    ? (int) 
#line 11454 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch6)
    : -1 },
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], V4SFmode)\n\
       || register_operand (operands[1], V4SFmode))",
    __builtin_constant_p 
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V4SFmode)
       || register_operand (operands[1], V4SFmode)))
    ? (int) 
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V4SFmode)
       || register_operand (operands[1], V4SFmode)))
    : -1 },
#line 531 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
  { "TARGET_ARM\n\
   && TARGET_HARD_FLOAT && TARGET_FPA\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], SFmode))",
    __builtin_constant_p 
#line 531 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))
    ? (int) 
#line 531 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))
    : -1 },
#line 686 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
  { "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_FPA && reload_completed",
    __builtin_constant_p 
#line 686 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_FPA && reload_completed)
    ? (int) 
#line 686 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_FPA && reload_completed)
    : -1 },
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "TARGET_NEON\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V16QImode))",
    __builtin_constant_p 
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V16QImode)))
    ? (int) 
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V16QImode)))
    : -1 },
#line 9754 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM\n\
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_OR_Y)\n\
       != CCmode)",
    __builtin_constant_p 
#line 9754 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_OR_Y)
       != CCmode))
    ? (int) 
#line 9754 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_OR_Y)
       != CCmode))
    : -1 },
#line 463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], V4HImode)\n\
       || register_operand (operands[1], V4HImode))",
    __builtin_constant_p 
#line 463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V4HImode)
       || register_operand (operands[1], V4HImode)))
    ? (int) 
#line 463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V4HImode)
       || register_operand (operands[1], V4HImode)))
    : -1 },
#line 4345 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
   && ((unsigned HOST_WIDE_INT) INTVAL (operands[3])\n\
       == (GET_MODE_MASK (GET_MODE (operands[5]))\n\
           & (GET_MODE_MASK (GET_MODE (operands[5]))\n\
	      << (INTVAL (operands[2])))))",
    __builtin_constant_p 
#line 4345 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && ((unsigned HOST_WIDE_INT) INTVAL (operands[3])
       == (GET_MODE_MASK (GET_MODE (operands[5]))
           & (GET_MODE_MASK (GET_MODE (operands[5]))
	      << (INTVAL (operands[2]))))))
    ? (int) 
#line 4345 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && ((unsigned HOST_WIDE_INT) INTVAL (operands[3])
       == (GET_MODE_MASK (GET_MODE (operands[5]))
           & (GET_MODE_MASK (GET_MODE (operands[5]))
	      << (INTVAL (operands[2]))))))
    : -1 },
#line 4655 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_INT_SIMD",
    __builtin_constant_p 
#line 4655 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_INT_SIMD)
    ? (int) 
#line 4655 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_INT_SIMD)
    : -1 },
#line 627 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE",
    __builtin_constant_p 
#line 627 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE)
    ? (int) 
#line 627 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE)
    : -1 },
#line 138 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP && arm_tune != cortexa8\n\
   && (   register_operand (operands[0], DImode)\n\
       || register_operand (operands[1], DImode))",
    __builtin_constant_p 
#line 138 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP && arm_tune != cortexa8
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode)))
    ? (int) 
#line 138 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP && arm_tune != cortexa8
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode)))
    : -1 },
#line 278 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_MAVERICK && 0",
    __builtin_constant_p 
#line 278 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_MAVERICK && 0)
    ? (int) 
#line 278 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_MAVERICK && 0)
    : -1 },
#line 1282 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && optimize_size && reload_completed",
    __builtin_constant_p 
#line 1282 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && optimize_size && reload_completed)
    ? (int) 
#line 1282 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && optimize_size && reload_completed)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_THUMB2\n\
   && TARGET_SOFT_FLOAT\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], SFmode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 289 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 289 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))))
    : -1 },
#line 662 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
   && GET_CODE (operands[2]) == CONST_INT\n\
   && !(const_ok_for_arm (INTVAL (operands[2]))\n\
        || const_ok_for_arm (-INTVAL (operands[2])))\n\
   && (reload_completed || !arm_eliminable_register (operands[1]))",
    __builtin_constant_p 
#line 662 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && GET_CODE (operands[2]) == CONST_INT
   && !(const_ok_for_arm (INTVAL (operands[2]))
        || const_ok_for_arm (-INTVAL (operands[2])))
   && (reload_completed || !arm_eliminable_register (operands[1])))
    ? (int) 
#line 662 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && GET_CODE (operands[2]) == CONST_INT
   && !(const_ok_for_arm (INTVAL (operands[2]))
        || const_ok_for_arm (-INTVAL (operands[2])))
   && (reload_completed || !arm_eliminable_register (operands[1])))
    : -1 },
  { "(TARGET_THUMB2) && ( reload_completed)",
    __builtin_constant_p (
#line 924 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2) && 
#line 928 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
( reload_completed))
    ? (int) (
#line 924 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2) && 
#line 928 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
( reload_completed))
    : -1 },
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V8QImode != V2SFmode && V8QImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (V8QImode == V4HImode && TARGET_REALLY_IWMMXT)",
    __builtin_constant_p 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V8QImode != V2SFmode && V8QImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V8QImode == V4HImode && TARGET_REALLY_IWMMXT))
    ? (int) 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V8QImode != V2SFmode && V8QImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V8QImode == V4HImode && TARGET_REALLY_IWMMXT))
    : -1 },
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V4SImode != V2SFmode && V4SImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (V4SImode == V4HImode && TARGET_REALLY_IWMMXT)",
    __builtin_constant_p 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V4SImode != V2SFmode && V4SImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V4SImode == V4HImode && TARGET_REALLY_IWMMXT))
    ? (int) 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V4SImode != V2SFmode && V4SImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V4SImode == V4HImode && TARGET_REALLY_IWMMXT))
    : -1 },
#line 5276 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1 && flag_pic",
    __builtin_constant_p 
#line 5276 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && flag_pic)
    ? (int) 
#line 5276 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && flag_pic)
    : -1 },
  { "(TARGET_32BIT) && ( ((GET_CODE (operands[1]) == CONST_INT\n\
       	&& !const_ok_for_arm (INTVAL (operands[1])))\n\
       || (GET_CODE (operands[2]) == CONST_INT\n\
	   && !const_ok_for_arm (INTVAL (operands[2])))))",
    __builtin_constant_p (
#line 1135 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1142 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
( ((GET_CODE (operands[1]) == CONST_INT
       	&& !const_ok_for_arm (INTVAL (operands[1])))
       || (GET_CODE (operands[2]) == CONST_INT
	   && !const_ok_for_arm (INTVAL (operands[2]))))))
    ? (int) (
#line 1135 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1142 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
( ((GET_CODE (operands[1]) == CONST_INT
       	&& !const_ok_for_arm (INTVAL (operands[1])))
       || (GET_CODE (operands[2]) == CONST_INT
	   && !const_ok_for_arm (INTVAL (operands[2]))))))
    : -1 },
#line 2600 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
   && reload_completed\n\
   && operands[0] != operands[1]",
    __builtin_constant_p 
#line 2600 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && reload_completed
   && operands[0] != operands[1])
    ? (int) 
#line 2600 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && reload_completed
   && operands[0] != operands[1])
    : -1 },
#line 1070 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2\n\
   && peep2_regno_dead_p(0, CC_REGNUM)\n\
   && ((GET_CODE(operands[3]) != ROTATE && GET_CODE(operands[3]) != ROTATERT)\n\
       || REG_P(operands[2]))",
    __builtin_constant_p 
#line 1070 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && peep2_regno_dead_p(0, CC_REGNUM)
   && ((GET_CODE(operands[3]) != ROTATE && GET_CODE(operands[3]) != ROTATERT)
       || REG_P(operands[2])))
    ? (int) 
#line 1070 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2
   && peep2_regno_dead_p(0, CC_REGNUM)
   && ((GET_CODE(operands[3]) != ROTATE && GET_CODE(operands[3]) != ROTATERT)
       || REG_P(operands[2])))
    : -1 },
#line 8879 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && GET_CODE (operands[1]) == SYMBOL_REF",
    __builtin_constant_p 
#line 8879 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && GET_CODE (operands[1]) == SYMBOL_REF)
    ? (int) 
#line 8879 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && GET_CODE (operands[1]) == SYMBOL_REF)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1181 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1181 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_THUMB2 && optimize_size && reload_completed)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1282 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && optimize_size && reload_completed))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1282 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && optimize_size && reload_completed))
    : -1 },
#line 4960 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_LDRD\n\
  && reg_overlap_mentioned_p (operands[0], operands[1])\n\
  && reg_overlap_mentioned_p (operands[0], operands[2])",
    __builtin_constant_p 
#line 4960 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_LDRD
  && reg_overlap_mentioned_p (operands[0], operands[1])
  && reg_overlap_mentioned_p (operands[0], operands[2]))
    ? (int) 
#line 4960 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_LDRD
  && reg_overlap_mentioned_p (operands[0], operands[1])
  && reg_overlap_mentioned_p (operands[0], operands[2]))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && XVECLEN (operands[0], 0) == 4)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 6534 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 4))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 6534 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 4))
    : -1 },
#line 1837 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON && (!false || flag_unsafe_math_optimizations)",
    __builtin_constant_p 
#line 1837 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON && (!false || flag_unsafe_math_optimizations))
    ? (int) 
#line 1837 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON && (!false || flag_unsafe_math_optimizations))
    : -1 },
#line 5787 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM\n\
   && arm_arch4\n\
   && (GET_CODE (operands[1]) != CONST_INT\n\
       || const_ok_for_arm (INTVAL (operands[1]))\n\
       || const_ok_for_arm (~INTVAL (operands[1])))",
    __builtin_constant_p 
#line 5787 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && arm_arch4
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1]))))
    ? (int) 
#line 5787 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && arm_arch4
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1]))))
    : -1 },
#line 649 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
  { "TARGET_THUMB2\n\
   && TARGET_HARD_FLOAT && TARGET_FPA\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], DFmode))",
    __builtin_constant_p 
#line 649 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_THUMB2
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode)))
    ? (int) 
#line 649 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_THUMB2
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode)))
    : -1 },
#line 318 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FP16 && !TARGET_NEON_FP16\n\
   && (   s_register_operand (operands[0], HFmode)\n\
       || s_register_operand (operands[1], HFmode))",
    __builtin_constant_p 
#line 318 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FP16 && !TARGET_NEON_FP16
   && (   s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode)))
    ? (int) 
#line 318 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FP16 && !TARGET_NEON_FP16
   && (   s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode)))
    : -1 },
#line 8496 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && TARGET_SOFT_FLOAT",
    __builtin_constant_p 
#line 8496 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && TARGET_SOFT_FLOAT)
    ? (int) 
#line 8496 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && TARGET_SOFT_FLOAT)
    : -1 },
#line 4860 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM\n\
   && !(TARGET_HARD_FLOAT && (TARGET_MAVERICK || TARGET_VFP))\n\
   && !TARGET_IWMMXT\n\
   && (   register_operand (operands[0], DImode)\n\
       || register_operand (operands[1], DImode))",
    __builtin_constant_p 
#line 4860 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && !(TARGET_HARD_FLOAT && (TARGET_MAVERICK || TARGET_VFP))
   && !TARGET_IWMMXT
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode)))
    ? (int) 
#line 4860 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && !(TARGET_HARD_FLOAT && (TARGET_MAVERICK || TARGET_VFP))
   && !TARGET_IWMMXT
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode)))
    : -1 },
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "TARGET_NEON\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4HImode))",
    __builtin_constant_p 
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4HImode)))
    ? (int) 
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4HImode)))
    : -1 },
#line 2380 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM || arm_arch_thumb2",
    __builtin_constant_p 
#line 2380 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM || arm_arch_thumb2)
    ? (int) 
#line 2380 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM || arm_arch_thumb2)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_FP16)\n\
   && (	  s_register_operand (operands[0], HFmode)\n\
       || s_register_operand (operands[1], HFmode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 5992 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_FP16)
   && (	  s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 5992 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_FP16)
   && (	  s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode))))
    : -1 },
#line 10695 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && load_multiple_sequence (operands, 3, NULL, NULL, NULL)",
    __builtin_constant_p 
#line 10695 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && load_multiple_sequence (operands, 3, NULL, NULL, NULL))
    ? (int) 
#line 10695 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && load_multiple_sequence (operands, 3, NULL, NULL, NULL))
    : -1 },
#line 10759 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM",
    __builtin_constant_p 
#line 10759 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM)
    ? (int) 
#line 10759 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM)
    : -1 },
#line 10559 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && adjacent_mem_locations (operands[2], operands[3])",
    __builtin_constant_p 
#line 10559 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && adjacent_mem_locations (operands[2], operands[3]))
    ? (int) 
#line 10559 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && adjacent_mem_locations (operands[2], operands[3]))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM && TARGET_HARD_FLOAT && TARGET_FPA\n\
   && (register_operand (operands[0], XFmode)\n\
       || register_operand (operands[1], XFmode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 594 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_FPA
   && (register_operand (operands[0], XFmode)
       || register_operand (operands[1], XFmode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 594 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_FPA
   && (register_operand (operands[0], XFmode)
       || register_operand (operands[1], XFmode))))
    : -1 },
#line 620 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
  { "TARGET_THUMB2\n\
   && TARGET_HARD_FLOAT && TARGET_FPA\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], SFmode))",
    __builtin_constant_p 
#line 620 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_THUMB2
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))
    ? (int) 
#line 620 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_THUMB2
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))
    : -1 },
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V4HImode != V2SFmode && V4HImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4HImode))",
    __builtin_constant_p 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V4HImode != V2SFmode && V4HImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4HImode)))
    ? (int) 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V4HImode != V2SFmode && V4HImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4HImode)))
    : -1 },
#line 7845 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && (GET_CODE (operands[0]) == EQ\n\
       || GET_CODE (operands[0]) == NE\n\
       || GET_CODE (operands[0]) == GE\n\
       || GET_CODE (operands[0]) == LT)",
    __builtin_constant_p 
#line 7845 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (GET_CODE (operands[0]) == EQ
       || GET_CODE (operands[0]) == NE
       || GET_CODE (operands[0]) == GE
       || GET_CODE (operands[0]) == LT))
    ? (int) 
#line 7845 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (GET_CODE (operands[0]) == EQ
       || GET_CODE (operands[0]) == NE
       || GET_CODE (operands[0]) == GE
       || GET_CODE (operands[0]) == LT))
    : -1 },
#line 9810 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM\n\
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)\n\
       != CCmode)",
    __builtin_constant_p 
#line 9810 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)
       != CCmode))
    ? (int) 
#line 9810 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)
       != CCmode))
    : -1 },
#line 786 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && INTVAL (operands[2]) == -INTVAL (operands[3])",
    __builtin_constant_p 
#line 786 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && INTVAL (operands[2]) == -INTVAL (operands[3]))
    ? (int) 
#line 786 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && INTVAL (operands[2]) == -INTVAL (operands[3]))
    : -1 },
#line 5937 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
   && (   register_operand (operands[0], QImode)\n\
       || register_operand (operands[1], QImode))",
    __builtin_constant_p 
#line 5937 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode)))
    ? (int) 
#line 5937 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode)))
    : -1 },
#line 11424 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_HARD_TP",
    __builtin_constant_p 
#line 11424 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_HARD_TP)
    ? (int) 
#line 11424 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_HARD_TP)
    : -1 },
#line 9814 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && reload_completed\n\
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)\n\
       != CCmode)",
    __builtin_constant_p 
#line 9814 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && reload_completed
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)
       != CCmode))
    ? (int) 
#line 9814 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && reload_completed
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)
       != CCmode))
    : -1 },
#line 6037 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && (	  s_register_operand (operands[0], HFmode) \n\
       || s_register_operand (operands[1], HFmode))",
    __builtin_constant_p 
#line 6037 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (	  s_register_operand (operands[0], HFmode) 
       || s_register_operand (operands[1], HFmode)))
    ? (int) 
#line 6037 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (	  s_register_operand (operands[0], HFmode) 
       || s_register_operand (operands[1], HFmode)))
    : -1 },
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], V4SImode)\n\
       || register_operand (operands[1], V4SImode))",
    __builtin_constant_p 
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V4SImode)
       || register_operand (operands[1], V4SImode)))
    ? (int) 
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V4SImode)
       || register_operand (operands[1], V4SImode)))
    : -1 },
#line 2240 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM\n\
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32\n\
       && INTVAL (operands[2]) > 0 \n\
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8\n\
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)\n\
   && !reg_overlap_mentioned_p (operands[0], operands[4])",
    __builtin_constant_p 
#line 2240 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)
   && !reg_overlap_mentioned_p (operands[0], operands[4]))
    ? (int) 
#line 2240 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)
   && !reg_overlap_mentioned_p (operands[0], operands[4]))
    : -1 },
#line 555 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP",
    __builtin_constant_p 
#line 555 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP)
    ? (int) 
#line 555 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP)
    : -1 },
#line 8428 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT",
    __builtin_constant_p 
#line 8428 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && TARGET_HARD_FLOAT)
    ? (int) 
#line 8428 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && TARGET_HARD_FLOAT)
    : -1 },
#line 365 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_MAVERICK",
    __builtin_constant_p 
#line 365 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_MAVERICK)
    ? (int) 
#line 365 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_MAVERICK)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_THUMB2 && reload_completed)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1399 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && reload_completed))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1399 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && reload_completed))
    : -1 },
#line 4914 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && reload_completed\n\
   && arm_const_double_by_parts (operands[1])",
    __builtin_constant_p 
#line 4914 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && reload_completed
   && arm_const_double_by_parts (operands[1]))
    ? (int) 
#line 4914 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && reload_completed
   && arm_const_double_by_parts (operands[1]))
    : -1 },
#line 6239 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && (   register_operand (operands[0], DFmode)\n\
       || register_operand (operands[1], DFmode))",
    __builtin_constant_p 
#line 6239 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))
    ? (int) 
#line 6239 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))
    : -1 },
#line 8454 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT && (TARGET_FPA || TARGET_VFP_DOUBLE)",
    __builtin_constant_p 
#line 8454 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && (TARGET_FPA || TARGET_VFP_DOUBLE))
    ? (int) 
#line 8454 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && (TARGET_FPA || TARGET_VFP_DOUBLE))
    : -1 },
#line 6109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM\n\
   && TARGET_SOFT_FLOAT\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], SFmode))",
    __builtin_constant_p 
#line 6109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))
    ? (int) 
#line 6109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))
    : -1 },
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V4SFmode != V2SFmode && V4SFmode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SFmode))",
    __builtin_constant_p 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V4SFmode != V2SFmode && V4SFmode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SFmode)))
    ? (int) 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V4SFmode != V2SFmode && V4SFmode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SFmode)))
    : -1 },
#line 70 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V2DImode != V2SFmode && V2DImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2DImode))",
    __builtin_constant_p 
#line 70 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V2DImode != V2SFmode && V2DImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2DImode)))
    ? (int) 
#line 70 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V2DImode != V2SFmode && V2DImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2DImode)))
    : -1 },
#line 8715 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && !arm_arch5",
    __builtin_constant_p 
#line 8715 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && !arm_arch5)
    ? (int) 
#line 8715 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && !arm_arch5)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && XVECLEN (operands[0], 0) == 2)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 6560 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 2))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 6560 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 2))
    : -1 },
  { "(TARGET_32BIT) && (arm_arch_thumb2)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 11445 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(arm_arch_thumb2))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 11445 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(arm_arch_thumb2))
    : -1 },
#line 976 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && arm_arch6",
    __builtin_constant_p 
#line 976 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && arm_arch6)
    ? (int) 
#line 976 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && arm_arch6)
    : -1 },
#line 10682 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && load_multiple_sequence (operands, 4, NULL, NULL, NULL)",
    __builtin_constant_p 
#line 10682 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && load_multiple_sequence (operands, 4, NULL, NULL, NULL))
    ? (int) 
#line 10682 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && load_multiple_sequence (operands, 4, NULL, NULL, NULL))
    : -1 },
#line 1947 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && operands[0] != operands[1] && reload_completed",
    __builtin_constant_p 
#line 1947 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && operands[0] != operands[1] && reload_completed)
    ? (int) 
#line 1947 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && operands[0] != operands[1] && reload_completed)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM && USE_RETURN_INSN (FALSE))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 8894 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && USE_RETURN_INSN (FALSE)))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 8894 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && USE_RETURN_INSN (FALSE)))
    : -1 },
#line 3839 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && reload_completed",
    __builtin_constant_p 
#line 3839 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && reload_completed)
    ? (int) 
#line 3839 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && reload_completed)
    : -1 },
#line 463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], V8QImode)\n\
       || register_operand (operands[1], V8QImode))",
    __builtin_constant_p 
#line 463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V8QImode)
       || register_operand (operands[1], V8QImode)))
    ? (int) 
#line 463 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V8QImode)
       || register_operand (operands[1], V8QImode)))
    : -1 },
#line 5263 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && flag_pic",
    __builtin_constant_p 
#line 5263 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && flag_pic)
    ? (int) 
#line 5263 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && flag_pic)
    : -1 },
#line 1434 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON && flag_unsafe_math_optimizations",
    __builtin_constant_p 
#line 1434 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON && flag_unsafe_math_optimizations)
    ? (int) 
#line 1434 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON && flag_unsafe_math_optimizations)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_NEON)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 3072 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 3072 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON))
    : -1 },
#line 10777 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM\n\
   && (((unsigned HOST_WIDE_INT) INTVAL (operands[1]))\n\
       == (((unsigned HOST_WIDE_INT) INTVAL (operands[1])) >> 24) << 24)",
    __builtin_constant_p 
#line 10777 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && (((unsigned HOST_WIDE_INT) INTVAL (operands[1]))
       == (((unsigned HOST_WIDE_INT) INTVAL (operands[1])) >> 24) << 24))
    ? (int) 
#line 10777 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && (((unsigned HOST_WIDE_INT) INTVAL (operands[1]))
       == (((unsigned HOST_WIDE_INT) INTVAL (operands[1])) >> 24) << 24))
    : -1 },
#line 449 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP\n\
   && (   register_operand (operands[0], DFmode)\n\
       || register_operand (operands[1], DFmode))",
    __builtin_constant_p 
#line 449 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))
    ? (int) 
#line 449 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))
    : -1 },
#line 8811 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && GET_CODE (operands[0]) == SYMBOL_REF\n\
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[0]))",
    __builtin_constant_p 
#line 8811 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && GET_CODE (operands[0]) == SYMBOL_REF
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[0])))
    ? (int) 
#line 8811 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && GET_CODE (operands[0]) == SYMBOL_REF
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[0])))
    : -1 },
#line 306 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && TARGET_SOFT_FLOAT\n\
   && (   register_operand (operands[0], DFmode)\n\
       || register_operand (operands[1], DFmode))",
    __builtin_constant_p 
#line 306 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && TARGET_SOFT_FLOAT
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))
    ? (int) 
#line 306 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && TARGET_SOFT_FLOAT
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))
    : -1 },
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], V8HImode)\n\
       || register_operand (operands[1], V8HImode))",
    __builtin_constant_p 
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V8HImode)
       || register_operand (operands[1], V8HImode)))
    ? (int) 
#line 512 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], V8HImode)
       || register_operand (operands[1], V8HImode)))
    : -1 },
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], XImode)\n\
       || register_operand (operands[1], XImode))",
    __builtin_constant_p 
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], XImode)
       || register_operand (operands[1], XImode)))
    ? (int) 
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], XImode)
       || register_operand (operands[1], XImode)))
    : -1 },
#line 1720 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && arm_arch3m && !arm_arch6",
    __builtin_constant_p 
#line 1720 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch3m && !arm_arch6)
    ? (int) 
#line 1720 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch3m && !arm_arch6)
    : -1 },
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V8QImode != V2SFmode && V8QImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8QImode))",
    __builtin_constant_p 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V8QImode != V2SFmode && V8QImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8QImode)))
    ? (int) 
#line 109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V8QImode != V2SFmode && V8QImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8QImode)))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && !arm_arch6)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1376 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !arm_arch6))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1376 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !arm_arch6))
    : -1 },
#line 151 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
  { "0 && TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_MAVERICK",
    __builtin_constant_p 
#line 151 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(0 && TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_MAVERICK)
    ? (int) 
#line 151 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(0 && TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_MAVERICK)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && arm_arch_thumb2)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 11326 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch_thumb2))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 11326 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch_thumb2))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1157 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1157 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP && arm_tune == cortexa8\n\
   && (   register_operand (operands[0], DImode)\n\
       || register_operand (operands[1], DImode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 180 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP && arm_tune == cortexa8
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 180 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP && arm_tune == cortexa8
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode))))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FP16)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 964 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FP16))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 964 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FP16))
    : -1 },
#line 4318 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && (GET_CODE (operands[1]) != MEM) && ! BYTES_BIG_ENDIAN",
    __builtin_constant_p 
#line 4318 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && (GET_CODE (operands[1]) != MEM) && ! BYTES_BIG_ENDIAN)
    ? (int) 
#line 4318 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && (GET_CODE (operands[1]) != MEM) && ! BYTES_BIG_ENDIAN)
    : -1 },
#line 6664 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1 || TARGET_32BIT",
    __builtin_constant_p 
#line 6664 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 || TARGET_32BIT)
    ? (int) 
#line 6664 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 || TARGET_32BIT)
    : -1 },
#line 38 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "TARGET_NEON\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SFmode))",
    __builtin_constant_p 
#line 38 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SFmode)))
    ? (int) 
#line 38 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SFmode)))
    : -1 },
#line 1707 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && arm_arch3m",
    __builtin_constant_p 
#line 1707 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch3m)
    ? (int) 
#line 1707 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && arm_arch3m)
    : -1 },
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "(TARGET_NEON && ((V16QImode != V2SFmode && V16QImode != V4SFmode)\n\
		    || flag_unsafe_math_optimizations))\n\
   || (V16QImode == V4HImode && TARGET_REALLY_IWMMXT)",
    __builtin_constant_p 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V16QImode != V2SFmode && V16QImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V16QImode == V4HImode && TARGET_REALLY_IWMMXT))
    ? (int) 
#line 80 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
((TARGET_NEON && ((V16QImode != V2SFmode && V16QImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V16QImode == V4HImode && TARGET_REALLY_IWMMXT))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_DSP_MULTIPLY)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1814 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_DSP_MULTIPLY))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1814 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_DSP_MULTIPLY))
    : -1 },
#line 263 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_NEON_FP16\n\
   && (   s_register_operand (operands[0], HFmode)\n\
       || s_register_operand (operands[1], HFmode))",
    __builtin_constant_p 
#line 263 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_NEON_FP16
   && (   s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode)))
    ? (int) 
#line 263 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_NEON_FP16
   && (   s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode)))
    : -1 },
#line 4255 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1 && !arm_arch6",
    __builtin_constant_p 
#line 4255 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && !arm_arch6)
    ? (int) 
#line 4255 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && !arm_arch6)
    : -1 },
#line 9872 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM\n\
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)\n\
       == CCmode)",
    __builtin_constant_p 
#line 9872 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)
       == CCmode))
    ? (int) 
#line 9872 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)
       == CCmode))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_THUMB2)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1431 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1431 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2))
    : -1 },
#line 6466 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT && XVECLEN (operands[0], 0) == 5",
    __builtin_constant_p 
#line 6466 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 5)
    ? (int) 
#line 6466 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && XVECLEN (operands[0], 0) == 5)
    : -1 },
#line 5747 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON",
    __builtin_constant_p 
#line 5747 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON)
    ? (int) 
#line 5747 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON)
    : -1 },
#line 731 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && (unsigned HOST_WIDE_INT) (INTVAL (operands[1])) < 1024\n\
   && (INTVAL (operands[1]) & 3) == 0",
    __builtin_constant_p 
#line 731 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (unsigned HOST_WIDE_INT) (INTVAL (operands[1])) < 1024
   && (INTVAL (operands[1]) & 3) == 0)
    ? (int) 
#line 731 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (unsigned HOST_WIDE_INT) (INTVAL (operands[1])) < 1024
   && (INTVAL (operands[1]) & 3) == 0)
    : -1 },
#line 11290 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1",
    __builtin_constant_p 
#line 11290 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1)
    ? (int) 
#line 11290 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1)
    : -1 },
#line 10745 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && store_multiple_sequence (operands, 2, NULL, NULL, NULL)",
    __builtin_constant_p 
#line 10745 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && store_multiple_sequence (operands, 2, NULL, NULL, NULL))
    ? (int) 
#line 10745 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && store_multiple_sequence (operands, 2, NULL, NULL, NULL))
    : -1 },
#line 1099 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP",
    __builtin_constant_p 
#line 1099 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP)
    ? (int) 
#line 1099 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP)
    : -1 },
#line 38 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "TARGET_NEON\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SFmode))",
    __builtin_constant_p 
#line 38 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SFmode)))
    ? (int) 
#line 38 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SFmode)))
    : -1 },
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "TARGET_NEON\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8QImode))",
    __builtin_constant_p 
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8QImode)))
    ? (int) 
#line 119 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8QImode)))
    : -1 },
#line 6128 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1\n\
   && (   register_operand (operands[0], SFmode) \n\
       || register_operand (operands[1], SFmode))",
    __builtin_constant_p 
#line 6128 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (   register_operand (operands[0], SFmode) 
       || register_operand (operands[1], SFmode)))
    ? (int) 
#line 6128 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1
   && (   register_operand (operands[0], SFmode) 
       || register_operand (operands[1], SFmode)))
    : -1 },
#line 6212 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && TARGET_SOFT_FLOAT\n\
   && (   register_operand (operands[0], DFmode)\n\
       || register_operand (operands[1], DFmode))",
    __builtin_constant_p 
#line 6212 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && TARGET_SOFT_FLOAT
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))
    ? (int) 
#line 6212 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && TARGET_SOFT_FLOAT
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))
    : -1 },
#line 4641 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_ARM && arm_arch6",
    __builtin_constant_p 
#line 4641 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch6)
    ? (int) 
#line 4641 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM && arm_arch6)
    : -1 },
#line 1089 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
  { "TARGET_THUMB2 && reload_completed\n\
   && ((GET_CODE(operands[3]) != ROTATE && GET_CODE(operands[3]) != ROTATERT)\n\
       || REG_P(operands[2]))",
    __builtin_constant_p 
#line 1089 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && reload_completed
   && ((GET_CODE(operands[3]) != ROTATE && GET_CODE(operands[3]) != ROTATERT)
       || REG_P(operands[2])))
    ? (int) 
#line 1089 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/thumb2.md"
(TARGET_THUMB2 && reload_completed
   && ((GET_CODE(operands[3]) != ROTATE && GET_CODE(operands[3]) != ROTATERT)
       || REG_P(operands[2])))
    : -1 },
#line 373 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
  { "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_MAVERICK",
    __builtin_constant_p 
#line 373 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_MAVERICK)
    ? (int) 
#line 373 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_MAVERICK)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM\n\
   && TARGET_SOFT_FLOAT\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], SFmode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 6109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 6109 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_ARM
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 2847 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 2847 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON))
    : -1 },
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
  { "TARGET_NEON\n\
   && (register_operand (operands[0], OImode)\n\
       || register_operand (operands[1], OImode))",
    __builtin_constant_p 
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], OImode)
       || register_operand (operands[1], OImode)))
    ? (int) 
#line 579 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/neon.md"
(TARGET_NEON
   && (register_operand (operands[0], OImode)
       || register_operand (operands[1], OImode)))
    : -1 },
#line 8745 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB1 && arm_arch5",
    __builtin_constant_p 
#line 8745 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && arm_arch5)
    ? (int) 
#line 8745 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB1 && arm_arch5)
    : -1 },
#line 461 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
  { "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_MAVERICK",
    __builtin_constant_p 
#line 461 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_MAVERICK)
    ? (int) 
#line 461 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_MAVERICK)
    : -1 },
  { "(TARGET_32BIT) && (TARGET_REALLY_IWMMXT)",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1336 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/iwmmxt.md"
(TARGET_REALLY_IWMMXT))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 1336 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/iwmmxt.md"
(TARGET_REALLY_IWMMXT))
    : -1 },
#line 11520 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_THUMB",
    __builtin_constant_p 
#line 11520 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB)
    ? (int) 
#line 11520 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_THUMB)
    : -1 },
#line 180 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
  { "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP && arm_tune == cortexa8\n\
   && (   register_operand (operands[0], DImode)\n\
       || register_operand (operands[1], DImode))",
    __builtin_constant_p 
#line 180 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP && arm_tune == cortexa8
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode)))
    ? (int) 
#line 180 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vfp.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP && arm_tune == cortexa8
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode)))
    : -1 },
#line 38 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
  { "TARGET_NEON\n\
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2DImode))",
    __builtin_constant_p 
#line 38 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2DImode)))
    ? (int) 
#line 38 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/vec-common.md"
(TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2DImode)))
    : -1 },
#line 409 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
  { "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_MAVERICK\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], SFmode))",
    __builtin_constant_p 
#line 409 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_MAVERICK
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))
    ? (int) 
#line 409 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/cirrus.md"
(TARGET_ARM && TARGET_HARD_FLOAT && TARGET_MAVERICK
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))
    : -1 },
#line 8796 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
  { "TARGET_32BIT\n\
   && (GET_CODE (operands[1]) == SYMBOL_REF)\n\
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[1]))",
    __builtin_constant_p 
#line 8796 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && (GET_CODE (operands[1]) == SYMBOL_REF)
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[1])))
    ? (int) 
#line 8796 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT
   && (GET_CODE (operands[1]) == SYMBOL_REF)
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[1])))
    : -1 },
  { "(TARGET_32BIT) && (TARGET_ARM\n\
   && TARGET_HARD_FLOAT && TARGET_FPA\n\
   && (GET_CODE (operands[0]) != MEM\n\
       || register_operand (operands[1], SFmode)))",
    __builtin_constant_p (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 531 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))))
    ? (int) (
#line 11360 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/arm.md"
(TARGET_32BIT) && 
#line 531 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/fpa.md"
(TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))))
    : -1 },
#line 252 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/sync.md"
  { "TARGET_HAVE_LDREX && TARGET_HAVE_MEMORY_BARRIER",
    __builtin_constant_p 
#line 252 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/sync.md"
(TARGET_HAVE_LDREX && TARGET_HAVE_MEMORY_BARRIER)
    ? (int) 
#line 252 "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/toolchain-arm_v5te_gcc-linaro_uClibc-0.9.32_eabi/gcc-linaro-4.5-2011.02-0/gcc/config/arm/sync.md"
(TARGET_HAVE_LDREX && TARGET_HAVE_MEMORY_BARRIER)
    : -1 },

};
#endif /* gcc >= 3.0.1 */

int
main(void)
{
  unsigned int i;
  const char *p;
  puts ("(define_conditions [");
#if GCC_VERSION >= 3001
  for (i = 0; i < ARRAY_SIZE (insn_conditions); i++)
    {
      printf ("  (%d \"", insn_conditions[i].value);
      for (p = insn_conditions[i].expr; *p; p++)
        {
          switch (*p)
	     {
	     case '\\':
	     case '\"': putchar ('\\'); break;
	     default: break;
	     }
          putchar (*p);
        }
      puts ("\")");
    }
#endif /* gcc >= 3.0.1 */
  puts ("])");
  fflush (stdout);
return ferror (stdout) != 0 ? FATAL_EXIT_CODE : SUCCESS_EXIT_CODE;
}

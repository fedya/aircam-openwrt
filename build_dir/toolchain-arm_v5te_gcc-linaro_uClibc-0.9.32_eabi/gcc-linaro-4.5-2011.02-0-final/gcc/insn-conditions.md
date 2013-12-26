(define_conditions [
  (-1 "(TARGET_32BIT) && (TARGET_32BIT
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode)))")
  (-1 "(TARGET_NEON && ((V2SFmode != V2SFmode && V2SFmode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SFmode))")
  (-1 "(TARGET_NEON && ((V4HImode != V2SFmode && V4HImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V4HImode == V4HImode && TARGET_REALLY_IWMMXT)")
  (-1 "TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SImode))")
  (-1 "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FP16")
  (-1 "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_FPA")
  (-1 "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP
   && (   s_register_operand (operands[0], SFmode)
       || s_register_operand (operands[1], SFmode))")
  (-1 "TARGET_32BIT && (GET_CODE (operands[1]) != MEM) && BYTES_BIG_ENDIAN")
  (-1 "TARGET_32BIT && XVECLEN (operands[0], 0) == 2")
  (-1 "TARGET_THUMB1 || arm_arch_thumb2")
  (-1 "TARGET_32BIT && XVECLEN (operands[0], 0) == 3")
  (-1 "(TARGET_32BIT) && (TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP
   && (   s_register_operand (operands[0], SFmode)
       || s_register_operand (operands[1], SFmode)))")
  (-1 "(TARGET_32BIT) && (TARGET_THUMB2 && reload_completed
   && GET_CODE(operands[3]) != PLUS
   && GET_CODE(operands[3]) != MINUS)")
  (-1 "(TARGET_32BIT) && (TARGET_THUMB2 && reload_completed
   && ((GET_CODE(operands[3]) != ROTATE && GET_CODE(operands[3]) != ROTATERT)
       || REG_P(operands[2])))")
  (-1 "(TARGET_NEON && ((V8HImode != V2SFmode && V8HImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V8HImode == V4HImode && TARGET_REALLY_IWMMXT)")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && arm_arch5)")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], V2SFmode)
       || register_operand (operands[1], V2SFmode))")
  (-1 "TARGET_REALLY_IWMMXT
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode))")
  (-1 "TARGET_REALLY_IWMMXT")
  (-1 "TARGET_NEON && (!true || flag_unsafe_math_optimizations)")
  (-1 "TARGET_32BIT
  && (INTVAL (operands[2]) >= 0 && INTVAL (operands[2]) < 32
      && INTVAL (operands[1]) > 0 
      && INTVAL (operands[1]) + (INTVAL (operands[2]) & 1) <= 8
      && INTVAL (operands[1]) + INTVAL (operands[2]) <= 32)")
  (-1 "TARGET_THUMB2 && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode))")
  (-1 "TARGET_THUMB2 && arm_arch_hwdiv")
  (-1 "TARGET_THUMB1 && XVECLEN (operands[0], 0) == 5")
  (-1 "TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_MAVERICK) && !TARGET_NEON")
  (-1 "arm_arch_thumb2")
  (-1 "(TARGET_32BIT) && (TARGET_THUMB2 && arm_arch6)")
  (-1 "TARGET_32BIT && USE_RETURN_INSN (FALSE)")
  (-1 "TARGET_ARM && USE_RETURN_INSN (FALSE)")
  (-1 "TARGET_THUMB1 && satisfies_constraint_J (operands[1])")
  (-1 "TARGET_32BIT
   && !const_ok_for_arm (INTVAL (operands[1]))
   && const_ok_for_arm (~INTVAL (operands[1]))")
  (-1 "TARGET_NEON && reload_completed")
  (-1 "TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_FP16)
   && (	  s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode))")
  (-1 "TARGET_32BIT
   && (GET_CODE (operands[0]) == SYMBOL_REF)
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[0]))")
  (-1 "TARGET_32BIT || optimize_size || flag_pic")
  (-1 "TARGET_THUMB1 && reload_completed")
  (-1 "TARGET_32BIT
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)")
  (-1 "TARGET_THUMB2
   && peep2_regno_dead_p(0, CC_REGNUM)
   && ((rtx_equal_p(operands[0], operands[1])
	&& INTVAL(operands[2]) > -256 && INTVAL(operands[2]) < 256)
       || (INTVAL(operands[2]) > -8 && INTVAL(operands[2]) < 8))")
  (-1 "(TARGET_32BIT) && (TARGET_ARM
   && arm_arch4
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1]))))")
  (-1 "TARGET_THUMB2 && TARGET_VFP && TARGET_HARD_FLOAT
   && (   s_register_operand (operands[0], SImode)
       || s_register_operand (operands[1], SImode))")
  (-1 "TARGET_ARM && peep2_reg_dead_p (2, operands[0])")
  (-1 "TARGET_THUMB1
   && GET_CODE (XEXP (operands[4], 0)) == PLUS
   && rtx_equal_p (operands[0], XEXP (XEXP (operands[4], 0), 0))
   && rtx_equal_p (operands[2], XEXP (XEXP (operands[4], 0), 1))
   && (peep2_reg_dead_p (3, operands[0])
       || rtx_equal_p (operands[0], operands[3]))
   && (peep2_reg_dead_p (3, operands[2])
       || rtx_equal_p (operands[2], operands[3]))")
  (-1 "(TARGET_32BIT) && (TARGET_THUMB2
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && arm_arch3m && !arm_arch6)")
  (-1 "(TARGET_NEON && ((V2SFmode != V2SFmode && V2SFmode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V2SFmode == V4HImode && TARGET_REALLY_IWMMXT)")
  (-1 "TARGET_THUMB1 && satisfies_constraint_K (operands[1])")
  (-1 "TARGET_EITHER && !low_irq_latency")
  (-1 "TARGET_32BIT &&
   !(const_ok_for_arm (INTVAL (operands[2]))
     || const_ok_for_arm (-INTVAL (operands[2])))
    && const_ok_for_arm (~INTVAL (operands[2]))")
  (-1 "TARGET_32BIT
   && TARGET_USE_MOVT && GET_CODE (operands[1]) == SYMBOL_REF
   && !flag_pic && !target_word_relocations
   && !arm_tls_referenced_p (operands[1])")
  (-1 "TARGET_ARM && TARGET_VFP && TARGET_HARD_FLOAT
   && (   s_register_operand (operands[0], SImode)
       || s_register_operand (operands[1], SImode))")
  (-1 "TARGET_THUMB2 && reload_completed")
  (-1 "TARGET_32BIT || TARGET_THUMB1")
  (-1 "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE && reload_completed
   && arm_general_register_operand (operands[0], DFmode)")
  (-1 "TARGET_THUMB2 && flag_pic")
  (-1 "(TARGET_32BIT) && (TARGET_THUMB2 && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode)))")
  (-1 "TARGET_THUMB1
   && (GET_CODE (operands[4]) == EQ
       || GET_CODE (operands[4]) == NE
       || GET_CODE (operands[4]) == GE
       || GET_CODE (operands[4]) == LT)")
  (-1 "TARGET_32BIT
   && GET_CODE (operands[1]) == GET_CODE (operands[9])
   && INTVAL (operands[3]) == 32 - INTVAL (operands[6])")
  (-1 "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_FPA")
  (-1 "TARGET_THUMB2
   && TARGET_HARD_FLOAT && TARGET_MAVERICK
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode))")
  (-1 "(TARGET_NEON && ((V16QImode != V2SFmode && V16QImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V16QImode))")
  (-1 "TARGET_32BIT && arm_arch5")
  (-1 "TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8HImode))")
  (-1 "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP")
  (-1 "TARGET_ARM && arm_arch6 && optimize_size")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], V2DImode)
       || register_operand (operands[1], V2DImode))")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], TImode)
       || register_operand (operands[1], TImode))")
  (-1 "TARGET_32BIT && !arm_eliminable_register (operands[1])")
  (-1 "TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))")
  (-1 "(TARGET_32BIT) && (TARGET_ARM && !arm_arch6)")
  (-1 "TARGET_DSP_MULTIPLY")
  (-1 "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE")
  (-1 "TARGET_THUMB1
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode))")
  (-1 "TARGET_THUMB2
  && !(TARGET_HARD_FLOAT && (TARGET_MAVERICK || TARGET_VFP))
  && !TARGET_IWMMXT")
  (-1 "TARGET_THUMB2 && USE_RETURN_INSN (FALSE)")
  (-1 "TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SImode))")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], CImode)
       || register_operand (operands[1], CImode))")
  (-1 "TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON")
  (-1 "TARGET_ARM && !arm_arch5 && (!CONSTANT_ADDRESS_P (XEXP (operands[1], 0)))")
  (-1 "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_FPA
   && (register_operand (operands[0], XFmode)
       || register_operand (operands[1], XFmode))")
  (-1 "TARGET_ARM && load_multiple_sequence (operands, 2, NULL, NULL, NULL)")
  (-1 "TARGET_SOFT_TP")
  (-1 "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP
   && (   s_register_operand (operands[0], SFmode)
       || s_register_operand (operands[1], SFmode))")
  (-1 "(TARGET_32BIT) && (TARGET_ARM && arm_arch6)")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], EImode)
       || register_operand (operands[1], EImode))")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], V2SImode)
       || register_operand (operands[1], V2SImode))")
  (-1 "!TARGET_THUMB2 && !arm_arch6")
  (-1 "TARGET_THUMB2
   && peep2_regno_dead_p(0, CC_REGNUM)")
  (-1 "(TARGET_NEON && ((V8HImode != V2SFmode && V8HImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8HImode))")
  (-1 "TARGET_32BIT
  && (!(const_ok_for_arm (INTVAL (operands[1]))
        || const_ok_for_arm (~INTVAL (operands[1]))))")
  (-1 "TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode))")
  (-1 "(TARGET_32BIT) && (TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))")
  (-1 "TARGET_32BIT && peep2_reg_dead_p (3, operands[2])")
  (-1 "TARGET_ARM
   && GET_CODE (operands[2]) == CONST_INT
   && !const_ok_for_arm (INTVAL (operands[2]))")
  (-1 "TARGET_THUMB1 && !arm_arch5")
  (-1 "TARGET_ARM && store_multiple_sequence (operands, 4, NULL, NULL, NULL)")
  (-1 "(TARGET_32BIT) && (TARGET_HARD_TP)")
  (-1 "TARGET_ARM && store_multiple_sequence (operands, 3, NULL, NULL, NULL)")
  (-1 "(TARGET_32BIT) && (TARGET_ARM && adjacent_mem_locations (operands[2], operands[3]))")
  (-1 "(TARGET_32BIT) && (TARGET_ARM && arm_arch4 && !arm_arch6)")
  (-1 "TARGET_THUMB2")
  (-1 "TARGET_32BIT && GET_CODE (operands[0]) == SYMBOL_REF")
  (-1 "TARGET_32BIT && !TARGET_NEON")
  (-1 "TARGET_ARM && USE_RETURN_INSN (TRUE)")
  (-1 "TARGET_NEON && !BYTES_BIG_ENDIAN")
  (-1 "TARGET_ARM && !reg_overlap_mentioned_p (operands[0], operands[3])")
  (-1 "(TARGET_32BIT) && (TARGET_THUMB2 && arm_arch_hwdiv)")
  (-1 "TARGET_EITHER
   && reload_completed
   && GET_CODE (operands[1]) == CONST_DOUBLE")
  (-1 "(TARGET_NEON && ((V4SFmode != V2SFmode && V4SFmode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V4SFmode == V4HImode && TARGET_REALLY_IWMMXT)")
  (-1 "TARGET_EITHER && (arm_arch6 || !optimize_size)")
  (-1 "TARGET_THUMB1
   && !(TARGET_HARD_FLOAT && TARGET_MAVERICK)
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode))")
  (-1 "TARGET_THUMB2
   && peep2_regno_dead_p(0, CC_REGNUM)
   && (unsigned HOST_WIDE_INT) INTVAL(operands[1]) < 256")
  (-1 "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FPA")
  (-1 "TARGET_THUMB1 && arm_arch6")
  (-1 "TARGET_THUMB2 && TARGET_SOFT_FLOAT")
  (-1 "(TARGET_32BIT) && (TARGET_ARM && arm_arch4)")
  (-1 "(TARGET_NEON && ((V2SImode != V2SFmode && V2SImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V2SImode == V4HImode && TARGET_REALLY_IWMMXT)")
  (-1 "TARGET_32BIT && arm_arch5e")
  (-1 "(TARGET_32BIT) && (TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode)))")
  (-1 "TARGET_ARM && arm_arch5")
  (-1 "(TARGET_ARM) && ( reload_completed)")
  (-1 "TARGET_THUMB1
   && (   register_operand (operands[0], SImode) 
       || register_operand (operands[1], SImode))")
  (-1 "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode))")
  (-1 "(TARGET_32BIT) && (TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP
   && (   s_register_operand (operands[0], SFmode)
       || s_register_operand (operands[1], SFmode)))")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], V16QImode)
       || register_operand (operands[1], V16QImode))")
  (-1 "TARGET_32BIT && arm_arch4")
  (-1 "(TARGET_32BIT) && (TARGET_ARM && TARGET_VFP && TARGET_HARD_FLOAT
   && (   s_register_operand (operands[0], SImode)
       || s_register_operand (operands[1], SImode)))")
  (1 "TARGET_EITHER")
  (-1 "(TARGET_32BIT) && (TARGET_ARM && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode)))")
  (-1 "TARGET_HAVE_MEMORY_BARRIER")
  (-1 "TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_MAVERICK)")
  (-1 "TARGET_32BIT && reload_completed
   && ! (TARGET_NEON && IS_VFP_REGNUM (REGNO (operands[0])))
   && ! IS_IWMMXT_REGNUM (REGNO (operands[0]))")
  (-1 "TARGET_EITHER && reload_completed")
  (-1 "TARGET_32BIT
   && reload_completed
   && (arm_const_double_inline_cost (operands[1])
       <= ((optimize_size || arm_ld_sched) ? 3 : 4))")
  (-1 "TARGET_HAVE_LDREXBHD && TARGET_HAVE_MEMORY_BARRIER")
  (-1 "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE")
  (-1 "(TARGET_NEON && ((V2SImode != V2SFmode && V2SImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SImode))")
  (-1 "TARGET_THUMB2 && optimize_size && peep2_regno_dead_p (0, CC_REGNUM)")
  (-1 "TARGET_THUMB2
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))")
  (-1 "!arm_arch6")
  (-1 "TARGET_ARM && arm_arch4 && !arm_arch6")
  (-1 "(TARGET_32BIT) && (TARGET_ARM)")
  (-1 "TARGET_ARM && reload_completed")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FPA)")
  (-1 "TARGET_ARM && !arm_arch6")
  (-1 "TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_MAVERICK
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode))")
  (-1 "TARGET_32BIT && TARGET_HARD_FLOAT && !TARGET_VFP_SINGLE")
  (-1 "(TARGET_THUMB1) && ( reload_completed && CONST_INT_P (operands[2])
   && operands[1] != stack_pointer_rtx
   && (INTVAL (operands[2]) > 255 || INTVAL (operands[2]) < -255))")
  (-1 "TARGET_THUMB1
   && (GET_CODE (operands[3]) == EQ
       || GET_CODE (operands[3]) == NE
       || GET_CODE (operands[3]) == GE
       || GET_CODE (operands[3]) == LT)")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && XVECLEN (operands[0], 0) == 5)")
  (-1 "TARGET_THUMB2 && reload_completed
   && GET_CODE(operands[3]) != PLUS
   && GET_CODE(operands[3]) != MINUS")
  (-1 "flag_pic")
  (-1 "TARGET_32BIT && TARGET_HARD_FLOAT && (TARGET_FPA || TARGET_VFP)")
  (-1 "(TARGET_32BIT) && (TARGET_INT_SIMD)")
  (-1 "(TARGET_NEON && ((V4SImode != V2SFmode && V4SImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SImode))")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && XVECLEN (operands[0], 0) == 3)")
  (-1 "TARGET_32BIT && arm_arch_thumb2")
  (-1 "TARGET_THUMB2
   && GET_CODE (operands[2]) == CONST_INT
   && !(const_ok_for_arm (INTVAL (operands[2]))
	|| const_ok_for_arm (~INTVAL (operands[2])))")
  (-1 "TARGET_ARM && arm_arch4")
  (-1 "TARGET_THUMB1
   && GET_CODE (operands[1]) == SYMBOL_REF
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[1]))")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && arm_arch6)")
  (-1 "TARGET_32BIT && XVECLEN (operands[0], 0) == 4")
  (-1 "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_MAVERICK
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))")
  (0 "TARGET_VXWORKS_RTP && TARGET_ARM && flag_pic")
  (-1 "TARGET_REALLY_IWMMXT
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode))")
  (-1 "(TARGET_THUMB1) && ( reload_completed)")
  (-1 "TARGET_THUMB2 && !flag_pic")
  (-1 "TARGET_ARM && ! TARGET_IWMMXT
   && !(TARGET_HARD_FLOAT && TARGET_VFP)
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode))")
  (-1 "(TARGET_32BIT) && ( reload_completed)")
  (-1 "TARGET_32BIT
   && GET_CODE (operands[2]) == CONST_INT
   && !(const_ok_for_arm (INTVAL (operands[2]))
	|| const_ok_for_arm (~INTVAL (operands[2])))")
  (-1 "TARGET_THUMB2 && optimize_size")
  (-1 "TARGET_32BIT && !arm_arch6")
  (-1 "(TARGET_32BIT) && (TARGET_THUMB2 && TARGET_VFP && TARGET_HARD_FLOAT
   && (   s_register_operand (operands[0], SImode)
       || s_register_operand (operands[1], SImode)))")
  (-1 "TARGET_THUMB1
   && (   register_operand (operands[0], HImode)
       || register_operand (operands[1], HImode))")
  (-1 "TARGET_ARM
   && !const_ok_for_arm (INTVAL (operands[2]))
   && const_ok_for_arm (~INTVAL (operands[2]))")
  (-1 "TARGET_32BIT")
  (-1 "TARGET_32BIT && arm_arch6")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], V4SFmode)
       || register_operand (operands[1], V4SFmode))")
  (-1 "TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))")
  (-1 "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_FPA && reload_completed")
  (-1 "TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V16QImode))")
  (-1 "TARGET_ARM
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_OR_Y)
       != CCmode)")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], V4HImode)
       || register_operand (operands[1], V4HImode))")
  (-1 "TARGET_32BIT
   && ((unsigned HOST_WIDE_INT) INTVAL (operands[3])
       == (GET_MODE_MASK (GET_MODE (operands[5]))
           & (GET_MODE_MASK (GET_MODE (operands[5]))
	      << (INTVAL (operands[2])))))")
  (-1 "TARGET_INT_SIMD")
  (-1 "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE")
  (-1 "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP && arm_tune != cortexa8
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode))")
  (0 "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_MAVERICK && 0")
  (-1 "TARGET_THUMB2 && optimize_size && reload_completed")
  (-1 "(TARGET_32BIT) && (TARGET_THUMB2
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))")
  (-1 "TARGET_32BIT
   && GET_CODE (operands[2]) == CONST_INT
   && !(const_ok_for_arm (INTVAL (operands[2]))
        || const_ok_for_arm (-INTVAL (operands[2])))
   && (reload_completed || !arm_eliminable_register (operands[1]))")
  (-1 "(TARGET_THUMB2) && ( reload_completed)")
  (-1 "(TARGET_NEON && ((V8QImode != V2SFmode && V8QImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V8QImode == V4HImode && TARGET_REALLY_IWMMXT)")
  (-1 "(TARGET_NEON && ((V4SImode != V2SFmode && V4SImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V4SImode == V4HImode && TARGET_REALLY_IWMMXT)")
  (-1 "TARGET_THUMB1 && flag_pic")
  (-1 "(TARGET_32BIT) && ( ((GET_CODE (operands[1]) == CONST_INT
       	&& !const_ok_for_arm (INTVAL (operands[1])))
       || (GET_CODE (operands[2]) == CONST_INT
	   && !const_ok_for_arm (INTVAL (operands[2])))))")
  (-1 "TARGET_32BIT
   && reload_completed
   && operands[0] != operands[1]")
  (-1 "TARGET_THUMB2
   && peep2_regno_dead_p(0, CC_REGNUM)
   && ((GET_CODE(operands[3]) != ROTATE && GET_CODE(operands[3]) != ROTATERT)
       || REG_P(operands[2]))")
  (-1 "TARGET_32BIT && GET_CODE (operands[1]) == SYMBOL_REF")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP_DOUBLE)")
  (-1 "(TARGET_32BIT) && (TARGET_THUMB2 && optimize_size && reload_completed)")
  (-1 "TARGET_LDRD
  && reg_overlap_mentioned_p (operands[0], operands[1])
  && reg_overlap_mentioned_p (operands[0], operands[2])")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && XVECLEN (operands[0], 0) == 4)")
  (-1 "TARGET_NEON && (!false || flag_unsafe_math_optimizations)")
  (-1 "TARGET_ARM
   && arm_arch4
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1])))")
  (-1 "TARGET_THUMB2
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode))")
  (-1 "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FP16 && !TARGET_NEON_FP16
   && (   s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode))")
  (-1 "TARGET_ARM && TARGET_SOFT_FLOAT")
  (-1 "TARGET_ARM
   && !(TARGET_HARD_FLOAT && (TARGET_MAVERICK || TARGET_VFP))
   && !TARGET_IWMMXT
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode))")
  (-1 "TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4HImode))")
  (-1 "TARGET_ARM || arm_arch_thumb2")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && !(TARGET_HARD_FLOAT && TARGET_FP16)
   && (	  s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode)))")
  (-1 "TARGET_ARM && load_multiple_sequence (operands, 3, NULL, NULL, NULL)")
  (-1 "TARGET_ARM")
  (-1 "TARGET_ARM && adjacent_mem_locations (operands[2], operands[3])")
  (-1 "(TARGET_32BIT) && (TARGET_ARM && TARGET_HARD_FLOAT && TARGET_FPA
   && (register_operand (operands[0], XFmode)
       || register_operand (operands[1], XFmode)))")
  (-1 "TARGET_THUMB2
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))")
  (-1 "(TARGET_NEON && ((V4HImode != V2SFmode && V4HImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4HImode))")
  (-1 "TARGET_THUMB1
   && (GET_CODE (operands[0]) == EQ
       || GET_CODE (operands[0]) == NE
       || GET_CODE (operands[0]) == GE
       || GET_CODE (operands[0]) == LT)")
  (-1 "TARGET_ARM
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)
       != CCmode)")
  (-1 "TARGET_32BIT && INTVAL (operands[2]) == -INTVAL (operands[3])")
  (-1 "TARGET_32BIT
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode))")
  (-1 "TARGET_HARD_TP")
  (-1 "TARGET_ARM && reload_completed
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)
       != CCmode)")
  (-1 "TARGET_THUMB1
   && (	  s_register_operand (operands[0], HFmode) 
       || s_register_operand (operands[1], HFmode))")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], V4SImode)
       || register_operand (operands[1], V4SImode))")
  (-1 "TARGET_ARM
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)
   && !reg_overlap_mentioned_p (operands[0], operands[4])")
  (-1 "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP")
  (-1 "TARGET_32BIT && TARGET_HARD_FLOAT")
  (-1 "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_MAVERICK")
  (-1 "(TARGET_32BIT) && (TARGET_THUMB2 && reload_completed)")
  (-1 "TARGET_ARM && reload_completed
   && arm_const_double_by_parts (operands[1])")
  (-1 "TARGET_THUMB1
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode))")
  (-1 "TARGET_32BIT && TARGET_HARD_FLOAT && (TARGET_FPA || TARGET_VFP_DOUBLE)")
  (-1 "TARGET_ARM
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))")
  (-1 "(TARGET_NEON && ((V4SFmode != V2SFmode && V4SFmode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SFmode))")
  (-1 "(TARGET_NEON && ((V2DImode != V2SFmode && V2DImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2DImode))")
  (-1 "TARGET_ARM && !arm_arch5")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && XVECLEN (operands[0], 0) == 2)")
  (-1 "(TARGET_32BIT) && (arm_arch_thumb2)")
  (-1 "TARGET_THUMB2 && arm_arch6")
  (-1 "TARGET_ARM && load_multiple_sequence (operands, 4, NULL, NULL, NULL)")
  (-1 "TARGET_32BIT && operands[0] != operands[1] && reload_completed")
  (-1 "(TARGET_32BIT) && (TARGET_ARM && USE_RETURN_INSN (FALSE))")
  (-1 "TARGET_32BIT && reload_completed")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], V8QImode)
       || register_operand (operands[1], V8QImode))")
  (-1 "TARGET_32BIT && flag_pic")
  (-1 "TARGET_NEON && flag_unsafe_math_optimizations")
  (-1 "(TARGET_32BIT) && (TARGET_NEON)")
  (-1 "TARGET_ARM
   && (((unsigned HOST_WIDE_INT) INTVAL (operands[1]))
       == (((unsigned HOST_WIDE_INT) INTVAL (operands[1])) >> 24) << 24)")
  (-1 "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode))")
  (-1 "TARGET_THUMB1
   && GET_CODE (operands[0]) == SYMBOL_REF
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[0]))")
  (-1 "TARGET_THUMB2 && TARGET_SOFT_FLOAT
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode))")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], V8HImode)
       || register_operand (operands[1], V8HImode))")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], XImode)
       || register_operand (operands[1], XImode))")
  (-1 "TARGET_32BIT && arm_arch3m && !arm_arch6")
  (-1 "(TARGET_NEON && ((V8QImode != V2SFmode && V8QImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8QImode))")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && !arm_arch6)")
  (0 "0 && TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_MAVERICK")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && arm_arch_thumb2)")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP)")
  (-1 "(TARGET_32BIT) && (TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP && arm_tune == cortexa8
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode)))")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_FP16)")
  (-1 "TARGET_32BIT && (GET_CODE (operands[1]) != MEM) && ! BYTES_BIG_ENDIAN")
  (-1 "TARGET_THUMB1 || TARGET_32BIT")
  (-1 "TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2SFmode))")
  (-1 "TARGET_32BIT && arm_arch3m")
  (-1 "(TARGET_NEON && ((V16QImode != V2SFmode && V16QImode != V4SFmode)
		    || flag_unsafe_math_optimizations))
   || (V16QImode == V4HImode && TARGET_REALLY_IWMMXT)")
  (-1 "(TARGET_32BIT) && (TARGET_DSP_MULTIPLY)")
  (-1 "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_NEON_FP16
   && (   s_register_operand (operands[0], HFmode)
       || s_register_operand (operands[1], HFmode))")
  (-1 "TARGET_THUMB1 && !arm_arch6")
  (-1 "TARGET_ARM
   && (arm_select_dominance_cc_mode (operands[3], operands[6], DOM_CC_X_AND_Y)
       == CCmode)")
  (-1 "(TARGET_32BIT) && (TARGET_THUMB2)")
  (-1 "TARGET_32BIT && XVECLEN (operands[0], 0) == 5")
  (-1 "TARGET_NEON")
  (-1 "TARGET_THUMB1
   && (unsigned HOST_WIDE_INT) (INTVAL (operands[1])) < 1024
   && (INTVAL (operands[1]) & 3) == 0")
  (-1 "TARGET_THUMB1")
  (-1 "TARGET_ARM && store_multiple_sequence (operands, 2, NULL, NULL, NULL)")
  (-1 "TARGET_32BIT && TARGET_HARD_FLOAT && TARGET_VFP")
  (-1 "TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V4SFmode))")
  (-1 "TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V8QImode))")
  (-1 "TARGET_THUMB1
   && (   register_operand (operands[0], SFmode) 
       || register_operand (operands[1], SFmode))")
  (-1 "TARGET_ARM && TARGET_SOFT_FLOAT
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode))")
  (-1 "TARGET_ARM && arm_arch6")
  (-1 "TARGET_THUMB2 && reload_completed
   && ((GET_CODE(operands[3]) != ROTATE && GET_CODE(operands[3]) != ROTATERT)
       || REG_P(operands[2]))")
  (-1 "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_MAVERICK")
  (-1 "(TARGET_32BIT) && (TARGET_ARM
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))")
  (-1 "(TARGET_32BIT) && (TARGET_32BIT && !TARGET_IWMMXT && !TARGET_NEON)")
  (-1 "TARGET_NEON
   && (register_operand (operands[0], OImode)
       || register_operand (operands[1], OImode))")
  (-1 "TARGET_THUMB1 && arm_arch5")
  (-1 "TARGET_THUMB2 && TARGET_HARD_FLOAT && TARGET_MAVERICK")
  (-1 "(TARGET_32BIT) && (TARGET_REALLY_IWMMXT)")
  (-1 "TARGET_THUMB")
  (-1 "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_VFP && arm_tune == cortexa8
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode))")
  (-1 "TARGET_NEON
   || (TARGET_REALLY_IWMMXT && VALID_IWMMXT_REG_MODE (V2DImode))")
  (-1 "TARGET_ARM && TARGET_HARD_FLOAT && TARGET_MAVERICK
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))")
  (-1 "TARGET_32BIT
   && (GET_CODE (operands[1]) == SYMBOL_REF)
   && !arm_is_long_call_p (SYMBOL_REF_DECL (operands[1]))")
  (-1 "(TARGET_32BIT) && (TARGET_ARM
   && TARGET_HARD_FLOAT && TARGET_FPA
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))")
  (-1 "TARGET_HAVE_LDREX && TARGET_HAVE_MEMORY_BARRIER")
])

; GCC machine description for Ubicom32
;
; Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009 Free Software
; Foundation, Inc.
; Contributed by Ubicom, Inc.
;
; This file is part of GCC.
;
; GCC is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 3, or (at your option)
; any later version.
;
; GCC is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with GCC; see the file COPYING3.  If not see
; <http://www.gnu.org/licenses/>.

(define_constants
  [(AUX_DATA_REGNO 15)
   (LINK_REGNO     21)
   (SP_REGNO       23)
   (ACC0_HI_REGNO  24)
   (ACC1_HI_REGNO  26)
   (CC_REGNO       30)])

(define_constants
  [(UNSPEC_FDPIC_GOT 0)
   (UNSPEC_FDPIC_GOT_FUNCDESC 1)])

(define_constants
  [(UNSPEC_VOLATILE_LOAD_FDPIC_FUNCDESC 0)])

;; Types of instructions (for scheduling purposes).

(define_attr "type" "mul,addr,other"
  (const_string "other"))

; Define instruction scheduling characteristics.  We can only issue
; one instruction per clock so we don't need to define CPU units.
;
(define_automaton "ubicom32")

(define_cpu_unit "i_pipeline" "ubicom32");

; We have a 4 cycle hazard associated with address calculations which
; seems rather tricky to avoid so we go with a defensive assumption
; that almost anything can be used to generate addresses.
;
;(define_insn_reservation "ubicom32_other" 4
;			 (eq_attr "type" "other")
;			 "i_pipeline")

; Some moves don't generate hazards.
;
;(define_insn_reservation "ubicom32_addr" 1
;			 (eq_attr "type" "addr")
;			 "i_pipeline")

; We need 3 cycles between a multiply instruction and any use of the
; matching accumulator register(s).
;
(define_insn_reservation "ubicom32_mul" 4
			 (eq_attr "type" "mul")
			 "i_pipeline")

(define_attr "length" ""
  (const_int 4))

(include "predicates.md")
(include "constraints.md")

; 8-bit move with no change to the flags reg.
;
(define_insn "movqi"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=rm")
	(match_operand:QI 1 "ubicom32_move_operand"  "g"))]
  ""
  "move.1\\t%0, %1")

; Combiner-generated 8-bit move with the zero flag set accordingly.
;
(define_insn "movqi_ccszn"
  [(set (reg CC_REGNO)
	(compare (match_operand:QI 0 "nonimmediate_operand" "rm")
		 (const_int 0)))
   (set (match_operand:QI 1 "nonimmediate_operand"	   "=rm")
	(match_dup 0))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "ext.1\\t%1, %0")

; Combine isn't very good at merging some types of operations so we
; have to make do with a peephole.  It's not as effective but it's better
; than doing nothing.
;
(define_peephole2
  [(set (match_operand:QI 0 "nonimmediate_operand" "")
	(match_operand:QI 1 "nonimmediate_operand" ""))
   (set (match_operand 2 "ubicom32_cc_register_operand" "")
	(match_operator 3 "ubicom32_compare_operator"
	  [(match_dup 0)
	   (const_int 0)]))]
  "(GET_MODE (operands[2]) == CCSZNmode
    || GET_MODE (operands[2]) == CCSZmode)"
  [(parallel
     [(set (match_dup 2)
	   (match_op_dup 3
	     [(match_dup 1)
	      (const_int 0)]))
      (set (match_dup 0)
	   (match_dup 1))])]
   "")

; Combine isn't very good at merging some types of operations so we
; have to make do with a peephole.  It's not as effective but it's better
; than doing nothing.
;
(define_peephole2
  [(set (match_operand:QI 0 "nonimmediate_operand" "")
	(match_operand:QI 1 "nonimmediate_operand" ""))
   (set (match_operand 2 "ubicom32_cc_register_operand" "")
	(match_operator 3 "ubicom32_compare_operator"
	  [(match_dup 1)
	   (const_int 0)]))]
  "(GET_MODE (operands[2]) == CCSZNmode
    || GET_MODE (operands[2]) == CCSZmode)"
  [(parallel
     [(set (match_dup 2)
	   (match_op_dup 3
	     [(match_dup 1)
	      (const_int 0)]))
      (set (match_dup 0)
	   (match_dup 1))])]
   "")

; 16-bit move with no change to the flags reg.
;
(define_insn "movhi"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=rm")
	(match_operand:HI 1 "ubicom32_move_operand"  "g"))]
  ""
  "*
   {
     if (CONST_INT_P (operands[1]))
       return \"movei\\t%0, %1\";

     return \"move.2\\t%0, %1\";
   }")

; Combiner-generated 16-bit move with the zero flag set accordingly.
;
(define_insn "movhi_ccszn"
  [(set (reg CC_REGNO)
	(compare (match_operand:HI 0 "nonimmediate_operand" "rm")
		 (const_int 0)))
   (set (match_operand:HI 1 "nonimmediate_operand"	   "=rm")
	(match_dup 0))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "ext.2\\t%1, %0")

; Combine isn't very good at merging some types of operations so we
; have to make do with a peephole.  It's not as effective but it's better
; than doing nothing.
;
(define_peephole2
  [(set (match_operand:HI 0 "nonimmediate_operand" "")
	(match_operand:HI 1 "nonimmediate_operand" ""))
   (set (match_operand 2 "ubicom32_cc_register_operand" "")
	(match_operator 3 "ubicom32_compare_operator"
	  [(match_dup 0)
	   (const_int 0)]))]
  "(GET_MODE (operands[2]) == CCSZNmode
    || GET_MODE (operands[2]) == CCSZmode)"
  [(parallel
     [(set (match_dup 2)
	   (match_op_dup 3
	     [(match_dup 1)
	      (const_int 0)]))
      (set (match_dup 0)
	   (match_dup 1))])]
   "")

; Combine isn't very good at merging some types of operations so we
; have to make do with a peephole.  It's not as effective but it's better
; than doing nothing.
;
(define_peephole2
  [(set (match_operand:HI 0 "nonimmediate_operand" "")
	(match_operand:HI 1 "nonimmediate_operand" ""))
   (set (match_operand 2 "ubicom32_cc_register_operand" "")
	(match_operator 3 "ubicom32_compare_operator"
	  [(match_dup 1)
	   (const_int 0)]))]
  "(GET_MODE (operands[2]) == CCSZNmode
    || GET_MODE (operands[2]) == CCSZmode)"
  [(parallel
     [(set (match_dup 2)
	   (match_op_dup 3
	     [(match_dup 1)
	      (const_int 0)]))
      (set (match_dup 0)
	   (match_dup 1))])]
   "")

; 32-bit move with no change to the flags reg.
;
(define_expand "movsi"
  [(set (match_operand:SI 0 "nonimmediate_operand" "")
	(match_operand:SI 1 "general_operand" ""))]
  ""
  "{
     /* Convert any complexities in operand 1 into something that can just
        fall into the default expander code.  */
     ubicom32_expand_movsi (operands);
   }")

(define_insn "movsi_high"
  [(set (match_operand:SI 0 "ubicom32_address_register_operand"		"=a")
	(high:SI (match_operand:SI 1 "ubicom32_symbolic_address_operand" "s")))]
  ""
  "moveai\\t%0, #%%hi(%E1)")

(define_insn "movsi_lo_sum"
  [(set (match_operand:SI 0 "nonimmediate_operand"			 "=rm")
	(lo_sum:SI (match_operand:SI 1 "ubicom32_address_register_operand" "a")
                   (match_operand:SI 2 "immediate_operand"		   "s")))]
  ""
  "lea.1\\t%0, %%lo(%E2)(%1)")

(define_insn "movsi_internal"
  [(set (match_operand:SI 0 "nonimmediate_operand"   "=rm")
	(match_operand:SI 1 "ubicom32_move_operand" "rmnY"))]
  ""
  "*
   {
     if (CONST_INT_P (operands[1]))
       {
         ubicom32_emit_move_const_int (operands[0], operands[1]);
         return \"\";
       }

     if (GET_CODE (operands[1]) == CONST_DOUBLE)
       {
         HOST_WIDE_INT i = CONST_DOUBLE_LOW (operands[1]);

         ubicom32_emit_move_const_int (operands[0], GEN_INT (i));
         return \"\";
       }

     if (ubicom32_address_register_operand (operands[0], VOIDmode)
	 && register_operand (operands[1], VOIDmode))
       {
	 if (ubicom32_address_register_operand (operands[1], VOIDmode))
	   return \"lea.1\\t%0, 0(%1)\";

	 /* Use movea here to utilize the hazard bypass in the >= v4 ISA.  */
         if (ubicom32_v4)
	   return \"movea\\t%0, %1\";

         return \"move.4\\t%0, %1\";
       }

     return \"move.4\\t%0, %1\";
   }")

; If we're not dependent on the state of the condition codes we can construct
; constants of value 2^n by using a bset.
;
(define_peephole2
  [(set (match_operand:SI 0 "nonimmediate_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "(exact_log2 (INTVAL (operands[1])) > 14
    && peep2_regno_dead_p (0, CC_REGNO))"
  [(parallel
     [(set (match_dup 0)
	   (ior:SI (const_int 0)
		   (match_dup 1)))
      (clobber (reg:CC CC_REGNO))])]
  "")

; If we're not dependent on the state of the condition codes we can construct
; constants of value ~(2^n) by using a bclr.
;
(define_peephole2
  [(set (match_operand:SI 0 "nonimmediate_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "(exact_log2 (~INTVAL (operands[1])) > 14
    && peep2_regno_dead_p (0, CC_REGNO))"
  [(parallel
     [(set (match_dup 0)
	   (and:SI (const_int -1)
		   (match_dup 1)))
      (clobber (reg:CC CC_REGNO))])]
  "")

; For 32-bit constants that have bits 0 through 24 and bit 31 set the same
; we can use swapb.4!
;
(define_peephole2
  [(set (match_operand:SI 0 "nonimmediate_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "(ubicom32_v4
    && (INTVAL (operands[1]) & 0xffffffff) != 0xffffffff
    && (INTVAL (operands[1]) & 0xffffffff) != 0
    && ((INTVAL (operands[1]) & 0x80ffffff) == 0
	|| (INTVAL (operands[1]) & 0x80ffffff) == 0x80ffffff))"
  [(set (match_dup 0)
	(bswap:SI (match_dup 2)))]
  "{
     operands[2] = GEN_INT (INTVAL (operands[1]) >> 24);
   }")

; If this is a write of a constant to memory look to see if we can usefully
; transform this into 2 smaller writes.
;
(define_peephole2
  [(set (match_operand:SI 0 "memory_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "! satisfies_constraint_I (operands[1])
   && ubicom32_legitimate_address_p (HImode, plus_constant (XEXP (operands[0], 0), 2), 1)"
  [(set (match_dup 4) (match_dup 2))
   (set (match_dup 5) (match_dup 3))]
  "{
     rtx low_hword_addr;

     operands[2] = gen_highpart_mode (HImode, SImode, operands[1]);
     operands[3] = gen_lowpart (HImode, operands[1]);

     operands[4] = gen_rtx_MEM (HImode, XEXP (operands[0], 0));
     MEM_COPY_ATTRIBUTES (operands[4], operands[0]);

     low_hword_addr = plus_constant (XEXP (operands[0], 0), 2);
     operands[5] = gen_rtx_MEM (HImode, low_hword_addr);
     MEM_COPY_ATTRIBUTES (operands[5], operands[0]);
   }")

; If we're writing memory and we've not found a better way to do this then
; try loading into a D register and then copying to memory.  This will
; perform the fewest possible memory read/writes.
;
(define_peephole2
  [(match_scratch:SI 2 "d")
   (set (match_operand:SI 0 "memory_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "! satisfies_constraint_I (operands[1])"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 0) (match_dup 2))]
  "")

; If we're not dependent on the state of the condition codes we can construct
; constants of value (2^n - 1) by using an lsr.4.
;
(define_peephole2
  [(set (match_operand:SI 0 "ubicom32_data_register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "(exact_log2 (INTVAL (operands[1]) + 1) > 14
    && peep2_regno_dead_p (0, CC_REGNO))"
  [(parallel
     [(set (match_dup 0)
	   (lshiftrt:SI (const_int -1)
			(match_dup 2)))
      (clobber (reg:CC CC_REGNO))])]
  "{
     operands[2] = GEN_INT (32 - exact_log2 (INTVAL (operands[1]) + 1));
   }")

; If we're not dependent on the state of the condition codes we can construct
; constants of value (2^n - 1) by using an lsr.4.
;
(define_peephole2
  [(match_scratch:SI 2 "d")
   (set (match_operand:SI 0 "nonimmediate_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "(exact_log2 (INTVAL (operands[1]) + 1) > 14
    && peep2_regno_dead_p (0, CC_REGNO))"
  [(parallel
     [(set (match_dup 2)
	   (lshiftrt:SI (const_int -1)
			(match_dup 3)))
      (clobber (reg:CC CC_REGNO))])
   (set (match_dup 0)
	(match_dup 2))]
  "{
     operands[3] = GEN_INT (32 - exact_log2 (INTVAL (operands[1]) + 1));
   }")

; If we're not dependent on the state of the condition codes we can construct
; some other constants by using an lsl.4 to shift 7 bits left by some
; constant.
;
(define_peephole2
  [(set (match_operand:SI 0 "ubicom32_data_register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "(ubicom32_shiftable_const_int (INTVAL (operands[1]))
    && peep2_regno_dead_p (0, CC_REGNO))"
  [(parallel
     [(set (match_dup 0)
	   (ashift:SI (match_dup 2)
		      (match_dup 3)))
      (clobber (reg:CC CC_REGNO))])]
  "{
     int shift = ubicom32_shiftable_const_int (INTVAL (operands[1]));
     operands[2] = GEN_INT (INTVAL (operands[1]) >> shift);
     operands[3] = GEN_INT (shift);
   }")

; If we're not dependent on the state of the condition codes we can construct
; some other constants by using an lsl.4 to shift 7 bits left by some
; constant.
;
(define_peephole2
  [(match_scratch:SI 2 "d")
   (set (match_operand:SI 0 "nonimmediate_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "(ubicom32_shiftable_const_int (INTVAL (operands[1]))
    && peep2_regno_dead_p (0, CC_REGNO))"
  [(parallel
     [(set (match_dup 2)
	   (ashift:SI (match_dup 3)
		      (match_dup 4)))
      (clobber (reg:CC CC_REGNO))])
   (set (match_dup 0)
	(match_dup 2))]
  "{
     int shift = ubicom32_shiftable_const_int (INTVAL (operands[1]));
     operands[3] = GEN_INT (INTVAL (operands[1]) >> shift);
     operands[4] = GEN_INT (shift);
   }")

; For some 16-bit unsigned constants that have bit 15 set we can use
; swapb.2!
;
; Note that the movsi code emits the same sequence but by using a peephole2
; we split the pattern early enough to allow instruction scheduling to
; occur.
;
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "(ubicom32_v4
    && (INTVAL (operands[1]) & 0xffff80ff) == 0x80ff)"
  [(set (match_dup 0)
	(zero_extend:SI (bswap:HI (match_dup 2))))]
  "{
     HOST_WIDE_INT i = INTVAL (operands[1]) >> 8;
     if (i >= 0x80)
       i -= 0x100;
     operands[2] = GEN_INT (i);
   }")

; In general for a 16-bit unsigned constant that has bit 15 set
; then we need a movei/move.2 pair unless we can represent it
; via just a move.2.
;
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "(INTVAL (operands[1]) & 0xffff8000) == 0x8000
    && (INTVAL (operands[1]) & 0xffff) < 0xff80"
  [(set (match_dup 2)
	(match_dup 1))
   (set (match_dup 0)
	(zero_extend:SI (match_dup 2)))]
  "{
     operands[2] = gen_rtx_REG (HImode, REGNO (operands[0]));
   }")

; If we're not dependent on the state of the condition codes we can construct
; 32-bit constants that have bits 16 through 31 set to arbitrary values
; and have bits 0 through 15 set to something representable as a default
; source-1 immediate - we use movei/shmrg.2
;
(define_peephole2
  [(set (match_operand:SI 0 "ubicom32_data_register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "(((INTVAL (operands[1]) >= 0x8000
      && INTVAL (operands[1]) < 0xff80)
     || INTVAL (operands[1]) >= 0x10000
     || INTVAL (operands[1]) < -0x8000)
    && ((INTVAL (operands[1]) & 0xffff) >= 0xff80
	|| (INTVAL (operands[1]) & 0xffff) < 0x80)
    && peep2_regno_dead_p (0, CC_REGNO))"
  [(set (match_dup 0)
	(match_dup 2))
   (parallel
     [(set (match_dup 0)
	   (ior:SI
	     (ashift:SI (match_dup 0)
			(const_int 16))
	     (zero_extend:SI
	       (match_dup 3))))
      (clobber (reg:CC CC_REGNO))])]
  "{
     operands[2] = gen_highpart_mode (HImode, SImode, operands[1]);
     operands[3] = gen_lowpart (HImode, operands[1]);
   }")

; Exactly the same as the peephole2 preceding except that this targets a
; general register instead of D register.  Hopefully the later optimization
; passes will notice that the value ended up in a D register first here
; and eliminate away the other register!
;
(define_peephole2
  [(match_scratch:SI 2 "d")
   (set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "(((INTVAL (operands[1]) >= 0x8000
      && INTVAL (operands[1]) < 0xff80)
     || INTVAL (operands[1]) >= 0x10000
     || INTVAL (operands[1]) < -0x8000)
    && ((INTVAL (operands[1]) & 0xffff) >= 0xff80
	|| (INTVAL (operands[1]) & 0xffff) < 0x80)
    && peep2_regno_dead_p (0, CC_REGNO))"
  [(set (match_dup 2)
	(match_dup 3))
   (parallel
     [(set (match_dup 2)
	   (ior:SI
	     (ashift:SI (match_dup 2)
			(const_int 16))
	     (zero_extend:SI
	       (match_dup 4))))
      (clobber (reg:CC CC_REGNO))])
   (set (match_dup 0)
	(match_dup 2))]
  "{
     operands[3] = gen_highpart_mode (HImode, SImode, operands[1]);
     operands[4] = gen_lowpart (HImode, operands[1]);
   }")

; If we have a load of a large integer constant which does not have bit 31
; set and we have a spare A reg then construct it with a moveai/lea.1 pair
; instead.  This avoids constructing it in 3 instructions on the stack.
;
; Note that we have to be careful not to match anything that matches
; something we can do in a single instruction!  There aren't many such
; constants but there are some.
;
(define_peephole2
  [(match_scratch:SI 2 "a")
   (set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))]
  "(! (INTVAL (operands[1]) & 0x80000000)
    && ((INTVAL (operands[1]) >= 0x8000
	 && INTVAL (operands[1]) < 0xff80)
	|| INTVAL (operands[1]) >= 0x10000))"
  [(set (match_dup 2)
	(match_dup 3))
   (set (match_dup 0)
	(plus:SI (match_dup 2)
		 (match_dup 4)))]
  "{
     HOST_WIDE_INT i = INTVAL (operands[1]);
     operands[3] = GEN_INT (i & 0xffffff80);
     operands[4] = GEN_INT (i & 0x7f);
   }")

; If we're not dependent on the state of the condition codes we can construct
; a 32-bit constant with a movei/movei/shmrg.2 sequence if possible.
;
(define_peephole2
  [(match_scratch:HI 2 "d")
   (set (match_operand:SI 0 "ubicom32_data_register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))
   (match_dup 2)]
  "(INTVAL (operands[1]) & 0x80000000
    && INTVAL (operands[1]) < -0x8000
    && peep2_regno_dead_p (0, CC_REGNO))"
  [(set (match_dup 0)
	(match_dup 3))
   (set (match_dup 2)
	(match_dup 4))
   (parallel
     [(set (match_dup 0)
	   (ior:SI
	     (ashift:SI (match_dup 0)
			(const_int 16))
	     (zero_extend:SI
	       (match_dup 2))))
      (clobber (reg:CC CC_REGNO))])]
  "{
     operands[3] = gen_highpart_mode (HImode, SImode, operands[1]);
     operands[4] = gen_lowpart (HImode, operands[1]);
   }")

; Exactly the same as the peephole2 preceding except that this targets a
; general register instead of D register.  Hopefully the later optimization
; passes will notice that the value ended up in a D register first here
; and eliminate away the other register!
;
(define_peephole2
  [(match_scratch:SI 2 "d")
   (match_scratch:HI 3 "d")
   (set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))
   (match_dup 3)]
  "(INTVAL (operands[1]) & 0x80000000
    && INTVAL (operands[1]) < -0x8000
    && peep2_regno_dead_p (0, CC_REGNO))"
  [(set (match_dup 2)
	(match_dup 4))
   (set (match_dup 3)
	(match_dup 5))
   (parallel
     [(set (match_dup 2)
	   (ior:SI
	     (ashift:SI (match_dup 2)
			(const_int 16))
	     (zero_extend:SI
	       (match_dup 3))))
      (clobber (reg:CC CC_REGNO))])
   (set (match_dup 0)
	(match_dup 2))]
  "{
     operands[4] = gen_highpart_mode (HImode, SImode, operands[1]);
     operands[5] = gen_lowpart (HImode, operands[1]);
   }")

(define_insn "movsi_fdpic_got_offset"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"   "=d")
	(match_operand:SI 1 "ubicom32_fdpic_got_offset_operand" "Y"))]
  ""
  "movei\\t%0, %1")

; The explicit MEM inside the UNSPEC prevents the compiler from moving
; the load before a branch after a NULL test, or before a store that
; initializes a function descriptor.

(define_insn_and_split "load_fdpic_funcdesc"
  [(set (match_operand:SI 0 "ubicom32_address_register_operand" "=a")
	(unspec_volatile:SI [(mem:SI (match_operand:SI 1 "address_operand" "p"))]
			     UNSPEC_VOLATILE_LOAD_FDPIC_FUNCDESC))]
  ""
  "#"
  "reload_completed"
  [(set (match_dup 0)
	(mem:SI (match_dup 1)))])

; Combiner-generated 32-bit move with the zero flag set accordingly.
;
(define_insn "movsi_ccwzn"
  [(set (reg CC_REGNO)
	(compare (match_operand:SI 0 "nonimmediate_operand" "rm, d")
		 (const_int 0)))
   (set (match_operand:SI 1 "nonimmediate_operand"	    "=d,rm")
	(match_dup 0))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "@
   lsl.4\\t%1, %0, #0
   add.4\\t%1, #0, %0")

; Combiner-generated 32-bit move with all flags set accordingly.
;
(define_insn "movsi_ccw"
  [(set (reg CC_REGNO)
	(compare (match_operand:SI 0 "ubicom32_data_register_operand" "d")
		 (const_int 0)))
   (set (match_operand:SI 1 "nonimmediate_operand"		    "=rm")
	(match_dup 0))]
  "ubicom32_match_cc_mode(insn, CCWmode)"
  "add.4\\t%1, #0, %0")

; Combine isn't very good at merging some types of operations so we
; have to make do with a peephole.  It's not as effective but it's better
; than doing nothing.
;
(define_peephole2
  [(set (match_operand:SI 0 "ubicom32_data_register_operand" "")
	(match_operand:SI 1 "nonimmediate_operand" ""))
   (parallel
     [(set (match_operand 2 "ubicom32_cc_register_operand" "")
	   (match_operator 3 "ubicom32_compare_operator"
	     [(match_dup 0)
	      (const_int 0)]))
      (clobber (match_operand:SI 4 "ubicom32_data_register_operand" ""))])]
  "(GET_MODE (operands[2]) == CCWZNmode
    || GET_MODE (operands[2]) == CCWZmode)"
  [(parallel
     [(set (match_dup 2)
	   (match_op_dup 3
	     [(match_dup 1)
	      (const_int 0)]))
      (set (match_dup 0)
	   (match_dup 1))])]
   "")

; Combine isn't very good at merging some types of operations so we
; have to make do with a peephole.  It's not as effective but it's better
; than doing nothing.
;
(define_peephole2
  [(set (match_operand:SI 0 "nonimmediate_operand" "")
	(match_operand:SI 1 "ubicom32_data_register_operand" ""))
   (parallel
     [(set (match_operand 2 "ubicom32_cc_register_operand" "")
	   (match_operator 3 "ubicom32_compare_operator"
	     [(match_dup 1)
	      (const_int 0)]))
      (clobber (match_operand:SI 4 "ubicom32_data_register_operand" ""))])]
  "(GET_MODE (operands[2]) == CCWZNmode
    || GET_MODE (operands[2]) == CCWZmode)"
  [(parallel
     [(set (match_dup 2)
	   (match_op_dup 3
	     [(match_dup 1)
	      (const_int 0)]))
      (set (match_dup 0)
	   (match_dup 1))])]
   "")

; Combine isn't very good at merging some types of operations so we
; have to make do with a peephole.  It's not as effective but it's better
; than doing nothing.
;
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "nonimmediate_operand" ""))
   (parallel
     [(set (match_operand 2 "ubicom32_cc_register_operand" "")
	   (match_operator 3 "ubicom32_compare_operator"
	     [(match_dup 0)
	      (const_int 0)]))
      (set (match_operand:SI 4 "ubicom32_data_register_operand" "")
	   (match_dup 0))])]
  "(peep2_reg_dead_p (2, operands[0])
    && (GET_MODE (operands[2]) == CCWZNmode
	|| GET_MODE (operands[2]) == CCWZmode))"
  [(parallel
     [(set (match_dup 2)
	   (match_op_dup 3
	     [(match_dup 1)
	      (const_int 0)]))
      (set (match_dup 4)
	   (match_dup 1))])]
   "")

; Register renaming may make a general reg into a D reg in which case
; we may be able to simplify a compare.
;
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "nonimmediate_operand" ""))
   (parallel
     [(set (match_operand 2 "ubicom32_cc_register_operand" "")
	   (match_operator 3 "ubicom32_compare_operator"
	     [(match_dup 0)
	      (const_int 0)]))
      (clobber (match_operand:SI 4 "ubicom32_data_register_operand" ""))])]
  "(peep2_reg_dead_p (2, operands[0])
    && (GET_MODE (operands[2]) == CCWZNmode
	|| GET_MODE (operands[2]) == CCWZmode))"
  [(parallel
     [(set (match_dup 2)
	   (match_op_dup 3
	     [(match_dup 1)
	      (const_int 0)]))
      (clobber (match_dup 4))])]
   "")

(define_insn_and_split "movdi"
  [(set (match_operand:DI 0 "nonimmediate_operand" "=r,rm")
	(match_operand:DI 1 "general_operand"	  "rmi,ri"))]
  ""
  "#"
  "reload_completed"
  [(set (match_dup 2) (match_dup 3))
   (set (match_dup 4) (match_dup 5))]
  "{
     rtx dest_low;
     rtx src_low;

     dest_low = gen_lowpart (SImode, operands[0]);
     src_low = gen_lowpart (SImode, operands[1]);

     if (REG_P (operands[0])
	 && REG_P (operands[1])
	 && REGNO (operands[0]) < REGNO (operands[1]))
       {
	 operands[2] = gen_highpart (SImode, operands[0]);
	 operands[3] = gen_highpart_mode (SImode, DImode, operands[1]);
	 operands[4] = dest_low;
	 operands[5] = src_low;
       }
     else if (reg_mentioned_p (dest_low, src_low))
       {
	 operands[2] = gen_highpart (SImode, operands[0]);
	 operands[3] = gen_highpart_mode (SImode, DImode, operands[1]);
	 operands[4] = dest_low;
	 operands[5] = src_low;
       }
     else
       {
	 operands[2] = dest_low;
	 operands[3] = src_low;
	 operands[4] = gen_highpart (SImode, operands[0]);
	 operands[5] = gen_highpart_mode (SImode, DImode, operands[1]);
       }
   }"
  [(set_attr "length" "8")])

; Combiner-generated 64-bit move with all flags set accordingly.
;
(define_insn "movdi_ccwzn"
  [(set (reg CC_REGNO)
	(compare (match_operand:DI 0 "nonimmediate_operand" "d, m,   r")
		 (const_int 0)))
   (set (match_operand:DI 1 "nonimmediate_operand"	 "=&rm,rm,!&rm")
	(match_dup 0))
   (clobber (match_scratch:SI 2				   "=X, d,   d"))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "*
   {
     operands[3] = gen_lowpart (SImode, operands[0]);
     operands[4] = gen_lowpart (SImode, operands[1]);
     operands[5] = gen_highpart (SImode, operands[0]);
     operands[6] = gen_highpart (SImode, operands[1]);

     if (ubicom32_data_register_operand (operands[0], VOIDmode))
       return \"add.4\\t%4, #0, %3\;addc\\t%6, #0, %5\";

     return \"movei\\t%2, #0\;add.4\\t%4, %3, %2\;addc\\t%6, %5, %2\";
   }"
  [(set_attr "length" "8")])

(define_insn "movdi_ccw"
  [(set (reg CC_REGNO)
	(compare (match_operand:DI 0 "nonimmediate_operand" "d, m,   r")
		 (const_int 0)))
   (set (match_operand:DI 1 "nonimmediate_operand"	 "=&rm,rm,!&rm")
	(match_dup 0))
   (clobber (match_scratch:SI 2				   "=X, d,   d"))]
  "ubicom32_match_cc_mode(insn, CCWmode)"
  "*
   {
     operands[3] = gen_lowpart (SImode, operands[0]);
     operands[4] = gen_lowpart (SImode, operands[1]);
     operands[5] = gen_highpart (SImode, operands[0]);
     operands[6] = gen_highpart (SImode, operands[1]);

     if (ubicom32_data_register_operand (operands[0], VOIDmode))
       return \"add.4\\t%4, #0, %3\;addc\\t%6, #0, %5\";

     return \"movei\\t%2, #0\;add.4\\t%4, %3, %2\;addc\\t%6, %5, %2\";
   }"
  [(set_attr "length" "8")])

(define_insn "movsf"
  [(set (match_operand:SF 0 "nonimmediate_operand"  "=!d,*rm")
	(match_operand:SF 1 "ubicom32_move_operand" "rmF,rmF"))]
  ""
  "*
   {
     if (GET_CODE (operands[1]) == CONST_DOUBLE)
       {
	 HOST_WIDE_INT val;
	 REAL_VALUE_TYPE rv;

	 REAL_VALUE_FROM_CONST_DOUBLE (rv, operands[1]);
	 REAL_VALUE_TO_TARGET_SINGLE (rv, val);

	 ubicom32_emit_move_const_int (operands[0], GEN_INT (val));
	 return \"\";
       }

     return \"move.4\\t%0, %1\";
   }")

(define_insn "zero_extendqihi2"
  [(set (match_operand:HI 0 "register_operand"			   "=r")
	(zero_extend:HI (match_operand:QI 1 "nonimmediate_operand" "rm")))]
  ""
  "move.1\\t%0, %1")

(define_insn "zero_extendqisi2"
  [(set (match_operand:SI 0 "register_operand"			   "=r")
	(zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "rm")))]
  ""
  "move.1\\t%0, %1")

(define_insn "zero_extendqisi2_ccwz_1"
  [(set (reg CC_REGNO)
	(compare
	  (zero_extend:SI (match_operand:QI 1 "nonimmediate_operand" "rm"))
          (const_int 0)))
   (set (match_operand:SI 0 "ubicom32_data_register_operand"	     "=d")
	(zero_extend:SI (match_dup 1)))]
  "ubicom32_match_cc_mode(insn, CCWZmode)"
  "shmrg.1\\t%0, %1, #0")

(define_insn "zero_extendhisi2"
  [(set (match_operand:SI 0 "register_operand"			   "=r")
	(zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "rm")))]
  ""
  "move.2\\t%0, %1")

(define_insn "zero_extendhisi2_ccwz_1"
  [(set (reg CC_REGNO)
	(compare
	  (zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "rm"))
          (const_int 0)))
   (set (match_operand:SI 0 "ubicom32_data_register_operand"	     "=d")
	(zero_extend:SI (match_dup 1)))]
  "ubicom32_match_cc_mode(insn, CCWZmode)"
  "shmrg.2\\t%0, %1, #0")

(define_insn_and_split "zero_extendqidi2"
  [(set (match_operand:DI 0 "register_operand"			   "=r")
	(zero_extend:DI (match_operand:QI 1 "nonimmediate_operand" "rm")))]
  ""
  "#"
  "reload_completed"
  [(set (match_dup 2)
	(zero_extend:SI (match_dup 1)))
   (set (match_dup 3)
	(const_int 0))]
  "{
     operands[2] = gen_lowpart (SImode, operands[0]);
     operands[3] = gen_highpart (SImode, operands[0]);
   }"
  [(set_attr "length" "8")])

(define_insn_and_split "zero_extendhidi2"
  [(set (match_operand:DI 0 "register_operand"			   "=r")
	(zero_extend:DI (match_operand:HI 1 "nonimmediate_operand" "rm")))]
  ""
  "#"
  "reload_completed"
  [(set (match_dup 2)
	(zero_extend:SI (match_dup 1)))
   (set (match_dup 3)
	(const_int 0))]
  "{
     operands[2] = gen_lowpart (SImode, operands[0]);
     operands[3] = gen_highpart (SImode, operands[0]);
   }"
  [(set_attr "length" "8")])

(define_insn_and_split "zero_extendsidi2"
  [(set (match_operand:DI 0 "nonimmediate_operand"		  "=rm")
	(zero_extend:DI (match_operand:SI 1 "nonimmediate_operand" "rm")))]
  ""
  "#"
  "reload_completed"
  [(set (match_dup 2)
	(match_dup 1))
   (set (match_dup 3)
	(const_int 0))]
  "{
     operands[2] = gen_lowpart (SImode, operands[0]);
     operands[3] = gen_highpart (SImode, operands[0]);
   }"
  [(set_attr "length" "8")])

(define_insn "extendqihi2"
  [(set (match_operand:HI 0 "register_operand"			   "=r")
	(sign_extend:HI (match_operand:QI 1 "nonimmediate_operand" "rm")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "ext.1\\t%0, %1")

(define_insn "extendqisi2"
  [(set (match_operand:SI 0 "register_operand"			   "=r")
	(sign_extend:SI (match_operand:QI 1 "nonimmediate_operand" "rm")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "ext.1\\t%0, %1")

(define_insn "extendhisi2"
  [(set (match_operand:SI 0 "register_operand"			   "=r")
	(sign_extend:SI (match_operand:HI 1 "nonimmediate_operand" "rm")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "ext.2\\t%0, %1")

(define_insn_and_split "extendsidi2"
  [(set (match_operand:DI 0 "nonimmediate_operand"		   "=d")
	(sign_extend:DI (match_operand:SI 1 "nonimmediate_operand" "rm")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "#"
  "reload_completed"
  [(set (match_dup 2)
	(match_dup 1))
   (parallel
     [(set (match_dup 3)
	   (ashiftrt:SI (match_dup 2)
		        (const_int 31)))
      (clobber (reg:CC CC_REGNO))])]
  "{
     operands[2] = gen_lowpart (SImode, operands[0]);
     operands[3] = gen_highpart (SImode, operands[0]);
   }"
  [(set_attr "length" "8")])

(define_insn "bswaphi"
  [(set (match_operand:HI 0 "nonimmediate_operand"	       "=rm")
	(bswap:HI (match_operand:HI 1 "ubicom32_arith_operand" "rmI")))]
  "(ubicom32_v4)"
  "swapb.2\\t%0, %1");

(define_insn "bswaphisi"
  [(set (match_operand:SI 0 "register_operand"			  "=r")
	(zero_extend:SI
	  (bswap:HI (match_operand:HI 1 "ubicom32_arith_operand" "rmI"))))]
  "(ubicom32_v4)"
  "swapb.2\\t%0, %1");

(define_insn "bswapsi"
  [(set (match_operand:SI 0 "nonimmediate_operand"	       "=rm")
	(bswap:SI (match_operand:SI 1 "ubicom32_arith_operand" "rmI")))]
  "(ubicom32_v4)"
  "swapb.4\\t%0, %1");

(define_insn "tstqi_ext1"
  [(set (reg CC_REGNO)
	(compare (match_operand:QI 0 "nonimmediate_operand" "rm")
		 (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "ext.1\\t#0, %0")

(define_expand "cmpqi"
  [(set (reg CC_REGNO)
	(compare (match_operand:QI 0 "ubicom32_arith_operand" "")
		 (match_operand:QI 1 "ubicom32_data_register_operand" "")))]
  "(ubicom32_v4)"
  "{
     ubicom32_compare_op0 = operands[0];
     ubicom32_compare_op1 = operands[1];
     DONE;
   }")

(define_insn "sub1_ccs"
  [(set (reg CC_REGNO)
	(compare (match_operand:QI 0 "ubicom32_arith_operand"       "rmI")
		 (match_operand:QI 1 "ubicom32_data_register_operand" "d")))]
  "(ubicom32_v4)"
  "sub.1\\t#0, %0, %1")

; If we're testing for equality we don't have to worry about reversing conditions.
;
(define_insn "sub1_ccsz_1"
  [(set (reg:CCSZ CC_REGNO)
	(compare:CCSZ (match_operand:QI 0 "nonimmediate_operand"	  "rm")
		      (match_operand:QI 1 "ubicom32_data_register_operand" "d")))]
  "(ubicom32_v4)"
  "sub.1\\t#0, %0, %1")

(define_insn "sub1_ccsz_2"
  [(set (reg:CCSZ CC_REGNO)
	(compare:CCSZ (match_operand:QI 0 "ubicom32_data_register_operand" "d")
		      (match_operand:QI 1 "ubicom32_arith_operand"	 "rmI")))]
  "(ubicom32_v4)"
  "sub.1\\t#0, %1, %0")

; When the combiner runs it doesn't have any insight into whether or not an argument
; to a compare is spilled to the stack and therefore can't swap the comparison in
; an attempt to use sub.1 more effectively.  We peephole this case here.
;
(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
	(match_operand:QI 1 "ubicom32_arith_operand" ""))
   (set (match_operand 2 "ubicom32_cc_register_operand" "")
	(compare (match_operand:QI 3 "ubicom32_data_register_operand" "")
		 (match_dup 0)))
   (set (pc)
	(if_then_else (match_operator 4 "comparison_operator"
			[(match_dup 2)
			 (const_int 0)])
		      (label_ref (match_operand 5 "" ""))
		      (pc)))]
  "(peep2_reg_dead_p (2, operands[0])
    && peep2_regno_dead_p (3, CC_REGNO))"
  [(set (match_dup 2)
	(compare (match_dup 1)
		 (match_dup 3)))
   (set (pc)
	(if_then_else (match_op_dup 6
			[(match_dup 2)
			 (const_int 0)])
		      (label_ref (match_dup 5))
		      (pc)))]
  "{
     rtx cc_reg;

     cc_reg = gen_rtx_REG (GET_MODE (operands[2]), CC_REGNO);
     operands[6] = gen_rtx_fmt_ee (swap_condition (GET_CODE (operands[4])),
	 			   GET_MODE (operands[4]),
				   cc_reg,
				   const0_rtx);
   }")

(define_insn "tsthi_ext2"
  [(set (reg CC_REGNO)
	(compare (match_operand:HI 0 "nonimmediate_operand" "rm")
		 (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "ext.2\\t#0, %0")

(define_expand "cmphi"
  [(set (reg CC_REGNO)
	(compare (match_operand:HI 0 "ubicom32_arith_operand" "")
		 (match_operand:HI 1 "ubicom32_compare_operand" "")))]
  ""
  "{
     do
       {
	 /* Is this a cmpi? */
	 if (CONST_INT_P (operands[1]))
	   break;

	 /* Must be a sub.2 - if necessary copy an operand into a reg.  */
	 if (! ubicom32_data_register_operand (operands[1], HImode))
	   operands[1] = copy_to_mode_reg (HImode, operands[1]);
       }
     while (0);

     ubicom32_compare_op0 = operands[0];
     ubicom32_compare_op1 = operands[1];
     DONE;
   }")

(define_insn "cmpi"
  [(set (reg CC_REGNO)
	(compare (match_operand:HI 0 "nonimmediate_operand" "rm")
		 (match_operand 1 "const_int_operand"	     "N")))]
  ""
  "cmpi\\t%0, %1")

(define_insn "sub2_ccs"
  [(set (reg CC_REGNO)
	(compare (match_operand:HI 0 "ubicom32_arith_operand"	    "rmI")
		 (match_operand:HI 1 "ubicom32_data_register_operand" "d")))]
  ""
  "sub.2\\t#0, %0, %1")

; If we're testing for equality we don't have to worry about reversing conditions.
;
(define_insn "sub2_ccsz_1"
  [(set (reg:CCSZ CC_REGNO)
	(compare:CCSZ (match_operand:HI 0 "nonimmediate_operand"	  "rm")
		      (match_operand:HI 1 "ubicom32_data_register_operand" "d")))]
  ""
  "sub.2\\t#0, %0, %1")

(define_insn "sub2_ccsz_2"
  [(set (reg:CCSZ CC_REGNO)
	(compare:CCSZ (match_operand:HI 0 "ubicom32_data_register_operand" "d")
		      (match_operand:HI 1 "ubicom32_arith_operand"	 "rmI")))]
  ""
  "sub.2\\t#0, %1, %0")

; When the combiner runs it doesn't have any insight into whether or not an argument
; to a compare is spilled to the stack and therefore can't swap the comparison in
; an attempt to use sub.2 more effectively.  We peephole this case here.
;
(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
	(match_operand:HI 1 "ubicom32_arith_operand" ""))
   (set (match_operand 2 "ubicom32_cc_register_operand" "")
	(compare (match_operand:HI 3 "ubicom32_data_register_operand" "")
		 (match_dup 0)))
   (set (pc)
	(if_then_else (match_operator 4 "comparison_operator"
			[(match_dup 2)
			 (const_int 0)])
		      (label_ref (match_operand 5 "" ""))
		      (pc)))]
  "(peep2_reg_dead_p (2, operands[0])
    && peep2_regno_dead_p (3, CC_REGNO))"
  [(set (match_dup 2)
	(compare (match_dup 1)
		 (match_dup 3)))
   (set (pc)
	(if_then_else (match_op_dup 6
			[(match_dup 2)
			 (const_int 0)])
		      (label_ref (match_dup 5))
		      (pc)))]
  "{
     rtx cc_reg;

     cc_reg = gen_rtx_REG (GET_MODE (operands[2]), CC_REGNO);
     operands[6] = gen_rtx_fmt_ee (swap_condition (GET_CODE (operands[4])),
	 			   GET_MODE (operands[4]),
				   cc_reg,
				   const0_rtx);
   }")

(define_insn_and_split "tstsi_lsl4"
  [(set (match_operand 0 "ubicom32_cc_register_operand" "=r")
	(match_operator 1 "ubicom32_compare_operator"
	  [(match_operand:SI 2 "nonimmediate_operand"   "rm")
	   (const_int 0)]))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "#"
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  [(parallel
     [(set (match_dup 0)
	   (match_op_dup 1
	     [(match_dup 2)
	      (const_int 0)]))
      (clobber (match_dup 3))])]
  "{
     operands[3] = gen_reg_rtx (SImode);
   }")

(define_insn "tstsi_lsl4_d"
  [(set (reg CC_REGNO)
	(compare (match_operand:SI 0 "nonimmediate_operand" "rm")
		 (const_int 0)))
   (clobber (match_operand:SI 1 "ubicom32_data_register_operand" "=d"))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "lsl.4\\t%1, %0, #0")

; Comparison for equality with -1.
;
(define_insn "cmpsi_not4_ccwz"
  [(set (reg CC_REGNO)
	(compare (match_operand:SI 0 "nonimmediate_operand" "rm")
		 (const_int -1)))]
  "ubicom32_match_cc_mode(insn, CCWZmode)"
  "not.4\\t#0, %0")

(define_expand "cmpsi"
  [(set (reg CC_REGNO)
	(compare (match_operand:SI 0 "ubicom32_arith_operand" "")
		 (match_operand:SI 1 "ubicom32_compare_operand" "")))]
  ""
  "{
     do
       {
	 /* Is this a cmpi?  We can't take a memory address as cmpi takes
            16-bit operands.  */
	 if (register_operand (operands[0], SImode)
	     && CONST_INT_P (operands[1])
	     && satisfies_constraint_N (operands[1]))
	   break;

	 /* Must be a sub.4 - if necessary copy an operand into a reg.  */
	 if (! ubicom32_data_register_operand (operands[1], SImode))
	   operands[1] = copy_to_mode_reg (SImode, operands[1]);
       }
     while (0);

     ubicom32_compare_op0 = operands[0];
     ubicom32_compare_op1 = operands[1];
     DONE;
   }")

(define_insn "cmpsi_cmpi"
  [(set (reg CC_REGNO)
	(compare (match_operand:SI 0 "register_operand" "r")
		 (match_operand 1 "const_int_operand"   "N")))]
  "(satisfies_constraint_N (operands[1]))"
  "cmpi\\t%0, %1")

(define_insn "cmpsi_sub4"
  [(set (reg CC_REGNO)
	(compare (match_operand:SI 0 "ubicom32_arith_operand"	    "rmI")
		 (match_operand:SI 1 "ubicom32_data_register_operand" "d")))]
  ""
  "sub.4\\t#0, %0, %1")

; If we're testing for equality we don't have to worry about reversing conditions.
;
(define_insn "cmpsi_sub4_ccwz_1"
  [(set (reg CC_REGNO)
	(compare (match_operand:SI 0 "nonimmediate_operand"	     "rm")
		 (match_operand:SI 1 "ubicom32_data_register_operand" "d")))]
  "ubicom32_match_cc_mode(insn, CCWZmode)"
  "sub.4\\t#0, %0, %1")

(define_insn "cmpsi_sub4_ccwz_2"
  [(set (reg CC_REGNO)
	(compare (match_operand:SI 0 "ubicom32_data_register_operand" "d")
		 (match_operand:SI 1 "nonimmediate_operand"	     "rm")))]
  "ubicom32_match_cc_mode(insn, CCWZmode)"
  "sub.4\\t#0, %1, %0")

; When the combiner runs it doesn't have any insight into whether or not an argument
; to a compare is spilled to the stack and therefore can't swap the comparison in
; an attempt to use sub.4 more effectively.  We peephole this case here.
;
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "ubicom32_arith_operand" ""))
   (set (match_operand 2 "ubicom32_cc_register_operand" "")
	(compare (match_operand:SI 3 "ubicom32_data_register_operand" "")
		 (match_dup 0)))
   (set (pc)
	(if_then_else (match_operator 4 "comparison_operator"
			[(match_dup 2)
			 (const_int 0)])
		      (label_ref (match_operand 5 "" ""))
		      (pc)))]
  "(peep2_reg_dead_p (2, operands[0])
    && peep2_regno_dead_p (3, CC_REGNO))"
  [(set (match_dup 2)
	(compare (match_dup 1)
		 (match_dup 3)))
   (set (pc)
	(if_then_else (match_op_dup 6
			[(match_dup 2)
			 (const_int 0)])
		      (label_ref (match_dup 5))
		      (pc)))]
  "{
     rtx cc_reg;

     cc_reg = gen_rtx_REG (GET_MODE (operands[2]), CC_REGNO);
     operands[6] = gen_rtx_fmt_ee (swap_condition (GET_CODE (operands[4])),
	 			   GET_MODE (operands[4]),
				   cc_reg,
				   const0_rtx);
   }")

(define_insn_and_split "tstdi_or4"
  [(set (reg:CCWZ CC_REGNO)
	(compare:CCWZ (match_operand:DI 0 "nonimmediate_operand" "rm")
		      (const_int 0)))]
  ""
  "#"
  ""
  [(parallel
     [(set (reg:CCWZ CC_REGNO)
	   (compare:CCWZ (match_dup 0)
			 (const_int 0)))
      (clobber (match_dup 1))])]
  "{
     operands[1] = gen_reg_rtx (SImode);
   }")

(define_insn "tstdi_or4_d"
  [(set (reg:CCWZ CC_REGNO)
	(compare:CCWZ (match_operand:DI 0 "nonimmediate_operand" "rm")
		      (const_int 0)))
   (clobber (match_operand:SI 1 "ubicom32_data_register_operand" "=d"))]
  ""
  "*
   {
     operands[2] = gen_lowpart (SImode, operands[0]);
     operands[3] = gen_highpart_mode (SImode, DImode, operands[0]);

     if (ubicom32_data_register_operand (operands[0], GET_MODE (operands[0])))
       return \"or.4\\t#0, %2, %3\";

     return \"move.4\\t%1, %2\;or.4\\t%1, %3, %1\";
   }"
  [(set_attr "length" "8")])

(define_expand "cmpdi"
  [(set (reg CC_REGNO)
	(compare (match_operand:DI 0 "ubicom32_arith_operand" "")
		 (match_operand:DI 1 "ubicom32_data_register_operand" "")))]
  ""
  "{
     ubicom32_compare_op0 = operands[0];
     ubicom32_compare_op1 = operands[1];
     DONE;
   }")

(define_insn "cmpdi_sub4subc"
  [(set (reg CC_REGNO)
	(compare (match_operand:DI 0 "ubicom32_arith_operand"	    "rmI")
		 (match_operand:DI 1 "ubicom32_data_register_operand" "d")))]
  ""
  "*
   {
     operands[2] = gen_lowpart (SImode, operands[0]);
     operands[3] = gen_lowpart (SImode, operands[1]);
     operands[4] = gen_highpart_mode (SImode, DImode, operands[0]);
     operands[5] = gen_highpart_mode (SImode, DImode, operands[1]);

     return \"sub.4\\t#0, %2, %3\;subc\\t#0, %4, %5\";
   }"
  [(set_attr "length" "8")])

; When the combiner runs it doesn't have any insight into whether or not an argument
; to a compare is spilled to the stack and therefore can't swap the comparison in
; an attempt to use sub.4/subc more effectively.  We peephole this case here.
;
(define_peephole2
  [(set (match_operand:DI 0 "register_operand" "")
	(match_operand:DI 1 "ubicom32_arith_operand" ""))
   (set (match_operand 2 "ubicom32_cc_register_operand" "")
	(compare (match_operand:DI 3 "ubicom32_data_register_operand" "")
		 (match_dup 0)))
   (set (pc)
	(if_then_else (match_operator 4 "comparison_operator"
			[(match_dup 2)
			 (const_int 0)])
		      (label_ref (match_operand 5 "" ""))
		      (pc)))]
  "(peep2_reg_dead_p (2, operands[0])
    && peep2_regno_dead_p (3, CC_REGNO))"
  [(set (match_dup 2)
	(compare (match_dup 1)
		 (match_dup 3)))
   (set (pc)
	(if_then_else (match_op_dup 6
			[(match_dup 2)
			 (const_int 0)])
		      (label_ref (match_dup 5))
		      (pc)))]
  "{
     rtx cc_reg;

     cc_reg = gen_rtx_REG (GET_MODE (operands[2]), CC_REGNO);
     operands[6] = gen_rtx_fmt_ee (swap_condition (GET_CODE (operands[4])),
	 			   GET_MODE (operands[4]),
				   cc_reg,
				   const0_rtx);
   }")

(define_insn "btst"
  [(set (reg:CCWZ CC_REGNO)
	(compare:CCWZ
	  (zero_extract:SI
	    (match_operand:SI 0 "nonimmediate_operand"   "rm")
	    (const_int 1)
	    (match_operand:SI 1 "ubicom32_arith_operand" "dM"))
	  (const_int 0)))]
  ""
  "btst\\t%0, %1")

(define_insn "bfextu_ccwz_null"
  [(set (reg:CCWZ CC_REGNO)
	(compare:CCWZ
	  (zero_extract:SI
	    (match_operand:SI 0 "nonimmediate_operand" "rm")
	    (match_operand 1 "const_int_operand"        "M")
	    (const_int 0))
	  (const_int 0)))
   (clobber (match_scratch:SI 2			       "=d"))]
  ""
  "bfextu\\t%2, %0, %1")

(define_expand "addqi3"
  [(parallel
     [(set (match_operand:QI 0 "memory_operand" "")
	   (plus:QI (match_operand:QI 1 "nonimmediate_operand" "")
		    (match_operand:QI 2 "ubicom32_arith_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  "(ubicom32_v4)"
  "{
     if (!memory_operand (operands[0], QImode))
       FAIL;

     /* If we have a non-data reg for operand 1 then prefer that over
        a CONST_INT in operand 2.  */
     if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	 && CONST_INT_P (operands[2]))
       operands[2] = copy_to_mode_reg (QImode, operands[2]);

     if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
       operands[2] = copy_to_mode_reg (QImode, operands[2]);
   }")

(define_insn "addqi3_add1"
  [(set (match_operand:QI 0 "memory_operand"		       "=m, m")
	(plus:QI (match_operand:QI 1 "nonimmediate_operand"    "%d,rm")
		 (match_operand:QI 2 "ubicom32_arith_operand" "rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4)"
  "@
   add.1\\t%0, %2, %1
   add.1\\t%0, %1, %2")

(define_insn "addqi3_add1_ccszn_null"
  [(set (reg CC_REGNO)
	(compare
	  (neg:QI (match_operand:QI 0 "nonimmediate_operand" "%d,rm"))
	  (match_operand:QI 1 "ubicom32_arith_operand"      "rmI, d")))]
  "(ubicom32_v4
    && ubicom32_match_cc_mode(insn, CCSZNmode))"
  "@
   add.1\\t#0, %1, %0
   add.1\\t#0, %0, %1")

(define_expand "addhi3"
  [(parallel
     [(set (match_operand:HI 0 "memory_operand" "")
	   (plus:HI (match_operand:HI 1 "nonimmediate_operand" "")
		    (match_operand:HI 2 "ubicom32_arith_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  ""
  "{
     if (!memory_operand (operands[0], HImode))
       FAIL;

     /* If we have a non-data reg for operand 1 then prefer that over
        a CONST_INT in operand 2.  */
     if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	 && CONST_INT_P (operands[2]))
       operands[2] = copy_to_mode_reg (HImode, operands[2]);

     if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
       operands[2] = copy_to_mode_reg (HImode, operands[2]);
   }")

(define_insn "addhi3_add2"
  [(set (match_operand:HI 0 "memory_operand"		       "=m, m")
	(plus:HI (match_operand:HI 1 "nonimmediate_operand"    "%d,rm")
		 (match_operand:HI 2 "ubicom32_arith_operand" "rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "@
   add.2\\t%0, %2, %1
   add.2\\t%0, %1, %2")

(define_insn "addhi3_add2_ccszn_null"
  [(set (reg CC_REGNO)
	(compare
	  (neg:HI (match_operand:HI 0 "nonimmediate_operand" "%d,rm"))
	  (match_operand:HI 1 "ubicom32_arith_operand"      "rmI, d")))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "@
   add.2\\t#0, %1, %0
   add.2\\t#0, %0, %1")

(define_expand "addsi3"
  [(set (match_operand:SI 0 "nonimmediate_operand" "")
	(plus:SI (match_operand:SI 1 "nonimmediate_operand" "")
		 (match_operand:SI 2 "ubicom32_move_operand" "")))]
  ""
  "{
     ubicom32_expand_addsi3 (operands);
     DONE;
   }")

; We start with an instruction pattern that can do all sorts of interesting
; things but we split out any uses of lea or pdec instructions because
; those instructions don't clobber the condition codes.
;
(define_insn_and_split "addsi3_1"
  [(set (match_operand:SI 0 "nonimmediate_operand"	   "=rm,rm,rm,rm,rm, rm,rm")
	(plus:SI (match_operand:SI 1 "nonimmediate_operand" "%a, a, a, a, a,  d,rm")
		 (match_operand:SI 2 "ubicom32_move_operand" "L, K, J, P, d,rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "@
   #
   #
   #
   #
   #
   add.4\\t%0, %2, %1
   add.4\\t%0, %1, %2"
  "(reload_completed
    && ubicom32_address_register_operand (operands[1], GET_MODE (operands[1])))"
  [(set (match_dup 0)
	(plus:SI (match_dup 1)
		 (match_dup 2)))]
  ""
)

(define_insn "addsi3_1_ccwzn"
  [(set (reg CC_REGNO)
	(compare
	  (plus:SI (match_operand:SI 1 "nonimmediate_operand"    "%d,rm")
		   (match_operand:SI 2 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))
   (set (match_operand:SI 0 "nonimmediate_operand"	        "=rm,rm")
	(plus:SI (match_dup 1)
		 (match_dup 2)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "@
   add.4\\t%0, %2, %1
   add.4\\t%0, %1, %2")

(define_insn "addsi3_1_ccwzn_null"
  [(set (reg CC_REGNO)
	(compare
	  (neg:SI (match_operand:SI 0 "nonimmediate_operand" "%d,rm"))
	  (match_operand:SI 1 "ubicom32_arith_operand"      "rmI, d")))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "@
   add.4\\t#0, %1, %0
   add.4\\t#0, %0, %1")

(define_insn_and_split "addsi3_2"
  [(set (match_operand:SI 0 "nonimmediate_operand"			"=rm,rm,rm,rm,rm,rm")
	(plus:SI (match_operand:SI 1 "ubicom32_address_register_operand" "%a, a, a, a, a, a")
		 (match_operand:SI 2 "ubicom32_move_operand"		  "L, K, J, P, d, n")))]
  ""
  "@
   lea.4\\t%0, %E2(%1)
   lea.2\\t%0, %E2(%1)
   lea.1\\t%0, %E2(%1)
   pdec\\t%0, %n2(%1)
   lea.1\\t%0, (%1,%2)
   #"
  "(reload_completed
    && ! satisfies_constraint_L (operands[2])
    && ! satisfies_constraint_K (operands[2])
    && ! satisfies_constraint_J (operands[2])
    && ! satisfies_constraint_P (operands[2])
    && ! ubicom32_data_register_operand (operands[2], GET_MODE (operands[2])))"
  [(set (reg:SI AUX_DATA_REGNO)
  	(match_dup 2))
   (set (match_dup 0)
	(plus:SI (match_dup 1)
		 (reg:SI AUX_DATA_REGNO)))]
  ""
)

(define_insn "lea_2"
  [(set (match_operand:SI 0 "nonimmediate_operand"			     "=rm")
	(plus:SI (mult:SI (match_operand:SI 1 "ubicom32_data_register_operand" "d")
			  (const_int 2))
		 (match_operand:SI 2 "ubicom32_address_register_operand"       "a")))]
  ""
  "lea.2\\t%0, (%2,%1)")

(define_insn "lea_4"
  [(set (match_operand:SI 0 "nonimmediate_operand"			     "=rm")
	(plus:SI (mult:SI (match_operand:SI 1 "ubicom32_data_register_operand" "d")
			  (const_int 4))
		 (match_operand:SI 2 "ubicom32_address_register_operand"       "a")))]
  ""
  "lea.4\\t%0, (%2,%1)")

(define_expand "adddi3"
  [(parallel
     [(set (match_operand:DI 0 "nonimmediate_operand" "")
	   (plus:DI (match_operand:DI 1 "nonimmediate_operand" "")
		    (match_operand:DI 2 "ubicom32_arith_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  ""
  "{
     /* If we have a non-data reg for operand 1 then prefer that over
        a CONST_INT in operand 2.  */
     if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	 && CONST_INT_P (operands[2]))
       operands[2] = copy_to_mode_reg (DImode, operands[2]);

     if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
       operands[2] = copy_to_mode_reg (DImode, operands[2]);
   }")

; We construct a 64-bit add from 32-bit operations.  Note that we use the
; & constraint to prevent overlapping registers being allocated.  We do
; allow identical registers though as that won't break anything.
;
(define_insn "adddi3_add4addc"
  [(set (match_operand:DI 0 "nonimmediate_operand"	      "=&r,&r,rm,  d,  m, m")
	(plus:DI (match_operand:DI 1 "nonimmediate_operand"    "%d,rm, 0,  0,  d,rm")
		 (match_operand:DI 2 "ubicom32_arith_operand" "rmI, d, d,rmI,rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "*
   {
     operands[3] = gen_lowpart (SImode, operands[0]);
     operands[4] = gen_lowpart (SImode, operands[1]);
     operands[5] = gen_lowpart (SImode, operands[2]);
     operands[6] = gen_highpart (SImode, operands[0]);
     operands[7] = gen_highpart (SImode, operands[1]);
     operands[8] = gen_highpart_mode (SImode, DImode, operands[2]);

     if (ubicom32_data_register_operand (operands[2], GET_MODE (operands[2])))
       return \"add.4\\t%3, %4, %5\;addc\\t%6, %7, %8\";

     return \"add.4\\t%3, %5, %4\;addc\\t%6, %8, %7\";
   }"
  [(set_attr "length" "8")])

(define_insn "adddi3_ccwz"
  [(set (reg CC_REGNO)
	(compare
	  (plus:DI (match_operand:DI 1 "nonimmediate_operand"    "%d,rm, 0,  0,  d,rm")
		   (match_operand:DI 2 "ubicom32_arith_operand" "rmI, d, d,rmI,rmI, d"))
	  (const_int 0)))
   (set (match_operand:DI 0 "nonimmediate_operand"		"=&r,&r,rm,  d,  m, m")
	(plus:DI (match_dup 1)
		 (match_dup 2)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "*
   {
     operands[3] = gen_lowpart (SImode, operands[0]);
     operands[6] = gen_highpart (SImode, operands[0]);

     if (ubicom32_data_register_operand (operands[1], GET_MODE (operands[1])))
       {
	 operands[4] = gen_lowpart (SImode, operands[1]);
	 operands[5] = gen_lowpart (SImode, operands[2]);
	 operands[7] = gen_highpart (SImode, operands[1]);
	 operands[8] = gen_highpart_mode (SImode, DImode, operands[2]);
       }
     else
       {
	 operands[4] = gen_lowpart (SImode, operands[2]);
	 operands[5] = gen_lowpart (SImode, operands[1]);
	 operands[7] = gen_highpart (SImode, operands[2]);
	 operands[8] = gen_highpart (SImode, operands[1]);
       }

     return \"add.4\\t%3, %5, %4\;addc\\t%6, %8, %7\";
   }"
  [(set_attr "length" "8")])

(define_insn "adddi3_ccwz_null"
  [(set (reg CC_REGNO)
	(compare
	  (neg:DI (match_operand:DI 0 "nonimmediate_operand" "%d,rm"))
	  (match_operand:DI 1 "ubicom32_arith_operand"      "rmI, d")))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "*
   {
     if (ubicom32_data_register_operand (operands[0], GET_MODE (operands[0])))
       {
	 operands[2] = gen_lowpart (SImode, operands[0]);
	 operands[3] = gen_lowpart (SImode, operands[1]);
	 operands[4] = gen_highpart (SImode, operands[0]);
	 operands[5] = gen_highpart_mode (SImode, DImode, operands[1]);
       }
     else
       {
	 operands[2] = gen_lowpart (SImode, operands[1]);
	 operands[3] = gen_lowpart (SImode, operands[0]);
	 operands[4] = gen_highpart (SImode, operands[1]);
	 operands[5] = gen_highpart (SImode, operands[0]);
       }

     return \"add.4\\t#0, %3, %2\;addc\\t#0, %5, %4\";
   }"
  [(set_attr "length" "8")])

(define_expand "subqi3"
  [(parallel
     [(set (match_operand:QI 0 "memory_operand" "")
	   (minus:QI (match_operand:QI 1 "ubicom32_arith_operand" "")
		     (match_operand:QI 2 "ubicom32_data_register_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  "(ubicom32_v4)"
  "{
     if (!memory_operand (operands[0], QImode))
       FAIL;
   }")

(define_insn "subqi3_sub1"
  [(set (match_operand:QI 0 "memory_operand"			      "=m")
	(minus:QI (match_operand:QI 1 "ubicom32_arith_operand"	     "rmI")
		  (match_operand:QI 2 "ubicom32_data_register_operand" "d")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4)"
  "sub.1\\t%0, %1, %2")

(define_expand "subhi3"
  [(parallel
     [(set (match_operand:HI 0 "memory_operand" "")
	   (minus:HI (match_operand:HI 1 "ubicom32_arith_operand" "")
		     (match_operand:HI 2 "ubicom32_data_register_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  "(ubicom32_v4)"
  "{
     if (!memory_operand (operands[0], HImode))
       FAIL;
   }")

(define_insn "subhi3_sub2"
  [(set (match_operand:HI 0 "memory_operand"			      "=m")
	(minus:HI (match_operand:HI 1 "ubicom32_arith_operand"	     "rmI")
		  (match_operand:HI 2 "ubicom32_data_register_operand" "d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "sub.2\\t%0, %1, %2")

(define_insn "subsi3"
  [(set (match_operand:SI 0 "nonimmediate_operand"		     "=rm")
	(minus:SI (match_operand:SI 1 "ubicom32_arith_operand"	     "rmI")
		  (match_operand:SI 2 "ubicom32_data_register_operand" "d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "sub.4\\t%0, %1, %2")

(define_insn "subsi3_ccwz"
  [(set (reg CC_REGNO)
	(compare
	  (minus:SI (match_operand:SI 1 "ubicom32_arith_operand"       "rmI")
		    (match_operand:SI 2 "ubicom32_data_register_operand" "d"))
	  (const_int 0)))
   (set (match_operand:SI 0 "nonimmediate_operand"		       "=rm")
	(minus:SI (match_dup 1)
		  (match_dup 2)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "sub.4\\t%0, %1, %2")

; We construct a 64-bit add from 32-bit operations.  Note that we use the
; & constraint to prevent overlapping registers being allocated.  We do
; allow identical registers though as that won't break anything.
;
(define_insn "subdi3"
  [(set (match_operand:DI 0 "nonimmediate_operand"		     "=&r,r,  d,  m")
	(minus:DI (match_operand:DI 1 "ubicom32_arith_operand"	     "rmI,0,rmI,rmI")
		  (match_operand:DI 2 "ubicom32_data_register_operand" "d,d,  0,  d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "*
   {
     operands[3] = gen_lowpart (SImode, operands[0]);
     operands[4] = gen_lowpart (SImode, operands[1]);
     operands[5] = gen_lowpart (SImode, operands[2]);
     operands[6] = gen_highpart (SImode, operands[0]);
     operands[7] = gen_highpart_mode (SImode, DImode, operands[1]);
     operands[8] = gen_highpart (SImode, operands[2]);

     return \"sub.4\\t%3, %4, %5\;subc\\t%6, %7, %8\";
   }"
  [(set_attr "length" "8")])

(define_insn "subdi3_ccwz"
  [(set (reg CC_REGNO)
	(compare
	  (minus:DI (match_operand:DI 1 "ubicom32_arith_operand"       "rmI,rmI")
		    (match_operand:DI 2 "ubicom32_data_register_operand" "d,  d"))
	  (const_int 0)))
   (set (match_operand:DI 0 "nonimmediate_operand"		       "=&r,  m")
	(minus:DI (match_dup 1)
		  (match_dup 2)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "*
   {
     operands[3] = gen_lowpart (SImode, operands[0]);
     operands[4] = gen_lowpart (SImode, operands[1]);
     operands[5] = gen_lowpart (SImode, operands[2]);
     operands[6] = gen_highpart (SImode, operands[0]);
     operands[7] = gen_highpart_mode (SImode, DImode, operands[1]);
     operands[8] = gen_highpart (SImode, operands[2]);

     return \"sub.4\\t%3, %4, %5\;subc\\t%6, %7, %8\";
   }"
  [(set_attr "length" "8")])

;(define_insn "negqi2"
;  [(set (match_operand:QI 0 "nonimmediate_operand"		   "=rm")
;	(neg:QI (match_operand:QI 1 "ubicom32_data_register_operand" "d")))
;   (clobber (reg:CC CC_REGNO))]
;  "(ubicom32_v4)"
;  "sub.1\\t%0, #0, %1")

;(define_insn "neghi2"
;  [(set (match_operand:HI 0 "nonimmediate_operand"		   "=rm")
;	(neg:HI (match_operand:HI 1 "ubicom32_data_register_operand" "d")))
;   (clobber (reg:CC CC_REGNO))]
;  ""
;  "sub.2\\t%0, #0, %1")

(define_insn "negsi2"
  [(set (match_operand:SI 0 "nonimmediate_operand"		   "=rm")
	(neg:SI (match_operand:SI 1 "ubicom32_data_register_operand" "d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "sub.4\\t%0, #0, %1")

(define_insn_and_split "negdi2"
  [(set (match_operand:DI 0 "nonimmediate_operand"		  "=&rm")
	(neg:DI (match_operand:DI 1 "ubicom32_data_register_operand" "d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "#"
  "reload_completed"
  [(parallel [(set (match_dup 0)
		   (minus:DI (const_int 0)
			     (match_dup 1)))
	      (clobber (reg:CC CC_REGNO))])]
  ""
  [(set_attr "length" "8")])

(define_insn "umulhisi3"
  [(set (match_operand:SI 0 "ubicom32_acc_lo_register_operand"	     "=l, l")
	(mult:SI
	  (zero_extend:SI (match_operand:HI 1 "nonimmediate_operand" "%d,rm"))
	  (zero_extend:SI (match_operand:HI 2 "nonimmediate_operand" "rm, d"))))
   (clobber (reg:HI ACC0_HI_REGNO))
   (clobber (reg:HI ACC1_HI_REGNO))]
  ""
  "@
   mulu\\t%A0, %2, %1
   mulu\\t%A0, %1, %2"
  [(set_attr "type" "mul,mul")])

(define_insn "mulhisi3"
  [(set (match_operand:SI 0 "ubicom32_acc_lo_register_operand"	     "=l, l")
	(mult:SI
	  (sign_extend:SI (match_operand:HI 1 "nonimmediate_operand" "%d,rm"))
	  (sign_extend:SI (match_operand:HI 2 "nonimmediate_operand" "rm, d"))))
   (clobber (reg:HI ACC0_HI_REGNO))
   (clobber (reg:HI ACC1_HI_REGNO))]
  ""
  "@
   muls\\t%A0, %2, %1
   muls\\t%A0, %1, %2"
  [(set_attr "type" "mul,mul")])

(define_expand "mulsi3"
  [(set (match_operand:SI 0 "ubicom32_acc_hi_register_operand" "")
	(mult:SI (match_operand:SI 1 "ubicom32_arith_operand" "")
		 (match_operand:SI 2 "ubicom32_arith_operand" "")))]
  ""
  "{
     if (ubicom32_emit_mult_sequence (operands))
       DONE;
   }")

(define_insn "umulsidi3"
  [(set (match_operand:DI 0 "ubicom32_acc_hi_register_operand"	     "=h, h")
	(mult:DI
	  (zero_extend:DI (match_operand:SI 1 "nonimmediate_operand" "%d,rm"))
	  (zero_extend:DI (match_operand:SI 2 "nonimmediate_operand" "rm, d"))))]
  "(ubicom32_v4)"
  "@
   mulu.4\\t%A0, %2, %1
   mulu.4\\t%A0, %1, %2"
  [(set_attr "type" "mul,mul")])

(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "nonimmediate_operand" ""))
   (set (match_operand:DI 2 "ubicom32_acc_hi_register_operand" "")
	(mult:DI
	  (zero_extend:DI (match_dup 0))
	  (zero_extend:DI (match_operand:SI 3 "ubicom32_data_register_operand" ""))))]
  "(peep2_reg_dead_p (2, operands[0])
    || REGNO (operands[0]) == REGNO (operands[2])
    || REGNO (operands[0]) == REGNO (operands[2]) + 1)
   && ! rtx_equal_p (operands[0], operands[3])"
  [(set (match_dup 2)
	(mult:DI
	  (zero_extend:DI (match_dup 1))
	  (zero_extend:DI (match_dup 3))))]
  "")

(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "nonimmediate_operand" ""))
   (set (match_operand:DI 2 "ubicom32_acc_hi_register_operand" "")
	(mult:DI
	  (zero_extend:DI (match_operand:SI 3 "ubicom32_data_register_operand" ""))
	  (zero_extend:DI (match_dup 0))))]
  "(peep2_reg_dead_p (2, operands[0])
    || REGNO (operands[0]) == REGNO (operands[2])
    || REGNO (operands[0]) == REGNO (operands[2]) + 1)
   && ! rtx_equal_p (operands[0], operands[3])"
  [(set (match_dup 2)
	(mult:DI
	  (zero_extend:DI (match_dup 1))
	  (zero_extend:DI (match_dup 3))))]
  "")

(define_insn "umulsidi3_const"
  [(set (match_operand:DI 0 "ubicom32_acc_hi_register_operand"		       "=h")
	(mult:DI
	  (zero_extend:DI (match_operand:SI 1 "ubicom32_data_register_operand" "%d"))
	  (match_operand 2 "const_int_operand"					"I")))]
  "(ubicom32_v4 && satisfies_constraint_I (operands[2]))"
  "mulu.4\\t%A0, %2, %1"
  [(set_attr "type" "mul")])

(define_insn "mulsidi3"
  [(set (match_operand:DI 0 "ubicom32_acc_hi_register_operand"	     "=h, h")
	(mult:DI
	  (sign_extend:DI (match_operand:SI 1 "nonimmediate_operand" "%d,rm"))
	  (sign_extend:DI (match_operand:SI 2 "nonimmediate_operand" "rm, d"))))]
  "(ubicom32_v4)"
  "@
   muls.4\\t%A0, %2, %1
   muls.4\\t%A0, %1, %2"
  [(set_attr "type" "mul,mul")])

(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "nonimmediate_operand" ""))
   (set (match_operand:DI 2 "ubicom32_acc_hi_register_operand" "")
	(mult:DI
	  (sign_extend:DI (match_dup 0))
	  (sign_extend:DI (match_operand:SI 3 "ubicom32_data_register_operand" ""))))]
  "(peep2_reg_dead_p (2, operands[0])
    || REGNO (operands[0]) == REGNO (operands[2])
    || REGNO (operands[0]) == REGNO (operands[2]) + 1)
   && ! rtx_equal_p (operands[0], operands[3])"
  [(set (match_dup 2)
	(mult:DI
	  (sign_extend:DI (match_dup 1))
	  (sign_extend:DI (match_dup 3))))]
  "")

(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "nonimmediate_operand" ""))
   (set (match_operand:DI 2 "ubicom32_acc_hi_register_operand" "")
	(mult:DI
	  (sign_extend:DI (match_operand:SI 3 "ubicom32_data_register_operand" ""))
	  (sign_extend:DI (match_dup 0))))]
  "(peep2_reg_dead_p (2, operands[0])
    || REGNO (operands[0]) == REGNO (operands[2])
    || REGNO (operands[0]) == REGNO (operands[2]) + 1)
   && ! rtx_equal_p (operands[0], operands[3])"
  [(set (match_dup 2)
	(mult:DI
	  (sign_extend:DI (match_dup 1))
	  (sign_extend:DI (match_dup 3))))]
  "")

(define_insn "mulsidi3_const"
  [(set (match_operand:DI 0 "ubicom32_acc_hi_register_operand"		       "=h")
	(mult:DI
	  (sign_extend:DI (match_operand:SI 1 "ubicom32_data_register_operand" "%d"))
	  (match_operand 2 "const_int_operand"					"I")))]
  "(ubicom32_v4 && satisfies_constraint_I (operands[2]))"
  "muls.4\\t%A0, %2, %1"
  [(set_attr "type" "mul")])

(define_expand "andqi3"
  [(parallel
     [(set (match_operand:QI 0 "memory_operand" "")
	   (and:QI (match_operand:QI 1 "nonimmediate_operand" "")
		   (match_operand:QI 2 "ubicom32_arith_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  "(ubicom32_v4)"
  "{
     if (!memory_operand (operands[0], QImode))
       FAIL;

     /* If we have a non-data reg for operand 1 then prefer that over
        a CONST_INT in operand 2.  */
     if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	 && CONST_INT_P (operands[2]))
       operands[2] = copy_to_mode_reg (QImode, operands[2]);

     if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
       operands[2] = copy_to_mode_reg (QImode, operands[2]);
   }")

(define_insn "andqi3_and1"
  [(set (match_operand:QI 0 "memory_operand"		      "=m, m")
	(and:QI (match_operand:QI 1 "nonimmediate_operand"    "%d,rm")
		(match_operand:QI 2 "ubicom32_arith_operand" "rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4)"
  "@
   and.1\\t%0, %2, %1
   and.1\\t%0, %1, %2")

(define_insn "andqi3_and1_ccszn"
  [(set (reg CC_REGNO)
	(compare
	  (and:QI (match_operand:QI 1 "nonimmediate_operand"    "%d,rm")
		  (match_operand:QI 2 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))
   (set (match_operand:QI 0 "memory_operand"		        "=m, m")
	(and:QI (match_dup 1)
		(match_dup 2)))]
  "(ubicom32_v4
    && ubicom32_match_cc_mode(insn, CCSZNmode))"
  "@
   and.1\\t%0, %2, %1
   and.1\\t%0, %1, %2")

(define_insn "andqi3_and1_ccszn_null"
  [(set (reg CC_REGNO)
	(compare
	  (and:QI (match_operand:QI 0 "nonimmediate_operand"    "%d,rm")
		  (match_operand:QI 1 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))]
  "(ubicom32_v4
    && ubicom32_match_cc_mode(insn, CCSZNmode))"
  "@
   and.1\\t#0, %1, %0
   and.1\\t#0, %0, %1")

(define_insn "and1_ccszn_null_1"
  [(set (reg CC_REGNO)
	(compare
	  (subreg:QI
	    (and:SI (match_operand:SI 0 "ubicom32_data_register_operand" "%d")
		    (match_operand:SI 1 "ubicom32_arith_operand"	 "rI"))
	    3)
	  (const_int 0)))]
  "(ubicom32_v4
    && ubicom32_match_cc_mode(insn, CCSZNmode))"
  "and.1\\t#0, %1, %0")

(define_insn "and1_ccszn_null_2"
  [(set (reg CC_REGNO)
	(compare
	  (subreg:QI
	    (and:SI (match_operand:SI 0 "ubicom32_data_register_operand" "d")
		    (subreg:SI
		      (match_operand:QI 1 "memory_operand"		 "m")
		      0))
	    3)
	  (const_int 0)))]
  "(ubicom32_v4
    && ubicom32_match_cc_mode(insn, CCSZNmode))"
  "and.1\\t#0, %1, %0")

(define_insn "and1_ccszn_null_3"
  [(set (reg CC_REGNO)
	(compare
	  (subreg:QI
	    (and:SI (subreg:SI
		      (match_operand:QI 0 "memory_operand"		 "m")
		      0)
		    (match_operand:SI 1 "ubicom32_data_register_operand" "d"))
	    3)
	  (const_int 0)))]
  "(ubicom32_v4
    && ubicom32_match_cc_mode(insn, CCSZNmode))"
  "and.1\\t#0, %0, %1")

(define_expand "andhi3"
  [(parallel
     [(set (match_operand:HI 0 "memory_operand" "")
	   (and:HI (match_operand:HI 1 "nonimmediate_operand" "")
		   (match_operand:HI 2 "ubicom32_arith_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  ""
  "{
     if (!memory_operand (operands[0], HImode))
       FAIL;

     /* If we have a non-data reg for operand 1 then prefer that over
        a CONST_INT in operand 2.  */
     if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	 && CONST_INT_P (operands[2]))
       operands[2] = copy_to_mode_reg (HImode, operands[2]);

     if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
       operands[2] = copy_to_mode_reg (HImode, operands[2]);
   }")

(define_insn "andhi3_and2"
  [(set (match_operand:HI 0 "memory_operand"		      "=m, m")
	(and:HI (match_operand:HI 1 "nonimmediate_operand"    "%d,rm")
		(match_operand:HI 2 "ubicom32_arith_operand" "rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "@
   and.2\\t%0, %2, %1
   and.2\\t%0, %1, %2")

(define_insn "andhi3_and2_ccszn"
  [(set (reg CC_REGNO)
	(compare
	  (and:HI (match_operand:HI 1 "nonimmediate_operand"    "%d,rm")
		  (match_operand:HI 2 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))
   (set (match_operand:HI 0 "memory_operand"		        "=m, m")
	(and:HI (match_dup 1)
		(match_dup 2)))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "@
   and.2\\t%0, %2, %1
   and.2\\t%0, %1, %2")

(define_insn "andhi3_and2_ccszn_null"
  [(set (reg CC_REGNO)
	(compare
	  (and:HI (match_operand:HI 0 "nonimmediate_operand"    "%d,rm")
		  (match_operand:HI 1 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "@
   and.2\\t#0, %1, %0
   and.2\\t#0, %0, %1")

(define_insn "and2_ccszn_null_1"
  [(set (reg CC_REGNO)
	(compare
	  (subreg:HI
	    (and:SI (match_operand:SI 0 "ubicom32_data_register_operand" "%d")
		    (match_operand:SI 1 "ubicom32_arith_operand"	 "rI"))
	    2)
	  (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "and.2\\t#0, %1, %0")

(define_insn "and2_ccszn_null_2"
  [(set (reg CC_REGNO)
	(compare
	  (subreg:HI
	    (and:SI (match_operand:SI 0 "ubicom32_data_register_operand" "d")
		    (subreg:SI
		      (match_operand:HI 1 "memory_operand"		 "m")
		      0))
	    2)
	  (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "and.2\\t#0, %1, %0")

(define_insn "and2_ccszn_null_3"
  [(set (reg CC_REGNO)
	(compare
	  (subreg:HI
	    (and:SI (subreg:SI
		      (match_operand:HI 0 "memory_operand"		 "m")
		      0)
		    (match_operand:SI 1 "ubicom32_data_register_operand" "d"))
	    2)
	  (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "and.2\\t#0, %0, %1")

(define_expand "andsi3"
  [(parallel
     [(set (match_operand:SI 0 "nonimmediate_operand" "")
	   (and:SI (match_operand:SI 1 "nonimmediate_operand" "")
		   (match_operand:SI 2 "ubicom32_and_or_si3_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  ""
  "{
     do
       {
	 /* Is this a bfextu?  */
	 if (ubicom32_data_register_operand (operands[0], SImode)
	     && CONST_INT_P (operands[2])
	     && exact_log2 (INTVAL (operands[2]) + 1) != -1)
	   break;

	 /* Is this a bclr?  */
	 if (CONST_INT_P (operands[2])
	     && exact_log2 (~INTVAL (operands[2])) != -1)
	   break;

	 /* Must be an and.4  */
	 if (!ubicom32_data_register_operand (operands[1], SImode))
	   operands[1] = copy_to_mode_reg (SImode, operands[1]);

	 if (!ubicom32_arith_operand (operands[2], SImode))
	   operands[2] = copy_to_mode_reg (SImode, operands[2]);
       }
     while (0);
   }")

(define_insn "andsi3_bfextu"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand" "=d")
  	(and:SI (match_operand:SI 1 "nonimmediate_operand"  "%rm")
		(match_operand:SI 2 "const_int_operand"	      "O")))
   (clobber (reg:CC CC_REGNO))]
  "(satisfies_constraint_O (operands[2]))"
  "*
   {
     operands[3] = GEN_INT (exact_log2 (INTVAL (operands[2]) + 1));

     return \"bfextu\\t%0, %1, %3\";
   }")

(define_insn "andsi3_bfextu_ccwz"
  [(set (reg CC_REGNO)
	(compare
	  (and:SI (match_operand:SI 1 "nonimmediate_operand" "%rm")
		  (match_operand:SI 2 "const_int_operand"      "O"))
	  (const_int 0)))
   (set (match_operand:SI 0 "ubicom32_data_register_operand"  "=d")
	(and:SI (match_dup 1)
		(match_dup 2)))]
  "(satisfies_constraint_O (operands[2])
    && ubicom32_match_cc_mode(insn, CCWZmode))"
  "*
   {
     operands[3] = GEN_INT (exact_log2 (INTVAL (operands[2]) + 1));

     return \"bfextu\\t%0, %1, %3\";
   }")

(define_insn "andsi3_bfextu_ccwz_null"
  [(set (reg CC_REGNO)
	(compare
	  (and:SI (match_operand:SI 0 "nonimmediate_operand" "%rm")
		  (match_operand:SI 1 "const_int_operand"      "O"))
	  (const_int 0)))
   (clobber (match_scratch:SI 2				      "=d"))]
  "(satisfies_constraint_O (operands[1])
    && ubicom32_match_cc_mode(insn, CCWZmode))"
  "*
   {
     operands[3] = GEN_INT (exact_log2 (INTVAL (operands[1]) + 1));

     return \"bfextu\\t%2, %0, %3\";
   }")

(define_insn "andsi3_bclr"
  [(set (match_operand:SI 0 "nonimmediate_operand"	      "=rm")
	(and:SI (match_operand:SI 1 "ubicom32_arith_operand" "%rmI")
		(match_operand:SI 2 "const_int_operand"	        "n")))
   (clobber (reg:CC CC_REGNO))]
  "(exact_log2 (~INTVAL (operands[2])) != -1)"
  "bclr\\t%0, %1, #%D2")

(define_insn "andsi3_and4"
  [(set (match_operand:SI 0 "nonimmediate_operand"	     "=rm,rm")
	(and:SI (match_operand:SI 1 "nonimmediate_operand"    "%d,rm")
		(match_operand:SI 2 "ubicom32_arith_operand" "rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "@
   and.4\\t%0, %2, %1
   and.4\\t%0, %1, %2")

(define_insn "andsi3_and4_ccwzn"
  [(set (reg CC_REGNO)
	(compare
	  (and:SI (match_operand:SI 1 "nonimmediate_operand"    "%d,rm")
		  (match_operand:SI 2 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))
   (set (match_operand:SI 0 "nonimmediate_operand"	       "=rm,rm")
	(and:SI (match_dup 1)
		(match_dup 2)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "@
   and.4\\t%0, %2, %1
   and.4\\t%0, %1, %2")

(define_insn "andsi3_and4_ccwzn_null"
  [(set (reg CC_REGNO)
	(compare
	  (and:SI (match_operand:SI 0 "nonimmediate_operand"    "%d,rm")
		  (match_operand:SI 1 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "@
   and.4\\t#0, %1, %0
   and.4\\t#0, %0, %1")

(define_insn "andsi3_lsr4_ccwz_null"
  [(set (reg CC_REGNO)
	(compare
	  (and:SI (match_operand:SI 0 "nonimmediate_operand" "%rm")
		  (match_operand:SI 1 "const_int_operand"      "n"))
	  (const_int 0)))
   (clobber (match_scratch:SI 2				      "=d"))]
  "(exact_log2 ((~(INTVAL (operands[1]))) + 1) != -1
    && ubicom32_match_cc_mode(insn, CCWZmode))"
  "*
   {
     operands[3] = GEN_INT (exact_log2 ((~(INTVAL (operands[1]))) + 1));

     return \"lsr.4\\t%2, %0, %3\";
   }")

; We really would like the combiner to recognize this scenario and deal with
; it but unfortunately it tries to canonicalize zero_extract ops on MEMs
; into QImode operations and we can't match them in any useful way.
;
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(match_operand:SI 1 "const_int_operand" ""))
   (set (reg:CCWZ CC_REGNO)
	(compare:CCWZ
	  (and:SI (match_operand:SI 2 "nonimmediate_operand" "")
		  (match_dup 0))
	  (const_int 0)))]
  "(exact_log2 (INTVAL (operands[1])) != -1
    && peep2_reg_dead_p (2, operands[0]))"
  [(set (reg:CCWZ CC_REGNO)
	(compare:CCWZ
	  (zero_extract:SI
	    (match_dup 2)
	    (const_int 1)
	    (match_dup 3))
	  (const_int 0)))]
  "{
     operands[3] = GEN_INT (exact_log2 (INTVAL (operands[1])));
   }")

(define_expand "anddi3"
  [(parallel
     [(set (match_operand:DI 0 "nonimmediate_operand" "")
	   (and:DI (match_operand:DI 1 "nonimmediate_operand" "")
		   (match_operand:DI 2 "ubicom32_arith_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  ""
  "{
     /* If we have a non-data reg for operand 1 then prefer that over
        a CONST_INT in operand 2.  */
     if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	 && CONST_INT_P (operands[2]))
       operands[2] = copy_to_mode_reg (DImode, operands[2]);

     if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
       operands[2] = copy_to_mode_reg (DImode, operands[2]);
   }")

(define_insn_and_split "anddi3_and4"
  [(set (match_operand:DI 0 "nonimmediate_operand"	     "=&r,&r,  d,rm,  m, m")
	(and:DI (match_operand:DI 1 "nonimmediate_operand"    "%d,rm,  0, 0,  d,rm")
		(match_operand:DI 2 "ubicom32_arith_operand" "rmI, d,rmI, d,rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "#"
  "reload_completed"
  [(parallel [(set (match_dup 3)
		   (and:SI (match_dup 4)
			   (match_dup 5)))
	      (clobber (reg:CC CC_REGNO))])
   (parallel [(set (match_dup 6)
		   (and:SI (match_dup 7)
			   (match_dup 8)))
	      (clobber (reg:CC CC_REGNO))])]
  "{
     operands[3] = gen_lowpart (SImode, operands[0]);
     operands[4] = gen_lowpart (SImode, operands[1]);
     operands[5] = gen_lowpart (SImode, operands[2]);
     operands[6] = gen_highpart (SImode, operands[0]);
     operands[7] = gen_highpart (SImode, operands[1]);
     operands[8] = gen_highpart_mode (SImode, DImode, operands[2]);
   }"
  [(set_attr "length" "8")])

(define_expand "iorqi3"
  [(parallel
     [(set (match_operand:QI 0 "memory_operand" "")
	   (ior:QI (match_operand:QI 1 "nonimmediate_operand" "")
		   (match_operand:QI 2 "ubicom32_arith_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  "(ubicom32_v4)"
  "{
     if (!memory_operand (operands[0], QImode))
       FAIL;

     /* If we have a non-data reg for operand 1 then prefer that over
        a CONST_INT in operand 2.  */
     if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	 && CONST_INT_P (operands[2]))
       operands[2] = copy_to_mode_reg (QImode, operands[2]);

     if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
       operands[2] = copy_to_mode_reg (QImode, operands[2]);
   }")

(define_insn "iorqi3_or1"
  [(set (match_operand:QI 0 "memory_operand"		      "=m, m")
	(ior:QI (match_operand:QI 1 "nonimmediate_operand"    "%d,rm")
		(match_operand:QI 2 "ubicom32_arith_operand" "rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4)"
  "@
   or.1\\t%0, %2, %1
   or.1\\t%0, %1, %2")

(define_expand "iorhi3"
  [(parallel
     [(set (match_operand:HI 0 "memory_operand" "")
	   (ior:HI (match_operand:HI 1 "nonimmediate_operand" "")
		   (match_operand:HI 2 "ubicom32_arith_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  ""
  "{
     if (!memory_operand (operands[0], HImode))
       FAIL;

     /* If we have a non-data reg for operand 1 then prefer that over
        a CONST_INT in operand 2.  */
     if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	 && CONST_INT_P (operands[2]))
       operands[2] = copy_to_mode_reg (HImode, operands[2]);

     if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
       operands[2] = copy_to_mode_reg (HImode, operands[2]);
   }")

(define_insn "iorhi3_or2"
  [(set (match_operand:HI 0 "memory_operand"		      "=m, m")
	(ior:HI (match_operand:HI 1 "nonimmediate_operand"    "%d,rm")
		(match_operand:HI 2 "ubicom32_arith_operand" "rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "@
   or.2\\t%0, %2, %1
   or.2\\t%0, %1, %2")

(define_expand "iorsi3"
  [(parallel
     [(set (match_operand:SI 0 "nonimmediate_operand" "")
	   (ior:SI (match_operand:SI 1 "nonimmediate_operand" "")
		   (match_operand:SI 2 "ubicom32_and_or_si3_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  ""
  "{
     do
       {
	 /* Is this a bset?  */
	 if (CONST_INT_P (operands[2])
	     && exact_log2 (INTVAL (operands[2])) != -1)
	   break;

	 /* Must be an or.4  */
	 if (!ubicom32_data_register_operand (operands[1], SImode))
	   operands[1] = copy_to_mode_reg (SImode, operands[1]);

	 if (!ubicom32_arith_operand (operands[2], SImode))
	   operands[2] = copy_to_mode_reg (SImode, operands[2]);
       } 
     while (0);
   }")

(define_insn "iorsi3_bset"
  [(set (match_operand:SI 0 "nonimmediate_operand"	      "=rm")
  	(ior:SI (match_operand:SI 1 "ubicom32_arith_operand" "%rmI")
		(match_operand 2 "const_int_operand"	        "n")))
   (clobber (reg:CC CC_REGNO))]
  "(exact_log2 (INTVAL (operands[2])) != -1)"
  "bset\\t%0, %1, #%d2")

(define_insn "iorsi3_or4"
  [(set (match_operand:SI 0 "nonimmediate_operand"	     "=rm,rm")
	(ior:SI (match_operand:SI 1 "nonimmediate_operand"    "%d,rm")
		(match_operand:SI 2 "ubicom32_arith_operand" "rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "@
   or.4\\t%0, %2, %1
   or.4\\t%0, %1, %2")

(define_insn "iorsi3_ccwzn"
  [(set (reg CC_REGNO)
	(compare
	  (ior:SI (match_operand:SI 1 "nonimmediate_operand"    "%d,rm")
		  (match_operand:SI 2 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))
   (set (match_operand:SI 0 "nonimmediate_operand"	       "=rm,rm")
	(ior:SI (match_dup 1)
		(match_dup 2)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "@
   or.4\\t%0, %2, %1
   or.4\\t%0, %1, %2")

(define_insn "iorsi3_ccwzn_null"
  [(set (reg CC_REGNO)
	(compare
	  (ior:SI (match_operand:SI 0 "nonimmediate_operand"    "%d,rm")
		  (match_operand:SI 1 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "@
   or.4\\t#0, %1, %0
   or.4\\t#0, %0, %1")

(define_expand "iordi3"
  [(parallel
     [(set (match_operand:DI 0 "nonimmediate_operand" "")
	   (ior:DI (match_operand:DI 1 "nonimmediate_operand" "")
		   (match_operand:DI 2 "ubicom32_arith_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  ""
  "{
     /* If we have a non-data reg for operand 1 then prefer that over
        a CONST_INT in operand 2.  */
     if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	 && CONST_INT_P (operands[2]))
       operands[2] = copy_to_mode_reg (DImode, operands[2]);

     if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
       operands[2] = copy_to_mode_reg (DImode, operands[2]);
   }")

(define_insn_and_split "iordi3_or4"
  [(set (match_operand:DI 0 "nonimmediate_operand"	     "=&r,&r,  d,rm,  m, m")
	(ior:DI (match_operand:DI 1 "nonimmediate_operand"    "%d,rm,  0, 0,  d,rm")
		(match_operand:DI 2 "ubicom32_arith_operand" "rmI, d,rmI, d,rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "#"
  "reload_completed"
  [(parallel [(set (match_dup 3)
		   (ior:SI (match_dup 4)
			   (match_dup 5)))
	      (clobber (reg:CC CC_REGNO))])
   (parallel [(set (match_dup 6)
		   (ior:SI (match_dup 7)
			   (match_dup 8)))
	      (clobber (reg:CC CC_REGNO))])]
  "{
     operands[3] = gen_lowpart (SImode, operands[0]);
     operands[4] = gen_lowpart (SImode, operands[1]);
     operands[5] = gen_lowpart (SImode, operands[2]);
     operands[6] = gen_highpart (SImode, operands[0]);
     operands[7] = gen_highpart (SImode, operands[1]);
     operands[8] = gen_highpart_mode (SImode, DImode, operands[2]);
   }"
  [(set_attr "length" "8")])

(define_expand "xorqi3"
  [(parallel
     [(set (match_operand:QI 0 "memory_operand" "")
	   (xor:QI (match_operand:QI 1 "nonimmediate_operand" "")
		   (match_operand:QI 2 "ubicom32_arith_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  "(ubicom32_v4)"
  "{
     if (!memory_operand (operands[0], QImode))
       FAIL;

     /* If we have a non-data reg for operand 1 then prefer that over
        a CONST_INT in operand 2.  */
     if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	 && CONST_INT_P (operands[2]))
       operands[2] = copy_to_mode_reg (QImode, operands[2]);

     if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
       operands[2] = copy_to_mode_reg (QImode, operands[2]);
   }")

(define_insn "xorqi3_xor1"
  [(set (match_operand:QI 0 "memory_operand"		      "=m, m")
	(xor:QI (match_operand:QI 1 "nonimmediate_operand"    "%d,rm")
		(match_operand:QI 2 "ubicom32_arith_operand" "rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4)"
  "@
   xor.1\\t%0, %2, %1
   xor.1\\t%0, %1, %2")

(define_insn "xorqi3_xor1_ccszn"
  [(set (reg CC_REGNO)
	(compare
	  (xor:QI (match_operand:QI 1 "nonimmediate_operand"    "%d,rm")
		  (match_operand:QI 2 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))
   (set (match_operand:QI 0 "memory_operand"		        "=m, m")
	(xor:QI (match_dup 1)
		(match_dup 2)))]
  "(ubicom32_v4
    && ubicom32_match_cc_mode(insn, CCSZNmode))"
  "@
   xor.1\\t%0, %2, %1
   xor.1\\t%0, %1, %2")

(define_insn "xorqi3_xor1_ccszn_null"
  [(set (reg CC_REGNO)
	(compare
	  (xor:QI (match_operand:QI 0 "nonimmediate_operand"    "%d,rm")
		  (match_operand:QI 1 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))]
  "(ubicom32_v4
    && ubicom32_match_cc_mode(insn, CCSZNmode))"
  "@
   xor.1\\t#0, %1, %0
   xor.1\\t#0, %0, %1")

(define_insn "xor1_ccszn_null_1"
  [(set (reg CC_REGNO)
	(compare
	  (subreg:QI
	    (xor:SI (match_operand:SI 0 "ubicom32_data_register_operand" "%d")
		    (match_operand:SI 1 "ubicom32_arith_operand"	 "rI"))
	    3)
	  (const_int 0)))]
  "(ubicom32_v4
    && ubicom32_match_cc_mode(insn, CCSZNmode))"
  "xor.1\\t#0, %1, %0")

(define_insn "xor1_ccszn_null_2"
  [(set (reg CC_REGNO)
	(compare
	  (subreg:QI
	    (xor:SI (match_operand:SI 0 "ubicom32_data_register_operand" "d")
		    (subreg:SI
		      (match_operand:QI 1 "memory_operand"		 "m")
		      0))
	    3)
	  (const_int 0)))]
  "(ubicom32_v4
    && ubicom32_match_cc_mode(insn, CCSZNmode))"
  "xor.1\\t#0, %1, %0")

(define_insn "xor1_ccwzn_null_3"
  [(set (reg CC_REGNO)
	(compare
	  (subreg:QI
	    (xor:SI (subreg:SI
		      (match_operand:QI 0 "memory_operand"		 "m")
		      0)
		    (match_operand:SI 1 "ubicom32_data_register_operand" "d"))
	    3)
	  (const_int 0)))]
  "(ubicom32_v4
    && ubicom32_match_cc_mode(insn, CCSZNmode))"
  "xor.1\\t#0, %0, %1")

(define_expand "xorhi3"
  [(parallel
     [(set (match_operand:HI 0 "memory_operand" "")
	   (xor:HI (match_operand:HI 1 "nonimmediate_operand" "")
		   (match_operand:HI 2 "ubicom32_arith_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  ""
  "{
     if (!memory_operand (operands[0], HImode))
       FAIL;

     /* If we have a non-data reg for operand 1 then prefer that over
        a CONST_INT in operand 2.  */
     if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	 && CONST_INT_P (operands[2]))
       operands[2] = copy_to_mode_reg (HImode, operands[2]);

     if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
       operands[2] = copy_to_mode_reg (HImode, operands[2]);
   }")

(define_insn "xorhi3_xor2"
  [(set (match_operand:HI 0 "memory_operand"		      "=m, m")
	(xor:HI (match_operand:HI 1 "nonimmediate_operand"    "%d,rm")
		(match_operand:HI 2 "ubicom32_arith_operand" "rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "@
   xor.2\\t%0, %2, %1
   xor.2\\t%0, %1, %2")

(define_insn "xorhi3_xor2_ccszn"
  [(set (reg CC_REGNO)
	(compare
	  (xor:HI (match_operand:HI 1 "nonimmediate_operand"    "%d,rm")
		  (match_operand:HI 2 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))
   (set (match_operand:HI 0 "memory_operand"		        "=m, m")
	(xor:HI (match_dup 1)
		(match_dup 2)))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "@
   xor.2\\t%0, %2, %1
   xor.2\\t%0, %1, %2")

(define_insn "xorhi3_xor2_ccszn_null"
  [(set (reg CC_REGNO)
	(compare
	  (xor:HI (match_operand:HI 0 "nonimmediate_operand"    "%d,rm")
		  (match_operand:HI 1 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "@
   xor.2\\t#0, %1, %0
   xor.2\\t#0, %0, %1")

(define_insn "xor2_ccszn_null_1"
  [(set (reg CC_REGNO)
	(compare
	  (subreg:HI
	    (xor:SI (match_operand:SI 0 "ubicom32_data_register_operand" "%d")
		    (match_operand:SI 1 "ubicom32_arith_operand"	 "rI"))
	    2)
	  (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "xor.2\\t#0, %1, %0")

(define_insn "xor2_ccszn_null_2"
  [(set (reg CC_REGNO)
	(compare
	  (subreg:HI
	    (xor:SI (match_operand:SI 0 "ubicom32_data_register_operand" "d")
		    (subreg:SI
		      (match_operand:HI 1 "memory_operand"		 "m")
		      0))
	    2)
	  (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "xor.2\\t#0, %1, %0")

(define_insn "xor2_ccszn_null_3"
  [(set (reg CC_REGNO)
	(compare
	  (subreg:HI
	    (xor:SI (subreg:SI
		      (match_operand:HI 0 "memory_operand"		 "m")
		      0)
		    (match_operand:SI 1 "ubicom32_data_register_operand" "d"))
	    2)
	  (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCSZNmode)"
  "xor.2\\t#0, %0, %1")

(define_insn "xorsi3"
  [(set (match_operand:SI 0 "nonimmediate_operand"	     "=rm,rm")
	(xor:SI (match_operand:SI 1 "nonimmediate_operand"    "%d,rm")
		(match_operand:SI 2 "ubicom32_arith_operand" "rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "@
   xor.4\\t%0, %2, %1
   xor.4\\t%0, %1, %2")

(define_insn "xorsi3_ccwzn"
  [(set (reg CC_REGNO)
	(compare
	  (xor:SI (match_operand:SI 1 "nonimmediate_operand"    "%d,rm")
		  (match_operand:SI 2 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))
   (set (match_operand:SI 0 "nonimmediate_operand"	       "=rm,rm")
	(xor:SI (match_dup 1)
		(match_dup 2)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "@
   xor.4\\t%0, %2, %1
   xor.4\\t%0, %1, %2")

(define_insn "xorsi3_ccwzn_null"
  [(set (reg CC_REGNO)
	(compare
	  (xor:SI (match_operand:SI 0 "nonimmediate_operand"    "%d,rm")
		  (match_operand:SI 1 "ubicom32_arith_operand" "rmI, d"))
	  (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "@
   xor.4\\t#0, %1, %0
   xor.4\\t#0, %0, %1")

(define_expand "xordi3"
  [(parallel
     [(set (match_operand:DI 0 "nonimmediate_operand" "")
	   (xor:DI (match_operand:DI 1 "nonimmediate_operand" "")
		   (match_operand:DI 2 "ubicom32_arith_operand" "")))
      (clobber (reg:CC CC_REGNO))])]
  ""
  "{
     /* If we have a non-data reg for operand 1 then prefer that over
        a CONST_INT in operand 2.  */
     if (! ubicom32_data_register_operand (operands[1], GET_MODE (operands[1]))
	 && CONST_INT_P (operands[2]))
       operands[2] = copy_to_mode_reg (DImode, operands[2]);

     if (CONST_INT_P (operands[2]) && ! satisfies_constraint_I (operands[2]))
       operands[2] = copy_to_mode_reg (DImode, operands[2]);
   }")

(define_insn_and_split "xordi3_xor4"
  [(set (match_operand:DI 0 "nonimmediate_operand"	     "=&r,&r,  d,rm,  m, m")
	(xor:DI (match_operand:DI 1 "nonimmediate_operand"    "%d,rm,  0, 0,  d,rm")
		(match_operand:DI 2 "ubicom32_arith_operand" "rmI, d,rmI, d,rmI, d")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "#"
  "reload_completed"
  [(parallel [(set (match_dup 3)
		   (xor:SI (match_dup 4)
			   (match_dup 5)))
	      (clobber (reg:CC CC_REGNO))])
   (parallel [(set (match_dup 6)
		   (xor:SI (match_dup 7)
			   (match_dup 8)))
	      (clobber (reg:CC CC_REGNO))])]
  "{
     operands[3] = gen_lowpart (SImode, operands[0]);
     operands[4] = gen_lowpart (SImode, operands[1]);
     operands[5] = gen_lowpart (SImode, operands[2]);
     operands[6] = gen_highpart (SImode, operands[0]);
     operands[7] = gen_highpart (SImode, operands[1]);
     operands[8] = gen_highpart_mode (SImode, DImode, operands[2]);
   }"
  [(set_attr "length" "8")])

(define_insn "not2_2"
  [(set (match_operand:HI 0 "memory_operand"		        "=m")
	(subreg:HI
	  (not:SI (match_operand:SI 1 "ubicom32_arith_operand" "rmI"))
	  2))
   (clobber (reg:CC CC_REGNO))]
  ""
  "not.2\\t%0, %1")

(define_insn "one_cmplsi2"
  [(set (match_operand:SI 0 "nonimmediate_operand"	     "=rm")
	(not:SI (match_operand:SI 1 "ubicom32_arith_operand" "rmI")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "not.4\\t%0, %1")

(define_insn "one_cmplsi2_ccwzn"
  [(set (reg CC_REGNO)
	(compare
	  (not:SI (match_operand:SI 1 "ubicom32_arith_operand" "rmI"))
	  (const_int 0)))
   (set (match_operand:SI 0 "nonimmediate_operand"	       "=rm")
	(not:SI (match_dup 1)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "not.4\\t%0, %1")

(define_insn "one_cmplsi2_ccwzn_null"
  [(set (reg CC_REGNO)
	(compare
	  (not:SI (match_operand:SI 0 "ubicom32_arith_operand" "rmI"))
	  (const_int 0)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "not.4\\t#0, %0")

(define_insn_and_split "one_cmpldi2"
  [(set (match_operand:DI 0 "nonimmediate_operand"	   "=&rm")
	(not:DI (match_operand:DI 1 "nonimmediate_operand" "rmI0")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "#"
  ""
  [(parallel [(set (match_dup 2)
		   (not:SI (match_dup 3)))
	      (clobber (reg:CC CC_REGNO))])
   (parallel [(set (match_dup 4)
		   (not:SI (match_dup 5)))
	      (clobber (reg:CC CC_REGNO))])]
  "{
     operands[2] = gen_lowpart (SImode, operands[0]);
     operands[3] = gen_lowpart (SImode, operands[1]);
     operands[4] = gen_highpart (SImode, operands[0]);
     operands[5] = gen_highpart (SImode, operands[1]);
   }"
  [(set_attr "length" "8")])

; Conditional jump instructions

(define_expand "beq"
  [(set (pc)
	(if_then_else (eq (match_dup 1)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "{
     operands[1] = ubicom32_gen_compare_reg (EQ, ubicom32_compare_op0,
					     ubicom32_compare_op1);
   }")

(define_expand "bne"
  [(set (pc)
	(if_then_else (ne (match_dup 1)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "{
     operands[1] = ubicom32_gen_compare_reg (NE, ubicom32_compare_op0,
					     ubicom32_compare_op1);
   }")

(define_expand "bgt"
  [(set (pc)
	(if_then_else (gt (match_dup 1)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "{
     operands[1] = ubicom32_gen_compare_reg (GT, ubicom32_compare_op0,
					     ubicom32_compare_op1);
   }")

(define_expand "ble"
  [(set (pc)
	(if_then_else (le (match_dup 1)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "{
     operands[1] = ubicom32_gen_compare_reg (LE, ubicom32_compare_op0,
					     ubicom32_compare_op1);
   }")

(define_expand "bge"
  [(set (pc)
	(if_then_else (ge (match_dup 1)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "{
     operands[1] = ubicom32_gen_compare_reg (GE, ubicom32_compare_op0,
					     ubicom32_compare_op1);
   }")

(define_expand "blt"
  [(set (pc)
	(if_then_else (lt (match_dup 1)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "{
     operands[1] = ubicom32_gen_compare_reg (LT, ubicom32_compare_op0,
					     ubicom32_compare_op1);
   }")

(define_expand "bgtu"
  [(set (pc)
	(if_then_else (gtu (match_dup 1)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "{
     operands[1] = ubicom32_gen_compare_reg (GTU, ubicom32_compare_op0,
					     ubicom32_compare_op1);
   }")

(define_expand "bleu"
  [(set (pc)
	(if_then_else (leu (match_dup 1)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "{
     operands[1] = ubicom32_gen_compare_reg (LEU, ubicom32_compare_op0,
					     ubicom32_compare_op1);
   }")

(define_expand "bgeu"
  [(set (pc)
	(if_then_else (geu (match_dup 1)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "{
     operands[1] = ubicom32_gen_compare_reg (GEU, ubicom32_compare_op0,
					     ubicom32_compare_op1);
   }")

(define_expand "bltu"
  [(set (pc)
	(if_then_else (ltu (match_dup 1)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "{
     operands[1] = ubicom32_gen_compare_reg (LTU, ubicom32_compare_op0,
					     ubicom32_compare_op1);
   }")

(define_insn "jcc"
  [(set (pc)
	(if_then_else (match_operator 1 "comparison_operator"
			[(match_operand 2 "ubicom32_cc_register_operand" "")
			 (const_int 0)])
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "*
   {
     ubicom32_output_cond_jump (insn, operands[1], operands[0]);
     return \"\";
   }")

; Reverse branch - reverse our comparison condition so that we can
; branch in the opposite sense.
;
(define_insn_and_split "jcc_reverse"
  [(set (pc)
	(if_then_else (match_operator 1 "comparison_operator"
			[(match_operand 2 "ubicom32_cc_register_operand" "")
			 (const_int 0)])
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "#"
  "reload_completed"
  [(set (pc)
	(if_then_else (match_dup 3)
		      (label_ref (match_dup 0))
		      (pc)))]
  "{
     rtx cc_reg;

     cc_reg = gen_rtx_REG (GET_MODE (operands[2]), CC_REGNO);
     operands[3] = gen_rtx_fmt_ee (reverse_condition (GET_CODE (operands[1])),
	 			   GET_MODE (operands[1]),
				   cc_reg,
				   const0_rtx);
   }")

(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  ""
  "jmpt\\t%l0")

(define_expand "indirect_jump"
  [(parallel [(set (pc)
  		   (match_operand:SI 0 "register_operand" ""))
	      (clobber (match_dup 0))])]
  ""
  "")

(define_insn "indirect_jump_internal"
  [(set (pc)
 	(match_operand:SI 0 "register_operand" "a"))
  (clobber (match_dup 0))]
  ""
  "calli\\t%0,0(%0)")

; Program Space: The table contains instructions, typically jumps. 
; CALL An,TABLE_SIZE(PC)	;An = Jump Table Base Address. 
; <Jump Table is Here>	;An -> Here. 
; LEA Ak, (An,Dn) 	;Ak -> Table Entry
; JMP/CALL (Ak) 

(define_expand "tablejump"
  [(parallel [(set (pc)
  		   (match_operand:SI 0 "nonimmediate_operand" ""))
	      (use (label_ref (match_operand 1 "" "")))])]
  ""
  "")

(define_insn "tablejump_internal"
  [(set (pc)
	(match_operand:SI 0 "nonimmediate_operand" "rm"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
  "ret\\t%0")

; Call subroutine with no return value.
;
(define_expand "call"
  [(call (match_operand:QI 0 "general_operand" "")
	 (match_operand:SI 1 "general_operand" ""))]
  ""
  "{
     if (TARGET_FDPIC)
       {
	 ubicom32_expand_call_fdpic (operands);
	 DONE;
       }

     if (! ubicom32_call_address_operand (XEXP (operands[0], 0), VOIDmode))
       XEXP (operands[0], 0) = force_reg (SImode, XEXP (operands[0], 0));
   }")

; We expand to a simple form that doesn't clobber the link register and
; then split to a form that does.  This allows the RTL optimizers that
; run before the splitter to have the opportunity to eliminate the call
; without marking A5 as being clobbered and this in turn avoids saves
; and returns in a number of cases.
;
(define_insn_and_split "call_1"
  [(call (mem:QI (match_operand:SI 0 "ubicom32_call_address_operand" "a,S"))
	 (match_operand:SI 1 "general_operand"			     "g,g"))]
  "! TARGET_FDPIC"
  "#"
  ""
  [(parallel
     [(call (mem:QI (match_dup 0))
	    (match_dup 1))
      (clobber (reg:SI LINK_REGNO))])]
  "")

(define_insn "call_slow"
  [(call (mem:QI (match_operand:SI 0 "ubicom32_call_address_operand" "a,S"))
	 (match_operand:SI 1 "general_operand"			     "g,g"))
   (clobber (reg:SI LINK_REGNO))]
  "(! TARGET_FDPIC && ! TARGET_FASTCALL)"
  "@
   calli\\ta5, 0(%0)
   moveai\\ta5, #%%hi(%C0)\;calli\\ta5, %%lo(%C0)(a5)")

(define_insn "call_fast"
  [(call (mem:QI (match_operand:SI 0 "ubicom32_call_address_operand" "a,S"))
	 (match_operand:SI 1 "general_operand"			     "g,g"))
   (clobber (reg:SI LINK_REGNO))]
  "(! TARGET_FDPIC && TARGET_FASTCALL)"
  "@
   calli\\ta5, 0(%0)
   call\\ta5, %C0")

; We expand to a simple form that doesn't clobber the link register and
; then split to a form that does.  This allows the RTL optimizers that
; run before the splitter to have the opportunity to eliminate the call
; without marking A5 as being clobbered and this in turn avoids saves
; and returns in a number of cases.
;
(define_insn_and_split "call_fdpic"
  [(call (mem:QI (match_operand:SI 0 "ubicom32_call_address_operand" "a,S"))
	 (match_operand:SI 1 "general_operand"			     "g,g"))
   (use (match_operand:SI 2 "ubicom32_fdpic_operand"		     "Z,Z"))]
  "TARGET_FDPIC"
  "#"
  ""
  [(parallel
     [(call (mem:QI (match_dup 0))
	    (match_dup 1))
      (use (match_dup 2))
      (clobber (reg:SI LINK_REGNO))])]
  "")

(define_insn "call_fdpic_clobber"
  [(call (mem:QI (match_operand:SI 0 "ubicom32_call_address_operand" "a,S"))
	 (match_operand:SI 1 "general_operand"			     "g,g"))
   (use (match_operand:SI 2 "ubicom32_fdpic_operand"		     "Z,Z"))
   (clobber (reg:SI LINK_REGNO))]
  "TARGET_FDPIC"
  "@
   move.4\\ta5, 0(%0)\;move.4\\t%2, 4(%0)\;calli\\ta5, 0(a5)
   call\\ta5, %C0")

; Call subroutine, returning value in operand 0
; (which must be a hard register).
;
(define_expand "call_value"
  [(set (match_operand 0 "" "")
	(call (match_operand:QI 1 "general_operand" "")
	      (match_operand:SI 2 "general_operand" "")))]
  ""
  "{
     if (TARGET_FDPIC)
       {
	 ubicom32_expand_call_value_fdpic (operands);
	 DONE;
       }

     if (! ubicom32_call_address_operand (XEXP (operands[1], 0), VOIDmode))
       XEXP (operands[1], 0) = force_reg (SImode, XEXP (operands[1], 0));
   }")

; We expand to a simple form that doesn't clobber the link register and
; then split to a form that does.  This allows the RTL optimizers that
; run before the splitter to have the opportunity to eliminate the call
; without marking A5 as being clobbered and this in turn avoids saves
; and returns in a number of cases.
;
(define_insn_and_split "call_value_1"
  [(set (match_operand 0 "register_operand"				 "=r,r")
	(call (mem:QI (match_operand:SI 1 "ubicom32_call_address_operand" "a,S"))
	      (match_operand:SI 2 "general_operand"			  "g,g")))]
  "! TARGET_FDPIC"
  "#"
  ""
  [(parallel
     [(set (match_dup 0)
	   (call (mem:QI (match_dup 1))
		 (match_dup 2)))
      (clobber (reg:SI LINK_REGNO))])]
  "")

(define_insn "call_value_slow"
  [(set (match_operand 0 "register_operand"				 "=r,r")
	(call (mem:QI (match_operand:SI 1 "ubicom32_call_address_operand" "a,S"))
	      (match_operand:SI 2 "general_operand"			  "g,g")))
   (clobber (reg:SI LINK_REGNO))]
  "(! TARGET_FDPIC && ! TARGET_FASTCALL)"
  "@
   calli\\ta5, 0(%1)
   moveai\\ta5, #%%hi(%C1)\;calli\\ta5, %%lo(%C1)(a5)")

(define_insn "call_value_fast"
  [(set (match_operand 0 "register_operand"				 "=r,r")
	(call (mem:QI (match_operand:SI 1 "ubicom32_call_address_operand" "a,S"))
	      (match_operand:SI 2 "general_operand"			  "g,g")))
   (clobber (reg:SI LINK_REGNO))]
  "(! TARGET_FDPIC && TARGET_FASTCALL)"
  "@
   calli\\ta5, 0(%1)
   call\\ta5, %C1")

; We expand to a simple form that doesn't clobber the link register and
; then split to a form that does.  This allows the RTL optimizers that
; run before the splitter to have the opportunity to eliminate the call
; without marking A5 as being clobbered and this in turn avoids saves
; and returns in a number of cases.
;
(define_insn_and_split "call_value_fdpic"
  [(set (match_operand 0 "register_operand"				 "=r,r")
	(call (mem:QI (match_operand:SI 1 "ubicom32_call_address_operand" "a,S"))
	      (match_operand:SI 2 "general_operand"			  "g,g")))
   (use (match_operand:SI 3 "ubicom32_fdpic_operand"			  "Z,Z"))]
  "TARGET_FDPIC"
  "#"
  ""
  [(parallel
     [(set (match_dup 0)
	   (call (mem:QI (match_dup 1))
		 (match_dup 2)))
      (use (match_dup 3))
      (clobber (reg:SI LINK_REGNO))])]
  "")

(define_insn "call_value_fdpic_clobber"
  [(set (match_operand 0 "register_operand"				 "=r,r")
	(call (mem:QI (match_operand:SI 1 "ubicom32_call_address_operand" "a,S"))
	      (match_operand:SI 2 "general_operand"			  "g,g")))
   (use (match_operand:SI 3 "ubicom32_fdpic_operand"			  "Z,Z"))
   (clobber (reg:SI LINK_REGNO))]
  "TARGET_FDPIC"
  "@
   move.4\\ta5, 0(%1)\;move.4\\t%3, 4(%1)\;calli\\ta5, 0(a5)
   call\\ta5, %C1")

(define_expand "untyped_call"
  [(parallel [(call (match_operand 0 "" "")
                    (const_int 0))
              (match_operand 1 "" "")
              (match_operand 2 "" "")])]
  ""
  "{
     int i;

     emit_call_insn (gen_call (operands[0], const0_rtx));

     for (i = 0; i < XVECLEN (operands[2], 0); i++)
       {
         rtx set = XVECEXP (operands[2], 0, i);
         emit_move_insn (SET_DEST (set), SET_SRC (set));
       }
     DONE;
   }")

(define_insn "lsl1_1"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	 "=d")
	(ashift:SI (subreg:SI
		     (match_operand:QI 1 "memory_operand"	  "m")
		     0)
		   (match_operand:SI 2 "ubicom32_arith_operand"  "dM")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4)"
  "lsl.1\\t%0, %1, %2")

; The combiner gets rather creative about left shifts of sub-word memory
; operands because it's uncertain about whether the memory is sign or
; zero extended.  It only wants zero-extended behaviour and so throws
; in an extra and operation.
;
(define_insn "lsl1_2"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand" "=d")
	(and:SI
	  (ashift:SI (subreg:SI
		       (match_operand:QI 1 "memory_operand"   "m")
		       0)
		     (match_operand:SI 2 "const_int_operand"  "M"))
	  (match_operand:SI 3 "const_int_operand"	      "n")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4
    && INTVAL (operands[3]) == (0xff << INTVAL (operands[2])))"
  "lsl.1\\t%0, %1, %2")

(define_insn "lsl2_1"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	 "=d")
	(ashift:SI (subreg:SI
		     (match_operand:HI 1 "memory_operand"	  "m")
		     0)
		   (match_operand:SI 2 "ubicom32_arith_operand"  "dM")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4)"
  "lsl.2\\t%0, %1, %2")

; The combiner gets rather creative about left shifts of sub-word memory
; operands because it's uncertain about whether the memory is sign or
; zero extended.  It only wants zero-extended behaviour and so throws
; in an extra and operation.
;
(define_insn "lsl2_2"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand" "=d")
	(and:SI
	  (ashift:SI (subreg:SI
		       (match_operand:HI 1 "memory_operand"   "m")
		       0)
		     (match_operand:SI 2 "const_int_operand"  "M"))
	  (match_operand:SI 3 "const_int_operand"	      "n")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4
    && INTVAL (operands[3]) == (0xffff << INTVAL (operands[2])))"
  "lsl.2\\t%0, %1, %2")

(define_insn "ashlsi3"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	 "=d")
	(ashift:SI (match_operand:SI 1 "ubicom32_arith_operand" "rmI")
		   (match_operand:SI 2 "ubicom32_arith_operand"  "dM")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "lsl.4\\t%0, %1, %2")

(define_insn "lshlsi3_ccwz"
  [(set (reg CC_REGNO)
	(compare
	  (ashift:SI (match_operand:SI 1 "ubicom32_arith_operand" "rmI")
		     (match_operand:SI 2 "ubicom32_arith_operand"  "dM"))
	  (const_int 0)))
   (set (match_operand:SI 0 "ubicom32_data_register_operand"	   "=d")
	(ashift:SI (match_dup 1)
		   (match_dup 2)))]
  "ubicom32_match_cc_mode(insn, CCWZmode)"
  "lsl.4\\t%0, %1, %2")

(define_insn "lshlsi3_ccwz_null"
  [(set (reg CC_REGNO)
	(compare
	  (ashift:SI (match_operand:SI 0 "ubicom32_arith_operand" "rmI")
		     (match_operand:SI 1 "ubicom32_arith_operand"  "dM"))
	  (const_int 0)))
   (clobber (match_scratch:SI 2					   "=d"))]
  "ubicom32_match_cc_mode(insn, CCWZmode)"
  "lsl.4\\t%2, %0, %1")

; The combiner finds this canonical form for what is in essence a right
; shift.
;
(define_insn "asr1_2"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	 "=d")
	(sign_extract:SI (match_operand:QI 1 "memory_operand"	  "m")
			 (match_operand:SI 2 "const_int_operand"  "M")
			 (match_operand:SI 3 "const_int_operand"  "M")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4
    && (INTVAL (operands[2]) + INTVAL (operands[3]) == 8))"
  "asr.1\\t%0, %1, %3")

; The combiner finds this canonical form for what is in essence a right
; shift.
;
(define_insn "asr2_2"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	 "=d")
	(sign_extract:SI (match_operand:HI 1 "memory_operand"	  "m")
			 (match_operand:SI 2 "const_int_operand"  "M")
			 (match_operand:SI 3 "const_int_operand"  "M")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4
    && (INTVAL (operands[2]) + INTVAL (operands[3]) == 16))"
  "asr.2\\t%0, %1, %3")

(define_insn "ashrsi3"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	   "=d")
	(ashiftrt:SI (match_operand:SI 1 "ubicom32_arith_operand" "rmJ")
		     (match_operand:SI 2 "ubicom32_arith_operand"  "dM")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "asr.4\\t%0, %1, %2")

(define_insn "ashrsi3_ccwzn"
  [(set (reg CC_REGNO)
	(compare
	  (ashiftrt:SI (match_operand:SI 1 "ubicom32_arith_operand" "rmJ")
		       (match_operand:SI 2 "ubicom32_arith_operand"  "dM"))
	  (const_int 0)))
   (set (match_operand:SI 0 "ubicom32_data_register_operand"	     "=d")
	(ashiftrt:SI (match_dup 1)
		     (match_dup 2)))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "asr.4\\t%0, %1, %2")

(define_insn "ashrsi3_ccwzn_null"
  [(set (reg CC_REGNO)
	(compare
	  (ashiftrt:SI (match_operand:SI 0 "ubicom32_arith_operand" "rmJ")
		       (match_operand:SI 1 "ubicom32_arith_operand"  "dM"))
	  (const_int 0)))
   (clobber (match_scratch:SI 2					     "=d"))]
  "ubicom32_match_cc_mode(insn, CCWZNmode)"
  "asr.4\\t%2, %0, %1")

(define_insn "lsr1_1"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	 "=d")
	(lshiftrt:SI (subreg:SI
		       (match_operand:QI 1 "memory_operand"	  "m")
		       0)
		   (match_operand:SI 2 "ubicom32_arith_operand"  "dM")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4)"
  "lsr.1\\t%0, %1, %2")

; The combiner finds this canonical form for what is in essence a right
; shift.
;
(define_insn "lsr1_2"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	 "=d")
	(zero_extract:SI (match_operand:QI 1 "memory_operand"	  "m")
			 (match_operand:SI 2 "const_int_operand"  "M")
			 (match_operand:SI 3 "const_int_operand"  "M")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4
    && (INTVAL (operands[2]) + INTVAL (operands[3]) == 8))"
  "lsr.1\\t%0, %1, %3")

(define_insn "lsr2_1"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	 "=d")
	(lshiftrt:SI (subreg:SI
		       (match_operand:HI 1 "memory_operand"	  "m")
		       0)
		   (match_operand:SI 2 "ubicom32_arith_operand"  "dM")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4)"
  "lsr.2\\t%0, %1, %2")

; The combiner finds this canonical form for what is in essence a right
; shift.
;
(define_insn "lsr2_2"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	 "=d")
	(zero_extract:SI (match_operand:HI 1 "memory_operand"	  "m")
			 (match_operand:SI 2 "const_int_operand"  "M")
			 (match_operand:SI 3 "const_int_operand"  "M")))
   (clobber (reg:CC CC_REGNO))]
  "(ubicom32_v4
    && (INTVAL (operands[2]) + INTVAL (operands[3]) == 16))"
  "lsr.2\\t%0, %1, %3")

(define_insn "lshrsi3"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	   "=d")
	(lshiftrt:SI (match_operand:SI 1 "ubicom32_arith_operand" "rmI")
		     (match_operand:SI 2 "ubicom32_arith_operand"  "dM")))
   (clobber (reg:CC CC_REGNO))]
  ""
  "lsr.4\\t%0, %1, %2")

(define_insn "lshrsi3_ccwz"
  [(set (reg CC_REGNO)
	(compare
	  (lshiftrt:SI (match_operand:SI 1 "ubicom32_arith_operand" "rmI")
		       (match_operand:SI 2 "ubicom32_arith_operand"  "dM"))
	  (const_int 0)))
   (set (match_operand:SI 0 "ubicom32_data_register_operand"	     "=d")
	(lshiftrt:SI (match_dup 1)
		     (match_dup 2)))]
  "ubicom32_match_cc_mode(insn, CCWZmode)"
  "lsr.4\\t%0, %1, %2")

(define_insn "lshrsi3_ccwz_null"
  [(set (reg CC_REGNO)
	(compare
	  (lshiftrt:SI (match_operand:SI 0 "ubicom32_arith_operand" "rmI")
		       (match_operand:SI 1 "ubicom32_arith_operand"  "dM"))
	  (const_int 0)))
   (clobber (match_scratch:SI 2					     "=d"))]
  "ubicom32_match_cc_mode(insn, CCWZmode)"
  "lsr.4\\t%2, %0, %1")

(define_expand "prologue"
  [(const_int 0)]
  ""
  "{
     ubicom32_expand_prologue ();
     DONE;
   }")

(define_expand "epilogue"
  [(return)]
  ""
  "{
     ubicom32_expand_epilogue ();
     DONE;
   }")

(define_expand "return"
  [(return)]
  ""
  "{
     ubicom32_expand_epilogue ();
     DONE;
   }")

(define_expand "_eh_return"
  [(use (match_operand:SI 0 "register_operand" "r"))
   (use (match_operand:SI 1 "register_operand" "r"))]
  ""
  "{
     ubicom32_expand_eh_return (operands);
     DONE;
   }")

; XXX - it looks almost certain that we could make return_internal use a Dn
; register too.  In that instance we'd have to use a ret instruction
; rather than a calli but it might save cycles.
;
(define_insn "return_internal"
  [(const_int 2)
   (return)
   (use (match_operand:SI 0 "ubicom32_mem_or_address_register_operand" "rm"))]
  ""
  "*
   {
     if (REG_P (operands[0]) && REGNO (operands[0]) == LINK_REGNO
	 && ubicom32_can_use_calli_to_ret)
       return \"calli\\t%0, 0(%0)\";

     return \"ret\\t%0\";
   }")

(define_insn "return_from_post_modify_sp"
  [(parallel
     [(const_int 2)
      (return)
      (use (mem:SI (post_modify:SI
		     (reg:SI SP_REGNO)
		     (plus:SI (reg:SI SP_REGNO)
			      (match_operand:SI 0 "const_int_operand" "n")))))])]
  "INTVAL (operands[0]) >= 4 && INTVAL (operands[0]) <= 7 * 4"
  "ret\\t(sp)%E0++")

;(define_insn "eh_return_internal"
;  [(const_int 4)
;   (return)
;   (use (reg:SI 34))]
;  ""
;  "ret\\ta2")

; No operation, needed in case the user uses -g but not -O.
(define_expand "nop"
  [(const_int 0)]
  ""
  "")

(define_insn "nop_internal"
  [(const_int 0)]
  ""
  "nop")

; The combiner will generate this pattern given shift and add operations.
; The canonical form that the combiner wants to use appears to be multiplies
; instead of shifts even if the compiled sources use shifts.
;
(define_insn "shmrg1_add"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	       "=d")
  	(plus:SI
	  (mult:SI (match_operand:SI 1 "ubicom32_data_register_operand" "d")
		   (const_int 256))
	  (zero_extend:SI
	    (match_operand:QI 2 "ubicom32_arith_operand"	      "rmI"))))
   (clobber (reg:CC CC_REGNO))]
  ""
  "shmrg.1\\t%0, %2, %1")

; The combiner will generate this pattern given shift and or operations.
;
(define_insn "shmrg1_ior"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"		 "=d")
  	(ior:SI
	  (ashift:SI (match_operand:SI 1 "ubicom32_data_register_operand" "d")
		     (const_int 8))
	  (zero_extend:SI
	    (match_operand:QI 2 "ubicom32_arith_operand"		"rmI"))))
   (clobber (reg:CC CC_REGNO))]
  ""
  "shmrg.1\\t%0, %2, %1")

; The combiner will generate this pattern given shift and add operations.
; The canonical form that the combiner wants to use appears to be multiplies
; instead of shifts even if the compiled sources use shifts.
;
(define_insn "shmrg2_add"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"	       "=d")
  	(plus:SI
	  (mult:SI (match_operand:SI 1 "ubicom32_data_register_operand" "d")
		   (const_int 65536))
	  (zero_extend:SI
	    (match_operand:HI 2 "ubicom32_arith_operand"	      "rmI"))))
   (clobber (reg:CC CC_REGNO))]
  ""
  "shmrg.2\\t%0, %2, %1")

; The combiner will generate this pattern given shift and or operations.
;
(define_insn "shmrg2_ior"
  [(set (match_operand:SI 0 "ubicom32_data_register_operand"		 "=d")
  	(ior:SI
	  (ashift:SI (match_operand:SI 1 "ubicom32_data_register_operand" "d")
		     (const_int 16))
	  (zero_extend:SI
	    (match_operand:HI 2 "ubicom32_arith_operand"		"rmI"))))
   (clobber (reg:CC CC_REGNO))]
  ""
  "shmrg.2\\t%0, %2, %1")

; Match the case where we load a word from the stack but then discard the
; upper 16 bits.  We turn this into a zero-extended load of that useful
; 16 bits direct from the stack where possible.
;

; XXX - do these peephole2 ops actually work after the CCmode conversion?
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(mem:SI (plus:SI (reg:SI SP_REGNO)
			 (match_operand:SI 1 "const_int_operand" ""))))
   (set (match_operand:SI 2 "nonimmediate_operand" "")
	(zero_extend:SI (match_operand:HI 3 "register_operand" "")))]
  "(INTVAL (operands[1]) <= 252
    && REGNO (operands[3]) == REGNO (operands[0])
    && ((peep2_reg_dead_p (2, operands[0])
	 && ! reg_mentioned_p (operands[0], operands[2]))
        || rtx_equal_p (operands[0], operands[2])))"
  [(set (match_dup 2)
	(zero_extend:SI (mem:HI (plus:SI (reg:SI SP_REGNO)
					 (match_dup 4)))))]
  "{
     operands[4] = GEN_INT (INTVAL (operands[1]) + 2);
   }")

; Match the case where we load a word from the stack but then discard the
; upper 16 bits.  We turn this into a 16-bit load of that useful
; 16 bits direct from the stack where possible.
;
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(mem:SI (plus:SI (reg:SI SP_REGNO)
			 (match_operand:SI 1 "const_int_operand" ""))))
   (set (match_operand:HI 2 "nonimmediate_operand" "")
	(match_operand:HI 3 "register_operand" ""))]
  "(INTVAL (operands[1]) <= 252
    && REGNO (operands[3]) == REGNO (operands[0])
    && ((peep2_reg_dead_p (2, operands[0])
	 && ! reg_mentioned_p (operands[0], operands[2]))
        || rtx_equal_p (operands[0], operands[2])))"
  [(set (match_dup 2)
	(mem:HI (plus:SI (reg:SI SP_REGNO)
			 (match_dup 4))))]
  "{
     operands[4] = GEN_INT (INTVAL (operands[1]) + 2);
   }")

; Match the case where we load a word from the stack but then discard the
; upper 24 bits.  We turn this into a zero-extended load of that useful
; 8 bits direct from the stack where possible.
;
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(mem:SI (plus:SI (reg:SI SP_REGNO)
			 (match_operand:SI 1 "const_int_operand" ""))))
   (set (match_operand:SI 2 "nonimmediate_operand" "")
	(zero_extend:SI (match_operand:QI 3 "register_operand" "")))]
  "(INTVAL (operands[1]) <= 124
    && REGNO (operands[3]) == REGNO (operands[0])
    && ((peep2_reg_dead_p (2, operands[0])
	 && ! reg_mentioned_p (operands[0], operands[2]))
        || rtx_equal_p (operands[0], operands[2])))"
  [(set (match_dup 2)
	(zero_extend:SI (mem:QI (plus:SI (reg:SI SP_REGNO)
					 (match_dup 4)))))]
  "{
     operands[4] = GEN_INT (INTVAL (operands[1]) + 3);
   }")

; Match the case where we load a word from the stack but then discard the
; upper 24 bits.  We turn this into an 8-bit load of that useful
; 8 bits direct from the stack where possible.
;
(define_peephole2
  [(set (match_operand:SI 0 "register_operand" "")
	(mem:SI (plus:SI (reg:SI SP_REGNO)
			 (match_operand:SI 1 "const_int_operand" ""))))
   (set (match_operand:QI 2 "nonimmediate_operand" "")
	(match_operand:QI 3 "register_operand" ""))]
  "(INTVAL (operands[1]) <= 124
    && REGNO (operands[3]) == REGNO (operands[0])
    && ((peep2_reg_dead_p (2, operands[0])
	 && ! reg_mentioned_p (operands[0], operands[2]))
        || rtx_equal_p (operands[0], operands[2])))"
  [(set (match_dup 2)
	(mem:QI (plus:SI (reg:SI SP_REGNO)
			 (match_dup 4))))]
  "{
     operands[4] = GEN_INT (INTVAL (operands[1]) + 3);
   }")


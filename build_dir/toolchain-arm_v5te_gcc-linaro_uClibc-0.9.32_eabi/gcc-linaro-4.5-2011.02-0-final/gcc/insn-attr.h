/* Generated automatically by the program `genattr'
   from the machine description file `md'.  */

#ifndef GCC_INSN_ATTR_H
#define GCC_INSN_ATTR_H

#define HAVE_ATTR_alternative
#define get_attr_alternative(insn) which_alternative
#define HAVE_ATTR_is_thumb
enum attr_is_thumb {IS_THUMB_NO, IS_THUMB_YES};
extern enum attr_is_thumb get_attr_is_thumb (void);

#define HAVE_ATTR_is_arch6
enum attr_is_arch6 {IS_ARCH6_NO, IS_ARCH6_YES};
extern enum attr_is_arch6 get_attr_is_arch6 (void);

#define HAVE_ATTR_shift
extern int get_attr_shift (rtx);
#define HAVE_ATTR_fpu
enum attr_fpu {FPU_NONE, FPU_FPA, FPU_FPE2, FPU_FPE3, FPU_MAVERICK, FPU_VFP};
extern enum attr_fpu get_attr_fpu (void);

#define HAVE_ATTR_sync_result
enum attr_sync_result {SYNC_RESULT_NONE, SYNC_RESULT_0, SYNC_RESULT_1, SYNC_RESULT_2, SYNC_RESULT_3, SYNC_RESULT_4, SYNC_RESULT_5};
extern enum attr_sync_result get_attr_sync_result (rtx);

#define HAVE_ATTR_sync_memory
enum attr_sync_memory {SYNC_MEMORY_NONE, SYNC_MEMORY_0, SYNC_MEMORY_1, SYNC_MEMORY_2, SYNC_MEMORY_3, SYNC_MEMORY_4, SYNC_MEMORY_5};
extern enum attr_sync_memory get_attr_sync_memory (rtx);

#define HAVE_ATTR_sync_required_value
enum attr_sync_required_value {SYNC_REQUIRED_VALUE_NONE, SYNC_REQUIRED_VALUE_0, SYNC_REQUIRED_VALUE_1, SYNC_REQUIRED_VALUE_2, SYNC_REQUIRED_VALUE_3, SYNC_REQUIRED_VALUE_4, SYNC_REQUIRED_VALUE_5};
extern enum attr_sync_required_value get_attr_sync_required_value (rtx);

#define HAVE_ATTR_sync_new_value
enum attr_sync_new_value {SYNC_NEW_VALUE_NONE, SYNC_NEW_VALUE_0, SYNC_NEW_VALUE_1, SYNC_NEW_VALUE_2, SYNC_NEW_VALUE_3, SYNC_NEW_VALUE_4, SYNC_NEW_VALUE_5};
extern enum attr_sync_new_value get_attr_sync_new_value (rtx);

#define HAVE_ATTR_sync_t1
enum attr_sync_t1 {SYNC_T1_NONE, SYNC_T1_0, SYNC_T1_1, SYNC_T1_2, SYNC_T1_3, SYNC_T1_4, SYNC_T1_5};
extern enum attr_sync_t1 get_attr_sync_t1 (rtx);

#define HAVE_ATTR_sync_t2
enum attr_sync_t2 {SYNC_T2_NONE, SYNC_T2_0, SYNC_T2_1, SYNC_T2_2, SYNC_T2_3, SYNC_T2_4, SYNC_T2_5};
extern enum attr_sync_t2 get_attr_sync_t2 (rtx);

#define HAVE_ATTR_sync_release_barrier
enum attr_sync_release_barrier {SYNC_RELEASE_BARRIER_YES, SYNC_RELEASE_BARRIER_NO};
extern enum attr_sync_release_barrier get_attr_sync_release_barrier (rtx);

#define HAVE_ATTR_sync_op
enum attr_sync_op {SYNC_OP_NONE, SYNC_OP_ADD, SYNC_OP_SUB, SYNC_OP_IOR, SYNC_OP_XOR, SYNC_OP_AND, SYNC_OP_NAND};
extern enum attr_sync_op get_attr_sync_op (rtx);

#define HAVE_ATTR_length
extern int get_attr_length (rtx);
extern void shorten_branches (rtx);
extern int insn_default_length (rtx);
extern int insn_min_length (rtx);
extern int insn_variable_length_p (rtx);
extern int insn_current_length (rtx);

#include "insn-addr.h"

#define HAVE_ATTR_pool_range
extern int get_attr_pool_range (rtx);
#define HAVE_ATTR_neg_pool_range
extern int get_attr_neg_pool_range (rtx);
#define HAVE_ATTR_insn
enum attr_insn {INSN_MOV, INSN_MVN, INSN_SMULXY, INSN_SMLAXY, INSN_SMLALXY, INSN_SMULWY, INSN_SMLAWX, INSN_MUL, INSN_MULS, INSN_MLA, INSN_MLAS, INSN_UMULL, INSN_UMULLS, INSN_UMLAL, INSN_UMLALS, INSN_SMULL, INSN_SMULLS, INSN_SMLAL, INSN_SMLALS, INSN_SMLAWY, INSN_SMUAD, INSN_SMUADX, INSN_SMLAD, INSN_SMLADX, INSN_SMUSD, INSN_SMUSDX, INSN_SMLSD, INSN_SMLSDX, INSN_SMMUL, INSN_SMMULR, INSN_SMMLA, INSN_UMAAL, INSN_SMLALD, INSN_SMLSLD, INSN_CLZ, INSN_MRS, INSN_MSR, INSN_XTAB, INSN_SDIV, INSN_UDIV, INSN_OTHER};
extern enum attr_insn get_attr_insn (rtx);

#define HAVE_ATTR_type
enum attr_type {TYPE_ALU, TYPE_ALU_SHIFT, TYPE_ALU_SHIFT_REG, TYPE_MULT, TYPE_BLOCK, TYPE_FLOAT, TYPE_FDIVX, TYPE_FDIVD, TYPE_FDIVS, TYPE_FMUL, TYPE_FMULS, TYPE_FMULD, TYPE_FMACS, TYPE_FMACD, TYPE_FFMUL, TYPE_FARITH, TYPE_FFARITH, TYPE_F_FLAG, TYPE_FLOAT_EM, TYPE_F_LOAD, TYPE_F_STORE, TYPE_F_LOADS, TYPE_F_LOADD, TYPE_F_STORES, TYPE_F_STORED, TYPE_F_MEM_R, TYPE_R_MEM_F, TYPE_F_2_R, TYPE_R_2_F, TYPE_F_CVT, TYPE_BRANCH, TYPE_CALL, TYPE_LOAD_BYTE, TYPE_LOAD1, TYPE_LOAD2, TYPE_LOAD3, TYPE_LOAD4, TYPE_STORE1, TYPE_STORE2, TYPE_STORE3, TYPE_STORE4, TYPE_MAV_FARITH, TYPE_MAV_DMULT, TYPE_FCONSTS, TYPE_FCONSTD, TYPE_FADDS, TYPE_FADDD, TYPE_FFARITHS, TYPE_FFARITHD, TYPE_FCMPS, TYPE_FCMPD, TYPE_FCPYS};
extern enum attr_type get_attr_type (rtx);

#define HAVE_ATTR_ldsched
enum attr_ldsched {LDSCHED_NO, LDSCHED_YES};
extern enum attr_ldsched get_attr_ldsched (void);

#define HAVE_ATTR_neon_type
enum attr_neon_type {NEON_TYPE_NEON_INT_1, NEON_TYPE_NEON_INT_2, NEON_TYPE_NEON_INT_3, NEON_TYPE_NEON_INT_4, NEON_TYPE_NEON_INT_5, NEON_TYPE_NEON_VQNEG_VQABS, NEON_TYPE_NEON_VMOV, NEON_TYPE_NEON_VABA, NEON_TYPE_NEON_VSMA, NEON_TYPE_NEON_VABA_QQQ, NEON_TYPE_NEON_MUL_DDD_8_16_QDD_16_8_LONG_32_16_LONG, NEON_TYPE_NEON_MUL_QQQ_8_16_32_DDD_32, NEON_TYPE_NEON_MUL_QDD_64_32_LONG_QQD_16_DDD_32_SCALAR_64_32_LONG_SCALAR, NEON_TYPE_NEON_MLA_DDD_8_16_QDD_16_8_LONG_32_16_LONG, NEON_TYPE_NEON_MLA_QQQ_8_16, NEON_TYPE_NEON_MLA_DDD_32_QQD_16_DDD_32_SCALAR_QDD_64_32_LONG_SCALAR_QDD_64_32_LONG, NEON_TYPE_NEON_MLA_QQQ_32_QQD_32_SCALAR, NEON_TYPE_NEON_MUL_DDD_16_SCALAR_32_16_LONG_SCALAR, NEON_TYPE_NEON_MUL_QQD_32_SCALAR, NEON_TYPE_NEON_MLA_DDD_16_SCALAR_QDD_32_16_LONG_SCALAR, NEON_TYPE_NEON_SHIFT_1, NEON_TYPE_NEON_SHIFT_2, NEON_TYPE_NEON_SHIFT_3, NEON_TYPE_NEON_VSHL_DDD, NEON_TYPE_NEON_VQSHL_VRSHL_VQRSHL_QQQ, NEON_TYPE_NEON_VSRA_VRSRA, NEON_TYPE_NEON_FP_VADD_DDD_VABS_DD, NEON_TYPE_NEON_FP_VADD_QQQ_VABS_QQ, NEON_TYPE_NEON_FP_VSUM, NEON_TYPE_NEON_FP_VMUL_DDD, NEON_TYPE_NEON_FP_VMUL_QQD, NEON_TYPE_NEON_FP_VMLA_DDD, NEON_TYPE_NEON_FP_VMLA_QQQ, NEON_TYPE_NEON_FP_VMLA_DDD_SCALAR, NEON_TYPE_NEON_FP_VMLA_QQQ_SCALAR, NEON_TYPE_NEON_FP_VRECPS_VRSQRTS_DDD, NEON_TYPE_NEON_FP_VRECPS_VRSQRTS_QQQ, NEON_TYPE_NEON_BP_SIMPLE, NEON_TYPE_NEON_BP_2CYCLE, NEON_TYPE_NEON_BP_3CYCLE, NEON_TYPE_NEON_LDR, NEON_TYPE_NEON_STR, NEON_TYPE_NEON_VLD1_1_2_REGS, NEON_TYPE_NEON_VLD1_3_4_REGS, NEON_TYPE_NEON_VLD2_2_REGS_VLD1_VLD2_ALL_LANES, NEON_TYPE_NEON_VLD2_4_REGS, NEON_TYPE_NEON_VLD3_VLD4, NEON_TYPE_NEON_VST1_1_2_REGS_VST2_2_REGS, NEON_TYPE_NEON_VST1_3_4_REGS, NEON_TYPE_NEON_VST2_4_REGS_VST3_VST4, NEON_TYPE_NEON_VST3_VST4, NEON_TYPE_NEON_VLD1_VLD2_LANE, NEON_TYPE_NEON_VLD3_VLD4_LANE, NEON_TYPE_NEON_VST1_VST2_LANE, NEON_TYPE_NEON_VST3_VST4_LANE, NEON_TYPE_NEON_VLD3_VLD4_ALL_LANES, NEON_TYPE_NEON_MCR, NEON_TYPE_NEON_MCR_2_MCRR, NEON_TYPE_NEON_MRC, NEON_TYPE_NEON_MRRC, NEON_TYPE_NEON_LDM_2, NEON_TYPE_NEON_STM_2, NEON_TYPE_NONE};
extern enum attr_neon_type get_attr_neon_type (rtx);

#define HAVE_ATTR_conds
enum attr_conds {CONDS_USE, CONDS_SET, CONDS_CLOB, CONDS_UNCONDITIONAL, CONDS_NOCOND};
extern enum attr_conds get_attr_conds (rtx);

#define HAVE_ATTR_predicable
enum attr_predicable {PREDICABLE_NO, PREDICABLE_YES};
extern enum attr_predicable get_attr_predicable (rtx);

#define HAVE_ATTR_model_wbuf
enum attr_model_wbuf {MODEL_WBUF_NO, MODEL_WBUF_YES};
extern enum attr_model_wbuf get_attr_model_wbuf (void);

#define HAVE_ATTR_write_conflict
enum attr_write_conflict {WRITE_CONFLICT_NO, WRITE_CONFLICT_YES};
extern enum attr_write_conflict get_attr_write_conflict (rtx);

#define HAVE_ATTR_core_cycles
enum attr_core_cycles {CORE_CYCLES_SINGLE, CORE_CYCLES_MULTI};
extern enum attr_core_cycles get_attr_core_cycles (rtx);

#define HAVE_ATTR_far_jump
enum attr_far_jump {FAR_JUMP_YES, FAR_JUMP_NO};
extern enum attr_far_jump get_attr_far_jump (rtx);

#define HAVE_ATTR_ce_count
extern int get_attr_ce_count (rtx);
#define HAVE_ATTR_tune
enum attr_tune {TUNE_ARM2, TUNE_ARM250, TUNE_ARM3, TUNE_ARM6, TUNE_ARM60, TUNE_ARM600, TUNE_ARM610, TUNE_ARM620, TUNE_ARM7, TUNE_ARM7D, TUNE_ARM7DI, TUNE_ARM70, TUNE_ARM700, TUNE_ARM700I, TUNE_ARM710, TUNE_ARM720, TUNE_ARM710C, TUNE_ARM7100, TUNE_ARM7500, TUNE_ARM7500FE, TUNE_ARM7M, TUNE_ARM7DM, TUNE_ARM7DMI, TUNE_ARM8, TUNE_ARM810, TUNE_STRONGARM, TUNE_STRONGARM110, TUNE_STRONGARM1100, TUNE_STRONGARM1110, TUNE_FA526, TUNE_ARM7TDMI, TUNE_ARM7TDMIS, TUNE_ARM710T, TUNE_ARM720T, TUNE_ARM740T, TUNE_ARM9, TUNE_ARM9TDMI, TUNE_ARM920, TUNE_ARM920T, TUNE_ARM922T, TUNE_ARM940T, TUNE_EP9312, TUNE_ARM10TDMI, TUNE_ARM1020T, TUNE_ARM9E, TUNE_ARM946ES, TUNE_ARM966ES, TUNE_ARM968ES, TUNE_ARM10E, TUNE_ARM1020E, TUNE_ARM1022E, TUNE_XSCALE, TUNE_IWMMXT, TUNE_IWMMXT2, TUNE_ARM926EJS, TUNE_ARM1026EJS, TUNE_ARM1136JS, TUNE_ARM1136JFS, TUNE_ARM1176JZS, TUNE_ARM1176JZFS, TUNE_MPCORENOVFP, TUNE_MPCORE, TUNE_ARM1156T2S, TUNE_ARM1156T2FS, TUNE_CORTEXA5, TUNE_CORTEXA8, TUNE_CORTEXA9, TUNE_CORTEXR4, TUNE_CORTEXR4F, TUNE_CORTEXM4, TUNE_CORTEXM3, TUNE_CORTEXM1, TUNE_CORTEXM0};
extern enum attr_tune get_attr_tune (void);

#define HAVE_ATTR_tune_cortexr4
enum attr_tune_cortexr4 {TUNE_CORTEXR4_YES, TUNE_CORTEXR4_NO};
extern enum attr_tune_cortexr4 get_attr_tune_cortexr4 (void);

#define HAVE_ATTR_generic_sched
enum attr_generic_sched {GENERIC_SCHED_YES, GENERIC_SCHED_NO};
extern enum attr_generic_sched get_attr_generic_sched (void);

#define HAVE_ATTR_generic_vfp
enum attr_generic_vfp {GENERIC_VFP_YES, GENERIC_VFP_NO};
extern enum attr_generic_vfp get_attr_generic_vfp (void);

#define HAVE_ATTR_alt_tune
enum attr_alt_tune {ALT_TUNE_ALL, ALT_TUNE_ONLYA8, ALT_TUNE_NOTA8};
extern enum attr_alt_tune get_attr_alt_tune (rtx);

#define HAVE_ATTR_enabled
extern int get_attr_enabled (rtx);
#define HAVE_ATTR_vfp10
enum attr_vfp10 {VFP10_YES, VFP10_NO};
extern enum attr_vfp10 get_attr_vfp10 (void);

#define HAVE_ATTR_cirrus
enum attr_cirrus {CIRRUS_NOT, CIRRUS_NORMAL, CIRRUS_DOUBLE, CIRRUS_COMPARE, CIRRUS_MOVE};
extern enum attr_cirrus get_attr_cirrus (rtx);

#define HAVE_ATTR_vqh_mnem
enum attr_vqh_mnem {VQH_MNEM_VADD, VQH_MNEM_VMIN, VQH_MNEM_VMAX};
extern enum attr_vqh_mnem get_attr_vqh_mnem (rtx);


#define INSN_SCHEDULING

/* DFA based pipeline interface.  */
#ifndef AUTOMATON_ALTS
#define AUTOMATON_ALTS 0
#endif


#ifndef AUTOMATON_STATE_ALTS
#define AUTOMATON_STATE_ALTS 0
#endif

#ifndef CPU_UNITS_QUERY
#define CPU_UNITS_QUERY 0
#endif

/* Internal insn code number used by automata.  */
extern int internal_dfa_insn_code (rtx);

/* Insn latency time defined in define_insn_reservation. */
extern int insn_default_latency (rtx);

/* Return nonzero if there is a bypass for given insn
   which is a data producer.  */
extern int bypass_p (rtx);

/* Insn latency time on data consumed by the 2nd insn.
   Use the function if bypass_p returns nonzero for
   the 1st insn. */
extern int insn_latency (rtx, rtx);

/* Maximal insn latency time possible of all bypasses for this insn.
   Use the function if bypass_p returns nonzero for
   the 1st insn. */
extern int maximal_insn_latency (rtx);


#if AUTOMATON_ALTS
/* The following function returns number of alternative
   reservations of given insn.  It may be used for better
   insns scheduling heuristics. */
extern int insn_alts (rtx);

#endif

/* Maximal possible number of insns waiting results being
   produced by insns whose execution is not finished. */
extern const int max_insn_queue_index;

/* Pointer to data describing current state of DFA.  */
typedef void *state_t;

/* Size of the data in bytes.  */
extern int state_size (void);

/* Initiate given DFA state, i.e. Set up the state
   as all functional units were not reserved.  */
extern void state_reset (state_t);
/* The following function returns negative value if given
   insn can be issued in processor state described by given
   DFA state.  In this case, the DFA state is changed to
   reflect the current and future reservations by given
   insn.  Otherwise the function returns minimal time
   delay to issue the insn.  This delay may be zero
   for superscalar or VLIW processors.  If the second
   parameter is NULL the function changes given DFA state
   as new processor cycle started.  */
extern int state_transition (state_t, rtx);

#if AUTOMATON_STATE_ALTS
/* The following function returns number of possible
   alternative reservations of given insn in given
   DFA state.  It may be used for better insns scheduling
   heuristics.  By default the function is defined if
   macro AUTOMATON_STATE_ALTS is defined because its
   implementation may require much memory.  */
extern int state_alts (state_t, rtx);
#endif

extern int min_issue_delay (state_t, rtx);
/* The following function returns nonzero if no one insn
   can be issued in current DFA state. */
extern int state_dead_lock_p (state_t);
/* The function returns minimal delay of issue of the 2nd
   insn after issuing the 1st insn in given DFA state.
   The 1st insn should be issued in given state (i.e.
    state_transition should return negative value for
    the insn and the state).  Data dependencies between
    the insns are ignored by the function.  */
extern int min_insn_conflict_delay (state_t, rtx, rtx);
/* The following function outputs reservations for given
   insn as they are described in the corresponding
   define_insn_reservation.  */
extern void print_reservation (FILE *, rtx);

#if CPU_UNITS_QUERY
/* The following function returns code of functional unit
   with given name (see define_cpu_unit). */
extern int get_cpu_unit_code (const char *);
/* The following function returns nonzero if functional
   unit with given code is currently reserved in given
   DFA state.  */
extern int cpu_unit_reservation_p (state_t, int);
#endif

/* The following function returns true if insn
   has a dfa reservation.  */
extern bool insn_has_dfa_reservation_p (rtx);

/* Clean insn code cache.  It should be called if there
   is a chance that condition value in a
   define_insn_reservation will be changed after
   last call of dfa_start.  */
extern void dfa_clean_insn_cache (void);

extern void dfa_clear_single_insn_cache (rtx);

/* Initiate and finish work with DFA.  They should be
   called as the first and the last interface
   functions.  */
extern void dfa_start (void);
extern void dfa_finish (void);

#define ATTR_FLAG_forward	0x1
#define ATTR_FLAG_backward	0x2
#define ATTR_FLAG_likely	0x4
#define ATTR_FLAG_very_likely	0x8
#define ATTR_FLAG_unlikely	0x10
#define ATTR_FLAG_very_unlikely	0x20

#endif /* GCC_INSN_ATTR_H */

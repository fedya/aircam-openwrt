/* Assembler definitions for AVR32.
   Copyright 2003,2004,2005,2006,2007,2008,2009 Atmel Corporation.

   Written by Haavard Skinnemoen, Atmel Norway, <hskinnemoen@atmel.com>

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#if 0
#define DEBUG
#define DEBUG1
#define DEBUG2
#define DEBUG3
#define DEBUG4
#define DEBUG5
#endif

/* Are we trying to be compatible with the IAR assembler? (--iar) */
extern int avr32_iarcompat;

/* By convention, you should define this macro in the `.h' file.  For
   example, `tc-m68k.h' defines `TC_M68K'.  You might have to use this
   if it is necessary to add CPU specific code to the object format
   file.  */
#define TC_AVR32

/* This macro is the BFD target name to use when creating the output
   file.  This will normally depend upon the `OBJ_FMT' macro.  */
#define TARGET_FORMAT "elf32-avr32"

/* This macro is the BFD architecture to pass to `bfd_set_arch_mach'.  */
#define TARGET_ARCH bfd_arch_avr32

/* This macro is the BFD machine number to pass to
   `bfd_set_arch_mach'.  If it is not defined, GAS will use 0.  */
#define TARGET_MACH 0

/* UNDOCUMENTED: Allow //-style comments */
#define DOUBLESLASH_LINE_COMMENTS

/* You should define this macro to be non-zero if the target is big
   endian, and zero if the target is little endian.  */
#define TARGET_BYTES_BIG_ENDIAN 1

/* FIXME: It seems that GAS only expects a one-byte opcode...
   #define NOP_OPCODE 0xd703 */

/* If you define this macro, GAS will warn about the use of
   nonstandard escape sequences in a string.  */
#undef ONLY_STANDARD_ESCAPES

#define DWARF2_FORMAT(SEC) dwarf2_format_32bit

/* Instructions are either 2 or 4 bytes long */
/* #define DWARF2_LINE_MIN_INSN_LENGTH 2 */

/* GAS will call this function for any expression that can not be
   recognized.  When the function is called, `input_line_pointer'
   will point to the start of the expression.  */
#define md_operand(x)

#define md_parse_name(name, expr, mode, c) avr32_parse_name(name, expr, c)
extern int avr32_parse_name(const char *, struct expressionS *, char *);

/* You may define this macro to generate a fixup for a data
   allocation pseudo-op.  */
#define TC_CONS_FIX_NEW(FRAG, OFF, LEN, EXP)	\
  avr32_cons_fix_new(FRAG, OFF, LEN, EXP)
void avr32_cons_fix_new (fragS *, int, int, expressionS *);

/* `extsym - .' expressions can be emitted using PC-relative relocs */
#define DIFF_EXPR_OK

/* This is used to construct expressions out of @gotoff, etc. The
   relocation type is stored in X_md */
#define O_got		O_md1
#define O_hi		O_md2
#define O_lo		O_md3
#define O_tlsgd		O_md4

/* You may define this macro to parse an expression used in a data
   allocation pseudo-op such as `.word'.  You can use this to
   recognize relocation directives that may appear in such directives.  */
/* #define TC_PARSE_CONS_EXPRESSION(EXPR,N) avr_parse_cons_expression (EXPR,N)
   void avr_parse_cons_expression (expressionS *exp, int nbytes); */

/* This should just call either `number_to_chars_bigendian' or
   `number_to_chars_littleendian', whichever is appropriate.  On
   targets like the MIPS which support options to change the
   endianness, which function to call is a runtime decision.  On
   other targets, `md_number_to_chars' can be a simple macro.  */
#define md_number_to_chars number_to_chars_bigendian

/* `md_short_jump_size'
   `md_long_jump_size'
   `md_create_short_jump'
   `md_create_long_jump'
   If `WORKING_DOT_WORD' is defined, GAS will not do broken word
   processing (*note Broken words::.).  Otherwise, you should set
   `md_short_jump_size' to the size of a short jump (a jump that is
   just long enough to jump around a long jmp) and
   `md_long_jump_size' to the size of a long jump (a jump that can go
   anywhere in the function), You should define
   `md_create_short_jump' to create a short jump around a long jump,
   and define `md_create_long_jump' to create a long jump.  */
#define WORKING_DOT_WORD

/* If you define this macro, it means that `tc_gen_reloc' may return
   multiple relocation entries for a single fixup.  In this case, the
   return value of `tc_gen_reloc' is a pointer to a null terminated
   array.  */
#undef RELOC_EXPANSION_POSSIBLE

/* If you define this macro, GAS will not require pseudo-ops to start with a .
   character. */
#define NO_PSEUDO_DOT (avr32_iarcompat)

/* The IAR assembler uses $ as the location counter. Unfortunately, we
   can't make this dependent on avr32_iarcompat... */
#define DOLLAR_DOT

/* Values passed to md_apply_fix3 don't include the symbol value.  */
#define MD_APPLY_SYM_VALUE(FIX) 0

/* The number of bytes to put into a word in a listing.  This affects
   the way the bytes are clumped together in the listing.  For
   example, a value of 2 might print `1234 5678' where a value of 1
   would print `12 34 56 78'.  The default value is 4.  */
#define LISTING_WORD_SIZE 4

/* extern const struct relax_type md_relax_table[];
#define TC_GENERIC_RELAX_TABLE md_relax_table */

/*
  An `.lcomm' directive with no explicit alignment parameter will use
  this macro to set P2VAR to the alignment that a request for SIZE
  bytes will have.  The alignment is expressed as a power of two.  If
  no alignment should take place, the macro definition should do
  nothing.  Some targets define a `.bss' directive that is also
  affected by this macro.  The default definition will set P2VAR to
  the truncated power of two of sizes up to eight bytes.

  We want doublewords to be word-aligned, so we're going to modify the
  default definition a tiny bit.
*/
#define TC_IMPLICIT_LCOMM_ALIGNMENT(SIZE, P2VAR)	\
  do							\
    {							\
      if ((SIZE) >= 4)					\
	(P2VAR) = 2;					\
      else if ((SIZE) >= 2)				\
	(P2VAR) = 1;					\
      else						\
	(P2VAR) = 0;					\
    }							\
  while (0)

/* When relaxing, we need to generate relocations for alignment
   directives.  */
#define HANDLE_ALIGN(frag) avr32_handle_align(frag)
extern void avr32_handle_align(fragS *);

/* See internals doc for explanation. Oh wait...
   Now, can you guess where "alignment" comes from? ;-) */
#define MAX_MEM_FOR_RS_ALIGN_CODE ((1 << alignment) - 1)

/* We need to stop gas from reducing certain expressions (e.g. GOT
   references) */
#define tc_fix_adjustable(fix) avr32_fix_adjustable(fix)
extern bfd_boolean avr32_fix_adjustable(struct fix *);

/* The linker needs to be passed a little more information when relaxing. */
#define TC_FORCE_RELOCATION(fix) avr32_force_reloc(fix)
extern bfd_boolean avr32_force_reloc(struct fix *);

/* I'm tired of working around all the madness in fixup_segment().
   This hook will do basically the same things as the generic code,
   and then it will "goto" right past it.  */
#define TC_VALIDATE_FIX(FIX, SEG, SKIP)		\
  do						\
    {						\
      avr32_process_fixup(FIX, SEG);		\
      if (!(FIX)->fx_done)			\
	++seg_reloc_count;			\
      goto SKIP;				\
    }						\
  while (0)
extern void avr32_process_fixup(struct fix *fixP, segT this_segment);

/* Positive values of TC_FX_SIZE_SLACK allow a target to define
   fixups that far past the end of a frag.  Having such fixups
   is of course most most likely a bug in setting fx_size correctly.
   A negative value disables the fixup check entirely, which is
   appropriate for something like the Renesas / SuperH SH_COUNT
   reloc.  */
/* This target is buggy, and sets fix size too large.  */
#define TC_FX_SIZE_SLACK(FIX) -1

/* We don't want the gas core to make any assumptions about our way of
   doing linkrelaxing.  */
#define TC_LINKRELAX_FIXUP(SEG)			0

/* ... but we do want it to insert lots of padding. */
#define LINKER_RELAXING_SHRINKS_ONLY

/* Better do it ourselves, really... */
#define TC_RELAX_ALIGN(SEG, FRAG, ADDR)	avr32_relax_align(SEG, FRAG, ADDR)
extern relax_addressT
avr32_relax_align(segT segment, fragS *fragP, relax_addressT address);

/* Use line number format that is amenable to linker relaxation.  */
#define DWARF2_USE_FIXED_ADVANCE_PC (linkrelax != 0)

/* This is called by write_object_file() just before symbols are
   attempted converted into section symbols.  */
#define tc_frob_file_before_adjust()	avr32_frob_file()
extern void avr32_frob_file(void);

/* If you define this macro, GAS will call it at the end of each input
   file.  */
#define md_cleanup() avr32_cleanup()
extern void avr32_cleanup(void);

/* There's an AVR32-specific hack in operand() which creates O_md
   expressions when encountering HWRD or LWRD. We need to generate
   proper relocs for them */
/* #define md_cgen_record_fixup_exp avr32_cgen_record_fixup_exp */

/* I needed to add an extra hook in gas_cgen_finish_insn() for
   conversion of O_md* operands because md_cgen_record_fixup_exp()
   isn't called for relaxable insns */
/* #define md_cgen_convert_expr(exp, opinfo) avr32_cgen_convert_expr(exp, opinfo)
   int avr32_cgen_convert_expr(expressionS *, int); */

/* #define tc_gen_reloc gas_cgen_tc_gen_reloc */

/* If you define this macro, it should return the position from which
   the PC relative adjustment for a PC relative fixup should be
   made. On many processors, the base of a PC relative instruction is
   the next instruction, so this macro would return the length of an
   instruction, plus the address of the PC relative fixup. The latter
   can be calculated as fixp->fx_where + fixp->fx_frag->fr_address. */
extern long md_pcrel_from_section (struct fix *, segT);
#define MD_PCREL_FROM_SECTION(FIX, SEC) md_pcrel_from_section (FIX, SEC)

#define LOCAL_LABEL(name) (name[0] == '.' && (name[1] == 'L'))
#define LOCAL_LABELS_FB		1

struct avr32_relaxer
{
  int (*estimate_size)(fragS *, segT);
  long (*relax_frag)(segT, fragS *, long);
  void (*convert_frag)(bfd *, segT, fragS *);
};

/* AVR32 has quite complex instruction coding, which means we need
 * lots of information in order to do the right thing during relaxing
 * (basically, we need to be able to reconstruct a whole new opcode if
 * necessary) */
#define TC_FRAG_TYPE struct avr32_frag_data

struct cpool;

struct avr32_frag_data
{
  /* TODO: Maybe add an expression object here so that we can use
     fix_new_exp() in md_convert_frag?  We may have to decide
     pcrel-ness in md_estimate_size_before_relax() as well...or we
     might do it when parsing.  Doing it while parsing may fail
     because the sub_symbol is undefined then... */
  int pcrel;
  int force_extended;
  int reloc_info;
  struct avr32_relaxer *relaxer;
  expressionS exp;

  /* Points to associated constant pool, for use by LDA and CALL in
     non-pic mode, and when relaxing the .cpool directive */
  struct cpool *pool;
  unsigned int pool_entry;
};

/* We will have to initialize the fields explicitly when needed */
#define TC_FRAG_INIT(fragP)

#define md_estimate_size_before_relax(fragP, segT)			\
  ((fragP)->tc_frag_data.relaxer->estimate_size(fragP, segT))
#define md_relax_frag(segment, fragP, stretch)				\
  ((fragP)->tc_frag_data.relaxer->relax_frag(segment, fragP, stretch))
#define md_convert_frag(abfd, segment, fragP)				\
  ((fragP)->tc_frag_data.relaxer->convert_frag(abfd, segment, fragP))

#define TC_FIX_TYPE struct avr32_fix_data

struct avr32_fix_data
{
  const struct avr32_ifield *ifield;
  unsigned int align;
  long min;
  long max;
};

#define TC_INIT_FIX_DATA(fixP)			\
  do						\
    {						\
      (fixP)->tc_fix_data.ifield = NULL;	\
      (fixP)->tc_fix_data.align = 0;		\
      (fixP)->tc_fix_data.min = 0;		\
      (fixP)->tc_fix_data.max = 0;		\
    }						\
  while (0)

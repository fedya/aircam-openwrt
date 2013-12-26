/* Assembler implementation for AVR32.
   Copyright 2003,2004,2005,2006,2007,2008,2009,2010 Atmel Corporation.

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

#include <stdio.h>
#include "as.h"
#include "safe-ctype.h"
#include "subsegs.h"
#include "symcat.h"
#include "opcodes/avr32-opc.h"
#include "opcodes/avr32-asm.h"
#include "elf/avr32.h"
#include "dwarf2dbg.h"

#define xDEBUG
#define xOPC_CONSISTENCY_CHECK

#ifdef DEBUG
# define pr_debug(fmt, args...) fprintf(stderr, fmt, ##args)
#else
# define pr_debug(fmt, args...)
#endif

/* 3 MSB of instruction word indicate group. Group 7 -> extended */
#define AVR32_COMPACT_P(opcode) ((opcode[0] & 0xe0) != 0xe0)

#define streq(a, b)		(strcmp(a, b) == 0)
#define skip_whitespace(str)	do { while(*(str) == ' ') ++(str); } while(0)

/* Flags given on the command line */
static int avr32_pic	= FALSE;
int linkrelax	= FALSE;
int avr32_iarcompat	= FALSE;

/* This array holds the chars that always start a comment. */
const char comment_chars[]		= "#";

/* This array holds the chars that only start a comment at the
   beginning of a line.  We must include '#' here because the compiler
   may produce #APP and #NO_APP in its output.  */
const char line_comment_chars[]		= "#";

/* These may be used instead of newline (same as ';' in C).  */
const char line_separator_chars[]	= ";";

/* Chars that can be used to separate mantissa from exponent in
   floating point numbers.  */
const char EXP_CHARS[]			= "eE";

/* Chars that mean this number is a floating point constant.  */
const char FLT_CHARS[]			= "dD";

/* Pre-defined "_GLOBAL_OFFSET_TABLE_"  */
symbolS *GOT_symbol;

static struct hash_control *avr32_mnemonic_htab;

struct avr32_ifield_data
{
  bfd_vma value;
  /* FIXME: Get rid of align_order and complain. complain is never
     used, align_order is used in one place.  Try to use the relax
     table instead.  */
  unsigned int align_order;
};

struct avr32_insn
{
  const struct avr32_syntax *syntax;
  expressionS immediate;
  int pcrel;
  int force_extended;
  unsigned int next_slot;
  bfd_reloc_code_real_type r_type;
  struct avr32_ifield_data field_value[AVR32_MAX_FIELDS];
};

static struct avr32_insn current_insn;

/* The target specific pseudo-ops we support. */
static void s_rseg (int);
static void s_cpool(int);

const pseudo_typeS md_pseudo_table[] =
{
  /* Make sure that .word is 32 bits */
  { "word", cons, 4 },
  { "file", (void (*) PARAMS ((int))) dwarf2_directive_file, 0 },
  { "loc", dwarf2_directive_loc, 0 },

  /* .lcomm requires an explicit alignment parameter */
  { "lcomm", s_lcomm, 1 },

  /* AVR32-specific pseudo-ops */
  { "cpool", s_cpool, 0},

  /* IAR compatible pseudo-ops */
  { "program", s_ignore, 0 },
  { "public", s_globl, 0 },
  { "extern", s_ignore, 0 },
  { "module", s_ignore, 0 },
  { "rseg", s_rseg, 0 },
  { "dc8", cons, 1 },
  { "dc16", cons, 2 },
  { "dc32", cons, 4 },

  { NULL, NULL, 0 }
};

/* Questionable stuff starts here */

enum avr32_opinfo {
  AVR32_OPINFO_NONE = BFD_RELOC_NONE,
  AVR32_OPINFO_GOT,
  AVR32_OPINFO_TLSGD,
  AVR32_OPINFO_HI,
  AVR32_OPINFO_LO,
};

enum avr32_arch {
  ARCH_TYPE_AP,
  ARCH_TYPE_UCR1,
  ARCH_TYPE_UCR2,
  ARCH_TYPE_UCR3,
  ARCH_TYPE_UCR3FP
};

struct arch_type_s
{
  /* Architecture name */
  char *name;
  /* Instruction Set Architecture Flags */
  unsigned long isa_flags;
};

struct part_type_s
{
  /* Part name */
  char *name;
  /* Architecture type */
  unsigned int arch;
};

static struct arch_type_s arch_types[] =
{
    {"ap", AVR32_V1 | AVR32_SIMD | AVR32_DSP | AVR32_PICO},
    {"ucr1", AVR32_V1 | AVR32_DSP | AVR32_RMW},
    {"ucr2", AVR32_V1 | AVR32_V2 | AVR32_DSP | AVR32_RMW},
    {"ucr3", AVR32_V1 | AVR32_V2 | AVR32_V3 | AVR32_DSP | AVR32_RMW},
    {"ucr3fp", AVR32_V1 | AVR32_V2 | AVR32_V3 | AVR32_DSP | AVR32_RMW | AVR32_V3FP},
    {"all-insn", AVR32_V1 | AVR32_V2 | AVR32_V3 | AVR32_SIMD | AVR32_DSP | AVR32_RMW | AVR32_V3FP | AVR32_PICO},
    {NULL, 0}
};

static struct part_type_s part_types[] =
{
    {"ap7000",        ARCH_TYPE_AP},
    {"ap7001",        ARCH_TYPE_AP},
    {"ap7002",        ARCH_TYPE_AP},
    {"ap7200",        ARCH_TYPE_AP},
    {"uc3a0128",      ARCH_TYPE_UCR2},
    {"uc3a0256",      ARCH_TYPE_UCR2},
    {"uc3a0512es",    ARCH_TYPE_UCR1},
    {"uc3a0512",      ARCH_TYPE_UCR2},
    {"uc3a1128",      ARCH_TYPE_UCR2},
    {"uc3a1256es",    ARCH_TYPE_UCR1},
    {"uc3a1256",      ARCH_TYPE_UCR2},
    {"uc3a1512es",    ARCH_TYPE_UCR1},
    {"uc3a1512",      ARCH_TYPE_UCR2},
    {"uc3a364",       ARCH_TYPE_UCR2},
    {"uc3a364s",      ARCH_TYPE_UCR2},
    {"uc3a3128",      ARCH_TYPE_UCR2},
    {"uc3a3128s",     ARCH_TYPE_UCR2},
    {"uc3a3256",      ARCH_TYPE_UCR2},
    {"uc3a3256s",     ARCH_TYPE_UCR2},
    {"uc3b064",       ARCH_TYPE_UCR1},
    {"uc3b0128",      ARCH_TYPE_UCR1},
    {"uc3b0256es",    ARCH_TYPE_UCR1},
    {"uc3b0256",      ARCH_TYPE_UCR1},
    {"uc3b0512",      ARCH_TYPE_UCR2},
    {"uc3b0512revc",  ARCH_TYPE_UCR2},
    {"uc3b164",       ARCH_TYPE_UCR1},
    {"uc3b1128",      ARCH_TYPE_UCR1},
    {"uc3b1256",      ARCH_TYPE_UCR1},
    {"uc3b1256es",    ARCH_TYPE_UCR1},
    {"uc3b1512",      ARCH_TYPE_UCR2},
    {"uc3b1512revc",  ARCH_TYPE_UCR2},
    {"uc3c0512crevc", ARCH_TYPE_UCR3},
    {"uc3c1512crevc", ARCH_TYPE_UCR3},
    {"uc3c2512crevc", ARCH_TYPE_UCR3},
    {"atuc3l0256",    ARCH_TYPE_UCR3},
    {"mxt768e",       ARCH_TYPE_UCR3},
    {"uc3l064",       ARCH_TYPE_UCR3},
    {"uc3l032",       ARCH_TYPE_UCR3},
    {"uc3l016",       ARCH_TYPE_UCR3},
    {"uc3l064revb",   ARCH_TYPE_UCR3},
    {"uc3c064c",  ARCH_TYPE_UCR3FP},
    {"uc3c0128c",   ARCH_TYPE_UCR3FP},
    {"uc3c0256c",   ARCH_TYPE_UCR3FP},
    {"uc3c0512c",   ARCH_TYPE_UCR3FP},
    {"uc3c164c",  ARCH_TYPE_UCR3FP},
    {"uc3c1128c",   ARCH_TYPE_UCR3FP},
    {"uc3c1256c",   ARCH_TYPE_UCR3FP},
    {"uc3c1512c",   ARCH_TYPE_UCR3FP},
    {"uc3c264c",  ARCH_TYPE_UCR3FP},
    {"uc3c2128c",   ARCH_TYPE_UCR3FP},
    {"uc3c2256c",   ARCH_TYPE_UCR3FP},
    {"uc3c2512c",   ARCH_TYPE_UCR3FP},
    {NULL, 0}
};

/* Current architecture type.  */
static struct arch_type_s default_arch = {"all-insn", AVR32_V1 | AVR32_V2 | AVR32_V3 | AVR32_SIMD | AVR32_DSP | AVR32_RMW | AVR32_V3FP | AVR32_PICO };
static struct arch_type_s *avr32_arch = &default_arch;

/* Display nicely formatted list of known part- and architecture names.  */

static void
show_arch_list (FILE *stream)
{
  int i, x;

  fprintf (stream, _("Known architecture names:"));
  x = 1000;

  for (i = 0; arch_types[i].name; i++)
    {
      int len = strlen (arch_types[i].name);

      x += len + 1;

      if (x < 75)
	fprintf (stream, " %s", arch_types[i].name);
      else
	{
	  fprintf (stream, "\n  %s", arch_types[i].name);
	  x = len + 2;
	}
    }

  fprintf (stream, "\n");
}

static void
show_part_list (FILE *stream)
{
  int i, x;

  fprintf (stream, _("Known part names:"));
  x = 1000;

  for (i = 0; part_types[i].name; i++)
    {
      int len = strlen(part_types[i].name);

      x += len + 1;

      if (x < 75)
	fprintf (stream, " %s", part_types[i].name);
      else
	{
	  fprintf(stream, "\n  %s", part_types[i].name);
	  x = len + 2;
	}
    }

  fprintf (stream, "\n");
}

const char *md_shortopts = "";
struct option md_longopts[] =
{
#define OPTION_ARCH		(OPTION_MD_BASE)
#define OPTION_PART		(OPTION_ARCH + 1)
#define OPTION_IAR		(OPTION_PART + 1)
#define OPTION_PIC		(OPTION_IAR + 1)
#define OPTION_NOPIC		(OPTION_PIC + 1)
#define OPTION_LINKRELAX	(OPTION_NOPIC + 1)
#define OPTION_NOLINKRELAX	(OPTION_LINKRELAX + 1)
#define OPTION_DIRECT_DATA_REFS (OPTION_NOLINKRELAX + 1)
  {"march",		required_argument, NULL, OPTION_ARCH},
  {"mpart",		required_argument, NULL, OPTION_PART},
  {"iar",		no_argument, NULL, OPTION_IAR},
  {"pic",		no_argument, NULL, OPTION_PIC},
  {"no-pic",		no_argument, NULL, OPTION_NOPIC},
  {"linkrelax",		no_argument, NULL, OPTION_LINKRELAX},
  {"no-linkrelax",	no_argument, NULL, OPTION_NOLINKRELAX},
  /* deprecated alias for -mpart=xxx */
  {"mcpu",		required_argument, NULL, OPTION_PART},
  {NULL,		no_argument, NULL, 0}
};

size_t md_longopts_size = sizeof (md_longopts);

void
md_show_usage (FILE *stream)
{
  fprintf (stream, _("\
AVR32 options:\n\
  -march=[arch-name]      Select cpu architecture. [Default `all-insn']\n\
  -mpart=[part-name]      Select specific part. [Default `none']\n\
  --pic                   Produce Position-Independent Code\n\
  --no-pic                Don't produce Position-Independent Code\n\
  --linkrelax             Produce output suitable for linker relaxing\n\
  --no-linkrelax          Don't produce output suitable for linker relaxing\n"));
  show_arch_list(stream);
}

int
md_parse_option (int c, char *arg ATTRIBUTE_UNUSED)
{
  switch (c)
    {
    case OPTION_ARCH:
      {
	int i;
	char *s = alloca (strlen (arg) + 1);

	{
	  char *t = s;
	  char *arg1 = arg;

	  do
	    *t = TOLOWER (*arg1++);
	  while (*t++);
	}

        /* Add backward compability */
        if (strcmp ("uc", s)== 0)
          {
            as_warn("Deprecated arch `%s' specified. "
                    "Please use '-march=ucr1' instead. "
                    "Converting to arch 'ucr1'\n",
                     s);
            s="ucr1";
          }

	for (i = 0; arch_types[i].name; ++i)
	  if (strcmp (arch_types[i].name, s) == 0)
	    break;

	if (!arch_types[i].name)
	  {
	    show_arch_list (stderr);
	    as_fatal (_("unknown architecture: %s\n"), arg);
	  }

        avr32_arch = &arch_types[i];
	break;
      }
    case OPTION_PART:
      {
	int i;
	char *s = alloca (strlen (arg) + 1);
	char *t = s;
	char *p = arg;

	/* If arch type has already been set, don't bother.
	   -march= always overrides -mpart=  */
	if (avr32_arch != &default_arch)
	  break;

	do
	  *t = TOLOWER (*p++);
	while (*t++);

	for (i = 0; part_types[i].name; ++i)
	  if (strcmp (part_types[i].name, s) == 0)
	    break;

	if (!part_types[i].name)
	  {
	    show_part_list (stderr);
	    as_fatal (_("unknown part: %s\n"), arg);
	  }

	avr32_arch = &arch_types[part_types[i].arch];
	break;
      }
    case OPTION_IAR:
      avr32_iarcompat = 1;
      break;
    case OPTION_PIC:
      avr32_pic = 1;
      break;
    case OPTION_NOPIC:
      avr32_pic = 0;
      break;
    case OPTION_LINKRELAX:
      linkrelax = 1;
      break;
    case OPTION_NOLINKRELAX:
      linkrelax = 0;
      break;
    default:
      return 0;
    }
  return 1;
}

/* Can't use symbol_new here, so have to create a symbol and then at
   a later date assign it a value. Thats what these functions do.

   Shamelessly stolen from ARM.  */

static void
symbol_locate (symbolS *    symbolP,
	       const char * name,	/* It is copied, the caller can modify.  */
	       segT         segment,	/* Segment identifier (SEG_<something>).  */
	       valueT       valu,	/* Symbol value.  */
	       fragS *      frag)	/* Associated fragment.  */
{
  unsigned int name_length;
  char * preserved_copy_of_name;

  name_length = strlen (name) + 1;   /* +1 for \0.  */
  obstack_grow (&notes, name, name_length);
  preserved_copy_of_name = obstack_finish (&notes);
#ifdef STRIP_UNDERSCORE
  if (preserved_copy_of_name[0] == '_')
    preserved_copy_of_name++;
#endif

#ifdef tc_canonicalize_symbol_name
  preserved_copy_of_name =
    tc_canonicalize_symbol_name (preserved_copy_of_name);
#endif

  S_SET_NAME (symbolP, preserved_copy_of_name);

  S_SET_SEGMENT (symbolP, segment);
  S_SET_VALUE (symbolP, valu);
  symbol_clear_list_pointers (symbolP);

  symbol_set_frag (symbolP, frag);

  /* Link to end of symbol chain.  */
  {
    extern int symbol_table_frozen;

    if (symbol_table_frozen)
      abort ();
  }

  symbol_append (symbolP, symbol_lastP, & symbol_rootP, & symbol_lastP);

  obj_symbol_new_hook (symbolP);

#ifdef tc_symbol_new_hook
  tc_symbol_new_hook (symbolP);
#endif

#ifdef DEBUG_SYMS
  verify_symbol_chain (symbol_rootP, symbol_lastP);
#endif /* DEBUG_SYMS  */
}

struct cpool_entry
{
  int			refcount;
  offsetT		offset;
  expressionS		exp;
};

struct cpool
{
  struct cpool		*next;
  int			used;
  struct cpool_entry	*literals;
  unsigned int		padding;
  unsigned int		next_free_entry;
  unsigned int		id;
  symbolS		*symbol;
  segT			section;
  subsegT		sub_section;
};

struct cpool *cpool_list = NULL;

static struct cpool *
find_cpool(segT section, subsegT sub_section)
{
  struct cpool *pool;

  for (pool = cpool_list; pool != NULL; pool = pool->next)
    {
      if (!pool->used
	  && pool->section == section
	  && pool->sub_section == sub_section)
	break;
    }

  return pool;
}

static struct cpool *
find_or_make_cpool(segT section, subsegT sub_section)
{
  static unsigned int next_cpool_id = 0;
  struct cpool *pool;

  pool = find_cpool(section, sub_section);

  if (!pool)
    {
      pool = xmalloc(sizeof(*pool));
      if (!pool)
	return NULL;

      pool->used = 0;
      pool->literals = NULL;
      pool->padding = 0;
      pool->next_free_entry = 0;
      pool->section = section;
      pool->sub_section = sub_section;
      pool->next = cpool_list;
      pool->symbol = NULL;

      cpool_list = pool;
    }

  /* NULL symbol means that the pool is new or has just been emptied.  */
  if (!pool->symbol)
    {
      pool->symbol = symbol_create(FAKE_LABEL_NAME, undefined_section,
				   0, &zero_address_frag);
      pool->id = next_cpool_id++;
    }

  return pool;
}

static struct cpool *
add_to_cpool(expressionS *exp, unsigned int *index, int ref)
{
  struct cpool *pool;
  unsigned int entry;

  pool = find_or_make_cpool(now_seg, now_subseg);

  /* Check if this constant is already in the pool.  */
  for (entry = 0; entry < pool->next_free_entry; entry++)
    {
      if ((pool->literals[entry].exp.X_op == exp->X_op)
	  && (exp->X_op == O_constant)
	  && (pool->literals[entry].exp.X_add_number
	      == exp->X_add_number)
	  && (pool->literals[entry].exp.X_unsigned
	      == exp->X_unsigned))
	break;

      if ((pool->literals[entry].exp.X_op == exp->X_op)
	  && (exp->X_op == O_symbol)
	  && (pool->literals[entry].exp.X_add_number
	      == exp->X_add_number)
	  && (pool->literals[entry].exp.X_add_symbol
	      == exp->X_add_symbol)
	  && (pool->literals[entry].exp.X_op_symbol
	      == exp->X_op_symbol))
	break;
    }

  /* Create an entry if we didn't find a match */
  if (entry == pool->next_free_entry)
    {
      pool->literals = xrealloc(pool->literals,
				sizeof(struct cpool_entry) * (entry + 1));
      pool->literals[entry].exp = *exp;
      pool->literals[entry].refcount = 0;
      pool->next_free_entry++;
    }

  if (index)
    *index = entry;
  if (ref)
    pool->literals[entry].refcount++;

  return pool;
}

struct avr32_operand
{
  int id;
  int is_signed;
  int is_pcrel;
  int align_order;
  int (*match)(char *str);
  void (*parse)(const struct avr32_operand *op, char *str, int opindex);
};

static int
match_anything(char *str ATTRIBUTE_UNUSED)
{
  return 1;
}

static int
match_intreg(char *str)
{
  int regid, ret = 1;

  regid = avr32_parse_intreg(str);
  if (regid < 0)
    ret = 0;

  pr_debug("match_intreg: `%s': %d\n", str, ret);

  return ret;
}

static int
match_intreg_predec(char *str)
{
  int regid;

  if (str[0] != '-' || str[1] != '-')
    return 0;

  regid = avr32_parse_intreg(str + 2);
  if (regid < 0)
    return 0;

  return 1;
}

static int
match_intreg_postinc(char *str)
{
  int regid, ret = 1;
  char *p, c;

  for (p = str; *p; p++)
    if (*p == '+')
      break;

  if (p[0] != '+' || p[1] != '+')
    return 0;

  c = *p, *p = 0;
  regid = avr32_parse_intreg(str);
  if (regid < 0)
    ret = 0;

  *p = c;
  return ret;
}

static int
match_intreg_lsl(char *str)
{
  int regid, ret = 1;
  char *p, c;

  for (p = str; *p; p++)
    if (*p == '<')
      break;

  if (p[0] && p[1] != '<')
    return 0;

  c = *p, *p = 0;
  regid = avr32_parse_intreg(str);
  if (regid < 0)
    ret = 0;

  *p = c;
  return ret;
}

static int
match_intreg_lsr(char *str)
{
  int regid, ret = 1;
  char *p, c;

  for (p = str; *p; p++)
    if (*p == '>')
      break;

  if (p[0] && p[1] != '>')
    return 0;

  c = *p, *p = 0;

  regid = avr32_parse_intreg(str);
  if (regid < 0)
    ret = 0;

  *p = c;
  return ret;
}

static int
match_intreg_part(char *str)
{
  int regid, ret = 1;
  char *p, c;

  for (p = str; *p; p++)
    if (*p == ':')
      break;

  if (p[0] != ':' || !ISPRINT(p[1]) || p[2] != '\0')
    return 0;

  c = *p, *p = 0;
  regid = avr32_parse_intreg(str);
  if (regid < 0)
    ret = 0;

  *p = c;

  return ret;
}

#define match_intreg_disp match_anything

static int
match_intreg_index(char *str)
{
  int regid, ret = 1;
  char *p, *end, c;

  for (p = str; *p; p++)
    if (*p == '[')
      break;

  /* don't allow empty displacement here (it makes no sense) */
  if (p[0] != '[')
    return 0;

  for (end = p + 1; *end; end++) ;
  if (*(--end) != ']')
    return 0;

  c = *end, *end = 0;
  if (!match_intreg_lsl(p + 1))
    ret = 0;
  *end = c;

  if (ret)
    {
      c = *p, *p = 0;
      regid = avr32_parse_intreg(str);
      if (regid < 0)
	ret = 0;
      *p = c;
    }

  return ret;
}

static int
match_intreg_xindex(char *str)
{
  int regid, ret = 1;
  char *p, *end, c;

  for (p = str; *p; p++)
    if (*p == '[')
      break;

  /* empty displacement makes no sense here either */
  if (p[0] != '[')
    return 0;

  for (end = p + 1; *end; end++)
    if (*end == '<')
      break;

  if (!streq(end, "<<2]"))
    return 0;

  c = *end, *end = 0;
  if (!match_intreg_part(p + 1))
    ret = 0;
  *end = c;

  if (ret)
    {
      c = *p, *p = 0;
      regid = avr32_parse_intreg(str);
      if (regid < 0)
	ret = 0;
      *p = c;
    }

  return ret;
}

/* The PC_UDISP_W operator may show up as a label or as a pc[disp]
   expression.  So there's no point in attempting to match this...  */
#define match_pc_disp	match_anything

static int
match_sp(char *str)
{
  /* SP in any form will do */
  return avr32_parse_intreg(str) == AVR32_REG_SP;
}

static int
match_sp_disp(char *str)
{
  int regid, ret = 1;
  char *p, c;

  for (p = str; *p; p++)
    if (*p == '[')
      break;

  /* allow empty displacement, meaning zero */
  if (p[0] == '[')
    {
      char *end;
      for (end = p + 1; *end; end++) ;
      if (end[-1] != ']')
	return 0;
    }

  c = *p, *p = 0;
  regid = avr32_parse_intreg(str);
  if (regid != AVR32_REG_SP)
    ret = 0;

  *p = c;
  return ret;
}

static int
match_cpno(char *str)
{
  if (strncasecmp(str, "cp", 2) != 0)
    return 0;
  return 1;
}

static int
match_cpreg(char *str)
{
  if (strncasecmp(str, "cr", 2) != 0)
    return 0;
  return 1;
}

/* We allow complex expressions, and register names may show up as
   symbols.  Just make sure immediate expressions are always matched
   last.  */
#define match_const		match_anything
#define match_jmplabel		match_anything
#define match_number		match_anything

/* Mnemonics that take reglists never accept anything else */
#define match_reglist8		match_anything
#define match_reglist9		match_anything
#define match_reglist16		match_anything
#define match_reglist_ldm	match_anything
#define match_reglist_cp8	match_anything
#define match_reglist_cpd8	match_anything

/* Ditto for retval, jospinc and mcall */
#define match_retval		match_anything
#define match_jospinc		match_anything
#define match_mcall		match_anything

/* COH is used to select between two different syntaxes */
static int
match_coh(char *str)
{
  return strcasecmp(str, "coh") == 0;
}
#if 0
static int
match_fpreg(char *str)
{
  unsigned long regid;
  char *endptr;

  if ((str[0] != 'f' && str[0] != 'F')
      || (str[1] != 'r' && str[1] != 'R'))
    return 0;

  str += 2;
  regid = strtoul(str, &endptr, 10);
  if (!*str || *endptr)
    return 0;

  return 1;
}
#endif

static int
match_picoreg(char *str)
{
  int regid;

  regid = avr32_parse_picoreg(str);
  if (regid < 0)
    return 0;
  return 1;
}

#define match_pico_reglist_w	match_anything
#define match_pico_reglist_d	match_anything

static int
match_pico_in(char *str)
{
  unsigned long regid;
  char *end;

  if (strncasecmp(str, "in", 2) != 0)
    return 0;

  str += 2;
  regid = strtoul(str, &end, 10);
  if (!*str || *end)
    return 0;

  return 1;
}

static int
match_pico_out0(char *str)
{
  if (strcasecmp(str, "out0") != 0)
    return 0;
  return 1;
}

static int
match_pico_out1(char *str)
{
  if (strcasecmp(str, "out1") != 0)
    return 0;
  return 1;
}

static int
match_pico_out2(char *str)
{
  if (strcasecmp(str, "out2") != 0)
    return 0;
  return 1;
}

static int
match_pico_out3(char *str)
{
  if (strcasecmp(str, "out3") != 0)
    return 0;
  return 1;
}

static void parse_nothing(const struct avr32_operand *op ATTRIBUTE_UNUSED,
			  char *str ATTRIBUTE_UNUSED,
			  int opindex ATTRIBUTE_UNUSED)
{
  /* Do nothing (this is used for "match-only" operands like COH) */
}

static void
parse_const(const struct avr32_operand *op, char *str,
	    int opindex ATTRIBUTE_UNUSED)
{
  expressionS *exp = &current_insn.immediate;
  expressionS *sym_exp;
  int slot;
  char *save;

  pr_debug("parse_const: `%s' (signed: %d, pcrel: %d, align: %d)\n",
	   str, op->is_signed, op->is_pcrel, op->align_order);

  save = input_line_pointer;
  input_line_pointer = str;

  expression(exp);

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].align_order = op->align_order;
  current_insn.pcrel = op->is_pcrel;

  switch (exp->X_op)
    {
    case O_illegal:
      as_bad(_("illegal operand"));
      break;
    case O_absent:
      as_bad(_("missing operand"));
      break;
    case O_constant:
      pr_debug("  -> constant: %ld\n", (long)exp->X_add_number);
      current_insn.field_value[slot].value = exp->X_add_number;
      break;
    case O_uminus:
      pr_debug("  -> uminus\n");
      sym_exp = symbol_get_value_expression(exp->X_add_symbol);
      switch (sym_exp->X_op) {
      case O_subtract:
	pr_debug("     -> subtract: switching operands\n");
	exp->X_op_symbol = sym_exp->X_add_symbol;
	exp->X_add_symbol = sym_exp->X_op_symbol;
	exp->X_op = O_subtract;
	/* TODO: Remove the old X_add_symbol */
	break;
      default:
	as_bad(_("Expression too complex\n"));
	break;
      }
      break;
#if 0
    case O_subtract:
      /* Any expression subtracting a symbol from the current section
	 can be made PC-relative by adding the right offset.  */
      if (S_GET_SEGMENT(exp->X_op_symbol) == now_seg)
	current_insn.pcrel = TRUE;
      pr_debug("  -> subtract: pcrel? %s\n",
	       current_insn.pcrel ? "yes" : "no");
      /* fall through */
#endif
    default:
      pr_debug("  -> (%p <%d> %p + %d)\n",
	       exp->X_add_symbol, exp->X_op, exp->X_op_symbol,
	       exp->X_add_number);
      current_insn.field_value[slot].value = 0;
      break;
    }

  input_line_pointer = save;
}

static void
parse_jmplabel(const struct avr32_operand *op, char *str,
	       int opindex ATTRIBUTE_UNUSED)
{
  expressionS *exp = &current_insn.immediate;
  int slot;
  char *save;

  pr_debug("parse_jmplabel: `%s' (signed: %d, pcrel: %d, align: %d)\n",
	   str, op->is_signed, op->is_pcrel, op->align_order);

  save = input_line_pointer;
  input_line_pointer = str;

  expression(exp);

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].align_order = op->align_order;
  current_insn.pcrel = TRUE;

  switch (exp->X_op)
    {
    case O_illegal:
      as_bad(_("illegal operand"));
      break;
    case O_absent:
      as_bad(_("missing operand"));
      break;
    case O_constant:
      pr_debug("  -> constant: %ld\n", (long)exp->X_add_number);
      current_insn.field_value[slot].value = exp->X_add_number;
      current_insn.pcrel = 0;
      break;
    default:
      pr_debug("  -> (%p <%d> %p + %d)\n",
	       exp->X_add_symbol, exp->X_op, exp->X_op_symbol,
	       exp->X_add_number);
      current_insn.field_value[slot].value = 0;
      break;
    }

  input_line_pointer = save;
}

static void
parse_intreg(const struct avr32_operand *op ATTRIBUTE_UNUSED,
	     char *str, int opindex ATTRIBUTE_UNUSED)
{
  int regid, slot;

  pr_debug("parse_intreg: `%s'\n", str);

  regid = avr32_parse_intreg(str);
  assert(regid >= 0);

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regid;
  current_insn.field_value[slot].align_order = op->align_order;
}

static void
parse_intreg_predec(const struct avr32_operand *op, char *str, int opindex)
{
  parse_intreg(op, str + 2, opindex);
}

static void
parse_intreg_postinc(const struct avr32_operand *op, char *str, int opindex)
{
  char *p, c;

  pr_debug("parse_intreg_postinc: `%s'\n", str);

  for (p = str; *p != '+'; p++) ;

  c = *p, *p = 0;
  parse_intreg(op, str, opindex);
  *p = c;
}

static void
parse_intreg_shift(const struct avr32_operand *op ATTRIBUTE_UNUSED,
		   char *str, int opindex ATTRIBUTE_UNUSED)
{
  int regid, slot, shift = 0;
  char *p, c;
  char shiftop;

  pr_debug("parse Ry<<sa: `%s'\n", str);

  for (p = str; *p; p++)
    if (*p == '<' || *p == '>')
      break;

  shiftop = *p;

  c = *p, *p = 0;
  regid = avr32_parse_intreg(str);
  assert(regid >= 0);
  *p = c;

  if (c)
    {
      if (p[0] != shiftop || p[1] != shiftop)
	as_bad(_("expected shift operator in `%s'"), p);
      else
	{
	  expressionS exp;
	  char *saved;

	  saved = input_line_pointer;
	  input_line_pointer = p + 2;
	  expression(&exp);
	  input_line_pointer = saved;

	  if (exp.X_op != O_constant)
	    as_bad(_("shift amount must be a numeric constant"));
	  else
	    shift = exp.X_add_number;
	}
    }

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regid;
  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = shift;
}

/* The match() function selected the right opcode, so it doesn't
   matter which way we shift any more.  */
#define parse_intreg_lsl	parse_intreg_shift
#define parse_intreg_lsr	parse_intreg_shift

static void
parse_intreg_part(const struct avr32_operand *op, char *str,
		  int opindex ATTRIBUTE_UNUSED)
{
  static const char bparts[] = { 'b', 'l', 'u', 't' };
  static const char hparts[] = { 'b', 't' };
  unsigned int slot, sel;
  int regid;
  char *p, c;

  pr_debug("parse reg:part `%s'\n", str);

  for (p = str; *p; p++)
    if (*p == ':')
      break;

  c = *p, *p = 0;
  regid = avr32_parse_intreg(str);
  assert(regid >= 0);
  *p = c;

  assert(c == ':');

  if (op->align_order)
    {
      for (sel = 0; sel < sizeof(hparts); sel++)
	if (TOLOWER(p[1]) == hparts[sel])
	  break;

      if (sel >= sizeof(hparts))
	{
	  as_bad(_("invalid halfword selector `%c' (must be either b or t)"),
		 p[1]);
	  sel = 0;
	}
    }
  else
    {
      for (sel = 0; sel < sizeof(bparts); sel++)
	if (TOLOWER(p[1]) == bparts[sel])
	  break;

      if (sel >= sizeof(bparts))
	{
	  as_bad(_("invalid byte selector `%c' (must be one of b,l,u,t)"),
		 p[1]);
	  sel = 0;
	}
    }

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regid;
  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = sel;
}

/* This is the parser for "Rp[displacement]" expressions.  In addition
   to the "official" syntax, we accept a label as a replacement for
   the register expression.  This syntax implies Rp=PC and the
   displacement is the pc-relative distance to the label.  */
static void
parse_intreg_disp(const struct avr32_operand *op, char *str, int opindex)
{
  expressionS *exp = &current_insn.immediate;
  int slot, regid;
  char *save, *p, c;

  pr_debug("parse_intreg_disp: `%s' (signed: %d, pcrel: %d, align: %d)\n",
	   str, op->is_signed, op->is_pcrel, op->align_order);

  for (p = str; *p; p++)
    if (*p == '[')
      break;

  slot = current_insn.next_slot++;

  /* First, check if we have a valid register either before '[' or as
     the sole expression.  If so, we use the Rp[disp] syntax.  */
  c = *p, *p = 0;
  regid = avr32_parse_intreg(str);
  *p = c;

  if (regid >= 0)
    {
      current_insn.field_value[slot].value = regid;

      slot = current_insn.next_slot++;
      current_insn.field_value[slot].align_order = op->align_order;

      if (c == '[')
	{
	  save = input_line_pointer;
	  input_line_pointer = p + 1;

	  expression(exp);

	  if (*input_line_pointer != ']')
	    as_bad(_("junk after displacement expression"));

	  input_line_pointer = save;

	  switch (exp->X_op)
	    {
	    case O_illegal:
	      as_bad(_("illegal displacement expression"));
	      break;
	    case O_absent:
	      as_bad(_("missing displacement expression"));
	      break;
	    case O_constant:
	      pr_debug("  -> constant: %ld\n", exp->X_add_number);
	      current_insn.field_value[slot].value = exp->X_add_number;
	      break;
#if 0
	    case O_subtract:
	      if (S_GET_SEGMENT(exp->X_op_symbol) == now_seg)
		current_insn.pcrel = TRUE;
	      pr_debug("  -> subtract: pcrel? %s\n",
		       current_insn.pcrel ? "yes" : "no");
	      /* fall through */
#endif
	    default:
	      pr_debug("  -> (%p <%d> %p + %d)\n",
		       exp->X_add_symbol, exp->X_op, exp->X_op_symbol,
		       exp->X_add_number);
	      current_insn.field_value[slot].value = 0;
	    }
	}
      else
	{
	  exp->X_op = O_constant;
	  exp->X_add_number = 0;
	  current_insn.field_value[slot].value = 0;
	}
    }
  else
    {
      /* Didn't find a valid register.  Try parsing it as a label.  */
      current_insn.field_value[slot].value = AVR32_REG_PC;
      parse_jmplabel(op, str, opindex);
    }
}

static void
parse_intreg_index(const struct avr32_operand *op ATTRIBUTE_UNUSED,
		   char *str, int opindex ATTRIBUTE_UNUSED)
{
  int slot, regid;
  char *p, *end, c;

  for (p = str; *p; p++)
    if (*p == '[')
      break;

  assert(*p);

  c = *p, *p = 0;
  regid = avr32_parse_intreg(str);
  assert(regid >= 0);
  *p = c;

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regid;

  p++;
  for (end = p; *end; end++)
    if (*end == ']' || *end == '<')
      break;

  assert(*end);

  c = *end, *end = 0;
  regid = avr32_parse_intreg(p);
  assert(regid >= 0);
  *end = c;

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regid;

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = 0;

  if (*end == '<')
    {
      expressionS exp;
      char *save;

      p = end + 2;
      for (end = p; *end; end++)
	if (*end == ']')
	  break;

      assert(*end == ']');

      c = *end, *end = 0;
      save = input_line_pointer;
      input_line_pointer = p;
      expression(&exp);

      if (*input_line_pointer)
	as_bad(_("junk after shift expression"));

      *end = c;
      input_line_pointer = save;

      if (exp.X_op == O_constant)
	current_insn.field_value[slot].value = exp.X_add_number;
      else
	as_bad(_("shift expression too complex"));
    }
}

static void
parse_intreg_xindex(const struct avr32_operand *op, char *str, int opindex)
{
  int slot, regid;
  char *p, *end, c;

  for (p = str; *p; p++)
    if (*p == '[')
      break;

  assert(*p);

  c = *p, *p = 0;
  regid = avr32_parse_intreg(str);
  assert(regid >= 0);
  *p = c;

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regid;

  p++;
  for (end = p; *end; end++)
    if (*end == '<')
      break;

  assert(*end);

  c = *end, *end = 0;
  parse_intreg_part(op, p, opindex);
  *end = c;
}

static void
parse_pc_disp(const struct avr32_operand *op, char *str, int opindex)
{
  char *p, c;

  for (p = str; *p; p++)
    if (*p == '[')
      break;

  /* The lddpc instruction comes in two different syntax variants:
       lddpc reg, expression
       lddpc reg, pc[disp]
     If the operand contains a '[', we use the second form.  */
  if (*p)
    {
      int regid;

      c = *p, *p = 0;
      regid = avr32_parse_intreg(str);
      *p = c;
      if (regid == AVR32_REG_PC)
	{
	  char *end;

	  for (end = ++p; *end; end++) ;
	  if (*(--end) != ']')
	    as_bad(_("unrecognized form of instruction: `%s'"), str);
	  else
	    {
	      c = *end, *end = 0;
	      parse_const(op, p, opindex);
	      *end = c;
	      current_insn.pcrel = 0;
	    }
	}
      else
	as_bad(_("unrecognized form of instruction: `%s'"), str);
    }
  else
    {
      parse_jmplabel(op, str, opindex);
    }
}

static void parse_sp(const struct avr32_operand *op ATTRIBUTE_UNUSED,
		     char *str ATTRIBUTE_UNUSED,
		     int opindex ATTRIBUTE_UNUSED)
{
  int slot;

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = AVR32_REG_SP;
}

static void
parse_sp_disp(const struct avr32_operand *op, char *str, int opindex)
{
  char *p, c;

  for (; *str; str++)
    if (*str == '[')
      break;

  assert(*str);

  for (p = ++str; *p; p++)
    if (*p == ']')
      break;

  c = *p, *p = 0;
  parse_const(op, str, opindex);
  *p = c;
}

static void
parse_cpno(const struct avr32_operand *op ATTRIBUTE_UNUSED, char *str,
	   int opindex ATTRIBUTE_UNUSED)
{
  int slot;

  str += 2;
  if (*str == '#')
    str++;
  if (*str < '0' || *str > '7' || str[1])
    as_bad(_("invalid coprocessor `%s'"), str);

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = *str - '0';
}

static void
parse_cpreg(const struct avr32_operand *op, char *str,
	    int opindex ATTRIBUTE_UNUSED)
{
  unsigned int crid;
  int slot;
  char *endptr;

  str += 2;
  crid = strtoul(str, &endptr, 10);
  if (*endptr || crid > 15 || crid & ((1 << op->align_order) - 1))
    as_bad(_("invalid coprocessor register `%s'"), str);

  crid >>= op->align_order;

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = crid;
}

static void
parse_number(const struct avr32_operand *op, char *str,
	     int opindex ATTRIBUTE_UNUSED)
{
  expressionS exp;
  int slot;
  char *save;

  save = input_line_pointer;
  input_line_pointer = str;
  expression(&exp);
  input_line_pointer = save;

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].align_order = op->align_order;

  if (exp.X_op == O_constant)
      current_insn.field_value[slot].value = exp.X_add_number;
  else
      as_bad(_("invalid numeric expression `%s'"), str);
}

static void
parse_reglist8(const struct avr32_operand *op ATTRIBUTE_UNUSED,
	       char *str, int opindex ATTRIBUTE_UNUSED)
{
  unsigned long regmask;
  unsigned long value = 0;
  int slot;
  char *tail;

  regmask = avr32_parse_reglist(str, &tail);
  if (*tail)
    as_bad(_("invalid register list `%s'"), str);
  else
    {
      if (avr32_make_regmask8(regmask, &value))
	as_bad(_("register list `%s' doesn't fit"), str);
    }

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = value;
}

static int
parse_reglist_tail(char *str, unsigned long regmask)
{
  expressionS exp;
  char *save, *p, c;
  int regid;

  for (p = str + 1; *p; p++)
    if (*p == '=')
      break;

  if (!*p)
    {
      as_bad(_("invalid register list `%s'"), str);
      return -2;
    }

  c = *p, *p = 0;
  regid = avr32_parse_intreg(str);
  *p = c;

  if (regid != 12)
    {
      as_bad(_("invalid register list `%s'"), str);
      return -2;
    }

  /* If we have an assignment, we must pop PC and we must _not_
     pop LR or R12 */
  if (!(regmask & (1 << AVR32_REG_PC)))
    {
      as_bad(_("return value specified for non-return instruction"));
      return -2;
    }
  else if (regmask & ((1 << AVR32_REG_R12) | (1 << AVR32_REG_LR)))
    {
      as_bad(_("can't pop LR or R12 when specifying return value"));
      return -2;
    }

  save = input_line_pointer;
  input_line_pointer = p + 1;
  expression(&exp);
  input_line_pointer = save;

  if (exp.X_op != O_constant
      || exp.X_add_number < -1
      || exp.X_add_number > 1)
    {
      as_bad(_("invalid return value `%s'"), str);
      return -2;
    }

  return exp.X_add_number;
}

static void
parse_reglist9(const struct avr32_operand *op ATTRIBUTE_UNUSED,
	       char *str, int opindex ATTRIBUTE_UNUSED)
{
  unsigned long regmask;
  unsigned long value = 0, kbit = 0;
  int slot;
  char *tail;

  regmask = avr32_parse_reglist(str, &tail);
  /* printf("parsed reglist16: %04lx, tail: `%s'\n", regmask, tail); */
  if (*tail)
    {
      int retval;

      retval = parse_reglist_tail(tail, regmask);

      switch (retval)
	{
	case -1:
	  regmask |= 1 << AVR32_REG_LR;
	  break;
	case 0:
	  break;
	case 1:
	  regmask |= 1 << AVR32_REG_R12;
	  break;
	default:
	  break;
	}

      kbit = 1;
    }

  if (avr32_make_regmask8(regmask, &value))
    as_bad(_("register list `%s' doesn't fit"), str);


  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = (value << 1) | kbit;
}

static void
parse_reglist16(const struct avr32_operand *op ATTRIBUTE_UNUSED,
		char *str, int opindex ATTRIBUTE_UNUSED)
{
  unsigned long regmask;
  int slot;
  char *tail;

  regmask = avr32_parse_reglist(str, &tail);
  if (*tail)
    as_bad(_("invalid register list `%s'"), str);

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regmask;
}

static void
parse_reglist_ldm(const struct avr32_operand *op ATTRIBUTE_UNUSED,
		  char *str, int opindex ATTRIBUTE_UNUSED)
{
  unsigned long regmask;
  int slot, rp, w_bit = 0;
  char *tail, *p, c;

  for (p = str; *p && *p != ','; p++)
    if (*p == '+')
      break;

  c = *p, *p = 0;
  rp = avr32_parse_intreg(str);
  *p = c;
  if (rp < 0)
    {
      as_bad(_("invalid destination register in `%s'"), str);
      return;
    }

  if (p[0] == '+' && p[1] == '+')
    {
      w_bit = 1;
      p += 2;
    }

  if (*p != ',')
    {
      as_bad(_("expected `,' after destination register in `%s'"), str);
      return;
    }

  str = p + 1;
  regmask = avr32_parse_reglist(str, &tail);
  if (*tail)
    {
      int retval;

      if (rp != AVR32_REG_SP)
	{
	  as_bad(_("junk at end of line: `%s'"), tail);
	  return;
	}

      rp = AVR32_REG_PC;

      retval = parse_reglist_tail(tail, regmask);

      switch (retval)
	{
	case -1:
	  regmask |= 1 << AVR32_REG_LR;
	  break;
	case 0:
	  break;
	case 1:
	  regmask |= 1 << AVR32_REG_R12;
	  break;
	default:
	  return;
	}
    }

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = rp;
  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = w_bit;
  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regmask;
}

static void
parse_reglist_cp8(const struct avr32_operand *op ATTRIBUTE_UNUSED,
		  char *str, int opindex ATTRIBUTE_UNUSED)
{
  unsigned long regmask;
  int slot, h_bit = 0;
  char *tail;

  regmask = avr32_parse_cpreglist(str, &tail);
  if (*tail)
    as_bad(_("junk at end of line: `%s'"), tail);
  else if (regmask & 0xffUL)
    {
      if (regmask & 0xff00UL)
	as_bad(_("register list `%s' doesn't fit"), str);
      regmask &= 0xff;
    }
  else if (regmask & 0xff00UL)
    {
      regmask >>= 8;
      h_bit = 1;
    }
  else
    as_warn(_("register list is empty"));

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regmask;
  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = h_bit;
}

static void
parse_reglist_cpd8(const struct avr32_operand *op ATTRIBUTE_UNUSED,
		   char *str, int opindex ATTRIBUTE_UNUSED)
{
  unsigned long regmask, regmask_d = 0;
  int slot, i;
  char *tail;

  regmask = avr32_parse_cpreglist(str, &tail);
  if (*tail)
    as_bad(_("junk at end of line: `%s'"), tail);

  for (i = 0; i < 8; i++)
    {
      if (regmask & 1)
	{
	  if (!(regmask & 2))
	    {
	      as_bad(_("register list `%s' doesn't fit"), str);
	      break;
	    }
	  regmask_d |= 1 << i;
	}
      else if (regmask & 2)
	{
	  as_bad(_("register list `%s' doesn't fit"), str);
	  break;
	}

      regmask >>= 2;
    }

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regmask_d;
}

static void
parse_retval(const struct avr32_operand *op ATTRIBUTE_UNUSED,
	     char *str, int opindex ATTRIBUTE_UNUSED)
{
  int regid, slot;

  regid = avr32_parse_intreg(str);
  if (regid < 0)
    {
      expressionS exp;
      char *save;

      regid = 0;

      save = input_line_pointer;
      input_line_pointer = str;
      expression(&exp);
      input_line_pointer = save;

      if (exp.X_op != O_constant)
	as_bad(_("invalid return value `%s'"), str);
      else
	switch (exp.X_add_number)
	  {
	  case -1:
	    regid = AVR32_REG_LR;
	    break;
	  case 0:
	    regid = AVR32_REG_SP;
	    break;
	  case 1:
	    regid = AVR32_REG_PC;
	    break;
	  default:
	    as_bad(_("invalid return value `%s'"), str);
	    break;
	  }
    }

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regid;
}

#define parse_mcall parse_intreg_disp

static void
parse_jospinc(const struct avr32_operand *op ATTRIBUTE_UNUSED,
	      char *str, int opindex ATTRIBUTE_UNUSED)
{
  expressionS exp;
  int slot;
  char *save;

  save = input_line_pointer;
  input_line_pointer = str;
  expression(&exp);
  input_line_pointer = save;

  slot = current_insn.next_slot++;

  if (exp.X_op == O_constant)
    {
      if (exp.X_add_number > 0)
	exp.X_add_number--;
      current_insn.field_value[slot].value = exp.X_add_number;
    }
  else
    as_bad(_("invalid numeric expression `%s'"), str);
}

#define parse_coh		parse_nothing
#if 0
static void
parse_fpreg(const struct avr32_operand *op,
	    char *str, int opindex ATTRIBUTE_UNUSED)
{
  unsigned long regid;
  int slot;

  regid = strtoul(str + 2, NULL, 10);

  if ((regid >= 16) || (regid & ((1 << op->align_order) - 1)))
    as_bad(_("invalid floating-point register `%s'"), str);

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regid;
  current_insn.field_value[slot].align_order = op->align_order;
}
#endif

static void
parse_picoreg(const struct avr32_operand *op,
	      char *str, int opindex ATTRIBUTE_UNUSED)
{
  unsigned long regid;
  int slot;

  regid = avr32_parse_picoreg(str);
  if (regid & ((1 << op->align_order) - 1))
    as_bad(_("invalid double-word PiCo register `%s'"), str);

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regid;
  current_insn.field_value[slot].align_order = op->align_order;
}

static void
parse_pico_reglist_w(const struct avr32_operand *op ATTRIBUTE_UNUSED,
		     char *str, int opindex ATTRIBUTE_UNUSED)
{
  unsigned long regmask;
  int slot, h_bit = 0;
  char *tail;

  regmask = avr32_parse_pico_reglist(str, &tail);
  if (*tail)
    as_bad(_("junk at end of line: `%s'"), tail);

  if (regmask & 0x00ffUL)
    {
      if (regmask & 0xff00UL)
	as_bad(_("register list `%s' doesn't fit"), str);
      regmask &= 0x00ffUL;
    }
  else if (regmask & 0xff00UL)
    {
      regmask >>= 8;
      h_bit = 1;
    }
  else
    as_warn(_("register list is empty"));

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regmask;
  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = h_bit;
}

static void
parse_pico_reglist_d(const struct avr32_operand *op ATTRIBUTE_UNUSED,
		     char *str, int opindex ATTRIBUTE_UNUSED)
{
  unsigned long regmask, regmask_d = 0;
  int slot, i;
  char *tail;

  regmask = avr32_parse_pico_reglist(str, &tail);
  if (*tail)
    as_bad(_("junk at end of line: `%s'"), tail);

  for (i = 0; i < 8; i++)
    {
      if (regmask & 1)
	{
	  if (!(regmask & 2))
	    {
	      as_bad(_("register list `%s' doesn't fit"), str);
	      break;
	    }
	  regmask_d |= 1 << i;
	}
      else if (regmask & 2)
	{
	  as_bad(_("register list `%s' doesn't fit"), str);
	  break;
	}

      regmask >>= 2;
    }

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regmask_d;
}

static void
parse_pico_in(const struct avr32_operand *op ATTRIBUTE_UNUSED,
	      char *str, int opindex ATTRIBUTE_UNUSED)
{
  unsigned long regid;
  int slot;

  regid = strtoul(str + 2, NULL, 10);

  if (regid >= 12)
    as_bad(_("invalid PiCo IN register `%s'"), str);

  slot = current_insn.next_slot++;
  current_insn.field_value[slot].value = regid;
  current_insn.field_value[slot].align_order = 0;
}

#define parse_pico_out0		parse_nothing
#define parse_pico_out1		parse_nothing
#define parse_pico_out2		parse_nothing
#define parse_pico_out3		parse_nothing

#define OP(name, sgn, pcrel, align, func) \
  { AVR32_OPERAND_##name, sgn, pcrel, align, match_##func, parse_##func }

struct avr32_operand avr32_operand_table[] = {
  OP(INTREG, 0, 0, 0, intreg),
  OP(INTREG_PREDEC, 0, 0, 0, intreg_predec),
  OP(INTREG_POSTINC, 0, 0, 0, intreg_postinc),
  OP(INTREG_LSL, 0, 0, 0, intreg_lsl),
  OP(INTREG_LSR, 0, 0, 0, intreg_lsr),
  OP(INTREG_BSEL, 0, 0, 0, intreg_part),
  OP(INTREG_HSEL, 0, 0, 1, intreg_part),
  OP(INTREG_SDISP, 1, 0, 0, intreg_disp),
  OP(INTREG_SDISP_H, 1, 0, 1, intreg_disp),
  OP(INTREG_SDISP_W, 1, 0, 2, intreg_disp),
  OP(INTREG_UDISP, 0, 0, 0, intreg_disp),
  OP(INTREG_UDISP_H, 0, 0, 1, intreg_disp),
  OP(INTREG_UDISP_W, 0, 0, 2, intreg_disp),
  OP(INTREG_INDEX, 0, 0, 0, intreg_index),
  OP(INTREG_XINDEX, 0, 0, 0, intreg_xindex),
  OP(DWREG, 0, 0, 1, intreg),
  OP(PC_UDISP_W, 0, 1, 2, pc_disp),
  OP(SP, 0, 0, 0, sp),
  OP(SP_UDISP_W, 0, 0, 2, sp_disp),
  OP(CPNO, 0, 0, 0, cpno),
  OP(CPREG, 0, 0, 0, cpreg),
  OP(CPREG_D, 0, 0, 1, cpreg),
  OP(UNSIGNED_CONST, 0, 0, 0, const),
  OP(UNSIGNED_CONST_W, 0, 0, 2, const),
  OP(SIGNED_CONST, 1, 0, 0, const),
  OP(SIGNED_CONST_W, 1, 0, 2, const),
  OP(JMPLABEL, 1, 1, 1, jmplabel),
  OP(UNSIGNED_NUMBER, 0, 0, 0, number),
  OP(UNSIGNED_NUMBER_W, 0, 0, 2, number),
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
  OP(PICO_REG_W, 0, 0, 0, picoreg),
  OP(PICO_REG_D, 0, 0, 1, picoreg),
  OP(PICO_REGLIST_W, 0, 0, 0, pico_reglist_w),
  OP(PICO_REGLIST_D, 0, 0, 0, pico_reglist_d),
  OP(PICO_IN, 0, 0, 0, pico_in),
  OP(PICO_OUT0, 0, 0, 0, pico_out0),
  OP(PICO_OUT1, 0, 0, 0, pico_out1),
  OP(PICO_OUT2, 0, 0, 0, pico_out2),
  OP(PICO_OUT3, 0, 0, 0, pico_out3),
};

symbolS *
md_undefined_symbol (char *name ATTRIBUTE_UNUSED)
{
  pr_debug("md_undefined_symbol: %s\n", name);
  return 0;
}

struct avr32_relax_type
{
  long lower_bound;
  long upper_bound;
  unsigned char align;
  unsigned char length;
  signed short next;
};

#define EMPTY { 0, 0, 0, 0, -1 }
#define C(lower, upper, align, next)			\
  { (lower), (upper), (align), 2, AVR32_OPC_##next }
#define E(lower, upper, align)				\
  { (lower), (upper), (align), 4, -1 }

static const struct avr32_relax_type avr32_relax_table[] =
  {
    /* 0 */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY,
    E(0, 65535, 0), E(0, 65535, 0), E(0, 65535, 0), E(0, 65535, 0),
    EMPTY,
    /* 16 */
    EMPTY, EMPTY, EMPTY, EMPTY,

    C(-256, 254, 1, BREQ2), C(-256, 254, 1, BRNE2),
    C(-256, 254, 1, BRCC2), C(-256, 254, 1, BRCS2),
    C(-256, 254, 1, BRGE2), C(-256, 254, 1, BRLT2),
    C(-256, 254, 1, BRMI2), C(-256, 254, 1, BRPL2),
    E(-2097152, 2097150, 1), E(-2097152, 2097150, 1),
    E(-2097152, 2097150, 1), E(-2097152, 2097150, 1),
    /* 32 */
    E(-2097152, 2097150, 1), E(-2097152, 2097150, 1),
    E(-2097152, 2097150, 1), E(-2097152, 2097150, 1),
    E(-2097152, 2097150, 1), E(-2097152, 2097150, 1),
    E(-2097152, 2097150, 1), E(-2097152, 2097150, 1),
    E(-2097152, 2097150, 1), E(-2097152, 2097150, 1),
    E(-2097152, 2097150, 1), E(-2097152, 2097150, 1),

    EMPTY, EMPTY, EMPTY, EMPTY,
    /* 48 */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY,

    C(-32, 31, 0, CP_W3), E(-1048576, 1048575, 0),

    EMPTY, EMPTY, EMPTY,
    /* 64: csrfcz */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    E(0, 65535, 0), E(0, 65535, 0),
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    E(-32768, 32767, 0),
    /* 80: LD_SB2 */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

    C(0, 7, 0, LD_UB4), E(-32768, 32767, 0),

    EMPTY,
    EMPTY, EMPTY,

    C(0, 14, 1, LD_SH4), E(-32768, 32767, 0),

    EMPTY, EMPTY, EMPTY,

    C(0, 14, 1, LD_UH4),

    /* 96: LD_UH4 */
    E(-32768, 32767, 0),

    EMPTY, EMPTY, EMPTY, EMPTY,

    C(0, 124, 2, LD_W4), E(-32768, 32767, 0),

    E(0, 1020, 2),	/* LDC_D1 */
    EMPTY, EMPTY,
    E(0, 1020, 2),	/* LDC_W1 */
    EMPTY, EMPTY,
    E(0, 16380, 2),	/* LDC0_D */
    E(0, 16380, 2),	/* LDC0_W */
    EMPTY,

    /* 112: LDCM_D_PU */
    EMPTY, EMPTY, EMPTY,

    C(0, 508, 2, LDDPC_EXT), E(-32768, 32767, 0),

    EMPTY,EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 134: MACHH_W */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    E(-131072, 131068, 2),	/* MCALL */
    E(0, 1020, 2),		/* MFDR */
    E(0, 1020, 2),		/* MFSR */
    EMPTY, EMPTY,

    C(-128, 127, 0, MOV2), E(-1048576, 1048575, 0),

    EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

    E(-128, 127, 0),		/* MOVEQ2 */
    E(-128, 127, 0),		/* MOVNE2 */
    E(-128, 127, 0),		/* MOVCC2 */
    E(-128, 127, 0),		/* 166: MOVCS2 */
    E(-128, 127, 0),		/* MOVGE2 */
    E(-128, 127, 0),		/* MOVLT2 */
    E(-128, 127, 0),		/* MOVMI2 */
    E(-128, 127, 0),		/* MOVPL2 */
    E(-128, 127, 0),		/* MOVLS2 */
    E(-128, 127, 0),		/* MOVGT2 */
    E(-128, 127, 0),		/* MOVLE2 */
    E(-128, 127, 0),		/* MOVHI2 */
    E(-128, 127, 0),		/* MOVVS2 */
    E(-128, 127, 0),		/* MOVVC2 */
    E(-128, 127, 0),		/* MOVQS2 */
    E(-128, 127, 0),		/* MOVAL2 */

    E(0, 1020, 2),		/* MTDR */
    E(0, 1020, 2),		/* MTSR */
    EMPTY,
    EMPTY,
    E(-128, 127, 0),		/* MUL3 */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 198: MVCR_W */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    E(0, 65535, 0), E(0, 65535, 0),
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 230: PASR_H */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 262: PUNPCKSB_H */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,

    C(-1024, 1022, 1, RCALL2), E(-2097152, 2097150, 1),

    EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY,

    C(-1024, 1022, 1, BRAL),

    EMPTY, EMPTY, EMPTY,
    E(-128, 127, 0),		/* RSUB2 */
    /* 294: SATADD_H */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    E(0, 255, 0),		/* SLEEP */
    EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 326: ST_B2 */
    EMPTY, EMPTY,
    C(0, 7, 0, ST_B4), E(-32768, 32767, 0),
    EMPTY, EMPTY, EMPTY, EMPTY,
    E(-32768, 32767, 0),
    EMPTY, EMPTY, EMPTY,
    C(0, 14, 1, ST_H4), E(-32768, 32767, 0),
    EMPTY, EMPTY,
    EMPTY,
    C(0, 60, 2, ST_W4), E(-32768, 32767, 0),
    E(0, 1020, 2),	/* STC_D1 */
    EMPTY, EMPTY,
    E(0, 1020, 2),	/* STC_W1 */
    EMPTY, EMPTY,
    E(0, 16380, 2),	/* STC0_D */
    E(0, 16380, 2),	/* STC0_W */

    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 358: STDSP */
    EMPTY, EMPTY,
    E(0, 1020, 2),	/* STHH_W1 */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY,
    E(-32768, 32767, 0),
    C(-512, 508, 2, SUB4),
    C(-128, 127, 0, SUB4), E(-1048576, 1048576, 0),
    /* SUB{cond} */
    E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0),
    E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0),
    E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0),
    E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0),
    /* SUBF{cond} */
    E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0),
    E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0),
    E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0),
    E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0),
    EMPTY,

    /* 406: SWAP_B */
    EMPTY, EMPTY, EMPTY,
    E(0, 255, 0),	/* SYNC */
    EMPTY, EMPTY, EMPTY, EMPTY,
    /* 414: TST */
    EMPTY, EMPTY, E(-65536, 65535, 2), E(-65536, 65535, 2), E(-65536, 65535, 2), EMPTY, EMPTY, EMPTY,
    /* 422: RSUB{cond} */
    E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0),
    E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0),
    E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0),
    E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0), E(-128, 127, 0),
    /* 436: ADD{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 454: SUB{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 472: AND{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 486: OR{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 502: EOR{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 518: LD.w{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 534: LD.sh{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 550: LD.uh{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 566: LD.sb{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 582: LD.ub{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 596: ST.w{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 614: ST.h{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 630: ST.b{cond} */
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    /* 646: movh */
    E(0, 65535, 0), EMPTY, EMPTY,
  /* 649: fmac.s */
  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  };

#undef E
#undef C
#undef EMPTY

#define AVR32_RS_NONE (-1)

#define avr32_rs_size(state) (avr32_relax_table[(state)].length)
#define avr32_rs_align(state) (avr32_relax_table[(state)].align)
#define relax_more(state) (avr32_relax_table[(state)].next)

#define opc_initial_substate(opc) ((opc)->id)

static int need_relax(int subtype, offsetT distance)
{
  offsetT upper_bound, lower_bound;

  upper_bound = avr32_relax_table[subtype].upper_bound;
  lower_bound = avr32_relax_table[subtype].lower_bound;

  if (distance & ((1 << avr32_rs_align(subtype)) - 1))
    return 1;
  if ((distance > upper_bound) || (distance < lower_bound))
    return 1;

  return 0;
}

enum {
  LDA_SUBTYPE_MOV1,
  LDA_SUBTYPE_MOV2,
  LDA_SUBTYPE_SUB,
  LDA_SUBTYPE_LDDPC,
  LDA_SUBTYPE_LDW,
  LDA_SUBTYPE_GOTLOAD,
  LDA_SUBTYPE_GOTLOAD_LARGE,
};

enum {
  CALL_SUBTYPE_RCALL1,
  CALL_SUBTYPE_RCALL2,
  CALL_SUBTYPE_MCALL_CP,
  CALL_SUBTYPE_MCALL_GOT,
  CALL_SUBTYPE_MCALL_LARGE,
};

#define LDA_INITIAL_SIZE	(avr32_pic ? 4 : 2)
#define CALL_INITIAL_SIZE	2

#define need_reloc(sym, seg, pcrel)					\
  (!(S_IS_DEFINED(sym)							\
     && ((pcrel && S_GET_SEGMENT(sym) == seg)				\
	 || (!pcrel && S_GET_SEGMENT(sym) == absolute_section)))	\
   || S_FORCE_RELOC(sym, 1))

/* Return an initial guess of the length by which a fragment must grow to
   hold a branch to reach its destination.
   Also updates fr_type/fr_subtype as necessary.

   Called just before doing relaxation.
   Any symbol that is now undefined will not become defined.
   The guess for fr_var is ACTUALLY the growth beyond fr_fix.
   Whatever we do to grow fr_fix or fr_var contributes to our returned value.
   Although it may not be explicit in the frag, pretend fr_var starts with a
   0 value.  */

static int
avr32_default_estimate_size_before_relax (fragS *fragP, segT segment)
{
  int growth = 0;

  assert(fragP);
  assert(fragP->fr_symbol);

  if (fragP->tc_frag_data.force_extended
      || need_reloc(fragP->fr_symbol, segment, fragP->tc_frag_data.pcrel))
    {
      int largest_state = fragP->fr_subtype;
      while (relax_more(largest_state) != AVR32_RS_NONE)
	largest_state = relax_more(largest_state);
      growth = avr32_rs_size(largest_state) - fragP->fr_var;
    }
  else
    {
      growth = avr32_rs_size(fragP->fr_subtype) - fragP->fr_var;
    }

  pr_debug("%s:%d: md_estimate_size_before_relax: %d\n",
	   fragP->fr_file, fragP->fr_line, growth);

  return growth;
}

static int
avr32_lda_estimate_size_before_relax(fragS *fragP, segT segment ATTRIBUTE_UNUSED)
{
  return fragP->fr_var - LDA_INITIAL_SIZE;
}

static int
avr32_call_estimate_size_before_relax(fragS *fragP, segT segment ATTRIBUTE_UNUSED)
{
  return fragP->fr_var - CALL_INITIAL_SIZE;
}

static int
avr32_cpool_estimate_size_before_relax(fragS *fragP,
				       segT segment ATTRIBUTE_UNUSED)
{
  return fragP->fr_var;
}

/* This macro may be defined to relax a frag. GAS will call this with the
 * segment, the frag, and the change in size of all previous frags;
 * md_relax_frag should return the change in size of the frag. */
static long
avr32_default_relax_frag (segT segment, fragS *fragP, long stretch)
{
  int state, next_state;
  symbolS *symbolP;	/* The target symbol */
  long growth = 0;

  state = next_state = fragP->fr_subtype;

  symbolP = fragP->fr_symbol;

  if (fragP->tc_frag_data.force_extended
      || need_reloc(symbolP, segment, fragP->tc_frag_data.pcrel))
    {
      /* Symbol must be resolved by the linker. Emit the largest
	 possible opcode. */
      while (relax_more(next_state) != AVR32_RS_NONE)
	next_state = relax_more(next_state);
    }
  else
    {
      addressT address;	/* The address of fragP */
      addressT target;	/* The address of the target symbol */
      offsetT distance;	/* The distance between the insn and the symbol */
      fragS *sym_frag;

      address = fragP->fr_address;
      target = fragP->fr_offset;
      symbolP = fragP->fr_symbol;
      sym_frag = symbol_get_frag(symbolP);

      address += fragP->fr_fix - fragP->fr_var;
      target += S_GET_VALUE(symbolP);

      if (stretch != 0
	  && sym_frag->relax_marker != fragP->relax_marker
	  && S_GET_SEGMENT(symbolP) == segment)
	/* if it was correctly aligned before, make sure it stays aligned */
	target += stretch & (~0UL << avr32_rs_align(state));

      if (fragP->tc_frag_data.pcrel)
	distance = target - (address & (~0UL << avr32_rs_align(state)));
      else
	distance = target;

      pr_debug("%s:%d: relax more? 0x%x - 0x%x = 0x%x (%d), align %d\n",
	       fragP->fr_file, fragP->fr_line, target, address,
	       distance, distance, avr32_rs_align(state));

      if (need_relax(state, distance))
	{
	  if (relax_more(state) != AVR32_RS_NONE)
	    next_state = relax_more(state);
	  pr_debug("%s:%d: relax more %d -> %d (%d - %d, align %d)\n",
		   fragP->fr_file, fragP->fr_line, state, next_state,
		   target, address, avr32_rs_align(state));
	}
    }

  growth = avr32_rs_size(next_state) - avr32_rs_size(state);
  fragP->fr_subtype = next_state;

  pr_debug("%s:%d: md_relax_frag: growth=%d, subtype=%d, opc=0x%08lx\n",
	   fragP->fr_file, fragP->fr_line, growth, fragP->fr_subtype,
	   avr32_opc_table[next_state].value);

  return growth;
}

static long
avr32_lda_relax_frag(segT segment, fragS *fragP, long stretch)
{
  struct cpool *pool= NULL;
  unsigned int entry = 0;
  addressT address, target;
  offsetT distance;
  symbolS *symbolP;
  fragS *sym_frag;
  long old_size, new_size;

  symbolP = fragP->fr_symbol;
  old_size = fragP->fr_var;
  if (!avr32_pic)
    {
      pool = fragP->tc_frag_data.pool;
      entry = fragP->tc_frag_data.pool_entry;
    }

  address = fragP->fr_address;
  address += fragP->fr_fix - LDA_INITIAL_SIZE;

  if (!S_IS_DEFINED(symbolP) || S_FORCE_RELOC(symbolP, 1))
    goto relax_max;

  target = fragP->fr_offset;
  sym_frag = symbol_get_frag(symbolP);
  target += S_GET_VALUE(symbolP);

  if (sym_frag->relax_marker != fragP->relax_marker
      && S_GET_SEGMENT(symbolP) == segment)
    target += stretch;

  distance = target - address;

  pr_debug("lda_relax_frag: target: %d, address: %d, var: %d\n",
	   target, address, fragP->fr_var);

  if (!avr32_pic && S_GET_SEGMENT(symbolP) == absolute_section
      && target <= 127 && (offsetT)target >= -128)
    {
      if (fragP->fr_subtype == LDA_SUBTYPE_LDDPC
	  || fragP->fr_subtype == LDA_SUBTYPE_LDW)
	pool->literals[entry].refcount--;
      new_size = 2;
      fragP->fr_subtype = LDA_SUBTYPE_MOV1;
    }
  else if (!avr32_pic && S_GET_SEGMENT(symbolP) == absolute_section
	   && target <= 1048575 && (offsetT)target >= -1048576)
    {
      if (fragP->fr_subtype == LDA_SUBTYPE_LDDPC
	  || fragP->fr_subtype == LDA_SUBTYPE_LDW)
	pool->literals[entry].refcount--;
      new_size = 4;
      fragP->fr_subtype = LDA_SUBTYPE_MOV2;
    }
  else if (!linkrelax && S_GET_SEGMENT(symbolP) == segment
	   /* the field will be negated, so this is really -(-32768)
	      and -(32767) */
	   && distance <= 32768 && distance >= -32767)
    {
      if (!avr32_pic
	  && (fragP->fr_subtype == LDA_SUBTYPE_LDDPC
	      || fragP->fr_subtype == LDA_SUBTYPE_LDW))
	pool->literals[entry].refcount--;
      new_size = 4;
      fragP->fr_subtype = LDA_SUBTYPE_SUB;
    }
  else
    {
    relax_max:
      if (avr32_pic)
	{
	  if (linkrelax)
	    {
	      new_size = 8;
	      fragP->fr_subtype = LDA_SUBTYPE_GOTLOAD_LARGE;
	    }
	  else
	    {
	      new_size = 4;
	      fragP->fr_subtype = LDA_SUBTYPE_GOTLOAD;
	    }
	}
      else
	{
	  if (fragP->fr_subtype != LDA_SUBTYPE_LDDPC
	      && fragP->fr_subtype != LDA_SUBTYPE_LDW)
	    pool->literals[entry].refcount++;

	  sym_frag = symbol_get_frag(pool->symbol);
	  target = (sym_frag->fr_address + sym_frag->fr_fix
		    + pool->padding + pool->literals[entry].offset);

	  pr_debug("cpool sym address: 0x%lx\n",
		   sym_frag->fr_address + sym_frag->fr_fix);

	  know(pool->section == segment);

	  if (sym_frag->relax_marker != fragP->relax_marker)
	    target += stretch;

	  distance = target - address;
	  if (distance <= 508 && distance >= 0)
	    {
	      new_size = 2;
	      fragP->fr_subtype = LDA_SUBTYPE_LDDPC;
	    }
	  else
	    {
	      new_size = 4;
	      fragP->fr_subtype = LDA_SUBTYPE_LDW;
	    }

	  pr_debug("lda_relax_frag (cpool): target=0x%lx, address=0x%lx, refcount=%d\n",
		   target, address, pool->literals[entry].refcount);
	}
    }

  fragP->fr_var = new_size;

  pr_debug("%s:%d: lda: relax pass done. subtype: %d, growth: %ld\n",
	   fragP->fr_file, fragP->fr_line,
	   fragP->fr_subtype, new_size - old_size);

  return new_size - old_size;
}

static long
avr32_call_relax_frag(segT segment, fragS *fragP, long stretch)
{
  struct cpool *pool = NULL;
  unsigned int entry = 0;
  addressT address, target;
  offsetT distance;
  symbolS *symbolP;
  fragS *sym_frag;
  long old_size, new_size;

  symbolP = fragP->fr_symbol;
  old_size = fragP->fr_var;
  if (!avr32_pic)
    {
      pool = fragP->tc_frag_data.pool;
      entry = fragP->tc_frag_data.pool_entry;
    }

  address = fragP->fr_address;
  address += fragP->fr_fix - CALL_INITIAL_SIZE;

  if (need_reloc(symbolP, segment, 1))
    {
      pr_debug("call: must emit reloc\n");
      goto relax_max;
    }

  target = fragP->fr_offset;
  sym_frag = symbol_get_frag(symbolP);
  target += S_GET_VALUE(symbolP);

  if (sym_frag->relax_marker != fragP->relax_marker
      && S_GET_SEGMENT(symbolP) == segment)
    target += stretch;

  distance = target - address;

  if (distance <= 1022 && distance >= -1024)
    {
      pr_debug("call: distance is %d, emitting short rcall\n", distance);
      if (!avr32_pic && fragP->fr_subtype == CALL_SUBTYPE_MCALL_CP)
	pool->literals[entry].refcount--;
      new_size = 2;
      fragP->fr_subtype = CALL_SUBTYPE_RCALL1;
    }
  else if (distance <= 2097150 && distance >= -2097152)
    {
      pr_debug("call: distance is %d, emitting long rcall\n", distance);
      if (!avr32_pic && fragP->fr_subtype == CALL_SUBTYPE_MCALL_CP)
	pool->literals[entry].refcount--;
      new_size = 4;
      fragP->fr_subtype = CALL_SUBTYPE_RCALL2;
    }
  else
    {
      pr_debug("call: distance %d too far, emitting something big\n", distance);

    relax_max:
      if (avr32_pic)
	{
	  if (linkrelax)
	    {
	      new_size = 10;
	      fragP->fr_subtype = CALL_SUBTYPE_MCALL_LARGE;
	    }
	  else
	    {
	      new_size = 4;
	      fragP->fr_subtype = CALL_SUBTYPE_MCALL_GOT;
	    }
	}
      else
	{
	  if (fragP->fr_subtype != CALL_SUBTYPE_MCALL_CP)
	    pool->literals[entry].refcount++;

	  new_size = 4;
	  fragP->fr_subtype = CALL_SUBTYPE_MCALL_CP;
	}
    }

  fragP->fr_var = new_size;

  pr_debug("%s:%d: call: relax pass done, growth: %d, fr_var: %d\n",
	   fragP->fr_file, fragP->fr_line,
	   new_size - old_size, fragP->fr_var);

  return new_size - old_size;
}

static long
avr32_cpool_relax_frag(segT segment ATTRIBUTE_UNUSED,
		       fragS *fragP,
		       long stretch ATTRIBUTE_UNUSED)
{
  struct cpool *pool;
  addressT address;
  long old_size, new_size;
  unsigned int entry;

  pool = fragP->tc_frag_data.pool;
  address = fragP->fr_address + fragP->fr_fix;
  old_size = fragP->fr_var;
  new_size = 0;

  for (entry = 0; entry < pool->next_free_entry; entry++)
    {
      if (pool->literals[entry].refcount > 0)
	{
	  pool->literals[entry].offset = new_size;
	  new_size += 4;
	}
    }

  fragP->fr_var = new_size;

  return new_size - old_size;
}

/* *fragP has been relaxed to its final size, and now needs to have
   the bytes inside it modified to conform to the new size.

   Called after relaxation is finished.
   fragP->fr_type == rs_machine_dependent.
   fragP->fr_subtype is the subtype of what the address relaxed to.  */

static void
avr32_default_convert_frag (bfd *abfd ATTRIBUTE_UNUSED,
			    segT segment ATTRIBUTE_UNUSED,
			    fragS *fragP)
{
  const struct avr32_opcode *opc;
  const struct avr32_ifield *ifield;
  bfd_reloc_code_real_type r_type;
  symbolS *symbolP;
  fixS *fixP;
  bfd_vma value;
  int subtype;

  opc = &avr32_opc_table[fragP->fr_subtype];
  ifield = opc->fields[opc->var_field];
  symbolP = fragP->fr_symbol;
  subtype = fragP->fr_subtype;
  r_type = opc->reloc_type;

  /* Clear the opcode bits and the bits belonging to the relaxed
     field.  We assume all other fields stay the same.  */
  value = bfd_getb32(fragP->fr_opcode);
  value &= ~(opc->mask | ifield->mask);

  /* Insert the new opcode */
  value |= opc->value;
  bfd_putb32(value, fragP->fr_opcode);

  fragP->fr_fix += opc->size - fragP->fr_var;

  if (fragP->tc_frag_data.reloc_info != AVR32_OPINFO_NONE)
    {
      switch (fragP->tc_frag_data.reloc_info)
	{
	case AVR32_OPINFO_HI:
	  r_type = BFD_RELOC_HI16;
	  break;
	case AVR32_OPINFO_LO:
	  r_type = BFD_RELOC_LO16;
	  break;
	case AVR32_OPINFO_GOT:
	  switch (r_type)
	    {
	    case BFD_RELOC_AVR32_18W_PCREL:
	      r_type = BFD_RELOC_AVR32_GOT18SW;
	      break;
	    case BFD_RELOC_AVR32_16S:
	      r_type = BFD_RELOC_AVR32_GOT16S;
	      break;
	    default:
	      BAD_CASE(r_type);
	      break;
	    }
	  break;
	default:
	  BAD_CASE(fragP->tc_frag_data.reloc_info);
	  break;
	}
    }

  pr_debug("%s:%d: convert_frag: new %s fixup\n",
	   fragP->fr_file, fragP->fr_line,
	   bfd_get_reloc_code_name(r_type));

#if 1
  fixP = fix_new_exp(fragP, fragP->fr_fix - opc->size, opc->size,
		     &fragP->tc_frag_data.exp,
		     fragP->tc_frag_data.pcrel, r_type);
#else
  fixP = fix_new(fragP, fragP->fr_fix - opc->size, opc->size, symbolP,
		 fragP->fr_offset, fragP->tc_frag_data.pcrel, r_type);
#endif

  /* Revert fix_new brain damage. "dot_value" is the value of PC at
     the point of the fixup, relative to the frag address.  fix_new()
     and friends think they are only being called during the assembly
     pass, not during relaxation or similar, so fx_dot_value, fx_file
     and fx_line are all initialized to the wrong value.  But we don't
     know the size of the fixup until now, so we really can't live up
     to the assumptions these functions make about the target.  What
     do these functions think the "where" and "frag" argument mean
     anyway?  */
  fixP->fx_dot_value = fragP->fr_fix - opc->size;
  fixP->fx_file = fragP->fr_file;
  fixP->fx_line = fragP->fr_line;

  fixP->tc_fix_data.ifield = ifield;
  fixP->tc_fix_data.align = avr32_rs_align(subtype);
  fixP->tc_fix_data.min = avr32_relax_table[subtype].lower_bound;
  fixP->tc_fix_data.max = avr32_relax_table[subtype].upper_bound;
}

static void
avr32_lda_convert_frag(bfd *abfd ATTRIBUTE_UNUSED,
		       segT segment ATTRIBUTE_UNUSED,
		       fragS *fragP)
{
  const struct avr32_opcode *opc;
  const struct avr32_ifield *ifield;
  bfd_reloc_code_real_type r_type;
  expressionS exp;
  struct cpool *pool;
  fixS *fixP;
  bfd_vma value;
  int regid, pcrel = 0, align = 0;
  char *p;

  r_type = BFD_RELOC_NONE;
  regid = fragP->tc_frag_data.reloc_info;
  p = fragP->fr_opcode;
  exp.X_add_symbol = fragP->fr_symbol;
  exp.X_add_number = fragP->fr_offset;
  exp.X_op = O_symbol;

  pr_debug("%s:%d: lda_convert_frag, subtype: %d, fix: %d, var: %d, regid: %d\n",
	   fragP->fr_file, fragP->fr_line,
	   fragP->fr_subtype, fragP->fr_fix, fragP->fr_var, regid);

  switch (fragP->fr_subtype)
    {
    case LDA_SUBTYPE_MOV1:
      opc = &avr32_opc_table[AVR32_OPC_MOV1];
      opc->fields[0]->insert(opc->fields[0], p, regid);
      ifield = opc->fields[1];
      r_type = opc->reloc_type;
      break;
    case LDA_SUBTYPE_MOV2:
      opc = &avr32_opc_table[AVR32_OPC_MOV2];
      opc->fields[0]->insert(opc->fields[0], p, regid);
      ifield = opc->fields[1];
      r_type = opc->reloc_type;
      break;
    case LDA_SUBTYPE_SUB:
      opc = &avr32_opc_table[AVR32_OPC_SUB5];
      opc->fields[0]->insert(opc->fields[0], p, regid);
      opc->fields[1]->insert(opc->fields[1], p, AVR32_REG_PC);
      ifield = opc->fields[2];
      r_type = BFD_RELOC_AVR32_16N_PCREL;

      /* Pretend that SUB5 isn't a "negated" pcrel expression for now.
	 We'll have to fix it up later when we know whether to
	 generate a reloc for it (in which case the linker will negate
	 it, so we shouldn't). */
      pcrel = 1;
      break;
    case LDA_SUBTYPE_LDDPC:
      opc = &avr32_opc_table[AVR32_OPC_LDDPC];
      align = 2;
      r_type = BFD_RELOC_AVR32_9W_CP;
      goto cpool_common;
    case LDA_SUBTYPE_LDW:
      opc = &avr32_opc_table[AVR32_OPC_LDDPC_EXT];
      r_type = BFD_RELOC_AVR32_16_CP;
    cpool_common:
      opc->fields[0]->insert(opc->fields[0], p, regid);
      ifield = opc->fields[1];
      pool = fragP->tc_frag_data.pool;
      exp.X_add_symbol = pool->symbol;
      exp.X_add_number = pool->literals[fragP->tc_frag_data.pool_entry].offset;
      pcrel = 1;
      break;
    case LDA_SUBTYPE_GOTLOAD_LARGE:
      /* ld.w Rd, r6[Rd << 2] (last) */
      opc = &avr32_opc_table[AVR32_OPC_LD_W5];
      bfd_putb32(opc->value, p + 4);
      opc->fields[0]->insert(opc->fields[0], p + 4, regid);
      opc->fields[1]->insert(opc->fields[1], p + 4, 6);
      opc->fields[2]->insert(opc->fields[2], p + 4, regid);
      opc->fields[3]->insert(opc->fields[3], p + 4, 2);

      /* mov Rd, (got_offset / 4) */
      opc = &avr32_opc_table[AVR32_OPC_MOV2];
      opc->fields[0]->insert(opc->fields[0], p, regid);
      ifield = opc->fields[1];
      r_type = BFD_RELOC_AVR32_LDA_GOT;
      break;
    case LDA_SUBTYPE_GOTLOAD:
      opc = &avr32_opc_table[AVR32_OPC_LD_W4];
      opc->fields[0]->insert(opc->fields[0], p, regid);
      opc->fields[1]->insert(opc->fields[1], p, 6);
      ifield = opc->fields[2];
      if (r_type == BFD_RELOC_NONE)
	r_type = BFD_RELOC_AVR32_GOT16S;
      break;
    default:
      BAD_CASE(fragP->fr_subtype);
    }

  value = bfd_getb32(p);
  value &= ~(opc->mask | ifield->mask);
  value |= opc->value;
  bfd_putb32(value, p);

  fragP->fr_fix += fragP->fr_var - LDA_INITIAL_SIZE;

  if (fragP->fr_next
      && ((offsetT)(fragP->fr_next->fr_address - fragP->fr_address)
	  != fragP->fr_fix))
    {
      fprintf(stderr, "LDA frag: fr_fix is wrong! fragP->fr_var = %ld, r_type = %s\n",
	      fragP->fr_var, bfd_get_reloc_code_name(r_type));
      abort();
    }

  fixP = fix_new_exp(fragP, fragP->fr_fix - fragP->fr_var, fragP->fr_var,
		     &exp, pcrel, r_type);

  /* Revert fix_new brain damage. "dot_value" is the value of PC at
     the point of the fixup, relative to the frag address.  fix_new()
     and friends think they are only being called during the assembly
     pass, not during relaxation or similar, so fx_dot_value, fx_file
     and fx_line are all initialized to the wrong value.  But we don't
     know the size of the fixup until now, so we really can't live up
     to the assumptions these functions make about the target.  What
     do these functions think the "where" and "frag" argument mean
     anyway?  */
  fixP->fx_dot_value = fragP->fr_fix - opc->size;
  fixP->fx_file = fragP->fr_file;
  fixP->fx_line = fragP->fr_line;

  fixP->tc_fix_data.ifield = ifield;
  fixP->tc_fix_data.align = align;
  /* these are only used if the fixup can actually be resolved */
  fixP->tc_fix_data.min = -32768;
  fixP->tc_fix_data.max = 32767;
}

static void
avr32_call_convert_frag(bfd *abfd ATTRIBUTE_UNUSED,
		       segT segment ATTRIBUTE_UNUSED,
		       fragS *fragP)
{
  const struct avr32_opcode *opc = NULL;
  const struct avr32_ifield *ifield;
  bfd_reloc_code_real_type r_type;
  symbolS *symbol;
  offsetT offset;
  fixS *fixP;
  bfd_vma value;
  int pcrel = 0, align = 0;
  char *p;

  symbol = fragP->fr_symbol;
  offset = fragP->fr_offset;
  r_type = BFD_RELOC_NONE;
  p = fragP->fr_opcode;

  pr_debug("%s:%d: call_convert_frag, subtype: %d, fix: %d, var: %d\n",
	   fragP->fr_file, fragP->fr_line,
	   fragP->fr_subtype, fragP->fr_fix, fragP->fr_var);

  switch (fragP->fr_subtype)
    {
    case CALL_SUBTYPE_RCALL1:
      opc = &avr32_opc_table[AVR32_OPC_RCALL1];
      /* fall through */
    case CALL_SUBTYPE_RCALL2:
      if (!opc)
	opc = &avr32_opc_table[AVR32_OPC_RCALL2];
      ifield = opc->fields[0];
      r_type = opc->reloc_type;
      pcrel = 1;
      align = 1;
      break;
    case CALL_SUBTYPE_MCALL_CP:
      opc = &avr32_opc_table[AVR32_OPC_MCALL];
      opc->fields[0]->insert(opc->fields[0], p, AVR32_REG_PC);
      ifield = opc->fields[1];
      r_type = BFD_RELOC_AVR32_CPCALL;
      symbol = fragP->tc_frag_data.pool->symbol;
      offset = fragP->tc_frag_data.pool->literals[fragP->tc_frag_data.pool_entry].offset;
      assert(fragP->tc_frag_data.pool->literals[fragP->tc_frag_data.pool_entry].refcount > 0);
      pcrel = 1;
      align = 2;
      break;
    case CALL_SUBTYPE_MCALL_GOT:
      opc = &avr32_opc_table[AVR32_OPC_MCALL];
      opc->fields[0]->insert(opc->fields[0], p, 6);
      ifield = opc->fields[1];
      r_type = BFD_RELOC_AVR32_GOT18SW;
      break;
    case CALL_SUBTYPE_MCALL_LARGE:
      assert(fragP->fr_var == 10);
      /* ld.w lr, r6[lr << 2] */
      opc = &avr32_opc_table[AVR32_OPC_LD_W5];
      bfd_putb32(opc->value, p + 4);
      opc->fields[0]->insert(opc->fields[0], p + 4, AVR32_REG_LR);
      opc->fields[1]->insert(opc->fields[1], p + 4, 6);
      opc->fields[2]->insert(opc->fields[2], p + 4, AVR32_REG_LR);
      opc->fields[3]->insert(opc->fields[3], p + 4, 2);

      /* icall lr */
      opc = &avr32_opc_table[AVR32_OPC_ICALL];
      bfd_putb16(opc->value >> 16, p + 8);
      opc->fields[0]->insert(opc->fields[0], p + 8, AVR32_REG_LR);

      /* mov lr, (got_offset / 4) */
      opc = &avr32_opc_table[AVR32_OPC_MOV2];
      opc->fields[0]->insert(opc->fields[0], p, AVR32_REG_LR);
      ifield = opc->fields[1];
      r_type = BFD_RELOC_AVR32_GOTCALL;
      break;
    default:
      BAD_CASE(fragP->fr_subtype);
    }

  /* Insert the opcode and clear the variable ifield */
  value = bfd_getb32(p);
  value &= ~(opc->mask | ifield->mask);
  value |= opc->value;
  bfd_putb32(value, p);

  fragP->fr_fix += fragP->fr_var - CALL_INITIAL_SIZE;

  if (fragP->fr_next
      && ((offsetT)(fragP->fr_next->fr_address - fragP->fr_address)
	  != fragP->fr_fix))
    {
      fprintf(stderr, "%s:%d: fr_fix %lu is wrong! fr_var=%lu, r_type=%s\n",
	      fragP->fr_file, fragP->fr_line,
	      fragP->fr_fix, fragP->fr_var, bfd_get_reloc_code_name(r_type));
      fprintf(stderr, "fr_fix should be %ld. next frag is %s:%d\n",
	      (offsetT)(fragP->fr_next->fr_address - fragP->fr_address),
	      fragP->fr_next->fr_file, fragP->fr_next->fr_line);
    }

  fixP = fix_new(fragP, fragP->fr_fix - fragP->fr_var, fragP->fr_var,
		 symbol, offset, pcrel, r_type);

  /* Revert fix_new brain damage. "dot_value" is the value of PC at
     the point of the fixup, relative to the frag address.  fix_new()
     and friends think they are only being called during the assembly
     pass, not during relaxation or similar, so fx_dot_value, fx_file
     and fx_line are all initialized to the wrong value.  But we don't
     know the size of the fixup until now, so we really can't live up
     to the assumptions these functions make about the target.  What
     do these functions think the "where" and "frag" argument mean
     anyway?  */
  fixP->fx_dot_value = fragP->fr_fix - opc->size;
  fixP->fx_file = fragP->fr_file;
  fixP->fx_line = fragP->fr_line;

  fixP->tc_fix_data.ifield = ifield;
  fixP->tc_fix_data.align = align;
  /* these are only used if the fixup can actually be resolved */
  fixP->tc_fix_data.min = -2097152;
  fixP->tc_fix_data.max = 2097150;
}

static void
avr32_cpool_convert_frag(bfd *abfd ATTRIBUTE_UNUSED,
			 segT segment ATTRIBUTE_UNUSED,
			 fragS *fragP)
{
  struct cpool *pool;
  addressT address;
  unsigned int entry;
  char *p;
  char sym_name[20];

  /* Did we get rid of the frag altogether? */
  if (!fragP->fr_var)
    return;

  pool = fragP->tc_frag_data.pool;
  address = fragP->fr_address + fragP->fr_fix;
  p = fragP->fr_literal + fragP->fr_fix;

  sprintf(sym_name, "$$cp_\002%x", pool->id);
  symbol_locate(pool->symbol, sym_name, pool->section, fragP->fr_fix, fragP);
  symbol_table_insert(pool->symbol);

  for (entry = 0; entry < pool->next_free_entry; entry++)
    {
      if (pool->literals[entry].refcount > 0)
	{
	  fix_new_exp(fragP, fragP->fr_fix, 4, &pool->literals[entry].exp,
		      FALSE, BFD_RELOC_AVR32_32_CPENT);
	  fragP->fr_fix += 4;
	}
    }
}

static struct avr32_relaxer avr32_default_relaxer = {
  .estimate_size	= avr32_default_estimate_size_before_relax,
  .relax_frag		= avr32_default_relax_frag,
  .convert_frag		= avr32_default_convert_frag,
};
static struct avr32_relaxer avr32_lda_relaxer = {
  .estimate_size	= avr32_lda_estimate_size_before_relax,
  .relax_frag		= avr32_lda_relax_frag,
  .convert_frag		= avr32_lda_convert_frag,
};
static struct avr32_relaxer avr32_call_relaxer = {
  .estimate_size	= avr32_call_estimate_size_before_relax,
  .relax_frag		= avr32_call_relax_frag,
  .convert_frag		= avr32_call_convert_frag,
};
static struct avr32_relaxer avr32_cpool_relaxer = {
  .estimate_size	= avr32_cpool_estimate_size_before_relax,
  .relax_frag		= avr32_cpool_relax_frag,
  .convert_frag		= avr32_cpool_convert_frag,
};

static void s_cpool(int arg ATTRIBUTE_UNUSED)
{
  struct cpool *pool;
  unsigned int max_size;
  char *buf;

  pool = find_cpool(now_seg, now_subseg);
  if (!pool || !pool->symbol || pool->next_free_entry == 0)
    return;

  /* Make sure the constant pool is properly aligned */
  frag_align_code(2, 0);
  if (bfd_get_section_alignment(stdoutput, pool->section) < 2)
    bfd_set_section_alignment(stdoutput, pool->section, 2);

  /* Assume none of the entries are discarded, and that we need the
     maximum amount of alignment.  But we're not going to allocate
     anything up front. */
  max_size = pool->next_free_entry * 4 + 2;
  frag_grow(max_size);
  buf = frag_more(0);

  frag_now->tc_frag_data.relaxer = &avr32_cpool_relaxer;
  frag_now->tc_frag_data.pool = pool;

  symbol_set_frag(pool->symbol, frag_now);

  /* Assume zero initial size, allowing other relaxers to be
     optimistic about things.  */
  frag_var(rs_machine_dependent, max_size, 0,
	   0, pool->symbol, 0, NULL);

  /* Mark the pool as empty.  */
  pool->used = 1;
}

/* The location from which a PC relative jump should be calculated,
   given a PC relative reloc.  */

long
md_pcrel_from_section (fixS *fixP, segT sec)
{
  pr_debug("pcrel_from_section, fx_offset = %d\n", fixP->fx_offset);

  if (fixP->fx_addsy != NULL
      && (! S_IS_DEFINED (fixP->fx_addsy)
          || S_GET_SEGMENT (fixP->fx_addsy) != sec
	  || S_FORCE_RELOC(fixP->fx_addsy, 1)))
    {
      pr_debug("Unknown pcrel symbol: %s\n", S_GET_NAME(fixP->fx_addsy));

      /* The symbol is undefined (or is defined but not in this section).
	 Let the linker figure it out.  */
      return 0;
    }

  pr_debug("pcrel from %x + %x, symbol: %s (%x)\n",
	   fixP->fx_frag->fr_address, fixP->fx_where,
	   fixP->fx_addsy?S_GET_NAME(fixP->fx_addsy):"(null)",
	   fixP->fx_addsy?S_GET_VALUE(fixP->fx_addsy):0);

  return ((fixP->fx_frag->fr_address + fixP->fx_where)
	  & (~0UL << fixP->tc_fix_data.align));
}

valueT
md_section_align (segT segment, valueT size)
{
  int align = bfd_get_section_alignment (stdoutput, segment);
  return ((size + (1 << align) - 1) & (-1 << align));
}

static int syntax_matches(const struct avr32_syntax *syntax,
			  char *str)
{
  int i;

  pr_debug("syntax %d matches `%s'?\n", syntax->id, str);

  if (syntax->nr_operands < 0)
    {
      struct avr32_operand *op;
      int optype;

      for (i = 0; i < (-syntax->nr_operands - 1); i++)
	{
	  char *p;
	  char c;

	  optype = syntax->operand[i];
	  assert(optype < AVR32_NR_OPERANDS);
	  op = &avr32_operand_table[optype];

	  for (p = str; *p; p++)
	    if (*p == ',')
	      break;

	  if (p == str)
	    return 0;

	  c = *p;
	  *p = 0;

	  if (!op->match(str))
	    {
	      *p = c;
	      return 0;
	    }

	  str = p;
	  *p = c;
	  if (c)
	    str++;
	}

      optype = syntax->operand[i];
      assert(optype < AVR32_NR_OPERANDS);
      op = &avr32_operand_table[optype];

      if (!op->match(str))
	return 0;
      return 1;
    }

  for (i = 0; i < syntax->nr_operands; i++)
    {
      struct avr32_operand *op;
      int optype = syntax->operand[i];
      char *p;
      char c;

      assert(optype < AVR32_NR_OPERANDS);
      op = &avr32_operand_table[optype];

      for (p = str; *p; p++)
	if (*p == ',')
	  break;

      if (p == str)
	return 0;

      c = *p;
      *p = 0;

      if (!op->match(str))
	{
	  *p = c;
	  return 0;
	}

      str = p;
      *p = c;
      if (c)
	str++;
    }

  if (*str == '\0')
    return 1;

  if ((*str == 'e' || *str == 'E') && !str[1])
    return 1;

  return 0;
}

static int parse_operands(char *str)
{
  int i;

  if (current_insn.syntax->nr_operands < 0)
    {
      int optype;
      struct avr32_operand *op;

      for (i = 0; i < (-current_insn.syntax->nr_operands - 1); i++)
	{
	  char *p;
	  char c;

	  optype = current_insn.syntax->operand[i];
	  op = &avr32_operand_table[optype];

	  for (p = str; *p; p++)
	    if (*p == ',')
	      break;

	  assert(p != str);

	  c = *p, *p = 0;
	  op->parse(op, str, i);
	  *p = c;

	  str = p;
	  if (c) str++;
	}

      /* give the rest of the line to the last operand */
      optype = current_insn.syntax->operand[i];
      op = &avr32_operand_table[optype];
      op->parse(op, str, i);
    }
  else
    {
      for (i = 0; i < current_insn.syntax->nr_operands; i++)
	{
	  int optype = current_insn.syntax->operand[i];
	  struct avr32_operand *op = &avr32_operand_table[optype];
	  char *p;
	  char c;

	  skip_whitespace(str);

	  for (p = str; *p; p++)
	    if (*p == ',')
	      break;

	  assert(p != str);

	  c = *p, *p = 0;
	  op->parse(op, str, i);
	  *p = c;

	  str = p;
	  if (c) str++;
	}

      if (*str == 'E' || *str == 'e')
	current_insn.force_extended = 1;
    }

  return 0;
}

static const char *
finish_insn(const struct avr32_opcode *opc)
{
  expressionS *exp = &current_insn.immediate;
  unsigned int i;
  int will_relax = 0;
  char *buf;

  assert(current_insn.next_slot == opc->nr_fields);

  pr_debug("%s:%d: finish_insn: trying opcode %d\n",
	   frag_now->fr_file, frag_now->fr_line, opc->id);

  /* Go through the relaxation stage for all instructions that can
     possibly take a symbolic immediate.  The relax code will take
     care of range checking and alignment.  */
  if (opc->var_field != -1)
    {
      int substate, largest_substate;
      symbolS *sym;
      offsetT off;

      will_relax = 1;
      substate = largest_substate = opc_initial_substate(opc);

      while (relax_more(largest_substate) != AVR32_RS_NONE)
	largest_substate = relax_more(largest_substate);

      pr_debug("will relax. initial substate: %d (size %d), largest substate: %d (size %d)\n",
	       substate, avr32_rs_size(substate),
	       largest_substate, avr32_rs_size(largest_substate));

      /* make sure we have enough room for the largest possible opcode */
      frag_grow(avr32_rs_size(largest_substate));
      buf = frag_more(opc->size);

      dwarf2_emit_insn(opc->size);

      frag_now->tc_frag_data.reloc_info = AVR32_OPINFO_NONE;
      frag_now->tc_frag_data.pcrel = current_insn.pcrel;
      frag_now->tc_frag_data.force_extended = current_insn.force_extended;
      frag_now->tc_frag_data.relaxer = &avr32_default_relaxer;

      if (exp->X_op == O_hi)
	{
	  frag_now->tc_frag_data.reloc_info = AVR32_OPINFO_HI;
	  exp->X_op = exp->X_md;
	}
      else if (exp->X_op == O_lo)
	{
	  frag_now->tc_frag_data.reloc_info = AVR32_OPINFO_LO;
	  exp->X_op = exp->X_md;
	}
      else if (exp->X_op == O_got)
	{
	  frag_now->tc_frag_data.reloc_info = AVR32_OPINFO_GOT;
	  exp->X_op = O_symbol;
	}

#if 0
      if ((opc->reloc_type == BFD_RELOC_AVR32_SUB5)
	  && exp->X_op == O_subtract)
	{
	  symbolS *tmp;
	  tmp = exp->X_add_symbol;
	  exp->X_add_symbol = exp->X_op_symbol;
	  exp->X_op_symbol = tmp;
	}
#endif

      frag_now->tc_frag_data.exp = current_insn.immediate;

      sym = exp->X_add_symbol;
      off = exp->X_add_number;
      if (exp->X_op != O_symbol)
	{
	  sym = make_expr_symbol(exp);
	  off = 0;
	}

      frag_var(rs_machine_dependent,
	       avr32_rs_size(largest_substate) - opc->size,
	       opc->size,
	       substate, sym, off, buf);
    }
  else
    {
      assert(avr32_rs_size(opc_initial_substate(opc)) == 0);

      /* Make sure we always have room for another whole word, as the ifield
	 inserters can only write words. */
      frag_grow(4);
      buf = frag_more(opc->size);
      dwarf2_emit_insn(opc->size);
    }

  assert(!(opc->value & ~opc->mask));

  pr_debug("inserting opcode: 0x%lx\n", opc->value);
  bfd_putb32(opc->value, buf);

  for (i = 0; i < opc->nr_fields; i++)
    {
      const struct avr32_ifield *f = opc->fields[i];
      const struct avr32_ifield_data *fd = &current_insn.field_value[i];

      pr_debug("inserting field: 0x%lx & 0x%lx\n",
	       fd->value >> fd->align_order, f->mask);

      f->insert(f, buf, fd->value >> fd->align_order);
    }

  assert(will_relax || !current_insn.immediate.X_add_symbol);
  return NULL;
}

static const char *
finish_alias(const struct avr32_alias *alias)
{
  const struct avr32_opcode *opc;
  struct {
    unsigned long value;
    unsigned long align;
  } mapped_operand[AVR32_MAX_OPERANDS];
  unsigned int i;

  opc = alias->opc;

  /* Remap the operands from the alias to the real opcode */
  for (i = 0; i < opc->nr_fields; i++)
    {
      if (alias->operand_map[i].is_opindex)
	{
	  struct avr32_ifield_data *fd;
	  fd = &current_insn.field_value[alias->operand_map[i].value];
	  mapped_operand[i].value = fd->value;
	  mapped_operand[i].align = fd->align_order;
	}
      else
	{
	  mapped_operand[i].value = alias->operand_map[i].value;
	  mapped_operand[i].align = 0;
	}
    }

  for (i = 0; i < opc->nr_fields; i++)
    {
      current_insn.field_value[i].value = mapped_operand[i].value;
      if (opc->id == AVR32_OPC_COP)
	current_insn.field_value[i].align_order = 0;
      else
	current_insn.field_value[i].align_order
	  = mapped_operand[i].align;
    }

  current_insn.next_slot = opc->nr_fields;

  return finish_insn(opc);
}

static const char *
finish_lda(const struct avr32_syntax *syntax ATTRIBUTE_UNUSED)
{
  expressionS *exp = &current_insn.immediate;
  relax_substateT initial_subtype;
  symbolS *sym;
  offsetT off;
  int initial_size, max_size;
  char *buf;

  initial_size = LDA_INITIAL_SIZE;

  if (avr32_pic)
    {
      initial_subtype = LDA_SUBTYPE_SUB;
      if (linkrelax)
	max_size = 8;
      else
	max_size = 4;
    }
  else
    {
      initial_subtype = LDA_SUBTYPE_MOV1;
      max_size = 4;
    }

  frag_grow(max_size);
  buf = frag_more(initial_size);
  dwarf2_emit_insn(initial_size);

  if (exp->X_op == O_symbol)
    {
      sym = exp->X_add_symbol;
      off = exp->X_add_number;
    }
  else
    {
      sym = make_expr_symbol(exp);
      off = 0;
    }

  frag_now->tc_frag_data.reloc_info = current_insn.field_value[0].value;
  frag_now->tc_frag_data.relaxer = &avr32_lda_relaxer;

  if (!avr32_pic)
    {
      /* The relaxer will bump the refcount if necessary */
      frag_now->tc_frag_data.pool
	= add_to_cpool(exp, &frag_now->tc_frag_data.pool_entry, 0);
    }

  frag_var(rs_machine_dependent, max_size - initial_size,
	   initial_size, initial_subtype, sym, off, buf);

  return NULL;
}

static const char *
finish_call(const struct avr32_syntax *syntax ATTRIBUTE_UNUSED)
{
  expressionS *exp = &current_insn.immediate;
  symbolS *sym;
  offsetT off;
  int initial_size, max_size;
  char *buf;

  initial_size = CALL_INITIAL_SIZE;

  if (avr32_pic)
    {
      if (linkrelax)
	max_size = 10;
      else
	max_size = 4;
    }
  else
    max_size = 4;

  frag_grow(max_size);
  buf = frag_more(initial_size);
  dwarf2_emit_insn(initial_size);

  frag_now->tc_frag_data.relaxer = &avr32_call_relaxer;

  if (exp->X_op == O_symbol)
    {
      sym = exp->X_add_symbol;
      off = exp->X_add_number;
    }
  else
    {
      sym = make_expr_symbol(exp);
      off = 0;
    }

  if (!avr32_pic)
    {
      /* The relaxer will bump the refcount if necessary */
      frag_now->tc_frag_data.pool
	= add_to_cpool(exp, &frag_now->tc_frag_data.pool_entry, 0);
    }

  frag_var(rs_machine_dependent, max_size - initial_size,
	   initial_size, CALL_SUBTYPE_RCALL1, sym, off, buf);

  return NULL;
}

void
md_begin (void)
{
  unsigned long flags = 0;
  int i;

  avr32_mnemonic_htab = hash_new();

  if (!avr32_mnemonic_htab)
    as_fatal(_("virtual memory exhausted"));

  for (i = 0; i < AVR32_NR_MNEMONICS; i++)
    {
      hash_insert(avr32_mnemonic_htab, avr32_mnemonic_table[i].name,
		  (void *)&avr32_mnemonic_table[i]);
    }

  if (linkrelax)
    flags |= EF_AVR32_LINKRELAX;
  if (avr32_pic)
    flags |= EF_AVR32_PIC;

  bfd_set_private_flags(stdoutput, flags);

#ifdef OPC_CONSISTENCY_CHECK
  if (sizeof(avr32_operand_table)/sizeof(avr32_operand_table[0])
      < AVR32_NR_OPERANDS)
    as_fatal(_("operand table is incomplete"));

  for (i = 0; i < AVR32_NR_OPERANDS; i++)
    if (avr32_operand_table[i].id != i)
      as_fatal(_("operand table inconsistency found at index %d\n"), i);
  pr_debug("%d operands verified\n", AVR32_NR_OPERANDS);

  for (i = 0; i < AVR32_NR_IFIELDS; i++)
    if (avr32_ifield_table[i].id != i)
      as_fatal(_("ifield table inconsistency found at index %d\n"), i);
  pr_debug("%d instruction fields verified\n", AVR32_NR_IFIELDS);

  for (i = 0; i < AVR32_NR_OPCODES; i++)
    {
      if (avr32_opc_table[i].id != i)
	as_fatal(_("opcode table inconsistency found at index %d\n"), i);
      if ((avr32_opc_table[i].var_field == -1
	   && avr32_relax_table[i].length != 0)
	  || (avr32_opc_table[i].var_field != -1
	      && avr32_relax_table[i].length == 0))
	as_fatal(_("relax table inconsistency found at index %d\n"), i);
    }
  pr_debug("%d opcodes verified\n", AVR32_NR_OPCODES);

  for (i = 0; i < AVR32_NR_SYNTAX; i++)
    if (avr32_syntax_table[i].id != i)
      as_fatal(_("syntax table inconsistency found at index %d\n"), i);
  pr_debug("%d syntax variants verified\n", AVR32_NR_SYNTAX);

  for (i = 0; i < AVR32_NR_ALIAS; i++)
    if (avr32_alias_table[i].id != i)
      as_fatal(_("alias table inconsistency found at index %d\n"), i);
  pr_debug("%d aliases verified\n", AVR32_NR_ALIAS);

  for (i = 0; i < AVR32_NR_MNEMONICS; i++)
    if (avr32_mnemonic_table[i].id != i)
      as_fatal(_("mnemonic table inconsistency found at index %d\n"), i);
  pr_debug("%d mnemonics verified\n", AVR32_NR_MNEMONICS);
#endif
}

void
md_assemble (char *str)
{
  struct avr32_mnemonic *mnemonic;
  char *p, c;

  memset(&current_insn, 0, sizeof(current_insn));
  current_insn.immediate.X_op = O_constant;

  skip_whitespace(str);
  for (p = str; *p; p++)
    if (*p == ' ')
      break;
  c = *p;
  *p = 0;

  mnemonic = hash_find(avr32_mnemonic_htab, str);
  *p = c;
  if (c) p++;

  if (mnemonic)
    {
      const struct avr32_syntax *syntax;

      for (syntax = mnemonic->syntax; syntax; syntax = syntax->next)
	{
	  const char *errmsg = NULL;

	  if (syntax_matches(syntax, p))
	    {
	      if (!(syntax->isa_flags & avr32_arch->isa_flags))
		{
		  as_bad(_("Selected architecture `%s'  does not support `%s'"),
			 avr32_arch->name, str);
		  return;
		}

	      current_insn.syntax = syntax;
	      parse_operands(p);

	      switch (syntax->type)
		{
		case AVR32_PARSER_NORMAL:
		  errmsg = finish_insn(syntax->u.opc);
		  break;
		case AVR32_PARSER_ALIAS:
		  errmsg = finish_alias(syntax->u.alias);
		  break;
		case AVR32_PARSER_LDA:
		  errmsg = finish_lda(syntax);
		  break;
		case AVR32_PARSER_CALL:
		  errmsg = finish_call(syntax);
		  break;
		default:
		  BAD_CASE(syntax->type);
		  break;
		}

	      if (errmsg)
		as_bad("%s in `%s'", errmsg, str);

	      return;
	    }
	}

      as_bad(_("unrecognized form of instruction: `%s'"), str);
    }
  else
    as_bad(_("unrecognized instruction `%s'"), str);
}

void avr32_cleanup(void)
{
  struct cpool *pool;

  /* Emit any constant pools that haven't been explicitly flushed with
     a .cpool directive. */
  for (pool = cpool_list; pool; pool = pool->next)
    {
      subseg_set(pool->section, pool->sub_section);
      s_cpool(0);
    }
}

/* Handle any PIC-related operands in data allocation pseudo-ops */
void
avr32_cons_fix_new (fragS *frag, int off, int size, expressionS *exp)
{
  bfd_reloc_code_real_type r_type = BFD_RELOC_UNUSED;
  int pcrel = 0;

  pr_debug("%s:%u: cons_fix_new, add_sym: %s, op_sym: %s, op: %d, add_num: %d\n",
	   frag->fr_file, frag->fr_line,
	   exp->X_add_symbol?S_GET_NAME(exp->X_add_symbol):"(none)",
	   exp->X_op_symbol?S_GET_NAME(exp->X_op_symbol):"(none)",
	   exp->X_op, exp->X_add_number);

  if (exp->X_op == O_subtract && exp->X_op_symbol)
    {
      if (exp->X_op_symbol == GOT_symbol)
	{
	  if (size != 4)
	    goto bad_size;
	  r_type = BFD_RELOC_AVR32_GOTPC;
	  exp->X_op = O_symbol;
	  exp->X_op_symbol = NULL;
	}
    }
  else if (exp->X_op == O_got)
    {
      switch (size)
	{
	case 1:
	  r_type = BFD_RELOC_AVR32_GOT8;
	  break;
	case 2:
	  r_type = BFD_RELOC_AVR32_GOT16;
	  break;
	case 4:
	  r_type = BFD_RELOC_AVR32_GOT32;
	  break;
	default:
	  goto bad_size;
	}

      exp->X_op = O_symbol;
    }

  if (r_type == BFD_RELOC_UNUSED)
    switch (size)
      {
      case 1:
	r_type = BFD_RELOC_8;
	break;
      case 2:
	r_type = BFD_RELOC_16;
	break;
      case 4:
	r_type = BFD_RELOC_32;
	break;
      default:
	goto bad_size;
      }
  else if (size != 4)
    {
    bad_size:
      as_bad(_("unsupported BFD relocation size %u"), size);
      r_type = BFD_RELOC_UNUSED;
    }

  fix_new_exp (frag, off, size, exp, pcrel, r_type);
}

static void
avr32_frob_section(bfd *abfd ATTRIBUTE_UNUSED, segT sec,
		   void *ignore ATTRIBUTE_UNUSED)
{
  segment_info_type *seginfo;
  fixS *fix;

  seginfo = seg_info(sec);
  if (!seginfo)
    return;

  for (fix = seginfo->fix_root; fix; fix = fix->fx_next)
    {
      if (fix->fx_done)
	continue;

      if (fix->fx_r_type == BFD_RELOC_AVR32_SUB5
	  && fix->fx_addsy && fix->fx_subsy)
	{
	  if (S_GET_SEGMENT(fix->fx_addsy) != S_GET_SEGMENT(fix->fx_subsy)
	      || linkrelax)
	    {
	      symbolS *tmp;
#ifdef DEBUG
	      fprintf(stderr, "Swapping symbols in fixup:\n");
	      print_fixup(fix);
#endif
	      tmp = fix->fx_addsy;
	      fix->fx_addsy = fix->fx_subsy;
	      fix->fx_subsy = tmp;
	      fix->fx_offset = -fix->fx_offset;
	    }
	}
    }
}

/* We need to look for SUB5 instructions with expressions that will be
   made PC-relative and switch fx_addsy with fx_subsy.  This has to be
   done before adjustment or the wrong symbol might be adjusted.

   This applies to fixups that are a result of expressions like -(sym
   - .) and that will make it all the way to md_apply_fix3().  LDA
   does the right thing in convert_frag, so we must not convert
   those. */
void
avr32_frob_file(void)
{
  /* if (1 || !linkrelax)
     return; */

  bfd_map_over_sections(stdoutput, avr32_frob_section, NULL);
}

static bfd_boolean
convert_to_diff_reloc(fixS *fixP)
{
  switch (fixP->fx_r_type)
    {
    case BFD_RELOC_32:
      fixP->fx_r_type = BFD_RELOC_AVR32_DIFF32;
      break;
    case BFD_RELOC_16:
      fixP->fx_r_type = BFD_RELOC_AVR32_DIFF16;
      break;
    case BFD_RELOC_8:
      fixP->fx_r_type = BFD_RELOC_AVR32_DIFF8;
      break;
    default:
      return FALSE;
    }

  return TRUE;
}

/* Simplify a fixup.  If possible, the fixup is reduced to a single
   constant which is written to the output file.  Otherwise, a
   relocation is generated so that the linker can take care of the
   rest.

   ELF relocations have certain constraints: They can only take a
   single symbol and a single addend.  This means that for difference
   expressions, we _must_ get rid of the fx_subsy symbol somehow.

   The difference between two labels in the same section can be
   calculated directly unless 'linkrelax' is set, or a relocation is
   forced.  If so, we must emit a R_AVR32_DIFFxx relocation.  If there
   are addends involved at this point, we must be especially careful
   as the relocation must point exactly to the symbol being
   subtracted.

   When subtracting a symbol defined in the same section as the fixup,
   we might be able to convert it to a PC-relative expression, unless
   linkrelax is set. If this is the case, there's no way we can make
   sure that the difference between the fixup and fx_subsy stays
   constant.  So for now, we're just going to disallow that.
   */
void
avr32_process_fixup(fixS *fixP, segT this_segment)
{
  segT add_symbol_segment = absolute_section;
  segT sub_symbol_segment = absolute_section;
  symbolS *fx_addsy, *fx_subsy;
  offsetT value = 0, fx_offset;
  bfd_boolean apply = FALSE;

  assert(this_segment != absolute_section);

  if (fixP->fx_r_type >= BFD_RELOC_UNUSED)
    {
      as_bad_where(fixP->fx_file, fixP->fx_line,
		   _("Bad relocation type %d\n"), fixP->fx_r_type);
      return;
    }

  /* BFD_RELOC_AVR32_SUB5 fixups have been swapped by avr32_frob_section() */
  fx_addsy = fixP->fx_addsy;
  fx_subsy = fixP->fx_subsy;
  fx_offset = fixP->fx_offset;

  if (fx_addsy)
    add_symbol_segment = S_GET_SEGMENT(fx_addsy);

  if (fx_subsy)
    {
      resolve_symbol_value(fx_subsy);
      sub_symbol_segment = S_GET_SEGMENT(fx_subsy);

      if (sub_symbol_segment == this_segment
	  && (!linkrelax
	      || S_GET_VALUE(fx_subsy) == (fixP->fx_frag->fr_address
					   + fixP->fx_where)))
	{
	  fixP->fx_pcrel = TRUE;
	  fx_offset += (fixP->fx_frag->fr_address + fixP->fx_where
			- S_GET_VALUE(fx_subsy));
	  fx_subsy = NULL;
	}
      else if (sub_symbol_segment == absolute_section)
	{
	  /* The symbol is really a constant.  */
	  fx_offset -= S_GET_VALUE(fx_subsy);
	  fx_subsy = NULL;
	}
      else if (SEG_NORMAL(add_symbol_segment)
	       && sub_symbol_segment == add_symbol_segment
	       && (!linkrelax || convert_to_diff_reloc(fixP)))
	{
	  /* Difference between two labels in the same section.  */
	  if (linkrelax)
	    {
	      /* convert_to_diff() has ensured that the reloc type is
		 either DIFF32, DIFF16 or DIFF8.  */
	      value = (S_GET_VALUE(fx_addsy) + fixP->fx_offset
		       - S_GET_VALUE(fx_subsy));

	      /* Try to convert it to a section symbol if possible  */
	      if (!S_FORCE_RELOC(fx_addsy, 1)
		  && !(sub_symbol_segment->flags & SEC_THREAD_LOCAL))
		{
		  fx_offset = S_GET_VALUE(fx_subsy);
		  fx_addsy = section_symbol(sub_symbol_segment);
		}
	      else
		{
		  fx_addsy = fx_subsy;
		  fx_offset = 0;
		}

	      fx_subsy = NULL;
	      apply = TRUE;
	    }
	  else
	    {
	      fx_offset += S_GET_VALUE(fx_addsy);
	      fx_offset -= S_GET_VALUE(fx_subsy);
	      fx_addsy = NULL;
	      fx_subsy = NULL;
	    }
	}
      else
	{
	  as_bad_where(fixP->fx_file, fixP->fx_line,
		       _("can't resolve `%s' {%s section} - `%s' {%s section}"),
		       fx_addsy ? S_GET_NAME (fx_addsy) : "0",
		       segment_name (add_symbol_segment),
		       S_GET_NAME (fx_subsy),
		       segment_name (sub_symbol_segment));
	  return;
	}
    }

  if (fx_addsy && !TC_FORCE_RELOCATION(fixP))
    {
      if (add_symbol_segment == this_segment
	  && fixP->fx_pcrel)
	{
	  value += S_GET_VALUE(fx_addsy);
	  value -= md_pcrel_from_section(fixP, this_segment);
	  fx_addsy = NULL;
	  fixP->fx_pcrel = FALSE;
	}
      else if (add_symbol_segment == absolute_section)
	{
	  fx_offset += S_GET_VALUE(fixP->fx_addsy);
	  fx_addsy = NULL;
	}
    }

  if (!fx_addsy)
    fixP->fx_done = TRUE;

  if (fixP->fx_pcrel)
    {
      if (fx_addsy != NULL
	  && S_IS_DEFINED(fx_addsy)
	  && S_GET_SEGMENT(fx_addsy) != this_segment)
	value += md_pcrel_from_section(fixP, this_segment);

      switch (fixP->fx_r_type)
	{
	case BFD_RELOC_32:
	  fixP->fx_r_type = BFD_RELOC_32_PCREL;
	  break;
	case BFD_RELOC_16:
	  fixP->fx_r_type = BFD_RELOC_16_PCREL;
	  break;
	case BFD_RELOC_8:
	  fixP->fx_r_type = BFD_RELOC_8_PCREL;
	  break;
	case BFD_RELOC_AVR32_SUB5:
	  fixP->fx_r_type = BFD_RELOC_AVR32_16N_PCREL;
	  break;
	case BFD_RELOC_AVR32_16S:
	  fixP->fx_r_type = BFD_RELOC_AVR32_16B_PCREL;
	  break;
	case BFD_RELOC_AVR32_14UW:
	  fixP->fx_r_type = BFD_RELOC_AVR32_14UW_PCREL;
	  break;
	case BFD_RELOC_AVR32_10UW:
	  fixP->fx_r_type = BFD_RELOC_AVR32_10UW_PCREL;
	  break;
	default:
	  /* Should have been taken care of already */
	  break;
	}
    }

  if (fixP->fx_done || apply)
    {
      const struct avr32_ifield *ifield;
      char *buf = fixP->fx_where + fixP->fx_frag->fr_literal;

      if (fixP->fx_done)
	value += fx_offset;

      /* For hosts with longs bigger than 32-bits make sure that the top
         bits of a 32-bit negative value read in by the parser are set,
         so that the correct comparisons are made.  */
      if (value & 0x80000000)
        value |= (-1L << 31);

      switch (fixP->fx_r_type)
	{
	case BFD_RELOC_32:
	case BFD_RELOC_16:
	case BFD_RELOC_8:
	case BFD_RELOC_AVR32_DIFF32:
	case BFD_RELOC_AVR32_DIFF16:
	case BFD_RELOC_AVR32_DIFF8:
	  md_number_to_chars(buf, value, fixP->fx_size);
	  break;
	case BFD_RELOC_HI16:
	  value >>= 16;
	case BFD_RELOC_LO16:
	  value &= 0xffff;
	  md_number_to_chars(buf + 2, value, 2);
	  break;
	case BFD_RELOC_AVR32_16N_PCREL:
	  value = -value;
	  /* fall through */
	case BFD_RELOC_AVR32_22H_PCREL:
	case BFD_RELOC_AVR32_18W_PCREL:
	case BFD_RELOC_AVR32_16B_PCREL:
	case BFD_RELOC_AVR32_11H_PCREL:
	case BFD_RELOC_AVR32_9H_PCREL:
	case BFD_RELOC_AVR32_9UW_PCREL:
	case BFD_RELOC_AVR32_3U:
	case BFD_RELOC_AVR32_4UH:
	case BFD_RELOC_AVR32_6UW:
	case BFD_RELOC_AVR32_6S:
	case BFD_RELOC_AVR32_7UW:
	case BFD_RELOC_AVR32_8S_EXT:
	case BFD_RELOC_AVR32_8S:
	case BFD_RELOC_AVR32_10UW:
	case BFD_RELOC_AVR32_10SW:
	case BFD_RELOC_AVR32_STHH_W:
	case BFD_RELOC_AVR32_14UW:
	case BFD_RELOC_AVR32_16S:
	case BFD_RELOC_AVR32_16U:
	case BFD_RELOC_AVR32_21S:
	case BFD_RELOC_AVR32_SUB5:
	case BFD_RELOC_AVR32_CPCALL:
	case BFD_RELOC_AVR32_16_CP:
	case BFD_RELOC_AVR32_9W_CP:
	case BFD_RELOC_AVR32_15S:
	  ifield = fixP->tc_fix_data.ifield;
	  pr_debug("insert field: %ld <= %ld <= %ld (align %u)\n",
		   fixP->tc_fix_data.min, value, fixP->tc_fix_data.max,
		   fixP->tc_fix_data.align);
	  if (value < fixP->tc_fix_data.min || value > fixP->tc_fix_data.max)
	    as_bad_where(fixP->fx_file, fixP->fx_line,
			 _("operand out of range (%ld not between %ld and %ld)"),
			 value, fixP->tc_fix_data.min, fixP->tc_fix_data.max);
	  if (value & ((1 << fixP->tc_fix_data.align) - 1))
	    as_bad_where(fixP->fx_file, fixP->fx_line,
			 _("misaligned operand (required alignment: %d)"),
			 1 << fixP->tc_fix_data.align);
	  ifield->insert(ifield, buf, value >> fixP->tc_fix_data.align);
	  break;
	case BFD_RELOC_AVR32_ALIGN:
	  /* Nothing to do */
	  fixP->fx_done = FALSE;
	  break;
	default:
	  as_fatal("reloc type %s not handled\n",
		   bfd_get_reloc_code_name(fixP->fx_r_type));
	}
    }

  fixP->fx_addsy = fx_addsy;
  fixP->fx_subsy = fx_subsy;
  fixP->fx_offset = fx_offset;

  if (!fixP->fx_done)
    {
      if (!fixP->fx_addsy)
	fixP->fx_addsy = abs_section_sym;

      symbol_mark_used_in_reloc(fixP->fx_addsy);
      if (fixP->fx_subsy)
	abort();
    }
}

#if 0
void
md_apply_fix3 (fixS *fixP, valueT *valP, segT seg)
{
  const struct avr32_ifield *ifield;
  offsetT	value = *valP;
  char		*buf = fixP->fx_where + fixP->fx_frag->fr_literal;
  bfd_boolean	apply;

  pr_debug("%s:%u: apply_fix3: r_type=%d value=%lx offset=%lx\n",
	   fixP->fx_file, fixP->fx_line, fixP->fx_r_type, *valP,
	   fixP->fx_offset);

  if (fixP->fx_r_type >= BFD_RELOC_UNUSED)
    {
      as_bad_where(fixP->fx_file, fixP->fx_line,
		   _("Bad relocation type %d\n"), fixP->fx_r_type);
      return;
    }

  if (!fixP->fx_addsy && !fixP->fx_subsy)
    fixP->fx_done = 1;

  if (fixP->fx_pcrel)
    {
      if (fixP->fx_addsy != NULL
	  && S_IS_DEFINED(fixP->fx_addsy)
	  && S_GET_SEGMENT(fixP->fx_addsy) != seg)
	value += md_pcrel_from_section(fixP, seg);

      switch (fixP->fx_r_type)
	{
	case BFD_RELOC_32:
	  fixP->fx_r_type = BFD_RELOC_32_PCREL;
	  break;
	case BFD_RELOC_16:
	case BFD_RELOC_8:
	  as_bad_where (fixP->fx_file, fixP->fx_line,
			_("8- and 16-bit PC-relative relocations not supported"));
	  break;
	case BFD_RELOC_AVR32_SUB5:
	  fixP->fx_r_type = BFD_RELOC_AVR32_PCREL_SUB5;
	  break;
	case BFD_RELOC_AVR32_16S:
	  fixP->fx_r_type = BFD_RELOC_AVR32_16_PCREL;
	  break;
	default:
	  /* Should have been taken care of already */
	  break;
	}
    }

  if (fixP->fx_r_type == BFD_RELOC_32
      && fixP->fx_subsy)
    {
      fixP->fx_r_type = BFD_RELOC_AVR32_DIFF32;

      /* Offsets are only allowed if it's a result of adjusting a
	 local symbol into a section-relative offset.
	 tc_fix_adjustable() should prevent any adjustment if there
	 was an offset involved before.  */
      if (fixP->fx_offset && !symbol_section_p(fixP->fx_addsy))
	as_bad_where(fixP->fx_file, fixP->fx_line,
		     _("cannot represent symbol difference with an offset"));

      value = (S_GET_VALUE(fixP->fx_addsy) + fixP->fx_offset
	       - S_GET_VALUE(fixP->fx_subsy));

      /* The difference before any relaxing takes place is written
	 out, and the DIFF32 reloc identifies the address of the first
	 symbol (i.e. the on that's subtracted.)  */
      *valP = value;
      fixP->fx_offset -= value;
      fixP->fx_subsy = NULL;

      md_number_to_chars(buf, value, fixP->fx_size);
    }

  if (fixP->fx_done)
    {
      switch (fixP->fx_r_type)
	{
	case BFD_RELOC_8:
	case BFD_RELOC_16:
	case BFD_RELOC_32:
	  md_number_to_chars(buf, value, fixP->fx_size);
	  break;
	case BFD_RELOC_HI16:
	  value >>= 16;
	case BFD_RELOC_LO16:
	  value &= 0xffff;
	  *valP = value;
	  md_number_to_chars(buf + 2, value, 2);
	  break;
	case BFD_RELOC_AVR32_PCREL_SUB5:
	  value = -value;
	  /* fall through */
	case BFD_RELOC_AVR32_9_PCREL:
	case BFD_RELOC_AVR32_11_PCREL:
	case BFD_RELOC_AVR32_16_PCREL:
	case BFD_RELOC_AVR32_18_PCREL:
	case BFD_RELOC_AVR32_22_PCREL:
	case BFD_RELOC_AVR32_3U:
	case BFD_RELOC_AVR32_4UH:
	case BFD_RELOC_AVR32_6UW:
	case BFD_RELOC_AVR32_6S:
	case BFD_RELOC_AVR32_7UW:
	case BFD_RELOC_AVR32_8S:
	case BFD_RELOC_AVR32_10UW:
	case BFD_RELOC_AVR32_10SW:
	case BFD_RELOC_AVR32_14UW:
	case BFD_RELOC_AVR32_16S:
	case BFD_RELOC_AVR32_16U:
	case BFD_RELOC_AVR32_21S:
	case BFD_RELOC_AVR32_BRC1:
	case BFD_RELOC_AVR32_SUB5:
	case BFD_RELOC_AVR32_CPCALL:
	case BFD_RELOC_AVR32_16_CP:
	case BFD_RELOC_AVR32_9_CP:
	case BFD_RELOC_AVR32_15S:
	  ifield = fixP->tc_fix_data.ifield;
	  pr_debug("insert field: %ld <= %ld <= %ld (align %u)\n",
		   fixP->tc_fix_data.min, value, fixP->tc_fix_data.max,
		   fixP->tc_fix_data.align);
	  if (value < fixP->tc_fix_data.min || value > fixP->tc_fix_data.max)
	    as_bad_where(fixP->fx_file, fixP->fx_line,
			 _("operand out of range (%ld not between %ld and %ld)"),
			 value, fixP->tc_fix_data.min, fixP->tc_fix_data.max);
	  if (value & ((1 << fixP->tc_fix_data.align) - 1))
	    as_bad_where(fixP->fx_file, fixP->fx_line,
			 _("misaligned operand (required alignment: %d)"),
			 1 << fixP->tc_fix_data.align);
	  ifield->insert(ifield, buf, value >> fixP->tc_fix_data.align);
	  break;
	case BFD_RELOC_AVR32_ALIGN:
	  /* Nothing to do */
	  fixP->fx_done = FALSE;
	  break;
	default:
	  as_fatal("reloc type %s not handled\n",
		   bfd_get_reloc_code_name(fixP->fx_r_type));
	}
    }
}
#endif

arelent *
tc_gen_reloc (asection *section ATTRIBUTE_UNUSED,
	      fixS *fixp)
{
  arelent *reloc;
  bfd_reloc_code_real_type code;

  reloc = xmalloc (sizeof (arelent));

  reloc->sym_ptr_ptr = xmalloc (sizeof (asymbol *));
  *reloc->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_addsy);
  reloc->address = fixp->fx_frag->fr_address + fixp->fx_where;
  reloc->addend = fixp->fx_offset;
  code = fixp->fx_r_type;

  reloc->howto = bfd_reloc_type_lookup (stdoutput, code);

  if (reloc->howto == NULL)
    {
      as_bad_where (fixp->fx_file, fixp->fx_line,
		    _("cannot represent relocation %s in this object file format"),
		    bfd_get_reloc_code_name (code));
      return NULL;
    }

  return reloc;
}

bfd_boolean
avr32_force_reloc(fixS *fixP)
{
  if (linkrelax && fixP->fx_addsy
      && !(S_GET_SEGMENT(fixP->fx_addsy)->flags & SEC_DEBUGGING)
      && S_GET_SEGMENT(fixP->fx_addsy) != absolute_section)
    {
      pr_debug(stderr, "force reloc: addsy=%p, r_type=%d, sec=%s\n",
	       fixP->fx_addsy, fixP->fx_r_type, S_GET_SEGMENT(fixP->fx_addsy)->name);
      return 1;
    }

  return generic_force_reloc(fixP);
}

bfd_boolean
avr32_fix_adjustable(fixS *fixP)
{
  switch (fixP->fx_r_type)
    {
      /* GOT relocations can't have addends since BFD treats all
	 references to a given symbol the same. This means that we
	 must avoid section-relative references to local symbols when
	 dealing with these kinds of relocs */
    case BFD_RELOC_AVR32_GOT32:
    case BFD_RELOC_AVR32_GOT16:
    case BFD_RELOC_AVR32_GOT8:
    case BFD_RELOC_AVR32_GOT21S:
    case BFD_RELOC_AVR32_GOT18SW:
    case BFD_RELOC_AVR32_GOT16S:
    case BFD_RELOC_AVR32_LDA_GOT:
    case BFD_RELOC_AVR32_GOTCALL:
      pr_debug("fix not adjustable\n");
      return 0;

    default:
      break;
    }

  return 1;
}

/* When we want the linker to be able to relax the code, we need to
   output a reloc for every .align directive requesting an alignment
   to a four byte boundary or larger.  If we don't do this, the linker
   can't guarantee that the alignment is actually maintained in the
   linker output.

   TODO: Might as well insert proper NOPs while we're at it... */
void
avr32_handle_align(fragS *frag)
{
  if (linkrelax
      && frag->fr_type == rs_align_code
      && frag->fr_address + frag->fr_fix > 0
      && frag->fr_offset > 0)
    {
      /* The alignment order (fr_offset) is stored in the addend. */
      fix_new(frag, frag->fr_fix, 2, &abs_symbol, frag->fr_offset,
	      FALSE, BFD_RELOC_AVR32_ALIGN);
    }
}

/* Relax_align. Advance location counter to next address that has 'alignment'
   lowest order bits all 0s, return size of adjustment made.  */
relax_addressT
avr32_relax_align(segT segment ATTRIBUTE_UNUSED,
		  fragS *fragP,
		  relax_addressT address)
{
  relax_addressT mask;
  relax_addressT new_address;
  int alignment;

  alignment = fragP->fr_offset;
  mask = ~((~0) << alignment);
  new_address = (address + mask) & (~mask);

  return new_address - address;
}

/* Turn a string in input_line_pointer into a floating point constant
   of type type, and store the appropriate bytes in *litP.  The number
   of LITTLENUMS emitted is stored in *sizeP .  An error message is
   returned, or NULL on OK. */

/* Equal to MAX_PRECISION in atof-ieee.c */
#define MAX_LITTLENUMS 6

char *
md_atof (type, litP, sizeP)
char   type;
char * litP;
int *  sizeP;
{
  int              i;
  int              prec;
  LITTLENUM_TYPE   words [MAX_LITTLENUMS];
  char *           t;

  switch (type)
  {
    case 'f':
    case 'F':
    case 's':
    case 'S':
      prec = 2;
      break;

    case 'd':
    case 'D':
    case 'r':
    case 'R':
      prec = 4;
      break;

      /* FIXME: Some targets allow other format chars for bigger sizes here.  */

    default:
      * sizeP = 0;
      return _("Bad call to md_atof()");
  }

  t = atof_ieee (input_line_pointer, type, words);
  if (t)
    input_line_pointer = t;
  * sizeP = prec * sizeof (LITTLENUM_TYPE);

  for (i = 0; i < prec; i++)
  {
    md_number_to_chars (litP, (valueT) words[i],
                        sizeof (LITTLENUM_TYPE));
    litP += sizeof (LITTLENUM_TYPE);
  }

  return 0;
}

static char *avr32_end_of_match(char *cont, char *what)
{
  int len = strlen (what);

  if (! is_part_of_name (cont[len])
      && strncasecmp (cont, what, len) == 0)
    return cont + len;

  return NULL;
}

int
avr32_parse_name (char const *name, expressionS *exp, char *nextchar)
{
  char *next = input_line_pointer;
  char *next_end;

  pr_debug("parse_name: %s, nextchar=%c (%02x)\n", name, *nextchar, *nextchar);

  if (*nextchar == '(')
    {
      if (strcasecmp(name, "hi") == 0)
	{
	  *next = *nextchar;

	  expression(exp);

	  if (exp->X_op == O_constant)
	    {
	      pr_debug("  -> constant hi(0x%08lx) -> 0x%04lx\n",
		       exp->X_add_number, exp->X_add_number >> 16);
	      exp->X_add_number = (exp->X_add_number >> 16) & 0xffff;
	    }
	  else
	    {
	      exp->X_md = exp->X_op;
	      exp->X_op = O_hi;
	    }

	  return 1;
	}
      else if (strcasecmp(name, "lo") == 0)
	{
	  *next = *nextchar;

	  expression(exp);

	  if (exp->X_op == O_constant)
	    exp->X_add_number &= 0xffff;
	  else
	    {
	      exp->X_md = exp->X_op;
	      exp->X_op = O_lo;
	    }

	  return 1;
	}
    }
  else if (*nextchar == '@')
    {
      exp->X_md = exp->X_op;

      if ((next_end = avr32_end_of_match (next + 1, "got")))
	exp->X_op = O_got;
      else if ((next_end = avr32_end_of_match (next + 1, "tlsgd")))
	exp->X_op = O_tlsgd;
      /* Add more as needed */
      else
	{
	  char c;
	  input_line_pointer++;
	  c = get_symbol_end();
	  as_bad (_("unknown relocation override `%s'"), next + 1);
	  *input_line_pointer = c;
	  input_line_pointer = next;
	  return 0;
	}

      exp->X_op_symbol = NULL;
      exp->X_add_symbol = symbol_find_or_make (name);
      exp->X_add_number = 0;

      *input_line_pointer = *nextchar;
      input_line_pointer = next_end;
      *nextchar = *input_line_pointer;
      *input_line_pointer = '\0';
      return 1;
    }
  else if (strcmp (name, "_GLOBAL_OFFSET_TABLE_") == 0)
    {
      if (!GOT_symbol)
	GOT_symbol = symbol_find_or_make(name);

      exp->X_add_symbol = GOT_symbol;
      exp->X_op = O_symbol;
      exp->X_add_number = 0;
      return 1;
    }

  return 0;
}

static void
s_rseg (int value ATTRIBUTE_UNUSED)
{
  /* Syntax: RSEG segment_name [:type] [NOROOT|ROOT] [(align)]
   * Defaults:
   *  - type: undocumented ("typically CODE or DATA")
   *  - ROOT
   *  - align: 1 for code, 0 for others
   *
   * TODO: NOROOT is ignored. If gas supports discardable segments, it should
   * be implemented.
   */
  char *name, *end;
  int length, type, attr;
  int align = 0;

  SKIP_WHITESPACE();

  end = input_line_pointer;
  while (0 == strchr ("\n\t;:( ", *end))
    end++;
  if (end == input_line_pointer)
    {
      as_warn (_("missing name"));
      ignore_rest_of_line();
      return;
    }

  name = xmalloc (end - input_line_pointer + 1);
  memcpy (name, input_line_pointer, end - input_line_pointer);
  name[end - input_line_pointer] = '\0';
  input_line_pointer = end;

  SKIP_WHITESPACE();

  type = SHT_NULL;
  attr = 0;

  if (*input_line_pointer == ':')
    {
      /* Skip the colon */
      ++input_line_pointer;
      SKIP_WHITESPACE();

      /* Possible options at this point:
       *   - flag (ROOT or NOROOT)
       *   - a segment type
       */
      end = input_line_pointer;
      while (0 == strchr ("\n\t;:( ", *end))
	end++;
      length = end - input_line_pointer;
      if (((length == 4) && (0 == strncasecmp( input_line_pointer, "ROOT", 4))) ||
	  ((length == 6) && (0 == strncasecmp( input_line_pointer, "NOROOT", 6))))
	{
	  /* Ignore ROOT/NOROOT */
	  input_line_pointer = end;
	}
      else
	{
	  /* Must be a segment type */
	  switch (*input_line_pointer)
	    {
	    case 'C':
	    case 'c':
	      if ((length == 4) &&
		  (0 == strncasecmp (input_line_pointer, "CODE", 4)))
		{
		  attr |= SHF_ALLOC | SHF_EXECINSTR;
		  type = SHT_PROGBITS;
		  align = 1;
		  break;
		}
	      if ((length == 5) &&
		  (0 == strncasecmp (input_line_pointer, "CONST", 5)))
		{
		  attr |= SHF_ALLOC;
		  type = SHT_PROGBITS;
		  break;
		}
	      goto de_fault;

	    case 'D':
	    case 'd':
	      if ((length == 4) &&
		  (0 == strncasecmp (input_line_pointer, "DATA", 4)))
		{
		  attr |= SHF_ALLOC | SHF_WRITE;
		  type = SHT_PROGBITS;
		  break;
		}
	      goto de_fault;

	      /* TODO: Add FAR*, HUGE*, IDATA and NEAR* if necessary */

	    case 'U':
	    case 'u':
	      if ((length == 7) &&
		  (0 == strncasecmp (input_line_pointer, "UNTYPED", 7)))
		break;
	      goto de_fault;

	      /* TODO: Add XDATA and ZPAGE if necessary */

	    de_fault:
	    default:
	      as_warn (_("unrecognized segment type"));
	    }

	  input_line_pointer = end;
	  SKIP_WHITESPACE();

	  if (*input_line_pointer == ':')
	    {
	      /*  ROOT/NOROOT */
	      ++input_line_pointer;
	      SKIP_WHITESPACE();

	      end = input_line_pointer;
	      while (0 == strchr ("\n\t;:( ", *end))
		end++;
	      length = end - input_line_pointer;
	      if (! ((length == 4) &&
		     (0 == strncasecmp( input_line_pointer, "ROOT", 4))) &&
		  ! ((length == 6) &&
		     (0 == strncasecmp( input_line_pointer, "NOROOT", 6))))
		{
		  as_warn (_("unrecognized segment flag"));
		}

	      input_line_pointer = end;
	      SKIP_WHITESPACE();
	    }
	}
    }

  if (*input_line_pointer == '(')
    {
      align = get_absolute_expression ();
    }

  demand_empty_rest_of_line();

  obj_elf_change_section (name, type, attr, 0, NULL, 0, 0);
#ifdef AVR32_DEBUG
  fprintf( stderr, "RSEG: Changed section to %s, type: 0x%x, attr: 0x%x\n",
      name, type, attr );
  fprintf( stderr, "RSEG: Aligning to 2**%d\n", align );
#endif

  if (align > 15)
    {
      align = 15;
      as_warn (_("alignment too large: %u assumed"), align);
    }

  /* Hope not, that is */
  assert (now_seg != absolute_section);

  /* Only make a frag if we HAVE to... */
  if (align != 0 && !need_pass_2)
    {
      if (subseg_text_p (now_seg))
	frag_align_code (align, 0);
      else
	frag_align (align, 0, 0);
    }

  record_alignment (now_seg, align - OCTETS_PER_BYTE_POWER);
}

/* vim: syntax=c sw=2
 */

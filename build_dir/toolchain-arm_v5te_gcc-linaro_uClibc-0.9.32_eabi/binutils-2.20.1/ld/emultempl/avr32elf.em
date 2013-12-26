# This shell script emits a C file. -*- C -*-
#   Copyright (C) 2007,2008,2009 Atmel Corporation
#
# This file is part of GLD, the Gnu Linker.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
#

# This file is sourced from elf32.em, and defines extra avr32-elf
# specific routines.
#

# Generate linker script for writable rodata
LD_FLAG=rodata-writable
DATA_ALIGNMENT=${DATA_ALIGNMENT_}
RELOCATING=" "
WRITABLE_RODATA=" "
( echo "/* Linker script for writable rodata */"
  . ${CUSTOMIZER_SCRIPT} ${EMULATION_NAME}
  . ${srcdir}/scripttempl/${SCRIPT_NAME}.sc
) | sed -e '/^ *$/d;s/[ 	]*$//' > ldscripts/${EMULATION_NAME}.xwr


cat >> e${EMULATION_NAME}.c <<EOF

#include "libbfd.h"
#include "elf32-avr32.h"

/* Whether to allow direct references (sub or mov) to SEC_DATA and
   !SEC_CONTENTS sections when optimizing.  Not enabled by default
   since it might cause link errors.  */
static int direct_data_refs = 0;

static void avr32_elf_after_open (void)
{
  bfd_elf32_avr32_set_options (&link_info, direct_data_refs);
  gld${EMULATION_NAME}_after_open ();
}

static int rodata_writable = 0;

static int stack_size = 0x1000;

static void avr32_elf_set_symbols (void)
{
  /* Glue the assignments into the abs section.  */
  lang_statement_list_type *save = stat_ptr;


  stat_ptr = &(abs_output_section->children);

  lang_add_assignment (exp_assop ('=', "_stack_size",
                                  exp_intop (stack_size)));
  
  stat_ptr = save;
}

static char * gld${EMULATION_NAME}_get_script (int *isfile);

static char * avr32_elf_get_script (int *isfile)
{
  if ( rodata_writable )
    {
EOF
if test -n "$COMPILE_IN"
then
# Scripts compiled in.

# sed commands to quote an ld script as a C string.
sc="-f stringify.sed"

cat >>e${EMULATION_NAME}.c <<EOF
      *isfile = 0;
      return
EOF
sed $sc ldscripts/${EMULATION_NAME}.xwr			>> e${EMULATION_NAME}.c
echo  ';'	                                        >> e${EMULATION_NAME}.c
else
# Scripts read from the filesystem.

cat >>e${EMULATION_NAME}.c <<EOF
      *isfile = 1;
      return "ldscripts/${EMULATION_NAME}.xwr";
EOF
fi

cat >>e${EMULATION_NAME}.c <<EOF
    }
  return gld${EMULATION_NAME}_get_script (isfile);
}


EOF

# Define some shell vars to insert bits of code into the standard elf
# parse_args and list_options functions.
#
PARSE_AND_LIST_PROLOGUE='
#define OPTION_DIRECT_DATA		300
#define OPTION_NO_DIRECT_DATA		301
#define OPTION_RODATA_WRITABLE		302
#define OPTION_NO_RODATA_WRITABLE	303
#define OPTION_STACK 	                304
'

PARSE_AND_LIST_LONGOPTS='
  { "direct-data", no_argument, NULL, OPTION_DIRECT_DATA },
  { "no-direct-data", no_argument, NULL, OPTION_NO_DIRECT_DATA },
  { "rodata-writable", no_argument, NULL, OPTION_RODATA_WRITABLE },
  { "no-rodata-writable", no_argument, NULL, OPTION_NO_RODATA_WRITABLE },
  { "stack", required_argument, NULL, OPTION_STACK },
'

PARSE_AND_LIST_OPTIONS='
  fprintf (file, _("  --direct-data\t\tAllow direct data references when optimizing\n"));
  fprintf (file, _("  --no-direct-data\tDo not allow direct data references when optimizing\n"));
  fprintf (file, _("  --rodata-writable\tPut read-only data in writable data section\n"));
  fprintf (file, _("  --no-rodata-writable\tDo not put read-only data in writable data section\n"));
  fprintf (file, _("  --stack <size>\tSet the initial size of the stack\n"));
'

PARSE_AND_LIST_ARGS_CASES='
    case OPTION_DIRECT_DATA:
      direct_data_refs = 1;
      break;
    case OPTION_NO_DIRECT_DATA:
      direct_data_refs = 0;
      break;
    case OPTION_RODATA_WRITABLE:
      rodata_writable = 1;
      break;
    case OPTION_NO_RODATA_WRITABLE:
      rodata_writable = 0;
      break;
    case OPTION_STACK: 
     {
      char *end;
      stack_size = strtoul (optarg, &end, 0);
      if (end == optarg)
        einfo (_("%P%F: invalid hex number for parameter '%s'\n"), optarg);
      optarg = end;
      break;
     }
'

# Replace some of the standard ELF functions with our own versions.
#
LDEMUL_AFTER_OPEN=avr32_elf_after_open
LDEMUL_GET_SCRIPT=avr32_elf_get_script
LDEMUL_SET_SYMBOLS=avr32_elf_set_symbols

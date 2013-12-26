/* Definitions of target machine for Ubicom32-uclinux

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

/* Don't assume anything about the header files.  */
#define NO_IMPLICIT_EXTERN_C

#undef  LIB_SPEC
#define LIB_SPEC  \
	"%{pthread:-lpthread} " \
	"-lc"

#undef LINK_GCC_C_SEQUENCE_SPEC
#define LINK_GCC_C_SEQUENCE_SPEC \
  "%{static:--start-group} %G %L %{static:--end-group} " \
  "%{!static: %G}"

#undef STARTFILE_SPEC
#define STARTFILE_SPEC \
  "%{!shared: %{pg|p|profile:gcrt1%O%s;pie:Scrt1%O%s;:crt1%O%s}} " \
  "crtreloc%O%s crti%O%s %{shared|pie:crtbeginS%O%s;:crtbegin%O%s}"

#undef ENDFILE_SPEC
#define ENDFILE_SPEC \
  "%{shared|pie:crtendS%O%s;:crtend%O%s} crtn%O%s"

/* taken from linux.h */
/* The GNU C++ standard library requires that these macros be defined.  */
#undef CPLUSPLUS_CPP_SPEC
#define CPLUSPLUS_CPP_SPEC "-D_GNU_SOURCE %(cpp)"

#define TARGET_OS_CPP_BUILTINS()				\
    do {							\
	builtin_define_std ("__UBICOM32__");			\
	builtin_define_std ("__ubicom32__");			\
	builtin_define ("__gnu_linux__");			\
	builtin_define_std ("linux");				\
	builtin_define_std ("unix");				\
	builtin_assert ("system=linux");			\
	builtin_assert ("system=unix");				\
	builtin_assert ("system=posix");			\
    } while (0)

#define OBJECT_FORMAT_ELF


#undef DRIVER_SELF_SPECS
#define DRIVER_SELF_SPECS \
  "%{!mno-fdpic:-mfdpic}"

#undef LINK_SPEC
#define LINK_SPEC "%{mfdpic: -m elf32ubicom32fdpic -z text } %{shared} %{pie} \
  %{static:-dn -Bstatic} \
  %{shared:-G -Bdynamic} \
  %{!shared: %{!static: \
   %{rdynamic:-export-dynamic} \
   %{!dynamic-linker:-dynamic-linker /lib/ld-uClibc.so.0}} \
   %{static}} "

/*
#define MD_UNWIND_SUPPORT "config/bfin/linux-unwind.h"
*/

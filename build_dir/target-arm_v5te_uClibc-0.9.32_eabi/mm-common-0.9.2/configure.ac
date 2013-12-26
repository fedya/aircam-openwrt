## Copyright (c) 2009  Openismus GmbH  <http://www.openismus.com/>
##
## This file is part of mm-common.
##
## mm-common is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published
## by the Free Software Foundation, either version 2 of the License,
## or (at your option) any later version.
##
## mm-common is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with mm-common.  If not, see <http://www.gnu.org/licenses/>.

AC_INIT([mm-common], [0.9.2], [https://bugzilla.gnome.org/enter_bug.cgi?product=mm-common],
        [mm-common], [http://www.gtkmm.org/])
AC_PREREQ([2.59])
AC_CONFIG_SRCDIR([util/mm-common-prepare.in])

AM_INIT_AUTOMAKE([1.9 -Wno-portability check-news dist-bzip2 no-define std-options])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES])
AM_MAINTAINER_MODE

AC_ARG_VAR([ACLOCAL_FLAGS], [aclocal flags, e.g. -I <macro dir>])
AC_ARG_VAR([CURL], [curl transfer command])
AC_ARG_VAR([WGET], [wget download command])

AC_PROG_SED
AC_CHECK_PROG([CURL], [curl], [curl])
AC_CHECK_PROG([WGET], [wget], [wget])

AS_IF([test "x$USE_MAINTAINER_MODE" != xno && test "x$CURL$WGET" = x],
      [AC_MSG_FAILURE([[Either curl or wget is required for building mm-common.]])])

AC_CONFIG_FILES([Makefile
                 doctags/mm-common-libstdc++.pc
                 doctags/mm-common-libstdc++-uninstalled.pc
                 macros/mm-common.m4
                 util/mm-common-prepare])
AC_OUTPUT

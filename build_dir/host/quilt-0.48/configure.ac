dnl Process this file with autoconf to produce a configure script.
AC_INIT([quilt],[0.48],[quilt-dev@nongnu.org])
AC_CONFIG_AUX_DIR(config)
AC_PREREQ(2.53)
AC_REVISION ($Revision: 1.84 $)

PACKAGE_RELEASE=1
AC_SUBST(PACKAGE_RELEASE)

dnl Setup for backup-files compilation
AC_HEADER_STDC
AC_CHECK_FUNCS([fchmod chmod])
AC_CHECK_FUNCS([mkstemp mktemp], break)
AC_CHECK_FUNCS([mkdir rmdir])
AC_CHECK_FUNCS([strchr strrchr])
AC_CHECK_FUNCS([strerror])
AC_C_CONST
AC_FUNC_STAT

#AC_CONFIG_LIBOBJ_DIR(lib)
#AC_REPLACE_FUNCS()

if test "$ac_compiler_gnu" = "yes"; then
  CFLAGS="$CFLAGS -Wall"
fi
EXEEXT="$ac_cv_exeext"
AC_SUBST(EXEEXT)

AC_PROG_INSTALL

AC_SYS_INTERPRETER
if test "$interpval" != yes ; then
	AC_MSG_WARN([no

bash/perl scripts may not be invoked correctly due to problems with your 
systems implementation of #! being either broken or non-existant.
])
fi

dnl Check for Bourne-Again Shell
unset BASH  # bash sets this itself!

QUILT_COMPAT_PROG_PATH(BASH, bash)

# It would be nice not to have to use backticks, but too many retarded sh
# implementations still don't support $( )
# BEWARE:  There is a distinct possibility that we are currently running under
# bash in this configure script (/bin/sh being a symlink to /bin/bash).  Even
# though the result /could/ be available to us directly as $BASH_VERSION we
# don't want to use, or trust it, incase the user is specifying a different
# bash executable.
if `$BASH -c '[[ "$BASH_VERSION" \< "2.04" ]]'` ; then
    AC_MSG_ERROR([
$PACKAGE_NAME requires at least version 2.04 of bash, you can download a current
version of bash from ftp.gnu.org
])
fi

AC_MSG_CHECKING(whether $BASH quoting works)
if test `$BASH -c "echo \"\\\$(set -- \\\$'a b'; echo \\\$#)\"" 2>/dev/null` = "1"; then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
	AC_MSG_ERROR([
You have a version of `bash' which suffers from a quoting bug.
This is a known bug of bash 3.1, which was fixed by patch bash31-011.
You can get this patch at ftp://ftp.gnu.org/gnu/bash/
])
fi

QUILT_COMPAT_PROG_PATH(CP, cp, [gcp cp])
AC_MSG_CHECKING(whether $CP -l works)
touch conftest.1
if $CP -l conftest.1 conftest.2 2>/dev/null; then
	AC_MSG_RESULT(yes)
else
	AC_MSG_ERROR([no

You appear to have a `cp' that does not support hard links.
You can download GNU fileutils from ftp.gnu.org
])
fi

QUILT_COMPAT_PROG_PATH(DATE, date, [gdate date])

if test -z "$INTERNAL_DATE"; then
  AC_MSG_CHECKING([whether $DATE --rfc-822 works])
  if $DATE --rfc-822 >/dev/null 2>/dev/null; then
	AC_MSG_RESULT(yes)
  else
	AC_MSG_ERROR([no

If you don't have a version of `date' that supports --rfc-822, you
can specify '--without-date' and $PACKAGE_NAME will use its own
internal date.
])
  fi
fi

QUILT_COMPAT_PROG_PATH(PERL, perl, [perl perl5])
QUILT_COMPAT_PROG_PATH(GREP, grep)

AC_MSG_CHECKING([whether $GREP -q works])
if echo first | $GREP -q first 2>/dev/null; then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
	AC_MSG_ERROR([
Sorry, you have a version of grep which doesn't understand -q.
$PACKAGE_NAME needs it.  If you have access to a version of grep which does
understand -q, you can supply its path with the '--with-grep=' option.
])
fi

AC_MSG_CHECKING([whether $GREP understands (foo|bar)])
if echo first | $GREP '^\(fir\|las\)' >/dev/null 2>&1; then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
	AC_MSG_ERROR([
Sorry, you have a version of grep which doesn't understand constructs
of the form (foo|bar).  $PACKAGE_NAME needs it.  If you have access to
a version of grep which does understand such constructs, you can supply
its path with the '--with-grep=' option.
])
fi

QUILT_COMPAT_PROG_PATH(TAIL, tail)

# Solaris' /usr/bin/tail doesn't understand -n.
AC_MSG_CHECKING([whether $TAIL -n works])
if test "`(echo first; echo second) | $TAIL -n 1 2>/dev/null`" = "second"; then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
	AC_MSG_ERROR([
Sorry, you have a version of tail which doesn't understand -n.
$PACKAGE_NAME needs it.  If you have access to a version of tail which does
understand -n, you can supply its path with the '--with-tail=' option.
Solaris users can use /usr/xpg4/bin/tail.
])
fi

QUILT_COMPAT_PROG_PATH(TR, tr)

AC_MSG_CHECKING([whether $TR understands a-z ])
if test "`echo first | $TR a-z A-Z 2>/dev/null`" = "FIRST"; then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
	AC_MSG_ERROR([
Sorry, you have a version of tr which doesn't understand constructs
of the form a-z.  $PACKAGE_NAME needs it.  If you have access to
a version of tr which does understand such constructs, you can supply
its path with the '--with-tr=' option.
Solaris users can use /usr/xpg4/bin/tr.
])
fi

QUILT_COMPAT_PROG_PATH(SED, sed)

AC_MSG_CHECKING([whether $SED understands (foo|bar)])
if test "`echo first | $SED -e 's/\(fir\|lo\)/la/' 2>/dev/null`" = "last"; then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
	AC_MSG_ERROR([
Sorry, you have a version of sed which doesn't understand constructs
of the form (foo|bar).  $PACKAGE_NAME needs it.  If you have access to
a version of sed which does understand such constructs, you can supply
its path with the '--with-sed=' option.
])
fi

QUILT_COMPAT_PROG_PATH(AWK, awk, [gawk awk])

AC_MSG_CHECKING([whether $AWK supports sub])
if test "`echo first | $AWK 'sub(/first/, "last")' 2>/dev/null`" = "last"; then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
	AC_MSG_ERROR([
Sorry, you have a version of awk which doesn't understand sub( ).
$PACKAGE_NAME needs it.  If you have access to a version of awk
which does understand such constructs, you can supply its path
with the '--with-awk=' option.
Solaris users can use /usr/xpg4/bin/awk.
])
fi

AC_ARG_WITH(pod2man, AC_HELP_STRING(
  [--with-pod2man],
  [name of the pod2man executable to use (use --without-pod2man to disable)]),
  [
    if test x"$withval" = xno; then
      POD2MAN=
    else
      POD2MAN=$withval
    fi],[
    AC_PATH_PROG(POD2MAN, pod2man)
  ]
  )
AC_SUBST(POD2MAN)

QUILT_COMPAT_PROG_PATH(COLUMN, column)
QUILT_COMPAT_PROG_PATH(GETOPT, getopt)

if test -z "$INTERNAL_GETOPT"; then
  AC_MSG_CHECKING(for getopt --long syntax)
  dnl check GNU syntax
  $GETOPT -o t --long test -- --test | grep 'illegal option' >/dev/null
  getopt_long_errors=$?
  $GETOPT -o t --long test -- --test | grep '^ *--test *--' >/dev/null
  getopt_long_works=$?
  if test $getopt_long_errors -eq 1 -a $getopt_long_works -eq 0; then
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
    AC_MSG_ERROR([
$GETOPT does not support the --long option.
If you don't have a version of getopt that supports long options, you
can specify '--without-getopt' and $PACKAGE_NAME will use its own
internal getopt.
])
  fi
fi

QUILT_COMPAT_PROG_PATH(MKTEMP, mktemp)
if test -z "$INTERNAL_MKTEMP" ; then
    AC_MSG_CHECKING(whether $MKTEMP -d works)
    if tempdir=`$MKTEMP -d /tmp/$PACKAGE_NAME.XXXXXX 2>/dev/null` && \
       rmdir "$tempdir" ; then
	AC_MSG_RESULT(yes)
    else
	AC_MSG_RESULT(no)
	AC_MSG_ERROR([
'$MKTEMP -d' does not create temporary directories.
If you don't have a version of mktemp that can create directories, you
can specify '--without-mktemp' and $PACKAGE_NAME will use its own
internal tempfile generation mechanism.
])
    fi
fi

QUILT_COMPAT_PROG_PATH(DIFF, diff)
QUILT_COMPAT_PROG_PATH(PATCH, patch)

# Sun diff and others will not work because GNU patch options are used.
AC_MSG_CHECKING([the version of $DIFF])
if $DIFF --version 2>/dev/null | grep GNU >/dev/null; then
  set -- `$DIFF --version 2>/dev/null | $AWK '{ print $NF; exit }'`
  diff_version=$1
  AC_MSG_RESULT($diff_version)
  saved_IFS=$IFS; IFS='.'
  set -- $diff_version
  IFS=$saved_IFS
  set -- `echo $1 | $TR -cd 0-9` `echo $2 | $TR -cd 0-9`
  if test 0$1 -lt 2 || test 0$1 -eq 2 -a 0$2 -lt 7 ; then
    diff_version=
  fi
else
  AC_MSG_RESULT(no GNU diff)
fi
if test -z "$diff_version" ; then
  AC_MSG_ERROR([
$PACKAGE_NAME requires at least version 2.7 of GNU diffutils. You can
download a current version of patch from ftp.gnu.org, or if you already
have GNU diff then you can supply its path with the '--with-diff=' option.
])
fi


# Sun's patch, and others, do not work because GNU patch options are used.
AC_MSG_CHECKING([the version of $PATCH])
if $PATCH --version 2> /dev/null | grep GNU >/dev/null; then
  set -- `$PATCH --version 2> /dev/null`
  if test x$1 = xGNU ; then
    patch_version=$3
  else
    patch_version=$2
  fi
  AC_MSG_RESULT($patch_version)
  saved_IFS=$IFS; IFS='.'
  set -- $patch_version
  IFS=$saved_IFS
  set -- `echo $1 | $TR -cd 0-9` `echo $2 | $TR -cd 0-9`
  if test 0$1 -lt 2 || test 0$1 -eq 2 -a 0$2 -lt 4 ; then
    patch_version=
  fi
else
  AC_MSG_RESULT(no GNU patch)
fi
if test -z "$patch_version" ; then
  AC_MSG_ERROR([
$PACKAGE_NAME requires at least version 2.4 of GNU patch. You can download a
current version of patch from ftp.gnu.org, or if you already have GNU patch
then you can supply its path with the '--with-patch=' option.
])
fi

QUILT_COMPAT_PROG_PATH(FIND, find)

AC_MSG_CHECKING([whether $FIND -path works])
if $FIND . -path '*' >/dev/null 2>&1; then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
	AC_MSG_ERROR([
Sorry, you have a version of find which doesn't understand -path.
$PACKAGE_NAME needs it.  If you have access to a version of find which
does understand -path, you can supply its path with the
'--with-find=' option.
])
fi

QUILT_COMPAT_PROG_PATH_OPT(DIFFSTAT, diffstat)

if test "$DIFFSTAT" != "diffstat"; then
    # We need diffstat version 1.32 or better, else quilt refresh --diffstat
    # will show progress data we don't want to see.  This is only a warning
    # and we continue even if version is older, as this is only a minor
    # annoyance.
    AC_MSG_CHECKING([for diffstat version])
    diffstat_version=`$DIFFSTAT -V 2>/dev/null | \
        sed 's/^[[^0-9]]*\([[0-9]]*\)\.\([[0-9]]*\).*/\1 \2/'`
    eval set -- "$diffstat_version"
    diffstat_major_version="$1"
    diffstat_minor_version="$2"
    if test -z "$diffstat_major_version" -o -z "$diffstat_minor_version"; then
	AC_MSG_RESULT(unknown)
	AC_MSG_WARN([
diffstat utility version couldn't be checked; chances are good that the
--diffstat option of the refresh command will not work properly.
])
    else
	AC_MSG_RESULT($diffstat_major_version.$diffstat_minor_version)
	if test "$diffstat_major_version" -lt 1 \
	   -o \( "$diffstat_major_version" -eq 1 -a "$diffstat_minor_version" -lt 32 \); then
	    AC_MSG_WARN([
diffstat utility is too old; the --diffstat option of the refresh command
will not work correctly until a newer version (>= 1.32) is installed.
])
	fi
    fi
fi

QUILT_COMPAT_PROG_PATH(SENDMAIL, sendmail, [], [/usr/sbin])

AC_PATH_PROG(MSGMERGE, [msgmerge])
AC_PATH_PROG(MSGFMT, [msgfmt])
AC_PATH_PROG(XGETTEXT, [xgettext])
AC_PATH_PROG(MSGUNIQ, [msguniq])
AC_PATH_PROG(MSGCAT, [msgcat])
if test -n "$MSGFMT" -a -n "$MSGMERGE" -a -n "$XGETTEXT" -a -n "$MSGUNIQ" -a -n "$MSGCAT" ; then
  HAVE_NLS=yes
else
  HAVE_NLS=no
fi

USE_NLS=no
AC_ARG_ENABLE(nls, AC_HELP_STRING(
    [--disable-nls], [exclude natural language support]),
    [USE_NLS=$enableval],[USE_NLS=$HAVE_NLS])

if test $USE_NLS = yes -a $HAVE_NLS = no ; then
  AC_MSG_ERROR([
You do not appear to have msgfmt, which is part of the GNU Gettext package.  It
is a required package as you chose the '--enable-nls' option to configure.
You can download GNU Gettext from ftp.gnu.org
])
fi

AC_ARG_WITH(patch-wrapper, AC_HELP_STRING(
    [--with-patch-wrapper], [include GNU patch wrapper]))
PATCH_WRAPPER=$withval
AC_SUBST(PATCH_WRAPPER)

if test $USE_NLS = no ; then
    AC_MSG_NOTICE([Building without natural language support])
fi
AC_SUBST(USE_NLS)

dnl Determine where package documentation is supposed to go
if test -n "$RPM_DOC_DIR" ; then
    docdir="$RPM_DOC_DIR"
else
    docdir='$(datadir)/doc'
fi
AC_SUBST(docdir)

dnl Check for rpmbuild (v4) vs. rpm (v3)
QUILT_COMPAT_PROG_PATH_OPT(RPMBUILD, rpmbuild, [rpmbuild rpm])

AC_SUBST(COMPAT_SYMLINKS)
AC_SUBST(COMPAT_PROGRAMS)

AC_CONFIG_FILES(Makefile)
AC_OUTPUT

dnl Print results
AC_MSG_RESULT([])
AC_MSG_RESULT([$PACKAGE_NAME version $PACKAGE_VERSION configured.])
AC_MSG_RESULT([])
AC_MSG_RESULT([Using '$prefix' for installation prefix.])

# we don't need to see this just for the backup-files command
# but we may as well spec it for the future
#AC_MSG_RESULT([Using '$CC' for C compiler.])
#AC_MSG_RESULT([Building with '$CFLAGS' for C compiler flags.])
#AC_MSG_RESULT([Building with '$LIBS' for linker flags.])

AC_MSG_RESULT([])
AC_MSG_RESULT([Report bugs to $PACKAGE_BUGREPORT])

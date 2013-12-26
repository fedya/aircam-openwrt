#
# spec file for quilt - patch management scripts
#

Name:		quilt
Summary:	Scripts for working with series of patches
License:	GPL
Group:		Productivity/Text/Utilities
Version:	0.48
Release:	1
Requires:	coreutils diffutils patch gzip bzip2 perl mktemp gettext
Autoreqprov:	off
Source:		quilt-%{version}.tar.gz
BuildRoot:	%{_tmppath}/%{name}-%{version}-build

%description
The scripts allow to manage a series of patches by keeping
track of the changes each patch makes. Patches can be
applied, un-applied, refreshed, etc.

The scripts are heavily based on Andrew Morton's patch scripts
found at http://www.zip.com.au/~akpm/linux/patches/.

Authors:
--------
    Andrew Morton <akpm@digeo.com>
    Andreas Gruenbacher <agruen@suse.de>

%prep
%setup

%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --mandir=%_mandir
make prefix=/usr BUILD_ROOT=$RPM_BUILD_ROOT RELEASE=%release

%install
rm -rf $RPM_BUILD_ROOT
make install prefix=/usr BUILD_ROOT=$RPM_BUILD_ROOT
%{find_lang} %{name}

%clean
rm -rf $RPM_BUILD_ROOT

%files -f %{name}.lang
%defattr(-, root, root)
/usr/bin/guards
/usr/bin/quilt
/usr/share/quilt/
/usr/share/emacs/
/usr/lib/quilt/
/etc/bash_completion.d/quilt
%config(noreplace) /etc/quilt.quiltrc
%doc %{_mandir}/man1/guards.1*
%doc %{_mandir}/man1/quilt.1*
%doc %{_docdir}/%{name}-%{version}/README
%doc %{_docdir}/%{name}-%{version}/README.MAIL
%doc %{_docdir}/%{name}-%{version}/quilt.pdf

%changelog
* Sat Nov 29 2008 - agruen@suse.de
- quilt setup: Define %__tar and %__patch to refer to the tar and
  patch wrapper. (Patch from Jan Kratochvil, Savannah bug 24964).
* Sun Nov 02 2008 - agruen@suse.de
- To allow using 'procmail' (which contains 'formail') as weak
  dependency in package managers, explicitely check for the
  'formail' binary when invoking 'quilt mail' so that the user
  gets a clear error message when 'formail' is missing and knows
  what to do. (From Bernhard Walle <bwalle@suse.de>.)
* Sun Nov 02 2008 - agruen@suse.de
- push command: Remove leftovers of the former interactive-option.
  (From Wolfram Sang <w.sang@pengutronix.de>.)
* Sun Nov 02 2008 - agruen@suse.de
- mail command: don't word boundaries in awk; it's a GNU awk
  extension. (From Wolfram Sang <w.sang@pengutronix.de>.)
* Sun Nov 02 2008 - agruen@suse.de
- From Wolfram Sang <w.sang@pengutronix.de>:
  "The test for import did not always recreate a patches-dir
   inside the temporary location, so it would fail if you
   modified quilt using quilt."
* Sun Nov 02 2008 - agruen@suse.de
- Fix from Dean Roehrich <Dean.Roehrich@sun.com> for Solaris:
  "I ran into a case solaris ON build 99 where configure found
   ginstall, but the Makefile.in used something else, and that
   led to things going a bit haywire."
* Sun Nov 02 2008 - agruen@suse.de
- Add a dual BSD/GPL license header to test/run.
- Only run test/patch-wrapper.test if the --with-patch-wrapper
  config option is used.
* Wed Sep 03 2008 - agruen@suse.de
- Bash completion fix for fold command from
  Bernhard Walle <bwalle@suse.de>.
* Thu Aug 21 2008 - agruen@suse.de
- Bump version to 0.47.
- Add a new "publish" makefile target for pushing releases to
  savannah.nongnu.org.
* Tue Jun 24 2008 - agruen@suse.de
- quilt/scripts/patchfns.in (cat_series): Handle lines properly
  that only contain whitespace.
* Fri Jun 13 2008 - agruen@suse.de
- quilt/import.in: When importing multiple patches at once, make
  sure that they won't end up in the series file in reverse order.
  This happened because each new patch was inserted before the
  "next" patch, which made it the new "next" patch.
* Tue May 20 2008 - agruen@suse.de
- Stop using cp -l: it doesn't fall back to doing a regular copy
  when hardlinks are not supported; on some types of filesystems
  like AFS and in some situtions, this is annoying.
* Tue Mar 25 2008 - agruen@suse.de
- Assume patches are in unified format by default when applying
  them. (For working with other patch formats, make sure to remove
  "--unified" from QUILT_PATCH_OPTS in ~/.quiltrc or
  /etc/quilt.quiltrc.)
* Mon Nov 05 2007 - agruen@suse.de
- quilt/mail.in: Remove the Recipient-* and Replace-* headers
  used internally from the resulting emails; not sure how that
  was missed so far.
* Mon Oct 29 2007 - agruen@suse.de
- Fix `make snapshot'.
- Make the changelog file chronological again.
* Mon Oct 29 2007 - agruen@suse.de
- quilt/mail.in: Ran into a bash ``Broken pipe'' problem in
  quilt_mail_patch_filter() which I cannot explain or cleanly
  reproduce. Work around it by not using pipes and subshells as
  extensively here.
* Thu Oct 25 2007 - agruen@suse.de
- quilt/mail.in: Make sure that shell meta-characters like * in
  subjects won't get expanded by accident (this was broken). Add
  test case.
- quilt/mail.in: ``formail -x Foo'' will extract all Foo* headers,
  so use ``formail -x Foo:'' instead.
- quilt/mail.in: Recognize sequences of common subject prefixes
  like Fwd:, Fw:, Re:, Aw:, Tr:, [foo] only at the beginning of
  subjects.
* Mon Oct 15 2007 - agruen@suse.de
- quilt/mail.in: Add a --signature flag that allows to override
  the signature used. (The test suite still depended on the
  .signature file of the user runnig it; fixed now.)
* Sat Oct 06 2007 - agruen@suse.de
- Allow to define a MUA different from the one found at configure
  time in .quiltrc (by setting QUILT_SENDMAIL).
* Sat Sep 08 2007 - agruen@suse.de
- quite refresh: Avoid a possible environment overflow with very,
  very large patches (#19748).
* Fri Aug 10 2007 - agruen@suse.de
- quilt mail: only add a "-- " at the end of messages if it is
  actually followed by a signature.
* Fri Aug 10 2007 - agruen@suse.de
- quilt mail: Add a --reply-to <message> option which adds the
  appropriate Subject, Reply-To, and References headers to the
  intro and patch messages.
* Wed Aug 08 2007 - agruen@suse.de
- quilt setup: replace the --path argument with --sourcedir for
  specifying the location of the package sources. Pass the source
  dir to inspect as well. Fix a test suite bug for quilt setup.
  Minor cosmetic changes.
- Fix a quilt mail test suite bug: specify a dummy --sender in
  test.quiltrc.
* Thu Aug 02 2007 - paul@pwsan.com
- quilt mail: strip closing angle brackets from the mail sender
  address before it is inserted into the message ID. (Changed
  by agruen@suse.de.)
* Thu Jul 26 2007 - agruen@suse.de
- quilt diff: don't colorize the output when the --diff=... option
  is used (allows diff utilities to use terminal IO).
* Tue Jun 05 2007 - agruen@suse.de
- Man page improvements from Ben Pfaff <blp@cs.stanford.edu>.
- QUILT_SERIES and series file search algorithm bugfix.
* Sun May 06 2007 - agruen@suse.de
- Mail command: when only a single patch is specified, only mail
  that one. Only mail all patches to the end of the series if the
  last patch is '-', or no patches have been specified.
- Revert command: change semantics to really revert the changes
  relative to the patch instead of removing the file from the
  patch. That's much more useful; the other behavior is available
  as ``quilt diff <file> | patch -p1'', anyway.
- Update German translation.
* Thu May 03 2007 - agruen@suse.de
- fold command: Add -R option.
- Update German translation.
* Fri Apr 27 2007 - agruen@suse.de
- Try to clean up the force-apply and interactive-apply logic
  in quilt push, and add a force option to quilt fold: Fold was
  applying patches interactively and was restoring files if there
  were rejects; this was very unexpected for users.
* Fri Apr 27 2007 - agruen@suse.de
- Rename the ``remove'' command to ``revert'': this removes
  ambiguities with the delete command, and more closely matches
  what the command does.
* Sun Apr 22 2007 - khali@linux-fr.org
- quilt mail: Document the possibility to specifiy a patch range.
* Sun Apr 22 2007 - khali@linux-fr.org
- Dependencies of quilt/* on quilt/scripts/* are run-time
  dependencies for the test suite, not build-time dependencies.
* Thu Apr 19 2007 - agruen@suse.de
- quilt mail: Allow to specify a patch range.
- Remove fuzzy German translations.
* Thu Apr 19 2007 - agruen@suse.de
- Make pod2man optional (./configure --without-pod2man).
* Wed Apr 18 2007 - agruen@suse.de
- Install quilt.el into $(datadir)/emacs/site-lisp/.
* Mon Apr 16 2007 - agruen@suse.de
- lib/backup-files.c: Do not modify the original file whenever
  possible: previously, a ``quilt add'' usually linked the file
  to the backup and then copied the backup over the file to ensure
  a link count of one. Recognize this case, and create a copy in
  the first place instead.
* Sun Apr 15 2007 - agruen@suse.de
- Add quilt.el and its README file. (The emacs mode is not being
  installed properly, yet.)
* Sun Apr 15 2007 - khali@linux-fr.org
- quilt/refresh.in: Remove stray 
 in error message. Reported by
  Randy Dunlap.
* Mon Mar 26 2007 - agruen@suse.de
- Quilt refresh: fix existing patch check.
* Thu Feb 22 2007 - khali@linux-fr.org
- quilt/scripts/patchfns.in: Fix an off-by-one error in
  change_db_strip_level when inserting the strip level in the
  patch argument list. Also cleanup this function a bit.
* Wed Feb 21 2007 - khali@linux-fr.org
- quilt/import.in, quilt/scripts/patchfns.in: Implement import -R
  for reverse-applying a patch. Original patch contributed by
  Randy Dunlap.
* Tue Feb 20 2007 - khali@linux-fr.org
- test/run: Don't attempt to remove comments from command lines.
  We don't put such comments in our test cases, and it prevents
  legitimate uses of the "#" character (e.g. in a string, inside
  quotes) in commands.
* Tue Feb 13 2007 - khali@linux-fr.org
- Update the Japanese translation. Patch from Yasushi Shoji.
* Wed Jan 31 2007 - agruen@suse.de
- Update the German translation.
* Thu Oct 19 2006 - agruen@suse.de
- Refresh command: add -z option, which refreshes changes into a
  new patch instead of modifying the existing patch.
* Thu Oct 19 2006 - khali@linux-fr.org
- Bump version to 0.46.
* Tue Oct 10 2006 - khali@linux-fr.org
- aclocal.m4: New helper for optional binaries.
- configure.ac: Use the new helper for diffstat and rpmbuild.
- compat/diffstat.in: Delete.
- compat/rpmbuild.in: Delete.
* Mon Oct 09 2006 - agruen@suse.de
- Update the German translation.
* Mon Oct 09 2006 - agruen@suse.de
- quilt.spec.in: Pass %_mandir to --configure (fix from Anders
  Larsen).
* Fri Sep 22 2006 - khali@linux-fr.org
- quilt/delete.in: Display the proper error message when using
  "quilt delete" with no argument and no patch is applied.
- test/delete.test: Add a test for this specific case.
* Wed Sep 20 2006 - khali@linux-fr.org
- po/ja.po: Update the Japanese translation, patch from
  Yasushi Shoji.
- po/fr.po: Update the French translation.
* Tue Sep 19 2006 - khali@linux-fr.org
- quilt/diff.in, quilt/refresh.in: Update usage line to mention
  "-p ab".
* Tue Sep 19 2006 - khali@linux-fr.org
- quilt/scripts/patchfns.in: Drop support for "-pab" in series
  file, for it breaks compatibility with previous versions of
  quilt, and is inconsistent.
- test/dir-a-b.test: Drop test case for "-pab" in series file.
* Tue Sep 19 2006 - khali@linux-fr.org
- test/*.test: Make sure that rm commands are non-interactive.
* Tue Sep 19 2006 - khali@linux-fr.org
- test/dir-a-b.test: Don't use diffstat in the regression test
  suite, as it may or may not be available.
* Mon Sep 18 2006 - khali@linux-fr.org
- doc/quilt.1.in: Document the fact that per-patch comments are now
  supported. Fix a typo.
* Mon Sep 18 2006 - khali@linux-fr.org
- Makefile.in: Set LC_ALL=C before running the regression tests,
  setting LANG alone isn't sufficient because LC_ALL overrides LANG
  when set.
* Tue Sep 05 2006 - khali@linux-fr.org
- quilt/mail.in, test/mail.test: Fix signature separator. Patch
  from Ville Syrjala.
* Tue Jun 27 2006 - agruen@suse.de
- A few more `-p ab' fixes.
* Tue Jun 27 2006 - agruen@suse.de
- quilt/push.in: Fix a bug introduced with the -p ab support.
- test/dir-a-b.test: Add a regression test case for this fix.
* Tue Jun 27 2006 - agruen@suse.de
- Remove some broken translations that slipped in on Jun 12.
* Sat Jun 24 2006 - khali@linux-fr.org
- configure.ac: Test for bash 3.1 quoting bug (#16763).
* Sat Jun 24 2006 - khali@linux-fr.org
- bin/quilt.in, doc/quilt.1.in: Document the --quiltrc=- feature
  (#16727).
* Mon Jun 12 2006 - agruen@suse.de
- quilt/diff.in, quilt/refresh.in: Add `-p ab' options which
  produce a/file and b/file filenams in headers instead of
  dir.orig/file and dir/file, similar to BK, git, hg patches.
- Only touch .pc/$patch/.timestamp when a path actually is being
  refreshed.
* Fri Jun 02 2006 - khali@linux-fr.org
- doc/quilt.1.in: Mention the QUILT_PATCHES_PREFIX,
  QUILT_NO_DIFF_INDEX and QUILT_NO_DIFF_TIMESTAMP variables, and
  describe their effects.
* Thu Jun 01 2006 - khali@linux-fr.org
- quilt/import.in: Add some missing quotes. This makes it possible
  to import a patch with unfriendly characters (say spaces) in its
  name as long as it is renamed to something more friendly in the
  process.
* Thu Jun 01 2006 - agruen@suse.de
- compat/rpmbuild.in: Add a version that tries to execute a
  rpmbuild that may have been installed after building quilt.
- compat/diffstat.in: Same as for compat/rpmbuild.in, try to
  execute diffstat even though none was there at quilt build
  time.
- aclocal.m4: Fix a quoting error.
* Wed May 31 2006 - agruen@suse.de
- Fix the --with-rpmbuild option of configure.
* Fri May 26 2006 - agruen@suse.de
- quilt/scripts/patchfns.in: Fix bugs in basename() and dirname().
* Thu May 25 2006 - khali@linux-fr.org
- quilt/edit.in, quilt/header.in, quilt/mail.in: Run $EDITOR with
  LANG=$ORIGINAL_LANG. Fallback to $ORIGINAL_LANG rather than
  $LANG for charset detection. Patch from Thomas Petazzoni.
* Tue May 23 2006 - khali@linux-fr.org
- quilt/scripts/edmail.in: Declare option parameters as mandatory
  rather than optional. This makes more sense as the parameters
  are indeed not optional, and this also works around a bug in
  Getopt::Long.
* Wed Apr 26 2006 - khali@linux-fr.org
- quilt/pop.in: Delete .pc/$patch directory when popping empty
  patches. Report and original patch by Arnaud Patard.
- quilt/push.in: Create .pc/$patch directory when pushing missing
  or empty patches. If we don't, some commands such as rename can
  fail later.
- test/missing.test: Test rename command on missing patches.
- test/empty.test: New test case similar to missing.test, but for
  empty patches. Somewhat based on a contribution by Arnaud
  Patard.
* Mon Apr 24 2006 - agruen@suse.de
- Add @datarootdir@ macro which future versions of Autoconf will
  start to use in @datadir@ (bug 16155, patch from Ralf
  Wildenhues).
* Mon Apr 24 2006 - khali@linux-fr.org
- Bump version to 0.45.
* Wed Apr 19 2006 - khali@linux-fr.org
- po/*.po: Delete obsolete translations.
* Wed Apr 19 2006 - khali@linux-fr.org
- compat/getopt.in: Ignore option -q, rather than failing on it.
  This prevents the test suite from failing on patch-wrapper
  when --without-getopt is used. Reported by Markus Oberhumer.
* Sat Apr 15 2006 - khali@linux-fr.org
- compat/getopt.in: Remove duplicate test.
* Sat Apr 15 2006 - khali@linux-fr.org
- quilt/scripts/patchfns.in: Fix broken return in function
  find_applied_patch, which was causing a bogus error message
  on failed pop.
- test/three.test: Test failed pop.
* Sun Mar 26 2006 - agruen@suse.de
- test/mail.in: Test UTF-8 header encoding.
* Sun Mar 26 2006 - khali@linux-fr.org
- test/mail.in: Test for recipients with a dot in their name.
* Sat Mar 25 2006 - agruen@suse.de
- quilt/mail.in: Use -gt instead of > in [ ... ] expression.
- quilt/scripts/edmail.in: Improve handling of quoting and
  special characters in recipient addresses: Automatically quote
  display names that contain quotable characters, and encode
  special and eight-bit characters according to RFC 2047
  (e.g., =?utf-8?q?Andreas=20Gr=C3=BCnbacher?=).
* Sat Mar 25 2006 - khali@linux-fr.org
- Makefile.in: configure depends on aclocal.m4 too.
- configure.ac: Cleanup two QUILT_COMPAT_PROG_PATH calls, the
  second parameter is not supposed to be a list.
- aclocal.m4: Document the fourth (optional) parameter of
  QUILT_COMPAT_PROG_PATH.
- aclocal.m4: No need to check for binaries we were told not to
  use.
* Tue Mar 21 2006 - agruen@suse.de
- quilt/edit.in: The last fix broke another way of using edit.
* Mon Mar 20 2006 - khali@linux-fr.org
- configure.ac: Fix a typo in a test expression which was breaking
  configure on FreeBSD. Patch contributed by Dirk Jagdmann.
* Mon Mar 20 2006 - khali@linux-fr.org
- Makefile.in: Fix building with GNU make < 3.80.
* Wed Mar 15 2006 - agruen@suse.de
- quilt/edit.in: Fix for working in subdirectories.
* Sun Mar 12 2006 - khali@linux-fr.org
- Makefile.in: Fix pseudo-symlinks under compat not being removed
  by "make clean".
* Sun Mar 05 2006 - khali@linux-fr.org
- quilt/import.in: Fix replacement of compressed patches.
- test/import2.test: New test case for compressed patches.
* Thu Feb 16 2006 - agruen@suse.de
- quilt/scripts/inspect.in: Also recognize when tar is passed a
  filename on the command line -- in this case, md5sum will add
  a `*' in front of the filename, which indicates a binary file.
* Thu Feb 16 2006 - agruen@suse.de
- quilt/scripts/inspect.in: Add default value for QUILT_DIR so
  that inspect can be used stand-alone as well.
* Wed Feb 15 2006 - agruen@suse.de
- quilt.spec.in: Use wildcards for matching manpage{,.gz,bz2}
  (Thomas Petazzoni <thomas.petazzoni@enix.org>).
* Tue Feb 14 2006 - agruen@suse.de
- Stop using bash's =~ operator: older versions don't support it.
- Bump version to 0.44.
* Tue Feb 14 2006 - khali@linux-fr.org
- po/fr.po: French translation update.
* Tue Feb 14 2006 - khali@linux-fr.org
- quilt/mail.in: Fix temporary file leak.
* Tue Feb 14 2006 - agruen@suse.de
- po/ja.po: Translation update from Yasushi.
* Tue Feb 14 2006 - agruen@suse.de
- Fix a critical bug in the push command that causes parts of
  patches to end up half-reversed (introduced on 22 Jan).
* Sat Feb 11 2006 - agruen@suse.de
- lib/backup-files.c: Close files before rename; Windows won't
  rename open files.
* Sat Feb 11 2006 - agruen@suse.de
- test/run: Avoid warnings with some versions of Perl.
* Sat Feb 11 2006 - agruen@suse.de
- Two bash 3.1 compatibility fixes from Arnaud Patard
  <apatard@mandriva.com> -- thank you.
* Sat Feb 11 2006 - agruen@suse.de
- Some versions of mkdir screw up on `mkdir -p dir/.' when dir
  does not already exist.
* Sat Feb 11 2006 - agruen@suse.de
- bin/patch-wrapper.in: Call GNU patch by absolute patch to avoid
  recursion. Don't print an error message when a patch contains
  no files.
* Sat Feb 11 2006 - agruen@suse.de
- quilt/setup.in: Add --path option for defining a tarball search
  path. Fix a bug in check_for_existing_files. When a series file
  is given as an argument, symlink to that series file in the
  same way as we symlink to the patches. Improve the symlink
  creation algorithm. Add testcase test/setup.test.
* Sat Feb 11 2006 - agruen@suse.de
- quilt/new.in: Try to explain how to choose another source tree
  root directory when quilt chooses the wrong one.
* Sat Feb 11 2006 - agruen@suse.de
- quilt/scripts/patchfns.in: Fix bug in quilt_command when working
  in subdirectories.
- Add testcase test/edit.test to catch this kind of bug.
* Wed Feb 08 2006 - agruen@suse.de
- bin/patch-wrapper.in: Fix permissions of installed file.
* Tue Feb 07 2006 - agruen@suse.de
- quilt/scripts/inspect.in: Improve recognition of patch's -i and
  --input options.
* Tue Feb 07 2006 - agruen@suse.de
- po/ja.po: Fix three errors: % used instead of %s (Yasushi Shoji).
* Mon Feb 06 2006 - agruen@suse.de
- Add --with-patch-wrapper configure argument, and optionally
  install the wrapper as /usr/share/quilt/wrapper/patch.
- bin/patch-wrapper.in: Minor cleanups.
* Mon Feb 06 2006 - agruen@suse.de
- bin/patch-wrapper.in: Add hack to follow pipes via /proc on
  Linux (zcat patch.diff.gz | patch -p0, etc.).
* Mon Feb 06 2006 - agruen@suse.de
- bin/patch-wrapper.in: A wrapper to GNU patch that recognizes the
  most common options and mimics GNU patch's behavior and output,
  and creates the quilt metadata as if quilt push was used to
  apply the patch.  When options are used that this wrapper does
  not recognize, GNU patch is used directly, and no quilt metadata
  will get created.
- lib/backup-files.c: Don't fail if a directory specified with
  -B does not exist.
* Sat Feb 04 2006 - agruen@suse.de
- make dist: Add the missing aclocal.m4 to the tarball.
* Fri Feb 03 2006 - khali@linux-fr.org
- bash_completion: Code refactoring, indentation fixes.
* Fri Feb 03 2006 - khali@linux-fr.org
- quilt/mail.in: Strip "Tr:" from subject lines like we do with
  "Re:" and "Fwd:".
* Fri Feb 03 2006 - agruen@suse.de
- quilt/mail.in: Disable broken patch header format check.
* Fri Feb 03 2006 - jayvdb@gmail.com
- configure.ac: Add check for GNU diff and use $GREP
  in GNU patch check.
* Wed Feb 01 2006 - agruen@suse.de
- Bump version to 0.43.
* Wed Feb 01 2006 - agruen@suse.de
- compat/date.in: Fix GetOptions problem with options starting
  with `+'.
- test/run: Adjust $ENV{PWD} in cd command; add FIXME comment in
  export command: in commands like `export foo=$var/bar', $var is
  not evaluated.
* Wed Feb 01 2006 - agruen@suse.de
- quilt/mail.in: Documentation fix.
* Wed Feb 01 2006 - agruen@suse.de
- configure.ac: Remove uniq -D test; we no longer use it.
* Wed Feb 01 2006 - jayvdb@gmail.com
- compat/date.in: Compatibility wrapper that emulates the GNU
  date features that quilt depends on (with minor cleanups from
  Andreas Gruenbacher).
- configure.ac: Add test for date --rfc-822.
* Wed Feb 01 2006 - agruen@suse.de
- bin/guards: Update to latest version (supports --with-guards
  option).
- doc/README.in: Update the description of the helper scripts.
  Add Typical Usages section from Randy Dunlap
  <rdunlap@xenotime.net>.
- contrib/import.diff: Remove obsolete file.
* Wed Feb 01 2006 - khali@linux-fr.org
- po/fr.po: Add missing translation of quilt mail's --sender
  option help. Thanks to Andreas Gruenbacher for pointing out.
* Wed Feb 01 2006 - jayvdb@gmail.com
- Filter duplicates with awk instead of using uniq -D.
* Wed Feb 01 2006 - agruen@suse.de
- Clean up quoting in messages. Add some spaces to the Japanese
  translation for good measure.
* Wed Feb 01 2006 - agruen@suse.de
- Japanese translation update from Yasushi Shoji.
* Tue Jan 31 2006 - agruen@suse.de
- Check for GNU uniq, and test if uniq -D is supported (from Gary
  V. Vaughan).
* Tue Jan 31 2006 - agruen@suse.de
- Make sure that the %changes section in the spec file will
  contain POSIX dates (#15591).
* Mon Jan 30 2006 - khali@linux-fr.org
- quilt/mail.in: Don't use sed's Q command, old versions of
  sed don't know about it.
* Mon Jan 30 2006 - agruen@suse.de
- quilt/mail.in: Use $(hostname) for the sender address for
  platforms on which $(hostname -f) doesn't work.
* Mon Jan 30 2006 - jayvdb@gmail.com
- compat/getopt.in: Properly handle spaces in arguments to getopt.
* Mon Jan 30 2006 - agruen@suse.de
- Allow backup-files to build on windows using the MinGW compiler.
* Mon Jan 30 2006 - jayvdb@gmail.com
- Makefile.in: The trailing slash in `find compat/' is causing OSX
  /usr/bin/find to print 'compat//sed', etc. As a result, they are
  appearing in compat_leftover, being removed in the middle of the
  build, causing errors during the construction of $(DOC).
* Mon Jan 30 2006 - jayvdb@gmail.com
- test/mail.test: /dev/stdin @CHANGELOG@ /dev/stdout dont exist on Tru64.
* Mon Jan 30 2006 - agruen@suse.de
- lib/backup-files.c: Tru64 does not have snprintf. (Reported by
  John Vandenberg).
* Mon Jan 30 2006 - khali@linux-fr.org
- bash_completion: Update import and mail commands options.
* Mon Jan 30 2006 - khali@linux-fr.org
- po/fr.po: Update French translation.
* Sun Jan 29 2006 - agruen@suse.de
- Also strip any bracketed fields and Re:, Fwd:, Fw:, Aw: from
  subject headers.
* Sun Jan 29 2006 - agruen@suse.de
- quilt/mail.in:
  + Add -m and --prefix options so that the mail command can be
    used without invoking an editor. 
  + Verify that each message generated will have a unique subject.
  + Verify that the introduction has a subject header.
  + Fix a bug in the heuristic for adding recipients.
  + Add a heuristic for extracting subject headers
  + Clarify some messages.
  + Add a test case.
  + Update the documentation.
* Sun Jan 29 2006 - agruen@suse.de
- Makefile.in: Add a missing dependency.
* Sun Jan 29 2006 - khali@linux-fr.org
- quilt/files.in, bash_completion: Command files only works on
  applied patches.
* Sun Jan 29 2006 - khali@linux-fr.org
- quilt/patches.in, quilt/refresh.in: Update usage line.
- bash_completion: Resync with quilt commands options.
* Sun Jan 29 2006 - khali@linux-fr.org
- quilt/push.in: Fix a temporary file leak and vulnerability.
* Sun Jan 29 2006 - khali@linux-fr.org
- quilt/scripts/patchfns.in: Drop an unused local variable in
  function find_patch_file.
- quilt/import.in: Add a missing print_patch call.
- quilt/import.in: Fix several temporary file leaks.
* Fri Jan 27 2006 - agruen@suse.de
- po/ja.po: Translation update from Yasushi.
* Fri Jan 27 2006 - agruen@suse.de
- Stop using bash <(...) process substitution: it triggers bugs
  on some platforms.
- In source tree compat/ "virtual symlink" scripts, exec the
  target binary instead of only calling it.
* Fri Jan 27 2006 - agruen@suse.de
- Unify existing -p and -n parameters that specify a patch:
  always use -P as the option name. Update test suite and the
  translations accordingly. Patch mostly from Gary V. Vaughan.
* Thu Jan 26 2006 - khali@linux-fr.org
- po/fr.po: Update French translation.
* Thu Jan 26 2006 - khali@linux-fr.org
- lib/backup-files.c: Fix usage line and help text.
- lib/backup-files.c: Improve recursive processing by avoiding
  repeated realloc and strlen calls. Hardly measurable though.
* Thu Jan 26 2006 - khali@linux-fr.org
- Makefile.in: Have Makefile depend on configure, so that the
  user runs ./configure only once if both Makefile.in and
  configure.ac have changed.
* Thu Jan 26 2006 - agruen@suse.de
- quilt/import.in: Add missing quoting, stop using diff -U for
  portability.
* Thu Jan 26 2006 - agruen@suse.de
- aclocal.m4(QUILT_COMPAT_PROG_PATH): Create missing compat
  symlinks for binaries that have a different basename than what
  quilt expects (e.g., awk/gawk). Patch from Gary V. Vaughan.
* Thu Jan 26 2006 - agruen@suse.de
- Stop using find's -printf option: some versions of find don't
  have it.
* Thu Jan 26 2006 - agruen@suse.de
- quilt/delete.in: Remove a superfluous message.
* Thu Jan 26 2006 - agruen@suse.de
- lib/backup-files.c: Stop using ftw and implement our own dir
  walking: ftw is a portability nightmare. The foreachdir function
  is based a proposal from Gary V. Vaughan <gary@gnu.org>, thanks!
* Wed Jan 25 2006 - agruen@suse.de
- quilt/import.in: Improve the help text.
- Makefile.in: Make sure configure is up to date.
* Wed Jan 25 2006 - agruen@suse.de
- lib/backup-files.c: Fix memory leaks (markus@oberhumer.com).
* Wed Jan 25 2006 - agruen@suse.de
- lib/backup-files.c: Don't fail when link(1) returns ENOSYS, and
  try to copy in that case.
* Wed Jan 25 2006 - agruen@suse.de
- lib/backup-files.c: Fix another missed optimization.
* Wed Jan 25 2006 - agruen@suse.de
- lib/backup-files.c: Cleanups by Markus F.X.J. Oberhumer
  <markus@oberhumer.com>.
- lib/backup-files.c: Several fixes from Markus as well.
* Tue Jan 24 2006 - agruen@suse.de
- quilt/scripts/parse-patch.in: files_in_patch improvement from
  jayvdb@gmail.com.
* Mon Jan 23 2006 - agruen@suse.de
- configure.ac: Use $TR instead of tr.
* Mon Jan 23 2006 - jayvdb@gmail.com
- Fix make check-all and make check-*.
* Mon Jan 23 2006 - agruen@suse.de
- configure.ac: Require GNU patch 2.4+.
- quilt/import.in: Don't use diff -U-1.
- Test suite: Recognize output of old versions of patch (from
  jayvdb@gmail.com).
* Mon Jan 23 2006 - jayvdb@gmail.com
- configure.ac: Check if awk supports sub().
- quilt/scripts/patchfns.in: Syntax changes for Solaris
  /usr/xpg4/bin/awk.
* Mon Jan 23 2006 - agruen@suse.de
- Stop using GNU awk's gensub.
* Mon Jan 23 2006 - jayvdb@gmail.com
- A better rule for test/.depend.
- In order to run make check immediately after configure, a few
  minor dependencies need to be added.
- The test suite depends on the compat layer and backup-files.
- quilt/refresh depends on remove-trailing-ws.
* Mon Jan 23 2006 - agruen@suse.de
- Remove unnecessary uses of diff's -N option.
* Sun Jan 22 2006 - agruen@suse.de
- Don't include any object files in LIBOBJS that we don't have.
* Sun Jan 22 2006 - agruen@suse.de
- Call $(CMD) instead of cmd in Makefile.in.
* Sun Jan 22 2006 - agruen@suse.de
- push command: when a patch doesn't apply, check if it reverse
  applies, and inform the user.
* Sun Jan 22 2006 - agruen@suse.de
- When inserting new diffstat, put a "---" line above it.
* Sat Jan 21 2006 - agruen@suse.de
- Add fallback ftw implementation for systems that don't have it.
* Sat Jan 21 2006 - agruen@suse.de
- Use ftw instead of nftw (some platforms don't have nftw).
* Sat Jan 21 2006 - jayvdb@gmail.com
- lib/backup-files.c: Only create and remove parents that are
  missing or empty. Report errors in nftw. (Minor changes by
  agruen@suse.de).
* Sat Jan 21 2006 - jayvdb@gmail.com
- Don't use date -r for comparing file timestamps.
* Sat Jan 21 2006 - jayvdb@gmail.com
- GNU patch 2.5 messages differ slightly from the messages of more
  recent versions. Recognize the old version's output when fixing
  patch's output in the push command, and also accept the old
  version's output in the test suite.
* Sat Jan 21 2006 - jayvdb@gmail.com
- test/run: Support regex matching in test scripts.
* Sat Jan 21 2006 - jayvdb@gmail.com
- Check whether cp -l works.
* Sat Jan 21 2006 - jayvdb@gmail.com
- Default to use NLS when NLS binaries are available (minor
  changes by agruen@suse.de).
* Sat Jan 21 2006 - agruen@suse.de
- Split off strip_diffstat from header command.
- import command: compare patch headers to prevent destruction;
  add -d option to choose which header(s) to keep.
* Sat Jan 21 2006 - jayvdb@gmail.com
- import command: catch several error conditions. Patches in
  QUILT_PATCHES can now be imported. A minor fix in the output.
* Sat Jan 21 2006 - agruen@suse.de
- Fix delete -n when no patches are applied.
* Fri Jan 20 2006 - jayvdb@gmail.com
- Tru64 /bin/sh complains when $(call ) is used without ; on each
  line of the define.
* Fri Jan 20 2006 - jayvdb@gmail.com
- Clear up the internal make rules so make -d is not so verbose.
* Fri Jan 20 2006 - agruen@suse.de
- Check if arguments to --with-<prog> exist, warn if they do not,
  and fail if they do but are no executables. (Based on a patch
  from jayvdb@gmail.com).
* Fri Jan 20 2006 - jayvdb@gmail.com
- configure.in: Document to use --without-foo instead of
  --with-foo=none.
* Thu Jan 19 2006 - agruen@suse.de
- Make sure quilt diff --snapshot visits files in a defined order.
* Thu Jan 19 2006 - jayvdb@gmail.com
- Move patch parameter checks into patchfns.in, adding quotes
  around all patch parameters, and reporting 'No patches in series'
- quilt/annonate.in: Fix case where no patches have been applied.
* Mon Jan 16 2006 - jayvdb@gmail.com
- test/three.test, test/import.test: Add various tests.
* Mon Jan 16 2006 - jayvdb@gmail.com
- Always use full command names in the test suite.
* Mon Jan 16 2006 - jayvdb@gmail.com
- doc/README and doc/quilt.1 depend on compat/getopt being built
  prior to their generation.  As the phony target 'compat' cant
  be used as a prereq, except to other phony targets, $(COMPAT)
  has been added to refer to all necessary compat/ objects.
- bin/*.in should be regenerated after a change to Makefile.
* Mon Jan 16 2006 - agruen@suse.de
- Change the rules for splitting a patch into header and body so
  that it will work for git extanded patches as well.
* Sun Jan 15 2006 - agruen@suse.de
- John Vandenberg <jayvdb@gmail.com>: Make check depends on
  programs that have been configured with --with-<name>=<prog>.
  Make dependencies would get confused by symlinks, so create
  wrappers in compat/ for make check.
- Remove binaries in compat/ that are not listed in COMPAT_PROGRAMS
  or COMPAT_SYMLINKS.
- configure.ac: use --without-$foo instead of --with-foo=none.
- Fix processing of translations again, and update de.po.
* Sat Jan 14 2006 - agruen@suse.de
- Move scripts directory to quilt/scripts to simplify running quilt
  directly from the source tree.
- test/Makefile: merge into Makefile.in (target check).
- Makefile.in: clean up.
* Sat Jan 14 2006 - agruen@suse.de
- Use the quilt version in the source tree instead of the
  installed version for running the test suite.
* Tue Jan 10 2006 - khali@linux-fr.org
- test/null-bug.test: Fix the original directory creation so that
  the test doesn't fail if any higher-level directory contains a
  patches/ sub-directory.
- test/*.test: Fix the test suite after the December 8th, 2005
  change to quilt diff. Contributed by Markus Oberhumer.
* Wed Jan 04 2006 - khali@linux-fr.org
- po/quilt.pot, po/de.po: Resync with code.
- po/ja.po: Update from Yasushi Shoji.
- po/fr.po: Update from me.
* Thu Dec 08 2005 - agruen@suse.de
- scripts/inspect.in (setup command): add heuristic to check for
  input files specified on the command line in the tar/patch
  wrapper.
- quilt/diff.in: Degrade the ``More recent patches modify files
  in patch'' message to a warning.
* Thu Dec 01 2005 - mquinson@debian.org
- bin/guards.in: s/ocnfiguration/configuration/ 
  (fixes Bug#341518 in Debian)
- doc/quilt.1.in: Document common options to all commands
  (fixes Bug#334408 in Debian)
* Mon Nov 28 2005 - agruen@suse.de
- quilt/diff.in, quilt/refresh.in: for -p0 style patches, when a
  file is being deleted, use the real filename in the +++ line
  instead of filename.orig: if we don't, the patch does not
  contain the real filename anymore, and patch will fail. See
  test/null-bug.test.
- files command: remove a redundant find_patch().
- Make top_patch() fail if there is no top patch.
* Fri Nov 04 2005 - agruen@suse.de
- Makefile.in: don't strip the backup-files binary in the install
  target.
- quilt.spec.in: set the CFLAGS to $RPM_OPT_FLAGS, and pass on
  the RPM release number when building quilt.
* Wed Oct 12 2005 - agruen@suse.de
- scripts/inspect.in: remove unnecessary path_search() function
  and let the shell search the path internally.
* Tue Oct 04 2005 - khali@linux-fr.org
- scripts/patchfns.in: Prevent patch_header and patch_body from
  mistakenly considering "---" alone on its header line as the
  beginning of the patch body.
* Fri Sep 23 2005 - khali@linux-fr.org
- configure.ac: Let the user select the find binary. Test that find
  supports -path.
* Fri Sep 23 2005 - khali@linux-fr.org
- quilt/files.in: Sort the file names rather than respecting the
  original order of the files in patches. This is slightly faster,
  and we don't much care about the original order.
* Fri Sep 23 2005 - khali@linux-fr.org
- Fix the test suite so that directories leftover from a previously
  interrupted test won't cause a test to fail.
* Fri Sep 23 2005 - khali@linux-fr.org
- Drop test/reorder.test, it duplicates test/sort.test but is
  less complete.
* Fri Sep 23 2005 - khali@linux-fr.org
- test/run: Do not depend on Term::ReadKey.
* Fri Sep 23 2005 - agruen@suse.de
- Fix the test suite so that it doesn't fail if any higher-level
  directory contains a patches/ sub-directory (iow, if quilt
  itself is patched with quilt).
* Thu Sep 22 2005 - khali@linux-fr.org
- Makefile.in: Fix extra spaces in foreach command which were
  causing "missing separator" errors. Patch from Gary V. Vaughan.
* Thu Sep 22 2005 - khali@linux-fr.org
- configure.ac: Test sed support for (foo|bar) constructs.
* Wed Sep 21 2005 - khali@linux-fr.org
- configure.ac, Makefile.in: Make the cp, date and tr binaries
  user-selectable.
- configure.ac: Test grep and tr features.
* Wed Sep 21 2005 - khali@linux-fr.org
- aclocal.m4: Fix typo causing binaries with alternative names not
  to be properly handled.
* Wed Sep 21 2005 - khali@linux-fr.org
- quilt/files.in: Compute file status only when needed (that is,
  when -v is used and -l isn't). This divides the execution time
  by 5 in all other cases.
* Wed Sep 21 2005 - khali@linux-fr.org
- test/subdir.test: Test quilt files -v.
* Wed Sep 21 2005 - khali@linux-fr.org
- test/sort.test: Quote grep arguments, as some grep
  implementations seem to otherwise choke on them. Original patch
  from John Vandenberg.
* Wed Sep 21 2005 - khali@linux-fr.org
- quilt/diff.in: Add missing call to print_patch.
* Tue Sep 20 2005 - khali@linux-fr.org
- Makefile.in: Fix compat/tail symlink creation. Fix variable 
  exporting bashism.
- configure.ac: Use grep >/dev/null instead of grep -q for
  compatibility. Test for grep -q, some quilt scripts need it.
- test/Makefile: Fix varible exporting and script sourcing
  bashisms.
* Mon Sep 19 2005 - agruen@suse.de
- Makefile.in: Fix when COMPAT_PROGRAMS is empty.
- Avoid spurious rebuilds because of unnecessary chmod.
* Mon Sep 19 2005 - khali@linux-fr.org
- Makefile.in: Fix harmless error message in reference rule.
* Mon Sep 19 2005 - khali@linux-fr.org
- aclocal.m4: Fix a bug causing user-selected binary names not
  to be displayed properly.
* Mon Sep 19 2005 - agruen@suse.de
- Use compatibility m4 for diffstat. Correctly diagnose older
  diffstat on OS X 10.3, which outputs '1.2 (patch 5)'. Update
  configure warning message to reflect reality that quilt diff
  does not accept --diffstat (John Vandenberg <jayvdb@gmail.com>).
* Mon Sep 19 2005 - agruen@suse.de
- quilt/mail.in: add a --sender option for specifying the envelope
  sender address (defaults to ${LOGNAME:-`whoami`}@`hostname -f`).
* Mon Sep 19 2005 - agruen@suse.de
- Rename MTA program to SENDMAIL, and improve configure.ac
  (from John Vandenberg again, as several other recent things --
   thanks!)
- Must build the compatibility programs before creating the
  command reference (which extracts the help texts).
* Mon Sep 19 2005 - agruen@suse.de
- Fix mta check when sendmail is found.
- Fix --disable-nls.
- configure.ac: Don't run tests when using internal compatibility
  programs.
* Sun Sep 18 2005 - agruen@suse.de
- Add a missing $(BUILD_ROOT) and fix compat-symlink dependencies.
* Sun Sep 18 2005 - khali@linux-fr.org
- configure.ac: Test for tail -n.
* Sun Sep 18 2005 - agruen@suse.de
- Makefile.in: Add a dependency of all to COMPAT_PROGRAMS which
  was lost somewhere. Fix internal "mta" program.
- Get rid of ``grep -E''.
* Sun Sep 18 2005 - agruen@suse.de
- Fix a few things in po/quilt.pot generation.
- Always create the mta symlink in compat/. Replace "@MTA@" with
  "mta" in scripts. Replace mta check in configure.ac with
  QUILT_COMPAT_PROG_PATH.
- quilt/mail.in: Add a User-Agent header.
- Update the German translation.
* Sun Sep 18 2005 - agruen@suse.de
- Makefile.in: remove obsolete substitutions.
* Sun Sep 18 2005 - agruen@suse.de
- Get rid of ``grep -e''.
* Sun Sep 18 2005 - agruen@suse.de
- Remove config.h, and use @DEFS@ in Makefile.in instead
  (Gary V. Vaughan <gary@gnu.org>).
* Sun Sep 18 2005 - khali@linux-fr.org
- configure.ac: Drop useless -e to grep.
* Sun Sep 18 2005 - agruen@suse.de
- Merge po/Makefile into Makefile.in. Add a lost @false to
  Makefile.in.
* Sun Sep 18 2005 - agruen@suse.de
- lib/backup-files.c: add missing dependency on config.h. Check
  for chmod and fchmod support. Remove two unnecessary chmod calls.
- Makefile.in: fix cyclic dependency on Makefile.
* Sun Sep 18 2005 - agruen@suse.de
- lib/backup-files.c: use mktemp if mkstemp is not available
  (Gary V. Vaughan <gary@gnu.org>). Add a config.h.
* Sun Sep 18 2005 - agruen@suse.de
- lib/backup-files.c: switch from alloca to malloc for portability.
* Sun Sep 18 2005 - khali@linux-fr.org
- configure.ac: Make the tail executable user-selectable.
* Sun Sep 18 2005 - agruen@suse.de
- Forgot to add new files to the repository.
* Sun Sep 18 2005 - agruen@suse.de
- Merge compatibility layer from  John Vandenberg <jayvdb@gmail.com>.
- Only add compat/ directory to PATH if it exists. Only create it
  when necessary.
- configure.ac: fail if getopt does not support --long.
- Replace @INSTALL@ with $(INSTALL) and @BASH@ with $(BASH) in
  Makefile.in.
- Replace @FOO@ macros in scripts with foo command where possible.
- Remove compatibility shell functions and convert them to
  standalone scripts.
* Fri Sep 16 2005 - khali@linux-fr.org
- configure.ac: Fix a copy-n-paste bug which was causing perl to
  be checked twice and awk not to be checked properly.
* Fri Sep 16 2005 - khali@linux-fr.org
- Make it possible to pass arbitrary options to diffstat, by
  defining a QUILT_DIFFSTAT_OPTS variable in ~/.quiltrc.
- Update documentation and sample configuration file accordingly.
* Thu Sep 15 2005 - agruen@suse.de
- Fix off-by-one error: it's not a good idea to edit $x instead
  of $x.in.
* Thu Sep 15 2005 - khali@linux-fr.org
- Makefile.in: Add missing dependency of doc/README on quilt/*.
- Makefile.in: Rewrite non-portable sed command in "reference"
  rule.
* Thu Sep 15 2005 - khali@linux-fr.org
- Omitting the search directory in "find" is not portable; add
  an explicit "." (Gary V. Vaughan, John Vandenberg).
* Thu Sep 15 2005 - agruen@suse.de
- Fix quilt.spec generation after the Sep 14 change.
* Thu Sep 15 2005 - agruen@suse.de
- quilt/pop.in: get rid of tac.
* Wed Sep 14 2005 - agruen@suse.de
- quilt/mail.in: Remove stray getopt line. If --subject is given,
  actually fill in the subject in the message template.
* Wed Sep 14 2005 - khali@linux-fr.org
- Makefile.in: Use @BASH@ instead of bash (John Vandenberg).
- Makefile.in: Use `foo` instead of $$(foo) for compatibility.
- Makefile.in: Drop call to chmod --reference (Gary V. Vaughan).
* Tue Sep 13 2005 - khali@linux-fr.org
- quilt/annotate.in: Rework the annotation algorithm (Andreas
  Gruenbacher). The new code is shorter and faster.
- quilt/annotate.in: New option "-p patch" to stop checking for
  changes at the specified rather than the topmost patch.
- bash_completion: Support the new -p option.
- test/annotation.test: Test the new -p option.
* Sun Sep 11 2005 - khali@linux-fr.org
- quilt/annotate.in: Update help text to mention that only applied
  patches will show (Andreas Gruenbacher). Optimize by using
  applied_patches() instead of cat_series().
* Fri Sep 09 2005 - khali@linux-fr.org
- test/*.test: Fix sed compatibility issues: do not use inplace
  (-i); do not filter out tabs in patches, it's no more needed
  now that --no-timestamps is used; do not use the "a" command.
  Based on a patch by John Vandenberg.
* Fri Sep 09 2005 - khali@linux-fr.org
- scripts/patchfns.in: New quilt_command function for running
  another quilt command. Patch from Joe Green.
- quilt/delete.in, quilt/edit.in: Use the new quilt_command
  function. Patch from Joe Green.
* Wed Sep 07 2005 - khali@linux-fr.org
- quilt/delete.in: Fix the quilt pop invocation when attempting
  to delete the top patch (default). Patch by Joe Green.
* Tue Sep 06 2005 - khali@linux-fr.org
- Japanese translation update contributed by Satoru Takeuchi and
  Yasushi Shoji.
* Tue Sep 06 2005 - khali@linux-fr.org
- It is now possible to set EDITOR in ~/.quiltrc in order to use
  a different editor for quilt and for the other tasks.
* Tue Sep 06 2005 - khali@linux-fr.org
- doc/README needs the same $EDITOR filtering quirk we just added
  for doc/quilt.1.
* Tue Sep 06 2005 - khali@linux-fr.org
- scripts/patchfns.in: Rewrite files_in_patch_ordered() so as to
  not require the awk asort function, which is not available on
  all versions of awk.
- test/sort.test: Add more files to better test the ordering and
  sorting issues. Original patch by John Vandenberg.
* Tue Sep 06 2005 - khali@linux-fr.org
- Merge doc/sample.quiltrc into quilt.quiltrc, and delete it.
  It will be easier to maintain a single configuration file.
* Tue Sep 06 2005 - khali@linux-fr.org
- Makefile.in: Filter out expanded $EDITOR in quilt.1. This value
  being set at installation time may be irrelevant at execution
  time.
* Tue Sep 06 2005 - agruen@suse.de
- Fix test/perms.test for LANG=POSIX (from John Vandenberg
  <jayvdb@gmail.com>), and always run the test cases with LANG set
  to POSIX.
* Sun Sep 04 2005 - khali@linux-fr.org
- quilt/diff.in: Fix incorrect error message when incompatible
  options are used. Fix direct call to awk.
* Fri Sep 02 2005 - khali@linux-fr.org
- scripts/patchfns.in: Fix permissions of .pc/applied-patches
  being lost in remove_from_db() and rename_in_db().
* Fri Sep 02 2005 - khali@linux-fr.org
- quilt/refresh.in: Drop a useless line in the awk script
  replacing the diffstat section in patch headers. Improve the
  regular expression matching diffstat lines. Original patch from
  John Vandenberg.
* Sun Aug 28 2005 - khali@linux-fr.org
- configure.ac, Makefile.in: Support GCC and other compilers.
  Allow executable extensions (non-Unix.) Original patch from
  John Vandenberg.
* Fri Aug 26 2005 - khali@linux-fr.org
- quilt/delete.in: Drop extra spaces in usage line.
- quilt/graph.in: Add missing -T option to usage line.
- quilt/push.in: Add missing --interactive and --color options to
  usage line.
* Fri Aug 26 2005 - khali@linux-fr.org
- quilt/delete.in: Add options -r (remove patch file) and
  --backup (backup rather than remove.) Patch from Joe Green.
* Fri Aug 26 2005 - agruen@suse.de
- scripts/patchfns.in: insert_in_series(): Insert new patches
  before the following patch instead of after the top patch. When
  no patch is in the series file, insert the new patch at the end
  (i.e., after comments from setup, etc.) instead of at the top.
* Fri Aug 26 2005 - khali@linux-fr.org
- quilt/import.in: Fix error path when imported patch doesn't
  exist.
* Wed Aug 24 2005 - agruen@suse.de
- quilt/edit.in: Check exit status after adding files, and fail
  except when files have already been added.
* Mon Aug 22 2005 - agruen@suse.de
- quilt/fold.in: fold did not work when invoked from a
  subdirectory; update test case.
* Fri Aug 12 2005 - khali@linux-fr.org
- configure.ac: Do not check for diffstat version if
  --without-diffstat is used.
* Thu Aug 11 2005 - mquinson@debian.org
- bash_completion: add completion of the header command
  [Joe Green].
- improve description of the import command
  [Joe Green].
* Fri Jul 29 2005 - khali@linux-fr.org
- test/delete.test: New test case for the delete command.
* Fri Jul 29 2005 - khali@linux-fr.org
- Makefile.in: add test/Makefile and test/test.quiltrc to SRC,
  so that they get included into the tarball by make dist.
* Wed Jul 27 2005 - agruen@suse.de
- test/missing.test: Fix test case.
* Tue Jul 26 2005 - khali@linux-fr.org
- Bump version to 0.42.
* Tue Jul 26 2005 - agruen@suse.de
- Mike Fabian <mfabian@suse.de> fixed two fuzzy Japanese messages
  for us -- thanks!
* Mon Jul 25 2005 - khali@linux-fr.org
- Fix one fuzzy message in French translation.
* Mon Jul 25 2005 - agruen@suse.de
- Japanese translation update from Yasushi SHOJI
  <yashi@atmark-techno.com>.
* Sun Jul 24 2005 - khali@linux-fr.org
- Let the perl scripts (edmail, parse-patch and
  remove-trailing-ws) work without Locale::gettext.
* Fri Jul 22 2005 - khali@linux-fr.org
- Makefile.in: Drop pointless dependency between quilt command
  files and scripts.
* Tue Jul 19 2005 - khali@linux-fr.org
- Update French translation.
* Tue Jul 19 2005 - agruen@suse.de
- Fix a few translation bugs; update german translation.
* Mon Jul 18 2005 - agruen@suse.de
- Add internationalization support to Perl scripts as well.
- Rename test/create-delete.diff to *.test and fix the test.
* Mon Jul 18 2005 - khali@linux-fr.org
- French translation unfuzzying.
* Mon Jul 18 2005 - agruen@suse.de
- uninstall target: remove $(LIB_DIR) as well.
* Mon Jul 18 2005 - agruen@suse.de
- Based on work done by several contributers, add a header
  command.
- Bump version to 0.41.
* Mon Jul 18 2005 - agruen@suse.de
- quilt/annotate.in: Fix case where the file is created/deleted
  by a patch.
- Makefile.in: add uninstall target.
* Sun Jul 17 2005 - agruen@suse.de
- bin/quilt.in: clean up helptext.
* Thu Jul 14 2005 - khali@linux-fr.org
- Update the French translation.
* Thu Jul 14 2005 - agruen@suse.de
- Use wildcard to generate the list of all commands in Makefile
  instead of explicitly listing them.
- Helptext improvements.
* Thu Jul 07 2005 - khali@linux-fr.org
- Makefile.in: Properly set LINGUAS before running "make all" in
  subdirectory po. This ensures that all .mo files will actually
  be built.
- Makefile.in: Delete po/*.mo only once on "make clean".
* Wed Jul 06 2005 - khali@linux-fr.org
- quilt delete: Add option -n, which deletes the next patch
  after topmost (rather than the specified or topmost patch).
* Tue Jul 05 2005 - khali@linux-fr.org
- fr.po: Add all missing translations.
- de.po: Add one easy translation.
* Tue Jul 05 2005 - khali@linux-fr.org
- quilt delete: Display the name of the deleted patch on success.
* Sat Jul 02 2005 - agruen@suse.de
- Remove outdated BUGS file and references.
- refresh command: don't write into existing patches but create
  new files instead to avoid overwriting hardlinked copies.
* Fri Jul 01 2005 - khali@linux-fr.org
- configure.ac: Add a test on diffstat version. Versions prior
  to 1.32 are known to behave in a confusing manner, so we warn
  the user if such an old version is found.
* Wed Jun 29 2005 - agruen@suse.de
- po/de.po: Small update from Debian Bug #313827.
* Wed Jun 29 2005 - khali@linux-fr.org
- Fix a typo in diff.in's help.
* Wed Jun 29 2005 - khali@linux-fr.org
- French translation unfuzzying.
* Mon Jun 27 2005 - khali@linux-fr.org
- Fix a typo in quilt.changes.
* Mon Jun 27 2005 - agruen@suse.de
- quilt diff: Fix -z option if QUILT_PATCHES is an absolute path.
* Sun Jun 26 2005 - agruen@suse.de
- Jean Delvare:
  + Add a --version option that prints the program version.
  + patch_args: Fix comments parsing in series files.
* Wed Jun 22 2005 - agruen@suse.de
- annotate: temp file leak; add bash completions (Jean Delvare).
* Tue Jun 21 2005 - agruen@suse.de
- quilt/annotate.in: Add comment; remove some dead code.
* Tue Jun 21 2005 - agruen@suse.de
- add command: "File %s added to patch %s" message goes to stdout
  and not to stderr (Jean Delvare).
- Don't fail when popping missing patches (Jerome Lacoste, Jean
  Delvare, I).
* Sat Jun 18 2005 - agruen@suse.de
- Add fix for refresh --strip-trailing-whitespace option: now
  refresh always checks for trailing whitespace and reports it
  per file and line number. With --strip-trailing-whitespace it
  strips the whitespace from the patch and from the affected files
  in the working tree.
* Sat Jun 18 2005 - agruen@suse.de
- Add new annotate command.
* Thu Jun 16 2005 - agruen@suse.de
- rpm build: add missing files to file list; remove percent-prep
  string from changes file so that RPM doesn't choke on it.
* Tue Jun 14 2005 - agruen@suse.de
- refresh command: also touch .pc/$patch/.timestamp when a patch
  is found to be up-to-date (Peter Williams,
  <pwil3058@bigpond.net.au>).
* Sun Jun 12 2005 - agruen@suse.de
- Documentation update from Jerome Lacoste
  <jerome.lacoste@gmail.com> and me.
* Sun Jun 12 2005 - agruen@suse.de
- Remove obsolete remnants of removed -n flag (Jean Delvare).
* Thu Jun 09 2005 - agruen@suse.de
- Additional fixes to the rename command.
* Wed Jun 08 2005 - agruen@suse.de
- Add rename command from Jean Delvare <khali@linux-fr.org>.
* Wed Jun 08 2005 - agruen@suse.de
- Add testcase for diff --no-index.
* Mon Jun 06 2005 - agruen@suse.de
- Add --no-index option to diff and refresh commands: Suppress
  Index: lines in the patch. From Jean Delvare <khali@linux-fr.org>.
* Wed Jun 01 2005 - agruen@suse.de
- test/remove2.test: Test for remove + refresh interactions.
* Mon May 23 2005 - agruen@suse.de
- quilt.quiltrc: A minor cleanup in quilt_mail_patch_filter().
* Sun May 22 2005 - agruen@suse.de
- setup command: When a patch cannot be identified, scan all the
  files in the build directory: the patch may have been created,
  unpacked from an archive, etc.
* Sun May 22 2005 - agruen@suse.de
- refresh command: Fix temporary file leak (from Jean Delvare).
* Wed May 18 2005 - agruen@suse.de
- debian/changelog: Fix typo.
* Thu May 12 2005 - agruen@suse.de
- Quoting fix from Jean Delvare <khali@linux-fr.org>, and one more
  from me.
* Sun May 08 2005 - agruen@suse.de
- doc/quilt.1.in: Fix some typos (mostly from Jean Delvare).
* Sun May 01 2005 - agruen@suse.de
- Fix misspelling in documentation (Nicolas Kaiser <nikai@nikai.net>).
* Fri Apr 29 2005 - agruen@suse.de
- Add trailing-whitespace check an --strip-trailing-whitespace
  option to the refresh command.
- Bump version to 0.40.
* Tue Apr 19 2005 - agruen@suse.de
- fork command help update (Jean Delvare <khali@linux-fr.org>).
* Wed Apr 13 2005 - agruen@suse.de
- Add --sort options to the diff and refresh commands which
  sorts files by filename. The default is to keep files in their
  original order whenever possible.
- refresh command: add new files in sort order instead of the
  filesystem order.
- diff command: output files in the order in which they show up
  in the patch instead of sorting them alphabetically.
- Add sort testcase.
* Wed Apr 13 2005 - agruen@suse.de
- Fix typos in introduction.
* Tue Apr 05 2005 - agruen@suse.de
- doc/quilt.1.in: Remove an unnecessary 8-bit character.
* Thu Mar 10 2005 - agruen@suse.de
- Debian Bug #298785: Pass -p $opt_strip_level option to diffstat
  in refresh command.
* Mon Mar 07 2005 - agruen@suse.de
- Another Japanese translation update from Yasushi SHOJI.
* Sun Mar 06 2005 - agruen@suse.de
- Add `graph', `grep' and `mail' to QUILT_IN in po/Makefile.
- Fix typo and translation updates.
- Add a missing ``exit 1'' to quilt/fold.in when no patches are
  applied. (All except the German translation update from Yasushi
  SHOJI -- thanks.)
* Thu Feb 24 2005 - mquinson@debian.org
- quilt add: Add check to ensure that no symlink will be added
  since we fail to backup them properly (see #295913 in Debian).
* Fri Feb 18 2005 - agruen@suse.de
- quilt add: Add check to ensure that no files below patches/
  or .pc/ will be added.
* Thu Feb 10 2005 - agruen@suse.de
- quilt remove: Update restored file's timestamp. Also, some uses
  caused a subsequent ``quilt diff -z'' to fail: we must mark
  the patch as dirty after removing files.
- test/run: adjust to terminal width.
- Bump version to 0.39.
* Sun Feb 06 2005 - agruen@suse.de
- Update of the Japanese translation from Yasushi SHOJI
  <yashi@atmark-techno.com> -- thank you.
- Don't leak so many temporary files in the push command.
- Include command names in tempfile names to find leaks more
  easily.
* Wed Feb 02 2005 - agruen@suse.de
- Fix date calculation bug with some versions of GNU date.
* Wed Feb 02 2005 - agruen@suse.de
- Use color defaults so that QUILT_COLORS can be used to only
  override some colors.
- When checking for remaining changes (refresh forgotten), use
  $QUILT_PATCH_OPTS as in the push command to get identical
  results. Don't generate the actually diff; it doesn't get
  printed anyway. This should speed things up.
* Thu Jan 27 2005 - agruen@suse.de
- lib/backup-files.c: Remove unused command line option and
  document the remaining options.
- quilt/pop.in: Colorize this as well:
  "Patch attempted to create file *, which already exists."
  "can't find file to patch at input line *"
  "* out of * hunk ignored"
* Tue Jan 25 2005 - agruen@suse.de
- Fix pop command for recent coreutils (cp dir dir2/ no longer
  works if dir2 doesn't yet exist).
* Sun Jan 23 2005 - agruen@suse.de
- Add color support for push command output and allow to customize
  colors via the QUILT_COLORS environment variable (for reverse
  video, etc.).
* Sun Jan 23 2005 - agruen@suse.de
- Add mail command, doc/README.MAIL and example ``mail'' command
  filter in quilt.quiltrc.
- Bump version to 0.38.
* Fri Jan 21 2005 - agruen@suse.de
- Add a missing find_patch call in the files command.
* Thu Jan 20 2005 - mquinson@debian.org
- fix the bash completion file: 
  - does not complete on dir anymore, just files.
  - use string comparison when dealing with BASH_VERSION, it may
    contain letters.
* Tue Jan 11 2005 - agruen@suse.de
- Add a default /etc/quilt.quiltrc file that is sourced if no
  ~/.quiltrc file exists.
* Fri Nov 19 2004 - agruen@suse.de
- Push command: Write out output of patch process immediately
  instead of reading into a variable and then printing that
  variable. This helps to see what's happening with large patches.
* Mon Nov 15 2004 - agruen@suse.de
- Get rid of the apatch and rpatch helper scripts: Integrate them
  in the push and pop commands. This should bring a small
  performance improvement.
* Mon Nov 08 2004 - agruen@suse.de
- Fix extra trailing space in series file after quilt refresh
  (David Vrabel <dvrabel@arcom.com>).
- Missing quotation in bash_completion
  (Axel Grossklaus <ag@pre-secure.de>).
- Fix patches command (Brent Casavant <bcasavan@sgi.com>).
* Sun Oct 17 2004 - agruen@suse.de
- When applying a patch with -q and without -f no (individual)
  reject files are created, and the "patching file" comments
  are missing as well. In that case, we don't know wich files the
  "-- saving rejects to file /tmp/foo" comments belong to, and we
  can't fix them up. Remove them, instead.
- Pop command: Touch files after they are restored so that tools
  like make don't get confused.
- Add a missing print_patch call (cosmetic).
- Bump to version 0.37.
* Wed Sep 22 2004 - agruen@suse.de
- scripts/patchfns.in: set the dotglob option so that dot files
  will show up in file globs as well. Bug reported by James Rowe.
- Add a missing tab to the patch header of files that are removed.
  Bug reported by James Rowe.
- Push command: Without -f or --leave-rejects, when applying a
  patch failed, the *.rej files were not removed properly. Change
  the code so that they won't be created in the working tree in
  the first place in this case.
- test/run script: Oops, the previous commit was not the latest
  version.
- Fix the version check in bash_completion for bash 3.
- Bump version to 0.36.
* Tue Sep 21 2004 - agruen@suse.de
- Series command: add missing paths to patches if
  $QUILT_PATCHES_PREFIX is set.
- test/run script: Add export and unset statements.
* Mon Sep 13 2004 - agruen@suse.de
- bin/quilt.in: Add optional verbose modifier to --trace option
  by request of Dean Roehrich <roehrich@sgi.com>.
- Fix a message; took a while. Thanks Yasushi SHOJI
  <yashi@atmark-techno.com>.
* Sun Sep 12 2004 - agruen@suse.de
- scripts/inspect.in: Run the RPM prep section in /var/tmp and
  not in /tmp: the sources can become large.
- The fork command at some point broke and did not insert the
  "serial number" in the right place in patch names anymore.
* Wed Aug 25 2004 - agruen@suse.de
- Refresh command: recognize and ignore header lines generated by
  ``cvs diff''.
- Add a missing print_patch call (cosmetic).
* Sat Aug 14 2004 - agruen@suse.de
- Add Japanese translation from
  Yasushi SHOJI <yashi@atmark-techno.com>. Thank you.
* Fri Aug 13 2004 - agruen@suse.de
- setup command: detect errors when running ``rpm -bp'' and
  improve the messages of ``quilt setup''.
* Wed Aug 11 2004 - agruen@suse.de
- Merge improvement to files command from Dean Roehrich: Allow to
  list all files modified, or all files modified by a range of
  patches.
- Fix pop command help.
* Wed Aug 11 2004 - agruen@suse.de
- Working in sub-directories: `quilt new' so far did always
  create patches in the current directory, even if the current
  directory appeared to be a sub-directory of the working tree.
  This was confusing for several people now. Make `quilt new'
  behave like all other commands, and thus allow it to be used
  from within sub-directories.
* Sat Aug 07 2004 - mquinson@fuxfamily.org
- Small fix by Yasushi SHOJI: display the name of the patch which
  is not applied in the error message.
* Thu Jul 22 2004 - mquinson@tuxfamily.org
- repharse the output of rpatch when patch does not remove properly:
  refreshing the patch is another way to fix it
- sync french translation.
* Thu Jul 15 2004 - mquinson@tuxfamily.org
- patch from Joe Green to "Complete options for a partial command
  as long as it's unique. quilt will accept such a partial command
  as valid."
* Thu Jul 15 2004 - agruen@suse.de
- Add test case for permissions.
- Very minor translation updates.
- Convert timestamps in changes file into UTC when generating
  the RPM changelog: the changelog does not understand
  timestamps.
- Bump version to 0.35.
* Tue Jul 13 2004 - mquinson@tuxfamily.org
- Fix a typo (s/prinf/printf/) reported by Dean Roehrich.
- Track down two variable expensions in printf $"" constructs.
- update the french translation accordingly
* Mon Jul 12 2004 - agruen@suse.de
- refresh command: Fix --diffstat option; it did swap lines. Also
  remove stray dollar sign.
- Also set TEXTDOMAINDIR in the wrapper script (bin/quilt.in).
- Fix stripping of the path to $QUILT_PATCHES when working in
  sub-dirextories. (This could be fixed more flexibly later...)
* Mon Jul 12 2004 - mquinson@tuxfamily.org
- Sync french translation after the recent butchery ;)
- Commit patch from Joe Green to make the completion file usable on
  non-debian systems.
* Mon Jul 12 2004 - agruen@suse.de
- Some more QUILT_PATCHES_PREFIX fixes.
- Disable CDPATH within quilt (Joe Green writes: I've seen some
  cases where having CDPATH set results in the path being printed
  by cd, and I suppose it's possible that the wrong destination
  might be selected in some cases.)
* Fri Jul 09 2004 - agruen@suse.de
- Switch from echo to printf for all translations: This is more
  stable than having expansions in messages that might change
  independent of the message.
- Add print_patch function and always use it when printing patch
  names. Remove -n options from various scripts and use a global
  switch QUILT_PATCHES_PREFIX in .quiltrc to decide between patch
  names with and without directory prefix. Depending on user
  experience this switch may eventually go away.
- Add --diffstat option to refresh command: If given, this option
  inserts diffstat statistics at the end of the patch header, or
  refreshes the existing diffstat output. No special tags in the
  path file (%diffstat or the like) are needed.
- Minor fix inserting changelog into RPM specfile.
* Wed Jul 07 2004 - mquinson@tuxfamily.org
- Fix the bash_completion by not using quilt internals, but the
  external commands [Joe Green].
* Sun Jul 04 2004 - agruen@suse.de
- Add common GNU diff format options to diff and refresh commands:
  -u, -U num, -c, -C num. Rename old -c option (combine patches)
  of diff command to --combine.
- Extend the function that splits comments from patches and
  syntax coloring to handle combined diff format.
- Honor the LANG environment variable again; this got broken at
  some point.
- Set TEXTDOMAINDIR to where the translations are put so that
  Messages are found even when not installing into /usr.
* Fri Jul 02 2004 - agruen@suse.de
- Several Fixes from Joe Green <jgreen@mvista.com>:
  + Another sed 	 problem, in Makefile.in
  + Bad error message on "quilt delete" of non-existent patch
  + "import" doesn't remove all leading components on patch file
  + Misspelling in "import" help text
- With using the GNU diff --label option, file timestamps
  disappeared. Specify the timestamps in --label by hand: This
  gives us added flexibility when generating patches; we may
  some day want to support diff styles other than unified.
  Also reported by Joe Green.
- Add --backup option to quilt refresh.
- Update documentation to QUILT_${COMMAND}_ARGS in .quiltrc.
  (The old settings still work but they are no longer documented.)
- Update German translation.
* Thu Jul 01 2004 - agruen@suse.de
- Allow passing of default options to commands by defining
  QUILT_${COMMAND}_ARGS in .quiltrc. This should eventually get
  us rid of some of the mess in .quiltrc.
* Wed Jun 30 2004 - agruen@suse.de
- Quilt diff: add --color option for optionally colorizing the
  output.
* Sat Jun 26 2004 - agruen@suse.de
- Quilt import: when importing patches with their original file
  name, strip path prefixes from the patch names.
* Sat Jun 12 2004 - agruen@suse.de
- Fix another upgrade bug: The series file may optionally contain
  a strip level argument (-pN) which must be stripped off when
  determining the patch name.
- A minor correction in the doumentation; some whitespace fixes.
- Translation update.
* Fri Jun 11 2004 - mquinson@tuxfamily.org
- Fix the man page generation
- Add a --no-timestamps option to diff and refresh doing what
  QUILT_NO_DIFF_TIMESTAMPS does. This should make more visible a
  feature I was about reimplementing ;)
- Document all options from the pdf into the man page.
- update the bash completion stuff so that :
  - it does not kill the shell when completing in a tree not
    matching the meta data version.
  - it does not move the current directory around, seeking for
    patches/
  - it completes on the new commands (grep graph upgrade)
- Various output messages polishing (and translation fuzzying)
- French translation unfuzzying
* Thu Jun 10 2004 - agruen@suse.de
- Add meta-data version check/upgrade code from Martin Quinson and
  me; new `quilt upgrade' command.
- Handle new/removed files correctly in generated patches: If the
  old or new file is missing, the file name in the patch should
  be /dev/null. Fix test suite accordingly, and add
  test/create-delete.diff. GNU patch recognizes this, and does
  additional file existance tests when a patch creates/deletes a
  file. Also use the --label option of GNU diff instead of
  hand-editing file name headers.
- Add test/Makefile for running the test suite. Note: The tests
  run against the installed version of quilt!
- Bump version to 0.34.
* Sun Jun 06 2004 - agruen@suse.de
- Preserve the order of files in patches in the diff and refresh
  commands. Files added to a patch appear at the end of a patch.
  The files command also lists the files in the order in which
  they appear in patches.
- Update to version 0.33.
* Sun Jun 06 2004 - agruen@suse.de
- dependency-graph: Fix for --lines option in patcher mode;
  restrict to applied patches.
* Sun Jun 06 2004 - agruen@suse.de
- import: Create patches/ and parent directories to patch to be
  imported. Insert the correct patch name in the series file.
* Sun Jun 06 2004 - agruen@suse.de
- backup-file.c: Add code to recursively search .pc directories.
  Add a "no-op" mode as default, and allow to just unlink files.
- apatch/rpatch: let backup-files search .pc directories instead
  of generating temporary file lists.
- rpatch/pop: unlink files of topmost patch after popping instead
  of unlinking at each step.
- If QUILTRC is set in the environment, use this as the
  configuration file.
* Sat Jun 05 2004 - agruen@suse.de
- Return exit status 2 when commands go beyond the series (top
  with no series, push/next when all patches are applied, pop/
  previous when no patches are applied).
- From  John Lenz <jelenz@students.wisc.edu>: Add compatibility
  code to the dependency-graph script for reading the meta-data
  of Holger Schurig's patcher tool,
  http://www.holgerschurig.de/patcher.html.
- Update German translation.
* Tue Jun 01 2004 - agruen@suse.de
- Fix from Dean Roehrich <roehrich@sgi.com>: Preserve file mode
  of original file when creating backup files.
* Tue Jun 01 2004 - mquinson@tuxfamily.org
- document common options to all scripts (--trace and --quiltrc)
- update french translation
* Mon May 31 2004 - agruen@suse.de
- Rewrite spec2series to return what the new `quilt setup' needs,
  and rename it to inspect.
- `quilt setup': Rewrite so that it supports multiple archives
  per spec file. Always make the patches directory a symlink to
  the original patch files instead of copying the patches.
- Update German translation.
* Sun May 16 2004 - agruen@suse.de
- Clarify help text of `quilt add' (thanks to  Tom Rini
  <trini@mvista.com>). Update translations.
* Sat May 15 2004 - agruen@suse.de
- Add `quilt grep': Grep over all files, recursively, skipping
  the $QUILT_PATCHES and $QUILT_PC directories.
* Tue Apr 27 2004 - agruen@suse.de
- Fix `quilt fork': It destroys .pc/applied patches; that bug got
  introduced when reverting it to its previous semantics.
* Wed Apr 21 2004 - agruen@suse.de
- Replace two occurrences of 	 in sed regular expressions
  with bash $'	': Some older versions of sed don't understand
  	. Thanks to Randy Dunlap <rddunlap@osdl.org> and Greg Edwards
  <edwardsg@sgi.com> for reporting.
* Sat Mar 20 2004 - agruen@suse.de
- Fix an algorithmic bug in `quilt graph --lines': Edges were
  sometimes lost.
- A few minor cleanups.
* Sun Mar 14 2004 - agruen@suse.de
- Extend `quilt graph' to also support checking for overlapping
  changes in patches.
- Export QUILT_PATCHES QUILT_PC SUBDIR SERIES DB for use in
  non-shell components of quilt.
* Sat Mar 13 2004 - agruen@suse.de
- Change `quilt import' to allow importing multiple patches
  at once.
- Fix a bug in backup-files.c (introduced just before).
- Update to version 0.32.
* Sat Mar 13 2004 - agruen@suse.de
- Add `quilt graph' command for generating a dependency graph
  between patches. This requires the graphviz package for
  removing transitive edges (optional) and for rendering the
  graph. Please note that the graph command itself is minimal,
  while the underlying scripts/dependency-graph is more flexible.
- Complete one more detail in scripts/dependency-graph.
- Revert `quilt fork' to how it originally worked: fork the
  *topmost* patch, not the next unapplied patch. Adapt the
  test suite accordingly.
- Add flag to backup-files.c to make sure that files in the
  working tree have a link count of at most one. Use this where
  appropriate.
- Fix a bug in `quilt snapshot': Taking snapshots caused files
  to get link counts bigger than one, so modifying them could
  cause multiple files to get modified. Speed up the command,
  too.
- Update the documentation. Add example1.test from the
  documentation.
* Wed Mar 10 2004 - agruen@suse.de
- Quilt push/pop: exit with a non-zero status when beyond series.
- Ignore empty lines in series file.
- Add man page to tarball and to spec file; add make dependencies.
- Sort the commands in the man page command reference.
* Tue Mar 09 2004 - agruen@suse.de
- Make files writeable in `quilt add' (and `quilt edit') for
  working with read-only trees.
* Wed Feb 25 2004 - martin.quinson@tuxfamily.org
- Add completion to the edit command in bash
* Sun Feb 22 2004 - martin.quinson@tuxfamily.org
- Add a man page (generated automatically from -h)
* Sat Feb 21 2004 - agruen@suse.de
- Sub-directory support: `quilt setup' could get confused, too.
* Fri Feb 20 2004 - agruen@suse.de
- Sub-directory support:
  + Fix another bug that triggers when /patches exists.
  + Working in sub-directories: Force `quilt new' to always create
    the new patch in the current working directory even if there
    is a patches/ sub-directory further up the directory tree. 
    This should restore the behavior from before Jan 28 very well.
- quilt files, diff, refresh: Sort the files in the patch
  alphabetically.
* Sun Feb 15 2004 - agruen@suse.de
- Never fail when patching temporary files in `quilt diff -z',
  only warn.
* Thu Feb 05 2004 - agruen@suse.de
- Add some fixes from Manuel Estrada Sainz <ranty@debian.org>.
- Add QUILT_SERIES setting: Override the location of the series
  file to use.
- Don't print an empty line for `quilt top' if no patches are
  applied.
* Tue Feb 03 2004 - agruen@suse.de
- Add --quiltrc={rcfile|-} option. Remove some superfluous quoting
  from quilt wrapper.
- Extra tests in `quilt refresh' did not check if the old patch
  was missing.
* Mon Feb 02 2004 - agruen@suse.de
- Fix test for bash in configure.ac.
- Add `--trace' option to quilt wrapper (runs commands with -x,
  which prints an execution trace).
- Add QUILT_BACKUP setting in .quiltrc (creates backup copies
  when refreshing patches).
- Fix `quilt next'.
* Sat Jan 31 2004 - agruen@suse.de
- Also fix `quilt import' after patch name mangling removal.
* Sat Jan 31 2004 - agruen@suse.de
- No longer remove common extensions (.dif, .diff, .patch, .gz,
  .bz2) from patch names. This allows patches in the same series
  that only differ by extension, and cleans up the scripts a
  little. Also don't remove extensions when printing patch names.
- Remove unused diffstat code.
- Fix generating doc/README after subdirectory support broke it.
- Remove excessive newlines in -h messages.
* Wed Jan 28 2004 - agruen@suse.de
- Fix bugs introduced while merging Chris's patch.
- Add more test cases for subdirectory support.
- Fix `make dist': Some files in debian/ did not exist.
- Update to version 0.30.
* Wed Jan 28 2004 - agruen@suse.de
- Patch from Chris Mason <mason@suse.de>: Add support for working
  in subdirectories of the base directory that contains patches/
  and .pc/. In the unlikely case that quilt shall operate in the
  sub-directory itself, it is sufficient to create a patches/
  directory there.
- Add a small testcase for subdirectory support.
* Sat Jan 24 2004 - agruen@suse.de
- Add QUILT_NO_DIFF_TIMESTAMPS option in .quiltrc.
- Add QUILT_PATCH_OPTS setting to allow passing options to
  GNU patch.
- Fix help for `quilt diff'.
* Thu Jan 08 2004 - agruen@suse.de
- Also accept `quilt diff -z ./file' (ignore the ./ prefix that
  is often produced by find etc.)
* Thu Nov 27 2003 - agruen@suse.de
- `quilt fold' did not create directories for additional files,
  and the code for recovering from failures was wrong.
* Tue Nov 25 2003 - agruen@suse.de
- Update quilt paper.
* Mon Nov 24 2003 - agruen@suse.de
- Error check for `quilt diff -z filelist' was too strict; relax.
* Mon Nov 17 2003 - agruen@suse.de
- Add new command `quilt fold' to fold one patch into another.
- Fix bug in `quilt files': Files that are neither present in the
  working directory and have an empty backup copy sometimes were
  still listed.
- Some minor cleanups in apatch.
- Add test/fold.test, and fix a now-useless check in one.test.
* Sat Nov 15 2003 - agruen@suse.de
- Merge some changes from wangdi:
  + Use -f (force) option of GNU patch in `quilt diff -z'.
  + Build source and binary RPMs in `make rpm' target.
  + Simplify sed expression in Makefile.in.
* Wed Nov 12 2003 - agruen@suse.de
- Add introductory paper to CVS repository.
- Remove obsolete documentation.
- Bump version number to 0.29 (cvs tag VER_0_29).
* Tue Nov 04 2003 - agruen@suse.de
- Add failpop.test that checks if modified patches are recognized.
- Fix the check for modified patches: That broke with the .pc file
  removal.
- `quilt setup' sometimes forgot to create the patches sub-dir.
- Fix the timestamp check in scripts/rpatch.in for missing files.
* Fri Oct 31 2003 - agruen@suse.de
- Bump version number to 0.28 (cvs tag VER_0_28).
* Fri Oct 31 2003 - agruen@suse.de
- No longer verify if patches remove cleanly by reverse applying
  them: This doesn't always succeed, and the  the equivalent of
  `quilt diff -z' should suffice, anyway.
- Add `touch .pc/$patch' after successful refresh: Speeds up
  `quilt pop' after a refresh.
- Update test suite.
- apatch was printing a spurious newline.
* Wed Oct 29 2003 - agruen@suse.de
- If a patch does not apply cleanly and `quilt push' removes
  rejects files it GNU patch left behind, change the output from
  ``saving rejects to file x.rej'' to ``rejects in file x''.
* Tue Oct 28 2003 - agruen@suse.de
- Remove .pc/*/.pc files, and adjust various scripts accordingly.
- Run test/one.test inside sub-directory.
- Bump version number to 0.27 (cvs tag VER_0_27).
* Tue Oct 28 2003 - agruen@suse.de
- Change name and semantics of PATCHSCRIPTS variable: Before, if
  PATCHSCRIPTS was set, the patches were in $PATCHSCRIPTS/patches.
  Now, if QUILT_PATCHES is set, the patches are in $QUILT_PATCHES.
  QUILT_PATCHES defaults to "patches", like before.
* Sun Oct 26 2003 - agruen@suse.de
- Get rid of patches_per cache_in `quilt patches'. Remove
  patches_per_file and associated functions. `quilt patches'
  now is a bit slow for unapplied patches.
* Sun Oct 26 2003 - agruen@suse.de
- Rename snapshot directory from ".snap0" to ".snap".
- `quilt diff: Speed up file list generation in `quilt diff'.
  Don't use modified_files anymore. Instead, add first_modified_by
  function. I want to get rid of the patches_per_file cache;
  it sometimes was not brought up to date. Some cleanups.
- Add test/snapshot.test.
- Fix two errors in the date format in quilt.changes.
* Thu Oct 23 2003 - agruen@suse.de
- `quilt add': Return exit code 2 when a file has already been
  added.
- Add `quilt edit' command: Does a `quilt add' and then invokes
  $EDITOR (fallback is `vi'). Files that don't exist after editing
  are again removed from the patch.
* Tue Oct 21 2003 - agruen@suse.de
- `Quilt fork' should better fork the next patch instead of the
  topmost one. Also fix a bug in updating the series file when
  forking. The rename_in_db() function is no longer needed.
* Tue Oct 21 2003 - agruen@suse.de
- Add `quilt fork' command.
- Add `quilt snapshot' command and `quilt diff --snapshot' option.
- Bump the version number to 0.26.
* Tue Sep 09 2003 - martin.quinson@tuxfamily.org
- fix LC_ALL to C during building the reference so that the 
  documentation comes to english and not to french when build 
  on my box.
- Update the french translation
- Bump the version number to 0.25
* Tue Sep 02 2003 - agruen@suse.de
- Pipe rpmbuild output to /dev/null in spec2series (seems to be
  necessary since rpm4).
* Sun Jul 27 2003 - agruen@suse.de
- Use %{find_lang} macro in spec file.
* Sat Jul 26 2003 - agruen@suse.de
- Pushing is only possible if the topmost file is found in the
  series file. Fix from Tom Wang <wangdi@clusterfs.com>.
* Sat Jul 26 2003 - agruen@suse.de
- When popping files, go through the list of applied patches
  instead of looking at the series file: The series file may
  have changed.
* Sat Jul 26 2003 - agruen@suse.de
- Install package documentation into $RPM_DOC_DIR if this
  environment variable is set.  On RedHat 9 $RPM_DOC_DIR points to
  /usr/share/doc/packages, but documentation lives in
  /usr/share/doc.  This is a bug in RedHat's RPM configuration;
  working around it would break other distributions.
* Wed Jul 23 2003 - agruen@suse.de
- Add Autoconf test for rpmbuild vs. rpm (needed for `make rpm').
- Update spec2series accordingly.
* Sat Jul 19 2003 - agruen@suse.de
- lib/backup-files.c: Copy files if hard linking is not possible,
  e.g., because the backups go to a ram disc, etc.
* Thu Jun 26 2003 - martin.quinson@tuxfamily.org
- Add what needed to have inteligent bash completion when using quilt
- Bump version to 0.24
* Wed Jun 25 2003 - martin.quinson@tuxfamily.org
- Make clean before dist to avoid to put cruft in tarballs
- remove po/*mo on make clean
- update fr.po
- Fix a typo in push.in help message about --interactive
* Tue May 20 2003 - agruen@suse.de
- Preserve permissions of files added with `quilt add'.
* Sun May 18 2003 - agruen@suse.de
- Do not add the whole /usr/share/locale directory to the RPM
  file list, but only the quilt message catalogs: Otherwise RPM
  complains when uninstalling.
* Fri May 16 2003 - agruen@suse.de
- Rename DIFF_OPTS environment variable to QUILT_DIFF_OPTS
- Add ~/.quiltrc resource file (this file is sourced from
  the patchfsn file, which is used by all commands).
* Fri Apr 11 2003 - agruen@suse.de
- Apply patches with `patch -f' by default. Add --interactive
  option top `quilt push' to allow applying patches without
  `patch -f'.
* Wed Apr 09 2003 - agruen@suse.de
- Remove awk hacks in Makefile.in with shell loop and sed hack:
  The awk hack(s) didn't work for Martin Quinson.
* Wed Apr 09 2003 - agruen@suse.de
- Never reorder files in patches. (Previously the file list
  was run through (sort | uniq) to remove duplicate entries.)
- Update test script to new format, and add regression test
  cases.
* Wed Apr 09 2003 - agruen@suse.de
- The change from Mar 24 caused empty lines to be removed in
  patch descriptions.
* Mon Apr 07 2003 - agruen@suse.de
- `head -1' and `tail -1' is non-standard and deprecated, and
  does not work with coreutils-5.0 any longer.
* Sat Apr 05 2003 - agruen@suse.de
- Improve error checking of `quilt diff'.
* Mon Mar 24 2003 - agruen@suse.de
- Update patch_description function used in `quilt refresh' to
  recognize `====' lines. Without that, quilt thinks these
  lines are part of the comments before the actual patch starts,
  so those lines will accumulate.
* Sun Mar 23 2003 - agruen@suse.de
- Update test script to reflect recent patch format changes.
* Sat Mar 22 2003 - agruen@suse.de
- Also substitute @SED@ and @AWK@.
- Some versions of sed don't like '	'. Expand those in the
  shell instead.
* Fri Mar 21 2003 - agruen@suse.de
- Bump version number to 0.23 (CVS tag VER_0_23).
* Fri Feb 14 2003 - agruen@suse.de
- A couple more I18N fixes, minor cleanups in status messages.
- Remove "quilt guidiff" (obsolete).
* Fri Feb 14 2003 - martin.quinson@tuxfamily.org
- Implement i18n, along with the french l10n.
- It was long enough that we didn't release, so bump the version
  number.
* Sat Feb 08 2003 - agruen@suse.de
- Merge James Rowe <Jay@jnrowe.uklinux.net>'s quilt-help.diff
  patch that fixes several help messages.
* Thu Feb 06 2003 - agruen@suse.de
- Quilt refresh: Create sub-directories in patches directory
  so that new patches in sub-directories can be added easily.
* Wed Feb 05 2003 - agruen@suse.de
- Quilt diff: If a patch has been force applied, quilt diff -z
  will also have failures when generating the temporary files.
  Ignore these failures in this case.
- Add -R option to quilt diff to generate a a reverse patch.
- Fix deleting the temporary directory (by using an absolute
  instead of a relative path).
* Sun Feb 02 2003 - agruen@suse.de
- Make quilt setup recognize the "# Sourcedir:" directive.
- Fix bug with spurious `Index:' lines in diff/refresh output.
- Add --diff option to quilt diff that allows to specify a
  different utility that diff to run files through. (That utility
  is only called for files that contain differences, not for
  files that are in a patches' file list but jave not been
  changed.)
* Sat Feb 01 2003 - agruen@suse.de
- Integrate James Rowe's Autoconf setup (with a comple of
  additional changes/fixes).
- Add some messages in configure.ac.
- Make distclean removes autoconf garbage. The configure script is
  no source file. Add rule to create it when necessary.
- Invoke configure in the RPM spec file.
- Merge James Rowe's guidiff.
* Fri Jan 31 2003 - agruen@suse.de
- Add syntax highlighting tags for emacs and vim.
- Move all scripts from lib/ to scripts/. Install lib/backup-files
  in /usr/lib/quilt/, and install the scripts in
  /usr/share/quilt/scripts/, so that no binaries are installed
  under /usr/share.
- Fix quoting strings for inclusion in regular expressions.
- Change literal '@SUBST@' strings in Makefile to '@SUBST''@',
  so that Autoconf won't mess them up later.
* Fri Jan 31 2003 - Martin.Quinson@ens-lyon.fr
- Change '#!@BASH@' to '#! @BASH@'. It can look a bit stupid, but
  it's needed for maximal portability.
* Fri Jan 31 2003 - Martin.Quinson@ens-lyon.fr
- Fight lintian warnings (lintian is a tool to check debian
  packages):
- Remove the dependencies on essential packages diff, gzip,
  debianutils
- Format the copyright file properly
- Remove the #! /bin/bash header from patchfns, since it's not an
  executable script, but a stuff to be sourced by others.
* Thu Jan 30 2003 - agruen@suse.de
- Also substitute @MKTEMP@.
- For improved scripting support, add -n option to scripts that
  print patch names prints the patch file name instead. This
  is useful for selecting which patches to grep, etc.
* Thu Jan 30 2003 - agruen@suse.de
- Allow a file list to be diffed to be passed to quilt diff. Add
  -P option for specifying a patch different from the topmost
  patch,
* Thu Jan 30 2003 -  Martin.Quinson@ens-lyon.fr
- Remove useless oldies. Missing functionnalities needs to be
  reimplemented almost from the scratch due to the bunch of
  changes to the core since those scripts were written.
* Thu Jan 30 2003 - agruen@suse.de
- Fix some regex quoting; use grep -E because we quote for extended
  regexps. Filenames with "+^$" in it did not work in some places. 
- Prepare for GNU Autoconf: substitute @PERL@, @BASH@, @DIFF@,
  @PATCH@ in scripts.
- Some cleanups in Makefile.
- Move documentation to doc/.
- Forgot to mention that the patches produced now contain
  `Index: dir/filename' just before the diff output for each file.
  This is required by POSIX, and is also used by diffstat.
- Adjust patch_description documentation extraction function to
  check for Index: lines.
- Quilt refresh looses text before actual patch due to bug in
  patch_description function.
* Tue Jan 28 2003 - agruen@suse.de
- Add --leave-rejects option to quilt push and apatch, to
  leave reject files around for inspection even if a patch
  does not apply.
* Tue Jan 28 2003 - agruen@suse.de
- Restart change log at quilt-0.21. Let orderly development
  begin :)

#! /usr/bin/env python

# mklibs.py: An automated way to create a minimal /lib/ directory.
#
# Copyright 2001 by Falk Hueffner <falk@debian.org>
#                 & Goswin Brederlow <goswin.brederlow@student.uni-tuebingen.de>
#
# mklibs.sh by Marcus Brinkmann <Marcus.Brinkmann@ruhr-uni-bochum.de>
# used as template
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import re
import sys
import os
import glob
import getopt
from stat import *

DEBUG_NONE    = 0
DEBUG_NORMAL  = 1
DEBUG_VERBOSE = 2
DEBUG_SPAM    = 3

debuglevel = DEBUG_NONE

def debug(level, *msg):
    if debuglevel >= level:
        print ' '.join(msg)

# return a list of lines of output of the command
def command(command, *args):
    debug(DEBUG_SPAM, "calling", command, ' '.join(args))
    pipe = os.popen(command + ' ' + ' '.join(args), 'r')
    output = pipe.read().strip()
    status = pipe.close()
    if status is not None and os.WEXITSTATUS(status) != 0:
        print "Command failed with status", os.WEXITSTATUS(status),  ":", \
               command, ' '.join(args)
	print "With output:", output
        sys.exit(1)
    return output.split('\n')

# Filter a list according to a regexp containing a () group. Return
# a set.
def regexpfilter(list, regexp, groupnr = 1):
    pattern = re.compile(regexp)
    result = set()
    for x in list:
        match = pattern.match(x)
        if match:
            result.add(match.group(groupnr))

    return result

def elf_header(obj):
    if not os.access(obj, os.F_OK):
        raise "Cannot find lib: " + obj
    output = command("mklibs-readelf", "--print-elf-header", obj)
    s = [int(i) for i in output[0].split()]
    return {'class': s[0], 'data': s[1], 'machine': s[2], 'flags': s[3]}

# Return a set of rpath strings for the passed object
def rpath(obj):
    if not os.access(obj, os.F_OK):
        raise "Cannot find lib: " + obj
    output = command("mklibs-readelf", "-R", obj)
    return [root + "/" + x for x in output if x]

# Return a set of libraries the passed objects depend on.
def library_depends(obj):
    if not os.access(obj, os.F_OK):
        raise "Cannot find lib: " + obj
    return [x for x in command("mklibs-readelf", "-n", obj) if x]

# Return real target of a symlink
def resolve_link(file):
    debug(DEBUG_SPAM, "resolving", file)
    while S_ISLNK(os.lstat(file)[ST_MODE]):
        new_file = os.readlink(file)
        if new_file[0] != "/":
            file = os.path.join(os.path.dirname(file), new_file)
        else:
            file = new_file
    debug(DEBUG_SPAM, "resolved to", file)
    return file

# Find complete path of a library, by searching in lib_path
def find_lib(lib):
    for path in lib_path:
        if os.access(path + "/" + lib, os.F_OK):
            return path + "/" + lib

    return ""

def extract_soname(so_file):
    soname_data = command("mklibs-readelf", "-s", so_file)
    if soname_data:
        return soname_data.pop()
    return ""

def usage(was_err):
    if was_err:
        outfd = sys.stderr
    else:
        outfd = sys.stdout
    print >> outfd, "Usage: mklibs [OPTION]... -d DEST FILE ..."
    print >> outfd, "Make a set of minimal libraries for FILE(s) in DEST."
    print >> outfd, "" 
    print >> outfd, "  -d, --dest-dir DIRECTORY     create libraries in DIRECTORY"
    print >> outfd, "  -D, --no-default-lib         omit default libpath (", ':'.join(default_lib_path), ")"
    print >> outfd, "  -L DIRECTORY[:DIRECTORY]...  add DIRECTORY(s) to the library search path"
    print >> outfd, "      --ldlib LDLIB            use LDLIB for the dynamic linker"
    print >> outfd, "      --libc-extras-dir DIRECTORY  look for libc extra files in DIRECTORY"
    print >> outfd, "      --root ROOT              search in ROOT for library rpaths"
    print >> outfd, "  -v, --verbose                explain what is being done"
    print >> outfd, "  -h, --help                   display this help and exit"
    sys.exit(was_err)

def version(vers):
    print "mklibs: version ",vers
    print ""

#################### main ####################
## Usage: ./mklibs.py [OPTION]... -d DEST FILE ...
## Make a set of minimal libraries for FILE ... in directory DEST.
## 
## Options:
##   -L DIRECTORY               Add DIRECTORY to library search path.
##   -D, --no-default-lib       Do not use default lib directories of /lib:/usr/lib
##   -n, --dry-run              Don't actually run any commands; just print them.
##   -v, --verbose              Print additional progress information.
##   -V, --version              Print the version number and exit.
##   -h, --help                 Print this help and exit.
##   --ldlib                    Name of dynamic linker (overwrites environment variable ldlib)
##   --libc-extras-dir          Directory for libc extra files
##   --target                   Use as prefix for gcc or binutils calls
## 
##   -d, --dest-dir DIRECTORY   Create libraries in DIRECTORY.
## 
## Required arguments for long options are also mandatory for the short options.

# Clean the environment
vers="0.12"
os.environ['LC_ALL'] = "C"

# Argument parsing
opts = "L:DnvVhd:r:l:"
longopts = ["no-default-lib", "dry-run", "verbose", "version", "help",
            "dest-dir=", "ldlib=", "libc-extras-dir=", "root="]

# some global variables
lib_rpath = []
lib_path = []
dest_path = "DEST"
ldlib = None
include_default_lib_path = True
default_lib_path = ["/lib/", "/usr/lib/", "/usr/X11R6/lib/"]
libc_extras_dir = "/usr/lib/libc_pic"
target = ""
root = ""
so_pattern = re.compile("((lib|ld).*)\.so(\..+)*")

try:
    optlist, proglist = getopt.getopt(sys.argv[1:], opts, longopts)
except getopt.GetoptError, msg:
    print >> sys.stderr, msg
    usage(1)

for opt, arg in optlist:
    if opt in ("-v", "--verbose"):
        if debuglevel < DEBUG_SPAM:
            debuglevel = debuglevel + 1
    elif opt == "-L":
        lib_path.extend(arg.split(":"))
    elif opt in ("-d", "--dest-dir"):
        dest_path = arg
    elif opt in ("-D", "--no-default-lib"):
        include_default_lib_path = "no"
    elif opt == "--ldlib":
        ldlib = arg
    elif opt == "--libc-extras-dir":
        libc_extras_dir = arg
    elif opt in ("-r", "--root"):
        root = arg
    elif opt in ("-l",):
        pass
    elif opt in ("--help", "-h"):
	usage(0)
        sys.exit(0)
    elif opt in ("--version", "-V"):
        version(vers)
        sys.exit(0)
    else:
        print "WARNING: unknown option: " + opt + "\targ: " + arg

if include_default_lib_path:
    lib_path.extend(default_lib_path)

objects = {}  # map from inode to filename
for prog in proglist:
    inode = os.stat(prog)[ST_INO]
    if objects.has_key(inode):
        debug(DEBUG_SPAM, prog, "is a hardlink to", objects[inode])
    elif so_pattern.match(prog):
        debug(DEBUG_SPAM, prog, "is a library")
    elif open(prog).read(4)[1:] == 'ELF':
        objects[inode] = prog
    else:
        debug(DEBUG_SPAM, prog, "is no ELF")

if not ldlib:
    for obj in objects.values():
        output = command("mklibs-readelf", "-i", obj)
	for x in output:
            ldlib = x
	if ldlib:
	    break

if not ldlib:
    sys.exit("E: Dynamic linker not found, aborting.")

debug(DEBUG_NORMAL, "I: Using", ldlib, "as dynamic linker.")

# Check for rpaths
for obj in objects.values():
    rpath_val = rpath(obj)
    if rpath_val:
        if root:
            for rpath_elem in rpath_val:
                if not rpath_elem in lib_rpath:
                    if debuglevel >= DEBUG_VERBOSE:
                        print "Adding rpath " + rpath_elem + " for " + obj
                    lib_rpath.append(rpath_elem)
        else:
            print "warning: " + obj + " may need rpath, but --root not specified"

lib_path.extend(lib_rpath)

passnr = 1
available_libs = []
previous_pass_libraries = set()
while 1:
    debug(DEBUG_NORMAL, "I: library reduction pass", `passnr`)
    if debuglevel >= DEBUG_VERBOSE:
        print "Objects:",
        for obj in objects.values():
            print obj[obj.rfind('/') + 1:],
        print

    passnr = passnr + 1
    # Gather all already reduced libraries and treat them as objects as well
    small_libs = []
    for lib in regexpfilter(os.listdir(dest_path), "(.*-so-stripped)$"):
        obj = dest_path + "/" + lib
        small_libs.append(obj)
        inode = os.stat(obj)[ST_INO]
        if objects.has_key(inode):
            debug(DEBUG_SPAM, obj, "is hardlink to", objects[inode])
        else:
            objects[inode] = obj

    # DEBUG
    for obj in objects.values():
        small_libs.append(obj)
        debug(DEBUG_VERBOSE, "Object:", obj)

    libraries = set()
    for obj in objects.values():
        libraries.update(library_depends(obj))

    if libraries == previous_pass_libraries:
        # No progress in last pass.
        break

    previous_pass_libraries = libraries

    # WORKAROUND: Always add libgcc on old-abi arm
    header = elf_header(find_lib(libraries.copy().pop()))
    if header['machine'] == 40 and header['flags'] & 0xff000000 == 0:
        libraries.add('libgcc_s.so.1')  

    # reduce libraries
    for library in libraries:
        so_file = find_lib(library)
        if root and (re.compile("^" + root).search(so_file)):
            debug(DEBUG_VERBOSE, "no action required for " + so_file)
            if not so_file in available_libs:
                debug(DEBUG_VERBOSE, "adding " + so_file + " to available libs")
                available_libs.append(so_file)
            continue
        so_file_name = os.path.basename(so_file)
        if not so_file:
            sys.exit("File not found:" + library)
        command("cp", so_file, dest_path + "/" + so_file_name + "-so-stripped")

# Finalising libs and cleaning up
for lib in regexpfilter(os.listdir(dest_path), "(.*)-so-stripped$"):
    os.rename(dest_path + "/" + lib + "-so-stripped", dest_path + "/" + lib)
for lib in regexpfilter(os.listdir(dest_path), "(.*-so)$"):
    os.remove(dest_path + "/" + lib)

# Make sure the dynamic linker is present and is executable
ld_file = find_lib(ldlib)
ld_file_name = os.path.basename(ld_file)

if not os.access(dest_path + "/" + ld_file_name, os.F_OK):
    debug(DEBUG_NORMAL, "I: stripping and copying dynamic linker.")
    command(target + "objcopy", "--strip-unneeded -R .note -R .comment",
            ld_file, dest_path + "/" + ld_file_name)

os.chmod(dest_path + "/" + ld_file_name, 0755)

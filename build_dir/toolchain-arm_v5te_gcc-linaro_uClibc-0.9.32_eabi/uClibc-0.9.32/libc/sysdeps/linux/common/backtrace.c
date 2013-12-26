/* backtrace.c for the generic platform.
 * 
 * Copyright (C) 2007 Eivind Naess <enaess@watchguard.com>,
 *                    Watchguard Technologies, Inc. http://www.watchguard.com
 *
 * The original file was a part of the GNU C Library licensed under the LGPL
 * Copyright (C) 1998, 2000, 2002, 2004 Free Software Foundation, Inc. 
 * Contributed by Ulrich Drepper <drepper@cygnus.com>, 1998.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Library General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <execinfo.h>

int __backtrace (void **array, int size)
{   
    /*
     * Not implemented for common, architecture specific.
     */
    return -1;
}

link_warning(__backtrace, "the `backtrace()' function is a stub.  Do you really need it?")
weak_alias(__backtrace, backtrace)



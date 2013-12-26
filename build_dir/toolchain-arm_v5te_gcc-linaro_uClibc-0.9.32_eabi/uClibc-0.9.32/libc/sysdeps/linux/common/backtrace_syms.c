/* backtrace.c for the generic platform.
 * 
 * Copyright (C) 2007 Eivind Naess <enaess@watchguard.com>,
 *                    Watchguard Technologies, Inc. http://www.watchguard.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at your
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


char **__backtrace_symbols( void *__const *__array, int __size )
{   
    return (char**)0;
}

link_warning(__backtrace_symbols, "the `backtrace_symbols()' function is a stub.  Do you really need it?")
weak_alias(__backtrace_symbols, backtrace_symbols)




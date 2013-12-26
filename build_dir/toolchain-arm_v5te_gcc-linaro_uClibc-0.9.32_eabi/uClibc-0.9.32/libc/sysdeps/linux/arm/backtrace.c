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
#include "frame.h"

/*
 * Secretly exported by uClibc or glibc.
 */
extern void *__libc_stack_end;


/* 
 * Get some notion of the current stack.  Need not be exactly the top
 *    of the stack, just something somewhere in the current frame.  
 */
#ifndef CURRENT_STACK_FRAME
#define CURRENT_STACK_FRAME  ({ char __csf; &__csf; })
#endif


/* 
 * By default we assume that the stack grows downward.  
 */
#ifndef INNER_THAN
#define INNER_THAN <
#endif


/* 
 * By default assume the `next' pointer in struct layout points to the
 *    next struct layout.  
 */
#ifndef ADVANCE_STACK_FRAME
#define ADVANCE_STACK_FRAME(next) (struct layout *) ( next )
#endif


/* 
 * By default, the frame pointer is just what we get from gcc.  
 */
#ifndef FIRST_FRAME_POINTER
#define FIRST_FRAME_POINTER  __builtin_frame_address (0)
#endif

#ifndef DEBUG_PRINT_FRAME
#define DEBUG_PRINT_FRAME(frame) {}
#endif

/*
 * This implementation of backtrace walks the stack-frames, reads the return address and 
 * frame pointer of next frame. 
 *
 * There is arguably a better way of getting around this by using GCC's __Unwind_Backtrace, 
 * but my preliminary investigation found that it returned after calling the provided 
 * function callback only once. For those of you who are interested in this, there is an 
 * excellent library called libunwind (www.hpl.hp.com/research/linux/libunwind/) that gets 
 * you a stacktrace the same way (stripped < ~40K).
 *
 * Do not compile with -fomit-frame-pointer, this will cause the compiler to possibly 
 * remove the stack frames we are walking. 
 */

int __backtrace (void **array, int size)
{
    struct layout *current = (struct layout*)0;
    struct layout *next    = (struct layout*)0;

    void *top_frame;
    void *top_stack;

    int cnt = 0;
    top_frame = FIRST_FRAME_POINTER;
    top_stack = CURRENT_STACK_FRAME;

    current = (struct layout*) top_frame;

#ifdef DEBUG
    printf("top_frame: 0x%08lx, top_stack: 0x%08lx\n", top_frame, top_stack);
    DEBUG_PRINT_FRAME(current);
#endif

    /* 
     * We skip the call to this function, it makes no sense to record it.
     */
    while ( cnt < size ) {

        /* Make sure we don't cross the upper bound */
        if (!current || (void *) current INNER_THAN top_stack || 
            !((void *) current INNER_THAN __libc_stack_end)) {

            break;
        }

        next = ADVANCE_STACK_FRAME(current->next);

        /* Make sure next is valid and we are not in a loop */
        if ( !next || next == current )
            break;

        /* Make sure we are moving in the right direction */
        if ( next INNER_THAN current )
            break;
        
        /* Don't record (NULL) addresses */
        if ( !current->return_address )
            break;

#ifdef DEBUG
        DEBUG_PRINT_FRAME(current);
#endif
        array[cnt++] = current->return_address;
        current = next;
    }

    return cnt;
}

weak_alias(__backtrace, backtrace)



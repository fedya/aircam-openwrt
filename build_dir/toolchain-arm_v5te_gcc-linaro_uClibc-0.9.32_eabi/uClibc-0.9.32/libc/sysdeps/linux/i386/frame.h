/* frame.h for generating stack traces on a i386/i686 architecture.
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

/* 
 * The Stack Frames on an i686/i386 consists of two values, EBP (Frame Pointer) and EIP (Instruction 
 * Pointer). The latter is the return address.
 *
 *          +-----------------+        +-----------------+
 *  %ebp -> | %ebp last frame-|------> | %ebp last frame-|->...
 *          |                 |        |                 |
 *          | return address  |        | return address  |
 *          +-----------------+        +-----------------+
 */
struct layout {

    struct layout *next;        /*! Holds the previous value of EBP (Frame Pointer)         */
    void *return_address;       /*! Holds the previous value of EIP (Return Address)        */
};
 
#define FIRST_FRAME_POINTER       (__builtin_frame_address(0))

#define ADVANCE_STACK_FRAME(next) (struct layout *)((int)next)

#define DEBUG_PRINT_FRAME(frame) \
    printf("EBP: 0x%08lx, EIP: 0x%08lx\n", frame->next, frame->return_address)


/* frame.h for generating stack traces on an ARM architecture.
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
 * There are 4 registers pushed onto the stack if the APCS (ARM Procedure Calling Standard) compatible 
 * frames are enabled, FP (Frame Pointer), SP (Stack Pointer), LR (Link Register), and 
 * PC (Program Counter).
 *
 * LR contains the abosulute return address to where the current frame was called from, the PC holds the
 * address of the function this frame was called from. The SP holds the previous top of stack. The FP holds
 * the address of the previous frame.
 *
 * If you use optimization then, you will need to enable -mapcs or -mapcs-frame which ensures that 
 * each function have a function prologue that pushes FP, SP, IP, and PC onto the stack. Please consult
 * your toolchain options and processor handbook.
 *
 * This was tested on a Intel IXP425 platform with an Big-Endian ARM (XScale) processor). Configured for 
 * CONFIG_GENERIC_ARM=y.
 */

struct layout {

    struct layout *next;        /*! This is the FP pointing to the previous frame           */
    void *sp;                   /*! This is the SP pointing to the top of previous frame    */
    void *return_address;       /*! This is the RA pointing to the location to return       */
    void *pc;                   /*! This is the PC pointing to the caller function          */
};


#define FIRST_FRAME_POINTER       (__builtin_frame_address(0) - 12)

#define ADVANCE_STACK_FRAME(next) (struct layout *) ((int)next - 12)

#define DEBUG_PRINT_FRAME(frame) \
    printf("FP: 0x%08lx, IP: 0x%08lx, LR: 0x%08lx, PC: 0x%08lx\n", frame->next, \
           frame->sp, frame->return_address, frame->pc)




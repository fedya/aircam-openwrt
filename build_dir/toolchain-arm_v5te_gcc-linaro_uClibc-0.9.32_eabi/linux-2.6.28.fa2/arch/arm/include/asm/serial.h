/*
 *  arch/arm/include/asm/serial.h
 *
 *  Copyright (C) 1996 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   15-10-1996	RMK	Created
 *   04-25-2005 Luke Lee   Bug fixed.
 *   09-15-2005 Luke Lee   Platform dependent implementation for Faraday
 *                         processors.
 */

#ifndef __ASM_SERIAL_H
#define __ASM_SERIAL_H

#include <asm/mach-types.h>

#if defined(CONFIG_ARCH_FARADAY)
#include <mach/serial.h>
#endif

#if defined(CONFIG_ARCH_GM)
#include <mach/serial.h>
#endif

#ifndef BASE_BAUD
#define BASE_BAUD	(1843200 / 16)
#endif

#endif

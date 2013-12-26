/*
 *  arch/arm/mach-faraday/include/mach/spec.h
 *  
 *  Faraday Platform Independent Specification
 *  
 *  Copyright (C) 2005 Faraday Corp. (http://www.faraday-tech.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 *  ChangeLog
 *  
 *   Luke Lee 09/14/2005 Created.
 *            09/15/2005 Completed.
 */

#ifndef __FARADAY_PLATFORM_INDEPENDENT_SPECIFICATION__
#define __FARADAY_PLATFORM_INDEPENDENT_SPECIFICATION__

#include <asm/glue.h>

/*
 * Platform dependent specification
 */
#include <mach/platform/spec.h>       /* Automatic generated spec file */
#include <mach/platform/misc_spec.h>  /* Manual defined spec */

/*
 * Platform independent specification
 */

#define NR_IRQS  PLATFORM_INTERRUPTS

#ifndef BOOT_PARAMETER_PA_BASE
#define BOOT_PARAMETER_PA_BASE    CPU_MEM_PA_BASE
#endif

#if !(defined(PLATFORM_BOOTTIME_IO_PA_BASE) && defined(PLATFORM_BOOTTIME_IO_VA_BASE))
#error Missing symbol PLATFORM_BOOTTIME_IO_PA_BASE or PLATFORM_BOOTTIME_IO_VA_BASE.
#endif

/*
 * Macros for retrieving IP related information
 */
#define IP_IDENTIFIER  __glue(__glue(IPMODULE,_),__glue(IPNAME,_))

#define IP_COUNT       __glue(IP_IDENTIFIER,COUNT)

#define IP_IRQ_COUNT   __glue(IP_IDENTIFIER,IRQ_COUNT)
#define IP_IRQ(n)      __glue(__glue(IP_IDENTIFIER,n),_IRQ)
#define IP_irq         __glue(IP_IDENTIFIER,irq)

#define IP_PA_COUNT    __glue(IP_IDENTIFIER,PA_COUNT)
#define IP_PA_BASE(n)  __glue(__glue(IP_IDENTIFIER,n),_PA_BASE)
#define IP_PA_LIMIT(n) __glue(__glue(IP_IDENTIFIER,n),_PA_LIMIT)
#define IP_PA_SIZE(n)  __glue(__glue(IP_IDENTIFIER,n),_PA_SIZE)
#define IP_pa_base     __glue(IP_IDENTIFIER,pa_base)
#define IP_pa_limit    __glue(IP_IDENTIFIER,pa_limit)
#define IP_pa_size     __glue(IP_IDENTIFIER,pa_size)

#define IP_VA_COUNT    __glue(IP_IDENTIFIER,VA_COUNT)
#define IP_VA_BASE(n)  __glue(__glue(IP_IDENTIFIER,n),_VA_BASE)
#define IP_VA_LIMIT(n) __glue(__glue(IP_IDENTIFIER,n),_VA_LIMIT)
#define IP_VA_SIZE(n)  __glue(__glue(IP_IDENTIFIER,n),_VA_SIZE)
#define IP_va_base     __glue(IP_IDENTIFIER,va_base)
#define IP_va_limit    __glue(IP_IDENTIFIER,va_limit)
#define IP_va_size     __glue(IP_IDENTIFIER,va_size)

/*
 * Facility macros
 */
/* IRQ0~7 */
#define IP_IRQ0(n)     __glue(__glue(IP_IDENTIFIER,n),_IRQ0)
#define IP_IRQ1(n)     __glue(__glue(IP_IDENTIFIER,n),_IRQ1)
#define IP_IRQ2(n)     __glue(__glue(IP_IDENTIFIER,n),_IRQ2)
#define IP_IRQ3(n)     __glue(__glue(IP_IDENTIFIER,n),_IRQ3)
#define IP_IRQ4(n)     __glue(__glue(IP_IDENTIFIER,n),_IRQ4)
#define IP_IRQ5(n)     __glue(__glue(IP_IDENTIFIER,n),_IRQ5)
#define IP_IRQ6(n)     __glue(__glue(IP_IDENTIFIER,n),_IRQ6)
#define IP_IRQ7(n)     __glue(__glue(IP_IDENTIFIER,n),_IRQ7)

/* PA_BASE0~7 */
#define IP_PA_BASE0(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_BASE0)
#define IP_PA_BASE1(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_BASE1)
#define IP_PA_BASE2(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_BASE2)
#define IP_PA_BASE3(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_BASE3)
#define IP_PA_BASE4(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_BASE4)
#define IP_PA_BASE5(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_BASE5)
#define IP_PA_BASE6(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_BASE6)
#define IP_PA_BASE7(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_BASE7)

/* PA_LIMIT0~7 */
#define IP_PA_LIMIT0(n)    __glue(__glue(IP_IDENTIFIER,n),_PA_LIMIT0)
#define IP_PA_LIMIT1(n)    __glue(__glue(IP_IDENTIFIER,n),_PA_LIMIT1)
#define IP_PA_LIMIT2(n)    __glue(__glue(IP_IDENTIFIER,n),_PA_LIMIT2)
#define IP_PA_LIMIT3(n)    __glue(__glue(IP_IDENTIFIER,n),_PA_LIMIT3)
#define IP_PA_LIMIT4(n)    __glue(__glue(IP_IDENTIFIER,n),_PA_LIMIT4)
#define IP_PA_LIMIT5(n)    __glue(__glue(IP_IDENTIFIER,n),_PA_LIMIT5)
#define IP_PA_LIMIT6(n)    __glue(__glue(IP_IDENTIFIER,n),_PA_LIMIT6)
#define IP_PA_LIMIT7(n)    __glue(__glue(IP_IDENTIFIER,n),_PA_LIMIT7)

/* PA_SIZE0~7 */
#define IP_PA_SIZE0(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_SIZE0)
#define IP_PA_SIZE1(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_SIZE1)
#define IP_PA_SIZE2(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_SIZE2)
#define IP_PA_SIZE3(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_SIZE3)
#define IP_PA_SIZE4(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_SIZE4)
#define IP_PA_SIZE5(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_SIZE5)
#define IP_PA_SIZE6(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_SIZE6)
#define IP_PA_SIZE7(n)     __glue(__glue(IP_IDENTIFIER,n),_PA_SIZE7)

/* VA_BASE0~7 */
#define IP_VA_BASE0(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_BASE0)
#define IP_VA_BASE1(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_BASE1)
#define IP_VA_BASE2(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_BASE2)
#define IP_VA_BASE3(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_BASE3)
#define IP_VA_BASE4(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_BASE4)
#define IP_VA_BASE5(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_BASE5)
#define IP_VA_BASE6(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_BASE6)
#define IP_VA_BASE7(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_BASE7)

/* VA_LIMIT0~7 */
#define IP_VA_LIMIT0(n)    __glue(__glue(IP_IDENTIFIER,n),_VA_LIMIT0)
#define IP_VA_LIMIT1(n)    __glue(__glue(IP_IDENTIFIER,n),_VA_LIMIT1)
#define IP_VA_LIMIT2(n)    __glue(__glue(IP_IDENTIFIER,n),_VA_LIMIT2)
#define IP_VA_LIMIT3(n)    __glue(__glue(IP_IDENTIFIER,n),_VA_LIMIT3)
#define IP_VA_LIMIT4(n)    __glue(__glue(IP_IDENTIFIER,n),_VA_LIMIT4)
#define IP_VA_LIMIT5(n)    __glue(__glue(IP_IDENTIFIER,n),_VA_LIMIT5)
#define IP_VA_LIMIT6(n)    __glue(__glue(IP_IDENTIFIER,n),_VA_LIMIT6)
#define IP_VA_LIMIT7(n)    __glue(__glue(IP_IDENTIFIER,n),_VA_LIMIT7)

/* VA_SIZE0~7 */
#define IP_VA_SIZE0(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_SIZE0)
#define IP_VA_SIZE1(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_SIZE1)
#define IP_VA_SIZE2(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_SIZE2)
#define IP_VA_SIZE3(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_SIZE3)
#define IP_VA_SIZE4(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_SIZE4)
#define IP_VA_SIZE5(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_SIZE5)
#define IP_VA_SIZE6(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_SIZE6)
#define IP_VA_SIZE7(n)     __glue(__glue(IP_IDENTIFIER,n),_VA_SIZE7)

#endif /* __FARADAY_PLATFORM_INDEPENDENT_SPECIFICATION__ */

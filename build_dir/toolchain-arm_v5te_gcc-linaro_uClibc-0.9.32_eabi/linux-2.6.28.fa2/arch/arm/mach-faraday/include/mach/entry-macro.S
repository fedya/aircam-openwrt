/*
 * arch/arm/mach-faraday/include/mach/entry-macro.S
 *
 * Faraday Low-level Interrupt Vector Interface Macros
 *
 * Copyright (C) 2005 Faraday Corp. (http://www.faraday-tech.com)
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
 */

#ifndef CONFIG_CPU_FMP626

#include <mach/spec.h>
#include <mach/ftintc010.h>

	.macro	disable_fiq
	.endm

	.macro	get_irqnr_preamble, base, tmp
	ldr	\base, =ftintc010_base_addr
	ldr	\base, [\base]
	.endm

	.macro	arch_ret_to_user, tmp1, tmp2
	.endm

#if __LINUX_ARM_ARCH__ >= 5
	.macro	__get_int_nr_and_base, irqnr, irqstat, tmp
	clz	\irqnr, \irqstat
	rsb	\irqnr, \irqnr, #31
	.endm
#else
	/*
	 * An O(1) optimized version for getting IRQ/FIQ number
	 * 08/29/2005 Luke Lee
	 * Input/output: irqnr (initial value), irqstat (the word to scan)
	 * Local R/W:	tmp
	 */
	.macro	__get_int_nr_and_base, irqnr, irqstat, tmp
	mov	\tmp, \irqstat, lsl #16	/* check if lower 16 bits = zero */
	cmp	\tmp, #0
	movne	\irqstat, \irqstat, lsl #16	/* irqstat <<= 16 */
	subne	\irqnr, \irqnr, #16	/* irqnr -= 16 */
	tst	\irqstat, #0x00FF0000
	movne	\irqstat, \irqstat, lsl #8	/* irqstat <<= 8 */
	subne	\irqnr, \irqnr, #8	/* irqnr -= 8 */
	tst	\irqstat, #0x0F000000
	movne	\irqstat, \irqstat, lsl #4	/* irqstat <<= 4 */
	subne	\irqnr, \irqnr, #4	/* irqnr -= 4 */
	tst	\irqstat, #0x30000000
	movne	\irqstat, \irqstat, lsl #2	/* irqstat <<= 2 */
	subne	\irqnr, \irqnr, #2	/* irqnr -= 2 */
	tst	\irqstat, #0x40000000
	subne	\irqnr, \irqnr, #1	/* irqnr -= 1 */
	.endm
#endif

	/*
	 * Get IRQ number and base
	 * Input: base
	 * Output: irqnr, Z flag
	 * Local R/W: irqstat, tmp
	 */
	.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp

	ldr	\irqstat, [\base, #FTINTC010_OFFSET_IRQSTATUS]
	cmp	\irqstat, #0
	beq	3f

	mov	\irqnr, #31

	__get_int_nr_and_base	\irqnr, \irqstat, \tmp

	cmp	\irqnr, #NR_IRQS
#ifdef CONFIG_FTINTC010EX
	b	5f
#endif
3:
#ifdef CONFIG_FTINTC010EX
	ldr	\irqstat, [\base, #FTINTC010_OFFSET_IRQSTATUSEX]
	cmp	\irqstat, #0
	beq	5f
	mov	\irqnr, #31

	__get_int_nr_and_base	\irqnr, \irqstat, \tmp

	add	\irqnr, \irqnr, #32

	cmp	\irqnr, #NR_IRQS
5:
#endif
	.endm	/* get_irqnr_and_base */

#else	/* CONFIG_CPU_FMP626 */

#include <mach/hardware.h>
#include <asm/hardware/gic.h>

	.macro	disable_fiq
	.endm

	.macro	get_irqnr_preamble, base, tmp
	ldr	\base, =gic_cpu_base_addr
	ldr	\base, [\base]
	.endm

	.macro	arch_ret_to_user, tmp1, tmp2
	.endm

	/*
	 * The interrupt numbering scheme is defined in the
	 * interrupt controller spec.  To wit:
	 *
	 * Interrupts 0-15 are IPI
	 * 16-28 are reserved
	 * 29-31 are local.  We allow 30 to be used for the watchdog.
	 * 32-1020 are global
	 * 1021-1022 are reserved
	 * 1023 is "spurious" (no interrupt)
	 *
	 * For now, we ignore all local interrupts so only return an interrupt if it's
	 * between 30 and 1020.  The test_for_ipi routine below will pick up on IPIs.
	 *
	 * A simple read from the controller will tell us the number of the highest
	 * priority enabled interrupt.  We then just need to check whether it is in the
	 * valid range for an IRQ (30-1020 inclusive).
	 */

	.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp

	ldr	\irqstat, [\base, #GIC_CPU_INTACK] /* bits 12-10 = src CPU, 9-0 = int # */

	ldr	\tmp, =1021

	bic	\irqnr, \irqstat, #0x1c00

	cmp	\irqnr, #29
	cmpcc	\irqnr, \irqnr
	cmpne	\irqnr, \tmp
	cmpcs	\irqnr, \irqnr

	.endm

	/* We assume that irqstat (the raw value of the IRQ acknowledge
	 * register) is preserved from the macro above.
	 * If there is an IPI, we immediately signal end of interrupt on the
	 * controller, since this requires the original irqstat value which
	 * we won't easily be able to recreate later.
	 */

	.macro	test_for_ipi, irqnr, irqstat, base, tmp
	bic	\irqnr, \irqstat, #0x1c00
	cmp	\irqnr, #16
	strcc	\irqstat, [\base, #GIC_CPU_EOI]
	cmpcs	\irqnr, \irqnr
	.endm

	/* As above, this assumes that irqstat and base are preserved.. */

	.macro	test_for_ltirq, irqnr, irqstat, base, tmp
	bic	\irqnr, \irqstat, #0x1c00
	mov 	\tmp, #0
	cmp	\irqnr, #29
	moveq	\tmp, #1
	streq	\irqstat, [\base, #GIC_CPU_EOI]
	cmp	\tmp, #0
	.endm

#endif	/* CONFIG_CPU_FMP626 */

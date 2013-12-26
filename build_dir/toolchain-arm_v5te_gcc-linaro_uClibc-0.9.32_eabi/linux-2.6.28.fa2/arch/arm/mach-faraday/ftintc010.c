/*
 *  linux/arch/arm/mach-faraday/ftintc010.c
 *
 *  Faraday FTINTC010 Interrupt Controller
 *
 *  Copyright (C) 2009 Po-Yu Chuang
 *  Copyright (C) 2009 Faraday Corp. (http://www.faraday-tech.com)
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

#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <asm/io.h>

#include <asm/mach/irq.h>
#include <mach/ftintc010.h>

struct ftintc010_chip_data {
	unsigned int irq_offset;
	void __iomem *base;
};

#ifndef MAX_FTINTC010_NR
#define MAX_FTINTC010_NR	1
#endif

static struct ftintc010_chip_data ftintc010_data[MAX_FTINTC010_NR];
static DEFINE_SPINLOCK(ftintc010_lock);

/******************************************************************************
 * internal functions
 *****************************************************************************/
static inline void __iomem *ftintc010_base(unsigned int irq)
{
	struct ftintc010_chip_data *chip_data = get_irq_chip_data(irq);
	return chip_data->base;
}

/*
 * return hardware irq number
 */
static inline unsigned int ftintc010_irq(unsigned int irq)
{
	struct ftintc010_chip_data *chip_data = get_irq_chip_data(irq);
	return irq - chip_data->irq_offset;
}

#ifdef CONFIG_FTINTC010EX
static inline void ftintc010_clear_irq(void __iomem *base, unsigned int hw_irq)
{
	unsigned int mask;

	if (hw_irq < 32) {
		mask = 1 << hw_irq;

		writel(mask, base + FTINTC010_OFFSET_IRQCLEAR);
	} else {
		mask = 1 << (hw_irq - 32);
		writel(mask, base + FTINTC010_OFFSET_IRQCLEAREX);
	}
}

static inline void ftintc010_mask_irq(void __iomem *base, unsigned int hw_irq)
{
	unsigned int mask;

	/*
	 * 0: masked
	 * 1: unmasked
	 */
	if (hw_irq < 32) {
		mask = readl(base + FTINTC010_OFFSET_IRQMASK);
		mask &= ~(1 << hw_irq);
		writel(mask, base + FTINTC010_OFFSET_IRQMASK);
	} else {
		mask = readl(base + FTINTC010_OFFSET_IRQMASKEX);
		mask &= ~(1 << (hw_irq - 32));
		writel(mask, base + FTINTC010_OFFSET_IRQMASKEX);
	}
}

static inline void ftintc010_unmask_irq(void __iomem *base, unsigned int hw_irq)
{
	unsigned int mask;

	/*
	 * 0: masked
	 * 1: unmasked
	 */
	if (hw_irq < 32) {
		mask = readl(base + FTINTC010_OFFSET_IRQMASK);
		mask |= 1 << hw_irq;
		writel(mask, base + FTINTC010_OFFSET_IRQMASK);
	} else {
		mask = readl(base + FTINTC010_OFFSET_IRQMASKEX);
		mask |= 1 << (hw_irq - 32);
		writel(mask, base + FTINTC010_OFFSET_IRQMASKEX);
	}
}
#else	/* CONFIG_FTINTC010EX */
static inline void ftintc010_clear_irq(void __iomem *base, unsigned int hw_irq)
{
	unsigned int mask = 1 << hw_irq;

	writel(mask, base + FTINTC010_OFFSET_IRQCLEAR);
}

static inline void ftintc010_mask_irq(void __iomem *base, unsigned int hw_irq)
{
	unsigned int mask;

	/*
	 * 0: masked
	 * 1: unmasked
	 */
	mask = readl(base + FTINTC010_OFFSET_IRQMASK);
	mask &= ~(1 << hw_irq);
	writel(mask, base + FTINTC010_OFFSET_IRQMASK);
}

static inline void ftintc010_unmask_irq(void __iomem *base, unsigned int hw_irq)
{
	unsigned int mask;

	/*
	 * 0: masked
	 * 1: unmasked
	 */
	mask = readl(base + FTINTC010_OFFSET_IRQMASK);
	mask |= 1 << hw_irq;
	writel(mask, base + FTINTC010_OFFSET_IRQMASK);
}
#endif	/* CONFIG_FTINTC010EX */

/******************************************************************************
 * struct irq_chip functions
 *****************************************************************************/

/*
 * Edge trigger IRQ chip methods
 */
static void ftintc010_edge_ack(unsigned int irq)
{
	unsigned int hw_irq = ftintc010_irq(irq);
	void __iomem *base = ftintc010_base(irq);

	spin_lock(&ftintc010_lock);
	ftintc010_clear_irq(base, hw_irq);
	spin_unlock(&ftintc010_lock);
}

static void ftintc010_mask(unsigned int irq)
{
	unsigned int hw_irq = ftintc010_irq(irq);
	void __iomem *base = ftintc010_base(irq);

	spin_lock(&ftintc010_lock);
	ftintc010_mask_irq(base, hw_irq);
	spin_unlock(&ftintc010_lock);
}

static void ftintc010_unmask(unsigned int irq)
{
	unsigned int hw_irq = ftintc010_irq(irq);
	void __iomem *base = ftintc010_base(irq);

	spin_lock(&ftintc010_lock);
	ftintc010_unmask_irq(base, hw_irq);
	spin_unlock(&ftintc010_lock);
}

static struct irq_chip ftintc010_edge_chip = {
	.ack	= ftintc010_edge_ack,
	.mask	= ftintc010_mask,
	.unmask	= ftintc010_unmask,
};

/*
 * Level trigger IRQ chip methods
 */
static void ftintc010_level_ack(unsigned int irq)
{
	/* do nothing */
}

static struct irq_chip ftintc010_level_chip = {
	.ack	= ftintc010_level_ack,
	.mask	= ftintc010_mask,
	.unmask	= ftintc010_unmask,
};

/******************************************************************************
 * initialization functions
 *****************************************************************************/
static void ftintc010_handle_cascade_irq(unsigned int irq, struct irq_desc *desc)
{
	struct ftintc010_chip_data *chip_data = get_irq_data(irq);
	struct irq_chip *chip = get_irq_chip(irq);
	unsigned int cascade_irq, hw_irq;
	unsigned long status;

	/* primary controller ack'ing */
	chip->mask(irq);
	chip->ack(irq);

	status = readl(chip_data->base + FTINTC010_OFFSET_IRQSTATUS);

#ifdef CONFIG_FTINTC010EX
	if (status) {
		hw_irq = ffs(status) - 1;
	} else {
		status = readl(chip_data->base + FTINTC010_OFFSET_IRQSTATUSEX);

		if (!status)
			goto out;

		hw_irq = ffs(status) - 1 + 32;
	}
#else
	if (!status)
		goto out;

	hw_irq = ffs(status) - 1;
#endif

	cascade_irq = hw_irq + chip_data->irq_offset;
	generic_handle_irq(cascade_irq);

out:
	/* primary controller unmasking */
	chip->unmask(irq);
}

void __init ftintc010_cascade_irq(unsigned int ftintc010_nr, unsigned int irq)
{
	if (ftintc010_nr >= MAX_FTINTC010_NR)
		BUG();
	if (set_irq_data(irq, &ftintc010_data[ftintc010_nr]) != 0)
		BUG();

	set_irq_chained_handler(irq, ftintc010_handle_cascade_irq);
}

/*
 * Initialization of master interrupt controller, after this INTC is
 * enabled, the rest of Linux initialization codes can then be completed.
 * For example, timer interrupts and UART interrupts must be enabled during
 * the boot process.
 */
void __init ftintc010_init(unsigned int ftintc010_nr, void __iomem *base,
			   unsigned int irq_start,
			   struct ftintc010_trigger_type *trigger_type)
{
	int irq;
	int i;

	if (ftintc010_nr >= MAX_FTINTC010_NR)
		BUG();

	ftintc010_data[ftintc010_nr].base = base;
	ftintc010_data[ftintc010_nr].irq_offset = irq_start;

	/*
	 * mask all interrupts
	 */
	writel(0, base + FTINTC010_OFFSET_IRQMASK);
	writel(0, base + FTINTC010_OFFSET_FIQMASK);
	writel(~0, base + FTINTC010_OFFSET_IRQCLEAR);
	writel(~0, base + FTINTC010_OFFSET_FIQCLEAR);
#ifdef CONFIG_FTINTC010EX
	writel(0, base + FTINTC010_OFFSET_IRQMASKEX);
	writel(~0, base + FTINTC010_OFFSET_IRQCLEAREX);
#endif
	/*
	 * setup trigger mode and level
	 */
	writel(trigger_type->irqmode, base + FTINTC010_OFFSET_IRQMODE);
	writel(trigger_type->irqlevel, base + FTINTC010_OFFSET_IRQLEVEL);
	writel(trigger_type->fiqmode, base + FTINTC010_OFFSET_FIQMODE);
	writel(trigger_type->fiqlevel, base + FTINTC010_OFFSET_FIQLEVEL);
#ifdef CONFIG_FTINTC010EX
	writel(trigger_type->irqmodeex, base + FTINTC010_OFFSET_IRQMODEEX);
	writel(trigger_type->irqlevelex, base + FTINTC010_OFFSET_IRQLEVELEX);
#endif

	/*
	 * setup the linux irq subsystem
	 */
	irq = irq_start; 
	for (i = 0; i < 32; i++) {
		if (trigger_type->irqmode & (1 << i)) {
			set_irq_chip(irq, &ftintc010_edge_chip);
			set_irq_handler(irq, handle_edge_irq);
		} else {
			set_irq_chip(irq, &ftintc010_level_chip);
			set_irq_handler(irq, handle_level_irq);
		}

		set_irq_chip_data(irq, &ftintc010_data[ftintc010_nr]);
		set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
		irq++;
	}

#ifdef CONFIG_FTINTC010EX
	for (i = 0; i < 32; i++) {
		if (trigger_type->irqmodeex & (1 << i)) {
			set_irq_chip(irq, &ftintc010_edge_chip);
			set_irq_handler(irq, handle_edge_irq);
		} else {
			set_irq_chip(irq, &ftintc010_level_chip);
			set_irq_handler(irq, handle_level_irq);
		}

		set_irq_chip_data(irq, &ftintc010_data[ftintc010_nr]);
		set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
		irq++;
	}
#endif
}

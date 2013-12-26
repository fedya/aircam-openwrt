/*
 *  arch/arm/mach-faraday/include/mach/ftscu000.h
 *
 *  Copyright (C) 2009 Faraday Technology.
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

#ifndef __FTSCU000_H
#define __FTSCU000_H

#define	FTSCU000_OFFSET_IIR		0x00
#define	FTSCU000_OFFSET_GCR		0x04
#define	FTSCU000_OFFSET_CCR		0x08
#define	FTSCU000_OFFSET_HCEG		0x0c
#define	FTSCU000_OFFSET_PCEG		0x10
#define	FTSCU000_OFFSET_CSR		0x14
#define	FTSCU000_OFFSET_IOMCR0		0x18
#define	FTSCU000_OFFSET_IOMCR1		0x1c
#define	FTSCU000_OFFSET_IOMCR2		0x20
#define	FTSCU000_OFFSET_IOMCR3		0x24
#define	FTSCU000_OFFSET_IOPCR0		0x28
#define	FTSCU000_OFFSET_IOPCR1		0x2c
#define	FTSCU000_OFFSET_CESCR		0x30

/*
 * Configuration Strap Register
 */
#define FTSCU000_CSR_BOOTMODE		(1 << 0)
#define FTSCU000_CSR_NANDDW		(1 << 1)
#define FTSCU000_CSR_NANDAC_3		(0x0 << 2)
#define FTSCU000_CSR_NANDAC_4		(0x1 << 2)
#define FTSCU000_CSR_NANDAC_5		(0x2 << 2)
#define FTSCU000_CSR_NANDPZ		(1 << 4)
#define FTSCU000_CSR_DBGEN		(1 << 5)
#define FTSCU000_CSR_DBGSOF		(1 << 6)
#define FTSCU000_CSR_HT			(1 << 9)

int ftscu000_in_half_turbo_mode(void __iomem *base);

#endif	/* __FTSCU000_H */

/*
 *  arch/arm/mach-mvzenith/include/mach/regs-irq.h
 *
 *  Copyright (C) 2010 MtekVision Co., Ltd.
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

#ifndef __ASM_ARCH_REGS_IRQ_H
#define __ASM_ARCH_REGS_IRQ_H

/*
 * Base Address : 0xD0000000
 */

/*
 * Register offset
 */
#define INT_IRQ_VECTOR		0x000
#define INT_FIQ_VECTOR		0x004
#define INT_RAW_STATUS		0x008
#define INT_MAP				0x010
#define INT_MASK_OFF		0x018
#define INT_MASK_ON			0x020
#define INT_MASK			0x030
#define INT_PRIORITY_MAP	0x100

#endif /* __ASM_ARCH_REGS_IRQ_H */

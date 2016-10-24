/*
 *  arch/arm/mach-mvformula/include/mach/vmalloc.h
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
#ifndef __ASM_ARCH_VMALLOC_H
#define __ASM_ARCH_VMALLOC_H

#if defined(CONFIG_VICTORIA_LPDDR2_256MB)
#define VMALLOC_END		(PAGE_OFFSET + 0x18000000)
#elif defined(CONFIG_VICTORIA_LPDDR2_512MB)
#define VMALLOC_END		(PAGE_OFFSET + 0x28000000)
#elif defined(CONFIG_VICTORIA_LPDDR2_128MB)
#define VMALLOC_END		(PAGE_OFFSET + 0x10000000)
#else
#define VMALLOC_END		(PAGE_OFFSET + 0x10000000)
#endif

#endif /* __ASM_ARCH_VMALLOC_H */

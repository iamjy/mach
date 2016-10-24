/*
 *  arch/arm/mach-mvfp7v2000tq/include/mach/map.h
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
#ifndef __ASM_ARCH_MAP_H
#define __ASM_ARCH_MAP_H

#include <mach/hardware.h>
#include <mach/io.h>

/*
 * Register Base Address
 */
#define INT_BASE		0x50000000	/* Interrupt */
#define UART0_BASE		0x50400000	/* UART 0 */
#define UART1_BASE		0x50410000	/* UART 1 */
#define UART2_BASE		0x50420000	/* UART 2 */
#define PWM_BASE		0x50500000	/* PWM Timer */
#define WDOG_BASE		0x50510000	/* Watchdog Timer */
#define SYS_BASE		0x50F00000	/* System Controller */

#define INT_VA_BASE		IO_ADDRESS(INT_BASE)
#define UART0_VA_BASE	IO_ADDRESS(UART0_BASE)
#define UART1_VA_BASE	IO_ADDRESS(UART1_BASE)
#define UART2_VA_BASE	IO_ADDRESS(UART2_BASE)
#define PWM_VA_BASE		IO_ADDRESS(PWM_BASE)
#define WDOG_VA_BASE	IO_ADDRESS(WDOG_BASE)
#define SYS_VA_BASE		IO_ADDRESS(SYS_BASE)

#endif /* __ASM_ARCH_MAP_H */

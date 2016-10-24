/*
 *  arch/arm/mach-mvzenith/include/mach/debug-macro.S
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
#include <mach/map.h>
#include <mach/regs-uart.h>

#if defined(CONFIG_MFP_7V2000T_Q_DEBUG_UART0)
#define DEBUG_UART_BASE		UART0_BASE
#define DEBUG_UART_VA_BASE	UART0_VA_BASE
#elif defined(CONFIG_MFP_7V2000T_Q_DEBUG_UART1)
#define DEBUG_UART_BASE		UART1_BASE
#define DEBUG_UART_VA_BASE	UART1_VA_BASE
#elif defined(CONFIG_MFP_7V2000T_Q_DEBUG_UART2)
#define DEBUG_UART_BASE		UART2_BASE
#define DEBUG_UART_VA_BASE	UART2_VA_BASE
#elif defined(CONFIG_MFP_7V2000T_Q_DEBUG_UART3)
#define DEBUG_UART_BASE		UART3_BASE
#define DEBUG_UART_VA_BASE	UART3_VA_BASE
#else
#error "Bad DEBUG_UART number"
#endif

	.macro	addruart, rp, rv, tmp
		ldr	\rp, =DEBUG_UART_BASE		@ phsycal base address
		ldr	\rv, =DEBUG_UART_VA_BASE	@ virtual base address
	.endm

	.macro	senduart, rd, rx
		str \rd, [\rx, #UART_TX_FIFO]	@ Write to Transmit Holding Register
	.endm

	.macro	busyuart, rd, rx
1001:		ldr	\rd, [\rx, #UART_FIFO_STATUS]	@ Read Line Status Register
		tst	\rd, #0x80							@ Holding & Shift registers empty?
		beq	1001b
	.endm

	.macro	waituart, rd, rx
1001:		ldr	\rd, [\rx, #UART_FIFO_STATUS]	@ Read Line Status Register
		tst	\rd, #0x80							@ Holding Register empty?
		beq	1001b
	.endm

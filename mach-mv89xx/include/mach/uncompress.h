/*
 *  arch/arm/mach-mvzenith/include/mach/uncompress.h
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
#ifndef __ASM_ARCH_UNCOMPRESS_H
#define __ASM_ARCH_UNCOMPRESS_H

#include <asm/io.h>

#include <mach/map.h>
#include <mach/regs-uart.h>

#if defined(CONFIG_VICTORIA_DEBUG_UART0)
#define DEBUG_UART_BASE		UART0_BASE
#define DEBUG_UART_VA_BASE	UART0_VA_BASE
#elif defined(CONFIG_VICTORIA_DEBUG_UART1)
#define DEBUG_UART_BASE		UART1_BASE
#define DEBUG_UART_VA_BASE	UART1_VA_BASE
#elif defined(CONFIG_VICTORIA_DEBUG_UART2)
#define DEBUG_UART_BASE		UART2_BASE
#define DEBUG_UART_VA_BASE	UART2_VA_BASE
#elif defined(CONFIG_VICTORIA_DEBUG_UART3)
#define DEBUG_UART_BASE		UART3_BASE
#define DEBUG_UART_VA_BASE	UART3_VA_BASE
#else
#error "Bad DEBUG_UART number"
#endif

#define TX_FIFO_CNT()	(readl(DEBUG_UART_BASE + UART_FIFO_STATUS) \
			& UART_FIFO_STATUS_TX_CNT_MASK)
#define PUSH_TX(c)	writeb(c, DEBUG_UART_BASE + UART_TX_FIFO)

static inline void putc(int c)
{
	while (TX_FIFO_CNT() == 0)
		barrier();

	PUSH_TX(c);
}

static inline void flush(void)
{
	while (TX_FIFO_CNT() < 0x40)
		barrier();
}

#define arch_decomp_setup()
#define arch_decomp_wdog()

#endif /* __ASM_ARCH_UNCOMPRESS_H */

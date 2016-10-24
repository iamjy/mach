/*
 *  arch/arm/mach-mvt12socfpga/include/mach/uart.h
 *
 *  Copyright (C) 2015 MtekVision Co., Ltd.
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
#ifndef __ASM_ARCH_UART_H
#define __ASM_ARCH_UART_H

#define UART_CFG_FIFO_SIZE_8BITS	0
#define UART_CFG_FIFO_SIZE_16BITS	1
#define UART_CFG_FIFO_SIZE_32BITS	2
#define UART_CFG_FIFO_SIZE_64BITS	3

#define UART_CFG_EVEN_PARITY		0
#define UART_CFG_ODD_PARITY			1
#define UART_CFG_NO_PARITY			2
#define UART_CFG_RESERVED_PARITY	3

#define UART_MODE_WORD_LEN_8BITS	0
#define UART_MODE_WORD_LEN_7BITS	1
#define UART_MODE_WORD_LEN_6BITS	2
#define UART_MODE_WORD_LEN_5BITS	3

struct mv_serial_platform_data {
	unsigned long membase;	/* ioremap cookie or NULL */
	unsigned long mapbase;	/* resource base */
	unsigned int irq;		/* interrupt number */
	unsigned int uartclk;	/* UART clock rate */
	int debug;
};

#endif /* __ASM_ARCH_UART_H */


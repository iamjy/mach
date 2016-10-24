/*
 *  arch/arm/mach-mvvictoria/include/mach/map.h
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
// #include <mach/memory.h>

/*
 * Register Base Address
 */
#define SYS_BASE		0xC0000000	/* System */
#define I2C0_BASE		0xC0011000	/* I2C 0 */
#define SPI0_BASE		0xC0012000	/* SPI 0 */
#define UART0_BASE		0xC0013000	/* UART 0 */
#define UART1_BASE		0xC0013000	/* UART 1 */
#define UART2_BASE		0xC0013000	/* UART 2 */
#define UART3_BASE		0xC0013000	/* UART 3 */
#define GPIO_BASE		0xC0014000	/* GPIO */
#define WDOG_BASE		0xC0015000	/* Watchdog Timer */
#define PWM_BASE		0xC0016000	/* PWM Timer */
#define RTC_BASE		0xC0019000	/* RTC */

#define INT_BASE		0xD0000000	/* Interrupt */
#define DMC_BASE		0xD0010000	/* DMC */
#define ISP0_BASE		0xD1000000
#define ISP1_BASE		0xD1060000
#define FPD_BASE		0xD00C0000
#define FD_BASE			0xD00D0000
#define S3C_BASE		0xD00E0000
#define CSS_BASE		0xD00F0000
#define VNR_BASE		0xD00F1000
#define MIPI0_CSI_BASE	0xD00F2000
#define MIPI1_CSI_BASE	0xD00F3000
#define MIPI2_CSI_BASE	0xD00F4000
#define CSS_I2C_BASE	0xD00F5000
#define JPEG_ENC_BASE	0xD00F6000
#define SCALER_BASE		0xD00F7000
#define MDIS_BASE		0xD00F8000
#define TUNER_BASE		0xD00F9000
#define ISP0_WRAP_BASE	0xD00FA000
#define ISP1_WRAP_BASE	0xD00FB000
#define GPV_BASE		0xD0100000

#define SYS_VA_BASE		IO_ADDRESS(SYS_BASE)
#define UART0_VA_BASE	IO_ADDRESS(UART0_BASE)
#define GPIO_VA_BASE	IO_ADDRESS(GPIO_BASE)
#define WDOG_VA_BASE	IO_ADDRESS(WDOG_BASE)
#define PWM_VA_BASE		IO_ADDRESS(PWM_BASE)
#define RTC_VA_BASE		IO_ADDRESS(RTC_BASE)
#define INT_VA_BASE		IO_ADDRESS(INT_BASE)
#define DMC_VA_BASE		IO_ADDRESS(DMC_BASE)

#endif /* __ASM_ARCH_MAP_H */


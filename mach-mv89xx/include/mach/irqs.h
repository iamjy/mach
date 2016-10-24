/*
 *  arch/arm/mach-mvzenith/include/mach/irq.h
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
#ifndef __ASM_ARCH_IRQS_H
#define __ASM_ARCH_IRQS_H

/*
 * IRQ numbers
 */
#define IRQ_UART0		0
#define IRQ_UNDEF1		1	//
#define IRQ_UNDEF2		2	//
#define IRQ_PWM0		3
#define IRQ_PWM1		4
#define IRQ_PWM2		5
#define IRQ_PWM3		6
#define IRQ_PWM4		7
#define IRQ_PWM5		8
#define IRQ_DMA			9
#define IRQ_UNDEF10		10	//
#define IRQ_I2C0		11
#define IRQ_UNDEF12		12	//
#define IRQ_I2C2		13
#define IRQ_UNDEF14		14	//
#define IRQ_UNDEF15		15	//
#define IRQ_UNDEF16		16	//
#define IRQ_UNDEF17		17	//
#define IRQ_UNDEF18		18	//
#define IRQ_UNDEF19		19	//
#define IRQ_UNDEF20		20	//
#define IRQ_UNDEF21		21	//
#define IRQ_UNDEF22		22	//
#define IRQ_UNDEF23		23	//
#define IRQ_UNDEF24		24	//
#define IRQ_UNDEF25		25	//
#define IRQ_UNDEF26		26	//
#define IRQ_UNDEF27		27	//
#define IRQ_UNDEF28		28	//
#define IRQ_UNDEF29		29	//
#define IRQ_UNDEF30		30	//
#define IRQ_UNDEF31		31	//

#define IRQ_WDOG		32
#define IRQ_RTC_TIC		33
#define IRQ_RTC_CALC	34
#define IRQ_RTC_ALARM	35
#define IRQ_GPIO		36
#define IRQ_SPI0		37
#define IRQ_UNDEF38		38	//
#define IRQ_UNDEF39		39	//
#define IRQ_UNDEF40		40	//
#define IRQ_UNDEF41		41	//
#define IRQ_I2C0		42
#define IRQ_I2C1		43
#define IRQ_I2C2		44
#define IRQ_I2C3		45
#define IRQ_FD			46
#define IRQ_FPD			47
#define IRQ_JENC		48
#define IRQ_FG			49
#define IRQ_MDIS		50
#define IRQ_S3D			51
#define IRQ_MIPI0		52
#define IRQ_MIPI1		53
#define IRQ_MIPI2		54
#define IRQ_CCP0		55
#define IRQ_VSYNC0		56
#define IRQ_LINE0		57
#define IRQ_CCP1		58
#define IRQ_VSYNC1		59
#define IRQ_LINE1		60
#define IRQ_VNR			61
#define IRQ_UNDEF62		62
#define NR_IRQS		   	63

struct pt_regs;

int mv_irq_get_state(int irq);
void mv_handle_irq(struct pt_regs *regs);

#endif /* __ASM_ARCH_IRQS_H */

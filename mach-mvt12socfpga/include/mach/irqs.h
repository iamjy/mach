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
#define IRQ_UART0		 0
#define IRQ_UART1		 1
#define IRQ_UART2		 2
#define IRQ_I2C0		 3
#define IRQ_I2C1		 4
#define IRQ_I2C2		 5
#define IRQ_SPI0		 6
#define IRQ_SPI1		 7
#define IRQ_I2S0		 8
#define IRQ_I2S1		 9
#define IRQ_GPIO		10
#define IRQ_PWN0		11
#define IRQ_PWN1		12
#define IRQ_PWN2		13
#define IRQ_PWN3		14
#define IRQ_PWN4		15
#define IRQ_PWN5		16
#define IRQ_WDT			17
#define IRQ_DMA			18
#define IRQ_USB			19
#define IRQ_UNDEF20		20
#define IRQ_UNDEF21		21
#define IRQ_UNDEF22		22
#define IRQ_UNDEF23		23
#define IRQ_UNDEF24		24
#define IRQ_UNDEF25		25
#define IRQ_UNDEF26		26
#define IRQ_UNDEF27		27
#define IRQ_UNDEF28		28
#define IRQ_UNDEF29		29
#define IRQ_UNDEF30		30
#define IRQ_FG_INTR		31
#define IRQ_OPR1		32
#define IRQ_OPR0		33
#define IRQ_HEVC_ING	34
#define IRQ_HEVC_INTR	35
#define IRQ_APEX		36
#define IRQ_DISP		37
#define IRQ_UNDEF38		38
#define IRQ_UNDEF39		39
#define IRQ_UNDEF40		40
#define IRQ_UNDEF41		41
#define IRQ_UNDEF42		42
#define IRQ_UNDEF43		43
#define IRQ_UNDEF44		44
#define IRQ_UNDEF45		45
#define IRQ_UNDEF46		46
#define IRQ_UNDEF47		47
#define IRQ_UNDEF48		48
#define IRQ_UNDEF49		49
#define IRQ_UNDEF50		50
#define IRQ_UNDEF51		51
#define IRQ_UNDEF52		52
#define IRQ_UNDEF53		53
#define IRQ_UNDEF54		54
#define IRQ_UNDEF55		55
#define IRQ_UNDEF56		56
#define IRQ_UNDEF57		57
#define IRQ_UNDEF58		58
#define IRQ_UNDEF59		59
#define IRQ_UNDEF60		60
#define IRQ_UNDEF61		61
#define IRQ_UNDEF62		62
#define NR_IRQS		   	63

struct pt_regs;

int mv_irq_get_state(int irq);
void mv_handle_irq(struct pt_regs *regs);

#endif /* __ASM_ARCH_IRQS_H */

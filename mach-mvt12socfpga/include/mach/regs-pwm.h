/*
 *  arch/arm/mach-mvt12socfpga/include/mach/regs-pwm.h
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
#ifndef __ASM_ARCH_REGS_PWM_H
#define __ASM_ARCH_REGS_PWM_H

/*
 * Base Address : 0x50500000
 */

/*
 * Register offset
 */
#define PWM_CFG0		0x00
#define PWM_CFG1		0x04
#define PWM_CFG2		0x08
#define PWM_CTRL		0x0C
#define PWM_TCMPB0		0x10
#define PWM_TCNTB0		0x14
#define PWM_TCNTO0		0x18
#define PWM_TCMPB1		0x1C
#define PWM_TCNTB1		0x20
#define PWM_TCNTO1		0x24
#define PWM_TCMPB2		0x28
#define PWM_TCNTB2		0x2C
#define PWM_TCNTO2		0x30
#define PWM_TCMPB3		0x34
#define PWM_TCNTB3		0x38
#define PWM_TCNTO3		0x3C
#define PWM_TCMPB4		0x40
#define PWM_TCNTB4		0x44
#define PWM_TCNTO4		0x48
#define PWM_TCMPB5		0x4C
#define PWM_TCNTB5		0x50
#define PWM_TCNTO5		0x54
#define PWM_INT_FLAG	0x58
#define PWM_INT_CLR		0x5C
#define PWM_INT_STATUS	0x60

/*
 * Field definitions
 */

/*
 * 0x00 : Configure 0
 */
#define PWM_CFG_DZ_LEN_SHIFT		24
#define PWM_CFG_DZ_LEN_WIDTH		0xFF
#define PWM_CFG_DZ_LEN_MASK		(PWM_CFG_DZ_LEN_WIDTH \
					<< PWM_CFG_DZ_LEN_SHIFT)
#define PWM_CFG_PRESCALER2_SHIFT	16
#define PWM_CFG_PRESCALER2_WIDTH	0xFF
#define PWM_CFG_PRESCALER2_MASK		(PWM_CFG_PRESCALER2_WIDTH \
					<< PWM_CFG_PRESCALER2_SHIFT)
#define PWM_CFG_PRESCALER1_SHIFT	8
#define PWM_CFG_PRESCALER1_WIDTH	0xFF
#define PWM_CFG_PRESCALER1_MASK		(PWM_CFG_PRESCALER1_WIDTH \
					<< PWM_CFG_PRESCALER1_SHIFT)
#define PWM_CFG_PRESCALER0_SHIFT	0
#define PWM_CFG_PRESCALER0_WIDTH	0xFF
#define PWM_CFG_PRESCALER0_MASK		(PWM_CFG_PRESCALER0_WIDTH \
					<< PWM_CFG_PRESCALER0_SHIFT)
/*
 * 0x04 : Configure 1
 */
#define PWM_CFG_PRESCALER5_SHIFT	16
#define PWM_CFG_PRESCALER5_WIDTH	0xFF
#define PWM_CFG_PRESCALER5_MASK		(PWM_CFG_PRESCALER5_WIDTH \
					<< PWM_CFG_PRESCALER5_SHIFT)
#define PWM_CFG_PRESCALER4_SHIFT	8
#define PWM_CFG_PRESCALER4_WIDTH	0xFF
#define PWM_CFG_PRESCALER4_MASK		(PWM_CFG_PRESCALER4_WIDTH \
					<< PWM_CFG_PRESCALER4_SHIFT)
#define PWM_CFG_PRESCALER3_SHIFT	0
#define PWM_CFG_PRESCALER3_WIDTH	0xFF
#define PWM_CFG_PRESCALER3_MASK		(PWM_CFG_PRESCALER3_WIDTH \
					<< PWM_CFG_PRESCALER3_SHIFT)

/*
 * 0x08 : Configure 2
 */
#define PWM_CFG_T5_CLK_SEL_SHIFT	20
#define PWM_CFG_T5_CLK_SEL_WIDTH	0x7
#define PWM_CFG_T5_CLK_SEL_MASK		(PWM_CFG_T5_CLK_SEL_WIDTH \
					<< PWM_CFG_T5_CLK_SEL_SHIFT)
#define PWM_CFG_T4_CLK_SEL_SHIFT	16
#define PWM_CFG_T4_CLK_SEL_WIDTH	0x7
#define PWM_CFG_T4_CLK_SEL_MASK		(PWM_CFG_T4_CLK_SEL_WIDTH \
					<< PWM_CFG_T4_CLK_SEL_SHIFT)
#define PWM_CFG_T3_CLK_SEL_SHIFT	12
#define PWM_CFG_T3_CLK_SEL_WIDTH	0x7
#define PWM_CFG_T3_CLK_SEL_MASK		(PWM_CFG_T3_CLK_SEL_WIDTH \
					<< PWM_CFG_T3_CLK_SEL_SHIFT)
#define PWM_CFG_T2_CLK_SEL_SHIFT	8
#define PWM_CFG_T2_CLK_SEL_WIDTH	0x7
#define PWM_CFG_T2_CLK_SEL_MASK		(PWM_CFG_T2_CLK_SEL_WIDTH \
					<< PWM_CFG_T2_CLK_SEL_SHIFT)
#define PWM_CFG_T1_CLK_SEL_SHIFT	4
#define PWM_CFG_T1_CLK_SEL_WIDTH	0x7
#define PWM_CFG_T1_CLK_SEL_MASK		(PWM_CFG_T1_CLK_SEL_WIDTH \
					<< PWM_CFG_T1_CLK_SEL_SHIFT)
#define PWM_CFG_T0_CLK_SEL_SHIFT	0
#define PWM_CFG_T0_CLK_SEL_WIDTH	0x7
#define PWM_CFG_T0_CLK_SEL_MASK		(PWM_CFG_T0_CLK_SEL_WIDTH \
					<< PWM_CFG_T0_CLK_SEL_SHIFT)

/*
 * 0x0C : Control
 */
#define PWM_CTRL_T5_AUTO_SHIFT		27
#define PWM_CTRL_T5_AUTO_WIDTH		0x1
#define PWM_CTRL_T5_AUTO_MASK		(PWM_CTRL_T5_AUTO_WIDTH \
					<< PWM_CTRL_T5_AUTO_SHIFT)
#define PWM_CTRL_T5_INV_SHIFT		26
#define PWM_CTRL_T5_INV_WIDTH		0x1
#define PWM_CTRL_T5_INV_MASK		(PWM_CTRL_T5_INV_WIDTH \
					<< PWM_CTRL_T5_INV_SHIFT)
#define PWM_CTRL_T5_UPDATE_SHIFT	25
#define PWM_CTRL_T5_UPDATE_WIDTH	0x1
#define PWM_CTRL_T5_UPDATE_MASK		(PWM_CTRL_T5_UPDATE_WIDTH \
					<< PWM_CTRL_T5_UPDATE_SHIFT)
#define PWM_CTRL_T5_START_SHIFT		24
#define PWM_CTRL_T5_START_WIDTH		0x1
#define PWM_CTRL_T5_START_MASK		(PWM_CTRL_T5_START_WIDTH \
					<< PWM_CTRL_T5_START_SHIFT)
#define PWM_CTRL_T4_AUTO_SHIFT		23
#define PWM_CTRL_T4_AUTO_WIDTH		0x1
#define PWM_CTRL_T4_AUTO_MASK		(PWM_CTRL_T4_AUTO_WIDTH \
					<< PWM_CTRL_T4_AUTO_SHIFT)
#define PWM_CTRL_T4_INV_SHIFT		22
#define PWM_CTRL_T4_INV_WIDTH		0x1
#define PWM_CTRL_T4_INV_MASK		(PWM_CTRL_T4_INV_WIDTH \
					<< PWM_CTRL_T4_INV_SHIFT)
#define PWM_CTRL_T4_UPDATE_SHIFT	21
#define PWM_CTRL_T4_UPDATE_WIDTH	0x1
#define PWM_CTRL_T4_UPDATE_MASK		(PWM_CTRL_T4_UPDATE_WIDTH \
					<< PWM_CTRL_T4_UPDATE_SHIFT)
#define PWM_CTRL_T4_START_SHIFT		20
#define PWM_CTRL_T4_START_WIDTH		0x1
#define PWM_CTRL_T4_START_MASK		(PWM_CTRL_T4_START_WIDTH \
					<< PWM_CTRL_T4_START_SHIFT)
#define PWM_CTRL_T3_AUTO_SHIFT		19
#define PWM_CTRL_T3_AUTO_WIDTH		0x1
#define PWM_CTRL_T3_AUTO_MASK		(PWM_CTRL_T3_AUTO_WIDTH \
					<< PWM_CTRL_T3_AUTO_SHIFT)
#define PWM_CTRL_T3_INV_SHIFT		18
#define PWM_CTRL_T3_INV_WIDTH		0x1
#define PWM_CTRL_T3_INV_MASK		(PWM_CTRL_T3_INV_WIDTH \
					<< PWM_CTRL_T3_INV_SHIFT)
#define PWM_CTRL_T3_UPDATE_SHIFT	17
#define PWM_CTRL_T3_UPDATE_WIDTH	0x1
#define PWM_CTRL_T3_UPDATE_MASK		(PWM_CTRL_T3_UPDATE_WIDTH \
					<< PWM_CTRL_T3_UPDATE_SHIFT)
#define PWM_CTRL_T3_START_SHIFT		16
#define PWM_CTRL_T3_START_WIDTH		0x1
#define PWM_CTRL_T3_START_MASK		(PWM_CTRL_T3_START_WIDTH \
					<< PWM_CTRL_T3_START_SHIFT)
#define PWM_CTRL_T2_AUTO_SHIFT		15
#define PWM_CTRL_T2_AUTO_WIDTH		0x1
#define PWM_CTRL_T2_AUTO_MASK		(PWM_CTRL_T2_AUTO_WIDTH \
					<< PWM_CTRL_T2_AUTO_SHIFT)
#define PWM_CTRL_T2_INV_SHIFT		14
#define PWM_CTRL_T2_INV_WIDTH		0x1
#define PWM_CTRL_T2_INV_MASK		(PWM_CTRL_T2_INV_WIDTH \
					<< PWM_CTRL_T2_INV_SHIFT)
#define PWM_CTRL_T2_UPDATE_SHIFT	13
#define PWM_CTRL_T2_UPDATE_WIDTH	0x1
#define PWM_CTRL_T2_UPDATE_MASK		(PWM_CTRL_T2_UPDATE_WIDTH \
					<< PWM_CTRL_T2_UPDATE_SHIFT)
#define PWM_CTRL_T2_START_SHIFT		12
#define PWM_CTRL_T2_START_WIDTH		0x1
#define PWM_CTRL_T2_START_MASK		(PWM_CTRL_T2_START_WIDTH \
					<< PWM_CTRL_T2_START_SHIFT)
#define PWM_CTRL_T1_AUTO_SHIFT		11
#define PWM_CTRL_T1_AUTO_WIDTH		0x1
#define PWM_CTRL_T1_AUTO_MASK		(PWM_CTRL_T1_AUTO_WIDTH \
					<< PWM_CTRL_T1_AUTO_SHIFT)
#define PWM_CTRL_T1_INV_SHIFT		10
#define PWM_CTRL_T1_INV_WIDTH		0x1
#define PWM_CTRL_T1_INV_MASK		(PWM_CTRL_T1_INV_WIDTH \
					<< PWM_CTRL_T1_INV_SHIFT)
#define PWM_CTRL_T1_UPDATE_SHIFT	9
#define PWM_CTRL_T1_UPDATE_WIDTH	0x1
#define PWM_CTRL_T1_UPDATE_MASK		(PWM_CTRL_T1_UPDATE_WIDTH \
					<< PWM_CTRL_T1_UPDATE_SHIFT)
#define PWM_CTRL_T1_START_SHIFT		8
#define PWM_CTRL_T1_START_WIDTH		0x1
#define PWM_CTRL_T1_START_MASK		(PWM_CTRL_T1_START_WIDTH \
					<< PWM_CTRL_T1_START_SHIFT)
#define PWM_CTRL_DZ_ENA_SHIFT		4
#define PWM_CTRL_DZ_ENA_WIDTH		0x1
#define PWM_CTRL_DZ_ENA_MASK		(PWM_CTRL_DZ_ENA_WIDTH \
					<< PWM_CTRL_DZ_ENA_SHIFT)
#define PWM_CTRL_T0_AUTO_SHIFT		3
#define PWM_CTRL_T0_AUTO_WIDTH		0x1
#define PWM_CTRL_T0_AUTO_MASK		(PWM_CTRL_T0_AUTO_WIDTH \
					<< PWM_CTRL_T0_AUTO_SHIFT)
#define PWM_CTRL_T0_INV_SHIFT		2
#define PWM_CTRL_T0_INV_WIDTH		0x1
#define PWM_CTRL_T0_INV_MASK		(PWM_CTRL_T0_INV_WIDTH \
					<< PWM_CTRL_T0_INV_SHIFT)
#define PWM_CTRL_T0_UPDATE_SHIFT	1
#define PWM_CTRL_T0_UPDATE_WIDTH	0x1
#define PWM_CTRL_T0_UPDATE_MASK		(PWM_CTRL_T0_UPDATE_WIDTH \
					<< PWM_CTRL_T0_UPDATE_SHIFT)
#define PWM_CTRL_T0_START_SHIFT		0
#define PWM_CTRL_T0_START_WIDTH		0x1
#define PWM_CTRL_T0_START_MASK		(PWM_CTRL_T0_START_WIDTH \
					<< PWM_CTRL_T0_START_SHIFT)

/*
 * 0x10, 0x1C, 0x28, 0x34, 0x40, 0x4C : Timer compare buffer
 */
#define PWM_TCMPB_COM_BUF_SHIFT		0
#define PWM_TCMPB_COM_BUF_WIDTH		0xFFFF
#define PWM_TCMPB_COM_BUF_MASK		(PWM_TCMPB_COM_BUF_WIDTH \
					<< PWM_TCMPB_COM_BUF_SHIFT)

/*
 * 0x14, 0x20, 0x2C, 0x38, 0x44, 0x50  : Timer count buffer
 */
#define PWM_TCNTB_CNT_BUF_SHIFT		0
#define PWM_TCNTB_CNT_BUF_WIDTH		0xFFFF
#define PWM_TCNTB_CNT_BUF_MASK		(PWM_TCNTB_CNT_BUF_WIDTH \
					<< PWM_TCNTB_CNT_BUF_SHIFT)

/*
 * 0x18, 0x24, 0x30, 0x3C, 0x48, 0x54  : Timer count observation
 */
#define PWM_TCNTO_CNT_OBS_SHIFT		0
#define PWM_TCNTO_CNT_OBS_WIDTH		0xFFFF
#define PWM_TCNTO_CNT_OBS_MASK		(PWM_TCNTO_CNT_OBS_WIDTH \
					<< PWM_TCNTO_CNT_OBS_SHIFT)

/*
 * 0x58  : Interrupt status flag
 */
#define PWM_INT_FLAG_SHIFT		0
#define PWM_INT_FLAG_WIDTH		0x1F
#define PWM_INT_FLAG_MASK		(PWM_INT_FLAG_WIDTH \
					<< PWM_INT_FLAG_SHIFT)

/*
 * 0x5C  : Interrupt clear
 */
#define PWM_INT_CLR_SHIFT		0
#define PWM_INT_CLR_WIDTH		0x1F
#define PWM_INT_CLR_MASK		(PWM_INT_CLR_WIDTH \
					<< PWM_INT_CLR_SHIFT)

/*
 * 0x60  : Interrupt status
 */
#define PWM_INT_STATUS_SHIFT		0
#define PWM_INT_STATUS_WIDTH		0x1F
#define PWM_INT_STATUS_MASK		(PWM_INT_STATUS_WIDTH \
					<< PWM_INT_STATUS_SHIFT)

/* Clock selection division values */
#define PWM_CLK_SEL_DIV2		0
#define PWM_CLK_SEL_DIV4		1
#define PWM_CLK_SEL_DIV8		2
#define PWM_CLK_SEL_DIV16		3
#define PWM_CLK_SEL_DIV1		4

#endif /* __ASM_ARCH_REGS_PWM_H */



/*
 *  arch/arm/mach-mvt12socfpga/include/mach/regs-sys.h
 *
 *  Copyright (C) 2010 MtekVision Co., Ltd.
 *	Jinyoung Park <parkjy@mtekvision.com>
 *	Jinyoung Park <parkjyb@mtekvision.com>
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
#ifndef __ASM_ARCH_REGS_SYS_H
#define __ASM_ARCH_REGS_SYS_H

/*
 * Base Address : 0x50F00000
 */

/*
 * Register values
 */
#define SYS_PRODUCT_ID			0x00
#define SYS_CTRL0				0x04
#define SYS_CTRL1				0x08
#define SYS_CTRL2				0x0C
#define SYS_GB_RST_CTRL			0x10
#define SYS_RST_CTRL0			0x14
#define SYS_RST_CTRL1			0x18
#define SYS_RST_CTRL2			0x1C
#define SYS_GB_CLK_CTRL			0x20
#define SYS_CLK_CTRL0			0x24
#define SYS_CLK_CTRL1			0x28
#define SYS_PLL_SRC_SEL			0x2C
#define SYS_MAIN_CLK_SEL		0x30
#define SYS_CLK_BYPASS			0x34
#define SYS_CLK_DIV0			0x38
#define SYS_CLK_DIV1			0x3C
#define SYS_CLK_DIV2			0x40
#define SYS_CLK_DIV3			0x44
#define SYS_CLK_DIV4			0x48
#define SYS_SYS_CLK_DIV			0x4C
#define SYS_PERI_CLK_CTRL0		0x50
#define SYS_PERI_CLK_CTRL1		0x54
#define SYS_PERI_CLK_CTRL2		0x58
#define SYS_PERI_CLK_CTRL3		0x5C
#define SYS_PERI_CLK_CTRL4		0x60
#define SYS_PERI_CLK_CTRL5		0x64
#define SYS_PERI_CLK_CTRL6		0x68
#define SYS_PERI_CLK_CTRL7		0x6C
#define SYS_PLL_PARAM0			0x70
#define SYS_PLL_PARAM1			0x74
#define SYS_PLL_PARAM2			0x78
#define SYS_PLL_PARAM3			0x7C
#define SYS_PLL_PARAM4			0x80
#define SYS_PERI_CLK_CTRL8		0x88
#define SYS_PE_CTRL				0x90
#define SYS_DMC_PAD_CTRL0		0xB0
#define SYS_DMC_PAD_CTRL1		0xB4
#define SYS_ARM11_CTRL			0xC0
#define SYS_OTP_STATUS0			0xD0
#define SYS_OTP_STATUS1			0xD4
#define SYS_OTP_STATUS2			0xD8
#define SYS_ALT_FUNC_ENA		0xE0
#define SYS_APS_MODE			0xE4
#define SYS_ISP_DEBUG			0xE8
#if defined(CONFIG_ARCH_MVFORMULA)
#define SYS_ENET_CTRL			0x300
#endif
#define SYS_EXT_INT_CTRL		0xF0
#define SYS_INT_PWR_SEL			0xF4
#define SYS_INT_PWR_POL			0xF8
#define SYS_INT_PWR_KEY			0xFC

/*
 * 0x04 : System Control 0
 */
#define SYS_CTRL_MEM_VENDOR_SHIFT	19
#define SYS_CTRL_MEM_VENDOR_WIDTH	0x1
#define SYS_CTRL_MEM_VENDOR_MASK	(SYS_CTRL_MEM_VENDOR_WIDTH \
					<< SYS_CTRL_MEM_VENDOR_SHIFT)
#define SYS_CTRL_MEM_MASKBIT_SHIFT	18
#define SYS_CTRL_MEM_MASKBIT_WIDTH	0x1
#define SYS_CTRL_MEM_MASKBIT_MASK	(SYS_CTRL_MEM_MASKBIT_WIDTH \
					<< SYS_CTRL_MEM_MASKBIT_SHIFT)
#define SYS_CTRL_MEM_TYPE_SHIFT		16
#define SYS_CTRL_MEM_TYPE_WIDTH		0x3
#define SYS_CTRL_MEM_TYPE_MASK		(SYS_CTRL_MEM_TYPE_WIDTH \
					<< SYS_CTRL_MEM_TYPE_SHIFT)
#define SYS_CTRL_CIO_CTRL_SHIFT		4
#define SYS_CTRL_CIO_CTRL_WIDTH		0x1
#define SYS_CTRL_CIO_CTRL_MASK		(SYS_CTRL_CIO_CTRL_WIDTH \
					<< SYS_CTRL_CIO_CTRL_SHIFT)
#define SYS_CTRL_MIPI_PWR_CUT_SHIFT	2
#define SYS_CTRL_MIPI_PWR_CUT_WIDTH	0x1
#define SYS_CTRL_MIPI_PWR_CUT_MASK	(SYS_CTRL_MIPI_PWR_CUT_WIDTH \
					<< SYS_CTRL_MIPI_PWR_CUT_SHIFT)
#define SYS_CTRL_HDMI_PWR_CUT_SHIFT	1
#define SYS_CTRL_HDMI_PWR_CUT_WIDTH	0x1
#define SYS_CTRL_HDMI_PWR_CUT_MASK	(SYS_CTRL_HDMI_PWR_CUT_WIDTH \
					<< SYS_CTRL_HDMI_PWR_CUT_SHIFT)
#define SYS_CTRL_MVC_PWRCUT_SHIFT	0
#define SYS_CTRL_MVC_PWRCUT_WIDTH	0x1
#define SYS_CTRL_MVC_PWRCUT_MASK	(SYS_CTRL_MVC_PWRCUT_WIDTH \
					<< SYS_CTRL_MVC_PWRCUT_SHIFT)

/*
 * 0x08 : System Control 1
 */
#define SYS_CTRL_REMAP_SHIFT		0
#define SYS_CTRL_REMAP_WIDTH		0x1
#define SYS_CTRL_REMAP_MASK		(SYS_CTRL_REMAP_WIDTH \
					<< SYS_CTRL_REMAP_SHIFT)

/*
 * 0x24 : CLOCK_CTRL0
 */
#define SYS_CTRL_DIP_CLOCK_PWDN_SHIFT	6
#define SYS_CTRL_DIP_CLOCK_PWDN_WIDTH	0x1
#define SYS_CTRL_DIP_CLOCK_PWDN_MASK	(SYS_CTRL_DIP_CLOCK_PWDN_WIDTH \
					<< SYS_CTRL_DIP_CLOCK_PWDN_SHIFT)

/*
 * 0x2C : PLL Source Select
 */
#define SYS_PLL_SRC_SEL_PLL4_SHIFT	24
#define SYS_PLL_SRC_SEL_PLL4_WIDTH	0x1F
#define SYS_PLL_SRC_SEL_PLL4_MASK	(SYS_PLL_SRC_SEL_PLL4_WIDTH \
					<< SYS_PLL_SRC_SEL_PLL4_SHIFT)
#define SYS_PLL_SRC_SEL_PLL3_SHIFT	18
#define SYS_PLL_SRC_SEL_PLL3_WIDTH	0x1F
#define SYS_PLL_SRC_SEL_PLL3_MASK	(SYS_PLL_SRC_SEL_PLL3_WIDTH \
					<< SYS_PLL_SRC_SEL_PLL3_SHIFT)
#define SYS_PLL_SRC_SEL_PLL2_SHIFT	12
#define SYS_PLL_SRC_SEL_PLL2_WIDTH	0x1F
#define SYS_PLL_SRC_SEL_PLL2_MASK	(SYS_PLL_SRC_SEL_PLL2_WIDTH \
					<< SYS_PLL_SRC_SEL_PLL2_SHIFT)
#define SYS_PLL_SRC_SEL_PLL1_SHIFT	6
#define SYS_PLL_SRC_SEL_PLL1_WIDTH	0x1F
#define SYS_PLL_SRC_SEL_PLL1_MASK	(SYS_PLL_SRC_SEL_PLL1_WIDTH \
					<< SYS_PLL_SRC_SEL_PLL1_SHIFT)
#define SYS_PLL_SRC_SEL_PLL0_SHIFT	0
#define SYS_PLL_SRC_SEL_PLL0_WIDTH	0x1F
#define SYS_PLL_SRC_SEL_PLL0_MASK	(SYS_PLL_SRC_SEL_PLL0_WIDTH \
					<< SYS_PLL_SRC_SEL_PLL0_SHIFT)

/*
 * 0x30 : Main clock select
 */
#define SYS_MAIN_CLK_SEL_USB_PHY_SHIFT	5
#define SYS_MAIN_CLK_SEL_USB_PHY_WIDTH	0x1
#define SYS_MAIN_CLK_SEL_USB_PHY_MASK	(SYS_MAIN_CLK_SEL_USB_PHY_WIDTH \
					<< SYS_MAIN_CLK_SEL_USB_PHY_SHIFT)
#define SYS_MAIN_CLK_SEL_PLL4_SHIFT	4
#define SYS_MAIN_CLK_SEL_PLL4_WIDTH	0x1
#define SYS_MAIN_CLK_SEL_PLL4_MASK	(SYS_MAIN_CLK_SEL_PLL4_WIDTH \
					<< SYS_MAIN_CLK_SEL_PLL4_SHIFT)
#define SYS_MAIN_CLK_SEL_PLL3_SHIFT	3
#define SYS_MAIN_CLK_SEL_PLL3_WIDTH	0x1
#define SYS_MAIN_CLK_SEL_PLL3_MASK	(SYS_MAIN_CLK_SEL_PLL3_WIDTH \
					<< SYS_MAIN_CLK_SEL_PLL3_SHIFT)
#define SYS_MAIN_CLK_SEL_PLL2_SHIFT	2
#define SYS_MAIN_CLK_SEL_PLL2_WIDTH	0x1
#define SYS_MAIN_CLK_SEL_PLL2_MASK	(SYS_MAIN_CLK_SEL_PLL2_WIDTH \
					<< SYS_MAIN_CLK_SEL_PLL2_SHIFT)
#define SYS_MAIN_CLK_SEL_PLL1_SHIFT	1
#define SYS_MAIN_CLK_SEL_PLL1_WIDTH	0x1
#define SYS_MAIN_CLK_SEL_PLL1_MASK	(SYS_MAIN_CLK_SEL_PLL1_WIDTH \
					<< SYS_MAIN_CLK_SEL_PLL1_SHIFT)
#define SYS_MAIN_CLK_SEL_PLL0_SHIFT	0
#define SYS_MAIN_CLK_SEL_PLL0_WIDTH	0x1
#define SYS_MAIN_CLK_SEL_PLL0_MASK	(SYS_MAIN_CLK_SEL_PLL0_WIDTH \
					<< SYS_MAIN_CLK_SEL_PLL0_SHIFT)

/*
 * 0x34 : Clock Bypass
 */
#define SYS_CLK_BYPASS_SYS_APB_SHIFT	11
#define SYS_CLK_BYPASS_SYS_APB_WIDTH	0x1
#define SYS_CLK_BYPASS_SYS_APB_MASK	(SYS_CLK_BYPASS_SYS_APB_WIDTH \
					<< SYS_CLK_BYPASS_SYS_APB_SHIFT)
#define SYS_CLK_BYPASS_SYS_BUS_SHIFT	9
#define SYS_CLK_BYPASS_SYS_BUS_WIDTH	0x1
#define SYS_CLK_BYPASS_SYS_BUS_MASK	(SYS_CLK_BYPASS_SYS_BUS_WIDTH \
					<< SYS_CLK_BYPASS_SYS_BUS_SHIFT)
#define SYS_CLK_BYPASS_SYS_ARM_SHIFT	8
#define SYS_CLK_BYPASS_SYS_ARM_WIDTH	0x1
#define SYS_CLK_BYPASS_SYS_ARM_MASK	(SYS_CLK_BYPASS_SYS_ARM_WIDTH \
					<< SYS_CLK_BYPASS_SYS_ARM_SHIFT)
#define SYS_CLK_BYPASS_PLL4_SHIFT	4
#define SYS_CLK_BYPASS_PLL4_WIDTH	0x1
#define SYS_CLK_BYPASS_PLL4_MASK	(SYS_CLK_BYPASS_PLL4_WIDTH \
					<< SYS_CLK_BYPASS_PLL4_SHIFT)
#define SYS_CLK_BYPASS_PLL3_SHIFT	3
#define SYS_CLK_BYPASS_PLL3_WIDTH	0x1
#define SYS_CLK_BYPASS_PLL3_MASK	(SYS_CLK_BYPASS_PLL3_WIDTH \
					<< SYS_CLK_BYPASS_PLL3_SHIFT)
#define SYS_CLK_BYPASS_PLL2_SHIFT	2
#define SYS_CLK_BYPASS_PLL2_WIDTH	0x1
#define SYS_CLK_BYPASS_PLL2_MASK	(SYS_CLK_BYPASS_PLL2_WIDTH \
					<< SYS_CLK_BYPASS_PLL2_SHIFT)
#define SYS_CLK_BYPASS_PLL1_SHIFT	1
#define SYS_CLK_BYPASS_PLL1_WIDTH	0x1
#define SYS_CLK_BYPASS_PLL1_MASK	(SYS_CLK_BYPASS_PLL1_WIDTH \
					<< SYS_CLK_BYPASS_PLL1_SHIFT)
#define SYS_CLK_BYPASS_PLL0_SHIFT	0
#define SYS_CLK_BYPASS_PLL0_WIDTH	0x1
#define SYS_CLK_BYPASS_PLL0_MASK	(SYS_CLK_BYPASS_PLL0_WIDTH \
					<< SYS_CLK_BYPASS_PLL0_SHIFT)

/*
 * 0x38, 0x3C, 0x40, 0x44, 0x48 : Clock Divide
 */
#define SYS_CLK_DIV_VAL_SHIFT		0
#define SYS_CLK_DIV_VAL_WIDTH		0x7F
#define SYS_CLK_DIV_VAL_MASK		(SYS_CLK_DIV_VAL_WIDTH \
					<< SYS_CLK_DIV_VAL_SHIFT)

/*
 * 0x4C : System Clock Divide
 */
#define SYS_SYS_CLK_DIV_DMC2_SHIFT	27
#define SYS_SYS_CLK_DIV_DMC2_WIDTH	0x7
#define SYS_SYS_CLK_DIV_DMC2_MASK	(SYS_SYS_CLK_DIV_DMC2_WIDTH \
					<< SYS_SYS_CLK_DIV_DMC2_SHIFT)
#define SYS_SYS_CLK_DIV_DMC1_SHIFT	24
#define SYS_SYS_CLK_DIV_DMC1_WIDTH	0x7
#define SYS_SYS_CLK_DIV_DMC1_MASK	(SYS_SYS_CLK_DIV_DMC1_WIDTH \
					<< SYS_SYS_CLK_DIV_DMC1_SHIFT)
#define SYS_SYS_CLK_DIV_APB_SHIFT	16
#define SYS_SYS_CLK_DIV_APB_WIDTH	0x7F
#define SYS_SYS_CLK_DIV_APB_MASK	(SYS_SYS_CLK_DIV_APB_WIDTH \
					<< SYS_SYS_CLK_DIV_APB_SHIFT)
#define SYS_SYS_CLK_DIV_BUS_SHIFT	8
#define SYS_SYS_CLK_DIV_BUS_WIDTH	0x7F
#define SYS_SYS_CLK_DIV_BUS_MASK	(SYS_SYS_CLK_DIV_BUS_WIDTH \
					<< SYS_SYS_CLK_DIV_BUS_SHIFT)
#define SYS_SYS_CLK_DIV_ARM_SHIFT	0
#define SYS_SYS_CLK_DIV_ARM_WIDTH	0x7F
#define SYS_SYS_CLK_DIV_ARM_MASK	(SYS_SYS_CLK_DIV_ARM_WIDTH \
					<< SYS_SYS_CLK_DIV_ARM_SHIFT)

/*
 * 0x50 : Peripheral Clock Control 0
 */
#define SYS_PERI_CLK_NAND_DIV_SHIFT	24
#define SYS_PERI_CLK_NAND_DIV_WIDTH	0x7F
#define SYS_PERI_CLK_NAND_DIV_MASK	(SYS_PERI_CLK_NAND_DIV_WIDTH \
					<< SYS_PERI_CLK_NAND_DIV_SHIFT)
#define SYS_PERI_CLK_NAND_BYPASS_SHIFT	23
#define SYS_PERI_CLK_NAND_BYPASS_WIDTH	0x1
#define SYS_PERI_CLK_NAND_BYPASS_MASK	(SYS_PERI_CLK_NAND_BYPASS_WIDTH \
					<< SYS_PERI_CLK_NAND_BYPASS_SHIFT)
#define SYS_PERI_CLK_NAND_SRC_SEL_SHIFT	22
#define SYS_PERI_CLK_NAND_SRC_SEL_WIDTH	0x1
#define SYS_PERI_CLK_NAND_SRC_SEL_MASK	(SYS_PERI_CLK_NAND_SRC_SEL_WIDTH \
					<< SYS_PERI_CLK_NAND_SRC_SEL_SHIFT)
#define SYS_PERI_CLK_NAND_ENA_SHIFT	21
#define SYS_PERI_CLK_NAND_ENA_WIDTH	0x1
#define SYS_PERI_CLK_NAND_ENA_MASK	(SYS_PERI_CLK_NAND_ENA_WIDTH \
					<< SYS_PERI_CLK_NAND_ENA_SHIFT)
#define SYS_PERI_CLK_SFR_RATIO_SHIFT	14
#define SYS_PERI_CLK_SFR_RATIO_WIDTH	0x3F
#define SYS_PERI_CLK_SFR_RATIO_MASK	(SYS_PERI_CLK_SFR_RATIO_WIDTH \
					<< SYS_PERI_CLK_SFR_RATIO_SHIFT)
#define SYS_PERI_CLK_SFR_SEL_SHIFT	13
#define SYS_PERI_CLK_SFR_SEL_WIDTH	0x1
#define SYS_PERI_CLK_SFR_SEL_MASK	(SYS_PERI_CLK_SFR_SEL_WIDTH \
					<< SYS_PERI_CLK_SFR_SEL_SHIFT)
#define SYS_PERI_CLK_SFR_BYPASS_SHIFT	12
#define SYS_PERI_CLK_SFR_BYPASS_WIDTH	0x1
#define SYS_PERI_CLK_SFR_BYPASS_MASK	(SYS_PERI_CLK_SFR_BYPASS_WIDTH \
					<< SYS_PERI_CLK_SFR_BYPASS_SHIFT)
#define SYS_PERI_CLK_SFR_POL_SHIFT	11
#define SYS_PERI_CLK_SFR_POL_WIDTH	0x1
#define SYS_PERI_CLK_SFR_POL_MASK	(SYS_PERI_CLK_SFR_POL_WIDTH \
					<< SYS_PERI_CLK_SFR_POL_SHIFT)
#define SYS_PERI_CLK_SFR_EN_SHIFT	10
#define SYS_PERI_CLK_SFR_EN_WIDTH	0x1
#define SYS_PERI_CLK_SFR_EN_MASK	(SYS_PERI_CLK_SFR_EN_WIDTH \
					<< SYS_PERI_CLK_SFR_EN_SHIFT)
#define SYS_PERI_CLK_TV_CLK_RATIO_SHIFT	3
#define SYS_PERI_CLK_TV_CLK_RATIO_WIDTH	0x3f
#define SYS_PERI_CLK_TV_CLK_RATIO_MASK	(SYS_PERI_CLK_TV_CLK_RATIO_WIDTH \
					<< SYS_PERI_CLK_TV_CLK_RATIO_SHIFT)
#define SYS_PERI_CLK_TV_CLK_BYPASS_SHIFT 2
#define SYS_PERI_CLK_TV_CLK_BYPASS_WIDTH 0x1
#define SYS_PERI_CLK_TV_CLK_BYPASS_MASK	 (SYS_PERI_CLK_TV_CLK_BYPASS_WIDTH \
					 << SYS_PERI_CLK_TV_CLK_BYPASS_SHIFT)
#define SYS_PERI_CLK_TV_CLK_ENA_SHIFT	0
#define SYS_PERI_CLK_TV_CLK_ENA_WIDTH	0x1
#define SYS_PERI_CLK_TV_CLK_ENA_MASK	(SYS_PERI_CLK_TV_CLK_ENA_WIDTH \
					<< SYS_PERI_CLK_TV_CLK_ENA_SHIFT)

/*
 * 0x54 : Peripheral Clock Control 1
 */
#define SYS_PERI_CLK_SEN2_SRC_SEL_SHIFT	30
#define SYS_PERI_CLK_SEN2_SRC_SEL_WIDTH	0x3
#define SYS_PERI_CLK_SEN2_SRC_SEL_MASK	(SYS_PERI_CLK_SEN2_SRC_SEL_WIDTH \
					<< SYS_PERI_CLK_SEN2_SRC_SEL_SHIFT)
#define SYS_PERI_CLK_SEN1_SRC_SEL_SHIFT	28
#define SYS_PERI_CLK_SEN1_SRC_SEL_WIDTH	0x3
#define SYS_PERI_CLK_SEN1_SRC_SEL_MASK	(SYS_PERI_CLK_SEN1_SRC_SEL_WIDTH \
					<< SYS_PERI_CLK_SEN1_SRC_SEL_SHIFT)
#define SYS_PERI_CLK_SEN0_SRC_SEL_SHIFT	26
#define SYS_PERI_CLK_SEN0_SRC_SEL_WIDTH	0x3
#define SYS_PERI_CLK_SEN0_SRC_SEL_MASK	(SYS_PERI_CLK_SEN0_SRC_SEL_WIDTH \
					<< SYS_PERI_CLK_SEN0_SRC_SEL_SHIFT)
#define SYS_PERI_CLK_SEN2_POL_SHIFT	25
#define SYS_PERI_CLK_SEN2_POL_WIDTH	0x1
#define SYS_PERI_CLK_SEN2_POL_MASK	(SYS_PERI_CLK_SEN2_POL_WIDTH \
					<< SYS_PERI_CLK_SEN2_POL_SHIFT)
#define SYS_PERI_CLK_SEN1_POL_SHIFT	24
#define SYS_PERI_CLK_SEN1_POL_WIDTH	0x1
#define SYS_PERI_CLK_SEN1_POL_MASK	(SYS_PERI_CLK_SEN1_POL_WIDTH \
					<< SYS_PERI_CLK_SEN1_POL_SHIFT)
#define SYS_PERI_CLK_SEN0_POL_SHIFT	23
#define SYS_PERI_CLK_SEN0_POL_WIDTH	0x1
#define SYS_PERI_CLK_SEN0_POL_MASK	(SYS_PERI_CLK_SEN0_POL_WIDTH \
					<< SYS_PERI_CLK_SEN0_POL_SHIFT)
#define SYS_PERI_CLK_SEN2_EN_SHIFT	22
#define SYS_PERI_CLK_SEN2_EN_WIDTH	0x1
#define SYS_PERI_CLK_SEN2_EN_MASK	(SYS_PERI_CLK_SEN2_EN_WIDTH \
					<< SYS_PERI_CLK_SEN2_EN_SHIFT)
#define SYS_PERI_CLK_SEN1_EN_SHIFT	21
#define SYS_PERI_CLK_SEN1_EN_WIDTH	0x1
#define SYS_PERI_CLK_SEN1_EN_MASK	(SYS_PERI_CLK_SEN1_EN_WIDTH \
					<< SYS_PERI_CLK_SEN1_EN_SHIFT)
#define SYS_PERI_CLK_SEN0_EN_SHIFT	20
#define SYS_PERI_CLK_SEN0_EN_WIDTH	0x1
#define SYS_PERI_CLK_SEN0_EN_MASK	(SYS_PERI_CLK_SEN0_EN_WIDTH \
					<< SYS_PERI_CLK_SEN0_EN_SHIFT)
#define SYS_PERI_CLK_SEN_RATIO_SHIFT	14
#define SYS_PERI_CLK_SEN_RATIO_WIDTH	0x3F
#define SYS_PERI_CLK_SEN_RATIO_MASK	(SYS_PERI_CLK_SEN_RATIO_WIDTH \
					<< SYS_PERI_CLK_SEN_RATIO_SHIFT)
#define SYS_PERI_CLK_SEN_BYPASS_SHIFT	13
#define SYS_PERI_CLK_SEN_BYPASS_WIDTH	0x1
#define SYS_PERI_CLK_SEN_BYPASS_MASK	(SYS_PERI_CLK_SEN_BYPASS_WIDTH \
					<< SYS_PERI_CLK_SEN_BYPASS_SHIFT)
#define SYS_PERI_CLK_SEN_POL_SHIFT	12
#define SYS_PERI_CLK_SEN_POL_WIDTH	0x1
#define SYS_PERI_CLK_SEN_POL_MASK	(SYS_PERI_CLK_SEN_POL_WIDTH \
					<< SYS_PERI_CLK_SEN_POL_SHIFT)
#define SYS_PERI_CLK_SEN_EN_SHIFT	11
#define SYS_PERI_CLK_SEN_EN_WIDTH	0x1
#define SYS_PERI_CLK_SEN_EN_MASK	(SYS_PERI_CLK_SEN_EN_WIDTH \
					<< SYS_PERI_CLK_SEN_EN_SHIFT)
#define SYS_PERI_CLK_ISP_DIV_SHIFT	4
#define SYS_PERI_CLK_ISP_DIV_WIDTH	0x7F
#define SYS_PERI_CLK_ISP_DIV_MASK	(SYS_PERI_CLK_ISP_DIV_WIDTH \
					<< SYS_PERI_CLK_ISP_DIV_SHIFT)
#define SYS_PERI_CLK_ISP_POL_SHIFT	3
#define SYS_PERI_CLK_ISP_POL_WIDTH	0x1
#define SYS_PERI_CLK_ISP_POL_MASK	(SYS_PERI_CLK_ISP_POL_WIDTH \
					<< SYS_PERI_CLK_ISP_POL_SHIFT)
#define SYS_PERI_CLK_ISP_BYPASS_SHIFT	2
#define SYS_PERI_CLK_ISP_BYPASS_WIDTH	0x1
#define SYS_PERI_CLK_ISP_BYPASS_MASK	(SYS_PERI_CLK_ISP_BYPASS_WIDTH \
					<< SYS_PERI_CLK_SEN_BYPASS_SHIFT)
#define SYS_PERI_CLK_ISP_SRC_SEL_SHIFT	1
#define SYS_PERI_CLK_ISP_SRC_SEL_WIDTH	0x1
#define SYS_PERI_CLK_ISP_SRC_SEL_MASK	(SYS_PERI_CLK_ISP_SRC_SEL_WIDTH \
					<< SYS_PERI_CLK_ISP_SRC_SEL_SHIFT)
#define SYS_PERI_CLK_ISP_EN_SHIFT	0
#define SYS_PERI_CLK_ISP_EN_WIDTH	0x1
#define SYS_PERI_CLK_ISP_EN_MASK	(SYS_PERI_CLK_ISP_EN_WIDTH \
					<< SYS_PERI_CLK_ISP_EN_SHIFT)

/*
 * 0x58 : Peripheral Clock Control 2
 */
#define SYS_PERI_CLK_DMC_ALT_DIV_SHIFT	29
#define SYS_PERI_CLK_DMC_ALT_DIV_WIDTH	0x7
#define SYS_PERI_CLK_DMC_ALT_DIV_MASK	(SYS_PERI_CLK_DMC_ALT_DIV_WIDTH \
					<< SYS_PERI_CLK_DMC_ALT_DIV_SHIFT)
#define SYS_PERI_CLK_DMC_ALT_SRC_SEL_SHIFT	27
#define SYS_PERI_CLK_DMC_ALT_SRC_SEL_WIDTH	0x3
#define SYS_PERI_CLK_DMC_ALT_SRC_SEL_MASK \
					(SYS_PERI_CLK_DMC_ALT_SRC_SEL_WIDTH \
					<< SYS_PERI_CLK_DMC_ALT_SRC_SEL_SHIFT)
#define SYS_PERI_CLK_DMC_ALT_BYPASS_SHIFT	26
#define SYS_PERI_CLK_DMC_ALT_BYPASS_WIDTH	0x1
#define SYS_PERI_CLK_DMC_ALT_BYPASS_MASK \
					(SYS_PERI_CLK_DMC_ALT_BYPASS_WIDTH \
					<< SYS_PERI_CLK_DMC_ALT_BYPASS_SHIFT)
#define SYS_PERI_CLK_DMC_ALT_SEL_SHIFT	25
#define SYS_PERI_CLK_DMC_ALT_SEL_WIDTH	0x1
#define SYS_PERI_CLK_DMC_ALT_SEL_MASK	(SYS_PERI_CLK_DMC_ALT_SEL_WIDTH \
					<< SYS_PERI_CLK_DMC_ALT_SEL_SHIFT)
#define SYS_PERI_CLK_DMC_ALT_EN_SHIFT	24
#define SYS_PERI_CLK_DMC_ALT_EN_WIDTH	0x1
#define SYS_PERI_CLK_DMC_ALT_EN_MASK	(SYS_PERI_CLK_DMC_ALT_EN_WIDTH \
					<< SYS_PERI_CLK_DMC_ALT_EN_SHIFT)
#define SYS_PERI_CLK_AUDIO_NCO_EN_SHIFT	0
#define SYS_PERI_CLK_AUDIO_NCO_EN_WIDTH	0x1
#define SYS_PERI_CLK_AUDIO_NCO_EN_MASK	(SYS_PERI_CLK_AUDIO_NCO_EN_WIDTH \
					<< SYS_PERI_CLK_AUDIO_NCO_EN_SHIFT)

/*
 * 0x68 : Peripheral Clock Control 6
 */
#define SYS_PERI_CLK_CEC_DIV_SHIFT	17
#define SYS_PERI_CLK_CEC_DIV_WIDTH	0x1FFF
#define SYS_PERI_CLK_CEC_DIV_MASK	(SYS_PERI_CLK_CEC_DIV_WIDTH \
					<< SYS_PERI_CLK_CEC_DIV_SHIFT)
#define SYS_PERI_CLK_CEC_ENA_SHIFT	16
#define SYS_PERI_CLK_CEC_ENA_WIDTH	0x1
#define SYS_PERI_CLK_CEC_ENA_MASK	(SYS_PERI_CLK_CEC_ENA_WIDTH \
					<< SYS_PERI_CLK_CEC_ENA_SHIFT)

#define SYS_PERI_CLK_LCD_DIV_SHIFT	3
#define SYS_PERI_CLK_LCD_DIV_WIDTH	0xFF
#define SYS_PERI_CLK_LCD_DIV_MASK	(SYS_PERI_CLK_LCD_DIV_WIDTH \
					<< SYS_PERI_CLK_LCD_DIV_SHIFT)
#define SYS_PERI_CLK_LCD_SRC_SEL_SHIFT	1
#define SYS_PERI_CLK_LCD_SRC_SEL_WIDTH	0x3
#define SYS_PERI_CLK_LCD_SRC_SEL_MASK	(SYS_PERI_CLK_LCD_SRC_SEL_WIDTH \
					<< SYS_PERI_CLK_LCD_SRC_SEL_SHIFT)
#define SYS_PERI_CLK_LCD_ENA_SHIFT	0
#define SYS_PERI_CLK_LCD_ENA_WIDTH	0x1
#define SYS_PERI_CLK_LCD_ENA_MASK	(SYS_PERI_CLK_LCD_ENA_WIDTH \
					<< SYS_PERI_CLK_LCD_ENA_SHIFT)

/*
 * 0x88 : Peripheral Clock Control 8
 */
#define SYS_PERI_CLK_SD1_DIV_SHIFT	26
#define SYS_PERI_CLK_SD1_DIV_WIDTH	0xFFFF
#define SYS_PERI_CLK_SD1_DIV_MASK	(SYS_PERI_CLK_SD1_DIV_WIDTH \
					<< SYS_PERI_CLK_SD1_DIV_SHIFT)
#define SYS_PERI_CLK_SD1_SRC_SEL_SHIFT	24
#define SYS_PERI_CLK_SD1_SRC_SEL_WIDTH	0x3
#define SYS_PERI_CLK_SD1_SRC_SEL_MASK	(SYS_PERI_CLK_SD1_SRC_SEL_WIDTH \
					<< SYS_PERI_CLK_SD1_SRC_SEL_SHIFT)
#define SYS_PERI_CLK_SD1_BYPASS_SHIFT	23
#define SYS_PERI_CLK_SD1_BYPASS_WIDTH	0x1
#define SYS_PERI_CLK_SD1_BYPASS_MASK	(SYS_PERI_CLK_SD1_BYPASS_WIDTH \
					<< SYS_PERI_CLK_SD1_BYPASS_SHIFT)
#define SYS_PERI_CLK_SD1_ENA_SHIFT	22
#define SYS_PERI_CLK_SD1_ENA_WIDTH	0x1
#define SYS_PERI_CLK_SD1_ENA_MASK	(SYS_PERI_CLK_SD1_ENA_WIDTH \
					<< SYS_PERI_CLK_SD1_ENA_SHIFT)
#define SYS_PERI_CLK_SD0_DIV_SHIFT	15
#define SYS_PERI_CLK_SD0_DIV_WIDTH	0x3F
#define SYS_PERI_CLK_SD0_DIV_MASK	(SYS_PERI_CLK_SD0_DIV_WIDTH \
					<< SYS_PERI_CLK_SD0_DIV_SHIFT)
#define SYS_PERI_CLK_SD0_SRC_SEL_SHIFT	13
#define SYS_PERI_CLK_SD0_SRC_SEL_WIDTH	0x3
#define SYS_PERI_CLK_SD0_SRC_SEL_MASK	(SYS_PERI_CLK_SD0_SRC_SEL_WIDTH \
					<< SYS_PERI_CLK_SD0_SRC_SEL_SHIFT)
#define SYS_PERI_CLK_SD0_BYPASS_SHIFT	12
#define SYS_PERI_CLK_SD0_BYPASS_WIDTH	0x1
#define SYS_PERI_CLK_SD0_BYPASS_MASK	(SYS_PERI_CLK_SD0_BYPASS_WIDTH \
					<< SYS_PERI_CLK_SD0_BYPASS_SHIFT)
#define SYS_PERI_CLK_SD0_ENA_SHIFT	11
#define SYS_PERI_CLK_SD0_ENA_WIDTH	0x1
#define SYS_PERI_CLK_SD0_ENA_MASK	(SYS_PERI_CLK_SD0_ENA_WIDTH \
					<< SYS_PERI_CLK_SD0_ENA_SHIFT)
#define SYS_PERI_CLK_MOVI_DIV_SHIFT	4
#define SYS_PERI_CLK_MOVI_DIV_WIDTH	0x3F
#define SYS_PERI_CLK_MOVI_DIV_MASK	(SYS_PERI_CLK_MOVI_DIV_WIDTH \
					<< SYS_PERI_CLK_MOVI_DIV_SHIFT)
#define SYS_PERI_CLK_MOVI_SRC_SEL_SHIFT	2
#define SYS_PERI_CLK_MOVI_SRC_SEL_WIDTH	0x3
#define SYS_PERI_CLK_MOVI_SRC_SEL_MASK	(SYS_PERI_CLK_MOVI_SRC_SEL_WIDTH \
					<< SYS_PERI_CLK_MOVI_SRC_SEL_SHIFT)
#define SYS_PERI_CLK_MOVI_BYPASS_SHIFT	1
#define SYS_PERI_CLK_MOVI_BYPASS_WIDTH	0x1
#define SYS_PERI_CLK_MOVI_BYPASS_MASK	(SYS_PERI_CLK_MOVI_BYPASS_WIDTH \
					<< SYS_PERI_CLK_MOVI_BYPASS_SHIFT)
#define SYS_PERI_CLK_MOVI_ENA_SHIFT	0
#define SYS_PERI_CLK_MOVI_ENA_WIDTH	0x1
#define SYS_PERI_CLK_MOVI_ENA_MASK	(SYS_PERI_CLK_MOVI_ENA_WIDTH \
					<< SYS_PERI_CLK_MOVI_ENA_SHIFT)

/*
 * 0x70 : PLL Parameter 0
 */
#define SYS_PLL_PARAM0_LOCK_SHIFT	28
#define SYS_PLL_PARAM0_LOCK_WIDTH	0x1
#define SYS_PLL_PARAM0_LOCK_MASK	(SYS_PLL_PARAM0_LOCK_WIDTH \
					<< SYS_PLL_PARAM0_LOCK_SHIFT)
#define SYS_PLL_PARAM0_NR_SHIFT		16
#define SYS_PLL_PARAM0_NR_WIDTH		0x1F
#define SYS_PLL_PARAM0_NR_MASK		(SYS_PLL_PARAM0_NR_WIDTH \
					<< SYS_PLL_PARAM0_NR_SHIFT)
#define SYS_PLL_PARAM0_NO_SHIFT		12
#define SYS_PLL_PARAM0_NO_WIDTH		0x3
#define SYS_PLL_PARAM0_NO_MASK		(SYS_PLL_PARAM0_NO_WIDTH \
					<< SYS_PLL_PARAM0_NO_SHIFT)
#define SYS_PLL_PARAM0_NF_SHIFT		4
#define SYS_PLL_PARAM0_NF_WIDTH		0x7F
#define SYS_PLL_PARAM0_NF_MASK		(SYS_PLL_PARAM0_NF_WIDTH \
					<< SYS_PLL_PARAM0_NF_SHIFT)
#define SYS_PLL_PARAM0_BYPASS_SHIFT	1
#define SYS_PLL_PARAM0_BYPASS_WIDTH	0x1
#define SYS_PLL_PARAM0_BYPASS_MASK	(SYS_PLL_PARAM0_BYPASS_WIDTH \
					<< SYS_PLL_PARAM0_BYPASS_SHIFT)
#define SYS_PLL_PARAM0_RST_SHIFT	0
#define SYS_PLL_PARAM0_RST_WIDTH	0x1
#define SYS_PLL_PARAM0_RST_MASK		(SYS_PLL_PARAM0_RST_WIDTH \
					<< SYS_PLL_PARAM0_RST_SHIFT)

/*
 * 0x74, 0x78, 0x7C, 0x80 : PLL Parameter 1~4
 */
#define SYS_PLL_PARAM_LOCK_SHIFT	28
#define SYS_PLL_PARAM_LOCK_WIDTH	0x1
#define SYS_PLL_PARAM_LOCK_MASK		(SYS_PLL_PARAM_LOCK_WIDTH \
					<< SYS_PLL_PARAM_LOCK_SHIFT)
#define SYS_PLL_PARAM_RANGE_SHIFT	24
#define SYS_PLL_PARAM_RANGE_WIDTH	0x7
#define SYS_PLL_PARAM_RANGE_MASK	(SYS_PLL_PARAM_RANGE_WIDTH \
					<< SYS_PLL_PARAM_RANGE_SHIFT)
#define SYS_PLL_PARAM_NR_SHIFT		16
#define SYS_PLL_PARAM_NR_WIDTH		0x1F
#define SYS_PLL_PARAM_NR_MASK		(SYS_PLL_PARAM_NR_WIDTH \
					<< SYS_PLL_PARAM_NR_SHIFT)
#define SYS_PLL_PARAM_NO_SHIFT		12
#define SYS_PLL_PARAM_NO_WIDTH		0x7
#define SYS_PLL_PARAM_NO_MASK		(SYS_PLL_PARAM_NO_WIDTH \
					<< SYS_PLL_PARAM_NO_SHIFT)
#define SYS_PLL_PARAM_NF_SHIFT		4
#define SYS_PLL_PARAM_NF_WIDTH		0xFF
#define SYS_PLL_PARAM_NF_MASK		(SYS_PLL_PARAM_NF_WIDTH \
					<< SYS_PLL_PARAM_NF_SHIFT)
#define SYS_PLL_PARAM_BYPASS_SHIFT	1
#define SYS_PLL_PARAM_BYPASS_WIDTH	0x1
#define SYS_PLL_PARAM_BYPASS_MASK	(SYS_PLL_PARAM_BYPASS_WIDTH \
					<< SYS_PLL_PARAM_BYPASS_SHIFT)
#define SYS_PLL_PARAM_RST_SHIFT		0
#define SYS_PLL_PARAM_RST_WIDTH		0x1
#define SYS_PLL_PARAM_RST_MASK		(SYS_PLL_PARAM_RST_WIDTH \
					<< SYS_PLL_PARAM_RST_SHIFT)

#if defined(CONFIG_ARCH_MVFORMULA)
/*
 * 0xEC : Ethernet control register
 */
#define SYS_ETH_CTRL_PHY_PLL_SEL_SHIFT		11
#define SYS_ETH_CTRL_PHY_PLL_SEL_WIDTH		0x3
#define SYS_ETH_CTRL_PHY_PLL_SEL_MASK		(SYS_ETH_CTRL_PHY_PLL_SEL_WIDTH \
						<< SYS_ETH_CTRL_PHY_PLL_SEL_SHIFT)
#define SYS_ETH_CTRL_PHY_CLK_RATIO_SHIFT	5
#define SYS_ETH_CTRL_PHY_CLK_RATIO_WIDTH	0x3F
#define SYS_ETH_CTRL_PHY_CLK_RATIO_MASK		(SYS_ETH_CTRL_PHY_CLK_RATIO_WIDTH \
						<< SYS_ETH_CTRL_PHY_CLK_RATIO_SHIFT)
#define SYS_ETH_CTRL_PHY_CLK_EN_SHIFT		4
#define SYS_ETH_CTRL_PHY_CLK_EN_WIDTH		0x1
#define SYS_ETH_CTRL_PHY_CLK_EN_MASK		(SYS_ETH_CTRL_PHY_CLK_EN_WIDTH \
						<< SYS_ETH_CTRL_PHY_CLK_EN_SHIFT)
#define SYS_ETH_CTRL_MAC_SPEED_SHIFT		2
#define SYS_ETH_CTRL_MAC_SPEED_WIDTH		0x3
#define SYS_ETH_CTRL_MAC_SPEED_MASK		(SYS_ETH_CTRL_MAC_SPEED_WIDTH \
						<< SYS_ETH_CTRL_MAC_SPEED_SHIFT)
#define SYS_ETH_CTRL_SBD_PWR_DOWN_ACK_SHIFT	1
#define SYS_ETH_CTRL_SBD_PWR_DOWN_ACK_WIDTH	0x1
#define SYS_ETH_CTRL_SBD_PWR_DOWN_ACK_MASK	(SYS_ETH_CTRL_SBD_PWR_DOWN_ACK_WIDTH \
						<< SYS_ETH_CTRL_SBD_PWR_DOWN_ACK_SHIFT)
#define SYS_ETH_CTRL_SBD_DATA_ENDIAN_SHIFT	0
#define SYS_ETH_CTRL_SBD_DATA_ENDIAN_WIDTH	0x1
#define SYS_ETH_CTRL_SBD_DATA_ENDIAN_MASK	(SYS_ETH_CTRL_SBD_DATA_ENDIAN_WIDTH \
						<< SYS_ETH_CTRL_SBD_DATA_ENDIAN_SHIFT)
#endif // defined(CONFIG_ARCH_MVFORMULA)

typedef union
{
    struct
    {
        unsigned int mvc_pwrcut : 1;       // 0     1 : Core level power cut
        unsigned int hdmi_pwrcut : 1;      // 1     1 : HDMI core & phy power cut
        unsigned int mipi_pwrcut : 1;       // 2        1 : MIPI core & phy power cut
        unsigned int reserved0 : 1;         // 3
        unsigned int core_io_control : 1;       // 4        Core level IO control -->what??
        unsigned int ring_osc_en : 1;       // 5        Crypto의 ring osc.를 enable/disable
        unsigned int reserved1 : 10;        // 6-15
        unsigned int pkg_opt : 4;       // 16-19        [17:16]: memory type, [18]: cs,cke masking bit, [19]: vendor -->what??
        unsigned int boot_mode : 2;     // 20-21        1:nfl, 2:spi    --> how??
        unsigned int boot_opt : 6;      // 22-27        [27]: read command type (1=dual, 0=single)
                                                    //                 [26:25]: address cycle(0=3, 1=4, 2=5, 3=6)
                                                     //                [24]: page size(1=2K, 0=512)
                                                     //                 [23]: spare size(1=28, 0=16 per 512byte)
                                                     //                 [22]: ecc enable(1=enable)

    } asField;
    unsigned int as32Bits;
} SYS_CTRL_0;

typedef union
{
    struct
    {
        unsigned int remap : 1;      // 0       0:DRAM base = 0x0000_0000, SRAM base = 0x4000_0000, 1:DRAM base is 0x4000_0000, SRAM base = 0x0000_0000
        unsigned int osc_en : 1;    // 1        Main Osc. enable
        unsigned int osc32k_en : 1;     // 2    32KHz Osc. enable
        unsigned int reserved0: 1;      // 3
        unsigned int mvc_shadow_boot : 3;       // 4-6      [6]:boot flag,
                                                                    //             [5:4]:next boot type setting 1:NFL, 2:SPI
        unsigned int reserved1: 1;      // 7
        unsigned int shadow_boot_rt : 3;        // 8-10        RO, Core power가 인가될 경우 mvc_shadow_boot 설정 값이 반영된다.
    } asField;
    unsigned int as32Bits;
} SYS_CTRL_1;

typedef union
{
    struct
    {
        unsigned int dmc_GB_sel0 : 2;      // 0-1   Mbus0's dram address(when remap)    0:0x00000000~0x3FFFFFFF, 1:0x40000000~0x7FFFFFFF, 2:0x80000000~0xBFFFFFFF, 3:0xC0000000~0xFFFFFFFF
        unsigned int dmc_cs0 : 1;         // 2          0:first chip, 1:second chip
        unsigned int reserved0 : 1;     // 3
        unsigned int dmc_GB_sel1 : 2;        // 4-5  Mbus1's dram address(when remap)
        unsigned int dmc_cs1 : 1;        // 6
        unsigned int reserved1 : 1;      // 7
        unsigned int dmc_sfresh_ent : 1;        // 8        Dram self refresh start
        unsigned int dmc_port_busy : 2;     // 9-10        Dmc port busy
        unsigned int dmc_controller_busy : 1;       // 11   Dmc controller busy
        unsigned int dmc_srefresh_ack : 1;          // 12   Dram self refresh doing report
        unsigned int dmc_q_almost_full : 1;     // 13   Command queue almost full
        unsigned int dmc_refresh_in_process : 1;        // 14   Dram auto refresh doing
        unsigned int dmc_controller_int : 1;        // 15   Interrupt of dmc controller
        unsigned int dac_gsw : 3;       // 16-18        Video gain control register
        unsigned int aud_bb_ctrl : 4;       // 19-22        Audio BB transfer control register
    } asField;
    unsigned int as32Bits;
} SYS_CTRL_2;

typedef union
{
    struct
    {
        unsigned int all    : 1;      // 0     all reset
        unsigned int arm    : 1;      // 1        arm reset
        unsigned int bus0    : 1;      // 2        bus0 reset
        unsigned int bus1 : 1;      // 3        bus1 reset
        unsigned int mbus0  : 1;      // 4      MBUS0 reset
        unsigned int mbus1  : 1;      // 5      MBUS1 reset
        unsigned int armI   : 1;      // 6      armI reset
        unsigned int armRW  : 1;      // 7      armRW reset
        unsigned int armDMA : 1;      // 8      armDMA reset
        unsigned int armP   : 1;      // 9      armP reset
        unsigned int armv : 1;      // 10       armv reset
    } asField;
    unsigned int as32Bits;
} G_RESET_CTRL;


typedef union
{
    struct
    {
        unsigned int host : 1;      // 0
        unsigned int dmc : 1;       // 1
        unsigned int dma : 1;       // 2
        unsigned int intc : 1;          // 3
        unsigned int scaler : 1;    // 4
        unsigned int tds : 1;       // 5
        unsigned int dip : 1;       // 6
        unsigned int crypto : 1;           // 7
        unsigned int jpegenc : 1;       // 8
        unsigned int jpegdec : 1;       // 9
        unsigned int usb : 1;           // 10
        unsigned int dvc : 1;           // 11
        unsigned int pds : 1;           // 12
        unsigned int isp : 1;           // 13
        unsigned int mali : 1;          // 14
        unsigned int mpc : 1;           // 15
        unsigned int vp6 : 1;           // 16
#if defined(CONFIG_ARCH_MVFORMULA)
	unsigned int eth : 1;		// 17
#endif
    } asField;
    unsigned int as32Bits;
} RESET_CTRL0;

typedef union
{
    struct
    {
        unsigned int apb : 1;      // 0
        unsigned int mvc : 1;       // 1
        unsigned int nfl : 1;       // 2
        unsigned int rtc : 1;          // 3
        unsigned int spi0 : 1;    // 4
        unsigned int mp2ts : 1;       // 5
        unsigned int uart0 : 1;       // 6
        unsigned int key : 1;           // 7
        unsigned int pwm : 1;       // 8
        unsigned int i2c0 : 1;       // 9
        unsigned int aud : 1;       // 10
        unsigned int sdc0 : 1;      // 11
        unsigned int sdc1 : 1;      // 12
        unsigned int mnd : 1;       // 13
        unsigned int smc : 1;       // 14
        unsigned int spi1 : 1;      // 15
        unsigned int spi2 : 1;      // 16
        unsigned int uart1 : 1;     // 17
        unsigned int uart2 : 1;     // 18
        unsigned int uart3 : 1;     // 19
        unsigned int i2c1 : 1;      // 20
        unsigned int i2c2 : 1;      // 21
    } asField;
    unsigned int as32Bits;
} RESET_CTRL1;

typedef union
{
    struct
    {
        unsigned int usbcore : 1;      // 0
        unsigned int usbphy : 1;       // 1
        unsigned int mptphy : 1;       // 2
        unsigned int mprphy : 1;          // 3
        unsigned int tve : 1;    // 4
        unsigned int tve_sl : 1;    // 5
        unsigned int hdmiphy : 1;       // 6
    } asField;
    unsigned int as32Bits;
} RESET_CTRL2;

typedef union
{
    struct
    {
        unsigned int all: 1;        // 0     all clock pw down
        unsigned int arm: 1;      // 1        arm clock pw down
        unsigned int bus0: 1;      // 2        bus0 clock pw down
        unsigned int bus1 : 1;      // 3        bus1 clock pw down
        unsigned int mbus0  : 1;      // 4      MBUS0 clock pw down
        unsigned int mbus1  : 1;      // 5      MBUS1 clock pw down
        unsigned int armI   : 1;      // 6      armI clock pw down
        unsigned int armRW  : 1;      // 7      armRW clock pw down
        unsigned int armDMA : 1;      // 8      armDMA clock pw down
        unsigned int armP   : 1;      // 9      armP clock pw down
    } asField;
    unsigned int as32Bits;
} G_CLOCK_CTRL;

typedef union
{
    struct
    {
        unsigned int host : 1;      // 0
        unsigned int dmc : 1;       // 1
        unsigned int dma : 1;       // 2
        unsigned int intc : 1;          // 3
        unsigned int scaler : 1;    // 4
        unsigned int tds : 1;       // 5
        unsigned int dip : 1;       // 6
        unsigned int crypto : 1;           // 7
        unsigned int jpegenc : 1;       // 8
        unsigned int jpegdec : 1;       // 9
        unsigned int usb : 1;           // 10
        unsigned int dvc : 1;           // 11
        unsigned int pds : 1;           // 12
        unsigned int isp : 1;           // 13
        unsigned int mali : 1;          // 14
        unsigned int mipi : 1;           // 15
        unsigned int vp6 : 1;               // 16
        unsigned int dmc1x : 1;         // 17
        unsigned int dmc2x : 1;         // 18
#if defined(CONFIG_ARCH_MVFORMULA)
	unsigned int eth : 1;		// 19
	unsigned int reserved0 : 1;		// 20
#else
        unsigned int reserved0 : 2;     // 19, 20
#endif
        unsigned int i_RxByteClkHS0 : 1;        // 21
        unsigned int i_RxByteClkHS1 : 1;        // 22
        unsigned int i_RxByteClkHS2 : 1;        // 23
        unsigned int i_RxByteClkHS3 : 1;        // 24
        unsigned int i_S_RxClkEsc0 : 1;     // 25
        unsigned int i_S_RxClkEsc1 : 1;     // 26
        unsigned int i_S_RxClkEsc2 : 1;     // 27
        unsigned int i_S_RxClkEsc3 : 1;     // 28
        unsigned int i_TxClkLaneHS : 1;     // 29
        unsigned int i_TxByteClkHS0 : 1;        // 30
        unsigned int i_TxByteClkHS1 : 1;        // 31
    } asField;
    unsigned int as32Bits;
} CLOCK_CTRL0;

typedef union
{
    struct
    {
        unsigned int apb : 1;      // 0
        unsigned int mvc : 1;       // 1
        unsigned int nfl : 1;       // 2
        unsigned int rtc : 1;          // 3
        unsigned int spi0 : 1;    // 4
        unsigned int mp2ts : 1;       // 5
        unsigned int uart0 : 1;       // 6
        unsigned int key : 1;           // 7
        unsigned int pwm : 1;       // 8
        unsigned int i2c0 : 1;       // 9
        unsigned int aud : 1;       // 10
        unsigned int sdc0 : 1;      // 11
        unsigned int sdc1 : 1;      // 12
        unsigned int mnd : 1;       // 13
        unsigned int smc : 1;       // 14
        unsigned int spi1 : 1;      // 15
        unsigned int spi2 : 1;      // 16
        unsigned int uart1 : 1;     // 17
        unsigned int uart2 : 1;     // 18
        unsigned int uart3 : 1;     // 19
        unsigned int i2c1 : 1;      // 20
        unsigned int i2c2 : 1;      // 21
    } asField;
    unsigned int as32Bits;
} CLOCK_CTRL1;

typedef union
{
    struct
    {
        unsigned int sel0 : 5;      // 0-4
        unsigned int reserved0 : 1;     // 5
        unsigned int sel1 : 5;      // 6-10
        unsigned int reserved1 : 1;     // 11
        unsigned int sel2 : 5;      // 12-16
        unsigned int reserved2 : 1;     // 17
        unsigned int sel3 : 5;      // 18-22
        unsigned int reserved3 : 1;     // 23
        unsigned int sel4 : 5;      // 24-28
    } asField;
    unsigned int as32Bits;
} PLL_SRC_SEL;

typedef union
{
    struct
    {
        unsigned int pllSrc0 : 1;      // 0
        unsigned int pllSrc1 : 1;     // 1
        unsigned int pllSrc2 : 1;      // 2
        unsigned int pllSrc3 : 1;     // 3
        unsigned int pllSrc4 : 1;      // 4
        unsigned int usbPlyClkSrc : 1;     // 5       0 : PLL_SRC1 use, 1 : OSC_CLK use
    } asField;
    unsigned int as32Bits;
} MAIN_CLK_SEL;

typedef union
{
    struct
    {
        unsigned int pll0MainClk : 1;      // 0
        unsigned int pll1MainClk : 1;     // 1
        unsigned int pll2MainClk : 1;      // 2
        unsigned int pll3MainClk : 1;     // 3
        unsigned int pll4MainClk : 1;      // 4
        unsigned int reserved0 : 3;     // 5-7
        unsigned int armClk : 1;      // 8
        unsigned int busClk : 1;        // 9
        unsigned int reserved1 : 1;     // 10
        unsigned int apbClk : 1;        // 11
    } asField;
    unsigned int as32Bits;
} CLK_BYPASS;

typedef union
{
    struct
    {
        unsigned int divSel : 7;      // 0-6
    } asField;
    unsigned int as32Bits;
} CLK_DIV;

typedef union
{
    struct
    {
        unsigned int divArm : 7;      // 0-6
        unsigned int reserved0 : 1;     // 7
        unsigned int divBus : 7;      // 8-14
        unsigned int reserved1 : 1;     // 15
        unsigned int divApb : 7;      // 16-22
        unsigned int reserved2 : 1;     // 23
        unsigned int divDmc1 : 3;      // 24-26
        unsigned int divDmc2 : 3;       // 27-29
    } asField;
    unsigned int as32Bits;
} SYS_CLK_DIV;

typedef union
{
    struct
    {
        unsigned int tvClkEn : 1;      // 0
        unsigned int tvClkPol : 1;     // 1
        unsigned int tvClkBypass : 1;      // 2
        unsigned int tvClkRatio : 6;        // 3-8
        unsigned int reserved0 : 1;     // 9
        unsigned int sfrClkEn : 1;      // 10
        unsigned int sfrClkPol : 1;     // 11
        unsigned int sfrClkBypass : 1;      // 12
        unsigned int sfrClkSel : 1;         // 13
        unsigned int sfrClkRatio : 6;       // 14-19
        unsigned int reserved1 : 1;     // 20
        unsigned int nflClkEn : 1;      // 21
        unsigned int nflClkSel : 1;     // 22
        unsigned int nflClkBypass : 1;      // 23
        unsigned int nflClkDiv : 7;     // 24-30
    } asField;
    unsigned int as32Bits;
} P_CLK0_CTRL;

typedef union
{
    struct
    {
        unsigned int ispClkEn : 1;      // 0
        unsigned int ispClkSel : 1;     // 1
        unsigned int ispClkBypass : 1;      // 2
        unsigned int ispClkPol : 1;     // 3
        unsigned int ispClkDiv : 7;    // 4-10
        unsigned int sensorClkEn : 1;       // 11
        unsigned int sensorClkPol : 1;      // 12
        unsigned int sensorClkBypass : 1;       // 13
        unsigned int sensorClkRatio : 6;        // 14-19
        unsigned int sensorPclk0En : 1;        // 20
        unsigned int sensorPclk1En : 1;        // 21
        unsigned int sensorPclk2En : 1;        // 22
        unsigned int sensorPclk0Pol : 1;     // 23
        unsigned int sensorPclk1Pol : 1;        // 24
        unsigned int sensorPclk2Pol : 1;        // 25
        unsigned int sensorPclk0Src : 2;     // 26-27
        unsigned int sensorPclk1Src : 2;        // 28-29
        unsigned int sensorPclk2Src : 2;        // 30-31
    } asField;
    unsigned int as32Bits;
} P_CLK1_CTRL;

typedef union
{
    struct
    {
        unsigned int audClkEn : 1;      // 0
        unsigned int audClkPol : 1;     // 1
        unsigned int audClkDiv : 20;      // 2-21
    } asField;
    unsigned int as32Bits;
} P_CLK2_CTRL;

typedef union
{
    struct
    {
        unsigned int audLrckEn : 1;      // 0
        unsigned int audBickValue : 8;     // 1-8
        unsigned int audLrckValue : 6;      // 9-14
    } asField;
    unsigned int as32Bits;
} P_CLK3_CTRL;

typedef union
{
    struct
    {
        unsigned int rtcClkEn : 1;      // 0
        unsigned int rtcClkSel : 1;     // 1
        unsigned int rtcClkDiv : 20;      // 2-21
    } asField;
    unsigned int as32Bits;
} P_CLK4_CTRL;

typedef union
{
    struct
    {
        unsigned int rtcFixAdd : 1;      // 0
        unsigned int rtcFixValue : 20;     // 1-20
    } asField;
    unsigned int as32Bits;
} P_CLK5_CTRL;

typedef union
{
    struct
    {
        unsigned int lcdClkEn : 1;      // 0
        unsigned int lcdClkSrcSel : 2;      // 1-2      0:sys_pll_use, 1:dac_pll_use, 2:sens_pll_use, 3:lcd_aclk_use
        unsigned int lcdClkRatio : 8;       // 3-10
        unsigned int reserved0 : 5;     // 11-15
        unsigned int cecClkEn : 1;      // 16
        unsigned int cecClkRatio : 13;      // 17-29
    } asField;
    unsigned int as32Bits;
} P_CLK6_CTRL;

typedef union
{
    struct
    {
        unsigned int mrxClkEn : 1;        // 0
        unsigned int mrxClkSrcEn : 1;   // 1
        unsigned int mrxClkSrc : 2;       // 2-3
        unsigned int mrxClkBp : 1;       // 4
        unsigned int mrxClkSrcDivPre : 3;     // 5-7
        unsigned int mrxClkSrcDiv : 2;     // 8-9
        unsigned int mrxClkSel : 2;      // 10-11
        unsigned int mrxPclkSelSen0 : 1;     // 12
        unsigned int mrxPclkSelSen1 : 1;     // 13
        unsigned int mrxPclkSelSen2 : 1;     // 14
        unsigned int reserved0 : 1;     // 15
        unsigned int txClkEscClkEn : 1;        // 16
        unsigned int mtxClkEn : 1;        // 17
        unsigned int mtxClkSrc : 2;       // 18-19
        unsigned int txClkHsClkEn : 1;       // 20
        unsigned int reserved1 : 11;     // 21-31
    } asField;
    unsigned int as32Bits;
} P_CLK7_CTRL;

typedef union
{
    struct
    {
        unsigned int mndClkEn : 1;      // 0
        unsigned int mndClkBp : 1;      // 1
        unsigned int mndClkSrc : 2;     // 2-3      0:sys_pll_use, 1:sen_pll_use, 2:aud_pll_use, 3:osc_use
        unsigned int mndClkRatio : 6;       // 4-9
        unsigned int reserved0 : 1;     // 10
        unsigned int sdc0ClkEn : 1;     // 11
        unsigned int sdc0ClkBp : 1;     // 12
        unsigned int sdc0ClkSrc : 2;        // 13-14
        unsigned int sdc0ClkRatio : 6;      // 15-20
        unsigned int reserved1 : 1;     // 21
        unsigned int sdc1ClkEn : 1;     // 22
        unsigned int sdc1ClkBp : 1;     // 23
        unsigned int sdc1ClkSrc : 2;        // 24-25
        unsigned int sdc1ClkRatio : 6;      // 26-31
    } asField;
    unsigned int as32Bits;
} P_CLK8_CTRL;

typedef union
{
    struct
    {
        unsigned int reset : 1;      // 0
        unsigned int bypass : 1;    // 1
        unsigned int reserved0 : 2;     // 2-3
        unsigned int nf : 7;        // 4-10
        unsigned int reserved1 : 1;     // 11
        unsigned int no : 2;        // 12-13
        unsigned int reserved2 : 2;     // 14-15
        unsigned int nr : 5;        // 16-20
        unsigned int reserved3 : 7;     // 21-27
        unsigned int lock : 1;      // 28
    } asField;
    unsigned int as32Bits;
} PLL1600_PARAM;

typedef union
{
    struct
    {
        unsigned int reset : 1;     // 0
        unsigned int bypass : 1;        // 1
        unsigned int reserved0 : 2;     // 2-3
        unsigned int nf : 8;        // 4-11
        unsigned int no : 3;        // 12-14
        unsigned int reserved1 : 1;     // 15
        unsigned int nr : 5;        // 16-20
        unsigned int reserved2 : 3;      // 21-23
        unsigned int range : 3;     // 24-26
        unsigned int reserved3 : 1;     // 27
        unsigned int lock : 1;      // 28
    } asField;
    unsigned int as32Bits;
} PLL1200_PARAM;

typedef union
{
    struct
    {
        unsigned int txGroupBlock1FuncSet : 1;      // 0        0:tri-state, 1:drive data
        unsigned int txGroupBlock0FuncSet : 1;      // 1        0:tri-state, 1:drive data
        unsigned int txPullUpInGrpblkDriving : 6;       // 2-7
        unsigned int txPullDownInGrpblkDriving : 6;     // 8-13
        unsigned int txInTxOeLow : 2;       // 14-15
        unsigned int txTerminationHigh : 6;     // 16-21
        unsigned int txTerminationLow : 6;      // 22-27
        unsigned int rxConfiguration : 2;       // 28-29
        unsigned int rx1_2v1_8vSel : 1;       // 30
    } asField;
    unsigned int as32Bits;
} DMC_PAD_CTRL;

typedef union
{
    struct
    {
        unsigned int cpuClamp : 1;      // 0        Enables the clamp cells between VDD Core and VDD SoC
        unsigned int cfpClamp : 1;      // 1        Controls clamping logic between core and VFP
        unsigned int ramClamp : 1;      // 2        Enables the clamp cells in Dormant mode
    } asField;
    unsigned int as32Bits;
} ARM11_CNTR;

typedef union
{
    struct
    {
        unsigned int siprom_Q_RR : 8;
        unsigned int siprom_Q_SR : 8;
        unsigned int siprom_Q : 8;
    } asField;
    unsigned int as32Bits;
} OTP_STATUS_0;

typedef union
{
    struct
    {
        unsigned int siprom_Q_MRA : 16;
        unsigned int siprom_Q_MR : 16;
    } asField;
    unsigned int as32Bits;
} OTP_STATUS_1;

typedef union
{
    struct
    {
        unsigned int siprom_macro_set : 1;  // 0
        unsigned int siprom_status : 1;         // 1
        unsigned int siprom_pgm : 1;        // 2
        unsigned int siprom_read : 1;       // 3
        unsigned int reserved0 : 12;        // 4-15
        unsigned int siprom_Q_MRB : 16;     // 16-31
    } asField;
    unsigned int as32Bits;
} OTP_STATUS_2;

typedef union
{
    struct
    {
        unsigned int moviNand : 1;      // 0    movi module use the nfl signals
        unsigned int sramIf : 1;        // 1        hpi_addr[6:0] = gpio[14:8], hpi_addr[7] = hpi_addr, hpi_addr[10:8] = gpio[17:15], hpi_addr[12:11] = gb_gpio[1:0]
        unsigned int keypad : 1;        // 2        keypad use hpi_data[15:8] & gpio[7:0]
        unsigned int sensor : 1;        // 3        sensor_reset = gpio[198], sensor_pdn = gpio[199]
        unsigned int lcd : 1;       // 4            lcd_reset = gpio[200]
        unsigned int aud : 1;       // 5        aud_dac_reset = gpio[201]
        unsigned int dmb : 1;       // 6        dmb_reset = gpio[202], dmb_interrupt = gpio[203]
        unsigned int pwm_out1 : 1;      // 7    use gpio[115]
        unsigned int pwm_out2 : 1;      // 8    use gpio[121]
        unsigned int pwm_out3 : 1;      // 9    use gpio[100]
        unsigned int pwm_out4 : 1;      // 10   use gpio[191]
        unsigned int pwm_out5 : 1;      // 11   use gpio[204]
        unsigned int reserved0 : 4;     // 12-15
        unsigned int tunning : 1;       // 16
        unsigned int davinci_uart : 1;      // 17
    } asField;
    unsigned int as32Bits;
} ALT_FUNC_EN;

typedef union
{
    struct
    {
        unsigned int aps_busdn : 1;     // 0
        unsigned int aps_mcopy_end : 1;     // 1
    } asField;
    unsigned int as32Bits;
} APS_MODE;

typedef union
{
    struct
    {
        unsigned int isp_debug_test0 : 1;       // 0
        unsigned int isp_debug_test1 : 1;       // 1
    } asField;
    unsigned int as32Bits;
} ISP_DEBUG;

typedef union
{
    struct
    {
        unsigned int ext_int_en : 1;        // 0        External Power Interrupt Enable signal
        unsigned int ext_int_out : 2;       // 1-2      Formula-1 have 2 GPO for indicating power interrupt signal. These signals select the pad.
    } asField;
    unsigned int as32Bits;
} EXT_INT_CON;

typedef union
{
    struct
    {
        unsigned int mpsclk : 1;        // 0
        unsigned int mpdata : 1;        // 1
        unsigned int mpsync : 1;        // 2
        unsigned int mpvalid : 1;       // 3
        unsigned int uready1 : 1;       // 4
        unsigned int uclear1 : 1;       // 5
        unsigned int uready2 : 1;       // 6
        unsigned int uclear2 : 1;       // 7
        unsigned int nand_cen2 : 1;     // 8
        unsigned int nand_cen3 : 1;     // 9
        unsigned int sd1_cmd : 1;       // 10
        unsigned int sd1_dat0 : 1;      // 11
        unsigned int sd1_dat1 : 1;      // 12
        unsigned int sd1_dat2 : 1;      // 13
        unsigned int sd1_dat3 : 1;      // 14
        unsigned int sd1_chk : 1;       // 15
        unsigned int sd1_prt : 1;       // 16
        unsigned int usbdrv : 1;        // 17
        unsigned int usbdt : 1;     // 18
        unsigned int hpi_gpio3 : 1;     // 19
        unsigned int sen_gpio0 : 1;     // 20
        unsigned int sen_gpio1 : 1;     // 21
        unsigned int dip_csn2 : 1;      // 22
        unsigned int dip_csn3 : 1;      // 23
        unsigned int utx3 : 1;      // 24
        unsigned int urx3 : 1;      // 25
        unsigned int uready3 : 1;       // 26
        unsigned int uclear3 : 1;       // 27
        unsigned int spclk2 : 1;        // 28
        unsigned int spmosi2 : 1;       // 29
        unsigned int spmiso2 : 1;       // 30
        unsigned int spcs2 : 1;     // 31
    } asField;
    unsigned int as32Bits;
} EXT_INT_SRC;

typedef union
{
    struct
    {
        unsigned int intp_key03_sel : 4;        // 0-3      power interrupt detect using key_sense0~3
        unsigned int intp_key47_sel : 4;        // 4-7      power interrupt detect using key_sense4~7
        unsigned int intp_key03_pol : 4;        // 8-11     power interrupt polarity set for key_sense0~3
        unsigned int intp_key47_pol : 4;        // 12-15    power interrupt polarity set for key_sense4~7
    } asField;
    unsigned int as32Bits;
} INT_PWR_KEY;

#if defined(CONFIG_ARCH_MVFORMULA)
typedef union
{
	struct
	{
		unsigned int sbd_data_endianness : 1;   // 0 	ethernet controller endianess
		unsigned int sbd_pwr_down_ack : 1;      // 1     	power down mode anknowledge
		unsigned int mac_speed : 2;        	// 2-3     	mac speed indication
		unsigned int phy_clk_en : 1;		// 4   	internal clock enable
		unsigned int phy_clk_ratio : 6;		// 5-10    internal clock divider configuration
		unsigned int phy_pll_sel : 2;		// 11-12	pll selection
		unsigned int reserved : 19;		// 13-31	reserved
	} asField;
	unsigned int as32Bits;
} ETH_CTRL;
#endif
// main structure
typedef struct
{
    unsigned int    productID;          // 0x00     0x5A454E31 ("ZEN1)
    SYS_CTRL_0     sysCtrl0;           // 0x04
    SYS_CTRL_1      sysCtrl1;           // 0x08
    SYS_CTRL_2      sysCtrl2;           // 0x0C
    G_RESET_CTRL    gResetCtrl;         // 0x10
    RESET_CTRL0     resetCtrl0;         // 0x14
    RESET_CTRL1     resetCtrl1;         // 0x18
    RESET_CTRL2     resetCtrl2;         // 0x1C
    G_CLOCK_CTRL    gClockCtrl;         // 0x20
    CLOCK_CTRL0     clockCtrl0;         // 0x24
    CLOCK_CTRL1     clockCtrl1;         // 0x28
    PLL_SRC_SEL     pllSrcSel;          // 0x2C
    MAIN_CLK_SEL    mainClkSel;         // 0x30
    CLK_BYPASS      clkBypass;          // 0x34
    CLK_DIV         clkDiv[5];          // 0x38-0x48
    SYS_CLK_DIV     sysClkDiv;          // 0x4C
    P_CLK0_CTRL     pClk0Ctrl;          // 0x50
    P_CLK1_CTRL     pClk1Ctrl;          // 0x54
    P_CLK2_CTRL     pClk2Ctrl;          // 0x58
    P_CLK3_CTRL     pClk3Ctrl;          // 0x5C
    P_CLK4_CTRL     pClk4Ctrl;          // 0x60
    P_CLK5_CTRL     pClk5Ctrl;          // 0x64
    P_CLK6_CTRL     pClk6Ctrl;          // 0x68
    P_CLK7_CTRL         pClk7Ctrl;          // 0x6C
    PLL1600_PARAM       pll0Param;        // 0x70
    PLL1200_PARAM       pll1Param;      // 0x74
    PLL1200_PARAM       pll2Param;      // 0x78
    PLL1200_PARAM       pll3Param;      // 0x7C
    PLL1200_PARAM       pll4Param;      // 0x80
    unsigned int    reserved84;         // 0x84
    P_CLK8_CTRL     pClk8Ctrl;          // 0x88
    unsigned int    reserved8C;         // 0x8C
    unsigned int            peCtrl[7];          // 0x90-0xA8
    unsigned int            reservedAC;
    DMC_PAD_CTRL    dmcPadCtrl0;         // 0xB0
    DMC_PAD_CTRL   dmcPadCtrl1;        // 0xB4
    unsigned int    reservedB8_BC[2];       // 0xB8-0xBC
    ARM11_CNTR          arm11Cntr;          // 0xC0
    unsigned int    reservedC4_CC[3];       // 0xC4-0xCC
    OTP_STATUS_0       otpSts0;                    // 0xD0
    OTP_STATUS_1       otpSts1;                    // 0xD4
    OTP_STATUS_2       otpSts2;                    // 0xD8
    unsigned int    reservedDC;                 // 0xDC
    ALT_FUNC_EN     altFuncEn;              // 0xE0
    APS_MODE        apsMode;                // 0xE4
    ISP_DEBUG       ispDebug;               // 0xE8
#if defined(CONFIG_ARCH_MVFORMULA)
    	ETH_CTRL	ethCtrl;		// 0xEC
#else
    unsigned int    reservedEC;             // 0xEC
#endif // defined(CONFIG_ARCH_MVFORMULA)    EXT_INT_CON     extIntCon;          // 0xF0
    EXT_INT_SRC     intPwrSel;              // 0xF4
    EXT_INT_SRC     intPwrPol;              // 0xF8
    INT_PWR_KEY     intPwrKey;              // 0xFC
} SYSTEM_REGISTERS;

#endif /* __ASM_ARCH_REGS_SYS_H */

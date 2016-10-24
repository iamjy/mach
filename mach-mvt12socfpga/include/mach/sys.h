/*
 *  arch/arm/mach-mvt12socfpga/include/mach/sys.h
 *
 *  Copyright (C) 2010 MtekVision Co., Ltd.
 *	Jinyoung Park <parkjy@mtekvision.com>
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
#ifndef __ASM_ARCH_SYS_H
#define __ASM_ARCH_SYS_H


/*
 *      Defines types for Clock
 */

typedef enum
{
    SRC_ISP_BUS = 0,
    SRC_ISP_PLL_SRC3,
    SRC_SEN_PLL_SRC3,
    SRC_SCLK_MAX = 0x7FFFFFFF

}SCLK_SRC_TYPE;

typedef enum
{
    SRC_SENSOR_PCLK,    // NOMAL PARALLEL SENSOR, THEN USE THIS OPTION
    SRC_MIPI_RX_CLK,     // IF  USE MIPI SENSOR, THEN USE THIS OPTION
    SRC_BUS_CLK,            // IF  USE READ PATH, THEN USE THIS OPTION
    SRC_SCLK                   // NOMAL PARALLEL SENSOR, BUT MUST BE SCLK = PCLK, THEN USE THIS OPTION, THIS OPTION MAY BE RARELY USED!!

}SENx_SRC_TYPE;

typedef enum
{
    SRC_PLL0,
    SRC_PLL1,
    SRC_PLL2,
    SRC_PLL3,
    SRC_NO_PLL

}MIPI_SRC_PLL_TYPE;

typedef enum
{
    SRC_MRX_CLK_SRC_DIV,
    SRC_MRX_DDR_4,			// MIPI RX 4 LANE
    SRC_MRX_DDR_2,			// MIPI RX 2 LANE
    SRC_MRX_DDR				// MIPI RX 1 LANE

}MIPI_RX_SRC_TYPE;

typedef enum
{
    SERIAL_FREQ_80_100,
    SERIAL_FREQ_100_120,
    SERIAL_FREQ_120_160,
    SERIAL_FREQ_160_200,
    SERIAL_FREQ_200_240,
    SERIAL_FREQ_240_320,
    SERIAL_FREQ_320_390,
    SERIAL_FREQ_390_450,
    SERIAL_FREQ_450_510,
    SERIAL_FREQ_510_560,
    SERIAL_FREQ_560_640,
    SERIAL_FREQ_640_690,
    SERIAL_FREQ_690_770,
    SERIAL_FREQ_770_870,
    SERIAL_FREQ_870_950,
    SERIAL_FREQ_950_1000,

    SERIAL_FREQ_INVALID = 0x7FFFFFFF

}MIPI_RX_DPHY_SERAIL_CLK_RANGE;


/*
 * Global index for clock power
 */
#define SYS_GB_CLK_ALL			0
#define SYS_GB_CLK_ARM			1
#define SYS_GB_CLK_BUS0			2
#define SYS_GB_CLK_BUS1			3
#define SYS_GB_CLK_MBUS0		4
#define SYS_GB_CLK_MBUS1		5
#define SYS_GB_CLK_ARM_I		6
#define SYS_GB_CLK_ARM_RW		7
#define SYS_GB_CLK_ARM_DAM		8
#define SYS_GB_CLK_ARM_P		9

#define SYS_GB_CLK_NUMBER		10

/*
 * Device index for clock power
 */
#define SYS_CLK_HPI			0
#define SYS_CLK_DMC			1
#define SYS_CLK_DMA			2
#define SYS_CLK_INT			3
#define SYS_CLK_SCALER			4
#define SYS_CLK_IMG_CONV		5
#define SYS_CLK_DIP			6
#define SYS_CLK_CRPTO			7
#define SYS_CLK_JPEG_ENC		8
#define SYS_CLK_JPEG_DEC		9
#define SYS_CLK_USB			10
#define SYS_CLK_DAVINCI			11
#define SYS_CLK_PDS			12
#define SYS_CLK_ISP			13
#define SYS_CLK_MALI			14
#define SYS_CLK_MIPI			15
#define SYS_CLK_VP6			16
#define SYS_CLK_DMC_1X			17
#define SYS_CLK_DMC_2X			18
#if defined(CONFIG_ARCH_MVFORMULA)
#define SYS_CLK_ETH			18
#endif

#define SYS_CLK_I_RXBYTE_CLK_HS0	21
#define SYS_CLK_I_RXBYTE_CLK_HS1	22
#define SYS_CLK_I_RXBYTE_CLK_HS2	23
#define SYS_CLK_I_RXBYTE_CLK_HS3	24
#define SYS_CLK_I_RX_CLK_ESC0		25
#define SYS_CLK_I_RX_CLK_ESC1		26
#define SYS_CLK_I_RX_CLK_ESC2		27
#define SYS_CLK_I_RX_CLK_ESC3		28
#define SYS_CLK_I_TX_CLK_LANE_HS	29
#define SYS_CLK_I_TXBYTE_CLK_HS0	30
#define SYS_CLK_I_TXBYTE_CLK_HS1	31

#define SYS_CLK_APB			32
#define SYS_CLK_SYS			33
#define SYS_CLK_NAND			34
#define SYS_CLK_RTC			35
#define SYS_CLK_SPI0			36
#define SYS_CLK_MP2TS			37
#define SYS_CLK_UART0			38
#define SYS_CLK_KEY			39
#define SYS_CLK_PWM			40
#define SYS_CLK_I2C0			41
#define SYS_CLK_AUDIO			42
#define SYS_CLK_MMCSD0			43
#define SYS_CLK_MMCSD1			44
#define SYS_CLK_MOVI			45
#define SYS_CLK_SMART			46
#define SYS_CLK_SPI1			47
#define SYS_CLK_SPI2			48
#define SYS_CLK_UART1			49
#define SYS_CLK_UART2			50
#define SYS_CLK_UART3			51
#define SYS_CLK_I2C1			52
#define SYS_CLK_I2C2			53

#define SYS_CLK_NUMBER			54

/*
 * Global index for reset
 */
#define SYS_GB_RST_ALL			0
#define SYS_GB_RST_ARM			1
#define SYS_GB_RST_BUS0			2
#define SYS_GB_RST_BUS1			3
#define SYS_GB_RST_MBUS0		4
#define SYS_GB_RST_MBUS1		5
#define SYS_GB_RST_ARM_I		6
#define SYS_GB_RST_ARM_RW		7
#define SYS_GB_RST_ARM_DMA		8
#define SYS_GB_RST_ARM_P		9
#define SYS_GB_RST_ARM_V		10

#define SYS_GB_RST_NUMBER		11

/*
 * Device index for reset
 */
#define SYS_RST_HPI			0
#define SYS_RST_DMC			1
#define SYS_RST_DMA			2
#define SYS_RST_INT			3
#define SYS_RST_SCALER			4
#define SYS_RST_IMG_CONV		5
#define SYS_RST_DIP			6
#define SYS_RST_CRPTO			7
#define SYS_RST_JPEG_ENC		8
#define SYS_RST_JPEG_DEC		9
#define SYS_RST_USB			10
#define SYS_RST_DAVINCI			11
#define SYS_RST_PDS			12
#define SYS_RST_ISP			13
#define SYS_RST_MALI			14
#define SYS_RST_MIPI			15
#define SYS_RST_VP6			16
#if defined(CONFIG_ARCH_MVFORMULA)
#define SYS_RST_ETH			18
#endif

#define SYS_RST_APB			32
#define SYS_RST_SYS			33
#define SYS_RST_NAND			34
#define SYS_RST_RTC			35
#define SYS_RST_SPI0			36
#define SYS_RST_MP2TS			37
#define SYS_RST_UART0			38
#define SYS_RST_KEY			39
#define SYS_RST_PWM			40
#define SYS_RST_I2C0			41
#define SYS_RST_AUDIO			42
#define SYS_RST_MMCSD0			43
#define SYS_RST_MMCSD1			44
#define SYS_RST_MOVI			45
#define SYS_RST_SMART			46
#define SYS_RST_SPI1			47
#define SYS_RST_SPI2			48
#define SYS_RST_UART1			49
#define SYS_RST_UART2			50
#define SYS_RST_UART3			51
#define SYS_RST_I2C1			52
#define SYS_RST_I2C2			53

#define SYS_RST_USB_CORE		64
#define SYS_RST_USB_PHY			65
#define SYS_RST_MPTPHY			66
#define SYS_RST_MPRPHY			67
#define SYS_RST_TVE			68
#define SYS_RST_TVE_SLR			69
#define SYS_RST_HDMI			70
#define SYS_RST_UTMI_PHY		70

#define SYS_RST_NUMBER			71

/*
 * alternative functions
 */
#define SYS_ALT_FUNC_MOVI		0
#define SYS_ALT_FUNC_HPI		1
#define SYS_ALT_FUNC_KEY		2
#define SYS_ALT_FUNC_SENSOR		3
#define SYS_ALT_FUNC_LCD		4
#define SYS_ALT_FUNC_AUDIO		5
#define SYS_ALT_FUNC_DMB		6
#define SYS_ALT_FUNC_PWM1		7
#define SYS_ALT_FUNC_PWM2		8
#define SYS_ALT_FUNC_PWM3		9
#define SYS_ALT_FUNC_PWM4		10
#define SYS_ALT_FUNC_PWM5		11
#define SYS_ALT_FUNC_TUNNER		16
#define SYS_ALT_FUNC_DAVINCI		17

#define SYS_ALT_FUNC_NUMBER		18

/* PLL Source Select */
#define SYS_PLL_ID_0			0
#define SYS_PLL_ID_1			1
#define SYS_PLL_ID_2			2
#define SYS_PLL_ID_3			3
#define SYS_PLL_ID_4			4
#define SYS_PLL_ID_OSC			5

/* DMC Clock Source */
#define SYS_DMA_ALT_SRC_DAC		0
#define SYS_DMA_ALT_SRC_USB		1
#define SYS_DMA_ALT_SRC_SENSOR		2
#define SYS_DMA_ALT_SRC_AUDIO		3

/* NAND Clock Source */
#define SYS_NAND_CLK_SRC_OSC		0
#define SYS_NAND_CLK_SRC_APB		1

/* SFR Clock Source */
#define SYS_SFR_CLK_SRC_SYS		0
#define SYS_SFR_CLK_SRC_DAC		1

/* LCD Clock Source */
#define SYS_LCD_CLK_SRC_SYS		0
#define SYS_LCD_CLK_SRC_DAC		1
#define SYS_LCD_CLK_SRC_SENSOR		2
#define SYS_LCD_CLK_SRC_BUS		3

/* SD and MOVI Clock Source */
#define SYS_SD_CLK_SRC_SYS		0
#define SYS_SD_CLK_SRC_SENSOR		1
#define SYS_SD_CLK_SRC_AUDIO		2
#define SYS_SD_CLK_SRC_OSC		3

struct mv_pll_range {
	u32 min;
	u32 max;
};

struct mv_pll_param {
	u32 fout;
	u8 nr;
	u8 no;
	u8 nf;
	u8 range;
};

struct mv_chip_info {
	u32 id;
	u8 mem_type;
	u8 mem_maskbit;
	u8 mem_vendor;
};

struct mv_power_info {
	u32 dev_id;
	u8 en;
};

struct mv_sys_clk_param {
	/* pll */
	int nr;
	int no;
	int nf;
	/* div */
	int sys;
	int arm;
	int bus;
	int dmc1;
	int dmc2;
	int apb;
};

u32 mv_sys_get_osc_clk_mhz(void);
int mv_sys_get_pll_param(int pll_id, u32 pll_out_hz,
		struct mv_pll_param *pll_param);
u32 mv_sys_get_pll_clk_hz(int pll_id);
int mv_sys_set_pll_clk_hz(int pll_id, u32 pll_out_hz);

u32 mv_sys_get_sys_clk_hz(void);
int mv_sys_set_sys_clk_hz(int pll_id, u32 pll_out_hz, int sys_div, int arm_div,
		int bus_div, int dmc1_div, int dmc2_div, int apb_div);
#if defined(CONFIG_MVFORMULA_PM_DYNAMIC_CLOCK)
int mv_sys_change_sys_clk(struct mv_sys_clk_param *);
#endif	/*CONFIG_MVFORMULA_PM_DYNAMIC_CLOCK*/
u32 mv_sys_get_arm_clk_hz(void);
u32 mv_sys_get_bus_clk_hz(void);
u32 mv_sys_get_apb_clk_hz(void);
u32 mv_sys_get_dmc1_clk_hz(void);
u32 mv_sys_get_dmc2_clk_hz(void);
u32 mv_sys_get_mem_clk_hz(void);

u32 mv_sys_get_usb_clk_hz(void);
int mv_sys_set_usb_clk_hz(int pll_id, u32 pll_out_hz, int usb_div);

u32 mv_sys_get_audio_clk_hz(void);
int mv_sys_set_audio_clk_hz(int pll_id, u32 pll_out_hz, int audio_div);
void mv_sys_audio_nco_ctrl(int en);

u32 mv_sys_get_dac_clk_hz(void);
int mv_sys_set_dac_clk_hz(int pll_id, u32 pll_out_hz, int dac_div);

u32 mv_sys_get_nand_clk_hz(void);
void mv_sys_set_nand_clk(int clk_src, int div);
void mv_sys_nand_clk_ctrl(int en);

u32 mv_sys_get_lcd_clk_hz(void);
void mv_sys_set_lcd_clk(int clk_src, int div);
void mv_sys_lcd_clk_ctrl(int en);

u32 mv_sys_get_sd0_clk_hz(void);
void mv_sys_set_sd0_clk(int clk_src, int div);
void mv_sys_sd0_clk_ctrl(int en);

u32 mv_sys_get_sd1_clk_hz(void);
void mv_sys_set_sd1_clk(int clk_src, int div);
void mv_sys_sd1_clk_ctrl(int en);

u32 mv_sys_get_movi_clk_hz(void);
void mv_sys_set_movi_clk(int clk_src, int div);
void mv_sys_movi_clk_ctrl(int en);

u32 mv_sys_get_sensor_clk_hz(void);
int mv_sys_set_sensor_clk_hz(int pll_id, u32 pll_out_hz, int sensor_div);
int mv_sys_sensor_sclk_enable(int type, u32 div, int polarity);
u32 mv_sys_disable_sensor_sclk(void);
u32 mv_sys_enable_sensor0_clk(SENx_SRC_TYPE type, bool mIsInvPolarity);
u32 mv_sys_disable_sensor0_clk(void);
u32 mv_sys_enable_sensor1_clk(SENx_SRC_TYPE type, bool mIsInvPolarity);
u32 mv_sys_disable_sensor1_clk(void);
u32 mv_sys_enable_isp_clk(SENx_SRC_TYPE type, bool mIsInvPolarity);
u32 mv_sys_disable_isp_clk(void);
u32 mv_sys_disable_mipi_rx_clk(void);
u32 mv_sys_enable_mipi_tx_clk(MIPI_SRC_PLL_TYPE mipiPll);
u32 mv_sys_disable_mipi_tx_clk(void);

void mv_sys_set_gb_clk_power(int no, int ena);
int mv_sys_get_dev_clk_power(int no);
void mv_sys_set_dev_clk_power(int no, int ena);

void mv_sys_gb_reset(int no);
void mv_sys_dev_reset(int no);

#if 1 /* leehh 100819  */
void mv_sys_dev_usb_set(int no);
void mv_sys_dev_usb_unset(int no);
#endif

void mv_sys_set_alt_func(int no, int ena);
void mv_sys_pad_ctrl(int no, int val);

struct mv_chip_info mv_sys_get_chip_info(void);

void mv_sys_hdmi_pwr_cut_ctrl(int en);
void mv_sys_set_sfr_clk(int clk_src, int div, int polarity);
void mv_sys_sfr_clk_ctrl(int en);

void mv_sys_set_sfr_clk_en(u32 value);
void mv_sys_set_cec_clk_ratio(u32 ratio);
void mv_sys_set_cec_clk_en(u32 value);

void mv_sys_set_tv_clk_ratio(u32 ratio);
void mv_sys_set_tv_clk_bypass(u32 value);
void mv_sys_set_tv_clk_enable(u32 value);

void mv_sys_dump_regs(void);

#if defined(CONFIG_ARCH_MVFORMULA)
void mv_sys_set_enet_init(void);
#endif

#endif /* __ASM_ARCH_SYS_H */

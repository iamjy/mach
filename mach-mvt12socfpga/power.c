/*
 *  arch/arm/mach-mvformula/power.c
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
#include <linux/init.h>
#include <linux/device.h>
#include <mach/board.h>
#include <mach/sys.h>

static struct mv_power_info power_info[] = {
	{ SYS_CLK_SYS, 1 },
	{ SYS_CLK_APB, 1 },
	{ SYS_CLK_I2C0, 0 },
	{ SYS_CLK_SPI0, 0 },
	{ SYS_CLK_RTC, 0 },
	{ SYS_CLK_INT, 1 },
	{ SYS_CLK_DMC, 1 },
#if defined(CONFIG_MFP_7V2000T_Q_DEBUG_UART0)
	{ SYS_CLK_UART0, 1 },
#else
	{ SYS_CLK_UART0, 0 },
#endif /* defined(CONFIG_DEBUG_UART0) */
	{ SYS_CLK_PWM, 1 },

#if defined(CONFIG_FORMULA_DEBUG_UART1)
	{ SYS_CLK_UART1, 1 },
#else
	{ SYS_CLK_UART1, 0 },
#endif /* defined(CONFIG_DEBUG_UART1) */
#if defined(CONFIG_FORMULA_DEBUG_UART2)
	{ SYS_CLK_UART2, 1 },
#else
	{ SYS_CLK_UART2, 0 },
#endif /* defined(CONFIG_DEBUG_UART2) */
#if defined(CONFIG_FORMULA_DEBUG_UART3)
	{ SYS_CLK_UART3, 1 },
#else
	{ SYS_CLK_UART3, 0 },
#endif /* defined(CONFIG_DEBUG_UART3) */
};

void __init mv_init_common_power(void)
{
	int i;
	int power_info_size = ARRAY_SIZE(power_info);

	for (i = 0; i < power_info_size; i++)
		mv_sys_set_dev_clk_power(power_info[i].dev_id, power_info[i].en);
}


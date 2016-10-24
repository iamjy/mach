/*
 *  arch/arm/mach-mvformula/pwm.c
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

#pragma GCC optimize("0")

#include <linux/kernel.h>
#include <linux/init.h>
// #include <linux/sysdev.h>
#include <linux/io.h>
#include <linux/time.h>

#include <asm/param.h>

#include <mach/regs-pwm.h>
#include <mach/sys.h>
#include <mach/pwm.h>

#define DRV_NAME	"mvformula-pwm"

static int pwm_hw_init;

static void mv_pwm_hw_init(void)
{
	mv_sys_set_dev_clk_power(SYS_CLK_PWM, 1);
	mv_sys_dev_reset(SYS_RST_PWM);
	pwm_hw_init = 1;
}

/* id : PWM index
 * usec : Timer interval time in usec
 * duty_cycle : Duty cycle of pulse frequency in percentage
 * return : PWM source clock in Hz */
u32 mv_pwm_config_usec(int id, unsigned int usec, int duty_cycle)
{
	u32 cntb, val;
	u32 prescaler, div;
	u32 apb_clk_hz = mv_sys_get_apb_clk_hz();
	u32 req_hz = USEC_PER_SEC / usec;
	int find = 0;

	pr_info("[NOT_CLKEVT] apb_clk_hz %u reqhz %u\n", apb_clk_hz, req_hz);

	if (pwm_hw_init == 0)
		mv_pwm_hw_init();

	for (div = 1; div <= 16; div *= 2) {
		cntb = (apb_clk_hz / div);
		if (cntb % req_hz != 0)
			continue;
		cntb /= req_hz;
		for (prescaler = 1; prescaler <= 256; prescaler++) {
			if ((cntb / prescaler) > PWM_TCNTB_CNT_BUF_WIDTH)
				continue;
			if (((cntb % prescaler) == 0) &&
					(((cntb / prescaler) % req_hz) == 0)) {
				find = 1;
				goto find_div;
			}
		}
	}
	
	pr_info("[NOT_CLKEVT] div %u prescaler %u cntb %u find %u \n",
			div, prescaler, cntb, find);

	if (find == 0) {
		pr_err("%s: Not find matched PWM%d %u usec\n",
			DRV_NAME, id, usec);
		BUG();
	}

find_div:
	cntb /= prescaler;
	prescaler--;
	
	pr_info("[NOT_CLKEVT] cntb /= prescaler %u\n", cntb);
	pr_info("[NOT_CLKEVT] prescaler-- %u\n", prescaler);

	if ((id >= PWM_ID_0) && (id <= PWM_ID_2)) {
		val = readl(PWM_VA_BASE + PWM_CFG0);
		val &= ~(PWM_CFG_PRESCALER0_WIDTH << (id << 3));
		val |= (prescaler << (id << 3));
		writel(val, PWM_VA_BASE + PWM_CFG0);
	} else {
		val = readl(PWM_VA_BASE + PWM_CFG1);
		val &= ~(PWM_CFG_PRESCALER0_WIDTH << ((id - 3) << 3));
		val |= (prescaler  << ((id - 3) << 3));
		writel(val, PWM_VA_BASE + PWM_CFG1);
	}

	val = readl(PWM_VA_BASE + PWM_CFG2);
	val &= ~(PWM_CFG_T0_CLK_SEL_WIDTH << (id << 2));
	if (div == 1)
		val |= (PWM_CLK_SEL_DIV1 << (id << 2));
	else if (div == 2)
		val |= (PWM_CLK_SEL_DIV2 << (id << 2));
	else if (div == 4)
		val |= (PWM_CLK_SEL_DIV4 << (id << 2));
	else if (div == 8)
		val |= (PWM_CLK_SEL_DIV8 << (id << 2));
	else
		val |= (PWM_CLK_SEL_DIV16 << (id << 2));
	writel(val, PWM_VA_BASE + PWM_CFG2);

	val = readl(PWM_VA_BASE + PWM_CTRL);
	if (id == PWM_ID_0)
		val |= PWM_CTRL_T0_UPDATE_MASK;
	else
		val |= (PWM_CTRL_T0_UPDATE_WIDTH << (1 + ((id + 1) << 2)));
	writel(val, PWM_VA_BASE + PWM_CTRL);

	val = cntb & PWM_TCNTB_CNT_BUF_WIDTH;
	writel(val, PWM_VA_BASE + PWM_TCNTB0 + (id * 12));

	val = (cntb * duty_cycle / 100) & PWM_TCMPB_COM_BUF_WIDTH;
	writel(val, PWM_VA_BASE + PWM_TCMPB0 + (id * 12));

	val = readl(PWM_VA_BASE + PWM_CTRL);
	if (id == PWM_ID_0)
		val &= ~PWM_CTRL_T0_UPDATE_MASK;
	else
		val &= ~(PWM_CTRL_T0_UPDATE_WIDTH << (1 + ((id + 1) << 2)));
	writel(val, PWM_VA_BASE + PWM_CTRL);

	return (u32)((apb_clk_hz / div) / (prescaler + 1));
}
EXPORT_SYMBOL(mv_pwm_config_usec);

u32 mv_pwm_init_usec(int id, unsigned int div, unsigned int prescaler, unsigned int cntb, int duty_cycle)
{
	u32  val;
	u32 apb_clk_hz = mv_sys_get_apb_clk_hz();


	if ((id >= PWM_ID_0) && (id <= PWM_ID_2)) {
		val = readl(PWM_VA_BASE + PWM_CFG0);
		val &= ~(PWM_CFG_PRESCALER0_WIDTH << (id << 3));
		val |= (prescaler << (id << 3));
		writel(val, PWM_VA_BASE + PWM_CFG0);
	} else {
		val = readl(PWM_VA_BASE + PWM_CFG1);
		val &= ~(PWM_CFG_PRESCALER0_WIDTH << ((id - 3) << 3));
		val |= (prescaler  << ((id - 3) << 3));
		writel(val, PWM_VA_BASE + PWM_CFG1);
	}

	val = readl(PWM_VA_BASE + PWM_CFG2);
	val &= ~(PWM_CFG_T0_CLK_SEL_WIDTH << (id << 2));
	if (div == 1)
		val |= (PWM_CLK_SEL_DIV1 << (id << 2));
	else if (div == 2)
		val |= (PWM_CLK_SEL_DIV2 << (id << 2));
	else if (div == 4)
		val |= (PWM_CLK_SEL_DIV4 << (id << 2));
	else if (div == 8)
		val |= (PWM_CLK_SEL_DIV8 << (id << 2));
	else
		val |= (PWM_CLK_SEL_DIV16 << (id << 2));
	writel(val, PWM_VA_BASE + PWM_CFG2);

	val = readl(PWM_VA_BASE + PWM_CTRL);
	if (id == PWM_ID_0)
		val |= PWM_CTRL_T0_UPDATE_MASK;
	else
		val |= (PWM_CTRL_T0_UPDATE_WIDTH << (1 + ((id + 1) << 2)));
	writel(val, PWM_VA_BASE + PWM_CTRL);

	val = cntb & PWM_TCNTB_CNT_BUF_WIDTH;
	writel(val, PWM_VA_BASE + PWM_TCNTB0 + (id * 12));

	val = (cntb * duty_cycle / 100) & PWM_TCMPB_COM_BUF_WIDTH;
	writel(val, PWM_VA_BASE + PWM_TCMPB0 + (id * 12));

	val = readl(PWM_VA_BASE + PWM_CTRL);
	if (id == PWM_ID_0)
		val &= ~PWM_CTRL_T0_UPDATE_MASK;
	else
		val &= ~(PWM_CTRL_T0_UPDATE_WIDTH << (1 + ((id + 1) << 2)));
	writel(val, PWM_VA_BASE + PWM_CTRL);

	return (u32)((apb_clk_hz / div) / (prescaler + 1));
}
EXPORT_SYMBOL(mv_pwm_init_usec);

/* id : PWM index
 * hz : PWM cycle frequency in Hz
 * duty_cycle : Duty cycle of pulse frequency in percentage
 * return : PWM source clock in Hz */
u32 mv_pwm_config_hz(int id, unsigned int hz, int duty_cycle)
{
	u32 val, in_clk_hz;
	u32 prescaler, div;
	u32 prescaler_keep = 1, div_keep = 1;
	u32 apb_clk_hz = mv_sys_get_apb_clk_hz();
	int find = 0;

	if (pwm_hw_init == 0)
		mv_pwm_hw_init();

	for (div = 16; div >= 1; div /= 2) {
		in_clk_hz = (apb_clk_hz / div);
		for (prescaler = 256; prescaler >= 1; prescaler--) {
			if ((in_clk_hz / prescaler / HZ) >
					PWM_TCNTB_CNT_BUF_WIDTH)
				continue;

			if ((in_clk_hz / prescaler) == hz) {
				prescaler_keep = prescaler;
				div_keep = div;
				find = 2;
				goto find_div;
			}

			if ((in_clk_hz / prescaler) < hz) {
				prescaler_keep = prescaler;
				div_keep = div;
				find = 1;
			}
		}
	}

	if (find == 0) {
		pr_err("%s: Not find matched PWM%d clock, ReqClk=%uHz\n",
			DRV_NAME, id, hz);
		BUG();
	} else if (find == 1) {
		pr_warning("%s: Not matched PWM%d clock, ReqClk=%uHz, "
			"RealClk=%uHz, prescaler=%u, div=%u\n", DRV_NAME,
			id, hz, (apb_clk_hz / prescaler_keep / div_keep),
			prescaler_keep, div_keep);
	}

find_div:
	if ((id >= PWM_ID_0) && (id <= PWM_ID_2)) {
		val = readl(PWM_VA_BASE + PWM_CFG0);
		val &= ~(PWM_CFG_PRESCALER0_WIDTH << (id << 3));
		val |= ((prescaler_keep - 1) << (id << 3));
		writel(val, PWM_VA_BASE + PWM_CFG0);
	} else {
		val = readl(PWM_VA_BASE + PWM_CFG1);
		val &= ~(PWM_CFG_PRESCALER0_WIDTH << ((id - 3) << 3));
		val |= ((prescaler_keep - 1)  << ((id - 3) << 3));
		writel(val, PWM_VA_BASE + PWM_CFG1);
	}

	val = readl(PWM_VA_BASE + PWM_CFG2);
	val &= ~(PWM_CFG_T0_CLK_SEL_WIDTH << (id << 2));
	if (div_keep == 1)
		val |= (PWM_CLK_SEL_DIV1 << (id << 2));
	else if (div_keep == 2)
		val |= (PWM_CLK_SEL_DIV2 << (id << 2));
	else if (div_keep == 4)
		val |= (PWM_CLK_SEL_DIV4 << (id << 2));
	else if (div_keep == 8)
		val |= (PWM_CLK_SEL_DIV8 << (id << 2));
	else
		val |= (PWM_CLK_SEL_DIV16 << (id << 2));
	writel(val, PWM_VA_BASE + PWM_CFG2);

	return (u32)((apb_clk_hz / div_keep) / (prescaler_keep));
}
EXPORT_SYMBOL(mv_pwm_config_hz);


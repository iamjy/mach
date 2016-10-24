/*
 *  arch/arm/mach-mvzenith/include/mach/pwm.h
 *
 *  Copyright (C) 2010 MtekVision Ltd.
 *  	Jinyoung Park <parkjy@mtekvision.com>
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

#ifndef __ASM_ARCH_PWM_H
#define __ASM_ARCH_PWM_H

#include <mach/map.h>
#include <mach/regs-pwm.h>

#define PWM_ID_0	0
#define PWM_ID_1	1
#define PWM_ID_2	2
#define PWM_ID_3	3
#define PWM_ID_4	4
#define PWM_ID_5	5

static inline void mv_pwm_int_clear(int id)
{
	writel((1 << id), PWM_VA_BASE + PWM_INT_CLR);
}

static inline int mv_pwm_get_int_status(int id)
{
	return (readl(PWM_VA_BASE + PWM_INT_STATUS) & (1 << id)) ? 1 : 0;
}

/* Get count buffer value for PWM ID */
static inline unsigned int mv_pwm_get_cnt_buf(int id)
{
	return (unsigned int)(readl(PWM_VA_BASE + PWM_TCNTB0 + (id * 12))
		& PWM_TCNTB_CNT_BUF_WIDTH);
}

/* Set count buffer value for PWM ID */
static inline void mv_pwm_set_cnt_buf(int id, unsigned int cntb,
		unsigned int duty_cycle)
{
	unsigned int update_shift, offset = (id * 12);
	unsigned int ctrl_val;

	if (id == PWM_ID_0)
		update_shift = PWM_CTRL_T0_UPDATE_SHIFT;
	else
		update_shift = (PWM_CTRL_T0_UPDATE_SHIFT + ((id + 1) << 2));

	ctrl_val = readl(PWM_VA_BASE + PWM_CTRL);
	ctrl_val |= (PWM_CTRL_T0_UPDATE_WIDTH << update_shift);
	writel(ctrl_val, PWM_VA_BASE + PWM_CTRL);

	writel(cntb, PWM_VA_BASE + PWM_TCNTB0 + offset);
	writel((cntb * duty_cycle / 100), PWM_VA_BASE + PWM_TCMPB0 + offset);

	ctrl_val &= ~(PWM_CTRL_T0_UPDATE_WIDTH << update_shift);
	writel(ctrl_val, PWM_VA_BASE + PWM_CTRL);
}

/* Get count observation value for PWM ID */
static inline unsigned int mv_pwm_get_cnt_obs(int id)
{
	return (unsigned int)(readl(PWM_VA_BASE + PWM_TCNTO0 + (id * 12))
		& PWM_TCNTO_CNT_OBS_WIDTH);
}

static inline void mv_pwm_start(int id, int oneshot)
{
	unsigned int auto_shift, start_shift;
	unsigned int val;

	if (id == PWM_ID_0) {
		auto_shift = PWM_CTRL_T0_AUTO_SHIFT;
		start_shift = PWM_CTRL_T0_START_SHIFT;
	} else {
		auto_shift = (PWM_CTRL_T0_AUTO_SHIFT + ((id + 1) << 2));
		start_shift = (PWM_CTRL_T0_START_SHIFT + ((id + 1) << 2));
	}

	val = readl(PWM_VA_BASE + PWM_CTRL);
	if (oneshot)
		val &= ~(PWM_CTRL_T0_AUTO_WIDTH << auto_shift);
	else
		val |= (PWM_CTRL_T0_AUTO_WIDTH << auto_shift);
	val |= (PWM_CTRL_T0_START_WIDTH  << start_shift);
	writel(val, PWM_VA_BASE + PWM_CTRL);
}

static inline void mv_pwm_stop(int id)
{
	unsigned int val;

	val = readl(PWM_VA_BASE + PWM_CTRL);
	if (id == PWM_ID_0)
		val &= ~PWM_CTRL_T0_START_MASK;
	else
		val &= ~(PWM_CTRL_T0_START_WIDTH  << ((id + 1) << 2));
	writel(val, PWM_VA_BASE + PWM_CTRL);
}

u32 mv_pwm_config_usec(int id, unsigned int usec, int duty_cycle);
u32 mv_pwm_config_hz(int id, unsigned int hz, int duty_cycle);
u32 mv_pwm_init_usec(int id, unsigned int div, unsigned int prescaler, unsigned int cntb, int duty_cycle);

#endif /* __ASM_ARCH_PWM_H */

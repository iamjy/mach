/*
 *  arch/arm/mach-mvformula/sys.c
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
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>

#include <mach/map.h>
#include <mach/regs-sys.h>
#include <mach/sys.h>
#include <mach/gpio.h>

#define DRV_NAME	"mvformula-sys"

static  SYSTEM_REGISTERS  * systemRegisters = (SYSTEM_REGISTERS *)SYS_VA_BASE;


/*
 * Debug Level
 *	2 : Print all debug messages
 *	1 : Print only dbg() messages
 *	0 : No debug messages
 */
#define MVFORMULA_SYS_DEBUG_LEVEL	0

#if (MVFORMULA_SYS_DEBUG_LEVEL == 2)
#define dbg(format, arg...)	\
	printk(KERN_ALERT DRV_NAME": Debug: %s(): " format, __func__, ## arg)
#define enter()			\
	printk(KERN_ALERT DRV_NAME": Enter: %s()\n", __func__)
#define leave()			\
	printk(KERN_ALERT DRV_NAME": Leave: %s()\n", __func__)
#elif (MVFORMULA_SYS_DEBUG_LEVEL == 1)
#define dbg(format, arg...)	\
	printk(KERN_ALERT DRV_NAME": Debug: %s(): " format, __func__, ## arg)
#define enter()
#define leave()
#else
#define dbg(format, arg...)
#define enter()
#define leave()
#endif

static DEFINE_SPINLOCK(sys_spinlock);

#if defined(CONFIG_TYCHE2_BOARD)
#define OSC_CLK_MHZ	24
#elif defined(CONFIG_FORMULA_EVB_BOARD)
#define OSC_CLK_MHZ	24
#elif defined(CONFIG_FORMULA_POP_BOARD)
#define OSC_CLK_MHZ	24
#elif defined(CONFIG_FIRENZE2_BOARD)
#define OSC_CLK_MHZ	24
#elif defined(CONFIG_MFP_7V2000T_Q_FPGA_BOARD)
#define OSC_CLK_MHZ	30
#else
#error "Bad CONFIG_XXX_BOARD"
#endif

u32 mv_sys_get_osc_clk_mhz(void)
{
	u32 osc_clk_mhz;

	osc_clk_mhz = OSC_CLK_MHZ;

	return osc_clk_mhz;
}
EXPORT_SYMBOL(mv_sys_get_osc_clk_mhz);

int mv_sys_get_pll_param(int pll_id, u32 pll_out_hz,
		struct mv_pll_param *pll_param)
{
	int nr, nr_max;
	int no, no_max;
	int nf, nf_max, nf_const;
	int range;
	u64 fref_khz = 0, fref_khz_min, fref_khz_max;
	u64 fvco_khz, fvco_khz_min, fvco_khz_max;
	u64 fout_khz, fout_khz_min, fout_khz_max;
	u64 fref_khz_prev = 0, fvco_khz_prev = 0, fout_khz_prev = 0;
	u32 fin_khz = mv_sys_get_osc_clk_mhz() * 1000;
	struct mv_pll_range ranges_khz[] = {
		{ 0, 10000 },
		{ 10000, 16000 },
		{ 16000, 25000 },
		{ 25000, 40000 },
		{ 40000, 65000 },
		{ 65000, 100000 },
		{ 100000, 160000 },
		{ 160000, 200000 }
	};
	int find = 0;
	int ret = 0;

	if (pll_id == SYS_PLL_ID_0) {
		nr_max = SYS_PLL_PARAM0_NR_WIDTH;
		no_max = SYS_PLL_PARAM0_NO_WIDTH;
		nf_max = SYS_PLL_PARAM0_NF_WIDTH;
		nf_const = 2;
		/* 10MHz <= Fref <= 50MHz */
		fref_khz_min = 10000;
		fref_khz_max = 50000;
		/* 800MHz <= Fvco <= 1600MHz */
		fvco_khz_min = 800000;
		fvco_khz_max = 1600000;
		/* 100MHz <= Fout <= 1600MHz */
		fout_khz_min = 100000;
		fout_khz_max = 1600000;
	} else if ((pll_id >= SYS_PLL_ID_1) && (pll_id <= SYS_PLL_ID_4)) {
		nr_max = SYS_PLL_PARAM_NR_WIDTH;
		no_max = SYS_PLL_PARAM_NO_WIDTH;
		nf_max = SYS_PLL_PARAM_NF_WIDTH;
		nf_const = 1;
		/* 10MHz <= Fref <= 200MHz */
		fref_khz_min = 10000;
		fref_khz_max = 200000;
		/* 600MHz <= Fvco <= 1200MHz */
		fvco_khz_min = 600000;
		fvco_khz_max = 1200000;
		/* 20MHz <= Fout <= 1200MHz */
		fout_khz_min = 20000;
		fout_khz_max = 1200000;
	} else {
		pr_err(DRV_NAME ": %s(): Invalid PLL ID=%d(0<=pll_id<=4)\n",
			__func__, pll_out_hz);
		ret = -EINVAL;
		goto out;
	}

	if ((pll_out_hz < ((u32)fout_khz_min * 1000))
			|| (pll_out_hz > ((u32)fout_khz_max * 1000))) {
		pr_err(DRV_NAME ": %s(): Invalid request PLL%d out clock=%uHz"
			"(%uHz < Fout < %uHz)\n", __func__, pll_id, pll_out_hz,
			(u32)fout_khz_min * 1000, (u32)fout_khz_max * 1000);
		ret = -ERANGE;
		goto out;
	}

	for (nr = 0; nr <= nr_max; nr++) {
		fref_khz = fin_khz / (nr + 1);
		if ((fref_khz < fref_khz_min) || (fref_khz > fref_khz_max))
			continue;

		for (nf = 0; nf <= nf_max; nf++) {
			for (no = 0; no <= no_max; no++) {
				fvco_khz = fref_khz * nf_const * (nf + 1);
				if ((fvco_khz < fvco_khz_min)
						|| (fvco_khz > fvco_khz_max))
					continue;

				fout_khz = fvco_khz >> no;
				if ((fout_khz < fout_khz_min)
						|| (fout_khz > fout_khz_max))
					continue;

				/* reject if above target frequency */
				if (fout_khz > (pll_out_hz / 1000))
					continue;

				if ((fout_khz >= fout_khz_prev)
						&& (fvco_khz > fvco_khz_prev)) {
					fref_khz_prev = fref_khz;
					fout_khz_prev = fout_khz;
					fvco_khz_prev = fvco_khz;
					pll_param->fout = (u32)fout_khz * 1000;
					pll_param->nr = nr;
					pll_param->no = no;
					pll_param->nf = nf;
				}

				if (fout_khz == (pll_out_hz / 1000)) {
					find = 1;
					goto out;
				}
			}
		}
	}

out:
	if (pll_id != SYS_PLL_ID_0) {
		for (range = SYS_PLL_PARAM_RANGE_WIDTH; range >= 0; range--) {
			if ((fref_khz_prev >= ranges_khz[range].min)
				&& (fref_khz_prev < ranges_khz[range].max)) {
				pll_param->range = range;
				break;
			}
		}
	} else
		pll_param->range = 0;

	if (find == 0)
		pr_warning(DRV_NAME ": Not matched PLL%d parameter, "
			"request clock=%dHz - real clock=%dHz\n",
			pll_id, pll_out_hz, pll_param->fout);

	dbg("pll_id=%d, pll_out_hz=%uHz, fout=%uHz, nr=%d, no=%d, nf=%d, "
		"range=%d\n", pll_id, pll_out_hz, pll_param->fout,
		pll_param->nr, pll_param->no, pll_param->nf, pll_param->range);

	return ret;
}

u32 mv_sys_get_pll_clk_hz(int pll_id)
{
	int offset = pll_id << 2;
	u32 nr, no, nf;
	u32 osc_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
	u32 pll_clk_hz;
	u32 val;

	val = readl(SYS_VA_BASE + SYS_PLL_PARAM0 + offset);
	if (pll_id == SYS_PLL_ID_0) {
		nr = (val & SYS_PLL_PARAM0_NR_MASK) >> SYS_PLL_PARAM0_NR_SHIFT;
		no = (val & SYS_PLL_PARAM0_NO_MASK) >> SYS_PLL_PARAM0_NO_SHIFT;
		nf = (val & SYS_PLL_PARAM0_NF_MASK) >> SYS_PLL_PARAM0_NF_SHIFT;
	} else {
		nr = (val & SYS_PLL_PARAM_NR_MASK) >> SYS_PLL_PARAM_NR_SHIFT;
		no = (val & SYS_PLL_PARAM_NO_MASK) >> SYS_PLL_PARAM_NO_SHIFT;
		nf = (val & SYS_PLL_PARAM_NF_MASK) >> SYS_PLL_PARAM_NF_SHIFT;
	}

	pll_clk_hz = osc_clk_hz;
	if (pll_id == SYS_PLL_ID_0)
		pll_clk_hz *= 2;
	pll_clk_hz *= (nf + 1);
	pll_clk_hz /= (nr + 1);
	pll_clk_hz /= (1 << no);

	dbg("pll_id=%d, pll_clk_hz=%u, nr=%d, no=%d, nf=%d\n",
		pll_id, pll_clk_hz, nr, no, nf);

	return pll_clk_hz;
}
EXPORT_SYMBOL(mv_sys_get_pll_clk_hz);

int mv_sys_set_pll_clk_hz(int pll_id, u32 pll_out_hz)
{
	int offset = pll_id << 2;
	struct mv_pll_param pll_param;
	u32 val;
	unsigned long flags;
	int ret = 0;

	ret = mv_sys_get_pll_param(pll_id, pll_out_hz, &pll_param);
	if (ret < 0) {
		pr_err(DRV_NAME ": %s(): Failed to mv_sys_get_pll_param\n",
			__func__);
		goto out;
	}

	dbg("PLL%d, requested clock=%dHz, real clock=%dHz\n",
		pll_id, pll_out_hz, pll_param.fout);

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_PLL_PARAM0 + offset);
	val &= ~SYS_PLL_PARAM0_RST_MASK;
	writel(val, SYS_VA_BASE + SYS_PLL_PARAM0 + offset);

	if (pll_id == SYS_PLL_ID_0) {
		val &= ~SYS_PLL_PARAM0_NR_MASK;
		val |= (pll_param.nr & SYS_PLL_PARAM0_NR_WIDTH)
			<< SYS_PLL_PARAM0_NR_SHIFT;
		val &= ~SYS_PLL_PARAM0_NO_MASK;
		val |= (pll_param.no & SYS_PLL_PARAM0_NO_WIDTH)
			<< SYS_PLL_PARAM0_NO_SHIFT;
		val &= ~SYS_PLL_PARAM0_NF_MASK;
		val |= (pll_param.nf & SYS_PLL_PARAM0_NF_WIDTH)
			<< SYS_PLL_PARAM0_NF_SHIFT;
	} else {
		val &= ~SYS_PLL_PARAM_NR_MASK;
		val |= (pll_param.nr & SYS_PLL_PARAM_NR_WIDTH)
			<< SYS_PLL_PARAM_NR_SHIFT;
		val &= ~SYS_PLL_PARAM_NO_MASK;
		val |= (pll_param.no & SYS_PLL_PARAM_NO_WIDTH)
			<< SYS_PLL_PARAM_NO_SHIFT;
		val &= ~SYS_PLL_PARAM_NF_MASK;
		val |= (pll_param.nf & SYS_PLL_PARAM_NF_WIDTH)
			<< SYS_PLL_PARAM_NF_SHIFT;
		val &= ~SYS_PLL_PARAM_RANGE_MASK;
		val |= (pll_param.range & SYS_PLL_PARAM_RANGE_WIDTH)
			<< SYS_PLL_PARAM_RANGE_SHIFT;
	}

	writel(val, SYS_VA_BASE + SYS_PLL_PARAM0 + offset);

	val |= SYS_PLL_PARAM0_RST_MASK;
	writel(val, SYS_VA_BASE + SYS_PLL_PARAM0 + offset);

	val &= ~SYS_PLL_PARAM0_RST_MASK;
	writel(val, SYS_VA_BASE + SYS_PLL_PARAM0 + offset);

#if 0 /* by kodh */
	do {
		val = readl(SYS_VA_BASE + SYS_PLL_PARAM0 + offset);
	} while ((val & SYS_PLL_PARAM0_LOCK_MASK) == 0);
#endif // FPGA Board PLL?¤ì •?€ ë¬´ì‹œ
	spin_unlock_irqrestore(&sys_spinlock, flags);

out:
	return ret;
}

static int mv_sys_get_sys_pll_id(void)
{
	u32 val;
	int ret = SYS_PLL_ID_OSC;

	val = readl(SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	if ((val & SYS_MAIN_CLK_SEL_PLL0_MASK) == 0)
		goto out;

	val = readl(SYS_VA_BASE + SYS_PLL_SRC_SEL);
	val &= SYS_PLL_SRC_SEL_PLL0_MASK;
	val >>= SYS_PLL_SRC_SEL_PLL0_SHIFT;
	if (val == 0)
		ret = SYS_PLL_ID_0;
	else
		ret = ffs((int)val) - 1;

out:
	return ret;
}

u32 mv_sys_get_sys_clk_hz(void)
{
	u32 sys_clk_hz;
#if defined(CONFIG_TYCHE2_BOARD) || defined(CONFIG_MFP_7V2000T_Q_FPGA_BOARD)
	sys_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
#else
	u32 src_clk_hz;
	int pll_id = mv_sys_get_sys_pll_id();
	int div_val = 1;
	u32 val;

	if (pll_id == SYS_PLL_ID_OSC)
		src_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
	else
		src_clk_hz = mv_sys_get_pll_clk_hz(pll_id);

	val = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	if (val & SYS_CLK_BYPASS_PLL0_MASK) {
		sys_clk_hz = src_clk_hz;
		goto out;
	}

	val = readl(SYS_VA_BASE + SYS_CLK_DIV0);
	div_val = ffs((int)val) + 1;

	sys_clk_hz = src_clk_hz / div_val;

out:
#endif /* defined(CONFIG_TYCHE2_BOARD) */
	return sys_clk_hz;
}

int mv_sys_set_sys_clk_hz(int pll_id, u32 pll_out_hz, int sys_div, int arm_div,
		int bus_div, int dmc1_div, int dmc2_div, int apb_div)
{
	u32 pll_src_sel, main_clk_sel, clk_bypass, sys_clk_div, clk_div0;
	int ret = 0;
	unsigned long flags;

	if ((pll_id < SYS_PLL_ID_0) || (pll_id > SYS_PLL_ID_OSC)) {
		pr_err(DRV_NAME ": %s(): Invalid pll_id, %d\n",
			__func__, pll_id);
		ret = -EINVAL;
		goto out;
	}

	if ((sys_div < 1) || (sys_div > 8)) {
		pr_err(DRV_NAME ": %s(): Invalid sys_div, %d\n",
			__func__, sys_div);
		ret = -EINVAL;
		goto out;
	}

	if ((arm_div < 1) || (arm_div > 8)) {
		pr_err(DRV_NAME ": %s(): Invalid arm_div, %d\n",
			__func__, arm_div);
		ret = -EINVAL;
		goto out;
	}

	if ((bus_div < 1) || (bus_div > 8)) {
		pr_err(DRV_NAME ": %s(): Invalid bus_div, %d\n",
			__func__, bus_div);
		ret = -EINVAL;
		goto out;
	}

	if ((dmc1_div < 2) || (dmc1_div > 8)) {
		pr_err(DRV_NAME ": %s(): Invalid dmc1_div, %d\n",
			__func__, dmc1_div);
		ret = -EINVAL;
		goto out;
	}

	if ((dmc2_div < 1) || (dmc2_div > 8)) {
		pr_err(DRV_NAME ": %s(): Invalid dmc2_div, %d\n",
			__func__, dmc2_div);
		ret = -EINVAL;
		goto out;
	}

	if ((apb_div < 1) || (apb_div > 8)) {
		pr_err(DRV_NAME ": %s(): Invalid apb_div, %d\n",
			__func__, apb_div);
		ret = -EINVAL;
		goto out;
	}

	dbg("pll_id=%d, pll_out_hz=%u, sys_div=%d, arm_div=%d, bus_div=%d, "
		"dmc1_div=%d, dmc2_div=%d, apb_div=%d\n",
		pll_id, pll_out_hz, sys_div, arm_div, bus_div, dmc1_div,
		dmc2_div, apb_div);

	pll_src_sel = readl(SYS_VA_BASE + SYS_PLL_SRC_SEL);
	main_clk_sel = readl(SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	clk_bypass = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	sys_clk_div = readl(SYS_VA_BASE + SYS_SYS_CLK_DIV);
	clk_div0 = readl(SYS_VA_BASE + SYS_CLK_DIV0);

	spin_lock_irqsave(&sys_spinlock, flags);

	/* Change system clock source from PLL to OSC */
	main_clk_sel &= ~SYS_MAIN_CLK_SEL_PLL0_MASK;
	writel(main_clk_sel, SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

	/* Set system clock to bypass */
#if 0
	clk_bypass |= SYS_CLK_BYPASS_SYS_ARM_MASK;
	clk_bypass |= SYS_CLK_BYPASS_SYS_BUS_MASK;
	clk_bypass |= SYS_CLK_BYPASS_SYS_APB_MASK;
#endif
	clk_bypass |= SYS_CLK_BYPASS_PLL0_MASK;
	writel(clk_bypass, SYS_VA_BASE + SYS_CLK_BYPASS);

	/* PLL setting */
	if (pll_id == SYS_PLL_ID_OSC)
		goto skip_set_pll;

	spin_unlock_irqrestore(&sys_spinlock, flags);
	mv_sys_set_pll_clk_hz(pll_id, pll_out_hz);
	if (ret < 0) {
		pr_err(DRV_NAME ": %s(): Failed to mv_sys_set_pll_param\n",
			__func__);
		goto out;
	}
	spin_lock_irqsave(&sys_spinlock, flags);

skip_set_pll:
	/* Set sys_div */
	pll_src_sel &= ~SYS_PLL_SRC_SEL_PLL0_MASK;
	clk_div0 &= ~SYS_CLK_DIV_VAL_MASK;
	clk_bypass |= SYS_CLK_BYPASS_PLL0_MASK;
	if (pll_id != SYS_PLL_ID_OSC) {
		pll_src_sel |= ((1 << pll_id) << SYS_PLL_SRC_SEL_PLL0_SHIFT);
		if (sys_div > 1) {
			clk_bypass &= ~SYS_CLK_BYPASS_PLL0_MASK;
			clk_div0 |= ((1 << (sys_div - 2))
					<< SYS_CLK_DIV_VAL_SHIFT);
		}
	}

	/* Set arm_div */
	sys_clk_div &= ~SYS_SYS_CLK_DIV_ARM_MASK;
	clk_bypass |= SYS_CLK_BYPASS_SYS_ARM_MASK;
	if (arm_div > 1) {
		sys_clk_div |= ((1 << (arm_div - 2))
				<< SYS_SYS_CLK_DIV_ARM_SHIFT);
		clk_bypass &= ~SYS_CLK_BYPASS_SYS_ARM_MASK;
	}

	/* Set bus_div */
	sys_clk_div &= ~SYS_SYS_CLK_DIV_BUS_MASK;
	clk_bypass |= SYS_CLK_BYPASS_SYS_BUS_MASK;
	if (bus_div > 1) {
		sys_clk_div |= ((1 << (bus_div - 2))
				<< SYS_SYS_CLK_DIV_BUS_SHIFT);
		clk_bypass &= ~SYS_CLK_BYPASS_SYS_BUS_MASK;
	}

	/* Set dmc1_div */
	sys_clk_div &= ~SYS_SYS_CLK_DIV_DMC1_MASK;
	sys_clk_div |= ((dmc1_div - 2) << SYS_SYS_CLK_DIV_DMC1_SHIFT);

	/* Set dmc2_div */
	sys_clk_div &= ~SYS_SYS_CLK_DIV_DMC2_MASK;
	sys_clk_div |= ((dmc2_div - 1) << SYS_SYS_CLK_DIV_DMC2_SHIFT);

	/* Set apb_div */
	sys_clk_div &= ~SYS_SYS_CLK_DIV_APB_MASK;
	clk_bypass |= SYS_CLK_BYPASS_SYS_APB_MASK;
	if (apb_div > 1) {
		sys_clk_div |= ((1 << (apb_div - 2))
				<< SYS_SYS_CLK_DIV_APB_SHIFT);
		clk_bypass &= ~SYS_CLK_BYPASS_SYS_APB_MASK;
	}

	/* Update values */
	writel(pll_src_sel, SYS_VA_BASE + SYS_PLL_SRC_SEL);
	writel(clk_div0, SYS_VA_BASE + SYS_CLK_DIV0);
	writel(sys_clk_div, SYS_VA_BASE + SYS_SYS_CLK_DIV);
	writel(clk_bypass, SYS_VA_BASE + SYS_CLK_BYPASS);

	/* Change system clock source from OSC to PLL */
	main_clk_sel |= SYS_MAIN_CLK_SEL_PLL0_MASK;
	writel(main_clk_sel, SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

	spin_unlock_irqrestore(&sys_spinlock, flags);

out:
	return ret;
}

u32 mv_sys_get_arm_clk_hz(void)
{
	u32 arm_clk_hz;
#if defined(CONFIG_TYCHE2_BOARD) || defined(CONFIG_MFP_7V2000T_Q_FPGA_BOARD)
	arm_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
#else
	u32 sys_clk_hz;
	int div_val = 1;
	u32 val;

	sys_clk_hz = mv_sys_get_sys_clk_hz();

	val = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	if (val & SYS_CLK_BYPASS_SYS_ARM_MASK) {
		arm_clk_hz = sys_clk_hz;
		goto out;
	}

	val = readl(SYS_VA_BASE + SYS_SYS_CLK_DIV);
	val &= SYS_SYS_CLK_DIV_ARM_MASK;
	val >>= SYS_SYS_CLK_DIV_ARM_SHIFT;
	if (val != 0)
		div_val = ffs((int)val) + 1;
	else
		div_val = 2;

	arm_clk_hz = sys_clk_hz / div_val;
out:
#endif /* defined(CONFIG_TYCHE2_BOARD) */
	return arm_clk_hz;
}

u32 mv_sys_get_bus_clk_hz(void)
{
	u32 bus_clk_hz;
#if defined(CONFIG_TYCHE2_BOARD) || defined(CONFIG_MFP_7V2000T_Q_FPGA_BOARD)
	bus_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
#else
	u32 sys_clk_hz;
	int div_val = 1;
	u32 val;

	sys_clk_hz = mv_sys_get_sys_clk_hz();

	val = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	if (val & SYS_CLK_BYPASS_SYS_BUS_MASK) {
		bus_clk_hz = sys_clk_hz;
		goto out;
	}

	val = readl(SYS_VA_BASE + SYS_SYS_CLK_DIV);
	val &= SYS_SYS_CLK_DIV_BUS_MASK;
	val >>= SYS_SYS_CLK_DIV_BUS_SHIFT;
	if (val != 0)
		div_val = ffs((int)val) + 1;
	else
		div_val = 2;

	bus_clk_hz = sys_clk_hz / div_val;
out:
#endif /* defined(CONFIG_TYCHE2_BOARD) */
	return bus_clk_hz;
}
EXPORT_SYMBOL(mv_sys_get_bus_clk_hz);

u32 mv_sys_get_apb_clk_hz(void)
{
	u32 apb_clk_hz;
#if defined(CONFIG_TYCHE2_BOARD) || defined(CONFIG_MFP_7V2000T_Q_FPGA_BOARD)
	apb_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
#else
	u32 sys_clk_hz;
	int div_val = 1;
	u32 val;

	sys_clk_hz = mv_sys_get_sys_clk_hz();

	val = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	if (val & SYS_CLK_BYPASS_SYS_APB_MASK) {
		apb_clk_hz = sys_clk_hz;
		goto out;
	}

	val = readl(SYS_VA_BASE + SYS_SYS_CLK_DIV);
	val &= SYS_SYS_CLK_DIV_APB_MASK;
	val >>= SYS_SYS_CLK_DIV_APB_SHIFT;
	if (val != 0)
		div_val = ffs((int)val) + 1;
	else
		div_val = 2;

	apb_clk_hz = sys_clk_hz / div_val;
out:
#endif /* defined(CONFIG_TYCHE2_BOARD) */
	return apb_clk_hz;
}
EXPORT_SYMBOL(mv_sys_get_apb_clk_hz);

u32 mv_sys_get_dmc1_clk_hz(void)
{
	u32 dmc1_clk_hz;
#if defined(CONFIG_TYCHE2_BOARD)
	dmc1_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
#else
	u32 sys_clk_hz;
	int div_val = 1;
	u32 val;

	sys_clk_hz = mv_sys_get_sys_clk_hz();

	val = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	if (val & SYS_CLK_BYPASS_SYS_BUS_MASK) {
		dmc1_clk_hz = sys_clk_hz;
		goto out;
	}

	val = readl(SYS_VA_BASE + SYS_SYS_CLK_DIV);
	val &= SYS_SYS_CLK_DIV_DMC1_MASK;
	val >>= SYS_SYS_CLK_DIV_DMC1_SHIFT;
	div_val = val + 2;

	dmc1_clk_hz = sys_clk_hz / div_val;
out:
#endif /* defined(CONFIG_TYCHE2_BOARD) */
	return dmc1_clk_hz;
}

u32 mv_sys_get_dmc2_clk_hz(void)
{
	u32 dmc2_clk_hz;
#if defined(CONFIG_TYCHE2_BOARD)
	dmc2_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
#else
	u32 sys_clk_hz;
	int div_val = 1;
	u32 val;

	sys_clk_hz = mv_sys_get_sys_clk_hz();

	val = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	if (val & SYS_CLK_BYPASS_SYS_BUS_MASK) {
		dmc2_clk_hz = sys_clk_hz;
		goto out;
	}

	val = readl(SYS_VA_BASE + SYS_SYS_CLK_DIV);
	val &= SYS_SYS_CLK_DIV_DMC2_MASK;
	val >>= SYS_SYS_CLK_DIV_DMC2_SHIFT;
	div_val = val + 1;

	dmc2_clk_hz = sys_clk_hz / div_val;
out:
#endif /* defined(CONFIG_TYCHE2_BOARD) */
	return dmc2_clk_hz;
}

static u32 mv_sys_get_dmc_alt_clk_hz(void)
{
	u32 dmc_alt_clk_hz;
	u32 dmc_src_clk_hz;
	int dmc_alt_src;
	int div_val;
	u32 val;

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL2);
	dmc_alt_src = (val & SYS_PERI_CLK_DMC_ALT_SRC_SEL_MASK)
			>> SYS_PERI_CLK_DMC_ALT_SRC_SEL_SHIFT;
	if (dmc_alt_src == SYS_DMA_ALT_SRC_DAC)
		dmc_src_clk_hz = mv_sys_get_dac_clk_hz();
	else if (dmc_alt_src == SYS_DMA_ALT_SRC_USB)
		dmc_src_clk_hz = mv_sys_get_usb_clk_hz();
	else if (dmc_alt_src == SYS_DMA_ALT_SRC_SENSOR)
		dmc_src_clk_hz = mv_sys_get_sensor_clk_hz();
	else
		dmc_src_clk_hz = mv_sys_get_audio_clk_hz();

	if (val & SYS_PERI_CLK_DMC_ALT_BYPASS_MASK)
		div_val = 1;
	else
		div_val = ((val & SYS_PERI_CLK_DMC_ALT_DIV_MASK)
			>> SYS_PERI_CLK_DMC_ALT_DIV_SHIFT) + 1;

	dmc_alt_clk_hz = dmc_src_clk_hz / div_val;

	return dmc_alt_clk_hz;
}

u32 mv_sys_get_mem_clk_hz(void)
{
	u32 mem_clk_hz;
#if defined(CONFIG_TYCHE2_BOARD) || defined(CONFIG_MFP_7V2000T_Q_FPGA_BOARD)
	mem_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
#else
	u32 val;

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL2);
	if (val & SYS_PERI_CLK_DMC_ALT_SEL_MASK)
		mem_clk_hz = mv_sys_get_dmc_alt_clk_hz();
	else
		mem_clk_hz = mv_sys_get_dmc1_clk_hz();
#endif
	return mem_clk_hz;
}

static int mv_sys_get_usb_pll_id(void)
{
	u32 val;
	int ret = SYS_PLL_ID_OSC;

	val = readl(SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	if ((val & SYS_MAIN_CLK_SEL_PLL2_MASK) == 0)
		goto out;

	val = readl(SYS_VA_BASE + SYS_PLL_SRC_SEL);
	val &= SYS_PLL_SRC_SEL_PLL2_MASK;
	val >>= SYS_PLL_SRC_SEL_PLL2_SHIFT;
	if (val == 0)
		ret = SYS_PLL_ID_2;
	else
		ret = ffs((int)val) - 1;

out:
	return ret;
}

u32 mv_sys_get_usb_clk_hz(void)
{
	u32 usb_clk_hz;
	u32 src_clk_hz;
	int pll_id = mv_sys_get_usb_pll_id();
	int div_val = 1;
	u32 val;

	if (pll_id == SYS_PLL_ID_OSC)
		src_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
	else
		src_clk_hz = mv_sys_get_pll_clk_hz(pll_id);

	val = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	if (val & SYS_MAIN_CLK_SEL_PLL2_MASK) {
		usb_clk_hz = src_clk_hz;
		goto out;
	}

	val = readl(SYS_VA_BASE + SYS_CLK_DIV2);
	div_val = ffs((int)val) + 1;
	usb_clk_hz = src_clk_hz / div_val;

out:
	return usb_clk_hz;
}
EXPORT_SYMBOL(mv_sys_get_usb_clk_hz);

int mv_sys_set_usb_clk_hz(int pll_id, u32 pll_out_hz, int usb_div)
{
	u32 pll_src_sel, main_clk_sel, clk_bypass, clk_div2;
	int ret = 0;
	unsigned long flags;

	dbg("pll_out_hz=%u\n", pll_out_hz);

	if ((pll_id < SYS_PLL_ID_0) || (pll_id > SYS_PLL_ID_OSC)) {
		pr_err(DRV_NAME ": %s(): Invalid pll_id, %d\n",
			__func__, pll_id);
		ret = -EINVAL;
		goto out;
	}

	if ((usb_div < 1) || (usb_div > 8)) {
		pr_err(DRV_NAME ": %s(): Invalid usb_div, %d\n",
			__func__, usb_div);
		ret = -EINVAL;
		goto out;
	}

	dbg("pll_id=%d, pll_out_hz=%u, usb_div=%d\n",
		pll_id, pll_out_hz, usb_div);

	pll_src_sel = readl(SYS_VA_BASE + SYS_PLL_SRC_SEL);
	main_clk_sel = readl(SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	clk_bypass = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	clk_div2 = readl(SYS_VA_BASE + SYS_CLK_DIV2);

	spin_lock_irqsave(&sys_spinlock, flags);

	/* Change usb clock source from PLL to OSC */
	main_clk_sel &= ~SYS_MAIN_CLK_SEL_PLL2_MASK;
	writel(main_clk_sel, SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

	/* Set usb clock to bypass */
	clk_bypass |= SYS_CLK_BYPASS_PLL2_MASK;
	writel(clk_bypass, SYS_VA_BASE + SYS_CLK_BYPASS);

	/* PLL setting */
	if (pll_id == SYS_PLL_ID_OSC)
		goto skip_set_pll;

	spin_unlock_irqrestore(&sys_spinlock, flags);
	mv_sys_set_pll_clk_hz(pll_id, pll_out_hz);
	if (ret < 0) {
		pr_err(DRV_NAME ": %s(): Failed to mv_sys_set_pll_param\n",
			__func__);
		goto out;
	}
	spin_lock_irqsave(&sys_spinlock, flags);

skip_set_pll:
	/* Set usb_div */
	pll_src_sel &= ~SYS_PLL_SRC_SEL_PLL2_MASK;
	clk_div2 &= ~SYS_CLK_DIV_VAL_MASK;
	if (pll_id != SYS_PLL_ID_OSC) {
		pll_src_sel |= ((1 << pll_id) << SYS_PLL_SRC_SEL_PLL2_SHIFT);
		if (usb_div > 1) {
			clk_bypass &= ~SYS_CLK_BYPASS_PLL2_MASK;
			clk_div2 |= ((1 << (usb_div - 2))
					<< SYS_CLK_DIV_VAL_SHIFT);
		}
	}

	/* Update values */
	writel(pll_src_sel, SYS_VA_BASE + SYS_PLL_SRC_SEL);
	writel(clk_div2, SYS_VA_BASE + SYS_CLK_DIV2);
	writel(clk_bypass, SYS_VA_BASE + SYS_CLK_BYPASS);

	/* Change usb clock source from OSC to PLL */
	main_clk_sel |= SYS_MAIN_CLK_SEL_PLL2_MASK;
	writel(main_clk_sel, SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

	spin_unlock_irqrestore(&sys_spinlock, flags);

out:
	return ret;
}

static int mv_sys_get_audio_pll_id(void)
{
	u32 val;
	int ret = SYS_PLL_ID_OSC;

	val = readl(SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	if ((val & SYS_MAIN_CLK_SEL_PLL4_MASK) == 0)
		goto out;

	val = readl(SYS_VA_BASE + SYS_PLL_SRC_SEL);
	val &= SYS_PLL_SRC_SEL_PLL4_MASK;
	val >>= SYS_PLL_SRC_SEL_PLL4_SHIFT;
	if (val == 0)
		ret = SYS_PLL_ID_4;
	else
		ret = ffs((int)val) - 1;

out:
	return ret;
}

u32 mv_sys_get_audio_clk_hz(void)
{
	u32 audio_clk_hz;
#if defined(CONFIG_TYCHE2_BOARD)
	audio_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
#else
	u32 src_clk_hz;
	int pll_id = mv_sys_get_audio_pll_id();
	int div_val = 1;
	u32 val;

	if (pll_id == SYS_PLL_ID_OSC)
		src_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
	else
		src_clk_hz = mv_sys_get_pll_clk_hz(pll_id);

	val = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	if (val & SYS_MAIN_CLK_SEL_PLL4_MASK) {
		audio_clk_hz = src_clk_hz;
		goto out;
	}

	val = readl(SYS_VA_BASE + SYS_CLK_DIV4);
	div_val = ffs((int)val) + 1;

	audio_clk_hz = src_clk_hz / div_val;
out:
#endif /* defined(CONFIG_TYCHE2_BOARD) */
	return audio_clk_hz;
}

int mv_sys_set_audio_clk_hz(int pll_id, u32 pll_out_hz, int audio_div)
{
	u32 pll_src_sel, main_clk_sel, clk_bypass, clk_div4;
	int ret = 0;
	unsigned long flags;

	dbg("pll_out_hz=%u\n", pll_out_hz);

	if ((pll_id < SYS_PLL_ID_0) || (pll_id > SYS_PLL_ID_OSC)) {
		pr_err(DRV_NAME ": %s(): Invalid pll_id, %d\n",
			__func__, pll_id);
		ret = -EINVAL;
		goto out;
	}

	if ((audio_div < 1) || (audio_div > 8)) {
		pr_err(DRV_NAME ": %s(): Invalid audio_div, %d\n",
			__func__, audio_div);
		ret = -EINVAL;
		goto out;
	}

	dbg("pll_id=%d, pll_out_hz=%u, audio_div=%d\n",
		pll_id, pll_out_hz, audio_div);

	pll_src_sel = readl(SYS_VA_BASE + SYS_PLL_SRC_SEL);
	main_clk_sel = readl(SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	clk_bypass = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	clk_div4 = readl(SYS_VA_BASE + SYS_CLK_DIV4);

	spin_lock_irqsave(&sys_spinlock, flags);

	/* Change audio clock source from PLL to OSC */
	main_clk_sel &= ~SYS_MAIN_CLK_SEL_PLL4_MASK;
	writel(main_clk_sel, SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

	/* Set audio clock to bypass */
	clk_bypass |= SYS_CLK_BYPASS_PLL4_MASK;
	writel(clk_bypass, SYS_VA_BASE + SYS_CLK_BYPASS);

	/* PLL setting */
	if (pll_id == SYS_PLL_ID_OSC)
		goto skip_set_pll;

	spin_unlock_irqrestore(&sys_spinlock, flags);
	mv_sys_set_pll_clk_hz(pll_id, pll_out_hz);
	if (ret < 0) {
		pr_err(DRV_NAME ": %s(): Failed to mv_sys_set_pll_param\n",
			__func__);
		goto out;
	}
	spin_lock_irqsave(&sys_spinlock, flags);

skip_set_pll:
	/* Set audio_div */
	pll_src_sel &= ~SYS_PLL_SRC_SEL_PLL4_MASK;
	clk_div4 &= ~SYS_CLK_DIV_VAL_MASK;
	if (pll_id != SYS_PLL_ID_OSC) {
		pll_src_sel |= ((1 << pll_id) << SYS_PLL_SRC_SEL_PLL4_SHIFT);
		if (audio_div > 1) {
			clk_bypass &= ~SYS_CLK_BYPASS_PLL4_MASK;
			clk_div4 |= ((1 << (audio_div - 2))
					<< SYS_CLK_DIV_VAL_SHIFT);
		}
	}

	/* Update values */
	writel(pll_src_sel, SYS_VA_BASE + SYS_PLL_SRC_SEL);
	writel(clk_div4, SYS_VA_BASE + SYS_CLK_DIV4);
	writel(clk_bypass, SYS_VA_BASE + SYS_CLK_BYPASS);

	/* Change audio clock source from OSC to PLL */
	main_clk_sel |= SYS_MAIN_CLK_SEL_PLL4_MASK;
	writel(main_clk_sel, SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

	spin_unlock_irqrestore(&sys_spinlock, flags);

out:
	return ret;
}

void mv_sys_audio_nco_ctrl(int en)
{
	u32 val;
	unsigned long flags;
	unsigned long readl;

	printk(KERN_ERR"audio nco ctrl %d--------------- \n",en);
	spin_lock_irqsave(&sys_spinlock, flags);
	readl = readl(SYS_VA_BASE + SYS_CLK_CTRL1);
	readl = readl & ~(0x01<<10);
	writel(readl, SYS_VA_BASE + SYS_CLK_CTRL1);
	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL2);
	if (en)
		val |= SYS_PERI_CLK_AUDIO_NCO_EN_MASK;
	else
		val &= ~SYS_PERI_CLK_AUDIO_NCO_EN_MASK;
	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL2);
	spin_unlock_irqrestore(&sys_spinlock, flags);
}

static int mv_sys_get_dac_pll_id(void)
{
	u32 val;
	int ret = SYS_PLL_ID_OSC;

	val = readl(SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	if ((val & SYS_MAIN_CLK_SEL_PLL1_MASK) == 0)
		goto out;

	val = readl(SYS_VA_BASE + SYS_PLL_SRC_SEL);
	val &= SYS_PLL_SRC_SEL_PLL1_MASK;
	val >>= SYS_PLL_SRC_SEL_PLL1_SHIFT;
	if (val == 0)
		ret = SYS_PLL_ID_1;
	else
		ret = ffs((int)val) - 1;

out:
	return ret;
}

u32 mv_sys_get_dac_clk_hz(void)
{
	u32 dac_clk_hz;
	u32 src_clk_hz;
	int pll_id = mv_sys_get_dac_pll_id();
	int div_val = 1;
	u32 val;

	if (pll_id == SYS_PLL_ID_OSC)
		src_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
	else
		src_clk_hz = mv_sys_get_pll_clk_hz(pll_id);

	val = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	if (val & SYS_MAIN_CLK_SEL_PLL1_MASK) {
		dac_clk_hz = src_clk_hz;
		goto out;
	}

	val = readl(SYS_VA_BASE + SYS_CLK_DIV1);
	div_val = ffs((int)val) + 1;
	dac_clk_hz = src_clk_hz / div_val;

out:
	return dac_clk_hz;
}

int mv_sys_set_dac_clk_hz(int pll_id, u32 pll_out_hz, int dac_div)
{
	u32 pll_src_sel, main_clk_sel, clk_bypass, clk_div1;
	int ret = 0;
	unsigned long flags;

	dbg("pll_out_hz=%u\n", pll_out_hz);

	if ((pll_id < SYS_PLL_ID_0) || (pll_id > SYS_PLL_ID_OSC)) {
		pr_err(DRV_NAME ": %s(): Invalid pll_id, %d\n",
			__func__, pll_id);
		ret = -EINVAL;
		goto out;
	}

	if ((dac_div < 1) || (dac_div > 8)) {
		pr_err(DRV_NAME ": %s(): Invalid dac_div, %d\n",
			__func__, dac_div);
		ret = -EINVAL;
		goto out;
	}

	dbg("pll_id=%d, pll_out_hz=%u, dac_div=%d\n",
		pll_id, pll_out_hz, dac_div);

	pll_src_sel = readl(SYS_VA_BASE + SYS_PLL_SRC_SEL);
	main_clk_sel = readl(SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	clk_bypass = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	clk_div1 = readl(SYS_VA_BASE + SYS_CLK_DIV1);

	spin_lock_irqsave(&sys_spinlock, flags);

	/* Change dac clock source from PLL to OSC */
	main_clk_sel &= ~SYS_MAIN_CLK_SEL_PLL1_MASK;
	writel(main_clk_sel, SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

	/* Set dac clock to bypass */
	clk_bypass |= SYS_CLK_BYPASS_PLL1_MASK;
	writel(clk_bypass, SYS_VA_BASE + SYS_CLK_BYPASS);

	/* PLL setting */
	if (pll_id == SYS_PLL_ID_OSC)
		goto skip_set_pll;

	spin_unlock_irqrestore(&sys_spinlock, flags);
	mv_sys_set_pll_clk_hz(pll_id, pll_out_hz);
	if (ret < 0) {
		pr_err(DRV_NAME ": %s(): Failed to mv_sys_set_pll_param\n",
			__func__);
		goto out;
	}
	spin_lock_irqsave(&sys_spinlock, flags);

skip_set_pll:
	/* Set dac_div */
	pll_src_sel &= ~SYS_PLL_SRC_SEL_PLL1_MASK;
	clk_div1 &= ~SYS_CLK_DIV_VAL_MASK;
	if (pll_id != SYS_PLL_ID_OSC) {
		pll_src_sel |= ((1 << pll_id) << SYS_PLL_SRC_SEL_PLL1_SHIFT);
		if (dac_div > 1) {
			clk_bypass &= ~SYS_CLK_BYPASS_PLL1_MASK;
			clk_div1 |= ((1 << (dac_div - 2))
					<< SYS_CLK_DIV_VAL_SHIFT);
		}
	}

	/* Update values */
	writel(pll_src_sel, SYS_VA_BASE + SYS_PLL_SRC_SEL);
	writel(clk_div1, SYS_VA_BASE + SYS_CLK_DIV1);
	writel(clk_bypass, SYS_VA_BASE + SYS_CLK_BYPASS);

	/* Change dac clock source from OSC to PLL */
	main_clk_sel |= SYS_MAIN_CLK_SEL_PLL1_MASK;
	writel(main_clk_sel, SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

	spin_unlock_irqrestore(&sys_spinlock, flags);

out:
	return ret;
}

u32 mv_sys_get_nand_clk_hz(void)
{
	u32 src_clk_hz;
	u32 nand_clk_hz;
	int div_val = 0;
	u32 val;

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL0);

	if (val & SYS_PERI_CLK_NAND_SRC_SEL_MASK)
		src_clk_hz = mv_sys_get_apb_clk_hz();
	else
		src_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;

	if (val & SYS_PERI_CLK_NAND_BYPASS_MASK) {
		nand_clk_hz = src_clk_hz;
		goto out;
	}

	val = (val & SYS_PERI_CLK_NAND_DIV_MASK) >> SYS_PERI_CLK_NAND_DIV_SHIFT;
	div_val = ffs((int)val);
	nand_clk_hz = src_clk_hz / (div_val + 1);

out:
	return nand_clk_hz;
}

void mv_sys_set_nand_clk(int clk_src, int div)
{
	u32 div_val = 1;
	u32 val;
	unsigned long flags;

	if (div <= 0)
		div = 1;

	if (div >= 2)
		div_val = 1 << (div - 2);

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL0);
	val &= ~SYS_PERI_CLK_NAND_SRC_SEL_MASK;
	val |= (clk_src << SYS_PERI_CLK_NAND_SRC_SEL_SHIFT);
	val &= ~SYS_PERI_CLK_NAND_DIV_MASK;
	val |= (div_val & SYS_PERI_CLK_NAND_DIV_WIDTH)
		<< SYS_PERI_CLK_NAND_DIV_SHIFT;
	if (div == 1)
		val |= SYS_PERI_CLK_NAND_BYPASS_MASK;
	else
		val &= ~SYS_PERI_CLK_NAND_BYPASS_MASK;
	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL0);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_nand_clk_ctrl(int en)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);
	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL0);
	if (en)
		val |= SYS_PERI_CLK_NAND_ENA_MASK;
	else
		val &= ~SYS_PERI_CLK_NAND_ENA_MASK;
	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL0);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

u32 mv_sys_get_lcd_clk_hz(void)
{
	u32 src_clk_hz;
	u32 lcd_clk_hz;
	int src_sel;
	int div_val = 0;
	u32 val;

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL6);

	src_sel = (val & SYS_PERI_CLK_LCD_SRC_SEL_MASK)
		>> SYS_PERI_CLK_LCD_SRC_SEL_SHIFT;

	if (src_sel == 0)
		src_clk_hz = mv_sys_get_sys_clk_hz();
	else if (src_sel == 1)
		src_clk_hz = mv_sys_get_dac_clk_hz();
	else if (src_sel == 2)
		src_clk_hz = mv_sys_get_sensor_clk_hz();
	else
		src_clk_hz = mv_sys_get_bus_clk_hz();

	div_val = (val & SYS_PERI_CLK_LCD_DIV_MASK)
		>> SYS_PERI_CLK_LCD_DIV_SHIFT;
	lcd_clk_hz = src_clk_hz / (div_val + 1);

	return lcd_clk_hz;
}

void mv_sys_set_lcd_clk(int clk_src, int div)
{
	u32 val;
	unsigned long flags;

	if (div <= 0)
		div = 1;
	div--;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL6);
	val &= ~SYS_PERI_CLK_LCD_SRC_SEL_MASK;
	val |= (clk_src << SYS_PERI_CLK_LCD_SRC_SEL_SHIFT);
	val &= ~SYS_PERI_CLK_LCD_DIV_MASK;
	val |= (div & SYS_PERI_CLK_LCD_DIV_WIDTH)
		<< SYS_PERI_CLK_LCD_DIV_SHIFT;
	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL6);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_lcd_clk_ctrl(int en)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);
	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL6);
	if (en)
		val |= SYS_PERI_CLK_LCD_ENA_MASK;
	else
		val &= ~SYS_PERI_CLK_LCD_ENA_MASK;
	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL6);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

u32 mv_sys_get_sd0_clk_hz(void)
{
	u32 src_clk_hz;
	u32 sd0_clk_hz;
	int src_sel;
	int div_val = 0;
	u32 val;

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL8);

	src_sel = (val & SYS_PERI_CLK_SD0_SRC_SEL_MASK)
		>> SYS_PERI_CLK_SD0_SRC_SEL_SHIFT;

	if (src_sel == SYS_SD_CLK_SRC_SYS)
		src_clk_hz = mv_sys_get_sys_clk_hz();
	else if (src_sel == SYS_SD_CLK_SRC_SENSOR)
		src_clk_hz = mv_sys_get_sensor_clk_hz();
	else if (src_sel == SYS_SD_CLK_SRC_AUDIO)
		src_clk_hz = mv_sys_get_audio_clk_hz();
	else
		src_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;

	if (val & SYS_PERI_CLK_SD0_BYPASS_MASK) {
		sd0_clk_hz = src_clk_hz;
		goto out;
	}

	div_val = (val & SYS_PERI_CLK_SD0_DIV_MASK)
		>> SYS_PERI_CLK_SD0_DIV_SHIFT;
	sd0_clk_hz = src_clk_hz / (div_val + 1);

out:
	return sd0_clk_hz;
}
EXPORT_SYMBOL(mv_sys_get_sd0_clk_hz);

void mv_sys_set_sd0_clk(int clk_src, int div)
{
	u32 val;
	unsigned long flags;

	if (div <= 0)
		div = 1;
	div--;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL8);
	val &= ~SYS_PERI_CLK_SD0_SRC_SEL_MASK;
	val |= (clk_src << SYS_PERI_CLK_SD0_SRC_SEL_SHIFT);
	val &= ~SYS_PERI_CLK_SD0_DIV_MASK;
	val |= (div & SYS_PERI_CLK_SD0_DIV_WIDTH)
		<< SYS_PERI_CLK_SD0_DIV_SHIFT;
	if (div == 0)
		val |= SYS_PERI_CLK_SD0_BYPASS_MASK;
	else
		val &= ~SYS_PERI_CLK_SD0_BYPASS_MASK;
	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL8);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_sd0_clk_ctrl(int en)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);
	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL8);
	if (en)
		val |= SYS_PERI_CLK_SD0_ENA_MASK;
	else
		val &= ~SYS_PERI_CLK_SD0_ENA_MASK;
	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL8);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

u32 mv_sys_get_sd1_clk_hz(void)
{
	u32 src_clk_hz;
	u32 sd1_clk_hz;
	int src_sel;
	int div_val = 0;
	u32 val;

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL8);

	src_sel = (val & SYS_PERI_CLK_SD1_SRC_SEL_MASK)
		>> SYS_PERI_CLK_SD1_SRC_SEL_SHIFT;

	if (src_sel == SYS_SD_CLK_SRC_SYS)
		src_clk_hz = mv_sys_get_sys_clk_hz();
	else if (src_sel == SYS_SD_CLK_SRC_SENSOR)
		src_clk_hz = mv_sys_get_sensor_clk_hz();
	else if (src_sel == SYS_SD_CLK_SRC_AUDIO)
		src_clk_hz = mv_sys_get_audio_clk_hz();
	else
		src_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;

	if (val & SYS_PERI_CLK_SD1_BYPASS_MASK) {
		sd1_clk_hz = src_clk_hz;
		goto out;
	}

	div_val = (val & SYS_PERI_CLK_SD1_DIV_MASK)
		>> SYS_PERI_CLK_SD1_DIV_SHIFT;
	sd1_clk_hz = src_clk_hz / (div_val + 1);

out:
	return sd1_clk_hz;
}
EXPORT_SYMBOL(mv_sys_get_sd1_clk_hz);

void mv_sys_set_sd1_clk(int clk_src, int div)
{
	u32 val;
	unsigned long flags;

	if (div <= 0)
		div = 1;
	div--;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL8);
	val &= ~SYS_PERI_CLK_SD1_SRC_SEL_MASK;
	val |= (clk_src << SYS_PERI_CLK_SD1_SRC_SEL_SHIFT);
	val &= ~SYS_PERI_CLK_SD1_DIV_MASK;
	val |= (div & SYS_PERI_CLK_SD1_DIV_WIDTH)
		<< SYS_PERI_CLK_SD1_DIV_SHIFT;
	if (div == 0)
		val |= SYS_PERI_CLK_SD1_BYPASS_MASK;
	else
		val &= ~SYS_PERI_CLK_SD1_BYPASS_MASK;
	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL8);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_sd1_clk_ctrl(int en)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);
	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL8);
	if (en)
		val |= SYS_PERI_CLK_SD1_ENA_MASK;
	else
		val &= ~SYS_PERI_CLK_SD1_ENA_MASK;
	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL8);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

u32 mv_sys_get_movi_clk_hz(void)
{
	u32 src_clk_hz;
	u32 movi_clk_hz;
	int src_sel;
	int div_val = 0;
	u32 val;

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL8);

	src_sel = (val & SYS_PERI_CLK_MOVI_SRC_SEL_MASK)
		>> SYS_PERI_CLK_MOVI_SRC_SEL_SHIFT;

	if (src_sel == SYS_SD_CLK_SRC_SYS)
		src_clk_hz = mv_sys_get_sys_clk_hz();
	else if (src_sel == SYS_SD_CLK_SRC_SENSOR)
		src_clk_hz = mv_sys_get_sensor_clk_hz();
	else if (src_sel == SYS_SD_CLK_SRC_AUDIO)
		src_clk_hz = mv_sys_get_audio_clk_hz();
	else
		src_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;

	if (val & SYS_PERI_CLK_MOVI_BYPASS_MASK) {
		movi_clk_hz = src_clk_hz;
		goto out;
	}

	div_val = (val & SYS_PERI_CLK_MOVI_DIV_MASK)
		>> SYS_PERI_CLK_MOVI_DIV_SHIFT;
	movi_clk_hz = src_clk_hz / (div_val + 1);

out:
	return movi_clk_hz;
}
EXPORT_SYMBOL(mv_sys_get_movi_clk_hz);

void mv_sys_set_movi_clk(int clk_src, int div)
{
	u32 val;
	unsigned long flags;

	if (div <= 0)
		div = 1;
	div--;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL8);
	val &= ~SYS_PERI_CLK_MOVI_SRC_SEL_MASK;
	val |= (clk_src << SYS_PERI_CLK_MOVI_SRC_SEL_SHIFT);
	val &= ~SYS_PERI_CLK_MOVI_DIV_MASK;
	val |= (div & SYS_PERI_CLK_MOVI_DIV_WIDTH)
		<< SYS_PERI_CLK_MOVI_DIV_SHIFT;
	if (div == 0)
		val |= SYS_PERI_CLK_MOVI_BYPASS_MASK;
	else
		val &= ~SYS_PERI_CLK_MOVI_BYPASS_MASK;
	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL8);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_movi_clk_ctrl(int en)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);
	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL8);
	if (en)
		val |= SYS_PERI_CLK_MOVI_ENA_MASK;
	else
		val &= ~SYS_PERI_CLK_MOVI_ENA_MASK;
	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL8);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_set_gb_clk_power(int no, int ena)
{
	u32 mask = 1 << (no % 32);
	u32 val;
	unsigned long flags;

	if (no >= SYS_GB_CLK_NUMBER) {
		pr_err(DRV_NAME ": %s(): error: unknown device number=%d\n",
			__func__, no);
		BUG();
	}

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_GB_CLK_CTRL);

	if (ena)
		val &= ~mask;
	else
		val |= mask;

	writel(val, SYS_VA_BASE + SYS_GB_CLK_CTRL);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

int mv_sys_get_dev_clk_power(int no)
{
	int offset = (no / 32) << 2;
	u32 mask = 1 << (no % 32);
	u32 val;

	if (no >= SYS_CLK_NUMBER) {
		pr_err(DRV_NAME ": %s(): error: unknown device number=%d\n",
			__func__, no);
		BUG();
	}

	val = readl(SYS_VA_BASE + SYS_CLK_CTRL0 + offset);

	return (val & mask) ? 0 : 1;
}
EXPORT_SYMBOL(mv_sys_get_dev_clk_power);

void mv_sys_set_dev_clk_power(int no, int ena)
{
	int offset = (no / 32) << 2;
	u32 mask = 1 << (no % 32);
	u32 val;
	unsigned long flags;

	if (no >= SYS_CLK_NUMBER) {
		printk(KERN_ERR "%s: %s(): error: unknown device number=%d\n",
			DRV_NAME, __func__, no);
		BUG();
	}

	if (mv_sys_get_dev_clk_power(no) == ena)
		return;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_CLK_CTRL0 + offset);

	if (ena)
		val &= ~mask;
	else
		val |= mask;

	writel(val, SYS_VA_BASE + SYS_CLK_CTRL0 + offset);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}
EXPORT_SYMBOL(mv_sys_set_dev_clk_power);

void mv_sys_gb_reset(int no)
{
	u32 mask = 1 << (no % 32);
	u32 val;
	unsigned long flags;

	if (no >= SYS_GB_RST_NUMBER) {
		printk(KERN_ERR "%s: %s(): error: unknown device number=%d\n",
			DRV_NAME, __func__, no);
		BUG();
	}

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_GB_RST_CTRL);
	val |= mask;
	writel(val, SYS_VA_BASE + SYS_GB_RST_CTRL);

	udelay(10);

	val &= ~mask;
	writel(val, SYS_VA_BASE + SYS_GB_RST_CTRL);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_dev_reset(int no)
{
	int offset = (no / 32) << 2;
	u32 mask = 1 << (no % 32);
	u32 val;
	unsigned long flags;

	if (no >= SYS_RST_NUMBER) {
		printk(KERN_ERR "%s: %s(): error: unknown device number=%d\n",
			DRV_NAME, __func__, no);
		BUG();
	}

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_RST_CTRL0 + offset);
	if (no == SYS_RST_HDMI)
		val |= 0x00000030;
	val |= mask;
	writel(val, SYS_VA_BASE + SYS_RST_CTRL0 + offset);

	udelay(10);

	val &= ~mask;
	writel(val, SYS_VA_BASE + SYS_RST_CTRL0 + offset);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}
EXPORT_SYMBOL(mv_sys_dev_reset);

void mv_sys_set_alt_func(int no, int ena)
{
	u32 mask = 1 << (no % 32);
	u32 val;
	unsigned long flags;

	if (no >= SYS_ALT_FUNC_NUMBER) {
		printk(KERN_ERR "%s: %s(): error: unknown device number=%d\n",
			DRV_NAME, __func__, no);
		BUG();
	}

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_ALT_FUNC_ENA);

	if (ena)
		val |= mask;
	else
		val &= ~mask;

	writel(val, SYS_VA_BASE + SYS_ALT_FUNC_ENA);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}
EXPORT_SYMBOL(mv_sys_set_alt_func);

void mv_sys_pad_ctrl(int no, int val)
{
	int offset = (no / 32) << 2;
	u32 mask = 1 << (no % 32);
	u32 reg;
	unsigned long flags;

	if (no >= GPIO_NUMBER) {
		pr_err(DRV_NAME ": %s(): error: unknown GPIO number=%d\n",
			__func__, no);
		BUG();
	}

	spin_lock_irqsave(&sys_spinlock, flags);
	reg = readl((SYS_VA_BASE + SYS_PE_CTRL) + offset);
	if (val == 1)
		reg |= mask;
	else
		reg &= ~mask;
	writel(reg, (SYS_VA_BASE + SYS_PE_CTRL) + offset);
	spin_unlock_irqrestore(&sys_spinlock, flags);
}
EXPORT_SYMBOL(mv_sys_pad_ctrl);

struct mv_chip_info mv_sys_get_chip_info(void)
{
	struct mv_chip_info chip_info;
	u32 val;

	chip_info.id = readl(SYS_VA_BASE + SYS_PRODUCT_ID);

	val = readl(SYS_VA_BASE + SYS_CTRL0);
	chip_info.mem_type = (val >> SYS_CTRL_MEM_TYPE_SHIFT)
				& SYS_CTRL_MEM_TYPE_WIDTH;
	chip_info.mem_maskbit = (val >> SYS_CTRL_MEM_MASKBIT_SHIFT)
				& SYS_CTRL_MEM_MASKBIT_WIDTH;
	chip_info.mem_vendor = (val >> SYS_CTRL_MEM_VENDOR_SHIFT)
				& SYS_CTRL_MEM_VENDOR_WIDTH;

	return chip_info;
}

static int mv_sys_get_sensor_pll_id(void)
{
	u32 val;
	int ret = SYS_PLL_ID_OSC;

	val = readl(SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	if ((val & SYS_MAIN_CLK_SEL_PLL3_MASK) == 0)
		goto out;

	val = readl(SYS_VA_BASE + SYS_PLL_SRC_SEL);
	val &= SYS_PLL_SRC_SEL_PLL3_MASK;
	val >>= SYS_PLL_SRC_SEL_PLL3_SHIFT;
	if (val == 0)
		ret = SYS_PLL_ID_3;
	else
		ret = ffs((int)val) - 1;

out:
	return ret;
}

u32 mv_sys_get_sensor_clk_hz(void)
{
	u32 sensor_clk_hz;
	u32 src_clk_hz;
	int pll_id = mv_sys_get_sensor_pll_id();
	int div_val = 1;
	u32 val;

	if (pll_id == SYS_PLL_ID_OSC)
		src_clk_hz = mv_sys_get_osc_clk_mhz() * 1000000;
	else
		src_clk_hz = mv_sys_get_pll_clk_hz(pll_id);

	val = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	if (val & SYS_MAIN_CLK_SEL_PLL3_MASK) {
		sensor_clk_hz = src_clk_hz;
		goto out;
	}

	val = readl(SYS_VA_BASE + SYS_CLK_DIV3);
	div_val = ffs((int)val) + 1;
	sensor_clk_hz = src_clk_hz / div_val;

out:
	return sensor_clk_hz;
}
EXPORT_SYMBOL(mv_sys_get_sensor_clk_hz);

int mv_sys_set_sensor_clk_hz(int pll_id, u32 pll_out_hz, int sensor_div)
{
	u32 pll_src_sel, main_clk_sel, clk_bypass, clk_div3;
	int ret = 0;
	unsigned long flags;

	dbg("pll_out_hz=%u\n", pll_out_hz);

	if ((pll_id < SYS_PLL_ID_0) || (pll_id > SYS_PLL_ID_OSC)) {
		pr_err(DRV_NAME ": %s(): Invalid pll_id, %d\n",
			__func__, pll_id);
		ret = -EINVAL;
		goto out;
	}

	if ((sensor_div < 1) || (sensor_div > 8)) {
		pr_err(DRV_NAME ": %s(): Invalid sensor_div, %d\n",
			__func__, sensor_div);
		ret = -EINVAL;
		goto out;
	}

	dbg("pll_id=%d, pll_out_hz=%u, sensor_div=%d\n",
		pll_id, pll_out_hz, sensor_div);

	pll_src_sel = readl(SYS_VA_BASE + SYS_PLL_SRC_SEL);
	main_clk_sel = readl(SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	clk_bypass = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	clk_div3 = readl(SYS_VA_BASE + SYS_CLK_DIV3);

	spin_lock_irqsave(&sys_spinlock, flags);

	/* Change sensor clock source from PLL to OSC */
	main_clk_sel &= ~SYS_MAIN_CLK_SEL_PLL3_MASK;
	writel(main_clk_sel, SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

	/* Set sensor clock to bypass */
	clk_bypass |= SYS_CLK_BYPASS_PLL3_MASK;
	writel(clk_bypass, SYS_VA_BASE + SYS_CLK_BYPASS);

	/* PLL setting */
	if (pll_id == SYS_PLL_ID_OSC)
		goto skip_set_pll;

	spin_unlock_irqrestore(&sys_spinlock, flags);
	mv_sys_set_pll_clk_hz(pll_id, pll_out_hz);
	if (ret < 0) {
		pr_err(DRV_NAME ": %s(): Failed to mv_sys_set_pll_param\n",
			__func__);
		goto out;
	}
	spin_lock_irqsave(&sys_spinlock, flags);

skip_set_pll:
	/* Set sensor_div */
	pll_src_sel &= ~SYS_PLL_SRC_SEL_PLL3_MASK;
	clk_div3 &= ~SYS_CLK_DIV_VAL_MASK;
	if (pll_id != SYS_PLL_ID_OSC) {
		pll_src_sel |= ((1 << pll_id) << SYS_PLL_SRC_SEL_PLL3_SHIFT);
		if (sensor_div > 1) {
			clk_bypass &= ~SYS_CLK_BYPASS_PLL3_MASK;
			clk_div3 |= ((1 << (sensor_div - 2))
					<< SYS_CLK_DIV_VAL_SHIFT);
		}
	}

	/* Update values */
	writel(pll_src_sel, SYS_VA_BASE + SYS_PLL_SRC_SEL);
	writel(clk_div3, SYS_VA_BASE + SYS_CLK_DIV3);
	writel(clk_bypass, SYS_VA_BASE + SYS_CLK_BYPASS);

	/* Change sensor clock source from OSC to PLL */
	main_clk_sel |= SYS_MAIN_CLK_SEL_PLL3_MASK;
	writel(main_clk_sel, SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");
	asm volatile("nop");

	spin_unlock_irqrestore(&sys_spinlock, flags);

out:
	return ret;
}

int mv_sys_sensor_sclk_enable(int type, u32 div, int polarity)
{
	u32 val;
	int ret = 0;

	if (((type == SRC_SEN_PLL_SRC3) && ((div > 64) || (div < 1)))
		|| ((type == SRC_ISP_PLL_SRC3) && ((div > 8) || (div < 1)))
		|| ((type == SRC_ISP_BUS) && ((div > 8) || (div < 1)))) {
		pr_err(DRV_NAME ": %s(): Invalid division value: %d\n",
			__func__, div);
		ret = -EINVAL;
		goto out;
	}

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL1);

	if (type == SRC_SEN_PLL_SRC3) {
		if ((div <= 64) && (div >= 1)) {
			/* SCLK SELECTION CURRENT PLL3 : SENSOR INPUT SOURCE */
			systemRegisters->pClk1Ctrl.asField.sensorClkEn = 1;   // PLL source 3(sensor pll)
			systemRegisters->pClk1Ctrl.asField.sensorClkPol = (unsigned int)polarity; // sensor clk polarity

			systemRegisters->pClk1Ctrl.asField.sensorClkBypass = 0; // Pll source 3 bypass to SCLK
			systemRegisters->pClk1Ctrl.asField.sensorClkRatio = div; // sensor clk ratio '0'

			systemRegisters->pClk1Ctrl.asField.ispClkEn = 0; // disable isp clk
		}
	} else if (type == SRC_ISP_PLL_SRC3) {
		systemRegisters->pClk1Ctrl.asField.sensorClkBypass = 1; // Pll source 3 bypass to SCLK
		systemRegisters->pClk1Ctrl.asField.sensorClkRatio = 0; // sensor clk ratio '0'

		if ((div <= 8) && (div >= 1)) {
			systemRegisters->pClk1Ctrl.asField.ispClkEn = 1; // Clock Enable
			systemRegisters->pClk1Ctrl.asField.ispClkSel = 1;   // pll_src3
			systemRegisters->pClk1Ctrl.asField.ispClkPol = (unsigned int)polarity;
			systemRegisters->pClk1Ctrl.asField.ispClkBypass = 0;

			if(div == 1) {
				systemRegisters->pClk1Ctrl.asField.ispClkBypass = 1;
			} else {
				systemRegisters->pClk1Ctrl.asField.ispClkBypass = 0;
				systemRegisters->pClk1Ctrl.asField.ispClkDiv =  (1 << (div-2)); // DIV 8
			}

			systemRegisters->pClk1Ctrl.asField.sensorClkEn = 0;   // disable PLL source 3(sensor pll)
		}
	} else if (type == SRC_ISP_BUS) {
		systemRegisters->pClk1Ctrl.asField.sensorClkBypass = 1; // Pll source 3 bypass to SCLK
		systemRegisters->pClk1Ctrl.asField.sensorClkRatio = 0; // sensor clk ratio '0'

		if ((div <= 8) && (div != 0)) {
			systemRegisters->pClk1Ctrl.asField.ispClkEn = 1; // Clock Enable
			systemRegisters->pClk1Ctrl.asField.ispClkSel = 0;   // bus_clk
			systemRegisters->pClk1Ctrl.asField.ispClkPol = (unsigned int)polarity;
			systemRegisters->pClk1Ctrl.asField.ispClkBypass = 0;

			if (div == 1) {
				systemRegisters->pClk1Ctrl.asField.ispClkBypass = 1;
			} else {
				systemRegisters->pClk1Ctrl.asField.ispClkBypass = 0;
				systemRegisters->pClk1Ctrl.asField.ispClkDiv =  (1 << (div-2)); // DIV 8
			}

			systemRegisters->pClk1Ctrl.asField.sensorClkEn = 0;   // disable PLL source 3(sensor pll)
		}
	}

out:
	return ret;
}
EXPORT_SYMBOL(mv_sys_sensor_sclk_enable);

/*
    Disable SMCLK

*/

u32 mv_sys_disable_sensor_sclk(void)
{

    systemRegisters->pClk1Ctrl.asField.sensorClkEn = 0;   // disable PLL source 3(sensor pll)
    systemRegisters->pClk1Ctrl.asField.ispClkEn = 0; // disable isp clk

    return 0;
}
EXPORT_SYMBOL(mv_sys_disable_sensor_sclk);

/*
    Enable SEN0 CLK
    type : SEN0 Clock Source type
    mIsInvPolarity : invert output clock porarity

*/

u32 mv_sys_enable_sensor0_clk(SENx_SRC_TYPE type, bool mIsInvPolarity)
{

    u32 mmpResult = 0;

    systemRegisters->pClk1Ctrl.asField.sensorPclk0En = 1;         // sen_pclk0_en
    systemRegisters->pClk1Ctrl.asField.sensorPclk0Pol = (unsigned int)mIsInvPolarity;     // sensor pclk 0 polarity

    if(type == SRC_SENSOR_PCLK)
    {
        systemRegisters->pClk1Ctrl.asField.sensorPclk0Src = 0x0;  // pclk  USE FOR NORMAL PARALLEL SENSOR
        systemRegisters->pClk7Ctrl.asField.mrxPclkSelSen0 = 0;    // MUST BE CHECKED FROM JIN-HO !!!
    }
    else if(type == SRC_MIPI_RX_CLK)
    {
        systemRegisters->pClk1Ctrl.asField.sensorPclk0Src = 0x0;  // pclk   USE FOR MIPI SENSOR
        systemRegisters->pClk7Ctrl.asField.mrxPclkSelSen0 = 1;    // MUST BE CHECKED FROM JIN-HO !!!
    }
    else if(type == SRC_BUS_CLK)
    {
        systemRegisters->pClk1Ctrl.asField.sensorPclk0Src = 0x2;  // bus  USE for READ PATH
    }
    else if(type == SRC_SCLK)
    {
        systemRegisters->pClk1Ctrl.asField.sensorPclk0Src = 0x1;  // sclk
    }
    else
    {
        mmpResult = -1;
    }

    return mmpResult;
}
EXPORT_SYMBOL(mv_sys_enable_sensor0_clk);

/*
    Disable SEN0 CLK

*/

u32 mv_sys_disable_sensor0_clk(void)
{
    systemRegisters->pClk1Ctrl.asField.sensorPclk0En = 0;         // sen_pclk0_en

    return 0;
}
EXPORT_SYMBOL(mv_sys_disable_sensor0_clk);

/*
    Enable SEN0 CLK
    type : SEN0 Clock Source type
    mIsInvPolarity : invert output clock porarity

*/

u32 mv_sys_enable_sensor1_clk(SENx_SRC_TYPE type, bool mIsInvPolarity)
{

    u32   mmpResult = 0;

    systemRegisters->pClk1Ctrl.asField.sensorPclk1En = 1;         // sen_pclk0_en
    systemRegisters->pClk1Ctrl.asField.sensorPclk1Pol = (unsigned int)mIsInvPolarity;     // sensor pclk 0 polarity

    if(type == SRC_SENSOR_PCLK)
    {
        systemRegisters->pClk1Ctrl.asField.sensorPclk1Src = 0x0;  // pclk  USE FOR NORMAL PARALLEL SENSOR
        systemRegisters->pClk7Ctrl.asField.mrxPclkSelSen1 = 0;    // MUST BE CHECKED FROM JIN-HO !!!
    }
    else if(type == SRC_MIPI_RX_CLK)
    {
        systemRegisters->pClk1Ctrl.asField.sensorPclk1Src = 0x0;  // pclk  USE FOR MIPI SENSOR
        systemRegisters->pClk7Ctrl.asField.mrxPclkSelSen1 = 1;    // MUST BE CHECKED FROM JIN-HO !!!
    }
    else if(type == SRC_BUS_CLK)
    {
        systemRegisters->pClk1Ctrl.asField.sensorPclk1Src = 0x2;  // bus  USE for READ PATH
    }
    else if(type == SRC_SCLK)
    {
        systemRegisters->pClk1Ctrl.asField.sensorPclk1Src = 0x1;  // sclk
    }
    else
    {
        mmpResult = -1;
    }

    return mmpResult;

}
EXPORT_SYMBOL(mv_sys_enable_sensor1_clk);

/*
    Disable SEN1 CLK

*/

u32 mv_sys_disable_sensor1_clk(void)
{
    systemRegisters->pClk1Ctrl.asField.sensorPclk1En = 0;         // sen_pclk0_en

    return 0;
}
EXPORT_SYMBOL(mv_sys_disable_sensor1_clk);

/*
    Enable ISP CLK
    type : ISP Clock Source type
    mIsInvPolarity : invert output clock porarity

*/


u32 mv_sys_enable_isp_clk(SENx_SRC_TYPE type, bool mIsInvPolarity)
{

    u32  mmpResult = 0;

    systemRegisters->pClk1Ctrl.asField.sensorPclk2En = 1;         // sen_pclk0_en
    systemRegisters->pClk1Ctrl.asField.sensorPclk2Pol = (unsigned int)mIsInvPolarity;     // sensor pclk 0 polarity

    if(type == SRC_SENSOR_PCLK)
    {
        systemRegisters->pClk1Ctrl.asField.sensorPclk2Src = 0x0;  // pclk
        systemRegisters->pClk7Ctrl.asField.mrxPclkSelSen2 = 0;    // MUST BE CHECKED FROM JIN-HO !!!
    }
    else if(type == SRC_MIPI_RX_CLK)
    {
        systemRegisters->pClk1Ctrl.asField.sensorPclk2Src = 0x0;  // pclk
        systemRegisters->pClk7Ctrl.asField.mrxPclkSelSen2 = 1;    // MUST BE CHECKED FROM JIN-HO !!!
    }
    else if(type == SRC_BUS_CLK)
    {
        systemRegisters->pClk1Ctrl.asField.sensorPclk2Src = 0x2;  // bus
    }
    else if(type == SRC_SCLK)
    {
        systemRegisters->pClk1Ctrl.asField.sensorPclk2Src = 0x1;  // sclk
    }
    else
    {
        mmpResult = -1;
    }

    return mmpResult;

}
EXPORT_SYMBOL(mv_sys_enable_isp_clk);

/*
    Disable ISP CLK

*/

u32 mv_sys_disable_isp_clk(void)
{
    systemRegisters->pClk1Ctrl.asField.sensorPclk2En = 0;         // sen_pclk0_en

    return 0;

}
EXPORT_SYMBOL(mv_sys_disable_isp_clk);

/*
    Enable MIPI RX CONTROLLER CLK
    type : ISP Clock Source type
    mIsInvPolarity : invert output clock porarity

*/
u32 mv_sys_enable_mipi_rx_clk(MIPI_SRC_PLL_TYPE mipiPll, MIPI_RX_SRC_TYPE mipiSrc, int lane, unsigned int preDiv, unsigned int div)
{
    u32  mmpResult = 0;

    systemRegisters->pClk7Ctrl.asField.mrxClkEn = 0x1;

    if(mipiSrc == SRC_MRX_CLK_SRC_DIV)
    {
        systemRegisters->pClk7Ctrl.asField.mrxClkSrcEn = 1;    // ENABLE PLL CLK OUTPUT
        systemRegisters->pClk7Ctrl.asField.mrxClkSel = 0;
        systemRegisters->pClk7Ctrl.asField.mrxClkSrc = (unsigned int)mipiPll;

        if((preDiv < 1) || (preDiv > 8))
        {
            mmpResult = -1;
        }
        else
        {
            if((div != 2) && (div != 4) && (div != 8))
            {
                mmpResult =  -1;
            }
            else
            {
                if(preDiv == 1)
                {
                    systemRegisters->pClk7Ctrl.asField.mrxClkBp = 1;
                }
                else
                {
                    systemRegisters->pClk7Ctrl.asField.mrxClkSrcDivPre = (preDiv - 2);
                }
                systemRegisters->pClk7Ctrl.asField.mrxClkSrcDiv = ((div>>1)==0)? 0 : (((div>>2) == 0)? 1 : 2);
            }
        }

    }
    else if(mipiSrc == SRC_MRX_DDR_4)
    {
        systemRegisters->pClk7Ctrl.asField.mrxClkSrcEn = 0;    // DISABLE PLL CLK OUTPUT
        systemRegisters->pClk7Ctrl.asField.mrxClkSel = 0x1;
    }
    else if(mipiSrc == SRC_MRX_DDR_2)
    {
        systemRegisters->pClk7Ctrl.asField.mrxClkSrcEn = 0;    // DISABLE PLL CLK OUTPUT
        systemRegisters->pClk7Ctrl.asField.mrxClkSel = 0x2;
    }
    else if(mipiSrc == SRC_MRX_DDR)
    {
        //stemRegisters->pClk7Ctrl.asField.mrxClkSrcEn = 0;    // DISABLE PLL CLK OUTPUT
        systemRegisters->pClk7Ctrl.asField.mrxClkSrcEn = 1;   // Enable PLL CLK OUTPUT
        systemRegisters->pClk7Ctrl.asField.mrxClkSel = 3;
    }

    // KKod
    //systemRegisters->pClk7Ctrl.as32Bits |= 0x1000;	    /* mrxPclkSelSen0 = 1*/
    //systemRegisters->pClk7Ctrl.asField.mrxPclkSelSen0 = 1;

	if( lane == 1 ) {
	    systemRegisters->clockCtrl0.asField.i_RxByteClkHS0 = 0;
	} else if( lane == 2) {
		systemRegisters->clockCtrl0.asField.i_RxByteClkHS0 = 0;
		systemRegisters->clockCtrl0.asField.i_RxByteClkHS1 = 0;
	} else if( lane == 3 ) {
		systemRegisters->clockCtrl0.asField.i_RxByteClkHS0 = 0;
		systemRegisters->clockCtrl0.asField.i_RxByteClkHS1 = 0;
		systemRegisters->clockCtrl0.asField.i_RxByteClkHS2 = 0;
	} else if( lane == 4 ) {
		systemRegisters->clockCtrl0.asField.i_RxByteClkHS0 = 0;
		systemRegisters->clockCtrl0.asField.i_RxByteClkHS1 = 0;
		systemRegisters->clockCtrl0.asField.i_RxByteClkHS2 = 0;
		systemRegisters->clockCtrl0.asField.i_RxByteClkHS3 = 0;
	}
	systemRegisters->pClk7Ctrl.asField.mrxPclkSelSen0 = 0x1;

    return mmpResult;
}
EXPORT_SYMBOL(mv_sys_enable_mipi_rx_clk);

/*
    Disable MIPI RX CONTROLLER CLK

*/

u32 mv_sys_disable_mipi_rx_clk(void)
{
    systemRegisters->pClk7Ctrl.asField.mrxClkEn = 0;         // DISABLE MIPI RX CLK OUTPUT
    systemRegisters->pClk7Ctrl.asField.mrxClkSrcEn = 0;    // DISABLE PLL CLK OUTPUT

    return 0;

}
EXPORT_SYMBOL(mv_sys_disable_mipi_rx_clk);


/*
    Enable MIPI TX CONTROLLER CLK
    type : ISP Clock Source type
    mIsInvPolarity : invert output clock porarity

*/

u32 mv_sys_enable_mipi_tx_clk(MIPI_SRC_PLL_TYPE mipiPll)
{

    u32  mmpResult = 0;

    systemRegisters->pClk7Ctrl.asField.mtxClkEn  = 1;
    systemRegisters->pClk7Ctrl.asField.mtxClkSrc  = (unsigned int)mipiPll;
    systemRegisters->pClk7Ctrl.asField.txClkEscClkEn  = 1;
    systemRegisters->pClk7Ctrl.asField.txClkHsClkEn  = 1;

    return mmpResult;
}
EXPORT_SYMBOL(mv_sys_enable_mipi_tx_clk);


/*
    Disable MIPI TX CONTROLLER CLK

*/

u32 mv_sys_disable_mipi_tx_clk(void)
{
    systemRegisters->pClk7Ctrl.asField.mtxClkEn  = 0;
    systemRegisters->pClk7Ctrl.asField.txClkEscClkEn  = 0;
    systemRegisters->pClk7Ctrl.asField.txClkHsClkEn  = 0;

    return 0;

}

EXPORT_SYMBOL(mv_sys_disable_mipi_tx_clk);

void mv_sys_ext_pwr_int_set (unsigned int ext_irq_num , int ext_int_poll, int enable)
{
	unsigned int read_val = 0;
	unsigned int write_val = 0;

	/* External interrupt select in order to make wake up signal */
	read_val = readl(SYS_VA_BASE + SYS_INT_PWR_SEL);
//	printk("%s:read1 val(0x%x)\n",__func__,read_val);
	write_val = read_val | (1<< ext_irq_num);
//	printk("%s:write1 val(0x%x)\n",__func__,write_val);
	writel(write_val, SYS_VA_BASE + SYS_INT_PWR_SEL);

	/* External interrupt pollarity select 1: high, 0: low signal detect */
	read_val = readl(SYS_VA_BASE + SYS_INT_PWR_POL);
#if 1
	if(ext_int_poll == 1)
	write_val = read_val | (1<< ext_irq_num);
	else
	write_val = read_val & (~(1<< ext_irq_num));
#endif
//	printk("%s:read2 val(0x%x)\n",__func__,read_val);
//	write_val = 0xffffffff;
//	printk("%s:write2 val(0x%x)\n",__func__,write_val);
	writel(write_val, SYS_VA_BASE + SYS_INT_PWR_POL);

	/* when exteral interrupt occured,
	 * this signal transfer to Power_control0(GPIO204)
	 * or Power_control1(GPIO207).
	 */
	read_val = readl(SYS_VA_BASE + SYS_EXT_INT_CTRL);
//	printk("%s:read3 val(0x%x)\n",__func__,read_val);
	if(enable == 1)
		write_val = read_val | 0x7;
	else
		write_val = 0x0;
//	printk("%s:write3 val(0x%x)\n",__func__,write_val);
	writel(write_val, SYS_VA_BASE + SYS_EXT_INT_CTRL);
}
EXPORT_SYMBOL(mv_sys_ext_pwr_int_set);


void mv_sys_mvc_pwr_cut(void)
{
	u32 val;
//	printk("%s:enter\n",__func__);
	val = readl(SYS_VA_BASE + SYS_CTRL0);
//	printk("%s:read1 val(0x%x)\n",__func__,val);

	val |= SYS_CTRL_MVC_PWRCUT_MASK;
//	printk("%s:read2 val(0x%x)\n",__func__,val);

//	msleep(100);
	writel(val ,SYS_VA_BASE + SYS_CTRL0);

	val = readl(SYS_VA_BASE + SYS_CTRL0);
//	printk("%s:read3 val(0x%x)\n",__func__,val);

	val |= SYS_CTRL_CIO_CTRL_MASK;
//	printk("%s:read2 val(0x%x)\n",__func__,val);

//	msleep(100);
	writel(val ,SYS_VA_BASE + SYS_CTRL0);
}
EXPORT_SYMBOL(mv_sys_mvc_pwr_cut);

#if 1 /* leehh 100819  */
void mv_sys_dev_usb_set(int no)
{
	int offset = (no / 32) << 2;
	u32 mask = 1 << (no % 32);
	u32 val;
	unsigned long flags;

	if (no >= SYS_RST_NUMBER) {
		printk(KERN_ERR "%s: %s(): error: unknown device number=%d\n",
			DRV_NAME, __func__, no);
		BUG();
	}

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_RST_CTRL0 + offset);
	if (no == SYS_RST_HDMI)
		val |= 0x00000030;
	val |= mask;
	writel(val, SYS_VA_BASE + SYS_RST_CTRL0 + offset);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}
EXPORT_SYMBOL(mv_sys_dev_usb_set);

void mv_sys_dev_usb_unset(int no)
{
	int offset = (no / 32) << 2;
	u32 mask = 1 << (no % 32);
	u32 val;
	unsigned long flags;

	if (no >= SYS_RST_NUMBER) {
		printk(KERN_ERR "%s: %s(): error: unknown device number=%d\n",
			DRV_NAME, __func__, no);
		BUG();
	}

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_RST_CTRL0 + offset);
	if (no == SYS_RST_HDMI)
		val |= 0x00000030;

	val &= ~mask;
	writel(val, SYS_VA_BASE + SYS_RST_CTRL0 + offset);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}
EXPORT_SYMBOL(mv_sys_dev_usb_unset);

#endif
void mv_sys_hdmi_pwr_cut_ctrl(int en)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_CTRL0);
	if (en)
		val &= ~SYS_CTRL_HDMI_PWR_CUT_MASK;
	else
		val |= SYS_CTRL_HDMI_PWR_CUT_MASK;
	writel(val, SYS_VA_BASE + SYS_CTRL0);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_set_sfr_clk(int clk_src, int div, int polarity)
{
	u32 val;
	unsigned long flags;

	if ((div <= 0) || (div > 64)) {
		pr_err(DRV_NAME ": %s(): Invalid division value: %d\n",
			__func__, div);
		BUG();
	}

	spin_lock_irqsave(&sys_spinlock, flags);
	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL0);

	if (polarity)
		val |= SYS_PERI_CLK_SFR_POL_MASK;
	else
		val &= ~SYS_PERI_CLK_SFR_POL_MASK;

	if (clk_src == SYS_SFR_CLK_SRC_DAC)
		val |= SYS_PERI_CLK_SFR_SEL_MASK;
	else
		val &= ~SYS_PERI_CLK_SFR_SEL_MASK;

	val &= ~SYS_PERI_CLK_SFR_RATIO_MASK;
	val |= (((div - 1) & SYS_PERI_CLK_SFR_RATIO_WIDTH)
		<< SYS_PERI_CLK_SFR_RATIO_SHIFT);
	if (div == 1)
		val |= SYS_PERI_CLK_SFR_BYPASS_MASK;
	else
		val &= ~SYS_PERI_CLK_SFR_BYPASS_MASK;

	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL0);
	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_sfr_clk_ctrl(int en)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);
	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL0);
	if (en)
		val |= SYS_PERI_CLK_SFR_EN_MASK;
	else
		val &= ~SYS_PERI_CLK_SFR_EN_MASK;
	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL0);
	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_set_sfr_clk_en(u32 value)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL0);

	if (value == 0)
		val &= ~SYS_PERI_CLK_SFR_EN_MASK;
	else
		val |= SYS_PERI_CLK_SFR_EN_MASK;

	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL0);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_set_cec_clk_ratio(u32 ratio)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL6);

	ratio <<= SYS_PERI_CLK_CEC_DIV_SHIFT;
	ratio &= SYS_PERI_CLK_CEC_DIV_MASK;

	val &= ~SYS_PERI_CLK_CEC_DIV_MASK;
	val |= ratio;

	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL6);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_set_cec_clk_en(u32 value)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL6);

	if (value == 0)
		val &= ~SYS_PERI_CLK_CEC_ENA_MASK;
	else
		val |= SYS_PERI_CLK_CEC_ENA_MASK;

	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL6);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_set_tv_clk_ratio(u32 ratio)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL0);

	ratio <<= SYS_PERI_CLK_TV_CLK_RATIO_SHIFT;
	ratio &= SYS_PERI_CLK_TV_CLK_RATIO_MASK;
	val &= ~SYS_PERI_CLK_TV_CLK_RATIO_MASK;
	val |= ratio;

	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL0);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_set_tv_clk_bypass(u32 value)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL0);

	value <<= SYS_PERI_CLK_TV_CLK_BYPASS_SHIFT;
	value &= SYS_PERI_CLK_TV_CLK_BYPASS_MASK;
	val &= ~SYS_PERI_CLK_TV_CLK_BYPASS_MASK;
	val |= value;

	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL0);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_set_tv_clk_enable(u32 value)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_PERI_CLK_CTRL0);

	value <<= SYS_PERI_CLK_TV_CLK_ENA_SHIFT;
	value &= SYS_PERI_CLK_TV_CLK_ENA_MASK;
	val &= ~SYS_PERI_CLK_TV_CLK_ENA_MASK;
	val |= value;

	writel(val, SYS_VA_BASE + SYS_PERI_CLK_CTRL0);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_set_dip_reset(u32 value)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_RST_CTRL0);


	value <<= 6;
	value &= (1 << 6);
	val &= ~(1 << 6);
	val |= value;

	writel(val, SYS_VA_BASE + SYS_RST_CTRL0);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

void mv_sys_set_hdmi_reset(u32 value)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);

	val = readl(SYS_VA_BASE + SYS_RST_CTRL2);

	value <<= 6;
	value &= (1 << 6);
	val &= ~(1 << 6);
	val |= value;

	val |= 0x00000030;

	writel(val, SYS_VA_BASE + SYS_RST_CTRL2);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

#if defined(CONFIG_ARCH_MVFORMULA)
void mv_sys_set_enet_init(void)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);

#if 1
	val = readl(SYS_VA_BASE + SYS_ENET_CTRL);
    	val &= ~SYS_ETH_CTRL_PHY_PLL_SEL_MASK;
	val |= (0 << SYS_ETH_CTRL_PHY_PLL_SEL_SHIFT);
    	val &= ~SYS_ETH_CTRL_PHY_CLK_EN_MASK;
	val |= (0 << SYS_ETH_CTRL_PHY_CLK_EN_SHIFT);
    	val &= ~SYS_ETH_CTRL_SBD_DATA_ENDIAN_MASK;
	val |= (0 << SYS_ETH_CTRL_SBD_DATA_ENDIAN_SHIFT);
	writel(val, SYS_VA_BASE + SYS_ENET_CTRL);
#else
    	systemRegisters->ethCtrl.asField.sbd_data_endianness = 0;
    	systemRegisters->ethCtrl.asField.phy_clk_en = 0;
    	systemRegisters->ethCtrl.asField.phy_pll_sel = 0;
#endif

	spin_unlock_irqrestore(&sys_spinlock, flags);
}
#endif  //#if defined(CONFIG_ARCH_MVFORMULA)

#define BO_DUMP_REG(reg) \
	do {val = readl(SYS_VA_BASE + reg); printk(KERN_INFO "    " #reg \
			" 0x%.8x : 0x%.8x\n", reg, val);} while(0)
void mv_sys_dump_regs(void)
{
	u32 val;
	unsigned long flags;

	spin_lock_irqsave(&sys_spinlock, flags);

	BO_DUMP_REG(SYS_PRODUCT_ID);
	BO_DUMP_REG(SYS_CTRL0);
	BO_DUMP_REG(SYS_CTRL1);
	BO_DUMP_REG(SYS_CTRL2);
	BO_DUMP_REG(SYS_GB_RST_CTRL);
	BO_DUMP_REG(SYS_RST_CTRL0);
	BO_DUMP_REG(SYS_RST_CTRL1);
	BO_DUMP_REG(SYS_RST_CTRL2);
	BO_DUMP_REG(SYS_GB_CLK_CTRL);
	BO_DUMP_REG(SYS_CLK_CTRL0);
	BO_DUMP_REG(SYS_CLK_CTRL1);
	BO_DUMP_REG(SYS_PLL_SRC_SEL);
	BO_DUMP_REG(SYS_MAIN_CLK_SEL);
	BO_DUMP_REG(SYS_CLK_BYPASS);
	BO_DUMP_REG(SYS_CLK_DIV0);
	BO_DUMP_REG(SYS_CLK_DIV1);
	BO_DUMP_REG(SYS_CLK_DIV2);
	BO_DUMP_REG(SYS_CLK_DIV3);
	BO_DUMP_REG(SYS_CLK_DIV4);
	BO_DUMP_REG(SYS_SYS_CLK_DIV);
	BO_DUMP_REG(SYS_PERI_CLK_CTRL0);
	BO_DUMP_REG(SYS_PERI_CLK_CTRL1);
	BO_DUMP_REG(SYS_PERI_CLK_CTRL2);
	BO_DUMP_REG(SYS_PERI_CLK_CTRL3);
	BO_DUMP_REG(SYS_PERI_CLK_CTRL4);
	BO_DUMP_REG(SYS_PERI_CLK_CTRL5);
	BO_DUMP_REG(SYS_PERI_CLK_CTRL6);
	BO_DUMP_REG(SYS_PERI_CLK_CTRL7);
	BO_DUMP_REG(SYS_PLL_PARAM0);
	BO_DUMP_REG(SYS_PLL_PARAM1);
	BO_DUMP_REG(SYS_PLL_PARAM2);
	BO_DUMP_REG(SYS_PLL_PARAM3);
	BO_DUMP_REG(SYS_PLL_PARAM4);
	BO_DUMP_REG(SYS_PERI_CLK_CTRL8);
	BO_DUMP_REG(SYS_PE_CTRL);
	BO_DUMP_REG(SYS_DMC_PAD_CTRL0);
	BO_DUMP_REG(SYS_DMC_PAD_CTRL1);
	BO_DUMP_REG(SYS_ARM11_CTRL);
	BO_DUMP_REG(SYS_OTP_STATUS0);
	BO_DUMP_REG(SYS_OTP_STATUS1);
	BO_DUMP_REG(SYS_OTP_STATUS2);
	BO_DUMP_REG(SYS_ALT_FUNC_ENA);
	BO_DUMP_REG(SYS_APS_MODE);
	BO_DUMP_REG(SYS_ISP_DEBUG);
	BO_DUMP_REG(SYS_EXT_INT_CTRL);
	BO_DUMP_REG(SYS_INT_PWR_SEL);
	BO_DUMP_REG(SYS_INT_PWR_POL);
	BO_DUMP_REG(SYS_INT_PWR_KEY);

	spin_unlock_irqrestore(&sys_spinlock, flags);
}

#if defined(CONFIG_MVFORMULA_PM_DYNAMIC_CLOCK)

#define	DENALI_REG(n)		(n << 2)
#define	l_readl(c)		(*((volatile unsigned int *)(c)))
#define	l_writel(v, c)		\
	do { (*((volatile unsigned int *)(c)) = ((unsigned int)v)); } while(0)

#define	nops(n)	\
	do { for(val = (n); val > 0; val--) asm volatile("nop"); } while(0)

static unsigned int change_system_pll_data[16];

static void change_system_pll(const unsigned int *data)
{
	register int val;

	/*
	 * DRAM use alt pll(pll4)
	 */
#if	0
	l_writel(0x00000081, SYS_VA_BASE + SYS_PLL_SRC_SEL);
	l_writel(0x07000000, SYS_VA_BASE + SYS_PERI_CLK_CTRL0);
#endif	/*0*/

	/*
	 * Enter DRAM Self-Refresh
	 */
	do {
		val = l_readl(SYS_VA_BASE + SYS_CTRL2);
	} while(val & (1 << 11));

	val = l_readl(DMC_VA_BASE + DENALI_REG(10));
	val |= (1 << 16);
	l_writel(val, DMC_VA_BASE + DENALI_REG(10));

	do {
		val = l_readl(DMC_VA_BASE + DENALI_REG(3));
	} while(val & (1 << 16));

	val = l_readl(DMC_VA_BASE + DENALI_REG(10));
	val &= ~(1 << 24);
	l_writel(val, DMC_VA_BASE + DENALI_REG(10));

	val = l_readl(DMC_VA_BASE + DENALI_REG(33));
	val &= 0x1ff;
	val |= (1 << 10);
	l_writel(val, DMC_VA_BASE + DENALI_REG(33));
	nops(50);

	/*
	 * Change system clock.
	 */

	/* change PLL to OSC */
	l_writel(data[3], SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	nops(50);

	/* setup pll */
	val = l_readl(SYS_VA_BASE + SYS_PLL_PARAM0);
	val &= ~SYS_PLL_PARAM0_RST_MASK;
	l_writel(val, SYS_VA_BASE + SYS_PLL_PARAM0);

	val = l_readl(SYS_VA_BASE + SYS_PLL_PARAM0);
	val &= ~(SYS_PLL_PARAM0_NR_MASK | \
			SYS_PLL_PARAM0_NO_MASK | SYS_PLL_PARAM0_NF_MASK);
	val |= (data[4] & \
			(SYS_PLL_PARAM0_NR_MASK | \
			 SYS_PLL_PARAM0_NO_MASK | SYS_PLL_PARAM0_NF_MASK));

	val |= SYS_PLL_PARAM0_RST_MASK;
	l_writel(val, SYS_VA_BASE + SYS_PLL_PARAM0);

	val &= ~SYS_PLL_PARAM0_RST_MASK;
	l_writel(val, SYS_VA_BASE + SYS_PLL_PARAM0);

	do {
		val = l_readl(SYS_VA_BASE + SYS_PLL_PARAM0);
	} while(!(val & SYS_PLL_PARAM0_LOCK_MASK));

	/* all system clock divider => div-2 */
	l_writel(data[8], SYS_VA_BASE + SYS_SYS_CLK_DIV);

	l_writel(data[5], SYS_VA_BASE + SYS_CLK_BYPASS);
	l_writel(data[6], SYS_VA_BASE + SYS_CLK_DIV0);
	l_writel(data[7], SYS_VA_BASE + SYS_SYS_CLK_DIV);

	/* change OSC to PLL */
	l_writel(data[3] | 0x3, SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	nops(50);

	/*
	 * Exit DRAM Self-Refresh
	 */
	val = l_readl(DMC_VA_BASE + DENALI_REG(10));
	val |= (1 << 24);
	l_writel(val, DMC_VA_BASE + DENALI_REG(10));

	do {
		val = l_readl(DMC_VA_BASE + DENALI_REG(4));
	} while(!(val & (1 << 8)));

	val = l_readl(DMC_VA_BASE + DENALI_REG(10));
	val &= ~(1 << 16);
	l_writel(val, DMC_VA_BASE + DENALI_REG(10));

	val = l_readl(DMC_VA_BASE + DENALI_REG(9));
	val |= 0x1;
	l_writel(val, DMC_VA_BASE + DENALI_REG(9));
}

int mv_sys_change_sys_clk(struct mv_sys_clk_param *param)
{
	int ret = -1;
	void *ncbuf_va;
	dma_addr_t ncbuf_pa;
	unsigned int val;
	unsigned int pll_src_sel, main_clk_sel, clk_bypass;
	unsigned int sys_clk_div, clk_div0;
	unsigned long flags;

	/* always pll0 */
	int pll_id = SYS_PLL_ID_0;

	/* defined in cachelock.S */
	extern void mv_cache_locker(void);
	extern int mv_cache_lock_id3(void *, int, void *, int);
	extern void mv_cache_unlock_id3(void);

	if(!param)
		goto done;

	/* alloc non-cacheable buffer for lockdown code */
	ncbuf_va = dma_alloc_coherent(NULL, PAGE_SIZE, &ncbuf_pa, GFP_KERNEL);
	if(!ncbuf_va) {
		printk("Non-cacheable buffer allocation failed. "
				"Aborted system clock change.\n");
		goto done;
	}
	dbg("ncbuf = 0x%x\n", ncbuf_va);

	/*
	 * Prepare setup data
	 */
	change_system_pll_data[0] = (unsigned int)ncbuf_va;
	change_system_pll_data[3] = readl(SYS_VA_BASE + SYS_MAIN_CLK_SEL) & (~0x3);

	pll_src_sel = readl(SYS_VA_BASE + SYS_PLL_SRC_SEL);
	main_clk_sel = readl(SYS_VA_BASE + SYS_MAIN_CLK_SEL);
	clk_bypass = readl(SYS_VA_BASE + SYS_CLK_BYPASS);
	sys_clk_div = readl(SYS_VA_BASE + SYS_SYS_CLK_DIV);
	clk_div0 = readl(SYS_VA_BASE + SYS_CLK_DIV0);

	val = readl(SYS_VA_BASE + SYS_PLL_PARAM0);
	val &= ~(SYS_PLL_PARAM0_NR_MASK | \
			SYS_PLL_PARAM0_NO_MASK | SYS_PLL_PARAM0_NF_MASK);
	val |= ((param->nr << SYS_PLL_PARAM0_NR_SHIFT) \
			& SYS_PLL_PARAM0_NR_MASK);
	val |= ((param->no << SYS_PLL_PARAM0_NO_SHIFT) \
			& SYS_PLL_PARAM0_NO_MASK);
	val |= ((param->nf << SYS_PLL_PARAM0_NF_SHIFT) \
			& SYS_PLL_PARAM0_NF_MASK);
	dbg("nr = 0x%x, no = 0x%x, nf = 0x%x\n", \
			param->nr, param->no, param->nf);
	/* PLL0_PARAM */
	change_system_pll_data[4] = val;

	/* Set sys_div */
	pll_src_sel &= ~SYS_PLL_SRC_SEL_PLL0_MASK;
	clk_div0 &= ~SYS_CLK_DIV_VAL_MASK;
	clk_bypass |= SYS_CLK_BYPASS_PLL0_MASK;
	if (pll_id != SYS_PLL_ID_OSC) {
		pll_src_sel |= ((1 << pll_id) << SYS_PLL_SRC_SEL_PLL0_SHIFT);
		if (param->sys > 1) {
			clk_bypass &= ~SYS_CLK_BYPASS_PLL0_MASK;
			clk_div0 |= ((1 << (param->sys - 2))
					<< SYS_CLK_DIV_VAL_SHIFT);
		}
	}

	/* Set arm_div */
	sys_clk_div &= ~SYS_SYS_CLK_DIV_ARM_MASK;
	clk_bypass |= SYS_CLK_BYPASS_SYS_ARM_MASK;
	if (param->arm > 1) {
		sys_clk_div |= ((1 << (param->arm - 2))
				<< SYS_SYS_CLK_DIV_ARM_SHIFT);
		clk_bypass &= ~SYS_CLK_BYPASS_SYS_ARM_MASK;
	}

	/* Set bus_div */
	sys_clk_div &= ~SYS_SYS_CLK_DIV_BUS_MASK;
	clk_bypass |= SYS_CLK_BYPASS_SYS_BUS_MASK;
	if (param->bus > 1) {
		sys_clk_div |= ((1 << (param->bus - 2))
				<< SYS_SYS_CLK_DIV_BUS_SHIFT);
		clk_bypass &= ~SYS_CLK_BYPASS_SYS_BUS_MASK;
	}

	/* Set dmc1_div */
	sys_clk_div &= ~SYS_SYS_CLK_DIV_DMC1_MASK;
	sys_clk_div |= ((param->dmc1 - 2) << SYS_SYS_CLK_DIV_DMC1_SHIFT);

	/* Set dmc2_div */
	sys_clk_div &= ~SYS_SYS_CLK_DIV_DMC2_MASK;
	sys_clk_div |= ((param->dmc2 - 1) << SYS_SYS_CLK_DIV_DMC2_SHIFT);

	/* Set apb_div */
	sys_clk_div &= ~SYS_SYS_CLK_DIV_APB_MASK;
	clk_bypass |= SYS_CLK_BYPASS_SYS_APB_MASK;
	if (param->apb > 1) {
		sys_clk_div |= ((1 << (param->apb - 2))
				<< SYS_SYS_CLK_DIV_APB_SHIFT);
		clk_bypass &= ~SYS_CLK_BYPASS_SYS_APB_MASK;
	}

	/* CLK_BYPASS */
	dbg("SYS_CLK_BYPASS = 0x%08x\n", clk_bypass);
	change_system_pll_data[5] = clk_bypass;

	/* CLK_DIV0 */
	dbg("SYS_CLK_DIV0 = 0x%08x\n", clk_div0);
	change_system_pll_data[6] = clk_div0;

	/* SYS_CLK_DIV(after) */
	dbg("SYS_SYS_CLK_DIV = 0x%08x\n", sys_clk_div);
	change_system_pll_data[7] = sys_clk_div;

	/* SYS_CLK_DIV(before) */
	change_system_pll_data[8] = 0x08010101;

	/*
	 * copy cache-locker-code to non-cacheable area.
	 */
	memcpy(ncbuf_va, (void *)mv_cache_locker, PAGE_SIZE);

	/* 1. disable irq */
	spin_lock_irqsave(&sys_spinlock, flags);

	/* 2. fill cache way3 */
	ret = mv_cache_lock_id3((void *)change_system_pll, PAGE_SIZE, \
			(void *)change_system_pll_data, 128);

	/* 3. execute in cache */
	change_system_pll(change_system_pll_data);

	mv_cache_unlock_id3();

	spin_unlock_irqrestore(&sys_spinlock, flags);

	dma_free_coherent(NULL, PAGE_SIZE, ncbuf_va, ncbuf_pa);

done:
	return ret;
}
EXPORT_SYMBOL(mv_sys_change_sys_clk);
#endif	/*CONFIG_MVFORMULA_PM_DYNAMIC_CLOCK*/

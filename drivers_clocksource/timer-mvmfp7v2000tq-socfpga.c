/*
 *  drivers/clocksource/timer-mvmfp7v2000tq-socfpga.c
 *
 *  Copyright (C) 2015 MtekVision Co., Ltd.
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
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/clockchips.h>
#include <linux/clocksource.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/of_fdt.h>
#include <linux/of_device.h>

#include <asm/system.h>
#include <asm/irq.h>

#include <asm/mach/time.h>

#include <mach/hardware.h>
#include <mach/sys.h>
#include <mach/pwm.h>

#define DRV_NAME	"mvmfp7v2000tq-timer"

/*
 * Debug Level
 *  2 : Print all debug messages
 *  1 : Print only dbg() messages
 *  0 : No debug messages
 */
#define MVFORMULA_TIME_DEBUG_LEVEL  0
#if (MVFORMULA_TIME_DEBUG_LEVEL == 2)
#define dbg(format, arg...) \
	printk(KERN_ALERT DRV_NAME ": Debug: " format, ## arg)
#define enter()         \
	printk(KERN_ALERT DRV_NAME ": Enter: %s\n", __func__)
#define leave()         \
	printk(KERN_ALERT DRV_NAME ": Leave: %s\n", __func__)
#elif (MVFORMULA_TIME_DEBUG_LEVEL == 1)
#define dbg(format, arg...) \
	printk(KERN_ALERT DRV_NAME ": Debug: " format, ## arg)
#define enter()
#define leave()
#else
#define dbg(format, arg...)
#define enter()
#define leave()
#endif

#if TIMER_MEASURE
#include <mach/gpio.h>

#define TIMER_MEASURE_GPIO	GPIO88
static int timer_measure_level;
#endif /* TIMER_MEASURE */

#define TIMER_CNT 	6
#define DUTY_CYCLE 50

struct mv_timer {
	unsigned int clksrc_id;
	irq_hw_number_t clksrc_hwirq;
	unsigned int clkevt_id;
	irq_hw_number_t clkevt_hwirq;
	struct irq_domain *dm;
	struct device_node *path;
	void __iomem *base;
	u32 cntb;
#if !defined(CONFIG_GENERIC_CLOCKEVENTS)
	unsigned int clk_id;
	irq_hw_number_t clk_hwirq;
	u32 usec;
#else
	u32 hz;
#endif
};

static struct mv_timer sys_timer = {
	.clksrc_id = 0,
	.clkevt_id = 0,
	.path = NULL,
	.base = NULL,
	.cntb = 0,
#if !defined(CONFIG_GENERIC_CLOCKEVENTS)
	.usec = USEC_PER_SEC / HZ,
#else
	.hz = 3000000
#endif /* !defined(CONFIG_GENERIC_CLOCKEVENTS) */
};

#if defined(CONFIG_GENERIC_CLOCKEVENTS)
static enum clock_event_mode clockevent_mode;

static const struct of_device_id timer_ids[] __initconst = {
	{ .compatible = "pwm-timer0" },
	{ .compatible = "pwm-timer1" },
	{ .compatible = "pwm-timer2" },
	{ .compatible = "pwm-timer3" },
	{ .compatible = "pwm-timer4" },
	{ .compatible = "pwm-timer5" },
	{},
};

cycle_t mv_timer_read(void)
{
	u64 tdone, tval;

	tval = mv_pwm_get_cnt_obs(sys_timer.clksrc_id);
	tdone = PWM_TCNTB_CNT_BUF_WIDTH - tval;

	return tdone;
}

static struct clocksource mv_timer_clocksource = {
	.name = "clock_source",
	.rating = 200,
	.read = mv_timer_read,
	.mask = CLOCKSOURCE_MASK(16),
	.flags = CLOCK_SOURCE_IS_CONTINUOUS,
};

static irqreturn_t mv_timer_clocksource_irq(int irq, void *dev_id)
{
	mv_pwm_int_clear(sys_timer.clksrc_id);

	return IRQ_HANDLED;
}

static struct irqaction mv_timer_clocksource_irqaction = {
	.name = "clock_source_handler",
	.flags = (IRQF_DISABLED | IRQF_TIMER),
	.handler = mv_timer_clocksource_irq,
};

static void mv_timer_clocksource_init(void)
{
	struct clocksource *cs = &mv_timer_clocksource;
	struct device_node *timer = NULL;
	struct irq_data *irq_dt = NULL;
	const char *path = NULL;
	unsigned int virq;
	unsigned int hwirq;
	unsigned int id;
	u32 hz;
	u32 src_clk_hz;
	u32 shift;
	u64 temp;

	id = sys_timer.clksrc_id;
	hz = sys_timer.hz;

	timer = of_find_matching_node(sys_timer.path, &timer_ids[id]);
	if (WARN(!timer, "Failed to find timer path"))
		return;

	// irq = irq_of_parse_and_map(timer, 0);
	virq = irq_find_mapping(sys_timer.dm, sys_timer.clksrc_hwirq);
	if (virq == 0) {
		pr_warning("Try to map clksrc hwirq\n");
		virq = irq_create_mapping(sys_timer.dm, sys_timer.clksrc_hwirq);
		if (WARN(virq == 0, "Failed to map clksrc hwirq"))
			return;
	}

	irq_dt = irq_get_irq_data(sys_timer.clksrc_hwirq);
	hwirq = irq_dt->hwirq;

	setup_irq(hwirq, &mv_timer_clocksource_irqaction);

	src_clk_hz = mv_pwm_config_hz(id, hz, DUTY_CYCLE);
	mv_pwm_set_cnt_buf(id, PWM_TCNTB_CNT_BUF_WIDTH, DUTY_CYCLE);
	mv_pwm_start(id, 0);

	for (shift = 32; shift > 0; shift--) {
		temp = (u64)NSEC_PER_SEC << shift;
		do_div(temp, src_clk_hz);
		if ((temp >> 32) == 0)
			break;
	}

	cs->shift = shift;
	cs->mult = (u32)temp;

	dbg("[CLKSRC] virq %d hwirq %d\n", virq, hwirq);
	dbg("[CLKSRC] PWM source clock %u\n", src_clk_hz);
	dbg("[CLKSRC] shift %u mult %u\n", cs->shift, cs->mult);

	clocksource_register(cs);
}

static void mv_timer_set_mode(enum clock_event_mode mode,
		struct clock_event_device *ce)
{
	unsigned int id = sys_timer.clkevt_id;
	u32 cntb = sys_timer.cntb;

	dbg("[CLKEVT] mode = %s\n", mode == 0 ? "CLOCK_EVT_MODE_UNUSED" :
				  mode == 1 ? "CLOCK_EVT_MODE_SHUTDOWN" :
				  mode == 2 ? "CLOCK_EVT_MODE_PERIODIC" :
				  mode == 3 ? "CLOCK_EVT_MODE_ONESHOT" :
							  "CLOCK_EVT_MODE_RESUME");

	clockevent_mode = mode;

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		mv_pwm_set_cnt_buf(id, cntb, DUTY_CYCLE);
		mv_pwm_start(id, 0);
		break;
	case CLOCK_EVT_MODE_RESUME:
	case CLOCK_EVT_MODE_SHUTDOWN:
	case CLOCK_EVT_MODE_ONESHOT:
		mv_pwm_stop(id);
		break;
	default:
		break;
	}
}

static int mv_timer_set_next_event(unsigned long cycle,
		struct clock_event_device *ce)
{
	unsigned int id = sys_timer.clkevt_id;
	
	dbg("[CLKEVT] cycle 0x%08x\n", (u32)cycle);

	mv_pwm_set_cnt_buf(id, cycle, DUTY_CYCLE);
	if (clockevent_mode == CLOCK_EVT_MODE_ONESHOT)
		mv_pwm_start(id, 1);
	else
		mv_pwm_start(id, 0);

	return 0;
}

static struct clock_event_device mv_timer_clockevent = {
	.name = "clock_event_device",
	.features = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.rating = 200,
	.set_mode = mv_timer_set_mode,
	.set_next_event	= mv_timer_set_next_event,
};

static irqreturn_t mv_timer_clockevent_irq(int irq, void *dev_id)
{
	struct clock_event_device *evt = &mv_timer_clockevent;

	mv_pwm_int_clear(sys_timer.clkevt_id);
	evt->event_handler(evt);

	return IRQ_HANDLED;
}

static struct irqaction mv_timer_clockevent_irqaction = {
	.name = "clock_event_handler",
	.flags = (IRQF_DISABLED | IRQF_TIMER),
	.handler = mv_timer_clockevent_irq,
};

static void mv_timer_clockevent_init(void)
{
	struct clock_event_device *ce = &mv_timer_clockevent;
	struct device_node *timer = NULL;
	struct irq_data *irq_dt = NULL;
	const char *path = NULL;
	unsigned int virq;
	unsigned int hwirq;
	unsigned int id;
	u32 hz;
	u32 src_clk_hz;
	u32 shift;
	u64 temp;

	id = sys_timer.clkevt_id;
	hz = sys_timer.hz;

	timer = of_find_matching_node(sys_timer.path, &timer_ids[id]);
	if (WARN(!timer, "Failed to find timer path"))
		return;

	// irq = irq_of_parse_and_map(timer, 0);
	virq = irq_find_mapping(sys_timer.dm, sys_timer.clkevt_hwirq);
	if (virq == 0) {
		pr_warning("Try to map clkevt hwirq\n");
		virq = irq_create_mapping(sys_timer.dm, sys_timer.clkevt_hwirq);
		if (WARN(virq == 0, "Failed to map clkevt hwirq"))
			return;
	}

	irq_dt = irq_get_irq_data(sys_timer.clkevt_hwirq);
	hwirq = irq_dt->hwirq;

	setup_irq(hwirq, &mv_timer_clockevent_irqaction);

	src_clk_hz = mv_pwm_config_hz(id, hz, DUTY_CYCLE);
	sys_timer.cntb = (src_clk_hz + HZ / 2) / HZ;
	mv_pwm_set_cnt_buf(id, PWM_TCNTB_CNT_BUF_WIDTH, DUTY_CYCLE);
	mv_pwm_start(id, 0);

	for (shift = 32; shift > 0; shift--) {
		temp = (u64)src_clk_hz << shift;
		do_div(temp, NSEC_PER_SEC);
		if ((temp >> 32) == 0)
			break;
	}

	ce->shift = shift;
	ce->mult = (u32)temp;
	ce->max_delta_ns = clockevent_delta2ns(0xFFFF, ce);
	ce->min_delta_ns = clockevent_delta2ns(0x4, ce);
	ce->cpumask = cpumask_of(0);
	
	dbg("[CLKEVT] virq %d hwirq %d\n", virq, hwirq);
	dbg("[CLKEVT] PWM source clock  %u\n", src_clk_hz);
	dbg("[CLKEVT] Count buffer value %u\n", sys_timer.cntb);
	dbg("[CLKEVT] shift %u mult %u\n", ce->shift, ce->mult);
    dbg("[CLKEVT] max_delta_ns %lu min_delta_ns %lu\n", ce->max_delta_ns,
														ce->min_delta_ns);
    dbg("[CLKEVT] cpumask %p\n", ce->cpumask);

	clockevents_config_and_register(ce,
									src_clk_hz,
									ce->min_delta_ns,
									ce->max_delta_ns);
}

#else

struct timer_device_id {
	const char *name;
} timer_ids[TIMER_CNT] = {
	{ .name = "pwm-timer0" },
	{ .name = "pwm-timer1" },
	{ .name = "pwm-timer2" },
	{ .name = "pwm-timer3" },
	{ .name = "pwm-timer4" },
	{ .name = "pwm-timer5" },
	{},
};

static unsigned long mv_timer_offset(void)
{
	u32 cntb = sys_timer.cntb;
	unsigned long tval, tdone;
	unsigned int id;

	id = sys_timer.clk_id;

	if (mv_pwm_get_int_status(id)) {
		tval = mv_pwm_get_cnt_obs(id);
		tdone = cntb - tval;
		if (tval != 0)
			tdone += cntb;
	} else {
		tval = mv_pwm_get_cnt_obs(id);
		tdone = cntb - tval;
	}

	tdone = tdone * (1000 / HZ) * USEC_PER_MSEC;
	do_div(tdone, cntb);

	return tdone;
}

static irqreturn_t mv_timer_irq(int irq, void *dev_id)
{
#if TIMER_MEASURE
	if (timer_measure_level)
		timer_measure_level = 0;
	else
		level = 1;
	mv_gpio_set_out_data(TIMER_MEASURE_GPIO, timer_measure_level);
#endif /* TIMER_MEASURE */

	mv_pwm_int_clear(sys_timer.clk_id);
	timer_tick();
	
	return IRQ_HANDLED;
}

static struct irqaction mv_timer_irqaction = {
	.name = "timer",
	.flags = (IRQF_DISABLED | IRQF_TIMER),
	.handler = mv_timer_irq,
};

static void mv_timer_hw_init(void)
{
	struct device_node *timer = NULL;
	struct irq_data *irq_dt = NULL;
	void __iomem *base = NULL;
	const char *path = NULL;
	unsigned int id;
	unsigned int virq = 0;
	unsigned int hwirq = 0;
	u32 usec;
	u32 ret;
	int err;

	id = sys_timer.clk_id;
	usec = sys_timer.usec;

	err = of_property_read_string(of_aliases, timer_ids[id].name, &path);
	if (WARN(err, "Failed to get timer property"))
		return;

	timer = of_find_node_by_path(path);
	if (WARN(!timer, "Failed to find timer path"))
		return;

	// irq = irq_of_parse_and_map(timer, 0);
	virq = irq_find_mapping(sys_timer.dm, sys_timer.clk_hwirq);
	if (virq == 0) {
		pr_warning("Try to map timer hwirq\n");
		virq = irq_create_mapping(sys_timer.dm, sys_timer.clk_hwirq);
		if (WARN(virq == 0, "Failed to map timer hwirq"))
			return;
	}

	irq_dt = irq_get_irq_data(sys_timer.clk_hwirq);
	hwirq = irq_dt->hwirq;

#if TIMER_MEASURE
	mv_gpio_enable(TIMER_MEASURE_GPIO);
	mv_gpio_set_dir(TIMER_MEASURE_GPIO, GPIO_DIR_OUTPUT);
	mv_gpio_set_out_data(TIMER_MEASURE_GPIO, timer_measure_level);
#endif /* TIMER_MEASURE */

	ret = mv_pwm_config_usec(id, usec, DUTY_CYCLE);
	sys_timer.cntb = mv_pwm_get_cnt_buf(id);
	mv_pwm_start(id, 0);

	setup_irq(hwirq, &mv_timer_irqaction);

	dbg("[TIMER] virq %d hwirq %d\n", virqm hwirq);
	dbg("[TIMER] PWM source clock %u\n", ret);
	dbg("[TIMER] Count buffer value %u\n", sys_timer.cntb);
}
#endif /*  */

void __init mfp7v2000tq_timer_init(void)
{
	struct device_node *intc = NULL;
	struct device_note *timer = NULL; 
	const char *path = NULL;
	int err;

	err = of_property_read_string(of_aliases,
								  "mv,interrupt-controller", &path);
	if (WARN(err, "Failed to get path!"))
		return;

	intc = of_find_node_by_path(path);
	if (WARN(!intc, "Failed to get node!"))
		return;

	sys_timer.dm = irq_find_host(intc);
	if (WARN(!sys_timer.dm, "Failed to get domain!"))
		return;

	err = of_property_read_string(of_aliases,
								  "mv,system-timer", &path);
	if (WARN(err, "Failed to find mv,mfp7v2000tq-systimer property!"))
		return;

	sys_timer.path = of_find_node_by_path(path);
	if (WARN(!sys_timer.path, "Failed to find system timer path!"))
		return;

	sys_timer.base = of_iomap(sys_timer.path, 0);
	if (WARN(!sys_timer.base, "Failed to get iomap base address!"))
		return;

#if defined(CONFIG_GENERIC_CLOCKEVENTS)
	sys_timer.clkevt_id = PWM_ID_0;
	sys_timer.clksrc_id = PWM_ID_1;

	timer = of_find_matching_node(sys_timer.path,
								  &timer_ids[sys_timer.clkevt_id]);
	if (timer != NULL) {
		if (of_property_read_u32(timer, "interrupts",
									    &sys_timer.clkevt_hwirq)) {
			pr_err("Failed to read clkevt hwirq\n");
			return;
		}
	}
	else
		WARN(!timer, "Failed to find clkevt node");

	timer = of_find_matching_node(sys_timer.path,
								  &timer_ids[sys_timer.clksrc_id]);
	if (timer != NULL) {
		if (of_property_read_u32(timer, "interrupts",
									   &sys_timer.clksrc_hwirq)) {
			pr_err("Failed to read clksrc hwirq\n");
			return;
		}
	}
	else
		WARN(!timer, "Failed to find clksrc node");

	mv_timer_clockevent_init();
	mv_timer_clocksource_init();
#else
	sys_timer.clk_id = PWM_ID_0;

	timer = of_find_matching_node(sys_timer.path,
								  &timer_ids[sys_timer.clk_id]);
	if (timer != NULL) {
		if (of_property_read_u32(timer, "interrupts",
									  &sys_timer.clkevt_hwirq)) {
			pr_err("Failed to read timer hwirq\n");
			return;
		}
	}
	else
		WARN(!timer, "Failed to find timer node");

	mv_timer_hw_init();
#endif /* CONFIG_GENERIC_CLOCKEVENTS */
	
	pr_info("[TIMER] IOMAP 0x%p\n", (void *)sys_timer.base); 
}

CLOCKSOURCE_OF_DECLARE(mfp7v2000tq_socfpga_timer, "mv,pwm-timer",
					   mfp7v2000tq_timer_init);

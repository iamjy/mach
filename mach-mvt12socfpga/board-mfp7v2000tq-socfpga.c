/*
 *  arch/arm/mach-mvt12socfpga/board-mfp7v2000tq-fpga.c
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
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/err.h>

#include <linux/mod_devicetable.h>
#include <linux/sys_soc.h>
#include <linux/irqchip.h>
#include <linux/clocksource.h>

#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>

#include <asm/mach/arch.h>

#include <mach/board.h>
#include <mach/sys.h>
#include <mach/ion/ion.h>

/*
 * Debug Level
 *  2 : Print all debug messages
 *  1 : Print only dbg() messages
 *  0 : No debug messages
 */
#define MVVICTORIA_BOARD_DEBUG_LEVEL  0
#if (MVVICTORIA_BOARD_DEBUG_LEVEL == 2)
#define dbg(format, arg...) \
	printk(KERN_ALERT "[T12] DBG: " format, ## arg)
#define enter()         \
	printk(KERN_ALERT "[T12] ENTER: %s\n", __func__)
#define leave()         \
	printk(KERN_ALERT "[T12] LEAVE: %s\n", __func__)
#elif (MVVICTORIA_BOARD_DEBUG_LEVEL == 1)
#define dbg(format, arg...) \
	printk(KERN_ALERT "[T12] DBG: " format, ## arg)
#define enter()
#define leave()
#else
#define dbg(format, arg...)
#define enter()
#define leave()
#endif

static struct platform_device *mem_pool_manager[] __initdata = {
	&mfp7v2000tq_ion,
};

void __iomem *syscon_base = NULL;

static void __init mv_clock_init(void)
{
	int ret = 0;

	pr_info("System clock=%uHz OSC(fixed)\n", mv_sys_get_sys_clk_hz());
	pr_info("ARM clock=%uHz OSC(fixed)\n", mv_sys_get_arm_clk_hz());
	pr_info("BUS clock=%uHz OSC(fixed)\n", mv_sys_get_bus_clk_hz());
	pr_info("APB clock=%uHz OSC(fixed)\n", mv_sys_get_apb_clk_hz());
	pr_info("MEM clock=%uHz OSC(fixed)\n", mv_sys_get_mem_clk_hz());

out:
	if (ret < 0)
		BUG();
}

static void __init mv_map_io(void)
{
	mv_map_common_io();
}

static void __init mv_init_machine(void)
{
	struct device_node *root   = NULL;
	struct device_node *syscon = NULL;
	struct soc_device           *soc_dev      = NULL;
	struct soc_device_attribute *soc_dev_attr = NULL;
	struct device *parent = NULL;
	unsigned long dt_root;
	const char *model     = NULL;
	const char *property  = NULL;
	const char *path = NULL;
	u32 product_id = 0x0;
	int err = 0;

	dt_root = of_get_flat_dt_root();

	model = of_get_flat_dt_prop(dt_root, "model", NULL);
	if (model != NULL)
		model = of_get_flat_dt_prop(dt_root, "compatible", NULL);
	WARN(!model, "Failed to get dt_root compatible");

	root = of_find_node_by_path("/");
	if (WARN(!root, "Failed to find root path"))
		return;

	err = of_property_read_string_index(root, "compatible", 0, &property);
	WARN(err, "Failed to read compatible property(index 0)");

	err = of_property_read_string_index(root, "compatible", 1, &property);
	WARN(err, "Failed to read compatible property(index 1");

	err = of_property_read_string(of_aliases, "mv,system-controller", &path);
	if (WARN(err, "Failed to get system-controller path"))
		return;

	syscon = of_find_node_by_path(path);
	if (WARN(!syscon, "Failed to find system-controller node"))
		return;

	err = of_property_read_string(syscon, "compatible", &property);
	WARN(err, "Failed to read compatible property");

	syscon_base = of_iomap(syscon, 0);
	if (WARN(!syscon_base, "Failed to map the syscon_base\n"))
		return;

	product_id = readl(syscon_base);

	soc_dev_attr = kzalloc(sizeof(*soc_dev_attr), GFP_KERNEL);
	if (WARN(!soc_dev_attr, "Failed to allocate memory for soc_dev_attr"))
		return;

	pr_info("soc_dev_attr %p\n", (void *)soc_dev_attr);

	soc_dev_attr->family   = "MVT12-SOCFPGA";
	soc_dev_attr->revision = 0x0;
	soc_dev_attr->soc_id   = kasprintf(GFP_KERNEL, "0x%x", product_id);
	soc_dev_attr->machine  = model;

	soc_dev = soc_device_register(soc_dev_attr);
	if (IS_ERR(soc_dev)) {
		kfree(soc_dev_attr->soc_id);
		kfree(soc_dev_attr);
		return;
	}

	parent = soc_device_to_device(soc_dev);
	WARN(!parent, "Failed to get soc_dev->dev's address");

	of_platform_populate(root, of_default_bus_match_table, NULL, parent);
	
	platform_add_devices(mem_pool_manager, ARRAY_SIZE(mem_pool_manager));

	mv_clock_init();
}

static const char *mfp7v2000tq_dt_board_compat[] = {
	"mtekvision,socfpga-mfp7v2000tq",
	NULL
};

DT_MACHINE_START(MFP7V2000TQ, "Mtekvision MFP-7V2000T-Q SOC FPGA")
	.map_io			= mv_map_io,
	.init_irq		= irqchip_init,
#ifdef CONFIG_MULTI_IRQ_HANDLER
	.handle_irq     = mv_handle_irq,
#endif
	.init_time		= clocksource_of_init,
	.init_machine	= mv_init_machine,
	.dt_compat		= mfp7v2000tq_dt_board_compat,
MACHINE_END

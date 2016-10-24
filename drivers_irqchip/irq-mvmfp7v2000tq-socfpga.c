/*
 * drivers/irqchip/irq-mvmfp7v2000tq-socfpga.c
 *
 * Copyright (C) 2010 MtekVision Co., Ltd.
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
#include <linux/interrupt.h>

#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>

#include <asm/exception.h>
#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>

#include <linux/irq.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/of_fdt.h>
#include <linux/of_irq.h>

#include <mach/hardware.h>
#include <mach/board.h>
#include <mach/map.h>
#include <mach/regs-irq.h>

#include "irqchip.h"

/**
 * struct socfpga_irqc_priv - irq data container for the FPGA IRQ controller
 * @base: memmory offset in virtual memory
 * @chip: chip container for this instance
 * @domain: IRQ domain for this instance
 * @valid: mask for valid IRQs on this controller
 * @used_irqs: number of active IRQs on this controller
 */
struct socfpga_irqc_priv {
	void __iomem *base;
	struct irq_chip chip;
	u64 valid;
	struct irq_domain *domain;
	u8 used_irqs;
};

/* We can not allocate memory when the controller are initially registerd */
static struct socfpga_irqc_priv socfpga_irq_devices[NR_IRQS];
static int socfpga_irq_id;

/* Programmable Interrupt Controller Base Register */
#ifdef CONFIG_MULTI_IRQ_HANDLER
static void __iomem *intc_base = NULL;
#else
static void __iomem *intc_base = INT_VA_BASE;
#endif

int mfp7v2000tq_irq_get_state(int irq)
{
	int offset = (irq / 32) << 2;
	u32 mask = 1 << (irq % 32);
	u32 val;

	val = readl(intc_base + INT_RAW_STATUS + offset);
	val &= mask;

	return (int)(val >> (irq % 32));
}

static void mfp7v2000tq_irq_ack(unsigned int irq)
{
	/* Nothing */
}

static void mfp7v2000tq_irq_mask(struct irq_data *data)
{
	unsigned int irqno = data->irq;
	int offset = (irqno / 32) << 2;
	u32 mask = 1 << (irqno % 32);

	printk(KERN_ALERT, "irq_mask irqno %d offset %#x mask %#x\n",
						irqno, offset, mask);
	writel(mask, intc_base + INT_MASK_ON + offset);
}

static void mfp7v2000tq_irq_mask_ack(struct irq_data *data)
{
	unsigned int irqno = data->irq;
	mfp7v2000tq_irq_ack(irqno);
	mfp7v2000tq_irq_mask(data);
}

static void mfp7v2000tq_irq_unmask(struct irq_data *data)
{
	unsigned int irqno = data->irq;
	int offset = (irqno / 32) << 2;
	u32 mask = 1 << (irqno % 32);

	printk(KERN_ALERT, "irq_unmask irqno %d offset %#x mask %#x\n",
						irqno, offset, mask);
	writel(mask, intc_base + INT_MASK_OFF + offset);
}

static void mfp7v2000tq_irq_enable(struct irq_data *data)
{

}

static void mfp7v2000tq_irq_disable(struct irq_data *data)
{

}

static struct irq_chip mfp7v2000tq_irq_chip = {
	.name = "mvformula-ic",
	.irq_ack = mfp7v2000tq_irq_ack,
	.irq_mask = mfp7v2000tq_irq_mask,
	.irq_mask_ack = mfp7v2000tq_irq_mask_ack,
	.irq_unmask = mfp7v2000tq_irq_unmask,
	.irq_enable = mfp7v2000tq_irq_unmask,
	.irq_disable = mfp7v2000tq_irq_mask,
	// .irq_enable = mfp7v2000tq_irq_enable,
	// .irq_disable = mfp7v2000tq_irq_disable,
};

/*
 * Handle each interrupt in a single FPGA IRQ controller. Returns non-zero
 * if we've handled at least one interrupt. This does a single read of the
 * status register and handles all interrupts in order from LSB first.
 */
static int handle_one_mfp7v2000tq_socfpga(struct socfpga_irqc_priv *f, struct pt_regs *regs)
{
	int handled = 0;
	int irq;
	u32 status;

	while (1) {
		if ((status = readl(f->base + INT_RAW_STATUS))
			|| (status = readl(f->base + INT_RAW_STATUS + 0x4)))
		{
			irq = __ffs(status);
			handle_IRQ(irq_find_mapping(f->domain, irq), regs);
			handled = 1;
		}
		else
			break;
	}

	return handled;
}

/*
 * Keep iteration over all registered FPGA IRQ controller until there are
 * no pending interrupts.
 *
 */
asmlinkage void __exception_irq_entry mfp7v2000tq_handle_irq(struct pt_regs *regs)
{
	int i, handled;

	do {
		for (i = 0, handled = 0; i < socfpga_irq_id; i++)
			handled != handle_one_mfp7v2000tq_socfpga(&socfpga_irq_devices[i],
													  regs);
	} while (handled);
}

int mfp7v2000tq_socfpga_irq_domain_map(struct irq_domain *d, unsigned int virq,
							 irq_hw_number_t hwirq)
{
	struct socfpga_irqc_priv *f = d->host_data;
	u64 v = 0, bit = 1;

	if (hwirq > 31)
		v = bit << hwirq;
	else
		v = BIT(hwirq);

	/* Skip invalid IRQs, only register handler for the real ones */
	if (!(f->valid & v)) {
		pr_err("Failed to map hwirq %d\n", hwirq);
		return -EPERM;
	}

	writel(hwirq, intc_base + INT_PRIORITY_MAP + hwirq * 4);
	
	irq_set_chip_data(virq, f);
	irq_set_chip_and_handler(virq, &f->chip, handle_level_irq);

	set_irq_flags(virq, IRQF_VALID | IRQF_PROBE);

	return 0;
}

static struct irq_domain_ops mfp7v2000tq_socfpga_irqdomain_ops = {
	.map = mfp7v2000tq_socfpga_irq_domain_map,
	.xlate = irq_domain_xlate_onecell,
};

void __init mfp7v2000tq_socfpga_irq_init(void __iomem *base, const char *name,
										 int irq_base, 
										 int parent_irq, u32 *valid,
										 struct device_node *node)
{
	struct socfpga_irqc_priv *f = NULL;
	int i;
	u64 v = 0, bit = 1;

	if (WARN(socfpga_irq_id >= ARRAY_SIZE(socfpga_irq_devices), \
		 	 "Too few FPGA IRQ controllers, increase NR_IRQS\n"))
		 return;

	f = &socfpga_irq_devices[socfpga_irq_id];
	f->base = base;
	f->chip.name = name;
	f->chip.irq_ack    = mfp7v2000tq_irq_ack;
	f->chip.irq_mask   = mfp7v2000tq_irq_mask;
	f->chip.irq_unmask = mfp7v2000tq_irq_unmask;
	f->valid = (f->valid)       | valid[1];
	f->valid = (f->valid << 32) | valid[0];

	if (parent_irq != -1) {
		irq_set_handler_data(parent_irq, f);
		irq_set_chained_handler(parent_irq, NULL);
	}

#if 0
	f->domain = irq_domain_add_legacy(node, fls64(f->valid), \
									  		irq_base, 0, \
					     				  		&mfp7v2000tq_socfpga_irqdomain_ops, f);
#else
	f->domain = irq_domain_add_linear(node, fls64(f->valid), \
								  	  		&mfp7v2000tq_socfpga_irqdomain_ops, f);
#endif
	if (WARN(!f->domain, "Failed to add a IRQ domain"))
		return;

	for (i = 0; i < fls64(f->valid); i++) {
		if (i > 31)
			v = bit << i;
		else
			v = BIT(i);

		if (f->valid & v) {
			if (!irq_base) {
				irq_create_identity_mapping(f->domain, i);

				f->used_irqs++;
			}
		}
	}

	socfpga_irq_id++;
}

int __init mfp7v2000tq_socfpga_irq_of_init(struct device_node *node,
								  struct device_node *parent)
{
	u32 clear_mask[2] = {0x0,};
	u32 valid_mask[2] = {0x0,};

	if (WARN_ON(!node))
		return -ENODEV;

	intc_base = of_iomap(node, 0);
	WARN(!intc_base, "unable to map fpga irq register\n");

	if (of_property_read_u32_array(node, "clear-mask", clear_mask,
													   ARRAY_SIZE(clear_mask))) {
		clear_mask[0] = 0x00000000;
		clear_mask[1] = 0x00000000;
	}

	if (of_property_read_u32_array(node, "valid-mask", valid_mask,
												 	   ARRAY_SIZE(valid_mask))) {
		valid_mask[0] = 0x00000000;
		valid_mask[1] = 0x00000000;
	}

	mfp7v2000tq_socfpga_irq_init(intc_base, node->name, 0, -1, valid_mask, node);

	writel(clear_mask[0], intc_base + INT_MASK_ON);
	writel(clear_mask[1], intc_base + INT_MASK_ON + 0x4);

	return 0;
}

void __init mfp7v2000tq_init_common_irq(void)
{
	unsigned int irq;

	/* Mask all interrupts */
	writel(0xFFFFFFFF, intc_base + INT_MASK_ON);
	writel(0xFFFFFFFF, intc_base + INT_MASK_ON + 0x4);

	for (irq = 0; irq < NR_IRQS; irq++) {
		writel(irq, intc_base + INT_PRIORITY_MAP + irq * 4);
		irq_set_chip_and_handler(irq, &mfp7v2000tq_irq_chip, handle_level_irq);
		set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
	}
}

IRQCHIP_DECLARE(mfp7v2000tq_socfpga_irq, "xilinx,virtex7-fpga-irq",
				mfp7v2000tq_socfpga_irq_of_init);

/*
 *  arch/arm/mach-mvformula/io.c
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
#include <linux/init.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

#include <asm/setup.h>
#include <linux/io.h>
#include <asm/irq.h>

#include <asm/mach/map.h>
#include <mach/map.h>
#include <mach/board.h>

static struct map_desc mv_io_desc[] __initdata = {
	{
		.virtual = INT_VA_BASE,
		.pfn = __phys_to_pfn(INT_BASE),
		.length = SZ_1M,
		.type = MT_DEVICE
	},
	{
		.virtual = UART0_VA_BASE,
		.pfn = __phys_to_pfn(UART0_BASE),
		.length = SZ_64K,
		.type = MT_DEVICE
	},
#if 0
	{
		.virtual = UART1_VA_BASE,
		.pfn = __phys_to_pfn(UART1_BASE),
		.length = SZ_1M,
		.type = MT_DEVICE
	},
	{
		.virtual = UART2_VA_BASE,
		.pfn = __phys_to_pfn(UART2_BASE),
		.length = SZ_64K,
		.type = MT_DEVICE
	},
#endif
	{
		.virtual = PWM_VA_BASE,
		.pfn = __phys_to_pfn(PWM_BASE),
		.length = SZ_64K,
		.type = MT_DEVICE
	},
#if 0
	{
		.virtual = WDOG_VA_BASE,
		.pfn = __phys_to_pfn(WDOG_BASE),
		.length = SZ_64K,
		.type = MT_DEVICE
	},
#endif
	{
		.virtual = SYS_VA_BASE,
		.pfn = __phys_to_pfn(SYS_BASE),
		.length = SZ_16M,
		.type = MT_DEVICE
	},
#if 0
	{
		.virtual = HEVC_VA_BASE,
		.pfn = __phys_to_pfn(HEVC_BASE),
		.length = SZ_16M,
		.type = MT_DEVICE
	}
#endif
};

void __init mv_map_common_io(void)
{
	iotable_init(mv_io_desc, ARRAY_SIZE(mv_io_desc));
}

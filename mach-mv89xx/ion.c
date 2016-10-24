/*
 *  arch/arm/mach-mv89xx/ion.c
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
#include <linux/sizes.h>
#include <linux/bug.h>

#include <linux/platform_device.h>
#include <mach/ion/ion.h>

struct ion_platform_heap mv_victoria_ion_heaps[] = {
		{
			.id	= ION_HEAP_TYPE_CARVEOUT,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= "carveout",
			.base   = 0x05000000,
			.size	= SZ_16M,
		},
};

struct ion_platform_data mv_victoria_ion_pdata = {
	.nr = ARRAY_SIZE(mv_victoria_ion_heaps),
	.heaps = mv_victoria_ion_heaps,
};

struct platform_device mv_victoria_ion = {
	.name = "mv-victoria-ion",
	.dev = {
		.platform_data = &mv_victoria_ion_pdata,
	},
	.id = -1,
};

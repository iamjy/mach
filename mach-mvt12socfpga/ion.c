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

struct ion_platform_heap mfp7v2000tq_ion_heaps[] = {
#if 0
		{
			.id	= ION_HEAP_TYPE_CARVEOUT,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= "carveout",
			.base   = 0x10000000,
			.size	= SZ_256M,
		},
#else
		{
			.id	= ION_HEAP_TYPE_CARVEOUT,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= "carveout",
			.base   = 0x20300000,
			.size	= SZ_64M,
		},
#endif
};

struct ion_platform_data mfp7v2000tq_ion_pdata = {
	.nr = ARRAY_SIZE(mfp7v2000tq_ion_heaps),
	.heaps = mfp7v2000tq_ion_heaps,
};

struct platform_device mfp7v2000tq_ion = {
	.name = "mvmfp7v2000tq-ion",
	.dev = {
		.platform_data = &mfp7v2000tq_ion_pdata,
	},
	.id = -1,
};

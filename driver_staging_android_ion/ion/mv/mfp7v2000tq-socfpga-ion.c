/*
 *  drivers/staging/android/ion/mv/mfp7v2000tq-socfpga-ion.c
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

#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/err.h>
#include "../ion.h"
#include "../ion_priv.h"

struct ion_device *idev;
static struct ion_heap **heaps;
static int num_heaps;

static void *carveout_ptr;

int mfp7v2000tq_ion_probe(struct platform_device *pdev)
{
	struct ion_platform_data *pdata = pdev->dev.platform_data;
	int err;
	int i;

	num_heaps = pdata->nr;

	heaps = kzalloc(sizeof(struct ion_heap *) * pdata->nr, GFP_KERNEL);
	if (!heaps)
		return -ENOMEM;

	idev = ion_device_create(NULL);
	if (IS_ERR_OR_NULL(idev)) {
		dev_err(&pdev->dev, "Failed to create ION device!\n");
		kfree(heaps);
		return PTR_ERR(idev);
	}

	/* create the heaps as specified in the board file */
	for (i = 0; i < num_heaps; i++) {
		struct ion_platform_heap *heap_data = &pdata->heaps[i];

		if (heap_data->type == ION_HEAP_TYPE_CARVEOUT && !heap_data->base) {
			carveout_ptr = alloc_pages_exact(heap_data->size, GFP_KERNEL);
			if (carveout_ptr)
				heap_data->base = virt_to_phys(carveout_ptr);
			else
				dev_err(&pdev->dev, "Could not allocate carveout heap!");
		}

		dev_info(&pdev->dev, "Carveout heap:  vir %p phy %p size %d",
				 (!heap_data->base) ? carveout_ptr : phys_to_virt(carveout_ptr),
				 (void *)heap_data->base,
				 heap_data->size);

		heaps[i] = ion_heap_create(heap_data);
		if (IS_ERR_OR_NULL(heaps[i])) {
			dev_err(&pdev->dev, "Failed to create ION heap memory!\n");
			err = PTR_ERR(heaps[i]);
			goto err;
		}

		ion_device_add_heap(idev, heaps[i]);
	}

	platform_set_drvdata(pdev, idev);
	return 0;

err:
	for (i = 0; i < num_heaps; i++) {
		if (heaps[i])
			ion_heap_destroy(heaps[i]);
	}

	kfree(heaps);
	return err;
}

static int mfp7v2000tq_ion_remove(struct platform_device *pdev)
{
	struct ion_platform_data *pdata = pdev->dev.platform_data;
	struct ion_device *idev = platform_get_drvdata(pdev);
	int i;

	ion_device_destroy(idev);

	for (i = 0; i < num_heaps; i++)
		ion_heap_destroy(heaps[i]);

	kfree(heaps);

	if (carveout_ptr) {
		free_pages_exact(carveout_ptr, pdata->heaps->size);
		carveout_ptr = NULL;
	}

	return 0;
}

static struct platform_driver mfp7v2000tq_ion_driver = {
	.probe = mfp7v2000tq_ion_probe,
	.remove = mfp7v2000tq_ion_remove,
	.driver = {
		.name = "mvmfp7v2000tq-ion",
		.owner = THIS_MODULE,
	},
};

module_platform_driver(mfp7v2000tq_ion_driver);


/*
 *  drivers/staging/android/ion/mv/mfp7v2000tq-socfpga-device.c
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

#define pr_fmt(fmt) "mv-ion: " fmt

#include <linux/dma-buf.h>
#include <linux/dma-direction.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/list.h>
#include <linux/mutex.h>

#include "../ion.h"
#include "../../uapi/mv_ion_drv.h"

#define u64_to_uptr(x) ((void __user *)(unsigned long)(x))

#define ION_SUCCESS  0
#define ION_FAILE   -1

struct mv_ion;
struct mv_ion_device;
struct mv_ion_data;
struct mv_ion_client;

struct mv_ion_device {
	struct miscdevice misc;
};

struct mv_ion_data {
	struct dma_buf *dma_buf;
	struct device *dev;
};

struct mv_ion_client {
	int fd;

	unsigned long vaddr;
	unsigned long paddr;
	size_t        len;

	struct ion_client *client;
	struct ion_handle *handle;

	struct list_head head;
	struct list_head list;
};

struct mv_ion {
	struct mv_ion_device *ion_dev;
	struct mv_ion_data    ion_data;
	struct mv_ion_client *ion_client;

	struct mutex list_lock;
};

extern struct ion_device *idev;
static struct mv_ion *g_mvion = NULL;

static unsigned long g_vaddr[10] = {0,}; // For test
static unsigned long g_paddr[10] = {0,}; // For test
static size_t        g_bsize[10] = {0,}; // For test
static unsigned int  g_bcnt   	 = 0;	 // For test

size_t
mv_ion_get_size_from_client(struct platform_device *pdev, int shared_fd)
{
	unsigned long size;
	struct mv_ion *mvion = NULL;
	struct mv_ion_client *s = NULL;

	mvion = pdev ? (struct mv_ion *)platform_get_drvdata(pdev) : g_mvion;

	mutex_lock(&mvion->list_lock);
	if (!list_empty(&mvion->ion_client->head)) {
		list_for_each_entry(s, &mvion->ion_client->head, list) {
			if (s->fd == shared_fd) {
				size = s->len;
				pr_info("[ION] FD %d size 0x%x\n", s->fd, size);
				mutex_unlock(&mvion->list_lock);

				return size;
			}
		}
	}
	else {
		mutex_unlock(&mvion->list_lock);
		return 0;
	}
}

unsigned long
mv_ion_get_vaddr_from_client(struct platform_device *pdev, int shared_fd)
{
	unsigned long vaddr;
	struct mv_ion *mvion = NULL;
	struct mv_ion_client *v = NULL;
	
	mvion = pdev ? (struct mv_ion *)platform_get_drvdata(pdev) : g_mvion;

	mutex_lock(&mvion->list_lock);
	if (!list_empty(&mvion->ion_client->head)) {
		list_for_each_entry(v, &mvion->ion_client->head, list) {
			if (v->fd == shared_fd) {
				vaddr = v->vaddr;
				pr_info("[ION] FD %d vaddr 0x%x\n", v->fd, vaddr);
				mutex_unlock(&mvion->list_lock);
				
				return vaddr;
			}
		}
	}
	else {
		mutex_unlock(&mvion->list_lock);
		return 0;
	}
}

phys_addr_t
mv_ion_get_paddr_from_client(struct platform_device *pdev, int shared_fd)
{
	unsigned long paddr;
	struct mv_ion *mvion = NULL;
	struct mv_ion_client *p = NULL;
	
	mvion = pdev ? (struct mv_ion *)platform_get_drvdata(pdev) : g_mvion;

	mutex_lock(&mvion->list_lock);
	if (!list_empty(&mvion->ion_client->head)) {
		list_for_each_entry(p, &mvion->ion_client->head, list) {
			if (p->fd == shared_fd) {
				paddr = p->paddr;
				pr_info("[ION] FD %d paddr 0x%x\n", p->fd, paddr);
				mutex_unlock(&mvion->list_lock);
				
				return paddr;
			}
		}
	}
	else {
		mutex_unlock(&mvion->list_lock);
		return 0;
	}
}

int
mv_ion_client_register(struct platform_device *pdev, int shared_fd)
{
	int ret;
	ion_phys_addr_t  phy_addr;
	unsigned long   *vir_addr;
	size_t len;
	struct mv_ion *mvion = NULL;
	struct mv_ion_client *new = NULL;

	mvion = pdev ? (struct mv_ion *)platform_get_drvdata(pdev) : g_mvion;

	new = kzalloc(sizeof(*new), GFP_KERNEL);
	if (new == NULL)
		return ION_FAILE;
	
	/* Create a new client corresponding to idev */
	new->client = ion_client_create(idev, "mv-ion");
	if (new->client == NULL) {
		pr_err("Faield to create ion client! %d\n", shared_fd);
		return ION_FAILE;
	}

	/* Get the buffer handler */
	new->handle = ion_import_dma_buf(new->client, shared_fd);
	if (new->handle == NULL) {
		pr_err("Failed to get ion buffer! %d\n", shared_fd);
		goto error;
	}

	/* Get the physical address and length */
	ret = ion_phys(new->client, new->handle, &phy_addr, &len);
	if (ret || !phy_addr) {
		pr_err("Failed to get phy_addr from ion client!");
		goto error;
	}

	/* Get the virture address */
	vir_addr = ion_map_kernel(new->client, new->handle);
	if (vir_addr == NULL) {
		pr_err("Failed to get vir_addr from ion client!");
		goto error;
	}

	new->fd     = shared_fd;
	new->paddr  = phy_addr;
	new->vaddr  = (unsigned long)vir_addr;
	new->len    = len;

	mutex_lock(&mvion->list_lock);
	list_add(&new->list, &mvion->ion_client->head);
	mutex_unlock(&mvion->list_lock);

	pr_info("New client (%d %p %p)\n", new->fd, new->client, new->handle);

	return ION_SUCCESS;

error:
	if (new->client && new->handle) {
		ion_free(new->client, new->handle);
		return ION_FAILE;
	}
	if (new->client)
		ion_client_destroy(new->client);

	return ION_FAILE;
}

int
mv_ion_client_unregister(struct platform_device *pdev, int shared_fd)
{
	struct mv_ion *mvion = NULL;
	struct mv_ion_client *c = NULL;

	mvion = pdev ? (struct mv_ion *)platform_get_drvdata(pdev) : g_mvion;

	mutex_lock(&mvion->list_lock);
	if (!list_empty(&mvion->ion_client->head)) {
		list_for_each_entry(c, &mvion->ion_client->head, list) {
			if (c->fd == shared_fd) {
				pr_info("Del client %d %p %p\n", c->fd, c->client,
														c->handle);
				ion_free(c->client, c->handle);
				list_del(&c->list);
				kfree(c);
				mutex_unlock(&mvion->list_lock);

				return ION_SUCCESS;
			}
		}
	}
	else {
		mutex_unlock(&mvion->list_lock);
		pr_err("There is no client registered\n");
		return ION_FAILE;
	}
}

static ssize_t mv_ion_read(struct file *f, char __user *buf,
										   size_t len, loff_t *off)
{
	int i;

	for (i = 0; i < g_bcnt; i++) {
		printk(KERN_INFO "[READ] Kernel vaddr %p\n", g_vaddr[i]);
		printk(KERN_INFO "[READ] Kernel paddr %p\n", g_paddr[i]);
		printk(KERN_INFO "[READ]  %p\n", g_bsize[i]);

		if (g_bsize[i] < len)
			len = g_bsize[i];

		if (g_vaddr[i] != NULL) {
			if (copy_to_user((char *)(g_vaddr[i]), buf, len) < 0) {
				pr_err("Failed to copy kernel data!\n");
					return -EFAULT;
			}
		}
	 	else {
			pr_err("Wrong kernel vaddr!\n");
			return -EFAULT;
		}
	}

	return len;
}

static ssize_t mv_ion_write(struct file *f, const char __user *buf,
										    size_t len, loff_t *off)
{
	int i;

	pr_info("[WRITE] User buf : %s\n", buf);

	for (i = 0; i < g_bcnt; i++) {
		printk(KERN_INFO "[WRITE] Kernel vaddr %p\n", g_vaddr[i]);
		printk(KERN_INFO "[WRITE] Kernel paddr %p\n", g_paddr[i]);
		printk(KERN_INFO "[WRITE]  %p\n", g_bsize[i]);

		if (g_bsize[i] < len)
			len = g_bsize[i];

		if (g_vaddr[i] != NULL) {
			if (copy_from_user((char *)(g_vaddr[i]), buf, len) < 0) {
				pr_err("Failed to copy kernel data!\n");
					return -EFAULT;
			}
		}
	 	else {
			pr_err("Wrong kernel vaddr!\n");
			return -EFAULT;
		}
	}

	return len;
}

static int ion_handle_test_dma(struct device *dev, struct dma_buf *dma_buf,
		void __user *ptr, size_t offset, size_t size, bool write)
{
	int ret = 0;
	struct dma_buf_attachment *attach;
	struct sg_table *table;
	pgprot_t pgprot = pgprot_writecombine(PAGE_KERNEL);
	enum dma_data_direction dir = write ? DMA_FROM_DEVICE : DMA_TO_DEVICE;
	struct sg_page_iter sg_iter;
	unsigned long offset_page;

	attach = dma_buf_attach(dma_buf, dev);
	if (IS_ERR(attach))
		return PTR_ERR(attach);

	table = dma_buf_map_attachment(attach, dir);
	if (IS_ERR(table))
		return PTR_ERR(table);

	offset_page = offset >> PAGE_SHIFT;
	offset %= PAGE_SIZE;

	for_each_sg_page(table->sgl, &sg_iter, table->nents, offset_page) {
		struct page *page = sg_page_iter_page(&sg_iter);
		void *vaddr = vmap(&page, 1, VM_MAP, pgprot);
		size_t to_copy = PAGE_SIZE - offset;

		to_copy = min(to_copy, size);
		if (!vaddr) {
			ret = -ENOMEM;
			goto err;
		}

		if (write)
			ret = copy_from_user(vaddr + offset, ptr, to_copy);
		else
			ret = copy_to_user(ptr, vaddr + offset, to_copy);

		vunmap(vaddr);
		if (ret) {
			ret = -EFAULT;
			goto err;
		}
		size -= to_copy;
		if (!size)
			break;
		ptr += to_copy;
		offset = 0;
	}

err:
	dma_buf_unmap_attachment(attach, table, dir);
	dma_buf_detach(dma_buf, attach);
	return ret;
}

static int ion_handle_test_kernel(struct dma_buf *dma_buf, void __user *ptr,
		size_t offset, size_t size, bool write)
{
	int ret;
	unsigned long page_offset = offset >> PAGE_SHIFT;
	size_t copy_offset = offset % PAGE_SIZE;
	size_t copy_size = size;
	enum dma_data_direction dir = write ? DMA_FROM_DEVICE : DMA_TO_DEVICE;

	if (offset > dma_buf->size || size > dma_buf->size - offset)
		return -EINVAL;

	ret = dma_buf_begin_cpu_access(dma_buf, offset, size, dir);
	if (ret)
		return ret;

	while (copy_size > 0) {
		size_t to_copy;
		void *vaddr = dma_buf_kmap(dma_buf, page_offset);

		if (!vaddr)
			goto err;

		to_copy = min_t(size_t, PAGE_SIZE - copy_offset, copy_size);

		if (write)
			ret = copy_from_user(vaddr + copy_offset, ptr, to_copy);
		else
			ret = copy_to_user(ptr, vaddr + copy_offset, to_copy);

		dma_buf_kunmap(dma_buf, page_offset, vaddr);
		if (ret) {
			ret = -EFAULT;
			goto err;
		}

		copy_size -= to_copy;
		ptr += to_copy;
		page_offset++;
		copy_offset = 0;
	}
err:
	dma_buf_end_cpu_access(dma_buf, offset, size, dir);
	return ret;
}

static long mv_ion_ioctl(struct file *filp, unsigned int cmd,
						unsigned long arg)
{
	struct mv_ion_data *test_data = filp->private_data;
	int ret = 0;

	union {
		struct ion_test_rw_data test_rw;
	} data;

	if (_IOC_SIZE(cmd) > sizeof(data))
		return -EINVAL;

	if (_IOC_DIR(cmd) & _IOC_WRITE)
		if (copy_from_user(&data, (void __user *)arg, _IOC_SIZE(cmd)))
			return -EFAULT;

	switch (cmd) {
	case MV_ION_IOC_SET_FD:
	{
		struct dma_buf *dma_buf = NULL;
		int fd = arg;

		if (fd >= 0) {
			dma_buf = dma_buf_get((int)arg);
			if (IS_ERR(dma_buf))
				return PTR_ERR(dma_buf);
		}
		if (test_data->dma_buf)
			dma_buf_put(test_data->dma_buf);
		test_data->dma_buf = dma_buf;
		break;
	}
	case MV_ION_IOC_DMA_MAPPING:
	{
		ret = ion_handle_test_dma(test_data->dev, test_data->dma_buf,
					u64_to_uptr(data.test_rw.ptr),
					data.test_rw.offset, data.test_rw.size,
					data.test_rw.write);
		break;
	}
	case MV_ION_IOC_KERNEL_MAPPING:
	{
		ret = ion_handle_test_kernel(test_data->dma_buf,
					u64_to_uptr(data.test_rw.ptr),
					data.test_rw.offset, data.test_rw.size,
					data.test_rw.write);
		break;
	}
	case MV_ION_IOC_REGISTER:
	{
		int shared_fd;

		if (copy_from_user(&shared_fd, (int *)arg, sizeof(int))) {
			pr_err("Failed to get ION FD data!\n");
			return -EFAULT;
		}

		ret = mv_ion_client_register(NULL, shared_fd);
		if (ret < 0) {
			pr_err("Failed to register %d client!", shared_fd);
			return ret;
		}

		/* ------------------------------------------------------------------ */
		/* For test                                                           */
		g_vaddr[g_bcnt] = mv_ion_get_vaddr_from_client(NULL, shared_fd);
		if (!g_vaddr)
			pr_err("Failed to get vaddr from %d client!", shared_fd);

		g_paddr[g_bcnt] = mv_ion_get_paddr_from_client(NULL, shared_fd);
		if (!g_paddr)
			pr_err("Failed to get paddr from %d client!", shared_fd);

		g_bsize[g_bcnt] = mv_ion_get_size_from_client(NULL, shared_fd);
		if (g_bsize <= 0)
			pr_err("Failed to get size form %d client!", shared_fd);

		g_bcnt++;
		/* ------------------------------------------------------------------ */

		break;
	}
	case MV_ION_IOC_UNREGISTER:
	{
		int shared_fd;

		if (copy_from_user(&shared_fd, (int *)arg, sizeof(int))) {
			pr_err("Failed to get ION FD data!\n");
			return -EFAULT;
		}

		ret = mv_ion_client_unregister(NULL, shared_fd);
		if (ret < 0) {
			pr_err("Failed to unregister %d client!", shared_fd);
			return ret;
		}

		/* ------------------------------------------------------------------ */
		/* For test                                                           */
		g_bcnt--;
		/* ------------------------------------------------------------------ */
		
		break;
	}
	default:
		return -ENOTTY;
	}

	if (_IOC_DIR(cmd) & _IOC_READ) {
		if (copy_to_user((void __user *)arg, &data, sizeof(data)))
			return -EFAULT;
	}
	return ret;
}

static int mv_ion_open(struct inode *inode, struct file *file)
{
	struct mv_ion_data *data;
	struct miscdevice *miscdev = file->private_data;

	data = kzalloc(sizeof(struct mv_ion_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->dev = miscdev->parent;
	file->private_data = data;

	return 0;
}

static int mv_ion_release(struct inode *inode, struct file *file)
{
	struct mv_ion_data *data = file->private_data;

	kfree(data);

	return 0;
}

static const struct file_operations mv_ion_fops = {
	.owner 			= THIS_MODULE,
	.unlocked_ioctl = mv_ion_ioctl,
	.compat_ioctl 	= mv_ion_ioctl,
	.open 			= mv_ion_open,
	.release 		= mv_ion_release,
	.read 			= mv_ion_read,
	.write 			= mv_ion_write
};

static int __init mv_ion_probe(struct platform_device *pdev)
{
	int ret;
	struct mv_ion *mvion;
	struct mv_ion_device *idev;
	struct mv_ion_client *iclient;

	mvion = kzalloc(sizeof(struct mv_ion), GFP_KERNEL);
	if (WARN(!mvion, "Failed to allocate ion"))
		return -ENOMEM;

	mvion->ion_dev = devm_kzalloc(&pdev->dev, sizeof(struct mv_ion_device),
											  GFP_KERNEL);
	if (WARN(!mvion->ion_dev, "Failed to allocate ion_device"))
		return -ENOMEM;

	mvion->ion_client = kzalloc(sizeof(struct mv_ion_client), GFP_KERNEL); 
	if (WARN(!mvion->ion_client, "Failed to allocate ion_client"))
		return -ENOMEM;

	idev    = mvion->ion_dev;
	iclient = mvion->ion_client;

	idev->misc.minor = MISC_DYNAMIC_MINOR;
	idev->misc.name = "mv-ion";
	idev->misc.fops = &mv_ion_fops;
	idev->misc.parent = &pdev->dev;
	ret = misc_register(&idev->misc);
	if (WARN(ret, "Failed to register mv-ion misc device.\n"))
		return ret;

	INIT_LIST_HEAD(&iclient->head);
	mutex_init(&mvion->list_lock);

	platform_set_drvdata(pdev, (void *)mvion);
	g_mvion = mvion;

	return 0;
}

static struct platform_driver mv_ion_platform_driver = {
	.driver = {
		.name = "mv-ion",
	},
};

static int __init mv_ion_init(void)
{
	platform_device_register_simple("mv-ion", -1, NULL, 0);
	return platform_driver_probe(&mv_ion_platform_driver, mv_ion_probe);
}

static void __exit mv_ion_exit(void)
{
	platform_driver_unregister(&mv_ion_platform_driver);
}

module_init(mv_ion_init);
module_exit(mv_ion_exit);

/*
 *  drivers/staging/android/ion/mv/mv_ion_drv.h
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

/*
 *
 */
size_t
mv_ion_get_size_from_client(struct platform_device *pdev, int shared_fd);

/*
 *
 */
unsigned long
mv_ion_get_vaddr_from_client(struct platform_device *pdev, int shared_fd);

/*
 *
 */
unsigned long
mv_ion_get_paddr_from_client(struct platform_device *pdev, int shared_fd);

/*
 *
 */
int
mv_ion_client_register(struct platform_device *pdev, int shared_fd);

/*
 *
 */
int
mv_ion_client_unregister(struct platform_device *pdev, int shared_fd);

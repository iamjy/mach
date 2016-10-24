/*
 *  arch/arm/mach-mvt12socfpga/include/mach/timex.h
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
#ifndef __ASM_ARCH_TIMEX_H
#define __ASM_ARCH_TIMEX_H

#if defined(CONFIG_GENERIC_TIME)
/* Set clock tick rate to 3300000 Hz */
#define CLOCK_TICK_RATE		(148000000 / 45 / 1)
#else
/* Set clock tick rate to 5920000 Hz */
#define CLOCK_TICK_RATE		(148000000 / 25 / 1)
#endif

extern void mv_timer_init(void);

#endif /* __ASM_ARCH_TIMEX_H */


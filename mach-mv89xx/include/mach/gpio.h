/*
 *  arch/arm/mach-mvzenith/include/mach/gpio.h
 *
 *  Copyright (C) 2010 MtekVision Co., Ltd.
 *	Jinyoung Park <parkjy@mtekvision.com>
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
#ifndef __ASM_ARCH_GPIO_H
#define __ASM_ARCH_GPIO_H

#include <linux/interrupt.h>

/* GPIOs */
#define GPIO0		0
#define GPIO1		1
#define GPIO2		2
#define GPIO3		3
#define GPIO4		4
#define GPIO5		5
#define GPIO6		6
#define GPIO7		7
#define GPIO8		8
#define GPIO9		9
#define GPIO10		10
#define GPIO11		11
#define GPIO12		12
#define GPIO13		13
#define GPIO14		14
#define GPIO15		15
#define GPIO16		16
#define GPIO17		17
#define GPIO18		18
#define GPIO19		19
#define GPIO20		20
#define GPIO21		21
#define GPIO22		22
#define GPIO23		23
#define GPIO24		24
#define GPIO25		25
#define GPIO26		26
#define GPIO27		27
#define GPIO28		28
#define GPIO29		29
#define GPIO30		30
#define GPIO31		31

#define GPIO32		32
#define GPIO33		33
#define GPIO34		34
#define GPIO35		35
#define GPIO36		36
#define GPIO37		37
#define GPIO38		38
#define GPIO39		39
#define GPIO40		40
#define GPIO41		41
#define GPIO42		42
#define GPIO43		43
#define GPIO44		44
#define GPIO45		45
#define GPIO46		46
#define GPIO47		47
#define GPIO48		48
#define GPIO49		49
#define GPIO50		50
#define GPIO51		51
#define GPIO52		52
#define GPIO53		53
#define GPIO54		54
#define GPIO55		55
#define GPIO56		56
#define GPIO57		57
#define GPIO58		58
#define GPIO59		59
#define GPIO60		60
#define GPIO61		61
#define GPIO62		62
#define GPIO63		63

#define GPIO64		64
#define GPIO65		65
#define GPIO66		66
#define GPIO67		67
#define GPIO68		68
#define GPIO69		69
#define GPIO70		70
#define GPIO71		71
#define GPIO72		72
#define GPIO73		73
#define GPIO74		74
#define GPIO75		75
#define GPIO76		76
#define GPIO77		77
#define GPIO78		78
#define GPIO79		79
#define GPIO80		80
#define GPIO81		81
#define GPIO82		82
#define GPIO83		83
#define GPIO84		84
#define GPIO85		85
#define GPIO86		86
#define GPIO87		87
#define GPIO88		88
#define GPIO89		89
#define GPIO90		90
#define GPIO91		91
#define GPIO92		92
#define GPIO93		93
#define GPIO94		94
#define GPIO95		95

#define GPIO96		96
#define GPIO97		97
#define GPIO98		98
#define GPIO99		99
#define GPIO100		100
#define GPIO101		101
#define GPIO102		102
#define GPIO103		103
#define GPIO104		104
#define GPIO105		105
#define GPIO106		106
#define GPIO107		107
#define GPIO108		108
#define GPIO109		109
#define GPIO110		110
#define GPIO111		111
#define GPIO112		112
#define GPIO113		113
#define GPIO114		114
#define GPIO115		115
#define GPIO116		116
#define GPIO117		117
#define GPIO118		118
#define GPIO119		119
#define GPIO120		120
#define GPIO121		121
#define GPIO122		122
#define GPIO123		123
#define GPIO124		124
#define GPIO125		125
#define GPIO126		126
#define GPIO127		127

#define GPIO128		128
#define GPIO129		129
#define GPIO130		130
#define GPIO131		131
#define GPIO132		132
#define GPIO133		133
#define GPIO134		134
#define GPIO135		135
#define GPIO136		136
#define GPIO137		137
#define GPIO138		138
#define GPIO139		139
#define GPIO140		140
#define GPIO141		141
#define GPIO142		142
#define GPIO143		143
#define GPIO144		144
#define GPIO145		145
#define GPIO146		146
#define GPIO147		147
#define GPIO148		148
#define GPIO149		149
#define GPIO150		150
#define GPIO151		151
#define GPIO152		152
#define GPIO153		153
#define GPIO154		154
#define GPIO155		155
#define GPIO156		156
#define GPIO157		157
#define GPIO158		158
#define GPIO159		159

#define GPIO160		160
#define GPIO161		161
#define GPIO162		162
#define GPIO163		163
#define GPIO164		164
#define GPIO165		165
#define GPIO166		166
#define GPIO167		167
#define GPIO168		168
#define GPIO169		169
#define GPIO170		170
#define GPIO171		171
#define GPIO172		172
#define GPIO173		173
#define GPIO174		174
#define GPIO175		175
#define GPIO176		176
#define GPIO177		177
#define GPIO178		178
#define GPIO179		179
#define GPIO180		180
#define GPIO181		181
#define GPIO182		182
#define GPIO183		183
#define GPIO184		184
#define GPIO185		185
#define GPIO186		186
#define GPIO187		187
#define GPIO188		188
#define GPIO189		189
#define GPIO190		190
#define GPIO191		191

#define GPIO192		192
#define GPIO193		193
#define GPIO194		194
#define GPIO195		195
#define GPIO196		196
#define GPIO197		197
#define GPIO198		198
#define GPIO199		199
#define GPIO200		200
#define GPIO201		201
#define GPIO202		202
#define GPIO203		203
#define GPIO204		204
#define GPIO205		205
#define GPIO206		206
#define GPIO207		207
#define GPIO208		208
#define GPIO209		209
#define GPIO210		210
#define GPIO211		211

#define GPIO_NUMBER	212

#define GPIO_DIR_OUTPUT		0
#define GPIO_DIR_INPUT		1

#define GPIO_INT_POL_LOW	0
#define GPIO_INT_POL_HIGH	1

#define GPIO_INT_TYPE_LEVEL	0
#define GPIO_INT_TYPE_EDGE	1

void mv_gpio_enable(int no);
void mv_gpio_disable(int no);
void mv_gpio_set_dir(int no, int dir);
void mv_gpio_set_out_data(int no, int data);
int mv_gpio_get_in_data(int no);
void mv_gpio_int_set_polarity(int no, int pol);
void mv_gpio_int_set_type(int no, int type);
void mv_gpio_int_enable(int no);
void mv_gpio_int_disable(int no);
void mv_gpio_int_clear(int no);
int mv_gpio_get_int_state(int no);
void mv_gpio_irq_init(int gpio_no, bool polarity, bool type, bool enable);
int mv_request_gpio_irq(int gpio_no, irq_handler_t handler, const char *devname, void *devid);
int mv_clear_gpio_irq(int gpio_no);

#endif /* __ASM_ARCH_GPIO_H */

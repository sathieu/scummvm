/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * This code is based on original Mortville Manor DOS source code
 * Copyright (c) 1988-1989 Lankhor
 */

#include "common/str.h"
#include "mortevielle/level15.h"
#include "mortevielle/mortevielle.h"
#include "mortevielle/mouse.h"
#include "mortevielle/outtext.h"
#include "mortevielle/ques.h"

namespace Mortevielle {

/* overlay */ bool ques() {
	const int ta[11] = {0, 511, 516, 524, 531, 545, 552, 559, 563, 570, 576};
	const int ok[11] = {0, 4, 7, 1, 6, 4, 4, 2, 5, 3, 1 };

	bool q, func, test;
	int i, j, k, y, memk;
	int tay , tmax;
	int rep, prem, der;
	char st[1410];
	char key;
	rectangle coor[max_rect];
	Common::String chaines[15];
	int compte;


	bool ques_result;
	test = false;
	i = 0;
	compte = 0;

	do {
		hide_mouse();
		hirs();
		show_mouse();
		i = i + 1;
		deline(ta[i], st, tay);
		if (res == 1)  y = 29;
		else y = 23;
		g_vm->_screenSurface.fillRect(15, Common::Rect(0, 14, 630, y));
		afftex(st, 20, 15, 100, 2, 0);
		if (i != 10) {
			prem = ta[i] + 1;
			der = ta[i + 1] - 1;
		} else {
			prem = 503;
			der = 510;
		}
		y = 35;
		tmax = 0;
		memk = 1;
		for (j = prem; j <= der; j ++) {
			deline(j, st, tay);
			if (tay > tmax)  tmax = tay;
			afftex(st, 100, y, 100, 1, 0);
			chaines[memk] = delig;
			memk = memk + 1;
			y = y + 8;
		}
		for (j = 1; j <= succ(int, der - prem); j ++) {
			{
				rectangle &with = coor[j];

				with.x1 = 45 * res;
				with.x2 = (tmax * 3 + 55) * res;
				with.y1 = 27 + j * 8;
				with.y2 = 34 + j * 8;
				with.etat = true;
			}
			while ((int)chaines[j].size() < tmax) {
				chaines[j] = chaines[j] + ' ';
			}
		}
		coor[j + 1].etat = false;
		if (res == 1)  rep = 10;
		else rep = 6;
		g_vm->_screenSurface.drawBox(80, 33, 40 + tmax * rep, (der - prem) * 8 + 16, 15);
		rep = 0;
		j = 0;
		memk = 0;
		do {
			g_vm->setMouseClick(false);
			tesok = false;
			mov_mouse(func, key);
			CHECK_QUIT0;

			k = 1;
			while (coor[k].etat && ! dans_rect(coor[k]))  k = k + 1;
			if (coor[k].etat) {
				if ((memk != 0) && (memk != k)) {
					for (j = 1; j <= tmax; j ++) st[j] = chaines[memk][j];
					st[1 + tmax] = '$';
					afftex(st, 100, 27 + memk * 8, 100, 1, 0);
				}
				if (memk != k) {
					for (j = 1; j <= tmax; j ++) st[j] = chaines[k][j];
					st[1 + tmax] = '$';
					afftex(st, 100, 27 + k * 8, 100, 1, 1);
					memk = k;
				}
			} else if (memk != 0) {
				for (j = 1; j <= tmax; j ++) st[j] = chaines[memk][j];
				st[1 + tmax] = '$';
				afftex(st, 100, 27 + memk * 8, 100, 1, 0);
				memk = 0;
			}
		} while (!((memk != 0) && g_vm->getMouseClick()));
		if (memk == ok[i])  compte = compte + 1;
		else {
			if (i == 5)  i = i + 1;
			if ((i == 7) || (i == 8))  i = 10;
		}
		if (i == 10)  q =   /*testprot*/ true;
	} while (!(i == 10));
	ques_result = (compte == 10) && q;
	return ques_result;
}

} // End of namespace Mortevielle

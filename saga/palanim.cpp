/* ScummVM - Scumm Interpreter
 * Copyright (C) 2004 The ScummVM project
 *
 * The ReInherit Engine is (C)2000-2003 by Daniel Balsom.
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */
/*
 Description:   
 
    Palette animation module

 Notes: 
*/

#include "reinherit.h"

#include "yslib.h"

/*
 * Uses the following modules:
\*--------------------------------------------------------------------------*/
#include "events_mod.h"
#include "game_mod.h"

/*
 * Begin module:
\*--------------------------------------------------------------------------*/
#include "palanim_mod.h"
#include "palanim.h"

namespace Saga {

static PALANIM_DATA PAnimData;

int PALANIM_Load(const uchar * resdata, size_t resdata_len)
{
	const uchar *read_p = resdata;
	void *test_p;

	uint i;

	YS_IGNORE_PARAM(resdata_len);

	if (PAnimData.loaded) {
		PALANIM_Free();
	}

	if (resdata == NULL) {
		return R_FAILURE;
	}

	if (GAME_GetGameType() == R_GAMETYPE_IHNM) {
		return R_SUCCESS;
	}

	PAnimData.entry_count = ys_read_u16_le(read_p, &read_p);

	R_printf(R_STDOUT,
	    "PALANIM_Load(): Loading %d PALANIM entries.\n",
	    PAnimData.entry_count);

	test_p = calloc(PAnimData.entry_count, sizeof(PALANIM_ENTRY));
	if (test_p == NULL) {
		R_printf(R_STDERR, "PALANIM_Load(): Allocation failure.\n");
		return R_MEM;
	}

	PAnimData.entries = (PALANIM_ENTRY *)test_p;

	for (i = 0; i < PAnimData.entry_count; i++) {

		int color_count;
		int pal_count;
		int p, c;

		color_count = ys_read_u16_le(read_p, &read_p);
		pal_count = ys_read_u16_le(read_p, &read_p);

		PAnimData.entries[i].pal_count = pal_count;
		PAnimData.entries[i].color_count = color_count;

#       if 0
		R_printf(R_STDOUT,
		    "PALANIM_Load(): Entry %d: Loading %d palette indices.\n",
		    i, pal_count);
#endif

		test_p = calloc(1, sizeof(char) * pal_count);
		if (test_p == NULL) {
			R_printf(R_STDERR,
			    "PALANIM_Load(): Allocation failure.\n");
			return R_MEM;
		}

		PAnimData.entries[i].pal_index = (uchar *)test_p;

#       if 0
		R_printf(R_STDOUT,
		    "PALANIM_Load(): Entry %d: Loading %d SAGA_COLOR "
		    "structures.\n", i, color_count);
#       endif

		test_p = calloc(1, sizeof(R_COLOR) * color_count);
		if (test_p == NULL) {
			R_printf(R_STDERR,
			    "PALANIM_Load(): Allocation failure.\n");
			return R_MEM;
		}

		PAnimData.entries[i].colors = (R_COLOR *)test_p;

		for (p = 0; p < pal_count; p++) {
			PAnimData.entries[i].pal_index[p] =
			    (uchar) ys_read_u8(read_p, &read_p);
		}

		for (c = 0; c < color_count; c++) {
			PAnimData.entries[i].colors[c].red =
			    (uchar) ys_read_u8(read_p, &read_p);

			PAnimData.entries[i].colors[c].green =
			    (uchar) ys_read_u8(read_p, &read_p);

			PAnimData.entries[i].colors[c].blue =
			    (uchar) ys_read_u8(read_p, &read_p);
		}
	}

	PAnimData.loaded = 1;
	return R_SUCCESS;

}

int PALANIM_CycleStart(void)
{

	R_EVENT event;

	if (!PAnimData.loaded) {
		return R_FAILURE;
	}

	event.type = R_ONESHOT_EVENT;
	event.code = R_PALANIM_EVENT;
	event.op = EVENT_CYCLESTEP;
	event.time = PALANIM_CYCLETIME;

	EVENT_Queue(&event);

	return R_SUCCESS;

}

int PALANIM_CycleStep(int vectortime)
{
	R_SURFACE *back_buf;

	static PALENTRY pal[256];
	uint pal_index;
	uint col_index;

	uint i, j;
	uint cycle;
	uint cycle_limit;

	R_EVENT event;

	if (!PAnimData.loaded) {
		return R_FAILURE;
	}

	SYSGFX_GetCurrentPal(pal);
	back_buf = SYSGFX_GetBackBuffer();

	for (i = 0; i < PAnimData.entry_count; i++) {

		cycle = PAnimData.entries[i].cycle;
		cycle_limit = PAnimData.entries[i].color_count;

		for (j = 0; j < PAnimData.entries[i].pal_count; j++) {

			pal_index =
			    (unsigned char)PAnimData.entries[i].pal_index[j];
			col_index = (cycle + j) % cycle_limit;

			pal[pal_index].red =
			    (uchar) PAnimData.entries[i].colors[col_index].red;

			pal[pal_index].green =
			    (uchar) PAnimData.entries[i].colors[col_index].
			    green;

			pal[pal_index].blue =
			    (uchar) PAnimData.entries[i].colors[col_index].
			    blue;
		}

		PAnimData.entries[i].cycle++;

		if (PAnimData.entries[i].cycle == cycle_limit) {
			PAnimData.entries[i].cycle = 0;
		}
	}

	SYSGFX_SetPalette(back_buf, pal);

	event.type = R_ONESHOT_EVENT;
	event.code = R_PALANIM_EVENT;
	event.op = EVENT_CYCLESTEP;
	event.time = vectortime + PALANIM_CYCLETIME;

	EVENT_Queue(&event);

	return R_SUCCESS;

}

int PALANIM_Free(void)
{

	uint i;

	if (!PAnimData.loaded) {
		return R_FAILURE;
	}

	for (i = 0; i < PAnimData.entry_count; i++) {
#if 0
		R_printf(R_STDOUT,
		    "PALANIM_Free(): Entry %d: Freeing colors.\n", i);
#endif
		free(PAnimData.entries[i].colors);
#if 0
		R_printf(R_STDOUT,
		    "PALANIM_Free(): Entry %d: Freeing indices.\n", i);
#endif
		free(PAnimData.entries[i].pal_index);

	}

	R_printf(R_STDOUT, "PALANIM_Free(): Freeing entries.\n");

	free(PAnimData.entries);

	PAnimData.loaded = 0;

	return R_SUCCESS;
}

} // End of namespace Saga

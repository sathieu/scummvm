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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef SCUMM_SE_H
#define SCUMM_SE_H

#ifdef ENABLE_SCUMM_SE

#include "graphics/pixelformat.h"
#include "scumm/scumm_v5.h"
#include "scumm/se/resource_se.h"

namespace Scumm {

class ScummEngine_se : public ScummEngine_v5 {
protected:
	ResourceManager_se *_resSE;
protected:
	virtual void setupRoomSubBlocks();
	virtual void readIndexFile();

	virtual void redrawBGStrip(int start, int num);
	//virtual void redrawBGAreas();
public:
	ScummEngine_se(OSystem *syst, const DetectorResult &dr);
	~ScummEngine_se();

	void setClassicMode(bool mode);
	void processKeyboard(Common::KeyState lastKeyHit);
	ResourceManager_se *getResourceManagerSE() { return _resSE; }
};


} // End of namespace Scumm

#endif

#endif

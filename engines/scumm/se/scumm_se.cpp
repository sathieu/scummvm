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

#include "util.h"
#include "scumm/se/scumm_se.h"

namespace Scumm {

#ifdef ENABLE_SCUMM_SE

ScummEngine_se::ScummEngine_se(OSystem *syst, const DetectorResult &dr)
	: ScummEngine_v5(syst, dr) {
	_resSE = new ResourceManager_se(this);
}

ScummEngine_se::~ScummEngine_se() {
	delete _resSE;
}

void ScummEngine_se::setClassicMode(bool mode) {
	if (mode) { // Go to classic mode
		if (_game.features & GF_CLASSIC_MODE)
			return;
		_game.features |= GF_CLASSIC_MODE;
		_screenWidth = 320;
		_screenHeight = 200;
		_stripWidth = 8;
		_gdi->_numStrips = _screenWidth / _stripWidth;
		initGraphics(_screenWidth, _screenHeight, (_screenWidth > 320), &_outputPixelFormat);
		_outputPixelFormat = _system->getScreenFormat();
		initScreens(16, 144);
		_virtscr[kMainVirtScreen].xstart /= 6;
	} else { // Go to SE mode
		if (!(_game.features & GF_CLASSIC_MODE))
			return;
		_game.features &= ~GF_CLASSIC_MODE;
		_screenWidth = 1920;
		_screenHeight = 1080;
		_stripWidth = 48;
		_gdi->_numStrips = _screenWidth / _stripWidth;
		initGraphics(_screenWidth, _screenHeight, false, &_outputPixelFormat);
		_outputPixelFormat = _system->getScreenFormat();
		initScreens(0, 1070);
		_virtscr[kMainVirtScreen].xstart *= 6;
	}
	_completeScreenRedraw = true;
	// _bgNeedsRedraw = true;
	//FIXME: HACK to avoid negative _screenTop
	if (camera._cur.y < (_screenHeight / 2)) {
		camera._cur.y = (_screenHeight / 2);
	}
}
#endif

} // End of namespace Scumm

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

#include "common/keyboard.h"
#include "scumm/se/scumm_se.h"

namespace Scumm {

#ifdef ENABLE_SCUMM_SE

void ScummEngine_se::processKeyboard(Common::KeyState lastKeyHit) {
	// Fall back to default behavior
	ScummEngine::processKeyboard(lastKeyHit);

	// Switch Classic mode
	if (lastKeyHit.keycode == Common::KEYCODE_F10 && lastKeyHit.hasFlags(0))
		setClassicMode(!(_game.features & GF_CLASSIC_MODE));
	if (lastKeyHit.keycode == Common::KEYCODE_PAGEUP && lastKeyHit.hasFlags(0))
		setClassicMode(true);
	if (lastKeyHit.keycode == Common::KEYCODE_PAGEDOWN && lastKeyHit.hasFlags(0))
		setClassicMode(false);
}

#endif

} // End of namespace Scumm

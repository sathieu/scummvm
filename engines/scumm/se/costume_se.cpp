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

#include "scumm/costume.h"
#include "scumm/scumm.h"
#include "scumm/util.h"
#include "scumm/se/scumm_se.h"
#include "scumm/se/costume_se.h"
#include "scumm/se/resource_se.h"

// FIXME copy from gfx.cpp
static void blit(byte *dst, int dstPitch, const byte *src, int srcPitch, int w, int h, uint8 bitDepth);
static void blit(byte *dst, int dstPitch, const byte *src, int srcPitch, int w, int h, uint8 bitDepth) {
	assert(w > 0);
	assert(h > 0);
	assert(src != NULL);
	assert(dst != NULL);

	if ((w * bitDepth == srcPitch) && (w * bitDepth == dstPitch)) {
		memcpy(dst, src, w * h * bitDepth);
	} else {
		do {
			memcpy(dst, src, w * bitDepth);
			dst += dstPitch;
			src += srcPitch;
		} while (--h);
	}
}

// FIXME copied from gfx_se.cpp
static void blitAlpha(byte *dst, int dstPitch, const byte *src, int srcPitch, int w, int h, uint8 bitDepth, const Graphics::PixelFormat *format);
static void blitAlpha(byte *dst, int dstPitch, const byte *src, int srcPitch, int w, int h, uint8 bitDepth, const Graphics::PixelFormat *format) {
	assert(w > 0);
	assert(h > 0);
	assert(src != NULL);
	assert(dst != NULL);
	assert(bitDepth == 4);

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			const uint32 *srcColor = (const uint32 *) ((const uint8 *)(src) + y * srcPitch + x * bitDepth);
			      uint32 *dstColor = (      uint32 *) ((      uint8 *)(dst) + y * dstPitch + x * bitDepth);
			const uint32 srcAlpha = ((*srcColor >> format->aShift) << format->aLoss) & 0xFF;
			if (srcAlpha == 0xFF) {
				*dstColor = *srcColor;
			} else if (srcAlpha != 0x00) {
				// naive alpha blending
				const uint32 srcR = ((*srcColor >> format->rShift) << format->rLoss) & 0xFF;
				const uint32 srcG = ((*srcColor >> format->gShift) << format->gLoss) & 0xFF;
				const uint32 srcB = ((*srcColor >> format->bShift) << format->bLoss) & 0xFF;
				const uint32 dstR = ((*dstColor >> format->rShift) << format->rLoss) & 0xFF;
				const uint32 dstG = ((*dstColor >> format->gShift) << format->gLoss) & 0xFF;
				const uint32 dstB = ((*dstColor >> format->bShift) << format->bLoss) & 0xFF;
				*dstColor = (0xFF << format->aShift)
				            | ((srcR * srcAlpha + dstR * (0xFF - srcAlpha)) / 0xFF) << format->rShift
				            | ((srcG * srcAlpha + dstG * (0xFF - srcAlpha)) / 0xFF) << format->gShift
				            | ((srcB * srcAlpha + dstB * (0xFF - srcAlpha)) / 0xFF) << format->bShift;
			}
		}
	}
}

namespace Scumm {

#ifdef ENABLE_SCUMM_SE

void SpecialEditionCostumeLoader::loadCostume(int id) {
	ClassicCostumeLoader::loadCostume(id);
	if (_vm->_game.features & GF_CLASSIC_MODE) {
		return;
	}
	_id = id;
	// Costume load is done in ResourceManager_se
}

byte SpecialEditionCostumeRenderer::drawLimb(const Actor *actor, int limb) {
	byte ret = ClassicCostumeRenderer::drawLimb(actor, limb);
	if (_vm->_game.features & GF_CLASSIC_MODE) {
		return ret;
	}

	ScummEngine_se *vm = static_cast<ScummEngine_se *>(_vm);
	const CostumeData &cost = actor->_cost;

	/*
	// If the specified limb is stopped or not existing, do nothing.
	if (cost.curpos[limb] == 0xFFFF || cost.stopped & (1 << limb))
		return 0;
	// */

	ResourceManager_se *resSE = vm->getResourceManagerSE();
	ResourceManager_se::Costume *costumeSE = resSE->getCostume(_loaded._id);

	// Construct animationGroup{Name,Index}
	Common::String animationGroupName;
	uint32 animationGroupIndex = 0;
	if (actor->_frame == actor->_initFrame) {
		animationGroupName = "Init";
	} else if (actor->_frame == actor->_walkFrame) {
		animationGroupName = "Walk";
	} else if (actor->_frame == actor->_standFrame) {
		animationGroupName = "Stand";
	} else if (actor->_frame == actor->_talkStartFrame) {
		animationGroupName = "StartTalk";
	} else if (actor->_frame == actor->_talkStopFrame) {
		animationGroupName = "StopTalk";
	} else {
		char tmp[4] = "";
		sprintf(tmp,"%d", actor->_frame);
		animationGroupName = "Chore";
		animationGroupName += tmp;
	}
	int facing = (actor->getFacing() + 360) % 360;
	if (facing < 45) {
		animationGroupName += "Back";
		animationGroupIndex += 3; // Back
	} else if (facing < 90 + 45) {
		animationGroupName += "Left";
		animationGroupIndex += 0; // Left
	} else if (facing < 180 + 45) {
		animationGroupName += "Front";
		animationGroupIndex += 2; // Front
	} else if (facing < 270 + 45) {
		animationGroupName += "Right";
		animationGroupIndex += 1; // Right
	} else {
		animationGroupName += "Back";
		animationGroupIndex += 3; // Back
	}
	uint32 animationIndex = limb;
	uint32 frameIndex = cost.curpos[limb];
	Graphics::Surface surf =costumeSE->getSurface(animationGroupName, animationGroupIndex, animationIndex, frameIndex, actor->_scalex);
	if (!surf.h || !surf.w) {
		return 0;
	}
	// prepare blit
	int w = surf.w;
	int h = surf.h;
	int srcX = 0;
	int srcY = 0;
	int dstX = _actorX;
	dstX *= 6; // FIXME Wild guessed SE/classic ratio
	dstX -= w / 2; // from left to middle
	int dstY = _actorY;
	dstY *= 6; // FIXME Wild guessed SE/classic ratio
	dstY = dstY * 6 / 5; // aspect ratio  correction
	dstY -= h; // from top to bottom
	if (dstX < 0) {
		w += dstX;
		srcX -= dstX;
		dstX = 0;
	}
	if (dstY < 0) {
		h += dstY;
		srcY -= dstY;
		dstY = 0;
	}
	// Fill info
	_draw_top = dstY;
	_draw_bottom = dstY + h;
	_width = w;
	_height = h;
	if ((h <= 0) || (w <= 0)) {
		return 0;
	}
	// blit
	byte *srcPtr = (byte *) surf.getBasePtr(srcX, srcY);
	byte *dst = (byte *)_out.getBasePtr(dstX, dstY);
	if (surf.format.aLoss < 8) {
		blitAlpha(dst, _out.pitch, srcPtr, surf.pitch, w, h, surf.format.bytesPerPixel, &surf.format);
	} else {
		blit(dst, _out.pitch, srcPtr, surf.pitch, w, h, surf.format.bytesPerPixel);
	}
	_vm->markRectAsDirty(kMainVirtScreen, dstX, dstX + w, _draw_top, _draw_bottom, _actorID);
	return ret;
}

#endif

} // End of namespace Scumm

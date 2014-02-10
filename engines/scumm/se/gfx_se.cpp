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

#include "common/algorithm.h"
#include "scumm/se/scumm_se.h"
#include "scumm/se/resource_se.h"
#include "scumm/file.h"
#include "scumm/usage_bits.h"
#include "scumm/charset.h"
#include "graphics/decoders/dds.h"

namespace Scumm {

#ifdef ENABLE_SCUMM_SE

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

void ResourceManager_se::Room::loadSprites() {
	if (_spritesLoaded) {
		return;
	}
	_spritesLoaded = true;
	Graphics::DXTDecoder *decoder = new Graphics::DXTDecoder();
	for(uint32 layer = 0; layer < _staticSpriteList.size(); layer++ ) {
		Common::Array<staticSprite> *innerStaticSpriteList = &_staticSpriteList[layer];
		for(uint32 index2 = 0; index2 < innerStaticSpriteList->size(); index2++ ) {
			struct staticSprite *innerStaticSprite = &(*innerStaticSpriteList)[index2];
			if (innerStaticSprite->textureFileName.empty()) {
				continue;
			}
			if (!_resSE->openFile(innerStaticSprite->textureFileName)) {
				warning("Unable to open static sprite file %s", innerStaticSprite->textureFileName.c_str());
				continue;
			}
			if (!decoder->loadStream(*_resSE->_fileHandle)) {
				warning("Unable to load stream from file %s", innerStaticSprite->textureFileName.c_str());
				continue;
			}
			innerStaticSprite->surface.copyFrom(*decoder->getSurface());
#if 0
			// Draw red rectangle with diagonals
			innerStaticSprite->surface.frameRect(
			  Common::Rect(0, 0, innerStaticSprite->width, innerStaticSprite->height),
			  0x0000FFFF);
			innerStaticSprite->surface.drawThickLine(
			  0, 0, innerStaticSprite->width, innerStaticSprite->height,
			  2, 2, 0x0000FFFF);
			innerStaticSprite->surface.drawThickLine(
			  0, innerStaticSprite->height, innerStaticSprite->width, 0,
			  2, 2, 0x0000FFFF);
#endif
		}
	}

	for(uint32 layer = 0; layer < _spriteList.size(); layer++ ) {
		Common::Array<sprite> *innerSpriteList = &_spriteList[layer];
		for(uint32 index2 = 0; index2 < innerSpriteList->size(); index2++ ) {
			struct sprite *innerSprite = &(*innerSpriteList)[index2];
			if (innerSprite->textureFileName.empty()) {
				continue;
			}
			if (!_resSE->openFile(innerSprite->textureFileName)) {
				warning("Unable to open sprite file %s", innerSprite->textureFileName.c_str());
				continue;
			}
			if (!decoder->loadStream(*_resSE->_fileHandle)) {
				warning("Unable to load stream from file %s", innerSprite->textureFileName.c_str());
				continue;
			}
			innerSprite->surface.copyFrom(*decoder->getSurface());
		}
	}

	for(uint32 index = 0; index < _extraSpriteList.size(); index++ ) {
		Common::Array<staticSprite> *innerExtraSpriteList = &_extraSpriteList[index];
		for(uint32 index2 = 0; index2 < innerExtraSpriteList->size(); index2++ ) {
			struct staticSprite *innerExtraSprite = &(*innerExtraSpriteList)[index2];
			if (innerExtraSprite->textureFileName.empty()) {
				continue;
			}
			if (!_resSE->openFile(innerExtraSprite->textureFileName)) {
				warning("Unable to open sprite file %s", innerExtraSprite->textureFileName.c_str());
				continue;
			}
			if (!decoder->loadStream(*_resSE->_fileHandle)) {
				warning("Unable to load stream from file %s", innerExtraSprite->textureFileName.c_str());
				continue;
			}
			innerExtraSprite->surface.copyFrom(*decoder->getSurface());
		}
	}
}

void ResourceManager_se::Room::drawStaticSpriteList(ScummEngine_se *vm, VirtScreenNumber virt, Common::Array< Common::Array<staticSprite> > staticSpriteList, int start, int num) {
	VirtScreen *vs = &vm->_virtscr[virt];

	for(uint32 layer = 0; layer < staticSpriteList.size(); layer++ ) {
		Common::Array<staticSprite> *innerStaticSpriteList = &staticSpriteList[layer];
		for(uint32 index2 = 0; index2 < innerStaticSpriteList->size(); index2++ ) {
			struct staticSprite *innerStaticSprite = &(*innerStaticSpriteList)[index2];
			if ((innerStaticSprite->surface.h == 0) || (innerStaticSprite->surface.h == 0)) {
				warning("Missing static sprite %i:%i from room %i", layer, index2, _identifier);
				continue;
			}
			assert(innerStaticSprite->width <= innerStaticSprite->surface.w);
			assert(innerStaticSprite->height <= innerStaticSprite->surface.h);
			Common::Rect rectSrc(innerStaticSprite->x,                            innerStaticSprite->y,
			                     innerStaticSprite->x + innerStaticSprite->width, innerStaticSprite->y + innerStaticSprite->height);
			Common::Rect rectDst(start * 48, 0, (start + num) * 48, vs->h);
			if (rectSrc.intersects(rectDst)) {
				rectSrc.clip(rectDst);
				rectDst.left = rectSrc.left;
				rectDst.top = rectSrc.top;
				rectSrc.left -= innerStaticSprite->x;
				rectSrc.right -= innerStaticSprite->x;
				rectSrc.top -= innerStaticSprite->y;
				rectSrc.bottom -= innerStaticSprite->y;

				warning("Rendering static sprite %i:%i=%s from room %i (%i, %i)+(%i, %i) at (%i, %i)+(%i, %i)", layer, index2, innerStaticSprite->textureFileName.c_str(), _identifier,
					innerStaticSprite->x, innerStaticSprite->y, innerStaticSprite->width, innerStaticSprite->height,
					rectDst.left, rectDst.top, rectSrc.width(), rectSrc.height());
				byte *srcPtr = (byte *) innerStaticSprite->surface.getBasePtr(rectSrc.left, rectSrc.top);
				byte *dstPtr = vs->getBackPixels(rectDst.left - vs->xstart, rectDst.top);
				if (innerStaticSprite->surface.format.aLoss < 8) {
					blitAlpha(dstPtr, vs->pitch, srcPtr, innerStaticSprite->surface.pitch, rectSrc.width(), rectSrc.height(), vs->format.bytesPerPixel, &vs->format);
				} else {
					blit(dstPtr, vs->pitch, srcPtr, innerStaticSprite->surface.pitch, rectSrc.width(), rectSrc.height(), vs->format.bytesPerPixel);
				}
				byte *src = vs->getBackPixels(rectDst.left - vs->xstart, rectDst.top);
				byte *dst = vs->getPixels(rectDst.left - vs->xstart, rectDst.top);
				blit(dst, vs->pitch, src, vs->pitch, rectSrc.width(), rectSrc.height(), vs->format.bytesPerPixel);
				//vm->markRectAsDirty(virt, start * 48, (start + num) * 48, 0, vs->h, USAGE_BIT_DIRTY);
			}
		}
	}
}

void ResourceManager_se::Room::redrawBGStrip(ScummEngine_se *vm, VirtScreenNumber virt, int start, int num) {
	assert(vm);
	VirtScreen *vs = &vm->_virtscr[virt];
	assert(vs->hasTwoBuffers);
	// Reset to green background (FIXME - should be black)
	Graphics::Surface backSurface = Graphics::Surface();
	backSurface.create(vs->w, vs->h, vs->format);
	backSurface.pitch = vs->pitch;
	backSurface.setPixels(vs->getBackPixels(start * 48 - vs->xstart, 0));
	backSurface.fillRect(Common::Rect(0, 0, num * 48, vs->h), 0x00FF00FF);

	Graphics::Surface frontSurface = Graphics::Surface();
	frontSurface.create(vs->w, vs->h, vs->format);
	frontSurface.pitch = vs->pitch;
	frontSurface.setPixels(vs->getPixels(start * 48 - vs->xstart, 0));
	frontSurface.fillRect(Common::Rect(0, 0, num * 48, vs->h), 0x00FF00FF);

	vm->markRectAsDirty(virt, start * 48 - vs->xstart, (start + num) * 48 - vs->xstart, 0, vs->h, USAGE_BIT_DIRTY);
	//drawStaticSpriteList(vm, virt, _extraSpriteList);
	drawStaticSpriteList(vm, virt, _staticSpriteList, start, num);
}

const Graphics::Surface ResourceManager_se::Costume::getTexture(uint32 index, bool mirror, byte scale) {
	if (index == 0xFFFFFFFF)
		return Graphics::Surface();
	assert(index < _textureList.size());
	struct texture *currentTexture = &_textureList[index];

	// load full size texture
	if (!(currentTexture->flags[0xFF] & TF_EXISTS)) {
		currentTexture->flags[0xFF] |= TF_EXISTS;
		if (currentTexture->filename.empty()) {
			return currentTexture->surface[0xFF];
		}
		if (!_resSE->openFile(currentTexture->filename)) {
			warning("Unable to open sprite file %s", currentTexture->filename.c_str());
			return currentTexture->surface[0xFF];
		}
		Graphics::DXTDecoder *decoder = new Graphics::DXTDecoder();
		if (!decoder->loadStream(*_resSE->_fileHandle)) {
			warning("Unable to load stream from file %s", currentTexture->filename.c_str());
			return currentTexture->surface[0xFF];
		}
		currentTexture->surface[0xFF].copyFrom(*decoder->getSurface());
	}

	// scale if necessary
	if (!(currentTexture->flags[scale] & TF_EXISTS)) {
		currentTexture->flags[scale] |= TF_EXISTS;
		int srcw = currentTexture->surface[0xFF].w;
		int srch = currentTexture->surface[0xFF].h;
		int dstw = srcw * scale / 0xFF;
		int dsth = srch * scale / 0xFF;
		currentTexture->surface[scale].create(dstw, dsth, currentTexture->surface[0xFF].format);
		for (int x = 0; x < dstw - 1; x++) {
			for (int y = 0; y < dsth - 1; y++) {
				uint32 *src = (uint32 *) currentTexture->surface[0xFF].getBasePtr(x * srcw / dstw, y * srch / dsth);
				uint32 *dst = (uint32 *) currentTexture->surface[scale].getBasePtr(x, y);
				WRITE_UINT32(dst, *src);
			}
		}
	}

	// mirror if necessary
	if (mirror && !(currentTexture->flags[scale] & TF_MIRROR_EXISTS)) {
		currentTexture->flags[scale] |= TF_MIRROR_EXISTS;
		int w = currentTexture->surface[scale].w;
		int h = currentTexture->surface[scale].h;
		currentTexture->mirroredSurface[scale].create(w, h, currentTexture->surface[scale].format);
		uint32 *src = (uint32 *) currentTexture->surface[scale].getBasePtr(0, 0);
		uint32 *dst = (uint32 *) currentTexture->mirroredSurface[scale].getBasePtr(0, 0);
		dst += w - 1; // end of first line
		do {
			do {
				WRITE_UINT32(dst, *src);
				src++;
				dst--;
			} while (--w);
			w = currentTexture->surface[scale].w;
			dst += 2 * w; // end of next line
		} while (--h);
	}
	if (mirror)
		return currentTexture->mirroredSurface[scale];
	else
		return currentTexture->surface[scale];
}

const Graphics::Surface ResourceManager_se::Costume::getSurface(const Common::String &animationGroupName, const uint32 animationGroupIndex, const uint32 animationIndex, const uint32 frameIndex, const byte scale) {
	// animationGroup > animation > frame
	uint32 animationGroupIndex2 = animationGroupIndex;
	if (animationGroupName.size()) {
		for(uint32 index = 0; index < _animationGroupList.size(); index++ ) {
			struct animationGroup *h = &_animationGroupList[index];
			if (h->name.equals(animationGroupName)) {
				animationGroupIndex2 = index;
				break;
			}
		}
	}
	assert(animationGroupIndex2 < _animationGroupList.size());
	struct animationGroup *ag = 0;
	ag = &_animationGroupList[animationGroupIndex2];
	if (animationIndex >= ag->animationList.size()) {
		return Graphics::Surface();
	}
	struct animation *a = 0;
	a = &ag->animationList[animationIndex];
	if (!a->frameList.size()) {
		return Graphics::Surface();
	}
	uint32 frameIndex2 = frameIndex % a->frameList.size();
	assert(frameIndex2 < a->frameList.size());
	struct frame *f = 0;
	f = &a->frameList[frameIndex2];
	// spriteGroup > sprite > texture
	assert(a->spriteGroupIdentitier < _spriteGroupList.size());
	struct spriteGroup *sg = 0;
	sg = &_spriteGroupList[a->spriteGroupIdentitier];
	if (f->spriteIdentitier == 0xFFFFFFFF) {
		return Graphics::Surface();
	}
	assert(f->spriteIdentitier < sg->spriteList.size());
	struct sprite *s = 0;
	s = &sg->spriteList[f->spriteIdentitier];
	bool mirror = ag->name.hasSuffix("Right");
	const Graphics::Surface surf = getTexture(s->textureNumber, mirror, scale);
	// get sprite
	Common::Rect rect;
	if (mirror)
		rect = Common::Rect(
		  surf.w - (s->textureX + s->textureWidth) * scale / 0xFF,
		  s->textureY * scale / 0xFF,
		  surf.w - s->textureX * scale / 0xFF,
		  (s->textureY + s->textureHeight) * scale / 0xFF);
	else
		rect = Common::Rect(
		  s->textureX * scale / 0xFF,
		  s->textureY * scale / 0xFF,
		  (s->textureX + s->textureWidth) * scale / 0xFF,
		  (s->textureY + s->textureHeight) * scale / 0xFF);
	return surf.getSubArea(rect);
}

void ScummEngine_se::redrawBGStrip(int start, int num) {
	if (_game.features & GF_CLASSIC_MODE) {
		ScummEngine::redrawBGStrip(start, num);
		return;
	}

	ResourceManager_se::Room *room = _resSE->getRoom(_currentRoom);
	room->loadSprites();
	if (((uint32) _virtscr[kMainVirtScreen].h != room->_roomHeight)) {
		initScreens(0, room->_roomHeight);
	}

	int s = _screenStartStrip + start;

	for (int i = 0; i < num; i++)
		setGfxUsageBit(s + i, USAGE_BIT_DIRTY);

	room->redrawBGStrip(this, kMainVirtScreen, s, num);
}

/*
void ScummEngine_se::redrawBGAreas() {
	if (_game.features & GF_CLASSIC_MODE) {
		ScummEngine::redrawBGAreas();
		return;
	}
	if (camera._cur.x != camera._last.x && _charset->_hasMask)
		stopTalk();

	ResourceManager_se::Room *room = _resSE->getRoom(_currentRoom);
	room->loadSprites();
	if (((uint32) _virtscr[kMainVirtScreen].h != room->_roomHeight)) {
		initScreens(0, room->_roomHeight);
	}
	if (_fullRedraw) {
		_bgNeedsRedraw = false;
		room->redrawBGStrip(this, kMainVirtScreen, 0, _gdi->_numStrips);
	}

	drawRoomObjects(0);
	_bgNeedsRedraw = false;

}
// */
#endif

} // End of namespace Scumm

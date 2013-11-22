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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @file
 * Image decoder used in engines:
 *  - scumm (monkeyse, monkey2se)
 */

#ifndef GRAPHICS_DECODERS_DDS_H
#define GRAPHICS_DECODERS_DDS_H

#include "common/scummsys.h"
#include "graphics/decoders/image_decoder.h"

namespace Common{
class SeekableReadStream;
}

namespace Graphics {

struct Surface;

class DDSDecoder : public ImageDecoder {
private:
	Surface *_surface;

public:
	DDSDecoder();
	virtual ~DDSDecoder();

	// ImageDecoder API
	void destroy();
	virtual bool loadStream(Common::SeekableReadStream &stream);
	bool loadFourCCStream(const uint32 height,const uint32 width,const uint32 fourCC, Common::SeekableReadStream &stream);
	virtual const Surface *getSurface() const { return _surface; }
};

class DXTDecoder : public DDSDecoder {
public:
	virtual bool loadStream(Common::SeekableReadStream &stream);
};

} // End of namespace Graphics

#endif

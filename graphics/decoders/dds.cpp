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

#include "common/stream.h"
#include "common/textconsole.h"

#include "graphics/pixelformat.h"
#include "graphics/surface.h"
#include "graphics/decoders/dds.h"

#ifdef USE_S2TC
extern "C"
{
#include "txc_dxtn.h"
}
#endif

namespace Graphics {

// DDSDecoder
DDSDecoder::DDSDecoder() {
	_surface = 0;
}

DDSDecoder::~DDSDecoder() {
	destroy();
}

void DDSDecoder::destroy() {
	if (_surface) {
		_surface->free();
		delete _surface; _surface = 0;
	}
}

bool DDSDecoder::loadStream(Common::SeekableReadStream &stream) {
	uint32 header[32];

	destroy();

	if (stream.read(header, sizeof(*header) != sizeof(*header))) {
		warning("DDS: header is too short");
		return false;
	}
	if (header[0] != 0x20534444) { // "DDS "
		warning("DDS: magic is incorrect");
		return false;
	}
	if (header[1] != 124) { // header.size
		warning("DDS: size is incorrect");
		return false;
	}
	if (!(header[2] & 0x00000001)) { // header.flags & DDSD_CAPS
		warning("DDS: flag DDSD_CAPS is missing");
		return false;
	}
	if (!(header[2] & 0x00000002)) { // header.flags & DDSD_HEIGHT
		warning("DDS: flag DDSD_HEIGHT is missing");
		return false;
	}
	if (!(header[2] & 0x00000004)) { // header.flags & DDSD_WIDTH
		warning("DDS: flag DDSD_WIDTH is missing");
		return false;
	}
	if (!(header[2] & 0x00001000)) { // header.flags & DDSD_PIXELFORMAT
		warning("DDS: flag DDSD_PIXELFORMAT is missing");
		return false;
	}
	if (header[21] != 0x00000004) { // hdr.pixelFormat.flags != DDPF_FOURCC
		warning("DDS: only pixel format flag DDPF_FOURCC is supported");
		return false;
	}
	return loadFourCCStream(
	  header[3],  // hdr.height
	  header[4],  // hdr.width,
	  header[22], // hdr.pixelFormat.fourCC,
	  stream);
}

bool DDSDecoder::loadFourCCStream(const uint32 height,const uint32 width,const uint32 fourCC, Common::SeekableReadStream &stream) {
	Graphics::PixelFormat format;
	// Image is divided in 4x4 blocks
	uint32 blockCountX = (width + 3) / 4;
	uint32 blockCountY = (height + 3) / 4;

	void (*blockDecoder)(GLint srcRowStride, const GLubyte *pixdata,
	                     GLint i, GLint j, GLvoid *texel);
	uint32 blockSize;
	// format RGBA (after decoding)
	format = Graphics::PixelFormat(
		4,           // BytesPerPixel
		8, 8, 8, 8,  // {R,G,B,A}Bits
		8, 16, 24, 0 // {R,G,B,A}Shift
	);
	switch(fourCC) {
	case MKTAG('1', 'T', 'X', 'D'):
		blockDecoder = *fetch_2d_texel_rgb_dxt1; // WORKAROUND should be fetch_2d_texel_rgba_dxt1
		blockSize = 8;
		format.aLoss = 8; // no Alpha channel
		break;
	case MKTAG('3', 'T', 'X', 'D'):
		blockDecoder = *fetch_2d_texel_rgba_dxt3;
		blockSize = 16;
		break;
	case MKTAG('5', 'T', 'X', 'D'):
		blockDecoder = *fetch_2d_texel_rgba_dxt5;
		blockSize = 16;
		break;
	default:
		char fourCCStr[5];
		fourCCStr[0] = (fourCC & 0x000000ff);
		fourCCStr[1] = (fourCC & 0x0000ff00) >> 8;
		fourCCStr[2] = (fourCC & 0x00ff0000) >> 16;
		fourCCStr[3] = (fourCC & 0xff000000) >> 24;
		fourCCStr[4] = '\0';
		warning("DDS: unsupported fourCC %x (%s)", fourCC, fourCCStr);
		return false;
	}
	uint32 inputSize = blockCountX * blockCountY * blockSize;
	unsigned char *inputBuffer = (unsigned char *) malloc(inputSize);
	if (stream.read(inputBuffer, inputSize) != inputSize) {
		warning("DDS: input data is too short");
		free(inputBuffer);
		return false;
	}
	_surface = new Graphics::Surface();
	_surface->create(width, height, format);
	for(uint32 y = 0; y < height; ++y) {
		for(uint32 x = 0; x < width; ++x) {
			char color[4];
			blockDecoder(width, inputBuffer, x, y, &color);
			// RGBA -> ARGB
			char alpha = color[3];
			color[3] = color[2];
			color[2] = color[1];
			color[1] = color[0];
			color[0] = alpha;
			// */
			byte *dst = (byte *)_surface->getBasePtr(x, y);
			WRITE_UINT32(dst, *((uint32 * ) color));
		}
	}
	free(inputBuffer);
	return true;
}

bool DXTDecoder::loadStream(Common::SeekableReadStream &stream) {
	uint32 fourCC, height, width;
	unsigned char tmp[2];

	destroy();

	fourCC = stream.readUint32LE();
	width = stream.readUint32LE();
	height = stream.readUint32LE();
	stream.read(&tmp, 2);
	stream.seek(-2, SEEK_CUR);
	if ((tmp[0] == 0x1F) && (tmp[1] == 0x8B)) {
		warning("DXT: compression not supported");
		return false;
	}
	return loadFourCCStream(height, width, fourCC, stream);
}
} // End of namespace Graphics

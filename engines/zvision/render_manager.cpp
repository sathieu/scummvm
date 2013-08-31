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

#include "common/scummsys.h"

#include "common/file.h"
#include "common/system.h"
#include "common/stream.h"

#include "engines/util.h"
#include "graphics/decoders/tga.h"

#include "zvision/render_manager.h"
#include "zvision/lzss_read_stream.h"

namespace ZVision {

RenderManager::RenderManager(OSystem *system, const Common::Rect workingWindow, const Graphics::PixelFormat pixelFormat)
		: _system(system),
		  _workingWidth(workingWindow.width()),
		  _workingHeight(workingWindow.height()),
		  _screenCenterX(_workingWidth / 2),
		  _screenCenterY(_workingHeight / 2),
		  _workingWindow(workingWindow),
		  _pixelFormat(pixelFormat),
		  _backgroundWidth(0),
		  _backgroundHeight(0),
		  _backgroundInverseVelocity(0),
		  _backgroundOffset(0, 0),
		  _accumulatedVelocityMilliseconds(0),
		  _renderTable(_workingWidth, _workingHeight) {

	_workingWindowBuffer = new uint16[_workingWidth *_workingHeight];
}

RenderManager::~RenderManager() {
	delete[] _workingWindowBuffer;
}

void RenderManager::update(uint deltaTimeInMillis) {
	// An inverse velocity of 0 would be infinitely fast, so we'll let 0 mean no velocity.
	if (_backgroundInverseVelocity != 0) {
		_accumulatedVelocityMilliseconds += deltaTimeInMillis;

		uint absVelocity = uint(abs(_backgroundInverseVelocity));

		int numberOfSteps = 0;
		while (_accumulatedVelocityMilliseconds >= absVelocity) {
			_accumulatedVelocityMilliseconds -= absVelocity;
			numberOfSteps++;
		}

		// Choose the direction of movement using the sign of the velocity
		moveBackground(_backgroundInverseVelocity < 0 ? -numberOfSteps : numberOfSteps);
	}
}

void RenderManager::clearWorkingWindowToColor(uint16 color) {
	uint32 workingWindowSize = _workingWidth * _workingHeight;

	for (uint32 i = 0; i < workingWindowSize; i++) {
		_workingWindowBuffer[i] = color;
	}

	_system->copyRectToScreen(_workingWindowBuffer, _workingWidth * sizeof(uint16), _workingWindow.left, _workingWindow.top, _workingWidth, _workingHeight);
}

void RenderManager::renderSubRectToScreen(Graphics::Surface &surface, int16 destinationX, int16 destinationY, bool wrap) {	
	int16 subRectX = 0;
	int16 subRectY = 0;

	// Take care of negative destinations
	if (destinationX < 0) {
		subRectX = -destinationX;
		destinationX = 0;
	} else if (destinationX >= surface.w) {
		// Take care of extreme positive destinations
		destinationX -= surface.w;
	}

	// Take care of negative destinations
	if (destinationY < 0) {
		subRectY = -destinationY;
		destinationY = 0;
	} else if (destinationY >= surface.h) {
		// Take care of extreme positive destinations
		destinationY -= surface.h;
	}

	if (wrap) {
		_backgroundWidth = surface.w;
		_backgroundHeight = surface.h;
	
		if (destinationX > 0) { 
			// Move destinationX to 0
			subRectX = surface.w - destinationX;
			destinationX = 0;
		}

		if (destinationY > 0) {
			// Move destinationY to 0
			subRectY = surface.h - destinationY;
			destinationY = 0;
		}
	}

	// Clip subRect to working window bounds
	Common::Rect subRect(subRectX, subRectY, subRectX + _workingWidth, subRectY + _workingHeight);

	if (!wrap) {
		// Clip to image bounds
		subRect.clip(surface.w, surface.h);
	}

	// Check destRect for validity
	if (!subRect.isValidRect() || subRect.isEmpty())
		return;

	if (_renderTable.getRenderState() == RenderTable::FLAT) {
		_system->copyRectToScreen(surface.getBasePtr(subRect.left, subRect.top), surface.pitch, destinationX + _workingWindow.left, destinationY + _workingWindow.top, subRect.width(), subRect.height());
	} else {
		_renderTable.mutateImage((uint16 *)surface.getPixels(), _workingWindowBuffer, surface.w, surface.h, destinationX, destinationY, subRect, wrap);

		_system->copyRectToScreen(_workingWindowBuffer, _workingWidth * sizeof(uint16), destinationX + _workingWindow.left, destinationY + _workingWindow.top, subRect.width(), subRect.height());
	}
}

void RenderManager::renderImageToScreen(const Common::String &fileName, int16 destinationX, int16 destinationY, bool wrap) {
	Graphics::Surface surface;
	readImageToSurface(fileName, surface);

	renderSubRectToScreen(surface, destinationX, destinationY, wrap);
}

void RenderManager::renderImageToScreen(Graphics::Surface &surface, int16 destinationX, int16 destinationY, bool wrap) {
	renderSubRectToScreen(surface, destinationX, destinationY, wrap);
}

void RenderManager::readImageToSurface(const Common::String &fileName, Graphics::Surface &destination) {
	Common::File file;

	if (!file.open(fileName)) {
		warning("Could not open file %s", fileName.c_str());
		return;
	}

	// Read the magic number
	// Some files are true TGA, while others are TGZ
	uint32 fileType = file.readUint32BE();

	uint32 imageWidth;
	uint32 imageHeight;
	Graphics::TGADecoder tga;
	uint16 *buffer;
	bool isTransposed = _renderTable.getRenderState() == RenderTable::PANORAMA;
	// All ZVision images are in RGB 555
	Graphics::PixelFormat pixelFormat555 = Graphics::PixelFormat(2, 5, 5, 5, 0, 10, 5, 0, 0);
	destination.format = pixelFormat555;

	bool isTGZ;

	// Check for TGZ files
	if (fileType == MKTAG('T', 'G', 'Z', '\0')) {
		isTGZ = true;

		// TGZ files have a header and then Bitmap data that is compressed with LZSS
		uint32 decompressedSize = file.readSint32LE();
		imageWidth = file.readSint32LE();
		imageHeight = file.readSint32LE();

		LzssReadStream lzssStream(&file);
		buffer = (uint16 *)(new uint16[decompressedSize]);
		lzssStream.read(buffer, decompressedSize);
	} else {
		isTGZ = false;

		// Reset the cursor
		file.seek(0);

		// Decode
		if (!tga.loadStream(file)) {
			warning("Error while reading TGA image");
			return;
		}

		Graphics::Surface tgaSurface = *(tga.getSurface());
		imageWidth = tgaSurface.w;
		imageHeight = tgaSurface.h;

		buffer = (uint16 *)tgaSurface.getPixels();
	}

	// Flip the width and height if transposed
	if (isTransposed) {
		uint16 temp = imageHeight;
		imageHeight = imageWidth;
		imageWidth = temp;
	}

	// If the destination internal buffer is the same size as what we're copying into it,
	// there is no need to free() and re-create
	if (imageWidth != destination.w || imageHeight != destination.h) {
		destination.create(imageWidth, imageHeight, pixelFormat555);
	}

	// If transposed, 'un-transpose' the data while copying it to the destination
	// Otherwise, just do a simple copy
	if (isTransposed) {
		uint16 *dest = (uint16 *)destination.getPixels();

		for (uint32 y = 0; y < imageHeight; y++) {
			uint32 columnIndex = y * imageWidth;

			for (uint32 x = 0; x < imageWidth; x++) {
				dest[columnIndex + x] = buffer[x * imageHeight + y];
			}
		}
	} else {
		memcpy(destination.getPixels(), buffer, imageWidth * imageHeight * _pixelFormat.bytesPerPixel);
	}

	// Cleanup
	if (isTGZ) {
		delete[] buffer;
	} else {
		tga.destroy();
	}

	// Convert in place to RGB 565 from RGB 555
	destination.convertToInPlace(_pixelFormat);
}

const Common::Point RenderManager::screenSpaceToImageSpace(const Common::Point &point) {
	// Convert from screen space to working window space
	Common::Point newPoint(point - Common::Point(_workingWindow.left, _workingWindow.top));

	RenderTable::RenderState state = _renderTable.getRenderState();
	if (state == RenderTable::PANORAMA || state == RenderTable::TILT) {
		newPoint = _renderTable.convertWarpedCoordToFlatCoord(newPoint);
	}

	if (state == RenderTable::PANORAMA) {
		newPoint -= (Common::Point(_screenCenterX, 0) - _backgroundOffset);
	} else if (state == RenderTable::TILT) {
		newPoint -= (Common::Point(0, _screenCenterY) - _backgroundOffset);
	}

	if (newPoint.x < 0)
		newPoint.x += _backgroundWidth;
	else if (newPoint.x >= _backgroundWidth)
		newPoint.x -= _backgroundWidth;
	if (newPoint.y < 0)
		newPoint.y += _backgroundHeight;
	else if (newPoint.y >= _backgroundHeight)
		newPoint.y -= _backgroundHeight;

	return newPoint;
}

RenderTable *RenderManager::getRenderTable() {
	return &_renderTable;
}

void RenderManager::setBackgroundImage(const Common::String &fileName) {
	Common::File *file = new Common::File;

	if (!file->open(fileName)) {
		warning("Could not open file %s", fileName.c_str());
		return;
	}

	readImageToSurface(fileName, _currentBackground);

	moveBackground(0);
}

void RenderManager::setBackgroundPosition(int offset) {
	RenderTable::RenderState state = _renderTable.getRenderState();
	if (state == RenderTable::TILT) {
		_backgroundOffset.x = 0;
		_backgroundOffset.y = offset;
	} else if (state == RenderTable::PANORAMA) {
		_backgroundOffset.x = offset;
		_backgroundOffset.y = 0;
	} else {
		_backgroundOffset.x = 0;
		_backgroundOffset.y = 0;
	}
}

void RenderManager::setBackgroundVelocity(int velocity) {
	// setBackgroundVelocity(0) will be called quite often, so make sure
	// _backgroundInverseVelocity isn't already 0 to prevent an extraneous assignment
	if (velocity == 0) {
		if (_backgroundInverseVelocity != 0) {
			_backgroundInverseVelocity = 0;
		}
	} else {
		_backgroundInverseVelocity = 1000 / velocity;
	}
}

void RenderManager::moveBackground(int offset) {
	RenderTable::RenderState state = _renderTable.getRenderState();
	if (state == RenderTable::TILT) {
		_backgroundOffset += Common::Point(0, offset);

		_backgroundOffset.y = CLIP<int16>(_backgroundOffset.y, _screenCenterY, (int16)_backgroundHeight - _screenCenterY);

		renderImageToScreen(_currentBackground, 0, _screenCenterY - _backgroundOffset.y, true);
	} else if (state == RenderTable::PANORAMA) {
		_backgroundOffset += Common::Point(offset, 0);

		if (_backgroundOffset.x <= -_backgroundWidth)
			_backgroundOffset.x += _backgroundWidth;
		else if (_backgroundOffset.x >= _backgroundWidth)
			_backgroundOffset.x -= _backgroundWidth;

		renderImageToScreen(_currentBackground, _screenCenterX - _backgroundOffset.x, 0, true);
	} else {
		renderImageToScreen(_currentBackground, 0, 0);
	}
}

} // End of namespace ZVision

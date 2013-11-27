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
 *
 */

#ifdef ENABLE_SCUMM_SE

#include <string.h>
#include "scumm/se/file_pak.h"
#include "common/debug.h"
#include "common/endian.h"
#include "common/md5.h"
#include "common/memstream.h"

namespace Scumm {

#pragma mark -
#pragma mark --- ScummPakFile ---
#pragma mark -

#include "common/pack-start.h"  // START STRUCT PACKING

struct ScummPakFile::Header {
	char magic[4]; // KAPL or LPAK
	uint32 version;
	uint32 indexOffset;
	uint32 fileEntriesOffset;
	uint32 fileNamesOffset;
	uint32 dataOffset;
	uint32 indexLength;
	uint32 fileEntriesLength;
	uint32 fileNamesLength;
	uint32 dataLength;
} PACKED_STRUCT;

struct ScummPakFile::FileEntry {
	uint32 dataOffset;
	uint32 fileNamesOffset;
	uint32 dataSize;
	uint32 realSize;
	uint32 compressed;
} PACKED_STRUCT;

#include "common/pack-end.h"    // END STRUCT PACKING

PakFileItem::PakFileItem(const char *f, const uint32 o, const uint32 s) : fileName(), offset(0), size(0) {
	fileName = Common::String(f);
	offset = o;
	size = s;
}

ScummPakFile::ScummPakFile() : _files(), _currentFile(), _myEos(0), classicPrefix("classic/en/") {
}

bool ScummPakFile::open(const Common::String &filename) {
	if (File::open(filename)) {
		return _generateIndex();
	} else {
		return false;
	}
}

bool ScummPakFile::seek(int32 offs, int whence) {
	switch (whence) {
	case SEEK_END:
		offs = _currentFile.offset + _currentFile.size + offs;
		break;
	case SEEK_SET:
		offs += _currentFile.offset;
		break;
	case SEEK_CUR:
		offs += File::pos();
		break;
	}
	whence = SEEK_SET;
	bool ret = File::seek(offs, whence);
	if (ret)
		_myEos = false;
	return ret;
}

bool ScummPakFile::_generateIndex() {
	struct ScummPakFile::Header rawHeader, header;
	uint32 fileCount = 0, currentFileNameOffset = 0;

	assert(isOpen());
	assert((uint32) File::size() >= sizeof(ScummPakFile::Header));

	File::seek(0, SEEK_SET);
	File::read(&rawHeader, sizeof(rawHeader));
	memset(&header, 0, sizeof(header));

	assert(!strncmp(rawHeader.magic, "KAPL", 4) || !strncmp(rawHeader.magic, "LPAK", 4));

	if (!strncmp(rawHeader.magic, "KAPL", 4)) { // LittleEndian
		// char magic[4];
		header.version = FROM_LE_32(rawHeader.version);
		header.indexOffset = FROM_LE_32(rawHeader.indexOffset);
		header.fileEntriesOffset = FROM_LE_32(rawHeader.fileEntriesOffset);
		header.fileNamesOffset = FROM_LE_32(rawHeader.fileNamesOffset);
		header.dataOffset = FROM_LE_32(rawHeader.dataOffset);
		header.indexLength = FROM_LE_32(rawHeader.indexLength);
		header.fileEntriesLength = FROM_LE_32(rawHeader.fileEntriesLength);
		header.fileNamesLength = FROM_LE_32(rawHeader.fileNamesLength);
		header.dataLength = FROM_LE_32(rawHeader.dataLength);
	} else if (!strncmp(rawHeader.magic, "LPAK", 4)) { // BigEndian
		// char magic[4];
		header.version = FROM_BE_32(rawHeader.version);
		header.indexOffset = FROM_BE_32(rawHeader.indexOffset);
		header.fileEntriesOffset = FROM_BE_32(rawHeader.fileEntriesOffset);
		header.fileNamesOffset = FROM_BE_32(rawHeader.fileNamesOffset);
		header.dataOffset = FROM_BE_32(rawHeader.dataOffset);
		header.indexLength = FROM_BE_32(rawHeader.indexLength);
		header.fileEntriesLength = FROM_BE_32(rawHeader.fileEntriesLength);
		header.fileNamesLength = FROM_BE_32(rawHeader.fileNamesLength);
		header.dataLength = FROM_BE_32(rawHeader.dataLength);
	}
	fileCount = header.fileEntriesLength / 20;
	assert((uint32) File::size() >= header.fileEntriesOffset + 20 * fileCount);

	for (uint32 i = 0; i < fileCount; i++) {
		struct ScummPakFile::FileEntry fentry;
		char fname[255];
		File::seek(header.fileEntriesOffset + 20 * i, SEEK_SET);
		File::read(&fentry, sizeof(fentry));
		File::seek(header.fileNamesOffset + currentFileNameOffset);
		File::read(&fname, sizeof(fname));
		currentFileNameOffset += strlen(fname)+1;
		PakFileItem f(fname, header.dataOffset + fentry.dataOffset, fentry.dataSize);
		_files[fname] = f;
	}
	return true;
}

bool ScummPakFile::openSubFile(const Common::String &filename) {
	assert(isOpen());
	_currentFileIsClassic = false;
	if (_files.contains(filename)) {
		_currentFile = _files[filename];
		seek(0, SEEK_SET);
		return true;
	} else if (_files.contains(classicPrefix + filename)) {
		_currentFileIsClassic = true;
		_currentFile = _files[classicPrefix + filename];
		seek(0, SEEK_SET);
		return true;
	} else {
		return false;
	}
}

void ScummPakFile::close() {
	File::close();
}

uint32 ScummPakFile::read(void *dataPtr, uint32 dataSize) {
	uint32 realLen;

	if (_currentFile.size) {
		// Limit the amount we read by the subfile boundaries.
		const int32 curPos = pos();
		assert(_currentFile.size >= (uint32) curPos);
		int32 newPos = curPos + dataSize;
		if ((uint32) newPos > _currentFile.size) {
			dataSize = _currentFile.size - curPos;
			_myEos = true;
		}
	}

	realLen = File::read(dataPtr, dataSize);


	// If an encryption byte was specified, XOR the data we just read by it.
	// This simple kind of "encryption" was used by some of the older SCUMM
	// games.
	if (_currentFileIsClassic && _encbyte) {
		byte *p = (byte *)dataPtr;
		byte *end = p + realLen;
		while (p < end)
			*p++ ^= _encbyte;
	}

	return realLen;
}

} // End of namespace Scumm

#endif

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

#ifndef SCUMM_FILE_PACK_H
#define SCUMM_FILE_PACK_H

#ifdef ENABLE_SCUMM_SE

#include "common/hashmap.h"
#include "common/str.h"
#include "scumm/file.h"

namespace Scumm {

class PakFileItem {
public:
	Common::String fileName;
	uint32 offset;
	uint32 size;
public:
	PakFileItem() {};
	PakFileItem(const char *f, const uint32 o, const uint32 s);
};

typedef Common::HashMap<Common::String, PakFileItem, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> FileMap;

class ScummPakFile : public BaseScummFile {
public:
	struct Header;
	struct FileEntry;
private:
	FileMap _files;
	PakFileItem _currentFile;
	bool _currentFileIsClassic;
	bool _myEos;
	bool _generateIndex();
public:
	Common::String classicPrefix;

	ScummPakFile();

	bool open(const Common::String &filename);
	bool openSubFile(const Common::String &filename);

	void close();
	bool eos() const { return _currentFile.size ? _myEos : File::eos();; }
	int32 pos() const { return File::pos() - _currentFile.offset; }
	int32 size() const { return _currentFile.size ? _currentFile.size : File::size(); }
	bool seek(int32 offs, int whence = SEEK_SET);
	uint32 read(void *dataPtr, uint32 dataSize);
};

} // End of namespace Scumm

#endif

#endif

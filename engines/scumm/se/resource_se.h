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

#ifndef SCUMM_RESOURCE_SE_H
#define SCUMM_RESOURCE_SE_H

namespace Scumm {

#ifdef ENABLE_SCUMM_SE

class ScummEngine_se;

/**
 * The 'resource manager' Special edition class.
 */
class ResourceManager_se {
protected:
	ScummEngine_se *_vm;
	BaseScummFile *_fileHandle;
	// From the index
	struct fileInfo {
		uint32 flags;
		uint32 nameAddress;
		Common::String name;
	};
	/*
	struct unknown {
		uint32 unknown[70];
	};
	*/
	uint32 _roomCount;
	uint32 _roomHeaderAddress;
	uint32 _costumeCount;
	uint32 _costumeHeaderAddress;
	uint32 _uiCount;
	uint32 _uiHeaderAddress;
	uint32 _unknownCount;
	uint32 _unknownAddress;
	Common::Array<fileInfo> _roomList;
	Common::Array<fileInfo> _uiList;
	Common::Array<fileInfo> _costumeList;
	//Common::Array<unknown> _unknownList;
public:
	// Room
	class Room {
	protected:
		struct staticSpriteHeader {
			uint32 identifier;
			uint32 unknown1;
			uint32 unknown2;
			uint32 staticSpriteCount;
			uint32 staticSpriteAddress;
		};
		struct spriteHeader {
			uint32 identifier;
			uint32 spriteCount;
			uint32 spriteAddress;
		};
		struct unknown6Header {
			uint32 unknown1;
			uint32 unknown2;
			uint32 unknown6Size;
			uint32 unknown6Address;
			byte *data;
		};
		struct objectHeader {
			uint32 objectNameAddress;
			uint32 objectCount;
			uint32 objectAddress;
			Common::String objectName;
		};
		struct unknown5Header {
			uint32 unknown5Size;
			uint32 unknown5Address;
			byte *data;
		};
		struct staticSprite {
			uint32 x;
			uint32 y;
			uint32 width;
			uint32 height;
			uint32 textureFileNameAddress;
			Common::String textureFileName;
		};
		struct sprite {
			uint32 textureFileNameAddress;
			uint32 textureX;
			uint32 textureY;
			uint32 textureWidth;
			uint32 textureHeight;
			float offsetX;
			float offsetY;
			uint32 layer;
			Common::String textureFileName;
		};
		struct object {
			uint32 objectAddress1;
			uint32 objectAddress2;
			uint32 unknown3;
			uint32 unknown4;
			// objectAddress1
			uint32 filenameAddress1;
			uint32 x;
			uint32 y;
			uint32 width;
			uint32 height;
			Common::String filename1;
			// objectAddress2
			uint32 unknown6[2];
			uint32 extraSpriteCount;
			uint32 extraSpriteAddress;
		};
		ResourceManager_se *_resSE;
		Common::String _roomFile;

		uint32 _identifier;
		uint32 _nameAddress;
		uint32 _roomWidth;
		uint32 _roomHeight;
		uint32 _staticSpriteHeaderCount;
		uint32 _staticSpriteHeaderAddress;
		uint32 _spriteHeaderCount;
		uint32 _spriteHeaderAddress;
		uint32 _unkn09;
		uint32 _unkn10;
		uint32 _unknown6HeaderAddress1;
		uint32 _unknown6HeaderCount;
		uint32 _unknown6HeaderAddress2;
		uint32 _objectHeaderCount;
		uint32 _objectHeaderAddress;
		uint32 _unknown5HeaderCount;
		uint32 _unknown5HeaderAddress;
		uint32 _alwaysZero1;
		uint32 _alwaysZero2;
		uint32 _alwaysZero3;
		Common::String _name;
		Common::Array<staticSpriteHeader> _staticSpriteHeaderList;
		Common::Array<spriteHeader> _spriteHeaderList;
		Common::Array<unknown6Header> _unknown6HeaderList;
		Common::Array<objectHeader> _objectHeaderList;
		Common::Array<unknown5Header> _unknown5HeaderList;
		Common::Array< Common::Array<staticSprite> > _staticSpriteList;
		Common::Array< Common::Array<sprite> > _spriteList;
		Common::Array< Common::Array<object> > _objectList;
		Common::Array< Common::Array<staticSprite> > _extraSpriteList;
	public:
		Room(ResourceManager_se *resSE, const Common::String &roomFile);
		const Common::String getRoomFile() { return _roomFile; }
	};
	friend class Room;
	// Costume
	class Costume {
	};
protected:
	Common::List<Room> _roomCache;
	Common::List<Costume> _costumeCache;
public:
	ResourceManager_se(ScummEngine_se *vm);
	~ResourceManager_se();
protected:
	bool openFile(const Common::String &filename);
public:
	void readIndexFile();
	Room *getRoom(const Common::String &roomFile);
	Room *getRoom(const uint32 roomNumber);
	Costume *getCostume(const Common::String &costumeFile);
};
#endif

} // End of namespace Scumm

#endif

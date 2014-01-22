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

#include "scumm/file.h"
#include "scumm/scumm_v4.h"
#include "scumm/resource.h"
#include "scumm/se/scumm_se.h"
#include "scumm/se/file_pak.h"
#include "scumm/se/resource_se.h"

namespace Scumm {

#ifdef ENABLE_SCUMM_SE


// ScummEngine_se
void ScummEngine_se::readIndexFile() {
	// Read classic game index
	this->ScummEngine::readIndexFile();

	// Read Special Edition index
	_resSE->readIndexFile();
}

// ResourceManager_se
ResourceManager_se::ResourceManager_se(ScummEngine_se *vm) : _vm(vm), _fileHandle(0), _roomList(), _uiList(), _costumeList() {
	_fileHandle = new ScummPakFile();
}

ResourceManager_se::~ResourceManager_se() {
	_fileHandle->close();
	delete _fileHandle;
}

void ResourceManager_se::readIndexFile() {
	const char *SEIndexFile;
	if (_vm->_game.id == GID_MONKEY)
		SEIndexFile = "monkey1_retail.scumm.xml";
	else if (_vm->_game.id == GID_MONKEY2)
		SEIndexFile = "monkey2_retail.scumm.xml";
	else
		error("Unsupported game id '%i'", _vm->_game.id);
	if (!_fileHandle->open(_vm->_containerFile))
		error("Couldn't open container file '%s'", _vm->_containerFile.c_str());
	if (!_fileHandle->openSubFile(SEIndexFile))
		error("Could not find Special Edition index %s", SEIndexFile);

	_roomCount = _fileHandle->readUint32LE();
	_roomHeaderAddress = _fileHandle->readAbsolutePositionUint32LE();
	_costumeCount = _fileHandle->readUint32LE();
	_costumeHeaderAddress = _fileHandle->readAbsolutePositionUint32LE();
	_uiCount = _fileHandle->readUint32LE();
	_uiHeaderAddress = _fileHandle->readAbsolutePositionUint32LE();
	_unknownCount = _fileHandle->readUint32LE();
	_unknownAddress = _fileHandle->readAbsolutePositionUint32LE();

	_fileHandle->seek(_roomHeaderAddress, SEEK_SET);
	for(uint32 index = 0; index < _roomCount; index++) {
		struct fileInfo fi;
		fi.flags = _fileHandle->readUint32LE();
		fi.nameAddress = _fileHandle->readAbsolutePositionUint32LE();
		_roomList.push_back(fi);
	}
	_fileHandle->seek(_costumeHeaderAddress, SEEK_SET);
	for(uint32 index = 0; index < _costumeCount; index++) {
		struct fileInfo fi;
		fi.flags = _fileHandle->readUint32LE();
		fi.nameAddress = _fileHandle->readAbsolutePositionUint32LE();
		_costumeList.push_back(fi);
	}
	_fileHandle->seek(_uiHeaderAddress, SEEK_SET);
	for(uint32 index = 0; index < _uiCount; index++) {
		struct fileInfo fi;
		fi.flags = _fileHandle->readUint32LE();
		fi.nameAddress = _fileHandle->readAbsolutePositionUint32LE();
		_uiList.push_back(fi);
	}
	/*
	// FIXME: undestand this part of the file
	_fileHandle->seek(_unknownAddress, SEEK_SET);
	for(uint32 index = 0; index < _unknownCount; index++) {
		struct unknown u;
		for(uint32 index2 = 0; index2 < 70; index2++) {
			u.unknown[index2] = _fileHandle->readUint32LE();
		}
		_unknownList.push_back(u);
	}
	*/

	for(uint32 index = 0; index < _roomCount; index++) {
		struct fileInfo *fi = &_roomList[index];
		if (fi->nameAddress == 0)
			continue;
		_fileHandle->seek(fi->nameAddress, SEEK_SET);
		fi->name = _fileHandle->readPadded16String();
	}
	for(uint32 index = 0; index < _costumeCount; index++) {
		struct fileInfo *fi = &_costumeList[index];
		if (fi->nameAddress == 0)
			continue;
		_fileHandle->seek(fi->nameAddress, SEEK_SET);
		fi->name = _fileHandle->readPadded16String();
	}
	for(uint32 index = 0; index < _uiCount; index++) {
		struct fileInfo *fi = &_uiList[index];
		if (fi->nameAddress == 0)
			continue;
		_fileHandle->seek(fi->nameAddress, SEEK_SET);
		fi->name = _fileHandle->readPadded16String();
	}
}

ResourceManager_se::Room *ResourceManager_se::getRoom(const Common::String &roomFile) {
	for (Common::List<Room>::iterator
			x = _roomCache.begin(); x != _roomCache.end(); ++x) {
		if (x->getRoomFile().equals(roomFile))
			return &*x;
	}
	ResourceManager_se::Room *room = new ResourceManager_se::Room(this, roomFile);
	_roomCache.push_back(*room);
	return room;
}

ResourceManager_se::Room *ResourceManager_se::getRoom(const uint32 roomNumber) {
	if (roomNumber >= _roomCount) {
		error("Unable to get out of bound SE room");
	}
	return getRoom(_roomList[roomNumber].name);
}

// ResourceManager_se::Room

/*
 * This function is based on Monkey Island SE Inspector
 * by Robin Theilade (License: Public Domain).
 *
 * Original code at:
 * http://sourceforge.net/p/mi1sexmlparser/code/31/tree/trunk/MonkeyIsland1SpecialEditionXmlParser/Formats/Rooms/Parser.cs
 *
 * Further enhancements have been done.
 */
ResourceManager_se::Room::Room(ResourceManager_se *resSE, const Common::String &roomFile)
	: _resSE(resSE), _roomFile(roomFile),
	_staticSpriteHeaderList(), _spriteHeaderList(),
	_unknown6HeaderList(), _objectHeaderList(), _unknown5HeaderList(),
	_staticSpriteList(), _spriteList(), _objectList(), _extraSpriteList(),
	_spritesLoaded(false)
	{
	if (!_resSE->_fileHandle->openSubFile(roomFile))
		error("Could not find file %s in container", roomFile.c_str());
	// read header
	_identifier = _resSE->_fileHandle->readUint32LE();
	_nameAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	_roomWidth = _resSE->_fileHandle->readUint32LE();
	_roomHeight = _resSE->_fileHandle->readUint32LE();
	_staticSpriteHeaderCount = _resSE->_fileHandle->readUint32LE();
	_staticSpriteHeaderAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	_spriteHeaderCount = _resSE->_fileHandle->readUint32LE();
	_spriteHeaderAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	_unkn09 = _resSE->_fileHandle->readUint32LE();
	_unkn10 = _resSE->_fileHandle->readUint32LE();
	_unknown6HeaderAddress1 = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	_unknown6HeaderCount = _resSE->_fileHandle->readUint32LE();
	_unknown6HeaderAddress2 = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	_objectHeaderCount = _resSE->_fileHandle->readUint32LE();
	_objectHeaderAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	_unknown5HeaderCount = _resSE->_fileHandle->readUint32LE();
	_unknown5HeaderAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	_alwaysZero1 = _resSE->_fileHandle->readUint32LE();
	_alwaysZero2 = _resSE->_fileHandle->readUint32LE();
	_alwaysZero3 = _resSE->_fileHandle->readUint32LE();
	//_resSE->_fileHandle->seek(_nameAddress, SEEK_SET);
	_name = _resSE->_fileHandle->readPadded16String();

	// read static sprite header list
	_resSE->_fileHandle->seek(_staticSpriteHeaderAddress, SEEK_SET);
	_staticSpriteHeaderList.reserve(_staticSpriteHeaderCount);
	for(uint32 index = 0; index < _staticSpriteHeaderCount; index++) {
		struct staticSpriteHeader h;
		h.identifier = _resSE->_fileHandle->readUint32LE();
		h.unknown1 = _resSE->_fileHandle->readUint32LE();
		h.unknown2 = _resSE->_fileHandle->readUint32LE();
		h.staticSpriteCount = _resSE->_fileHandle->readUint32LE();
		h.staticSpriteAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
		_staticSpriteHeaderList.push_back(h);
	}
	// read sprite header list
	_resSE->_fileHandle->seek(_spriteHeaderAddress, SEEK_SET);
	_spriteHeaderList.reserve(_spriteHeaderCount);
	for(uint32 index = 0; index < _spriteHeaderCount; index++) {
		struct spriteHeader h;
		h.identifier = _resSE->_fileHandle->readUint32LE();
		h.spriteCount = _resSE->_fileHandle->readUint32LE();
		h.spriteAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
		_spriteHeaderList.push_back(h);
	}
	// read unknown6 header list
	_resSE->_fileHandle->seek(_unknown6HeaderAddress2, SEEK_SET);
	_unknown6HeaderList.reserve(_unknown6HeaderCount);
	for(uint32 index = 0; index < _unknown6HeaderCount; index++ ) {
		struct unknown6Header h;
		h.unknown1 = _resSE->_fileHandle->readUint32LE();
		h.unknown2 = _resSE->_fileHandle->readUint32LE();
		h.unknown6Size = _resSE->_fileHandle->readUint32LE();
		h.unknown6Address = _resSE->_fileHandle->readAbsolutePositionUint32LE();
		h.data = 0;
		_unknown6HeaderList.push_back(h);
	}
	// read object header list
	_resSE->_fileHandle->seek(_objectHeaderAddress, SEEK_SET);
	_objectHeaderList.reserve(_objectHeaderCount);
	for(uint32 index = 0; index < _objectHeaderCount; index++ ) {
		struct objectHeader h;
		h.objectNameAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
		h.objectCount = _resSE->_fileHandle->readUint32LE();
		h.objectAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
		_objectHeaderList.push_back(h);
	}
	// read unknown5 header list
	_resSE->_fileHandle->seek(_unknown5HeaderAddress, SEEK_SET);
	_unknown5HeaderList.reserve(_unknown5HeaderCount);
	for(uint32 index = 0; index < _unknown5HeaderCount; index++ ) {
		struct unknown5Header h;
		h.unknown5Size = _resSE->_fileHandle->readUint32LE();
		h.unknown5Address = _resSE->_fileHandle->readAbsolutePositionUint32LE();
		_unknown5HeaderList.push_back(h);
	}
	// read static sprite list
	_staticSpriteList.reserve(_staticSpriteHeaderList.size());
	for(uint32 index = 0; index < _staticSpriteHeaderList.size(); index++ ) {
		struct staticSpriteHeader *ssh = &_staticSpriteHeaderList[index];
		Common::Array<staticSprite> innerStaticSpriteList;
		innerStaticSpriteList.reserve(ssh->staticSpriteCount);
		_resSE->_fileHandle->seek(ssh->staticSpriteAddress, SEEK_SET);
		for(uint32 index2 = 0; index2 < ssh->staticSpriteCount; index2++) {
			struct staticSprite s;
			s.x = _resSE->_fileHandle->readUint32LE();
			s.y = _resSE->_fileHandle->readUint32LE();
			s.width = _resSE->_fileHandle->readUint32LE();
			s.height = _resSE->_fileHandle->readUint32LE();
			s.textureFileNameAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
			s.textureFileName.clear();
			innerStaticSpriteList.push_back(s);
		}
		_staticSpriteList.push_back(innerStaticSpriteList);
	}
	for(uint32 index = 0; index < _staticSpriteList.size(); index++ ) {
		Common::Array<staticSprite> *innerStaticSpriteList = &_staticSpriteList[index];
		for(uint32 index2 = 0; index2 < innerStaticSpriteList->size(); index2++ ) {
			struct staticSprite *s = &(*innerStaticSpriteList)[index2];
			_resSE->_fileHandle->seek(s->textureFileNameAddress, SEEK_SET);
			s->textureFileName = _resSE->_fileHandle->readPadded16String();
		}
	}
	// read sprite list
	_spriteList.reserve(_spriteHeaderList.size());
	for(uint32 index = 0; index < _spriteHeaderList.size(); index++) {
		struct spriteHeader *sh = &_spriteHeaderList[index];
		Common::Array<sprite> innerSpriteList;
		innerSpriteList.reserve(sh->spriteCount);
		_resSE->_fileHandle->seek(sh->spriteAddress, SEEK_SET);
		for(uint32 index2 = 0; index2 < sh->spriteCount; index2++ ) {
			struct sprite s;
			s.textureFileNameAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
			s.textureX = _resSE->_fileHandle->readUint32LE();
			s.textureY = _resSE->_fileHandle->readUint32LE();
			s.textureWidth = _resSE->_fileHandle->readUint32LE();
			s.textureHeight = _resSE->_fileHandle->readUint32LE();
			_resSE->_fileHandle->read(&s.offsetX, 4);
			_resSE->_fileHandle->read(&s.offsetY, 4);
			s.layer = _resSE->_fileHandle->readUint32LE();
			s.textureFileName.clear();
			innerSpriteList.push_back(s);
		}
		_spriteList.push_back(innerSpriteList);
	}
	for(uint32 index = 0; index < _spriteList.size(); index++ ) {
		Common::Array<sprite> *innerSpriteList = &_spriteList[index];
		for(uint32 index2 = 0; index2 < innerSpriteList->size(); index2++ ) {
			struct sprite *s = &(*innerSpriteList)[index2];
			_resSE->_fileHandle->seek(s->textureFileNameAddress, SEEK_SET);
			s->textureFileName = _resSE->_fileHandle->readPadded16String();
		}
	}
	// read unknown 6 list
	for(uint32 index = 0; index < _unknown6HeaderList.size(); index++ ) {
		struct unknown6Header *h = &_unknown6HeaderList[index];
		_resSE->_fileHandle->seek(h->unknown6Address, SEEK_SET);
		h->data = (byte *)malloc(h->unknown6Size);
		_resSE->_fileHandle->read(h->data, h->unknown6Size);
	}
	// read object group list
	_objectList.reserve(_objectHeaderList.size());
	for(uint32 index = 0; index < _objectHeaderList.size(); index++) {
		struct objectHeader *objH = &_objectHeaderList[index];
		_resSE->_fileHandle->seek(objH->objectNameAddress, SEEK_SET);
		objH->objectName = _resSE->_fileHandle->readPadded16String();
		Common::Array<object> innerObjectList;
		innerObjectList.reserve(objH->objectCount);
		_resSE->_fileHandle->seek(objH->objectAddress, SEEK_SET);
		for(uint32 index2 = 0; index2 < objH->objectCount; index2++ ) {
			struct object obj;
			memset(&obj, 0, sizeof(obj));
			obj.objectAddress1 = _resSE->_fileHandle->readAbsolutePositionUint32LE();
			obj.objectAddress2 = _resSE->_fileHandle->readAbsolutePositionUint32LE();
			obj.unknown3 = _resSE->_fileHandle->readUint32LE();
			obj.unknown4 = _resSE->_fileHandle->readUint32LE();
			obj.filename1.clear();
			innerObjectList.push_back(obj);
		}
		_objectList.push_back(innerObjectList);
	}
	// read object_x entities
	for(uint32 index = 0; index < _objectList.size(); index++) {
		Common::Array<object> *innerObjectList = &_objectList[index];
		for(uint32 index2 = 0; index2 < innerObjectList->size(); index2++ ) {
			struct object *obj = &(*innerObjectList)[index2];
			if(obj->objectAddress1 > 0 ) {
				_resSE->_fileHandle->seek(obj->objectAddress1, SEEK_SET);
				obj->filenameAddress1 = _resSE->_fileHandle->readAbsolutePositionUint32LE();
				obj->x = _resSE->_fileHandle->readUint32LE();
				obj->y = _resSE->_fileHandle->readUint32LE();
				obj->width = _resSE->_fileHandle->readUint32LE();
				obj->height = _resSE->_fileHandle->readUint32LE();
				_resSE->_fileHandle->seek(obj->filenameAddress1, SEEK_SET);
				obj->filename1 = _resSE->_fileHandle->readPadded16String();
			}
			if(obj->objectAddress2 > 0 ) {
				_resSE->_fileHandle->seek(obj->objectAddress2, SEEK_SET);
				obj->unknown6[0] = _resSE->_fileHandle->readUint32LE();
				obj->unknown6[1] = _resSE->_fileHandle->readUint32LE();
				obj->extraSpriteCount = _resSE->_fileHandle->readUint32LE();
				obj->extraSpriteAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
			}
		}
	}
	for(uint32 index = 0; index < _objectList.size(); index++) {
		Common::Array<object> *innerObjectList = &_objectList[index];
		for(uint32 index2 = 0; index2 < innerObjectList->size(); index2++ ) {
			struct object *obj = &(*innerObjectList)[index2];
			if(obj->extraSpriteAddress > 0 ) {
				Common::Array<staticSprite> innerExtraSpriteList;
				innerExtraSpriteList.reserve(obj->extraSpriteCount);
				_resSE->_fileHandle->seek(obj->extraSpriteAddress, SEEK_SET);
				for(uint32 index3 = 0; index3 < obj->extraSpriteCount; index3++) {
					struct staticSprite s;
					s.x = _resSE->_fileHandle->readUint32LE();
					s.y = _resSE->_fileHandle->readUint32LE();
					s.width = _resSE->_fileHandle->readUint32LE();
					s.height = _resSE->_fileHandle->readUint32LE();
					s.textureFileNameAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
					s.textureFileName.clear();
					innerExtraSpriteList.push_back(s);
				}
				_extraSpriteList.push_back(innerExtraSpriteList);
			}
		}
	}
	for(uint32 index = 0; index < _extraSpriteList.size(); index++ ) {
		Common::Array<staticSprite> *innerExtraSpriteList = &_extraSpriteList[index];
		for(uint32 index2 = 0; index2 < innerExtraSpriteList->size(); index2++ ) {
			struct staticSprite *innerExtraSprite = &(*innerExtraSpriteList)[index2];
			_resSE->_fileHandle->seek(innerExtraSprite->textureFileNameAddress, SEEK_SET);
			innerExtraSprite->textureFileName = _resSE->_fileHandle->readPadded16String();
		}
	}
	// read unknown5 list
	for(uint32 index = 0; index < _unknown5HeaderList.size(); index++ ) {
		struct unknown5Header *h = &_unknown5HeaderList[index];
		_resSE->_fileHandle->seek(h->unknown5Address, SEEK_SET);
		h->data = (byte *)malloc(h->unknown5Size);
		_resSE->_fileHandle->read(h->data, h->unknown5Size);
	}
}
#endif

} // End of namespace Scumm

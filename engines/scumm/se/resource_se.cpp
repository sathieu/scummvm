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
ResourceManager_se::ResourceManager_se(ScummEngine_se *vm)
	: _vm(vm), _fileHandle(0), _roomList(), _uiList(), _costumeList(),
	_roomCache(), _costumeCache(), _uiCache() {
	if (vm->_game.features & GF_PAKFILE) {
		_fileHandle = new ScummPakFile();
	} else {
		_fileHandle = new ScummFile();
	}
}

ResourceManager_se::~ResourceManager_se() {
	_fileHandle->close();
	delete _fileHandle;
}

bool ResourceManager_se::openFile(const Common::String &filename) {
	if (!_vm->_containerFile.empty()) {
		_fileHandle->close();
		if (!_fileHandle->open(_vm->_containerFile)) {
			error("Couldn't open container file '%s'", _vm->_containerFile.c_str());
		}

		if (!_fileHandle->openSubFile(filename)) {
			warning("Could not find file %s in container file", filename.c_str());
			return false;
		}
	} else {
		_fileHandle->close();
		if (!_fileHandle->open(filename)) {
			warning("Could not find file %s", filename.c_str());
			return false;
		}
	}
	return true;
}

void ResourceManager_se::readIndexFile() {
	Common::String SEIndexFile;
	if (_vm->_game.id == GID_MONKEY)
		SEIndexFile = "monkey1_retail.scumm.xml";
	else if (_vm->_game.id == GID_MONKEY2)
		SEIndexFile = "monkey.scumm.xml";
	else
		error("Unsupported game id '%i'", _vm->_game.id);

	if (!openFile(SEIndexFile))
		error("Could not open Special Edition index %s", SEIndexFile.c_str());

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
		if (_vm->_game.id == GID_MONKEY2) {
			// MI2SE has longer header
			_fileHandle->readUint32LE();
			_fileHandle->readUint32LE();
			_fileHandle->readUint32LE();
		}
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
	delete(room);
	return &_roomCache.back();
}

ResourceManager_se::Room *ResourceManager_se::getRoom(const uint32 roomNumber) {
	if (roomNumber >= _roomCount) {
		error("Unable to get out of bound SE room");
	}
	if (_roomList[roomNumber].name.empty()) {
		error("Room %i is not in the index", roomNumber);
	}
	return getRoom(_roomList[roomNumber].name);
}

ResourceManager_se::Costume *ResourceManager_se::getCostume(const Common::String &costumeFile) {
	for (Common::List<Costume>::iterator
			x = _costumeCache.begin(); x != _costumeCache.end(); ++x) {
		if (x->getCostumeFile().equals(costumeFile))
			return &*x;
	}
	ResourceManager_se::Costume *costume = new ResourceManager_se::Costume(this, costumeFile);
	_costumeCache.push_back(*costume);
	delete(costume);
	return &_costumeCache.back();
}

ResourceManager_se::Costume *ResourceManager_se::getCostume(const uint32 costumeNumber) {
	if (costumeNumber >= _costumeCount) {
		error("Unable to get out of bound SE costume");
	}
	if (_costumeList[costumeNumber].name.empty()) {
		error("Costume %i is not in the index", costumeNumber);
	}
	return getCostume(_costumeList[costumeNumber].name);
}

ResourceManager_se::Ui *ResourceManager_se::getUi(const Common::String &uiFile) {
	for (Common::List<Ui>::iterator
			x = _uiCache.begin(); x != _uiCache.end(); ++x) {
		if (x->getUiFile().equals(uiFile))
			return &*x;
	}
	ResourceManager_se::Ui *ui = new ResourceManager_se::Ui(this, uiFile);
	_uiCache.push_back(*ui);
	delete(ui);
	return &_uiCache.back();
}

ResourceManager_se::Ui *ResourceManager_se::getUi(const uint32 uiNumber) {
	if (uiNumber >= _uiCount) {
		error("Unable to get out of bound SE costume");
	}
	return getUi(_uiList[uiNumber].name);
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
	if (!_resSE->openFile(roomFile))
		error("Could not open room file %s", roomFile.c_str());

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
			if (_resSE->_vm->_game.id == GID_MONKEY2) {
				s.unknown3 = _resSE->_fileHandle->readUint32LE();
				s.unknown4 = _resSE->_fileHandle->readUint32LE();
			} else {
				s.unknown3 = 0;
				s.unknown4 = 0;
			}
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
			if (s->textureFileNameAddress > _resSE->_fileHandle->size()) {
				warning("static textureFileNameAddress=%i is beyond file length=%i",
					s->textureFileNameAddress, _resSE->_fileHandle->size());
				continue;
			}
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
			if (s->textureFileNameAddress > _resSE->_fileHandle->size()) {
				warning("textureFileNameAddress=%i is beyond file length=%i",
					s->textureFileNameAddress, _resSE->_fileHandle->size());
				continue;
			}
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

ResourceManager_se::Room::~Room() {
	for(uint32 layer = 0; layer < _staticSpriteList.size(); layer++ ) {
		Common::Array<staticSprite> *innerStaticSpriteList = &_staticSpriteList[layer];
		for(uint32 index2 = 0; index2 < innerStaticSpriteList->size(); index2++ ) {
			struct staticSprite *innerStaticSprite = &(*innerStaticSpriteList)[index2];
			innerStaticSprite->surface.free();
		}
	}

	for(uint32 layer = 0; layer < _spriteList.size(); layer++ ) {
		Common::Array<sprite> *innerSpriteList = &_spriteList[layer];
		for(uint32 index2 = 0; index2 < innerSpriteList->size(); index2++ ) {
			struct sprite *innerSprite = &(*innerSpriteList)[index2];
			innerSprite->surface.free();
		}
	}

	for(uint32 index = 0; index < _extraSpriteList.size(); index++ ) {
		Common::Array<staticSprite> *innerExtraSpriteList = &_extraSpriteList[index];
		for(uint32 index2 = 0; index2 < innerExtraSpriteList->size(); index2++ ) {
			struct staticSprite *innerExtraSprite = &(*innerExtraSpriteList)[index2];
			innerExtraSprite->surface.free();
		}
	}
}

// ResourceManager_se::Costume

/*
 * This function is based on Monkey Island SE Inspector
 * by Robin Theilade (License: Public Domain).
 *
 * Original code at:
 * http://sourceforge.net/p/mi1sexmlparser/code/43/tree/trunk/MonkeyIsland1SpecialEditionXmlParser/Formats/Costumes/Parser.cs
 *
 * Further enhancements have been done.
 */
ResourceManager_se::Costume::Costume(ResourceManager_se *resSE, const Common::String &costumeFile)
	: _resSE(resSE), _costumeFile(costumeFile),
	_textureList(), _animationGroupList(), _spriteGroupList(),
	_pathPointList()
	{
	if (!_resSE->openFile(costumeFile))
		error("Could not open costume file %s", costumeFile.c_str());

	// read header
	_identifier = _resSE->_fileHandle->readUint32LE();
	_nameAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	_textureHeaderCount = _resSE->_fileHandle->readUint32LE();
	_textureHeaderAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	_animationHeaderCount = _resSE->_fileHandle->readUint32LE();
	_animationHeaderAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	_unknown7 = _resSE->_fileHandle->readUint32LE();
	_spriteHeaderCount = _resSE->_fileHandle->readUint32LE();
	_spriteHeaderAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	_unknown10 = _resSE->_fileHandle->readUint32LE();
	_pathPointCount = _resSE->_fileHandle->readUint32LE();
	_pathPointAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	_unknown13 = _resSE->_fileHandle->readUint32LE();
	_unknown14 = _resSE->_fileHandle->readUint32LE();
	_unknown15 = _resSE->_fileHandle->readUint32LE();
	_unknown16 = _resSE->_fileHandle->readUint32LE();
	_unknown17 = _resSE->_fileHandle->readUint32LE();
	_unknown18 = _resSE->_fileHandle->readUint32LE();
	_unknown19 = _resSE->_fileHandle->readUint32LE();
	_unknown20 = _resSE->_fileHandle->readUint32LE();
	if (_resSE->_vm->_game.id == GID_MONKEY2) {
		_unknown21 = _resSE->_fileHandle->readUint32LE();
		_unknown22 = _resSE->_fileHandle->readUint32LE();
		_unknown23 = _resSE->_fileHandle->readUint32LE();
		_unknown24 = _resSE->_fileHandle->readUint32LE();
	}
	assert(_resSE->_fileHandle->pos() == _nameAddress);
	_name = _resSE->_fileHandle->readPadded16String();
	if (_unknown7)
		warning("Costume %i:%s has _unknown7 = %d", _identifier, _name.c_str(), _unknown7);
	if (_unknown10)
		warning("Costume %i:%s has _unknown10 = %d", _identifier, _name.c_str(), _unknown10);
	if (_unknown13)
		warning("Costume %i:%s has _unknown13 = %d", _identifier, _name.c_str(), _unknown13);
	if (_unknown14)
		warning("Costume %i:%s has _unknown14 = %d", _identifier, _name.c_str(), _unknown14);
	if (_unknown15)
		warning("Costume %i:%s has _unknown15 = %d", _identifier, _name.c_str(), _unknown15);
	if (_unknown16)
		warning("Costume %i:%s has _unknown16 = %d", _identifier, _name.c_str(), _unknown16);

	// read texture header list
	_resSE->_fileHandle->seek(_textureHeaderAddress, SEEK_SET);
	_textureList.reserve(_textureHeaderCount);
	for(uint32 index = 0; index < _textureHeaderCount; index++) {
		struct texture h;
		memset(&h, 0, sizeof(h));
		h.textureSpriteCount = _resSE->_fileHandle->readUint32LE();
		h.filenameAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
		h.filename.clear();
		_textureList.push_back(h);
	}

	// read animation header list
	_resSE->_fileHandle->seek(_animationHeaderAddress, SEEK_SET);
	_animationGroupList.reserve(_animationHeaderCount);
	for(uint32 index = 0; index < _animationHeaderCount; index++) {
		struct animationGroup h;
		h.nameAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
		h.identifier = _resSE->_fileHandle->readUint32LE();
		h.animationCount = _resSE->_fileHandle->readUint32LE();
		h.animationAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
		h.name.clear();
		h.animationList.clear();
		_animationGroupList.push_back(h);
	}

	// read sprite header list
	_resSE->_fileHandle->seek(_spriteHeaderAddress, SEEK_SET);
	_spriteGroupList.reserve(_spriteHeaderCount);
	for(uint32 index = 0; index < _spriteHeaderCount; index++) {
		struct spriteGroup h;
		h.identifier = _resSE->_fileHandle->readUint32LE();
		h.unknown2 = _resSE->_fileHandle->readUint32LE();
		h.spriteCount = _resSE->_fileHandle->readUint32LE();
		h.spriteAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
		h.spriteList.clear();
		_spriteGroupList.push_back(h);
	}

	// read path point list
	_resSE->_fileHandle->seek(_pathPointAddress, SEEK_SET);
	_pathPointList.reserve(_pathPointCount);
	for(uint32 index = 0; index < _pathPointCount; index++) {
		struct pathPoint p;
		p.unknown1 = _resSE->_fileHandle->readUint32LE();
		p.unknown2 = _resSE->_fileHandle->readUint32LE();
		p.unknown3 = _resSE->_fileHandle->readUint32LE();
		_pathPointList.push_back(p);
	}

	// read texture filenames
	for(uint32 index = 0; index < _textureList.size(); index++ ) {
		struct texture *h = &_textureList[index];
		_resSE->_fileHandle->seek(h->filenameAddress, SEEK_SET);
		h->filename = _resSE->_fileHandle->readPadded16String();
	}

	// read animation names and frameHeader
	for(uint32 index = 0; index < _animationGroupList.size(); index++ ) {
		struct animationGroup *h = &_animationGroupList[index];
		_resSE->_fileHandle->seek(h->nameAddress, SEEK_SET);
		h->name = _resSE->_fileHandle->readPadded16String();
		_resSE->_fileHandle->seek(h->animationAddress, SEEK_SET);
		h->animationList.reserve(h->animationCount);
		for(uint32 index2 = 0; index2 < h->animationCount; index2++ ) {
			struct animation a;
			a.spriteGroupIdentitier = _resSE->_fileHandle->readUint32LE();
			a.unknown2 = _resSE->_fileHandle->readUint32LE();
			if (_resSE->_vm->_game.id == GID_MONKEY2) {
				a.unknown3 = _resSE->_fileHandle->readUint32LE();
			} else {
				a.unknown3 = 0;
			}
			a.frameCount = _resSE->_fileHandle->readUint32LE();
			a.frameAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
			a.frameList.clear();
			h->animationList.push_back(a);
		}
	}

	// read sprite list
	for(uint32 index = 0; index < _spriteGroupList.size(); index++) {
		struct spriteGroup *sh = &_spriteGroupList[index];
		sh->spriteList.reserve(sh->spriteCount);
		_resSE->_fileHandle->seek(sh->spriteAddress, SEEK_SET);
		for(uint32 index2 = 0; index2 < sh->spriteCount; index2++ ) {
			struct sprite s;
			s.textureNumber = _resSE->_fileHandle->readUint32LE();
			s.textureX = _resSE->_fileHandle->readUint32LE();
			s.textureY = _resSE->_fileHandle->readUint32LE();
			s.textureWidth = _resSE->_fileHandle->readUint32LE();
			s.textureHeight = _resSE->_fileHandle->readUint32LE();
			s.screenX = _resSE->_fileHandle->readUint32LE();
			s.screenY = _resSE->_fileHandle->readUint32LE();
			s.unknown8 = _resSE->_fileHandle->readUint32LE();
			s.unknown9 = _resSE->_fileHandle->readUint32LE();
			s.unknown10 = _resSE->_fileHandle->readUint32LE();
			sh->spriteList.push_back(s);
		}
	}

	// read animation frame list
	for(uint32 index = 0; index < _animationGroupList.size(); index++) {
		struct animationGroup *h = &_animationGroupList[index];
		for(uint32 index2 = 0; index2 < h->animationList.size(); index2++ ) {
			struct animation *a = &h->animationList[index2];
			_resSE->_fileHandle->seek(a->frameAddress, SEEK_SET);
			a->frameList.reserve(a->frameCount);
			for(uint32 index3 = 0; index3 < a->frameCount; index3++ ) {
				struct frame f;
				f.spriteIdentitier =  _resSE->_fileHandle->readUint32LE();
				f.unknown2 = _resSE->_fileHandle->readUint32LE();
				f.unknown3 = _resSE->_fileHandle->readUint32LE();
				a->frameList.push_back(f);
			}
		}
	}
}

ResourceManager_se::Costume::~Costume() {
	for(uint32 index = 0; index < _textureList.size(); index++ ) {
		struct texture *currentTexture = &_textureList[index];
		for(int scale = 0; scale < 256; scale++) {
			if (currentTexture->flags[scale] & TF_EXISTS) {
				currentTexture->surface[scale].free();
			}
			if (currentTexture->flags[scale] & TF_MIRROR_EXISTS) {
				currentTexture->mirroredSurface[scale].free();
			}
		}
	}
}
// ResourceManager_se::Ui

ResourceManager_se::Ui::Ui(ResourceManager_se *resSE, const Common::String &uiFile)
	: _resSE(resSE), _uiFile(uiFile)
	{
	if (!_resSE->openFile(uiFile))
		error("Could not open UI file %s", uiFile.c_str());

	// read header
	uint32 staticSpriteGroupCount = _resSE->_fileHandle->readUint32LE();
	uint32 staticSpriteGroupAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
	uint32 textureCount = _resSE->_fileHandle->readUint32LE();
	uint32 textureAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();

	// read static sprite group list
	_resSE->_fileHandle->seek(staticSpriteGroupAddress, SEEK_SET);
	_staticSpriteGroupList.reserve(staticSpriteGroupCount);
	for(uint32 index = 0; index < staticSpriteGroupCount; index++) {
		struct staticSpriteGroup h;
		h.unknown1 = _resSE->_fileHandle->readUint32LE();
		h.width = _resSE->_fileHandle->readUint32LE();
		h.height = _resSE->_fileHandle->readUint32LE();
		h.staticSpriteCount = _resSE->_fileHandle->readUint32LE();
		h.staticSpriteAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
		_staticSpriteGroupList.push_back(h);
	}

	// read texture
	_resSE->_fileHandle->seek(textureAddress, SEEK_SET);
	_textureList.reserve(textureCount);
	for(uint32 index = 0; index < textureCount; index++) {
		struct texture h;
		h.unknown1 = _resSE->_fileHandle->readUint32LE();
		h.filenameAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
		h.textureX = _resSE->_fileHandle->readUint32LE();
		h.textureY = _resSE->_fileHandle->readUint32LE();
		h.textureWidth = _resSE->_fileHandle->readUint32LE();
		h.textureHeight = _resSE->_fileHandle->readUint32LE();
		_textureList.push_back(h);
	}

	// read static sprite list
	for(uint32 index = 0; index < _staticSpriteGroupList.size(); index++ ) {
		struct staticSpriteGroup *h = &_staticSpriteGroupList[index];
		_resSE->_fileHandle->seek(h->staticSpriteAddress, SEEK_SET);
		h->staticSpriteList.reserve(h->staticSpriteCount);
		for(uint32 index2 = 0; index2 < h->staticSpriteCount; index2++ ) {
			struct staticSprite c;
			c.x = _resSE->_fileHandle->readUint32LE();
			c.y = _resSE->_fileHandle->readUint32LE();
			c.width = _resSE->_fileHandle->readUint32LE();
			c.height = _resSE->_fileHandle->readUint32LE();
			c.filenameAddress = _resSE->_fileHandle->readAbsolutePositionUint32LE();
			h->staticSpriteList.push_back(c);
		}
	}

	// read texture filenames
	for(uint32 index = 0; index < _textureList.size(); index++ ) {
		struct texture *h = &_textureList[index];
		_resSE->_fileHandle->seek(h->filenameAddress, SEEK_SET);
		h->filename = _resSE->_fileHandle->readPadded16String();
	}

	// read static sprite filenames
	for(uint32 index = 0; index < _staticSpriteGroupList.size(); index++ ) {
		struct staticSpriteGroup *h = &_staticSpriteGroupList[index];
		for(uint32 index2 = 0; index2 < h->staticSpriteList.size(); index2++ ) {
			struct staticSprite *c = &h->staticSpriteList[index2];
			_resSE->_fileHandle->seek(c->filenameAddress, SEEK_SET);
			c->filename = _resSE->_fileHandle->readPadded16String();
		}
	}

}

ResourceManager_se::Ui::~Ui() {

}

#endif

} // End of namespace Scumm

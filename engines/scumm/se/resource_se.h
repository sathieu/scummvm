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

#include "scumm/se/costume_se.h"

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
			uint32 unknown3; // MI2SE only
			uint32 unknown4; // MI2SE only
			uint32 width;
			uint32 height;
			uint32 textureFileNameAddress;
			Common::String textureFileName;
			//
			Graphics::Surface surface;
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
			//
			Graphics::Surface surface;
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
	public:
		uint32 _roomWidth;
		uint32 _roomHeight;
	protected:
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
		bool _spritesLoaded;

		void drawStaticSpriteList(ScummEngine_se *vm, VirtScreenNumber virt, Common::Array< Common::Array<staticSprite> > staticSpriteList, int start, int num);
	public:
		Room(ResourceManager_se *resSE, const Common::String &roomFile);
		~Room();
		const Common::String getRoomFile() { return _roomFile; }
		void loadSprites();
		void redrawBGStrip(ScummEngine_se *vm, VirtScreenNumber virt, int start, int num);
	};
	friend class Room;
	// Costume
	class Costume {
	protected:
		enum TextureFlags {
			TF_EXISTS = 1,
			TF_MIRROR_EXISTS = 2
		};

		struct texture {
			uint32 textureSpriteCount;
			uint32 filenameAddress;
			Common::String filename;
			byte flags[256]; // TextureFlags
			Graphics::Surface surface[256];
			Graphics::Surface mirroredSurface[256];
		};
		struct frame {
			uint32 spriteIdentitier;
			uint32 unknown2;
			uint32 unknown3;
		};
		struct animation {
			uint32 spriteGroupIdentitier;
			uint32 unknown2;
			uint32 unknown3; // MI2SE only
			uint32 frameCount;
			uint32 frameAddress;
			Common::Array<struct frame> frameList;
		};
		struct animationGroup {
			uint32 nameAddress;
			uint32 identifier;
			uint32 animationCount;
			uint32 animationAddress;
			Common::String name;
			Common::Array<struct animation> animationList;
		};
		struct sprite {
			uint32 textureNumber;
			uint32 textureX;
			uint32 textureY;
			uint32 textureWidth;
			uint32 textureHeight;
			uint32 screenX;
			uint32 screenY;
			uint32 unknown8;
			uint32 unknown9;
			uint32 unknown10;
		};
		struct spriteGroup {
			uint32 identifier;
			uint32 unknown2;
			uint32 spriteCount;
			uint32 spriteAddress;
			Common::Array<struct sprite> spriteList;
		};
		struct pathPoint {
			uint16 unknown1;
			uint32 unknown2;
			uint32 unknown3;
		};

		ResourceManager_se *_resSE;
		Common::String _costumeFile;

		uint32 _identifier;
		uint32 _nameAddress;
		uint32 _textureHeaderCount;
		uint32 _textureHeaderAddress;
		uint32 _animationHeaderCount;
		uint32 _animationHeaderAddress;
		uint32 _unknown7;
		uint32 _spriteHeaderCount;
		uint32 _spriteHeaderAddress;
		uint32 _unknown10;
		uint32 _pathPointCount;
		uint32 _pathPointAddress;
		uint32 _unknown13;
		uint32 _unknown14;
		uint32 _unknown15;
		uint32 _unknown16;
		uint32 _unknown17;
		uint32 _unknown18;
		uint32 _unknown19;
		uint32 _unknown20;
		uint32 _unknown21;
		uint32 _unknown22;
		uint32 _unknown23;
		uint32 _unknown24;
		Common::String _name;

		Common::Array<struct texture> _textureList;
		Common::Array<struct animationGroup> _animationGroupList;
		Common::Array<struct spriteGroup> _spriteGroupList;
		Common::Array<struct pathPoint> _pathPointList;
	public:
		Costume(ResourceManager_se *resSE, const Common::String &costumeFile);
		~Costume();
		const Common::String getCostumeFile() { return _costumeFile; }
		const Graphics::Surface getTexture(uint32 index, bool mirror, byte scale);
		const Graphics::Surface getSurface(const Common::String &animationGroupName, const uint32 animationGroupIndex, const uint32 animationIndex, const uint32 frameIndex, const byte scale);
	};
	// Ui
	class Ui {
	protected:
		ResourceManager_se *_resSE;
		Common::String _uiFile;

		struct staticSprite {
			uint32 x;
			uint32 y;
			uint32 width;
			uint32 height;
			uint32 filenameAddress;
			Common::String filename;
		};
		struct staticSpriteGroup {
			uint32 unknown1;
			uint32 width;
			uint32 height;
			uint32 staticSpriteCount;
			uint32 staticSpriteAddress;
			Common::Array<struct staticSprite> staticSpriteList;
		};
		struct texture {
			uint32 unknown1;
			uint32 filenameAddress;
			uint32 textureX;
			uint32 textureY;
			uint32 textureWidth;
			uint32 textureHeight;
			Common::String filename;
		};

		Common::Array<struct staticSpriteGroup> _staticSpriteGroupList;
		Common::Array<struct texture> _textureList;
	public:
		Ui(ResourceManager_se *resSE, const Common::String &uiFile);
		~Ui();
		const Common::String getUiFile() { return _uiFile; }
	};
protected:
	Common::List<Room> _roomCache;
	Common::List<Costume> _costumeCache;
	Common::List<Ui> _uiCache;
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
	Costume *getCostume(const uint32 costumeNumber);
	Ui *getUi(const Common::String &uiFile);
	Ui *getUi(const uint32 uiNumber);
};
#endif

} // End of namespace Scumm

#endif

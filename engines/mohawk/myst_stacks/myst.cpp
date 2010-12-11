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
 * $URL$
 * $Id$
 *
 */

#include "mohawk/cursors.h"
#include "mohawk/myst.h"
#include "mohawk/graphics.h"
#include "mohawk/myst_areas.h"
#include "mohawk/myst_saveload.h"
#include "mohawk/sound.h"
#include "mohawk/video.h"
#include "mohawk/myst_stacks/myst.h"

#include "gui/message.h"

namespace Mohawk {

// NOTE: Credits Start Card is 10000

MystScriptParser_Myst::MystScriptParser_Myst(MohawkEngine_Myst *vm) : MystScriptParser(vm) {
	setupOpcodes();

	// Card ID preinitialized by the engine for use by opcode 18
	// when linking back to Myst in the library
	_savedCardId = 4329;
	_libraryBookcaseChanged = false;
	_dockVaultState = 0;
}

MystScriptParser_Myst::~MystScriptParser_Myst() {
}

#define OPCODE(op, x) _opcodes.push_back(new MystOpcode(op, (OpcodeProcMyst) &MystScriptParser_Myst::x, #x))

void MystScriptParser_Myst::setupOpcodes() {
	// "Stack-Specific" Opcodes
	OPCODE(100, NOP);
	OPCODE(101, o_libraryBookPageTurnLeft);
	OPCODE(102, o_libraryBookPageTurnRight);
	OPCODE(103, o_fireplaceToggleButton);
	OPCODE(104, o_fireplaceRotation);
	OPCODE(105, opcode_105);
	OPCODE(106, o_towerRotationStart);
	OPCODE(107, NOP);
	OPCODE(108, o_towerRotationEnd);
	OPCODE(109, opcode_109);
	OPCODE(113, o_dockVaultOpen);
	OPCODE(114, o_dockVaultClose);
	OPCODE(115, o_bookGivePage);
	OPCODE(116, o_clockWheelsExecute);
	OPCODE(117, opcode_117);
	OPCODE(118, opcode_118);
	OPCODE(119, opcode_119);
	OPCODE(120, o_generatorButtonPressed);
	OPCODE(121, opcode_121);
	OPCODE(122, opcode_122);
	OPCODE(123, opcode_123);
	OPCODE(129, opcode_129);
	OPCODE(130, opcode_130);
	OPCODE(131, opcode_131);
	OPCODE(132, opcode_132);
	OPCODE(133, opcode_133);
	OPCODE(134, opcode_134);
	OPCODE(135, opcode_135);
	OPCODE(136, opcode_136);
	OPCODE(137, opcode_137);
	OPCODE(141, o_circuitBreakerStartMove);
	OPCODE(142, o_circuitBreakerMove);
	OPCODE(143, o_circuitBreakerEndMove);
	OPCODE(146, opcode_146);
	OPCODE(147, opcode_147);
	OPCODE(149, opcode_149);
	OPCODE(150, opcode_150);
	OPCODE(151, opcode_151);
	OPCODE(158, o_rocketSoundSliderStartMove);
	OPCODE(159, o_rocketSoundSliderMove);
	OPCODE(160, o_rocketSoundSliderEndMove);
	OPCODE(163, o_rocketLeverStartMove);
	OPCODE(164, o_rocketOpenBook);
	OPCODE(165, o_rocketLeverMove);
	OPCODE(166, o_rocketLeverEndMove);
	OPCODE(169, opcode_169);
	OPCODE(170, opcode_170);
	OPCODE(171, opcode_171);
	OPCODE(172, opcode_172);
	OPCODE(173, opcode_173);
	OPCODE(174, opcode_174);
	OPCODE(175, opcode_175);
	OPCODE(176, opcode_176);
	OPCODE(177, opcode_177);
	OPCODE(180, o_libraryCombinationBookStop);
	OPCODE(181, opcode_181);
	OPCODE(182, opcode_182);
	OPCODE(183, opcode_183);
	OPCODE(184, opcode_184);
	OPCODE(185, opcode_185);
	OPCODE(186, o_clockMinuteWheelStartTurn);
	OPCODE(187, NOP);
	OPCODE(188, o_clockWheelEndTurn);
	OPCODE(189, o_clockHourWheelStartTurn);
	OPCODE(190, o_libraryCombinationBookStartRight);
	OPCODE(191, o_libraryCombinationBookStartLeft);
	OPCODE(192, opcode_192);
	OPCODE(194, opcode_194);
	OPCODE(195, opcode_195);
	OPCODE(196, opcode_196);
	OPCODE(197, opcode_197);
	OPCODE(198, o_dockVaultForceClose);
	OPCODE(199, opcode_199);

	// "Init" Opcodes
	OPCODE(200, o_libraryBook_init);
	OPCODE(201, opcode_201);
	OPCODE(202, o_towerRotationMap_init);
	OPCODE(203, o_forechamberDoor_init);
	OPCODE(204, opcode_204);
	OPCODE(205, opcode_205);
	OPCODE(206, opcode_206);
	OPCODE(208, opcode_208);
	OPCODE(209, o_libraryBookcaseTransform_init);
	OPCODE(210, o_generatorControlRoom_init);
	OPCODE(211, o_fireplace_init);
	OPCODE(212, opcode_212);
	OPCODE(213, opcode_213);
	OPCODE(214, opcode_214);
	OPCODE(215, opcode_215);
	OPCODE(216, opcode_216);
	OPCODE(217, opcode_217);
	OPCODE(218, opcode_218);
	OPCODE(219, o_rocketSliders_init);
	OPCODE(220, o_rocketLinkVideo_init);
	OPCODE(221, o_greenBook_init);
	OPCODE(222, opcode_222);

	// "Exit" Opcodes
	OPCODE(300, opcode_300);
	OPCODE(301, opcode_301);
	OPCODE(302, opcode_302);
	OPCODE(303, opcode_303);
	OPCODE(304, opcode_304);
	OPCODE(305, opcode_305);
	OPCODE(306, opcode_306);
	OPCODE(307, opcode_307);
	OPCODE(308, opcode_308);
	OPCODE(309, opcode_309);
	OPCODE(312, opcode_312);
}

#undef OPCODE

void MystScriptParser_Myst::disablePersistentScripts() {
	opcode_201_disable();
	opcode_205_disable();

	_libraryBookcaseMoving = false;
	_generatorControlRoomRunning = false;
	_libraryCombinationBookPagesTurning = false;
	_clockTurningWheel = 0;
	_towerRotationMapRunning = false;

	opcode_212_disable();
}

void MystScriptParser_Myst::runPersistentScripts() {
	opcode_201_run();

	if (_towerRotationMapRunning)
		towerRotationMap_run();

	opcode_205_run();

	if (_generatorControlRoomRunning)
		generatorControlRoom_run();

	if (_libraryCombinationBookPagesTurning)
		libraryCombinationBook_run();

	if (_libraryBookcaseMoving)
		libraryBookcaseTransform_run();

	if (_clockTurningWheel)
		clockWheel_run();

	opcode_212_run();
}

uint16 MystScriptParser_Myst::getVar(uint16 var) {
	MystVariables::Globals &globals = _vm->_saveLoad->_v->globals;
	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;

	switch(var) {
	case 0: // Myst Library Bookcase Closed
		return myst.libraryBookcaseDoor;
	case 1:
		if (globals.ending != 4) {
			return myst.libraryBookcaseDoor != 1;
		} else if (myst.libraryBookcaseDoor == 1) {
			return 2;
		} else {
			return 3;
		}
	case 2: // Marker Switch Near Cabin
		return myst.cabinMarkerSwitch;
	case 3: // Marker Switch Near Clock Tower
		return myst.clockTowerMarkerSwitch;
	case 4: // Marker Switch on Dock
		return myst.dockMarkerSwitch;
	case 5: // Marker Switch Near Ship Pool
		return myst.poolMarkerSwitch;
	case 6: // Marker Switch Near Cogs
		return myst.gearsMarkerSwitch;
	case 7: // Marker Switch Near Generator Room
		return myst.generatorMarkerSwitch;
	case 8: // Marker Switch Near Stellar Observatory
		return myst.observatoryMarkerSwitch;
	case 9: // Marker Switch Near Rocket Ship
		return myst.rocketshipMarkerSwitch;
	case 12: // Clock tower gears bridge
		return myst.clockTowerBridgeOpen;
	case 23: // Fireplace Pattern Correct
		return _fireplaceLines[0] == 195
				&& _fireplaceLines[1] == 107
				&& _fireplaceLines[2] == 163
				&& _fireplaceLines[3] == 147
				&& _fireplaceLines[4] == 204
				&& _fireplaceLines[5] == 250;
	case 24: // Fireplace Blue Page Present
		if (globals.ending != 4) {
			return !(globals.bluePagesInBook & 32) && (globals.heldPage != 6);
		} else {
			return 0;
		}
	case 25: // Fireplace Red Page Present
		if (globals.ending != 4) {
			return !(globals.redPagesInBook & 32) && (globals.heldPage != 12);
		} else {
			return 0;
		}
	case 37: // Clock Tower Control Wheels Position
		return 3 * ((myst.clockTowerMinutePosition / 5) % 3) + myst.clockTowerHourPosition % 3;
	case 41: // Dock Marker Switch Vault State
		return _dockVaultState;
	case 43: // Clock Tower Time
		return myst.clockTowerHourPosition * 12 + myst.clockTowerMinutePosition / 5;
	case 44: // Rocket ship power state
		if (myst.generatorBreakers || myst.generatorVoltage == 0)
			return 0;
		else if (myst.generatorVoltage != 59)
			return 1;
		else
			return 2;
	case 46:
		return bookCountPages(100);
	case 47:
		return bookCountPages(101);
	case 49: // Generator running
		return myst.generatorVoltage > 0;
	case 52: // Generator Switch #1
		return (myst.generatorButtons & 1) != 0;
	case 53: // Generator Switch #2
		return (myst.generatorButtons & 2) != 0;
	case 54: // Generator Switch #3
		return (myst.generatorButtons & 4) != 0;
	case 55: // Generator Switch #4
		return (myst.generatorButtons & 8) != 0;
	case 56: // Generator Switch #5
		return (myst.generatorButtons & 16) != 0;
	case 57: // Generator Switch #6
		return (myst.generatorButtons & 32) != 0;
	case 58: // Generator Switch #7
		return (myst.generatorButtons & 64) != 0;
	case 59: // Generator Switch #8
		return (myst.generatorButtons & 128) != 0;
	case 60: // Generator Switch #9
		return (myst.generatorButtons & 256) != 0;
	case 61: // Generator Switch #10
		return (myst.generatorButtons & 512) != 0;
	case 62: // Generator Power Dial Left LED Digit
		return _generatorVoltage / 10;
	case 63: // Generator Power Dial Right LED Digit
		return _generatorVoltage % 10;
	case 64: // Generator Power To Spaceship Dial Left LED Digit
		if (myst.generatorVoltage > 59 || myst.generatorBreakers)
			return 0;
		else
			return myst.generatorVoltage / 10;
	case 65: // Generator Power To Spaceship Dial Right LED Digit
		if (myst.generatorVoltage > 59 || myst.generatorBreakers)
			return 0;
		else
			return myst.generatorVoltage % 10;
	case 66: // Generators lights on
		return 0;
	case 93: // Breaker nearest Generator Room Blown
		return myst.generatorBreakers == 1;
	case 94: // Breaker nearest Rocket Ship Blown
		return myst.generatorBreakers == 2;
	case 96: // Generator Power Dial Needle Position
		return myst.generatorVoltage / 4;
	case 97: // Generator Power To Spaceship Dial Needle Position
		if (myst.generatorVoltage > 59 || myst.generatorBreakers)
			return 0;
		else
			return myst.generatorVoltage / 4;
	case 102: // Red page
		if (globals.ending != 4) {
			return !(globals.redPagesInBook & 1) && (globals.heldPage != 7);
		} else {
			return 0;
		}
	case 103: // Blue page
		if (globals.ending != 4) {
			return !(globals.bluePagesInBook & 1) && (globals.heldPage != 1);
		} else {
			return 0;
		}
	case 300: // Rocket Ship Music Puzzle Slider State
		return 1;
	case 302: // Green Book Opened Before Flag
		return myst.greenBookOpenedBefore;
	case 304: // Tower Rotation Map Initialized
		return _towerRotationMapInitialized;
	default:
		return MystScriptParser::getVar(var);
	}
}

void MystScriptParser_Myst::toggleVar(uint16 var) {
	MystVariables::Globals &globals = _vm->_saveLoad->_v->globals;
	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;

	switch(var) {
	case 2: // Marker Switch Near Cabin
		myst.cabinMarkerSwitch = (myst.cabinMarkerSwitch + 1) % 2;
		break;
	case 3: // Marker Switch Near Clock Tower
		myst.clockTowerMarkerSwitch = (myst.clockTowerMarkerSwitch + 1) % 2;
		break;
	case 4: // Marker Switch on Dock
		myst.dockMarkerSwitch = (myst.dockMarkerSwitch + 1) % 2;
		break;
	case 5: // Marker Switch Near Ship Pool
		myst.poolMarkerSwitch = (myst.poolMarkerSwitch + 1) % 2;
		break;
	case 6: // Marker Switch Near Cogs
		myst.gearsMarkerSwitch = (myst.gearsMarkerSwitch + 1) % 2;
		break;
	case 7: // Marker Switch Near Generator Room
		myst.generatorMarkerSwitch = (myst.generatorMarkerSwitch + 1) % 2;
		break;
	case 8: // Marker Switch Near Stellar Observatory
		myst.observatoryMarkerSwitch = (myst.observatoryMarkerSwitch + 1) % 2;
		break;
	case 9: // Marker Switch Near Rocket Ship
		myst.rocketshipMarkerSwitch = (myst.rocketshipMarkerSwitch + 1) % 2;
		break;
	case 24: // Fireplace Blue Page
		if (globals.ending != 4 && !(globals.bluePagesInBook & 32)) {
			if (globals.heldPage == 6)
				globals.heldPage = 0;
			else {
				globals.heldPage = 6;
			}
		}
		break;
	case 25: // Fireplace Red page
		if (globals.ending != 4 && !(globals.redPagesInBook & 32)) {
			if (globals.heldPage == 12)
				globals.heldPage = 0;
			else {
				globals.heldPage = 12;
			}
		}
		break;
	case 41: // Vault white page
		if (globals.ending != 4) {
			if (_dockVaultState == 1) {
				_dockVaultState = 2;
				globals.heldPage = 0;
			} else if (_dockVaultState == 2) {
				_dockVaultState = 1;
				globals.heldPage = 13;
			}
		}
		break;
	case 102: // Red page
		if (globals.ending != 4 && !(globals.redPagesInBook & 1)) {
			if (globals.heldPage == 7)
				globals.heldPage = 0;
			else {
				globals.heldPage = 7;
			}
		}
		break;
	case 103: // Blue page
		if (globals.ending != 4 && !(globals.bluePagesInBook & 1)) {
			if (globals.heldPage == 1)
				globals.heldPage = 0;
			else {
				globals.heldPage = 1;
			}
		}
		break;
	default:
		MystScriptParser::toggleVar(var);
		break;
	}
}

bool MystScriptParser_Myst::setVarValue(uint16 var, uint16 value) {
	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;
	bool refresh = false;

	switch (var) {
	case 0: // Myst Library Bookcase Closed
		if (myst.libraryBookcaseDoor != value) {
			myst.libraryBookcaseDoor = value;
			_tempVar = 0;
			refresh = true;
		}
		break;
	case 302: // Green Book Opened Before Flag
		myst.greenBookOpenedBefore = value;
		break;
	case 303: // Library Bookcase status changed
		_libraryBookcaseChanged = value;
		break;
	case 304: // Myst Library Image Present on Tower Rotation Map
		_towerRotationMapInitialized = value;
		break;
	default:
		refresh = MystScriptParser::setVarValue(var, value);
		break;
	}

	return refresh;
}

uint16 MystScriptParser_Myst::bookCountPages(uint16 var) {
	MystVariables::Globals &globals = _vm->_saveLoad->_v->globals;

	uint16 pages = 0;
	uint16 cnt = 0;

	// Select book according to var
	if (var == 100)
		pages = globals.redPagesInBook;
	else if (var == 101)
		pages = globals.bluePagesInBook;

	// Special page present
	if (pages & 64)
		return 6;

	// Count pages
	if (pages & 1)
		cnt++;

	if (pages & 2)
		cnt++;

	if (pages & 4)
		cnt++;

	if (pages & 8)
		cnt++;

	if (pages & 16)
		cnt++;

	return cnt;
}

void MystScriptParser_Myst::o_libraryBookPageTurnLeft(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Turn book page left", op);

	if (_libraryBookPage - 1 >= 0) {
		_libraryBookPage--;

		Common::Rect rect = Common::Rect(0, 0, 544, 333);
		_vm->_gfx->copyImageToScreen(_libraryBookBaseImage + _libraryBookPage, rect);

		if (_vm->_rnd->getRandomBit())
			_vm->_sound->playSound(_libraryBookSound1);
		else
			_vm->_sound->playSound(_libraryBookSound2);

		_vm->_gfx->updateScreen();
	}
}

void MystScriptParser_Myst::o_libraryBookPageTurnRight(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Turn book page right", op);

	if (_libraryBookPage + 1 < _libraryBookNumPages) {
		_libraryBookPage++;

		Common::Rect rect = Common::Rect(0, 0, 544, 333);
		_vm->_gfx->copyImageToScreen(_libraryBookBaseImage + _libraryBookPage, rect);

		if (_vm->_rnd->getRandomBit())
			_vm->_sound->playSound(_libraryBookSound1);
		else
			_vm->_sound->playSound(_libraryBookSound2);

		_vm->_gfx->updateScreen();
	}
}

void MystScriptParser_Myst::o_fireplaceToggleButton(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Myst Card 4162 (Fireplace Grid)
	debugC(kDebugScript, "Opcode %d: Fireplace grid toggle button", op);

	uint16 bitmask = argv[0];
	uint16 line = _fireplaceLines[var - 17];

	debugC(kDebugScript, "\tvar: %d", var);
	debugC(kDebugScript, "\tbitmask: 0x%02X", bitmask);

	if (line & bitmask) {
		// Unset button
		for (uint i = 4795; i >= 4779; i--) {
			_vm->_gfx->copyImageToScreen(i, _invokingResource->getRect());
			_vm->_gfx->updateScreen();
			_vm->_system->delayMillis(1);
		}
		_fireplaceLines[var - 17] &= ~bitmask;
	} else {
		// Set button
		for (uint i = 4779; i <= 4795; i++) {
			_vm->_gfx->copyImageToScreen(i, _invokingResource->getRect());
			_vm->_gfx->updateScreen();
			_vm->_system->delayMillis(1);
		}
		_fireplaceLines[var - 17] |= bitmask;
	}
}

void MystScriptParser_Myst::o_fireplaceRotation(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Myst Card 4162 and 4166 (Fireplace Puzzle Rotation Movies)
	uint16 movieNum = argv[0];
	debugC(kDebugScript, "Opcode %d: Play Fireplace Puzzle Rotation Movies", op);
	debugC(kDebugScript, "\tmovieNum: %d", movieNum);

	if (movieNum)
		_vm->_video->playMovie(_vm->wrapMovieFilename("fpout", kMystStack), 167, 4);
	else
		_vm->_video->playMovie(_vm->wrapMovieFilename("fpin", kMystStack), 167, 4);
}

void MystScriptParser_Myst::opcode_105(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	if (argc == 1) {
		varUnusedCheck(op, var);

		uint16 soundId = argv[0];
		uint16 boxValue = 0;
		Audio::SoundHandle *handle;

		debugC(kDebugScript, "Opcode %d: Ship Puzzle Logic", op);
		debugC(kDebugScript, "\tsoundId: %d", soundId);

		// Logic for Myst Ship Box Puzzle Solution
		for (byte i = 0; i < 8; i++)
			boxValue |= _vm->_varStore->getVar(i + 26) ? (1 << i) : 0;

		uint16 var10 = _vm->_varStore->getVar(10);

		if (boxValue == 0x32 && var10 == 0) {
			handle = _vm->_sound->playSound(soundId);

			while (_vm->_mixer->isSoundHandleActive(*handle))
				_vm->_system->delayMillis(10);

			_vm->_varStore->setVar(10, 1);
		} else if (boxValue != 0x32 && var10 == 1) {
			handle = _vm->_sound->playSound(soundId);

			while (_vm->_mixer->isSoundHandleActive(*handle))
				_vm->_system->delayMillis(10);

			_vm->_varStore->setVar(10, 0);
		}
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::o_towerRotationStart(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	_towerRotationMapClicked = true;
}

void MystScriptParser_Myst::o_towerRotationEnd(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	_towerRotationMapClicked = false;
}

void MystScriptParser_Myst::opcode_109(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	if (argc == 1) {
		int16 signedValue = argv[0];

		debugC(kDebugScript, "Opcode %d: Add Signed Value to Var", op);
		debugC(kDebugScript, "\tVar: %d", var);
		debugC(kDebugScript, "\tsignedValue: %d", signedValue);

		_vm->_varStore->setVar(var, _vm->_varStore->getVar(var) + signedValue);
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::o_dockVaultOpen(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Myst 4143 (Dock near Marker Switch)
	MystVariables::Globals &globals = _vm->_saveLoad->_v->globals;
	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;
	uint16 soundId = argv[0];
	uint16 delay = argv[1];
	uint16 directionalUpdateDataSize = argv[2];

	debugC(kDebugScript, "Opcode %d: Vault Open Logic", op);
	debugC(kDebugScript, "\tsoundId: %d", soundId);
	debugC(kDebugScript, "\tdirectionalUpdateDataSize: %d", directionalUpdateDataSize);

	if ((myst.cabinMarkerSwitch == 1) &&
		(myst.clockTowerMarkerSwitch == 1) &&
		(myst.dockMarkerSwitch == 0) &&
		(myst.gearsMarkerSwitch == 1) &&
		(myst.generatorMarkerSwitch == 1) &&
		(myst.observatoryMarkerSwitch == 1) &&
		(myst.poolMarkerSwitch == 1) &&
		(myst.rocketshipMarkerSwitch == 1)) {
		if (globals.heldPage != 13 && globals.ending != 4)
			_dockVaultState = 2;
		else
			_dockVaultState = 1;

		_vm->_sound->playSound(soundId);
		_vm->redrawArea(41, false);
		animatedUpdate(directionalUpdateDataSize, &argv[3], delay);
	}
}

void MystScriptParser_Myst::o_dockVaultClose(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Myst 4143 (Dock near Marker Switch)
	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;
	uint16 soundId = argv[0];
	uint16 delay = argv[1];
	uint16 directionalUpdateDataSize = argv[2];

	debugC(kDebugScript, "Opcode %d: Vault Close Logic", op);
	debugC(kDebugScript, "\tsoundId: %d", soundId);
	debugC(kDebugScript, "\tdirectionalUpdateDataSize: %d", directionalUpdateDataSize);

	if ((myst.cabinMarkerSwitch == 1) &&
		(myst.clockTowerMarkerSwitch == 1) &&
		(myst.dockMarkerSwitch == 1) &&
		(myst.gearsMarkerSwitch == 1) &&
		(myst.generatorMarkerSwitch == 1) &&
		(myst.observatoryMarkerSwitch == 1) &&
		(myst.poolMarkerSwitch == 1) &&
		(myst.rocketshipMarkerSwitch == 1)) {
		if (_dockVaultState == 1 || _dockVaultState == 2)
			_dockVaultState = 0;

		_vm->_sound->playSound(soundId);
		_vm->redrawArea(41, false);
		animatedUpdate(directionalUpdateDataSize, &argv[3], delay);
	}
}

void MystScriptParser_Myst::o_bookGivePage(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	MystVariables::Globals &globals = _vm->_saveLoad->_v->globals;

	uint16 cardIdLose = argv[0];
	uint16 cardIdBookCover = argv[1];
	uint16 soundIdAddPage = argv[2];

	debugC(kDebugScript, "Opcode %d: Red and Blue Book/Page Interaction", op);
	debugC(kDebugScript, "Var: %d", var);
	debugC(kDebugScript, "Card Id (Lose): %d", cardIdLose);
	debugC(kDebugScript, "Card Id (Book Cover): %d", cardIdBookCover);
	debugC(kDebugScript, "SoundId (Add Page): %d", soundIdAddPage);

	// No page or white page
	if (!globals.heldPage || globals.heldPage == 13) {
		_vm->changeToCard(cardIdBookCover, true);
		return;
	}

	uint16 bookVar = 101;
	uint16 mask = 0;

	switch (globals.heldPage) {
	case 7:
		bookVar = 100;
	case 1:
		mask = 1;
		break;
	case 8:
		bookVar = 100;
	case 2:
		mask = 2;
		break;
	case 9:
		bookVar = 100;
	case 3:
		mask = 4;
		break;
	case 10:
		bookVar = 100;
	case 4:
		mask = 8;
		break;
	case 11:
		bookVar = 100;
	case 5:
		mask = 16;
		break;
	case 12:
		bookVar = 100;
	case 6:
		mask = 32;
		break;
	}

	// Wrong book
	if (bookVar != var) {
		_vm->changeToCard(cardIdBookCover, true);
		return;
	}

	_vm->_cursor->hideCursor();
	_vm->_sound->playSoundBlocking(soundIdAddPage);
	_vm->setMainCursor(kDefaultMystCursor);

	// Add page to book
	if (var == 100)
		globals.redPagesInBook |= mask;
	else
		globals.bluePagesInBook |= mask;

	// Remove page from hand
	globals.heldPage = 0;

	_vm->_cursor->showCursor();

	if (mask == 32) {
		// You lose!
		if (var == 100)
			globals.currentAge = 9;
		else
			globals.currentAge = 10;

		_vm->changeToCard(cardIdLose, true);
	} else {
		_vm->changeToCard(cardIdBookCover, true);
	}
}

void MystScriptParser_Myst::o_clockWheelsExecute(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4006 (Clock Tower Time Controls)
	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;
	uint16 soundId = argv[0];

	debugC(kDebugScript, "Opcode %d: Clock Tower Bridge Puzzle Execute Button", op);

	// Correct time is 2:40
	bool correctTime = myst.clockTowerHourPosition == 2
						&& myst.clockTowerMinutePosition == 40;

	if (!myst.clockTowerBridgeOpen && correctTime) {
		_vm->_sound->playSound(soundId);
		_vm->_system->delayMillis(500);

		// TODO: Play only 1st half of movie i.e. gears rise up, from 0 to 650
		_vm->_video->playMovie(_vm->wrapMovieFilename("gears", kMystStack), 305, 36);

		myst.clockTowerBridgeOpen = 1;
		_vm->redrawArea(12);
	} else if (myst.clockTowerBridgeOpen && !correctTime) {
		_vm->_sound->playSound(soundId);
		_vm->_system->delayMillis(500);

		// TODO: Play only 2nd half of movie i.e. gears sink down, from 700 to 1300
		_vm->_video->playMovie(_vm->wrapMovieFilename("gears", kMystStack), 305, 36);

		myst.clockTowerBridgeOpen = 0;
		_vm->redrawArea(12);
	}
}

void MystScriptParser_Myst::opcode_117(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	if (argc == 2) {
		// Used by Myst Imager Control Button
		uint16 varValue = _vm->_varStore->getVar(var);

		if (varValue)
			_vm->_sound->playSound(argv[1]);
		else
			_vm->_sound->playSound(argv[0]);

		_vm->_varStore->setVar(var, !varValue);
		// TODO: Change Var 45 "Dock Forechamber Imager Water Effect Enabled" here?
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_118(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	if (argc == 5) {
		// Used by Card 4709 (Myst Imager Control Panel Red Button)

		debugC(kDebugScript, "Opcode %d: Imager Change Value", op);

		uint16 soundIdBeepLo = argv[0];
		uint16 soundIdBeepHi = argv[1];
		uint16 soundIdBwapp = argv[2];
		uint16 soundIdBeepTune = argv[3]; // 5 tones..
		uint16 soundIdPanelSlam = argv[4];

		debugC(kDebugScript, "\tsoundIdBeepLo: %d", soundIdBeepLo);
		debugC(kDebugScript, "\tsoundIdBeepHi: %d", soundIdBeepHi);
		debugC(kDebugScript, "\tsoundIdBwapp: %d", soundIdBwapp);
		debugC(kDebugScript, "\tsoundIdBeepTune: %d", soundIdBeepTune);
		debugC(kDebugScript, "\tsoundIdPanelSlam: %d", soundIdPanelSlam);

		_vm->_sound->playSound(soundIdBeepLo);

		// TODO: Complete Logic...
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_119(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	if (argc == 1) {
		// Used on Card 4383 and 4451 (Tower Elevator)
		switch (argv[0]) {
		case 0:
			_vm->_video->playMovie(_vm->wrapMovieFilename("libdown", kMystStack), 216, 78);
			break;
		case 1:
			_vm->_video->playMovie(_vm->wrapMovieFilename("libup", kMystStack), 214, 75);
			break;
		default:
			break;
		}
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::o_generatorButtonPressed(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Generator button pressed", op);

	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;
	MystResource *button = _invokingResource->_parent;

	generatorRedrawRocket();

	_generatorVoltage = myst.generatorVoltage;

	uint16 mask = 0;
	uint16 value = 0;
	generatorButtonValue(button, mask, value);

	// Button pressed
	if (myst.generatorButtons & mask) {
		myst.generatorButtons &= ~mask;
		myst.generatorVoltage -= value;

		if (myst.generatorVoltage)
			_vm->_sound->playSound(8297);
		else
			_vm->_sound->playSound(9297);
	} else {
		if (_generatorVoltage)
			_vm->_sound->playSound(6297);
		else
			_vm->_sound->playSound(7297); //TODO: Replace with play sound and replace background 4297

		myst.generatorButtons |= mask;
		myst.generatorVoltage += value;
	}

	// Redraw button
	_vm->redrawArea(button->getType8Var());

	// Blow breaker
	if (myst.generatorVoltage > 59)
		myst.generatorBreakers = _vm->_rnd->getRandomNumberRng(1, 2);
}

void MystScriptParser_Myst::generatorRedrawRocket() {
	_vm->redrawArea(64);
	_vm->redrawArea(65);
	_vm->redrawArea(97);
}

void MystScriptParser_Myst::generatorButtonValue(MystResource *button, uint16 &mask, uint16 &value) {
	switch (button->getType8Var()) {
	case 52: // Generator Switch #1
		mask = 1;
		value = 10;
		break;
	case 53: // Generator Switch #2
		mask = 2;
		value = 7;
		break;
	case 54: // Generator Switch #3
		mask = 4;
		value = 8;
		break;
	case 55: // Generator Switch #4
		mask = 8;
		value = 16;
		break;
	case 56: // Generator Switch #5
		mask = 16;
		value = 5;
		break;
	case 57: // Generator Switch #6
		mask = 32;
		value = 1;
		break;
	case 58: // Generator Switch #7
		mask = 64;
		value = 2;
		break;
	case 59: // Generator Switch #8
		mask = 128;
		value = 22;
		break;
	case 60: // Generator Switch #9
		mask = 256;
		value = 19;
		break;
	case 61: // Generator Switch #10
		mask = 512;
		value = 9;
		break;
	}
}

void MystScriptParser_Myst::opcode_121(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4100 (Cabin Safe Buttons)
	// Correct Solution (724) -> Var 67=2, 68=7, 69=5
	// Jump to Card 4103 when solution correct and handle pulled...
	if (argc == 0) {
		uint16 varValue = _vm->_varStore->getVar(var);
		if (varValue == 0)
			varValue = 9;
		else
			varValue--;
		_vm->_varStore->setVar(var, varValue);
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_122(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4100
	// TODO: Mouse down on handle
}

void MystScriptParser_Myst::opcode_123(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4100
	// TODO: Mouse drag on handle
}

void MystScriptParser_Myst::opcode_129(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Month increase
}

void MystScriptParser_Myst::opcode_130(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Month decrease
}

void MystScriptParser_Myst::opcode_131(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Day increase
}

void MystScriptParser_Myst::opcode_132(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Day decrease
}

void MystScriptParser_Myst::opcode_133(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used on Card 4500 (Stellar Observatory Controls)
	if (argc == 1) {
		// Called by Telescope Slew Button
		uint16 soundId = argv[0];

		// TODO: Function to change variables controlling telescope view
		//       etc.

		// TODO: Sound seems to be stuck looping?
		_vm->_sound->playSound(soundId);
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_134(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Month slider movement
}

void MystScriptParser_Myst::opcode_135(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Day slider movement
}

void MystScriptParser_Myst::opcode_136(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Year slider movement
}

void MystScriptParser_Myst::opcode_137(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Time slider movement
}

void MystScriptParser_Myst::o_circuitBreakerStartMove(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Circuit breaker start move", op);

	MystResourceType12 *breaker = static_cast<MystResourceType12 *>(_invokingResource);
	breaker->drawFrame(0);
	_vm->_cursor->setCursor(700);
	_tempVar = 0;
}

void MystScriptParser_Myst::o_circuitBreakerMove(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Circuit breaker move", op);

	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;
	MystResourceType12 *breaker = static_cast<MystResourceType12 *>(_invokingResource);

	int16 maxStep = breaker->getStepsV() - 1;
	int16 step = ((_vm->_mouse.y - 80) * breaker->getStepsV()) / 65;
	step = CLIP<uint16>(step, 0, maxStep);

	breaker->drawFrame(step);

	if (_tempVar != step) {
		_tempVar = step;

		// Breaker switched
		if (step == maxStep) {

			// Choose breaker
			if (breaker->getType8Var() == 93) {

				// Voltage is still too high or not broken
				if (myst.generatorVoltage > 59 || myst.generatorBreakers != 1) {
					uint16 soundId = breaker->getList2(1);
					if (soundId)
						_vm->_sound->playSound(soundId);
				} else {
					uint16 soundId = breaker->getList2(0);
					if (soundId)
						_vm->_sound->playSound(soundId);

					// Reset breaker state
					myst.generatorBreakers = 0;
				}
			} else {
				// Voltage is still too high or not broken
				if (myst.generatorVoltage > 59 || myst.generatorBreakers != 2) {
					uint16 soundId = breaker->getList2(1);
					if (soundId)
						_vm->_sound->playSound(soundId);
				} else {
					uint16 soundId = breaker->getList2(0);
					if (soundId)
						_vm->_sound->playSound(soundId);

					// Reset breaker state
					myst.generatorBreakers = 0;
				}
			}
		}
	}
}

void MystScriptParser_Myst::o_circuitBreakerEndMove(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Circuit breaker end move", op);

	MystResourceType12 *breaker = static_cast<MystResourceType12 *>(_invokingResource);
	_vm->redrawArea(breaker->getType8Var());
	_vm->checkCursorHints();
}

void MystScriptParser_Myst::opcode_146(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4098
	// TODO: Boiler wheel clockwise mouse down
}

void MystScriptParser_Myst::opcode_147(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	if (argc == 0) {
		// TODO: Extra Logic to do this in INIT process watching cursor and var 98?
		_vm->_varStore->setVar(98, 0);
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_149(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4098
	// TODO: Boiler wheel clockwise mouse up
}

void MystScriptParser_Myst::opcode_150(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4098
	// TODO: Boiler wheel counter-clockwise mouse down
}

void MystScriptParser_Myst::opcode_151(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4098
	// TODO: Boiler wheel counter-clockwise mouse up
}

void MystScriptParser_Myst::o_rocketSoundSliderStartMove(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Rocket slider start move", op);

	_rocketSliderSound = 0;
	_vm->_cursor->setCursor(700);
	_vm->_sound->pauseBackground();
	rocketSliderMove();
}

void MystScriptParser_Myst::o_rocketSoundSliderMove(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Rocket slider move", op);

	rocketSliderMove();
}

void MystScriptParser_Myst::o_rocketSoundSliderEndMove(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Rocket slider end move", op);

	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;

	_vm->checkCursorHints();

	if (myst.generatorVoltage == 59 && !myst.generatorBreakers) {
		if (_rocketSliderSound)
			_vm->_sound->stopSound();
	}

	if (_invokingResource == _rocketSlider1) {
		myst.rocketSliderPosition[0] = _rocketSlider1->_pos.y;
	} else if (_invokingResource == _rocketSlider2) {
		myst.rocketSliderPosition[1] = _rocketSlider2->_pos.y;
	} else if (_invokingResource == _rocketSlider3) {
		myst.rocketSliderPosition[2] = _rocketSlider3->_pos.y;
	} else if (_invokingResource == _rocketSlider4) {
		myst.rocketSliderPosition[3] = _rocketSlider4->_pos.y;
	} else if (_invokingResource == _rocketSlider5) {
		myst.rocketSliderPosition[4] = _rocketSlider5->_pos.y;
	}

	_vm->_sound->resumeBackground();
}

void MystScriptParser_Myst::rocketSliderMove() {
	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;

	MystResourceType10 *slider = static_cast<MystResourceType10 *>(_invokingResource);

	if (myst.generatorVoltage == 59 && !myst.generatorBreakers) {
		uint16 soundId = rocketSliderGetSound(slider->_pos.y);
		if (soundId != _rocketSliderSound) {
			_rocketSliderSound = soundId;
			_vm->_sound->replaceSound(soundId, Audio::Mixer::kMaxChannelVolume, true);
		}
	}
}

uint16 MystScriptParser_Myst::rocketSliderGetSound(uint16 pos) {
	return (uint16)(9530 + (pos - 216) * 35.0 * 0.01639344262295082);
}

void MystScriptParser_Myst::rocketCheckSolution() {
	_vm->_cursor->hideCursor();

	uint16 soundId;
	bool solved = true;

	soundId = rocketSliderGetSound(_rocketSlider1->_pos.y);
	_vm->_sound->replaceSound(soundId);
	_rocketSlider1->drawConditionalDataToScreen(2);
	_vm->_system->delayMillis(250);
	if (soundId != 9558)
		solved = false;

	soundId = rocketSliderGetSound(_rocketSlider2->_pos.y);
	_vm->_sound->replaceSound(soundId);
	_rocketSlider2->drawConditionalDataToScreen(2);
	_vm->_system->delayMillis(250);
	if (soundId != 9546)
		solved = false;

	soundId = rocketSliderGetSound(_rocketSlider3->_pos.y);
	_vm->_sound->replaceSound(soundId);
	_rocketSlider3->drawConditionalDataToScreen(2);
	_vm->_system->delayMillis(250);
	if (soundId != 9543)
		solved = false;

	soundId = rocketSliderGetSound(_rocketSlider4->_pos.y);
	_vm->_sound->replaceSound(soundId);
	_rocketSlider4->drawConditionalDataToScreen(2);
	_vm->_system->delayMillis(250);
	if (soundId != 9553)
		solved = false;

	soundId = rocketSliderGetSound(_rocketSlider5->_pos.y);
	_vm->_sound->replaceSound(soundId);
	_rocketSlider5->drawConditionalDataToScreen(2);
	_vm->_system->delayMillis(250);
	if (soundId != 9560)
		solved = false;

	_vm->_sound->stopSound();

	if (solved) {
		_vm->_video->playBackgroundMovie(_vm->wrapMovieFilename("selenbok", kMystStack), 224, 41, true);

		// TODO: Movie control
		// Play from 0 to 660
		// Then from 660 to 3500, looping

		_tempVar = 1;
	}

	_rocketSlider1->drawConditionalDataToScreen(1);
	_rocketSlider2->drawConditionalDataToScreen(1);
	_rocketSlider3->drawConditionalDataToScreen(1);
	_rocketSlider4->drawConditionalDataToScreen(1);
	_rocketSlider5->drawConditionalDataToScreen(1);

	_vm->_cursor->showCursor();
}

void MystScriptParser_Myst::o_rocketLeverStartMove(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Rocket lever start move", op);

	MystResourceType12 *lever = static_cast<MystResourceType12 *>(_invokingResource);

	_vm->_cursor->setCursor(700);
	_rocketLeverPosition = 0;
	lever->drawFrame(0);
}

void MystScriptParser_Myst::o_rocketOpenBook(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Rocket open link book", op);

	// TODO: Update video playing
	// Play from 3500 to 13100, looping

	// Set linkable
	_tempVar = 2;
}

void MystScriptParser_Myst::o_rocketLeverMove(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Rocket lever move", op);

	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;
	MystResourceType12 *lever = static_cast<MystResourceType12 *>(_invokingResource);

	// Make the lever follow the mouse
	int16 maxStep = lever->getStepsV() - 1;
    Common::Rect rect = lever->getRect();
    int16 step = ((_vm->_mouse.y - rect.top) * lever->getStepsV()) / rect.height();
	step = CLIP<uint16>(step, 0, maxStep);

	lever->drawFrame(step);

	// If lever pulled
	if (step == maxStep && step != _rocketLeverPosition) {
		uint16 soundId = lever->getList2(0);
		if (soundId)
			_vm->_sound->playSound(soundId);

		// If rocket correctly powered
		if (myst.generatorVoltage == 59 && !myst.generatorBreakers) {
			rocketCheckSolution();
		}
	}

	_rocketLeverPosition = step;
}

void MystScriptParser_Myst::o_rocketLeverEndMove(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Rocket lever end move", op);

	MystResourceType12 *lever = static_cast<MystResourceType12 *>(_invokingResource);

	_vm->checkCursorHints();
	_rocketLeverPosition = 0;
	lever->drawFrame(0);
}

void MystScriptParser_Myst::opcode_169(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used on Card 4099 (In Cabin, Looking Out Door)
	// TODO: Finish Implementation...
}

void MystScriptParser_Myst::opcode_170(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Month slider mouse down
}

void MystScriptParser_Myst::opcode_171(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Month slider mouse up
}

void MystScriptParser_Myst::opcode_172(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Day slider mouse down
}

void MystScriptParser_Myst::opcode_173(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Day slider mouse up
}

void MystScriptParser_Myst::opcode_174(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Year slider mouse down
}

void MystScriptParser_Myst::opcode_175(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Year slider mouse up
}

void MystScriptParser_Myst::opcode_176(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Time slider mouse down
}

void MystScriptParser_Myst::opcode_177(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Time slider mouse up
}

void MystScriptParser_Myst::o_libraryCombinationBookStop(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Combiation book stop turning pages", op);
	_libraryCombinationBookPagesTurning = false;
}

void MystScriptParser_Myst::opcode_181(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	if (argc == 0) {
		// TODO: Logic for lighting the match
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_182(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	if (argc == 0) {
		// TODO: Logic for lighting the match
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_183(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	if (argc == 0) {
		// Used for Myst Cards 4257, 4260, 4263, 4266, 4269, 4272, 4275 and 4278 (Ship Puzzle Boxes)
		_vm->_varStore->setVar(105, 1);
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_184(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	if (argc == 0) {
		// Used for Myst Cards 4257, 4260, 4263, 4266, 4269, 4272, 4275 and 4278 (Ship Puzzle Boxes)
		_vm->_varStore->setVar(105, 0);
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_185(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	if (argc == 0) {
		// Used for Myst Card 4098 (Cabin Boiler Pilot Light)
		// TODO: Extra Logic to do this in INIT process watching cursor and var 98?
		_vm->_varStore->setVar(98, 1);
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::o_clockMinuteWheelStartTurn(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4006
	debugC(kDebugScript, "Opcode %d: Minute wheel start turn", op);

	clockWheelStartTurn(2);
}

void MystScriptParser_Myst::o_clockWheelEndTurn(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4006
	debugC(kDebugScript, "Opcode %d: Wheel end turn", op);

	_clockTurningWheel = 0;
}

void MystScriptParser_Myst::o_clockHourWheelStartTurn(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4006
	debugC(kDebugScript, "Opcode %d: Hour wheel start turn", op);

	clockWheelStartTurn(1);
}

void MystScriptParser_Myst::clockWheel_run() {
	// Turn wheel one step each second
	uint32 time = _vm->_system->getMillis();
	if (time > _startTime + 1000) {
		_startTime = time;

		if (_clockTurningWheel == 1) {
			clockWheelTurn(39);
		} else {
			clockWheelTurn(38);
		}
		_vm->redrawArea(37);
	}

}

void MystScriptParser_Myst::clockWheelStartTurn(uint16 wheel) {
	MystResourceType11 *resource = static_cast<MystResourceType11 *>(_invokingResource);
	uint16 soundId = resource->getList1(0);
	if (soundId)
		_vm->_sound->playSound(soundId);

	// Turn wheel one step
	if (wheel == 1) {
		clockWheelTurn(39);
	} else {
		clockWheelTurn(38);
	}
	_vm->redrawArea(37);

	// Continue turning wheel until mouse button is released
	_clockTurningWheel = wheel;
	_startTime = _vm->_system->getMillis();
}

void MystScriptParser_Myst::clockWheelTurn(uint16 var) {
	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;

	if (var == 38) {
		// Hours
		myst.clockTowerHourPosition = (myst.clockTowerHourPosition + 1) % 12;
	} else {
		// Minutes
		myst.clockTowerMinutePosition = (myst.clockTowerMinutePosition + 5) % 60;
	}
}

void MystScriptParser_Myst::o_libraryCombinationBookStartRight(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Combination book start turning pages right", op);

	_tempVar = 0;
	libraryCombinationBookTurnRight();
	_startTime = _vm->_system->getMillis();
	_libraryCombinationBookPagesTurning = true;
}

void MystScriptParser_Myst::o_libraryCombinationBookStartLeft(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Combination book start turning pages left", op);

	_tempVar = 0;
	libraryCombinationBookTurnLeft();
	_startTime = _vm->_system->getMillis();
	_libraryCombinationBookPagesTurning = true;
}

void MystScriptParser_Myst::libraryCombinationBookTurnLeft() {
	// Turn page left
	if (_libraryBookPage - 1 >=  0) {
		_tempVar--;

		if (_tempVar >= -5) {
			_libraryBookPage--;
		} else {
			_libraryBookPage -= 5;
			_tempVar = -5;
		}

		_libraryBookPage = CLIP<int16>(_libraryBookPage, 0, _libraryBookNumPages - 1);

		Common::Rect rect = Common::Rect(157, 115, 544, 333);
		_vm->_gfx->copyImageToScreen(_libraryBookBaseImage + _libraryBookPage, rect);

		if (_vm->_rnd->getRandomBit())
			_vm->_sound->playSound(_libraryBookSound1);
		else
			_vm->_sound->playSound(_libraryBookSound2);

		_vm->_gfx->updateScreen();
	}
}

void MystScriptParser_Myst::libraryCombinationBookTurnRight() {
	// Turn page right
	if (_libraryBookPage + 1 < _libraryBookNumPages) {
		_tempVar++;

		if (_tempVar <= 5) {
			_libraryBookPage++;
		} else {
			_libraryBookPage += 5;
			_tempVar = 5;
		}

		_libraryBookPage = CLIP<uint16>(_libraryBookPage, 0, _libraryBookNumPages - 1);

		Common::Rect rect = Common::Rect(157, 115, 544, 333);
		_vm->_gfx->copyImageToScreen(_libraryBookBaseImage + _libraryBookPage, rect);

		if (_vm->_rnd->getRandomBit())
			_vm->_sound->playSound(_libraryBookSound1);
		else
			_vm->_sound->playSound(_libraryBookSound2);

		_vm->_gfx->updateScreen();
	}
}

void MystScriptParser_Myst::libraryCombinationBook_run() {
	uint32 time = _vm->_system->getMillis();
	if (time >= _startTime + 500) {
		if (_tempVar > 0) {
			libraryCombinationBookTurnRight();
			_startTime = time;
		} else if (_tempVar < 0) {
			libraryCombinationBookTurnLeft();
			_startTime = time;
		}
	}
}

void MystScriptParser_Myst::opcode_192(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Time increase
}

void MystScriptParser_Myst::opcode_194(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Constellation drawing
}

void MystScriptParser_Myst::opcode_195(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Time decrease
}

void MystScriptParser_Myst::opcode_196(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Year increase
}

void MystScriptParser_Myst::opcode_197(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Card 4500
	// TODO: Year decrease
}

void MystScriptParser_Myst::o_dockVaultForceClose(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used on Myst 4143 (Dock near Marker Switch)
	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;
	uint16 soundId = argv[0];
	uint16 delay = argv[1];
	uint16 directionalUpdateDataSize = argv[2];

	debugC(kDebugScript, "Opcode %d: Vault Force Close", op);
	debugC(kDebugScript, "\tsoundId: %d", soundId);
	debugC(kDebugScript, "\tdirectionalUpdateDataSize: %d", directionalUpdateDataSize);

	if (_dockVaultState) {
		// Open switch
		myst.dockMarkerSwitch = 1;
		_vm->_sound->playSound(4143);
		_vm->redrawArea(4);

		// Close vault
		_dockVaultState = 0;
		_vm->_sound->playSound(soundId);
		_vm->redrawArea(41, false);
		animatedUpdate(directionalUpdateDataSize, &argv[3], delay);
	}
}

void MystScriptParser_Myst::opcode_199(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	if (argc == 0) {
		debugC(kDebugScript, "Opcode %d: Myst Imager Control Execute Button Logic", op);

		uint16 numericSelection = (_vm->_varStore->getVar(36) + 1) % 10;
		numericSelection += ((_vm->_varStore->getVar(35) + 1) % 10) * 10;

		debugC(kDebugScript, "\tImager Selection: %d", numericSelection);

		switch (numericSelection) {
		case 40:
			_vm->_varStore->setVar(51, 1); // Mountain
			break;
		case 67:
			_vm->_varStore->setVar(51, 2); // Water
			break;
		case 47:
			_vm->_varStore->setVar(51, 4); // Marker Switch
			break;
		case 8:
			_vm->_varStore->setVar(51, 3); // Atrus
			break;
		default:
			_vm->_varStore->setVar(51, 0); // Blank
			break;
		}

		// TODO: Fill in Logic
		//{  34, 2, "Dock Forechamber Imager State" }, // 0 to 2 = Off, Mountain, Water
		//{ 310, 0, "Dock Forechamber Imager Control Temp Value?" }
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::o_libraryBook_init(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	_libraryBookPage = 0;
	_libraryBookNumPages = argv[0];
	_libraryBookBaseImage = argv[1];
	_libraryBookSound1 = argv[2];
	_libraryBookSound2 = argv[3];
}

static struct {
	uint16 lastVar105;
	uint16 soundId;

	bool enabled;
} g_opcode201Parameters;

void MystScriptParser_Myst::opcode_201_run() {
	if (g_opcode201Parameters.enabled) {
		uint16 var105 = _vm->_varStore->getVar(105);

		if (var105 && !g_opcode201Parameters.lastVar105)
			_vm->_sound->playSound(g_opcode201Parameters.soundId);

		g_opcode201Parameters.lastVar105 = var105;
	}
}

void MystScriptParser_Myst::opcode_201_disable() {
	g_opcode201Parameters.enabled = false;
	g_opcode201Parameters.soundId = 0;
	g_opcode201Parameters.lastVar105 = 0;
}

void MystScriptParser_Myst::opcode_201(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Cards 4257, 4260, 4263, 4266, 4269, 4272, 4275 and 4278 (Ship Puzzle Boxes)
	if (argc == 1) {
		g_opcode201Parameters.soundId = argv[0];
		g_opcode201Parameters.lastVar105 = 0;
		g_opcode201Parameters.enabled = true;
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::towerRotationMap_run() {
	if (!_towerRotationMapInitialized) {
		_towerRotationMapInitialized = true;
		_vm->_sound->playSound(4378);

		// Draw library
		_vm->redrawArea(304, false);

		// Draw other resources
		for (uint i = 1; i <= 10; i++) {
			MystResourceType8 *resource = static_cast<MystResourceType8 *>(_vm->_resources[i]);
			_vm->redrawResource(resource, false);
		}

		// Draw to screen
		_vm->_gfx->updateScreen();
	}

	uint32 time = _vm->_system->getMillis();
	if (time > _startTime) {
		if (_towerRotationMapClicked) {
			// TODO: handle tower rotation
			_startTime = time + 100;
		} else {
			// Blink tower
			_startTime = time + 500;
			_tempVar = (_tempVar + 1) % 2;
			_towerRotationMapTower->drawConditionalDataToScreen(_tempVar);
		}
	}
}

void MystScriptParser_Myst::o_towerRotationMap_init(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	_towerRotationMapRunning = true;
	_towerRotationMapTower = static_cast<MystResourceType11 *>(_invokingResource);
	_towerRotationMapLabel = static_cast<MystResourceType8 *>(_vm->_resources[argv[0]]);
	_tempVar = 0;
	_startTime = 0;
	_towerRotationMapClicked = false;
}

void MystScriptParser_Myst::o_forechamberDoor_init(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used for Card 4138 (Dock Forechamber Door)
	// Set forechamber door to closed
	_tempVar = 0;
}

void MystScriptParser_Myst::opcode_204(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4134 and 4149 (Dock)
}

static struct {
	bool enabled;
} g_opcode205Parameters;

void MystScriptParser_Myst::opcode_205_run(void) {
	if (g_opcode205Parameters.enabled) {
		// Used for Card 4532 (Rocketship Piano)
		// TODO: Fill in function...
	}
}

void MystScriptParser_Myst::opcode_205_disable(void) {
	g_opcode205Parameters.enabled = false;
}

void MystScriptParser_Myst::opcode_205(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4532 (Rocketship Piano)

	if (argc == 0)
		g_opcode205Parameters.enabled = true;
	else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_206(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4256 (Butterfly Movie Activation)
	// TODO: Implement Logic...
}

void MystScriptParser_Myst::opcode_208(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	if (argc == 0) {
		debugC(kDebugScript, "Opcode %d: Imager Function", op);
		debugC(kDebugScript, "Var: %d", var);

		// TODO: Fill in Correct Function
		if (false) {
			_vm->_video->playMovie(_vm->wrapMovieFilename("vltmntn", kMystStack), 159, 97);
		}
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::libraryBookcaseTransform_run(void) {
	if (_libraryBookcaseChanged) {
		_libraryBookcaseChanged = false;
		_libraryBookcaseMoving = false;

		// Play transform sound and video
		_vm->_sound->playSound(_libraryBookcaseSoundId);
		_libraryBookcaseMovie->playMovie();
	}
}

void MystScriptParser_Myst::o_libraryBookcaseTransform_init(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	if (_libraryBookcaseChanged) {
		MystResourceType7 *resource = static_cast<MystResourceType7 *>(_invokingResource);
		_libraryBookcaseMovie = static_cast<MystResourceType6 *>(resource->getSubResource(getVar(0)));
		_libraryBookcaseSoundId = argv[0];
		_libraryBookcaseMoving = true;
	}
}

void MystScriptParser_Myst::generatorControlRoom_run(void) {
	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;

	if (_generatorVoltage == myst.generatorVoltage) {
		generatorRedrawRocket();
	} else {
		// Animate generator gauge		
		if (_generatorVoltage > myst.generatorVoltage)
			_generatorVoltage--;
		else
			_generatorVoltage++;

		// Redraw generator gauge
		_vm->redrawArea(62);
		_vm->redrawArea(63);
		_vm->redrawArea(96);
	}
}

void MystScriptParser_Myst::o_generatorControlRoom_init(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;

	debugC(kDebugScript, "Opcode %d: Generator control room init", op);

	_generatorVoltage = myst.generatorVoltage;
	_generatorControlRoomRunning = true;
}

void MystScriptParser_Myst::o_fireplace_init(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Fireplace grid init", op);

	// Clear fireplace grid
	for (uint i = 0; i < 6; i++)
		_fireplaceLines[i] = 0;
}

static struct {
	bool enabled;
} g_opcode212Parameters;

void MystScriptParser_Myst::opcode_212_run(void) {
	if (g_opcode212Parameters.enabled) {
		// TODO: Implement Correct Code for Myst Clock Tower Cog Puzzle
		// Card 4113

		if (false) {
			// 3 videos to be played of Cog Movement
			// TODO: Not 100% sure of movie positions.
			_vm->_video->playMovie(_vm->wrapMovieFilename("cl1wg1", kMystStack), 220, 50);
			_vm->_video->playMovie(_vm->wrapMovieFilename("cl1wg2", kMystStack), 220, 80);
			_vm->_video->playMovie(_vm->wrapMovieFilename("cl1wg3", kMystStack), 220, 110);

			// 1 video of weight descent
			_vm->_video->playMovie(_vm->wrapMovieFilename("cl1wlfch", kMystStack), 123, 0);

			// Video of Cog Open on Success
			_vm->_video->playMovie(_vm->wrapMovieFilename("cl1wggat", kMystStack), 195, 225);
			// Var 40 set on success
			_vm->_varStore->setVar(40, 1);
		}
	}
}

void MystScriptParser_Myst::opcode_212_disable(void) {
	g_opcode212Parameters.enabled = false;
}

void MystScriptParser_Myst::opcode_212(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4113 (Clock Tower Cog Puzzle)
	if (argc == 0)
		g_opcode212Parameters.enabled = true;
	else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_213(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4524 (Dockside Facing Towards Ship)
	if (argc == 0) {
		// TODO: Implement Code...
		// Code for Gull Videos?
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_214(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4500 (Stellar Observatory)
	if (argc == 5) {
		debugC(kDebugScript, "Opcode %d: Unknown...", op);

		uint16 u0 = argv[0];
		uint16 u1 = argv[1];
		uint16 u2 = argv[2];
		uint16 u3 = argv[3];
		uint16 u4 = argv[4];

		debugC(kDebugScript, "\tu0: %d", u0);
		debugC(kDebugScript, "\tu1: %d", u1);
		debugC(kDebugScript, "\tu2: %d", u2);
		debugC(kDebugScript, "\tu3: %d", u3);
		debugC(kDebugScript, "\tu4: %d", u4);
		// TODO: Complete Implementation...
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_215(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4134 (Dock Facing Marker Switch)
	// TODO: Fill in logic for Gull Videos.
	//       may be offset and overlap and need video update to all these
	//       to run in sequence with opcode215_run() process...
	if (false) {
		// All birds(x) videos are 120x48 and played in top right corner of card
		_vm->_video->playMovie(_vm->wrapMovieFilename("birds1", kMystStack), 544-120-1, 0);
		_vm->_video->playMovie(_vm->wrapMovieFilename("birds2", kMystStack), 544-120-1, 0);
		_vm->_video->playMovie(_vm->wrapMovieFilename("birds3", kMystStack), 544-120-1, 0);
	}
}

void MystScriptParser_Myst::opcode_216(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Cards 4571 (Channelwood Tree), 4586 (Channelwood Tree), 
	// 4615 (Channelwood Tree) and 4601 (Channelwood Tree)
	if (argc == 0) {
		// TODO: Fill in logic for Channelwood Tree Position i.e. Var 72 update // 0 to 12, 4 for Alcove
		// Based on Timer code and following variables :
		// 98  "Cabin Boiler Pilot Light Lit"
		// 99  "Cabin Boiler Gas Valve Position" }, // 0 to 5
		// 305 "Cabin Boiler Lit" },
		// 306 "Cabin Boiler Steam Sound Control" }, // 0 to 27
		// 307 "Cabin Boiler Needle Position i.e. Fully Pressurised" }, // 0 to 1

		// Note : Opcode 218 does boiler update code..
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_217(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4601 (Channelwood Tree)
	if (argc == 2) {
		// TODO: Fill in logic for Tree Position Close Up...
		// 2 arguments: 4, 4
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_218(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4097 (Cabin Boiler)
	// TODO: Fill in logic
	if (false) {
		_vm->_video->playMovie(_vm->wrapMovieFilename("cabfirfr", kMystStack), 254, 244);
		_vm->_video->playMovie(_vm->wrapMovieFilename("cabcgfar", kMystStack), 254, 138);
	}

	// Used for Card 4098 (Cabin Boiler)
	// TODO: Fill in logic
	if (false) {
		_vm->_video->playMovie(_vm->wrapMovieFilename("cabfire", kMystStack), 240, 279);
		_vm->_video->playMovie(_vm->wrapMovieFilename("cabingau", kMystStack), 243, 97);
	}
}

void MystScriptParser_Myst::o_rocketSliders_init(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Rocket sliders init", op);

	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;

	_rocketSlider1 = static_cast<MystResourceType10 *>(_vm->_resources[argv[0]]);
	_rocketSlider2 = static_cast<MystResourceType10 *>(_vm->_resources[argv[1]]);
	_rocketSlider3 = static_cast<MystResourceType10 *>(_vm->_resources[argv[2]]);
	_rocketSlider4 = static_cast<MystResourceType10 *>(_vm->_resources[argv[3]]);
	_rocketSlider5 = static_cast<MystResourceType10 *>(_vm->_resources[argv[4]]);

	if (myst.rocketSliderPosition[0]) {
		_rocketSlider1->setPosition(myst.rocketSliderPosition[0]);
	}
	if (myst.rocketSliderPosition[1]) {
		_rocketSlider2->setPosition(myst.rocketSliderPosition[1]);
	}
	if (myst.rocketSliderPosition[2]) {
		_rocketSlider3->setPosition(myst.rocketSliderPosition[2]);
	}
	if (myst.rocketSliderPosition[3]) {
		_rocketSlider4->setPosition(myst.rocketSliderPosition[3]);
	}
	if (myst.rocketSliderPosition[4]) {
		_rocketSlider5->setPosition(myst.rocketSliderPosition[4]);
	}
}

void MystScriptParser_Myst::o_rocketLinkVideo_init(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	debugC(kDebugScript, "Opcode %d: Rocket link video init", op);
	_tempVar = 0;
}

void MystScriptParser_Myst::o_greenBook_init(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used for Card 4168 (Green Book Movies)
	debugC(kDebugScript, "Opcode %d: Green book init", op);

	MystVariables::Globals &globals = _vm->_saveLoad->_v->globals;
	MystVariables::Myst &myst = _vm->_saveLoad->_v->myst;

	uint loopStart = 0;
	uint loopEnd = 0;
	Common::String file;

	if (!myst.greenBookOpenedBefore) {
		loopStart = 113200;
		loopEnd = 116400;
		file = _vm->wrapMovieFilename("atrusbk1", kMystStack);
	} else {
		loopStart = 8800;
		loopEnd = 9700;
		file = _vm->wrapMovieFilename("atrusbk2", kMystStack);
	}

	_vm->_sound->stopSound();
	_vm->_sound->pauseBackground();

	if (globals.ending != 4) {
		_vm->_video->playBackgroundMovie(file, 314, 76);
	}

	// TODO: Movie play control
	// loop between loopStart and loopEnd
}

void MystScriptParser_Myst::opcode_222(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4141 (Myst Dock Facing Sea)
	if (argc == 0) {
		// TODO: Logic for Gull Videos?
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_300(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used in Card 4371 (Blue Book) Var = 101
	//     and Card 4363 (Red Book)  Var = 100
	debugC(kDebugScript, "Opcode %d: Book Exit Function...", op);
	debugC(kDebugScript, "Var: %d", var);
	// TODO: Fill in Logic
}

void MystScriptParser_Myst::opcode_301(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used in Cards 4047, 4059, 4060, 4068 and 4080 (Myst Library Books - Open)
	// TODO: Fill in Logic. Clear Variable on Book exit.. or Copy from duplicate..
	_vm->_varStore->setVar(0, 1);
}

void MystScriptParser_Myst::opcode_302(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	// Used in Card 4113 (Clock Tower Cog Puzzle)
	// TODO: Fill in Logic
}

void MystScriptParser_Myst::opcode_303(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4134 (Dock Facing Marker Switch)
	// Used for Card 4141 (Myst Dock Facing Sea)
	// In the original engine, this opcode stopped Gull Movies if playing,
	// upon card change, but this behavior is now default in this engine.
}

void MystScriptParser_Myst::opcode_304(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4601 (Channelwood Tree)
	if (argc == 0) {
		debugC(kDebugScript, "Opcode %d: Unknown...", op);
		// TODO: Logic for clearing variable?
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_305(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4601 (Channelwood Tree)
	if (argc == 0) {
		debugC(kDebugScript, "Opcode %d: Unknown...", op);
		// TODO: Logic for clearing variable?
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_306(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4098 (Cabin Boiler Puzzle)
	// In the original engine, this opcode stopped the Boiler Fire and Meter Needle videos
	// if playing, upon card change, but this behavior is now default in this engine.
}

void MystScriptParser_Myst::opcode_307(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4297 (Generator Room Controls)
	if (argc == 0) {
		debugC(kDebugScript, "Opcode %d: Unknown...", op);
		// TODO: Logic for clearing variable?
	} else
		unknown(op, var, argc, argv);
}

void MystScriptParser_Myst::opcode_308(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4530 (Rocketship Music Sliders)
	// In the original engine, this opcode stopped the Selenitic Book Movie if playing,
	// upon card change, but this behavior is now default in this engine.
}

void MystScriptParser_Myst::opcode_309(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4168 (Green D'ni Book Open)
	// In the original engine, this opcode stopped the Green Book Atrus Movies if playing,
	// upon card change, but this behavior is now default in this engine.
}

void MystScriptParser_Myst::opcode_312(uint16 op, uint16 var, uint16 argc, uint16 *argv) {
	varUnusedCheck(op, var);

	// Used for Card 4698 (Dock Forechamber Imager)
	// In the original engine, this opcode stopped the Imager Movie if playing,
	// especially the hardcoded Topological Extrusion (Mountain) video,
	// upon card change, but this behavior is now default in this engine.
}

} // End of namespace Mohawk

/* ScummVM - Scumm Interpreter
 * Copyright (C) 2004 The ScummVM project
 *
 * The ReInherit Engine is (C)2000-2003 by Daniel Balsom.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */

// Scripting module script function component

#include "saga/saga.h"

#include "saga/gfx.h"
#include "saga/actor.h"
#include "saga/animation.h"
#include "saga/console.h"
#include "saga/interface.h"
#include "saga/music.h"
#include "saga/objectdata.h"
#include "saga/sound.h"
#include "saga/sndres.h"

#include "saga/script.h"
#include "saga/sdata.h"

#include "saga/scene.h"

namespace Saga {

#define OPCODE(x) &Script::x

void Script::setupScriptFuncList(void) {
	static const SFunc_T SFuncList[SFUNC_NUM] = {
		OPCODE(SF_putString),
		OPCODE(SF_sleep),
		OPCODE(SF_takeObject),
		OPCODE(SF_objectIsCarried),
		OPCODE(SF_setStatusText),
		OPCODE(SF_commandMode),
		OPCODE(SF_actorWalkTo),
		OPCODE(SF_doAction),
		OPCODE(SF_setFacing),
		OPCODE(SF_startBgdAnim),
		OPCODE(SF_stopBgdAnim),
		OPCODE(SF_freezeInterface),
		OPCODE(SF_dialogMode),
		OPCODE(SF_killActorThreads),
		OPCODE(SF_faceTowards),
		OPCODE(SF_setFollower),
		OPCODE(SF_gotoScene),
		OPCODE(SF_setObjImage),
		OPCODE(SF_setObjName),
		OPCODE(SF_getObjImage),
		OPCODE(SF_getNumber),
		OPCODE(SF_openDoor),
		OPCODE(SF_closeDoor),
		OPCODE(SF_setBgdAnimSpeed),
		OPCODE(SF_cycleColors),
		OPCODE(SF_centerActor),
		OPCODE(SF_startAnim),
		OPCODE(SF_actorWalkToAsync),
		OPCODE(SF_enableZone),
		OPCODE(SF_setActorState),
		OPCODE(SF_moveTo),
		OPCODE(SF_sceneEq),
		OPCODE(SF_dropObject),
		OPCODE(SF_finishBgdAnim),
		OPCODE(SF_swapActors),
		OPCODE(SF_simulSpeech),
		OPCODE(SF_actorWalk),
		OPCODE(SF_cycleActorFrames),
		OPCODE(SF_setFrame),
		OPCODE(SF_setRightPortrait),
		OPCODE(SF_setLeftPortrait),
		OPCODE(SF_linkAnim),
		OPCODE(SF_scriptSpecialWalk),
		OPCODE(SF_placeActor),
		OPCODE(SF_checkUserInterrupt),
		OPCODE(SF_walkRelative),
		OPCODE(SF_moveRelative),
		OPCODE(SF_simulSpeech2),
		OPCODE(SF_placard),
		OPCODE(SF_placardOff),
		OPCODE(SF_setProtagState),
		OPCODE(SF_resumeBgdAnim),
		OPCODE(SF_throwActor),
		OPCODE(SF_waitWalk),
		OPCODE(SF_sceneID),
		OPCODE(SF_changeActorScene),
		OPCODE(SF_climb),
		OPCODE(SF_setDoorState),
		OPCODE(SF_setActorZ),
		OPCODE(SF_text),
		OPCODE(SF_getActorX),
		OPCODE(SF_getActorY),
		OPCODE(SF_eraseDelta),
		OPCODE(SF_playMusic),
		OPCODE(SF_pickClimbOutPos),
		OPCODE(SF_tossRif),
		OPCODE(SF_showControls),
		OPCODE(SF_showMap),
		OPCODE(SF_puzzleWon),
		OPCODE(SF_enableEscape),
		OPCODE(SF_playSound),
		OPCODE(SF_playLoopedSound),
		OPCODE(SF_getDeltaFrame),
		OPCODE(SF_showProtect),
		OPCODE(SF_protectResult),
		OPCODE(SF_rand),
		OPCODE(SF_fadeMusic),
		OPCODE(SF_playVoice)
	};
	_SFuncList = SFuncList;
}

// Script function #0 (0x00)
// Print a debugging message
int Script::SF_putString(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();

	_vm->_console->DebugPrintf(getString(param));
	return SUCCESS;
}

// Script function #1 (0x01) blocking
// Suspends thread execution for the specified time period
int Script::SF_sleep(SCRIPTFUNC_PARAMS) {
	SDataWord_T time_param;
	long time;

	if (!_skipSpeeches) {
		time_param = thread->pop();
		time = _vm->_sdata->readWordU(time_param);
		thread->wait(kWaitTypeDelay); // put thread to sleep
		thread->sleepTime = ticksToMSec(time);
	}
	return SUCCESS;
}

// Script function #2 (0x02)
int Script::SF_takeObject(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();
	int index = param & 0x1FFF;

	if (index >= ARRAYSIZE(ObjectTable)) {
		return FAILURE;
	}

	if (ObjectTable[index].sceneIndex != -1) {
		ObjectTable[index].sceneIndex = -1;
		_vm->_interface->addToInventory(index);
	}

	return SUCCESS;
}

// Script function #3 (0x03)
// Check if an object is carried.
int Script::SF_objectIsCarried(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();
	int index = param & 0x1FFF;

	if (index >= ARRAYSIZE(ObjectTable)) {
		thread->retVal = 0;
		return FAILURE;
	}

	thread->retVal = (ObjectTable[index].sceneIndex == -1) ? 1 : 0;
	return SUCCESS;
}

// Script function #4 (0x04) nonblocking
// Set the command display to the specified text string
// Param1: dialogue index of string
int Script::SF_setStatusText(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();

	return _vm->_interface->setStatusText(getString(param));
}

// Script function #5 (0x05)
int Script::SF_commandMode(SCRIPTFUNC_PARAMS) {
	return _vm->_interface->setMode(kPanelCommand);
}

// Script function #6 (0x06) blocking
// Commands the specified actor to walk to the given position
// Param1: actor id
// Param2: actor destination x
// Param3: actor destination y
int Script::SF_actorWalkTo(SCRIPTFUNC_PARAMS) {
	SDataWord_T actor_parm;
	SDataWord_T x_parm;
	SDataWord_T y_parm;
	uint16 actorId;
	Point pt;

	actor_parm = thread->pop();
	x_parm = thread->pop();
	y_parm = thread->pop();

	actorId = _vm->_sdata->readWordS(actor_parm);

	pt.x = _vm->_sdata->readWordS(x_parm);
	pt.y = _vm->_sdata->readWordS(y_parm);

	_vm->_actor->walkTo(actorId, &pt, 0, &thread->sem);

	return SUCCESS;
}

// Script function #7 (0x07)
int Script::SF_doAction(SCRIPTFUNC_PARAMS) {
	SDataWord_T actor_parm = thread->pop();
	SDataWord_T action_parm = thread->pop();
	SDataWord_T obj_parm = thread->pop();
	SDataWord_T withobj_parm = thread->pop();

	// The parameters correspond with the thread variables.

	debug(1, "stub: SF_doAction(%d, %d, %d, %d)", actor_parm, action_parm, obj_parm, withobj_parm);
	return SUCCESS;
}

// Script function #8 (0x08) nonblocking
// Sets the orientation of the specified actor.
// Param1: actor id
// Param2: actor orientation
int Script::SF_setFacing(SCRIPTFUNC_PARAMS) {
	SDataWord_T actor_parm;
	SDataWord_T orient_parm;
	uint16 actorId;
	int orientation;

	actor_parm = thread->pop();
	orient_parm = thread->pop();

	actorId = _vm->_sdata->readWordS(actor_parm);
	orientation = _vm->_sdata->readWordS(orient_parm);

	_vm->_actor->setOrientation(actorId, orientation);
	return SUCCESS;
}

// Script function #9 (0x09)
int Script::SF_startBgdAnim(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();

	debug(1, "stub: SF_startBgdAnim(%d, %d)", param1, param2);
	return SUCCESS;
}

// Script function #10 (0x0A)
int Script::SF_stopBgdAnim(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();

	debug(1, "stub: SF_stopBgdAnim(%d)", param);
	return SUCCESS;
}

// Script function #11 (0x0B) nonblocking
// If the parameter is true, the user interface is disabled while script
// continues to run. If the parameter is false, the user interface is 
// reenabled.
// Param1: boolean
int Script::SF_freezeInterface(SCRIPTFUNC_PARAMS) {
	SDataWord_T b_param;

	b_param = thread->pop();

	if (b_param) {
		_vm->_interface->deactivate();
	} else {
		_vm->_interface->activate();
	}

	return SUCCESS;
}

// Script function #12 (0x0C)
// Disables mouse input, etc.
int Script::SF_dialogMode(SCRIPTFUNC_PARAMS) {
	return _vm->_interface->setMode(kPanelDialogue);
}

// Script function #13 (0x0D)
int Script::SF_killActorThreads(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_killActorThreads(), %d args", nArgs);
	return SUCCESS;
}

// Script function #14 (0x0E)
int Script::SF_faceTowards(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();

	debug(1, "stub: SF_faceTowards(%d, %d)", param1, param2);
	return SUCCESS;
}

// Script function #15 (0x0F)
int Script::SF_setFollower(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();

	debug(1, "stub: SF_setFollower(%d, %d)", param1, param2);
	return SUCCESS;
}

// Script function #16 (0x10)
int Script::SF_gotoScene(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();

	debug(1, "stub: SF_gotoScene(%d, %d)", param1, param2);
	return SUCCESS;
}

// Script function #17 (0x11)
int Script::SF_setObjImage(SCRIPTFUNC_PARAMS) {
	SDataWord_T obj_param = thread->pop();
	SDataWord_T sprite_param = thread->pop();

	int index = obj_param & 0x1FFF;

	if (index >= ARRAYSIZE(ObjectTable)) {
		return FAILURE;
	}

	ObjectTable[index].spritelistRn = sprite_param + 9;
	_vm->_interface->draw();

	return SUCCESS;
}

// Script function #18 (0x12)
int Script::SF_setObjName(SCRIPTFUNC_PARAMS) {
	SDataWord_T obj_param = thread->pop();
	SDataWord_T name_param = thread->pop();

	int index = obj_param & 0x1FFF;

	if (index >= ARRAYSIZE(ObjectTable)) {
		return FAILURE;
	}

	ObjectTable[index].nameIndex = name_param;
	return SUCCESS;
}

// Script function #19 (0x13)
int Script::SF_getObjImage(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();
	int index = param & 0x1FFF;

	if (index >= ARRAYSIZE(ObjectTable)) {
		thread->retVal = 0;
		return FAILURE;
	}

	thread->retVal = ObjectTable[index].spritelistRn;
	return SUCCESS;
}

// Script function #20 (0x14)
int Script::SF_getNumber(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_getNumber(), %d args", nArgs);
	return SUCCESS;
}

// Script function #21 (0x15)
int Script::SF_openDoor(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_openDoor(), %d args", nArgs);
	return SUCCESS;
}

// Script function #22 (0x16)
int Script::SF_closeDoor(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_closeDoor(), %d args", nArgs);
	return SUCCESS;
}

// Script function #23 (0x17)
int Script::SF_setBgdAnimSpeed(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();

	debug(1, "stub: SF_setBgdAnimSpeed(%d, %d)", param1, param2);
	return SUCCESS;
}

// Script function #24 (0x18)
int Script::SF_cycleColors(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_cycleColors(), %d args", nArgs);
	return SUCCESS;
}

// Script function #25 (0x19)
int Script::SF_centerActor(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();

	debug(1, "stub: SF_centerActor(%d)", param);
	return SUCCESS;
}

// Script function #26 (0x1A) nonblocking
// Starts the specified animation 
// Param1: ?
// Param2: frames of animation to play or -1 to loop
// Param3: animation id
int Script::SF_startAnim(SCRIPTFUNC_PARAMS) {
// FIXME: implementation is wrong. Should link animation
	SDataWord_T timer_parm;
	SDataWord_T frame_parm;
	SDataWord_T anim_id_parm;
	int frame_count;
	int anim_id;

	anim_id_parm = thread->pop();
	frame_parm = thread->pop();
	timer_parm = thread->pop();

	frame_count = _vm->_sdata->readWordS(frame_parm);
	anim_id = _vm->_sdata->readWordS(anim_id_parm);

	if (_vm->_anim->play(anim_id, 0) != SUCCESS) {
		_vm->_console->DebugPrintf(S_WARN_PREFIX "SF.26: Anim::play() failed. Anim id: %u\n", anim_id);
		return FAILURE;
	}

	return SUCCESS;
}

// Script function #27 (0x1B) nonblocking
// Commands the specified actor to walk to the given position
// Param1: actor id
// Param2: actor destination x
// Param3: actor destination y
int Script::SF_actorWalkToAsync(SCRIPTFUNC_PARAMS) {
	SDataWord_T actor_parm;
	SDataWord_T x_parm;
	SDataWord_T y_parm;
	uint16 actorId;
	Point pt;

	actor_parm = thread->pop();

	x_parm = thread->pop();
	y_parm = thread->pop();

	actorId = _vm->_sdata->readWordS(actor_parm);

	pt.x = _vm->_sdata->readWordS(x_parm);
	pt.y = _vm->_sdata->readWordS(y_parm);
	_vm->_actor->walkTo(actorId, &pt, 0, NULL);

	return SUCCESS;
}

// Script function #28 (0x1C)
int Script::SF_enableZone(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_enableZone(), %d args", nArgs);
	return SUCCESS;
}

// Script function #29 (0x1D)
int Script::SF_setActorState(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();

	debug(1, "stub: SF_setActorState(%d, %d)", param1, param2);
	return SUCCESS;
}

// Script function #30 (0x1E) nonblocking
// Positions an actor at the specified location; actor is created if the 
// actor does not already exist.
// Param1: actor id
// Param2: actor pos x
// Param3: actor pos y
int Script::SF_moveTo(SCRIPTFUNC_PARAMS) {
	SDataWord_T actor_parm;
	SDataWord_T x_parm;
	SDataWord_T y_parm;
	uint16 actorId;
	Point pt;

	actor_parm = thread->pop();
	x_parm = thread->pop();
	y_parm = thread->pop();

	actorId = _vm->_sdata->readWordS(actor_parm);
	pt.x = _vm->_sdata->readWordS(x_parm);
	pt.y = _vm->_sdata->readWordS(y_parm);

	_vm->_actor->move(actorId, pt);

	return SUCCESS;
}

// Script function #31 (0x21)
int Script::SF_sceneEq(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();

	if (_vm->_scene->getSceneLUT(param) == _vm->_scene->currentSceneNumber())
		thread->retVal = 1;
	else 
		thread->retVal = 0;
	return SUCCESS;
}

// Script function #32 (0x20)
int Script::SF_dropObject(SCRIPTFUNC_PARAMS) {
	SDataWord_T obj_param = thread->pop();
	SDataWord_T sprite_param = thread->pop();
	SDataWord_T x_param = thread->pop();
	SDataWord_T y_param = thread->pop();

	int index = obj_param & 0x1FFF;

	if (index >= ARRAYSIZE(ObjectTable)) {
		return FAILURE;
	}

	if (ObjectTable[index].sceneIndex == -1) {
		_vm->_interface->removeFromInventory(index);
	}

	ObjectTable[index].sceneIndex = _vm->_scene->currentSceneNumber();
	ObjectTable[index].spritelistRn = 9 + sprite_param;
	ObjectTable[index].x = x_param;
	ObjectTable[index].y = y_param;

	return SUCCESS;
}

// Script function #33 (0x21)
int Script::SF_finishBgdAnim(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();

	debug(1, "stub: SF_finishBgdAnim(%d)", param);
	return SUCCESS;
}

// Script function #34 (0x22)
int Script::SF_swapActors(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();

	debug(1, "stub: SF_swapActors(%d, %d)", param1, param2);
	return SUCCESS;
}

// Script function #35 (0x23)
int Script::SF_simulSpeech(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_simulSpeech(), %d args", nArgs);
	return SUCCESS;
}

// Script function #36 (0x24) ?
// Commands the specified actor to walk to the given position
// Param1: actor id
// Param2: actor destination x
// Param3: actor destination y
// Param4: flags telling how to walk
int Script::SF_actorWalk(SCRIPTFUNC_PARAMS) {
	// INCOMPLETE
	SDataWord_T actor_parm;
	SDataWord_T x_parm;
	SDataWord_T y_parm;
	SDataWord_T flags_parm;
	uint16 actorId;
	Point pt;

	actor_parm = thread->pop();
	x_parm = thread->pop();
	y_parm = thread->pop();
	flags_parm = thread->pop();

	actorId = _vm->_sdata->readWordS(actor_parm);

	pt.x = _vm->_sdata->readWordS(x_parm);
	pt.y = _vm->_sdata->readWordS(y_parm);

#if 1
	_vm->_actor->walkTo(actorId, &pt, 0, NULL);
#else
	_vm->_actor->walkTo(actorId, &pt, 0, &thread->sem);
#endif

	return SUCCESS;
}

// Script function #37 (0x25) nonblocking
// Sets an actor to the specified action state
// Param1: actor id
// Param2: flags telling how to cycle the frames
// Param3: actor action state
// Param4: some kind of delay/speed thing?
int Script::SF_cycleActorFrames(SCRIPTFUNC_PARAMS) {
	// INCOMPLETE
	SDataWord_T actor_parm;
	SDataWord_T flags_parm;
	SDataWord_T delay_parm;
	SDataWord_T action_parm;
	uint16 actorId;
	int action;
	//uint16 flags;

	actor_parm = thread->pop();
	flags_parm = thread->pop();
	action_parm = thread->pop();
	delay_parm = thread->pop();
	actorId = _vm->_sdata->readWordS(actor_parm);
	action = _vm->_sdata->readWordS(action_parm);

	_vm->_actor->setAction(actorId, action, ACTION_NONE);

	return SUCCESS;
}

// Script function #38 (0x26) nonblocking
// Sets an actor to the specified action state
// Param1: actor id
// Param2: actor action state
// Param3: which frame of the action to use
int Script::SF_setFrame(SCRIPTFUNC_PARAMS) {
	// INCOMPLETE

	SDataWord_T actorParam;
	SDataWord_T actionParam;
	SDataWord_T frameParam;

	uint16 actorId;
	int action;

	actorParam = thread->pop();
	actionParam = thread->pop();
	frameParam = thread->pop();

	actorId = _vm->_sdata->readWordS(actorParam);
	action = _vm->_sdata->readWordS(actionParam);

	_vm->_actor->setAction(actorId, action, ACTION_NONE);

	return SUCCESS;
}

// Script function #39 (0x27)
// Sets the right-hand portrait
int Script::SF_setRightPortrait(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();

	return _vm->_interface->setRightPortrait(param);
}

// Script function #40 (0x28)
// Sets the left-hand portrait
int Script::SF_setLeftPortrait(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();

	return _vm->_interface->setLeftPortrait(param);
}

// Script function #41 (0x29) nonblocking
// Links the specified animations for playback

// Param1: ?
// Param2: total linked frame count
// Param3: animation id link target
// Param4: animation id link source
int Script::SF_linkAnim(SCRIPTFUNC_PARAMS) {
	SDataWord_T timer_parm;
	SDataWord_T tframes_parm;
	SDataWord_T anim1_parm;
	SDataWord_T anim2_parm;
	int tframes;
	uint16 anim_id1;
	uint16 anim_id2;

	anim1_parm = thread->pop();
	anim2_parm = thread->pop();
	tframes_parm = thread->pop();
	timer_parm = thread->pop();
	tframes = _vm->_sdata->readWordS(tframes_parm);
	anim_id1 = _vm->_sdata->readWordU(anim1_parm);
	anim_id2 = _vm->_sdata->readWordU(anim2_parm);

	if (_vm->_anim->link(anim_id1, anim_id2) != SUCCESS) {
		_vm->_console->DebugPrintf(S_WARN_PREFIX "SF.41: Anim::link() failed. (%u->%u)\n", anim_id1, anim_id2);
		return FAILURE;
	}

	return SUCCESS;
}

// Script function #42 (0x2A)
int Script::SF_scriptSpecialWalk(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();
	SDataWord_T param3 = thread->pop();
	SDataWord_T param4 = thread->pop();

	debug(1, "stub: SF_scriptSpecialWalk(%d, %d, %d, %d)", param1, param2, param3, param4);
	return SUCCESS;
}

// Script function #43 (0x2B) nonblocking
// Positions an actor at the specified location; actor is created if the 
//  actor does not already exist.
// Param1: actor id
// Param2: actor pos x
// Param3: actor pos y
// Param4: ?
// Param5: actor action
// Param6: ?
int Script::SF_placeActor(SCRIPTFUNC_PARAMS) {
	// INCOMPLETE
	SDataWord_T actor_parm;
	SDataWord_T x_parm;
	SDataWord_T y_parm;
	SDataWord_T orient_parm;
	SDataWord_T action_parm;
	SDataWord_T frame_parm;
	uint16 actorId;
	int action_state;
	Point pt;

	actor_parm = thread->pop();
	x_parm = thread->pop();
	y_parm = thread->pop();
	orient_parm = thread->pop();
	action_parm = thread->pop();
	frame_parm = thread->pop();

	actorId = _vm->_sdata->readWordS(actor_parm);
	pt.x = _vm->_sdata->readWordS(x_parm);
	pt.y = _vm->_sdata->readWordS(y_parm);
	action_state = _vm->_sdata->readWordS(action_parm);

	_vm->_actor->move(actorId, pt);

	if (action_state < 0)
		action_state = ACTION_IDLE;
 	_vm->_actor->setDefaultAction(actorId, action_state, ACTION_NONE);
	_vm->_actor->setAction(actorId, action_state, ACTION_NONE);

	return SUCCESS;
}

// Script function #44 (0x2C) nonblocking
// Checks to see if the user has interrupted a currently playing 
// game cinematic. Pushes a zero or positive value if the game 
// has not been interrupted.
int Script::SF_checkUserInterrupt(SCRIPTFUNC_PARAMS) {
	thread->retVal = (_skipSpeeches == true);

	return SUCCESS;
}

// Script function #45 (0x2D)
int Script::SF_walkRelative(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();
	SDataWord_T param3 = thread->pop();
	SDataWord_T param4 = thread->pop();
	SDataWord_T param5 = thread->pop();

	debug(1, "stub: SF_walkRelative(%d, %d, %d, %d, %d)", param1, param2, param3, param4, param5);
	return SUCCESS;
}

// Script function #46 (0x2E)
int Script::SF_moveRelative(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();
	SDataWord_T param3 = thread->pop();
	SDataWord_T param4 = thread->pop();
	SDataWord_T param5 = thread->pop();

	debug(1, "stub: SF_moveRelative(%d, %d, %d, %d, %d)", param1, param2, param3, param4, param5);
	return SUCCESS;
}

// Script function #47 (0x2F)
int Script::SF_simulSpeech2(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_simulSpeech2(), %d args", nArgs);
	return SUCCESS;
}

// Script function #48 (0x30)
int Script::SF_placard(SCRIPTFUNC_PARAMS) {
	debug(1, "stub: SF_placard()");
	return SUCCESS;
}

// Script function #49 (0x31)
int Script::SF_placardOff(SCRIPTFUNC_PARAMS) {
	debug(1, "stub: SF_placardOff()");
	return SUCCESS;
}

// Script function #50 (0x32)
int Script::SF_setProtagState(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_setProtagState(), %d args", nArgs);
	return SUCCESS;
}

// Script function #51 (0x33)
int Script::SF_resumeBgdAnim(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_resumeBgdAnim(), %d args", nArgs);
	return SUCCESS;
}

// Script function #52 (0x34)
int Script::SF_throwActor(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();
	SDataWord_T param3 = thread->pop();
	SDataWord_T param4 = thread->pop();
	SDataWord_T param5 = thread->pop();
	SDataWord_T param6 = thread->pop();

	debug(1, "stub: SF_throwActor(%d, %d, %d, %d, %d, %d)", param1, param2, param3, param4, param5, param6);
	return SUCCESS;
}

// Script function #53 (0x35)
int Script::SF_waitWalk(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();

	debug(1, "stub: SF_waitWalk(%d)", param);
	return SUCCESS;
}

// Script function #54 (0x36)
int Script::SF_sceneID(SCRIPTFUNC_PARAMS) {
	thread->retVal = _vm->_scene->currentSceneNumber();
	return SUCCESS;
}

// Script function #55 (0x37)
int Script::SF_changeActorScene(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();

	debug(1, "stub: SF_changeActorScene(%d, %d)", param1, param2);
	return SUCCESS;
}

// Script function #56 (0x38)
int Script::SF_climb(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();
	SDataWord_T param3 = thread->pop();
	SDataWord_T param4 = thread->pop();

	debug(1, "stub: SF_climb(%d, %d, %d, %d)", param1, param2, param3, param4);
	return SUCCESS;
}

// Script function #57 (0x39)
int Script::SF_setDoorState(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_setDoorState(), %d args", nArgs);
	return SUCCESS;
}

// Script function #58 (0x3A)
int Script::SF_setActorZ(SCRIPTFUNC_PARAMS) {
	SDataWord_T param1 = thread->pop();
	SDataWord_T param2 = thread->pop();

	debug(1, "stub: SF_setActorZ(%d, %d)", param1, param2);
	return SUCCESS;
}

// Script function #59 (0x3B)
int Script::SF_text(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_text(), %d args", nArgs);
	return SUCCESS;
}

// Script function #60 (0x3C)
int Script::SF_getActorX(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();

	debug(1, "stub: SF_getActorX(%d)", param);
	return SUCCESS;
}

// Script function #61 (0x3D)
int Script::SF_getActorY(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();

	debug(1, "stub: SF_getActorY(%d)", param);
	return SUCCESS;
}

// Script function #62 (0x3E)
int Script::SF_eraseDelta(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_eraseDelta(), %d args", nArgs);
	return SUCCESS;
}

// Script function #63 (0x3F)
int Script::SF_playMusic(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop() + 9;

	if (param >= 9 && param <= 34)
		_vm->_music->play(param);
	else
		_vm->_music->stop();

	return SUCCESS;
}

// Script function #64 (0x40)
int Script::SF_pickClimbOutPos(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_pickClimbOutPos(), %d args", nArgs);
	return SUCCESS;
}

// Script function #65 (0x41)
int Script::SF_tossRif(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_tossRif(), %d args", nArgs);
	return SUCCESS;
}

// Script function #66 (0x42)
int Script::SF_showControls(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_showControls(), %d args", nArgs);
	return SUCCESS;
}

// Script function #67 (0x43)
int Script::SF_showMap(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_showMap(), %d args", nArgs);
	return SUCCESS;
}

// Script function #68 (0x44)
int Script::SF_puzzleWon(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_puzzleWon(), %d args", nArgs);
	return SUCCESS;
}

// Script function #69 (0x45)
int Script::SF_enableEscape(SCRIPTFUNC_PARAMS) {
	if (thread->pop())
		_abortEnabled = true;
	else {
		_skipSpeeches = false;
		_abortEnabled = false;
	}
	
	return SUCCESS;
}

static struct {
	int res;
	int vol;
} sfxTable[] = {
	{ FX_DOOR_OPEN,    127 },
	{ FX_DOOR_CLOSE,   127 },
	{ FX_RUSH_WATER,    63 }, // Floppy volume: 127
	{ FX_RUSH_WATER,    26 }, // Floppy volume: 40
	{ FX_CRICKET,       64 },
	{ FX_PORTICULLIS,   84 }, // Floppy volume: 127
	{ FX_CLOCK_1,       64 },
	{ FX_CLOCK_2,       64 },
	{ FX_DAM_MACHINE,   64 },
	{ FX_DAM_MACHINE,   40 },
	{ FX_HUM1,          64 },
	{ FX_HUM2,          64 },
	{ FX_HUM3,          64 },
	{ FX_HUM4,          64 },
	{ FX_WATER_LOOP_S,  32 }, // Floppy volume: 64
	{ FX_SURF,          42 }, // Floppy volume: 127
	{ FX_SURF,          32 }, // Floppy volume: 64
	{ FX_FIRELOOP,      64 }, // Floppy volume: 96
	{ FX_SCRAPING,      84 }, // Floppy volume: 127
	{ FX_BEE_SWARM,     64 }, // Floppy volume: 96
	{ FX_BEE_SWARM,     26 }, // Floppy volume: 40
	{ FX_SQUEAKBOARD,   64 },
	{ FX_KNOCK,        127 },
	{ FX_COINS,         32 }, // Floppy volume: 48
	{ FX_STORM,         84 }, // Floppy volume: 127
	{ FX_DOOR_CLOSE_2,  84 }, // Floppy volume: 127
	{ FX_ARCWELD,       84 }, // Floppy volume: 127
	{ FX_RETRACT_ORB,  127 },
	{ FX_DRAGON,       127 },
	{ FX_SNORES,       127 },
	{ FX_SPLASH,       127 },
	{ FX_LOBBY_DOOR,   127 },
	{ FX_CHIRP_LOOP,    26 }, // Floppy volume: 40
	{ FX_DOOR_CREAK,    96 },
	{ FX_SPOON_DIG,     64 },
	{ FX_CROW,          96 },
	{ FX_COLDWIND,      42 }, // Floppy volume: 64
	{ FX_TOOL_SND_1,    96 },
	{ FX_TOOL_SND_2,   127 },
	{ FX_TOOL_SND_3,    64 },
	{ FX_DOOR_METAL,    96 },
	{ FX_WATER_LOOP_S,  32 },
	{ FX_WATER_LOOP_L,  32 }, // Floppy volume: 64
	{ FX_DOOR_OPEN_2,  127 },
	{ FX_JAIL_DOOR,     64 },
	{ FX_KILN_FIRE,     53 }, // Floppy volume: 80

	// Only in the CD version
	{ FX_CROWD_01,      64 },
	{ FX_CROWD_02,      64 },
	{ FX_CROWD_03,      64 },
	{ FX_CROWD_04,      64 },
	{ FX_CROWD_05,      64 },
	{ FX_CROWD_06,      64 },
	{ FX_CROWD_07,      64 },
	{ FX_CROWD_08,      64 },
	{ FX_CROWD_09,      64 },
	{ FX_CROWD_10,      64 },
	{ FX_CROWD_11,      64 },
	{ FX_CROWD_12,      64 },
	{ FX_CROWD_13,      64 },
	{ FX_CROWD_14,      64 },
	{ FX_CROWD_15,      64 },
	{ FX_CROWD_16,      64 },
	{ FX_CROWD_17,      64 }
};

// Script function #70 (0x46)
int Script::SF_playSound(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop() - 13;

	if (/* param >= 0 && */ param < ARRAYSIZE(sfxTable))
		_vm->_sndRes->playSound(sfxTable[param].res, sfxTable[param].vol, false);
	else
		_vm->_sound->stopSound();

	return SUCCESS;
}

// Script function #71 (0x47)
int Script::SF_playLoopedSound(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_playLoopedSound(), %d args", nArgs);
	return SUCCESS;
}

// Script function #72 (0x48)
int Script::SF_getDeltaFrame(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_getDeltaFrame(), %d args", nArgs);
	return SUCCESS;
}

// Script function #73 (0x49)
int Script::SF_showProtect(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_showProtect(), %d args", nArgs);
	return SUCCESS;
}

// Script function #74 (0x4A)
int Script::SF_protectResult(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_protectResult(), %d args", nArgs);
	return SUCCESS;
}

// Script function #75 (0x4d)
int Script::SF_rand(SCRIPTFUNC_PARAMS) {
	SDataWord_T param = thread->pop();

	thread->retVal = (_vm->_rnd.getRandomNumber(param));

	return SUCCESS;
}

// Script function #76 (0x4c)
int Script::SF_fadeMusic(SCRIPTFUNC_PARAMS) {
	debug(1, "stub: SF_fadeMusic()");
	return SUCCESS;
}

// Script function #77 (0x4d)
int Script::SF_playVoice(SCRIPTFUNC_PARAMS) {
	for (int i = 0; i < nArgs; i++)
		thread->pop();

	debug(1, "stub: SF_playVoice(), %d args", nArgs);
	return SUCCESS;
}

} // End of namespace Saga

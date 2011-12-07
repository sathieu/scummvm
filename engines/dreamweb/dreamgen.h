#ifndef TASMRECOVER_DREAMGEN_STUBS_H__
#define TASMRECOVER_DREAMGEN_STUBS_H__

/* PLEASE DO NOT MODIFY THIS FILE. ALL CHANGES WILL BE LOST! LOOK FOR README FOR DETAILS */

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

#include "dreamweb/runtime.h"

#include "dreamweb/structs.h"
#include "dreamweb/dreambase.h"

namespace DreamGen {

static const uint16 offset_speechfile = 0x060f;
static const uint16 offset_operand1 = 0x052a;
static const uint16 offset_comlist = 0x0484;
static const uint16 offset_money2poke = 0x0559;
static const uint16 offset_discopslist = 0x05b0;
static const uint16 offset_openchangesize = 0x044c;
static const uint16 offset_keys = 0x04c0;
static const uint16 offset_diarylist = 0x055c;
static const uint16 offset_quitlist = 0x046e;
static const uint16 offset_rootdir = 0x0538;
static const uint16 offset_decidelist = 0x05df;
static const uint16 offset_money1poke = 0x0554;
static const uint16 offset_commandline = 0x061c;
static const uint16 offset_opslist = 0x0586;
static const uint16 offset_talklist = 0x044e;
static const uint16 kStartvars = 0;
static const uint16 kProgresspoints = 1;
static const uint16 kWatchon = 2;
static const uint16 kShadeson = 3;
static const uint16 kSecondcount = 4;
static const uint16 kMinutecount = 5;
static const uint16 kHourcount = 6;
static const uint16 kZoomon = 7;
static const uint16 kLocation = 8;
static const uint16 kExpos = 9;
static const uint16 kExframepos = 10;
static const uint16 kExtextpos = 12;
static const uint16 kCard1money = 14;
static const uint16 kListpos = 16;
static const uint16 kRyanpage = 18;
static const uint16 kWatchingtime = 19;
static const uint16 kReeltowatch = 21;
static const uint16 kEndwatchreel = 23;
static const uint16 kSpeedcount = 25;
static const uint16 kWatchspeed = 26;
static const uint16 kReeltohold = 27;
static const uint16 kEndofholdreel = 29;
static const uint16 kWatchmode = 31;
static const uint16 kDestafterhold = 32;
static const uint16 kNewsitem = 33;
static const uint16 kLiftflag = 34;
static const uint16 kLiftpath = 35;
static const uint16 kLockstatus = 36;
static const uint16 kDoorpath = 37;
static const uint16 kCounttoopen = 38;
static const uint16 kCounttoclose = 39;
static const uint16 kRockstardead = 40;
static const uint16 kGeneraldead = 41;
static const uint16 kSartaindead = 42;
static const uint16 kAidedead = 43;
static const uint16 kBeenmugged = 44;
static const uint16 kGunpassflag = 45;
static const uint16 kCanmovealtar = 46;
static const uint16 kTalkedtoattendant = 47;
static const uint16 kTalkedtosparky = 48;
static const uint16 kTalkedtoboss = 49;
static const uint16 kTalkedtorecep = 50;
static const uint16 kCardpassflag = 51;
static const uint16 kMadmanflag = 52;
static const uint16 kKeeperflag = 53;
static const uint16 kLasttrigger = 54;
static const uint16 kMandead = 55;
static const uint16 kSeed = 56;
static const uint16 kNeedtotravel = 59;
static const uint16 kThroughdoor = 60;
static const uint16 kNewobs = 61;
static const uint16 kRyanon = 62;
static const uint16 kCombatcount = 63;
static const uint16 kLastweapon = 64;
static const uint16 kDreamnumber = 65;
static const uint16 kRoomafterdream = 66;
static const uint16 kShakecounter = 67;
static const uint16 kSpeechcount = 68;
static const uint16 kCharshift = 69;
static const uint16 kKerning = 71;
static const uint16 kBrightness = 72;
static const uint16 kRoomloaded = 73;
static const uint16 kDidzoom = 74;
static const uint16 kLinespacing = 75;
static const uint16 kTextaddressx = 77;
static const uint16 kTextaddressy = 79;
static const uint16 kTextlen = 81;
static const uint16 kLastxpos = 82;
static const uint16 kIcontop = 84;
static const uint16 kIconleft = 86;
static const uint16 kItemframe = 88;
static const uint16 kItemtotran = 89;
static const uint16 kRoomad = 90;
static const uint16 kOldsubject = 92;
static const uint16 kWithobject = 94;
static const uint16 kWithtype = 95;
static const uint16 kLookcounter = 96;
static const uint16 kCommand = 98;
static const uint16 kCommandtype = 99;
static const uint16 kOldcommandtype = 100;
static const uint16 kObjecttype = 101;
static const uint16 kGetback = 102;
static const uint16 kInvopen = 103;
static const uint16 kMainmode = 104;
static const uint16 kPickup = 105;
static const uint16 kLastinvpos = 106;
static const uint16 kExamagain = 107;
static const uint16 kNewtextline = 108;
static const uint16 kOpenedob = 109;
static const uint16 kOpenedtype = 110;
static const uint16 kOldmapadx = 111;
static const uint16 kOldmapady = 113;
static const uint16 kMapadx = 115;
static const uint16 kMapady = 117;
static const uint16 kMapoffsetx = 119;
static const uint16 kMapoffsety = 121;
static const uint16 kMapxstart = 123;
static const uint16 kMapystart = 125;
static const uint16 kMapxsize = 127;
static const uint16 kMapysize = 128;
static const uint16 kHavedoneobs = 129;
static const uint16 kManisoffscreen = 130;
static const uint16 kRainspace = 131;
static const uint16 kFacing = 132;
static const uint16 kLeavedirection = 133;
static const uint16 kTurntoface = 134;
static const uint16 kTurndirection = 135;
static const uint16 kMaintimer = 136;
static const uint16 kIntrocount = 138;
static const uint16 kArrowad = 139;
static const uint16 kCurrentkey = 141;
static const uint16 kOldkey = 142;
static const uint16 kUseddirection = 143;
static const uint16 kCurrentkey2 = 144;
static const uint16 kTimercount = 145;
static const uint16 kOldtimercount = 146;
static const uint16 kMapx = 147;
static const uint16 kMapy = 148;
static const uint16 kNewscreen = 149;
static const uint16 kRyanx = 150;
static const uint16 kRyany = 151;
static const uint16 kLastflag = 152;
static const uint16 kLastflagex = 153;
static const uint16 kFlagx = 154;
static const uint16 kFlagy = 155;
static const uint16 kCurrentex = 156;
static const uint16 kCurrentfree = 157;
static const uint16 kCurrentframe = 158;
static const uint16 kFramesad = 160;
static const uint16 kDataad = 162;
static const uint16 kFrsegment = 164;
static const uint16 kObjectx = 166;
static const uint16 kObjecty = 168;
static const uint16 kOffsetx = 170;
static const uint16 kOffsety = 172;
static const uint16 kSavesize = 174;
static const uint16 kSavesource = 176;
static const uint16 kSavex = 178;
static const uint16 kSavey = 179;
static const uint16 kCurrentob = 180;
static const uint16 kPrioritydep = 181;
static const uint16 kDestpos = 182;
static const uint16 kReallocation = 183;
static const uint16 kRoomnum = 184;
static const uint16 kNowinnewroom = 185;
static const uint16 kResetmanxy = 186;
static const uint16 kNewlocation = 187;
static const uint16 kAutolocation = 188;
static const uint16 kMustload = 189;
static const uint16 kAnswered = 190;
static const uint16 kSaidno = 191;
static const uint16 kDoorcheck1 = 192;
static const uint16 kDoorcheck2 = 193;
static const uint16 kDoorcheck3 = 194;
static const uint16 kDoorcheck4 = 195;
static const uint16 kMousex = 196;
static const uint16 kMousey = 198;
static const uint16 kMousebutton = 200;
static const uint16 kMousebutton1 = 202;
static const uint16 kMousebutton2 = 204;
static const uint16 kMousebutton3 = 206;
static const uint16 kMousebutton4 = 208;
static const uint16 kOldbutton = 210;
static const uint16 kOldx = 212;
static const uint16 kOldy = 214;
static const uint16 kLastbutton = 216;
static const uint16 kOldpointerx = 218;
static const uint16 kOldpointery = 220;
static const uint16 kDelherex = 222;
static const uint16 kDelherey = 224;
static const uint16 kPointerxs = 226;
static const uint16 kPointerys = 227;
static const uint16 kDelxs = 228;
static const uint16 kDelys = 229;
static const uint16 kPointerframe = 230;
static const uint16 kPointerpower = 231;
static const uint16 kAuxpointerframe = 232;
static const uint16 kPointermode = 233;
static const uint16 kPointerspeed = 234;
static const uint16 kPointercount = 235;
static const uint16 kInmaparea = 236;
static const uint16 kReelpointer = 237;
static const uint16 kSlotdata = 239;
static const uint16 kThisslot = 240;
static const uint16 kSlotflags = 241;
static const uint16 kTakeoff = 242;
static const uint16 kTalkmode = 244;
static const uint16 kTalkpos = 245;
static const uint16 kCharacter = 246;
static const uint16 kPersondata = 247;
static const uint16 kTalknum = 249;
static const uint16 kNumberinroom = 250;
static const uint16 kCurrentcel = 251;
static const uint16 kOldselection = 252;
static const uint16 kStopwalking = 253;
static const uint16 kMouseon = 254;
static const uint16 kPlayed = 255;
static const uint16 kTimer1 = 257;
static const uint16 kTimer2 = 258;
static const uint16 kTimer3 = 259;
static const uint16 kWholetimer = 260;
static const uint16 kTimer1to = 262;
static const uint16 kTimer2to = 263;
static const uint16 kTimer3to = 264;
static const uint16 kWatchdump = 265;
static const uint16 kLogonum = 266;
static const uint16 kOldlogonum = 267;
static const uint16 kNewlogonum = 268;
static const uint16 kNetseg = 269;
static const uint16 kNetpoint = 271;
static const uint16 kKeynum = 273;
static const uint16 kCursorstate = 274;
static const uint16 kPressed = 275;
static const uint16 kPresspointer = 276;
static const uint16 kGraphicpress = 278;
static const uint16 kPresscount = 279;
static const uint16 kKeypadax = 280;
static const uint16 kKeypadcx = 282;
static const uint16 kLightcount = 284;
static const uint16 kFolderpage = 285;
static const uint16 kDiarypage = 286;
static const uint16 kMenucount = 287;
static const uint16 kSymboltopx = 288;
static const uint16 kSymboltopnum = 289;
static const uint16 kSymboltopdir = 290;
static const uint16 kSymbolbotx = 291;
static const uint16 kSymbolbotnum = 292;
static const uint16 kSymbolbotdir = 293;
static const uint16 kSymboltolight = 294;
static const uint16 kSymbol1 = 295;
static const uint16 kSymbol2 = 296;
static const uint16 kSymbol3 = 297;
static const uint16 kSymbolnum = 298;
static const uint16 kDumpx = 299;
static const uint16 kDumpy = 301;
static const uint16 kWalkandexam = 303;
static const uint16 kWalkexamtype = 304;
static const uint16 kWalkexamnum = 305;
static const uint16 kCursloc = 306;
static const uint16 kCurslocx = 308;
static const uint16 kCurslocy = 310;
static const uint16 kCurpos = 312;
static const uint16 kMonadx = 314;
static const uint16 kMonady = 316;
static const uint16 kGotfrom = 318;
static const uint16 kMonsource = 320;
static const uint16 kNumtodo = 322;
static const uint16 kTimecount = 324;
static const uint16 kCounttotimed = 326;
static const uint16 kTimedseg = 328;
static const uint16 kTimedoffset = 330;
static const uint16 kTimedy = 332;
static const uint16 kTimedx = 333;
static const uint16 kNeedtodumptimed = 334;
static const uint16 kHandle = 335;
static const uint16 kLoadingorsave = 337;
static const uint16 kCurrentslot = 338;
static const uint16 kCursorpos = 339;
static const uint16 kColourpos = 340;
static const uint16 kFadedirection = 341;
static const uint16 kNumtofade = 342;
static const uint16 kFadecount = 343;
static const uint16 kAddtogreen = 344;
static const uint16 kAddtored = 345;
static const uint16 kAddtoblue = 346;
static const uint16 kLastsoundreel = 347;
static const uint16 kSoundbuffer = 349;
static const uint16 kSoundbufferad = 351;
static const uint16 kSoundbufferpage = 353;
static const uint16 kSoundtimes = 354;
static const uint16 kNeedsoundbuff = 355;
static const uint16 kOldint9seg = 356;
static const uint16 kOldint9add = 358;
static const uint16 kOldint8seg = 360;
static const uint16 kOldint8add = 362;
static const uint16 kOldsoundintseg = 364;
static const uint16 kOldsoundintadd = 366;
static const uint16 kSoundbaseadd = 368;
static const uint16 kDsp_status = 370;
static const uint16 kDsp_write = 372;
static const uint16 kDmaaddress = 374;
static const uint16 kSoundint = 375;
static const uint16 kSounddmachannel = 376;
static const uint16 kSampleplaying = 377;
static const uint16 kTestresult = 378;
static const uint16 kCurrentirq = 379;
static const uint16 kSpeechloaded = 380;
static const uint16 kSpeechlength = 381;
static const uint16 kVolume = 383;
static const uint16 kVolumeto = 384;
static const uint16 kVolumedirection = 385;
static const uint16 kVolumecount = 386;
static const uint16 kPlayblock = 387;
static const uint16 kWongame = 388;
static const uint16 kLasthardkey = 389;
static const uint16 kBufferin = 390;
static const uint16 kBufferout = 392;
static const uint16 kExtras = 394;
static const uint16 kWorkspace = 396;
static const uint16 kMapstore = 398;
static const uint16 kCharset1 = 400;
static const uint16 kBuffers = 402;
static const uint16 kMainsprites = 404;
static const uint16 kBackdrop = 406;
static const uint16 kMapdata = 408;
static const uint16 kSounddata = 410;
static const uint16 kSounddata2 = 412;
static const uint16 kRecordspace = 414;
static const uint16 kFreedat = 416;
static const uint16 kSetdat = 418;
static const uint16 kReel1 = 420;
static const uint16 kReel2 = 422;
static const uint16 kReel3 = 424;
static const uint16 kRoomdesc = 426;
static const uint16 kFreedesc = 428;
static const uint16 kSetdesc = 430;
static const uint16 kBlockdesc = 432;
static const uint16 kSetframes = 434;
static const uint16 kFreeframes = 436;
static const uint16 kPeople = 438;
static const uint16 kReels = 440;
static const uint16 kCommandtext = 442;
static const uint16 kPuzzletext = 444;
static const uint16 kTraveltext = 446;
static const uint16 kTempgraphics = 448;
static const uint16 kTempgraphics2 = 450;
static const uint16 kTempgraphics3 = 452;
static const uint16 kTempsprites = 454;
static const uint16 kTextfile1 = 456;
static const uint16 kTextfile2 = 458;
static const uint16 kTextfile3 = 460;
static const uint16 kBlinkframe = 462;
static const uint16 kBlinkcount = 463;
static const uint16 kReasseschanges = 464;
static const uint16 kPointerspath = 465;
static const uint16 kManspath = 466;
static const uint16 kPointerfirstpath = 467;
static const uint16 kFinaldest = 468;
static const uint16 kDestination = 469;
static const uint16 kLinestartx = 470;
static const uint16 kLinestarty = 472;
static const uint16 kLineendx = 474;
static const uint16 kLineendy = 476;
static const uint16 kIncrement1 = 478;
static const uint16 kIncrement2 = 480;
static const uint16 kLineroutine = 482;
static const uint16 kLinepointer = 483;
static const uint16 kLinedirection = 484;
static const uint16 kLinelength = 485;
static const uint16 kLiftsoundcount = 486;
static const uint16 kEmmhandle = 487;
static const uint16 kEmmpageframe = 489;
static const uint16 kEmmhardwarepage = 491;
static const uint16 kCh0emmpage = 492;
static const uint16 kCh0offset = 494;
static const uint16 kCh0blockstocopy = 496;
static const uint16 kCh0playing = 498;
static const uint16 kCh0repeat = 499;
static const uint16 kCh0oldemmpage = 500;
static const uint16 kCh0oldoffset = 502;
static const uint16 kCh0oldblockstocopy = 504;
static const uint16 kCh1playing = 506;
static const uint16 kCh1emmpage = 507;
static const uint16 kCh1offset = 509;
static const uint16 kCh1blockstocopy = 511;
static const uint16 kCh1blocksplayed = 513;
static const uint16 kSoundbufferwrite = 515;
static const uint16 kSoundemmpage = 517;
static const uint16 kSpeechemmpage = 519;
static const uint16 kCurrentsample = 521;
static const uint16 kRoomssample = 522;
static const uint16 kGameerror = 523;
static const uint16 kHowmuchalloc = 524;
static const uint16 kReelroutines = 526;
static const uint16 kCharacterset3 = 983;
static const uint16 kBasicsample = 996;
static const uint16 kDiarygraphic = 1009;
static const uint16 kDiarytext = 1022;
static const uint16 kEndtextname = 1035;
static const uint16 kVolumetabname = 1048;
static const uint16 kGungraphic = 1061;
static const uint16 kMonkface = 1074;
static const uint16 kTitle7graphics = 1087;
static const uint16 kCurrentfile = 1350;
static const uint16 kRoomscango = 1661;
static const uint16 kRoompics = 1677;
static const uint16 kOplist = 1692;
static const uint16 kInputline = 1695;
static const uint16 kPresslist = 1823;
static const uint16 kSavenames = 1829;
static const uint16 kQuitrequested = 1948;
static const uint16 kSubtitles = 1949;
static const uint16 kForeignrelease = 1950;
static const uint16 kBlocktextdat = (0);
static const uint16 kPersonframes = (0);
static const uint16 kDebuglevel1 = (0);
static const uint16 kDebuglevel2 = (0);
static const uint16 kPlayback = (0);
static const uint16 kMap = (0);
static const uint16 kSettextdat = (0);
static const uint16 kSpanish = (0);
static const uint16 kFramedata = (0);
static const uint16 kRecording = (0);
static const uint16 kFlags = (0);
static const uint16 kGerman = (0);
static const uint16 kTextunder = (0);
static const uint16 kPathdata = (0);
static const uint16 kDemo = (0);
static const uint16 kExframedata = (0);
static const uint16 kIntextdat = (0);
static const uint16 kFreetextdat = (0);
static const uint16 kFrframedata = (0);
static const uint16 kSettext = (0+(130*2));
static const uint16 kOpeninvlist = (0+(228*13));
static const uint16 kRyaninvlist = (0+(228*13)+32);
static const uint16 kPointerback = (0+(228*13)+32+60);
static const uint16 kMapflags = (0+(228*13)+32+60+(32*32));
static const uint16 kStartpal = (0+(228*13)+32+60+(32*32)+(11*10*3));
static const uint16 kEndpal = (0+(228*13)+32+60+(32*32)+(11*10*3)+768);
static const uint16 kMaingamepal = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768);
static const uint16 kSpritetable = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768);
static const uint16 kSetlist = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32));
static const uint16 kFreelist = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5));
static const uint16 kExlist = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5));
static const uint16 kPeoplelist = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5));
static const uint16 kZoomspace = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5));
static const uint16 kPrintedlist = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40));
static const uint16 kListofchanges = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40)+(5*80));
static const uint16 kUndertimedtext = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40)+(5*80)+(250*4));
static const uint16 kRainlist = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40)+(5*80)+(250*4)+(256*30));
static const uint16 kInitialreelrouts = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40)+(5*80)+(250*4)+(256*30)+(6*64));
static const uint16 kInitialvars = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40)+(5*80)+(250*4)+(256*30)+(6*64)+983-526);
static const uint16 kLengthofbuffer = (0+(228*13)+32+60+(32*32)+(11*10*3)+768+768+768+(32*32)+(128*5)+(80*5)+(100*5)+(12*5)+(46*40)+(5*80)+(250*4)+(256*30)+(6*64)+983-526+68-0);
static const uint16 kReellist = (0+(36*144));
static const uint16 kIntext = (0+(38*2));
static const uint16 kLengthofmap = (0+(66*60));
static const uint16 kFreetext = (0+(82*2));
static const uint16 kBlocktext = (0+(98*2));
static const uint16 kBlocks = (0+192);
static const uint16 kFrframes = (0+2080);
static const uint16 kExframes = (0+2080);
static const uint16 kFrames = (0+2080);
static const uint16 kExdata = (0+2080+30000);
static const uint16 kExtextdat = (0+2080+30000+(16*114));
static const uint16 kExtext = (0+2080+30000+(16*114)+((114+2)*2));
static const uint16 kLengthofextra = (0+2080+30000+(16*114)+((114+2)*2)+18000);
static const uint16 kPersontxtdat = (0+24);
static const uint16 kPersontext = (0+24+(1026*2));
static const uint16 kInputport = (0x63);
static const uint16 kForeign = (1);
static const uint16 kCd = (1);
static const uint16 kNumexobjects = (114);
static const uint16 kUndertextsizey = (13);
static const uint16 kZoomy = (132);
static const uint16 kFreedatlen = (16*80);
static const uint16 kExtextlen = (18000);
static const uint16 kLenofmapstore = (22*8*20*8);
static const uint16 kUndertextsizex = (228);
static const uint16 kNumchanges = (250);
static const uint16 kUndertimedysize = (30);
static const uint16 kExframeslen = (30000);
static const uint16 kTablesize = (32);
static const uint16 kScreenwidth = (320);
static const uint16 kKeypadx = (36+112);
static const uint16 kItempicsize = (44);
static const uint16 kDiaryy = (48+12);
static const uint16 kOpsy = (52);
static const uint16 kSymboly = (56);
static const uint16 kInventy = (58);
static const uint16 kMenuy = (60);
static const uint16 kOpsx = (60);
static const uint16 kMaplength = (60);
static const uint16 kSymbolx = (64);
static const uint16 kSetdatlen = (64*128);
static const uint16 kMapwidth = (66);
static const uint16 kTextstart = (66*2);
static const uint16 kMaplen = (66*60);
static const uint16 kDiaryx = (68+24);
static const uint16 kLengthofvars = (68-0);
static const uint16 kKeypady = (72);
static const uint16 kZoomx = (8);
static const uint16 kInventx = (80);
static const uint16 kMenux = (80+40);
static const uint16 kHeaderlen = (96);
static const uint16 kLenofreelrouts = (983-526);


class DreamGenContext : public DreamBase, public Context {
public:
	DreamGenContext(DreamWeb::DreamWebEngine *en) : DreamBase(en), Context(data) {}

	void __start();
#include "stubs.h" // Allow hand-reversed functions to have a signature different than void f()

	void useWire();
	void getNamePos();
	void identifyOb();
	void useLighter();
	void runEndSeq();
	void useOpenBox();
	void clearBuffers();
	void getObTextStart();
	void dumpDiaryKeys();
	void entryTexts();
	void checkInput();
	void putUnderCentre();
	void checkObjectSize();
	void watchReel();
	void findText1();
	void isRyanHolding();
	void showSlots();
	void useCashCard();
	void moneyPoke();
	void doSomeTalk();
	void showSaveOps();
	void introMonks1();
	void resetLocation();
	void introMonks2();
	void greyscaleSum();
	void getOpenedSize();
	void adjustUp();
	void fadeScreenDownHalf();
	void fadeDownMon();
	void printmessage2();
	void bartender();
	void showDiary();
	void outOfOpen();
	void dirCom();
	void endGameSeq();
	void findFirstPath();
	void useSLab();
	void useAltar();
	void startTalk();
	void getAnyAd();
	void endGame();
	void usePipe();
	void getUnderZoom();
	void reminders();
	void runTap();
	void talk();
	void showMonk();
	void checkForExit();
	void lookInInterface();
	void inToInv();
	void adjustLeft();
	void deleteExText();
	void entryAnims();
	void getFreeAd();
	void showArrows();
	void printOuterMon();
	void showDecisions();
	void removeObFromInv();
	void heavy();
	void useKey();
	void lockLightOn();
	void discOps();
	void middlePanel();
	void monitorLogo();
	void dirFile();
	void pickupConts();
	void lockLightOff();
	void advisor();
	void allPalette();
	void cantDrop();
	void copper();
	void placeFreeObject();
	void putUnderZoom();
	void findInvPos();
	void workoutFrames();
	void rollEndCredits();
	void getKeyAndLogo();
	void selectOb();
	void useCooker();
	void receptionist();
	void selectSlot();
	void decide();
	void fadeUpMon();
	void showDiaryPage();
	void transferToEx();
	void reExFromInv();
	void examineInventory();
	void businessMan();
	void outOfInv();
	void diaryKeyP();
	void transferMap();
	void diaryKeyN();
	void purgeAnItem();
	void purgeALocation();
	void notHeldError();
	void getSetAd();
	void soldier1();
	void getUnderCentre();
	void showKeys();
	void nextColon();
	void hangOnPQ();
	void findOpenPos();
	void describeOb();
	void deleteExFrame();
	void searchForSame();
	void fadeFromWhite();
	void rollEm();
	void poolGuard();
	void lookAtPlace();
	void useAxe();
	void findAllOpen();
	void quitSymbol();
	void findSetObject();
	void useDiary();
	void deleteExObject();
	void removeFreeObject();
	void lookAtCard();
	void helicopter();
	void getEitherAd();
	void setPickup();
	void dropObject();
	void openOb();
	void drawItAll();
	void useStereo();
	void showOpBox();
	void doFade();
	void dumpCurrent();
	void showDiaryKeys();
	void rollEndCredits2();
	void useOpened();
	void fillOpen();
	void signOn();
	void showGun();
	void locationPic();
	void triggerMessage();
	void swapWithOpen();
	void dreamweb();
	void dropError();
	void checkInside();
	void findPathOfPoint();
	void getBack1();
	void getDestInfo();
	void setupTimedUse();
	void makeCaps();
	void read();
	void additionalText();
	void mugger();
	void searchForString();
	void selectOpenOb();
	void useGun();
	void autoAppear();
	void newPlace();
	void useHandle();
	void showPuzText();
	void incRyanPage();
	void findExObject();
	void clearChanges();
	void searchForFiles();
	void monkSpeaking();
	void madmanRun();
	void afterNewRoom();
	void getExAd();
	void initialMonCols();
	void checkForShake();
	void useButtonA();
	void execCommand();
	void updateSymbolBot();
	void findPuzText();
	void swapWithInv();
	void useControl();
	void adjustRight();
	void updateSymbolTop();
	void doSaveLoad();
	void createName();
	void getPersonText();
	void parser();
	void showLoadOps();
	void underTextLine();
	void showNames();
	void emergencyPurge();
	void quitKey();
	void processTrigger();
	void transferConToEx();
	void adjustDown();
	void withWhat();
	void fadeToWhite();
};

} // End of namespace DreamGen

#endif

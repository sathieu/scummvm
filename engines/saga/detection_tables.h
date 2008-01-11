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

// Game detection information and MD5s

namespace Saga {

static const GameResourceDescription ITE_Resources = {
	RID_ITE_SCENE_LUT,  // Scene lookup table RN
	RID_ITE_SCRIPT_LUT, // Script lookup table RN
	RID_ITE_MAIN_PANEL,
	RID_ITE_CONVERSE_PANEL,
	RID_ITE_OPTION_PANEL,
	0,					// Warning panel (IHNM only)
	0,					// Warning panel sprites (IHNM only)
	RID_ITE_MAIN_SPRITES,
	RID_ITE_MAIN_PANEL_SPRITES,
	0,					// Option panel sprites (IHNM only)
	RID_ITE_DEFAULT_PORTRAITS,
	0,					// Psychic profile background (IHNM only)
	RID_ITE_MAIN_STRINGS,
	RID_ITE_ACTOR_NAMES
};

static const GameResourceDescription ITEDemo_Resources = {
	RID_ITEDEMO_SCENE_LUT,  // Scene lookup table RN
	RID_ITEDEMO_SCRIPT_LUT, // Script lookup table RN
	RID_ITEDEMO_MAIN_PANEL,
	RID_ITEDEMO_CONVERSE_PANEL,
	RID_ITEDEMO_OPTION_PANEL,
	0,						// Warning panel (IHNM only)
	0,						// Warning panel sprites (IHNM only)
	RID_ITEDEMO_MAIN_SPRITES,
	RID_ITEDEMO_MAIN_PANEL_SPRITES,
	0,						// Option panel sprites (IHNM only)
	RID_ITEDEMO_DEFAULT_PORTRAITS,
	0,						// Psychic profile background (IHNM only)
	RID_ITEDEMO_MAIN_STRINGS,
	RID_ITEDEMO_ACTOR_NAMES
};

static const GameResourceDescription IHNM_Resources = {
	RID_IHNM_SCENE_LUT,  // Scene lookup table RN
	RID_IHNM_SCRIPT_LUT, // Script lookup table RN
	RID_IHNM_MAIN_PANEL,
	RID_IHNM_CONVERSE_PANEL,
	RID_IHNM_OPTION_PANEL,
	RID_IHNM_WARNING_PANEL,
	RID_IHNM_WARNING_PANEL_SPRITES,
	RID_IHNM_MAIN_SPRITES,
	RID_IHNM_MAIN_PANEL_SPRITES,
	RID_IHNM_OPTION_PANEL_SPRITES,
	0,					// Default portraits (ITE only)
	RID_IHNM_PROFILE_BG,
	RID_IHNM_MAIN_STRINGS,
	0					// Actors strings (ITE only)
};

static const GameResourceDescription IHNMDEMO_Resources = {
	RID_IHNMDEMO_SCENE_LUT,  // Scene lookup table RN
	RID_IHNMDEMO_SCRIPT_LUT, // Script lookup table RN
	RID_IHNMDEMO_MAIN_PANEL,
	RID_IHNMDEMO_CONVERSE_PANEL,
	RID_IHNMDEMO_OPTION_PANEL,
	RID_IHNMDEMO_WARNING_PANEL,
	RID_IHNMDEMO_WARNING_PANEL_SPRITES,
	RID_IHNMDEMO_MAIN_SPRITES,
	RID_IHNMDEMO_MAIN_PANEL_SPRITES,
	RID_IHNMDEMO_OPTION_PANEL_SPRITES,
	0,					// Default portraits (ITE only)
	RID_IHNMDEMO_PROFILE_BG,
	RID_IHNMDEMO_MAIN_STRINGS,
	0					// Actors strings (ITE only)
};

static const GameFontDescription ITEDEMO_GameFonts[]    = { {0}, {1} };
static const GameFontDescription ITEWINDEMO_GameFonts[] = { {2}, {0} };
static const GameFontDescription ITE_GameFonts[]        = { {2}, {0}, {1} };
static const GameFontDescription IHNMDEMO_GameFonts[]   = { {2}, {3}, {4} };
// Font 6 is kIHNMFont8, font 8 is kIHNMMainFont
static const GameFontDescription IHNMCD_GameFonts[]     = { {2}, {3}, {4}, {5}, {6}, {7}, {8} };

//                                                             frequency, sampleBits, stereo, isBigEndian, isSigned
static const GameSoundInfo ITEPC_GameSound       = { kSoundPCM,    22050,         16,  false,       false,     true };
static const GameSoundInfo ITEMAC_GameSound      = { kSoundPCM,    22050,         16,  false,        true,     true };
static const GameSoundInfo ITEWINDEMO1_GameSound = { kSoundPCM,    22050,          8,  false,       false,    false };
static const GameSoundInfo ITEMACCD_G_GameSound  = { kSoundMacPCM, 22050,          8,  false,       false,    false };
static const GameSoundInfo ITEDISK_GameSound     = { kSoundVOC,       -1,         -1,  false,       false,     true };
static const GameSoundInfo ITEDEMO_GameVoice     = { kSoundVOX,    22050,         16,  false,       false,     true };
static const GameSoundInfo IHNM_GameSound        = { kSoundWAV,       -1,         -1,  false,       false,     true };

// Patch files. Files not found will be ignored
static const GamePatchDescription ITEPatch_Files[] = {
	{       "cave.mid", GAME_RESOURCEFILE,    9, NULL},
	{      "intro.mid", GAME_RESOURCEFILE,   10, NULL},
	{   "fvillage.mid", GAME_RESOURCEFILE,   11, NULL},
	{    "elkhall.mid", GAME_RESOURCEFILE,   12, NULL},
	{      "mouse.mid", GAME_RESOURCEFILE,   13, NULL},
	{   "darkclaw.mid", GAME_RESOURCEFILE,   14, NULL},
	{   "birdchrp.mid", GAME_RESOURCEFILE,   15, NULL},
	{   "orbtempl.mid", GAME_RESOURCEFILE,   16, NULL},
	{     "spooky.mid", GAME_RESOURCEFILE,   17, NULL},
	{    "catfest.mid", GAME_RESOURCEFILE,   18, NULL},
	{ "elkfanfare.mid", GAME_RESOURCEFILE,   19, NULL},
	{     "bcexpl.mid", GAME_RESOURCEFILE,   20, NULL},
	{   "boargtnt.mid", GAME_RESOURCEFILE,   21, NULL},
	{   "boarking.mid", GAME_RESOURCEFILE,   22, NULL},
	{   "explorea.mid", GAME_RESOURCEFILE,   23, NULL},
	{   "exploreb.mid", GAME_RESOURCEFILE,   24, NULL},
	{   "explorec.mid", GAME_RESOURCEFILE,   25, NULL},
	{   "sunstatm.mid", GAME_RESOURCEFILE,   26, NULL},
	{   "nitstrlm.mid", GAME_RESOURCEFILE,   27, NULL},
	{   "humruinm.mid", GAME_RESOURCEFILE,   28, NULL},
	{   "damexplm.mid", GAME_RESOURCEFILE,   29, NULL},
	{     "tychom.mid", GAME_RESOURCEFILE,   30, NULL},
	{     "kitten.mid", GAME_RESOURCEFILE,   31, NULL},
	{      "sweet.mid", GAME_RESOURCEFILE,   32, NULL},
	{   "brutalmt.mid", GAME_RESOURCEFILE,   33, NULL},
	{     "shiala.mid", GAME_RESOURCEFILE,   34, NULL},

	{       "wyrm.pak", GAME_RESOURCEFILE, 1529, NULL},
	{      "wyrm1.dlt", GAME_RESOURCEFILE, 1530, NULL},
	{      "wyrm2.dlt", GAME_RESOURCEFILE, 1531, NULL},
	{      "wyrm3.dlt", GAME_RESOURCEFILE, 1532, NULL},
	{      "wyrm4.dlt", GAME_RESOURCEFILE, 1533, NULL},
	{   "credit3n.dlt", GAME_RESOURCEFILE, 1796, NULL}, // PC
	{   "credit3m.dlt", GAME_RESOURCEFILE, 1796, NULL}, // Macintosh
	{   "credit4n.dlt", GAME_RESOURCEFILE, 1797, NULL}, // PC
	{   "credit4m.dlt", GAME_RESOURCEFILE, 1797, NULL}, // Macintosh
	{       "p2_a.voc", GAME_VOICEFILE,       4, NULL},
	{       "p2_a.iaf", GAME_VOICEFILE,       4, &ITEPC_GameSound}
};

static const GamePatchDescription ITEMacPatch_Files[] = {
	{       "wyrm.pak", GAME_RESOURCEFILE, 1529, NULL},
	{      "wyrm1.dlt", GAME_RESOURCEFILE, 1530, NULL},
	{      "wyrm2.dlt", GAME_RESOURCEFILE, 1531, NULL},
	{      "wyrm3.dlt", GAME_RESOURCEFILE, 1532, NULL},
	{      "wyrm4.dlt", GAME_RESOURCEFILE, 1533, NULL},
	{   "credit3m.dlt", GAME_RESOURCEFILE, 1796, NULL},
	{   "credit4m.dlt", GAME_RESOURCEFILE, 1797, NULL},
	{       "p2_a.iaf", GAME_VOICEFILE,       4, &ITEMAC_GameSound}
};

static const SAGAGameDescription gameDescriptions[] = {
	// ITE Section ////////////////////////////////////////////////////////////////////////////////////////////


	// ITE Demos //////////////////////////////////////////////////////////////////////////////////////////////

	// Note: This version is NOT supported yet
	// Based on a very early version of the engine

	// Inherit the earth - DOS Demo version
	// sound unchecked
	{
		{
			"ite",
			"Demo", // Game title
			{
	{"ite.rsc",		GAME_RESOURCEFILE,					"986c79c4d2939dbe555576529fd37932", -1},
	//{"ite.dmo",	GAME_DEMOFILE},						"0b9a70eb4e120b6f00579b46c8cae29e"
	{"scripts.rsc", GAME_SCRIPTFILE,					"d5697dd3240a3ceaddaa986c47e1a2d7", -1},
	//{"voices.rsc",	GAME_SOUNDFILE | GAME_VOICEFILE,	"c58e67c506af4ffa03fd0aac2079deb0", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformPC,
			Common::ADGF_DEMO
		},
		GType_ITE,
		GID_ITE_DEMO_G, // Game id
		0, // features
		ITE_DEFAULT_SCENE, // Starting scene number
		&ITEDemo_Resources,
		ARRAYSIZE(ITEDEMO_GameFonts),
		ITEDEMO_GameFonts,
		&ITEDISK_GameSound,
		&ITEDISK_GameSound,
		0,
		NULL,
	},


	// Inherit the earth - MAC Demo version
	{
		{
			"ite",
			"Demo 2",
			{
	{"ited.rsc",		GAME_RESOURCEFILE,	"addfc9d82bc2fa1f4cab23743c652c08", 1865461},
	{"scriptsd.rsc",	GAME_SCRIPTFILE,	"fded5c59b8b7c5976229f960d21e6b0b", 70083},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformMacintosh,
			Common::ADGF_DEMO
		},
		GType_ITE,
		GID_ITE_MACDEMO2,
		GF_BIG_ENDIAN_DATA | GF_WYRMKEEP | GF_CD_FX | GF_SCENE_SUBSTITUTES,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEWINDEMO_GameFonts),
		ITEWINDEMO_GameFonts,
		&ITEDEMO_GameVoice,
		&ITEMAC_GameSound,
		ARRAYSIZE(ITEMacPatch_Files),
		ITEMacPatch_Files,
	},


	// Inherit the earth - MAC Demo version 1
	// Non-interactive demo
	{
		{
			"ite",
			"Demo 1",
			{
	{"ited.rsc",		GAME_RESOURCEFILE,	"addfc9d82bc2fa1f4cab23743c652c08", 1131098},
	{"scriptsd.rsc",	GAME_SCRIPTFILE,	"fded5c59b8b7c5976229f960d21e6b0b", 38613},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformMacintosh,
			Common::ADGF_DEMO
		},
		GType_ITE,
		GID_ITE_MACDEMO1,
		GF_BIG_ENDIAN_DATA | GF_WYRMKEEP | GF_CD_FX | GF_NON_INTERACTIVE,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEWINDEMO_GameFonts),
		ITEWINDEMO_GameFonts,
		&ITEDEMO_GameVoice,
		&ITEMAC_GameSound,
		ARRAYSIZE(ITEMacPatch_Files),
		ITEMacPatch_Files,
	},


	// Inherit the earth - Win32 Demo version 2/3, Linux Demo version
	// Win32 Version 3 and Linux Demo version have digital music, Win32 version 2 has MIDI music
	{
		{
			"ite",
			"Win Demo 2/3, Linux Demo",
			{
	{"ited.rsc",		GAME_RESOURCEFILE,	"3a450852cbf3c80773984d565647e6ac", 1951395},
	{"scriptsd.rsc",	GAME_SCRIPTFILE,	"3f12b67fa93e56e1a6be39d2921d80bb", 70051},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			Common::ADGF_DEMO
		},
		GType_ITE,
		GID_ITE_WINDEMO3,
		GF_WYRMKEEP | GF_CD_FX | GF_SCENE_SUBSTITUTES,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEWINDEMO_GameFonts),
		ITEWINDEMO_GameFonts,
		&ITEDEMO_GameVoice,
		&ITEPC_GameSound,
		ARRAYSIZE(ITEPatch_Files),
		ITEPatch_Files,
	},


	// Inherit the earth - Win32 Demo version 1
	// Non-interactive demo
	{
		{
			"ite",
			"Demo 1",
			{
	{"ited.rsc",		GAME_RESOURCEFILE,	"3a450852cbf3c80773984d565647e6ac", 1327323},
	{"scriptsd.rsc",	GAME_SCRIPTFILE,	"3f12b67fa93e56e1a6be39d2921d80bb", 38613},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			Common::ADGF_DEMO
		},
		GType_ITE,
		GID_ITE_WINDEMO1,
		GF_WYRMKEEP | GF_CD_FX | GF_NON_INTERACTIVE,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEWINDEMO_GameFonts),
		ITEWINDEMO_GameFonts,
		&ITEWINDEMO1_GameSound,
		&ITEWINDEMO1_GameSound,
		ARRAYSIZE(ITEPatch_Files),
		ITEPatch_Files,
	},


	// TODO: Add Amiga demos here (not supported yet)


	// ITE Mac versions ///////////////////////////////////////////////////////////////////////////////////////

	// Inherit the earth - MAC CD Guild version
	{
		{
			"ite",
			"CD",
			{
	{"ite resources.bin",	GAME_RESOURCEFILE | GAME_MACBINARY,	"0bd506aa887bfc7965f695c6bd28237d", -1},
	{"ite scripts.bin",		GAME_SCRIPTFILE | GAME_MACBINARY,	"af0d7a2588e09ad3ecbc5b474ea238bf", -1},
	{"ite sounds.bin",		GAME_SOUNDFILE | GAME_MACBINARY,	"441426c6bb2a517f65c7e49b57f7a345", -1},
	{"ite music.bin",		GAME_MUSICFILE_GM | GAME_MACBINARY,	"c1d20324b7cdf1650e67061b8a93251c", -1},
	{"ite voices.bin",		GAME_VOICEFILE | GAME_MACBINARY,	"dba92ae7d57e942250fe135609708369", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformMacintosh,
			Common::ADGF_NO_FLAGS
		},
		GType_ITE,
		GID_ITE_MACCD_G,
		GF_BIG_ENDIAN_DATA | GF_CD_FX,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEWINDEMO_GameFonts),
		ITEWINDEMO_GameFonts,
		&ITEMACCD_G_GameSound,
		&ITEMACCD_G_GameSound,
		0,
		NULL,
	},

	// Inherit the earth - MAC CD Wyrmkeep version
	{
		{
			"ite",
			"Wyrmkeep CD",
			{
	{"ite.rsc",						GAME_RESOURCEFILE,	"4f7fa11c5175980ed593392838523060", -1},
	{"scripts.rsc",					GAME_SCRIPTFILE,	"adf1f46c1d0589083996a7060c798ad0", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformMacintosh,
			Common::ADGF_NO_FLAGS
		},
		GType_ITE,
		GID_ITE_MACCD,
		GF_BIG_ENDIAN_DATA | GF_WYRMKEEP | GF_CD_FX,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEWINDEMO_GameFonts),
		ITEWINDEMO_GameFonts,
		&ITEMAC_GameSound,
		&ITEMAC_GameSound,
		ARRAYSIZE(ITEMacPatch_Files),
		ITEMacPatch_Files,
	},


	// ITE PC CD versions //////////////////////////////////////////////////////////////////////////////////////

	// Inherit the earth - Wyrmkeep combined Windows/Mac/Linux CD

	// version is different from the other Wyrmkeep re-releases in that it does
	// not have any substitute files. Presumably the ite.rsc file has been
	// modified to include the Wyrmkeep changes. The resource files are little-
	// endian, except for the voice file which is big-endian.
	{
		{
			"ite",
			"Multi-OS CD Version",
			{
	{"ite.rsc",						GAME_RESOURCEFILE,					"a6433e34b97b15e64fe8214651012db9", -1},
	{"scripts.rsc",					GAME_SCRIPTFILE,					"a891405405edefc69c9d6c420c868b84", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformUnknown,
			Common::ADGF_NO_FLAGS
		},
		GType_ITE,
		GID_ITE_MULTICD,
		GF_WYRMKEEP | GF_CD_FX,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITE_GameFonts),
		ITE_GameFonts,
		&ITEMAC_GameSound,
		&ITEPC_GameSound,
		0,
		NULL,
	},

	// Inherit the earth - Italian Wyrmkeep combined Windows/Mac/Linux CD (fan translation)

	// version is different from the other Wyrmkeep re-releases in that it does
	// not have any substitute files. Presumably the ite.rsc file has been
	// modified to include the Wyrmkeep changes. The resource files are little-
	// endian, except for the voice file which is big-endian.
	{
		{
			"ite",
			"Multi-OS CD Version",
			{
	{"ite.rsc",						GAME_RESOURCEFILE,					"a6433e34b97b15e64fe8214651012db9", -1},
	{"scripts.rsc",					GAME_SCRIPTFILE,					"a891405405edefc69c9d6c420c868b84", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::IT_ITA,
			Common::kPlatformUnknown,
			Common::ADGF_NO_FLAGS
		},
		GType_ITE,
		GID_ITE_MULTICD,
		GF_WYRMKEEP | GF_CD_FX,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITE_GameFonts),
		ITE_GameFonts,
		&ITEMAC_GameSound,
		&ITEPC_GameSound,
		0,
		NULL,
	},

	// Inherit the earth - Windows/Linux/DOS CD version
	{
		{
			"ite",
			"Windows/Linux/DOS CD Version",
			{
	{"ite.rsc",		GAME_RESOURCEFILE,	"8f4315a9bb10ec839253108a032c8b54", -1},
	{"scripts.rsc",	GAME_SCRIPTFILE,	"a891405405edefc69c9d6c420c868b84", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_ITE,
		GID_ITE_CD,
		GF_CD_FX,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITE_GameFonts),
		ITE_GameFonts,
		&ITEPC_GameSound,
		&ITEPC_GameSound,
		ARRAYSIZE(ITEPatch_Files),
		ITEPatch_Files,
	},

	// Inherit the earth - Italian Windows/Linux/DOS CD version (fan translation)
	{
		{
			"ite",
			"Windows/Linux/DOS CD Version",
			{
	{"ite.rsc",		GAME_RESOURCEFILE,	"8f4315a9bb10ec839253108a032c8b54", -1},
	{"scripts.rsc",	GAME_SCRIPTFILE,	"a891405405edefc69c9d6c420c868b84", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::IT_ITA,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_ITE,
		GID_ITE_CD,
		GF_CD_FX,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITE_GameFonts),
		ITE_GameFonts,
		&ITEPC_GameSound,
		&ITEPC_GameSound,
		ARRAYSIZE(ITEPatch_Files),
		ITEPatch_Files,
	},

	// Inherit the earth - DOS CD version
	{
		{
			"ite",
			"CD Version",
			{
	{"ite.rsc",		GAME_RESOURCEFILE,	"8f4315a9bb10ec839253108a032c8b54", -1},
	{"scripts.rsc",	GAME_SCRIPTFILE,	"50a0d2d7003c926a3832d503c8534e90", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_ITE,
		GID_ITE_CD_G,
		GF_CD_FX,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITE_GameFonts),
		ITE_GameFonts,
		&ITEPC_GameSound,
		&ITEPC_GameSound,
		ARRAYSIZE(ITEPatch_Files),
		ITEPatch_Files,
	},

	// Inherit the earth - DOS CD German version
	// reported by mld. Bestsellergamers cover disk
	{
		{
			"ite",
			"CD Version",
			{
	{"ite.rsc",		GAME_RESOURCEFILE,	"869fc23c8f38f575979ec67152914fee", -1},
	{"scripts.rsc",	GAME_SCRIPTFILE,	"a891405405edefc69c9d6c420c868b84", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::DE_DEU,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_ITE,
		GID_ITE_CD_DE,
		GF_CD_FX,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITE_GameFonts),
		ITE_GameFonts,
		&ITEPC_GameSound,
		&ITEPC_GameSound,
		0,
		NULL,
	},


	// ITE floppy versions ////////////////////////////////////////////////////////////////////////////////////

	// Inherit the earth - German Floppy version
	{
		{
			"ite",
			"Floppy",
			{
	{"ite.rsc",		GAME_RESOURCEFILE,					"869fc23c8f38f575979ec67152914fee", -1},
	{"scripts.rsc",	GAME_SCRIPTFILE,					"516f7330f8410057b834424ea719d1ef", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::DE_DEU,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_ITE,
		GID_ITE_DISK_DE,
		0,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITE_GameFonts),
		ITE_GameFonts,
		&ITEDISK_GameSound,
		&ITEDISK_GameSound,
		0,
		NULL,
	},

	// Inherit the earth - Disk version
	{
		{
			"ite",
			"Floppy",
			{
	{"ite.rsc",		GAME_RESOURCEFILE,					"8f4315a9bb10ec839253108a032c8b54", -1},
	{"scripts.rsc",	GAME_SCRIPTFILE,					"516f7330f8410057b834424ea719d1ef", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_ITE,
		GID_ITE_DISK_G,
		0,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITE_GameFonts),
		ITE_GameFonts,
		&ITEDISK_GameSound,
		&ITEDISK_GameSound,
		ARRAYSIZE(ITEPatch_Files),
		ITEPatch_Files,
	},

	// Inherit the earth - Italian Disk version (fan translation)
	{
		{
			"ite",
			"Floppy",
			{
	{"ite.rsc",		GAME_RESOURCEFILE,					"8f4315a9bb10ec839253108a032c8b54", -1},
	{"scripts.rsc",	GAME_SCRIPTFILE,					"516f7330f8410057b834424ea719d1ef", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::IT_ITA,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_ITE,
		GID_ITE_DISK_G,
		0,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITE_GameFonts),
		ITE_GameFonts,
		&ITEDISK_GameSound,
		&ITEDISK_GameSound,
		ARRAYSIZE(ITEPatch_Files),
		ITEPatch_Files,
	},

	// ITE Amiga versions /////////////////////////////////////////////////////////////////////////////////////

	// TODO: Add the Amiga versions here (not supported yet)


	// IHNM Section ///////////////////////////////////////////////////////////////////////////////////////////

	// I Have No Mouth And I Must Scream - Demo version
	{
		{
			"ihnm",
			"Demo",
			{
	{"music.res",		GAME_MUSICFILE_FM,	"0439083e3dfdc51b486071d45872ae52", -1},
	{"scream.res",		GAME_RESOURCEFILE,	"46bbdc65d164ba7e89836a0935eec8e6", -1},
	{"scripts.res",		GAME_SCRIPTFILE,	"9626bda8978094ff9b29198bc1ed5f9a", -1},
	{"sfx.res",			GAME_SOUNDFILE,		"1c610d543f32ec8b525e3f652536f269", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformPC,
			Common::ADGF_DEMO
		},
		GType_IHNM,
		GID_IHNM_DEMO,
		0,
		IHNMDEMO_DEFAULT_SCENE,
		&IHNMDEMO_Resources,
		ARRAYSIZE(IHNMDEMO_GameFonts),
		IHNMDEMO_GameFonts,
		&IHNM_GameSound,
		&IHNM_GameSound,
		0,
		NULL,
	},

	// I Have No Mouth And I Must Scream - English CD version
	{
		{
			"ihnm",
			"",
			{
	{"musicfm.res",	GAME_MUSICFILE_FM,					"0439083e3dfdc51b486071d45872ae52", -1},
	{"musicgm.res",	GAME_MUSICFILE_GM,					"80f875a1fb384160d1f4b27166eef583", -1},
	{"scream.res",	GAME_RESOURCEFILE,					"46bbdc65d164ba7e89836a0935eec8e6", -1},
	{"patch.re_",	GAME_PATCHFILE | GAME_RESOURCEFILE,	"58b79e61594779513c7f2d35509fa89e", -1},
	{"scripts.res",	GAME_SCRIPTFILE,					"be38bbc5a26be809dbf39f13befebd01", -1},
	//{"sfx.res",		GAME_SOUNDFILE,						"1c610d543f32ec8b525e3f652536f269", -1},
	// There are two English versions of the game, each one with a different sfx.res file
	// Known MD5 checksums for sfx.res in the English version of the game are 
	// 1c610d543f32ec8b525e3f652536f269 and 45a9a9f5d37740be24fd2ae2edf36573
	{"sfx.res",		GAME_SOUNDFILE,						NULL, -1},
	{ NULL, 0, NULL, 0}
			},
			Common::EN_ANY,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_IHNM,
		GID_IHNM_CD,
		0,
		IHNM_DEFAULT_SCENE,
		&IHNM_Resources,
		ARRAYSIZE(IHNMCD_GameFonts),
		IHNMCD_GameFonts,
		&IHNM_GameSound,
		&IHNM_GameSound,
		0,
		NULL,
	},

	// I Have No Mouth And I Must Scream - De CD version
	// Censored CD version (without Nimdok)
	// Reported by mld. German Retail
	{
		{
			"ihnm",
			"",
			{
	{"musicfm.res",	GAME_MUSICFILE_FM,					"0439083e3dfdc51b486071d45872ae52", -1},
	{"musicgm.res",	GAME_MUSICFILE_GM,					"80f875a1fb384160d1f4b27166eef583", -1},
	{"scream.res",	GAME_RESOURCEFILE,					"c92370d400e6f2a3fc411c3729d09224", -1},
	{"scripts.res",	GAME_SCRIPTFILE,					"32aa01a89937520fe0ea513950117292", -1},
	{"patch.re_",	GAME_PATCHFILE | GAME_RESOURCEFILE,	"58b79e61594779513c7f2d35509fa89e", -1},
	{"sfx.res",		GAME_SOUNDFILE,						"1c610d543f32ec8b525e3f652536f269", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::DE_DEU,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_IHNM,
		GID_IHNM_CD_DE,
		0,
		IHNM_DEFAULT_SCENE,
		&IHNM_Resources,
		ARRAYSIZE(IHNMCD_GameFonts),
		IHNMCD_GameFonts,
		&IHNM_GameSound,
		&IHNM_GameSound,
		0,
		NULL,
	},

	// I Have No Mouth And I Must Scream - Sp CD version
	{
		{
			"ihnm",
			"",
			{
	{"musicfm.res",	GAME_MUSICFILE_FM,					"0439083e3dfdc51b486071d45872ae52", -1},
	{"musicgm.res",	GAME_MUSICFILE_GM,					"80f875a1fb384160d1f4b27166eef583", -1},
	{"scream.res",	GAME_RESOURCEFILE,					"c92370d400e6f2a3fc411c3729d09224", -1},
	{"patch.re_",	GAME_PATCHFILE | GAME_RESOURCEFILE,	"58b79e61594779513c7f2d35509fa89e", -1},
	{"scripts.res",	GAME_SCRIPTFILE,					"be38bbc5a26be809dbf39f13befebd01", -1},
	{"sfx.res",		GAME_SOUNDFILE,						"1c610d543f32ec8b525e3f652536f269", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::ES_ESP,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_IHNM,
		GID_IHNM_CD_ES,
		0,
		IHNM_DEFAULT_SCENE,
		&IHNM_Resources,
		ARRAYSIZE(IHNMCD_GameFonts),
		IHNMCD_GameFonts,
		&IHNM_GameSound,
		&IHNM_GameSound,
		0,
		NULL,
	},

	// I Have No Mouth And I Must Scream - Ru CD version
	{
		{
			"ihnm",
			"",
			{
	{"musicfm.res",	GAME_MUSICFILE_FM,					"0439083e3dfdc51b486071d45872ae52", -1},
	{"musicgm.res",	GAME_MUSICFILE_GM,					"80f875a1fb384160d1f4b27166eef583", -1},
	{"scream.res",	GAME_RESOURCEFILE,					"46bbdc65d164ba7e89836a0935eec8e6", -1},
	{"patch.re_",	GAME_PATCHFILE | GAME_RESOURCEFILE,	"58b79e61594779513c7f2d35509fa89e", -1},
	{"scripts.res",	GAME_SCRIPTFILE,					"be38bbc5a26be809dbf39f13befebd01", -1},
	{"sfx.res",		GAME_SOUNDFILE,						"1c610d543f32ec8b525e3f652536f269", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::RU_RUS,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_IHNM,
		GID_IHNM_CD_RU,
		0,
		IHNM_DEFAULT_SCENE,
		&IHNM_Resources,
		ARRAYSIZE(IHNMCD_GameFonts),
		IHNMCD_GameFonts,
		&IHNM_GameSound,
		&IHNM_GameSound,
		0,
		NULL,
	},

	// I Have No Mouth And I Must Scream - Fr CD version
	// Censored CD version (without Nimdok)
	{
		{
			"ihnm",
			"",
			{
	{"musicfm.res",	GAME_MUSICFILE_FM,					"0439083e3dfdc51b486071d45872ae52", -1},
	{"musicgm.res",	GAME_MUSICFILE_GM,					"80f875a1fb384160d1f4b27166eef583", -1},
	{"scream.res",	GAME_RESOURCEFILE,					"c92370d400e6f2a3fc411c3729d09224", -1},
	{"scripts.res",	GAME_SCRIPTFILE,					"32aa01a89937520fe0ea513950117292", -1},
	{"patch.re_",	GAME_PATCHFILE | GAME_RESOURCEFILE,	"58b79e61594779513c7f2d35509fa89e", -1},
	{"sfx.res",		GAME_SOUNDFILE,						"1c610d543f32ec8b525e3f652536f269", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::FR_FRA,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_IHNM,
		GID_IHNM_CD_FR,
		0,
		IHNM_DEFAULT_SCENE,
		&IHNM_Resources,
		ARRAYSIZE(IHNMCD_GameFonts),
		IHNMCD_GameFonts,
		&IHNM_GameSound,
		&IHNM_GameSound,
		0,
		NULL,
	},

	// I Have No Mouth And I Must Scream - Italian fan CD translation
	{
		{
			"ihnm",
			"",
			{
	{"musicfm.res",	GAME_MUSICFILE_FM,					"0439083e3dfdc51b486071d45872ae52", -1},
	{"musicgm.res",	GAME_MUSICFILE_GM,					"80f875a1fb384160d1f4b27166eef583", -1},
	{"scream.res",	GAME_RESOURCEFILE,					"46bbdc65d164ba7e89836a0935eec8e6", -1},
	{"patch.re_",	GAME_PATCHFILE | GAME_RESOURCEFILE,	"58b79e61594779513c7f2d35509fa89e", -1},
	{"scripts.res",	GAME_SCRIPTFILE,					"be38bbc5a26be809dbf39f13befebd01", -1},
	{"sfx.res",		GAME_SOUNDFILE,						"1c610d543f32ec8b525e3f652536f269", -1},
	{ NULL, 0, NULL, 0}
			},
			Common::IT_ITA,
			Common::kPlatformPC,
			Common::ADGF_NO_FLAGS
		},
		GType_IHNM,
		GID_IHNM_CD_ITA,
		0,
		IHNM_DEFAULT_SCENE,
		&IHNM_Resources,
		ARRAYSIZE(IHNMCD_GameFonts),
		IHNMCD_GameFonts,
		&IHNM_GameSound,
		&IHNM_GameSound,
		0,
		NULL,
	},
	{ AD_TABLE_END_MARKER, 0, 0, 0, 0, NULL, 0, NULL, NULL, NULL, 0, NULL }
};

} // End of namespace Saga

#include "voxelquest/setting.h"

Settings::Settings()
{
	settings[E_BS_PLACING_GEOM]=false;
	settings[E_BS_DEBUG_VIEW]=false;
	settings[E_BS_VSYNC]=true;
	settings[E_BS_RENDER_OCT_BOUNDS]=false;
	settings[E_BS_RENDER_VOXELS]=false;
	settings[E_BS_SHOW_GRID]=false;
	settings[E_BS_FOG]=true;
	settings[E_BS_PHSYICS]=true;
	settings[E_BS_FXAA]=false;
	settings[E_BS_WATER_BULLET]=false;
	settings[E_BS_PATH_FINDING]=false;
	settings[E_BS_PATH_FINDING_GEN]=false;
	settings[E_BS_PATH_FINDING_TEST]=false;
	settings[E_BS_TEST_1]=false;
	settings[E_BS_TEST_2]=false;
	settings[E_BS_TEST_3]=false;
	settings[E_BS_WATER]=true;
	settings[E_BS_TREES]=true;
	settings[E_BS_UPDATE_HOLDERS]=true;
	settings[E_BS_PLACING_PATTERN]=false;
	settings[E_BS_DRAW_TARG_PATHS]=false;
	settings[E_BS_UPDATE_FLUID]=false;
	settings[E_BS_RADIOSITY]=true;
	settings[E_BS_DESTROY_TERRAIN]=false;
	settings[E_BS_TURN_BASED]=false;
	settings[E_BS_MIRROR_POSE]=true;
	settings[E_BS_COMBAT]=false;
	settings[E_BS_EDIT_POSE]=false;
	settings[E_BS_SHOW_HEALTH]=false;

    gammaVal=0.5f;

	cavesOn=false;

	heightMapMaxInCells=HM_MAX_IN_CELLS;

	voxelsPerCell=VOXELS_PER_CELL;
	paddingInCells=PADDING_IN_CELLS;

	cellsPerHolder=CELLS_PER_HOLDER;
	holdersPerChunk=HOLDERS_PER_CHUNK;
	chunksPerBlock=CHUNKS_PER_BLOCK;
	holdersPerWorld=HOLDERS_PER_WORLD;// newPitch;

	cellsPerNodeXY=16;
	cellsPerNodeZ=8;

	holdersPerBlock=holdersPerChunk*chunksPerBlock;//8 * HOLDER_MOD;
	cellsPerWorld=holdersPerWorld*cellsPerHolder;
	cellsPerBlock=holdersPerBlock*cellsPerHolder;
	blocksPerWorld=holdersPerWorld/holdersPerBlock;
	cellsPerChunk=cellsPerHolder*holdersPerChunk;
	chunksPerWorld=blocksPerWorld*chunksPerBlock;

	cellsPerHolderPad=cellsPerHolder+paddingInCells*2;
	voxelsPerHolderPad=voxelsPerCell*cellsPerHolderPad;
	voxelsPerHolder=voxelsPerCell*cellsPerHolder;

	terDataTexScale=1;

	terDataVisPitchXY=cellsPerBlock/cellsPerNodeXY;
	iNodeDivsPerHolder=terDataVisPitchXY/holdersPerBlock;
	terDataBufAmount=iNodeDivsPerHolder; // pad with one extra lot
	terDataBufPitchXY=terDataVisPitchXY+terDataBufAmount*2;
	terDataBufPitchScaledXY=terDataBufPitchXY*terDataTexScale;

	terDataVisPitchZ=cellsPerBlock/cellsPerNodeZ;
	terDataBufPitchZ=terDataVisPitchZ+terDataBufAmount*2;
	terDataBufPitchScaledZ=terDataBufPitchZ*terDataTexScale;

	terDataVisSize=terDataVisPitchXY*terDataVisPitchXY * terDataVisPitchZ;
	terDataBufSize=terDataBufPitchXY*terDataBufPitchXY * terDataBufPitchZ;
	terDataBufSizeScaled=terDataBufPitchScaledXY*terDataBufPitchScaledXY * terDataBufPitchScaledZ;

	floorHeightInCells=(float)cellsPerNodeZ;
	roofHeightInCells=((float)cellsPerNodeXY)/4.0f;
	wallRadInCells=((float)cellsPerNodeXY)/4.0f;

	guiSaveLoc="..\\data\\saves\\save0\\compMap.txt";

    iNumSteps=16;
}

void Settings::speakSetting(int settingName)
{
	string speakString=makePretty(E_BOOL_SETTING_STRINGS[settingName], "E_BS_");

	if(settings[settingName])
	{
		speakString+=" on  ";
	}
	else
	{
		speakString+=" off  ";
	}

	speak(speakString);
}

void Settings::toggleSetting(int settingName, bool withVoice=true)
{
	settings[settingName]=!(settings[settingName]);
	if(withVoice)
	{
		speakSetting(settingName);
	}
}

void Settings::setSetting(int settingName, bool value, bool withVoice=true)
{
	settings[settingName]=value;
	if(withVoice)
	{
		speakSetting(settingName);
	}
}

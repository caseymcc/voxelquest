#ifndef _voxelquest_settings_h_
#define _voxelquest_settings_h_

#include "voxelquest/enums.h"

#include <string>

const static bool DO_CACHE=false;
const static int NUM_MIP_LEVELS=4;
const static int NUM_MIP_LEVELS_WITH_FIRST=NUM_MIP_LEVELS+1;
const static bool DO_AO=false;
const static bool DO_MIP=true;
const static bool DO_POINTS=true;
const static int MAX_THREADS=6;
// const static int NORM_RAD = 2;
// const static int AO_RAD = 1;
const static int MAX_HOLDER_LOAD_COUNT=512;
//const static int RASTER_HOLDER_RAD = 8;

const static int VOXELS_PER_CELL=16;
const static int CELLS_PER_HOLDER=8;
const static int HOLDERS_PER_CHUNK=2;
const static int CHUNKS_PER_BLOCK=32;


const static int HOLDER_MOD=2; // HOLDER_MOD*CELLS_PER_HOLDER should == 16
const static int HOLDERS_PER_WORLD=2048*2*HOLDER_MOD; //hack for moment

const static int PADDING_IN_CELLS=1;
const static float HM_MAX_IN_CELLS=CELLS_PER_HOLDER*256*HOLDER_MOD;//8192.0f;

const static int MAX_PDPOOL_SIZE=MAX_THREADS;
// const static int MAX_TBOPOOL_SIZE = 8;

#define E_BOOL_SETTING(DDD) \
DDD(E_BS_DEBUG_VIEW) \
DDD(E_BS_VSYNC) \
DDD(E_BS_RENDER_OCT_BOUNDS) \
DDD(E_BS_RENDER_VOXELS) \
DDD(E_BS_SHOW_GRID) \
DDD(E_BS_FOG) \
DDD(E_BS_PHSYICS) \
DDD(E_BS_FXAA) \
DDD(E_BS_WATER_BULLET) \
DDD(E_BS_PATH_FINDING) \
DDD(E_BS_PATH_FINDING_GEN) \
DDD(E_BS_PATH_FINDING_TEST) \
DDD(E_BS_TEST_1) \
DDD(E_BS_TEST_2) \
DDD(E_BS_TEST_3) \
DDD(E_BS_WATER) \
DDD(E_BS_TREES) \
DDD(E_BS_UPDATE_HOLDERS) \
DDD(E_BS_PLACING_PATTERN) \
DDD(E_BS_DRAW_TARG_PATHS) \
DDD(E_BS_UPDATE_FLUID) \
DDD(E_BS_RADIOSITY) \
DDD(E_BS_DESTROY_TERRAIN) \
DDD(E_BS_TURN_BASED) \
DDD(E_BS_MIRROR_POSE) \
DDD(E_BS_COMBAT) \
DDD(E_BS_EDIT_POSE) \
DDD(E_BS_SHOW_HEALTH) \
DDD(E_BS_PLACING_GEOM) \
DDD(E_BS_LENGTH)

std::string E_BOOL_SETTING_STRINGS[]={
    E_BOOL_SETTING(DO_DESCRIPTION)
};

enum E_BOOL_SETTING_VALS
{
    E_BOOL_SETTING(DO_ENUM)
};


class Settings
{
public:
    Settings();

    
    bool settings[E_BS_LENGTH];
    bool cavesOn;
    float heightMapMaxInCells;

    int cellsPerHolder;
    int cellsPerHolderPad;
    int voxelsPerHolder;
    int voxelsPerHolderPad;
    int cellsPerBlock;
    int holdersPerBlock;
    int holdersPerChunk;
    int chunksPerBlock;
    int chunksPerWorld;
    int cellsPerChunk;

    int cellsPerWorld;
    int holdersPerWorld;
    int blocksPerWorld;

    int voxelsPerCell;

    int paddingInCells;

    int cellsPerNodeXY;
    int cellsPerNodeZ;

    int terDataVisPitchXY;
    int terDataBufPitchXY;
    int terDataBufPitchScaledXY;
    int terDataVisPitchZ;
    int terDataBufPitchZ;
    int terDataBufPitchScaledZ;
    int terDataBufAmount;
    int terDataVisSize;
    int terDataBufSize;
    int terDataTexScale;
    int terDataBufSizeScaled;
    int iNodeDivsPerHolder;

    float floorHeightInCells;
    float roofHeightInCells;
    float wallRadInCells;


    
};

Settings g_settings;

inline Settings::Settings()
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

    
}

#endif//_voxelquest_settings_h_
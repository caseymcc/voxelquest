#ifndef _voxelquest_settings_h_
#define _voxelquest_settings_h_


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

class Settings
{
public:
    Settings();

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
};

Settings g_settings;

inline Settings::Settings()
{
    voxelsPerCell=VOXELS_PER_CELL;
    paddingInCells=PADDING_IN_CELLS;

    cellsPerHolder=CELLS_PER_HOLDER;
    holdersPerChunk=HOLDERS_PER_CHUNK;
    chunksPerBlock=CHUNKS_PER_BLOCK;
    holdersPerWorld=HOLDERS_PER_WORLD;// newPitch;

    holdersPerBlock=holdersPerChunk*chunksPerBlock;//8 * HOLDER_MOD;
    cellsPerWorld=holdersPerWorld*cellsPerHolder;
    cellsPerBlock=holdersPerBlock*cellsPerHolder;
    blocksPerWorld=holdersPerWorld/holdersPerBlock;
    cellsPerChunk=cellsPerHolder*holdersPerChunk;
    chunksPerWorld=blocksPerWorld*chunksPerBlock;

    cellsPerHolderPad=cellsPerHolder+paddingInCells*2;
    voxelsPerHolderPad=voxelsPerCell*cellsPerHolderPad;
    voxelsPerHolder=voxelsPerCell*cellsPerHolder;
}

#endif//_voxelquest_settings_h_
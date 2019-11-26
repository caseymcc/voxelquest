#ifndef _voxelquest_gamechunk_h_
#define _voxelquest_gamechunk_h_

#include "voxelquest/vbos.h"
#include "voxelquest/gamepageholder.h"

class GameChunk
{
public:
    GameChunk();

    void init(
        Singleton* _singleton,
        int _blockId,
        int _chunkId,

        int trueX,
        int trueY,
        int trueZ

    );

    void makeDirty();

    VBOWrapper* getCurVBO();

    void drawLoadingHolders();

    void checkHolders();

    void reset();

    void fillVBO();

private:

    Singleton *singleton;
    VBOWrapper vboWrapper[NUM_MIP_LEVELS_WITH_FIRST];

    int lastPointCount;
    int mipLev;
    int changeTick;

    bool readyToRender;
    bool listEmpty;
    bool changeFlag;
    bool isDirty;

    int iHolderSize;
    int holdersPerChunk;
    GamePageHolder **holderData;

    FIVector4 offsetInChunks;
    FIVector4 chunkCenInCells;

    int chunkId;
    int blockId;

    std::vector<ObjectStruct> localObjects;




};

#endif//_voxelquest__h_
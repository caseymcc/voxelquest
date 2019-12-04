#ifndef _voxelquest_gamevoxelwrap_h_
#define _voxelquest_gamevoxelwrap_h_

#include "voxelquest/voxelbuffer.h"
#include "voxelquest/fbos.h"
#include "voxelquest/memorypool.h"

class GamePageHolder;
class Singleton;

class GameVoxelWrap
{
public:
    GameVoxelWrap();

    void init(
        Singleton* _singleton
        //,int _dimInVoxels
    );

    void fillVec();

    void process(GamePageHolder* _gph);

    bool findNextCoord(ivec3* voxResult);

    bool inBounds(ivec3* pos, int minB, int maxB);

    int getNode(ivec3* pos);

    void floodFill(ivec3 startVox);

    bool isInvSurfaceVoxel(ivec3* pos, int ignorePtr, int &curPtr, bool checkVisited);

    bool isSurfaceVoxel(ivec3* pos, int &curPtr, bool checkVisited);

    int getVoxelAtCoord(ivec3* pos);

    // todo: 
    // mark cells as visited  <---- important
    // get rid of DONE_WITH_IT
    // should be able to check 6 faces of this holder instead for starting surface point?

    float sampLinear(vec3* pos, vec3 offset);

    PaddedDataEntry* getPadData(int ii, int jj, int kk);

    float rand2D(vec3 co);

    vec3 randPN(vec3 co);

    void getVoro(vec3* worldPos, vec3* worldClosestCenter, vec3* otherData, float fSpacing);

    // should only be called when a new node is inserted!
    void calcVoxel(ivec3* _pos, int octPtr, int VLIndex);

private:
    Singleton* singleton;


    VoxelBuffer* voxelBuffer;
    PaddedData* basePD;
    PaddedDataEntry* baseData;
    //ivec3 octOffsetInVoxels;

    GamePageHolder* gph;

    int lastFFSteps;
    int curPD;
    //int dimInVoxels;
    //int octInVoxels;
    float fVoxelsPerCell;
    int voxelsPerCell;
    int cellsPerHolder;
    int cellsPerHolderPad;
    int voxelsPerHolderPad;
    int voxelsPerHolder;
    int paddingInCells;
    int paddingInVoxels;

    ivec3 offsetInCells;
    ivec3 offsetInVoxels;
    vec3 fOffsetInVoxels;

    vec3 oneVec;
    vec3 halfOff;
    vec3 crand0;
    vec3 crand1;
    vec3 crand2;

    FBOWrapper *hmFBO;
};








#endif//_voxelquest__h_
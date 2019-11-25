#ifndef _voxelquest_gamevoxelwrap_h_
#define _voxelquest_gamevoxelwrap_h_

#include "voxelquest/voxelbuffer.h"

#include <glm/glm.hpp>

struct PaddedDataEntry
{
    float terVal;
    int cellVal;
    bool visited;
};

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

    bool findNextCoord(glm::ivec3* voxResult);

    bool inBounds(glm::ivec3* pos, int minB, int maxB);

    int getNode(glm::ivec3* pos);

    void floodFill(glm::ivec3 startVox);

    bool isInvSurfaceVoxel(glm::ivec3* pos, int ignorePtr, int &curPtr, bool checkVisited);

    bool isSurfaceVoxel(glm::ivec3* pos, int &curPtr, bool checkVisited);

    int getVoxelAtCoord(glm::ivec3* pos);

    // todo: 
    // mark cells as visited  <---- important
    // get rid of DONE_WITH_IT
    // should be able to check 6 faces of this holder instead for starting surface point?

    float sampLinear(glm::vec3* pos, glm::vec3 offset);

    PaddedDataEntry* getPadData(int ii, int jj, int kk);

    float rand2D(glm::vec3 co);

    glm::vec3 randPN(glm::vec3 co);

    void getVoro(glm::vec3* worldPos, glm::vec3* worldClosestCenter, glm::vec3* otherData, float fSpacing);

    // should only be called when a new node is inserted!
    void calcVoxel(glm::ivec3* _pos, int octPtr, int VLIndex);

private:
    Singleton* singleton;


    VoxelBuffer* voxelBuffer;
    PaddedData* basePD;
    PaddedDataEntry* baseData;
    //glm::ivec3 octOffsetInVoxels;

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

    glm::ivec3 offsetInCells;
    glm::ivec3 offsetInVoxels;
    glm::vec3 fOffsetInVoxels;

    glm::vec3 oneVec;
    glm::vec3 halfOff;
    glm::vec3 crand0;
    glm::vec3 crand1;
    glm::vec3 crand2;

    FBOWrapper *hmFBO;
};








#endif//_voxelquest__h_
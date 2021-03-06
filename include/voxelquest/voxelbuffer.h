#ifndef _voxelquest_voxelbuffer_h_
#define _voxelquest_voxelbuffer_h_

#include "voxelquest/types.h"
#include "voxelquest/settings.h"
#include "voxelquest/ivec.h"
#include "voxelquest/mat.h"

struct intTrip
{
    int v0; // blockId
    int v1; // chunkId
    int v2; // holderId
};

struct VoxelBufferEntry
{
    uint flags;
    int vbeIndex;
};

struct VoxelInfo
{
    int viIndex;
    uint normId;
    uint matId;
    vec3 normal;
    vec3 pos;
};

struct VoxelCell
{
    int curSize;
    int* indexArr;
};

struct VoxelMip
{
    bool* mipArr;
    //vector<int> mipList;
};

struct VoxelBuffer
{
    int addIndex(int val);

    // VoxelInfo* getVoxelInfo(int index) 

    uint getFlags(int flagPtr);

    bool getFlag(int flagPtr, uint flagVal);

    void setFlag(int flagPtr, uint flagVal);

    void clearFlag(int flagPtr, uint flagVal);

    int addNode(VectorI3* pos, bool &wasNew);

    uint getFlagsAtNode(int ind);

    uint getIndAtNode(int ind);

    void clearAllNodes();


    VoxelBufferEntry* data;
    VoxelCell* cellLists;
    std::vector<VoxelInfo> voxelList;

    VoxelMip mipMaps[NUM_MIP_LEVELS];

    int vcSize;
    int vcPitch;

    int vbSize;
    int vbPitch;
};

#endif//_voxelquest_voxelbuffer_h_

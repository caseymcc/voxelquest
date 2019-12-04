#ifndef _voxelquest_memorypool_h_
#define _voxelquest_memorypool_h_

#include "voxelquest/voxelbuffer.h"

#include <vector>

struct PaddedDataEntry
{
    float terVal;
    int cellVal;
    bool visited;
};

struct PaddedData
{
    PaddedDataEntry* data;
    std::vector<VectorI3> fillStack;
    VoxelBuffer voxelBuffer;

    intTrip boundToHolder;
    bool isFree;
};

class MemoryPool
{
public:
    static MemoryPool *singleton();

    static PaddedData &pd(size_t index) 
    {
        return singleton()->pdPool[index];
    }

    PaddedData pdPool[MAX_PDPOOL_SIZE];

private:
    MemoryPool();//force singletion

};

#endif//_voxelquest_memorypool_h_
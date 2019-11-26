#ifndef _voxelquest_memorypool_h_
#define _voxelquest_memorypool_h_

#include "voxelquest/settings.h"

class MemoryPool
{
public:
    static MemoryPool *singleton();

    PaddedData pdPool[MAX_PDPOOL_SIZE];

    static PaddedData &pd(size_t index) 
    {
        return singleton()->pdPool[index];
    }

private:
    MemoryPool();//force singletion

};

#endif//_voxelquest_memorypool_h_
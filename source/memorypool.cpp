#include "voxelquest/memorypool.h"
#include "voxelquest/settings.h"

MemoryPool *MemoryPool::singleton()
{
    static MemoryPool memoryPool;

    return &memoryPool;
}

MemoryPool::MemoryPool()
{
    int curMipSize=0;
    int cellsPerHolderPad=g_settings.cellsPerHolderPad;
    int voxelsPerHolderPad=g_settings.voxelsPerHolderPad;
    int voxelsPerCell=g_settings.voxelsPerCell;

    for(int i=0; i<MAX_PDPOOL_SIZE; i++)
    {
        pdPool[i].data=new PaddedDataEntry[cellsPerHolderPad*cellsPerHolderPad*cellsPerHolderPad];
        //pdPool[i].voxData = new VoxEntry[voxelsPerCell*voxelsPerCell*voxelsPerCell]
        pdPool[i].isFree=true;

        pdPool[i].voxelBuffer.vcPitch=cellsPerHolderPad;
        pdPool[i].voxelBuffer.vcSize=cellsPerHolderPad*cellsPerHolderPad*cellsPerHolderPad;
        pdPool[i].voxelBuffer.cellLists=new VoxelCell[
            pdPool[i].voxelBuffer.vcSize
        ];

        pdPool[i].voxelBuffer.vbPitch=voxelsPerHolderPad;
        pdPool[i].voxelBuffer.vbSize=voxelsPerHolderPad*voxelsPerHolderPad*voxelsPerHolderPad;
        pdPool[i].voxelBuffer.data=new VoxelBufferEntry[
            pdPool[i].voxelBuffer.vbSize
        ];

        if(DO_MIP)
        {
            curMipSize=voxelsPerHolderPad/2;
            for(int j=0; j<NUM_MIP_LEVELS; j++)
            {
                pdPool[i].voxelBuffer.mipMaps[j].mipArr=new bool[curMipSize*curMipSize*curMipSize];

                curMipSize/=2;
            }
        }

        for(int j=0; j<pdPool[i].voxelBuffer.vcSize; j++)
        {
            pdPool[i].voxelBuffer.cellLists[j].curSize=0;
            pdPool[i].voxelBuffer.cellLists[j].indexArr=new int[
                voxelsPerCell*voxelsPerCell*voxelsPerCell
            ];
        }

        for(int j=0; j<pdPool[i].voxelBuffer.vbSize; j++)
        {
            pdPool[i].voxelBuffer.data[j].vbeIndex=-1;
            pdPool[i].voxelBuffer.data[j].flags=0;
        }

        // octPool[i] = new GameOctree();
        // octPool[i]->init(
        // 	this,
        // 	cellsPerHolder*voxelsPerCell*2,
        // 	1024*1024*2
        // );
    }
}

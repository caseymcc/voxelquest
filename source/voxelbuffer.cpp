#include "voxelquest/voxelbuffer.h"
#include "voxelquest/texenums.h"
#include "voxelquest/gameoctree.h"

int VoxelBuffer::addIndex(int val)
{
    voxelList.push_back(VoxelInfo());
    voxelList.back().viIndex=val;
    voxelList.back().normId=0;
    voxelList.back().matId=TEX_NULL;

    int VLInd=(int)(voxelList.size()-1);

    data[val].vbeIndex=VLInd;

    return VLInd;
}

// VoxelInfo* VoxelBuffer::getVoxelInfo(int index) {
// 	if (index < 0) {
// 		return NULL;
// 	}
// 	else {
// 		return &(voxelList[index]);
// 	}
// }

uint VoxelBuffer::getFlags(int flagPtr)
{
    return data[flagPtr].flags;
}

bool VoxelBuffer::getFlag(int flagPtr, uint flagVal)
{
    return (
        (data[flagPtr].flags & flagVal)>0
        );
}
void VoxelBuffer::setFlag(int flagPtr, uint flagVal)
{
    data[flagPtr].flags=data[flagPtr].flags|flagVal;
}
void VoxelBuffer::clearFlag(int flagPtr, uint flagVal)
{
    data[flagPtr].flags=data[flagPtr].flags & (~flagVal);
}

int VoxelBuffer::addNode(VectorI3* pos, bool &wasNew)
{
    int ind=pos->x+pos->y*vbPitch+pos->z*vbPitch*vbPitch;

    wasNew=!(getFlag(ind, E_OCT_NOTNEW));

    return ind;
}

uint VoxelBuffer::getFlagsAtNode(int ind)
{
    return data[ind].flags;
}
uint VoxelBuffer::getIndAtNode(int ind)
{
    return data[ind].vbeIndex;
}

void VoxelBuffer::clearAllNodes()
{
    int i;
    int j;

    int mySize=(int)voxelList.size();
    int curInd;

    for(i=0; i<mySize; i++)
    {
        curInd=voxelList[i].viIndex;
        data[curInd].flags=0;
        data[curInd].vbeIndex=-1;
    }

    for(j=0; j<vcSize; j++)
    {
        cellLists[j].curSize=0;
    }

    voxelList.clear();

    int mipSize=vbPitch/2;
    int mipVol;

    for(i=0; i<NUM_MIP_LEVELS; i++)
    {
        mipVol=mipSize*mipSize*mipSize;
        for(j=0; j<mipVol; j++)
        {
            mipMaps[i].mipArr[j]=false;
        }
        //mipMaps[i].mipList.clear();
        mipSize/=2;
    }

}


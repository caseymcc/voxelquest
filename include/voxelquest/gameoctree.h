#ifndef _voxelquest_gameoctree_h_
#define _voxelquest_gameoctree_h_

#include "voxelquest/types.h"
#include "voxelquest/ivec.h"

#include <vector>

class Singleton;

uint E_OCT_XP=1;
uint E_OCT_XM=2;
uint E_OCT_YP=4;
uint E_OCT_YM=8;
uint E_OCT_ZP=16;
uint E_OCT_ZM=32;
uint E_OCT_VISITED=64;
uint E_OCT_SOLID=128;
uint E_OCT_SURFACE=256;
uint E_OCT_NOTNEW=512;


struct OctNode
{
    int parent;
    int children[8];
    uint flags;

    int x;
    int y;
    int z;

    OctNode()
    {

    }

    void init(int _parent)
    {
        int i;

        parent=_parent;
        flags=0;

        for(i=0; i<8; i++)
        {
            children[i]=-1;
        }
    }

};

class GameOctree {
public:
    GameOctree();

    ~GameOctree();

    void init(
        Singleton* _singleton,
        int _dimInVoxels,
        int reserveSize
        // bool _hasTBO,
        // bool _hasVBO,
        // int _maxVerts
        //int _maxSize = -1,
        //int _nodeSize = -1
    );

    int allocNode(int parent);


    int getNode(VectorI3* pos);

    int addNode(VectorI3* pos, bool &wasNew);

    void clearAllNodes();

private:
	Singleton* singleton;
	
	int rootPtr;
	
	std::vector<OctNode> octNodes;
	
	int dimInVoxels;
	int maxDepth;
	
	//uint* vData;
	//uint* nData;
	
	//int numNeighbors;
	//int vDataSize;
	//int nDataSize;
	
	//int indexCount;
	
	
	//int nullPtr;
	//int rootPtr;
	//int nodeSize;
	//int nextOpen;
	//int renderLevel;
	//int maxVerts;
	//int vertComponents;
	
	//bool hasTBO;
	//bool hasVBO;
	
	
	//std::vector<uint> indexVec;
	//std::vector<float> vertexVec;
	//VBOWrapper vboWrapper;
	//TBOWrapper octTBO;
	
   
		
};








#endif//_voxelquest__h_
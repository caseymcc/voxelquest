#ifndef _voxelquest_path_h_
#define _voxelquest_path_h_

#include "voxelquest/types.h"

#include <LinearMath/btVector3.h>
#include <LinearMath/btMatrix3x3.h>



// enum E_PATH_INDEX_TYPE {
// 	E_PIT_TYPE,
// 	E_PIT_RES1,
// 	E_PIT_RES2,
// 	E_PIT_RES3,
// 	E_PIT_LENGTH
// };

enum E_PATH_VALUE_TYPE
{
    E_PVT_INVALID=1,
    E_PVT_LAND_BELOW=2,
    E_PVT_LENGTH=128
};


// enum E_CELL_DATA {
// 	E_CD_EMPTY,
// 	E_CD_WATER,
// 	E_CD_SOLID,
// 	//E_CD_PORTAL, // cost of moving through a door is very high, but less than a wall
// 	E_CD_LENGTH	
// };

// dont use enums, not cast to uint
const static uint E_CD_UNKNOWN=0;
const static uint E_CD_EMPTY=1;
const static uint E_CD_WATER=2;
const static uint E_CD_SOLID=4;

struct ConnectingNodeStruct
{

    float dist;

    int blockIdFrom;
    int holderIdFrom;

    int blockIdTo;
    int holderIdTo;

    int groupIdFrom;
    int groupIdTo;

    int cellIndFrom;
    int cellIndTo;

    int totCost;
};

bool sortByDist(const ConnectingNodeStruct &lhs, const ConnectingNodeStruct &rhs)
{
    return lhs.dist<rhs.dist;
}


struct PathInfo
{
    btVector3 points[2];
    bool searchedForPath;
    bool didFindPath;
    int nextInd;

    std::vector<btVector3> finalPoints;

};


struct GroupIdStruct
{
    int ind;
    int groupId;
    int cameFromInd;
    int pathCost;
};

struct GroupInfoStruct
{
    //bool touchesFace[6];
    int begInd;
    int endInd;
    int centerInd;

    int visitId;
};


struct PathResult
{

    ConnectingNodeStruct conNode;

    int blockId;
    int chunkId;
    int holderId;

    int lastBlockId;
    int lastChunkId;
    int lastHolderId;

    int groupId;



    int lastGroupId;

    int lastIndex;
};

enum E_PATH_FILL_OPS
{
    E_PFO_CLEAR_GROUPS,
    E_PFO_SEARCH_GROUPS,
    E_PFO_LENGTH
};

#endif//_voxelquest_path_h_
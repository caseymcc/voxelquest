#ifndef _voxelquest_gameblock_h_
#define _voxelquest_gameblock_h_

#include "voxelquest/gameplantnode.h"
#include "voxelquest/gameent.h"
#include "voxelquest/gamechunk.h"

enum E_BLOCK_GEN
{
    E_BG_ROADS_AND_BUILDINGS,
    //E_BG_STAIRS,
    //E_BG_REMOVE_REDUNDANT_Z_SEGMENTS,
    E_BG_ROOFS,
    //E_BG_BASEMENTS,
    E_BG_WING_TIPS,
    E_BG_DOORS,
    E_BG_FILL_TERRAIN,
    E_BG_LENGTH
};

enum E_CONNECTION_TYPE_CLASSES
{
    E_CTC_NOTHING,
    E_CTC_ROAD,
    E_CTC_ROOF,
    E_CTC_ROOM,
    E_CTC_STAIRS,
    E_CTC_LANTERN,
    E_CTC_LENGTH
};

enum E_CONNECTION_TYPES
{
    E_CT_NULL,
    E_CT_CONNECTED,
    E_CT_FOUNDATION,
    E_CT_ROAD,
    E_CT_DOCK,
    E_CT_MAINHALL,
    E_CT_WING,
    E_CT_ROOF,
    E_CT_WALKWAY_TOP,
    E_CT_ROOM_BRICK,
    E_CT_ROOM_TUDOR,
    E_CT_WALKWAY,
    E_CT_STAIRS,
    E_CT_LANTERN,
    E_CT_WINDOWFRAME,
    E_CT_WINDOW,
    E_CT_DOORWAY,
    E_CT_DOOR,
    E_CT_OBJECT,
    E_CT_LINES,
    E_CT_TREE,
    E_CT_LENGTH
};

enum E_DIR_SPECS
{
    E_DIR_X,
    E_DIR_Y,
    E_DIR_Z,
    E_DIR_LENGTH
};

enum E_DIR_SPECS_SIGN
{
    E_DIR_XP,
    E_DIR_XM,
    E_DIR_YP,
    E_DIR_YM,
    E_DIR_ZP,
    E_DIR_ZM,
    E_DIR_MP_LENGTH
};

enum E_MAT_SUBPARAM
{
    E_MAT_SUBPARAM_NONE,
    E_MAT_SUBPARAM_TUDOR,
    E_MAT_SUBPARAM_DOCK,
    E_MAT_SUBPARAM_BRICK,
    E_MAT_SUBPARAM_BRICK_ARCH,
    E_MAT_SUBPARAM_LENGTH
};


enum E_MAT_PARAM
{
    E_MAT_PARAM_FOUNDATION,
    E_MAT_PARAM_ROOF,
    //E_MAT_PARAM_WALKWAY_TOP,
    //E_MAT_PARAM_DOCK,
    E_MAT_PARAM_BUILDING,
    //E_MAT_PARAM_WALKWAY,
    E_MAT_PARAM_LANTERN,
    E_MAT_PARAM_DOORWAY,
    E_MAT_PARAM_DOOR,
    //E_MAT_PARAM_STAIRS,
    E_MAT_PARAM_TREE, // TREES MUST BE LAST ENTRY
    E_MAT_PARAM_LENGTH
};

enum E_TER_TYPE
{
    E_TER_GROUNDLEVEL,
    E_TER_UNDERGROUND,
    E_TER_AIR,
    E_TER_LENGTH
};

class Singleton;
class GameWorld;

struct MapNode
{
    int connectionProps[TOT_MAP_DIRS];
    int terHeight;
    int adjustedHeight;
    int houseHeight;
    int id;
};

const static unsigned int BC_FLAG_INSIDE=1;
const static unsigned int BC_FLAG_WING_BEG=2;
const static unsigned int BC_FLAG_WING_END=4;

struct BuildingCon
{
    int conType;
    unsigned int nodeFlags;
    //bool isWingBeg;
    //bool isWingEnd;
    float wingMult;
    float wallRadInCells;
    int heightDelta;
    int direction; 	// -1: negative,
                    //  0: neutral,
                    //  1: positve
};


struct BuildingNode
{

    BuildingCon con[TOT_NODE_VALS];

    // int con[TOT_NODE_VALS]; // connections
    // bool isWingBeg[TOT_NODE_VALS];
    // bool isWingEnd[TOT_NODE_VALS];

    //std::vector<ConnectionProp> connections;

    int mazeIndex;
    int id;
    int visited;
    float powerValU;
    float powerValV;
    bool nearTerrain;
    bool nearAir;

    // float terHeight;
    // bool isWingTip;

    // int centerType;
    // int connectionTypes[4];
    // int shortTypes[4];

    // int centerHeight;
    // int connectionHeights[4];
    // int shortHeights[4];


};

class GameBlock
{
public:

    GameBlock();

    void init(
        Singleton *_singleton,
        int _blockId,
        int _x,
        int _y,
        int _z,
        int _xw,
        int _yw,
        int _zw
    );





    int getNodeIndexClamped(int _x, int _y, int _z);

    int getNodeIndex(int x, int y, int z, int bufAmount);

    int touchesPlanarBN(int x, int y, int z, int buildingType, int bufAmount);
    int touchesPlanarClass(int x, int y, int z, int classType, int bufAmount);

    int touchesPlanarClass2(int x, int y, int z, int classType, int bufAmount);



    bool isLCorner(int x, int y, int z, int classType, bool includeSingle);


    void applyWingValues(
        int _x1, int _y1, int _z1,
        int _x2, int _y2, int _z2,
        int cnum,
        bool isWingBeg,
        bool isWingEnd,
        float multiplier
    );


    void addPlantNodes(GamePlantNode* curPlantNode, FIVector4* orig, float scale);

    void addNewGeom(
        int _curBT,
        int _curAlign,
        float _baseOffset,
        FIVector4* _p1,
        FIVector4* _p2,
        FIVector4* _rad,
        FIVector4* _cornerRad,
        FIVector4* _visInsetFromMin,
        FIVector4* _visInsetFromMax,
        FIVector4* _powerVals,
        FIVector4* _powerVals2,
        FIVector4* _thickVals,
        FIVector4* _matParams,
        FIVector4* _anchorPoint,
        int _minRot,
        int _maxRot

    );


    void connectNodes(
        int _x1,
        int _y1,
        int _z1,
        int _x2,
        int _y2,
        int _z2,

        int ct,
        int id=-1,
        int _heightDelta=0,
        int _direction=0,
        float _wallRadInCells=-1.0f,
        unsigned int _nodeFlags=0
    );


    int getMapNodeIndex(int x, int y, int bufAmount);




    int touchesMap(int x, int y, int buildingType, int bufAmount);


    int touchesWithinRadMap(int x, int y, int buildingType, int rad, int bufAmount);

    int touches2Map(int x, int y, int buildingType, int bufAmount);

    void connectMapNodes(int _x1, int _y1, int _x2, int _y2, int buildingType, int id, int bufAmount);


    int getAdjustedHeightInHolders(int xInHolders, int yInHolders);



    float fIsNearTerrain(
        FIVector4 *worldMinVisInPixels
    );




    void modifyTerrain(
        FIVector4 *worldPos,
        bool doSub
    );


    int isNearTerrain(FIVector4 *worldPosInPix);


    int findNearestNode(FIVector4* worldPositionInPixelsIn, FIVector4* posInNodesOut, FIVector4* posInPixelsOut);

    void nodeIndexToWorldSpaceInPixels(int ind, FIVector4* posInPixelsOut);




    bool hasTerrainBelow(int i, int j, int k, int conDir, bool bothEnds);

    bool hasRoomBelow(int i, int j, int k, int conDir);

    bool isSupportedBelow(int i, int j, int k, int conDir, bool bothEnds);


    bool touchesRoomOnLevel(int i, int j, int k);

    bool touchesBaseOnLevel(int i, int j, int k, int layer);



    bool buildingAbove(int x, int y, int z);

    void makeMazeUG();

//private:

    Singleton *singleton;

    int blockId;
    int holdersPerBlock;
    int terDataBufAmount;

    bool forceUpdate;

    float trilin[8];

    int terDataVisPitchXY;
    float fTerDataVisPitchXY;
    int terDataBufPitchXY;
    int terDataBufPitchScaledXY;

    int terDataVisPitchZ;
    float fTerDataVisPitchZ;
    int terDataBufPitchZ;
    int terDataBufPitchScaledZ;

    float plantScale;

    int terDataTexScale;
    int terDataVisSize;
    int terDataBufSize;
    int cellsPerBlock;

    int maxFloors;
    float fCellsPerBlock;

    int dirModX[6];
    int dirModY[6];
    int dirModZ[6];
    int opDir[6];


    float floorHeightInCells;

    float floorHeight;
    float roofHeight;

    bool isBuilding[E_CT_LENGTH];
    bool isRoad[E_CT_LENGTH];
    bool isRoom[E_CT_LENGTH];
    bool isBase[E_CT_LENGTH];
    bool isTraversable[E_CT_LENGTH];
    int ctClasses[E_CT_LENGTH];
    int layerHash[E_CT_LENGTH];

    FIVector4 anchorPoint;

    FIVector4 p1;
    FIVector4 p2;
    FIVector4 newP1;
    FIVector4 newP2;
    FIVector4 rad;
    FIVector4 visInsetFromMin;
    FIVector4 visInsetFromMax;
    FIVector4 cornerRad;
    FIVector4 thickVals;
    FIVector4 powerVals;
    FIVector4 powerVals2;
    FIVector4 matParams;
    FIVector4 centerPoint;
    FIVector4 lightVec;
    FIVector4 offsetInBlocks;
    FIVector4 offsetInBlocksWrapped;
    FIVector4 origin;
    FIVector4 blockMinBufInPixels;
    FIVector4 blockMaxBufInPixels;

    FIVector4 tempVec;
    FIVector4 tempVec2;
    FIVector4 tempVec3;
    FIVector4 tempVec4;
    FIVector4 tempVecB;
    FIVector4 tempVecB2;
    FIVector4 tempVecB3;

    GameEnt baseEnt;


    //std::vector<GameOrg *> GameOrgs; // only one instance per block

    EntVec gameEnts[E_ET_LENGTH];


    GameWorld *gw;

    int iChunkSize;

    GameChunk **chunkData;

    BuildingNode *buildingData;
    MapNode *mapData;
    uint *terData;

    BuildingCon *curCon;

    static bool bufferInit;
    static int *rbStack;
    static int *rbHeightStack;
};
#endif//_voxelquest__h_
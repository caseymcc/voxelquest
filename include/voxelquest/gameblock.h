#ifndef _voxelquest_gameblock_h_
#define _voxelquest_gameblock_h_

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

private:

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



    



};
#endif//_voxelquest__h_
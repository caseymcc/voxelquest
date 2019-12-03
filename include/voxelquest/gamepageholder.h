#ifndef _voxelquest_gamepageholder_h_
#define _voxelquest_gamepageholder_h_

#include "voxelquest/path.h"
#include "voxelquest/constants.h"
#include "voxelquest/object.h"
#include "voxelquest/volumewrapper.h"
#include "voxelquest/gamevoxelwrap.h"

#include <BulletDynamics/Dynamics/btRigidBody.h>

class Singleton;

class GamePageHolder
{
private:
    int* pathData;
    int* cellData;
    int* extrData;

public:
    void reset(bool destroyCache);

    GamePageHolder();

    void init(
        Singleton* _singleton,
        int _blockId, 			// MUST BE UNIQUE FOR ENTITES
        int _chunkId,
        int _holderId,

        int trueX,
        int trueY,
        int trueZ

        //, bool _isBlockHolder = false

    );

    void makeDirty();

    void gatherObjects();

    int getCellAtCoordsLocal(int xx, int yy, int zz);

    int getCellAtInd(
        int ind
    );

    void getArrAtInd(
        int ind,
        int* tempCellData,
        int* tempCellData2
    );

    void setArrAtInd(
        int ind,
        int* tempCellData=NULL,
        int* tempCellData2=NULL
    );




    void clearPathPreserve();

    void clearPathSizes();

    void checkData(bool checkPath);

    void clearGroupFlags(int targId);

    int floodFillAtInd(
        int firstInd,
        int newId,
        bool findCenter,
        GroupInfoStruct* curGI
    );

    void findIdealNodes();

    int getGroupId(int pathDataIndex);

    int getInfoPD(int pathDataIndex);

    GroupIdStruct* getInfo(int pathDataIndex);

    void getInfoReport(int pathDataIndex);

    btVector3 holderIndToBTV(GamePageHolder* curPointHolder, int curPointIndex);



    void sortConNodes(GamePageHolder* endHolder, int endInd);

    void linkRegions();

    bool prepPathRefresh(int rad);

    void refreshPaths();


    void genCellData();


    void bindPD(int pd);

    void unbindPD();

    void applyFill();

    void fillVBO();


    inline PaddedDataEntry* getPadData(int ii, int jj, int kk);

    int gatherData();


    bool checkCache();

    void generateList();

    void wrapPolys();


    std::vector<float> vertexVec;

    int begMip[NUM_MIP_LEVELS_WITH_FIRST];
    int endMip[NUM_MIP_LEVELS_WITH_FIRST];

    //uint* cubeData;
    //std::vector<CubeWrap> cubeWraps;

    bool listGenerated;
    bool readyToRender;
    bool listEmpty;
    bool hasData;
    bool hasPath;
    bool hasCache;
    bool wasStacked;
    bool isDirty;

    bool lockWrite;
    // /bool lockRead;

    //VBOWrapper vboWrapper;
    VolumeWrapper* terVW;

    GameVoxelWrap* voxelWrap;

    std::vector<ObjectStruct> tempObjects;
    std::vector<intPair> objectOrder;

    //GLuint holderDL;


    int curPD;

    int blockId;
    int chunkId;
    int holderId;

    //bool isBlockHolder;

    int pathSize;
    int totIdealNodes;
    int totGroupIds;


    //int cubeDataSize;
    int cellDataSize;
    int cellsPerHolder;

    int visitId;

    bool pathsInvalid;
    bool idealPathsInvalid;

    bool pathsReady;
    bool idealPathsReady;

    uint holderFlags;

    std::vector<int> indexStack; // temp buffer used for flood fill
    std::vector<GroupIdStruct> groupIdStack; // stores all ids within a group
    std::vector<GroupInfoStruct> groupInfoStack; // stores all info about one group
    std::vector<ConnectingNodeStruct> bestConnectingNodes; // best connections between groups


    std::vector<int> collideIndices;
    std::vector<btRigidBody*> collideBodies;

    FIVector4 offsetInHolders;

    FIVector4 gphMinInCells;
    FIVector4 gphMaxInCells;
    FIVector4 gphCenInCells;

    FIVector4 origOffset;




    Singleton* singleton;

    //intPairVec containsEntIds[E_ET_LENGTH];

    bool wasGenerated;



    //q3Body* body;

    // q3BodyDef bodyDef;
    // q3Transform tx;
    // q3BoxDef boxDef;



    // btTriangleIndexVertexArray* meshInterface;
    // btIndexedMesh part;
    // btRigidBody* body;
    // btBvhTriangleMeshShape* trimeshShape;
    // btBoxShape* boxShape;

    // int vertCount;
    // int indCount;
    // btScalar *vertices;
    // unsigned short *indices;



   



};








#endif//_voxelquest__h_
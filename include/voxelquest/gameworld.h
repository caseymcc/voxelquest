#ifndef _voxelquest_gameworld_h_
#define _voxelquest_gameworld_h_

#include "voxelquest/baseobject.h"
#include "voxelquest/helperfuncs.h"
#include "voxelquest/gameblock.h"
#include "voxelquest/volumewrapper.h"
#include "voxelquest/texture3d.h"

class GameOrg;
class GameOrgNode;
class Singleton;
class Image;


enum E_VOLUME_WRAPPERS
{
    //E_VW_TERGEN,
    E_VW_HOLDER,
    E_VW_WORLD,
    E_VW_VORO,
    E_VW_LENGTH
};

struct materialNode
{
    float h;
    float s;
    float l;
    float r;
    float g;
    float b;
    float power;
    float ratio;
};
void hsv2rgb(materialNode* matNode);

//const static int MAX_PRIM_DATA_IN_BYTES=8*1024*1024;
//const static int MAX_LIMB_DATA_IN_BYTES=65536;
const static int MAX_ZO_CUBES=64;

const static uint ALL_FACES=63;

#define E_PATTERN_TYPE(DDD) \
DDD(E_PAT_1X1SQUARE) \
DDD(E_PAT_3X3SQUARE) \
DDD(E_PAT_5X5SQUARE) \
DDD(E_PAT_1X1DIAMOND) \
DDD(E_PAT_3X3DIAMOND) \
DDD(E_PAT_5X5DIAMOND) \
DDD(E_PAT_LENGTH)

const std::string E_PATTERN_TYPE_STRINGS[]={
    E_PATTERN_TYPE(DO_DESCRIPTION)
};

enum E_PATTERN_TYPE_VALS
{
    E_PATTERN_TYPE(DO_ENUM)
};

enum PATTERN_SHAPES
{
    E_PATSHAPE_SQUARE,
    E_PATSHAPE_DIAMOND,
    E_PATSHAPE_LENGTH
};

const static int PATTERN_SIZE=5;
const static int PATTERN_CENTER=(PATTERN_SIZE/2);

struct PatternStruct
{
    int addPat;
    int addX;
    int addY;

    int subPat;
    int subX;
    int subY;

    float patternVals[PATTERN_SIZE*PATTERN_SIZE];
};

class GameWorld
{
public:
    GameWorld();
    ~GameWorld();

    void init(Singleton *_singleton);
    void initVolumeWrappers();

    GameBlock *getBlockAtCoords(int xInBlocks, int yInBlocks, int zInBlocks, bool createOnNull=false);

    void clearAllHolders();

    // x, y, and z are measured in chunks
    GameChunk* getChunkAtCoords(int x, int y, int z, bool createOnNull=false);

    // x, y, and z are measured in holders
    GamePageHolder *getHolderAtCoords(int x, int y, int z, bool createOnNull=false);

    // assumed that holder exists when calling this
    GameChunk *getChunkAtId(int blockId, int chunkId);

    // assumed that holder exists when calling this
    GamePageHolder *getHolderAtId(int blockId, int chunkId, int holderId);

    // assumed that block exists when calling this
    GameBlock *getBlockAtId(int id);

    int getCellInd(btVector3 cParam, GamePageHolder* &curHolder);


    int getCellInd(GamePageHolder* &curHolder, int xv, int yv, int zv);

    // x, y, z measured in cells
    int getCellAtCoords(int xv, int yv, int zv);

    float getCellAtCoordsLin(btVector3 pos);

    btVector3 getNormalAtCoord(btVector3 coord, float* cellVal);


    void setArrAtCoords(int xv, int yv, int zv, int* tempCellData, int* tempCellData2);

    // x, y, z measured in cells
    void getArrAtCoords(int xv, int yv, int zv, int* tempCellData, int* tempCellData2);

    void fireEvent(BaseObjType uid, int opCode, float fParam);

    void doMedian();

    void preUpdate();

    void update();

    void updateWorld();

    void toggleVis(GameEnt* se);

    void ensureBlocks();


    void drawVol(
        VolumeWrapper* curVW,
        FIVector4* minc,
        FIVector4* maxc,
        bool copyToTex,
        bool forceFinish,
        bool getVoro=false,
        bool getBlockHolders=false
    );

    void updateLimbTBOData(bool showLimbs);

    void drawPrim(bool doSphereMap, bool doTer, bool doPoly);

    void drawOrg(GameOrg* curOrg, bool drawAll);


    void drawNodeEnt(
        GameOrgNode* curNode,
        FIVector4* basePosition,
        float scale,
        int drawMode,
        bool drawAll
    );

    void polyCombine();

    void drawPolys(
        std::string fboName,
        int minPeel,
        int maxPeel
        //, bool isBlockHolder
    );

    void rastChunk(int rad, uint flags);

    void renderGeom();

    void updateMouseCoords(FIVector4 *fPixelWorldCoordsBase);


    float weighPath(float x1, float y1, float x2, float y2, float rad, bool doSet, bool isOcean);


    float findBestPath(float x1, float y1, float x2, float y2, int generation, int roadIndex, bool doSet, bool isOcean);

    void initMap();

    void generateNoise();

    void drawMap();

    void doBlur(std::string fboName, int _baseFBO=0);

    float getHeightAtPixelPos(float x, float y, bool dd=false);

    void drawBasicPrims(bool doShadow);

    void rasterHoldersCheck();

    void rasterHolders(bool doShadow);


    void renderDebug();
    void finalStep(bool postToScreen);
    void postProcess(bool postToScreen);

    float getSLNormalized()
    {
        return (((float)seaLevel)/255.0f);
    }

    float getSeaHeightScaled();

    float getTerHeightScaled(float val);

    void updatePrimArr();

    float getUnderWater();

    void updateMatVol();

    void applyPat(int patInd, int patShape, int rot, int x, int y, int val, int rad);
    void generatePatterns();

    void updatePrimTBOData();
        
//private:
    VolumeWrapper* volumeWrappers[E_VW_LENGTH];

    float timeOfDay;
    float targetTimeOfDay;

    bool skippedPrim;

    int medianCount;
    int actorCount;

    bool depthInvalidMove;
    bool lastDepthInvalidMove;
    float amountInvalidMove;
    bool depthInvalidRotate;

    int numProvinces;
    int seaLevel;
    int seaSlack;
    int pageCount;
    int mapSwapFlag;
    int holdersPerBlock;
    int shiftCounter;

    int holdersPerChunk;
    int chunksPerWorld;
    int chunksPerBlock;


    int renderCount;
    float invalidCount;
    float invalidCountMax;

    int iBlockSize;
    int renderMethod;
    int iBufferSize;
    int maxThreads;
    int availThreads;

    int lightCount;

    int MIN_MIP;
    int MAX_MIP;
    int AVG_MIP;

    int blockMip;

    int hmChannel;
    int idChannel;
    int densityChannel;
    int blockChannel;


    int stChannel;
    int btChannel;
    int pathChannel;
    int houseChannel;

    float mapWidth;
    float mapHeight;

    int *curDiagram;
    int *provinceGrid;
    int *provinceX;
    int *provinceY;

    bool doDrawFBO;
    bool mapLockOn;
    bool foundPath;

    float mapStep;
    //float mapTrans;

    // int tempCellData[4];
    // int tempCellData2[4];

    int dirFlags[4];
    int dirFlagsOp[4];
    int dirFlagsO[4];
    int dirFlagsOpO[4];
    int dirModX[6];
    int dirModY[6];
    int dirModZ[6];
    int opDir[4];

    GameEnt *activeLights[MAX_EVAL_LIGHTS];

    int dirFlagClear;
    int visFlag;
    int visFlagO;
    int activeFBO;

    bool noiseGenerated;

    std::vector<intTrip> gamePageHolderList;

    std::vector<coordAndIndex> roadCoords;

    std::vector<int> ocThreads;

    btVector3 offsetVal[4];

    std::string curTargFBO[3];
    std::string curDepthFBO[3];

    GamePageHolder* blockHolder;
    GamePageHolder* holderInFocus;


    FIVector4 lScreenCoords;
    FIVector4 cScreenCoords;
    int cellsPerWorld;
    int holdersPerWorld;
    int blocksPerWorld;

    FIVector4 entMin;
    FIVector4 entMax;

    FIVector4 camHolderPos;
    //FIVector4 cutHolderPos;
    FIVector4 camBlockPos;
    FIVector4 camChunkPos;
    FIVector4 iPixelWorldCoords;
    FIVector4 pagePos;
    FIVector4 unitPos;
    FIVector4 lastUnitPos;

    int curPrimTemplate;
    FIVector4 tempVec1;
    FIVector4 tempVec2;
    FIVector4 tempVec3;
    FIVector4 tempVec4;

    bool updateHolderLookat;
    bool updateLock;
    bool mapInvalid;

    bool lightChanged;

//    bool fpsTest;

    FIVector4 holderLookAt;
    FIVector4 blockPos[2];
    FIVector4 nodePos[2];
    FIVector4 nodePosInPixels[2];
    //PathNode blockAndIndexPath[2];
    //PathNode blockAndIndexSplitPath[2];
    FIVector4 lineSeg[2];
    //PathHolder* finalPath;

    int nodeInd[2];
    //int finalInd;

    GameBlock* blockRef[2];


    //FIVector4 minShadowBounds;
    //FIVector4 maxShadowBounds;
    //FIVector4 minShadowBoundsGrow;
    //FIVector4 maxShadowBoundsGrow;

    FIVector4 minv;
    FIVector4 maxv;

    FIVector4 tempVec;
    FIVector4 unitPosMin;
    FIVector4 unitPosMax;
    FIVector4 unitPosCenter;
    //FIVector4 unitPosMinIS;
    //FIVector4 unitPosMaxIS;

    FIVector4 startBounds;
    FIVector4 endBounds;

    //FIVector4 *fogPos;
    //FIVector4 *cutPos;
    // FIVector4 *lightPos;
    // FIVector4 *globLightPos;
    // FIVector4 lightPosBase;

    FIVector4 *curBoxPos;
    FIVector4 tv0;
    FIVector4 tv1;
    FIVector4 tv2;
    FIVector4 tv3;
    FIVector4 tv4;
    FIVector4 tv5;

    //glm::quat myQuat;
    //glm::vec3 paramVec;
    //glm::vec3 spinVec;

    Singleton *singleton;

    GameBlock **blockData;

    FBOWrapper *curFBO;
    FBOWrapper *curFBO2;

    BaseObjType lastHitObjUID;

    FIVector4 mapFreqs;
    FIVector4 mapAmps;

    int primArrPos;
    float *primArr;
    int limbArrPos;
    float *limbArr;
    float *splashArr;
    float *explodeArr;
    float *paramArrMap;
    float lightArr[MAX_LIGHTS*16];

    int curPattern;
    int curPatternRot;
    PatternStruct patterns[E_PAT_LENGTH*4];

//    float limbTBOData[MAX_LIMB_DATA_IN_BYTES];
//    float primTBOData[MAX_PRIM_DATA_IN_BYTES];

    int mapPitch;
    Image *imageVoro;
    Image *imageHM0;
    Image *imageHM1;
    Image *cloudImage;
    //Poco::ThreadPool threadpool;

    FIVector4 lightPosStatic;
    FIVector4 lightPosDynamic;
    Matrix4 lightSpaceMatrix;
    Matrix4 lightSpaceMatrixLow;

    VBOWrapper zoCubes;

//volume data
    bool matVolLock;
    Texture3D volumeTex;
    uint *matVol;
    FIVector4 matVolDim;
    int matVolSize;
    materialNode* matSlice0;
    materialNode* matSlice1;
    materialNode* matSlice2;

    FIVector4 lastMouseZO;

    bool drawBrush;
    float curBrushRad;
    int earthMod;
};


#endif//_voxelquest__h_
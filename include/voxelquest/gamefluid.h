#ifndef _voxelquest_gamefluid_h_
#define _voxelquest_gamefluid_h_

#include "voxelquest/vectors.h"
#include "voxelquest/tbos.h"
#include "voxelquest/geom.h"
#include "voxelquest/threadwrapper.h"

enum E_PRIM_LAYERS
{
	E_PL_TERRAIN,
	//E_PL_PRIMIDS,
	E_PL_LENGTH
};

struct PushModStruct
{
    int actionType;
    FIVector4 data[4];
};

struct FluidPlane
{
	std::vector<int> planeIds;
};

struct FluidStruct
{
	int minX;
	int minY;
	int minZ;

	int maxX;
	int maxY;
	int maxZ;

	int id;
	long long int collectedWater;

	bool didCollectWater;

	int fidBegInd;
	int fidEndInd;

	int iciBegInd;
	int iciEndInd;

};

class Singleton;

class GameFluid
{
public:
    GameFluid();

    int getFluidId(int groupNum, int ind);

    int getFIDSize(int groupNum);

    void fidPushBack(int groupNum, int val);

    int* fidGetBeg(int groupNum);
    int* fidGetEnd(int groupNum);

    int getIdealCellId(int groupNum, int ind);

    int getICISize(int groupNum);

    void iciPushBack(int groupNum, int val);

    void init(Singleton* _singleton, int _mainId);

    void flushActionStack();

    void pushPlaceTemplate(bool isReq, FIVector4* newPos, int pt, int orientation);

    bool addPrimObj(FIVector4* pos, int tempId, int uid);

    void flushAllGeom();

    void addGeom(FIVector4* newPos, int templateId, int orientation);

    void fetchGeom();

    void setupPrimTexture();

    bool anyThreadsRunning();

    void flushStacks();

    bool updateAll();

    void copyPrimTexture(int ox, int oy, int oz, int dim, uint** myData);


    void updateTBOData(bool firstTime, bool reloadTemplates);

    void terminateCycle();

    void beginFluidRead(FIVector4* _campPosVPDump);

    bool tryToEndRead();

    void tryToEndThreads();

    void proceedWithRead();

    void endFluidRead();

    void shiftRegion();

    void funcFT();
    void startFT();
    bool stopFT();

    void funcTL();
    void startTL();
    bool stopTL();

    void funcTT();
    void startTT();
    bool stopTT();

    void getPrimData(int n);

    void writeFluidData();

    void prereadFluidData();

    void readFluidData();

    bool passesCheck(int n);

    bool updateFluidData();

    bool findStableRegions(int startInd, int newId);

    bool floodFillId(int startInd, int newId);

    bool inBounds(int i, int j, int k);


    void roundBox(

        FIVector4* absVecFromCenter,
        FIVector4* innerBoxRad,
        FIVector4* cornerDisThicknessPower,

        bool &isInObj,
        bool &isInside

        //float vectorFromCenterX, float vectorFromCenterY, float vectorFromCenterZ,
        //float box_dimX, float box_dimY, float box_dimZ, float box_dimW, 
        //float box_powerX, float box_powerY,
        //float wallThickness
    );

    void clearAllGeom();
    void clearInsideValues();

    void fillCurrentGeom(int templateId, FIVector4* templatePos);

    void resetDirtyRegion();
    void shrinkDirtyRegion();

    void maxDirtyRegion();

//private:
    Singleton* singleton;

    uint* volDataPrim[E_PL_LENGTH];

    int forceFullRefresh;

    int volSizePrim;
    int volSizePrimBuf;
    int bufAmount;
    int vspMin;
    int vspMax;
    int* fluidData;
    int* extraData;

    int shrinkCount;
    int immobileHeight;
    int immobileInd;
    int maxWaterHeight;
    int maxWaterInd;

    std::vector<int> indexStack;

    //int curTick;
    //int maxTicks;

    int curGeomCount;

    int UNIT_MIN;
    int UNIT_MAX;
    int UNIT_INSIDE;

    int watchMinX;
    int watchMaxX;
    int watchMinY;
    int watchMaxY;
    int watchMinZ;
    int watchMaxZ;
    int totSize;

    FIVector4 readMIP;
    FIVector4 writeMIP;


    FIVector4 dirtyMin;
    FIVector4 dirtyMax;

    FIVector4 tempMin;
    FIVector4 tempMax;

    FIVector4 tempMin2;
    FIVector4 tempMax2;

    FIVector4 minV;
    FIVector4 maxV;

    bool waitingOnThreads;
    bool fluidChanged;
    bool posShifted;
    bool hasRead;
    bool invalidated;

    float F_UNIT_MAX;
    float F_UNIT_MIN;

    std::vector<FluidStruct> fsVec;


    int* fluidIds;
    int* idealCellIds;


    FluidPlane fluidPlane;



    ///////////////////

    bool waterTickReady;
    bool cycleTerminated;
    bool readyForTermination;
    bool fluidReading;
    bool proceedingToRead;
    //bool modifiedUnit;
    bool modifiedGeom;
    bool readyForTBOUpdate;
    bool firstVPUpdate;


    //int volSizePrim;
    int volSizePrimMacro;
    int floatsInPrimMacro;
    int floatsPerPrimEntry;
    int primsPerMacro;
    int primDiv;
    int waterTick;
    int waterTickMax;
    int waterTickMaxDiv;
    int cellsPerHolder;
    int cellsPerBlock;
    GLenum internalPrimFormat;
    GLenum precPrimFormat;
    int mainId;

    int volSizes[E_FID_LENGTH];
    int mipSizes[E_FID_LENGTH];

    float waterLerp;
    float* tboData;

    GLuint volIdPrim[E_PL_LENGTH];
    TBOWrapper tboWrapper;


    std::vector<PushModStruct> pmStack;
    //std::vector<ObjectStruct> modStack; //ModUnitStruct

    FIVector4 volMinInPixels;
    FIVector4 volMaxInPixels;
    FIVector4 volMinReadyInPixels;
    FIVector4 volMaxReadyInPixels;
    FIVector4 camPosVP;
    FIVector4 campPosVPDump;
    FIVector4 newCamPos;
    FIVector4 lastCamPos;
    FIVector4 camPosVPInPixels;
    FIVector4 lastCamPosVP;
    FIVector4 curDirtyMax;
    FIVector4 curDirtyMin;
    FIVector4 curWaterMax;
    FIVector4 curWaterMin;
    FIVector4 tempBoundsMin;
    FIVector4 tempBoundsMax;



    ThreadWrapper threadLoader;
    ThreadWrapper threadTex;
    ThreadWrapper threadFluid;

};


#endif//_voxelquest__h_
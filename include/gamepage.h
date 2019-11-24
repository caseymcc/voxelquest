

class GamePage //: public Poco::Runnable 
{
public:
    GamePage();
    ~GamePage();

    void init(
        Singleton *_singleton,
        GamePageHolder *_parentGPH,
        int _thisPageId,
        int offsetX,
        int offsetY,
        int offsetZ,
        int oxLoc,
        int oyLoc,
        int ozLoc,
        bool _isEntity=false
    );

    uint *getVolData();

    uint *getVolDataLinear();

    void copyToTexture(bool isForEmptyVD);

    void setFalse();


    void addEntityGeom(bool justTesting);

    void addGeom(bool justTesting);

    void getVoroPoints();

    void addAllGeom();

    bool generateVolume(bool dd=false);

    int getIndex(int i, int j, int k, int p);

    // bool isAir() {
    // 	// WAS DOING
    // }

    bool getPoints(int fboNum);


private:

    Singleton *singleton;

    int thisPageId;
    int terRes;

    int bufferedPageSizeInUnits;
    FIVector4 offsetInUnits;
    FIVector4 offsetInPages;
    FIVector4 offsetInPagesLocal;
    float pageDepth;
    //float seaHeightInPixels;
    int iVolumeSize;
    //int totCol[E_CD_LENGTH]

    int* cellData;

    bool hasSolids;
    bool hasTrans;
    bool hasGeom;
    bool hasLines;
    bool hasTerrain;
    bool hasWater;
    bool hasWindow;
    bool hasTree;
    bool isDirty;

    int voroCount;

    bool volDataModified;

    bool isRendering;
    bool isEntity;

    int paramsPerEntry;
    int numEntries;
    int totParams;
    int maxEntries;

    FloatVec vertices[MAX_MIP_LEV*MAX_LAYERS];

    bool threadRunning;

    float unitSizeInPixels;

    GameWorld *gw;


    FIVector4 worldMinVisInPixels;
    FIVector4 worldMaxVisInPixels;
    FIVector4 worldMinBufInPixels;
    FIVector4 worldMaxBufInPixels;

    FIVector4 centerPosition;



    FIVector4 worldUnitMin;
    FIVector4 worldUnitMax;


    FIVector4 voroSize;
    FIVector4 curPos;
    FIVector4 randNum;
    FIVector4 testNum;
    FIVector4 newPos;

    FIVector4 tempVec1;
    FIVector4 tempVec2;



    GamePageHolder *parentGPH;
    GameBlock *parentBlock;

private:

    uint *volData;
    uint *volDataLinear;

};

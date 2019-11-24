class GameChunk
{
public:
    GameChunk();

    void init(
        Singleton* _singleton,
        int _blockId,
        int _chunkId,

        int trueX,
        int trueY,
        int trueZ

    );

    void makeDirty();

    VBOWrapper* getCurVBO();

    void drawLoadingHolders();

    void checkHolders();

    void reset();

    void fillVBO();

private:

    Singleton *singleton;
    VBOWrapper vboWrapper[NUM_MIP_LEVELS_WITH_FIRST];

    int lastPointCount;
    int mipLev;
    int changeTick;

    bool readyToRender;
    bool listEmpty;
    bool changeFlag;
    bool isDirty;

    int iHolderSize;
    int holdersPerChunk;
    GamePageHolder **holderData;

    FIVector4 offsetInChunks;
    FIVector4 chunkCenInCells;

    int chunkId;
    int blockId;

    std::vector<ObjectStruct> localObjects;




};

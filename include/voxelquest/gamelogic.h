#ifndef _voxelquest_gamelogic_h_
#define _voxelquest_gamelogic_h_

class GameLogic
{
public:
    GameLogic();

    void setEntTargPath(int sourceUID, int destUID);

    void init(Singleton* _singleton);


    void applyTBBehavior();

    void applyBehavior();

    GamePageHolder* getHolderById(int blockId, int chunkId, int holderId);

    GamePageHolder* getHolderByPR(PathResult* pr);

    bool holdersEqual(GamePageHolder* h0, GamePageHolder* h1);

    void addHolderToStack(GamePageHolder* curHolder);

    void remHolderFromStack(int opCode);

    void fillAllPaths(
        GamePageHolder* begHolder,
        GamePageHolder* endHolder,
        int begInd,
        int endInd,
        int opCode
    );

    bool addGroupToStack(
        ConnectingNodeStruct* testConNode,
        GamePageHolder* curHolder,
        int groupId,
        GamePageHolder* lastHolder,
        int lastGroupId,
        int lastIndex
    );

    void remGroupFromStack(int opCode);

    void fillAllGroups(
        GamePageHolder* begHolder,
        GamePageHolder* endHolder,
        int begInd,
        int endInd,
        int opCode
    );






    bool findNaivePath(PathInfo* pathInfo);


    bool findBestPath(
        PathInfo* pathInfo,
        GamePageHolder* closestHolder,
        GamePageHolder* closestHolder2,
        int bestInd,
        int bestInd2
    );


    void drawFinalPath(PathInfo* pathInfo);

    void getPath(PathInfo* pathInfo);

    void update();


    void drawLineAtIndices(
        GamePageHolder* curPointHolder, int curPointIndex,
        GamePageHolder* curPointHolder2, int curPointIndex2
        //,int r, int g, int b
    );

    btVector3holderIndToBTV(GamePageHolder* curPointHolder, int curPointIndex, bool addHalfOff);

    void drawPointAtIndex(GamePageHolder* curPointHolder, int curPointIndex, int r, int g, int b, float rad);


    void getPointsForPath(GamePageHolder* curHolderFrom, int _curInd, PathInfo* pathInfo, bool reverseOrder);




    void drawRegions(
        int offX,
        int offY,
        int offZ

    );



    int getClosestPathRad(btVector3 cpBTV, GamePageHolder* &closestHolder);

    bool anyThreadsRunning();

    void freePD();


    void processCurHolder(GamePageHolder* curHolder, bool doPaths);

    void loadNearestHolders(bool doUpdate);

private:
    Singleton* singleton;

    std::vector<PathResult> pathSearchStack;
    std::vector<PathResult> pathFinalStack;
    //std::vector<PathResult> readyHolderList;

    std::list<LoadHolderStruct> holderStack;

    ThreadPoolWrapper* threadPoolPath;
    ThreadPoolWrapper* threadPoolList;

    FIVector4 minv;
    FIVector4 maxv;

    // bool didFindPath;
    // bool searchedForPath;

    PathInfo testPath;
    std::vector<btVector3> tempStack;


    int idCounter;
    //int pathCount;

    // 
    GamePageHolder* globEndHolder;
    int globEndGroupId;
    bool globFoundTarg;
    bool dirtyStack;

    bool allowThreadCreation;
};


#endif//_voxelquest__h_
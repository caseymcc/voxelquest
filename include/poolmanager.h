
class PoolManager
{
public:
    PoolManager();

    void init(
        Singleton* _singleton,
        bool _isEntity,
        bool _isCPU,
        int _sizeX,
        int _sizeY
    );

    float getMaxMem();
    float getTotMemUsed();

    void reorderIds();

    int findFurthestHolderId();

    int requestPoolId(int blockId, int holderId);

private:
    int poolItemsCreated;
    bool isCPU;

    list<int> holderPoolIds;
    vector<intPair> orderedIds;
    vector<PooledResource *> holderPoolItems;

    Singleton* singleton;
    GameWorld* gw;

    bool isEntity;

    int sizeX;
    int sizeY;

};




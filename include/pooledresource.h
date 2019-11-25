#ifndef _voxelquest_pooledresource_h_
#define _voxelquest_pooledresource_h_

class PooledResource {
public:
    PooledResource();

    FBOSet* getFBOS(int fboNum);

    void init(
        Singleton* _singleton,
        bool _isCPU,
        int _sizeX,
        int _sizeY
    );

    void bindHolderDL(int mipLev, int layer);
    void unbindHolderDL(int mipLev, int layer);

private:
	FBOSet* fboSet[MAX_LAYERS];
	uint* cpuSet[MAX_LAYERS];

	intPair usedById;
	bool isCPU;
	Singleton* singleton;
	GLuint holderDL[MAX_MIP_LEV*MAX_LAYERS];
	bool listGenerated;

    
};

#endif//_voxelquest__h_
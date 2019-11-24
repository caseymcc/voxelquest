
class GameOctree {
public:
    GameOctree();

    ~GameOctree();

    void init(
        Singleton* _singleton,
        int _dimInVoxels,
        int reserveSize
        // bool _hasTBO,
        // bool _hasVBO,
        // int _maxVerts
        //int _maxSize = -1,
        //int _nodeSize = -1
    );

    int allocNode(int parent);


    int getNode(VectorI3* pos);

    int addNode(VectorI3* pos, bool &wasNew);

    void clearAllNodes();

private:
	Singleton* singleton;
	
	int rootPtr;
	
	vector<OctNode> octNodes;
	
	int dimInVoxels;
	int maxDepth;
	
	//uint* vData;
	//uint* nData;
	
	//int numNeighbors;
	//int vDataSize;
	//int nDataSize;
	
	//int indexCount;
	
	
	//int nullPtr;
	//int rootPtr;
	//int nodeSize;
	//int nextOpen;
	//int renderLevel;
	//int maxVerts;
	//int vertComponents;
	
	//bool hasTBO;
	//bool hasVBO;
	
	
	//std::vector<uint> indexVec;
	//std::vector<float> vertexVec;
	//VBOWrapper vboWrapper;
	//TBOWrapper octTBO;
	
   
		
};








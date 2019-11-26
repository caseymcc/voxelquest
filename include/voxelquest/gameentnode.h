#ifndef _voxelquest_gameentnode_h_
#define _voxelquest_gameentnode_h_



/////////////////////////


//   IMPORTANT: SPECIFY ZERO VECTOR FOR BASE ROTATION
//   (TWO VECTORS TOTAL FOR ROTATION AND ORIENTATION)


/////////////////////////


class GameEntNode
{
public:
    GameEntNode* parent;
    std::vector<GameEntNode*> children;

    GameEntNode(
        GameEntNode* _parent,
        int _nodeName,
        float _boneLength,
        float _tanLengthInCells,
        float _bitLengthInCells,
        float _norLengthInCells,

        float _tanX, float _tanY, float _tanZ,
        float _bitX, float _bitY, float _bitZ,
        float _norX, float _norY, float _norZ


    );

    GameEntNode* addChild(
        int _nodeName,
        float _boneLength,
        float _tanLengthInCells,
        float _bitLengthInCells,
        float _norLengthInCells,

        float _tanX, float _tanY, float _tanZ,
        float _bitX, float _bitY, float _bitZ,
        float _norX, float _norY, float _norZ
    );

    GameEntNode* getNode(int _nodeName);

    //void rotate(FIVector4 axis, )

    void doTransform(
        Singleton* singleton
    );

private:

    float rotThe; // vertical rotation
    float rotPhi; // horizontal rotation
    float rotRho;

    float material;
    float boneLengthHalf;


    const static float multiplier;

    FIVector4* readTBN;
    FIVector4* writeTBN;

    FIVector4 tbnBase[3];
    FIVector4 tbnBaseTrans[3];
    FIVector4 tbnRotA[3];
    FIVector4 tbnRotB[3];
    FIVector4 tbnRotC[3];
    FIVector4 tbnTrans[3];


    FIVector4 tbnRadInCells0;
    FIVector4 tbnRadInCells1;

    FIVector4 tbnRadScale0;
    FIVector4 tbnRadScale1;
    float boneLengthScale;

    // transformed points
    FIVector4 orgTrans[3]; // start, middle, end



    int nodeName;




};

const float GameEntNode::multiplier=2.0f;
#endif//_voxelquest__h_
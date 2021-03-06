#ifndef _voxelquest_gameorgnode_h_
#define _voxelquest_gameorgnode_h_

#include "voxelquest/vectors.h"

#include <LinearMath/btVector3.h>

// do not reorder
enum E_ORG_VECS
{
    E_OV_TANGENT,
    E_OV_BITANGENT,
    E_OV_NORMAL,
    E_OV_TBNRAD0,
    E_OV_TBNRAD1,
    E_OV_THETAPHIRHO,
    E_OV_TPRORIG,
    E_OV_MATPARAMS,
    E_OV_TBNOFFSET,
    E_OV_POWVALS, // x: lerpValP0toP1, y: lerpValBit, z: lerpValTan
    E_OV_LENGTH
};

/////////////////////////
//   IMPORTANT: SPECIFY ZERO VECTOR FOR BASE ROTATION
//   (TWO VECTORS TOTAL FOR ROTATION AND ORIENTATION)
/////////////////////////
class Singleton;

class GameOrgNode
{
public:
    GameOrgNode(
        GameOrgNode* _parent,
        int _nodeName,
        //float _boneLength,

        float _material,

        float _rotThe,
        float _rotPhi,
        float _rotRho,

        float _tanLengthInCells0,
        float _bitLengthInCells0,
        float _norLengthInCells0,

        float _tanLengthInCells1,
        float _bitLengthInCells1,
        float _norLengthInCells1,

        float _tanX, float _tanY, float _tanZ,
        float _bitX, float _bitY, float _bitZ,
        float _norX, float _norY, float _norZ




    );

    GameOrgNode* addChild(
        int _nodeName,
        //float _boneLength,

        float _material,

        float _rotThe,
        float _rotPhi,
        float _rotRho,

        float _tanLengthInCells0,
        float _bitLengthInCells0,
        float _norLengthInCells0,

        float _tanLengthInCells1,
        float _bitLengthInCells1,
        float _norLengthInCells1,

        float _tanX, float _tanY, float _tanZ,
        float _bitX, float _bitY, float _bitZ,
        float _norX, float _norY, float _norZ
    );

    GameOrgNode* getNode(int _nodeName);

    //void rotate(FIVector4 axis, )

    void flipOrient(float newVal);

    void doTransform(
        Singleton* singleton,
        GameOrgNode* tempParent
    );


    int nodeName;
    GameOrgNode* parent;
    std::vector<GameOrgNode*> children;




    // FIVector4 tbnBase[3];
    // FIVector4 tbnRadInCells0;
    // FIVector4 tbnRadInCells1;

    // float rotThe;
    // float rotPhi;
    // float rotRho;
    // float material;

    FIVector4 orgVecs[E_OV_LENGTH];


    FIVector4 tempFI;
    btVector3 tbnOffset;
    btVector3 tempOffset;

    // computed results

    FIVector4* readTBN;
    FIVector4* writeTBN;
    FIVector4 tbnBaseTrans[3];
    FIVector4 tbnRotA[3];
    FIVector4 tbnRotB[3];
    FIVector4 tbnRotC[3];
    FIVector4 tbnTrans[3];
    FIVector4 orgTrans[3]; // start, middle, end





};

#endif//_voxelquest__h_
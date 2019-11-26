#ifndef _voxelquest_gameplantnode_h_
#define _voxelquest_gameplantnode_h_

#include "voxelquest/vectors.h"

// class GamePlantNode {

// };

class GamePlantNode
{

public:
    GamePlantNode();

    void updateTangent(float angleInRadians);

    void init(
        GamePlantNode* _parent,
        int _maxChildren,
        int _numChildren
    );

//private:
    FIVector4 begPoint;
    FIVector4 endPoint;
    //FIVector4 ctrPoint;


    FIVector4 tangent;
    FIVector4 baseShoot;

    //float rotation;
    float shootLength;
    float begThickness;
    float endThickness;
    float midThickness;

    float sphereRad;

    FIVector4 startEndWidth;
    FIVector4 upVec;

    GamePlantNode* parent;
    GamePlantNode* children;
    int maxChildren;
    int numChildren;


};

#endif//_voxelquest__h_
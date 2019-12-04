#include "voxelquest/gameplantnode.h"

GamePlantNode::GamePlantNode()
{
    sphereRad=0.0f;
    parent=NULL;
    children=NULL;
    upVec.setFXYZ(0.0f, 0.0f, 1.0f);
}

void GamePlantNode::updateTangent(float angleInRadians)
{
    tangent.setFXYZRef(&endPoint);
    tangent.addXYZRef(&begPoint, -1.0f);
    tangent.normalize();
    baseShoot.setFXYZRef(&tangent);
    baseShoot.rotate(angleInRadians, E_PLANE_XZ);
    baseShoot.normalize();

    shootLength=begPoint.distance(&endPoint);

}

void GamePlantNode::init(
    GamePlantNode* _parent,
    int _maxChildren,
    int _numChildren
)
{
    parent=_parent;

    maxChildren=_maxChildren;
    numChildren=_numChildren;

    sphereRad=0.0f;

    if(maxChildren>0)
    {

        if(children==NULL)
        {
            children=new GamePlantNode[maxChildren];
        }
        else
        {

        }


    }

}

#include "voxelquest/geom.h"

const static int FLOATS_PER_TEMPLATE=((int)E_PRIMTEMP_LENGTH)*4;
const static float defaultTemplate[FLOATS_PER_TEMPLATE]={
    -2.0,-2.0,-2.0, 0.0,
     2.0, 2.0, 2.0, 0.0,
    -2.0,-2.0,-2.0, 0.0,
     2.0, 2.0, 2.0, 0.0,
     1.0, 1.0, 1.0, 1.0,
     0.0, 0.0, 0.0, 0.0
};

float paramArrGeom[128];

float &getGeoParam(int param)
{
    return paramArrGeom[param];
}

float getMinGeom(int baseIndex)
{
    int newIndex=baseIndex*4;

    return min(
        min(
        paramArrGeom[newIndex+0],
        paramArrGeom[newIndex+1]
    ),
        paramArrGeom[newIndex+2]
    );

}

FIVector4* getGeomRef(int templateId, int enumVal)
{
    return &(primTemplateStack[templateId*E_PRIMTEMP_LENGTH+enumVal]);
}

void setFXYZWGeom(int baseIndex, FIVector4* baseVec)
{
    int newIndex=baseIndex*4;
    int i;

    for(i=0; i<4; i++)
    {
        paramArrGeom[newIndex+i]=baseVec->getIndex(i);
    }

}

void setFXYGeom(int baseIndex, float xv, float yv)
{
    int newIndex=baseIndex*4;
    paramArrGeom[newIndex+0]=xv;
    paramArrGeom[newIndex+1]=yv;
}
void setFXGeom(int baseIndex, float xv)
{
    int newIndex=baseIndex*4;
    paramArrGeom[newIndex+0]=xv;
}
void setFYGeom(int baseIndex, float yv)
{
    int newIndex=baseIndex*4;
    paramArrGeom[newIndex+1]=yv;
}
void setFZGeom(int baseIndex, float zv)
{
    int newIndex=baseIndex*4;
    paramArrGeom[newIndex+2]=zv;
}
void setFWGeom(int baseIndex, float wv)
{
    int newIndex=baseIndex*4;
    paramArrGeom[newIndex+3]=wv;
}


float getFXGeom(int baseIndex)
{
    int newIndex=baseIndex*4;
    return paramArrGeom[newIndex+0];
}
float getFYGeom(int baseIndex)
{
    int newIndex=baseIndex*4;
    return paramArrGeom[newIndex+1];
}
float getFZGeom(int baseIndex)
{
    int newIndex=baseIndex*4;
    return paramArrGeom[newIndex+2];
}
float getFWGeom(int baseIndex)
{
    int newIndex=baseIndex*4;
    return paramArrGeom[newIndex+3];
}

void resetGeom()
{
    int i;
    geomStep=0;
    geomOrigOffset.setFXYZ(0.0f);
    geomPoints[0].setFXYZ(0.0f, 0.0f, 0.0f);

    for(i=0; i<FLOATS_PER_TEMPLATE; i++)
    {
        paramArrGeom[i]=defaultTemplate[i];
    }
}


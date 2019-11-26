#ifndef _voxelquest_draw_h_
#define _voxelquest_draw_h_

#include "voxelquest/vectors.h"
#include "voxelquest/fbos.h"

void idrawCrossHairs(FIVector4 originVec, float radius);


void drawLine(FIVector4 *p0, FIVector4 *p1);



void drawCubeCentered(FIVector4 *originVec, float radius);


void drawBoxUp(FIVector4 originVec, float radiusX, float radiusY, float diamZ);


void drawBoxMinMax(
    btVector3 v0,
    btVector3 v1
);


void drawBoxRad(
    btVector3 v0,
    btVector3 v1
);


void drawBox(
    FIVector4 *v0,
    FIVector4 *v1,
    int faceFlag=2
);


void drawQuadWithCoords(
    FIVector4* p0,
    FIVector4* p1,
    FIVector4* p2,
    FIVector4* p3,

    float tx1,
    float ty1,
    float tx2,
    float ty2

);


void drawQuadBounds(
    float fx1,
    float fy1,
    float fx2,
    float fy2,

    float fz
);



void drawFSQuad();



void drawFSQuadOffset(
    float xOff,
    float yOff,
    float zm
);


void drawFBO(std::string fboName, int ind, float zm, int swapFlag=-1);


void drawFBOOffsetDirect(FBOSet *fbos, int ind, float xOff, float yOff, float zm);


void drawFBOOffset(std::string fboName, int ind, float xOff, float yOff, float zm);

#endif//_voxelquest_draw_h_
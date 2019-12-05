#ifndef _voxelquest_object_h_
#define _voxelquest_object_h_

#include "voxelquest/vectors.h"
#include "voxelquest/mat.h"

struct ObjectStruct
{
    int64_t globalId;
    // int objType;
    // int addType;
    int templateId;
    int orientation;
    vec3 offset;
    //vec4 data[E_PRIMTEMP_LENGTH];

};

inline void getBoundsCenRad(
    ObjectStruct* curObj,
    FIVector4* baseGeom,
    vec3 &visCen,
    vec3 &visRad
);

void getVisCenRad(
    ObjectStruct* curObj,
    FIVector4* baseGeom,
    vec3 &visCen,
    vec3 &visRad
);

vec3 getUVW(
    vec3 _pos,
    ObjectStruct* curObj,
    FIVector4* baseGeom,

    vec3 uvwScale,
    float angModBase,
    bool mirrored
);

float primDis(
    vec3 pos,
    ObjectStruct* curObj,
    FIVector4* baseGeom
);


#endif//_voxelquest_object_h_
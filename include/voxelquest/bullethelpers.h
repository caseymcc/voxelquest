#ifndef _voxelquest_bullethelpers_h_
#define _voxelquest_bullethelpers_h_

#include "voxelquest/vectors.h"

#include <LinearMath/btVector3.h>
#include <LinearMath/btMatrix3x3.h>
#include <LinearMath/btQuaternion.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

btVector3 convertToBTV(FIVector4 &vec4); //should be const but FIVector4 functions are not const
FIVector4 convertToVQV(const btVector3 &myBTV);

void safeNorm(btVector3 &normRef);

btVector3 roundBTV(btVector3 v);
btVector3 floorBTV(btVector3 v);
btVector3 multByOtherRot(btVector3 imp, btMatrix3x3 otherRot);
btVector3 rotBTV2D(btVector3 source, float ang);
btVector3 rotate(const btQuaternion& quat, const btVector3 & vec);

btQuaternion matToQuat(
    float r11, float r12, float r13,
    float r21, float r22, float r23,
    float r31, float r32, float r33
);

btVector3 quatToEulerXYZ(const btQuaternion &quat);

void moveToOrientation(
    btRigidBody* myBody,
    btVector3 tanAxis,
    btVector3 bitAxis,
    btVector3 norAxis,
    float kv=0.5f
);

void traceBTV(std::string traceVal0, btVector3 myBTV);

#endif//_voxelquest__h_
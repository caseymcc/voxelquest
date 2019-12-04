#include "voxelquest/bullethelpers.h"
#include "voxelquest/helperfuncs.h"

#include <iostream>

btVector3 convertToBTV(FIVector4 &vec4)
{
    return btVector3(vec4.getFX(), vec4.getFY(), vec4.getFZ());
}

FIVector4 convertToVQV(const btVector3 &myBTV)
{
    FIVector4 vec4;
    
    vec4.setFXYZ(myBTV.getX(), myBTV.getY(), myBTV.getZ());
    return vec4;
}


void safeNorm(btVector3 &normRef)
{
    if(normRef.fuzzyZero())
    {

    }
    else
    {
        normRef.normalize();
    }
}

btVector3 roundBTV(btVector3 v)
{
    return btVector3(
        roundVal(v.getX()),
        roundVal(v.getY()),
        roundVal(v.getZ())
    );
}
btVector3 floorBTV(btVector3 v)
{
    return btVector3(
        floor(v.getX()),
        floor(v.getY()),
        floor(v.getZ())
    );
}

btVector3 multByOtherRot(btVector3 imp, btMatrix3x3 otherRot)
{
    // Vector3 myRHS = Vector3(imp.getX(),imp.getY(),imp.getZ());
    // Vector3 res = otherRot*myRHS;

    // return btVector3(res.x,res.y,res.z);

    return otherRot*imp;
}

btVector3 rotBTV2D(btVector3 source, float ang)
{
    float baseAng=atan2(source.getY(), source.getX());
    baseAng+=ang+((float)M_PI)/2.0f;



    return -btVector3(cos(baseAng), sin(baseAng), 0.0f);
}

btVector3 rotate(const btQuaternion& quat, const btVector3 & vec)
{
    float tmpX, tmpY, tmpZ, tmpW;
    tmpX=(((quat.getW() * vec.getX())+(quat.getY() * vec.getZ()))-(quat.getZ() * vec.getY()));
    tmpY=(((quat.getW() * vec.getY())+(quat.getZ() * vec.getX()))-(quat.getX() * vec.getZ()));
    tmpZ=(((quat.getW() * vec.getZ())+(quat.getX() * vec.getY()))-(quat.getY() * vec.getX()));
    tmpW=(((quat.getX() * vec.getX())+(quat.getY() * vec.getY()))+(quat.getZ() * vec.getZ()));
    return btVector3(
        ((((tmpW * quat.getX())+(tmpX * quat.getW()))-(tmpY * quat.getZ()))+(tmpZ * quat.getY())),
        ((((tmpW * quat.getY())+(tmpY * quat.getW()))-(tmpZ * quat.getX()))+(tmpX * quat.getZ())),
        ((((tmpW * quat.getZ())+(tmpZ * quat.getW()))-(tmpX * quat.getY()))+(tmpY * quat.getX()))
    );
}

btQuaternion matToQuat(
    float r11, float r12, float r13,
    float r21, float r22, float r23,
    float r31, float r32, float r33
)
{
    float q0=(r11+r22+r33+1.0f)/4.0f;
    float q1=(r11-r22-r33+1.0f)/4.0f;
    float q2=(-r11+r22-r33+1.0f)/4.0f;
    float q3=(-r11-r22+r33+1.0f)/4.0f;
    if(q0<0.0f) q0=0.0f;
    if(q1<0.0f) q1=0.0f;
    if(q2<0.0f) q2=0.0f;
    if(q3<0.0f) q3=0.0f;
    q0=sqrt(q0);
    q1=sqrt(q1);
    q2=sqrt(q2);
    q3=sqrt(q3);
    if(q0>=q1&&q0>=q2&&q0>=q3)
    {
        q0*=+1.0f;
        q1*=qSign(r32-r23);
        q2*=qSign(r13-r31);
        q3*=qSign(r21-r12);
    }
    else if(q1>=q0&&q1>=q2&&q1>=q3)
    {
        q0*=qSign(r32-r23);
        q1*=+1.0f;
        q2*=qSign(r21+r12);
        q3*=qSign(r13+r31);
    }
    else if(q2>=q0&&q2>=q1&&q2>=q3)
    {
        q0*=qSign(r13-r31);
        q1*=qSign(r21+r12);
        q2*=+1.0f;
        q3*=qSign(r32+r23);
    }
    else if(q3>=q0&&q3>=q1&&q3>=q2)
    {
        q0*=qSign(r21-r12);
        q1*=qSign(r31+r13);
        q2*=qSign(r32+r23);
        q3*=+1.0f;
    }
    else
    {
        std::cout<<"Quaternion error\n";
    }
    float r=qNorm(q0, q1, q2, q3);
    q0/=r;
    q1/=r;
    q2/=r;
    q3/=r;

    return btQuaternion(q0, q1, q2, q3);
}

btVector3 quatToEulerXYZ(const btQuaternion &quat)
{
    btVector3 euler;
    float w=quat.getW();   float x=quat.getX();   float y=quat.getY();   float z=quat.getZ();
    double sqw=w*w; double sqx=x*x; double sqy=y*y; double sqz=z*z;
    euler.setZ((atan2(2.0f * (x*y+z*w), (float)(sqx-sqy-sqz+sqw))));
    euler.setX((atan2(2.0f * (y*z+x*w), (float)(-sqx-sqy+sqz+sqw))));
    euler.setY((asin(-2.0f * (x*z-y*w))));

    return euler;
}


void moveToOrientation(
    btRigidBody* myBody,
    btVector3 tanAxis,
    btVector3 bitAxis,
    btVector3 norAxis,
    float kv
)
{
    btQuaternion targetOrientation=matToQuat(
        tanAxis.getX(), bitAxis.getX(), norAxis.getX(),
        tanAxis.getX(), bitAxis.getX(), norAxis.getX(),
        tanAxis.getX(), bitAxis.getX(), norAxis.getX()
    );
    btQuaternion currentOrientation=myBody->getOrientation();
    btQuaternion deltaOrientation=targetOrientation*currentOrientation.inverse();
    btVector3 deltaEuler=quatToEulerXYZ(deltaOrientation);
    btVector3 torqueToApply=(-1.0f*kv)*deltaEuler;
    myBody->applyTorque(torqueToApply);

}

void traceBTV(std::string traceVal0, btVector3 myBTV)
{
    doTraceND(traceVal0, " ", f__s(myBTV.getX()), " ", f__s(myBTV.getY()), " ", f__s(myBTV.getZ()));

}
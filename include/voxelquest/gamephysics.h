#ifndef _voxelquest_gamephysics_h_
#define _voxelquest_gamephysics_h_

#include "voxelquest/benchmarkdemo.h"
#include "voxelquest/myoglapp.h"
#include "voxelquest/vectors.h"
#include "voxelquest/baseobject.h"

#include <LinearMath/btVector3.h>

enum EVENT_OPS
{
    EV_COLLISION,
    EV_LENGTH
};

struct DebrisStruct
{
    btVector3 pos;
};

struct ExplodeStruct
{
    btVector3 pos;
    float radius;
    float power;
};

struct SphereStruct
{
    FIVector4 position;
    float maxRad;
    float curRad;
    float radVel;
    float radAcc;
};

class Singleton;

class GamePhysics
{
public:
    GamePhysics();
    ~GamePhysics();

    void init(Singleton* _singleton);

    void pickBody(FIVector4* mmPD);

    void collectDebris();

    void beginDrop();

    void remBoxFromObj(BaseObjType _uid);

    void addBoxFromObj(BaseObjType _uid, bool refreshLimbs);

    void flushImpulses();

    void explodeBullet(BaseObj* ge);

    void procCol(
        BaseObj** geArr,
        BodyStruct** curBodyArr
    );

    void collideWithWorld(double curStepTime);

    void remFarAway();

    void updateAll();

    //private:
    Singleton* singleton;

    BenchmarkDemo* example;
    MyOGLApp* myOGLApp;
    GUIHelperInterface* guiHelper;

    float myMat[16];
    float BASE_ENT_HEIGHT;
    float BASE_ENT_RAD;
    float CONTACT_THRESH;
    //double totTime;

    Matrix4 myMatrix4;
    Vector4 myVector4;
    Vector4 resVector4;

    btRigidBody* lastBodyPick;
    int lastBodyUID;


    btVector3 orig;
    btVector3 xyMask;
    btVector3 zMask;

    std::vector<ExplodeStruct> explodeStack;
    std::vector<DebrisStruct> debrisStack;
    std::vector<SphereStruct> sphereStack;
};


#endif//_voxelquest__h_
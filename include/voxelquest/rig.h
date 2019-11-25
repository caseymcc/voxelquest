#ifndef _voxelquest_rig_h_
#define _voxelquest_rig_h_

#include <LinearMath/btVector3.h>
#include <LinearMath/btMatrix3x3.h>
#include <LinearMath/btQuaternion.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>

#include <vector>
#include <string>

enum E_COL_TYPES
{
    COL_NOTHING=0,
    COL_STATIC=1,
    COL_DYN=2,
    COL_MARKER=4,
    COL_BODY=8
};

const static int terCollidesWith=COL_NOTHING;//COL_DYN|COL_BODY; //COL_MARKER|
const static int markerCollidesWith=COL_NOTHING;//COL_STATIC;
const static int dynCollidesWith=COL_DYN|COL_BODY; //COL_STATIC|
const static int bodyCollidesWith=COL_DYN|COL_BODY; //COL_STATIC|


enum E_JOINT_TYPES
{
    E_JT_LIMB,
    E_JT_BALL,
    E_JT_NORM,
    E_JT_CONT,
    E_JT_OBJ,
    E_JT_LENGTH
};

// enum E_LIMB_CLASSES {
// 	E_LIMB_CLASS_FIST_R,
// 	E_LIMB_CLASS_
// };

struct BodyStruct
{
    btRigidBody* body;
    btVector3 lastVel;
    btVector3 totAV;
    btVector3 totLV;

    float mass;
    float rad;
    float length;

    int actorJointId;
    int boneId;
    int jointType;
    //int classType;

    bool isVisible;
    bool inWater;
    bool isFalling;
    bool hasContact;
    bool isInside;



};

struct RigJointStruct
{
    int boneId;
    int jointType;
    int jointId;
    int parentId;

    float rad;
    float length;

    bool isFixed;

    btVector3 begOrig;
    btVector3 midOrig;
    btVector3 endOrig;
    btVector3 targAlignT;
    btVector3 targAlignB;
    btVector3 targAlignN;
    btQuaternion quat;
    btMatrix3x3 basis;
    //btVector3 pivotAxis;


    std::vector<int> children;
    btCollisionShape* shape;
    btRigidBody* body;
    btTypedConstraint* joint;
};

struct PoseKey
{
    int index;
    int group;
    int RLBN;
    int step;
};

#define E_POSE_GROUPS(DDD) \
DDD(E_PG_TPOSE) \
DDD(E_PG_NONPOSE) \
DDD(E_PG_JUMP) \
DDD(E_PG_DEAD) \
DDD(E_PG_FLAIL) \
DDD(E_PG_PICKUP) \
DDD(E_PG_IDLE) \
DDD(E_PG_WALKFORWARD) \
DDD(E_PG_SLSH) \
DDD(E_PG_BACK) \
DDD(E_PG_HACK) \
DDD(E_PG_STAB) \
DDD(E_PG_HOOK) \
DDD(E_PG_ELBO) \
DDD(E_PG_UPPR) \
DDD(E_PG_JABP) \
DDD(E_PG_ROUN) \
DDD(E_PG_REVR) \
DDD(E_PG_BKIK) \
DDD(E_PG_FRNT) \
DDD(E_PG_WPTPOSE) \
DDD(E_PG_WPSWORD) \
DDD(E_PG_WPAXE) \
DDD(E_PG_WPMACE) \
DDD(E_PG_WPHAMMER) \
DDD(E_PG_WPSTAFF) \
DDD(E_PG_WPSPEAR) \
DDD(E_PG_LENGTH)

std::string E_POSE_GROUP_STRINGS[]={
    E_POSE_GROUPS(DO_DESCRIPTION)
};

enum E_POSE_GROUP_VALS
{
    E_POSE_GROUPS(DO_ENUM)
};


enum E_BODY_GUIDES
{
    E_BDG_CENTER,
    //E_BDG_LFOOT,
    //E_BDG_RFOOT,
    E_BDG_LENGTH
};

// const static int E_BDG_LFOOT = -99;
// const static int E_BDG_RFOOT = -98;


enum E_BONES_HUMAN
{

    E_BONE_L_BEG, ///////////////
    E_BONE_L_SHOULDER,
    E_BONE_L_UPPERARM,
    E_BONE_L_LOWERARM,
    E_BONE_L_METACARPALS,
    // E_BONE_L_HOLDBASE,
    // E_BONE_L_HOLDEND,
    E_BONE_L_PROXIMAL0, // pinky
    E_BONE_L_PROXIMAL1, // ring
    E_BONE_L_PROXIMAL2, // middle
    E_BONE_L_PROXIMAL3, // index
    E_BONE_L_PROXIMAL4, // thumb
    E_BONE_L_INTER0, // pinky
    E_BONE_L_INTER1, // ring
    E_BONE_L_INTER2, // middle
    E_BONE_L_INTER3, // index
    E_BONE_L_INTER4, // thumb
    E_BONE_L_DISTAL0, // pinky
    E_BONE_L_DISTAL1, // ring
    E_BONE_L_DISTAL2, // middle
    E_BONE_L_DISTAL3, // index
    E_BONE_L_DISTAL4, // thumb
    E_BONE_L_HIP,
    E_BONE_L_UPPERLEG,
    E_BONE_L_LOWERLEG,
    E_BONE_L_TALUS,
    E_BONE_L_PHALANGE,
    E_BONE_L_END, //////////////

    E_BONE_R_BEG, //////////////
    E_BONE_R_SHOULDER,
    E_BONE_R_UPPERARM,
    E_BONE_R_LOWERARM,
    E_BONE_R_METACARPALS,
    // E_BONE_R_HOLDBASE,
    // E_BONE_R_HOLDEND,
    E_BONE_R_PROXIMAL0, // pinky
    E_BONE_R_PROXIMAL1, // ring
    E_BONE_R_PROXIMAL2, // middle
    E_BONE_R_PROXIMAL3, // index
    E_BONE_R_PROXIMAL4, // thumb
    E_BONE_R_INTER0, // pinky
    E_BONE_R_INTER1, // ring
    E_BONE_R_INTER2, // middle
    E_BONE_R_INTER3, // index
    E_BONE_R_INTER4, // thumb
    E_BONE_R_DISTAL0, // pinky
    E_BONE_R_DISTAL1, // ring
    E_BONE_R_DISTAL2, // middle
    E_BONE_R_DISTAL3, // index
    E_BONE_R_DISTAL4, // thumb
    E_BONE_R_HIP,
    E_BONE_R_UPPERLEG,
    E_BONE_R_LOWERLEG,
    E_BONE_R_TALUS,
    E_BONE_R_PHALANGE,
    E_BONE_R_END, /////////////////
    E_BONE_C_BEG, /////////////////
    E_BONE_C_BASE,
    E_BONE_C_SPINE0, // tail
    E_BONE_C_SPINE1,
    E_BONE_C_SPINE2,
    E_BONE_C_SPINE3,
    E_BONE_C_SPINE4, // neck
    E_BONE_C_SKULL,

    E_BONE_WEAPON_POMMEL,
    E_BONE_WEAPON_HANDLE,
    E_BONE_WEAPON_CENTER,
    E_BONE_WEAPON_CROSSR,
    E_BONE_WEAPON_CROSSL,
    E_BONE_WEAPON_BLADER,
    E_BONE_WEAPON_BLADEL,
    E_BONE_WEAPON_BLADEU,

    E_BONE_C_END //////////////////

};

#endif//_voxelquest_rig_h_
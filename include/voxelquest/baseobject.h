#ifndef _voxelquest_baseobject_h_
#define _voxelquest_baseobject_h_

#include "voxelquest/enums.h"
#include "voxelquest/vectors.h"
#include "voxelquest/path.h"
#include "voxelquest/rig.h"

#include <vector>
#include <string>
#include <map>

const static int MAX_STAT_VALUE=10;

#define E_CHAR_STAT(DDD) \
DDD(E_CS_STRENGTH) \
DDD(E_CS_DEXTERITY) \
DDD(E_CS_AGILITY) \
DDD(E_CS_INTELLIGENCE) \
DDD(E_CS_DISCIPLINE) \
DDD(E_CS_CHARISMA) \
DDD(E_CS_LENGTH)

const std::string E_CHAR_STAT_STRINGS[]={
    E_CHAR_STAT(DO_DESCRIPTION)
};

enum E_CHAR_STAT_VALS
{
    E_CHAR_STAT(DO_ENUM)
};

#define E_CHAR_STATUS(DDD) \
DDD(E_STATUS_HEALTH) \
DDD(E_STATUS_STAMINA) \
DDD(E_STATUS_MANA) \
DDD(E_STATUS_FOCUS) \
DDD(E_STATUS_SPIRIT) \
DDD(E_STATUS_LENGTH)

const std::string E_CHAR_STATUS_STRINGS[]={
    E_CHAR_STATUS(DO_DESCRIPTION)
};

enum E_CHAR_STATUS_VALS
{
    E_CHAR_STATUS(DO_ENUM)
};

enum E_POSETYPES
{
    E_POSETYPE_HUMAN,
    E_POSETYPE_WEAPON,
    E_POSETYPE_LENGTH
};

enum RLBN_FLAGS
{
    RLBN_FLAG_RIGHT=1,
    RLBN_FLAG_LEFT=2,
    RLBN_FLAG_BOTH=4,
    RLBN_FLAG_NEITHER=8
};

// do not rearrange
enum RLBN_ENUMS
{
    RLBN_RIGT,
    RLBN_LEFT,
    RLBN_BOTH,
    RLBN_NEIT,
    RLBN_LENGTH
};
// do not rearrange
enum E_ACTION_STATES
{
    E_ACT_NULL,
    E_ACT_ISHIT,
    E_ACT_ISJUMPING,
    E_ACT_ISWALKING,
    E_ACT_ISSWINGING,
    E_ACT_HASNOTHIT,
    E_ACT_ISPICKINGUP,
    E_ACT_LENGTH
};


typedef int BaseObjType;

struct SkillCard {
	
	// whenever x, do y
	
	
	
	// condition
	// subject
	// action
	// subject
	
	// gain / lose
	
	
	std::vector<int> triggers;
};

struct StatSheet {
	std::vector<int> availableSkills;
	std::vector<int> activeSkills;
	std::vector<int> statusList;
	
	int baseStats[E_CS_LENGTH];
	int unapplyedStats[E_CS_LENGTH];
	
	int curStatus[E_STATUS_LENGTH];
	int maxStatus[E_STATUS_LENGTH];
	
	int availPoints;
	
	
};




class BaseObj
{
private:
	FIVector4 centerPoint;
	FIVector4 linVelocity;
	
	bool actionStates[E_ACT_LENGTH*RLBN_LENGTH];
	
public:
	
	PathInfo targPath;
	StatSheet statSheet;
	
	int objectType;
	int maxFrames;
	
	PoseKey defaultPose;
	
	BaseObjType uid;
	BaseObjType parentUID;
	std::vector<BaseObjType> children;
	btVector3 startPoint;
	btVector3 skelOffset;
	
	std::vector<int> targWeaponStack;
	std::vector<BodyStruct> bodies;
	
	int actorId;
	int orgId;
	
	int contactCount;
	int isGrabbedById;
	int isGrabbedByHand;
	int entType;
	int subType;
	bool isHidden;
	bool isOpen;
	bool isEquipped;
	bool zeroZ;
	
	float bounciness;
	float friction;
	float windResistance;
	
	// skeleton
	btVector3 aabbMinSkel;
	btVector3 aabbMaxSkel;
	
	// visual objects
	btVector3 aabbMinVis;
	btVector3 aabbMaxVis;
	
	
	
	
	//////////////////
	// NPC SPECIFIC //
	//////////////////
	
	int tbDir;
	btVector3 tbPos;
	
	int swingType[4];
	int isGrabbingId[4];
	
	int hitCooldown;
	int jumpCooldown;
	
	
	float airCount;	
	float bindingPower;
	float swingCount;
	float blockCount;
	float lastBlockDis;
	
	btVector3 behaviorTarget;
	btVector3 npcRepel;
	
	
	
	//////////////////
	// END SPECIFIC //
	//////////////////
	
    bool hasAtLeast(int status, int val);
	
    void modifyStatus(int status, int modVal);
	
    btVector3 getUnitBounds(bool getMax);
	
	
	
	
    bool holdingWeapon(int handNum);
	
	
    void setDamping(float linear, float angular);
	
    void clearAABB(btVector3* aabbMin, btVector3* aabbMax);
	
    void addAABBPoint(btVector3* aabbMin, btVector3* aabbMax, btVector3 newPoint);
	
	
    bool hasBodies();
	
    void multVel(int i, btVector3 velMod);
	
    void multVelAng(int i, btVector3 velMod);
	
    void addVel(int i, btVector3 velMod);
	
    FIVector4* getVel(int i);
	
    BodyStruct* getBodyByBoneId(int id);
	
    float getTotalMass();
	
    float getMarkerMass();
	
	
	
    void wakeAll();
	
	
	
    bool allFalling();
	
    bool baseContact();
	
    float getPlanarVel();
	
    void setLinVel(btVector3 newVel, int i);
	
    void applyImpulses(float timeDelta, int i);
	
	
    btVector3 getWeaponPos(int curPos);
	
    void setGrabbedBy(int newId, int handNum);

    void flushImpulses();
	
    float turnTowardsTargAng(float targAng);
	
    float turnTowardsPointDelta(btVector3 targPoint);
	
    void applyAngularImpulse(btVector3 newAV, bool delayed, int i);
	
    void applyImpulse(btVector3 imp, bool delayed, int i);
	
    void applyImpulseOtherRot(btVector3 imp, btMatrix3x3 otherRot, bool delayed, int i);
	
    bool isHumanoid();	
	
    void moveOffset(btVector3 offset, int ind);
	
    void moveToPoint(btVector3 newPoint, int ind);
	
    btVector3 getCenterPoint(int ind);
	
    FIVector4* getCenterPointFIV(int ind);
	
	
    BaseObj();
	
    void removeChild(BaseObjType _uid);
	
	
    float healthPerc();
	
    bool isDead();
	
    bool isAlive();
	
    bool getActionState(int action, int handNum);
    void setActionState(int action, int handNum, bool newVal);
	
    void clearActionStates();
	
	
    void init(
        BaseObjType _uid,
        BaseObjType _parentUID,
        int _objectType,
        int _entType,
        int _subType,
        FIVector4* cellPos
    );
	
};

typedef std::map<BaseObjType, BaseObj>::iterator itBaseObj;

#endif//_voxelquest__h_
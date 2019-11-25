


int boolToInt(bool val);

uint* toUintPtr(char* baseAdr);

int* toIntPtr(char* baseAdr);

float* toFloatPtr(char* baseAdr);

float fract(float val);

int intDiv(int v, int s);

void pack16(float num, float &outR, float &outG);

uint zipBits(bool* boolArr, int len);

void unzipBits(uint zipped, bool* boolArr, int len);


int clamp(int val, int min, int max);

float mixf(float v1, float v2, float lerpVal);

float mixb(float v1, float v2, bool lerpVal);


float roundVal(float val);


float signedFloor(float val);

float clampf(float val, float min, float max);

float smoothstep(float edge0, float edge1, float x);

inline float clampfZO(float val) {
	return clampf(val, 0.0f, 1.0f);
}


// these random generators are used for things that DO NOT effect networking

inline float fGenRand() {
	float intPart;
	float res = abs ( modf(sin(RAND_COUNTER*433.2351267) * 43758.8563f, &intPart) );
	RAND_COUNTER += 1.0f;
	if (RAND_COUNTER >= 8000000.0f) {
		RAND_COUNTER = 0.0f;
	}
	
	return res;
}

inline int iGenRand(int nMin, int nMax)
{
	return nMin + (int)( (fGenRand()) * (nMax + 1 - nMin));
}

// these random generators are used for things that DO effect networking

inline float fGenRand2() {
	float intPart;
	float res = abs ( modf(sin(RAND_COUNTER2*433.2351267) * 43758.8563f, &intPart) );
	RAND_COUNTER2 += 1.0f;
	if (RAND_COUNTER2 >= 8000000.0f) {
		RAND_COUNTER2 = 0.0f;
	}
	
	return res;
}

inline int iGenRand2(int nMin, int nMax)
{
	return nMin + (int)( (fGenRand2()) * (nMax + 1 - nMin));
}




float fSeedRand2(float x, float y);

int iSeedRand2(float x, float y, int minV, int maxV);




unsigned int intLogB2(unsigned int val);

int intPow(int x, int p);

int wrapCoord(int val, int max);



class FIVector4 {
private:
	iVector4 iv4;
	fVector4 fv4;

	iVector3 iv3;
	fVector3 fv3;

	iVector2 iv2;
	fVector2 fv2;

public:

    FIVector4();

    float operator[] (int ind);

    void setIndex(int ind, float val);
	
    float getIndex(int ind);

    btVector3 getBTV();
    void setBTV(btVector3 myBTV);

    void setIXYZW(int x, int y, int z, int w);
    void setIXYZ(int x, int y, int z);
    void setIXY(int x, int y);
    void setIX(int x);
    void setIY(int y);
    void setIZ(int z);
    void setIW(int w);

    void copyFrom(FIVector4 *cf);

    void copyIntDiv(FIVector4 *cf, int val);

    void copyIntMult(FIVector4 *cf, int val);

    void setFloatArr(float* vals);

    void setFXYZW(float x, float y, float z, float w);
    void setFXYZ(float x, float y, float z);
	
    void setFXYZ(float scalar);

    void setFXYZRef(FIVector4 *v1);

    void setFXY(float x, float y);
    void setFX(float x);
    void setFY(float y);
    void setFZ(float z);
    void setFW(float w);
	
	
	
	
    void fixForRot();
	
    void setRandNoSeed();
	
    void setRand(FIVector4 *seedPos, float addVal, float multVal);

    void addXYZ(float scalar, float multiplier=1.0f);
	
    void addW(float scalar);
	
    void addXYZ(float scalarX, float scalarY, float scalarZ, float multiplier=1.0f);
	
    void addXYZW(float scalarX, float scalarY, float scalarZ, float scalarW, float multiplier=1.0f);
	
    void addXYZRef(FIVector4 *scalar, float multiplier=1.0f);
	
    void multXYZRef(FIVector4 *scalar);

    void lerpXYZ(FIVector4 *v2, float amount);
	
    void lerpXYZW(FIVector4 *v2, float amount);
	
    void lerpXYZ(FIVector4 *v2, FIVector4* av);


    void multXYZ(float scalar);
    void multXYZ(float scalarX, float scalarY, float scalarZ);


    void multXYZ(FIVector4 *scalar);

    void divXYZ(FIVector4 *scalar);

    void floorXYZ();


    void intDivXYZ(int scalar);
	
    void intDivXYZ(int scalarX, int scalarY, int scalarZ);

    void modXYZ(int scalar);

	

    void wrapXYZ(int scalar);

    void wrapXY(int scalar);

    void powXYZ(float p1, float p2, float p3);

    void absXYZ();
	
    void averageXYZ(FIVector4 *v1, FIVector4 *v2);
	
    void averageNegXYZ(FIVector4 *v1, FIVector4 *v2);


    void minXYZ(FIVector4 *v1, FIVector4 *v2);

    void maxXYZ(FIVector4 *v1, FIVector4 *v2);

    void clampXYZS(float minV, float maxV);

    void clampXYZ(FIVector4 *minV, FIVector4 *maxV);

    void clampX(FIVector4 *minV, FIVector4 *maxV);
    void clampY(FIVector4 *minV, FIVector4 *maxV);
    void clampZ(float minV, float maxV);



    bool inBoundsIsEqualXYZ(FIVector4 *minV, FIVector4 *maxV);

    static void normalizeBounds(FIVector4 *minBounds, FIVectorW4 *maxBounds);


	
    static bool intersectInt(FIVector4 *aMin, FIVector4 *aMax, FIVector4 *bMin, FIVector4 *bMax);
	
	

    static bool intersect(FIVector4 *aMin, FIVector4 *aMax, FIVector4 *bMin, FIVector4 *bMax);

    static void growBoundary(FIVector4 *minB, FIVector4 *maxB, FIVector4 *minV, FIVector4 *maxV);
	
    bool anyXYZ();
	
    bool any();
    bool all();


    bool inBoundsXYZ(FIVector4 *minV, FIVector4 *maxV);
	
    bool inBoundsXYZSlack(FIVector4 *minV, FIVector4 *maxV, float slack);





    bool inBoundsXY(FIVector4 *minV, FIVector4 *maxV);

    bool iNotEqual(FIVector4 *otherVec);
	
    bool fNotEqual4(FIVector4 *otherVec);



    float wrapDistance(FIVector4 *otherVec, int maxPitch, bool doSet=true);

    void rotate(float a, int plane);

    void rotate90(int ind);
	
    float distanceXY(FIVector4 *otherVec);

    float distance(FIVector4 *otherVec);
	
	

    float length();
    float lengthXY();

    float manhattanDis(FIVector4 *otherVec);

    void normalize();
	
    void normalizeXY();
	

    float dot(FIVector4 *otherVec);

    static void cross(FIVector4 *outVec, FIVector4 *v1, FIVector4 *v2);




    iVector4 *getIXYZW();
    fVector4 *getFXYZW();

    iVector3 *getIXYZ();
    fVector3 *getFXYZ();

    iVector2 *getIXY();
    fVector2 *getFXY();

    int getIX();
    int getIY();
    int getIZ();
    int getIW();

    float getFX();
    float getFY();
    float getFZ();
    float getFW();


};




void hsv2rgb(materialNode* matNode);


struct RotationInfo {
	float rotMatrix[16];
	FIVector4 basePoint;
	FIVector4 axisAngle;
};

struct ModUnitStruct {
	FIVector4 basePos;
	int brushAction;
	int modType;
	int radius;
};



float getRandSeededPos(float xv, float yv, float zv);


float getRandSeeded(FIVector4 *seedPos, FIVector4 *seedVals);

int iGetRandSeeded(
    FIVector4 *seedPos,
    FIVector4 *seedVals,
    int minV,
    int maxV
);




class AxisRotation {

public:

	float rotationMatrix[16];
	float inputMatrix[4];
	float outputMatrix[4];

	FIVector4 tempRes1;
	FIVector4 tempRes2;
	FIVector4 tempRes3;

    void doRotationOr(FIVector4 *output, FIVector4 *input, int orientationOffset);
	

    void doRotation(FIVector4 *output, FIVector4 *input, FIVector4 *axis, float angle);
	
	
	
	
    void buildRotMatrix(
        RotationInfo* rotInfo
    );
	
	
    void applyRotation(
        FIVector4 *output,
        FIVector4 *input,
        RotationInfo* rotInfo
    );
	
	
    void doRotationTBN(
        FIVector4 *output,
        FIVector4 *input,
        FIVector4 *axisAngle,
        FIVector4 *parentOffset,
        FIVector4 *baseOffset
    );
	

};
AxisRotation axisRotationInstance;








void safeNorm(btVector3 &normRef);

float getShortestAngle(float begInRad, float endInRad, float amount);

btVector3 roundBTV(btVector3 v);
btVector3 floorBTV(btVector3 v);

btVector3 multByOtherRot(btVector3 imp, btMatrix3x3 otherRot);

btVector3 rotBTV2D(btVector3 source, float ang);

struct SphereStruct {
	FIVector4 position;
	float maxRad;
	float curRad;
	float radVel;
	float radAcc;
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
	vector<BaseObjType> children;
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

typedef map<BaseObjType, BaseObj>::iterator itBaseObj;


class VNode {
public:
	
	int tokenIndex;
	int ruleNumber;
	std::vector<VNode*> children;
	
    VNode(int _tokenIndex);
	
    ~VNode();
	
};

struct AssignStruct {
	VNode* lastAssign;
	VNode* newAssign;
	int tokenIndex;
	int genIndex;
};



inline float qSign(float x) {return (x >= 0.0f) ? +1.0f : -1.0f;}
inline float qNorm(float a, float b, float c, float d) {return sqrt(a * a + b * b + c * c + d * d);}


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


class VBOWrapper {
public:
	GLuint vao, vbo, ibo;

	int drawEnum;
	int sizeOfID;
	int maxSizeOfID;
	int sizeOfVD;
	int maxSizeOfVD;
	int numVecs;
	int attSize;
	// GLfloat* vertexData;
	// GLuint* indexData;

    VBOWrapper();
	
    void update(
        GLfloat* _vertexData,
        int _sizeOfVD,
        GLuint* _indexData,
        int _sizeOfID
    );
	
    void updateNew();
	
    void init(
        GLfloat* _vertexData,
        int _sizeOfVD,
        int _maxSizeOfVD,
        GLuint* _indexData,
        int _sizeOfID,
        int _maxSizeOfID,
        int _numVecs, // number of 4 component vecs
        int _drawEnum //GL_DYNAMIC_DRAW GL_STATIC_DRAW
    );
	
    void draw();
	
    void drawPoints();
	
};


class VBOGrid {
public:

	int xpitch;
	int ypitch;
	
	int totVerts;
	int totInd;

	VBOWrapper vboWrapper;


	std::vector<float> vertexVec;
	std::vector<uint> indexVec;
	

    VBOGrid();
	
    void init(
        int _xpitch,
        int _ypitch
    );
};

class TBOWrapper {
public:
	GLuint tbo_buf;
	GLuint tbo_tex;
	
	int dataSize;
	
	bool isFloat;
	
    TBOWrapper();
	
    void init(bool _isFloat, float* tbo_data, uint* tbo_data2, int _dataSize);
	
    void update(float* tbo_data, uint* tbo_data2, int newDataSize);
	
	
};

class ThreadWrapper {
private:
	bool threadRunningEx; // thread is running (exclusive, must lock)
public:
	std::thread threadMain;
	std::mutex threadMutex;
	bool threadRunning; // thread is running (not exclusive)
	
	int threadDataInt[THREAD_DATA_COUNT];
	
    ThreadWrapper();
	
    void init();
	
    void setRunningLocked(bool val) \;
	
    bool isReady();
	
};



struct PushModStruct
{
	int actionType;
	FIVector4 data[4];
};

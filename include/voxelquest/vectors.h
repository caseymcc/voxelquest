#ifndef _voxelquest_vectors_h_
#define _voxelquest_vectors_h_

#include "voxelquest/types.h"
#include "voxelquest/constants.h"
#include "voxelquest/mat.h"
#include "voxelquest/ivec.h"

//#include <LinearMath/btVector3.h>
//#include <LinearMath/btMatrix3x3.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

enum E_PLANE_TYPES
{
    E_PLANE_XY,
    E_PLANE_YZ,
    E_PLANE_XZ,
    E_PLANE_LENGTH
};

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

float fGenRand();

inline int iGenRand(int nMin, int nMax)
{
	return nMin + (int)( (fGenRand()) * (nMax + 1 - nMin));
}

// these random generators are used for things that DO effect networking

float fGenRand2();

inline int iGenRand2(int nMin, int nMax)
{
	return nMin + (int)( (fGenRand2()) * (nMax + 1 - nMin));
}




float fSeedRand2(float x, float y);

int iSeedRand2(float x, float y, int minV, int maxV);




unsigned int intLogB2(unsigned int val);

int intPow(int x, int p);

int wrapCoord(int val, int max);

struct iVector4
{
    int x;
    int y;
    int z;
    int w;
};
struct fVector4
{
    float x;
    float y;
    float z;
    float w;
};

struct iVector3
{
    int x;
    int y;
    int z;
};
struct fVector3
{
    float x;
    float y;
    float z;
};


struct iVector2
{
    int x;
    int y;
};
struct fVector2
{
    float x;
    float y;
};

class FIVector4 {
public:

    FIVector4();

    float operator[] (int ind);

    void setIndex(int ind, float val);
	
    float getIndex(int ind);

//    btVector3 getBTV();
//    void setBTV(btVector3 myBTV);
	void setVec3(vec3 v);
	vec3 getVec3();
	ivec3 getIVec3();

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

    static void normalizeBounds(FIVector4 *minBounds, FIVector4 *maxBounds);


	
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

private:
    iVector4 iv4;
    fVector4 fv4;

    iVector3 iv3;
    fVector3 fv3;

    iVector2 iv2;
    fVector2 fv2;
};
typedef Vector4 vec4;




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

void angleToVec(FIVector4* fv, float xr, float yr);
float getShortestAngle(float begInRad, float endInRad, float amount);

inline float qSign(float x) {return (x >= 0.0f) ? +1.0f : -1.0f;}
inline float qNorm(float a, float b, float c, float d) {return sqrt(a * a + b * b + c * c + d * d);}

void doTraceVecND(std::string traceVal0, FIVector4 *fv);
void doTraceVecND4(std::string traceVal0, FIVector4 *fv);
void doTraceVec(std::string traceVal0, FIVector4 *fv);

#endif//_voxelquest__h_
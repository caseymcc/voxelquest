#include "vectors.h"


int boolToInt(bool val) {
	if (val) {
		return 1;
	}
	else {
		return 0;
	}
}

uint* toUintPtr(char* baseAdr) {
	void* voidPtr = baseAdr;
	uint* uintPtr = (uint*)voidPtr;
	return uintPtr;
}

int* toIntPtr(char* baseAdr) {
	void* voidPtr = baseAdr;
	int* intPtr = (int*)voidPtr;
	return intPtr;
}

float* toFloatPtr(char* baseAdr) {
	void* voidPtr = baseAdr;
	float* floatPtr = (float*)voidPtr;
	return floatPtr;
}

float fract(float val) {
	return (val - floor(val));
}

int intDiv(int v, int s) {
	float fv = v;
	float fs = s;

	if (v < 0) {
		return -ceil(-fv / fs);
	}
	else {
		return v / s;
	}
}

void pack16(float num, float &outR, float &outG) {

    int iz = int(num);
    int ir = iz%256;
    int ig = iz/256;

    outR = float(ir)/255.0;
    outG = float(ig)/255.0;

}

uint zipBits(bool* boolArr, int len) {
	uint i;
	uint result = 0;
	uint oneVal = 1;
	
	for (i = 0; i < len; i++) {
		if (boolArr[i]) {
			result = result | (oneVal<<i);
		}
		
	}
	
	return result;
}

void unzipBits(uint zipped, bool* boolArr, int len) {
	uint i;
	uint zeroVal = 0;
	uint oneVal = 1;
	
	
	
	for (i = 0; i < len; i++) {
		boolArr[i] = (
			(zipped & (oneVal<<i)) > zeroVal
		);
	}
	
}


// inline uint clampChar(int baseVal, int bitShift) {
// 	int val = baseVal;
	
// 	if (val > 255) {
// 		val = 255;
// 	}
// 	if (val < 0) {
// 		val = 0;
// 	}
	
// 	uint retVal = val;
	
// 	return (retVal << bitShift);
	
// }

int clamp(int val, int min, int max) {
	if (val > max) {
		val = max;
	}
	if (val < min) {
		val = min;
	}
	return val;
}

float mixf(float v1, float v2, float lerpVal) {
	return v1*(1.0f-lerpVal) + v2*lerpVal;
}

float mixb(float v1, float v2, bool lerpVal) {
	if (lerpVal) {
		return v2;
	}
	else {
		return v1;
	}
}


float roundVal(float val) {
	//return floor(val);
	if (val < 0.0) {
		return -floor(abs(val)+0.5f);
	}
	else {
		return floor(val+0.5f);
	}
}


float signedFloor(float val) {
	//return floor(val);
	if (val < 0.0) {
		return -floor(abs(val));
	}
	else {
		return floor(val);
	}
}

float clampf(float val, float min, float max) {
	if (val > max) {
		val = max;
	}
	if (val < min) {
		val = min;
	}
	return val;
}

float smoothstep(float edge0, float edge1, float x) {
    // Scale, bias and saturate x to 0..1 range
    x = clampf((x - edge0)/(edge1 - edge0), 0.0f, 1.0f); 
    // Evaluate polynomial
    return x*x*(3.0f - 2.0f*x);
}

float fSeedRand2(float x, float y) {
	float intPart;
	return modf(sin(x * 12.9898 + y * 78.233) * 43758.5453, &intPart);
}

int iSeedRand2(float x, float y, int minV, int maxV) {
	float res = fSeedRand2(x, y);
	return minV + max(int((maxV + 1 - minV) * res), 0);
}




unsigned int intLogB2 (unsigned int val) {
	unsigned int ret = -1;
	while (val != 0) {
		val >>= 1;
		ret++;
	}
	return ret;
}

int intPow(int x, int p) {
	int i = 1;
	for (int j = 1; j <= p; j++) {
		i *= x;
	}
	return i;
}

int wrapCoord(int val, int max) {
	while (val < 0) {
		val += max;
	}
	while (val >= max) {
		val -= max;
	}

	return val;
}




FIVector4::FIVector4() {
		iv4.x = 0;
		iv4.y = 0;
		iv4.z = 0;
		iv4.w = 0;
		fv4.x = 0.0;
		fv4.y = 0.0;
		fv4.z = 0.0;
		fv4.w = 0.0;
	}

	float FIVector4::operator[] (int ind) { //float&
		
		switch (ind) {
			case 0:
				return fv4.x;
			break;
			case 1:
				return fv4.y;
			break;
			case 2:
				return fv4.z;
			break;
			case 3:
				return fv4.w;
			break;
		}
		
		cout << "invalid vector index";
		return -1.0f;
	}

	void FIVector4::setIndex(int ind, float val) {
		
		switch (ind) {
			case 0:
				fv4.x = val;
				iv4.x = val;
			break;
			case 1:
				fv4.y = val;
				iv4.y = val;
			break;
			case 2:
				fv4.z = val;
				iv4.z = val;
			break;
			case 3:
				fv4.w = val;
				iv4.w = val;
			break;
		}
	}
	
	float FIVector4::getIndex(int ind) {
		
		switch (ind) {
			case 0:
				return fv4.x;
			break;
			case 1:
				return fv4.y;
			break;
			case 2:
				return fv4.z;
			break;
			case 3:
				return fv4.w;
			break;
		}
		
		return 0.0f;
	}

	btVector3 FIVector4::getBTV() {
		return btVector3(fv4.x,fv4.y,fv4.z);
	}
	void FIVector4::setBTV(btVector3 myBTV) {
		setFXYZ(myBTV.getX(), myBTV.getY(), myBTV.getZ());
	}

	void FIVector4::setIXYZW(int x, int y, int z, int w) {
		iv4.x = x;
		iv4.y = y;
		iv4.z = z;
		iv4.w = w;
		fv4.x = (float)x;
		fv4.y = (float)y;
		fv4.z = (float)z;
		fv4.w = (float)w;
	}
	void FIVector4::setIXYZ(int x, int y, int z) {
		iv4.x = x;
		iv4.y = y;
		iv4.z = z;
		fv4.x = (float)x;
		fv4.y = (float)y;
		fv4.z = (float)z;
	}
	void FIVector4::setIXY(int x, int y) {
		iv4.x = x;
		iv4.y = y;
		fv4.x = (float)x;
		fv4.y = (float)y;
	}
	void FIVector4::setIX(int x) {
		iv4.x = x;
		fv4.x = (float)x;
	}
	void FIVector4::setIY(int y) {
		iv4.y = y;
		fv4.y = (float)y;
	}
	void FIVector4::setIZ(int z) {
		iv4.z = z;
		fv4.z = (float)z;
	}
	void FIVector4::setIW(int w) {
		iv4.w = w;
		fv4.w = (float)w;
	}

	void FIVector4::copyFrom(FIVector4 *cf) {
		iv4.x = cf->getIX();
		iv4.y = cf->getIY();
		iv4.z = cf->getIZ();
		iv4.w = cf->getIW();

		fv4.x = cf->getFX();
		fv4.y = cf->getFY();
		fv4.z = cf->getFZ();
		fv4.w = cf->getFW();
	}

	void FIVector4::copyIntDiv(FIVector4 *cf, int val) {
		iv4.x = cf->getIX();
		iv4.y = cf->getIY();
		iv4.z = cf->getIZ();
		iv4.w = cf->getIW();

		fv4.x = cf->getFX();
		fv4.y = cf->getFY();
		fv4.z = cf->getFZ();
		fv4.w = cf->getFW();

		intDivXYZ(val);
	}

	void FIVector4::copyIntMult(FIVector4 *cf, int val) {
		iv4.x = cf->getIX();
		iv4.y = cf->getIY();
		iv4.z = cf->getIZ();
		iv4.w = cf->getIW();

		fv4.x = cf->getFX();
		fv4.y = cf->getFY();
		fv4.z = cf->getFZ();
		fv4.w = cf->getFW();

		multXYZ(val);
	}

	void FIVector4::setFloatArr(float* vals) {
		fv4.x = vals[0];
		fv4.y = vals[1];
		fv4.z = vals[2];
		iv4.x = fv4.x;
		iv4.y = fv4.y;
		iv4.z = fv4.z;
	}

	void FIVector4::setFXYZW(float x, float y, float z, float w) {
		fv4.x = x;
		fv4.y = y;
		fv4.z = z;
		fv4.w = w;
		iv4.x = (int)x;
		iv4.y = (int)y;
		iv4.z = (int)z;
		iv4.w = (int)w;
	}
	void FIVector4::setFXYZ(float x, float y, float z) {
		fv4.x = x;
		fv4.y = y;
		fv4.z = z;
		iv4.x = (int)x;
		iv4.y = (int)y;
		iv4.z = (int)z;
	}
	
	void FIVector4::setFXYZ(float scalar) {
		fv4.x = scalar;
		fv4.y = scalar;
		fv4.z = scalar;
		iv4.x = (int)scalar;
		iv4.y = (int)scalar;
		iv4.z = (int)scalar;
	}

	void FIVector4::setFXYZRef(FIVector4 *v1) {
		fv4.x = v1->getFX();
		fv4.y = v1->getFY();
		fv4.z = v1->getFZ();
		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void FIVector4::setFXY(float x, float y) {
		fv4.x = x;
		fv4.y = y;
		iv4.x = (int)x;
		iv4.y = (int)y;
	}
	void FIVector4::setFX(float x) {
		fv4.x = x;
		iv4.x = (int)x;
	}
	void FIVector4::setFY(float y) {
		fv4.y = y;
		iv4.y = (int)y;
	}
	void FIVector4::setFZ(float z) {
		fv4.z = z;
		iv4.z = (int)z;
	}
	void FIVector4::setFW(float w) {
		fv4.w = w;
		iv4.w = (int)w;
	}
	
	
	
	
	void FIVector4::fixForRot() {
		iv4.x = -iv4.x;
		iv4.y = -iv4.y;
		iv4.z = -iv4.z;
		
		iv4.x = max(iv4.x, 0);
		iv4.y = max(iv4.y, 0);
		iv4.z = max(iv4.z, 0);
		
		fv4.x = (float)iv4.x;
		fv4.y = (float)iv4.y;
		fv4.z = (float)iv4.z;
	}
	
	void FIVector4::setRandNoSeed() {

		fv4.x = fGenRand();
		fv4.y = fGenRand();
		fv4.z = fGenRand();

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void FIVector4::setRand(FIVector4 *seedPos, float addVal, float multVal) {

		// default output is -1 to 1

		FIVector4 r0;
		FIVector4 r1;
		FIVector4 r2;

		FIVector4 l0;
		FIVector4 l1;
		FIVector4 l2;

		float intPart;

		r0.setFXYZ(12.989f, 78.233f, 98.422f);
		r1.setFXYZ(93.989f, 67.345f, 54.256f);
		r2.setFXYZ(43.332f, 93.532f, 43.734f);

		l0.setFXYZ(
			seedPos->getFX(),
			seedPos->getFY(),
			seedPos->getFZ()
		);
		l1.setFXYZ(
			seedPos->getFZ(),
			seedPos->getFY(),
			seedPos->getFX()
		);
		l2.setFXYZ(
			seedPos->getFY(),
			seedPos->getFX(),
			seedPos->getFZ()
		);

		fv4.x = modf(sin(l0.dot(&r0)) * 43758.8563f, &intPart);
		fv4.y = modf(sin(l1.dot(&r1)) * 24634.6345f, &intPart);
		fv4.z = modf(sin(l2.dot(&r2)) * 56445.2345f, &intPart);
		
		
		fv4.x += addVal;
		fv4.y += addVal;
		fv4.z += addVal;
		
		fv4.x *= multVal;
		fv4.y *= multVal;
		fv4.z *= multVal;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void addXYZ(float scalar, float multiplier = 1.0f) {
		fv4.x += scalar * multiplier;
		fv4.y += scalar * multiplier;
		fv4.z += scalar * multiplier;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void FIVector4::addW(float scalar) {
		fv4.w += scalar;
		iv4.w = (int)fv4.w;
	}
	
	void FIVector4::addXYZ(float scalarX, float scalarY, float scalarZ, float multiplier = 1.0f) {
		fv4.x += scalarX * multiplier;
		fv4.y += scalarY * multiplier;
		fv4.z += scalarZ * multiplier;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void FIVector4::addXYZW(float scalarX, float scalarY, float scalarZ, float scalarW, float multiplier = 1.0f) {
		fv4.x += scalarX * multiplier;
		fv4.y += scalarY * multiplier;
		fv4.z += scalarZ * multiplier;
		fv4.w += scalarW * multiplier;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
		iv4.w = (int)fv4.w;
	}
	
	void FIVector4::addXYZRef(FIVector4 *scalar, float multiplier = 1.0f) {
		fv4.x += scalar->getFX() * multiplier;
		fv4.y += scalar->getFY() * multiplier;
		fv4.z += scalar->getFZ() * multiplier;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void FIVector4::multXYZRef(FIVector4 *scalar) {
		fv4.x *= scalar->getFX();
		fv4.y *= scalar->getFY();
		fv4.z *= scalar->getFZ();

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void FIVector4::lerpXYZ(FIVector4 *v2, float amount) {
		float iamount = 1.0f-amount;
		fv4.x = fv4.x*iamount + v2->getFX()*amount;
		fv4.y = fv4.y*iamount + v2->getFY()*amount;
		fv4.z = fv4.z*iamount + v2->getFZ()*amount;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void FIVector4::lerpXYZW(FIVector4 *v2, float amount) {
		float iamount = 1.0f-amount;
		fv4.x = fv4.x*iamount + v2->getFX()*amount;
		fv4.y = fv4.y*iamount + v2->getFY()*amount;
		fv4.z = fv4.z*iamount + v2->getFZ()*amount;
		fv4.w = fv4.w*iamount + v2->getFW()*amount;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
		iv4.w = (int)fv4.w;
	}
	
	void FIVector4::lerpXYZ(FIVector4 *v2, FIVector4* av) {
		
		float xa;
		float ya;
		float za;
		
		if (av == NULL) {
			xa = 1.0f;
			ya = 1.0f;
			za = 1.0f;
		}
		else {
			xa = (*av)[0];
			ya = (*av)[1];
			za = (*av)[2];
		}
		
		
		float ixa = 1.0f-xa;
		float iya = 1.0f-ya;
		float iza = 1.0f-za;
		fv4.x = fv4.x*ixa + v2->getFX()*xa;
		fv4.y = fv4.y*iya + v2->getFY()*ya;
		fv4.z = fv4.z*iza + v2->getFZ()*za;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}


	void FIVector4::multXYZ(float scalar) {
		fv4.x *= scalar;
		fv4.y *= scalar;
		fv4.z *= scalar;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	void FIVector4::multXYZ(float scalarX, float scalarY, float scalarZ) {
		fv4.x *= scalarX;
		fv4.y *= scalarY;
		fv4.z *= scalarZ;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}


	void FIVector4::multXYZ(FIVector4 *scalar) {
		fv4.x *= scalar->getFX();
		fv4.y *= scalar->getFY();
		fv4.z *= scalar->getFZ();

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void FIVector4::divXYZ(FIVector4 *scalar) {
		fv4.x /= scalar->getFX();
		fv4.y /= scalar->getFY();
		fv4.z /= scalar->getFZ();

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void FIVector4::floorXYZ() {
		fv4.x = floor(fv4.x);
		fv4.y = floor(fv4.y);
		fv4.z = floor(fv4.z);

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}


	void FIVector4::intDivXYZ(int scalar) {

		iv4.x = intDiv(iv4.x, scalar);
		iv4.y = intDiv(iv4.y, scalar);
		iv4.z = intDiv(iv4.z, scalar);

		fv4.x = (float)iv4.x;
		fv4.y = (float)iv4.y;
		fv4.z = (float)iv4.z;

	}
	
	void FIVector4::intDivXYZ(int scalarX, int scalarY, int scalarZ) {


		iv4.x = intDiv(iv4.x, scalarX);
		iv4.y = intDiv(iv4.y, scalarY);
		iv4.z = intDiv(iv4.z, scalarZ);

		fv4.x = (float)iv4.x;
		fv4.y = (float)iv4.y;
		fv4.z = (float)iv4.z;

	}

	void FIVector4::modXYZ(int scalar) {
		iv4.x = iv4.x % scalar;
		iv4.y = iv4.y % scalar;
		iv4.z = iv4.z % scalar;

		fv4.x = (float)iv4.x;
		fv4.y = (float)iv4.y;
		fv4.z = (float)iv4.z;

	}

	

	void FIVector4::wrapXYZ(int scalar) {
		iv4.x = wrapCoord(iv4.x, scalar);
		iv4.y = wrapCoord(iv4.y, scalar);
		iv4.z = wrapCoord(iv4.z, scalar);

		fv4.x = (float)iv4.x;
		fv4.y = (float)iv4.y;
		fv4.z = (float)iv4.z;

	}

	void FIVector4::wrapXY(int scalar) {
		iv4.x = wrapCoord(iv4.x, scalar);
		iv4.y = wrapCoord(iv4.y, scalar);

		fv4.x = (float)iv4.x;
		fv4.y = (float)iv4.y;

	}

	void FIVector4::powXYZ(float p1, float p2, float p3) {
		fv4.x = pow(fv4.x,p1);
		fv4.y = pow(fv4.y,p2);
		fv4.z = pow(fv4.z,p3);

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void FIVector4::absXYZ() {
		fv4.x = abs(fv4.x);
		fv4.y = abs(fv4.y);
		fv4.z = abs(fv4.z);

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void FIVector4::averageXYZ(FIVector4 *v1, FIVector4 *v2) {
		fv4.x = (v1->getFX() + v2->getFX())*0.5f;
		fv4.y = (v1->getFY() + v2->getFY())*0.5f;
		fv4.z = (v1->getFZ() + v2->getFZ())*0.5f;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	
	void FIVector4::averageNegXYZ(FIVector4 *v1, FIVector4 *v2) {
		fv4.x = (v1->getFX() - v2->getFX())*0.5f;
		fv4.y = (v1->getFY() - v2->getFY())*0.5f;
		fv4.z = (v1->getFZ() - v2->getFZ())*0.5f;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}


	void FIVector4::minXYZ(FIVector4 *v1, FIVector4 *v2) {
		fv4.x = std::min(v1->getFX(), v2->getFX());
		fv4.y = std::min(v1->getFY(), v2->getFY());
		fv4.z = std::min(v1->getFZ(), v2->getFZ());

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void FIVector4::maxXYZ(FIVector4 *v1, FIVector4 *v2) {
		fv4.x = std::max(v1->getFX(), v2->getFX());
		fv4.y = std::max(v1->getFY(), v2->getFY());
		fv4.z = std::max(v1->getFZ(), v2->getFZ());

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void FIVector4::clampXYZS(float minV, float maxV) {
		if (fv4.x < minV) {
			fv4.x = minV;
		}
		if (fv4.y < minV) {
			fv4.y = minV;
		}
		if (fv4.z < minV) {
			fv4.z = minV;
		}
		if (fv4.x > maxV) {
			fv4.x = maxV;
		}
		if (fv4.y > maxV) {
			fv4.y = maxV;
		}
		if (fv4.z > maxV) {
			fv4.z = maxV;
		}

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void FIVector4::clampXYZ(FIVector4 *minV, FIVector4 *maxV) {
		if (fv4.x < minV->getFX()) {
			fv4.x = minV->getFX();
		}
		if (fv4.y < minV->getFY()) {
			fv4.y = minV->getFY();
		}
		if (fv4.z < minV->getFZ()) {
			fv4.z = minV->getFZ();
		}
		if (fv4.x > maxV->getFX()) {
			fv4.x = maxV->getFX();
		}
		if (fv4.y > maxV->getFY()) {
			fv4.y = maxV->getFY();
		}
		if (fv4.z > maxV->getFZ()) {
			fv4.z = maxV->getFZ();
		}

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}

	void FIVector4::clampX(FIVector4 *minV, FIVector4 *maxV) {
		if (fv4.x < minV->getFX()) {
			fv4.x = minV->getFX();
		}
		if (fv4.x > maxV->getFX()) {
			fv4.x = maxV->getFX();
		}

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	void FIVector4::clampY(FIVector4 *minV, FIVector4 *maxV) {

		if (fv4.y < minV->getFY()) {
			fv4.y = minV->getFY();
		}
		if (fv4.y > maxV->getFY()) {
			fv4.y = maxV->getFY();
		}

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;
	}
	void FIVector4::clampZ(float minV, float maxV) {

		if (fv4.z < minV) {
			fv4.z = minV;
		}
		if (fv4.z > maxV) {
			fv4.z = maxV;
		}

		iv4.z = (int)fv4.z;
	}



	bool FIVector4::inBoundsIsEqualXYZ(FIVector4 *minV, FIVector4 *maxV) {
		if (fv4.x < minV->getFX()) {
			return false;
		}
		if (fv4.y < minV->getFY()) {
			return false;
		}
		if (fv4.z < minV->getFZ()) {
			return false;
		}
		if (fv4.x > maxV->getFX()) {
			return false;
		}
		if (fv4.y > maxV->getFY()) {
			return false;
		}
		if (fv4.z > maxV->getFZ()) {
			return false;
		}

		return true;
	}

	static void FIVector4::normalizeBounds(FIVector4 *minBounds, FIVector4 *maxBounds) {
		float temp;

		if (minBounds->getFX() > maxBounds->getFX()) {
			temp = maxBounds->getFX();
			maxBounds->setFX(minBounds->getFX());
			minBounds->setFX(temp);
		}
		if (minBounds->getFY() > maxBounds->getFY()) {
			temp = maxBounds->getFY();
			maxBounds->setFY(minBounds->getFY());
			minBounds->setFY(temp);
		}
		if (minBounds->getFZ() > maxBounds->getFZ()) {
			temp = maxBounds->getFZ();
			maxBounds->setFZ(minBounds->getFZ());
			minBounds->setFZ(temp);
		}
	}


	
	static bool FIVector4::intersectInt(FIVector4 *aMin, FIVector4 *aMax, FIVector4 *bMin, FIVector4 *bMax) {
		
		if (aMax->getFX() <= bMin->getFX()) return false;
		if (aMin->getFX() >= bMax->getFX()) return false;
		if (aMax->getFY() <= bMin->getFY()) return false;
		if (aMin->getFY() >= bMax->getFY()) return false;
		if (aMax->getFZ() <= bMin->getFZ()) return false;
		if (aMin->getFZ() >= bMax->getFZ()) return false;
		
		return true;
		
	}
	
	

	static bool FIVector4::intersect(FIVector4 *aMin, FIVector4 *aMax, FIVector4 *bMin, FIVector4 *bMax) {

		float aWidth = aMax->getFX() - aMin->getFX();
		float aHeight = aMax->getFY() - aMin->getFY();
		float aDepth = aMax->getFZ() - aMin->getFZ();

		float bWidth = bMax->getFX() - bMin->getFX();
		float bHeight = bMax->getFY() - bMin->getFY();
		float bDepth = bMax->getFZ() - bMin->getFZ();

		float aWidthC = (aMax->getFX() + aMin->getFX()) / 2.0f;
		float aHeightC = (aMax->getFY() + aMin->getFY()) / 2.0f;
		float aDepthC = (aMax->getFZ() + aMin->getFZ()) / 2.0f;

		float bWidthC = (bMax->getFX() + bMin->getFX()) / 2.0f;
		float bHeightC = (bMax->getFY() + bMin->getFY()) / 2.0f;
		float bDepthC = (bMax->getFZ() + bMin->getFZ()) / 2.0f;



		return  (abs(aWidthC - bWidthC) * 2.0f <= (aWidth + bWidth)) &&
						(abs(aHeightC - bHeightC) * 2.0f <= (aHeight + bHeight)) &&
						(abs(aDepthC - bDepthC) * 2.0f <= (aDepth + bDepth));
	}

	static void FIVector4::growBoundary(FIVector4 *minB, FIVector4 *maxB, FIVector4 *minV, FIVector4 *maxV) {
		if (minB->getFX() > minV->getFX()) {
			minB->setFX(minV->getFX());
		}
		if (minB->getFY() > minV->getFY()) {
			minB->setFY(minV->getFY());
		}
		if (minB->getFZ() > minV->getFZ()) {
			minB->setFZ(minV->getFZ());
		}
		if (maxB->getFX() < maxV->getFX()) {
			maxB->setFX(maxV->getFX());
		}
		if (maxB->getFY() < maxV->getFY()) {
			maxB->setFY(maxV->getFY());
		}
		if (maxB->getFZ() < maxV->getFZ()) {
			maxB->setFZ(maxV->getFZ());
		}

	}
	
	bool FIVector4::anyXYZ() {
		return (
			(fv4.x != 0.0f) ||
			(fv4.y != 0.0f) ||
			(fv4.z != 0.0f)
		);
	}
	
	bool FIVector4::any() {
		return (
			(fv4.x != 0.0f) ||
			(fv4.y != 0.0f) ||
			(fv4.z != 0.0f) ||
			(fv4.w != 0.0f)
		);
	}
	bool FIVector4::all() {
		return (
			(fv4.x != 0.0f) &&
			(fv4.y != 0.0f) &&
			(fv4.z != 0.0f) &&
			(fv4.w != 0.0f)
		);
	}


	bool FIVector4::inBoundsXYZ(FIVector4 *minV, FIVector4 *maxV) {
		if (fv4.x < minV->getFX()) {
			return false;
		}
		if (fv4.y < minV->getFY()) {
			return false;
		}
		if (fv4.z < minV->getFZ()) {
			return false;
		}
		if (fv4.x >= maxV->getFX()) {
			return false;
		}
		if (fv4.y >= maxV->getFY()) {
			return false;
		}
		if (fv4.z >= maxV->getFZ()) {
			return false;
		}

		return true;
	}
	
	bool FIVector4::inBoundsXYZSlack(FIVector4 *minV, FIVector4 *maxV, float slack) {
		if (fv4.x < minV->getFX()-slack) {
			return false;
		}
		if (fv4.y < minV->getFY()-slack) {
			return false;
		}
		if (fv4.z < minV->getFZ()-slack) {
			return false;
		}
		if (fv4.x >= maxV->getFX()+slack) {
			return false;
		}
		if (fv4.y >= maxV->getFY()+slack) {
			return false;
		}
		if (fv4.z >= maxV->getFZ()+slack) {
			return false;
		}

		return true;
	}





	bool FIVector4::inBoundsXY(FIVector4 *minV, FIVector4 *maxV) {
		if (fv4.x < minV->getFX()) {
			return false;
		}
		if (fv4.y < minV->getFY()) {
			return false;
		}
		if (fv4.x > maxV->getFX()) {
			return false;
		}
		if (fv4.y > maxV->getFY()) {
			return false;
		}

		return true;
	}

	bool FIVector4::iNotEqual(FIVector4 *otherVec) {
		return (
			(iv4.x != otherVec->getIX()) ||
			(iv4.y != otherVec->getIY()) ||
			(iv4.z != otherVec->getIZ()) ||
			(iv4.w != otherVec->getIW())
		);
	}
	
	bool FIVector4::fNotEqual4(FIVector4 *otherVec) {
		return (
			(fv4.x != otherVec->getFX()) ||
			(fv4.y != otherVec->getFY()) ||
			(fv4.z != otherVec->getFZ()) ||
			(fv4.w != otherVec->getFW())
		);
	}



	float FIVector4::wrapDistance(FIVector4 *otherVec, int maxPitch, bool doSet = true) {

		int i;
		int j;

		int bestI;
		int bestJ;

		float shortestDis = FLT_MAX;
		float curDis;

		for (i = -1; i <= 1; i++) {
			for (j = -1; j <= 1; j++) {

				otherVec->addXYZ(i * maxPitch, j * maxPitch, 0, 1.0f);
				curDis = distance(otherVec);
				otherVec->addXYZ(i * maxPitch, j * maxPitch, 0, -1.0f);

				if (curDis < shortestDis) {
					shortestDis = curDis;
					bestI = i;
					bestJ = j;
				}

			}
		}

		if (doSet) {
			otherVec->addXYZ(bestI * maxPitch, bestJ * maxPitch, 0);
		}


		return shortestDis;

	}

	/*

	var raw_dx = Math.abs(x2 - x1);
	var raw_dy = Math.abs(y2 - y1);

	var dx = (raw_dx < (xmax / 2)) ? raw_dx : xmax - raw_dx;
	var dy = (raw_dy < (ymax / 2)) ? raw_dy : ymax - raw_dy;

	var l2dist = Math.sqrt((dx * dx) + (dy * dy));

	*/

	void FIVector4::rotate(float a, int plane) {

		float xp = fv4.x;
		float yp = fv4.y;
		float zp = fv4.z;

		switch (plane) {
		case E_PLANE_XY:
			xp = fv4.x * cos(a) - fv4.y * sin(a);
			yp = fv4.y * cos(a) + fv4.x * sin(a);
			break;

		case E_PLANE_YZ:
			zp = fv4.z * cos(a) - fv4.y * sin(a);
			yp = fv4.y * cos(a) + fv4.z * sin(a);
			break;

		case E_PLANE_XZ:
			xp = fv4.x * cos(a) - fv4.z * sin(a);
			zp = fv4.z * cos(a) + fv4.x * sin(a);
			break;
		}

		fv4.x = xp;
		fv4.y = yp;
		fv4.z = zp;

		iv4.x = (int)fv4.x;
		iv4.y = (int)fv4.y;
		iv4.z = (int)fv4.z;

	}

	void FIVector4::rotate90(int ind) {
		switch (ind) {
		case 0: // 0 deg

			break;

		case 1: // 90 deg
			setFXY(-fv4.y, fv4.x);
			break;

		case 2: // 180 deg
			setFXY(-fv4.x, -fv4.y);
			break;

		case 3: // 270 deg
			setFXY(fv4.y, -fv4.x);
			break;
		}
	}
	
	float FIVector4::distanceXY(FIVector4 *otherVec) {

		float dx = fv4.x - otherVec->getFX();
		float dy = fv4.y - otherVec->getFY();

		return sqrt(dx * dx + dy * dy);
	}

	float FIVector4::distance(FIVector4 *otherVec) {

		float dx = fv4.x - otherVec->getFX();
		float dy = fv4.y - otherVec->getFY();
		float dz = fv4.z - otherVec->getFZ();

		return sqrt(dx * dx + dy * dy + dz * dz);
	}
	
	

	float FIVector4::length() {
		return sqrt(fv4.x * fv4.x + fv4.y * fv4.y + fv4.z * fv4.z);
	}
	float FIVector4::lengthXY() {
		return sqrt(fv4.x * fv4.x + fv4.y * fv4.y);
	}

	float FIVector4::manhattanDis(FIVector4 *otherVec) {

		float dx = abs(fv4.x - otherVec->getFX());
		float dy = abs(fv4.y - otherVec->getFY());
		float dz = abs(fv4.z - otherVec->getFZ());

		return max(max(dx, dy), dz);
	}

	void FIVector4::normalize() {
		float len = sqrt(fv4.x * fv4.x + fv4.y * fv4.y + fv4.z * fv4.z);

		fv4.x = fv4.x / len;
		fv4.y = fv4.y / len;
		fv4.z = fv4.z / len;

	}
	
	void FIVector4::normalizeXY() {
		float len = sqrt(fv4.x * fv4.x + fv4.y * fv4.y);
		
		fv4.x = fv4.x / len;
		fv4.y = fv4.y / len;
	}
	

	float FIVector4::dot(FIVector4 *otherVec) {

		return fv4.x * otherVec->getFX() +
					 fv4.y * otherVec->getFY() +
					 fv4.z * otherVec->getFZ();
	}

	static void FIVector4::cross(FIVector4 *outVec, FIVector4 *v1, FIVector4 *v2) {


		float x1 = v1->getFX();
		float y1 = v1->getFY();
		float z1 = v1->getFZ();

		float x2 = v2->getFX();
		float y2 = v2->getFY();
		float z2 = v2->getFZ();

		outVec->setFXYZ(
			(y1 * z2) - (y2 * z1),
			(z1 * x2) - (z2 * x1),
			(x1 * y2) - (x2 * y1)
		);

	}




	iVector4 *FIVector4::getIXYZW() {
		return &iv4;
	}
	fVector4 *FIVector4::getFXYZW() {
		return &fv4;
	}

	iVector3 *FIVector4::getIXYZ() {
		iv3.x = iv4.x;
		iv3.y = iv4.y;
		iv3.z = iv4.z;
		return &iv3;
	}
	fVector3 *FIVector4::getFXYZ() {
		fv3.x = fv4.x;
		fv3.y = fv4.y;
		fv3.z = fv4.z;
		return &fv3;
	}

	iVector2 *FIVector4::getIXY() {
		iv2.x = iv4.x;
		iv2.y = iv4.y;
		return &iv2;
	}
	fVector2 *FIVector4::getFXY() {
		fv2.x = fv4.x;
		fv2.y = fv4.y;
		return &fv2;
	}

	int FIVector4::getIX() {
		return iv4.x;
	}
	int FIVector4::getIY() {
		return iv4.y;
	}
	int FIVector4::getIZ() {
		return iv4.z;
	}
	int FIVector4::getIW() {
		return iv4.w;
	}

	float FIVector4::getFX() {
		return fv4.x;
	}
	float FIVector4::getFY() {
		return fv4.y;
	}
	float FIVector4::getFZ() {
		return fv4.z;
	}
	float FIVector4::getFW() {
		return fv4.w;
	}


void hsv2rgb(materialNode* matNode) {
	
	
	static FIVector4 K_HSV;
	static FIVector4 P_HSV;
	static FIVector4 C_HSV;
	static FIVector4 R_HSV;
	
	K_HSV.setFXYZW(1.0f, 2.0f / 3.0f, 1.0f / 3.0f, 3.0f);
	
	C_HSV.setFXYZ(matNode->h,matNode->s,matNode->l);
	
	
	P_HSV.setFXYZ(
		abs(fract(C_HSV[0] + K_HSV[0]) * 6.0f - K_HSV[3]),
		abs(fract(C_HSV[0] + K_HSV[1]) * 6.0f - K_HSV[3]),
		abs(fract(C_HSV[0] + K_HSV[2]) * 6.0f - K_HSV[3])
	);
	R_HSV.setFXYZ(
		C_HSV[2] * mixf(K_HSV[0], clampf(P_HSV[0] - K_HSV[0], 0.0f, 1.0f), C_HSV[1]),
		C_HSV[2] * mixf(K_HSV[0], clampf(P_HSV[1] - K_HSV[0], 0.0f, 1.0f), C_HSV[1]),
		C_HSV[2] * mixf(K_HSV[0], clampf(P_HSV[2] - K_HSV[0], 0.0f, 1.0f), C_HSV[1])	
	);
	
	matNode->r = R_HSV[0];
	matNode->g = R_HSV[1];
	matNode->b = R_HSV[2];
	
}


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



float getRandSeededPos(float xv, float yv, float zv) {
	float intPart;		
	return abs ( modf(sin(xv*433.2 + yv*522.9 + zv*839.4) * 43758.8563f, &intPart) );
}


float getRandSeeded(FIVector4 *seedPos, FIVector4 *seedVals) {
	float intPart;		
	return abs ( modf(sin(seedPos->dot(seedVals)) * 43758.8563f, &intPart) );
}

int iGetRandSeeded(
	FIVector4 *seedPos,
	FIVector4 *seedVals,
	int minV,
	int maxV
) {
	float intPart = 0.0f;		
	int res = abs( modf(sin(seedPos->dot(seedVals)) * 43758.8563f, &intPart) )*1000000.0f;
	res = (res % (maxV + 1 - minV)) + minV;
	
	return res;
}




	void AxisRotation::doRotationOr(FIVector4 *output, FIVector4 *input, int orientationOffset)
	{
		int i;
		int j;
		int k;

		outputMatrix[0] = 0.0f;
		outputMatrix[1] = 0.0f;
		outputMatrix[2] = 0.0f;
		outputMatrix[3] = 0.0f;

		inputMatrix[0] = input->getFX();
		inputMatrix[1] = input->getFY();
		inputMatrix[2] = input->getFZ();
		inputMatrix[3] = 1.0;


		for (i = 0; i < 4; i++ ) {
			for (j = 0; j < 1; j++) {
				outputMatrix[i] = 0;
				for (k = 0; k < 4; k++) {
					outputMatrix[i] += ALL_ROT[orientationOffset+i*4+k] * inputMatrix[k];
				}
			}
		}

		output->setFXYZW(
			outputMatrix[0],
			outputMatrix[1],
			outputMatrix[2],
			outputMatrix[3]
		);

	}

	void AxisRotation::doRotation(FIVector4 *output, FIVector4 *input, FIVector4 *axis, float angle)
	{
		int i;
		int j;
		int k;

		float u = axis->getFX();
		float v = axis->getFY();
		float w = axis->getFZ();

		float L = (u * u + v * v + w * w);
		float u2 = u * u;
		float v2 = v * v;
		float w2 = w * w;

		float sqrtL = sqrt(L);
		float ca = cos(angle);
		float sa = sin(angle);

		rotationMatrix[0] = (u2 + (v2 + w2) * ca) / L;
		rotationMatrix[1] = (u * v * (1 - ca) - w * sqrtL * sa) / L;
		rotationMatrix[2] = (u * w * (1 - ca) + v * sqrtL * sa) / L;
		rotationMatrix[3] = 0.0f;

		rotationMatrix[4] = (u * v * (1 - ca) + w * sqrtL * sa) / L;
		rotationMatrix[5] = (v2 + (u2 + w2) * ca) / L;
		rotationMatrix[6] = (v * w * (1 - ca) - u * sqrtL * sa) / L;
		rotationMatrix[7] = 0.0f;

		rotationMatrix[8] = (u * w * (1 - ca) - v * sqrtL * sa) / L;
		rotationMatrix[9] = (v * w * (1 - ca) + u * sqrtL * sa) / L;
		rotationMatrix[10] = (w2 + (u2 + v2) * ca) / L;
		rotationMatrix[11] = 0.0f;

		rotationMatrix[12] = 0.0f;
		rotationMatrix[13] = 0.0f;
		rotationMatrix[14] = 0.0f;
		rotationMatrix[15] = 1.0f;



		outputMatrix[0] = 0.0f;
		outputMatrix[1] = 0.0f;
		outputMatrix[2] = 0.0f;
		outputMatrix[3] = 0.0f;

		inputMatrix[0] = input->getFX();
		inputMatrix[1] = input->getFY();
		inputMatrix[2] = input->getFZ();
		inputMatrix[3] = 1.0;


		for (i = 0; i < 4; i++ ) {
			for (j = 0; j < 1; j++) {
				outputMatrix[i] = 0;
				for (k = 0; k < 4; k++) {
					outputMatrix[i] += rotationMatrix[i*4+k] * inputMatrix[k];
				}
			}
		}

		output->setFXYZW(
			outputMatrix[0],
			outputMatrix[1],
			outputMatrix[2],
			outputMatrix[3]
		);

	}
	
	
	
	void AxisRotation::buildRotMatrix(
		RotationInfo* rotInfo
	) {
		
		float u = rotInfo->axisAngle.getFX();
		float v = rotInfo->axisAngle.getFY();
		float w = rotInfo->axisAngle.getFZ();
		float angle = rotInfo->axisAngle.getFW();

		float L = (u * u + v * v + w * w);
		float u2 = u * u;
		float v2 = v * v;
		float w2 = w * w;

		float sqrtL = sqrt(L);
		float ca = cos(angle);
		float sa = sin(angle);

		rotInfo->rotMatrix[0] = (u2 + (v2 + w2) * ca) / L;
		rotInfo->rotMatrix[1] = (u * v * (1 - ca) - w * sqrtL * sa) / L;
		rotInfo->rotMatrix[2] = (u * w * (1 - ca) + v * sqrtL * sa) / L;
		rotInfo->rotMatrix[3] = 0.0f;

		rotInfo->rotMatrix[4] = (u * v * (1 - ca) + w * sqrtL * sa) / L;
		rotInfo->rotMatrix[5] = (v2 + (u2 + w2) * ca) / L;
		rotInfo->rotMatrix[6] = (v * w * (1 - ca) - u * sqrtL * sa) / L;
		rotInfo->rotMatrix[7] = 0.0f;

		rotInfo->rotMatrix[8] = (u * w * (1 - ca) - v * sqrtL * sa) / L;
		rotInfo->rotMatrix[9] = (v * w * (1 - ca) + u * sqrtL * sa) / L;
		rotInfo->rotMatrix[10] = (w2 + (u2 + v2) * ca) / L;
		rotInfo->rotMatrix[11] = 0.0f;

		rotInfo->rotMatrix[12] = 0.0f;
		rotInfo->rotMatrix[13] = 0.0f;
		rotInfo->rotMatrix[14] = 0.0f;
		rotInfo->rotMatrix[15] = 1.0f;

	}
	
	
	void AxisRotation::applyRotation(
		FIVector4 *output,
		FIVector4 *input,
		RotationInfo* rotInfo
	) {
		int i;
		int j;
		int k;
		int m;
		
		//tempRes3.copyFrom(baseOffset);
		//tempRes3.addXYZRef(parentOffset,-1.0f);

		float u = rotInfo->axisAngle.getFX();
		float v = rotInfo->axisAngle.getFY();
		float w = rotInfo->axisAngle.getFZ();
		float angle = rotInfo->axisAngle.getFW();

		float L = (u * u + v * v + w * w);
		float u2 = u * u;
		float v2 = v * v;
		float w2 = w * w;

		float sqrtL = sqrt(L);
		float ca = cos(angle);
		float sa = sin(angle);

		


		for (m = 0; m < 3; m++) {
			outputMatrix[0] = 0.0f;
			outputMatrix[1] = 0.0f;
			outputMatrix[2] = 0.0f;
			outputMatrix[3] = 0.0f;

			inputMatrix[0] = input[m].getFX();// - (rotInfo->basePoint[0]);
			inputMatrix[1] = input[m].getFY();// - (rotInfo->basePoint[1]);
			inputMatrix[2] = input[m].getFZ();// - (rotInfo->basePoint[2]);
			inputMatrix[3] = 1.0;


			for (i = 0; i < 4; i++ ) {
				for (j = 0; j < 1; j++) {
					outputMatrix[i] = 0;
					for (k = 0; k < 4; k++) {
						outputMatrix[i] += rotInfo->rotMatrix[i*4+k] * inputMatrix[k];
					}
				}
			}

			output[m].setFXYZW(
				outputMatrix[0],// + (rotInfo->basePoint[0]),
				outputMatrix[1],// + (rotInfo->basePoint[1]),
				outputMatrix[2],// + (rotInfo->basePoint[2]),
				outputMatrix[3]
			);
			//output[m].normalize();
		}

		

	}
	
	
	void AxisRotation::doRotationTBN(
		FIVector4 *output,
		FIVector4 *input,
		FIVector4 *axisAngle,
		FIVector4 *parentOffset,
		FIVector4 *baseOffset
	) {
		int i;
		int j;
		int k;
		int m;
		
		tempRes3.copyFrom(baseOffset);
		tempRes3.addXYZRef(parentOffset,-1.0f);

		float u = axisAngle->getFX();
		float v = axisAngle->getFY();
		float w = axisAngle->getFZ();
		float angle = axisAngle->getFW();

		float L = (u * u + v * v + w * w);
		float u2 = u * u;
		float v2 = v * v;
		float w2 = w * w;

		float sqrtL = sqrt(L);
		float ca = cos(angle);
		float sa = sin(angle);

		rotationMatrix[0] = (u2 + (v2 + w2) * ca) / L;
		rotationMatrix[1] = (u * v * (1 - ca) - w * sqrtL * sa) / L;
		rotationMatrix[2] = (u * w * (1 - ca) + v * sqrtL * sa) / L;
		rotationMatrix[3] = 0.0f;

		rotationMatrix[4] = (u * v * (1 - ca) + w * sqrtL * sa) / L;
		rotationMatrix[5] = (v2 + (u2 + w2) * ca) / L;
		rotationMatrix[6] = (v * w * (1 - ca) - u * sqrtL * sa) / L;
		rotationMatrix[7] = 0.0f;

		rotationMatrix[8] = (u * w * (1 - ca) - v * sqrtL * sa) / L;
		rotationMatrix[9] = (v * w * (1 - ca) + u * sqrtL * sa) / L;
		rotationMatrix[10] = (w2 + (u2 + v2) * ca) / L;
		rotationMatrix[11] = 0.0f;

		rotationMatrix[12] = 0.0f;
		rotationMatrix[13] = 0.0f;
		rotationMatrix[14] = 0.0f;
		rotationMatrix[15] = 1.0f;



		for (m = 0; m < 3; m++) {
			outputMatrix[0] = 0.0f;
			outputMatrix[1] = 0.0f;
			outputMatrix[2] = 0.0f;
			outputMatrix[3] = 0.0f;

			inputMatrix[0] = input[m].getFX() + (tempRes3[0]);
			inputMatrix[1] = input[m].getFY() + (tempRes3[1]);
			inputMatrix[2] = input[m].getFZ() + (tempRes3[2]);
			inputMatrix[3] = 1.0;


			for (i = 0; i < 4; i++ ) {
				for (j = 0; j < 1; j++) {
					outputMatrix[i] = 0;
					for (k = 0; k < 4; k++) {
						outputMatrix[i] += rotationMatrix[i*4+k] * inputMatrix[k];
					}
				}
			}

			output[m].setFXYZW(
				outputMatrix[0] - (tempRes3[0]),
				outputMatrix[1] - (tempRes3[1]),
				outputMatrix[2] - (tempRes3[2]),
				outputMatrix[3]
			);
			output[m].normalize();
		}

		

	}







void safeNorm(btVector3 &normRef) {
	if (normRef.fuzzyZero()) {
		
	}
	else {
		normRef.normalize();
	}
}

float getShortestAngle(float begInRad, float endInRad, float amount) {
	int begInDeg = begInRad*180/M_PI;
	int endInDeg = endInRad*180/M_PI;
	
	float shortest_angle = ((((endInDeg - begInDeg) % 360) + 540) % 360) - 180;
	
	return shortest_angle * amount * M_PI / 180.0f;
}

btVector3 roundBTV(btVector3 v) {
	return btVector3(
		roundVal(v.getX()),
		roundVal(v.getY()),
		roundVal(v.getZ())	
	);
}
btVector3 floorBTV(btVector3 v) {
	return btVector3(
		floor(v.getX()),
		floor(v.getY()),
		floor(v.getZ())	
	);
}

btVector3 multByOtherRot( btVector3 imp, btMatrix3x3 otherRot) {
	// Vector3 myRHS = Vector3(imp.getX(),imp.getY(),imp.getZ());
	// Vector3 res = otherRot*myRHS;
	
	// return btVector3(res.x,res.y,res.z);
	
	return otherRot*imp;
}

btVector3 rotBTV2D(btVector3 source, float ang) {
	float baseAng = atan2(source.getY(),source.getX());
	baseAng += ang + M_PI/2.0f;
	
	

	return -btVector3(cos(baseAng),sin(baseAng),0.0f);
}



	
	bool BaseObj::hasAtLeast(int status, int val) {
		return (statSheet.curStatus[status] >= val);
	}
	
	void BaseObj::modifyStatus(int status, int modVal) {
		statSheet.curStatus[status] += modVal;
		
		
		if (statSheet.curStatus[status] < 0) {
			statSheet.curStatus[status] = 0;
		}
		if (statSheet.curStatus[status] > statSheet.maxStatus[status]) {
			statSheet.curStatus[status] = statSheet.maxStatus[status];
		}
		
	}
	
	btVector3 BaseObj::getUnitBounds(bool getMax) {
		
		if (bodies.size() < 1) {
			cout << "ERROR: getUnitBounds() with no bodies\n";
		}
		
		btVector3 cp = getCenterPoint( E_BDG_CENTER );
		
		float diamXY = 2.0f;
		float diamZ = 4.0f;
		
		btVector3 newRad = btVector3(diamXY*0.5f,diamXY*0.5f,diamZ*0.5f);
		
		
		switch (entType) {
			case E_ENTTYPE_NPC:
				
			break;
			default:
			
			break;
		}
		
		cp = btVector3(floor(cp.getX()),floor(cp.getY()),cp.getZ());
		
		if (getMax) {
			cp += btVector3(1.0f,1.0f,newRad.getZ());
		}
		else {
			cp -= btVector3(0.0f,0.0f,newRad.getZ());
		}
		
		return cp;
		
	}
	
	
	
	
	bool BaseObj::holdingWeapon(int handNum) {
		
		if (handNum == -1) {
			return (
				( isGrabbingId[RLBN_LEFT] > -1 ) ||
				( isGrabbingId[RLBN_RIGT] > -1 )
			);
		}
		else {
			return ( isGrabbingId[handNum] > -1 );
		}
		
		
	}
	
	
	void BaseObj::setDamping(float linear, float angular) {
		int i;
		
		for (i = 0; i < bodies.size(); i++) {
			bodies[i].body->setDamping(linear,angular);
		}
	}
	
	void BaseObj::clearAABB(btVector3* aabbMin, btVector3* aabbMax) {
		*aabbMin = btVector3(FLT_MAX,FLT_MAX,FLT_MAX);
		*aabbMax = btVector3(FLT_MIN,FLT_MIN,FLT_MIN);	
	}
	
	void BaseObj::addAABBPoint(btVector3* aabbMin, btVector3* aabbMax, btVector3 newPoint) {
		btVector3 tempv;
		
		tempv = btVector3(
			max(newPoint.getX(), aabbMax->getX()),
			max(newPoint.getY(), aabbMax->getY()),
			max(newPoint.getZ(), aabbMax->getZ())
		);
		
		*aabbMax = tempv;
		
		tempv = btVector3(
			min(newPoint.getX(), aabbMin->getX()),
			min(newPoint.getY(), aabbMin->getY()),
			min(newPoint.getZ(), aabbMin->getZ())
		);
		
		*aabbMin = tempv;
	}
	
	
	bool BaseObj::hasBodies() {
		return (bodies.size() > 0);
	}
	
	void BaseObj::multVel(int i, btVector3 velMod) {
		
		bodies[i].body->setLinearVelocity(
			bodies[i].body->getLinearVelocity() * velMod
		);
	}
	
	void BaseObj::multVelAng(int i, btVector3 velMod) {
		
		bodies[i].body->setAngularVelocity(
			bodies[i].body->getAngularVelocity() * velMod
		);
	}
	
	void BaseObj::addVel(int i, btVector3 velMod) {
		
		bodies[i].body->setLinearVelocity(
			bodies[i].body->getLinearVelocity() + velMod
		);
	}
	
	FIVector4* BaseObj::getVel(int i) {
		
		if (i < bodies.size()) {
			
			linVelocity.setBTV( bodies[i].body->getLinearVelocity() );
		}
		
		
		return &linVelocity;
	}
	
	BodyStruct* BaseObj::getBodyByBoneId(int id) {
		int i;
		
		for (i = 0; i < bodies.size(); i++) {
			if (bodies[i].boneId == id) {
				return &(bodies[i]);
			}
		}
		
		return NULL;
	}
	
	float BaseObj::getTotalMass() {
		int i;
		
		float tot = 0.0f;
		
		for (i = 0; i < bodies.size(); i++) {
			tot += bodies[i].mass;
		}
		
		return tot;
	}
	
	float BaseObj::getMarkerMass() {
		return bodies[E_BDG_CENTER].mass;
	}
	
	
	
	void BaseObj::wakeAll() {
		// int i;
		
		// for (i = 0; i < bodies.size(); i++) {
		// 	bodies[i].body->setActivationState(ACTIVE_TAG);
		// }
	}
	
	
	
	bool BaseObj::allFalling() {
		int i;
		
		for (i = 0; i < bodies.size(); i++) {
			if (bodies[i].hasContact) {
				return false;
			}
		}
		
		return true;
	}
	
	bool baseContact() {
		return bodies[E_BDG_CENTER].hasContact;
	}
	
	float BaseObj::getPlanarVel() {
		
		if (bodies.size() < 1) {
			return 0.0f;
		}
		
		btVector3 sourceVel = bodies[E_BDG_CENTER].body->getLinearVelocity();
		
		return sqrt( sourceVel.getX()*sourceVel.getX() + sourceVel.getY()*sourceVel.getY() );
	}
	
	void BaseObj::setLinVel(btVector3 newVel, int i) {
		if (i < bodies.size()) {
			bodies[i].body->setLinearVelocity(newVel);
			bodies[i].body->setActivationState(ACTIVE_TAG);
		}
	}
	
	void BaseObj::applyImpulses(float timeDelta, int i) {
		
		if (i < bodies.size()) {
			if (bodies[i].totAV.isZero()&&bodies[i].totLV.isZero()) {
				
			}
			else {
				
				// if (zeroZ) {
				// 	bodies[i].body->setLinearVelocity(btVector3(
				// 		bodies[i].body->getLinearVelocity().getX(),
				// 		bodies[i].body->getLinearVelocity().getY(),
				// 		0.0f	
				// 	));
				// }
				
				bodies[i].body->setAngularVelocity(bodies[i].body->getAngularVelocity() + bodies[i].totAV*timeDelta);
				bodies[i].body->applyCentralImpulse(bodies[i].totLV*timeDelta);
				bodies[i].body->setActivationState(ACTIVE_TAG);
			
			}
		}
		
	}
	
	
	
	// void begSwing() {
	// 	targWeaponStack.push_back(E_WEAPON_STATE_BEG);
	// }
	// void endSwing() {
	// 	targWeaponStack.push_back(E_WEAPON_STATE_END);
	// 	targWeaponStack.push_back(E_WEAPON_STATE_IDLE);
	// 	//targWeaponStack.push_back(E_WEAPON_POS_RELAXED);
	// }
	
	// int getStackElem(int n) {
	// 	std::list<int>::iterator ptr;
	// 	int i;

	// 	for( i = 0 , ptr = targWeaponStack.begin() ; i < n && ptr != targWeaponStack.end() ; i++ , ptr++ );

	// 	if( ptr == targWeaponStack.end() ) {
	// 	    // list too short 
	// 	    return -1;
	// 	}
	// 	else {
	// 	    // 'ptr' points to n-th element of list
	// 	    return *ptr;
	// 	}
	// }
	
	
	btVector3 BaseObj::getWeaponPos(int curPos) {
		
		float newLR = 0.0f;
		float newUD = 0.0f;
		
		switch (curPos) {
			case E_WEAPON_POS_RELAXED:
				newLR = 0.5f;
				newUD = 0.25f;
			break;
			case E_WEAPON_POS_LEFT:
				newLR = 0.0f;
				newUD = 0.5f;
			break;
			case E_WEAPON_POS_RIGHT:
				newLR = 1.0f;
				newUD = 0.5f;
			break;
			case E_WEAPON_POS_UP:
				newLR = 0.5f;
				newUD = 0.0f;
			break;
			case E_WEAPON_POS_DOWN:
				newLR = 0.5f;
				newUD = 1.0f;
			break;
			
			case E_WEAPON_POS_UP_LEFT:
				newLR = 0.0f;
				newUD = 0.25f;
			break;
			case E_WEAPON_POS_UP_RIGHT:
				newLR = 1.0f;
				newUD = 0.25f;
			break;
			case E_WEAPON_POS_DOWN_LEFT:
				newLR = 0.0f;
				newUD = 0.75f;
			break;
			case E_WEAPON_POS_DOWN_RIGHT:
				newLR = 1.0f;
				newUD = 0.75f;
			break;
			case E_WEAPON_POS_FORWARD:
				newLR = 0.5f;
				newUD = 0.5f;
			break;
			
		}
		
		return btVector3(newLR,newUD,0.0f);
	}
	
	void BaseObj::setGrabbedBy(int newId, int handNum) {
		int i;
		
		isGrabbedByHand = handNum;
		isGrabbedById = newId;
		
		int heldUID = newId;
		
		if (heldUID < 0) {
			heldUID = -3;
		}
		
		for (i = 0; i < bodies.size(); i++) {
			bodies[i].body->heldByUID = heldUID;
		}
	}
	
	// void updateWeaponTargs(double curStepTime) {
		
		
	// 	totWeaponTime += curStepTime;
		
		
	// 	if (totWeaponTime >= 1.0) {
	// 		totWeaponTime = 1.0;
			
	// 		if (targWeaponStack.size() > 1) {
	// 			//targWeaponStack.pop_front();
	// 			targWeaponStack.erase(targWeaponStack.begin() + 0);
	// 			totWeaponTime = 0.0f;
	// 		}
	// 	}
		
	// 	int curStep = E_WEAPON_STATE_IDLE;
	// 	int nextStep = E_WEAPON_STATE_IDLE;
		
	// 	if (targWeaponStack.size() > 0) {
	// 		curStep = targWeaponStack[0];
	// 	}
		
	// 	if (targWeaponStack.size() > 1) {
	// 		nextStep = targWeaponStack[1];
	// 	}
	// 	else {
	// 		nextStep = curStep;
	// 	}
		
		
		
		
		
		
	// 	float lerpTime = totWeaponTime;
		
	// 	btVector3 res0;
	// 	btVector3 res1;
		
		
	// 	if (
	// 		(curStep == E_WEAPON_STATE_IDLE) &&
	// 		(nextStep == E_WEAPON_STATE_IDLE)	
	// 	) {
	// 		res0 = getWeaponPos(E_WEAPON_POS_RELAXED);
	// 		res1 = getWeaponPos(E_WEAPON_POS_RELAXED);
	// 	}
		
	// 	if (
	// 		(curStep == E_WEAPON_STATE_BEG) &&
	// 		(nextStep == E_WEAPON_STATE_BEG)
	// 	) {
	// 		res0 = getWeaponPos(E_WEAPON_POS_UP_LEFT);
	// 		res1 = getWeaponPos(E_WEAPON_POS_UP_LEFT);
	// 	}
		
	// 	if (
	// 		(curStep == E_WEAPON_STATE_IDLE) &&
	// 		(nextStep == E_WEAPON_STATE_BEG)	
	// 	) {
	// 		res0 = getWeaponPos(E_WEAPON_POS_RELAXED);
	// 		res1 = getWeaponPos(E_WEAPON_POS_UP_LEFT);
	// 	}
		
		
	// 	if (
	// 		(curStep == E_WEAPON_STATE_BEG) &&
	// 		(nextStep == E_WEAPON_STATE_END)	
	// 	) {
	// 		if (totWeaponTime < 0.5f) {
	// 			res0 = getWeaponPos(E_WEAPON_POS_UP_LEFT);
	// 			res1 = getWeaponPos(E_WEAPON_POS_FORWARD);
	// 			lerpTime = totWeaponTime*2.0f;
	// 		}
	// 		else {
	// 			res0 = getWeaponPos(E_WEAPON_POS_FORWARD);
	// 			res1 = getWeaponPos(E_WEAPON_POS_RIGHT);
	// 			lerpTime = (totWeaponTime-0.5f)*2.0f;
	// 		}
	// 	}
	
	// 	if (
	// 		(curStep == E_WEAPON_STATE_END) &&
	// 		(nextStep == E_WEAPON_STATE_IDLE)	
	// 	) {
	// 		res0 = getWeaponPos(E_WEAPON_POS_RELAXED);
	// 		res1 = getWeaponPos(E_WEAPON_POS_RIGHT);
	// 	}
		
		
		
	// 	lrBounds = mixf(res0.getX(), res1.getX(), lerpTime);
	// 	udBounds = mixf(res0.getY(), res1.getY(), lerpTime);
		
		
	// }
	
	
	// void updateWeapon(
	// 	int handNum,
	// 	btVector3 weaponBeg,
	// 	btVector3 weaponEnd,
	// 	double curStepTime,
	// 	// float lrBounds,
	// 	// float udBounds,
	// 	float weaponLen
	// ) {
		
	// 	totTime += curStepTime;
		
	// 	//updateWeaponTargs(curStepTime);
		
		
		
		
	// 	float myMat[16];
	// 	Matrix4 myMatrix4;
	// 	Vector3 myVector0;
	// 	Vector3 myVector1;
	// 	Vector3 normVec;
	// 	Vector4 resVector0;
	// 	Vector4 resVector1;
		
	// 	Vector4 vf0;
	// 	Vector4 vf1;
		
	// 	btVector3 basePos;
	// 	float rad0 = 1.0f;
	// 	float rad1 = rad0 + weaponLen;
		
	// 	//float lrBounds = sin(totTime/4.0);
	// 	//float udBounds = sin(totTime);
	// 	//float udBounds2 = udBounds;//sin(totTime/8.0);
		
	// 	if (bodies.size() < 1) {
	// 		return;
	// 	}
		
		
	// 	// float weaponTheta = M_PI_2 + lrBounds*M_PI_8;
	// 	// float weaponPhi = M_PI_4 + udBounds*M_PI_4;
		
	// 	// float weaponTheta2 = (1.0f - lrBounds)*M_PI + cos(totTime/2.0f)*0.1f;
	// 	// float weaponPhi2 = 0 + udBounds*M_PI_2*1.5f + sin(totTime/3.0f)*0.1f;
		
		
	// 	bodies[E_BDG_CENTER].body->getWorldTransform().getOpenGLMatrix(myMat);
	// 	myMatrix4 = Matrix4(myMat);
		
	// 	// myVector0 = Vector3(
	// 	// 	cos(weaponTheta)*sin(weaponPhi)*rad0,
	// 	// 	sin(weaponTheta)*sin(weaponPhi)*rad0 + 0.5f,
	// 	// 	cos(weaponPhi)*rad0 + (1.0f-udBounds2)*0.75f
	// 	// );
	// 	// myVector1 = Vector3(
	// 	// 	cos(weaponTheta2)*sin(weaponPhi2)*rad1,
	// 	// 	sin(weaponTheta2)*sin(weaponPhi2)*rad1 + 0.5f,
	// 	// 	cos(weaponPhi2)*rad1
	// 	// );
		
	// 	// myVector0.x -= (myVector0.x*0.5f + myVector1.x*0.5f)*0.25f;
	// 	// myVector0.y -= (myVector0.y*0.5f + myVector0.y*0.5f)*0.25f;
		
	// 	// myVector0 *= 0.75f;
		
	// 	// myVector0.y += 0.25f;
		
	// 	// //if (myVector1.x > 0.0f) {
	// 	// 	myVector0.x += myVector1.x*0.25f;
	// 	// //}
		
	// 	// myVector1.y += 1.0f-abs(cos(weaponPhi2));
		
	// 	BodyStruct* handBody;
		
	// 	if (handNum == RLBN_LEFT) {
	// 		handBody = getBodyByBoneId(getCorrectedName(E_BONE_L_METACARPALS));
	// 	}
	// 	else {
	// 		handBody = getBodyByBoneId(getCorrectedName(E_BONE_R_METACARPALS));
	// 	}
		
		
		
	// 	btVector3 handCenter = handBody->body->getCenterOfMassPosition();
		
	// 	myVector0 = Vector3(weaponBeg.getX(), weaponBeg.getY(),weaponBeg.getZ());
	// 	myVector1 = Vector3(weaponEnd.getX(), weaponEnd.getY(),weaponEnd.getZ());
		
		
	// 	normVec = myVector1 - myVector0;
	// 	normVec.normalize();
	// 	normVec = normVec*(rad1-rad0);
	// 	myVector1 = myVector0 + normVec;
		
	// 	rightHandTop = true;//(myVector0.x < 0.0f);
		
		
		
	// 	vf0 = Vector4(myVector0.x, myVector0.y, myVector0.z, 1.0f);
	// 	vf1 = Vector4(myVector1.x, myVector1.y, myVector1.z, 1.0f);
		
	// 	resVector0 = myMatrix4*vf0;
	// 	resVector1 = myMatrix4*vf1;
		
	// 	weaponVec0[handNum] = btVector3(resVector0.x,resVector0.y,resVector0.z);
	// 	weaponVec1[handNum] = btVector3(resVector1.x,resVector1.y,resVector1.z);
		
	// 	btVector3 weapDif = handCenter-weaponVec0[handNum];
		
	// 	weaponVec0[handNum] += weapDif;
	// 	weaponVec1[handNum] += weapDif;
		
		
	// 	vf0 = Vector4( 1.0f,0.0f,0.0f,1.0f);
	// 	vf1 = Vector4(-1.0f,0.0f,0.0f,1.0f);
		
	// 	resVector0 = myMatrix4*vf0;
	// 	resVector1 = myMatrix4*vf1;
		
	// 	rightVec = btVector3(resVector0.x,resVector0.y,resVector0.z);
	// 	leftVec = btVector3(resVector1.x,resVector1.y,resVector1.z);
		
		
		
		
		
		
	// }
	
	
	void BaseObj::flushImpulses() {
		
		int i;
		
		for (i = 0; i < bodies.size(); i++) {
			bodies[i].totAV = btVector3(0.0f,0.0f,0.0f);
			bodies[i].totLV = btVector3(0.0f,0.0f,0.0f);
		}
		
		
	}
	
	float BaseObj::turnTowardsTargAng( float targAng ) {
		btVector3 curVec = bodies[E_BDG_CENTER].body->getCenterOfMassTransform().getBasis()*btVector3(0.0f,1.0f,0.0f);
		curVec.setZ(0.0f);
		curVec.normalize();
		
		float curAng = atan2(curVec.getY(),curVec.getX());
		
		return getShortestAngle(curAng,targAng,1.0f);
	}
	
	float BaseObj::turnTowardsPointDelta(btVector3 targPoint) {
		btVector3 centerPoint = getCenterPoint(E_BDG_CENTER);
		
		btVector3 targVec = targPoint-centerPoint;
		targVec.setZ(0.0f);
		targVec.normalize();
		
		float targAng = atan2(targVec.getY(),targVec.getX());
		
		return turnTowardsTargAng(targAng);
		
		
		//return targAng-curAng;
		
		// btQuaternion quat;
		
		// if ( abs(vectorA.dot(vectorB)) == 1.0f ) {
		// 	// todo: handle (anti)parallel case
		// 	//curJoint->pivotAxis = btVector3(0.0f,0.0f,0.0f);
		// 	//curJoint->quat = btQuaternion(btVector3(0.0f,0.0f,1.0f), 0.0f);
		// }
		// else {
		// 	axis = (vectorA.cross(vectorB)).normalized();
		// 	angle = btAcos(vectorA.dot(vectorB)) / (vectorA.length() * vectorB.length());
		// 	quat = btQuaternion(axis, angle);
		// 	transform.setRotation(curJoint->quat);
		// 	//curJoint->pivotAxis = axis;
		// }
	}
	
	void BaseObj::applyAngularImpulse(btVector3 newAV, bool delayed, int i) {
		
		
		if (i < bodies.size()) {
			if (delayed) {
				bodies[i].totAV += newAV;
			}
			else {
				
				bodies[i].body->setAngularVelocity(bodies[i].body->getAngularVelocity() + newAV);
				bodies[i].body->setActivationState(ACTIVE_TAG);
			}

		}
		
	}
	
	void BaseObj::applyImpulse(btVector3 imp, bool delayed, int i) {
		if (i < bodies.size()) {
			
			if (delayed) {
				bodies[i].totLV += imp;
			}
			else {
			
			
				bodies[i].body->applyCentralImpulse(imp);
				bodies[i].body->setActivationState(ACTIVE_TAG);
			}
		}
		
	}
	
	void BaseObj::applyImpulseOtherRot(btVector3 imp, btMatrix3x3 otherRot, bool delayed, int i) {
		
		//Vector3 myRHS = Vector3(imp.getX(),imp.getY(),imp.getZ());
		//Vector3 res = otherRot*myRHS;
		btVector3 newImp = otherRot*imp;//btVector3(res.x,res.y,res.z);
		
		if (i < bodies.size()) {
			if (delayed) {
				bodies[i].totLV += newImp;
			}
			else {
				
				bodies[i].body->applyCentralImpulse(newImp);
				bodies[i].body->setActivationState(ACTIVE_TAG);
			}
		}
		
		
	}
	
	bool BaseObj::isHumanoid() {
		return (
			(entType == E_ENTTYPE_NPC)
		);
	}
	
	// void makeWalk(btVector3 imp, btMatrix3x3 otherRot) {
		
	// 	if (isHumanoid()) {
			
	// 	}
	// 	else {
	// 		return;
	// 	}
		
		
	// 	bool lfDown = bodies[E_BDG_LFOOT].hasContact && 
	// 		(bodies[E_BDG_LFOOT].body->getLinearVelocity().length() < 0.1f);
	// 	bool rfDown = bodies[E_BDG_RFOOT].hasContact && 
	// 		(bodies[E_BDG_RFOOT].body->getLinearVelocity().length() < 0.1f);
		
	// 	int resInd = -1;
		
	// 	if (lfDown && rfDown) {
	// 		leftActive = !leftActive;
	// 		//resInd = E_BDG_LFOOT;
			
	// 		if (leftActive) {
	// 			resInd = E_BDG_LFOOT;
	// 		}
	// 		else {
	// 			resInd = E_BDG_RFOOT;
	// 		}
			
	// 	}
	// 	// else {
	// 	// 	if (lfDown) {
	// 	// 		resInd = E_BDG_LFOOT;
	// 	// 	}
	// 	// 	else {
	// 	// 		if (rfDown) {
	// 	// 			resInd = E_BDG_RFOOT;
	// 	// 		}
	// 	// 	}
	// 	// }
		
	// 	if (resInd == -1) {
	// 		return;
	// 	}
		
		
	// 	btVector3 newImp = otherRot*imp*bodies[resInd].mass;
		
	// 	bodies[resInd].body->applyCentralImpulse(newImp);
	// 	bodies[resInd].body->setActivationState(ACTIVE_TAG);
		
		
	// }
	
	
	
	void BaseObj::moveOffset(btVector3 offset, int ind) {
		btTransform trans;
		
		
		if (ind < bodies.size()) {
			
			trans.setIdentity();
			trans.setOrigin(
				bodies[ind].body->getCenterOfMassPosition() + offset
			);
			bodies[ind].body->setActivationState(ACTIVE_TAG);
			bodies[ind].body->setCenterOfMassTransform(
				trans
			);
			
		}
	}
	
	void BaseObj::moveToPoint(btVector3 newPoint, int ind) {
		btTransform trans;
		
		
		if (ind < bodies.size()) {
			
			trans.setIdentity();
			trans.setOrigin(newPoint);
			bodies[ind].body->setActivationState(ACTIVE_TAG);
			bodies[ind].body->setCenterOfMassTransform(
				trans
			);
			
		}
	}
	
	// void setCenterPoint(FIVector4* newPos) {
		
	// 	centerPoint.copyFrom(newPos);
				
	// }
	
	btVector3 BaseObj::getCenterPoint(int ind) {
		if (
			ind < bodies.size()
		) {
			return bodies[ind].body->getCenterOfMassPosition();
		}
		else {
			return btVector3(0.0f,0.0f,0.0f);
		}
	}
	
	FIVector4* BaseObj::wgetCenterPointFIV(int ind) {
		if (
			ind < bodies.size()
		) {
			centerPoint.setBTV( bodies[ind].body->getCenterOfMassPosition() );
		}
		else {
			centerPoint.setFXYZ(0.0f,0.0f,0.0f);
		}
		
		return &centerPoint;
	}
	
	
    BaseObj::BaseObj() {
		
	}
	
	void BaseObj::removeChild(BaseObjType _uid) {
		int i;
		
		for (i = 0; i < children.size(); i++) {
			if (children[i] == _uid) {
				children.erase(children.begin() + i);
				return;
			}
		}
	}
	
	
	float BaseObj::healthPerc() {
		return ((float)statSheet.curStatus[E_STATUS_HEALTH])/((float)statSheet.maxStatus[E_STATUS_HEALTH]);
	}
	
	bool BaseObj::isDead() {
		return (statSheet.curStatus[E_STATUS_HEALTH] <= 0);
	}
	
	bool BaseObj::isAlive() {
		return (statSheet.curStatus[E_STATUS_HEALTH] > 0);
	}
	
	bool BaseObj::getActionState(int action, int handNum) {
		return actionStates[action*RLBN_LENGTH + handNum];
	}
	
	void BaseObj::setActionState(int action, int handNum, bool newVal) {
		actionStates[action*RLBN_LENGTH + handNum] = newVal;
	}
	
	void BaseObj::clearActionStates() {
		int i;
		
		for (i = 0; i < E_ACT_LENGTH*RLBN_LENGTH; i++) {
			actionStates[i] = false;
		}
		
	}
	
	
	void BaseObj::init(
		BaseObjType _uid,
		BaseObjType _parentUID,
		int _objectType,
		int _entType,
		int _subType,
		FIVector4* cellPos
	) {
		
		int i;
		
		contactCount = 0;
		
		//mass = 10.0f;
		orgId = -1;
		actorId = -1;
		isHidden = false;
		
		maxFrames = 0;
		objectType = _objectType;
		entType = _entType;
		subType = _subType;
		
		behaviorTarget = btVector3(0.0f,0.0f,0.0f);
		npcRepel = btVector3(0.0f,0.0f,0.0f);
		
		isGrabbedByHand = -1;
		isGrabbedById = -1;
		
		lastBlockDis = 0.0f;
		blockCount = 0.0f;
		swingCount = 0.0f;
		bindingPower = 1.0f;
		airCount = 0.0f;
		
		tbDir = 0;
		
		for (i = 0; i < RLBN_LENGTH; i++) {
			isGrabbingId[i] = -1;
			swingType[i] = E_PG_SLSH;
		}
		
		for (i = 0; i < E_CS_LENGTH; i++) {
			statSheet.baseStats[i] = 5;
			statSheet.unapplyedStats[i] = 5;	
		}
		statSheet.availPoints = 10;
		
		for (i = 0; i < E_STATUS_LENGTH; i++) {
			statSheet.curStatus[i] = 10;
			statSheet.maxStatus[i] = 10;
		}
		
		
		zeroZ = false;
		jumpCooldown = 0;
		hitCooldown = 0;
		
		
		clearActionStates();
		
		
		isOpen = false;
		isEquipped = false;
		parentUID = _parentUID;
		uid = _uid;
		
		skelOffset = btVector3(0.0f,0.0f,0.0f);
		startPoint = cellPos->getBTV();
		
		bounciness = 0.0f;
		friction = 0.9;
		windResistance = 0.9;
		
		
		//tbPos = getUnitBounds(false);
		
		targPath.points[0] = btVector3(0.0f,0.0f,0.0f);
		targPath.points[1] = btVector3(0.0f,0.0f,0.0f);
		targPath.searchedForPath = false;
		targPath.didFindPath = false;
		targPath.finalPoints.clear();
		targPath.nextInd = -1;
		
		
	}


    

btQuaternion matToQuat(
	float r11, float r12, float r13,
	float r21, float r22, float r23,
	float r31, float r32, float r33 	
) {
	float q0 = ( r11 + r22 + r33 + 1.0f) / 4.0f;
	float q1 = ( r11 - r22 - r33 + 1.0f) / 4.0f;
	float q2 = (-r11 + r22 - r33 + 1.0f) / 4.0f;
	float q3 = (-r11 - r22 + r33 + 1.0f) / 4.0f;
	if(q0 < 0.0f) q0 = 0.0f;
	if(q1 < 0.0f) q1 = 0.0f;
	if(q2 < 0.0f) q2 = 0.0f;
	if(q3 < 0.0f) q3 = 0.0f;
	q0 = sqrt(q0);
	q1 = sqrt(q1);
	q2 = sqrt(q2);
	q3 = sqrt(q3);
	if(q0 >= q1 && q0 >= q2 && q0 >= q3) {
	    q0 *= +1.0f;
	    q1 *= qSign(r32 - r23);
	    q2 *= qSign(r13 - r31);
	    q3 *= qSign(r21 - r12);
	} else if(q1 >= q0 && q1 >= q2 && q1 >= q3) {
	    q0 *= qSign(r32 - r23);
	    q1 *= +1.0f;
	    q2 *= qSign(r21 + r12);
	    q3 *= qSign(r13 + r31);
	} else if(q2 >= q0 && q2 >= q1 && q2 >= q3) {
	    q0 *= qSign(r13 - r31);
	    q1 *= qSign(r21 + r12);
	    q2 *= +1.0f;
	    q3 *= qSign(r32 + r23);
	} else if(q3 >= q0 && q3 >= q1 && q3 >= q2) {
	    q0 *= qSign(r21 - r12);
	    q1 *= qSign(r31 + r13);
	    q2 *= qSign(r32 + r23);
	    q3 *= +1.0f;
	} else {
	    cout << "Quaternion error\n";
	}
	float r = qNorm(q0, q1, q2, q3);
	q0 /= r;
	q1 /= r;
	q2 /= r;
	q3 /= r;
	
	return btQuaternion(q0,q1,q2,q3);
}

btVector3 quatToEulerXYZ(const btQuaternion &quat) {
	btVector3 euler;
	float w=quat.getW();   float x=quat.getX();   float y=quat.getY();   float z=quat.getZ();
	double sqw = w*w; double sqx = x*x; double sqy = y*y; double sqz = z*z; 
	euler.setZ((atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))));
	euler.setX((atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))));
	euler.setY((asin(-2.0 * (x*z - y*w))));
	
	return euler;
}


void moveToOrientation(
	btRigidBody* myBody,
	btVector3 tanAxis,
	btVector3 bitAxis,
	btVector3 norAxis,
	float kv = 0.5f
) {
	btQuaternion targetOrientation = matToQuat(
		tanAxis.getX(),	bitAxis.getX(), norAxis.getX(),
		tanAxis.getX(),	bitAxis.getX(), norAxis.getX(),
		tanAxis.getX(),	bitAxis.getX(), norAxis.getX()
	);
	btQuaternion currentOrientation = myBody->getOrientation();
	btQuaternion deltaOrientation = targetOrientation * currentOrientation.inverse();
	btVector3 deltaEuler = quatToEulerXYZ(deltaOrientation);
	btVector3 torqueToApply = (-1.0f*kv)*deltaEuler;
	myBody->applyTorque(torqueToApply);
	
}

    ThreadWrapper::ThreadWrapper() {
		
	}
	
	void ThreadWrapper::init() {
		threadRunning = false;
		threadRunningEx = false;
	}
	
	void ThreadWrapper::setRunningLocked(bool val) {
		threadMutex.lock();
		threadRunningEx = val;
		threadMutex.unlock();
	}
	
	bool ThreadWrapper::isReady() {
		bool doProc = false;
		
		if (threadRunning) {
			if (threadMutex.try_lock()) {
				if (threadRunningEx) {
					
				}
				else {
					doProc = true;
				}
				threadMutex.unlock();
			}
		}
		else {
			doProc = true;
		}
		
		return doProc;
	}

// class Q3Rend : public q3Render {
// public:
// 	void SetPenColor( f32 r, f32 g, f32 b, f32 a = 1.0f ) override
// 	{
// 		// Q3_UNUSED( a );

// 		// glColor3f( (float)r, (float)g, (float)b );
// 	}

// 	void SetPenPosition( f32 x, f32 y, f32 z ) override
// 	{
// 		x_ = x, y_ = y, z_ = z;
// 	}

// 	void SetScale( f32 sx, f32 sy, f32 sz ) override
// 	{
// 		// glPointSize( (float)sx );
// 		// sx_ = sx, sy_ = sy, sz_ = sz;
// 	}

// 	void Line( f32 x, f32 y, f32 z ) override
// 	{
// 		// glBegin( GL_LINES );
// 		// glVertex3f( (float)x_, (float)y_, (float)z_ );
// 		// glVertex3f( (float)x, (float)y, (float)z );
// 		// SetPenPosition( x, y, z );
// 		// glEnd( );
// 	}

// 	void Triangle(
// 		f32 x1, f32 y1, f32 z1,
// 		f32 x2, f32 y2, f32 z2,
// 		f32 x3, f32 y3, f32 z3
// 		) override
// 	{
		
// 		//glEnable( GL_LIGHTING );
// 		//glBegin( GL_TRIANGLES );
// 		//glColor4f( 0.2f, 0.4f, 0.7f, 0.5f );
// 		glVertex3f( (float)x1, (float)y1, (float)z1 );
// 		glVertex3f( (float)x2, (float)y2, (float)z2 );
// 		glVertex3f( (float)x3, (float)y3, (float)z3 );
// 		//glEnd( );
// 		//glDisable( GL_LIGHTING );
// 	}

// 	void SetTriNormal( f32 x, f32 y, f32 z ) override
// 	{
// 		glNormal3f( (float)x, (float)y, (float)z );
// 	}

// 	void Point( ) override
// 	{
// 		// glBegin( GL_POINTS );
// 		// glVertex3f( (float)x_, (float)y_, (float)z_ );
// 		// glEnd( );
// 	};

// private:
// 	f32 x_, y_, z_;
// 	f32 sx_, sy_, sz_;
// };

// Q3Rend q3Rend;





// struct vec2 {
// 	float x;
// 	float y;
// 	float z;	
// };

// struct vec3 {
// 	float x;
// 	float y;
// 	float z;	
// };

// struct vec4 {
// 	float x;
// 	float y;
// 	float z;
// 	float w;
// };

// class ShaderSimplex() {
// public:

// 	ShaderSimplex() {
		
// 	}
	
	
// 	//#############################


// 	//
// 	// Description : Array and textureless GLSL 2D/3D/4D simplex 
// 	//               noise functions.
// 	//      Author : Ian McEwan, Ashima Arts.
// 	//  Maintainer : ijm
// 	//     Lastmod : 20110822 (ijm)
// 	//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
// 	//               Distributed under the MIT License. See LICENSE file.
// 	//               https://github.com/ashima/webgl-noise
// 	// 

// 	vec3 mod289(vec3 q) {
// 		vec3 res;
// 		q - floor(q * (1.0 / 289.0)) * 289.0;
// 		return res;
// 	}

// 	vec4 mod289(vec4 q) {
// 		vec4 res;
// 		q - floor(q * (1.0 / 289.0)) * 289.0;
// 		return res;
// 	}

// 	vec4 permute(vec4 q) {
// 		vec4 res;
// 		mod289(((q*34.0)+1.0)*q);
// 		return res;
		
// 	}

// 	vec4 taylorInvSqrt(vec4 q) {
// 		vec4 res;
// 		1.79284291400159 - 0.85373472095314 * q;
// 		return res;
// 	}

// 	float snoise(vec3 v) { 
// 		const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
// 		const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// 		// First corner
// 		vec3 i  = floor(v + dot(v, C.yyy) );
// 		vec3 x0 =   v - i + dot(i, C.xxx) ;

// 		// Other corners
// 		vec3 g = step(x0.yzx, x0.xyz);
// 		vec3 l = 1.0 - g;
// 		vec3 i1 = min( g.xyz, l.zxy );
// 		vec3 i2 = max( g.xyz, l.zxy );

// 		//   x0 = x0 - 0.0 + 0.0 * C.xxx;
// 		//   x1 = x0 - i1  + 1.0 * C.xxx;
// 		//   x2 = x0 - i2  + 2.0 * C.xxx;
// 		//   x3 = x0 - 1.0 + 3.0 * C.xxx;
// 		vec3 x1 = x0 - i1 + C.xxx;
// 		vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
// 		vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// 		// Permutations
// 		i = mod289(i); 
// 		vec4 p = permute( permute( permute( 
// 		         i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
// 		       + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
// 		       + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// 		// Gradients: 7x7 points over a square, mapped onto an octahedron.
// 		// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
// 		float n_ = 0.142857142857; // 1.0/7.0
// 		vec3  ns = n_ * D.wyz - D.xzx;

// 		vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

// 		vec4 x_ = floor(j * ns.z);
// 		vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

// 		vec4 x = x_ * ns.x + ns.yyyy;
// 		vec4 y = y_ * ns.x + ns.yyyy;
// 		vec4 h = 1.0 - abs(x) - abs(y);

// 		vec4 b0 = vec4( x.xy, y.xy );
// 		vec4 b1 = vec4( x.zw, y.zw );

// 		//vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
// 		//vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
// 		vec4 s0 = floor(b0)*2.0 + 1.0;
// 		vec4 s1 = floor(b1)*2.0 + 1.0;
// 		vec4 sh = -step(h, vec4(0.0));

// 		vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
// 		vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

// 		vec3 p0 = vec3(a0.xy,h.x);
// 		vec3 p1 = vec3(a0.zw,h.y);
// 		vec3 p2 = vec3(a1.xy,h.z);
// 		vec3 p3 = vec3(a1.zw,h.w);

// 		//Normalise gradients
// 		vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
// 		p0 *= norm.x;
// 		p1 *= norm.y;
// 		p2 *= norm.z;
// 		p3 *= norm.w;

// 		// Mix final noise value
// 		vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
// 		m = m * m;
// 		return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3) ) );
// 	}


// 	//#############################
	
	
// };



// void opSeamlessSimplexNoise(float oct, float pers, float freq) {
	
// 	currentState = S_OP_SEAMLESS_SIMPLEX_NOISE;

// 	int i;

// 	int ix;
// 	int iy;
// 	int iz;

// 	float fx;
// 	float fy;
// 	float fz;

// 	float testVal;

// 	//float oct = 4.0;
// 	//float pers = 0.5;
// 	//float freq =- 1.0/gfTEXTURE_SIZE;

// 	float ts = gfTEXTURE_SIZE;


// 	for (i = 0; i < iTotalUnits; i++) {
// 		ix = i%(iPageSize);
// 		iy = (i%(iPageSize*iPageSize))/iPageSize;
// 		iz = i/(iPageSize*iPageSize);

// 		fx = ((float)ix) + fOffsets.x;
// 		fy = ((float)iy) + fOffsets.y;
// 		fz = ((float)iz) + fOffsets.z;

// 		//float simplexScaledNoise( const float octaves, const float persistence, const float scale, const float loBound, const float hiBound, const float x, const float y, const float z ) {
// 		testVal = 
// 			(
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy, 	fz) * 		(ts-fx) *		(ts-fy) *	(ts-fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy, 	fz) * 		(fx) *			(ts-fy) *	(ts-fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy-ts, 	fz) * 		(ts-fx) *		(fy) *		(ts-fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy-ts, 	fz) * 		(fx) *			(fy) *		(ts-fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy, 	fz-ts) * 	(ts-fx) *		(ts-fy) *	(fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy, 	fz-ts) * 	(fx) *			(ts-fy) *	(fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx, 	fy-ts, 	fz-ts) * 	(ts-fx) *		(fy) *		(fz) +
// 			simplexScaledNoise(oct, pers, freq, 0.0f, 1.0f, fx-ts, 	fy-ts, 	fz-ts) * 	(fx) *			(fy) *		(fz)
// 			)/(ts*ts*ts);

// 		fDataArray[i] = testVal;

		
// 	}

// 	currentState = E_OP_SEAMLESS_SIMPLEX_NOISE;
// }



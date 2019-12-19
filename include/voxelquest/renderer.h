#ifndef _voxelquest_renderer_h_
#define _voxelquest_renderer_h_

#include "voxelquest/vectors.h"
#include "voxelquest/fbos.h"
#include "voxelquest/vbos.h"
#include "voxelquest/tbos.h"
#include "voxelquest/shader.h"
#include "voxelquest/timer.h"

const int NUM_POLY_STRINGS=0;
const std::string polyFBOStrings[]={
    "polyFBO0",
    "polyFBO1",
    "polyFBO2",
    "polyFBO3",
    "polyFBO4"
};

class BaseObj;
class Singleton;

class Renderer
{
public:
    static void init(int width, int height);

    static void setLookAt(const FIVector4 &lookAt);
    static void updateView();

    static void updateProjection();
    static void perspectiveProjection();
    static void orthographicProjection();

    static void setWH(int w, int h);
    static void reshape(int w, int h);

    static void moveCamera(FIVector4 *pModXYZ);
    static void setCameraToElevation();
    static void updateCamVals();
    static void handleMovement();
    
    static GLfloat getCamRot(int ind);
    static FIVector4* cameraGetPos() { return &resultCameraPos; }
    static FIVector4* cameraGetPosNoShake() { return cameraPos; }
    static void performCamShake(BaseObj* ge, float fp);

    static void doShaderRefresh(bool doBake);
    static void refreshIncludeMap();

	static void bindShader(std::string shaderName);
	static void unbindShader();

	static void setShaderArrayfVec3(std::string paramName, float *x, int count);

	static void setShaderArrayfVec4(std::string paramName, float *x, int count);

	static void setShaderMatrix4x4(std::string paramName, float *x, int count);

	static void setShaderMatrix3x3(std::string paramName, float *x, int count);

	static void setShaderArray(std::string paramName, float *x, int count);

	static GLint getShaderLoc(std::string paramName);

	static void setShaderFloat(std::string paramName, float x);

	static void setShaderInt(std::string paramName, int x);

	static void setShaderfVec2(std::string paramName, FIVector4 *v);

	static void setShaderVec2(std::string paramName, float x, float y);

	static void setShaderVec3(std::string paramName, float x, float y, float z);

	static void setShaderfVec3(std::string paramName, FIVector4 *v);

	static void setShaderbtVec3(std::string paramName, btVector3 v);

	static void setShaderVec4(std::string paramName, float x, float y, float z, float w);

	static void setShaderfVec4(std::string paramName, FIVector4 *v);

	static void setShaderFloatUB(std::string paramName, float x);

	static void setShaderfVec4UB(std::string paramName, FIVector4 *v);

	static void updateUniformBlock(int ubIndex, int ubDataSize=-1);

	static void invalidateUniformBlock(int ubIndex);

	static void beginUniformBlock(int ubIndex);

	static bool wasUpdatedUniformBlock(int ubIndex);

	static void setShaderTBO(int multitexNumber, GLuint tbo_tex, GLuint tbo_buf, bool isFloat);

	static void setShaderTexture(int multitexNumber, uint texId);

	static void setShaderTexture3D(int multitexNumber, uint texId);

	static void idrawCrossHairs(FIVector4 originVec, float radius);

	static void drawLine(FIVector4 *p0, FIVector4 *p1);

	static void drawCubeCentered(FIVector4 *originVec, float radius);

	static void drawBoxUp(FIVector4 originVec, float radiusX, float radiusY, float diamZ);

	static void drawBoxMinMax(
		btVector3 v0,
		btVector3 v1
	);

	static void drawBoxRad(
		btVector3 v0,
		btVector3 v1
	);

	static void drawBox(
		FIVector4 *v0,
		FIVector4 *v1,
		int faceFlag=2
	);

	static void drawQuadWithCoords(
		FIVector4* p0,
		FIVector4* p1,
		FIVector4* p2,
		FIVector4* p3,

		float tx1,
		float ty1,
		float tx2,
		float ty2

	);

	static void drawQuadBounds(
		float fx1,
		float fy1,
		float fx2,
		float fy2,

		float fz
	);

	static void drawFSQuad();

	static void drawFSQuadOffset(
		float xOff,
		float yOff,
		float zm
	);

//fbo
    static void bindFBO(std::string fboName, int swapFlag=-1, int doClear=1);

    static void unbindFBO();

    static void sampleFBO(std::string fboName, int offset=0, int swapFlag=-1, int minOff=0, int maxOff=-1);

    static void unsampleFBO(std::string fboName, int offset=0, int swapFlag=-1, int minOff=0, int maxOff=-1);

    static void bindFBODirect(FBOSet *fbos, int doClear=1);

    // offset - write offset, _minOff - read min, _maxoff - read max
    static void sampleFBODirect(FBOSet *fbos, int offset=0, int _minOff=0, int _maxOff=-1);

    // offset - write offset, _minOff - read min, _maxoff - read max
    static void unsampleFBODirect(FBOSet *fbos, int offset=0, int _minOff=0, int _maxOff=-1);

    static void copyFBO(std::string src, std::string dest, int num=0);

    static void copyFBO2(std::string src, std::string dest, int num1=0, int num2=1);

    static void copyFBO3(std::string src, std::string dest, int num1=0, int num2=1, int num3=2);

    void getMatrixFromFBO(std::string fboName);

	static void drawFBO(std::string fboName, int ind, float zm, int swapFlag=-1);

	static void drawFBOOffsetDirect(FBOSet *fbos, int ind, float xOff, float yOff, float zm);

	static void drawFBOOffset(std::string fboName, int ind, float xOff, float yOff, float zm);

//matrix
    static void ComputeFOVProjection(float* result, float fov, float aspect, float nearDist, float farDist, bool leftHanded);
    static void getLSMatrix(FIVector4* lightPosParam, Matrix4 &lsMat, float orthoSize);
    static void setMatrices(int w, int h);

    static int widthWin;
    static int heightWin;
    static int baseW;
    static int baseH;
    static int lastW;
    static int lastH;

    static float FOV;
    static float heightOfNearPlane;

    static Matrix4 pmMatrix;

    static GLuint fsqDL;
    static VBOWrapper fsQuad;
    static TBOWrapper limbTBO;
    static TBOWrapper primTBO;

    static unsigned int fboVertexArray;
    static unsigned int fboVertexBuffer;
    static unsigned int fboIndexBuffer;
    static unsigned int fboIndexSize;

    static float clipDist[2];
    static bool perspectiveOn;
    static bool lastPersp;
    static bool sphereMapOn;
    static bool drawOrient;
    static bool bakeParamsOn;

    static int forceShadowUpdate;
    static FIVector4 lastLightPos;

    static float cameraShake;
    static Timer shakeTimer;

    static GLfloat camRotation[2];
    static GLfloat curCamRotation[2];
    static float cameraZoom;
    static FIVector4 *cameraPos;
    static FIVector4 tempLerpPos;
    static FIVector4 camLerpPos;
    static FIVector4 resultCameraPos;
    static FIVector4 targetCameraPos;
    static FIVector4 baseCameraPos;

    static float scaleFactor;
    static FIVector4 lookAtVec;
    static Matrix4 viewMatrix;
    static Matrix4 projMatrix;
    static Matrix4 curObjMatrix;
    static Matrix3 curObjMatrix3;
    static GLint viewport[4];
    static float viewMatrixDI[16];

    static FIVector4 lightVec;
    static FIVector4 lightVecOrig;
    static FIVector4 lightLookAt;
    static Matrix4 lightView;

	static int currentFBOResolutionX;
	static int currentFBOResolutionY;

	static int shadersAreLoaded;
	static std::map<std::string, Shader*> shaderMap;
	static std::string curShader;
	static Shader *curShaderPtr;
    static std::vector<std::string> shaderStrings;
    static std::vector<std::string> shaderTextureIds;
    static int readyToRecompile;

    static FIVector4 bufferDim;
    static FIVector4 bufferDimTarg;
    static FIVector4 bufferDimHalf;
    static FIVector4 bufferModDim;
    static FIVector4 bufferRenderDim;

    static FIVector4 rasterLowDim;

    static bool wsBufferInvalid;

    static bool LAST_COMPILE_ERROR;
};



#endif//_voxelquest_renderer_h_
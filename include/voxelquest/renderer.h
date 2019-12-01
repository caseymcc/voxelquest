#ifndef _voxelquest_renderer_h_
#define _voxelquest_renderer_h_

#include "voxelquest/vectors.h"
#include "voxelquest/fbos.h"
#include "voxelquest/shader.h"

class Renderer
{
public:
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

	static void doShaderRefresh(bool doBake);

	static void bindFBODirect(FBOSet *fbos, int doClear);

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

	static void drawFBO(std::string fboName, int ind, float zm, int swapFlag=-1);

	static void drawFBOOffsetDirect(FBOSet *fbos, int ind, float xOff, float yOff, float zm);

	static void drawFBOOffset(std::string fboName, int ind, float xOff, float yOff, float zm);

	static int currentFBOResolutionX;
	static int currentFBOResolutionY;

	static int shadersAreLoaded;
	static std::map<std::string, Shader*> shaderMap;
	static std::string curShader;
	static Shader *curShaderPtr;
};



#endif//_voxelquest_renderer_h_
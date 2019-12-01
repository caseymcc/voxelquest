#ifndef _voxelquest_myoglapp_h_
#define _voxelquest_myoglapp_h_

#include "voxelquest/oglrenderer.h"

#include <CommonInterfaces/CommonGraphicsAppInterface.h>

#include <iostream>

// ??????????????

struct MyOGLAppInternalData
{
	GLuint m_fontTextureId;
	GLuint m_largeFontTextureId;

};

struct MyOGLApp: public CommonGraphicsApp
{
protected:
	struct MyOGLAppInternalData*	m_data;

public:
	MyOGLApp(const char* title, int width, int height)
	{
		m_renderer=new OGLRenderer(width, height);
	}
	virtual ~MyOGLApp() {}

	virtual void drawGrid(DrawGridData data=DrawGridData()) {}
	virtual void setUpAxis(int axis) {}
	virtual int getUpAxis() const { return 2; }

	virtual void swapBuffer() {}
	virtual void drawText(const char* txt, int posX, int posY) {}
	virtual void setBackgroundColor(float red, float green, float blue) {}
	virtual int	registerCubeShape(float halfExtentsX, float halfExtentsY, float halfExtentsZ, int textureIndex=-1, float textureScaling=1)
	{
		std::cout<<"a\n";
		return 0;
	}
	//virtual int	registerGraphicsUnitSphereShape(EnumSphereLevelOfDetail lod, int textureId=-1)
	virtual int	registerGraphicsSphereShape(float radius, bool usePointSprites=true, int largeSphereThreshold=100, int mediumSphereThreshold=10)
	{
		std::cout<<"b\n";
		return 0;
	}
	virtual void drawText3D(const char* txt, float posX, float posZY, float posZ, float size) {}
	virtual void registerGrid(int xres, int yres, float color0[4], float color1[4])
	{
		std::cout<<"c\n";
	}

	virtual struct	GLInstanceRendererInternalData* getInternalData()
	{
		std::cout<<"d\n";
		return 0;
	}

};

#endif//_voxelquest_myoglapp_h_
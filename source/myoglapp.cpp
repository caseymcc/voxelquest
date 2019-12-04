#include "voxelquest/myoglapp.h"
#include "voxelquest/oglrenderer.h"

#include <iostream>
#include <glbinding/gl/gl.h>

using namespace gl;

struct MyOGLAppInternalData
{
    GLuint m_fontTextureId;
    GLuint m_largeFontTextureId;

};

MyOGLApp::MyOGLApp(const char* title, int width, int height)
{
    m_renderer=new OGLRenderer(width, height);
}

int	MyOGLApp::registerCubeShape(float halfExtentsX, float halfExtentsY, float halfExtentsZ)
{
    std::cout<<"a\n";
    return 0;
}

int	MyOGLApp::registerGraphicsSphereShape(float radius, bool usePointSprites, int largeSphereThreshold, int mediumSphereThreshold)
{
    std::cout<<"b\n";
    return 0;
}

void MyOGLApp::registerGrid(int xres, int yres, float color0[4], float color1[4])
{
    std::cout<<"c\n";
}

struct	GLInstanceRendererInternalData* getInternalData()
{
    std::cout<<"d\n";
    return nullptr;
}
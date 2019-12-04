#include "voxelquest/oglrenderer.h"

#include "LinearMath/btVector3.h"

#include <glbinding/gl/gl.h>

using namespace gl;

OGLRenderer::OGLRenderer(int width, int height)
	:m_width(width),
	m_height(height)
{

}

void OGLRenderer::init()
{
}

const CommonCameraInterface* OGLRenderer::getActiveCamera() const
{
	return &m_camera;
}
CommonCameraInterface* OGLRenderer::getActiveCamera()
{
	return &m_camera;
}
void OGLRenderer::setActiveCamera(CommonCameraInterface* cam)
{
	b3Assert(0);//not supported yet
}

void OGLRenderer::updateCamera(int upAxis)
{
	float projection[16];
	float view[16];
	m_camera.setAspectRatio((float)m_width/(float)m_height);
	m_camera.update();
	m_camera.getCameraProjectionMatrix(projection);
	m_camera.getCameraViewMatrix(view);
	GLfloat projMat[16];
	GLfloat viewMat[16];
	for(int i=0; i<16; i++)
	{
		viewMat[i]=view[i];
		projMat[i]=projection[i];
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(projMat);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(viewMat);
}

void OGLRenderer::removeAllInstances()
{
}


void OGLRenderer::writeSingleInstanceColorToCPU(float* color, int srcIndex)
{
}
void OGLRenderer::writeSingleInstanceColorToCPU(double* color, int srcIndex)
{

}
void OGLRenderer::getCameraViewMatrix(float viewMat[16]) const
{
	b3Assert(0);
}
void OGLRenderer::getCameraProjectionMatrix(float projMat[16]) const
{
	b3Assert(0);

}


void OGLRenderer::renderScene()
{
	//cout << "asdfasdf\n";
}




int OGLRenderer::registerGraphicsInstance(int shapeIndex, const double* position, const double* quaternion, const double* color, const double* scaling)
{
	return 0;
}

int OGLRenderer::registerGraphicsInstance(int shapeIndex, const float* position, const float* quaternion, const float* color, const float* scaling)
{
	return 0;
}

void OGLRenderer::drawLines(const float* positions, const float color[4], int numPoints, int pointStrideInBytes, const unsigned int* indices, int numIndices, float pointDrawSize)
{
	int pointStrideInFloats=pointStrideInBytes/4;
	for(int i=0; i<numIndices; i+=2)
	{
		int index0=indices[i];
		int index1=indices[i+1];

		//btVector3 fromColor=b3MakeVector3(color[0], color[1], color[2]);
		//btVector3 toColor=b3MakeVector3(color[0], color[1], color[2]);
        //
		//btVector3 from=b3MakeVector3(positions[index0*pointStrideInFloats], positions[index0*pointStrideInFloats+1], positions[index0*pointStrideInFloats+2]);
		//btVector3 to=b3MakeVector3(positions[index1*pointStrideInFloats], positions[index1*pointStrideInFloats+1], positions[index1*pointStrideInFloats+2]);

		// glBegin(GL_LINES);
		// glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
		// glVertex3d(from.getX(), from.getY(), from.getZ());
		// glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
		// glVertex3d(to.getX(), to.getY(), to.getZ());
		// glEnd();

	}
}

void OGLRenderer::drawLine(const float from[4], const float to[4], const float color[4], float lineWidth)
{
	// glBegin(GL_LINES);
	// glColor3f(color[0],color[1],color[2]);
	// glVertex3d(from[0],from[1],from[2]);
	// glVertex3d(to[0],to[1],to[2]);
	// glEnd();
}
int OGLRenderer::registerShape(const float* vertices, int numvertices, const int* indices, int numIndices, int primitiveType, int textureIndex)
{
	return 0;
}

void OGLRenderer::writeSingleInstanceTransformToCPU(const float* position, const float* orientation, int srcIndex)
{
}
void OGLRenderer::writeSingleInstanceTransformToCPU(const double* position, const double* orientation, int srcIndex)
{
}
void OGLRenderer::writeTransforms()
{
}


void OGLRenderer::drawLine(const double from[4], const double to[4], const double color[4], double lineWidth)
{

}
void OGLRenderer::drawPoint(const float* position, const float color[4], float pointDrawSize)
{
}
void OGLRenderer::drawPoint(const double* position, const double color[4], double pointDrawSize)
{
}

void OGLRenderer::updateShape(int shapeIndex, const float* vertices)
{
}

void OGLRenderer::enableBlend(bool blend)
{
}

#ifndef _voxelquest_includes_h_
#define _voxelquest_includes_h_


class HPClock
{
public:
	HPClock();

	HPClock(const HPClock& other);
	HPClock& operator=(const HPClock& other);

	~HPClock();

	/// Resets the initial reference time.
	void reset();

	/// Returns the time in ms since the last call to reset or since 
	/// the HPClock was created.
	unsigned long int getTimeMilliseconds();

	/// Returns the time in us since the last call to reset or since 
	/// the Clock was created.
	unsigned long int getTimeMicroseconds();
private:
	struct HPClockData* m_data;
};



template <class T>
const T& HPClockMin(const T& a, const T& b) 
{
  return a < b ? a : b ;
}


struct HPClockData
{

#ifdef B3_USE_WINDOWS_TIMERS
	LARGE_INTEGER mClockFrequency;
	DWORD mStartTick;
	LONGLONG mPrevElapsedTime;
	LARGE_INTEGER mStartTime;
#else
#ifdef __CELLOS_LV2__
	uint64_t	mStartTime;
#else
	struct timeval mStartTime;
#endif
#endif //__CELLOS_LV2__

};






static inline int newcasecmp(const char *s1, const char *s2, size_t n)
{
	int lc1  = 0;
	int lc2  = 0;

	while (n--)
	{
		lc1 = towlower (*s1);
		lc2 = towlower (*s2);

		if (lc1 != lc2)
			return (lc1 - lc2);

		if (!lc1)
			return 0;

		++s1;
		++s2;
	}

	return 0;
}

// todo: this define of wcsncasecmp should not be here

//Win32 incompatibilities
#if defined(WIN32) && !defined(__GNUC__)
static inline bool isnan(double x) {
	return x != x;
}
static inline bool isinf(double x) {
	return !isnan(x) && isnan(x - x);
}
#endif

// Linux compile fix - from quaker66
#ifdef __GNUC__
// #include <cstring>
// #include <cstdlib>
#endif

// Mac compile fixes - from quaker66, Lion fix by dabrahams
#if defined(__APPLE__) && __DARWIN_C_LEVEL < 200809L || (defined(WIN32) && defined(__GNUC__))
#include <wctype.h>
#include <wchar.h>

#endif

// Simple function to check a string 's' has at least 'n' characters
static inline bool simplejson_wcsnlen(const char *s, size_t n) {
	if (s == 0)
		return false;

	const char *save = s;
	while (n-- > 0)
	{
		if (*(save++) == 0) return false;
	}

	return true;
}

struct membuf : std::streambuf
{
	membuf(char *begin, char *end) {
		this->setg(begin, begin, end);
	}
};



bool PROG_ACTIVE = true;



#define glError() { \
		GLenum err = glGetError(); \
		while (err != GL_NO_ERROR) { \
			printf("glError: %s caught at %s:%u", \
						 (char*)gluErrorString(err), __FILE__, __LINE__); \
			err = glGetError(); \
			exit(-1); \
		} \
	}


// WGL specific extensions for v3.0+ //////////////////////////////////////////
#ifdef _WIN32
//#include <windows.h>
#ifndef WGLGETEXTENSIONSSTRINGARB_DEF
#define WGLGETEXTENSIONSSTRINGARB_DEF
typedef const char* (WINAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC hdc);
PFNWGLGETEXTENSIONSSTRINGARBPROC    pwglGetExtensionsStringARB = 0;
#define wglGetExtensionsStringARB  pwglGetExtensionsStringARB
#endif
#endif

// version 2.0 or greater
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C



// function pointers for PBO Extension
// Windows needs to get function pointers from ICD OpenGL drivers,
// because opengl32.dll does not support extensions higher than v1.1.
#ifdef _WIN32
PFNGLGENBUFFERSARBPROC pglGenBuffersARB = 0;                     // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC pglBindBufferARB = 0;                     // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC pglBufferDataARB = 0;                     // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB = 0;               // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB = 0;               // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB = 0; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC pglMapBufferARB = 0;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB = 0;                   // unmap VBO procedure
// #define glGenBuffersARB           pglGenBuffersARB
// #define glBindBufferARB           pglBindBufferARB
// #define glBufferDataARB           pglBufferDataARB
// #define glBufferSubDataARB        pglBufferSubDataARB
// #define glDeleteBuffersARB        pglDeleteBuffersARB
// #define glGetBufferParameterivARB pglGetBufferParameterivARB
// #define glMapBufferARB            pglMapBufferARB
// #define glUnmapBufferARB          pglUnmapBufferARB
#endif



// typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
// extern PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;

// static inline void init_EXT_Vsync()
// {
// wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)extgl_GetProcAddress( "wglSwapIntervalEXT" );
// }







// data for a fullscreen quad (this time with texture coords)
GLfloat vertexDataQuad[] = {
//  X     Y     Z           U     V     
   1.0f, 1.0f, 0.0f, 1.0f,       1.0f, 1.0f, 0.0f, 0.0f, // vertex 0
  -1.0f, 1.0f, 0.0f, 1.0f,       0.0f, 1.0f, 0.0f, 0.0f, // vertex 1
   1.0f,-1.0f, 0.0f, 1.0f,       1.0f, 0.0f, 0.0f, 0.0f, // vertex 2
  -1.0f,-1.0f, 0.0f, 1.0f,       0.0f, 0.0f, 0.0f, 0.0f  // vertex 3
}; // 4 vertices with 8 components (floats) each
GLuint indexDataQuad[] = {
    0,1,2, // first triangle
    2,1,3  // second triangle
};








// ??????????????????????????????





#define NUMRAYS 500
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Z 5

#define COLLISION_RADIUS 0.0f


#define BT_LINE_BATCH_SIZE 512
#if defined(BT_USE_DOUBLE_PRECISION)
#define btglLoadMatrix glLoadMatrixd
#define btglMultMatrix glMultMatrixd
#define btglColor3 glColor3d
#define btglVertex3 glVertex3d
#else
#define btglLoadMatrix glLoadMatrixf
#define btglMultMatrix glMultMatrixf
#define btglColor3 glColor3f
#define btglVertex3 glVertex3d
#endif

// #ifndef M_PI
// #define M_PI       btScalar(3.14159265358979323846)
// #endif

// #ifndef M_PI_2
// #define M_PI_2     btScalar(1.57079632679489661923)
// #endif

// #ifndef M_PI_4
// #define M_PI_4     btScalar(0.785398163397448309616)
// #endif

#ifndef M_PI_8
#define M_PI_8     0.5 * M_PI_4
#endif

//#define NUM_LEGS_GA 6
//#define BODYPART_COUNT_GA 2 * NUM_LEGS_GA + 1
//#define JOINT_COUNT_GA BODYPART_COUNT_GA - 1



class btConvexShape;
class btCollisionShape;
class btShapeHull;
class btDiscreteDynamicsWorld;
class MyShapeDrawer;
class btDynamicsWorld;
class btRigidBody;
class btBroadphaseInterface;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;





#include "../CommonInterfaces/CommonGraphicsAppInterface.h"
#include "../CommonInterfaces/CommonRenderInterface.h"
#include "../CommonInterfaces/CommonExampleInterface.h"
#include "../CommonInterfaces/CommonGUIHelperInterface.h"
#include "../CommonInterfaces/CommonRigidBodyBase.h"


#include "btBulletDynamicsCommon.h"

// #include "Bullet3Common/btVector3.h"
// #include "Bullet3Common/b3Scalar.h"

// #include "LinearMath/btVector3.h"
// #include "LinearMath/btAlignedObjectArray.h"
// #include "LinearMath/btTransformUtil.h"



#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "BulletCollision/CollisionShapes/btPolyhedralConvexShape.h"
#include "BulletCollision/CollisionShapes/btConvexPolyhedron.h"
#include "BulletCollision/CollisionDispatch/btSimulationIslandManager.h"

// included within btBulletDynamicsCommon.h
// #include "LinearMath/btIDebugDraw.h"
// #include "LinearMath/btDefaultMotionState.h"
// #include "LinearMath/btTransform.h"
// #include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
// #include "BulletCollision/CollisionShapes/btBoxShape.h"
// #include "BulletCollision/CollisionShapes/btSphereShape.h"
// #include "BulletCollision/CollisionShapes/btConeShape.h"
// #include "BulletCollision/CollisionShapes/btCylinderShape.h"
// #include "BulletCollision/CollisionShapes/btTetrahedronShape.h"
// #include "BulletCollision/CollisionShapes/btCompoundShape.h"
// #include "BulletCollision/CollisionShapes/btCapsuleShape.h"
// #include "BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h"
// #include "BulletCollision/CollisionShapes/btUniformScalingShape.h"
// #include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"
// #include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
// #include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
// #include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
// #include "BulletDynamics/Dynamics/btRigidBody.h"

#include "../OpenGLWindow/OpenGL2Include.h"
#include "../OpenGLWindow/SimpleCamera.h"
#include "../OpenGLWindow/GLInstanceGraphicsShape.h"

#include "../Benchmarks/TaruData.h"
//#include "../Benchmarks/landscapeData.h"






/////







// ??????????????????????????????






/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/









struct ShapeCache
{
	struct Edge { btVector3 n[2];int v[2]; };
	ShapeCache(btConvexShape* s) : m_shapehull(s) {}
	btShapeHull					m_shapehull;
	btAlignedObjectArray<Edge>	m_edges;
};

void OGL_displaylist_register_shape(btCollisionShape * shape);
void OGL_displaylist_clean();



///










class GlDrawcallback : public btTriangleCallback
{

public:

	bool	m_wireframe;

	GlDrawcallback()
		:m_wireframe(false)
	{
	}

    virtual void processTriangle(btVector3* triangle, int partId, int triangleIndex);
	
};

class TriangleGlDrawcallback : public btInternalTriangleIndexCallback
{
public:
    virtual void internalProcessTriangleIndex(btVector3* triangle, int partId, int  triangleIndex);
	
};


// ???????????


struct OGLRenderer : public CommonRenderInterface
{
    int m_width;
    int m_height;
    SimpleCamera	m_camera;
    
    OGLRenderer(int width, int height);
    
    virtual void init();
    
    virtual void updateCamera(int upAxis);
    
	virtual const CommonCameraInterface* getActiveCamera() const;
	virtual CommonCameraInterface* getActiveCamera();
	virtual void setActiveCamera(CommonCameraInterface* cam);

	virtual void resize(int width, int height)
	{
		m_width = width;
		m_height = height;
	}

    virtual void removeAllInstances();
    
    
    virtual void writeSingleInstanceColorToCPU(float* color, int srcIndex);
    virtual void writeSingleInstanceColorToCPU(double* color, int srcIndex);
    virtual void getCameraViewMatrix(float viewMat[16]) const;
    virtual void getCameraProjectionMatrix(float projMat[16]) const;

    
    virtual void renderScene();
    
    virtual int getScreenWidth()
    {
        return m_width;
    }
    virtual int getScreenHeight()
    {
        return m_height;
    }
	virtual int	registerTexture(const unsigned char* texels, int width, int height)
	{
		return -1;
	}
    virtual int registerGraphicsInstance(int shapeIndex, const double* position, const double* quaternion, const double* color, const double* scaling);
    
    virtual int registerGraphicsInstance(int shapeIndex, const float* position, const float* quaternion, const float* color, const float* scaling);
    
    virtual void drawLines(const float* positions, const float color[4], int numPoints, int pointStrideInBytes, const unsigned int* indices, int numIndices, float pointDrawSize);
    
    virtual void drawLine(const float from[4], const float to[4], const float color[4], float lineWidth);
    
    virtual int registerShape(const float* vertices, int numvertices, const int* indices, int numIndices,int primitiveType=B3_GL_TRIANGLES, int textureIndex=-1);
    
    virtual void writeSingleInstanceTransformToCPU(const float* position, const float* orientation, int srcIndex);
    
    virtual void writeSingleInstanceTransformToCPU(const double* position, const double* orientation, int srcIndex);
    
    virtual void writeTransforms();
    
    virtual void drawLine(const double from[4], const double to[4], const double color[4], double lineWidth);
    
    virtual void drawPoint(const float* position, const float color[4], float pointDrawSize);
    
    virtual void drawPoint(const double* position, const double color[4], double pointDrawSize);
    
    virtual void updateShape(int shapeIndex, const float* vertices);
    
    virtual void enableBlend(bool blend);

	virtual struct	GLInstanceRendererInternalData* getInternalData()
	{
		return 0;
	}
    
};





// ??????????????

struct MyOGLAppInternalData
{
	GLuint m_fontTextureId;
	GLuint m_largeFontTextureId;
	
};

struct MyOGLApp : public CommonGraphicsApp
{
protected:
	struct MyOGLAppInternalData*	m_data;

public:
	MyOGLApp(const char* title, int width, int height) {
		m_renderer = new OGLRenderer(width,height);		
	}
	virtual ~MyOGLApp() {}

	virtual void drawGrid(DrawGridData data=DrawGridData()) {}
	virtual void setUpAxis(int axis) {}
	virtual int getUpAxis() const {return 2;}
	
	virtual void swapBuffer() {}
	virtual void drawText( const char* txt, int posX, int posY) {}
	virtual void setBackgroundColor(float red, float green, float blue) {}
	virtual int	registerCubeShape(float halfExtentsX,float halfExtentsY, float halfExtentsZ, int textureIndex = -1,  float textureScaling = 1)
	{
		cout << "a\n";
		return 0;
	}
	virtual int	registerGraphicsUnitSphereShape(EnumSphereLevelOfDetail lod, int textureId=-1)
	{
		cout << "b\n";
		return 0;
	}
    virtual void drawText3D( const char* txt, float posX, float posZY, float posZ, float size) {}
    virtual void registerGrid(int xres, int yres, float color0[4], float color1[4]) {
    	cout << "c\n";
    }
    
    virtual struct	GLInstanceRendererInternalData* getInternalData()
    {
    	cout << "d\n";
    	return 0;
    }
    
};






static btVector4 sColors[4] =
{
	btVector4(0.3,0.3,1,1),
	btVector4(0.6,0.6,1,1),
	btVector4(0,1,0,1),
	btVector4(0,1,1,1),
	//btVector4(1,1,0,1),
};


struct MyDebugVec3
{
	MyDebugVec3(const btVector3& org)
	:x(org.x()),
	y(org.y()),
	z(org.z())
	{
	}

	float x;
	float y;
	float z;
};
class MyDebugDrawer : public btIDebugDraw
{
	CommonGraphicsApp* m_glApp;
	int m_debugMode;

    btAlignedObjectArray<MyDebugVec3> m_linePoints;
    btAlignedObjectArray<unsigned int> m_lineIndices;
    btVector3 m_currentLineColor;
	DefaultColors m_ourColors;

public:

    MyDebugDrawer(CommonGraphicsApp* app);
	
    virtual DefaultColors	getDefaultColors() const;
	
	///the default implementation for setDefaultColors has no effect. A derived class can implement it and store the colors.
    virtual void setDefaultColors(const DefaultColors& colors);
	


    virtual void drawLine(const btVector3& from1, const btVector3& to1, const btVector3& color1);
	

    virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	
     

    virtual void reportErrorWarning(const char* warningString);

    virtual void draw3dText(const btVector3& location, const char* textString);

    virtual void setDebugMode(int debugMode);

    virtual int		getDebugMode() const;

    virtual void flushLines();
	

};




struct MyConvertPointerSizeT
{
	union 
	{
			const void* m_ptr;
			size_t m_int;
	};
};

bool shapePointerCompareFunc(const btCollisionObject* colA, const btCollisionObject* colB);


struct MyGLHelperInternalData
{
	struct CommonGraphicsApp* m_glApp;
	class MyDebugDrawer* m_debugDraw;
	MyShapeDrawer* m_gl2ShapeDrawer;
};














// %%%%%%%%%%%%%%%%%%%%%%%%%


/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/














struct CustFilterCallback : public btOverlapFilterCallback
{
	
	// CustFilterCallback()
	// {
	// }
	// virtual ~CustFilterCallback()
	// {
	// }
	
	// return true when pairs need collision
    virtual bool needBroadphaseCollision(
        btBroadphaseProxy* proxy0,
        btBroadphaseProxy* proxy1) const;
	
};




class BenchmarkDemo : public CommonRigidBodyBase
{

	//keep the collision shapes, for deletion/cleanup

	btAlignedObjectArray<class RagDoll*>	m_ragdolls;
	
	//btOverlapFilterCallback * filterCallback;
	
	btVector3 initOffset;
	
	int	m_benchmark;

	void myinit()
	{
		//??
	}

	void setCameraDistance(btScalar dist)
	{
	}
	void createTest1();
	void createTest2();
	void createTest3();
	void createTest4();
	void createTest5();
	void createTest6();
	void createTest7();
	void createTest8();

	
	void createWall(const btVector3& offsetPosition,int stackSize,const btVector3& boxSize);
	void createPyramid(const btVector3& offsetPosition,int stackSize,const btVector3& boxSize);
	void createTowerCircle(const btVector3& offsetPosition,int stackSize,int rotSize,const btVector3& boxSize);
	//void createLargeMeshBody();


	class SpuBatchRaycaster* m_batchRaycaster;
	class btThreadSupportInterface* m_batchRaycasterThreadSupport;

	void castRays();
	void initRays();

	public:

	
	BenchmarkDemo(struct GUIHelperInterface* helper, int benchmark)
	:CommonRigidBodyBase(helper),
	m_benchmark(benchmark)
	{
	}
	virtual ~BenchmarkDemo()
	{
		exitPhysics();
	}
	
	btDiscreteDynamicsWorld* getWorld();
	
	btRigidBody* createRigidBodyMask(
		btScalar mass, const btTransform& startTransform, btCollisionShape* shape, int maskFrom, int maskTo
	);
	
	void removeRigidBody(btRigidBody* body);
	
	void updateGraphicsObjects();
	
	void beginDrop(float x, float y, float z);
	
	// btRigidBody* bodyPick(const btVector3& rayFromWorld, const btVector3& rayToWorld);
	
	void initPhysics();

	void exitPhysics();

	void stepSimulation(float deltaTime);

	void resetCamera()
	{
		float dist = 120;
		float pitch = 52;
		float yaw = 35;
		float targetPos[3]={0,10.46,0};
		m_guiHelper->resetCamera(dist,pitch,yaw,targetPos[0],targetPos[1],targetPos[2]);
	}
};

// btRigidBody* BenchmarkDemo::bodyPick(const btVector3& rayFromWorld, const btVector3& rayToWorld) {
	
// 	if (m_dynamicsWorld==0) {
// 		cout << "world not ready\n";
// 		return NULL;
// 	}

// 	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFromWorld, rayToWorld);

// 	m_dynamicsWorld->rayTest(rayFromWorld, rayToWorld, rayCallback);
// 	if (rayCallback.hasHit())
// 	{

// 		btVector3 pickPos = rayCallback.m_hitPointWorld;
// 		btRigidBody* body = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
// 		if (body)
// 		{
// 			//other exclusions?
// 			if (!(body->isStaticObject() || body->isKinematicObject()))
// 			{
				
// 				return body;
				
// 				// m_pickedBody = body;
// 				// m_savedState = m_pickedBody->getActivationState();
// 				// m_pickedBody->setActivationState(DISABLE_DEACTIVATION);
// 				// //printf("pickPos=%f,%f,%f\n",pickPos.getX(),pickPos.getY(),pickPos.getZ());
// 				// btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;
// 				// btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*body, localPivot);
// 				// m_dynamicsWorld->addConstraint(p2p, true);
// 				// m_pickedConstraint = p2p;
// 				// btScalar mousePickClamping = 30.f;
// 				// p2p->m_setting.m_impulseClamp = mousePickClamping;
// 				// //very weak constraint for picking
// 				// p2p->m_setting.m_tau = 0.001f;
// 			}
// 		}
		
		
// 		// m_oldPickingPos = rayToWorld;
// 		// m_hitPos = pickPos;
// 		// m_oldPickingDist = (pickPos - rayFromWorld).length();
// 	}
// 	return NULL;
	
// }


class btRaycastBar2
{
public:
	btVector3 source[NUMRAYS];
	btVector3 dest[NUMRAYS];
	btVector3 direction[NUMRAYS];
	btVector3 hit[NUMRAYS];
	btVector3 normal[NUMRAYS];
	struct GUIHelperInterface* m_guiHelper;
	
	int frame_counter;
	int ms;
	int sum_ms;
	int sum_ms_samples;
	int min_ms;
	int max_ms;

#ifdef USE_BT_CLOCK
	btClock frame_timer;
#endif //USE_BT_CLOCK

	btScalar dx;
	btScalar min_x;
	btScalar max_x;
	btScalar max_y;
	btScalar sign;

    btRaycastBar2();
	



    btRaycastBar2(btScalar ray_length, btScalar z, btScalar max_y, struct GUIHelperInterface* guiHelper);
	

    void move(btScalar dt);
	

    void cast(btCollisionWorld* cw);
	

    void draw();
	
};


static btRaycastBar2 raycastBar;



const btVector3 rotate(const btQuaternion& quat, const btVector3 & vec);


class RagDoll
{
	enum
	{
		BODYPART_PELVIS = 0,
		BODYPART_SPINE,
		BODYPART_HEAD,

		BODYPART_LEFT_UPPER_LEG,
		BODYPART_LEFT_LOWER_LEG,

		BODYPART_RIGHT_UPPER_LEG,
		BODYPART_RIGHT_LOWER_LEG,

		BODYPART_LEFT_UPPER_ARM,
		BODYPART_LEFT_LOWER_ARM,

		BODYPART_RIGHT_UPPER_ARM,
		BODYPART_RIGHT_LOWER_ARM,

		BODYPART_COUNT
	};

	enum
	{
		JOINT_PELVIS_SPINE = 0,
		JOINT_SPINE_HEAD,

		JOINT_LEFT_HIP,
		JOINT_LEFT_KNEE,

		JOINT_RIGHT_HIP,
		JOINT_RIGHT_KNEE,

		JOINT_LEFT_SHOULDER,
		JOINT_LEFT_ELBOW,

		JOINT_RIGHT_SHOULDER,
		JOINT_RIGHT_ELBOW,

		JOINT_COUNT
	};

	btDynamicsWorld* m_ownerWorld;
	btCollisionShape* m_shapes[BODYPART_COUNT];
	btRigidBody* m_bodies[BODYPART_COUNT];
	btTypedConstraint* m_joints[JOINT_COUNT];

    btRigidBody* createRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape);

public:
    RagDoll(btDynamicsWorld* ownerWorld, const btVector3& positionOffset, btScalar scale);

    virtual	~RagDoll();
	
};



btVector3 NET_MASKS[256];
void initNetMasks();

bool replaceStr(std::string& str, const std::string& from, const std::string& to);

// struct CommonExampleInterface*    BenchmarkCreateFunc(struct CommonExampleOptions& options)
// {
// 	return new BenchmarkDemo(options.m_guiHelper,options.m_option);
// }


#endif//_voxelquest__h_
#ifndef _voxelquest_includes_h_
#define _voxelquest_includes_h_

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif


int RUN_COUNT;

bool ND_TRACE_OFF = false;


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

const static int MAX_LIMB_DATA_IN_BYTES = 65536;

const static bool POLYS_FOR_CELLS = true;
//const static bool POLY_COLLISION = false;
const static bool VOXEL_COLLISION = true;

const static bool GEN_DEBRIS = false;
const static int  MAX_DEBRIS = 0;
const static bool GEN_COLLISION = false;
const static bool GEN_POLYS_WORLD = true;

const static bool SINGLE_THREADED = false;
const static bool DO_RANDOMIZE = false;

// no greater than 8 unless shader changed (primIdList[8])
const static int MAX_PRIM_IDS = 16;
const static int MAX_PRIMTEST = 8;

const static int MAX_DEPTH_PEELS = 4;

double STEP_TIME_IN_SEC;

const static float OFFSET_X[4] = {-0.5,0.5,0.5,-0.5};
const static float OFFSET_Y[4] = {-0.5,-0.5,0.5,0.5};

const static int MAX_SPLASHES = 8;
const static int MAX_EXPLODES = 8;

//const static bool DO_CONNECT = true;
const static bool DO_SHADER_DUMP = false;
bool EDIT_POSE = false;

// warning: changing this changes the size of saved poses
// should be a one time change, then revereted to 1.0 after save
const static float ORG_SCALE_DELTA = 1.0f;

// base scale applied to base org generation
// only change this after changing ORG_SCALE_DELTA from 1.0
const static float ORG_SCALE_BASE = 0.5f;





// const static int DEF_WIN_W = 1440;
// const static int DEF_WIN_H = 720;

// qqqq

//#define STREAM_RES 1

#ifdef STREAM_RES
	const static int DEF_WIN_W = 2048; //2048;//
	const static int DEF_WIN_H = 1024; //1024;//
#else
	const static int DEF_WIN_W = 1536;//1536;
	const static int DEF_WIN_H = 768;//768;
#endif



const static int DEF_SCALE_FACTOR = 4;
const static int RENDER_SCALE_FACTOR = 1;
const static float SPHEREMAP_SCALE_FACTOR = 0.5f; // lower is faster

const static int DEF_VOL_SIZE = 128;

const static bool USE_SPHERE_MAP = false;

const static float TIME_DELTA = 1.0f/60.0f;

const static int THREAD_DATA_COUNT = 16;

const static float MASS_PER_LIMB = 0.1f;

// #define DEBUG_BOUNDS 1

const static float explodeRad = 5.0f;

float RAND_COUNTER = 25.0f;
float RAND_COUNTER2 = 25.0f;


const static int GROUP_ID_NONEXISTENT = -3;
const static int GROUP_ID_UNMARKED = -2;
const static int GROUP_ID_UNMARKED_IDEAL = -1;

const static int FLUID_UNIT_MIN = -1;
const static int FLUID_UNIT_MAX = 16384;

const static int MAX_LAYERS = 2;
const static int MAX_MIP_LEV = 1; // min of 1

const static bool DO_POINTS = true;


const static int MAX_KEYS = 256;

//const static int NUM_PRIM_LAYERS = 1;

const static int MAX_LIGHTS = 24;
const static int MAX_EVAL_LIGHTS = 128;
const static int FLOATS_PER_LIGHT = 12;



const static float UI_SCALE_FACTOR = 1.0f;

const static int MAX_PLANT_GEN = 16;

const static int MAX_TER_TEX = 9;

const static bool RT_TRANSFORM = false;



const static int MAX_NODE_DIRS = 6;
const static int MAX_NODE_VALS = 4;
const static int TOT_NODE_VALS =
	MAX_NODE_DIRS * MAX_NODE_VALS;

const static int TOT_MAP_DIRS = 4;

const static int MAX_BLOCK_STACK = 10;
const static int MAX_UI_LAYERS = 4;

// solid, water, air
const static float CUBE_POINTS[42] = {
		0,0,0,
		1,0,1,
		0,0,1,
		0,1,1,
		0,1,0,
		1,1,0,
		1,0,0,
		
		
		1,1,1,
		1,0,0,
		1,1,0,
		0,1,0,
		0,1,1,
		0,0,1,
		1,0,1
		
		
		
};




// solid, water, air
const static bool PROC_MATRIX[3][3] = {
		{false, true, true},
		{false, false, true},
		{false, false, false}
};

bool TEMP_DEBUG;

const static int MAX_OBJ_TYPES = 1673;
const static int ITEMS_PER_COL = 48;
const static int ITEMS_PER_ROW = 36;
const static int MAX_ICON_ID = ITEMS_PER_COL*ITEMS_PER_ROW;
const static int EMPTY_OBJECT_TYPE = 1672;
const static int EMPTY_OBJECT_ID = 1;

enum E_DIR_SPECS_SIGN {
	E_DIR_XP,
	E_DIR_XM,
	E_DIR_YP,
	E_DIR_YM,
	E_DIR_ZP,
	E_DIR_ZM,
	E_DIR_MP_LENGTH
};


const static int NUM_ORIENTATIONS = 6;
const static float DIR_VECS[NUM_ORIENTATIONS][3] = {
	{1.0f, 0.0f, 0.0f},
	{-1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, -1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, -1.0f}
};
const static int DIR_VECS_I[NUM_ORIENTATIONS][3] = {
	{1, 0, 0},
	{-1, 0, 0},
	{0, 1, 0},
	{0, -1, 0},
	{0, 0, 1},
	{0, 0, -1}
};


const static int NUM_PLANAR_ORIENTATIONS = 4;
const static int BASE_MOVEABLE_Z = 2;
const static int MAX_MOVEABLE_Z = BASE_MOVEABLE_Z*2 + 1;
const static int NUM_MOVEABLE_DIRS_ONE_AWAY = NUM_PLANAR_ORIENTATIONS*3;

const static int NUM_MOVEABLE_DIRS = MAX_MOVEABLE_Z*NUM_PLANAR_ORIENTATIONS;// 12 20 24
const static int DIR_VECS_MOVE[NUM_MOVEABLE_DIRS][3] = {
	
	{ 1,  0,  -2},
	{-1,  0,  -2},
	{ 0,  1,  -2},
	{ 0, -1,  -2},
	
	{ 1,  0, -1},
	{-1,  0, -1},
	{ 0,  1, -1},
	{ 0, -1, -1},
	
	
	{ 1,  0,  0},
	{-1,  0,  0},
	{ 0,  1,  0},
	{ 0, -1,  0},
	
	
	
	
	{ 1,  0,  1},
	{-1,  0,  1},
	{ 0,  1,  1},
	{ 0, -1,  1},
		
	{ 1,  0,  2},
	{-1,  0,  2},
	{ 0,  1,  2},
	{ 0, -1,  2}
	
	
	
	
	
	// ,
	// { 1,  0,  -3},
	// {-1,  0,  -3},
	// { 0,  1,  -3},
	// { 0, -1,  -3},
	
	// { 1,  0,  3},
	// {-1,  0,  3},
	// { 0,  1,  3},
	// { 0, -1,  3}
	
	
	
	// ,{ 1,  1,  1},
	// { 1, -1,  1},
	// {-1,  1,  1},
	// {-1, -1,  1},
	
	// { 1,  1,  0},
	// { 1, -1,  0},
	// {-1,  1,  0},
	// {-1, -1,  0},
	
	// { 1,  1, -1},
	// { 1, -1, -1},
	// {-1,  1, -1},
	// {-1, -1, -1}
	
	
};


float ALL_ROT[16*NUM_ORIENTATIONS*NUM_ORIENTATIONS*NUM_ORIENTATIONS];
const static int ROT_MAP[36] = {
	
	// x, inc theta
	//-------------
	E_DIR_XP,// x+ -> x+
	E_DIR_XM,// x- -> x-
	E_DIR_ZP,// y+ -> z+
	E_DIR_ZM,// y- -> z-
	E_DIR_YM,// z+ -> y-
	E_DIR_YP,// z- -> y+
	
	// x, dec theta
	//-------------
	E_DIR_XP,// x+ -> x+
	E_DIR_XM,// x- -> x-
	E_DIR_ZM,// y+ -> z-
	E_DIR_ZP,// y- -> z+
	E_DIR_YP,// z+ -> y+
	E_DIR_YM,// z- -> y-
	
	// y, inc theta
	//-------------
	E_DIR_ZP,// x+ -> z+
	E_DIR_ZM,// x- -> z-
	E_DIR_YP,// y+ -> y+
	E_DIR_YM,// y- -> y-
	E_DIR_XM,// z+ -> x-
	E_DIR_XP,// z- -> x+
	
	// y, dec theta
	//-------------
	E_DIR_ZM,// x+ -> z-
	E_DIR_ZP,// x- -> z+
	E_DIR_YP,// y+ -> y+
	E_DIR_YM,// y- -> y-
	E_DIR_XP,// z+ -> x+
	E_DIR_XM,// z- -> x-
	
	// z, inc theta
	//-------------
	E_DIR_YP,// x+ -> y+
	E_DIR_YM,// x- -> y-
	E_DIR_XM,// y+ -> x-
	E_DIR_XP,// y- -> x+
	E_DIR_ZP,// z+ -> z+
	E_DIR_ZM,// z- -> z-
	
	// z, dec theta
	//-------------
	E_DIR_YM,// x+ -> y-
	E_DIR_YP,// x- -> y+
	E_DIR_XP,// y+ -> x+
	E_DIR_XM,// y- -> x-
	E_DIR_ZP,// z+ -> z+
	E_DIR_ZM// z- -> z-
	
};

int totalPointCount;


bool LAST_COMPILE_ERROR = false;

char *BUF_NAMES[] =
{
	"ublock0",
	"ublock1",
	"ublock2",
	"ublock3",
	"ublock4",
	"ublock5",
	"ublock6",
	"ublock7",
	"ublock8",
	"ublock9",
	"ublock10",
	"ublock11",
	"ublock12",
	"ublock13"
};

const static int R_CHANNEL = 0;
const static int G_CHANNEL = 1;
const static int B_CHANNEL = 2;
const static int A_CHANNEL = 3;

const static int MAX_VG_FBOS = 2;
int CUR_VG_FBO = 0;

const static int MAX_VGT_FBOS = 2;
int CUR_VGT_FBO = 0;

const static float M_PI = 3.14159265359;

const static float TBDIR_ARR[] = {
	0.0f,
	M_PI*0.5f,
	M_PI,
	M_PI*1.5f
};


int PAGE_COUNT = 0;
// set to 0 to disable
int MAX_HOLDERS = 2048;


float MAX_GPU_MEM = 2560.0f;
float TOT_GPU_MEM_USAGE = 0.0f;

float MAX_CPU_MEM = 4096.0f;
float TOT_CPU_MEM_USAGE = 0.0f;

long long ENT_COUNTER = 0;

bool TRACE_ON = false;
//#define DEBUG_MODE 1





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



struct charArr {
	long size;
	char *data;
};

#define _USE_MATH_DEFINES

typedef unsigned int uint;


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

string SPACE_BUFFER[] = {" ", "  ", "   ", "    ", "     ", "      ", "       ", "        ", "         ", "          ", "           ", "            ", "             ", "              ", "               ", "                ", "                 ", "                  ", "                   ", "                    ", "                     ", "                      ", "                       ", "                        ", "                         ", "                          ", "                           ", "                            ", "                             ", "                              ", "                               ", "                                ", "                                 ", "                                  ", "                                   ", "                                    ", "                                     ", "                                      ", "                                       ", "                                        ", "                                         ", "                                          ", "                                           ", "                                            ", "                                             ", "                                              ", "                                               ", "                                                ", "                                                 ", "                                                  ", "                                                   ", "                                                    ", "                                                     ", "                                                      ", "                                                       ", "                                                        ", "                                                         ", "                                                          ", "                                                           ", "                                                            ", "                                                             ", "                                                              ", "                                                               ", "                                                                ", "                                                                 ", "                                                                  ", "                                                                   ", "                                                                    ", "                                                                     ", "                                                                      ", "                                                                       ", "                                                                        ", "                                                                         ", "                                                                          ", "                                                                           ", "                                                                            ", "                                                                             ", "                                                                              ", "                                                                               ", "                                                                                ", "                                                                                 ", "                                                                                  ", "                                                                                   ", "                                                                                    ", "                                                                                     ", "                                                                                      ", "                                                                                       ", "                                                                                        ", "                                                                                         ", "                                                                                          ", "                                                                                           ", "                                                                                            ", "                                                                                             ", "                                                                                              ", "                                                                                               ", "                                                                                                ", "                                                                                                 ", "                                                                                                  ", "                                                                                                   ", "                                                                                                    "};

#define glError() { \
		GLenum err = glGetError(); \
		while (err != GL_NO_ERROR) { \
			printf("glError: %s caught at %s:%u", \
						 (char*)gluErrorString(err), __FILE__, __LINE__); \
			err = glGetError(); \
			exit(-1); \
		} \
	}





// struct variable to store OpenGL info
struct glInfo
{
    std::string vendor;
    std::string renderer;
    std::string version;
    std::string glslVersion;
    std::vector <std::string> extensions;
    int redBits;
    int greenBits;
    int blueBits;
    int alphaBits;
    int depthBits;
    int stencilBits;
    int maxTextureSize;
    int maxLights;
    int maxAttribStacks;
    int maxModelViewStacks;
    int maxProjectionStacks;
    int maxClipPlanes;
    int maxTextureStacks;

    // ctor, init all members
    glInfo() : redBits(0), greenBits(0), blueBits(0), alphaBits(0), depthBits(0),
               stencilBits(0), maxTextureSize(0), maxLights(0), maxAttribStacks(0),
               maxModelViewStacks(0), maxClipPlanes(0), maxTextureStacks(0) {}

    void getInfo(unsigned int param=0);         // extract info
    void printSelf();                           // print itself
    bool isExtensionSupported(const std::string& ext); // check if a extension is supported
};



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


// function pointers for WGL_EXT_swap_control
#ifdef _WIN32
typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef int (WINAPI * PFNWGLGETSWAPINTERVALEXTPROC) (void);
PFNWGLSWAPINTERVALEXTPROC pwglSwapIntervalEXT = 0;
PFNWGLGETSWAPINTERVALEXTPROC pwglGetSwapIntervalEXT = 0;
#define wglSwapIntervalEXT      pwglSwapIntervalEXT
#define wglGetSwapIntervalEXT   pwglGetSwapIntervalEXT
#endif




// typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
// extern PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;

// static inline void init_EXT_Vsync()
// {
// wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)extgl_GetProcAddress( "wglSwapIntervalEXT" );
// }





const static GLenum bufNames[] = {
	GL_COLOR_ATTACHMENT0_EXT,
	GL_COLOR_ATTACHMENT1_EXT,
	GL_COLOR_ATTACHMENT2_EXT,
	GL_COLOR_ATTACHMENT3_EXT,
	GL_COLOR_ATTACHMENT4_EXT,
	GL_COLOR_ATTACHMENT5_EXT,
	GL_COLOR_ATTACHMENT6_EXT,
	GL_COLOR_ATTACHMENT7_EXT
};






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

// #include "Bullet3Common/b3Vector3.h"
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
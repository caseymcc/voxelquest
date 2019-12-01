#ifndef _voxelquest_gamestate_h_
#define _voxelquest_gamestate_h_

#include "voxelquest/enums.h"
#include "voxelquest/gameent.h"

#include <string>
#include <map>

enum E_MOVE_TYPES
{
	E_MT_NONE,
	E_MT_RELATIVE,
	E_MT_TRACKBALL,
	E_MT_LENGTH
};

class GameWorld;
class GameEntManager;
class GamePhysics;
class GameFluid;
class GameLogic;
class GameNetwork;
class GameAI;
class GameGUI;
class Shader;

class GameState
{
public:
    static GameState *singleton();

	void init();

    static FIVector4* cameraGetPos() { return &singleton()->resultCameraPos; }
    static FIVector4* cameraGetPosNoShake() { return singleton()->cameraPos; }

    static int &totPointCount(){return singleton()->TOT_POINT_COUNT;}
    static bool &wsBufferInvalid() {return singleton()->m_wsBufferInvalid;}
    static bool &forceGetPD() { return singleton()->m_forceGetPD; }
    static int &forceShadowUpdate() { return singleton()->m_forceShadowUpdate; }

	static void stopAllThreads();

	static int tbTicks;

	static FIVector4 lookAtVec;
	static FIVector4 origin;
	
	static bool markerFound;
	static FIVector4 worldMarker;

	static float subjectZoom;
	static float targetSubjectZoom;

	static bool refreshPaths;

	static float MAX_GPU_MEM;
	static float VERTEX_MEM_USAGE;
	static float TOT_GPU_MEM_USAGE;

	static float MAX_CPU_MEM;
	static float TOT_CPU_MEM_USAGE;

	static int TOT_POINT_COUNT;

	static long long ENT_COUNTER;

	static GameWorld* gw;
	static GameEntManager* gem;
	static GamePhysics* gamePhysics;
	static GameFluid* gameFluid[E_FID_LENGTH];
	static GameLogic* gameLogic;
	static GameNetwork* gameNetwork;
	static GameAI* gameAI;
	static GameGUI *ui;

private:
    GameState();

    std::vector<DynObject *> dynObjects;

	FIVector4* cameraPos;
    FIVector4 tempLerpPos;
    FIVector4 camLerpPos;
    FIVector4 resultCameraPos;
    FIVector4 targetCameraPos;
    FIVector4 baseCameraPos;

    

    bool m_wsBufferInvalid;
    bool m_forceGetPD;
    int m_forceShadowUpdate;
};

#endif//_voxelquest_gamestate_h_
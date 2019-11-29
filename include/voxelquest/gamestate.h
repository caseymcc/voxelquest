#ifndef _voxelquest_gamestate_h_
#define _voxelquest_gamestate_h_

#include "voxelquest/enums.h"
#include "voxelquest/gameent.h"

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

	static int tbTicks;

	static FIVector4 origin;
	
	static bool markerFound;
	static FIVector4 worldMarker;

	static float subjectZoom;
	static float targetSubjectZoom;

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

    int TOT_POINT_COUNT=0;

    bool m_wsBufferInvalid;
    bool m_forceGetPD;
    int m_forceShadowUpdate;
};

#endif//_voxelquest_gamestate_h_
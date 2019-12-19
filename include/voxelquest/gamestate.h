#ifndef _voxelquest_gamestate_h_
#define _voxelquest_gamestate_h_

#include "voxelquest/enums.h"
#include "voxelquest/gameent.h"
#include "voxelquest/vectors.h"
#include "voxelquest/timer.h"

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
class BaseObj;

class GameState
{
public:
//    static GameState *singleton();

    static void init(int w, int h);

    static void initGameItems(int w, int h);

    static int &totPointCount() { return GameState::TOT_POINT_COUNT; }

    static void stopAllThreads();

    static bool display(bool doFrameRender=true);
    static bool frameUpdate(bool doFrameRender=true);
    static void checkFluid(GameFluid* gf);
    static void syncObjects();

    static void funcNT2();
    static void startNT2();
    static bool stopNT2();
    static void funcNT();
    static void startNT();
    static bool stopNT();

    static bool cursorCapture;
    static bool helpCommands;
    static bool showMap;
    static bool showSettings;
    static bool orthoProjection;

    //static float cameraShake;
    //static Timer shakeTimer;
    static int frameCount;
    static int frameSkipCount;
    static int currentTick;

    static int fpsCountMax;
    static int fpsCount;

    static bool firstRun;
    static bool updateMatFlag;
    static int tbTicks;
    static bool allInit;

    //	static FIVector4 lookAtVec;
    static FIVector4 origin;

    static bool markerFound;
    static FIVector4 worldMarker;

    static float targetZoom;
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

    static bool timeMod;
    static double timeDelta;
    static double curTime;
    static float smoothTime;
    static double pauseTime;
    static double clickTime;
    static double mdTime;
    static double muTime;

    static int pathFindingStep;
    static FIVector4 lastHolderPos;

    static std::vector<RotationInfo> rotMatStack;
    static std::vector<DynObject *> dynObjects;

    static bool EDIT_POSE;
    static AxisRotation axisRotationInstance;

    static GameWorld* gw;
    static GameEntManager* gem;
    static GamePhysics* gamePhysics;
    static GameFluid* gameFluid[E_FID_LENGTH];
    static GameLogic* gameLogic;
    static GameNetwork* gameNetwork;
    static GameAI* gameAI;
    static GameGUI *ui;

//private:
//    GameState();

    static bool forceGetPD;
};

#endif//_voxelquest_gamestate_h_
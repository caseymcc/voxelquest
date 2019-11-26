#ifndef _voxelquest_gamestate_h_
#define _voxelquest_gamestate_h_

#include "voxelquest/gameent.h"

class GameState
{
public:
    static GameState *singleton();

    static FIVector4* cameraGetPos() { return &singleton()->resultCameraPos; }
    static FIVector4* cameraGetPosNoShake() { return singleton()->cameraPos; }

    static int &totPointCount(){return singleton()->TOT_POINT_COUNT;}
    static bool &wsBufferInvalid() {return singleton()->m_wsBufferInvalid;}
    static bool &forceGetPD() { return singleton()->m_forceGetPD; }
    static int &forceShadowUpdate() { return singleton()->m_forceShadowUpdate; }

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
#include "voxelquest/gamestate.h"
#include "voxelquest/enums.h"
#include "voxelquest/baseobject.h"
#include "voxelquest/gamelogic.h"
#include "voxelquest/threadpoolwrapper.h"
#include "voxelquest/renderer.h"
#include "voxelquest/gameworld.h"
#include "voxelquest/settings.h"
#include "voxelquest/gameaudio.h"
#include "voxelquest/gameentmanager.h"
#include "voxelquest/bullethelpers.h"
#include "voxelquest/gamegui.h"
#include "voxelquest/gamephysics.h"
#include "voxelquest/gameworld.h"
#include "voxelquest/gameai.h"
#include "voxelquest/gameplant.h"
#include "voxelquest/timer.h"
#include "voxelquest/hpclock.h"
#include "voxelquest/gamenetwork.h"
#include "voxelquest/gamefluid.h"

#include <algorithm>
#include <iostream>

Timer myTimer;
Timer fpsTimer;
HPClock bulletTimer;
ThreadWrapper threadNetSend;
ThreadWrapper threadNetRecv;

int GameState::frameCount=0;
int GameState::frameSkipCount=0;
int GameState::currentTick=0;
bool GameState::firstRun=true;
bool GameState::updateMatFlag=true;
bool GameState::allInit=false;

int GameState::fpsCountMax=500;
int GameState::fpsCount=0;

bool GameState::forceGetPD=false;

int GameState::tbTicks;
FIVector4 GameState::origin;

float GameState::targetZoom=1.0f;
float GameState::subjectZoom=1.0f;
float GameState::targetSubjectZoom=1.0f;
bool GameState::markerFound=false;
FIVector4 GameState::worldMarker;

bool GameState::refreshPaths=false;

float GameState::MAX_GPU_MEM=2048.0f;
float GameState::VERTEX_MEM_USAGE=0.0f;
float GameState::TOT_GPU_MEM_USAGE=0.0f;

float GameState::MAX_CPU_MEM=4096.0f;
float GameState::TOT_CPU_MEM_USAGE=0.0f;

int GameState::TOT_POINT_COUNT=0;

long long GameState::ENT_COUNTER=0;

bool GameState::timeMod=true;
double GameState::timeDelta;
double GameState::curTime;
float GameState::smoothTime=0.0f;
double GameState::pauseTime;
double GameState::clickTime;
double GameState::mdTime;
double GameState::muTime;

int GameState::pathFindingStep=0;
FIVector4 GameState::lastHolderPos;

std::vector<RotationInfo> GameState::rotMatStack;
std::vector<DynObject *> GameState::dynObjects;

bool GameState::EDIT_POSE=false;
AxisRotation GameState::axisRotationInstance;

GameWorld* GameState::gw=nullptr;
GameEntManager* GameState::gem=nullptr;
GamePhysics* GameState::gamePhysics=nullptr;
GameFluid* GameState::gameFluid[E_FID_LENGTH];
GameLogic* GameState::gameLogic=nullptr;
GameNetwork* GameState::gameNetwork=nullptr;
GameAI* GameState::gameAI=nullptr;
GameGUI *GameState::ui=nullptr;

//GameState *GameState::singleton()
//{
//    static GameState gameState;
//
//    return &gameState;
//}

//GameState::GameState()
//{
//	init();
//}

void GameState::init(int w, int h)
{
	tbTicks=0;
//    m_forceShadowUpdate=0;

    for(int i=0; i<E_OBJ_LENGTH; i++)
    {
        dynObjects.push_back(new DynObject());
    }

    dynObjects[E_OBJ_CAMERA]->init(0, 0, 0, 0, 0, 0, false, E_MT_NONE, NULL, 4.0f);

    int ccr=0;
    int ccg=0;
    int ccb=0;
    float tempf;

    for(int i=E_OBJ_LIGHT0; i<E_OBJ_LENGTH; i++)
    {
        int j=i-E_OBJ_LIGHT0;

        switch(j)
        {
        case 0:
            ccr=255;
            ccg=255;
            ccb=255;
            break;

        case 1:
            ccr=255;
            ccg=0;
            ccb=0;

            break;

        case 2:
            ccr=255;
            ccg=128;
            ccb=0;

            break;

        case 3:
            ccr=0;
            ccg=255;
            ccb=0;

            break;

        case 4:
            ccr=0;
            ccg=128;
            ccb=255;

            break;
        case 5:
            ccr=128;
            ccg=0;
            ccb=255;

            break;
        }

        if(i==E_OBJ_LIGHT0)
        {
            tempf=128.0f;
        }
        else
        {
            tempf=16.0f;
        }

        dynObjects[i]->init(
            -16+i*2,
            -16+i*2,
            4,
            ccr, ccg, ccb,
            true,
            E_MT_RELATIVE,
            &(dynObjects[E_OBJ_CAMERA]->pos),
            0.125f,
            tempf
        );
    }

    initGameItems(w, h);

    dynObjects[E_OBJ_LIGHT0]->moveType=E_MT_TRACKBALL;
    Renderer::cameraPos=&(dynObjects[E_OBJ_CAMERA]->pos);
}

void GameState::initGameItems(int w, int h)
{
//init game items
    gameNetwork=new GameNetwork();
    gameNetwork->init(nullptr);

    gameNetwork->socketConnect(true);

    for(int i=0; i<E_FID_LENGTH; i++)
    {
        gameFluid[i]=new GameFluid();
        gameFluid[i]->init(nullptr, i);
    }

    gameLogic=new GameLogic();
    gameLogic->init(nullptr);

    GameAudio::init();
    Renderer::init(w, h);

    ui=new GameGUI();
    ui->init(nullptr);

    GamePlant::initAllPlants(nullptr);
    
    gem=new GameEntManager();
    gem->init(nullptr);

    gw=new GameWorld();
    gw->init(nullptr);
    gw->initMap();

    // must be done after all are init
    for(int i=0; i<E_FID_LENGTH; i++)
    {
        gameFluid[i]->updateTBOData(true, false);
    }

    gameAI=new GameAI();
    gameAI->init(nullptr);

    ui->loadGUI();
    ui->loadValuesGUI();

    if(Renderer::bakeParamsOn)
        Renderer::doShaderRefresh(true);

    allInit=true;
    doTraceND("GW DONE");
    popTrace();
}

void GameState::stopAllThreads()
{
	glFlush();
	glFinish();
	gameLogic->threadPoolPath->stopAll();
	gameLogic->threadPoolList->stopAll();
	glFlush();
	glFinish();
}

bool GameState::display(bool doFrameRender)
{
    bool needSwap=false;
    bool noTravel=false;
    double milVox=(((double)(TOT_POINT_COUNT))/1000000.0);
    float maxPhysTime=getConst(E_CONST_STEP_TIME_IN_MICRO_SEC)*getConst(E_CONST_PHYS_STEPS_PER_FRAME);

    frameSkipCount++;

    curTime=myTimer.getElapsedTimeInMilliSec();
    smoothTime=(sinf((float)curTime/200.0f)+1.0f)*0.5f;

    if(timeMod)
        pauseTime=curTime;


    //    for(int i=0; i<MAX_KEYS; i++)
    //    {
    //        if(keysPressed[i])
    //        {
    //            break;
    //        }
    //    }
    //
    //    if(i!=MAX_KEYS)
    //    {
    //        frameMouseMove=true;
    //    }

    if(firstRun)
    {
        bulletTimer.reset();
    }

    unsigned long int curTimePassed=bulletTimer.getTimeMicroseconds();
    timeDelta=1.0/60.0;
    bulletTimer.reset();

    if(currentTick>4)
    {
        if(gamePhysics!=NULL)
        {
            if(threadNetRecv.isReady())
            {
                stopNT2();

                gameNetwork->applyNetworkActions();

                if(gameNetwork->isConnected)
                {
                    startNT2();
                }

            }

            if(threadNetSend.isReady())
            {
                stopNT();

                gameNetwork->flushNetworkActions();

                if(gameNetwork->isConnected)
                {
                    startNT();
                }

            }

            gameLogic->applyBehavior();

            if(g_settings.settings[E_BS_TURN_BASED])
            {

            }
            else
            {

            }

            //            flushKeyStack();
            //            gatherKeyActions();
            Renderer::handleMovement();

            if(g_settings.settings[E_BS_PHSYICS])
            {
                gamePhysics->updateAll();
            }

            gamePhysics->updateBullets();
        }
    }
    
    if(
        //true  
        // ( 
        // 	((frameSkipCount%frameSkip) == 0) &&
        // 	(frameMouseMove||ignoreFrameLimit)
        // ) || g_settings.fpsTest
        //totTimePassedGraphics > 8000

        true
        )
    {
        //cout << "totTimePassedGraphics " << totTimePassedGraphics << "\n";
        //totTimePassedGraphics -= 8000;

//       frameMouseMove=false;
//
//       if(firstRun)
//       {
//
//       }
//       else
//       {
//           // if ( (frameCount % 3000) == 0)
//           // {
//           // 	gw->updateLights();
//           // }
//       }
//
//       //lastTime = curTime;
//       timeOfDay+=(getTargetTimeOfDay()-timeOfDay)/8.0;
//
//       if(
//           lbDown &&
//           gem->isDraggingObject&&
//           (gem->draggingFromType!=E_DT_NOTHING)&&
//           ((curTime-mdTime)>300)&&
//           (!settings[E_BS_EDIT_POSE])
//           )
//       {
//           glutSetCursor(GLUT_CURSOR_CROSSHAIR);
//
//           mouseMoveVec.setFXYZ(lastMouseX, lastMouseY, 0.0);
//           noTravel=mouseMoveVec.distance(&mouseStart)<MAX_TRAVEL_DIS;
//           markerFound=!noTravel;
//
//           getMarkerPos(lastMouseX, lastMouseY);
//           //getPixData(&worldMarker, lastMouseX, lastMouseY, true);
//
//       }
//       else
//       {
//           markerFound=(menuList[E_FM_DDMENU]->visible)&&(gem->selObjInd<E_OBJ_LENGTH);
//           glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
//       }
//
//       mouseMovingLoc=(mouseMovingLoc+1)%mouseMovingSize;
//       mouseCount++;
//       mouseMoving[mouseMovingLoc].setIXY(lastMouseX, lastMouseY);

        if(Renderer::shadersAreLoaded)
        {
//            if(traceOn)
//            {
//                traceOn=false;
//                TRACE_ON=true;
//            }
            if(gw->updateLock)
            {

            }
            else
            {
                //if (doFrameRender) {
                if(frameUpdate(doFrameRender))
                    needSwap=true;

                gw->lastDepthInvalidMove=gw->depthInvalidMove;
                gw->depthInvalidMove=false;
                gw->depthInvalidRotate=false;
                //}
            }
        }

        if(g_settings.fpsTest)
        {
            fpsCount++;

            if(fpsCount==fpsCountMax)
            {

                //g_settings.fpsTest=false;
//                speak("end FPS timer");
                std::cout<<"\nNumber of voxels (in millions): "<<milVox<<"\n";
                std::cout<<"Available pdPool: "<<MemoryPool::getAvailPD()<<"\n";
                std::cout<<"FPS: "<<1.0/(fpsTimer.getElapsedTimeInSec()/((double)(fpsCountMax)))<<"\n";
                std::cout<<"Microseconds per frame: "<<(fpsTimer.getElapsedTimeInMilliSec()*1000.0/((double)(fpsCountMax)))<<"\n";
                std::cout<<"Microseconds per million voxels: "<<(fpsTimer.getElapsedTimeInMilliSec()*1000.0/(
                    ((double)(fpsCountMax))*
                    milVox
                    ))<<"\n\n";

                fpsTimer.stop();
                //myDynBuffer->setVsync(true);
            }
        }
    }

    if(firstRun)
    {
        //toggleFullScreen();
    }
    firstRun=false;

    return needSwap;
}

bool GameState::frameUpdate(bool doFrameRender)
{
    bool needSwap=false;
    float temp;
    float temp2;

    if(firstRun)
    {

    }
    else
    {
    }

    if(updateMatFlag&&(!gw->matVolLock))
    {
        updateMatFlag=false;
        gw->updateMatVol();
    }


    if(g_settings.ignoreFrameLimit)
    {
        subjectZoom+=(targetSubjectZoom-subjectZoom)/32.0f;
        Renderer::cameraZoom+=(targetZoom-Renderer::cameraZoom)/4.0f;
    }
    if(Renderer::cameraZoom<1.0f)
        Renderer::cameraZoom=1.0f;

//    if(!g_settings.settings[E_BS_PLACING_GEOM])
//    {
//        if(abs(globWheelDelta)>0.001f)
//        {
//            if(gem->getCurActor()!=NULL)
//            {
//                subjectDelta-=globWheelDelta;
//                targetSubjectZoom=pow(2.0, subjectDelta);
//                if(!ignoreFrameLimit)
//                {
//                    subjectZoom=targetSubjectZoom;
//                }
//            }
//        }
//    }
//    globWheelDelta*=0.5f;

    
    float hs=(float)g_settings.cellsPerHolder;
    float scrollAmount=0.0f;

    GameAudio::updateAmbientSounds();
    if((ui!=NULL))
    {
        if(ui->isReady)
        {
            //ui->testOver(guiX, guiY);
        }
    }
    syncObjects();

    bool guiValid=false;

    if((ui!=NULL)&&(ui->isReady))
        guiValid=true;

    if(guiValid && ui->anyMenuVisible())
    {
        ui->updateGUI();
        needSwap=true;
    }

    if((gw->mapLockOn)||(gw->mapInvalid))
    {
    }
    else
    {
        if(guiValid)
        {
            currentTick++;

            if(gem->getCurActor()!=NULL)
            {
                if((currentTick%10)==0)
                {

                    // if (rbDown) {
                    // 	makeShoot(gem->getCurActor()->uid, E_ENTTYPE_TRACE);
                    // }


                }
            }

            if(currentTick>2)
            {
                Renderer::updateCamVals();

                if(currentTick<4)
                {
                    Renderer::cameraGetPosNoShake()->setFXYZ(3192.0, 3192.0, 0.0);
                    Renderer::camLerpPos.copyFrom(Renderer::cameraGetPosNoShake());
                }

                if(currentTick==4)
                {
                    Renderer::setCameraToElevation();

                    gamePhysics=new GamePhysics();
                    gamePhysics->init(nullptr);

                    GLint maxTBO;
                    glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxTBO);
                    std::cout<<"GL_MAX_TEXTURE_BUFFER_SIZE "<<maxTBO<<"\n";
                }

                if((gem->getCurActor()!=NULL)&&gem->getCurActor()->hasBodies())
                {
                    if(gem->getCurActor()->bodies[E_BDG_CENTER].inWater)
                    {
                        temp=clampfZO(
                            gem->getCurActor()->getVel(0)->length()
                        )*0.25f;
                        temp2=0.0f;
                    }
                    else
                    {

                        if(gem->getCurActor()->allFalling())
                        {
                            temp2=0.0f;
                        }
                        else
                        {
                            temp2=clampfZO(
                                gem->getCurActor()->getVel(0)->length()
                            );
                        }

                        temp=0.0f;
                    }

                    FIVector4 centerPoint=convertToVQV(gem->getCurActor()->getCenterPoint(E_BDG_CENTER));

                    GameAudio::updateSoundPosAndPitch(
                        "swimming0",
                        Renderer::cameraGetPosNoShake(),
                        &centerPoint,
                        temp*0.2f,
                        0.01f
                    );
                    GameAudio::updateSoundPosAndPitch(
                        "walkinggravel0",
                        Renderer::cameraGetPosNoShake(),
                        &centerPoint,
                        temp2*0.2f,
                        0.1f
                    );
                }

                checkFluid(gameFluid[E_FID_BIG]);
            }

            if((currentTick>4)&&allInit)
            {
                gw->preUpdate();
                gw->update();
//
//                if(g_settings.settings[E_BS_TURN_BASED])
//                {
//                    if(((tbTicks%iGetConst(E_CONST_TURNBASED_TICKS))==0)/* || (gem->getCurActor() != NULL)*/)
//                        gem->cycleTurn();
//                    tbTicks++;
//                }
                needSwap=true;
            }
        }
    }

    g_settings.TRACE_ON=false;
    frameCount++;
    return needSwap;
}

void GameState::checkFluid(GameFluid* gf)
{
    gameLogic->loadNearestHolders(g_settings.settings[E_BS_UPDATE_HOLDERS]);
    gw->rastChunk(iGetConst(E_CONST_RASTER_CHUNK_RAD), 1);// RH_FLAG_DOCHECK);
//    gw->rasterHoldersCheck();

    return;

//    if((!draggingMap)&&(!g_settings.fpsTest)&&settings[E_BS_UPDATE_HOLDERS])
//    {
//        gf->updateAll();
//
//        if(gf->fluidReading)
//        {
//            if(gf->proceedingToRead)
//            {
//
//                if(gf->waitingOnThreads)
//                {
//                    gf->tryToEndThreads();
//                }
//                else
//                {
//                    gf->tryToEndRead();
//                }
//
//
//            }
//            else
//            {
//                gf->proceedWithRead();
//            }
//        }
//
//
//        if(gf->readyForTermination)
//        {
//
//
//            if(
//                gf->anyThreadsRunning()
//                )
//            {
//
//            }
//            else
//            {
//                gf->readyForTermination=false;
//                gf->cycleTerminated=true;
//            }
//
//
//        }
//
//        if(gf->cycleTerminated)
//        {
//
//            gameLogic->loadNearestHolders(settings[E_BS_UPDATE_HOLDERS]);
//            holderLoadCount++;
//
//            if(holderLoadCount==MAX_HOLDER_LOAD_COUNT)
//            {
//                holderLoadCount=0;
//                gf->cycleTerminated=false;
//                gf->startFT();
//            }
//
//
//        }
//    }
}

void GameState::syncObjects()
{
    FIVector4 tempVec1;
    FIVector4 tempVec2;
    int i;
    float xrp;
    float yrp;

    float xrotrad=Renderer::getCamRot(0);
    float yrotrad=Renderer::getCamRot(1);

    for(i=1; i<E_OBJ_LENGTH; i++)
    {
        if(dynObjects[i]->moveType==E_MT_TRACKBALL)
        {

            xrp=xrotrad+dynObjects[i]->posTrackball.getFX()/200.0f;
            yrp=yrotrad+dynObjects[i]->posTrackball.getFY()/200.0f;

            angleToVec(&tempVec1, xrp, yrp);
            tempVec1.multXYZ(dynObjects[i]->posTrackball.getFZ()*0.5f+2.0f);

            dynObjects[i]->pos.copyFrom(Renderer::cameraGetPosNoShake());
            dynObjects[i]->pos.addXYZRef(&(tempVec1));
        }
        else
        {
            if(dynObjects[i]->moveType==E_MT_RELATIVE)
            {
                dynObjects[i]->pos.copyFrom(Renderer::cameraGetPosNoShake());
                dynObjects[i]->pos.addXYZRef(&(dynObjects[i]->posRel));
            }
        }
    }
}


void GameState::funcNT2()
{
    threadNetRecv.setRunningLocked(true);
    gameNetwork->updateRecv();
    threadNetRecv.setRunningLocked(false);
}

void GameState::startNT2()
{
    if(threadNetRecv.threadRunning)
    {

    }
    else
    {
        threadNetRecv.threadRunning=true;
        threadNetRecv.threadMain=std::thread(&GameState::funcNT2);
    }
}

bool GameState::stopNT2()
{
    bool didStop=false;
    if(threadNetRecv.threadRunning)
    {
        threadNetRecv.threadMain.join();
        threadNetRecv.threadRunning=false;
        didStop=true;
    }
    return didStop;
}

void GameState::funcNT()
{
    threadNetSend.setRunningLocked(true);
    gameNetwork->updateSend();
    threadNetSend.setRunningLocked(false);
}

void GameState::startNT()
{
    if(threadNetSend.threadRunning)
    {

    }
    else
    {
        threadNetSend.threadRunning=true;
        threadNetSend.threadMain=std::thread(&GameState::funcNT);
    }
}

bool GameState::stopNT()
{
    bool didStop=false;

    if(threadNetSend.threadRunning)
    {
        threadNetSend.threadMain.join();
        threadNetSend.threadRunning=false;
        didStop=true;
    }
    return didStop;

}
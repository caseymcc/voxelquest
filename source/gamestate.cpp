#include "voxelquest/gamestate.h"
#include "voxelquest/emuns.h"

GameState *GameState::singleton()
{
    static GameState gameState;

    return &gameState;
}

GameState::GameState()
{
    m_wsBufferInvalid=true;
    m_forceGetPD=false;
    m_forceShadowUpdate=0;

    for(i=0; i<E_OBJ_LENGTH; i++)
    {
        dynObjects.push_back(new DynObject());
    }

    dynObjects[E_OBJ_CAMERA]->init(0, 0, 0, 0, 0, 0, false, E_MT_NONE, NULL, 4.0f);

    for(i=E_OBJ_LIGHT0; i<E_OBJ_LENGTH; i++)
    {

        j=i-E_OBJ_LIGHT0;

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

    dynObjects[E_OBJ_LIGHT0]->moveType=E_MT_TRACKBALL;
    cameraPos=&(dynObjects[E_OBJ_CAMERA]->pos);
}

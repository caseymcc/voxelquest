#include "voxelquest/gamechunk.h"
#include "voxelquest/gamelogic.h"
#include "voxelquest/gamestate.h"
#include "voxelquest/renderer.h"

bool GameChunk::staticInit=false;
VIStruct GameChunk::chunkVI[NUM_MIP_LEVELS_WITH_FIRST];
float GameChunk::mipDis[8];

GameChunk::GameChunk()
{
    if(!staticInit)
    {
        mipDis[0]=getConst(E_CONST_MIPDIS0);
        mipDis[1]=getConst(E_CONST_MIPDIS1);
        mipDis[2]=getConst(E_CONST_MIPDIS2);
        mipDis[3]=getConst(E_CONST_MIPDIS3);
        mipDis[4]=getConst(E_CONST_MIPDIS4);
        mipDis[5]=getConst(E_CONST_MIPDIS5);
        mipDis[6]=getConst(E_CONST_MIPDIS6);
        mipDis[7]=getConst(E_CONST_MIPDIS7);
        staticInit=true;
    }

    lastPointCount=0;
    changeFlag=false;
    changeTick=1;
    isDirty=false;

    readyToRender=false;
    listEmpty=true;

    mipLev=0;

}

void GameChunk::init(
    Singleton* _singleton,
    int _blockId,
    int _chunkId,

    int trueX,
    int trueY,
    int trueZ

)
{
    singleton=_singleton;

    blockId=_blockId;
    chunkId=_chunkId;

    int i;

    float cellsPerChunk=(float)g_settings.cellsPerChunk;

    for(i=0; i<NUM_MIP_LEVELS_WITH_FIRST; i++)
    {
        vboWrapper[i].setVI(&(chunkVI[i]), true);
    }

    holdersPerChunk=g_settings.holdersPerChunk;

    offsetInChunks.setIXYZ(trueX, trueY, trueZ);
    chunkCenInCells.copyFrom(&offsetInChunks);
    chunkCenInCells.addXYZ(0.5f);
    chunkCenInCells.multXYZ(cellsPerChunk);

    iHolderSize=holdersPerChunk*holdersPerChunk * holdersPerChunk;
    holderData=new GamePageHolder*[iHolderSize];
    for(i=0; i<iHolderSize; i++)
    {
        holderData[i]=NULL;
    }

}

void GameChunk::makeDirty()
{
    isDirty=true;
}

VBOWrapper* GameChunk::getCurVBO()
{
    return &(vboWrapper[mipLev]);
}

void GameChunk::drawLoadingHolders()
{
    int i;
    GamePageHolder* curHolder;

    for(i=0; i<iHolderSize; i++)
    {
        curHolder=holderData[i];

        if(curHolder==NULL)
        {

        }
        else
        {
            if(curHolder->lockWrite)
            {
                Renderer::drawBox(&(curHolder->gphMinInCells), &(curHolder->gphMaxInCells));
            }
        }
    }

}

void GameChunk::checkHolders()
{
    int i;

    FIVector4 testPos;

    float cellsPerChunk=(float)g_settings.cellsPerChunk;

    int testMip=-1;
    testPos.copyFrom(&offsetInChunks);
    testPos.addXYZ(0.5f);
    testPos.multXYZ(cellsPerChunk);

    float testDis=testPos.distance(Renderer::cameraGetPosNoShake());

    testDis/=cellsPerChunk;

    for(i=0; i<NUM_MIP_LEVELS_WITH_FIRST; i++)
    {
        if(
            (testDis>=mipDis[i])&&
            (testDis<mipDis[i+1])
            )
        {
            testMip=i;
            break;
        }
    }

    if(testMip==-1)
    {
        testMip=NUM_MIP_LEVELS;
    }
    if(testMip>NUM_MIP_LEVELS)
    {
        testMip=NUM_MIP_LEVELS;
    }


    mipLev=testMip;

    bool foundDirty=false;

    int maxTicks=iGetConst(E_CONST_MAX_CHUNK_TICKS);
//    GamePageHolder* curHolder;

    if(isDirty)
    {
        // for (i = 0; i < iHolderSize; i++) {
        // 	curHolder = holderData[i];

        // 	if (curHolder == NULL) {

        // 	}
        // 	else {
        // 		if (curHolder->lockWrite) {
        // 			foundDirty = true;
        // 		}
        // 		else {
        // 			if (curHolder->isDirty) {
        // 				foundDirty = true;
        // 			}
        // 		}
        // 	}
        // }

        // if (foundDirty) {

        // }
        // else {
        // 	fillVBO();
        // }

        if(GameState::gameLogic->dirtyStack)
        {

        }
        else
        {
            fillVBO();
        }



    }
    else
    {
        if(g_settings.settings[E_BS_UPDATE_HOLDERS])
        {
            if(
                changeFlag
                // || (abs(testMip-mipLev) > 2) ||
                // (
                // 	(testMip == 0) &&
                // 	(mipLev != 0)	
                // ) ||
                // (
                // 	(testMip == NUM_MIP_LEVELS) &&
                // 	(mipLev != NUM_MIP_LEVELS)	
                // )
                )
            {
                changeTick++;

                if(((changeTick%maxTicks)==0))
                {
                    fillVBO();
                }
            }
        }
    }








}

void GameChunk::reset()
{
    int i;
    changeTick=1;
    //changeCount = 0;
    for(i=0; i<NUM_MIP_LEVELS_WITH_FIRST; i++)
    {
        vboWrapper[i].deallocVBO();
    }
}

void GameChunk::fillVBO()
{

    isDirty=false;
    changeFlag=false;

    //cout << "fillVBO a\n";

    readyToRender=false;

    int totFloats=0;
    int i;
    int j;
    int k;

    int curSize;

    GamePageHolder* curHolder;


    for(j=0; j<NUM_MIP_LEVELS_WITH_FIRST; j++)
    {


        vboWrapper[j].beginFill();


        if(vboWrapper[j].hasInit)
        {

        }
        else
        {
            vboWrapper[j].init(
                2,
                (int)GL_STATIC_DRAW
            );
        }


        for(i=0; i<iHolderSize; i++)
        {
            curHolder=holderData[i];

            if(curHolder==NULL)
            {

            }
            else
            {
                if(curHolder->lockWrite)
                {

                }
                else
                {
                    if(curHolder->listGenerated)
                    {

                        curSize=(int)curHolder->vertexVec.size();

                        for(k=curHolder->begMip[j]; k<curHolder->endMip[j]; k++)
                        {

                            if(k<curSize)
                            {
                                vboWrapper[j].vi->vertexVec.push_back(curHolder->vertexVec[k]);
                            }

                        }

                        // curHolder->vertexVec.clear();
                        // curHolder->vertexVec.shrink_to_fit();

                    }
                }
            }
        }

        if(j==0)
        {
            listEmpty=(vboWrapper[j].vi->vertexVec.size()==0);
            GameState::totPointCount()-=lastPointCount;
            GameState::totPointCount()+=vboWrapper[j].getNumVerts();
            lastPointCount=vboWrapper[j].getNumVerts();
        }


        vboWrapper[j].endFill();

        glFlush();
        glFinish();


        vboWrapper[j].clearVecs(false);

        Renderer::wsBufferInvalid=true;
        GameState::forceGetPD=true;
    }




    changeTick=1;
    //changeCount = 0;

    readyToRender=true;
    Renderer::forceShadowUpdate=32;

    //cout << "fillVBO b\n";

}

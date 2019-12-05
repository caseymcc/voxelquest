#include "voxelquest/gameworld.h"
#include "voxelquest/settings.h"
#include "voxelquest/gamestate.h"
#include "voxelquest/gameentmanager.h"
#include "voxelquest/gamephysics.h"
#include "voxelquest/gameaudio.h"
#include "voxelquest/bullethelpers.h"
#include "voxelquest/renderer.h"
#include "voxelquest/gamefluid.h"
#include "voxelquest/imageloader.h"
#include "voxelquest/gameorg.h"
#include "voxelquest/geom.h"
#include "voxelquest/fontwrapper.h"
#include "voxelquest/gamegui.h"
#include "voxelquest/gamelogic.h"
#include "voxelquest/jsonhelpers.h"

#include <algorithm>
#include <iostream>

const static uint RH_FLAG_DOCHECK=1;
const static uint RH_FLAG_DRAWLOADING=2;
const static uint RH_FLAG_CLIPTOVIEW=4;

GameWorld::GameWorld()
{
    timeOfDay=1.0f;
    targetTimeOfDay=1.0f;

    curBrushRad=2.0f;
    earthMod=E_PTT_TER;

    curPrimTemplate=1;

    medianCount=0;
    actorCount=0;

    depthInvalidMove=true;
    lastDepthInvalidMove=true;
    depthInvalidRotate=true;
    amountInvalidMove=0.0f;

    updateHolderLookat=true;
    updateLock=false;
    mapInvalid=true;

    lightChanged=false;

//    fpsTest=false;

    mapFreqs.setFXYZW(
        1.0f,
        4.0f,
        16.0f,
        32.0f
    );
    mapAmps.setFXYZW(
        16.0f/16.0f,
        2.0f/16.0f,
        1.0f/16.0f,
        0.25f/16.0f
    );
    // Notes
    // -----
    // Depth: 16 bits
    // Material: 16 bits
    // AO val: 8 bits, normal: 24 bits

    blockHolder=NULL;
    holderInFocus=NULL;

    primArr=new float[MAX_ZO_CUBES*4*2];
    limbArr=new float[MAX_ZO_CUBES*4*2];
    splashArr=new float[MAX_SPLASHES*4];
    explodeArr=new float[MAX_EXPLODES*4];
    paramArrMap=new float[4096];

    imageVoro=loadBMP("..\\data\\voro.bmp");
    imageHM0=loadBMP("..\\data\\hm0.bmp");
    imageHM1=loadBMP("..\\data\\hm1.bmp");
    cloudImage=loadBMP("..\\data\\clouds.bmp");

    //invItems->getTextureId(GL_NEAREST);
    imageVoro->getTextureId(GL_NEAREST);
    imageHM0->getTextureId(GL_NEAREST);
    imageHM1->getTextureId(GL_NEAREST);
    cloudImage->getTextureId(GL_LINEAR);

    mapPitch=(imageHM0->width);

    matVolLock=false;
    matVolDim.setIXYZ(64, 64, 256);
    matVolSize=matVolDim.getIX()*matVolDim.getIY()*matVolDim.getIZ();
    matVol=new uint[matVolSize];
    matSlice0=new materialNode[matVolDim.getIX()*matVolDim.getIY()];
    matSlice1=new materialNode[matVolDim.getIX()*matVolDim.getIY()];
    matSlice2=new materialNode[matVolDim.getIX()*matVolDim.getIY()];

    for(int i=0; i<matVolSize; i++)
    {
        matVol[i]=0;
    }

    volumeTex.init(matVolDim.getIX(), matVolDim.getIY(), matVolDim.getIZ());
//    primTBO.init(true, primTBOData, NULL, MAX_PRIM_DATA_IN_BYTES);
//    limbTBO.init(true, limbTBOData, NULL, MAX_LIMB_DATA_IN_BYTES);

    float tempData[16];

    for(int i=0; i<MAX_ZO_CUBES; i++)
    {
        tempData[0]=(float)i;
        zoCubes.vboBox(
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f,
            ALL_FACES,
            tempData,
            4
        );
    }
    zoCubes.init(
        2,
        (int)GL_STATIC_DRAW
    );
    zoCubes.updateNew();

    curPattern=E_PAT_5X5DIAMOND;
    curPatternRot=0;
    generatePatterns();
}

GameWorld::~GameWorld()
{
    delete primArr;
    delete limbArr;
    delete splashArr;
    delete explodeArr;
    delete paramArrMap;

    delete matVol;
    delete matSlice0;
    delete matSlice1;
    delete matSlice2;
    //threadpool.stopAll();

//    int i;
//    int j;

}

void GameWorld::init(Singleton *_singleton)
{

    singleton=_singleton;

    chunksPerBlock=g_settings.chunksPerBlock;
    chunksPerWorld=g_settings.chunksPerWorld;
    holdersPerChunk=g_settings.holdersPerChunk;

    int i;
//    int j;

    skippedPrim=false;

    shiftCounter=0;

    curTargFBO[0]="terTargFBO";
    curTargFBO[1]="sphTargFBO";
    curTargFBO[2]="prmTargFBO";

    curDepthFBO[0]="terDepthFBO";
    curDepthFBO[1]="sphDepthFBO";
    curDepthFBO[2]="prmDepthFBO";

    lastHitObjUID=0;
    renderCount=0;
    invalidCount=0.0f;
    invalidCountMax=10.0f;

    //finalInd = -1;

    lightCount=1;

    noiseGenerated=false;

    //finalPath = NULL;


    float deltVal=-1.5f;
    offsetVal[0]=btVector3(deltVal, 0.0f, 0.0f);
    offsetVal[1]=btVector3(0.0f, deltVal, 0.0f);
    offsetVal[2]=btVector3(0.0f, 0.0f, deltVal);
    offsetVal[3]=btVector3(0.0f, 0.0f, 0.0f);


    holdersPerBlock=g_settings.holdersPerBlock;

    dirFlagsO[0]=1;
    dirFlagsO[1]=2;
    dirFlagsO[2]=4;
    dirFlagsO[3]=8;

    dirFlagsOpO[0]=2;
    dirFlagsOpO[1]=1;
    dirFlagsOpO[2]=8;
    dirFlagsOpO[3]=4;

    dirFlags[0]=~1;
    dirFlags[1]=~2;
    dirFlags[2]=~4;
    dirFlags[3]=~8;

    dirFlagsOp[0]=~2;
    dirFlagsOp[1]=~1;
    dirFlagsOp[2]=~8;
    dirFlagsOp[3]=~4;

    dirModX[0]=1;
    dirModX[1]=-1;
    dirModX[2]=0;
    dirModX[3]=0;
    dirModX[4]=0;
    dirModX[5]=0;

    dirModY[0]=0;
    dirModY[1]=0;
    dirModY[2]=1;
    dirModY[3]=-1;
    dirModY[4]=0;
    dirModY[5]=0;

    dirModZ[0]=0;
    dirModZ[1]=0;
    dirModZ[2]=0;
    dirModZ[3]=0;
    dirModZ[4]=1;
    dirModZ[5]=-1;

    opDir[0]=1;
    opDir[1]=0;
    opDir[2]=3;
    opDir[3]=2;

    dirFlagClear=~15;
    visFlag=16;
    visFlagO=~16;

    foundPath=false;

    activeFBO=0;
    mapLockOn=false;

    numProvinces=16;
    provinceGrid=new int[numProvinces * numProvinces];
    provinceX=new int[numProvinces];
    provinceY=new int[numProvinces];

    mapSwapFlag=0;
    mapStep=0.0f;

    pageCount=0;
    maxThreads=7;
    availThreads=maxThreads;

    for(i=0; i<maxThreads; i++)
    {
        ocThreads.push_back(-1);
    }

    hmChannel=0;
    idChannel=1;
    densityChannel=2;
    blockChannel=3;

    stChannel=0;
    btChannel=1;
    pathChannel=2;
    houseChannel=3;




    MIN_MIP=0;
    MAX_MIP=1;
    AVG_MIP=2;


    doDrawFBO=false;


    renderMethod=(int)E_RENDER_VOL;

    cellsPerWorld=g_settings.cellsPerWorld;
    holdersPerWorld=g_settings.holdersPerWorld;
    blocksPerWorld=g_settings.blocksPerWorld;


    iBlockSize=blocksPerWorld*blocksPerWorld * blocksPerWorld;



    blockData=new GameBlock*[iBlockSize];
    for(i=0; i<iBlockSize; i++)
    {
        blockData[i]=NULL;
    }

    // if (GEN_POLYS_WORLD) {
    // 	blockHolder = new GamePageHolder();
    // 	blockHolder->init(singleton, -1, -1, 0,0,0, true);
    // }



}




// x, y, and z are measured in blocks
GameBlock *GameWorld::getBlockAtCoords(
    int xInBlocks,
    int yInBlocks,
    int zInBlocks,
    bool createOnNull
)
{





    int newX=wrapCoord(xInBlocks, blocksPerWorld);
    int newY=wrapCoord(yInBlocks, blocksPerWorld);
    int newZ=wrapCoord(zInBlocks, blocksPerWorld);

    //zInBlocks;//


    int ind=
        newZ*blocksPerWorld*blocksPerWorld+
        newY*blocksPerWorld+
        newX;

    if(blockData[ind]==NULL)
    {
        if(createOnNull)
        {
            blockData[ind]=new GameBlock();
            blockData[ind]->init(singleton, ind, xInBlocks, yInBlocks, zInBlocks, newX, newY, newZ);
        }
    }

    return blockData[ind];

}

void GameWorld::clearAllHolders()
{
    GameState::stopAllThreads();

    GamePageHolder* gph;

    int i;

    for(i=0; i<gamePageHolderList.size(); i++)
    {
        gph=getHolderAtId(gamePageHolderList[i].v0, gamePageHolderList[i].v1, gamePageHolderList[i].v2);
        if(gph!=NULL)
        {
            gph->reset(true);
        }
    }

    GameState::totPointCount()=0;

    glFlush();
    glFinish();

}





// x, y, and z are measured in chunks
GameChunk* GameWorld::getChunkAtCoords(int x, int y, int z, bool createOnNull)
{

    GameChunk **chunkData;


    int newX=wrapCoord(x, chunksPerWorld);
    int newY=wrapCoord(y, chunksPerWorld);
    int newZ=z;

    int chunkX=newX-intDiv(newX, chunksPerBlock) * chunksPerBlock;
    int chunkY=newY-intDiv(newY, chunksPerBlock) * chunksPerBlock;
    int chunkZ=newZ-intDiv(newZ, chunksPerBlock) * chunksPerBlock;

    int chunkId=chunkZ*chunksPerBlock * chunksPerBlock+chunkY*chunksPerBlock+chunkX;


    GameBlock *curBlock=getBlockAtCoords(
        intDiv(x, chunksPerBlock),
        intDiv(y, chunksPerBlock),
        intDiv(z, chunksPerBlock),
        createOnNull
    );

    if(curBlock==NULL)
    {
        return NULL;
    }
    else
    {
        chunkData=curBlock->chunkData;


        if(chunkData[chunkId]==NULL)
        {
            if(createOnNull)
            {
                chunkData[chunkId]=new GameChunk();
                chunkData[chunkId]->init(singleton, curBlock->blockId, chunkId, x, y, z); //, x, y, z
            }
        }

        return chunkData[chunkId];


    }



}






// x, y, and z are measured in holders
GamePageHolder *GameWorld::getHolderAtCoords(int x, int y, int z, bool createOnNull)
{

    GamePageHolder **holderData;

    intTrip ip;

    int newX=wrapCoord(x, holdersPerWorld);
    int newY=wrapCoord(y, holdersPerWorld);
    int newZ=z;

    int holderX=newX-intDiv(newX, holdersPerChunk) * holdersPerChunk;
    int holderY=newY-intDiv(newY, holdersPerChunk) * holdersPerChunk;
    int holderZ=newZ-intDiv(newZ, holdersPerChunk) * holdersPerChunk;

    int holderId=holderZ*holdersPerChunk * holdersPerChunk+holderY*holdersPerChunk+holderX;


    GameBlock *curBlock=getBlockAtCoords(
        intDiv(x, holdersPerBlock),
        intDiv(y, holdersPerBlock),
        intDiv(z, holdersPerBlock),
        createOnNull
    );

    GameChunk* curChunk;

    if(curBlock==NULL)
    {
        return NULL;
    }
    else
    {

        curChunk=getChunkAtCoords(
            intDiv(x, holdersPerChunk),
            intDiv(y, holdersPerChunk),
            intDiv(z, holdersPerChunk),
            createOnNull
        );


        if(curChunk==NULL)
        {
            return NULL;
        }
        else
        {
            holderData=curChunk->holderData;


            if(holderData[holderId]==NULL)
            {
                if(createOnNull)
                {
                    holderData[holderId]=new GamePageHolder();
                    ip.v0=curBlock->blockId;
                    ip.v1=curChunk->chunkId;
                    ip.v2=holderId;
                    gamePageHolderList.push_back(ip);
                    holderData[holderId]->init(
                        singleton,
                        curBlock->blockId,
                        curChunk->chunkId,
                        holderId,
                        x, y, z
                    ); //, x, y, z
                }
            }

            return holderData[holderId];
        }




    }



}



// assumed that holder exists when calling this
GameChunk *GameWorld::getChunkAtId(int blockId, int chunkId)
{

    if(blockData[blockId]==NULL)
    {
        return NULL;
    }
    else
    {
        return blockData[blockId]->chunkData[chunkId];
    }
}

// assumed that holder exists when calling this
GamePageHolder *GameWorld::getHolderAtId(int blockId, int chunkId, int holderId)
{

    if(blockData[blockId]==NULL)
    {
        return NULL;
    }
    else
    {
        if(blockData[blockId]->chunkData[chunkId]==NULL)
        {
            return NULL;
        }
        else
        {
            return blockData[blockId]->chunkData[chunkId]->holderData[holderId];
        }
    }
}

// assumed that block exists when calling this
GameBlock *GameWorld::getBlockAtId(int id)
{


    if((id<0)||(id>=iBlockSize))
    {
        doTraceND("BLOCK INDEX OUT OF RANGE");
        return NULL;
    }
    else
    {
        if(blockData[id]==NULL)
        {
            doTraceND("BLOCK DATA IS NULL");
            return NULL;
        }
        else
        {
            return blockData[id];
        }
    }


}




int GameWorld::getCellInd(
    btVector3 cParam,
    GamePageHolder* &curHolder
)
{

    int xv=(int)cParam.getX();
    int yv=(int)cParam.getY();
    int zv=(int)cParam.getZ();

    int cellsPerHolder=g_settings.cellsPerHolder;

    int newX=wrapCoord(xv, cellsPerWorld);
    int newY=wrapCoord(yv, cellsPerWorld);
    int newZ=zv;

    int x2=intDiv(newX, cellsPerHolder);
    int y2=intDiv(newY, cellsPerHolder);
    int z2=intDiv(newZ, cellsPerHolder);


    curHolder=(getHolderAtCoords(x2, y2, z2, true));
    if(curHolder->wasGenerated)
    {

    }
    else
    {
        //std::cout << "attempted getCellInd without generation\n";
        return -1;
    }
    //GamePage* gp = getPageAtCoords(x2, y2, z2, false);

    int xr=newX-x2*cellsPerHolder;
    int yr=newY-y2*cellsPerHolder;
    int zr=newZ-z2*cellsPerHolder;

    return (zr*cellsPerHolder*cellsPerHolder+yr*cellsPerHolder+xr);
}

int GameWorld::getCellInd(
    GamePageHolder* &curHolder,
    int xv,
    int yv,
    int zv
)
{



    int cellsPerHolder=g_settings.cellsPerHolder;

    int newX=wrapCoord(xv, cellsPerWorld);
    int newY=wrapCoord(yv, cellsPerWorld);
    int newZ=zv;

    int x2=intDiv(newX, cellsPerHolder);
    int y2=intDiv(newY, cellsPerHolder);
    int z2=intDiv(newZ, cellsPerHolder);


    curHolder=(getHolderAtCoords(x2, y2, z2, true));
    if(curHolder->wasGenerated)
    {

    }
    else
    {
        //std::cout << "attempted getCellInd without generation\n";
        return -1;
    }
    //GamePage* gp = getPageAtCoords(x2, y2, z2, false);

    int xr=newX-x2*cellsPerHolder;
    int yr=newY-y2*cellsPerHolder;
    int zr=newZ-z2*cellsPerHolder;

    return (zr*cellsPerHolder*cellsPerHolder+yr*cellsPerHolder+xr);
}






// void setCellAtCoords(
// 	int xv, int yv, int zv
// ) {
// 	int cellsPerHolder = g_settings.cellsPerHolder;

// 	int newX = wrapCoord(xv,cellsPerWorld);
// 	int newY = wrapCoord(yv,cellsPerWorld);
// 	int newZ = zv;

// 	int x2 = intDiv(newX,cellsPerHolder);
// 	int y2 = intDiv(newY,cellsPerHolder);
// 	int z2 = intDiv(newZ,cellsPerHolder);

// 	GamePageHolder* curHolder = getHolderAtCoords(x2, y2, z2, true);

// 	if (curHolder == NULL) {
// 		return;
// 	}

// 	int xr = newX - x2*cellsPerHolder;
// 	int yr = newY - y2*cellsPerHolder;
// 	int zr = newZ - z2*cellsPerHolder;

// 	int ind = (zr*cellsPerHolder*cellsPerHolder + yr*cellsPerHolder + xr)*4;

// 	curHolder->setCellAtInd(ind);
// }


// x, y, z measured in cells
int GameWorld::getCellAtCoords(
    int xv,
    int yv,
    int zv
)
{

    int cellsPerHolder=g_settings.cellsPerHolder;

    int newX=wrapCoord(xv, cellsPerWorld);
    int newY=wrapCoord(yv, cellsPerWorld);
    int newZ=zv;

    int x2=intDiv(newX, cellsPerHolder);
    int y2=intDiv(newY, cellsPerHolder);
    int z2=intDiv(newZ, cellsPerHolder);

//    int q;


    GamePageHolder* curHolder=getHolderAtCoords(x2, y2, z2, true);

    if(curHolder==NULL)
    {
        return E_CD_EMPTY;
    }

    // if (curHolder->lockRead) {
    // 	return E_CD_EMPTY;
    // }

    if(curHolder->wasGenerated)
    {

    }
    else
    {
        return E_CD_EMPTY;
    }

    int xr=newX-x2*cellsPerHolder;
    int yr=newY-y2*cellsPerHolder;
    int zr=newZ-z2*cellsPerHolder;

    int ind=(zr*cellsPerHolder*cellsPerHolder+yr*cellsPerHolder+xr)*4;

    return curHolder->getCellAtInd(ind);

}

float GameWorld::getCellAtCoordsLin(btVector3 pos)
{

    int xv=(int)pos.getX();
    int yv=(int)pos.getY();
    int zv=(int)pos.getZ();

    float fx=pos.getX()-xv;
    float fy=pos.getY()-yv;
    float fz=pos.getZ()-zv;

    float res[8];

    int q;
    int i;
    int j;
    int k;

    int curRes;

    for(q=0; q<8; q++)
    {
        k=q/(2*2);
        j=(q-k*2*2)/2;
        i=q-(k*2*2+j*2);

        curRes=getCellAtCoords(xv+i, yv+j, zv+k);

        if(curRes==E_CD_SOLID)
        {
            res[q]=1.0f;
        }
        else
        {
            res[q]=0.0f;
        }

    }

    res[0]=res[0]*(1.0f-fz)+res[4]*fz;
    res[1]=res[1]*(1.0f-fz)+res[5]*fz;
    res[2]=res[2]*(1.0f-fz)+res[6]*fz;
    res[3]=res[3]*(1.0f-fz)+res[7]*fz;

    res[0]=res[0]*(1.0f-fy)+res[2]*fy;
    res[1]=res[1]*(1.0f-fy)+res[3]*fy;

    return res[0]*(1.0f-fx)+res[1]*fx;
}

btVector3 GameWorld::getNormalAtCoord(btVector3 coord, float* cellVal)
{

    btVector3 norVal;
    int q;

    for(q=0; q<4; q++)
    {
        cellVal[q]=GameState::gw->getCellAtCoordsLin(
            coord+offsetVal[q]
        );
    }

    norVal=btVector3(
        (cellVal[0]-cellVal[3]),
        (cellVal[1]-cellVal[3]),
        (cellVal[2]-cellVal[3])
    );

    safeNorm(norVal);


    return norVal;
}


void GameWorld::setArrAtCoords(
    int xv, int yv, int zv,
    int* tempCellData,
    int* tempCellData2
)
{
    int cellsPerHolder=g_settings.cellsPerHolder;

    int newX=wrapCoord(xv, cellsPerWorld);
    int newY=wrapCoord(yv, cellsPerWorld);
    int newZ=zv;

    int x2=intDiv(newX, cellsPerHolder);
    int y2=intDiv(newY, cellsPerHolder);
    int z2=intDiv(newZ, cellsPerHolder);

    GamePageHolder* curHolder=getHolderAtCoords(x2, y2, z2, true);

    if(curHolder==NULL)
    {
        return;
    }

    if(curHolder->lockWrite)
    {
        return;
    }


    int xr=newX-x2*cellsPerHolder;
    int yr=newY-y2*cellsPerHolder;
    int zr=newZ-z2*cellsPerHolder;

    int ind=(zr*cellsPerHolder*cellsPerHolder+yr*cellsPerHolder+xr)*4;

    // if (singleton->refreshPaths) {	
    // 	curHolder->reset(); // todo: look at this?
    // }

    curHolder->setArrAtInd(ind, tempCellData, tempCellData2);
}


// x, y, z measured in cells
void GameWorld::getArrAtCoords(
    int xv,
    int yv,
    int zv,
    int* tempCellData,
    int* tempCellData2
)
{

    int cellsPerHolder=g_settings.cellsPerHolder;

    int newX=wrapCoord(xv, cellsPerWorld);
    int newY=wrapCoord(yv, cellsPerWorld);
    int newZ=zv;

    int x2=intDiv(newX, cellsPerHolder);
    int y2=intDiv(newY, cellsPerHolder);
    int z2=intDiv(newZ, cellsPerHolder);

    int q;

    if(tempCellData!=NULL)
    {
        for(q=0; q<4; q++)
        {
            tempCellData[q]=FLUID_UNIT_MIN;
            tempCellData2[q]=FLUID_UNIT_MIN;
        }
    }

    GamePageHolder* curHolder=getHolderAtCoords(x2, y2, z2, true);

    if(curHolder==NULL)
    {
        return;
    }

    int xr=newX-x2*cellsPerHolder;
    int yr=newY-y2*cellsPerHolder;
    int zr=newZ-z2*cellsPerHolder;

    int ind=(zr*cellsPerHolder*cellsPerHolder+yr*cellsPerHolder+xr)*4;

    curHolder->getArrAtInd(ind, tempCellData, tempCellData2);

}


void GameWorld::fireEvent(BaseObjType uid, int opCode, float fParam)
{
    BaseObj* ge=&(GameState::gem->gameObjects[uid]);
    switch(opCode)
    {
    case EV_COLLISION:

        switch(ge->entType)
        {
        case E_ENTTYPE_BULLET:
            GameAudio::playSoundEnt("bump0", ge, 0.0f, 0.25f);
            break;
        case E_ENTTYPE_WEAPON:
            GameAudio::playSoundEnt("metalclash0", ge, 0.25f, 0.25f);
            break;
        default:
            GameAudio::playSoundEnt("land0", ge, 0.1f, fParam*0.2f);
            break;
        }
        Renderer::performCamShake(ge, fParam);
        break;
    }
}


void GameWorld::doMedian()
{
    int i;
    for(i=0; i<medianCount; i++)
    {
        Renderer::bindShader("MedianShader");
        Renderer::bindFBO("resultFBO", activeFBO);
        Renderer::sampleFBO("resultFBO", 0, activeFBO);
        Renderer::sampleFBO("solidTargFBO", 1);
        Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
        Renderer::setShaderVec2("bufferDim", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY);
        //Renderer::setShaderfVec2("bufferDim", &(Renderer::bufferDim) );

        Renderer::drawFSQuad();

        Renderer::unsampleFBO("solidTargFBO", 1);
        Renderer::unsampleFBO("resultFBO", 0, activeFBO);
        Renderer::unbindFBO();
        Renderer::unbindShader();

        activeFBO=1-activeFBO;
    }
}


void GameWorld::preUpdate()
{
    activeFBO=0;


    camChunkPos.copyFrom(Renderer::cameraGetPosNoShake());
    camChunkPos.intDivXYZ(g_settings.cellsPerChunk);

    camBlockPos.copyFrom(Renderer::cameraGetPosNoShake());
    camBlockPos.intDivXYZ(g_settings.cellsPerBlock);

    if(GameState::gem->getCurActor()==NULL)
    {
        camHolderPos.copyFrom(Renderer::cameraGetPosNoShake());
        camHolderPos.intDivXYZ(g_settings.cellsPerHolder);
        camHolderPos.addXYZRef(&(Renderer::lookAtVec), 4.0);
    }
    else
    {
        camHolderPos=convertToVQV(GameState::gem->getCurActor()->getCenterPoint(0));
        camHolderPos.intDivXYZ(g_settings.cellsPerHolder);
    }


    if(depthInvalidMove)
    {
        invalidCount+=1.25f+amountInvalidMove*0.25f;
    }
    // if (depthInvalidRotate) {
    // 	invalidCount += 1.25f + singleton->amountInvalidRotate*200.0f;
    // }
    invalidCount=std::min(invalidCount, invalidCountMax);
    invalidCount-=1.0f;
    if(invalidCount<0.0f)
    {
        invalidCount=0.0f;
    }

    // if was moving and just stopped moving, render sphere map once

    // if (GameState::gameFluid[E_FID_BIG]->posShifted) {
    // 	drawVol((volumeWrappers[E_VW_TERGEN]));
    // }


    if(GameState::gameFluid[E_FID_BIG]->posShifted)
    {
        shiftCounter=10;
        GameState::gameFluid[E_FID_BIG]->posShifted=false;
    }

    shiftCounter--;
    if(shiftCounter<0)
    {
        shiftCounter=0;
    }

    Matrix4 curMVP;

    curMVP=Renderer::projMatrix*Renderer::viewMatrix;
    Renderer::curObjMatrix3.set4(curMVP.get());
    Renderer::curObjMatrix3.invert();
    Renderer::curObjMatrix3.transpose();

    if(updateHolderLookat)
    {
        holderLookAt.copyFrom(&camHolderPos);
        holderLookAt.addXYZRef(&Renderer::lookAtVec, 1.0f);
        holderInFocus=getHolderAtCoords(
            holderLookAt.getIX(),
            holderLookAt.getIY(),
            holderLookAt.getIZ(),
            false
        );
    }


}


void GameWorld::update()
{

    updateLock=true;

    bool postToScreen=true;

    if(noiseGenerated)
    {

    }
    else
    {
        noiseGenerated=true;
        Renderer::bindShader("NoiseShader");
        Renderer::bindFBO("noiseFBO");
        Renderer::drawFSQuad();
        Renderer::unbindFBO();
        Renderer::unbindShader();

        Renderer::copyFBO("noiseFBO", "noiseFBOLinear");

        tempVec1.setFXYZ(0.0f, 0.0f, 0.0f);
        tempVec2.setFXYZ(256.0, 256.0f, 256.0f);

        drawVol((volumeWrappers[E_VW_VORO]), &tempVec1, &tempVec2, true, true, true);


    }

    // get view matrix
    // Renderer::perspectiveOn = true;
    // singleton->getMatrixFromFBO("rasterFBO0");
    // Renderer::perspectiveOn = false;




    if(g_settings.fpsTest)
    {

    }
    else
    {

        glEnable(GL_DEPTH_TEST);
        Renderer::perspectiveOn=true;

        if(g_settings.settings[E_BS_DEBUG_VIEW])
        { //||(singleton->mouseState == E_MOUSE_STATE_BRUSH)
//renderGeom();
            renderDebug();
        }
        else
        {
            Renderer::bindShader("GeomShader");
            Renderer::bindFBO("debugTargFBO");
            Renderer::unbindFBO();
            Renderer::unbindShader();
        }

        Renderer::perspectiveOn=false;
        glDisable(GL_DEPTH_TEST);


    }


    Renderer::forceShadowUpdate--;
    if(Renderer::forceShadowUpdate<0)
    {
        Renderer::forceShadowUpdate=0;
    }

    if(g_settings.settings[E_BS_RENDER_VOXELS])
    {
        updatePrimArr();
        updateLimbTBOData(true);

        if(
            lightChanged||
            (Renderer::forceShadowUpdate==1)||
            (
            (Renderer::lastLightPos.distance(Renderer::cameraGetPosNoShake()))>
            getConst(E_CONST_LIGHTTHRESH)
            )
            )
        {
            // if (!lightChanged) {
            // 	std::cout << "updateShadows\n";
            // }
            rasterHolders(true);
            Renderer::lastLightPos.copyFrom(Renderer::cameraGetPosNoShake());
            lightChanged=false;
            //singleton->updateShadows = false;
        }

        rasterHolders(false);
    }
    else
    {






        // if (

        // 	(

        // 		( shiftCounter == 1 ) ||
        // 		(
        // 			(singleton->lastDepthInvalidMove) &&
        // 			(!depthInvalidMove)
        // 		)	

        // 	)
        // 	&&
        // 	USE_SPHERE_MAP
        // ) {
        // 	drawPrim(true,true,false);
        // }


        // if (GEN_POLYS_WORLD||POLY_COLLISION) {
        // 	glEnable(GL_DEPTH_TEST);
        // 	//glEnable(GL_CULL_FACE);

        // 	//back face
        // 	//glDepthFunc(GL_GREATER);
        // 	// glCullFace(GL_FRONT);
        // 	// drawPolys(polyFBOStrings[1], 4,-1);

        // 	//front face
        // 	//glDepthFunc(GL_LESS);
        // 	//glCullFace(GL_BACK);
        // 	//glDepthFunc(GL_LEQUAL);

        // 	//glDepthRange(Renderer::clipDist[0],Renderer::clipDist[1]);
        // 	Renderer::perspectiveOn = true;

        // 	if (GEN_POLYS_WORLD) {
        // 		drawPolys(polyFBOStrings[0], 0, 0,true);
        // 	}
        // 	if (POLY_COLLISION) {
        // 		drawPolys(polyFBOStrings[0], 0, DEF_VOL_SIZE/g_settings.cellsPerHolder + 1,false);
        // 	}

        // 	Renderer::perspectiveOn = false;

        // 	//glDisable(GL_CULL_FACE);
        // 	glDisable(GL_DEPTH_TEST);

        // 	//polyCombine();
        // }



        skippedPrim=false;

        updateLimbTBOData(true);
        drawPrim(false, true, false);
        drawPrim(false, false, false);

        //Renderer::copyFBO2("solidBaseTargFBO","solidTargFBO");

        Renderer::bindShader("SolidCombineShader");
        Renderer::setShaderInt("skippedPrim", (int)(skippedPrim));
        Renderer::bindFBO("solidTargFBO");//, -1, 0);
        Renderer::sampleFBO("solidBaseTargFBO", 0);
        Renderer::sampleFBO("geomTargFBO", 2);
        Renderer::drawFSQuad();
        Renderer::unsampleFBO("geomTargFBO", 2);
        Renderer::unsampleFBO("solidBaseTargFBO", 0);
        Renderer::unbindFBO();
        Renderer::unbindShader();

        postProcess(postToScreen);
    }


    doMedian();

    finalStep(postToScreen);

    if(postToScreen)
    {
        drawMap();
//        glutSwapBuffers();
    }


    updateLock=false;

}


void GameWorld::toggleVis(GameEnt* se)
{

    if(se==NULL)
    {

    }
    else
    {
        se->visible=!(se->visible);
    }
}




void GameWorld::ensureBlocks()
{
    // first fetch all the blocks to make sure they get created
    // don't remove this, necessary for overlapping geom

    int i;
    int j;
    int k;
    int ii;
    int jj;
    int kk;
    int blockRad=1;

    for(k=-blockRad; k<=blockRad; k++)
    {
        for(j=-blockRad; j<=blockRad; j++)
        {
            for(i=-blockRad; i<=blockRad; i++)
            {
                ii=i+camBlockPos.getIX();
                jj=j+camBlockPos.getIY();
                kk=k+camBlockPos.getIZ();

                getBlockAtCoords(ii, jj, kk, true);

            }
        }
    }

}







// void findNearestEnt(
// 	EntSelection* entSelection,
// 	int entType,
// 	int maxLoadRad,
// 	int radStep,
// 	FIVector4 *testPoint,
// 	bool onlyInteractive = false,
// 	bool ignoreDistance = false
// ) {

// 	GameEnt* myEnt;

// 	int curInd = 0;
// 	float bestDis;
// 	float curDis;

// 	bool doProc = false;

// 	entSelection->selEntList.clear();
// 	entSelection->selEntMap.clear();
// 	entSelection->selEntListInd = 0;

// 	bestDis = 99999.0f;




// 	//////////////////////



// 	int i, j, k;
// 	int ii, jj, kk;
// 	int incVal;

// 	int tot = 0;


// 	int mink;
// 	int maxk;
// 	int minj;
// 	int maxj;
// 	int mini;
// 	int maxi;
// 	int curLoadRadius;
// 	intPair curId;

// 	tempVec.copyFrom(testPoint);
// 	tempVec.intDivXYZ(g_settings.cellsPerHolder);


// 	GamePageHolder* curHolder;
// 	GameBlock *curBlock;


// 	ensureBlocks();




// 	for (curLoadRadius = 0; curLoadRadius < maxLoadRad; curLoadRadius++) {

// 		mink = max(tempVec.getIZ() - curLoadRadius,0);
// 		maxk = min(tempVec.getIZ() + curLoadRadius,holdersPerWorld-1);
// 		minj = tempVec.getIY() - curLoadRadius;
// 		maxj = tempVec.getIY() + curLoadRadius;
// 		mini = tempVec.getIX() - curLoadRadius;
// 		maxi = tempVec.getIX() + curLoadRadius;

// 		for (jj = minj; jj <= maxj; jj += radStep) {

// 			if (curLoadRadius <= 2) {
// 				incVal = 1;
// 			}
// 			else {
// 				if ( (jj == minj) || (jj == maxj) ) {
// 					incVal = radStep;
// 				}
// 				else {
// 					incVal = maxi - mini;
// 				}
// 			}

// 			for (ii = maxi; ii >= mini; ii -= incVal) {


// 				for (kk = mink; kk <= maxk; kk += radStep) {



// 					curHolder = getHolderAtCoords(ii, jj, kk, true);
// 					curBlock = getBlockAtId(curHolder->blockId);

// 					if (curBlock == NULL) {
// 						std::cout << "NULL BLOCK\n";
// 					}
// 					else {

// 						for (k = 0; k < curHolder->containsEntIds[entType].data.size(); k++) { //curBlock->gameEnts[entType].data.size()

// 							curId = curHolder->containsEntIds[entType].data[k];
// 							myEnt = &(blockData[curId.v0]->gameEnts[entType].data[curId.v1]);



// 							if (
// 								ignoreDistance||
// 								testPoint->inBoundsXYZSlack(
// 									myEnt->getVisMinInPixelsT(),
// 									myEnt->getVisMaxInPixelsT(),
// 									0.0625
// 								)
// 							)
// 							{

// 								if (entSelection->selEntMap.count(curId) == 0 ) {

// 									entSelection->selEntList.push_back(myEnt);
// 									entSelection->selEntMap[curId] = 1;



// 									curDis = 
// 									abs(myEnt->getVisMaxInPixelsT()->getFX()-testPoint->getFX()) +
// 									abs(myEnt->getVisMaxInPixelsT()->getFY()-testPoint->getFY()) +
// 									abs(myEnt->getVisMaxInPixelsT()->getFZ()-testPoint->getFZ());

// 									//myEnt->getVisMinInPixelsT()->distance(testPoint) +
// 									//myEnt->getVisMaxInPixelsT()->distance(testPoint);

// 									if (myEnt->visible) {

// 									}
// 									else {
// 										curDis *= 16.0f;
// 									}

// 									if (onlyInteractive) {
// 										doProc = singleton->isInteractiveEnt[myEnt->buildingType];
// 									}
// 									else {
// 										doProc = true;
// 									}

// 									if ((curDis < bestDis)&&doProc) {
// 										bestDis = curDis;
// 										entSelection->selEntListInd = curInd;
// 									}


// 									curInd++;

// 								}



// 							}
// 						}


// 					}



// 				}

// 			}
// 		}

// 	}




// 	//////////////////////


// 	//return resEnt;
// }


void GameWorld::drawVol(
    VolumeWrapper* curVW,
    FIVector4* minc,
    FIVector4* maxc,
    bool copyToTex,
    bool forceFinish,
    bool getVoro,
    bool getBlockHolders
)
{


    curVW->genPosMin.copyFrom(minc);
    curVW->genPosMax.copyFrom(maxc);

    //curVW->genPosMin.copyFrom( &(GameState::gameFluid[E_FID_BIG]->volMinReadyInPixels) );
    //curVW->genPosMax.copyFrom( &(GameState::gameFluid[E_FID_BIG]->volMaxReadyInPixels) );

    Renderer::bindShader("TerGenShader");
    Renderer::bindFBODirect(&(curVW->fboSet));
    Renderer::sampleFBO("hmFBOLinearBig", 2);

    if(!getVoro)
    {
        Renderer::setShaderTexture3D(13, volumeWrappers[E_VW_VORO]->volId);
    }



    // if (!getBlockHolders) {
    // 	Renderer::setShaderTexture3D(14, volumeWrappers[E_VW_WORLD]->volId);
    // }

    Renderer::setShaderTexture(15, imageVoro->tid);

    Renderer::setShaderfVec3("bufferDim", &(curVW->terGenDim));

    Renderer::setShaderFloat("voroSize", volumeWrappers[E_VW_VORO]->terGenDim.getFZ());

    Renderer::setShaderFloat("mapPitch", (float)mapPitch);
    Renderer::setShaderFloat("seaLevel", getSLNormalized());
    Renderer::setShaderFloat("heightMapMaxInCells", g_settings.heightMapMaxInCells);
    Renderer::setShaderfVec4("mapFreqs", &(mapFreqs));
    Renderer::setShaderfVec4("mapAmps", &(mapAmps));

    Renderer::setShaderfVec3("volMinReadyInPixels", &(curVW->genPosMin));
    Renderer::setShaderfVec3("volMaxReadyInPixels", &(curVW->genPosMax));
    //Renderer::setShaderfVec3("volMinReadyInPixels", &(GameState::gameFluid[E_FID_BIG]->volMinInPixels) );
    //Renderer::setShaderfVec3("volMaxReadyInPixels", &(GameState::gameFluid[E_FID_BIG]->volMaxInPixels) );

    Renderer::setShaderInt("getVoro", (int)(getVoro));
    Renderer::setShaderInt("getBlockHolders", (int)(false));

    Renderer::setShaderFloat("cellsPerWorld", (float)cellsPerWorld);

    Renderer::fsQuad.draw();

    Renderer::setShaderTexture(15, 0);
    // if (!getBlockHolders) {
    // 	Renderer::setShaderTexture3D(14, 0);
    // }
    if(!getVoro)
    {
        Renderer::setShaderTexture3D(13, 0);
    }
    Renderer::unsampleFBO("hmFBOLinearBig", 2);
    Renderer::unbindFBO();
    Renderer::unbindShader();

    if(forceFinish)
    {
        glFlush();
        glFinish();
    }

    FBOWrapper* fbow=curVW->fboSet.getFBOWrapper(0);
    fbow->getPixels();

    if(forceFinish)
    {
        glFlush();
        glFinish();
    }

    if(copyToTex)
    {
        if(curVW->isFloat)
        {
            curVW->copyFloatArr(fbow->pixelsFloat);
        }
        else
        {
            curVW->copyCharArr(fbow->pixelsChar);
        }
    }

}





void GameWorld::updateLimbTBOData(bool showLimbs)
{
    int i;
    int j;
//    int q;

    BodyStruct* curBody;

    BaseObj* ge;

    btVector3 centerPoint;
    btVector3 tanVec;
    btVector3 bitVec;
    btVector3 norVec;
    btVector3 len0;
    btVector3 len1;
    btMatrix3x3 basis;

    GameOrg* curOrg=NULL;
    GameOrgNode* curOrgNode=NULL;

    float buffer=0.5f;

    int dataInd=0;
    int localActorCount=0;
    int headerStart;

    float myMat[16];
    btVector3 basePos;
    btVector3 tempBTV;
    Matrix4 myMatrix4;
    Vector4 myVector4;
    Vector4 resVector4;

    int limbAP=0;
    limbArrPos=0;

    BaseObj* grabber;

    // if (singleton->doPathReport) {
    // 	std::cout << "\n\n";
    // }


    limbTBOData[dataInd]=0.0f; dataInd++;
    limbTBOData[dataInd]=0.0f; dataInd++;
    limbTBOData[dataInd]=0.0f; dataInd++;
    limbTBOData[dataInd]=0.0f; dataInd++;

    dataInd=0;

    if(!showLimbs)
    {
        return;
    }



    for(i=0; i<GameState::gem->visObjects.size(); i++)
    {
        ge=&(GameState::gem->gameObjects[GameState::gem->visObjects[i]]);

        if(
            (!(ge->isHidden))&&
            (ge->orgId>-1)&&
            (ge->actorId>-1)
            )
        {

            localActorCount++;



            ge->clearAABB(&(ge->aabbMinVis), &(ge->aabbMaxVis));
            for(j=0; j<ge->bodies.size(); j++)
            {
                curBody=&(ge->bodies[j]);
                ge->addAABBPoint(
                    &(ge->aabbMinVis),
                    &(ge->aabbMaxVis),
                    curBody->body->getCenterOfMassPosition()
                );
            }

            // ge->aabbMinVis = (ge->aabbMinSkel);
            // ge->aabbMaxVis = (ge->aabbMaxSkel);

            ge->aabbMinVis=ge->aabbMinVis+btVector3(-buffer, -buffer, -buffer);
            ge->aabbMaxVis=ge->aabbMaxVis+btVector3(buffer, buffer, buffer);

            // if (ge->entType == E_ENTTYPE_NPC) {
            // 	ge->aabbMinVis = ge->aabbMinVis + ge->skelOffset;
            // 	ge->aabbMaxVis = ge->aabbMaxVis + ge->skelOffset;
            // }


            curOrg=GameState::gem->gameOrgs[ge->orgId];

            if(ge->isGrabbedById>-1)
            {
                grabber=&(GameState::gem->gameObjects[ge->isGrabbedById]);
            }
            else
            {
                grabber=ge;
            }

            grabber->bodies[E_BDG_CENTER].body->getWorldTransform().getOpenGLMatrix(myMat);
            myMatrix4=Matrix4(myMat);




            float randOff;


            if(
                GameState::gem->firstPerson&&
                (ge->uid==GameState::gem->getCurActorUID())
                )
            {

            }
            else
            {

                // header info
                headerStart=dataInd;
                limbTBOData[dataInd]=0.0f; dataInd++;
                limbTBOData[dataInd]=0.0f; dataInd++;
                limbTBOData[dataInd]=0.0f; dataInd++;
                limbTBOData[dataInd]=0.0f; dataInd++;

                // todo: aabbMinSkel wont show weapons

                limbTBOData[dataInd]=ge->aabbMinVis.getX(); dataInd++;
                limbTBOData[dataInd]=ge->aabbMinVis.getY(); dataInd++;
                limbTBOData[dataInd]=ge->aabbMinVis.getZ(); dataInd++;
                limbTBOData[dataInd]=0.0f; dataInd++;


                limbTBOData[dataInd]=ge->aabbMaxVis.getX(); dataInd++;
                limbTBOData[dataInd]=ge->aabbMaxVis.getY(); dataInd++;
                limbTBOData[dataInd]=ge->aabbMaxVis.getZ(); dataInd++;
                limbTBOData[dataInd]=0.0f; dataInd++;



                for(j=0; j<ge->bodies.size(); j++)
                {
                    curBody=&(ge->bodies[j]);

                    if(
                        (curBody->jointType!=E_JT_LIMB)||
                        (curBody->boneId<0)||
                        (curBody->boneId==E_BONE_C_BASE)


                        )
                    {

                    }
                    else
                    {



                        curOrgNode=curOrg->allNodes[curBody->boneId];

                        centerPoint=curBody->body->getCenterOfMassPosition();
                        //centerPoint += btVector3(0.0,0.0,-0.4f);
                        basis=curBody->body->getCenterOfMassTransform().getBasis();



                        tempBTV=convertToBTV(curOrgNode->tbnTrans[1]);
                        myVector4=Vector4(
                            tempBTV.getX(),
                            tempBTV.getY(),
                            tempBTV.getZ(),
                            1.0f
                        );
                        resVector4=myMatrix4*myVector4;
                        basePos=btVector3(resVector4.x, resVector4.y, resVector4.z);
                        basePos+=grabber->skelOffset;
                        basePos-=centerPoint;
                        safeNorm(basePos);



                        tanVec=basis.getColumn(0);//basis*curOrgNode->orgVecs[0].getBTV();
                        safeNorm(tanVec);
                        // bitVec = basis.getColumn(1);//basis*curOrgNode->orgVecs[1].getBTV();
                        // norVec = basis.getColumn(2);//basis*curOrgNode->orgVecs[2].getBTV();

                        //tanVec = basePos[0];//basis*curOrgNode->orgVecs[0].getBTV();
                        bitVec=basePos;//basis*curOrgNode->orgVecs[1].getBTV();
                        //norVec = basePos[2];//basis*curOrgNode->orgVecs[2].getBTV();

                        norVec=tanVec.cross(bitVec);
                        safeNorm(norVec);

                        bitVec=norVec.cross(tanVec);
                        safeNorm(bitVec);


                        len0=convertToBTV(curOrgNode->orgVecs[E_OV_TBNRAD0]);
                        len1=convertToBTV(curOrgNode->orgVecs[E_OV_TBNRAD1]);


                        // datVec
                        randOff=abs(fSeedRand2((ge->uid*37.19232f), (curOrgNode->orgVecs[E_OV_MATPARAMS].getFX()*17.89923f)));

                        limbTBOData[dataInd]=(float)ge->uid; dataInd++;
                        limbTBOData[dataInd]=0;/*curBody->body->limbUID;*/ dataInd++;
                        limbTBOData[dataInd]=curOrgNode->orgVecs[E_OV_MATPARAMS].getFX(); dataInd++;
                        limbTBOData[dataInd]=randOff; dataInd++;

                        //cenVec

                        limbTBOData[dataInd]=centerPoint.getX(); dataInd++;
                        limbTBOData[dataInd]=centerPoint.getY(); dataInd++;
                        limbTBOData[dataInd]=centerPoint.getZ(); dataInd++;
                        limbTBOData[dataInd]=curOrgNode->orgVecs[E_OV_POWVALS].getFX(); dataInd++;

                        limbTBOData[dataInd]=tanVec.getX(); dataInd++;
                        limbTBOData[dataInd]=tanVec.getY(); dataInd++;
                        limbTBOData[dataInd]=tanVec.getZ(); dataInd++;
                        limbTBOData[dataInd]=curOrgNode->orgVecs[E_OV_POWVALS].getFY(); dataInd++;

                        limbTBOData[dataInd]=bitVec.getX(); dataInd++;
                        limbTBOData[dataInd]=bitVec.getY(); dataInd++;
                        limbTBOData[dataInd]=bitVec.getZ(); dataInd++;
                        limbTBOData[dataInd]=curOrgNode->orgVecs[E_OV_POWVALS].getFZ(); dataInd++;

                        // if (singleton->doPathReport) {
                        // 	std::cout << curOrgNode->orgVecs[E_OV_MATPARAMS].getFX() << "\n";
                        // }

                        limbTBOData[dataInd]=norVec.getX(); dataInd++;
                        limbTBOData[dataInd]=norVec.getY(); dataInd++;
                        limbTBOData[dataInd]=norVec.getZ(); dataInd++;
                        limbTBOData[dataInd]=curOrgNode->orgVecs[E_OV_POWVALS].getFW(); dataInd++;

                        // ln0Vec

                        limbTBOData[dataInd]=len0.getX(); dataInd++;
                        limbTBOData[dataInd]=len0.getY(); dataInd++;
                        limbTBOData[dataInd]=len0.getZ(); dataInd++;
                        limbTBOData[dataInd]=(float)ge->entType; dataInd++;

                        // ln1Vec

                        limbTBOData[dataInd]=len1.getX(); dataInd++;
                        limbTBOData[dataInd]=len1.getY(); dataInd++;
                        limbTBOData[dataInd]=len1.getZ(); dataInd++;
                        limbTBOData[dataInd]=curOrgNode->orgVecs[E_OV_TBNOFFSET].getFW(); dataInd++;


                    }
                }


                limbTBOData[headerStart+0]=(float)(dataInd/4);
                limbTBOData[headerStart+1]=0.0f;
                limbTBOData[headerStart+2]=0.0f;
                limbTBOData[headerStart+3]=0.0f;


                limbAP=limbArrPos*8;
                //texelRes1
                limbArr[limbAP+0]=ge->aabbMinVis[0];
                limbArr[limbAP+1]=ge->aabbMinVis[1];
                limbArr[limbAP+2]=ge->aabbMinVis[2];
                limbArr[limbAP+3]=(float)(headerStart/4);

                //texelRes2
                limbArr[limbAP+4]=ge->aabbMaxVis[0];
                limbArr[limbAP+5]=ge->aabbMaxVis[1];
                limbArr[limbAP+6]=ge->aabbMaxVis[2];
                limbArr[limbAP+7]=0;
                limbArrPos++;

            }
        }
    }

    // pad end with blank data

    limbTBOData[dataInd]=0.0f; dataInd++;
    limbTBOData[dataInd]=0.0f; dataInd++;
    limbTBOData[dataInd]=0.0f; dataInd++;
    limbTBOData[dataInd]=0.0f; dataInd++;

    actorCount=localActorCount;

    Renderer::limbTBO.update(limbTBOData, NULL, dataInd*4);

    // if (singleton->doPathReport) {
    // 	std::cout << "\n\n";
    // }
    // singleton->doPathReport = false;
}

void GameWorld::drawPrim(bool doSphereMap, bool doTer, bool doPoly)
{
    int i;

    int ind=0;

    if(doSphereMap)
    {

        //std::cout << "doSphereMap\n";

        ind=1;
    }


    bool skipPrim=
        (GameState::gameFluid[E_FID_BIG]->curGeomCount==0)&&
        (g_settings.settings[E_BS_PLACING_GEOM]==false);


    bool doPrim=!doTer;

    int curGeomCount=0;





    if(doPrim)
    {
        ind=2;

        curGeomCount=GameState::gameFluid[E_FID_BIG]->curGeomCount;

        if(skipPrim)
        {
            Renderer::copyFBO2("terTargFBO", "solidBaseTargFBO", 0, 1);
            Renderer::copyFBO2("terTargFBO", "waterTargFBO", 2, 3);
            Renderer::copyFBO2("terTargFBO", "prmDepthFBO", 4, 5);
            Renderer::copyFBO("terTargFBO", "numstepsFBO", 7);
            skippedPrim=true;
            return;
        }

    }
    else
    {
        curGeomCount=MAX_PRIM_IDS;
    }



    if(doPoly)
    {
        Renderer::bindShader("PrimShader_330_DOTER_DOPOLY");
        Renderer::bindFBO("polyFBO");
    }
    else
    {
        if(doTer)
        {

            if(USE_SPHERE_MAP)
            {
                Renderer::bindShader("PrimShader_330_DOTER_USESPHEREMAP");
            }
            else
            {
                Renderer::bindShader("PrimShader_330_DOTER");
            }

        }
        else
        {
            Renderer::bindShader("PrimShader_330_DOPRIM");
        }
        Renderer::bindFBO(curTargFBO[ind]);
    }








    Renderer::setShaderTexture3D(0, GameState::gameFluid[E_FID_BIG]->volIdPrim[0]);

    if(doPrim)
    {
        Renderer::setShaderTBO(
            1,
            GameState::gameFluid[E_FID_BIG]->tboWrapper.tbo_tex,
            GameState::gameFluid[E_FID_BIG]->tboWrapper.tbo_buf,
            true
        );
    }
    else
    {
        Renderer::setShaderTBO(
            1,
            Renderer::limbTBO.tbo_tex,
            Renderer::limbTBO.tbo_buf,
            true
        );
    }



    Renderer::sampleFBO("hmFBOLinearBig", 2);

    //Renderer::sampleFBO("rasterFBO0",3);
    //Renderer::sampleFBO("terDepthFBO",3);

    //if (USE_SPHERE_MAP) {
    //	Renderer::sampleFBO("sphDepthFBO",5);
    //}

    Renderer::sampleFBO("geomTargFBO", 5);

    // sample opposite pass
    if(doPrim)
    {
        Renderer::sampleFBO("terTargFBO", 7, -1, 0, 6);
    }
    else
    {
        Renderer::sampleFBO("prmTargFBO", 7, -1, 0, 6);
    }

    Renderer::setShaderTexture3D(13, volumeWrappers[E_VW_VORO]->volId);
    //Renderer::setShaderTexture3D(14, volumeWrappers[E_VW_WORLD]->volId);


    //Renderer::sampleFBO("noiseFBOLinear", 15);
    Renderer::setShaderTexture(15, imageVoro->tid);

    // if (!doPoly) {
    // 	Renderer::sampleFBO(polyFBOStrings[NUM_POLY_STRINGS],14);
    // }




    if((GameState::gem->getCurActor()==NULL)||GameState::gem->firstPerson)
    {
        Renderer::setShaderFloat("thirdPerson", 0.0f);
        //Renderer::setShaderFloat("CAM_BOX_SIZE", 0.5f);
    }
    else
    {
        Renderer::setShaderFloat("thirdPerson", 1.0f);
        //Renderer::setShaderFloat("CAM_BOX_SIZE", 0.5f);
        Renderer::setShaderfVec3("entPos", GameState::gem->getCurActor()->getCenterPointFIV(0));
    }


    Renderer::setShaderFloat("isUnderWater", -(getUnderWater()-0.5f)*2.0f);


    Renderer::setShaderMatrix4x4("modelview", Renderer::viewMatrix.get(), 1);
    Renderer::setShaderMatrix4x4("proj", Renderer::projMatrix.get(), 1);
    Renderer::setShaderMatrix4x4("modelviewInverse", Renderer::viewMatrixDI, 1);
    //Renderer::setShaderInt("readPoly", (int)(readPoly));
    Renderer::setShaderInt("depthInvalidMove", (int)(depthInvalidMove));
    Renderer::setShaderInt("depthInvalidRotate", (int)(depthInvalidRotate));

    Renderer::setShaderFloat("voroSize", volumeWrappers[E_VW_VORO]->terGenDim.getFZ());

    Renderer::setShaderInt("actorCount", actorCount);
    Renderer::setShaderInt("MAX_PRIM_IDS", std::min(curGeomCount, MAX_PRIM_IDS));
    Renderer::setShaderInt("MAX_PRIMTEST", std::min(curGeomCount, MAX_PRIMTEST));


    Renderer::setShaderFloat("invalidCount", invalidCount/invalidCountMax);
    Renderer::setShaderInt("doSphereMap",
        // (int)(doSphereMap)
        (int)(Renderer::sphereMapOn)
    );
    Renderer::setShaderInt("testOn", (int)(g_settings.settings[E_BS_TEST_1]));
    Renderer::setShaderInt("skipPrim", (int)(skipPrim));
    Renderer::setShaderInt("placingGeom", (int)(g_settings.settings[E_BS_PLACING_GEOM]));



    Renderer::setShaderfVec3("waterMin", &(GameState::gameFluid[E_FID_BIG]->curWaterMin));
    Renderer::setShaderfVec3("waterMax", &(GameState::gameFluid[E_FID_BIG]->curWaterMax));


    Renderer::setShaderfVec3("volMinReadyInPixels", &(GameState::gameFluid[E_FID_BIG]->volMinReadyInPixels));
    Renderer::setShaderfVec3("volMaxReadyInPixels", &(GameState::gameFluid[E_FID_BIG]->volMaxReadyInPixels));


    Renderer::setShaderFloat("mapPitch", (float)mapPitch);
    Renderer::setShaderFloat("seaLevel", getSLNormalized());
    Renderer::setShaderFloat("heightMapMaxInCells", g_settings.heightMapMaxInCells);
    Renderer::setShaderfVec4("mapFreqs", &(mapFreqs));
    Renderer::setShaderfVec4("mapAmps", &(mapAmps));

    Renderer::setShaderFloat("SPHEREMAP_SCALE_FACTOR", SPHEREMAP_SCALE_FACTOR);
    Renderer::setShaderFloat("UNIT_MAX", FLUID_UNIT_MAX+1);
    Renderer::setShaderFloat("waterLerp", GameState::gameFluid[E_FID_BIG]->waterLerp); //todo: E_FID_SML?
    Renderer::setShaderFloat("volSizePrim", (float)GameState::gameFluid[E_FID_BIG]->volSizePrim);
    Renderer::setShaderFloat("curTime", (float)GameState::pauseTime/1000.0f);
    Renderer::setShaderfVec2("bufferDim", &(Renderer::bufferRenderDim));
    Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
    Renderer::setShaderfVec3("lookAtVec", &(Renderer::lookAtVec));
    Renderer::setShaderfVec3("lightVec", &(Renderer::lightVec));
    Renderer::setShaderFloat("cellsPerWorld", (float)cellsPerWorld);

    // Renderer::setShaderFloat("volSizePrimSmall", GameState::gameFluid[E_FID_SML]->volSizePrim);
    // Renderer::setShaderfVec3("volMinReadyInPixelsSmall", &(GameState::gameFluid[E_FID_SML]->volMinReadyInPixels) );
    // Renderer::setShaderfVec3("volMaxReadyInPixelsSmall", &(GameState::gameFluid[E_FID_SML]->volMaxReadyInPixels) );

    Renderer::setShaderVec2("clipDist", Renderer::clipDist[0], Renderer::clipDist[1]);
    Renderer::setShaderFloat("FOV", Renderer::FOV*(float)M_PI/180.0f);


    //paramFetch1: x,y,z: position; w: template number
    //paramFetch2: x:diagMask, y:uid, z:?, w:?



    tempVec1.copyFrom(&(geomPoints[0]));
    tempVec1.addXYZRef(&(geomOrigOffset));
    tempVec2.setFXYZW(0.0f, -99.0f, 0.0f, 0.0f);
    Renderer::setShaderfVec4("paramFetch1", &tempVec1);
    Renderer::setShaderfVec4("paramFetch2", &tempVec2);
    Renderer::setShaderArrayfVec4("paramArrGeom", &getGeoParam(0), E_PRIMTEMP_LENGTH);


    if(GameState::gem->getCurActor()!=NULL)
    {

        splashArr[0]=GameState::gem->getCurActor()->getCenterPointFIV(0)->getFX();
        splashArr[1]=GameState::gem->getCurActor()->getCenterPointFIV(0)->getFX();
        splashArr[2]=GameState::gem->getCurActor()->getCenterPointFIV(0)->getFX();
        splashArr[3]=GameState::gem->getCurActor()->getVel(0)->length();

        Renderer::setShaderInt("numSplashes", 1);
        Renderer::setShaderArrayfVec4("splashArr", splashArr, MAX_SPLASHES);
    }
    else
    {
        Renderer::setShaderInt("numSplashes", 0);
    }



    if(g_settings.settings[E_BS_WATER_BULLET])
    {
        Renderer::setShaderInt("numExplodes", 0);
    }
    else
    {
        Renderer::setShaderInt("numExplodes", (int)GameState::gamePhysics->sphereStack.size());

        if(GameState::gamePhysics->sphereStack.size()>0)
        {
            for(i=0; i<GameState::gamePhysics->sphereStack.size(); i++)
            {
                explodeArr[i*4+0]=GameState::gamePhysics->sphereStack[i].position[0];
                explodeArr[i*4+1]=GameState::gamePhysics->sphereStack[i].position[1];
                explodeArr[i*4+2]=GameState::gamePhysics->sphereStack[i].position[2];
                explodeArr[i*4+3]=GameState::gamePhysics->sphereStack[i].curRad;
            }
            Renderer::setShaderArrayfVec4("explodeArr", explodeArr, MAX_EXPLODES);
        }
    }


    if(doPoly)
    {
        //rasterPolys(0,5);
    }
    else
    {
        Renderer::fsQuad.draw();
    }

    // if (!doPoly) {
    // 	Renderer::unsampleFBO(polyFBOStrings[NUM_POLY_STRINGS],14);
    // }


    //Renderer::unsampleFBO("noiseFBOLinear", 15);
    Renderer::setShaderTexture(15, 0);
    //Renderer::setShaderTexture3D(14, 0);
    Renderer::setShaderTexture3D(13, 0);


    if(doPrim)
    {
        Renderer::unsampleFBO("terTargFBO", 7, -1, 0, 6);
    }
    else
    {
        Renderer::unsampleFBO("prmTargFBO", 7, -1, 0, 6);
    }

    // if (USE_SPHERE_MAP) {
    // 	Renderer::unsampleFBO("sphDepthFBO",5);
    // }
    Renderer::unsampleFBO("geomTargFBO", 5);

    //Renderer::unsampleFBO("rasterFBO0",3);
    //Renderer::unsampleFBO("terDepthFBO",3);
    Renderer::unsampleFBO("hmFBOLinearBig", 2);

    Renderer::setShaderTBO(1, 0, 0, true);
    Renderer::setShaderTexture3D(1, 0);

    Renderer::unbindFBO();
    Renderer::unbindShader();

    if(doPoly)
    {
        return;
    }

    if(doTer)
    {
        Renderer::copyFBO("terTargFBO", "limbFBO", 6);
    }


    Renderer::copyFBO2(curTargFBO[ind], curDepthFBO[ind], 4, 5);

    if(doPrim)
    {
        Renderer::copyFBO2("prmTargFBO", "waterTargFBO", 2, 3);
        Renderer::copyFBO2("prmTargFBO", "solidBaseTargFBO", 0, 1);
    }




}





void GameWorld::drawOrg(GameOrg* curOrg, bool drawAll)
{

    if(curOrg==NULL)
    {
        return;
    }

    float scale=1.0f;





    // tangents
    Renderer::setShaderVec3("matVal", 255, 0, 0);
    drawNodeEnt((curOrg->baseNode), &(curOrg->basePosition), scale, 0, drawAll);

    // bitangents
    Renderer::setShaderVec3("matVal", 0, 255, 0);
    drawNodeEnt((curOrg->baseNode), &(curOrg->basePosition), scale, 1, drawAll);

    // normals
    Renderer::setShaderVec3("matVal", 0, 0, 255);
    drawNodeEnt((curOrg->baseNode), &(curOrg->basePosition), scale, 2, drawAll);

    if(drawAll)
    {
        // nodes
        Renderer::setShaderVec3("matVal", 254, 254, 254);
        drawNodeEnt((curOrg->baseNode), &(curOrg->basePosition), scale, 3, drawAll);
    }

    // std::cout << "yay\n";

}


void GameWorld::drawNodeEnt(
    GameOrgNode* curNode,
    FIVector4* basePosition,
    float scale,
    int drawMode,
    bool drawAll
)
{


    BaseObj* grabber;


    bool doProc=false;

    if(drawAll)
    {
        doProc=true;
    }
    else
    {
        if(curNode==GameState::gem->selectedNode)
        {
            doProc=true;
        }
    }

    int i;


    if(doProc)
    {
        lineSeg[0].setFXYZRef(&(curNode->orgTrans[0]));
        lineSeg[0].multXYZ(scale);

        // if (drawAll) {
        // 	lineSeg[1].setFXYZRef(&(curNode->tbnTrans[drawMode%3]));
        // 	lineSeg[1].multXYZ(  scale  );
        // }
        // else {
        lineSeg[1].setFXYZRef(&(curNode->tbnRotC[drawMode%3]));
        lineSeg[1].multXYZ(1.0f);//(curNode->orgVecs[E_OV_TBNRAD0][drawMode%3]*scale)  ); //*16.0f
        //lineSeg[1].multXYZ(&(curNode->tbnRadScale0));
        lineSeg[1].addXYZRef(&(lineSeg[0]));
        //}

        if(GameState::gem->getCurActor()!=NULL)
        {


            if(GameState::gem->getCurActor()->isGrabbedById>-1)
            {
                grabber=&(GameState::gem->gameObjects[GameState::gem->getCurActor()->isGrabbedById]);
            }
            else
            {
                grabber=GameState::gem->getCurActor();
            }


            lineSeg[0].addXYZ(0.0f, 0.0f, grabber->skelOffset.getZ());
            lineSeg[1].addXYZ(0.0f, 0.0f, grabber->skelOffset.getZ());
        }




        switch(drawMode)
        {
        case 0: // tangents
        case 1: // bitangents
        case 2: // normals
            Renderer::drawLine(&(lineSeg[0]), &(lineSeg[1]));
            break;
        case 3: // nodes
            Renderer::drawCubeCentered(&(lineSeg[1]), 0.0125f*scale);
            break;

        }
    }


    for(i=0; i<curNode->children.size(); i++)
    {
        drawNodeEnt(curNode->children[i], basePosition, scale, drawMode, drawAll);
    }

}




void GameWorld::polyCombine()
{
    int q;

    Renderer::bindShader("PolyCombineShader");
    Renderer::bindFBO(polyFBOStrings[NUM_POLY_STRINGS]);

    for(q=0; q<NUM_POLY_STRINGS; q++)
    {
        Renderer::sampleFBO(polyFBOStrings[q], q);
    }

    Renderer::fsQuad.draw();

    for(q=NUM_POLY_STRINGS-1; q>=0; q--)
    {
        Renderer::unsampleFBO(polyFBOStrings[q], q);
    }

    Renderer::unbindFBO();
    Renderer::unbindShader();
}


void drawPolys(
    std::string fboName,
    int minPeel,
    int maxPeel
    //, bool isBlockHolder
)
{

    //VolumeWrapper* curVW = (volumeWrappers[E_VW_VORO]);

    Renderer::bindShader("PolyShader");
    Renderer::bindFBO(fboName);

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Renderer::sampleFBO("polyFBO",0,fboNum);

    ///Renderer::setShaderTexture3D(0, GameState::gameFluid[E_FID_BIG]->volIdPrim[0]);
    //Renderer::sampleFBO("hmFBOLinearBig",2);
    //Renderer::setShaderTexture3D(13, curVW->volId);

    if(polyFBOStrings[0].compare(fboName)==0)
    {
        Renderer::setShaderVec4("maskVals", 1.0f, 0.0f, 0.0f, 0.0f);
    }
    else
    {
        Renderer::setShaderVec4("maskVals", 0.0f, 1.0f, 0.0f, 0.0f);
    }

    Renderer::setShaderFloat("volSizePrim", (float)GameState::gameFluid[E_FID_BIG]->volSizePrim);
    Renderer::setShaderFloat("FOV", Renderer::FOV*(float)M_PI/180.0f);
    Renderer::setShaderVec2("clipDist", Renderer::clipDist[0], Renderer::clipDist[1]);
    Renderer::setShaderfVec2("bufferDim", &(Renderer::bufferRenderDim));
    Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
    Renderer::setShaderfVec3("volMinReadyInPixels", &(GameState::gameFluid[E_FID_BIG]->volMinReadyInPixels));
    Renderer::setShaderfVec3("lookAtVec", &(Renderer::lookAtVec));

    Renderer::setShaderMatrix4x4("modelviewInverse", Renderer::viewMatrixDI, 1);
    Renderer::setShaderMatrix4x4("modelview", Renderer::viewMatrix.get(), 1);
    Renderer::setShaderMatrix4x4("proj", Renderer::projMatrix.get(), 1);

    // if (isBlockHolder) {
    // 	rasterWorldPolys();
    // }
    // else {
    //	rasterPolys(minPeel,maxPeel*4, 6);
    //}



    // Renderer::setShaderTexture3D(13, 0);
    // Renderer::unsampleFBO("hmFBOLinearBig",2);
    // Renderer::setShaderTexture3D(0,0);

    //Renderer::unsampleFBO("polyFBO",0,fboNum);
    Renderer::unbindFBO();
    Renderer::unbindShader();
}

// void rasterWorldPolys() {


// 	GamePageHolder* curHolder = blockHolder;

// 	if (curHolder == NULL) {
// 		return;
// 	}

// 	if (
// 		(curHolder->readyToRender) &&
// 		(!(curHolder->listEmpty))
// 	) {
// 		curHolder->vboWrapper.draw();
// 	}



// }







// 	void updateTBOPool(int rad) {

// 		int q;
// 		int p;

// 		int ii;
// 		int jj;
// 		int kk;

// 		int cellsPerHolder = g_settings.cellsPerHolder;

// 		GamePageHolder* curHolder;

// 		minv.copyFrom(&camHolderPos);
// 		maxv.copyFrom(&camHolderPos);


// 		int minK = minv.getIZ() - rad;
// 		int maxK = maxv.getIZ() + rad;
// 		int minJ = minv.getIY() - rad;
// 		int maxJ = maxv.getIY() + rad;
// 		int minI = minv.getIX() - rad;
// 		int maxI = maxv.getIX() + rad;

// 		int curTBOPool = 0;
// 		int writeOffset = 0;
// 		int curCubeInd = 0;

// 		int writeOffset0;
// 		int writeOffset1;

// 		TBOEntry* curTBO = &(singleton->tboPool[0]);
// 		curTBO->lock();

// 		int curPtr;
// 		int curSize;

// 		float tempData[12];

// 		float voxelsPerHolder = singleton->voxelsPerHolder;


// 		int tboCount = 0;

// 		for (kk = minK; kk < maxK; kk++) {
// 			for (jj = minJ; jj < maxJ; jj++) {
// 				for (ii = minI; ii < maxI; ii++) {
// 					curHolder = getHolderAtCoords(ii,jj,kk,true);

// 					if (curHolder == NULL) {

// 					}
// 					else {

// 						if (curHolder->lockWrite) {

// 						}
// 						else {
// 							if (
// 								(curHolder->readyToRender) &&
// 								(!(curHolder->listEmpty))
// 							) {

// 								writeOffset = curTBO->writeIndex;

// 								if (curTBO->cantWrite(CUBE_DATA_SIZE)) {
// 									goto TBO_FULL;
// 								}


// 								// fill up cells with pointers
// 								for (q = 0; q < CUBE_DATA_SIZE; q++) {

// 									curCubeInd = curHolder->cubeData[q];

// 									if (curCubeInd == CUBE_DATA_INVALID) {
// 										curTBO->writeData(CUBE_DATA_INVALID);
// 									}
// 									else {
// 										curTBO->writeData(
// 											(curCubeInd)*CUBE_WRAP_SIZE + CUBE_DATA_SIZE + writeOffset
// 										);
// 									}


// 								}

// 								curSize = curHolder->cubeWraps.size();
// 								// fill up list of cubeWraps
// 								for (q = 0; q < curSize; q++) {

// 									if (curTBO->cantWrite(CUBE_WRAP_SIZE)) {
// 										goto TBO_FULL;
// 									}

// 									curTBO->writeDataArr(
// 										curHolder->cubeWraps[q].data,
// 										CUBE_WRAP_SIZE
// 									);
// 								}


// 								// curOffset
// 								writeOffset0 = writeOffset/4096;
// 								writeOffset1 = writeOffset - writeOffset0*4096;

// 								// vdata0
// 								tempData[0] = writeOffset0;
// 								tempData[1] = writeOffset1;
// 								tempData[2] = 0.0f;
// 								tempData[3] = 0.0f;

// 								// vdata1
// 								tempData[4] = curHolder->gphMinInCells[0];
// 								tempData[5] = curHolder->gphMinInCells[1];
// 								tempData[6] = curHolder->gphMinInCells[2];
// 								tempData[7] = 1.0f;

// 								// vdata2
// 								tempData[8] = curHolder->gphMaxInCells[0];
// 								tempData[9] = curHolder->gphMaxInCells[1];
// 								tempData[10] = curHolder->gphMaxInCells[2];
// 								tempData[11] = 1.0f;

// 								curTBO->vbo.vboBox(
// 									curHolder->gphMinInCells[0],
// 									curHolder->gphMinInCells[1],
// 									curHolder->gphMinInCells[2],

// 									0.0f,
// 									cellsPerHolder,

// 									ALL_FACES,
// 									tempData,
// 									12
// 								);

// 								tboCount++;


// 							}
// 						}
// 					}
// 				}
// 			}
// 		}

// TBO_FULL:

// 		if (curTBO->cantWrite(1)) {
// 			std::cout << "TBO FULL\n";
// 		}

// 		curTBO->unlock();


// 		std::cout << "writeIndex " << curTBO->writeIndex << "\n";

// 		std::cout << "tboCount " << tboCount << "\n";

// 		glFlush();
// 		glFinish();

// 	}








void GameWorld::rastChunk(
    int rad,
    uint flags
    // bool drawLoading,
    // bool doCheck,
    // bool clipToView
)
{



    int ii;
    int jj;
    int kk;

    bool doProc=false;

    //GamePageHolder* curHolder;
    GameChunk* curChunk;


    minv.copyFrom(&camChunkPos);
    maxv.copyFrom(&camChunkPos);

    FIVector4 tempFIV;


    int minK=minv.getIZ()-rad;
    int maxK=maxv.getIZ()+rad;
    int minJ=minv.getIY()-rad;
    int maxJ=maxv.getIY()+rad;
    int minI=minv.getIX()-rad;
    int maxI=maxv.getIX()+rad;

    float disClip=(float)(g_settings.cellsPerChunk*2);

    // if (getBounds) {
    // 	minShadowBounds.setFXYZ(16777216.0f,16777216.0f,16777216.0f);
    // 	maxShadowBounds.setFXYZ(0.0f,0.0f,0.0f);
    // }


    for(kk=minK; kk<maxK; kk++)
    {
        for(jj=minJ; jj<maxJ; jj++)
        {
            for(ii=minI; ii<maxI; ii++)
            {
                //curHolder = getHolderAtCoords(ii,jj,kk,true);
                curChunk=getChunkAtCoords(ii, jj, kk, false);

                if(curChunk==NULL)
                {

                }
                else
                {

                    if((flags&RH_FLAG_DRAWLOADING)>0)
                    {

                        curChunk->drawLoadingHolders();



                    }
                    else
                    {

                        if((flags&RH_FLAG_DOCHECK)>0)
                        {



                            curChunk->checkHolders();
                        }


                        if(
                            (curChunk->readyToRender)&&
                            (!(curChunk->listEmpty))
                            )
                        {


                            doProc=false;
                            if((flags&RH_FLAG_CLIPTOVIEW)>0)
                            {

                                tempFIV.copyFrom(&(curChunk->chunkCenInCells));
                                tempFIV.addXYZRef(Renderer::cameraGetPosNoShake(), -1.0f);
                                tempFIV.normalize();

                                if(
                                    (tempFIV.dot(&(Renderer::lookAtVec))>getConst(E_CONST_DOT_CLIP))
                                    ||(curChunk->chunkCenInCells.distance(Renderer::cameraGetPosNoShake())<disClip)
                                    )
                                {
                                    doProc=true;
                                }
                            }
                            else
                            {
                                doProc=true;
                            }


                            if(doProc)
                            {
                                if(DO_POINTS)
                                {
                                    curChunk->getCurVBO()->drawPoints();
                                }
                                else
                                {
                                    curChunk->getCurVBO()->draw();
                                }
                            }

                        }



                    }






                }
            }
        }
    }


    //float boundsDepth = maxShadowBounds.getFZ() - minShadowBounds.getFZ();

    // if (getBounds) {

    // 	minShadowBoundsGrow.copyFrom(&minShadowBounds);
    // 	minShadowBoundsGrow.addXYZ(
    // 		-abs(Renderer::lightVec.getFX())*boundsDepth,
    // 		-abs(Renderer::lightVec.getFY())*boundsDepth,
    // 		0.0f
    // 	);
    // 	minShadowBounds.minXYZ(&minShadowBoundsGrow);

    // 	maxShadowBoundsGrow.copyFrom(&maxShadowBounds);
    // 	maxShadowBoundsGrow.addXYZ(
    // 		abs(Renderer::lightVec.getFX())*boundsDepth,
    // 		abs(Renderer::lightVec.getFY())*boundsDepth,
    // 		0.0f
    // 	);
    // 	maxShadowBounds.maxXYZ(&maxShadowBoundsGrow);
    // }

}






// void rasterPolys(
// 	int minPeel,
// 	int maxPeel,
// 	int extraRad = 0,
// 	bool doPoints = false
// ) {

// 	int q;

// 	int ii;
// 	int jj;
// 	int kk;

// 	int cellsPerHolder = g_settings.cellsPerHolder;



// 	GamePageHolder* curHolder;

// 	minv.copyIntDiv(&(GameState::gameFluid[E_FID_BIG]->volMinReadyInPixels),cellsPerHolder);
// 	maxv.copyIntDiv(&(GameState::gameFluid[E_FID_BIG]->volMaxReadyInPixels),cellsPerHolder);



// 	bool idealDis = false;

// 	int pCount = 0;


// 	int bi = GameState::lastHolderPos.getIX();
// 	int bj = GameState::lastHolderPos.getIY();
// 	int bk = GameState::lastHolderPos.getIZ();

// 	int curRad = 0;


// 	int minK = minv.getIZ() - extraRad;
// 	int maxK = maxv.getIZ() + extraRad;
// 	int minJ = minv.getIY() - extraRad;
// 	int maxJ = maxv.getIY() + extraRad;
// 	int minI = minv.getIX() - extraRad;
// 	int maxI = maxv.getIX() + extraRad;

// 	int qmod;

// 	if (minPeel < maxPeel) {
// 		qmod = 1;
// 	}
// 	else {
// 		qmod = -1;
// 	}

// 	for (q = minPeel; q != maxPeel; q += qmod) {

// 		// if (q <= 1) {
// 		// 	doClear = 1;
// 		// }
// 		// else {
// 		// 	doClear = 0;
// 		// }


// 		// switch(q) {
// 		// 	case 0:
// 		// 		Renderer::setShaderVec4("maskVals", 1.0f, 0.0f, 0.0f, 0.0f);
// 		// 	break;
// 		// 	case 1:
// 		// 		Renderer::setShaderVec4("maskVals", 0.0f, 1.0f, 0.0f, 0.0f);
// 		// 	break;
// 		// 	case 2:
// 		// 		Renderer::setShaderVec4("maskVals", 0.0f, 0.0f, 1.0f, 0.0f);
// 		// 	break;
// 		// 	case 3:
// 		// 		Renderer::setShaderVec4("maskVals", 0.0f, 0.0f, 0.0f, 1.0f);
// 		// 	break;
// 		// 	case 4:
// 		// 		Renderer::setShaderVec4("maskVals", 1.0f, 0.0f, 0.0f, 0.0f);
// 		// 	break;
// 		// }


// 		// Renderer::bindFBO("polyFBO");//,swapFlag);
// 		// //Renderer::sampleFBO("polyFBO", 0, swapFlag);

// 		for (kk = minK; kk < maxK; kk++) {
// 			for (jj = minJ; jj < maxJ; jj++) {
// 				for (ii = minI; ii < maxI; ii++) {
// 					curHolder = getHolderAtCoords(ii,jj,kk,true);

// 					if (curHolder == NULL) {

// 					}
// 					else {

// 						curRad = max(max(abs(bi-ii),abs(bj-jj)),abs(bk-kk));

// 						// if (q == 4) {
// 						// 	idealDis = true;
// 						// }
// 						// else {
// 						// 	if (q == 3) {
// 						// 		idealDis = (q>=curRad);
// 						// 	}
// 						// 	else {
// 						// 		idealDis = (q==curRad);
// 						// 	}
// 						// }

// 						if (curHolder->lockWrite) {

// 						}
// 						else {
// 							if (
// 								(curHolder->readyToRender) &&
// 								(!(curHolder->listEmpty)) &&
// 								((q==curRad) || (q == -1))
// 							) {

// 								pCount++;

// 								// Renderer::setShaderFloat("volSizePrim", g_settings.cellsPerHolder);
// 								// Renderer::setShaderfVec3("volMinReadyInPixels", &(curHolder->gphMinInCells) );
// 								// Renderer::setShaderfVec3("volMaxReadyInPixels", &(curHolder->gphMaxInCells) );
// 								// Renderer::setShaderTexture3D(0, curHolder->terVW->volId);


// 								if (doPoints) {
// 									curHolder->vboWrapper.drawPoints();
// 								}
// 								else {
// 									curHolder->vboWrapper.draw();
// 								}

// 								//


// 							}
// 						}
// 					}
// 				}
// 			}
// 		}

// 		//Renderer::unsampleFBO("polyFBO", 0);//, swapFlag);
// 		//Renderer::unbindFBO();

// 		//swapFlag = 1 - swapFlag;

// 	}


// 	// Renderer::setShaderTexture3D(13, 0);
// 	// Renderer::unsampleFBO("hmFBOLinearBig",2);
// 	// Renderer::setShaderTexture3D(0,0);

// 	// Renderer::unbindShader();

// 	singleton->polyCount = pCount;


// }


void GameWorld::renderGeom()
{

//    int i;
//    int j;
//    int k;
//    int n;
//    int q;

//    int ii;
//    int jj;
//    int kk;
//    int ii2;
//    int jj2;
//    int kk2;

    FIVector4 p0;
    FIVector4 p1;
    FIVector4 p2;
    FIVector4 p3;
    FIVector4 rotVec;

//    int ind;
//    int lastId;
//    int curId;
//    int curInd;
//    int cameFromInd;

    int cellsPerHolder=g_settings.cellsPerHolder;


//    float frameMod;
    int objCount=0;

//    int curOr;
//    bool doProc;
//    int cellVal;
//    int xmax, ymax, zmax;
//    int xmin, ymin, zmin;

//    CharStruct* tempCS;

//    BaseObj* curObj;
//    BaseObj* eqObj;

    bool showHit=false;
    float dirVecLength=4.0f;

    float x1=0.0f;
    float x2=0.0f;
    float y1=0.0f;
    float y2=0.0f;
    float iw=(float)GameState::ui->fontWrappers[EFW_ICONS]->fontImage->width;
    float ih=(float)GameState::ui->fontWrappers[EFW_ICONS]->fontImage->height;



    // Renderer::bindShader("GeomShader");
    // Renderer::bindFBO("geomBaseTargFBO");
    // Renderer::setShaderFloat("objectId",0.0);
    // Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
    // Renderer::setShaderfVec3("lookAtVec", &(Renderer::lookAtVec));
    // Renderer::setShaderFloat("isWire", 0.0);
    // Renderer::setShaderFloat("clipDist",Renderer::clipDist[1]);
    // Renderer::setShaderfVec3("offsetPos",&(singleton->origin));
    // Renderer::setShaderMatrix4x4("modelview",Renderer::viewMatrix.get(),1);
    // Renderer::setShaderMatrix4x4("proj",Renderer::projMatrix.get(),1);




    // Renderer::setShaderVec3("matVal", 30, 30, 30);


    // //GameState::gamePhysics->example->renderScene();




    // for(i = 0; i < GameState::gem->visObjects.size(); i++) {

    // 	curObj = &(GameState::gem->gameObjects[visObjects[i]]);

    // 	objCount++;

    // 	if (curObj->isHidden) {

    // 	}
    // 	else {




    // 		// tempVec1.copyFrom( &(curObj->boundsMinTransInPixels) );
    // 		// tempVec2.copyFrom( &(curObj->boundsMaxTransInPixels) );



    // 		// tempVec3.copyFrom(curObj->getCenterPointFIV(0));
    // 		// tempVec3.setFW(curObj->ang);

    // 		// Renderer::setShaderfVec4("rotationZ",&tempVec3);

    // 		doProc = false;

    // 		if (visObjects[i] == singleton->actObjInd) {

    // 			if (!GameState::gem->firstPerson) {

    // 				// Renderer::drawBox(
    // 				// 	&tempVec1,
    // 				// 	&tempVec2
    // 				// );

    // 				doProc = true;




    // 				// Renderer::setShaderFloat("objectId",0);

    // 				// tempVec1.copyFrom( curObj->getCenterPointFIV(0) );


    // 				// curOr = curObj->orientationXYZ.getIX();
    // 				// tempVec2.setFXYZRef( &(singleton->dirVecs[curOr]) );
    // 				// tempVec2.multXYZ(2.0f);
    // 				// tempVec2.addXYZRef(&tempVec1);
    // 				// Renderer::setShaderVec3("matVal", 255, 0, 0);
    // 				// Renderer::drawLine(&tempVec1,&tempVec2);

    // 				// curOr = curObj->orientationXYZ.getIY();
    // 				// tempVec2.setFXYZRef( &(singleton->dirVecs[curOr]) );
    // 				// tempVec2.multXYZ(2.0f);
    // 				// tempVec2.addXYZRef(&tempVec1);
    // 				// Renderer::setShaderVec3("matVal", 0, 255, 0);
    // 				// Renderer::drawLine(&tempVec1,&tempVec2);

    // 				// curOr = curObj->orientationXYZ.getIZ();
    // 				// tempVec2.setFXYZRef( &(singleton->dirVecs[curOr]) );
    // 				// tempVec2.multXYZ(2.0f);
    // 				// tempVec2.addXYZRef(&tempVec1);
    // 				// Renderer::setShaderVec3("matVal", 0, 0, 255);
    // 				// Renderer::drawLine(&tempVec1,&tempVec2);


    // 				// Renderer::setShaderVec3("matVal", 30, 30, 30);
    // 			}



    // 		}
    // 		else {

    // 			if (
    // 				(curObj->entType == E_ENTTYPE_BULLET) ||
    // 				(curObj->entType == E_ENTTYPE_TRACE) ||
    // 				(
    // 					(GameState::gem->firstPerson) &&
    // 					(curObj->uid == GameState::gem->getCurActorUID())
    // 				)
    // 			) {

    // 			}
    // 			else {
    // 				// Renderer::drawBox(
    // 				// 	&tempVec1,
    // 				// 	&tempVec2
    // 				// );

    // 				doProc = true;
    // 			}


    // 		}

    // 		if (doProc) {

    // 			Renderer::setShaderFloat("objectId",visObjects[i]);

    // 			// if (curObj->body != NULL) {
    // 			// 	glBegin( GL_TRIANGLES );
    // 			// 	//curObj->body->Render(&q3Rend);
    // 			// 	glEnd( );
    // 			// }
    // 		}

    // 	}






    // }



    // tempVec3.setFXYZW(0.0f,0.0f,0.0f,0.0f);
    // Renderer::setShaderfVec4("rotationZ",&tempVec3);
    // Renderer::setShaderFloat("objectId",0.0);






    // // Renderer::setShaderFloat("isWire", 1.0);
    // // Renderer::setShaderVec3("matVal", 255, 0, 0);
    // // minv.setFXYZRef(&(GameState::gameFluid[E_FID_BIG]->curDirtyMin));
    // // maxv.setFXYZRef(&(GameState::gameFluid[E_FID_BIG]->curDirtyMax));
    // // minv.addXYZRef(&(GameState::gameFluid[E_FID_BIG]->volMinReadyInPixels));
    // // maxv.addXYZRef(&(GameState::gameFluid[E_FID_BIG]->volMinReadyInPixels));
    // // minv.addXYZ(-4.0f,-4.0f,0.0f);
    // // maxv.addXYZ(4.0f,4.0f,0.0f);
    // // Renderer::drawBox(&minv, &maxv);


    // // Renderer::setShaderFloat("isWire", 1.0);
    // // Renderer::setShaderVec3("matVal", 255, 0, 0);
    // // minv.setFXYZRef(&(GameState::gameFluid[E_FID_BIG]->curWaterMin));
    // // maxv.setFXYZRef(&(GameState::gameFluid[E_FID_BIG]->curWaterMax));
    // // minv.addXYZ(-4.0f,-4.0f,0.0f);
    // // maxv.addXYZ(4.0f,4.0f,0.0f);
    // // Renderer::drawBox(&minv, &maxv);




    // switch (singleton->mouseState)
    // {

    // case E_MOUSE_STATE_MOVE:



    // 	// if (g_settings.settings[E_BS_PLACING_GEOM]) {


    // 	// 	Renderer::setShaderVec3("matVal", 255, 0, 0);
    // 	// 	Renderer::setShaderFloat("isWire", 0.0);

    // 	// 	for (i = 0; i <= singleton->geomStep; i++) {

    // 	// 		Renderer::drawCubeCentered(
    // 	// 			&geomPoints[i],
    // 	// 			0.25f	
    // 	// 		);
    // 	// 	}
    // 	// }

    // 	break;

    // case E_MOUSE_STATE_BRUSH:


    // 	if (earthMod == E_PTT_TER) {
    // 		Renderer::setShaderVec3("matVal", 255, 0, 0);
    // 	}
    // 	else {
    // 		Renderer::setShaderVec3("matVal", 0, 0, 255);
    // 	}


    // 	Renderer::setShaderFloat("isWire", 1.0);
    // 	Renderer::drawCubeCentered(
    // 		&lastUnitPos,
    // 		((int)curBrushRad)
    // 	);


    // 	//glClear(GL_DEPTH_BUFFER_BIT);
    // break;
    // case E_MOUSE_STATE_PICKING:




    // 	if (singleton->highlightedEnt == NULL)
    // 	{

    // 	}
    // 	else
    // 	{
    // 		Renderer::setShaderVec3("matVal", 254, 254, 254);
    // 		Renderer::setShaderFloat("isWire", 1.0);

    // 		minv.setFXYZRef(singleton->highlightedEnt->getVisMinInPixelsT());
    // 		maxv.setFXYZRef(singleton->highlightedEnt->getVisMaxInPixelsT());

    // 		minv.addXYZ(-0.25);
    // 		maxv.addXYZ(0.25);


    // 		Renderer::drawBox(&minv, &maxv);
    // 	}

    // 	if (singleton->selectedEnts.getSelectedEnt() == NULL)
    // 	{

    // 	}
    // 	else
    // 	{
    // 		Renderer::setShaderVec3("matVal", 255, 255, 0);
    // 		Renderer::setShaderFloat("isWire", 1.0);

    // 		minv.setFXYZRef(singleton->selectedEnts.getSelectedEnt()->getVisMinInPixelsT());
    // 		maxv.setFXYZRef(singleton->selectedEnts.getSelectedEnt()->getVisMaxInPixelsT());

    // 		Renderer::drawBox(&minv, &maxv);
    // 	}


    // 	break;

    // }

    // if (g_settings.settings[E_BS_PATH_FINDING]) {
    // 	GameState::gameLogic->update();
    // }



    // if (
    // 	(singleton->bCtrl) &&
    // 	(singleton->mouseState != E_MOUSE_STATE_BRUSH) &&
    // 	(!(singleton->anyMenuVisible()))	
    // ) {
    // 	for (i = 1; i < singleton->dynObjects.size(); i++) {
    // 		if (singleton->dynObjects[i]->doRender)
    // 		{

    // 			doProc = false;

    // 			if (i >= E_OBJ_LIGHT0)
    // 			{
    // 				if (i - E_OBJ_LIGHT0 < singleton->numDynLights)
    // 				{
    // 					doProc = true;
    // 				}
    // 			}
    // 			else
    // 			{
    // 				doProc = true;
    // 			}

    // 			if (doProc)
    // 			{
    // 				Renderer::setShaderFloat("objectId",i);
    // 				Renderer::setShaderfVec3("matVal", &(singleton->dynObjects[i]->color) );
    // 				curBoxPos = &(singleton->dynObjects[i]->pos);
    // 				Renderer::drawCubeCentered(curBoxPos, singleton->dynObjects[i]->radius);


    // 				if (i == singleton->activeObject)
    // 				{
    // 					//singleton->drawCrossHairs(singleton->dynObjects[i]->pos,4.0f);
    // 				}
    // 			}


    // 		}
    // 	}
    // }


    // // Renderer::setShaderVec3("matVal", 254, 0, 0);
    // // Renderer::setShaderFloat("isWire", 1.0);
    // // for (i = 0; i < singleton->nearestLights.selEntList.size(); i++) {


    // // 	minv.setFXYZRef(singleton->nearestLights.selEntList[i]->getVisMinInPixelsT());
    // // 	maxv.setFXYZRef(singleton->nearestLights.selEntList[i]->getVisMaxInPixelsT());


    // // 	Renderer::drawBox(&minv, &maxv);
    // // }





    // Renderer::unbindFBO();
    // Renderer::unbindShader();



//    float yval;





    //~~~~~~~~~~~~~~

    Renderer::bindShader("BoxShader");
    Renderer::bindFBO("geomBaseTargFBO");//, -1, 0); //solidTargFBO
    Renderer::setShaderfVec3("lightVec", &(Renderer::lightVec));
    Renderer::setShaderFloat("objectId", 0.0);
    Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
    Renderer::setShaderfVec3("lookAtVec", &(Renderer::lookAtVec));
    Renderer::setShaderFloat("isWire", 0.0);
    Renderer::setShaderFloat("clipDist", Renderer::clipDist[1]);
    Renderer::setShaderMatrix4x4("modelview", Renderer::viewMatrix.get(), 1);
    Renderer::setShaderMatrix4x4("proj", Renderer::projMatrix.get(), 1);
    Renderer::setShaderVec3("matVal", 1, 1, 1);

    // glBegin( GL_TRIANGLES );
    // //m_data->m_gl2ShapeDrawer->drawScene(rbWorld,true);
    // GameState::gamePhysics->myShapeDrawer->drawScene(
    // 	GameState::gamePhysics->example->getDynamicsWorld(), true); //drawAll();
    // glEnd( );

    //glMatrixMode(GL_MODELVIEW);		
    // GameState::gamePhysics->myShapeDrawer->drawScene(
    // 	GameState::gamePhysics->example->getDynamicsWorld(),
    // 	false
    // );



    if(GameState::gamePhysics!=NULL)
    {
        Renderer::drawOrient=false;
        GameState::gamePhysics->example->renderScene();
    }


    Renderer::unbindFBO();
    Renderer::unbindShader();


    Renderer::copyFBO2("geomBaseTargFBO", "geomTargFBO", 0, 1);


    //~~~~~~~~~~~~~~






    //////////////////////////////


    // Renderer::bindShader("CylBBShader");
    // Renderer::bindFBO("geomBaseTargFBO", -1, 0);
    // Renderer::setShaderfVec2("bufferDim", &(Renderer::bufferModDim) );
    // Renderer::setShaderFloat("clipDist",Renderer::clipDist[1]);
    // Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
    // Renderer::setShaderFloat("curTime", GameState::pauseTime/1000.0f);
    // Renderer::setShaderTexture(0,GameState::ui->fontWrappers[EFW_ICONS]->fontImage->tid);

    // for(i = 0; i < GameState::gem->visObjects.size(); i++) {

    // 	curObj = &(GameState::gem->gameObjects[visObjects[i]]);

    // 	eqObj = singleton->getEquipped(curObj);

    // 	if (
    // 		curObj->isHidden ||
    // 		(eqObj == NULL) ||
    // 		(
    // 			(GameState::gem->firstPerson) &&
    // 			(curObj->uid == GameState::gem->getCurActorUID())
    // 		)
    // 	) {

    // 	}
    // 	else {



    // 		/////
    // 		tempVec1.copyFrom( curObj->getCenterPointFIV(0) );
    // 		curOr = curObj->orientationXYZ.getIY();
    // 		tempVec2.setFXYZRef( &(singleton->dirVecs[curOr]) );
    // 		//tempVec2.multXYZ(1.0f);
    // 		tempVec2.addXYZRef(&tempVec1);
    // 		//tempVec3.averageXYZ(&tempVec1,&tempVec2);


    // 		rotVec.copyFrom(curObj->getCenterPointFIV(0));
    // 		//rotVec.addXYZ(0.0,2.0,0.0);
    // 		rotVec.setFW( curObj->ang + curObj->angRelative );
    // 		Renderer::setShaderfVec4("rotZ",&rotVec);
    // 		Renderer::setShaderfVec4("rotZ2",&tempVec2);

    // 		p0.copyFrom(&tempVec2);
    // 		p1.copyFrom(&tempVec2);
    // 		p2.copyFrom(&tempVec2);
    // 		p3.copyFrom(&tempVec2);

    // 		p0.addXYZ(-0.5f, -0.5f,  0.0f);
    // 		p1.addXYZ( 0.5f, -0.5f,  0.0f);
    // 		p2.addXYZ( 0.5f,  0.5f,  0.0f);
    // 		p3.addXYZ(-0.5f,  0.5f,  0.0f);

    // 		/////



    // 		tempCS = &(GameState::ui->fontWrappers[EFW_ICONS]->charVals[
    // 			GameState::gem->entIdToIcon[eqObj->objectType] 
    // 		]);
    // 		frameMod = 0;
    // 		if (eqObj->maxFrames != 0) {
    // 			frameMod = ((singleton->currentTick/(20))%(eqObj->maxFrames))*16.0f;
    // 		}


    // 		x1 = (tempCS->sampX + frameMod )/iw;
    // 		x2 = (tempCS->sampX + tempCS->sampW + frameMod)/iw;
    // 		y1 = 1.0f-(tempCS->sampY)/ih;
    // 		y2 = 1.0f-(tempCS->sampY+tempCS->sampH)/ih;

    // 		if (
    // 			(eqObj->entType == E_ENTTYPE_BULLET) ||
    // 			(eqObj->entType == E_ENTTYPE_TRACE)	
    // 		) {
    // 			yval = 1.0f;
    // 		}
    // 		else {
    // 			yval = -1.0f;
    // 		}

    // 		// glMultiTexCoord4f(GL_TEXTURE0, visObjects[i], yval, 1.0f, 1.0f);
    // 		// glMultiTexCoord4f(GL_TEXTURE1, x1,y1,x2,y2);



    // 		singleton->drawQuadWithCoords(
    // 				&p0,
    // 				&p1,
    // 				&p2,
    // 				&p3,

    // 				x1, y1, x2, y2
    // 		);

    // 	}



    // }
    // Renderer::setShaderTexture(0,0);
    // Renderer::unbindFBO();
    // Renderer::unbindShader();












    // Renderer::copyFBO2("geomBaseTargFBO","geomTargFBO");


    // Renderer::bindShader("BillboardShader");
    // Renderer::setShaderfVec2("bufferDim", &(Renderer::bufferModDim) );
    // Renderer::setShaderFloat("cellsPerHolder",cellsPerHolder);
    // Renderer::setShaderFloat("heightOfNearPlane",singleton->heightOfNearPlane);
    // Renderer::setShaderFloat("clipDist",Renderer::clipDist[1]);
    // Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
    // Renderer::setShaderTexture(0,GameState::ui->fontWrappers[EFW_ICONS]->fontImage->tid);
    // Renderer::sampleFBO("geomBaseTargFBO",1);
    // Renderer::bindFBO("geomTargFBO", -1, 0);


    // glBegin(GL_POINTS);

    // for(i = 0; i < GameState::gem->visObjects.size(); i++) {

    // 	curObj = &(GameState::gem->gameObjects[visObjects[i]]);

    // 	if (
    // 		curObj->isHidden ||
    // 		(curObj->objectType <= 0) ||
    // 		(
    // 			(GameState::gem->firstPerson) &&
    // 			(curObj->uid == GameState::gem->getCurActorUID())
    // 		)
    // 	) {

    // 	}
    // 	else {
    // 		tempCS = &(GameState::ui->fontWrappers[EFW_ICONS]->charVals[
    // 			GameState::gem->entIdToIcon[curObj->objectType] 
    // 		]);
    // 		frameMod = 0;
    // 		if (curObj->maxFrames != 0) {
    // 			frameMod = ((singleton->currentTick/(20))%(curObj->maxFrames))*16.0f;
    // 		}


    // 		x1 = (tempCS->sampX + frameMod )/iw;
    // 		x2 = (tempCS->sampX + tempCS->sampW + frameMod)/iw;
    // 		y1 = 1.0f-(tempCS->sampY)/ih;
    // 		y2 = 1.0f-(tempCS->sampY+tempCS->sampH)/ih;

    // 		if (
    // 			(curObj->entType == E_ENTTYPE_BULLET) ||
    // 			(curObj->entType == E_ENTTYPE_TRACE)	
    // 		) {
    // 			yval = 1.0f;
    // 		}
    // 		else {
    // 			yval = -1.0f;
    // 		}

    // 		glMultiTexCoord4f(GL_TEXTURE0, visObjects[i], yval, 1.0f, 1.0f);
    // 		glMultiTexCoord4f(GL_TEXTURE1, x1,y1,x2,y2);
    // 		glVertex3f(
    // 			curObj->getCenterPointFIV(0)->getFX(),
    // 			curObj->getCenterPointFIV(0)->getFY(),
    // 			curObj->getCenterPointFIV(0)->getFZ()
    // 		);
    // 	}



    // }
    // glEnd();



    // Renderer::unsampleFBO("geomBaseTargFBO",1);
    // Renderer::setShaderTexture(0,0);
    // Renderer::unbindFBO();
    // Renderer::unbindShader();

    //////////////////////////////



}


void GameWorld::updateMouseCoords(FIVector4 *fPixelWorldCoordsBase)
{

    FIVector4 fPixelWorldCoords;
    fPixelWorldCoords.copyFrom(fPixelWorldCoordsBase);


    lastUnitPos.copyFrom(&fPixelWorldCoords);
    lastUnitPos.setFW(1.0);
    lastUnitPos.floorXYZ();

}





float GameWorld::weighPath(float x1, float y1, float x2, float y2, float rad, bool doSet, bool isOcean)
{

    int i;
    int iMax=(int)std::min(64.0f, rad * 4.0f);

    float fi;
    float fMax=(float)(iMax-1);
    float lerp;

    float curX;
    float curY;


    float startVal=(float)curFBO->getPixelAtWrapped((int)x1, (int)y1, hmChannel);
    float lastRes=startVal;
    float curRes;
//    float curRes2;
    float tempRes;
    float tot=0.0f;

    bool startsInWater=startVal<=seaLevel;

    for(i=1; i<iMax; i++)
    {
        fi=(float)i;
        lerp=fi/fMax;
        curX=(1.0f-lerp) * x1+(lerp)* x2;
        curY=(1.0f-lerp) * y1+(lerp)* y2;
        curRes=(float)curFBO->getPixelAtWrapped((int)curX, (int)curY, hmChannel);

        if(isOcean)
        {
            if(doSet)
            {

                if(curRes>seaLevel)
                {

                    tempRes=abs(curRes-lastRes);
                    tempRes=tempRes*tempRes * tempRes;
                    //tempRes = 255-curRes;
                }
                else
                {
                    tempRes=curRes;
                }


                //tempRes = abs(curRes-lastRes);
                //tempRes = tempRes*tempRes*tempRes;
                if((curRes>seaLevel)!=(lastRes>seaLevel))
                {
                    tempRes+=100000.0f;
                }

                // if (startsInWater) {
                //  if (curRes > seaLevel) {
                //      tempRes += 1000000.0f;
                //  }
                // }

            }
            else
            {
                tempRes=0.0;
                if(curRes>seaLevel)
                {
                    tempRes=1.0f;
                }
                else
                {
                    tempRes=-1.0f;
                }
            }
        }
        else
        {
            tempRes=abs(curRes-lastRes) * 10.0f+curRes;
            //tempRes = tempRes;
            if(curRes<=seaLevel-20.0f)
            {
                tempRes+=100000.0f;
            }
            if((curRes>seaLevel)!=(lastRes>seaLevel))
            {
                tempRes+=100000.0f;
            }
        }


        tot+=tempRes;
        lastRes=curRes;

    }

    return tot;
}

float GameWorld::findBestPath(float x1, float y1, float x2, float y2, int generation, int roadIndex, bool doSet, bool isOcean)
{
    int i;
//    int j;

    coordAndIndex baseCoord;
    coordAndIndex bestCoord;
    coordAndIndex testCoord;

    float mpx=(x1+x2)/2.0f;
    float mpy=(y1+y2)/2.0f;
    float dis=quickDis(x1, y1, x2, y2);
    float rad=dis/2.0f;
    float mpxTemp;
    float mpyTemp;
    float delta;
    float bestDis;
    float curDis;
    float bestDelta=FLT_MAX;
    float bestX;
    float bestY;
    float genMod;

//    int q;
//    int p;

    int ibx;
    int iby;
    int ix2;
    int iy2;
    int tot1=0;
    int tot2=0;
//    int iRad;
    int numTries=std::max((int)(rad), 20);

    if((rad<2.0f)||(generation>1024))
    {
        // do manhattan distance

        if(doSet)
        {
            ibx=(int)x1;
            iby=(int)y1;
            ix2=(int)x2;
            iy2=(int)y2;


            while(ibx!=ix2)
            {
                curFBO2->setPixelAtWrapped(ibx, iby, pathChannel, 255);
                if(ibx<ix2)
                {
                    ibx++;
                }
                else
                {
                    ibx--;
                }
            }
            while(iby!=iy2)
            {
                curFBO2->setPixelAtWrapped(ibx, iby, pathChannel, 255);
                if(iby<iy2)
                {
                    iby++;
                }
                else
                {
                    iby--;
                }
            }

            curFBO2->setPixelAtWrapped(ibx, iby, pathChannel, 255);
        }
        else
        {

        }

        return 0.0f;

    }



    genMod=2.0f;

    // if (generation > 32) {
    //  return 0.0f;
    // }




    for(i=0; i<numTries; i++)
    {
        mpxTemp=mpx+(fGenRand() * dis-rad)/genMod;
        mpyTemp=mpy+(fGenRand() * dis-rad)/genMod;

        delta=weighPath(x1, y1, mpxTemp, mpyTemp, rad/2.0f, doSet, isOcean);
        delta+=weighPath(mpxTemp, mpyTemp, x2, y2, rad/2.0f, doSet, isOcean);

        if(delta<bestDelta)
        {
            bestDelta=delta;
            bestX=mpxTemp;
            bestY=mpyTemp;
        }

    }



    if(doSet)
    {



        if(generation<8)
        {

            baseCoord.x=bestX;
            baseCoord.y=bestY;
            baseCoord.index=roadIndex;
            //roadCoords.push_back(baseCoord);

            bestDis=FLT_MAX;
            for(i=0; i<roadCoords.size(); i++)
            {
                testCoord=roadCoords[i];


                if(baseCoord.index==testCoord.index)
                {

                }
                else
                {
                    curDis=coordDis(&baseCoord, &testCoord);

                    if(curDis<bestDis)
                    {
                        bestDis=curDis;
                        bestCoord=roadCoords[i];
                    }
                }


            }

            if(bestDis!=FLT_MAX)
            {
                curDis=coordDis(&bestCoord, &baseCoord);

                if(curDis<std::min(400.0f, rad))   //
                {
                    baseCoord=bestCoord;
                    baseCoord.index=roadIndex;
                }

                bestX=baseCoord.x;
                bestY=baseCoord.y;
            }

            roadCoords.push_back(baseCoord);
        }


        //curFBO2->setPixelAtWrapped((int)bestX, (int)bestY, pathChannel, 255);
        //curFBO2->setPixelAtWrapped((int)bestX, (int)bestY, breadCrumbChannel, 255);
        findBestPath(x1, y1, bestX, bestY, generation+1, roadIndex, doSet, isOcean);
        findBestPath(bestX, bestY, x2, y2, generation+1, roadIndex, doSet, isOcean);
    }

    return bestDelta;

}

void GameWorld::initMap()
{
    mapLockOn=true;

    mapSwapFlag=0;

    mapStep=0.0f;



    FBOWrapper *fbow=FBOManager::getFBOWrapper("hmFBO", 0);
    FBOWrapper *fbow2=FBOManager::getFBOWrapper("cityFBO", 0);

    curFBO=fbow;
    curFBO2=fbow2;



    FIVector4 startVec;
    FIVector4 endVec;
    FIVector4 midPointVec;

    int w=fbow2->width;
    int h=fbow2->height;


    //0:r
    //1:g
    //2:b
    //3:a




    int i;
    int j;
    int k;
    int m;
    int q;

    int totSize=w*h;
    int *btStack=new int[totSize];
    int btStackInd=0;
    int curInd;
    int curX;
    int curY;
//    int destX;
//    int destY;
//    int lastDir;
//    int curXRight;
//    int curXLeft;
//    int curYUp;
//    int curYDown;
    int testX;
    int testY;
    int testInd;
    int basePix;
//    int basePix2;
    int testPix;
    int testPix1;
    int testPix2;
    int testPix3;
    int testPix4;
    int count;
    int p1;
    int p2;



    // int alleyFlag = 32;
    // int streetFlag = 64;
    // int highwayFlag = 128;

    int startDir;
    int curDir;
    blockMip=intLogB2(g_settings.holdersPerBlock);

//    int cx1;
//    int cy1;
//    int cx2;
//    int cy2;

    int histogram[256];

    float delta;
    float bestDelta;
//    float nextBestDelta;

//    int nextBestInd;
    int bestDir;
//    int nextBestDir;

    int bestInd;
    int tempVal;

    int xind;
    int yind;
    int curHeight;

    bool isValid;
    bool notFound;
//    bool touchesWater;
//    bool doBreak;

//    float mult;
    float tempDis;

    int minSL=0;
    int maxSL=0;
    int curSL=0;
    int avgSL=0;
    //int totFilled = 0;



    for(i=0; i<16; i++)
    {
        paramArrMap[i*3+0]=fGenRand();
        paramArrMap[i*3+1]=fGenRand();
        paramArrMap[i*3+2]=fGenRand();
    }

    for(i=0; i<6; i++)
    {
        paramArrMap[i*3+0]=(float)i;
    }

    for(i=0; i<30; i++)
    {
        p1=iGenRand(0, 5);
        p2=iGenRand(0, 5);

        if(p1!=p2)
        {
            tempVal=(int)paramArrMap[p1];
            paramArrMap[p1]=paramArrMap[p2];
            paramArrMap[p2]=(float)tempVal;
        }
    }



    for(q=0; q<2; q++)
    {


        Renderer::bindShader("Simplex2D");
        Renderer::bindFBO("simplexFBO");
        Renderer::setShaderFloat("curTime", fGenRand() * 100.0f);
        Renderer::drawFSQuad();
        Renderer::unbindFBO();
        Renderer::unbindShader();

        Renderer::bindShader("TerrainMix");
        Renderer::bindFBO("hmFBOLinear");
        Renderer::sampleFBO("simplexFBO", 0);
        Renderer::setShaderTexture(1, imageHM0->tid);
        Renderer::setShaderTexture(2, imageHM1->tid);
        Renderer::setShaderInt("passNum", q);
        Renderer::setShaderVec2("minAndMax", ((float)minSL)/255.0f, ((float)maxSL)/255.0f);
        Renderer::setShaderArrayfVec3("paramArrMap", paramArrMap, 16);
        //Renderer::setShaderFloat("mapSampScale", 1.0f); //singleton->mapSampScale
        Renderer::drawFSQuad();
        Renderer::setShaderTexture(2, 0);
        Renderer::setShaderTexture(1, 0);
        Renderer::unsampleFBO("simplexFBO", 0);
        Renderer::unbindFBO();
        Renderer::unbindShader();

        Renderer::copyFBO("hmFBOLinear", "hmFBO");

        glFlush();
        glFinish();

        fbow->getPixels(true);
        fbow->setAllPixels(densityChannel, 255);
        fbow->setAllPixels(idChannel, 0);
        fbow->setAllPixels(blockChannel, 0);


        // determine sea level

        for(i=0; i<256; i++)
        {
            histogram[i]=0;
        }

        minSL=255;
        maxSL=0;
        curSL=0;
        avgSL=0;
        //totFilled = 0;

        for(i=0; i<totSize; i++)
        {
            curSL=fbow->getPixelAtIndex(i, hmChannel);

            if(curSL<minSL)
            {
                minSL=curSL;
            }
            if(curSL>maxSL)
            {
                maxSL=curSL;
            }

            histogram[curSL]++;
        }

        avgSL=(minSL+maxSL)/2;
    }

    seaLevel=100;


    Renderer::copyFBO("hmFBOLinear", "hmFBOLinearBig");


    seaSlack=seaLevel-1;
    std::cout<<"Sea Level: "<<seaLevel<<"\n";

    mapWidth=(float)fbow->width;
    mapHeight=(float)fbow->height;

    goto DONE_WITH_MAP;


    std::cout<<"start place cities\n";

    for(i=1; i<numProvinces; i++)
    {

        isValid=false;

        do
        {

            xind=(int)(fGenRand() * fbow->width);
            yind=(int)(fGenRand() * fbow->height);

            tempVec1.setFXYZ((float)xind, (float)yind, 0.0f);

            notFound=false;
            for(j=1; j<i; j++)
            {
                tempVec2.setFXYZ((float)provinceX[j], (float)provinceY[j], 0.0f);
                tempDis=tempVec2.wrapDistance(&tempVec1, w, false);

                if(tempDis<10.0f)
                {
                    notFound=true;
                }

            }

            if(notFound)
            {

            }
            else
            {
                if(fbow->getPixelAtC(xind, yind, idChannel)==0)
                {
                    curHeight=fbow->getPixelAtC(xind, yind, hmChannel);

                    if(
                        (curHeight>seaLevel)
                        )
                    {

                        provinceX[i]=xind;
                        provinceY[i]=yind;
                        fbow->setPixelAtC(xind, yind, idChannel, i);
                        fbow->setPixelAtC(xind, yind, densityChannel, 0);
                        isValid=true;

                    }
                }
            }



        } while(!isValid);

    }

    std::cout<<"end place cities\n";


    std::cout<<"start grow provinces\n";

    fbow->cpuToGPU();

    Renderer::copyFBO("hmFBO", "swapFBO0");
    Renderer::bindShader("MapBorderShader");
    mapStep=0.0f;
    for(i=0; i<1024; i++)
    {

        Renderer::bindFBO("swapFBO", mapSwapFlag);
        Renderer::sampleFBO("swapFBO", 0, mapSwapFlag);
        Renderer::setShaderFloat("seaSlack", ((float)seaSlack)/255.0f);
        Renderer::setShaderFloat("mapStep", mapStep);
        Renderer::setShaderFloat("texPitch", (float)w);
        Renderer::drawFSQuad();
        Renderer::unsampleFBO("swapFBO", 0, mapSwapFlag);
        Renderer::unbindFBO();


        mapSwapFlag=1-mapSwapFlag;
        mapStep+=1.0f;
    }
    Renderer::unbindShader();


    Renderer::bindShader("MapBorderShader");
    mapStep=1.0f;
    for(i=0; i<256; i++)
    {

        Renderer::bindFBO("swapFBO", mapSwapFlag);
        Renderer::sampleFBO("swapFBO", 0, mapSwapFlag);
        Renderer::setShaderFloat("seaSlack", ((float)seaSlack)/255.0f);
        Renderer::setShaderFloat("mapStep", -mapStep);
        Renderer::setShaderFloat("texPitch", (float)w);
        Renderer::drawFSQuad();
        Renderer::unsampleFBO("swapFBO", 0, mapSwapFlag);
        Renderer::unbindFBO();


        mapSwapFlag=1-mapSwapFlag;
        mapStep+=1.0f;
    }
    Renderer::unbindShader();


    Renderer::copyFBO("swapFBO0", "hmFBO");


    fbow->getPixels();
    fbow->updateMips();

    std::cout<<"end grow provinces\n";


    std::cout<<"start find neighboring cities\n";

    for(i=0; i<numProvinces * numProvinces; i++)
    {
        provinceGrid[i]=0;
    }

    for(k=0; k<totSize; k++)
    {
        curInd=k;
        curY=curInd/w;
        curX=curInd-curY*w;

        basePix=fbow->getPixelAtIndex(curInd, idChannel);

        testPix=fbow->getPixelAtIndex(fbow->getIndex(curX+1, curY), idChannel);
        testPix2=fbow->getPixelAtIndex(fbow->getIndex(curX, curY+1), idChannel);


        if(basePix!=0)
        {
            if(testPix!=0)
            {
                if(basePix!=testPix)
                {

                    provinceGrid[basePix * numProvinces+testPix]=1;
                    provinceGrid[basePix+testPix*numProvinces]=1;

                }
            }
            if(testPix2!=0)
            {
                if(basePix!=testPix2)
                {
                    provinceGrid[basePix * numProvinces+testPix2]=1;
                    provinceGrid[basePix+testPix2*numProvinces]=1;
                }
            }
        }


    }

    std::cout<<"end find neighboring cities\n";




    // 1 - x+
    // 2 - x-
    // 4 - y+
    // 8 - y-


    std::cout<<"start find city blocks\n";


    fbow2->getPixels(true);
    fbow2->setAllPixels(btChannel, 15);
    fbow2->setAllPixels(stChannel, 0);
    fbow2->setAllPixels(pathChannel, 0);
    fbow2->setAllPixels(houseChannel, 0);


    int blockMod=g_settings.holdersPerBlock;
    for(k=0; k<totSize; k++)
    {
        curInd=k;
        curY=curInd/w;
        curX=curInd-curY*w;

        basePix=fbow->getMipVal(curX, curY, blockMip, idChannel, MAX_MIP);
        testPix=fbow->getMipVal(curX, curY, blockMip, densityChannel, AVG_MIP);

        testPix1=fbow->getMipVal(curX, curY, blockMip, idChannel, MAX_MIP, -1, -1, 0);
        testPix2=fbow->getMipVal(curX, curY, blockMip, idChannel, MAX_MIP, -1, 1, 0);
        testPix3=fbow->getMipVal(curX, curY, blockMip, idChannel, MAX_MIP, -1, 0, -1);
        testPix4=fbow->getMipVal(curX, curY, blockMip, idChannel, MAX_MIP, -1, 0, 1);


        // TODO: EDIT CITY

        //testPix > xxx <- (xxx = 0: no city, xxx = 255: all city, def: 220)
        if(testPix1!=testPix2||testPix3!=testPix4||testPix>0)
        {
            fbow->setPixelAtIndex(curInd, blockChannel, 0);
        }
        else
        {
            fbow->setPixelAtIndex(curInd, blockChannel, basePix);
        }

    }

    fbow->cpuToGPU();
    Renderer::copyFBO("hmFBO", "hmFBOLinear");

    std::cout<<"end find city blocks\n";


    std::cout<<"start add in city roads\n";
    //add in city roads

    for(i=0; i<numProvinces; i++)
    {
        // recursive backtrack
        btStack[0]=fbow2->getIndex(provinceX[i], provinceY[i]);
        btStackInd=0;

        while(btStackInd>-1)
        {

            curInd=btStack[btStackInd];
            curY=curInd/w;
            curX=curInd-curY*w;

            fbow2->orPixelAtIndex(curInd, btChannel, visFlag);

            startDir=0;
            count=0;
            notFound=true;
            bestDelta=FLT_MAX;


            testPix2=fbow->getPixelAtIndex(curInd, blockChannel);

            //testPix2 = fbow->getMipVal(curX,curY,blockMip,densityChannel,AVG_MIP);
            //testPix3 = fbow->getMipVal(curX,curY,blockMip,idChannel,MIN_MIP);
            //testPix4 = fbow->getMipVal(curX,curY,blockMip,idChannel,MAX_MIP);


            if(testPix2!=0)
            {
                do
                {
                    curDir=(startDir+count)%4;

                    testX=curX+dirModX[curDir];
                    testY=curY+dirModY[curDir];
                    testInd=fbow2->getIndex(testX, testY);
                    testPix=fbow2->getPixelAtIndex(testInd, btChannel);
                    testPix3=fbow->getPixelAtIndex(testInd, blockChannel);

                    if((testPix & visFlag)==0&&(testPix3!=0))
                    {
                        //not visited, proceed
                        notFound=false;


                        delta=(float)abs(
                            fbow->getPixelAtIndex(curInd, hmChannel)-
                            fbow->getPixelAtIndex(testInd, hmChannel)
                        );

                        if(delta<bestDelta)
                        {
                            bestDelta=delta;
                            bestDir=curDir;
                            bestInd=testInd;
                        }

                    }

                    count++;
                } while(count<4); //notFound &&
            }

            if(notFound)
            {
                btStackInd--;
            }
            else
            {

                // join the two and remove walls
                fbow2->andPixelAtIndex(curInd, btChannel, dirFlags[bestDir]);
                fbow2->andPixelAtIndex(bestInd, btChannel, dirFlagsOp[bestDir]);

                btStackInd++;
                btStack[btStackInd]=bestInd;
            }

        }
    }

    std::cout<<"end add in city roads\n";



    // clear visited
    for(k=0; k<totSize; k++)
    {
        testPix=fbow2->getPixelAtIndex(k, btChannel);
        if((testPix & visFlag)==0)
        {
            //not visited
            for(i=0; i<4; i++)
            {
                fbow2->andPixelAtIndex(k, btChannel, dirFlags[i]);
            }
        }
        else
        {
            //visited
        }

        fbow2->andPixelAtIndex(k, btChannel, visFlagO);
    }



    std::cout<<"start link close cities\n";

    // link close cities

    for(i=1; i<numProvinces-1; i++)
    {
        for(j=i+1; j<numProvinces; j++)
        {

            curInd=i+j*numProvinces;

            if(provinceGrid[curInd]==1)
            {
                p1=i;
                p2=j;



                tempVec1.setIXYZ(provinceX[p1], provinceY[p1], 0);
                tempVec2.setIXYZ(provinceX[p2], provinceY[p2], 0);

                tempVec2.wrapDistance(&tempVec1, w);
                tempVec3.copyFrom(&tempVec1);

                findBestPath(
                    tempVec2.getFX(),
                    tempVec2.getFY(),
                    tempVec3.getFX(),
                    tempVec3.getFY(),
                    0,
                    curInd,
                    true,
                    false
                );

            }

        }
    }



    std::cout<<"end link close cities\n";



    floatAndIndex *oceanRes=new floatAndIndex[numProvinces * numProvinces];

    for(i=0; i<numProvinces * numProvinces; i++)
    {
        oceanRes[i].value=FLT_MAX;
        oceanRes[i].index1=0;
        oceanRes[i].index2=0;
    }

    std::cout<<"start find biggest ocean gaps\n";


    for(k=0; k<2; k++)
    {

        std::cout<<"iteration: "<<k<<"\n";

        count=0;

        if(k==0)
        {
            for(i=1; i<numProvinces-1; i++)
            {
                for(j=i+1; j<numProvinces; j++)
                {
                    curInd=i+j*numProvinces;
                    if(provinceGrid[curInd]!=1)
                    {
                        p1=i;
                        p2=j;



                        tempVec1.setIXYZ(provinceX[p1], provinceY[p1], 0);
                        tempVec2.setIXYZ(provinceX[p2], provinceY[p2], 0);

                        tempVec2.wrapDistance(&tempVec1, w);
                        tempVec3.copyFrom(&tempVec1);


                        oceanRes[count].value=findBestPath(
                            tempVec2.getFX(),
                            tempVec2.getFY(),
                            tempVec3.getFX(),
                            tempVec3.getFY(),
                            0,
                            curInd,
                            false,
                            true
                        );
                        oceanRes[count].index1=i;
                        oceanRes[count].index2=j;

                        count++;

                    }

                }
            }
        }
        else
        {
            bubbleSortF(oceanRes, numProvinces * numProvinces);


            for(i=0; i<30; i++)
            {

                p1=oceanRes[i].index1;
                p2=oceanRes[i].index2;

                curInd=p1+p2*numProvinces;

                tempVec1.setIXYZ(provinceX[p1], provinceY[p1], 0);
                tempVec2.setIXYZ(provinceX[p2], provinceY[p2], 0);

                tempVec2.wrapDistance(&tempVec1, w);
                tempVec3.copyFrom(&tempVec1);

                findBestPath(
                    tempVec2.getFX(),
                    tempVec2.getFY(),
                    tempVec3.getFX(),
                    tempVec3.getFY(),
                    0,
                    curInd,
                    true,
                    true
                );
            }

        }


    }



    std::cout<<"end find biggest ocean gaps\n";








    mapSwapFlag=0;
    mapStep=0.0f;

    fbow2->cpuToGPU();
    Renderer::copyFBO("cityFBO", "swapFBO0");
    Renderer::bindShader("DilateShader");
    for(i=0; i<2; i++)
    {

        Renderer::bindFBO("swapFBO", mapSwapFlag);
        Renderer::sampleFBO("swapFBO", 0, mapSwapFlag);
        Renderer::sampleFBO("hmFBO", 1);
        Renderer::setShaderFloat("seaLevel", getSLNormalized());
        Renderer::setShaderFloat("mapStep", 1.0f);
        Renderer::setShaderFloat("doDilate", 1.0f);
        Renderer::setShaderFloat("texPitch", (float)w);
        Renderer::drawFSQuad();
        Renderer::unsampleFBO("hmFBO", 1);
        Renderer::unsampleFBO("swapFBO", 0, mapSwapFlag);
        Renderer::unbindFBO();

        mapSwapFlag=1-mapSwapFlag;
        mapStep+=1.0f;
    }
    Renderer::unbindShader();
    Renderer::copyFBO("swapFBO0", "cityFBO");
    fbow2->getPixels();
    //fbow2->updateMips();






    //bool notCovered = true;
    int id=1;
    int totCount;
    int fillColor;
    bool incId;

    std::cout<<"start road regions\n";

    for(i=0; i<totSize; i++)
    {

        if(fbow2->getPixelAtIndex(i, pathChannel)==0)
        {

            for(j=0; j<2; j++)
            {
                btStack[0]=i;
                btStackInd=0;
                totCount=0;

                if(j==0)
                {
                    fillColor=id;
                }
                else
                {
                    fillColor=255;
                }


                while(btStackInd>-1)
                {

                    curInd=btStack[btStackInd];
                    curY=curInd/w;
                    curX=curInd-curY*w;

                    if(j==0)
                    {
                        fbow2->orPixelAtIndex(curInd, btChannel, visFlag);
                    }
                    else
                    {
                        fbow2->andPixelAtIndex(curInd, btChannel, visFlagO);
                    }


                    fbow2->setPixelAtIndex(curInd, pathChannel, fillColor);


                    count=0;
                    notFound=true;



                    do
                    {
                        curDir=count;

                        testX=curX+dirModX[curDir];
                        testY=curY+dirModY[curDir];
                        testInd=fbow2->getIndex(testX, testY);
                        testPix=fbow2->getPixelAtIndex(testInd, btChannel);
                        testPix2=fbow2->getPixelAtIndex(testInd, pathChannel);


                        if(j==0)
                        {
                            if(((testPix & visFlag)==0)&&(testPix2==0))
                            {
                                notFound=false;
                                totCount++;

                            }
                        }
                        else
                        {
                            if(((testPix & visFlag)>0)&&(testPix2==id))
                            {
                                notFound=false;
                                totCount++;
                            }
                        }




                        count++;
                    } while(notFound && count<4);

                    if(notFound)
                    {
                        btStackInd--;
                    }
                    else
                    {

                        // join the two and remove walls
                        //fbow2->andPixelAtIndex(curInd, btChannel, dirFlags[bestDir]);
                        //fbow2->andPixelAtIndex(bestInd, btChannel, dirFlagsOp[bestDir]);


                        btStackInd++;
                        btStack[btStackInd]=testInd;
                    }

                }

                incId=false;

                if(j==0)
                {
                    if(totCount<1000)
                    {
                        //std::cout << "Too Small\n";
                    }
                    else
                    {
                        incId=true;
                        j++;
                    }
                }
                else
                {
                    incId=true;
                }

                if(incId)
                {
                    //std::cout << "Id: " << id << "\n";
                    id++;
                    if(id>254)
                    {
                        id=1;
                    }
                }
            }




        }

    }


    std::cout<<"end road regions\n";

    // clear visited
    for(k=0; k<totSize; k++)
    {
        fbow2->andPixelAtIndex(k, btChannel, visFlagO);
    }

    fbow2->cpuToGPU();


    mapSwapFlag=0;
    mapStep=0.0f;
    Renderer::copyFBO("cityFBO", "swapFBO0");



    Renderer::bindShader("SkeletonShader");
    for(k=0; k<19; k++)
    {

        Renderer::bindFBO("swapFBO", mapSwapFlag);
        Renderer::sampleFBO("swapFBO", 0, mapSwapFlag);
        //Renderer::sampleFBO("hmFBO",1);
        Renderer::setShaderFloat("seaLevel", getSLNormalized());
        Renderer::setShaderFloat("mapStep", 0.0f);
        Renderer::setShaderFloat("texPitch", (float)w);
        Renderer::drawFSQuad();
        //Renderer::unsampleFBO("hmFBO",1);
        Renderer::unsampleFBO("swapFBO", 0, mapSwapFlag);
        Renderer::unbindFBO();
        mapSwapFlag=1-mapSwapFlag;
        mapStep+=1.0f;

    }
    Renderer::unbindShader();


    Renderer::bindShader("RoadShader");
    for(k=0; k<1; k++)
    {

        Renderer::bindFBO("swapFBO", mapSwapFlag);
        Renderer::sampleFBO("swapFBO", 0, mapSwapFlag);
        //Renderer::sampleFBO("hmFBO",1);
        Renderer::setShaderFloat("seaLevel", getSLNormalized());
        Renderer::setShaderFloat("mapStep", 0.0f);
        Renderer::setShaderFloat("texPitch", (float)w);
        Renderer::drawFSQuad();
        //Renderer::unsampleFBO("hmFBO",1);
        Renderer::unsampleFBO("swapFBO", 0, mapSwapFlag);
        Renderer::unbindFBO();
        mapSwapFlag=1-mapSwapFlag;
        mapStep+=1.0f;

    }
    Renderer::unbindShader();


    Renderer::copyFBO("swapFBO0", "cityFBO");
    fbow2->getPixels();







    // generate streets

    int *streetFlagsV=new int[w]; //runs vertical
    int *streetFlagsH=new int[h]; //runs horizontal

    for(i=0; i<w; i++)
    {
        streetFlagsV[i]=0;
    }
    for(i=0; i<h; i++)
    {
        streetFlagsH[i]=0;
    }

    // 1 - x+
    // 2 - x-
    // 4 - y+
    // 8 - y-

    for(i=0; i<w; i+=g_settings.holdersPerBlock)
    {

        curInd=i-1;

        while(curInd<0)
        {
            curInd+=w;
        }

        streetFlagsV[curInd]|=1;
        streetFlagsV[i]|=2;
    }
    for(i=0; i<h; i+=g_settings.holdersPerBlock)
    {

        curInd=i-1;

        while(curInd<0)
        {
            curInd+=h;
        }

        streetFlagsH[curInd]|=4;
        streetFlagsH[i]|=8;
    }












    //add in main streets
    for(k=0; k<totSize; k++)
    {
        curInd=k;
        curY=curInd/w;
        curX=curInd-curY*w;

        testPix=fbow->getPixelAtIndex(curInd, blockChannel);

        if(testPix!=0)
        {
            fbow2->orPixelAtIndex(curInd, btChannel, streetFlagsH[curY]|streetFlagsV[curX]);
            fbow2->orPixelAtIndex(curInd, stChannel, streetFlagsH[curY]|streetFlagsV[curX]); //stChannel
        }


    }


    // for (k = 0; k < totSize; k++) {

    //  // int stChannel;
    //  // int btChannel;
    //  // int pathChannel;
    //  // int houseChannel;

    //  testPix = fbow2->getPixelAtIndex(k, stChannel);
    //  fbow2->orPixelAtIndex(k, btChannel, testPix );

    //  testPix = fbow2->getPixelAtIndex(k, pathChannel);
    //  fbow2->orPixelAtIndex(k, btChannel, testPix );
    // }


    /*
    for (i = 0; i < numProvinces-1; i++) {
        for (j = i + 1; j < numProvinces; j++) {
            if (provinceGrid[i + j*numProvinces] == 1) {

                k = fbow->numMips-1;

                cx1 = provinceX[i];
                cy1 = provinceY[i];
                cx2 = provinceX[j];
                cy2 = provinceY[j];

                while (getMipInd(cx1,cy1,k) == getMipInd(cx2,cy2,k)) {
                    k--;
                }

            }
        }
    }



    // remove any road that touches water or is out of town

    int cityLevel = 0;
    for (k = 0; k < totSize; k++) {
        curInd = k;
        curY = curInd/w;
        curX = curInd-curY*w;

        touchesWater = false;

        for (j = -1; j <= 1; j++) {
            for (i = -1; i <= 1; i++) {
                testX = curX + i;
                testY = curY + j;
                testInd = fbow->getIndex(testX,testY);
                testPix = fbow->getPixelAtIndex(testInd, hmChannel);


                if (testPix < seaLevel + 10) {
                    touchesWater = true;
                    break;
                }

            }
            if (touchesWater) {
                break;
            }
        }

        testPix2 = fbow->getPixelAtIndex(curInd, densityChannel);


        if (touchesWater) { // || (testPix2 > 120)
            fbow2->andPixelAtIndex(curInd, btChannel, dirFlagClear);
            fbow2->andPixelAtIndex(curInd, stChannel, dirFlagClear);

            //TODO: repair all broken (half) edges
        }
        // if (testPix2 > 80) {
        //  fbow2->andPixelAtIndex(curInd, stChannel, dirFlagClear);
        // }


    }



    // fix half streets

    for (k = 0; k < totSize; k++) {
        curInd = k;
        curY = curInd/w;
        curX = curInd-curY*w;

        basePix = fbow2->getPixelAtIndex(curInd, btChannel);
        basePix2 = fbow2->getPixelAtIndex(curInd, stChannel);

        for (i = 0; i < 4; i++) {



            curDir = i;

            testX = curX + dirModX[curDir];
            testY = curY + dirModY[curDir];
            testInd = fbow2->getIndex(testX,testY);
            testPix = fbow2->getPixelAtIndex(testInd, btChannel);
            testPix2 = fbow2->getPixelAtIndex(testInd, stChannel);


            if ( (basePix & dirFlagsO[curDir]) != (testPix & dirFlagsOpO[curDir]) ) {
                fbow2->orPixelAtIndex(curInd, btChannel, dirFlagsO[curDir]);
                fbow2->orPixelAtIndex(testInd, btChannel, dirFlagsOpO[curDir]);
            }

            if ( (basePix2 & dirFlagsO[curDir]) != (testPix2 & dirFlagsOpO[curDir]) ) {
                fbow2->orPixelAtIndex(curInd, stChannel, dirFlagsO[curDir]);
                fbow2->orPixelAtIndex(testInd, stChannel, dirFlagsOpO[curDir]);
            }

        }

    }

    */


















    // ?????




    std::cout<<"start filling houses\n";

    // clear visited
    for(k=0; k<totSize; k++)
    {
        fbow2->andPixelAtIndex(k, btChannel, visFlagO);
    }

    fbow->updateMips();




    int wb=blocksPerWorld;
    int hb=blocksPerWorld;
    int blockInd;

    for(i=0; i<wb; i++)
    {
        for(j=0; j<hb; j++)
        {
            blockInd=i+j*wb;

            testPix=fbow->getMipAtIndex(blockInd, blockMip, blockChannel, MIN_MIP);

            if(testPix!=0)
            {



                id=1;

                for(k=i*blockMod; k<(i+1)*blockMod; k++)
                {
                    for(m=j*blockMod; m<(j+1)*blockMod; m++)
                    {
                        curInd=k+m*w;
                        curX=k;
                        curY=m;


                        testPix=fbow2->getPixelAtIndex(curInd, btChannel);

                        if(testPix & visFlag)
                        {

                        }
                        else
                        {
                            btStack[0]=curInd;
                            btStackInd=0;
                            totCount=0;


                            while((btStackInd>-1)&&(totCount<3))
                            {

                                curInd=btStack[btStackInd];
                                curY=curInd/w;
                                curX=curInd-curY*w;

                                fbow2->orPixelAtIndex(curInd, btChannel, visFlag);
                                fbow2->setPixelAtIndex(curInd, houseChannel, id);


                                count=0;
                                notFound=true;

                                testPix2=fbow2->getPixelAtIndex(curInd, btChannel);
                                //testPix3 = fbow2->getPixelAtIndex(curInd, stChannel);
                                testPix4=fbow2->getPixelAtIndex(curInd, pathChannel);

                                do
                                {
                                    curDir=count;

                                    testX=curX+dirModX[curDir];
                                    testY=curY+dirModY[curDir];

                                    testInd=fbow2->getIndex(testX, testY);

                                    testPix=fbow2->getPixelAtIndex(testInd, btChannel);

                                    if(
                                        ((testPix & visFlag)==0)&&
                                        ((testPix2 & dirFlagsO[curDir])==0)&&
                                        //( (testPix3 & dirFlagsO[curDir]) == 0) &&
                                        ((testPix4 & dirFlagsO[curDir])==0)
                                        )
                                    {
                                        notFound=false;
                                        totCount++;

                                    }


                                    count++;
                                } while(notFound && count<4);

                                if(notFound)
                                {
                                    btStackInd--;
                                }
                                else
                                {

                                    // join the two and remove walls
                                    //fbow2->andPixelAtIndex(curInd, btChannel, dirFlags[bestDir]);
                                    //fbow2->andPixelAtIndex(bestInd, btChannel, dirFlagsOp[bestDir]);


                                    btStackInd++;
                                    btStack[btStackInd]=testInd;
                                }

                            }

                            id++;
                            if(id>254)
                            {
                                id=1;
                            }




                        }


                    }
                }

            }

        }
    }


    std::cout<<"end filling houses\n";

    // clear visited
    for(k=0; k<totSize; k++)
    {
        fbow2->andPixelAtIndex(k, btChannel, visFlagO);
    }



    // ?????










    fbow2->cpuToGPU();

    delete[] btStack;
    delete[] streetFlagsH;
    delete[] streetFlagsV;
    delete[] oceanRes;

    //////////

DONE_WITH_MAP:

    mapInvalid=false;



    std::cout<<"DONE WITH MAP\n";

    mapLockOn=false;

}


void GameWorld::drawMap()
{

//    if(singleton->menuList[E_FM_MAINMENU]==NULL)
//    {
//        return;
//    }
//    if(singleton->menuList[E_FM_MAINMENU]->visible)
//    {
//
//    }
//    else
//    {
//        return;
//    }
    if(GameState::ui->mapComp==NULL)
    {
        return;
    }
    if(GameState::ui->mapComp->visible)
    {

    }
    else
    {
        return;
    }


    FBOWrapper *fbow=FBOManager::getFBOWrapper("hmFBOLinear", 0);




    Renderer::bindShader("TopoShader");

    //Renderer::bindFBO("resultFBO0");

    //Renderer::sampleFBO("palFBO", 0);
    Renderer::setShaderTexture3D(0, volumeTex.id);
    Renderer::sampleFBO("hmFBO", 1); //Linear
    Renderer::sampleFBO("cityFBO", 2);
    Renderer::sampleFBO("hmFBOLinear", 3);
    Renderer::setShaderTexture(4, imageVoro->tid);
    //Renderer::sampleFBO("frontFaceMapFBO",4);


    Renderer::setShaderFloat("timeOfDay", timeOfDay);
    Renderer::setShaderfVec4("mapFreqs", &(mapFreqs));
    Renderer::setShaderfVec4("mapAmps", &(mapAmps));
    Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
    Renderer::setShaderfVec3("lookAtVec", &(Renderer::lookAtVec));

    Renderer::setShaderFloat("cameraZoom", Renderer::cameraZoom);
    //Renderer::setShaderFloat("mapTrans", mapTrans);
    Renderer::setShaderFloat("seaLevel", getSLNormalized());
    Renderer::setShaderFloat("curTime", (float)GameState::curTime);
    Renderer::setShaderVec2("mapDimInPixels", (float)fbow->width, (float)fbow->height);

    Renderer::setShaderFloat("mapPitch", (float)mapPitch);
    Renderer::setShaderFloat("heightMapMaxInCells", g_settings.heightMapMaxInCells);
    Renderer::setShaderFloat("cellsPerWorld", (float)cellsPerWorld);


    GameState::ui->renderQuadDirect(GameState::ui->mapComp);


    //singleton->drawQuadBounds

    //Renderer::drawFSQuadOffset(0.75f,0.75f,0.5f,1.0f/singleton->aspectRatio);
    //Renderer::drawFSQuad();


    Renderer::setShaderTexture(4, 0);
    //Renderer::unsampleFBO("frontFaceMapFBO",4);
    Renderer::unsampleFBO("hmFBOLinear", 3);
    Renderer::unsampleFBO("cityFBO", 2);
    Renderer::unsampleFBO("hmFBO", 1);
    //Renderer::unsampleFBO("palFBO", 0);
    Renderer::setShaderTexture3D(0, 0);

    //Renderer::unbindFBO();

    Renderer::unbindShader();



    //glEnable(GL_BLEND);
    //Renderer::drawFBO("resultFBO0",0,1.0f);
    //glDisable(GL_BLEND);


}

void GameWorld::doBlur(std::string fboName, int _baseFBO)
{
    int i;
    int baseFBO=_baseFBO;

    Renderer::bindShader("BlurShader");
    Renderer::setShaderFloat("numBlurPixelsPerSide", 4.0); // value of 4 is a 9x9 kernel (4*2+1)
    Renderer::setShaderFloat("sigma", 4.0);

    for(i=0; i<2; i++)
    {

        Renderer::bindFBO(fboName, baseFBO);
        Renderer::sampleFBO(fboName, 0, baseFBO);

        if(i==0)
        { // horizontal
            Renderer::setShaderFloat("blurSize", 1.0f/(Renderer::currentFBOResolutionX));
            Renderer::setShaderVec2("blurMultiplyVec", 1.0f, 0.0f);
        }
        else
        { // vertical
            Renderer::setShaderFloat("blurSize", 1.0f/(Renderer::currentFBOResolutionY));
            Renderer::setShaderVec2("blurMultiplyVec", 0.0f, 1.0f);
        }


        Renderer::drawFSQuad();

        Renderer::unsampleFBO(fboName, 0, baseFBO);
        Renderer::unbindFBO();


        baseFBO=1-baseFBO;

    }


    Renderer::unbindShader();

}

float GameWorld::getHeightAtPixelPos(float x, float y, bool dd)
{
//    FBOWrapper *fbow;
    float xc;
    float yc;

    int channel=0;

    float testHeight;

    float v0=0.0f;
    float v1=0.0f;
    float v2=0.0f;
    float v3=0.0f;


    if(mapInvalid)
    {

        std::cout<<"mapInvalid\n";
        return 0.0f;

    }
    else
    {
        FBOWrapper *fbow=FBOManager::getFBOWrapper("hmFBO", 0);

        xc=(x/((float)cellsPerWorld)) * ((float)fbow->width);
        yc=(y/((float)cellsPerWorld)) * ((float)fbow->height);

        v0=fbow->getPixelAtLinear((xc * mapFreqs.getFX()), (yc * mapFreqs.getFX()), channel);
        v1=fbow->getPixelAtLinear((xc * mapFreqs.getFY()), (yc * mapFreqs.getFY()), channel);
        v2=fbow->getPixelAtLinear((xc * mapFreqs.getFZ()), (yc * mapFreqs.getFZ()), channel);
        v3=fbow->getPixelAtLinear((xc * mapFreqs.getFW()), (yc * mapFreqs.getFW()), channel);





        if(dd)
        {
            //std::cout << "hmvals: " << v0 << ", " << v1 << ", " << v2 << ", " << v3 << "\n";
        }


        testHeight=
            v0*mapAmps.getFX()
            +v1*mapAmps.getFY()
            +v2*mapAmps.getFZ()
            +v3*mapAmps.getFW()

            // - v1 * mapAmps.getFY()*0.5f
            // - v2 * mapAmps.getFZ()*0.5f
            // - v3 * mapAmps.getFW()*0.5f

            ;

        if(dd)
        {
            //std::cout << "testHeight " << testHeight << "\n";
        }

        testHeight=clampf(testHeight, 0.0f, 1.0f);

        return getTerHeightScaled(testHeight);



    }



}

// 	void updateLights()
// 	{


// 		int i;
// 		int j;
// 		int k;
// 		bool flag = true;
// 		GameEnt *tempLight;
// 		GameEnt *curLight;


// 		lightCount = singleton->numDynLights;

// 		if (singleton->targetTimeOfDay <= 0.5)
// 		{


// 			findNearestEnt(&(singleton->nearestLights),E_ET_LIGHT,4,2,Renderer::cameraGetPosNoShake(),false,true);


// 			for (i = 0; i < singleton->nearestLights.selEntList.size(); i++) {


// 				curLight = singleton->nearestLights.selEntList[i];//&(curBlock->gameEnts[E_ET_LIGHT].data[k]);
// 				curLight->camDistance = Renderer::cameraGetPosNoShake()->distance(&(curLight->geomParams[E_LP_POSITION]));

// 				if (curLight->toggled) {
// 					activeLights[lightCount] = singleton->nearestLights.selEntList[i];//&(curBlock->gameEnts[E_ET_LIGHT].data[k]);
// 					lightCount++;
// 				}

// 				if (lightCount >= MAX_EVAL_LIGHTS)
// 				{
// 					goto UPDATE_LIGHTS_END;
// 				}


// 			}



// UPDATE_LIGHTS_END:

// 			for (i = singleton->numDynLights + 1; (i <= lightCount) && flag; i++)
// 			{
// 				flag = false;
// 				for (j = singleton->numDynLights; j < (lightCount - 1); j++)
// 				{
// 					if (activeLights[j + 1]->camDistance < activeLights[j]->camDistance) // ascending order simply changes to <
// 					{
// 						tempLight = activeLights[j];
// 						activeLights[j] = activeLights[j + 1];
// 						activeLights[j + 1] = tempLight;
// 						flag = true;
// 					}
// 				}
// 			}

// 			lightCount = min(lightCount, MAX_LIGHTS);
// 		}



// 	}

// void renderWaveHeight() {
// 	Renderer::bindShader("WaveHeightShader");
// 	Renderer::bindFBO("waveFBO");

// 	Renderer::sampleFBO("solidTargFBO",0);
// 	Renderer::sampleFBO("waterTargFBO",2);

// 	Renderer::setShaderVec2("resolution", Renderer::currentFBOResolutionX, Renderer::currentFBOResolutionY);
// 	Renderer::setShaderFloat("curTime", GameState::curTime);
// 	Renderer::drawFSQuad();

// 	Renderer::unsampleFBO("waterTargFBO",2);
// 	Renderer::unsampleFBO("solidTargFBO",0);

// 	Renderer::unbindFBO();
// 	Renderer::unbindShader();
// }



void GameWorld::drawBasicPrims(bool doShadow)
{




    // int numCubes = singleton->tempPrimList.size();
    // if (g_settings.settings[E_BS_PLACING_GEOM]) {
    // 	numCubes++;
    // }

    // numCubes += actorCount;

    int numCubes=0;
    int i;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);


    numCubes=primArrPos;

    Renderer::getLSMatrix(
        &(lightPosDynamic),
        lightSpaceMatrixLow,
        getConst(E_CONST_LIGHTORTHOSIZE_LOW)
    );

    Renderer::bindShader("BasicPrimShader");
    for(i=0; i<2; i++)
    {
        if(i==0)
        {
            Renderer::bindFBO("rasterLowFBO");
        }
        else
        {
            Renderer::bindFBO("shadowLowFBO");
        }
        if(numCubes>0)
        {

            Renderer::setShaderTBO(
                0,
                Renderer::primTBO.tbo_tex,
                Renderer::primTBO.tbo_buf,
                true
            );

            //std::cout << "actorCount " << actorCount << "\n";

            // Renderer::setShaderInt("actorCount",actorCount);
            //Renderer::setShaderInt("MAX_PRIM_IDS", min(actorCount,MAX_PRIM_IDS));
            //Renderer::setShaderInt("MAX_PRIMTEST", min(actorCount,MAX_PRIMTEST));

            //Renderer::setShaderFloat("heightOfNearPlane",singleton->heightOfNearPlane);
            //Renderer::setShaderFloat("FOV", Renderer::FOV*M_PI/180.0f);
            Renderer::setShaderVec2("clipDist", Renderer::clipDist[0], Renderer::clipDist[1]);
            Renderer::setShaderVec2("bufferDim", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY);
            Renderer::setShaderfVec3("lightPos", &(lightPosDynamic));

            if(i==0)
            {
                Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
                Renderer::setShaderMatrix4x4("pmMatrix", Renderer::pmMatrix.get(), 1);
            }
            else
            {
                Renderer::setShaderfVec3("cameraPos", &(lightPosDynamic));
                Renderer::setShaderMatrix4x4("pmMatrix", lightSpaceMatrixLow.get(), 1);
            }


            Renderer::setShaderArrayfVec4("primArr", primArr, numCubes*2);
            zoCubes.drawCubes(numCubes);

            Renderer::setShaderTBO(0, 0, 0, true);

        }
        Renderer::unbindFBO();
    }
    Renderer::unbindShader();


    //Renderer::copyFBO("rasterLowFBO","solidBaseTargFBO");


    numCubes=limbArrPos;

    Renderer::bindShader("BasicLimbShader");
    for(i=0; i<2; i++)
    {
        if(numCubes>0)
        {
            if(i==0)
            {
                Renderer::bindFBO("rasterLowFBO", -1, 0);
            }
            else
            {
                Renderer::bindFBO("shadowLowFBO", -1, 0);
            }

            Renderer::setShaderTBO(
                0,
                Renderer::limbTBO.tbo_tex,
                Renderer::limbTBO.tbo_buf,
                true
            );


            // std::cout << "actorCount " << actorCount << "\n";

            //Renderer::setShaderInt("actorCount",actorCount);
            Renderer::setShaderInt("MAX_PRIM_IDS", std::min(actorCount, MAX_PRIM_IDS));
            Renderer::setShaderInt("MAX_PRIMTEST", std::min(actorCount, MAX_PRIMTEST));

            //Renderer::setShaderFloat("heightOfNearPlane",singleton->heightOfNearPlane);
            //Renderer::setShaderFloat("FOV", Renderer::FOV*M_PI/180.0f);
            Renderer::setShaderfVec3("lightPos", &(lightPosDynamic));
            Renderer::setShaderVec2("clipDist", Renderer::clipDist[0], Renderer::clipDist[1]);
            Renderer::setShaderVec2("bufferDim", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY);
            if(i==0)
            {
                Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
                Renderer::setShaderMatrix4x4("pmMatrix", Renderer::pmMatrix.get(), 1);
            }
            else
            {
                Renderer::setShaderfVec3("cameraPos", &(lightPosDynamic));
                Renderer::setShaderMatrix4x4("pmMatrix", lightSpaceMatrixLow.get(), 1);
            }

            Renderer::setShaderArrayfVec4("limbArr", limbArr, numCubes*2);
            zoCubes.drawCubes(numCubes);

            Renderer::setShaderTBO(0, 0, 0, true);

            Renderer::unbindFBO();

        }
    }
    Renderer::unbindShader();

    glCullFace(GL_BACK);
    glDisable(GL_CULL_FACE);
}


void GameWorld::rasterHolders(bool doShadow)
{

    // get view matrix
    // Renderer::perspectiveOn = true;
    // singleton->getMatrixFromFBO("rasterFBO0");
    // Renderer::perspectiveOn = false;

    int activeRaster=0;

    int q;




    glEnable(GL_DEPTH_TEST);

    if(!DO_POINTS)
    {
        glEnable(GL_CULL_FACE);
    }


    if(doShadow)
    {

        // glDepthRangef(
        // 	Renderer::clipDist[0],
        // 	Renderer::clipDist[1]+getConst(E_CONST_LIGHTDIS)
        // );

        //singleton->updateLightPos();
        Renderer::getLSMatrix(
            &(lightPosStatic),
            lightSpaceMatrix,
            getConst(E_CONST_LIGHTORTHOSIZE)
        );


        Renderer::bindShader("ShadowMapShader");
        Renderer::bindFBO("shadowMapFBO");

        Renderer::setShaderVec2("clipDist", Renderer::clipDist[0], Renderer::clipDist[1]);
        Renderer::setShaderVec2("bufferDim", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY);
        Renderer::setShaderfVec3("lightPos", &(lightPosStatic));
        //Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
        Renderer::setShaderMatrix4x4("lightSpaceMatrix", lightSpaceMatrix.get(), 1);


        // Renderer::setShaderfVec3("minBounds",&(minShadowBounds));
        // Renderer::setShaderfVec3("maxBounds",&(maxShadowBounds));
        // Renderer::setShaderfVec3("lightVec",&(Renderer::lightVec));

        rastChunk(iGetConst(E_CONST_RASTER_CHUNK_RAD), 0);

        Renderer::unbindFBO();
        Renderer::unbindShader();

        // glDepthRangef(
        // 	Renderer::clipDist[0],
        // 	Renderer::clipDist[1]	
        // );
    }
    else
    {
        Renderer::bindShader("HolderShader");
        Renderer::bindFBO("rasterFBO", activeRaster);

        Renderer::setShaderFloat("heightOfNearPlane", Renderer::heightOfNearPlane);
        Renderer::setShaderFloat("FOV", Renderer::FOV*(float)M_PI/180.0f);
        Renderer::setShaderVec2("clipDist", Renderer::clipDist[0], Renderer::clipDist[1]);
        Renderer::setShaderVec2("bufferDim", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY);
        //Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());

        //Renderer::setShaderMatrix4x4("lightSpaceMatrix",lightSpaceMatrix.get(),1);
        Renderer::setShaderMatrix4x4("pmMatrix", Renderer::pmMatrix.get(), 1);

        rastChunk(iGetConst(E_CONST_RASTER_CHUNK_RAD), RH_FLAG_CLIPTOVIEW);

        Renderer::unbindFBO();
        Renderer::unbindShader();

        activeRaster=1-activeRaster;


        drawBasicPrims(false);


    }



    if(!DO_POINTS)
    {
        glDisable(GL_CULL_FACE);
    }

    glDisable(GL_DEPTH_TEST);


    if(doShadow)
    {
        return;
    }




    if(
        (iGetConst(E_CONST_GROWPOINTSTEPS)>0)&&
        (DO_POINTS)
        )
    {

        Renderer::bindShader("PointShader");

        Renderer::setShaderFloat("voxelsPerCell", (float)g_settings.voxelsPerCell);
        Renderer::setShaderFloat("FOV", Renderer::FOV*(float)M_PI/180.0f);
        Renderer::setShaderVec2("clipDist", Renderer::clipDist[0], Renderer::clipDist[1]);
        Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
        Renderer::setShaderInt("totRad", iGetConst(E_CONST_HVRAD));
        Renderer::setShaderMatrix4x4("modelviewInverse", Renderer::viewMatrixDI, 1);

        for(q=0; q<iGetConst(E_CONST_GROWPOINTSTEPS); q++)
        {


            if((q%2)==0)
            {
                Renderer::setShaderVec2("hvMult", 1.0f, 0.0f);
            }
            else
            {
                Renderer::setShaderVec2("hvMult", 0.0f, 1.0f);
            }

            Renderer::bindFBO("rasterFBO", activeRaster);
            Renderer::sampleFBO("rasterFBO", 0, activeRaster);

            Renderer::setShaderInt("lastPass", (int)(q==(iGetConst(E_CONST_GROWPOINTSTEPS)-1)));
            Renderer::setShaderVec2("bufferDim", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY);

            Renderer::fsQuad.draw();

            Renderer::unsampleFBO("rasterFBO", 0, activeRaster);
            Renderer::unbindFBO();

            activeRaster=1-activeRaster;
        }

        Renderer::unbindShader();


        Renderer::bindShader("NearestShader");
        Renderer::bindFBO("rasterFBO", activeRaster);
        Renderer::sampleFBO("rasterFBO", 0, activeRaster);
        Renderer::sampleFBO("rasterLowFBO", 3);
        Renderer::setShaderVec2("bufferDim", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY);
        Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
        Renderer::setShaderInt("totRad", iGetConst(E_CONST_FILLNEARESTRAD));
        Renderer::fsQuad.draw();
        Renderer::unsampleFBO("rasterLowFBO", 3);
        Renderer::unsampleFBO("rasterFBO", 0, activeRaster);
        Renderer::unbindFBO();
        Renderer::unbindShader();
        activeRaster=1-activeRaster;

    }


    Renderer::copyFBO3("rasterFBO"+i__s(activeRaster), "readFBO");


    Renderer::bindShader("LightShader");
    Renderer::bindFBO("resultFBO", activeFBO);
    Renderer::sampleFBO("rasterFBO", 0, activeRaster);
    Renderer::sampleFBO("debugTargFBO", 3);
    Renderer::setShaderTexture3D(5, volumeTex.id);
    Renderer::sampleFBO("shadowMapFBO", 6);
    Renderer::sampleFBO("shadowLowFBO", 7);


    //if(singleton->mouseState==E_MOUSE_STATE_BRUSH)
    if(drawBrush)
    {
        lastUnitPos.setFW(((float)curBrushRad));
        Renderer::setShaderfVec4("brushPos", &(lastUnitPos));

        switch(earthMod)
        {
        case E_PTT_TER:
            Renderer::setShaderVec3("brushCol", 1.0f, 0.0f, 0.0f);
            break;
        case E_PTT_WAT:
            Renderer::setShaderVec3("brushCol", 0.0f, 0.0f, 1.0f);
            break;
        case E_PTT_BLD:
            Renderer::setShaderVec3("brushCol", 0.0f, 1.0f, 0.0f);
            break;
        }

    }
    else
    {
        Renderer::setShaderVec4("brushPos", 0.0f, 0.0f, 0.0f, 0.0f);
    }

    /*
    if (singleton->mouseState == E_MOUSE_STATE_BRUSH) {
        if (earthMod == E_PTT_TER) {
            Renderer::setShaderVec3("matVal", 255, 0, 0);
        }
        else {
            Renderer::setShaderVec3("matVal", 0, 0, 255);
        }


        Renderer::setShaderFloat("isWire", 1.0);
        Renderer::drawCubeCentered(
            &lastUnitPos,
            ((int)curBrushRad)
        );

    }

    */




    Renderer::setShaderInt("iNumSteps", g_settings.iNumSteps);
    Renderer::setShaderInt("cellsPerHolder", g_settings.cellsPerHolder);


    Renderer::setShaderInt("combatOn", g_settings.settings[E_BS_COMBAT]);
    Renderer::setShaderInt("editPose", g_settings.settings[E_BS_EDIT_POSE]);
    Renderer::setShaderInt("gridOn", g_settings.settings[E_BS_SHOW_GRID]);
    Renderer::setShaderInt("mouseDown", GameState::ui->abDown);
    Renderer::setShaderInt("testOn3", (int)(g_settings.settings[E_BS_TEST_3]));

    Renderer::setShaderFloat("activeActorInd", (float)GameState::gem->actObjInd);
    Renderer::setShaderfVec4("readDataMU", &(GameState::ui->mouseUpPixData.pd[2]));
    Renderer::setShaderfVec4("readDataMD", &(GameState::ui->mouseDownPixData.pd[2]));

    Renderer::setShaderFloat("curTime", (float)GameState::curTime);
    Renderer::setShaderfVec2("mouseCoords", &(lastMouseZO));
    Renderer::setShaderFloat("gammaVal", g_settings.gammaVal);
    Renderer::setShaderFloat("cellsPerChunk", (float)g_settings.cellsPerChunk);
    Renderer::setShaderfVec3("lightPosStatic", &(lightPosStatic));
    Renderer::setShaderfVec3("lightPosDynamic", &(lightPosDynamic));
    // Renderer::setShaderfVec3("minBounds",&(minShadowBounds));
    // Renderer::setShaderfVec3("maxBounds",&(maxShadowBounds));
    Renderer::setShaderfVec3("lookAtVec", &(Renderer::lookAtVec));
    Renderer::setShaderFloat("voxelsPerCell", (float)g_settings.voxelsPerCell);
    Renderer::setShaderFloat("FOV", Renderer::FOV*(float)M_PI/180.0f);
    Renderer::setShaderVec2("clipDist", Renderer::clipDist[0], Renderer::clipDist[1]);
    Renderer::setShaderVec2("shadowBias",
        getConst(E_CONST_SHADOWBIASMIN),
        getConst(E_CONST_SHADOWBIASMAX)
    );
    Renderer::setShaderVec2("bufferDim", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY);
    Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
    Renderer::setShaderfVec3("lightVec", &(Renderer::lightVec));
    Renderer::setShaderMatrix4x4("modelviewInverse", Renderer::viewMatrixDI, 1);
    Renderer::setShaderMatrix4x4("lightSpaceMatrix", lightSpaceMatrix.get(), 1);
    Renderer::setShaderMatrix4x4("lightSpaceMatrixLow", lightSpaceMatrixLow.get(), 1);
    Renderer::setShaderMatrix4x4("pmMatrix", Renderer::pmMatrix.get(), 1);

    Renderer::fsQuad.draw();


    Renderer::unsampleFBO("shadowLowFBO", 7);
    Renderer::unsampleFBO("shadowMapFBO", 6);
    Renderer::setShaderTexture3D(5, 0);
    Renderer::unsampleFBO("debugTargFBO", 3);
    Renderer::unsampleFBO("rasterFBO", 0, activeRaster);
    Renderer::unbindFBO();

    Renderer::unbindShader();

    activeFBO=1-activeFBO;




}

// void rasterGrid(VBOGrid* vboGrid, bool showResults) {

// 	// get view matrix
// 	Renderer::perspectiveOn = true;
// 	singleton->getMatrixFromFBO("rasterFBO0");
// 	Renderer::perspectiveOn = false;


// 	glEnable(GL_DEPTH_TEST);

// 	Renderer::bindShader("GridShader");
// 	Renderer::bindFBO("rasterFBO0");

// 	Renderer::sampleFBO("rasterPosFBO",0);
// 	Renderer::sampleFBO("rasterSourceFBO",1);

// 	Renderer::setShaderFloat("FOV", Renderer::FOV*M_PI/180.0f);
// 	Renderer::setShaderVec2("clipDist",Renderer::clipDist[0],Renderer::clipDist[1]);
// 	Renderer::setShaderfVec2("bufferDim", &(Renderer::bufferDim));
// 	Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());


// 	Renderer::setShaderMatrix4x4("modelviewInverse",Renderer::viewMatrixDI,1);
// 	Renderer::setShaderMatrix4x4("modelview",Renderer::viewMatrix.get(),1);
// 	Renderer::setShaderMatrix4x4("proj",Renderer::projMatrix.get(),1);

// 	//Renderer::fsQuad.draw();
// 	vboGrid->vboWrapper.draw();


// 	Renderer::unsampleFBO("rasterSourceFBO",1);
// 	Renderer::unsampleFBO("rasterPosFBO",0);
// 	Renderer::unbindFBO();
// 	Renderer::unbindShader();

// 	glDisable(GL_DEPTH_TEST);


// 	if (showResults) {
// 		Renderer::drawFBO("rasterFBO0", 0, 1.0f);

// 		glutSwapBuffers();


// 	}


// }

// void rasterOct(GameVoxelWrap* gameOct, bool showResults) {

// 	// get view matrix
// 	Renderer::perspectiveOn = true;
// 	singleton->getMatrixFromFBO("rasterFBO0");
// 	Renderer::perspectiveOn = false;


// 	glEnable(GL_DEPTH_TEST);
// 	//glDepthMask(GL_TRUE);

// 	Renderer::bindShader("RasterShader");
// 	Renderer::bindFBO("rasterFBO0");

// 	// Renderer::setShaderTBO(
// 	// 	0,
// 	// 	gameOct->octTBO.tbo_tex,
// 	// 	gameOct->octTBO.tbo_buf,
// 	// 	false
// 	// );

// 	Renderer::setShaderFloat("curTime", GameState::curTime);
// 	Renderer::setShaderFloat("heightOfNearPlane",singleton->heightOfNearPlane);
// 	Renderer::setShaderFloat("dimInVoxels", gameOct->dimInVoxels);
// 	Renderer::setShaderInt("renderLevel", gameOct->renderLevel);
// 	Renderer::setShaderInt("vDataSize", gameOct->vDataSize);
// 	Renderer::setShaderInt("rootPtr", gameOct->rootPtr);
// 	Renderer::setShaderInt("nodeSize", gameOct->nodeSize);
// 	Renderer::setShaderFloat("FOV", Renderer::FOV*M_PI/180.0f);
// 	Renderer::setShaderVec2("clipDist",Renderer::clipDist[0],Renderer::clipDist[1]);
// 	Renderer::setShaderfVec2("bufferDim", &(Renderer::bufferDim));
// 	Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());


// 	Renderer::setShaderMatrix4x4("modelviewInverse",Renderer::viewMatrixDI,1);
// 	Renderer::setShaderMatrix4x4("modelview",Renderer::viewMatrix.get(),1);
// 	Renderer::setShaderMatrix4x4("proj",Renderer::projMatrix.get(),1);

// 	gameOct->vboWrapper.drawPoints();

// 	//Renderer::setShaderTBO(0,0,0,false);
// 	Renderer::unbindFBO();
// 	Renderer::unbindShader();


// 	//glDepthMask(GL_FALSE);
// 	glDisable(GL_DEPTH_TEST);


// 	if (showResults) {
// 		Renderer::drawFBO("rasterFBO0", 0, 1.0f);

// 		glutSwapBuffers();
// 	}


// }

// void renderOct(GameVoxelWrap* gameOct) {

// 	// get view matrix
// 	Renderer::perspectiveOn = true;
// 	singleton->getMatrixFromFBO("resultFBO0");
// 	Renderer::perspectiveOn = false;
// 	// 


// 	Renderer::bindShader("OctShader");
// 	Renderer::bindFBO("resultFBO0");

// 	Renderer::setShaderTBO(
// 		0,
// 		gameOct->octTBO.tbo_tex,
// 		gameOct->octTBO.tbo_buf,
// 		false
// 	);

// 	Renderer::setShaderFloat("dimInVoxels", gameOct->dimInVoxels);
// 	Renderer::setShaderInt("renderLevel", gameOct->renderLevel);
// 	Renderer::setShaderInt("vDataSize", gameOct->vDataSize);
// 	Renderer::setShaderInt("rootPtr", gameOct->rootPtr);
// 	Renderer::setShaderInt("nodeSize", gameOct->nodeSize);
// 	Renderer::setShaderFloat("FOV", Renderer::FOV*M_PI/180.0f);
// 	Renderer::setShaderVec2("clipDist",Renderer::clipDist[0],Renderer::clipDist[1]);
// 	Renderer::setShaderfVec2("bufferDim", &(Renderer::bufferDim));
// 	Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
// 	Renderer::setShaderMatrix4x4("modelviewInverse",Renderer::viewMatrixDI,1);

// 	Renderer::fsQuad.draw();


// 	Renderer::setShaderTBO(0,0,0,false);
// 	Renderer::unbindFBO();
// 	Renderer::unbindShader();

// 	Renderer::drawFBO("resultFBO0", 0, 1.0f);

// 	glutSwapBuffers();

// }

void GameWorld::renderDebug()
{

    //drawScene

    BaseObj* ge=GameState::gem->getCurActor();

    int i;

    float identMat[16];

    for(i=0; i<16; i++)
    {
        identMat[i]=0.0f;
    }

    identMat[0]=1.0f;
    identMat[5]=1.0f;
    identMat[10]=1.0f;
    identMat[15]=1.0f;

    float myMat[16];
    Matrix4 myMatrix4;
    Vector4 myVector0;
    Vector4 myVector1;
    Vector4 resVector0;
    Vector4 resVector1;
    btVector3 basePos;
    float rad0=1.0f;
    float rad1=3.0f;

    btVector3 boxCenter;
    btVector3 boxRadius;
    BaseObj* grabber;


    float xrotrad=Renderer::getCamRot(0);

    Renderer::bindShader("GeomShader");
    Renderer::bindFBO("debugTargFBO");


    Renderer::setShaderVec4(
        "rotationZ", 0.0f, 0.0f, 0.0f, 0.0f
    );
    Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
    Renderer::setShaderfVec3("lookAtVec", &(Renderer::lookAtVec));
    Renderer::setShaderFloat("isWire", 0.0);
    Renderer::setShaderFloat("clipDist", Renderer::clipDist[1]);
    Renderer::setShaderMatrix4x4("modelview", Renderer::viewMatrix.get(), 1);
    Renderer::setShaderMatrix4x4("proj", Renderer::projMatrix.get(), 1);
    Renderer::setShaderFloat("objectId", 0.0);
    Renderer::setShaderVec3("matVal", 255, 0, 0);



    // if (GameState::gamePhysics != NULL) {
    // 	Renderer:drawOrient = true;
    // 	GameState::gamePhysics->example->renderScene();
    // }



    glLineWidth(1.0f);

    Renderer::setShaderFloat("objectId", 0.0);



    // skeleton outline		
    if(GameState::gem->getCurActor()!=NULL)
    {
        if(GameState::gem->getCurActor()->orgId>-1)
        {

            if(GameState::gem->getCurActor()->isGrabbedById>-1)
            {
                grabber=&(GameState::gem->gameObjects[GameState::gem->getCurActor()->isGrabbedById]);
            }
            else
            {
                grabber=GameState::gem->getCurActor();
            }


            grabber->bodies[E_BDG_CENTER].body->getWorldTransform().getOpenGLMatrix(myMat);

            Renderer::setShaderMatrix4x4("objmat", myMat, 1);


            drawOrg(GameState::gem->gameOrgs[GameState::gem->getCurActor()->orgId], false);
        }
    }

    glLineWidth(4.0f);


    Renderer::setShaderMatrix4x4("objmat", identMat, 1);

    float healthMeterScale=0.5f;

    if(g_settings.settings[E_BS_SHOW_HEALTH])
    {
        for(i=0; i<GameState::gem->visObjects.size(); i++)
        {
            ge=&(GameState::gem->gameObjects[GameState::gem->visObjects[i]]);
            if(ge->entType==E_ENTTYPE_NPC)
            {

                //ge->bodies[E_BDG_CENTER].body->getWorldTransform().getOpenGLMatrix(myMat);

                if(ge->isAlive())
                {
                    Renderer::setShaderVec3("matVal", 1, 1, 1);
                    boxCenter=ge->getCenterPoint(E_BDG_CENTER);
                    boxCenter+=btVector3(0.0f, 0.0f, 5.0f);
                    boxRadius=btVector3(1.95f, 0.2f, 0.2f)*healthMeterScale;

                    Renderer::setShaderVec4(
                        "rotationZ",
                        boxCenter.getX(),
                        boxCenter.getY(),
                        boxCenter.getZ(),
                        xrotrad
                    );

                    Renderer::drawBoxRad(boxCenter, boxRadius);

                    Renderer::setShaderVec3("matVal", 255, 0, 0);
                    boxCenter=ge->getCenterPoint(E_BDG_CENTER);
                    boxCenter+=btVector3(-2.0f*(1.0f-ge->healthPerc())*healthMeterScale, 0.0f, 5.0f);
                    boxRadius=btVector3(2.0f*ge->healthPerc(), 0.25f, 0.25f)*healthMeterScale;
                    Renderer::drawBoxRad(boxCenter, boxRadius);
                }

                if(g_settings.settings[E_BS_TURN_BASED])
                {
                    boxCenter=(ge->tbPos+btVector3(0.5f, 0.5f, 0.5f));
                    Renderer::setShaderVec4(
                        "rotationZ",
                        boxCenter.getX(),
                        boxCenter.getY(),
                        boxCenter.getZ(),
                        0.0f
                    );
                    Renderer::setShaderVec3("matVal", 255, 0, 0);
                    Renderer::drawBoxMinMax(ge->tbPos, ge->tbPos+btVector3(1.0f, 1.0f, 1.0f));
                }



            }
        }
    }



    Renderer::setShaderVec4(
        "rotationZ", 0.0f, 0.0f, 0.0f, 0.0f
    );
    Renderer::setShaderMatrix4x4("objmat", identMat, 1);


    if(g_settings.settings[E_BS_PATH_FINDING])
    {
        GameState::gameLogic->update();
    }

    if(g_settings.settings[E_BS_RENDER_VOXELS])
    {
        Renderer::setShaderFloat("isWire", 1.0);
        Renderer::setShaderVec3("matVal", 255, 0, 0);

        rastChunk(iGetConst(E_CONST_RASTER_CHUNK_RAD), RH_FLAG_DRAWLOADING);

        if(holderInFocus!=NULL)
        {
            Renderer::setShaderVec3("matVal", 0, 0, 255);
            Renderer::drawBox(&(holderInFocus->gphMinInCells), &(holderInFocus->gphMaxInCells));
        }

        // Renderer::setShaderVec3("matVal", 0, 255, 0);
        // Renderer::drawBox(&(minShadowBounds),&(maxShadowBounds));

    }

    Renderer::setShaderFloat("isWire", 0.0f);
    Renderer::setShaderVec3("matVal", 255, 0, 0);
    Renderer::drawOrient=false;
    GameState::gamePhysics->example->renderScene();


    // if (singleton->mouseState == E_MOUSE_STATE_BRUSH) {
    // 	if (earthMod == E_PTT_TER) {
    // 		Renderer::setShaderVec3("matVal", 255, 0, 0);
    // 	}
    // 	else {
    // 		Renderer::setShaderVec3("matVal", 0, 0, 255);
    // 	}


    // 	Renderer::setShaderFloat("isWire", 1.0);
    // 	Renderer::drawCubeCentered(
    // 		&lastUnitPos,
    // 		((int)curBrushRad)
    // 	);

    // }










    // if (g_settings.settings[E_BS_RENDER_OCT_BOUNDS]) {
    // 	Renderer::setShaderFloat("isWire", 1.0);
    // 	Renderer::setShaderVec3("matVal", 255, 0, 0);
    // 	singleton->gameOct->startRender();
    // }



    // btVector3 begPos = btVector3(0.0f,0.0f,0.0f);
    // btVector3 endPos = btVector3(0.0f,0.0f,0.0f);
    // btVector3 rayDir = btVector3(0.0f,0.0f,0.0f);

    // singleton->getRay(
    // 	singleton->lastMouseX,
    // 	singleton->lastMouseY,
    // 	begPos,
    // 	endPos,
    // 	rayDir
    // );
    //lastBodyPick = example->bodyPick(begPos,endPos);

    // tempVec1.setBTV(rayDir);
    // tempVec2.setBTV(rayDir);





    // tempVec1.setBTV(rayFrom*0.99 + rayTo*0.01);
    // tempVec2.setBTV(rayTo);

    // Renderer::setShaderFloat("objectId",0.0);
    // Renderer::setShaderVec3("matVal", 255, 0, 0);
    // Renderer::drawBox(&tempVec1,&tempVec2);




    // tempVec1.copyFrom(Renderer::cameraGetPosNoShake());
    // tempVec2.copyFrom(Renderer::cameraGetPosNoShake());

    // tempVec1.addXYZ(
    // 	20.0f,
    // 	20.0f,
    // 	20.0f	
    // );

    // tempVec2.addXYZ(
    // 	200.0f,
    // 	200.0f,
    // 	GameState::smoothTime*200.0f	
    // );

    // Renderer::drawLine(&tempVec1,&tempVec2);







    // btVector3 rayFrom = Renderer::cameraGetPosNoShake()->getBTV();
    // btVector3 rayTo = singleton->getRayTo(
    // 	singleton->origWinW - singleton->lastMouseOrigX,
    // 	singleton->origWinH - singleton->lastMouseOrigY
    // );

    // tempVec1.setBTV(rayFrom*(0.995+GameState::smoothTime*0.005) + rayTo*(0.005-(1.0-GameState::smoothTime)*0.005));
    // tempVec2.copyFrom(&tempVec1);
    // //tempVec3.setBTV(rayDir);


    // tempVec1.addXYZ(-1.0f);
    // tempVec2.addXYZ( 1.0f);

    // Renderer::setShaderFloat("objectId",0.0);
    // Renderer::setShaderVec3("matVal", 255, 0, 0);
    // Renderer::drawBox(&tempVec1,&tempVec2);





    Renderer::unbindFBO();
    Renderer::unbindShader();

}



void GameWorld::finalStep(bool postToScreen)
{
    if(g_settings.settings[E_BS_TEST_1])
    {




        //terGenFBO
        //solidTargFBO



        //solidBaseTargFBO
        //"solidTargFBO" //"polyFBO"
        Renderer::drawFBO("terTargFBO", 0, 1.0f);//solidTargFBO //waterTargFBO //solidTargFBO

        // leave this here to catch errors
        //std::cout << "Getting Errors: \n";




        //glError();

    }
    else
    {

        // if (singleton->frameCount > 1) {
        // 	Renderer::bindShader("MergeShader");
        // 	Renderer::bindFBO("resultFBO",activeFBO);
        // 	Renderer::sampleFBO("resultFBO", 0, activeFBO);
        // 	Renderer::sampleFBO("lastFBO", 1);
        // 	Renderer::drawFSQuad();
        // 	Renderer::unsampleFBO("lastFBO", 1);
        // 	Renderer::unsampleFBO("resultFBO", 0, activeFBO);
        // 	Renderer::unbindFBO();
        // 	Renderer::unbindShader();

        // 	activeFBO = 1-activeFBO;
        // }
        // if (activeFBO == 0) {
        // 	Renderer::copyFBO("resultFBO0", "lastFBO");
        // }
        // else {
        // 	Renderer::copyFBO("resultFBO1", "lastFBO");
        // }


        if(g_settings.settings[E_BS_FXAA])
        {
            Renderer::bindShader("FXAAShader");
            Renderer::bindFBO("resultFBO", activeFBO);
            Renderer::sampleFBO("resultFBO", 0, activeFBO);
            Renderer::setShaderfVec2("resolution", &(Renderer::bufferDim));
            Renderer::drawFSQuad();
            Renderer::unsampleFBO("resultFBO", 0, activeFBO);
            Renderer::unbindFBO();
            Renderer::unbindShader();

            activeFBO=1-activeFBO;
        }



        if(postToScreen)
        {
            Renderer::drawFBO("resultFBO", 0, 1.0f, 1-activeFBO);
        }
        else
        {
            // Renderer::copyFBO("solidTargFBO", "rasterPosFBO");
            // Renderer::copyFBO("resultFBO"+i__s(activeFBO), "rasterSourceFBO");
        }


    }




//    if(singleton->anyMenuVisible()&&postToScreen)
//    {
//        glEnable(GL_BLEND);
//
//        Renderer::bindShader("GUIShader");
//        Renderer::setShaderTexture(0, GameState::ui->fontWrappers[EFW_TEXT]->fontImage->tid);
//        Renderer::setShaderTexture(1, GameState::ui->fontWrappers[EFW_ICONS]->fontImage->tid);
//        Renderer::sampleFBO("swapFBOBLin0", 2);
//        Renderer::setShaderTexture3D(3, volumeTex.id);
//
//        GameState::ui->renderGUI();
//
//
//        Renderer::setShaderTexture3D(3, 0);
//        Renderer::unsampleFBO("swapFBOBLin0", 2);
//        Renderer::setShaderTexture(1, 0);
//        Renderer::setShaderTexture(0, 0);
//        Renderer::unbindShader();
//
//        glDisable(GL_BLEND);
//    }
}

void GameWorld::postProcess(bool postToScreen)
{



//    int i;
//    int k;
//    int curCount;
//    int baseInd;

    bool doProc=false;

    //GameEnt *curLight;



    // for (i = 0; i < singleton->numDynLights; i++)
    // {
    // 	activeLights[i] = singleton->dynObjects[E_OBJ_LIGHT0 + i]->getLight();
    // }

    // curCount = 0;
    // for (k = 0; k < lightCount; k++)
    // {
    // 	baseInd = curCount * FLOATS_PER_LIGHT;
    // 	curLight = activeLights[k];
    // 	lightPos = &(curLight->geomParams[E_LP_POSITION]);

    // 	// if (curLight->toggled) {
    // 	// 	singleton->worldToScreenBase(&lScreenCoords, lightPos);

    // 	// 	lightArr[baseInd + 0] = lightPos->getFX();
    // 	// 	lightArr[baseInd + 1] = lightPos->getFY();
    // 	// 	lightArr[baseInd + 2] = lightPos->getFZ();
    // 	// 	lightArr[baseInd + 3] = lScreenCoords.getFZ();


    // 	// 	lightArr[baseInd + 4] = lScreenCoords.getFX();
    // 	// 	lightArr[baseInd + 5] = lScreenCoords.getFY();
    // 	// 	lightArr[baseInd + 6] = lScreenCoords.getFZ();
    // 	// 	lightArr[baseInd + 7] = curLight->geomParams[E_LP_RADIUS].getFX();


    // 	// 	// light color

    // 	// 	lightArr[baseInd + 8] = curLight->geomParams[E_LP_COLOR].getFX(); // light red
    // 	// 	lightArr[baseInd + 9] = curLight->geomParams[E_LP_COLOR].getFY(); // light green
    // 	// 	lightArr[baseInd + 10] = curLight->geomParams[E_LP_COLOR].getFZ(); // light blue

    // 	// 	// switch (k)
    // 	// 	// {
    // 	// 	// case 0:
    // 	// 	// 	lightArr[baseInd + 11] = 1.0f; // light intensity (unused?)
    // 	// 	// 	lightArr[baseInd + 12] = 0.0f; // light colorization (0-1)
    // 	// 	// 	lightArr[baseInd + 13] = 0.0f; // light flooding (colorizes regardless of shadows) (0-1)
    // 	// 	// 	break;
    // 	// 	// default:
    // 	// 	// 	lightArr[baseInd + 11] = 1.0f;
    // 	// 	// 	lightArr[baseInd + 12] = 1.0f;
    // 	// 	// 	lightArr[baseInd + 13] = 0.0f;
    // 	// 	// 	break;

    // 	// 	// }

    // 	// 	curCount++;
    // 	// }


    // }





    //renderWaveHeight();

    if(g_settings.settings[E_BS_WATER])
    {





        Renderer::bindShader("WaterShaderCombine");
        Renderer::bindFBO("combineWithWaterTargFBO");
        Renderer::sampleFBO("solidTargFBO", 0);
        Renderer::sampleFBO("waterTargFBO", 2);
        //Renderer::sampleFBO("geomTargFBO", 4);
        //Renderer::sampleFBO("waveFBO", 6);

        // Renderer::setShaderFloat("clipDist",Renderer::clipDist[1]);
        // Renderer::setShaderVec2("resolution", Renderer::currentFBOResolutionX, Renderer::currentFBOResolutionY);
        // Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
        // Renderer::setShaderfVec2("bufferDim", &(Renderer::bufferModDim));
        // Renderer::setShaderFloat("curTime", GameState::curTime);
        Renderer::drawFSQuad();

        //Renderer::unsampleFBO("waveFBO", 6);
        //Renderer::unsampleFBO("geomTargFBO", 4);
        Renderer::unsampleFBO("waterTargFBO", 2);
        Renderer::unsampleFBO("solidTargFBO", 0);
        Renderer::unbindFBO();
        Renderer::unbindShader();



    }
    else
    {
        // TODO: copy over any necessary buffers when water is off

        Renderer::copyFBO2("solidTargFBO", "combineWithWaterTargFBO");

    }


    //Renderer::projMatrix*






    Renderer::bindShader("PreLightingShader");
    Renderer::bindFBO("prelightFBO");
    Renderer::sampleFBO("solidTargFBO", 0);
    Renderer::sampleFBO("combineWithWaterTargFBO", 2);
    Renderer::sampleFBO("geomTargFBO", 4);
    Renderer::sampleFBO("prmDepthFBO", 6);
    Renderer::setShaderMatrix3x3("rotMat", Renderer::curObjMatrix3.get(), 1);
    Renderer::setShaderMatrix4x4("modelview", Renderer::viewMatrix.get(), 1);
    Renderer::setShaderMatrix4x4("proj", Renderer::projMatrix.get(), 1);
    Renderer::setShaderVec2("clipDist", Renderer::clipDist[0], Renderer::clipDist[1]);
    Renderer::setShaderfVec3("lookAtVec", &(Renderer::lookAtVec));
    Renderer::setShaderVec2("bufferDim", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY); //MUST BE CALLED AFTER FBO IS BOUND
    Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
    Renderer::setShaderInt("testOn", (int)(g_settings.settings[E_BS_TEST_1]));
    Renderer::setShaderInt("iNumSteps", g_settings.iNumSteps);
    Renderer::setShaderArrayfVec4("lightArr", lightArr, (FLOATS_PER_LIGHT * lightCount)/4);
    Renderer::setShaderInt("vecsPerLight", FLOATS_PER_LIGHT/4);
    Renderer::setShaderFloat("lightCount", (float)lightCount);
    Renderer::setShaderFloat("timeOfDay", timeOfDay);
    Renderer::setShaderfVec3("baseLightVec", &(Renderer::lightVec));

    Renderer::drawFSQuad();

    Renderer::unsampleFBO("prmDepthFBO", 6);
    Renderer::unsampleFBO("geomTargFBO", 4);
    Renderer::unsampleFBO("combineWithWaterTargFBO", 2);
    Renderer::unsampleFBO("solidTargFBO", 0);
    Renderer::unbindFBO();
    Renderer::unbindShader();





    Renderer::bindShader("PostLightingShader");
    Renderer::bindFBO("resultFBO", activeFBO);

    Renderer::sampleFBO("solidTargFBO", 0);
    Renderer::sampleFBO("prelightFBO", 2);
    // Renderer::sampleFBO("geomTargFBO", 6);
    // Renderer::sampleFBO("debugTargFBO", 8);
    Renderer::setShaderTexture3D(10, volumeTex.id);

    Renderer::setShaderfVec4("worldMarker", &(GameState::worldMarker));
    Renderer::setShaderInt("markerFound", (int)(GameState::markerFound));


    Renderer::setShaderMatrix4x4("modelviewInverse", Renderer::viewMatrixDI, 1);
    Renderer::setShaderFloat("FOV", Renderer::FOV*(float)M_PI/180.0f);
    Renderer::setShaderVec2("clipDist", Renderer::clipDist[0], Renderer::clipDist[1]);
    Renderer::setShaderfVec3("lightVec", &(Renderer::lightVec));
    Renderer::setShaderfVec3("lightVecOrig", &(Renderer::lightVecOrig));


    Renderer::setShaderfVec3("lookAtVec", &(Renderer::lookAtVec));
    Renderer::setShaderVec2("bufferDim", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY); //MUST BE CALLED AFTER FBO IS BOUND
    Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
    Renderer::setShaderInt("testOn", (int)(g_settings.settings[E_BS_TEST_1]));
    Renderer::setShaderInt("testOn2", (int)(g_settings.settings[E_BS_TEST_2]));
    Renderer::setShaderFloat("curTime", (float)GameState::curTime);
    Renderer::setShaderFloat("cellsPerBlock", (float)g_settings.cellsPerBlock);
    Renderer::setShaderFloat("timeOfDay", timeOfDay);
    Renderer::drawFSQuad();

    Renderer::setShaderTexture3D(10, 0);
    // Renderer::unsampleFBO("debugTargFBO", 8);
    // Renderer::unsampleFBO("geomTargFBO", 6);
    Renderer::unsampleFBO("prelightFBO", 2);
    Renderer::unsampleFBO("solidTargFBO", 0);

    Renderer::unbindFBO();
    Renderer::unbindShader();

    activeFBO=1-activeFBO;



    if(g_settings.settings[E_BS_WATER])
    {
        Renderer::copyFBO("resultFBO1", "swapFBOLinHalf0");
        doBlur("swapFBOLinHalf");

        Renderer::bindShader("WaterShader");
        Renderer::bindFBO("resultFBO", activeFBO);
        Renderer::sampleFBO("solidTargFBO", 0);
        Renderer::sampleFBO("combineWithWaterTargFBO", 2);
        Renderer::sampleFBO("resultFBO", 4, activeFBO);
        Renderer::sampleFBO("swapFBOLinHalf0", 5);
        Renderer::sampleFBO("noiseFBO", 6);
        //Renderer::sampleFBO("waveFBO", 7);
        Renderer::setShaderTexture3D(7, volumeTex.id);
        Renderer::sampleFBO("prelightFBO", 9);
        //Renderer::sampleFBO("geomTargFBO", 13);


        Renderer::setShaderMatrix4x4("modelviewInverse", Renderer::viewMatrixDI, 1);
        Renderer::setShaderFloat("seaLevel", getSeaHeightScaled());
        Renderer::setShaderFloat("FOV", Renderer::FOV*(float)M_PI/180.0f);
        Renderer::setShaderVec2("clipDist", Renderer::clipDist[0], Renderer::clipDist[1]);
        Renderer::setShaderfVec2("bufferDim", &(Renderer::bufferModDim));

        Renderer::setShaderfVec3("lookAtVec", &(Renderer::lookAtVec));
        Renderer::setShaderFloat("timeOfDay", timeOfDay);
        Renderer::setShaderVec2("resolution", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY);
        Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
        Renderer::setShaderFloat("curTime", (float)GameState::curTime);
        Renderer::setShaderFloat("isUnderWater", getUnderWater());
        Renderer::drawFSQuad();


        //Renderer::unsampleFBO("geomTargFBO", 13);
        Renderer::unsampleFBO("prelightFBO", 9);
        Renderer::setShaderTexture3D(7, 0);
        //Renderer::unsampleFBO("waveFBO", 7);
        Renderer::unsampleFBO("noiseFBO", 6);
        Renderer::unsampleFBO("swapFBOLinHalf0", 5);
        Renderer::unsampleFBO("resultFBO", 4, activeFBO);
        Renderer::unsampleFBO("waterTargFBO", 2);
        Renderer::unsampleFBO("solidTargFBO", 0);
        Renderer::unbindFBO();
        Renderer::unbindShader();

        activeFBO=1-activeFBO;
    }


    if(g_settings.settings[E_BS_RADIOSITY]||g_settings.settings[E_BS_FOG])
    {

        if(activeFBO==0)
        {
            Renderer::copyFBO("resultFBO0", "swapFBOBLin0");
        }
        else
        {
            Renderer::copyFBO("resultFBO1", "swapFBOBLin0");
        }


        doBlur("swapFBOBLin");
    }



    if(g_settings.settings[E_BS_RADIOSITY])
    {

        Renderer::bindShader("RadiosityShader");

        Renderer::bindFBO("swapFBOLinHalf0");
        Renderer::sampleFBO("combineWithWaterTargFBO", 0);
        Renderer::sampleFBO("swapFBOBLin0", 2);
        Renderer::setShaderVec2("resolution", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY); //MUST BE CALLED AFTER FBO IS BOUND
        Renderer::setShaderfVec2("bufferDim", &(Renderer::bufferModDim));
        Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
        Renderer::setShaderfVec3("lightVec", &(Renderer::lightVec));
        //Renderer::setShaderfVec3("lightPosWS", lightPos);
        Renderer::setShaderInt("iNumSteps", g_settings.iNumSteps);
        Renderer::drawFSQuad();
        Renderer::unsampleFBO("swapFBOBLin0", 2);
        Renderer::unsampleFBO("combineWithWaterTargFBO", 0);
        Renderer::unbindFBO();
        Renderer::unbindShader();


        doBlur("swapFBOLinHalf");

        Renderer::bindShader("RadiosityCombineShader");
        Renderer::bindFBO("resultFBO", activeFBO);
        Renderer::sampleFBO("resultFBO", 0, activeFBO);
        Renderer::sampleFBO("swapFBOLinHalf0", 1);
        Renderer::sampleFBO("combineWithWaterTargFBO", 2);
        //Renderer::sampleFBO("geomTargFBO", 4);
        Renderer::setShaderInt("testOn", (int)(g_settings.settings[E_BS_TEST_1]));
        Renderer::drawFSQuad();
        //Renderer::unsampleFBO("geomTargFBO", 4);
        Renderer::unsampleFBO("combineWithWaterTargFBO", 2);
        Renderer::unsampleFBO("swapFBOLinHalf0", 1);
        Renderer::unsampleFBO("resultFBO", 0, activeFBO);
        Renderer::unbindFBO();
        Renderer::unbindShader();

        activeFBO=1-activeFBO;


    }

    if(g_settings.settings[E_BS_FOG])
    {



        Renderer::bindShader("FogShader");
        Renderer::bindFBO("resultFBO", activeFBO);

        Renderer::sampleFBO("combineWithWaterTargFBO", 0);
        Renderer::sampleFBO("resultFBO", 2, activeFBO);
        Renderer::sampleFBO("swapFBOBLin0", 3);

        Renderer::sampleFBO("limbFBO", 4);//, -1, 4, 7);
        //Renderer::sampleFBO("geomBaseTargFBO", 4);

        Renderer::setShaderTexture3D(7, volumeTex.id);
        Renderer::sampleFBO("noiseFBOLinear", 8);
        Renderer::sampleFBO("debugTargFBO", 9);

        Renderer::sampleFBO("numstepsFBO", 11);


        if((GameState::gem->getCurActor()==NULL)||GameState::gem->firstPerson)
        {
            Renderer::setShaderFloat("thirdPerson", 0.0f);
            Renderer::setShaderVec3("entPos", 0.0f, 0.0f, 0.0f);
        }
        else
        {
            Renderer::setShaderFloat("thirdPerson", 1.0f);
            Renderer::setShaderfVec3("entPos", GameState::gem->getCurActor()->getCenterPointFIV(0));
            Renderer::setShaderFloat("volSizePrim", (float)GameState::gameFluid[E_FID_BIG]->volSizePrim);
        }

        if(g_settings.settings[E_BS_PLACING_PATTERN])
        {
            Renderer::setShaderArray(
                "patternCells",
                patterns[
                    curPattern*4+curPatternRot
                ].patternVals,
                PATTERN_SIZE*PATTERN_SIZE
                        );
            Renderer::setShaderfVec3("patternTarg", &(GameState::ui->mouseMovePixData.pd[0]));

        }
        Renderer::setShaderInt("placingPattern", g_settings.settings[E_BS_PLACING_PATTERN]);


        Renderer::setShaderInt("testOn2", (int)(g_settings.settings[E_BS_TEST_2]));

        Renderer::setShaderInt("gridOn", g_settings.settings[E_BS_SHOW_GRID]);

        if(GameState::gem->getCurActor()==NULL)
        {
            Renderer::setShaderInt("isFalling", false);
            Renderer::setShaderInt("isJumping", false);
        }
        else
        {
            Renderer::setShaderInt("isFalling", GameState::gem->getCurActor()->allFalling());
            Renderer::setShaderInt("isJumping", GameState::gem->getCurActor()->getActionState(E_ACT_ISJUMPING, RLBN_NEIT));
        }


        Renderer::setShaderFloat("seaLevel", getSeaHeightScaled());
        Renderer::setShaderMatrix4x4("modelviewInverse", Renderer::viewMatrixDI, 1);
        Renderer::setShaderFloat("FOV", Renderer::FOV*(float)M_PI/180.0f);
        Renderer::setShaderVec2("clipDist", Renderer::clipDist[0], Renderer::clipDist[1]);
        Renderer::setShaderfVec3("lightVec", &(Renderer::lightVec));
        Renderer::setShaderfVec3("lightVecOrig", &(Renderer::lightVecOrig));
        Renderer::setShaderInt("iNumSteps", g_settings.iNumSteps);
        Renderer::setShaderFloat("curTime", (float)GameState::curTime);
        Renderer::setShaderFloat("selLimbInd", (float)GameState::gem->highlightedLimb);
        Renderer::setShaderFloat("selObjInd", (float)GameState::gem->selObjInd);
        Renderer::setShaderFloat("actObjInd", (float)GameState::gem->actObjInd);
        Renderer::setShaderFloat("isUnderWater", getUnderWater());
        Renderer::setShaderFloat("timeOfDay", timeOfDay);
        Renderer::setShaderVec2("resolution", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY); //MUST BE CALLED AFTER FBO IS BOUND
        Renderer::setShaderfVec2("bufferDim", &(Renderer::bufferModDim));
        Renderer::setShaderfVec3("cameraPos", Renderer::cameraGetPos());
        Renderer::setShaderfVec3("lookAtVec", &(Renderer::lookAtVec));
        //Renderer::setShaderfVec4("fogPos", fogPos);

        Renderer::drawFSQuad();

        Renderer::unsampleFBO("numstepsFBO", 11);
        Renderer::unsampleFBO("debugTargFBO", 9);
        Renderer::unsampleFBO("noiseFBOLinear", 8);
        Renderer::setShaderTexture3D(7, 0);

        Renderer::unsampleFBO("limbFBO", 4);//, -1, 4, 7);
        //Renderer::unsampleFBO("geomBaseTargFBO", 4);

        Renderer::unsampleFBO("swapFBOBLin0", 3);
        Renderer::unsampleFBO("resultFBO", 2, activeFBO);
        Renderer::unsampleFBO("combineWithWaterTargFBO", 0);

        Renderer::unbindFBO();
        Renderer::unbindShader();


        activeFBO=1-activeFBO;

    }





}

float GameWorld::getSeaHeightScaled()
{
    return getSLNormalized()*g_settings.heightMapMaxInCells;
}

float GameWorld::getTerHeightScaled(float val)
{
    return val*g_settings.heightMapMaxInCells;
}

void GameWorld::updatePrimArr()
{
    int i;
//    int j;

    ObjectStruct* curObj;
//    FIVector4* baseGeom;

    for(i=0; i<tempPrimList.size(); i++)
    {
        curObj=&(tempPrimList[i]);
        //baseGeom = getGeomRef(curObj->templateId,0);

        primArr[i*8+0]=curObj->offset.x;
        primArr[i*8+1]=curObj->offset.y;
        primArr[i*8+2]=curObj->offset.z;
        primArr[i*8+3]=(float)curObj->templateId;

        // texelRes2
        primArr[i*8+4]=(float)curObj->globalId;
        primArr[i*8+5]=0;
        primArr[i*8+6]=0;
        primArr[i*8+7]=0;

    }

    if(g_settings.settings[E_BS_PLACING_GEOM])
    {
        tempVec1.copyFrom(&(geomPoints[0]));
        tempVec1.addXYZRef(&(geomOrigOffset));
        tempVec1.setFW((float)curPrimTemplate);
        tempVec2.setFXYZW(0.0f, 0.0f, 0.0f, 0.0f);

        primArr[i*8+0]=tempVec1[0];
        primArr[i*8+1]=tempVec1[1];
        primArr[i*8+2]=tempVec1[2];
        primArr[i*8+3]=tempVec1[3];

        primArr[i*8+4]=0;
        primArr[i*8+5]=0;
        primArr[i*8+6]=0;
        primArr[i*8+7]=0;

        i++;
    }

    primArrPos=i;

    // for (j = i; j < (i+actorCount); j++) {
    // 	primArr[i*8 + 0] = tempVec1[0];
    // 	primArr[i*8 + 1] = tempVec1[1];
    // 	primArr[i*8 + 2] = tempVec1[2];
    // 	primArr[i*8 + 3] = tempVec1[3];

    // 	primArr[i*8 + 4] = 0;
    // 	primArr[i*8 + 5] = 1;
    // 	primArr[i*8 + 6] = 0;
    // 	primArr[i*8 + 7] = 0;
    // }
}

float GameWorld::getUnderWater()
{
    if(
        (getCellAtCoords(
        (int)Renderer::cameraGetPosNoShake()->getFX(),
        (int)Renderer::cameraGetPosNoShake()->getFY(),
        (int)(Renderer::cameraGetPosNoShake()->getFZ()-1.0f)
        )==E_CD_WATER)||
        (Renderer::cameraGetPosNoShake()->getFZ()<(getSeaHeightScaled()-32.0f))
        )
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }

}


void GameWorld::updateMatVol()
{
    matVolLock=true;

    JSONValue* jv=g_settings.fetchJSONData("materials.js", false);
    JSONValue* curJV=NULL;
    JSONValue* curK=NULL;
    JSONValue* curJ=NULL;
    JSONValue* curI=NULL;
    JSONValue* curCol=NULL;

    int i;
    int j;
    int k;
    int m;
    int n;
    int p;
    int totN;

    int curInd0;
    int curInd0Prev;
    int curInd0Next;
    int curInd1;

    int mvInd;

    uint rv, gv, bv;

    joi_type itI;
    joi_type itJ;
    joi_type itK;

    float totRatio;
    float curLerp;
    float curLerpNoPower;
    float curPower;
    float curLerpWithPower;
    float curRatio;

    int mvx=matVolDim.getIX();
    int mvy=matVolDim.getIY();
    int mvz=matVolDim.getIZ();

    if(jv!=NULL)
    {





        curJV=jv->Child("materials");

        k=0;
        for(itK=curJV->object_value.begin(); itK!=curJV->object_value.end(); itK++)
        {
            curK=itK->second;

            j=0;
            for(itJ=curK->object_value.begin(); itJ!=curK->object_value.end(); itJ++)
            {
                curJ=itJ->second;

                i=0;
                totRatio=0.0f;
                for(itI=curJ->object_value.begin(); itI!=curJ->object_value.end(); itI++)
                {
                    curI=itI->second;

                    curInd0=i+j*mvx;
                    curCol=curI->Child("i0_color");


                    matSlice0[curInd0].h=(float)curCol->Child(0)->number_value;
                    matSlice0[curInd0].s=(float)curCol->Child(1)->number_value;
                    matSlice0[curInd0].l=(float)curCol->Child(2)->number_value;

                    hsv2rgb(&(matSlice0[curInd0]));

                    matSlice0[curInd0].power=(float)curI->Child("i1_power")->number_value;
                    matSlice0[curInd0].ratio=(float)curI->Child("i2_ratio")->number_value;

                    if(matSlice0[curInd0].ratio<=0.0f)
                    {
                        matSlice0[curInd0].ratio=1.0f/(mvx-1.0f);
                    }

                    totRatio+=matSlice0[curInd0].ratio;

                    i++;
                }


                if(i==0)
                {

                }
                else
                {
                    for(m=0; m<i; m++)
                    {
                        curInd0=m+j*mvx;
                        matSlice0[curInd0].ratio=matSlice0[curInd0].ratio*(matVolDim.getFX()-1.0f)/totRatio;
                    }


                    totN=0;
                    for(m=0; m<i; m++)
                    {
                        curInd0=m+j*mvx;

                        if(i==1)
                        {
                            curInd0Prev=curInd0;
                            curInd0Next=curInd0;
                        }
                        else
                        {
                            curInd0Prev=(m-1)+j*mvx;
                            curInd0Next=(m+1)+j*mvx;
                        }



                        curRatio=matSlice0[curInd0].ratio;

                        for(n=0; (n<(curRatio+0.1f))&&(totN<mvx); n++)
                        {
                            curLerp=((float)n)/curRatio;
                            curInd1=totN+j*mvx;

                            if(m==0)
                            {

                                curLerpNoPower=curLerp*0.5f;
                                curPower=mixf(matSlice0[curInd0].power, matSlice0[curInd0Next].power, curLerpNoPower);
                                curLerpWithPower=pow(curLerpNoPower, curPower*8.0f);

                                matSlice1[curInd1].r=mixf(matSlice0[curInd0].r, matSlice0[curInd0Next].r, curLerpWithPower);
                                matSlice1[curInd1].g=mixf(matSlice0[curInd0].g, matSlice0[curInd0Next].g, curLerpWithPower);
                                matSlice1[curInd1].b=mixf(matSlice0[curInd0].b, matSlice0[curInd0Next].b, curLerpWithPower);
                            }
                            else if(m==(i-1))
                            {
                                curLerpNoPower=curLerp*0.5f+0.5f;
                                curPower=mixf(matSlice0[curInd0Prev].power, matSlice0[curInd0].power, curLerpNoPower);
                                curLerpWithPower=pow(curLerpNoPower, curPower*8.0f);

                                matSlice1[curInd1].r=mixf(matSlice0[curInd0Prev].r, matSlice0[curInd0].r, curLerpWithPower);
                                matSlice1[curInd1].g=mixf(matSlice0[curInd0Prev].g, matSlice0[curInd0].g, curLerpWithPower);
                                matSlice1[curInd1].b=mixf(matSlice0[curInd0Prev].b, matSlice0[curInd0].b, curLerpWithPower);
                            }
                            else
                            {


                                if(curLerp<0.5f)
                                {
                                    curLerpNoPower=curLerp+0.5f;
                                    curPower=mixf(matSlice0[curInd0Prev].power, matSlice0[curInd0].power, curLerpNoPower);
                                    curLerpWithPower=pow(curLerpNoPower, curPower*8.0f);

                                    matSlice1[curInd1].r=mixf(matSlice0[curInd0Prev].r, matSlice0[curInd0].r, curLerpWithPower);
                                    matSlice1[curInd1].g=mixf(matSlice0[curInd0Prev].g, matSlice0[curInd0].g, curLerpWithPower);
                                    matSlice1[curInd1].b=mixf(matSlice0[curInd0Prev].b, matSlice0[curInd0].b, curLerpWithPower);
                                }
                                else
                                {
                                    curLerpNoPower=curLerp-0.5f;
                                    curPower=mixf(matSlice0[curInd0].power, matSlice0[curInd0Next].power, curLerpNoPower);
                                    curLerpWithPower=pow(curLerpNoPower, curPower*8.0f);

                                    matSlice1[curInd1].r=mixf(matSlice0[curInd0].r, matSlice0[curInd0Next].r, curLerpWithPower);
                                    matSlice1[curInd1].g=mixf(matSlice0[curInd0].g, matSlice0[curInd0Next].g, curLerpWithPower);
                                    matSlice1[curInd1].b=mixf(matSlice0[curInd0].b, matSlice0[curInd0Next].b, curLerpWithPower);

                                }



                            }



                            totN++;
                        }


                    }
                }





                j++;

            }





            ////////////////////





            for(p=0; p<mvx; p++)
            {

                totN=0;
                for(m=0; m<j; m++)
                {
                    curInd0=p+m*mvx;

                    if(j==1)
                    {
                        curInd0Prev=curInd0;
                        curInd0Next=curInd0;
                    }
                    else
                    {
                        curInd0Prev=p+(m-1)*mvx;
                        curInd0Next=p+(m+1)*mvx;
                    }



                    if(j==0)
                    {
                        curRatio=(float(mvy));
                    }
                    else
                    {
                        curRatio=(float(mvy))/(float(j));
                    }


                    for(n=0; (n<(curRatio+0.1f))&&(totN<mvy); n++)
                    {
                        curLerp=((float)n)/curRatio;
                        curInd1=p+totN*mvx;

                        if(m==0)
                        {

                            curLerpWithPower=curLerp*0.5f;

                            matSlice2[curInd1].r=mixf(matSlice1[curInd0].r, matSlice1[curInd0Next].r, curLerpWithPower);
                            matSlice2[curInd1].g=mixf(matSlice1[curInd0].g, matSlice1[curInd0Next].g, curLerpWithPower);
                            matSlice2[curInd1].b=mixf(matSlice1[curInd0].b, matSlice1[curInd0Next].b, curLerpWithPower);
                        }
                        else if(m==(j-1))
                        {
                            curLerpWithPower=curLerp*0.5f+0.5f;

                            matSlice2[curInd1].r=mixf(matSlice1[curInd0Prev].r, matSlice1[curInd0].r, curLerpWithPower);
                            matSlice2[curInd1].g=mixf(matSlice1[curInd0Prev].g, matSlice1[curInd0].g, curLerpWithPower);
                            matSlice2[curInd1].b=mixf(matSlice1[curInd0Prev].b, matSlice1[curInd0].b, curLerpWithPower);
                        }
                        else
                        {


                            if(curLerp<0.5f)
                            {
                                curLerpWithPower=curLerp+0.5f;

                                matSlice2[curInd1].r=mixf(matSlice1[curInd0Prev].r, matSlice1[curInd0].r, curLerpWithPower);
                                matSlice2[curInd1].g=mixf(matSlice1[curInd0Prev].g, matSlice1[curInd0].g, curLerpWithPower);
                                matSlice2[curInd1].b=mixf(matSlice1[curInd0Prev].b, matSlice1[curInd0].b, curLerpWithPower);
                            }
                            else
                            {
                                curLerpWithPower=curLerp-0.5f;

                                matSlice2[curInd1].r=mixf(matSlice1[curInd0].r, matSlice1[curInd0Next].r, curLerpWithPower);
                                matSlice2[curInd1].g=mixf(matSlice1[curInd0].g, matSlice1[curInd0Next].g, curLerpWithPower);
                                matSlice2[curInd1].b=mixf(matSlice1[curInd0].b, matSlice1[curInd0Next].b, curLerpWithPower);

                            }



                        }



                        mvInd=curInd1+k*mvx*mvy;
                        rv=(uint)(matSlice2[curInd1].r*255.0f);
                        gv=(uint)(matSlice2[curInd1].g*255.0f);
                        bv=(uint)(matSlice2[curInd1].b*255.0f);
                        matVol[mvInd]=(0<<24)|(bv<<16)|(gv<<8)|(rv);




                        totN++;
                    }


                }
            }




            ////////////////////










            k++;
        }

        int size=mvx*mvy*mvz;

        volumeTex.update(matVol, size);

//
//        glBindTexture(GL_TEXTURE_3D, 0);
//        glBindTexture(GL_TEXTURE_3D, volIdMat);
//        glTexSubImage3D(
//            GL_TEXTURE_3D,
//            0,
//
//            0,
//            0,
//            0,
//
//            mvx,
//            mvy,
//            mvz,
//
//            GL_RGBA,
//            GL_UNSIGNED_BYTE,
//
//            matVol
//        );
//        glBindTexture(GL_TEXTURE_3D, 0);
//
    }
    matVolLock=false;

}

void hsv2rgb(materialNode* matNode)
{


    static FIVector4 K_HSV;
    static FIVector4 P_HSV;
    static FIVector4 C_HSV;
    static FIVector4 R_HSV;

    K_HSV.setFXYZW(1.0f, 2.0f/3.0f, 1.0f/3.0f, 3.0f);

    C_HSV.setFXYZ(matNode->h, matNode->s, matNode->l);


    P_HSV.setFXYZ(
        abs(fract(C_HSV[0]+K_HSV[0]) * 6.0f-K_HSV[3]),
        abs(fract(C_HSV[0]+K_HSV[1]) * 6.0f-K_HSV[3]),
        abs(fract(C_HSV[0]+K_HSV[2]) * 6.0f-K_HSV[3])
    );
    R_HSV.setFXYZ(
        C_HSV[2]*mixf(K_HSV[0], clampf(P_HSV[0]-K_HSV[0], 0.0f, 1.0f), C_HSV[1]),
        C_HSV[2]*mixf(K_HSV[0], clampf(P_HSV[1]-K_HSV[0], 0.0f, 1.0f), C_HSV[1]),
        C_HSV[2]*mixf(K_HSV[0], clampf(P_HSV[2]-K_HSV[0], 0.0f, 1.0f), C_HSV[1])
    );

    matNode->r=R_HSV[0];
    matNode->g=R_HSV[1];
    matNode->b=R_HSV[2];

}

void GameWorld::applyPat(
    int patInd,
    int patShape,
    int rot,
    int x,
    int y,
    int val,
    int rad
)
{
    int i;
    int j;
    int ind;

    bool doProc=false;

    int cenX;
    int cenY;

    switch(rot)
    {
    case 0:
        cenX=PATTERN_CENTER+x;
        cenY=PATTERN_CENTER+y;
        break;
    case 1:
        cenX=PATTERN_CENTER-y;
        cenY=PATTERN_CENTER+x;
        break;
    case 2:
        cenX=PATTERN_CENTER-x;
        cenY=PATTERN_CENTER-y;
        break;
    case 3:
        cenX=PATTERN_CENTER+y;
        cenY=PATTERN_CENTER-x;
        break;
    }

    std::cout<<"\n\n";

    for(j=0; j<PATTERN_SIZE; j++)
    {
        std::cout<<"\n";
        for(i=0; i<PATTERN_SIZE; i++)
        {
            ind=i+j*PATTERN_SIZE;

            doProc=false;
            switch(patShape)
            {
            case E_PATSHAPE_SQUARE:
                doProc=(
                    (abs(i-cenX)<=rad)&&
                    (abs(j-cenY)<=rad)
                    );
                break;
            case E_PATSHAPE_DIAMOND:
                doProc=(
                    (
                    abs(i-cenX)+
                    abs(j-cenY)
                    )<=rad
                    );
                break;
            }

            if(doProc)
            {
                patterns[patInd*4+rot].patternVals[ind]=(float)val;
                std::cout<<"X";
            }
            else
            {
                std::cout<<"O";
            }
        }
    }

}



void GameWorld::generatePatterns()
{
    int q;

//    int i;
//    int j;
    int k;

//    int xb;
//    int yb;

    for(k=0; k<4; k++)
    {
        for(q=0; q<E_PAT_LENGTH; q++)
        {
            switch(q)
            {
            case E_PAT_1X1SQUARE:
                applyPat(q, E_PATSHAPE_SQUARE, k, 0, 0, 1, 0);
                break;
            case E_PAT_3X3SQUARE:
                applyPat(q, E_PATSHAPE_SQUARE, k, 0, 0, 1, 1);
                break;
            case E_PAT_5X5SQUARE:
                applyPat(q, E_PATSHAPE_SQUARE, k, 0, 0, 1, 2);
                break;
            case E_PAT_1X1DIAMOND:
                applyPat(q, E_PATSHAPE_DIAMOND, k, 0, 0, 1, 0);
                break;
            case E_PAT_3X3DIAMOND:
                applyPat(q, E_PATSHAPE_DIAMOND, k, 0, 0, 1, 1);
                break;
            case E_PAT_5X5DIAMOND:
                applyPat(q, E_PATSHAPE_DIAMOND, k, 0, 0, 1, 2);
                break;

            }
        }
    }
}

void GameWorld::updatePrimTBOData()
{
    int i;
    int j;

    for(i=0; i<primTemplateStack.size(); i++)
    {

        for(j=0; j<4; j++)
        {
            primTBOData[i*4+j]=primTemplateStack[i][j];
        }

    }

    Renderer::primTBO.update(primTBOData, NULL, (int)primTemplateStack.size()*4*4);
}
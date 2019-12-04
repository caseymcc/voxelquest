#include "voxelquest/cache.h"
#include "voxelquest/gamepageholder.h"
#include "voxelquest/gamestate.h"
#include "voxelquest/helperfuncs.h"
#include "voxelquest/gamelogic.h"
#include "voxelquest/fileio.h"

#include <iostream>

enum E_CACHE_METADATA
{
    E_CMD_MIPMIN_0,
    E_CMD_MIPMAX_0,

    E_CMD_MIPMIN_1,
    E_CMD_MIPMAX_1,

    E_CMD_MIPMIN_2,
    E_CMD_MIPMAX_2,

    E_CMD_MIPMIN_3,
    E_CMD_MIPMAX_3,

    E_CMD_MIPMIN_4,
    E_CMD_MIPMAX_4,

    E_CMD_MIPMIN_5,
    E_CMD_MIPMAX_5,

    E_CMD_VERSION,
    E_CMD_SIZEINFLOATS,
    E_CMD_LENGTH
};

std::string curCLFull;
std::string curCLBaseDir;
std::string curCLWorldDir;
JSONValue *cacheMetaJS;
int cacheVersion=1;

bool checkCacheEntry(int blockId, int chunkId, int holderId, bool doErase)
{

    if(DO_CACHE)
    {

    }
    else
    {
        return false;
    }

    std::string entryName="b"+i__s(blockId)+"c"+i__s(chunkId)+"h"+i__s(holderId);
    GamePageHolder* curHolder=GameState::gameLogic->getHolderById(blockId, chunkId, holderId);

    if(curHolder==NULL)
    {
        return false;
    }
    if(cacheMetaJS==NULL)
    {
        return false;
    }

    if(cacheMetaJS->Child("pages")->HasChild(entryName))
    {
        if(doErase)
        {
            cacheMetaJS->Child("pages")->RemoveChild(entryName);
        }
        return true;
    }

    return false;

}

bool loadCacheEntry(int blockId, int chunkId, int holderId)
{
    std::string entryName="b"+i__s(blockId)+"c"+i__s(chunkId)+"h"+i__s(holderId);
    GamePageHolder* curHolder=GameState::gameLogic->getHolderById(blockId, chunkId, holderId);
    JSONValue* curEntry;

    int i;
    int curVersion;
    int curDataSizeInFloats;

    if(curHolder==NULL)
    {
        return false;
    }
    if(cacheMetaJS==NULL)
    {
        return false;
    }
    if(cacheMetaJS->Child("pages")->HasChild(entryName))
    {

        curEntry=cacheMetaJS->Child("pages")->Child(entryName);

        curVersion=(int)curEntry->array_value[E_CMD_VERSION]->number_value;
        curDataSizeInFloats=(int)curEntry->array_value[E_CMD_SIZEINFLOATS]->number_value;

        if(curDataSizeInFloats==0)
        {
            return true;
        }

        for(i=0; i<NUM_MIP_LEVELS_WITH_FIRST; i++)
        {
            curHolder->begMip[i]=(int)curEntry->array_value[i*2+0]->number_value;
            curHolder->endMip[i]=(int)curEntry->array_value[i*2+1]->number_value;
        }

        curHolder->vertexVec.resize(curDataSizeInFloats);

        if(
            loadFloatArray(
            curCLFull+entryName+".bin",
            &(curHolder->vertexVec[0]),
            curDataSizeInFloats
            )
            )
        {
            return true;
        }
    }


    return false;
}

bool saveCacheEntry(int blockId, int chunkId, int holderId)
{
    std::string entryName="b"+i__s(blockId)+"c"+i__s(chunkId)+"h"+i__s(holderId);
    JSONValue* curEntry;
    bool justCreated=false;
    int i;

    GamePageHolder* curHolder=GameState::gameLogic->getHolderById(blockId, chunkId, holderId);

    int dataSizeInFloats;


    if(curHolder==NULL)
    {
        return false;
    }

    if(cacheMetaJS==NULL)
    {
        return false;
    }

    dataSizeInFloats=(int)curHolder->vertexVec.size();

    bool doProc=false;

    if(dataSizeInFloats==0)
    {
        doProc=true;
    }
    else
    {
        doProc=saveFloatArray(
            curCLFull+entryName+".bin",
            &(curHolder->vertexVec[0]),
            (int)curHolder->vertexVec.size()
        );
    }

    if(doProc)
    {

        if(cacheMetaJS->Child("pages")->HasChild(entryName))
        {

        }
        else
        {
            cacheMetaJS->Child("pages")->object_value[entryName]=new JSONValue(JSONArray());
            justCreated=true;
        }
        curEntry=cacheMetaJS->Child("pages")->Child(entryName);

        if(justCreated)
        {
            for(i=0; i<E_CMD_LENGTH; i++)
            {
                curEntry->array_value.push_back(new JSONValue(0.0));
            }
        }

        curEntry->array_value[E_CMD_VERSION]->number_value=cacheVersion;
        curEntry->array_value[E_CMD_SIZEINFLOATS]->number_value=dataSizeInFloats;

        for(i=0; i<NUM_MIP_LEVELS_WITH_FIRST; i++)
        {
            curEntry->array_value[i*2+0]->number_value=curHolder->begMip[i];
            curEntry->array_value[i*2+1]->number_value=curHolder->endMip[i];
        }


        return true;
    }



    return false;

}

bool loadCacheMetaData()
{
    if(loadJSON(curCLFull+"meta.js", &cacheMetaJS))
    {
        std::cout<<"Cache metadata loaded\n";
        return true;
    }
    else
    {
        std::cout<<"Cache metadata not loaded, creating new metadata\n";
        cleanJVPointer(&cacheMetaJS);
        cacheMetaJS=new JSONValue(JSONObject());
        cacheMetaJS->object_value["pages"]=new JSONValue(JSONObject());



        return false;
    }
}

bool saveCacheMetaData()
{
    if(cacheMetaJS==NULL)
    {
        return false;
    }
    if(
        saveFileString(
        curCLFull+"meta.js",
        &(cacheMetaJS->Stringify())
        )
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void clearCache()
{
    // todo: delete all cache files
}

bool updateCurCacheLoc()
{
    curCLFull=curCLBaseDir;
    if(createFolder(curCLFull)) {}
    else { return false; }

    curCLFull+="\\"+curCLWorldDir;
    if(createFolder(curCLFull)) {}
    else { return false; }

    curCLFull+="\\";

    std::cout<<"curCLFull "<<curCLFull<<"\n";

    return true;
}
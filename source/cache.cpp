#include "voxelquest/cache.h"
#include "voxelquest/gamepageholder.h"
#include "voxelquest/gamestate.h"

std::string curCLFull;
std::string curCLBaseDir;
std::string curCLWorldDir;
JSONValue *cacheMetaJS;

bool checkCacheEntry(int blockId, int chunkId, int holderId, bool doErase=false)
{

    if(DO_CACHE)
    {

    }
    else
    {
        return false;
    }

    string entryName="b"+i__s(blockId)+"c"+i__s(chunkId)+"h"+i__s(holderId);
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
    string entryName="b"+i__s(blockId)+"c"+i__s(chunkId)+"h"+i__s(holderId);
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

        curVersion=curEntry->array_value[E_CMD_VERSION]->number_value;
        curDataSizeInFloats=curEntry->array_value[E_CMD_SIZEINFLOATS]->number_value;

        if(curDataSizeInFloats==0)
        {
            return true;
        }

        for(i=0; i<NUM_MIP_LEVELS_WITH_FIRST; i++)
        {
            curHolder->begMip[i]=curEntry->array_value[i*2+0]->number_value;
            curHolder->endMip[i]=curEntry->array_value[i*2+1]->number_value;
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
    string entryName="b"+i__s(blockId)+"c"+i__s(chunkId)+"h"+i__s(holderId);
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

    dataSizeInFloats=curHolder->vertexVec.size();

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
            curHolder->vertexVec.size()
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
        cout<<"Cache metadata loaded\n";
        return true;
    }
    else
    {
        cout<<"Cache metadata not loaded, creating new metadata\n";
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

    cout<<"curCLFull "<<curCLFull<<"\n";

    return true;
}
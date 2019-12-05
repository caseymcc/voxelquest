#define SETTINGS_NOEXTERN
#include "voxelquest/settings.h"

#include "voxelquest/gamestate.h"
#include "voxelquest/gameaudio.h"
#include "voxelquest/gameentmanager.h"
#include "voxelquest/baseobject.h"
#include "voxelquest/fileio.h"

#include <iostream>

Settings::Settings()
{
	settings[E_BS_PLACING_GEOM]=false;
	settings[E_BS_DEBUG_VIEW]=false;
	settings[E_BS_VSYNC]=true;
	settings[E_BS_RENDER_OCT_BOUNDS]=false;
	settings[E_BS_RENDER_VOXELS]=false;
	settings[E_BS_SHOW_GRID]=false;
	settings[E_BS_FOG]=true;
	settings[E_BS_PHSYICS]=true;
	settings[E_BS_FXAA]=false;
	settings[E_BS_WATER_BULLET]=false;
	settings[E_BS_PATH_FINDING]=false;
	settings[E_BS_PATH_FINDING_GEN]=false;
	settings[E_BS_PATH_FINDING_TEST]=false;
	settings[E_BS_TEST_1]=false;
	settings[E_BS_TEST_2]=false;
	settings[E_BS_TEST_3]=false;
	settings[E_BS_WATER]=true;
	settings[E_BS_TREES]=true;
	settings[E_BS_UPDATE_HOLDERS]=true;
	settings[E_BS_PLACING_PATTERN]=false;
	settings[E_BS_DRAW_TARG_PATHS]=false;
	settings[E_BS_UPDATE_FLUID]=false;
	settings[E_BS_RADIOSITY]=true;
	settings[E_BS_DESTROY_TERRAIN]=false;
	settings[E_BS_TURN_BASED]=false;
	settings[E_BS_MIRROR_POSE]=true;
	settings[E_BS_COMBAT]=false;
	settings[E_BS_EDIT_POSE]=false;
	settings[E_BS_SHOW_HEALTH]=false;

    for(int i=0; i<E_SDT_LENGTH; i++)
    {
        externalJSON[E_SPECIAL_DATA_TYPE_STRINGS[i]].jv=NULL;
    }
    externalJSON["kb.js"].jv=NULL;

    gammaVal=0.5f;
    ignoreFrameLimit=false;
    smoothMove=true;

    fxVolume=0.0f;
    masterVolume=0.5f;
    ambientVolume=0.5f;
    guiVolume=0.0f;

	cavesOn=false;

	heightMapMaxInCells=HM_MAX_IN_CELLS;

	voxelsPerCell=VOXELS_PER_CELL;
	paddingInCells=PADDING_IN_CELLS;

	cellsPerHolder=CELLS_PER_HOLDER;
	holdersPerChunk=HOLDERS_PER_CHUNK;
	chunksPerBlock=CHUNKS_PER_BLOCK;
	holdersPerWorld=HOLDERS_PER_WORLD;// newPitch;

	cellsPerNodeXY=16;
	cellsPerNodeZ=8;

	holdersPerBlock=holdersPerChunk*chunksPerBlock;//8 * HOLDER_MOD;
	cellsPerWorld=holdersPerWorld*cellsPerHolder;
	cellsPerBlock=holdersPerBlock*cellsPerHolder;
	blocksPerWorld=holdersPerWorld/holdersPerBlock;
	cellsPerChunk=cellsPerHolder*holdersPerChunk;
	chunksPerWorld=blocksPerWorld*chunksPerBlock;

	cellsPerHolderPad=cellsPerHolder+paddingInCells*2;
	voxelsPerHolderPad=voxelsPerCell*cellsPerHolderPad;
	voxelsPerHolder=voxelsPerCell*cellsPerHolder;

	terDataTexScale=1;

	terDataVisPitchXY=cellsPerBlock/cellsPerNodeXY;
	iNodeDivsPerHolder=terDataVisPitchXY/holdersPerBlock;
	terDataBufAmount=iNodeDivsPerHolder; // pad with one extra lot
	terDataBufPitchXY=terDataVisPitchXY+terDataBufAmount*2;
	terDataBufPitchScaledXY=terDataBufPitchXY*terDataTexScale;

	terDataVisPitchZ=cellsPerBlock/cellsPerNodeZ;
	terDataBufPitchZ=terDataVisPitchZ+terDataBufAmount*2;
	terDataBufPitchScaledZ=terDataBufPitchZ*terDataTexScale;

	terDataVisSize=terDataVisPitchXY*terDataVisPitchXY * terDataVisPitchZ;
	terDataBufSize=terDataBufPitchXY*terDataBufPitchXY * terDataBufPitchZ;
	terDataBufSizeScaled=terDataBufPitchScaledXY*terDataBufPitchScaledXY * terDataBufPitchScaledZ;

	floorHeightInCells=(float)cellsPerNodeZ;
	roofHeightInCells=((float)cellsPerNodeXY)/4.0f;
	wallRadInCells=((float)cellsPerNodeXY)/4.0f;

	guiSaveLoc="..\\data\\saves\\save0\\compMap.txt";

    STEP_TIME_IN_SEC=getConst(E_CONST_STEP_TIME_IN_MICRO_SEC)/1000000.0;
    iNumSteps=16;

    fpsTest=true;
    TRACE_ON=false;
    ND_TRACE_OFF=false;
    TEMP_DEBUG=false;
}

void Settings::speakSetting(int settingName)
{
	std::string speakString=makePretty(E_BOOL_SETTING_STRINGS[settingName], "E_BS_");

	if(settings[settingName])
	{
		speakString+=" on  ";
	}
	else
	{
		speakString+=" off  ";
	}

//	GameAudio::speak(speakString);
}

void Settings::toggleSetting(int settingName, bool withVoice)
{
	settings[settingName]=!(settings[settingName]);
	if(withVoice)
	{
		speakSetting(settingName);
	}
}

void Settings::setSetting(int settingName, bool value, bool withVoice)
{
	settings[settingName]=value;
	if(withVoice)
	{
		speakSetting(settingName);
	}
}

JSONValue* Settings::fetchJSONData(std::string dataFile, bool doClean, JSONValue* params)
{
    bool doLoad=false;
    bool loadRes=false;
    int dataFileEnum;

    if(externalJSON.find(dataFile)==externalJSON.end())
    {
        doLoad=true;
    }
    else
    {
        if(externalJSON[dataFile].jv==NULL)
        {
            doLoad=true;
        }
        else
        {
            if(doClean)
            {
                cleanJVPointer(&(externalJSON[dataFile].jv));
                doLoad=true;
            }
        }
    }

    if(doLoad)
    {
        dataFileEnum=stringToEnum(
            E_SPECIAL_DATA_TYPE_STRINGS,
            E_SDT_LENGTH,
            dataFile
        );

        switch(dataFileEnum)
        {
        case E_SDT_SHADERPARAMS:
            std::cout<<"attempted to load shaderParams\n";
            break;
        case -1:
            std::cout<<"load jv data "+dataFile<<"\n";
            loadRes=loadJSON(
                "..\\data\\"+dataFile,
                &((externalJSON[dataFile]).jv)
            );

            if(loadRes==false)
            {
                return NULL;
            }
            break;
        default:
            getSpecialData(dataFileEnum, dataFile);
            break;
        }




    }

    return (externalJSON[dataFile]).jv;
}


void Settings::getSpecialData(int datEnum, std::string datString)
{
    int i;
    int objectType;
    int childId;

    JSONValue* tempVal0;
    JSONValue* tempVal1;
    BaseObj* curCont;

    cleanJVPointer(&(externalJSON[datString].jv));

    externalJSON[datString].jv=new JSONValue(JSONObject());

    StatSheet* curSS;

    switch(datEnum)
    {
    case E_SDT_OBJECTDATA:
        externalJSON[datString].jv->object_value["objects"]=new JSONValue(JSONArray());
        tempVal0=externalJSON[datString].jv->object_value["objects"];

        for(itBaseObj iterator=GameState::gem->gameObjects.begin(); iterator!=GameState::gem->gameObjects.end(); iterator++)
        {
            // iterator->first = key
            // iterator->second = value

            curCont=&(GameState::gem->gameObjects[iterator->first]);

            if(curCont->isOpen)
            {

                tempVal0->array_value.push_back(new JSONValue(JSONObject()));

                tempVal0->array_value.back()->object_value["children"]=new JSONValue(JSONArray());
                tempVal0->array_value.back()->object_value["objectId"]=new JSONValue(((double)(iterator->first)));
                tempVal1=tempVal0->array_value.back()->Child("children");


                for(i=0; i<curCont->children.size(); i++)
                {
                    tempVal1->array_value.push_back(new JSONValue(JSONObject()));
                    childId=curCont->children[i];
                    objectType=GameState::gem->gameObjects[childId].objectType;
                    tempVal1->array_value.back()->object_value["objectType"]=new JSONValue(((double)(objectType)));
                    tempVal1->array_value.back()->object_value["objectId"]=new JSONValue(((double)(childId)));
                }
            }
        }
        break;
    case E_SDT_STATDATA:

        externalJSON[datString].jv->object_value["stats"]=new JSONValue(JSONArray());
        tempVal0=externalJSON[datString].jv->object_value["stats"];

        if(GameState::gem->getCurActor()==NULL)
        {
            std::cout<<"NULL STATS\n";
            return;
        }
        else
        {

            curSS=&(GameState::gem->getCurActor()->statSheet);

            for(i=0; i<E_CS_LENGTH; i++)
            {
                tempVal0->array_value.push_back(new JSONValue(JSONObject()));

                tempVal0->array_value.back()->object_value["label"]=new JSONValue(makePretty(E_CHAR_STAT_STRINGS[i], "E_CS_"));
                tempVal0->array_value.back()->object_value["value"]=new JSONValue(((double)(curSS->unapplyedStats[i]))/((double)(MAX_STAT_VALUE)));
                tempVal0->array_value.back()->object_value["divisions"]=new JSONValue(((double)(MAX_STAT_VALUE)));


            }
        }
        break;
    case E_SDT_STATUSDATA:

        externalJSON[datString].jv->object_value["status"]=new JSONValue(JSONArray());
        tempVal0=externalJSON[datString].jv->object_value["status"];

        if(GameState::gem->getCurActor()==NULL)
        {
            std::cout<<"NULL STATUS\n";
            return;
        }
        else
        {

            curSS=&(GameState::gem->getCurActor()->statSheet);

            for(i=0; i<E_STATUS_LENGTH; i++)
            {
                tempVal0->array_value.push_back(new JSONValue(JSONObject()));

                tempVal0->array_value.back()->object_value["label"]=new JSONValue(makePretty(E_CHAR_STATUS_STRINGS[i], "E_STATUS_"));
                tempVal0->array_value.back()->object_value["value"]=new JSONValue(((double)(curSS->curStatus[i]))/((double)(curSS->maxStatus[i])));
                tempVal0->array_value.back()->object_value["divisions"]=new JSONValue(((double)(curSS->maxStatus[i])));


            }
        }
        break;
    default:
        std::cout<<"ERROR: unexpected type in getSpecialData(): "<<datEnum<<"\n";
        break;
    }
}

void Settings::saveExternalJSON()
{
    std::cout<<"Saving External JSON Values\n";

    for(itJSStruct iterator=externalJSON.begin(); iterator!=externalJSON.end(); iterator++)
    {

        if(iterator->second.jv!=NULL)
        {
            saveFileString(
                "..\\data\\"+iterator->first,
                &(iterator->second.jv->Stringify())
            );
        }

        // iterator->first = key
        // iterator->second = value
    }

    std::cout<<"End Saving External JSON Values\n";
}

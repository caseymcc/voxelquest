#include "voxelquest/gamegui.h"
#include "voxelquest/jsonhelpers.h"
#include "voxelquest/gamestate.h"
#include "voxelquest/gameentmanager.h"
#include "voxelquest/helperfuncs.h"
#include "voxelquest/renderer.h"
#include "voxelquest/shader.h"
#include "voxelquest/fileio.h"

#include <algorithm>
#include <iostream>

GameGUI::GameGUI()
{
	guiDirty=true;
	maxLayerOver=-1;
}

void GameGUI::init(Singleton* _singleton)
{
    singleton=_singleton;
    isReady=false;
    isLoaded=false;
}

// inline bool compChildStr(string childStr) {
// 	return tempStrings[E_GDS_CHILD_TYPE].compare(childStr) == 0;
// }

JSONValue* GameGUI::findNearestKey(JSONValue* jv, std::string key)
{
    int i;
    int numChildren=0;
    int numFloatingChildren=0;
    JSONValue* jvChildren=NULL;
    JSONValue* jvFloatingChildren=NULL;

    JSONValue* tempJV;

    if(jv->HasChild(key))
    {
        return jv;
    }

    if(jv->HasChild("children"))
    {
        jvChildren=jv->Child("children");
        numChildren=jvChildren->CountChildren();
    }
    for(i=0; i<numChildren; i++)
    {
        tempJV=findNearestKey(jvChildren->Child(i), key);

        if(tempJV!=NULL)
        {
            return tempJV;
        }
    }


    if(jv->HasChild("floatingChildren"))
    {
        jvFloatingChildren=jv->Child("floatingChildren");
        numFloatingChildren=jvFloatingChildren->CountChildren();
    }
    for(i=0; i<numFloatingChildren; i++)
    {
        tempJV=findNearestKey(jvFloatingChildren->Child(i), key);

        if(tempJV!=NULL)
        {
            return tempJV;
        }
    }

    return NULL;

}


void GameGUI::addChildFromJSON(
    int lastIndex,
    JSONValue* jv,
    int curParentId,
    bool isFloating
)
{
    int i;
    int j;
//    int k;
    //int q;
    joi_type iterator;
    //int numEntries;
    int curIcon=0;
    int objectId=0;

    JSONValue* curTempl=NULL;
    JSONValue* tempJV=NULL;




    if(
        jv->HasChild("template")
        )
    {
        if(
            jvTemplates->HasChild(jv->Child("template")->string_value)
            )
        {

            curTempl=jvTemplates->Child(jv->Child("template")->string_value);
        }
        else
        {
            std::cout<<"invalid template \n";
        }
    }


    for(i=0; i<E_GST_LENGTH; i++)
    {
        guiStringValues[i]="";
    }
    for(i=0; i<E_GFT_LENGTH; i++)
    {
        guiFloatValues[i]=0.0f;
    }

    guiFloatValues[E_GFT_HASBACKGROUND]=1.0;
    guiFloatValues[E_GFT_SINGLELINE]=1.0;
    guiStringValues[E_GST_SS]="defaultSS";


    for(j=0; j<2; j++)
    {
        if(curTempl!=NULL)
        {
            for(i=0; i<E_GST_LENGTH; i++)
            {
                if(
                    curTempl->HasChild(guiStringTypes[i])
                    )
                {
                    guiStringValues[i]=curTempl->Child(guiStringTypes[i])->string_value;
                }
            }
            for(i=0; i<E_GFT_LENGTH; i++)
            {
                if(
                    curTempl->HasChild(guiFloatTypes[i])
                    )
                {
                    guiFloatValues[i]=curTempl->Child(guiFloatTypes[i])->number_value;
                }
            }
        }
        curTempl=jv;
    }

    UIComponent* parentPtr=compStack[curParentId].data;

    guiFloatValues[E_GFT_LAYER]=std::max(
        guiFloatValues[E_GFT_LAYER],
        (double)(parentPtr->layer)
    );

    int newParent=parentPtr->addChild(
        lastIndex,
        parentPtr->nodeId,
        guiStringValues,
        guiFloatValues,
        isFloating,
        jv
    );




    int numChildren=0;
    int numFloatingChildren=0;
    int numDataChildren=0;
    int numFilters=0;
    int childType=0;
    JSONValue* jvChildren=NULL;
    JSONValue* jvFloatingChildren=NULL;
    JSONValue* jvChildTemplate=NULL;
    JSONValue* jvFilter=NULL;
    JSONValue* curFilter=NULL;

    JSONValue* jvData=NULL;
    JSONValue* jvDataRoot=NULL;
    JSONValue* curData=NULL;

    //tempStrings[E_GDS_CHILD_TYPE] = "";








    ////////////////



    bool doProc=false;
    //bool isInternal = false;
    int totCount=0;
    int curCT=0;


    if(jv->HasChild("childType"))
    {
        childType=stringToEnum(
            E_GUI_CHILD_TYPE_STRINGS,
            E_GCT_LENGTH,
            jv->Child("childType")->string_value
        );
        //tempStrings[E_GDS_CHILD_TYPE] = jv->Child("childType")->string_value;


        // if (jv->HasChild("isInternal")) {
        // 	if (jv->Child("isInternal")->number_value != 0 ) {
        // 		// use an internally generated JSON node
        // 		isInternal = true;
        // 	}
        // 	else {
        // 		// todo: load JSON file
        // 	}
        // }

        if(jv->HasChild("whereAllEqual"))
        {
            jvFilter=jv->Child("whereAllEqual");
            numFilters=jvFilter->CountChildren();
        }
        else
        {
            jvFilter=NULL;
            numFilters=0;
        }


        if(jv->HasChild("dataSource"))
        {
            tempStrings[E_GDS_DATA_SOURCE]=jv->Child("dataSource")->string_value;

            // if (isInternal) {
            // 	if (jv->HasChild("dataFile")) {
            // 		tempStrings[E_GDS_DATA_FILE] = jv->Child("dataFile")->string_value;
            // 		jvDataRoot = (singleton->internalJSON[tempStrings[E_GDS_DATA_FILE]]).jv;
            // 	}


            // }
            // else {

            if(jv->HasChild("dataFile"))
            {
                tempStrings[E_GDS_DATA_FILE]=jv->Child("dataFile")->string_value;

                if(jv->HasChild("dataParams"))
                {
                    jvDataRoot=fetchJSONData(tempStrings[E_GDS_DATA_FILE], false, jv->Child("dataParams"));
                }
                else
                {
                    jvDataRoot=fetchJSONData(tempStrings[E_GDS_DATA_FILE], false, NULL);
                }
            }
            else
            {
                jvDataRoot=jvRoot;
            }
            //}

            if(jvDataRoot!=NULL)
            {
                getJVNodeByString(jvDataRoot, &jvData, tempStrings[E_GDS_DATA_SOURCE]);

                numDataChildren=jvData->CountChildren();
                if(jv->HasChild("childTemplate"))
                {
                    jvChildTemplate=jv->Child("childTemplate");
                }
            }
        }


        if((jvData!=NULL)&&(jvChildTemplate!=NULL))
        {

            curCT=childType;

            // if (compChildStr("E_GCT_INV_ITEM")) {
            // 	curCT = E_GCT_INV_ITEM;
            // }
            // else if (compChildStr("E_GCT_SHADER_PARAM")) {
            // 	curCT = E_GCT_SHADER_PARAM;
            // }
            // else if (compChildStr("E_GCT_GENERIC")) {
            // 	curCT = E_GCT_GENERIC;
            // }

            //////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////




            for(i=0; i<numDataChildren; i++)
            {

                curData=jvData->Child(i);
                if(curData==NULL)
                {
                    std::cout<<"NULL DATA\n";
                    tempStrings[E_GDS_LAST_KEY]="";
                }
                else
                {
                    tempStrings[E_GDS_LAST_KEY]=jvData->lastKey;
                }


                doProc=true;
                if(jvFilter!=NULL)
                {


                    for(j=0; j<numFilters; j++)
                    {
                        curFilter=jvFilter->Child(j);

                        if(curData->Child(curFilter->Child(0)->string_value)->IsNumber())
                        {
                            if(
                                curData->Child(curFilter->Child(0)->string_value)->number_value!=
                                curFilter->Child(1)->number_value
                                )
                            {
                                doProc=false;
                            }
                        }
                        else
                        {
                            // todo: string support
                        }
                    }


                }

                if(doProc)
                {
                    switch(curCT)
                    {
                    case E_GCT_INV_ITEM:
                        tempStrings[E_GDS_CHILD_NAME]=curData->Child("name")->string_value;

                        curIcon=GameState::gem->entIdToIcon[
                            (int)(jvRoot->
                                Child("itemDefs")->
                                Child(tempStrings[E_GDS_CHILD_NAME])->
                                Child("iconNum")->
                                number_value)
                        ];



                        jvChildTemplate->
                            Child("floatingChildren")->
                            Child(0)->
                            Child("children")->
                            Child(0)->
                            Child("label")->
                            string_value=
                            jvRoot->
                            Child("itemDefs")->
                            Child(tempStrings[E_GDS_CHILD_NAME])->
                            Child("class")->
                            string_value;

                        tempStrings[E_GDS_MATERIAL]=curData->Child("mat")->string_value;
                        if(tempStrings[E_GDS_MATERIAL].compare("none")==0)
                        {
                            tempStrings[E_GDS_MATERIAL]="";
                        }

                        jvChildTemplate->Child("label")->string_value=
                            i__s(curIcon)+
                            "& "+
                            tempStrings[E_GDS_MATERIAL]+
                            " "+
                            tempStrings[E_GDS_CHILD_NAME];
                        break;
                    case E_GCT_SHADER_PARAM:

                        jvChildTemplate->Child("label")->string_value=
                            curData->Child("shaderName")->string_value+
                            "."+
                            curData->Child("paramName")->string_value;

                        jvChildTemplate->Child("uid")->string_value=curData->Child("uid")->string_value;

                        jvChildTemplate->Child("callbackData")->Child("shaderName")->string_value=
                            curData->Child("shaderName")->string_value;
                        jvChildTemplate->Child("callbackData")->Child("paramName")->string_value=
                            curData->Child("paramName")->string_value;

                        break;

                    case E_GCT_GENERIC:

                        splitStrings.clear();
                        splitStrings=split(tempStrings[E_GDS_LAST_KEY], '_');

                        if(jvChildTemplate->HasChild("label"))
                        {

                            switch(splitStrings.size())
                            {
                            case 0:
                            case 1:
                                jvChildTemplate->Child("label")->string_value=tempStrings[E_GDS_LAST_KEY];
                                break;
                            case 2:
                            case 3:
                                jvChildTemplate->Child("label")->string_value=splitStrings[1];
                                break;
                            }
                        }

                        tempJV=findNearestKey(jvChildTemplate, "dataSource");
                        if(tempJV!=NULL)
                        { // is a branch node
                            tempJV->Child("dataSource")->string_value=
                                tempStrings[E_GDS_DATA_SOURCE]+"."+tempStrings[E_GDS_LAST_KEY];
                        }
                        else
                        { // is a leaf node

                            if(jvChildTemplate->HasChild("dataRef"))
                            {
                                jvChildTemplate->Child("dataRef")->string_value=
                                    tempStrings[E_GDS_DATA_SOURCE]+"."+tempStrings[E_GDS_LAST_KEY];
                            }
                            if(jvChildTemplate->HasChild("dataFile"))
                            {
                                jvChildTemplate->Child("dataFile")->string_value=
                                    tempStrings[E_GDS_DATA_FILE];
                            }
                        }



                        break;

                    case E_GCT_CONTAINER:

                        // curIcon = GameState::gem->entIdToIcon[
                        // 	(int)(curData->Child("objectType")->number_value)
                        // ];
                        objectId=(int)curData->Child("objectId")->number_value;
                        jvChildTemplate->Child("objectId")->number_value=objectId;
                        jvChildTemplate->Child("label")->string_value=GameState::gem->getStringForObjectId(objectId);
                        break;

                    case E_GCT_CONTAINER_PARENT:
						tempJV=findNearestKey(jvChildTemplate, "objectId");
                        if(tempJV!=NULL)
                        {
                            tempJV->Child("objectId")->number_value=curData->Child("objectId")->number_value;
                        }

                        tempJV=findNearestKey(jvChildTemplate, "dataSource");
                        if(tempJV!=NULL)
                        {
                            tempJV->Child("dataSource")->string_value="objects."+i__s(totCount)+".children";
                        }
                        break;

                    case E_GCT_AUTOMATIC:
                        //numEntries = curData->CountChildren();
                        // for (q = 0; q < numEntries; q++) {
                        // 	jvChildTemplate->Child("label")->string_value = GameState::gem->getStringForObjectId(objectId);
                        // }

                        for(
                            iterator=curData->object_value.begin();
                            iterator!=curData->object_value.end();
                            iterator++
                            )
                        {
                            // iterator->first = key
                            // iterator->second = value

                            // if (jvChildTemplate->HasChild(iterator->first)) {
                            // 	// todo: report error if key does not exist?
                            // }

                            if(iterator->second->IsNumber())
                            {
                                jvChildTemplate->Child(iterator->first)->number_value=iterator->second->number_value;
                            }
                            else
                            {
                                // todo: support additional JSON types
                                jvChildTemplate->Child(iterator->first)->string_value=iterator->second->string_value;
                            }


                        }



                        break;
                    default:
                        std::cout<<"Error: unexpected GTC type\n";
                        break;
                    }





                    // copy template to new child
                    jv->Child("children")->array_value.push_back(
                        JSON::Parse(jvChildTemplate->Stringify().c_str())
                    );



                    totCount++;


                }



            }
        }

    }




    ////////////////










    if(jv->HasChild("children"))
    {
        jvChildren=jv->Child("children");
        numChildren=jvChildren->CountChildren();
    }
    for(i=0; i<numChildren; i++)
    {
        addChildFromJSON(-1, jvChildren->Child(i), newParent, false);
    }


    if(jv->HasChild("floatingChildren"))
    {
        jvFloatingChildren=jv->Child("floatingChildren");
        numFloatingChildren=jvFloatingChildren->CountChildren();
    }
    for(i=0; i<numFloatingChildren; i++)
    {
        addChildFromJSON(-1, jvFloatingChildren->Child(i), newParent, true);
    }

}


void GameGUI::deleteJSONNodes(JSONValue* jv)
{
    JSONValue* jvChildren=NULL;
    JSONValue* jvFloatingChildren=NULL;

    int numChildren=0;
    int numFloatingChildren=0;

    int i;






    if(jv->HasChild("floatingChildren"))
    {
        jvFloatingChildren=jv->Child("floatingChildren");
        numFloatingChildren=jvFloatingChildren->CountChildren();
    }
    for(i=0; i<numFloatingChildren; i++)
    {
        deleteJSONNodes(jvFloatingChildren->Child(i));
    }

    if(jv->HasChild("children"))
    {
        jvChildren=jv->Child("children");
        numChildren=jvChildren->CountChildren();
    }
    for(i=0; i<numChildren; i++)
    {
        deleteJSONNodes(jvChildren->Child(i));
    }

    if(jv->HasChild("dataSource"))
    {
        jvChildren->array_value.clear();
    }


}


void GameGUI::clearRenderOrder()
{
    int i;
    for(i=0; i<compStack.size(); i++)
    {
        compStack[i].data->layerId=-1;
    }

    for(i=0; i<MAX_UI_LAYERS; i++)
    {
        guiLayers[i].clear();
    }

}

void GameGUI::deleteNodes(UIComponent* curNode)
{
    int i;
    int curNodeId;

    for(i=0; i<curNode->getChildCount(); i++)
    {
        deleteNodes(curNode->getChild(i));
    }

    for(i=0; i<curNode->getFloatingChildCount(); i++)
    {
        deleteNodes(curNode->getFloatingChild(i));
    }

    curNodeId=curNode->nodeId;


    compMap[curNode->uid].nodeId=-1;

    // if (curNode->layerId != -1) {
    // 	singleton->guiLayers[curNode->layer][curNode->layerId] = -1;
    // 	singleton->emptyLayers[curNode->layer].push_back(curNode->layerId);
    // }

    //delete (singleton->compStack[curNodeId].data);
    //singleton->compStack[curNodeId].data = NULL;
    compStack[curNodeId].isValid=false;
    emptyStack.push_back(curNodeId);

    curNode->clearChildren();

}

void GameGUI::refreshNode(UIComponent* oldNode)
{

    if(oldNode==NULL)
    {
        std::cout<<"refreshNode is NULL\n";
    }

    isReady=false;
    isLoaded=false;

    //TEMP_DEBUG = true;


    // TODO: there is a problem with this - templated data must be cleared
    JSONValue* oldJV=oldNode->jvNodeNoTemplate;

    int curParentId=oldNode->parentId;
    int lastIndex=oldNode->index;
    //oldNode->getParent()->getParent()->isDirty = true;
    compStack[0].data->isDirty=true;


    deleteNodes(oldNode);


    deleteJSONNodes(oldJV);


    addChildFromJSON(
        lastIndex,
        oldJV,
        curParentId,
        false
    );

    clearRenderOrder();


    TEMP_DEBUG=false;


    testOver((int)guiX, (int)guiY);
    doRefresh();
    testOver((int)guiX, (int)guiY);
    doRefresh();
    compStack[0].data->updateSS();


    isReady=true;
    isLoaded=true;


}

void GameGUI::guiFromJSON(JSONValue* jv)
{

    int i;

    isLoaded=false;
    isReady=false;

    guiRenderCount=0;


    for(i=0; i<compStack.size(); i++)
    {
        if(compStack[i].data==NULL)
        {

        }
        else
        {
            delete (compStack[i].data);
            compStack[i].data=NULL;
            compStack[i].isValid=false;
        }

    }

    compStack.clear();
    emptyStack.clear();
    for(i=0; i<MAX_UI_LAYERS; i++)
    {
        guiLayers[i].clear();
        //emptyLayers[i].clear();
    }


    compStack.push_back(CompStruct());
    compStack[0].data=new UIComponent;

    for(i=0; i<E_GST_LENGTH; i++)
    {
        stringVals[i]="";
    }
    for(i=0; i<E_GFT_LENGTH; i++)
    {
        floatVals[i]=0.0f;
    }

    stringVals[E_GST_LABEL]="";
    stringVals[E_GST_UID]="";
    stringVals[E_GST_SS]="defaultSS";

    floatVals[E_GFT_TYPE]=E_GT_HOLDER;
    floatVals[E_GFT_DIVISIONS]=0.0f;
    floatVals[E_GFT_HASBACKGROUND]=0.0f;
    floatVals[E_GFT_SINGLELINE]=1.0f;
    floatVals[E_GFT_FILLRATIOX]=0.0f;
    floatVals[E_GFT_FILLRATIOY]=0.0f;
    floatVals[E_GFT_FILLDIR]=E_FILL_HORIZONTAL;
    floatVals[E_GFT_ALIGNX]=E_ALIGNH_LEFT;
    floatVals[E_GFT_ALIGNY]=E_ALIGNV_TOP;
    floatVals[E_GFT_VALUE]=0.0f;
    floatVals[E_GFT_LAYER]=0.0f;
    floatVals[E_GFT_HOVERTYPE]=E_HT_NORMAL;
    floatVals[E_GFT_MAXDIMX]=0.0f;
    floatVals[E_GFT_MAXDIMY]=0.0f;
    floatVals[E_GFT_MINDIMX]=0.0f;
    floatVals[E_GFT_MINDIMY]=0.0f;
    floatVals[E_GFT_FLAGS]=0.0f;

    compStack[0].data->init(
        singleton,
        -1,
        0,
        0,

        NULL,
        false,

        stringVals,
        floatVals
    );

    // singleton->nullComp->init(
    // 	singleton,
    // 	-1,
    // 	0,
    // 	0,

    // 	NULL,
    // 	false,

    // 	stringVals,
    // 	floatVals
    // );



    compStack[0].data->resultDimInPixels.x=(float)guiWinW;
    compStack[0].data->resultDimInPixels.y=(float)guiWinH;







    ///////



    jvRoot=jv;
    jvTemplates=jv->Child("templates");
    jvSounds=jv->Child("sounds");


    addChildFromJSON(
        -1,
        jv->Child("baseGUI"),  //jv->Child("curMenu")->string_value
        0,
        false
    );

    compStack[0].data->isDirty=true;
    isReady=true;
    isLoaded=true;
}



void GameGUI::doRefresh()
{

    int i;

    guiDirty=false;
    dirtyVec.clear();
    compStack[0].data->gatherDirty(&dirtyVec);
    compStack[0].data->clearDirty();

    for(i=0; i<dirtyVec.size(); i++)
    {
        dirtyVec[i]->layout();
    }

    compStack[0].data->renderAll();


}


void GameGUI::testOver(int x, int y)
{
    maxLayerOver=-1;

    int i;

    mouseTrans.x=(float)x;
    mouseTrans.y=(float)y;
    mouseTrans.x/=guiWinW;
    mouseTrans.y/=guiWinH;
    mouseTrans.x=((1.0f-mouseTrans.x)-0.5f)*2.0f;
    mouseTrans.y=((1.0f-mouseTrans.y)-0.5f)*2.0f;

    for(i=0; i<compStack.size(); i++)
    {
        compStack[i].data->overSelf=false;
    }
    //singleton->compStack[0].data->clearOver();
    compStack[0].data->findMaxLayer((float)x, (float)y, mouseTrans.x, mouseTrans.y);
    compStack[0].data->testOver((float)x, (float)y);
}

bool GameGUI::testHit(int button, int state, int x, int y)
{
    return compStack[0].data->testHit(button, state, (float)x, (float)y);
}


UIComponent* GameGUI::findNodeByString(std::string _uid)
{
    return compStack[0].data->findNodeByString(_uid);
}

void GameGUI::renderCharAt(
    UIComponent* uiComp,
    CharStruct* cs,
    FontWrapper* activeFont,
    float px,
    float py,
    float shadowOffset
)
{

    StyleSheetResult* resSS=&(uiComp->resSS);

    float sampX=cs->sampX;
    float sampY=cs->sampY;
    float sampW=cs->sampW;
    float sampH=cs->sampH;
    float offsetX=(cs->offsetX)*activeFont->fontScale;
    float offsetY=((activeFont->fontHeight-cs->offsetY)+activeFont->descender)*activeFont->fontScale;
    float sourceW=(float)activeFont->fontImage->width;
    float sourceH=(float)activeFont->fontImage->height;

    fBoundingBox destPos;
    fBoundingBox srcPos;



    destPos.xMin=(px+offsetX)+uiComp->totOffset.x;
    destPos.yMin=(py+offsetY)+uiComp->totOffset.y+shadowOffset;
    destPos.xMax=(px+offsetX+sampW*activeFont->fontScale)+uiComp->totOffset.x;
    destPos.yMax=(py+offsetY+sampH*activeFont->fontScale)+uiComp->totOffset.y+shadowOffset;

    srcPos.xMin=(sampX)/sourceW;
    srcPos.yMin=(sourceH-(sampY+sampH))/sourceH;
    srcPos.xMax=(sampX+sampW)/sourceW;
    srcPos.yMax=(sourceH-sampY)/sourceH;



    float x0=destPos.xMin/guiWinW;
    float x1=destPos.xMax/guiWinW;
    float y0=destPos.yMin/guiWinH;
    float y1=destPos.yMax/guiWinH;

    x0=(x0-0.5f)*2.0f;
    x1=(x1-0.5f)*2.0f;
    y0=((1.0f-y0)-0.5f)*2.0f;
    y1=((1.0f-y1)-0.5f)*2.0f;

    float iconVal=0.0;
    if(activeFont->isIcons)
    {
        iconVal=1.0f;
    }




    //dimensions
    glMultiTexCoord4f((GLenum)1, sampW, sampH, 0.0f, 0.0f);

    glMultiTexCoord4f((GLenum)4, 1.0f, 1.0f, 1.0f, 1.0f);
    glMultiTexCoord4f((GLenum)5, iconVal, shadowOffset, uiComp->scrollMaskY.x, uiComp->scrollMaskY.y);
    //border color
    glMultiTexCoord4f((GLenum)6, 1.0f, 1.0f, 1.0f, 1.0f);
    //misc
    glMultiTexCoord4f((GLenum)7, 0.0f, 0.0f, 0.0f, 0.0f);


    glMultiTexCoord4f((GLenum)2, resSS->props[E_SS_BGCOLTEXT1_R], resSS->props[E_SS_BGCOLTEXT1_G], resSS->props[E_SS_BGCOLTEXT1_B], resSS->props[E_SS_BGCOLTEXT1_A]);
    glMultiTexCoord4f((GLenum)3, resSS->props[E_SS_FGCOLTEXT1_R], resSS->props[E_SS_FGCOLTEXT1_G], resSS->props[E_SS_FGCOLTEXT1_B], resSS->props[E_SS_FGCOLTEXT1_A]);


    glMultiTexCoord4f((GLenum)0, srcPos.xMin, srcPos.yMin, 0.0f, 1.0f);
    glVertex3f(x0, y1, -1.0f);
    glMultiTexCoord4f((GLenum)0, srcPos.xMax, srcPos.yMin, 1.0f, 1.0f);
    glVertex3f(x1, y1, -1.0f);

    glMultiTexCoord4f((GLenum)2, resSS->props[E_SS_BGCOLTEXT0_R], resSS->props[E_SS_BGCOLTEXT0_G], resSS->props[E_SS_BGCOLTEXT0_B], resSS->props[E_SS_BGCOLTEXT0_A]);
    glMultiTexCoord4f((GLenum)3, resSS->props[E_SS_FGCOLTEXT0_R], resSS->props[E_SS_FGCOLTEXT0_G], resSS->props[E_SS_FGCOLTEXT0_B], resSS->props[E_SS_FGCOLTEXT0_A]);

    glMultiTexCoord4f((GLenum)0, srcPos.xMax, srcPos.yMax, 1.0f, 0.0f);
    glVertex3f(x1, y0, -1.0f);
    glMultiTexCoord4f((GLenum)0, srcPos.xMin, srcPos.yMax, 0.0f, 0.0f);
    glVertex3f(x0, y0, -1.0f);

}

void GameGUI::renderQuad(
    UIComponent* uiComp,
    fBoundingBox fbb,
    float shadowOffset
)
{

    StyleSheetResult* resSS=&(uiComp->resSS);

    float fMatCode=(float)uiComp->matCode;
    bool isHSL=uiComp->matCode==E_MC_HSV;


    float selMod=0.0f;
    float selMod2=1.0f;



    float x0=(fbb.xMin+uiComp->totOffset.x)/guiWinW;
    float x1=(fbb.xMax+uiComp->totOffset.x)/guiWinW;
    float y0=(fbb.yMin+uiComp->totOffset.y+shadowOffset)/guiWinH;
    float y1=(fbb.yMax+uiComp->totOffset.y+shadowOffset)/guiWinH;

    x0=(x0-0.5f)*2.0f;
    x1=(x1-0.5f)*2.0f;
    y0=((1.0f-y0)-0.5f)*2.0f;
    y1=((1.0f-y1)-0.5f)*2.0f;


    //dimensions
    glMultiTexCoord4f((GLenum)1, fbb.xMax-fbb.xMin, fbb.yMax-fbb.yMin, resSS->props[E_SS_BORDER], resSS->props[E_SS_CORNERRAD]);

    glMultiTexCoord4f((GLenum)5, 0.0f, shadowOffset, uiComp->scrollMaskY.x, uiComp->scrollMaskY.y);


    // todo: fix this to use style sheet
    if(uiComp->selected)
    {
        selMod=0.5f;
        selMod2=0.5f;
    }

    //border color
    glMultiTexCoord4f(
		(GLenum)6,
        resSS->props[E_SS_BDCOL_R]*selMod2+selMod,
        resSS->props[E_SS_BDCOL_G]*selMod2+selMod,
        resSS->props[E_SS_BDCOL_B]*selMod2,
        resSS->props[E_SS_BDCOL_A]*selMod2+selMod
    );



    //misc
    glMultiTexCoord4f((GLenum)7, uiComp->getValue(), uiComp->getValueY(), resSS->props[E_SS_ROUNDNESS], fMatCode);

    if(isHSL)
    { // bg with hsv

        glMultiTexCoord4f(
			(GLenum)2,
            mixf(uiComp->getValueIndexPtr(0), -1.0f, uiComp->valVecMask[0]),
            mixf(uiComp->getValueIndexPtr(1), -1.0f, uiComp->valVecMask[1]),
            mixf(uiComp->getValueIndexPtr(2), -1.0f, uiComp->valVecMask[2]),
            1.0f
        );


    }

    if(!isHSL)
    {
        //bg
        glMultiTexCoord4f((GLenum)2, resSS->props[E_SS_BGCOL1_R], resSS->props[E_SS_BGCOL1_G], resSS->props[E_SS_BGCOL1_B], resSS->props[E_SS_BGCOL1_A]);
    }
    //fg
    glMultiTexCoord4f((GLenum)3, resSS->props[E_SS_FGCOL1_R], resSS->props[E_SS_FGCOL1_G], resSS->props[E_SS_FGCOL1_B], resSS->props[E_SS_FGCOL1_A]);
    //tg
    glMultiTexCoord4f((GLenum)4, resSS->props[E_SS_TGCOL1_R], resSS->props[E_SS_TGCOL1_G], resSS->props[E_SS_TGCOL1_B], resSS->props[E_SS_TGCOL1_A]);




    glMultiTexCoord4f((GLenum)0, 0.0f, 0.0f, 0.0f, 1.0f);
    glVertex3f(x0, y1, -1.0f);
    glMultiTexCoord4f((GLenum)0, 0.0f, 0.0f, 1.0f, 1.0f);
    glVertex3f(x1, y1, -1.0f);

    if(!isHSL)
    {
        //bg
        glMultiTexCoord4f((GLenum)2, resSS->props[E_SS_BGCOL0_R], resSS->props[E_SS_BGCOL0_G], resSS->props[E_SS_BGCOL0_B], resSS->props[E_SS_BGCOL0_A]);
    }
    //fg
    glMultiTexCoord4f((GLenum)3, resSS->props[E_SS_FGCOL0_R], resSS->props[E_SS_FGCOL0_G], resSS->props[E_SS_FGCOL0_B], resSS->props[E_SS_FGCOL0_A]);
    //tg
    glMultiTexCoord4f((GLenum)4, resSS->props[E_SS_TGCOL0_R], resSS->props[E_SS_TGCOL0_G], resSS->props[E_SS_TGCOL0_B], resSS->props[E_SS_TGCOL0_A]);

    glMultiTexCoord4f((GLenum)0, 0.0f, 0.0f, 1.0f, 0.0f);
    glVertex3f(x1, y0, -1.0f);
    glMultiTexCoord4f((GLenum)0, 0.0f, 0.0f, 0.0f, 0.0f);
    glVertex3f(x0, y0, -1.0f);


}


void GameGUI::renderQuadDirect(
    UIComponent* uiComp
)
{

    if(uiComp==NULL)
    {
        return;
    }
    if(uiComp->visible && uiComp->enabled)
    {

    }
    else
    {
        return;
    }



    float x0=(uiComp->hitBounds.xMin+uiComp->totOffset.x)/guiWinW;
    float x1=(uiComp->hitBounds.xMax+uiComp->totOffset.x)/guiWinW;
    float y0=(uiComp->hitBounds.yMin+uiComp->totOffset.y)/guiWinH;
    float y1=(uiComp->hitBounds.yMax+uiComp->totOffset.y)/guiWinH;

    x0=(x0-0.5f)*2.0f;
    x1=(x1-0.5f)*2.0f;
    y0=((1.0f-y0)-0.5f)*2.0f;
    y1=((1.0f-y1)-0.5f)*2.0f;

    // glBegin(GL_QUADS);

    // glTexCoord2f(0.0f, 1.0f);
    // glVertex3f (  x0, y1, -1.0f );
    // glTexCoord2f(1.0f, 1.0f);
    // glVertex3f (  x1, y1, -1.0f );
    // glTexCoord2f(1.0f, 0.0f);
    // glVertex3f (  x1, y0, -1.0f );
    // glTexCoord2f(0.0f, 0.0f);
    // glVertex3f (  x0, y0, -1.0f );

    // glEnd();

    Renderer::curShaderPtr->setShaderVec4("blitCoords", x0, y0, x1, y1);
//    singleton->fsQuad.draw();
	Renderer::drawFSQuad();

}


void GameGUI::runReport()
{
    compStack[0].data->runReport();
}

void GameGUI::renderGUI()
{


    int i;

    int j;
    int k;
    int m;
    int n;

    int maxLoop=0;
    float shadowOffset=0.0;
    UICont* curCont=NULL;
    UIComponent* curComp=NULL;

    testOver((int)guiX, (int)guiY);
    doRefresh();
    compStack[0].data->updateSS();

    guiRenderCount++;

    if(guiRenderCount<5)
    {
        return;
    }

    for(i=0; i<2; i++)
    {


        if(i==0)
        {
            maxLoop=1;
        }
        else
        {
            maxLoop=2;
        }

        Renderer::setShaderFloat("passNum", (float)i);
		Renderer::setShaderVec2("resolution", (float)Renderer::currentFBOResolutionX, (float)Renderer::currentFBOResolutionY);

        glBegin(GL_QUADS);

        for(j=0; j<MAX_UI_LAYERS; j++)
        {
            for(k=0; k<guiLayers[j].size(); k++)
            {



                if(guiLayers[j][k]>=0)
                {

                    curComp=compStack[guiLayers[j][k]].data;
                    curCont=&(curComp->uiCont);

                    if(curComp->visible  && curComp->enabled&&(curComp->layerId>=0))
                    {

                        for(m=0; m<maxLoop; m++)
                        {

                            shadowOffset=((1-m)*i)*2.0f;


                            // only shadow text
                            if(shadowOffset==0.0f)
                            {


                                if(curCont->bg.fontId>-1)
                                {
                                    renderQuad(
                                        curComp,
                                        curCont->bg.hitBounds,
                                        shadowOffset
                                    );
                                }


                            }

                            if(i==0)
                            {
                                // don't render text in first pass
                            }
                            else
                            {

                                if(false)
                                { //curCont->locked) {
                        // busy updating characters
                                }
                                else
                                {
                                    for(n=0; n<curCont->charVec.size(); n++)
                                    {
                                        renderCharAt(
                                            curComp,
                                            curCont->charVec[n].cs,
                                            fontWrappers[curCont->charVec[n].fontId],
                                            curCont->charVec[n].hitBounds.xMin,
                                            curCont->charVec[n].hitBounds.yMin,
                                            shadowOffset
                                        );
                                    }
                                }
                            }
                        }
                    }




                }



            }
        }

        glEnd();
    }
}

void GameGUI::updateStatGUI()
{
	UIComponent* tempComp;


	if(GameState::gem->getCurActor()==NULL)
	{
		return;
	}

	StatSheet* curSS=&(GameState::gem->getCurActor()->statSheet);

	tempComp=getGUIComp("statMenu.availPoints");
	tempComp->setValue(
		((float)curSS->availPoints)/((float)(tempComp->divisions))
	);



}

void GameGUI::updateStatusHUD()
{
	int i;

	if(GameState::gem->getCurActor()==NULL)
	{
		return;
	}
	if(menuList[E_FM_HUDMENU]==NULL)
	{
		return;
	}
	if(menuList[E_FM_HUDMENU]->visible)
	{

	}
	else
	{
		return;
	}

	StatSheet* curStatSheet=&(GameState::gem->getCurActor()->statSheet);

	UIComponent* tempComp=getGUIComp("hudMenu.statContainer");
	UIComponent* childComp;

	if(tempComp==NULL)
	{
		return;
	}

	float v1;
	float v2;

	for(i=0; i<E_STATUS_LENGTH; i++)
	{
		childComp=tempComp->getChild(i);

		v1=(float)curStatSheet->curStatus[i];
		v2=(float)curStatSheet->maxStatus[i];

		childComp->setValue(v1/v2);
	}
}


void GameGUI::showHudMenu(bool visible)
{
	if(menuList[E_FM_HUDMENU]!=NULL)
	{
		menuList[E_FM_HUDMENU]->visible=visible;

		externalJSON.erase("E_SDT_STATUSDATA"); // mem leak?

		refreshNode(
			findNodeByString("hudMenu.hudContainer")
		);

		if(visible)
		{

			updateStatGUI();

		}


	}
}

void GameGUI::showStatMenu(bool visible)
{


	std::cout<<"refreshStats\n";

	if(menuList[E_FM_STATMENU]!=NULL)
	{
		menuList[E_FM_STATMENU]->visible=visible;
		externalJSON.erase("E_SDT_STATDATA"); // mem leak?
		externalJSON.erase("E_SDT_STATUSDATA"); // mem leak?

		refreshNode(
			findNodeByString("statMenu.statContainer")
		);

		if(visible)
		{

			updateStatGUI();

		}


	}

}


void GameGUI::refreshContainers(bool onMousePos)
{
	UIComponent* objCont=NULL;

	bool oldVis=false;

	if(menuList[E_FM_CONTMENU]!=NULL)
	{

		std::cout<<"refreshContainers\n";

		externalJSON.erase("E_SDT_OBJECTDATA"); // mem leak?



		oldVis=menuList[E_FM_CONTMENU]->visible;
		menuList[E_FM_CONTMENU]->visible=GameState::gem->anyContainerOpen();

		objCont=findNodeByString("objectContainer");
		//objCont->jvNodeNoTemplate->Child("dataParams")->number_value = contIndex;

		refreshNode(objCont);

		if(onMousePos&&(oldVis==false))
		{

			// menuList[E_FM_CONTMENU]->dragOffset.x = 0.0f;
			// menuList[E_FM_CONTMENU]->dragOffset.y = 0.0f;
			contMenuBar=menuList[E_FM_CONTMENU]->getChild(0)->getChild(0);

			contMenuBar->lastDrag.x=(guiX);
			contMenuBar->lastDrag.y=std::min((float)(guiY), (float)((guiWinH-menuList[E_FM_CONTMENU]->getChild(0)->resultDimInPixels.y)));
			contMenuBar->forceDragUpdate=true;
		}

	}
}

void GameGUI::setGUIText(
	std::string key,
	std::string stringValue,
	float floatValue,
	bool applyVal,
	bool applyString
)
{
	UICStruct* curComp;
	if(compMap.find(key)==compMap.end())
	{
		// invalid key
	}
	else
	{
		curComp=&(compMap[key]);

		if(curComp->nodeId<0)
		{
			// component was deleted
		}
		else
		{
			if(applyString)
			{
				compStack[curComp->nodeId].data->setText(stringValue);
			}
			if(applyVal)
			{
				compStack[curComp->nodeId].data->setValue(floatValue);
			}

		}
	}
}

float GameGUI::getGUIValue(std::string key)
{
	UICStruct* curComp;
	if(compMap.find(key)==compMap.end())
	{
		// invalid key
	}
	else
	{
		curComp=&(compMap[key]);

		if(curComp->nodeId<0)
		{
			// component was deleted
		}
		else
		{
			return compStack[curComp->nodeId].data->getValue();
		}
	}

	return 0.0;
}

UIComponent* GameGUI::getGUIComp(std::string key)
{
	UICStruct* curComp;
	if(compMap.find(key)==compMap.end())
	{
		// invalid key
	}
	else
	{
		curComp=&(compMap[key]);

		if(curComp->nodeId<0)
		{
			// component was deleted
		}
		else
		{
			return compStack[curComp->nodeId].data;
		}
	}

	return NULL;
}

void GameGUI::setGUIValue(
	std::string key,
	float floatValue,
	bool dispatchEvent,
	bool preventRefresh
)
{
	UICStruct* curComp;

	if(compMap.find(key)==compMap.end())
	{
		// invalid key
	}
	else
	{
		curComp=&(compMap[key]);

		if(curComp->nodeId<0)
		{
			// component was deleted
		}
		else
		{
			compStack[curComp->nodeId].data->setValue(floatValue, dispatchEvent, preventRefresh);
		}
	}
}


void GameGUI::loadValuesGUI(bool applyValues)
{
	std::cout<<"Loading GUI Values\n";

	int i;

	charArr dest;
	dest.data=NULL;
	dest.size=0;

//	UICStruct* curComp;

	std::string loadBuf;
	//vector<string> splitStrings;


	if(loadFile(g_settings.guiSaveLoc, &dest))
	{
		loadBuf=std::string(dest.data);

		splitStrings.clear();
		splitStrings=split(loadBuf, '^');

		for(i=0; i<splitStrings.size(); i+=2)
		{

			setGUIValue(
				splitStrings[i],
				hexToFloat(&(splitStrings[i+1])),
				true,
				true
			);

			if(applyValues)
			{

			}
			else
			{

			}

		}

	}
	else
	{
		std::cout<<"Unable to load GUI Values\n";
	}

	if(dest.data!=NULL)
	{
		delete[] dest.data;
		dest.data=NULL;
	}

	std::cout<<"End Loading GUI Values\n";
}


void GameGUI::saveExternalJSON()
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


void GameGUI::saveGUIValues()
{
	std::cout<<"Saving GUI Values\n";

	std::string stringBuf="";

	for(itUICStruct iterator=compMap.begin(); iterator!=compMap.end(); iterator++)
	{

		if(iterator->second.nodeId<0)
		{

		}
		else
		{
			if(iterator->first[0]=='$')
			{ // values with $ are saved
				stringBuf.append(
					iterator->first+"^"+floatToHex(compStack[iterator->second.nodeId].data->getValue())+"^"
				);
			}


		}

		// iterator->first = key
		// iterator->second = value
	}

	saveFileString(g_settings.guiSaveLoc, &stringBuf);

	std::cout<<"End Saving GUI Values\n";
}


void GameGUI::updateGUI()
{

	float milVox=(
		((float)(GameState::TOT_POINT_COUNT))/1000000.0f
		);

	int mvPerPage=1;

	float voxelsGen=(float)(PAGE_COUNT*mvPerPage);

	std::string maxGPUMString=" / "+fi__s(GameState::MAX_GPU_MEM);

	float totUsage=GameState::TOT_GPU_MEM_USAGE+GameState::VERTEX_MEM_USAGE;

	// if (frameCount%120 == 0) {

	setGUIText("debug.fbMem", "Frame Buffer Mem Used: "+fi__s(GameState::TOT_GPU_MEM_USAGE)+maxGPUMString, GameState::TOT_GPU_MEM_USAGE/GameState::MAX_GPU_MEM, true);
	setGUIText("debug.vertMem", "Vert Mem Used: "+fi__s(GameState::VERTEX_MEM_USAGE)+maxGPUMString, GameState::VERTEX_MEM_USAGE/GameState::MAX_GPU_MEM, true);
	setGUIText("debug.totMem", "Total Mem Used: "+fi__s(totUsage)+maxGPUMString, totUsage/GameState::MAX_GPU_MEM, true);
	setGUIText("debug.numVoxels", "Voxels Generated (In Millions!): "+f__s(milVox));

	// }

}

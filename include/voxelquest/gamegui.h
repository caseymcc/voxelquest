#ifndef _voxelquest_gamegui_h_
#define _voxelquest_gamegui_h_

#include "voxelquest/vectors.h"
#include "voxelquest/uicomponent.h"

class Singleton;
class JSONValue;


#define E_FLOATING_MENU(DDD) \
DDD(E_FM_MAINMENU) \
DDD(E_FM_DDMENU) \
DDD(E_FM_CONTMENU) \
DDD(E_FM_FIELDMENU) \
DDD(E_FM_STATMENU) \
DDD(E_FM_HUDMENU) \
DDD(E_FM_LENGTH)

std::string E_FLOATING_MENU_STRINGS[]={
	E_FLOATING_MENU(DO_DESCRIPTION)
};

enum E_FLOATING_MENU_VALS
{
	E_FLOATING_MENU(DO_ENUM)
};

enum E_FONT_WRAPPERS
{
	EFW_TEXT,
	EFW_ICONS,
	EFW_LENGTH
};

struct JSONStruct
{
	JSONValue* jv;
};

class GameGUI {
public:
    GameGUI();

    void init(Singleton* _singleton);

    JSONValue* findNearestKey(JSONValue* jv, std::string key);

    void addChildFromJSON(
        int lastIndex,
        JSONValue* jv,
        int curParentId,
        bool isFloating=false
    );

    void deleteJSONNodes(JSONValue* jv);

    void clearRenderOrder();

    void deleteNodes(UIComponent* curNode);

    void refreshNode(UIComponent* oldNode);

    void guiFromJSON(JSONValue* jv);

	void doRefresh();

    void testOver(int x, int y);

    bool testHit(int button, int state, int x, int y);

	UIComponent* findNodeByString(std::string _uid);

    void renderCharAt(
        UIComponent* uiComp,
        CharStruct* cs,
        FontWrapper* activeFont,
        float px,
        float py,
        float shadowOffset
    );

    void renderQuad(
        UIComponent* uiComp,
        fBoundingBox fbb,
        float shadowOffset
    );


    void renderQuadDirect(
        UIComponent* uiComp
    );


    void runReport();

    void renderGUI();

	void refreshContainers(bool onMousePos);

//private:
	Singleton* singleton;

	//Image* textImage;
	//GLuint guiTextureId;

	UIComponent* mapComp;

	UIComponent* menuList[E_FM_LENGTH];

	UIComponent* contMenuBar;
	UIComponent* ddMenuBar;
	UIComponent* fieldText;

	FontWrapper* fontWrappers[EFW_LENGTH];

	//    GameMusic* music[EML_LENGTH];
	//    map<string, GameSound> soundMap;

	std::map<std::string, StyleSheet> styleSheetMap;
	std::map<std::string, JSONStruct> externalJSON;


	//Shader shaderGUI;
	JSONValue* jvRoot;
	JSONValue* jvTemplates;
	JSONValue* jvSounds;
	
	bool isReady;
	bool isLoaded;
	
	fVector2 mouseTrans;
	
	int guiRenderCount;
	std::vector<UIComponent*> dirtyVec;
	
	std::string tempStrings[10];
	
	std::string stringVals[E_GST_LENGTH];
	double floatVals[E_GFT_LENGTH];

	FIVector4 lastCellPos;
};


#endif//_voxelquest__h_3+
#ifndef _voxelquest_gamegui_h_
#define _voxelquest_gamegui_h_

#include "voxelquest/vectors.h"
#include "voxelquest/uicomponent.h"

class Singleton;
class JSONValue;

const static int MAX_UI_LAYERS=4;

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

#define E_GUI_CHILD_TYPE(DDD) \
DDD(E_GCT_INV_ITEM) \
DDD(E_GCT_SHADER_PARAM) \
DDD(E_GCT_GENERIC) \
DDD(E_GCT_CONTAINER) \
DDD(E_GCT_CONTAINER_PARENT) \
DDD(E_GCT_AUTOMATIC) \
DDD(E_GCT_LENGTH)

std::string E_GUI_CHILD_TYPE_STRINGS[]={
    E_GUI_CHILD_TYPE(DO_DESCRIPTION)
};

enum E_GUI_CHILD_TYPE_VALS
{
    E_GUI_CHILD_TYPE(DO_ENUM)
};

enum E_FONT_WRAPPERS
{
    EFW_TEXT,
    EFW_ICONS,
    EFW_LENGTH
};

enum E_GUI_DATA_STRINGS
{
    E_GDS_DATA_SOURCE,
    E_GDS_DATA_FILE,
    //E_GDS_CHILD_TYPE,
    E_GDS_CHILD_NAME,
    E_GDS_MATERIAL,
    E_GDS_LAST_KEY,
    E_GDS_LENGTH
};

enum class MouseButton
{
    Left,
    Right,
    Middle
};

enum class MouseState
{
    Up,
    Down
};

struct PixData
{
    FIVector4 pd[3];
};

struct CompStruct
{
    bool isValid;
    UIComponent* data;
};

struct UICStruct
{
    int nodeId;
};

class GameGUI
{
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

    void updateStatGUI();

    void updateStatusHUD();

    void showHudMenu(bool visible);

    void showStatMenu(bool visible);

    void refreshContainers(bool onMousePos);

    void setGUIText(
        std::string key,
        std::string stringValue,
        float floatValue=0.0f,
        bool applyVal=false,
        bool applyString=true
    );

    float getGUIValue(std::string key);

    UIComponent* getGUIComp(std::string key);

    void setGUIValue(
        std::string key,
        float floatValue,
        bool dispatchEvent=false,
        bool preventRefresh=false
    );

    void loadValuesGUI(bool applyValues=false);

    //	void saveExternalJSON();

    void saveGUIValues();

    void updateGUI();

    int placeInStack();

    int placeInLayer(int nodeId, int layer);

    //private:
    Singleton* singleton;

    typedef std::map<std::string, UICStruct>::iterator itUICStruct;
    std::map<std::string, UICStruct> compMap;
    std::vector<CompStruct> compStack;
    std::vector<int> emptyStack;
    std::vector<int> guiLayers[MAX_UI_LAYERS];
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

    //	std::map<std::string, JSONStruct> externalJSON;


        //Shader shaderGUI;
    JSONValue* jvRoot;
    JSONValue* jvTemplates;
    JSONValue* jvSounds;

    bool guiDirty;
    bool isReady;
    bool isLoaded;
    bool dragging;

    int maxLayerOver;

    fVector2 mouseTrans;

    int guiRenderCount;
    std::vector<UIComponent*> dirtyVec;

    std::string tempStrings[10];

    std::string stringVals[E_GST_LENGTH];
    double floatVals[E_GFT_LENGTH];

    float guiX;
    float guiY;
    int guiWinW;
    int guiWinH;

    FIVector4 lastCellPos;

    bool lbDown;
    bool abDown; // lb, rb, or mb is down
    bool rbDown;
    bool mbDown;

    bool bShiftOld;
    bool bCtrlOld;
    bool bCtrl;
    bool bShift;

    PixData spaceUpPixData;
    PixData mouseUpPixData;
    PixData mouseDownPixData;
    PixData mouseMovePixData;

    FIVector4 mouseStart;
    FIVector4 mouseEnd;
    FIVector4 mouseMoveVec;
    FIVector4 *mouseMoving;

    int PAGE_COUNT;
};


#endif//_voxelquest__h_3+
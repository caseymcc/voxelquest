#ifndef _voxelquest_uicomponent_h_
#define _voxelquest_uicomponent_h_

#include "voxelquest/fontwrapper.h"
#include "voxelquest/stylesheet.h"

class Singleton;

// do not reorder!
enum GUI_TYPES
{
	E_GT_HOLDER,  // 0
	E_GT_SLIDER,  // 1 also a toggle if 1 division
	E_GT_BUTTON,  // 2
	E_GT_RADIO,   // 3
	E_GT_MENUBAR, // 4
	E_GT_DRAGPAD, // 5
	E_GT_CHECK,   // 6
	E_GT_LENGTH
};

enum GUI_STRING_TYPES
{
	E_GST_LABEL,
	E_GST_UID,
	E_GST_SS,
	E_GST_DATAREF,
	E_GST_DATAFILE,
	E_GST_DATAKEY,
	E_GST_VALREF,
	E_GST_LENGTH
};

char* guiStringTypes[]={
	"label",
	"uid",
	"ss",
	"dataRef",
	"dataFile",
	"dataKey",
	"valRef"
};

enum E_HOVER_TYPES
{
	E_HT_NORMAL,		// 0
	E_HT_TOOLTIP,		// 1
	E_HT_ONSELECTED,	// 2
	E_HT_TOOLTIP_VALUE, // 3
	E_HT_ROOT,			// 4
	E_HT_LENGTH
};

enum GUI_FLOAT_TYPES
{
	E_GFT_TYPE,
	E_GFT_DIVISIONS,
	E_GFT_VALUE,
	E_GFT_HASBACKGROUND,
	E_GFT_SINGLELINE,
	E_GFT_FILLRATIOX,
	E_GFT_FILLRATIOY,
	E_GFT_FILLDIR,
	E_GFT_ALIGNX,
	E_GFT_ALIGNY,
	E_GFT_LAYER,
	E_GFT_HOVERTYPE, // 0: normal, 1: tooltip, 2: show when parent is selected
	E_GFT_MAXDIMX,
	E_GFT_MAXDIMY,
	E_GFT_MINDIMX,
	E_GFT_MINDIMY,
	E_GFT_FLAGS,
	E_GFT_VALUE0,
	E_GFT_VALUE1,
	E_GFT_VALUE2,
	E_GFT_VALUE3,
	E_GFT_MATCODE,
	E_GFT_LENGTH
};
char* guiFloatTypes[]={
	"type",
	"divisions",
	"value",
	"hasBackground",
	"singleLine",
	"fillRatioX",
	"fillRatioY",
	"fillDir",
	"alignX",
	"alignY",
	"layer",
	"hoverType",
	"maxDimX",
	"maxDimY",
	"minDimX",
	"minDimY",
	"flags",
	"value0",
	"value1",
	"value2",
	"value3",
	"matCode"
};

enum E_GUI_FLAGS
{
	E_GF_X=1,
	E_GF_Y=2,
	E_GF_Z=4,
	E_GF_W=8,
	E_GF_MAT=16
};

std::string guiStringValues[E_GST_LENGTH];
double guiFloatValues[E_GFT_LENGTH];

enum eAlignH
{
	E_ALIGNH_LEFT,
	E_ALIGNH_CENTER,
	E_ALIGNH_RIGHT,
};

enum eAlignV
{
	E_ALIGNV_TOP,
	E_ALIGNV_MIDDLE,
	E_ALIGNV_BOTTOM
};

enum eFillDir
{
	E_FILL_HORIZONTAL,
	E_FILL_VERTICAL
};

enum E_MAT_CODE
{
	E_MC_DEFAULT,
	E_MC_HSV,
	E_MC_MATERIAL,
	E_MC_LENGTH
};

struct iBoundingBox
{
	int xMin;
	int yMin;
	int xMax;
	int yMax;
};

struct fBoundingBox
{
	float xMin;
	float yMin;
	float xMax;
	float yMax;
};

struct UIQuad
{
	fBoundingBox hitBounds;
	CharStruct* cs;
	int fontId;
};

struct UICont
{
	//UIComponent* uiComp;
	UIQuad bg;
	std::vector<UIQuad> charVec;
	//bool locked;
};

class UIComponent
{
public:
    UIComponent();

    void init(
		Singleton* _singleton,

        int _parentId,
        int _nodeId,
        int _index,

        JSONValue* _jvNodeNoTemplate, // original node without template applied to it
        bool _isFloating,

        std::string* stringVals,
        double* floatVals

    );

    void clearChildren();

    UIComponent* getChild(int ind);
    UIComponent* getFloatingChild(int ind);
    int getChildCount();
    int getFloatingChildCount();

    float getDimYClamped(float val);

    float getResultDimYClamped();

	std::string findKeyString(int valEnum);

    void updateLinkedValues(bool isRead=false);

    void setValueIndex(int ind, float val);
    float getValueIndex(int ind);
    float getValueIndexPtr(int ind);

    void updateTextNumber();

    void setValue(float _value, bool doEventDispatch=false, bool preventRefresh=false);
    float getValue();

    void setValueY(float _value, bool doEventDispatch=false, bool preventRefresh=false);
    float getValueY();

    UIComponent* getParent();

    UIComponent* findParentByUID(std::string parUID);

    UIComponent* getValuePtr();

    UIComponent* findNodeByString(std::string _uid);

    float getMinWidth();

    float getMinHeight();

    int addChild(
        int _lastIndex,
        int _parentId,
		std::string* stringVals,
        double* floatVals,
        bool _isFloating,
        JSONValue* _jvNodeNoTemplate

    );

    void setOrigPos();

    void applyHeight();

    void applyWidth();

    void gatherDirty(std::vector<UIComponent*>* dirtyVec);

    void clearDirty();

    void alignToComp(UIComponent* myComp);

    void layout();

    void updateOffset();

    void updateValue(float x, float y);

    void runReport();

    void clearOver();

    bool findMaxLayer(float x, float y, float xTransformed, float yTransformed);

    void testOver(float x, float y);

    bool testHit(int button, int state, float x, float y);

    UIComponent* getHighestCont(UIComponent* curNode, int genCount);

    void setText(std::string _text);

    void updateVecs();

    void renderAll();

    void updateSS();

    float getLineOffset(int lineCount);

    float lengthOfWord(int i, int j, bool isIcon);

    int maxCharsForWord(int i, int j);

    void renderText(bool getDimensions);

public:
    UIComponent* valuePtr;

    float privValueX;
    float privValueY;

    std::vector<int> _children;
    std::vector<int> _floatingChildren;

    Singleton* singleton;

    UICont uiCont;

    std::string uid;
    std::string ss;
    std::string text; // resulting text
    std::string label; // base label
    std::string dataFile;
    std::string dataRef;
    std::string dataKey;
    std::string valRef;

    int matCode;
    int parentId;
    int nodeId;
    int index;
    int fillDir;
    int layer;
    int layerId;
    int hoverType;
    int guiClass;
    //int guiId;

    bool forceDragUpdate;
    bool selected;
    bool foundParent;
    bool foundValuePtr;
    bool visible;
    bool enabled;

    iVector2 align;

    FIVector4 valVec;
    //FIVector4* valVecPtr;
    FIVector4 valVecMask;

    JSONValue* jvNodeNoTemplate;

    fVector2 scrollMaskY;
    fVector2 dragStart;
    fVector2 lastDrag;
    fVector2 dragOffset;

    fVector2 totOffset;
    fVector2 targScrollOffset;
    fVector2 scrollOffset;
    fVector2 floatOffset;
    fVector2 originPos;
    fVector2 resultDimInPixels;
    fVector2 textDimInPixels;
    fVector2 rmDimInPixels; // resulting minimum dim
    fVector2 minDimInPixels;
    fVector2 maxDimInPixels;
    fVector2 fillRatioDim;
    FontWrapper* curFont;
    FontWrapper* curFontIcons;

    bool dragging;
    bool overChild;
    bool overSelf;
    bool singleLine;
    bool hasBackground;
    bool mouseOver;
    bool mouseDown;
    bool wasHit;
    bool isDirty;
    bool isFloating;
    bool dataLoaded;

    uint flags;

    float divisions;
    float paddingInPixels; // inner
    float borderInPixels;
    float marginInPixels; // outer
    float spaceForCharsInPixels;


    iVector2 spacing;
    std::vector<std::string> lineVec;
    std::vector< std::vector<std::string> > wordVec;
    std::vector<float> linePitchVec;

    fBoundingBox hitBounds;
    StyleSheetResult resSS;
};

#endif//_voxelquest__h_

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


        string* stringVals,
        double* floatVals

    );


    void clearChildren();

    UIComponent* getChild(int ind);
    UIComponent* getFloatingChild(int ind);
    int getChildCount();
    int getFloatingChildCount();





    float getDimYClamped(float val);

    float getResultDimYClamped();

    string findKeyString(int valEnum);

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


    UIComponent* findParentByUID(string parUID);


    UIComponent* getValuePtr();


    UIComponent* findNodeByString(string _uid);

    float getMinWidth();


    float getMinHeight();



    int addChild(
        int _lastIndex,
        int _parentId,
        string* stringVals,
        double* floatVals,
        bool _isFloating,
        JSONValue* _jvNodeNoTemplate

    );

    void setOrigPos();

    void applyHeight();

    void applyWidth();

    void gatherDirty(vector<UIComponent*>* dirtyVec);

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

    void setText(string _text);


    void updateVecs();

    void renderAll();


    void updateSS();


    float getLineOffset(int lineCount);

    float lengthOfWord(int i, int j, bool isIcon);

    int maxCharsForWord(int i, int j);

    void renderText(bool getDimensions);

private:
    UIComponent* valuePtr;

    float privValueX;
    float privValueY;

    std::vector<int> _children;
    std::vector<int> _floatingChildren;

    Singleton* singleton;

    Singleton::UICont uiCont;

    string uid;
    string ss;
    string text; // resulting text
    string label; // base label
    string dataFile;
    string dataRef;
    string dataKey;
    string valRef;

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
    std::vector<string> lineVec;
    std::vector< std::vector<string> > wordVec;
    std::vector<float> linePitchVec;

    fBoundingBox hitBounds;
    StyleSheetResult resSS;






    

};

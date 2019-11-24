
class GameGUI {
public:
    GameGUI();

    void init(Singleton* _singleton);



    JSONValue* findNearestKey(JSONValue* jv, string key);


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


    UIComponent* findNodeByString(string _uid);

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

private:
	Singleton* singleton;

	//Image* textImage;
	//GLuint guiTextureId;

	//Shader shaderGUI;

	
	JSONValue* jvRoot;
	JSONValue* jvTemplates;
	JSONValue* jvSounds;
	
	bool isReady;
	bool isLoaded;
	
	fVector2 mouseTrans;
	
	
	int guiRenderCount;
	vector<UIComponent*> dirtyVec;
	
	
	string tempStrings[10];
	
	
	
	string stringVals[E_GST_LENGTH];
	double floatVals[E_GFT_LENGTH];

    

};


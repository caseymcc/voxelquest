#ifndef _voxelquest_singleton_h_
#define _voxelquest_singleton_h_

class Singleton
{
public:
    
    
    

    struct CompareStruct
    {
        bool operator()(const string& first, const string& second)
        {
            return first.compare(second)<0;//first.size() < second.size();
        }
    };
    typedef map<string, UICStruct>::iterator itUICStruct;
    typedef map<string, JSONStruct>::iterator itJSStruct;


    Singleton();
    ~Singleton();

    FIVector4* BTV2FIV(btVector3btv);



    void init(int _defaultWinW, int _defaultWinH, int _scaleFactor

        // #ifdef USE_POCO
        // 	, WebSocketServer *_myWS
        // #endif	


    );


    void listVoices();


    bool speak(string speechString);


    void applyPat(
        int patInd,
        int patShape,
        int rot,
        int x,
        int y,
        int val,
        int rad
    );

    void getVoroOffsets();

    void generatePatterns();


    int placeInStack();

    int placeInLayer(int nodeId, int layer);


    void initAllMatrices();

















    void funcNT2();
    void startNT2();
    bool stopNT2();


    void funcNT();
    void startNT();
    bool stopNT();







    



    












    void toggleDDMenu(int x, int y, bool toggled);

    void dispatchEvent(
        int button,
        int state,
        float x,
        float y,
        UIComponent* comp,
        bool automated=false,
        bool preventRefresh=false
    );

    StyleSheet* getNewStyleSheet(string ssName);

    void initStyleSheet();

    static void qNormalizeAngle(int &angle);










    void setProgAction(eProgramState ps, unsigned char kc, eProgramAction pa, bool isDown);


    void setProgActionAll(unsigned char kc, eProgramAction pa, bool isDown);




    

    void getMaterialString();


    void updatePrimArr();

    void updatePrimTBOData();

    bool getPrimTemplateString();

    void refreshIncludeMap();


    void doShaderRefresh(bool doBake);








    void setWH(int w, int h);








    bool shiftDown();


    bool ctrlDown();


    bool altDown();

    

    float getTerHeightScaled(float val);


    float getSLNormalized();

    float getSeaHeightScaled();


    float getHeightAtPixelPos(float x, float y, bool dd=false);





    void angleToVec(FIVector4* fv, float xr, float yr);

    void vecToAngle(FIVector4* fv, FIVector4 * ta);


    void syncObjects();

    void updateCamVals();

    void moveCamera(FIVector4 *pModXYZ);







    GLfloat getCamRot(int ind);


    void moveObject(float dx, float dy);


    void updateMultiLights();


    void toggleFullScreen();





    /*
    GLUT_KEY_F1   F1 function key
    GLUT_KEY_F2   F2 function key
    GLUT_KEY_F3   F3 function key
    GLUT_KEY_F4   F4 function key
    GLUT_KEY_F5   F5 function key
    GLUT_KEY_F6   F6 function key
    GLUT_KEY_F7   F7 function key
    GLUT_KEY_F8   F8 function key
    GLUT_KEY_F9   F9 function key
    GLUT_KEY_F10    F10 function key
    GLUT_KEY_F11    F11 function key
    GLUT_KEY_F12    F12 function key
    GLUT_KEY_LEFT   Left function key
    GLUT_KEY_RIGHT    Right function key
    GLUT_KEY_UP   Up function key
    GLUT_KEY_DOWN   Down function key
    GLUT_KEY_PAGE_UP  Page Up function key
    GLUT_KEY_PAGE_DOWN  Page Down function key
    GLUT_KEY_HOME   Home function key
    GLUT_KEY_END    End function key
    GLUT_KEY_INSERT   Insert function key
    */



    void setCameraToElevation();




    btVector3 screenToWorld(
        float mx, // zero to one
        float my, // zero to one
        float camAng
    );


    btVector3 getRayTo(float x, float y);





    void runReport();





    void updateCS();


    void getMarkerPos(int x, int y);


    void holderReport();


    


    void resetGeom();
    void stopAllThreads();

    void processInput(unsigned char key, bool keyDown, int x, int y);

	void getPixData(PixData *toPixData, int _xv, int _yv, bool forceUpdate);

    float getMinGeom(int baseIndex);

    FIVector4* getGeomRef(int templateId, int enumVal);

    void setFXYZWGeom(int baseIndex, FIVector4* baseVec);

    void setFXYGeom(int baseIndex, float xv, float yv);
    void setFXGeom(int baseIndex, float xv);
    void setFYGeom(int baseIndex, float yv);
    void setFZGeom(int baseIndex, float zv);
    void setFWGeom(int baseIndex, float wv);

    float getFXGeom(int baseIndex);
    float getFYGeom(int baseIndex);
    float getFZGeom(int baseIndex);
    float getFWGeom(int baseIndex);

    /////////////////////////////////////////
    //                                     //
    // ############## INPUT ############## //
    //                                     //
    /////////////////////////////////////////

    void processSpecialKeys(int key, int _x, int _y);

    void keyboardUp(unsigned char key, int _x, int _y);

    void keyboardDown(unsigned char key, int _x, int _y);

    void updateCurGeom(int x, int y);

    void mouseMove(int _x, int _y);

    void mouseClick(int button, int state, int _x, int _y);

    /////////////////////////////////////////
    //                                     //
    // ########### END INPUT ############# //
    //                                     //
    /////////////////////////////////////////

    bool feetContact(BaseObj* ge);

    void flushKeyStack();

    void applyKeyAction(bool isReq, int actorId, uint keyFlags, float camRotX, float camRotY);

    void gatherKeyActions();

    void handleMovement();

    bool anyMenuVisible();

    void performCamShake(BaseObj* ge, float fp);

    void explodeBullet(BaseObj* ge);

    


    void getSpecialData(int datEnum, string datString);


    


    

    void doAlert();

    

    



    void beginFieldInput(string defString, int cb);

    void processFieldInput(unsigned char key);



    void endFieldInput(bool success);

    void saveOrg();

    void loadOrg();


    float getConst(string conName);

    int iGetConst(int ev);

    void loadConstants();

    void loadGUI();



    // file layout
    // new folder for worldId / versionId
    // blockId + "_" + holderId + ".bin"

    bool checkCacheEntry(int blockId, int chunkId, int holderId, bool doErase=false);

    bool loadCacheEntry(int blockId, int chunkId, int holderId);

    bool saveCacheEntry(int blockId, int chunkId, int holderId);

    bool loadCacheMetaData();

    bool saveCacheMetaData();

    void clearCache();

    bool updateCurCacheLoc();

    
    float getUnderWater();

    void updateAmbientSounds();





    void checkFluid(GameFluid* gf);


    void frameUpdate(bool doFrameRender);



    int getAvailPD();

    FIVector4* cameraGetPos();
    FIVector4* cameraGetPosNoShake();

    float getTargetTimeOfDay();

    void updateBullets();

    void idleFunc();

    void display(bool doFrameRender);




    bool gluInvertMatrix(const double m[16], float invOut[16]);



    int getMatrixInd(int col, int row);

    void ComputeFOVProjection(
        float* result,
        float fov,
        float aspect,
        float nearDist,
        float farDist,
        bool leftHanded
    );


    void getLSMatrix(FIVector4* lightPosParam, Matrix4 &lsMat, float orthoSize);

    void updateLightPos();

    void setMatrices(int w, int h);


    void reshape(int w, int h);


private:
    FIVector4* cameraPos;
    FIVector4 tempLerpPos;
    FIVector4 camLerpPos;
    FIVector4 resultCameraPos;
    FIVector4 targetCameraPos;
    FIVector4 baseCameraPos;

public:

    

    DynBuffer* myDynBuffer;

    CompareStruct compareStruct;


    





    //unsigned long int totTimePassedGraphics;
    //unsigned long int totTimePassedPhysics;


    float mipDis[8];

    bool keysPressed[MAX_KEYS];
    double keyDownTimes[MAX_KEYS];
    unsigned char keyMap[KEYMAP_LENGTH];
    bool keyMapResultZipped[KEYMAP_LENGTH];
    bool keyMapResultUnzipped[KEYMAP_LENGTH];
    int keyMapCoolDown[KEYMAP_LENGTH];
    int keyMapMaxCoolDown[KEYMAP_LENGTH];

    GLdouble viewMatrixD[16];
    float viewMatrixDI[16];
    GLdouble projMatrixD[16];

    Matrix4 lightView;
    Matrix4 identMatrix;
    Matrix4 viewMatrix;
    Matrix4 projMatrix;
    Matrix4 pmMatrix;
    std::vector<Matrix4> objMatrixStack;
    Matrix4 curObjMatrix;
    Matrix4 curMVP;
    Matrix3 curObjMatrix3;
    Matrix4 tempObjMatrix;
    Matrix4 lightSpaceMatrix;
    Matrix4 lightSpaceMatrixLow;

    GLint viewport[4];

    E_OBJ activeObject;
    E_OBJ tempObj;

    eProgramState programState;
    eProgramAction progActionsDown[E_PS_SIZE*256];
    eProgramAction progActionsUp[E_PS_SIZE*256];

    EntSelection nearestLights;
    EntSelection highlightedEnts;
    EntSelection selectedEnts;
    GameEnt* selectedEnt;
    GameEnt* highlightedEnt;

    PatternStruct patterns[E_PAT_LENGTH*4];

    TBOWrapper limbTBO;
    float limbTBOData[MAX_LIMB_DATA_IN_BYTES];

    std::vector<ObjectStruct> tempPrimList;
    TBOWrapper primTBO;
    float primTBOData[MAX_PRIM_DATA_IN_BYTES];


    bool settings[E_BS_LENGTH];

    bool fpsTest;
    bool commandOn;


    bool sphereMapOn;
    bool waitingOnDestruction;
    bool lightChanged;
    bool updateShadows;
    bool updateHolderLookat;


    bool isPressingMove;
    bool doPathReport;
    bool refreshPaths;
    bool placingTemplate;
    bool smoothMove;
    bool ignoreFrameLimit;
    bool autoMove;
    bool allInit;
    bool updateMatFlag;
    bool matVolLock;
    bool isMoving;
    bool perspectiveOn;
    bool lastPersp;
    bool isInteractiveEnt[E_CT_LENGTH];
    bool inputOn;
    bool isMacro;
    bool cavesOn;
    bool bakeParamsOn;
    bool dragging;
    bool mouseMoved;
    bool hitGUI;
    bool draggingMap;
    bool guiDirty;
    bool applyToChildren;
    
    bool emptyVDNotReady;
    bool updateLock;
    bool isFullScreen;
    bool mapInvalid;
    bool wsBufferInvalid;
    bool forceGetPD;
    bool mouseLeftDown;
    bool mouseRightDown;
    bool pboSupported;
    bool notQuit;
    bool timerNotSet;
    bool isBare;
    bool showMap;
    bool traceOn;
    bool firstRun;
    bool rotOn;
    bool doPageRender;
    bool markerFound;


    bool frameMouseMove;
    bool depthInvalidRotate;
    bool depthInvalidMove;
    bool lastDepthInvalidMove;
    bool drawOrient;
    bool multiLights;
    bool timeMod;


    int forceShadowUpdate;
    int destructCount;
    int curPrimTemplate;
    int geomStep;
    int earthMod;
    int currentTick;

    int curPattern;
    int curPatternRot;

    int curSelPrim;
    int limbArrPos;
    int primArrPos;
    int cacheVersion;
    int holderLoadCount;
    int bakeTicks;
    int tbTicks;
    int tempCounter;
    int actorCount;
    int polyCount;
    int fdWritePos;
    int fdReadPos;
    int fpsCountMax;
    int fpsCount;
    int medianCount;
    int maxHolderDis;
    int fieldCallback;
    int mouseState;
    int lastW;
    int lastH;
    int maxLayerOver;
    int pathFindingStep;
    int baseW;
    int baseH;
    int scaleFactor;
    int numDynLights;
    int iNumSteps;
    int curOrgId;
    int extraRad;
    int defaultWinW;
    int defaultWinH;
    int guiWinW;
    int guiWinH;
    int shadersAreLoaded;
    int readyToRecompile;
    int lastPosX;
    int lastPosY;
    int frameCount;
    int screenWidth;
    int screenHeight;
    int mouseMovingSize;
    int mouseMovingLoc;
    int mouseMovingStepsBack;
    int mouseCount;
    int lastMouseX;
    int lastMouseY;
    int cellsPerNodeXY;
    int terDataVisPitchXY;
    int terDataBufPitchXY;
    int terDataBufPitchScaledXY;
    int cellsPerNodeZ;
    int terDataVisPitchZ;
    int terDataBufPitchZ;
    int terDataBufPitchScaledZ;
    int terDataBufAmount;
    int terDataVisSize;
    int terDataBufSize;
    int terDataTexScale;
    int terDataBufSizeScaled;
    int iNodeDivsPerHolder;
    int matVolSize;
    int escCount;
    int mapPitch;
    int lastNodeId;
    int diagCount;
    int frameSkip;
    int frameSkipCount;





    
    //TBOEntry tboPool[MAX_TBOPOOL_SIZE];
    PaddedData pdPool[MAX_PDPOOL_SIZE];
    //GameOctree* octPool[MAX_PDPOOL_SIZE];





    intPair entIdArr[1024];





    uint palWidth;
    uint palHeight;
    uint blockShift;
    uint *terDataScaled;


    GLfloat camRotation[2];
    GLfloat curCamRotation[2];


    uint naUintData[8];
    int naIntData[8];
    float naFloatData[8];
    //float conVals[E_CONST_LENGTH];

    float gammaVal;
    float lastMouseOrigX;
    float lastMouseOrigY;
    float globWheelDelta;
    float amountInvalidMove;
    float amountInvalidRotate;
    float sphereMapPrec;
    float heightMapMaxInCells;
    float resultShake;
    float cameraShake;
    float lastx;
    float lasty;
    float FOV;
    float focalLength;
    float zoomDelta;
    float subjectDelta;
    float subjectZoom;
    float targetSubjectZoom;
    float cameraZoom;
    float targetZoom;
    //float mapSampScale;
    float curBrushRad;
    float timeOfDay;
    float targetTimeOfDay;
    float origWinW;
    float origWinH;
    float guiX;
    float guiY;
    float aspectRatio;
    float currentFBOResolutionX;
    float currentFBOResolutionY;
    float curPrimMod;
    float mouseX;
    float mouseY;
    float mouseXUp;
    float mouseYUp;
    float holderSizeMB;
    float bestNodeDis;
    float heightOfNearPlane;
    float scrollDiv;
    float curMoveSpeed;
    float curMoveAccel;
    float masterVolume;
    float ambientVolume;
    float guiVolume;
    float musicVolume;
    float fxVolume;
    float *paramArr;
    float *paramArrGeom;
    float *primArr;
    float *limbArr;
    float *splashArr;
    float *explodeArr;
    float *voroArr;
    float *matCountArr;
    float *paramArrMap;
    float clipDist[2];

    float MAX_TRAVEL_DIS;

    //double curMoveTime;
    //double lastMoveTime;
    double timeDelta;
    double curTime;
    float smoothTime;
    double pauseTime;
    double clickTime;
    //double lastTime;
    double mdTime;
    double muTime;

    double clickTimeLR[2];
    double mdTimeLR[2];
    double muTimeLR[2];




    FIVector4 geomPoints[E_GEOM_POINTS_LENGTH];



    FIVector4 colVecs[16];

    FIVector4 geomOrigOffset;
    FIVector4 lastSend;





    FIVector4 lastMouseZO;
    FIVector4 lastLightPos;
    FIVector4 lightPosStatic;
    FIVector4 lightPosDynamic;
    FIVector4 lightLookAt;
    FIVector4 lastHolderPos;
    FIVector4 lightVec;
    FIVector4 lightVecOrig;
    FIVector4 dirVecs[6];
    FIVector4 lastCellPos;
    FIVector4 worldMarker;
    FIVector4 lookAtVec;
    FIVector4 lookAtVec2D;
    FIVector4 baseScrollPos;






    



    FIVector4 tempVec1;
    FIVector4 tempVec2;
    FIVector4 tempVec3;





    //FIVector4 moveNodes[2];
    FIVector4 voroVecArr[125];
    floatAndIndex indexArr[125];
    
    FIVector4 mapFreqs;
    FIVector4 mapAmps;
    

    FIVector4 rasterLowDim;
    FIVector4 bufferDim;
    FIVector4 bufferDimTarg;
    FIVector4 bufferDimHalf;
    FIVector4 bufferModDim;
    FIVector4 bufferRenderDim;
    FIVector4 origin;
    FIVector4 panMod;
    FIVector4 dMod;
    FIVector4 modXYZ;
    FIVector4 matVolDim;

    uint* matVol;



    ThreadWrapper threadNetSend;
    ThreadWrapper threadNetRecv;

    std::list<KeyStackEvent> keyStack;



    ThreadPoolWrapper* threadPoolSpeech;



    std::vector<ExplodeStruct> explodeStack;
    std::vector<DebrisStruct> debrisStack;
    //std::vector<btRigidBody*> debrisBodies;


    // must stop all threads before modifying primTemplateStack
    std::vector<FIVector4> primTemplateStack;
    std::vector<SphereStruct> sphereStack;

    std::vector<int> guiLayers[MAX_UI_LAYERS];
    //std::vector<int> emptyLayers[MAX_UI_LAYERS];

    std::vector<RotationInfo> rotMatStack;
    std::vector<DynObject *> dynObjects;
    // PathHolder charPathHolder;
    // PathHolder splitPathHolder;

    VBOWrapper fsQuad;
    VBOWrapper zoCube;

    VBOWrapper zoCubes;


    float floorHeightInCells;
    float roofHeightInCells;
    float wallRadInCells;


    Image *imageVoro;
    Image *imageHM0;
    Image *imageHM1;
    Image *cloudImage;



    GamePageHolder* closestHolder;

    GamePlant* gamePlants[E_PT_LENGTH/2];

    Shader *curShaderPtr;
    string jsonPostString;
    string currentFieldString;
    string curShader;
    string allText;
    string stringBuf;
    string curCallback;
    string cbDataStrings[10];

    string guiSaveLoc;

    string curCLFull;
    string curCLBaseDir;
    string curCLWorldDir;


    VolumeWrapper* volumeWrappers[E_VW_LENGTH];
	

    vector<string> jsonPostStack;
    
    vector<string> shaderStrings;
    vector<string> shaderTextureIds;
    map<string, Shader*> shaderMap;
    map<string, UICStruct> compMap;
    map<string, string> includeMap;
    map<string, FBOSet> fboMap;



    // FBOSet vgFBOArr[MAX_VG_FBOS];
    // FBOSet vgtFBOArr[MAX_VGT_FBOS];






    GLuint fsqDL;

    // GLuint volId;
    // GLuint volIdLinear;
    // GLuint volIdEmpty;
    // GLuint volIdEmptyLinear;

    GLuint volIdMat;




    GLuint volGenId;
    uint *lookup2to3;
    unsigned char *resultImage;

    materialNode* matSlice0;
    materialNode* matSlice1;
    materialNode* matSlice2;

    charArr nullBuffer;
    charArr lastImageBuffer;
    //charArr lastJSONBuffer;
    charArr lastJSONBufferGUI;


    JSONValue *rootObjJS;
    JSONValue *guiRootJS;
    JSONValue *constRootJS;

    JSONValue *cacheMetaJS;

    // #ifdef USE_POCO
    // 	WebSocketServer *myWS;
    // #endif

    HPClock bulletTimer;
    Timer fpsTimer;
    Timer shakeTimer;
    Timer myTimer;
    Timer scrollTimer;
    Timer moveTimer;
    VBOGrid myVBOGrid;
    //GameOctree* gameOct;
    GameWorld* gw;
    GameEntManager* gem;
    GamePhysics* gamePhysics;
    GameFluid* gameFluid[E_FID_LENGTH];
    GameLogic* gameLogic;
    GameNetwork* gameNetwork;
    GameAI* gameAI;

    VIStruct chunkVI[NUM_MIP_LEVELS_WITH_FIRST];

    float lightArr[MAX_LIGHTS*16];
    int numLights;




    
    


    TerTexture terTextures[MAX_TER_TEX];




    GameGUI* mainGUI;

    UIComponent* mapComp;

    UIComponent* menuList[E_FM_LENGTH];


    UIComponent* contMenuBar;
    UIComponent* ddMenuBar;
    UIComponent* fieldText;

    FontWrapper* fontWrappers[EFW_LENGTH];

//    GameMusic* music[EML_LENGTH];
//    map<string, GameSound> soundMap;

    map<string, StyleSheet> styleSheetMap;
    map<string, JSONStruct> externalJSON;

    FIVector4 btvConv;

};

#endif//_voxelquest__h_
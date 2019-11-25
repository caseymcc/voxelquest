#ifndef _voxelquest_constants_h_
#define _voxelquest_constants_h_

int RUN_COUNT;

bool ND_TRACE_OFF=false;


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

const static int MAX_LIMB_DATA_IN_BYTES=65536;

const static bool POLYS_FOR_CELLS=true;
//const static bool POLY_COLLISION = false;
const static bool VOXEL_COLLISION=true;

const static bool GEN_DEBRIS=false;
const static int  MAX_DEBRIS=0;
const static bool GEN_COLLISION=false;
const static bool GEN_POLYS_WORLD=true;

const static bool SINGLE_THREADED=false;
const static bool DO_RANDOMIZE=false;

// no greater than 8 unless shader changed (primIdList[8])
const static int MAX_PRIM_IDS=16;
const static int MAX_PRIMTEST=8;

const static int MAX_DEPTH_PEELS=4;

double STEP_TIME_IN_SEC;

const static float OFFSET_X[4]={-0.5,0.5,0.5,-0.5};
const static float OFFSET_Y[4]={-0.5,-0.5,0.5,0.5};

const static int MAX_SPLASHES=8;
const static int MAX_EXPLODES=8;

//const static bool DO_CONNECT = true;
const static bool DO_SHADER_DUMP=false;
bool EDIT_POSE=false;

// warning: changing this changes the size of saved poses
// should be a one time change, then revereted to 1.0 after save
const static float ORG_SCALE_DELTA=1.0f;

// base scale applied to base org generation
// only change this after changing ORG_SCALE_DELTA from 1.0
const static float ORG_SCALE_BASE=0.5f;





// const static int DEF_WIN_W = 1440;
// const static int DEF_WIN_H = 720;

// qqqq

//#define STREAM_RES 1

#ifdef STREAM_RES
const static int DEF_WIN_W=2048; //2048;//
const static int DEF_WIN_H=1024; //1024;//
#else
const static int DEF_WIN_W=1536;//1536;
const static int DEF_WIN_H=768;//768;
#endif



const static int DEF_SCALE_FACTOR=4;
const static int RENDER_SCALE_FACTOR=1;
const static float SPHEREMAP_SCALE_FACTOR=0.5f; // lower is faster

const static int DEF_VOL_SIZE=128;

const static bool USE_SPHERE_MAP=false;

const static float TIME_DELTA=1.0f/60.0f;

const static int THREAD_DATA_COUNT=16;

const static float MASS_PER_LIMB=0.1f;

// #define DEBUG_BOUNDS 1

const static float explodeRad=5.0f;

float RAND_COUNTER=25.0f;
float RAND_COUNTER2=25.0f;


const static int GROUP_ID_NONEXISTENT=-3;
const static int GROUP_ID_UNMARKED=-2;
const static int GROUP_ID_UNMARKED_IDEAL=-1;

const static int FLUID_UNIT_MIN=-1;
const static int FLUID_UNIT_MAX=16384;

const static int MAX_LAYERS=2;
const static int MAX_MIP_LEV=1; // min of 1

const static bool DO_POINTS=true;


const static int MAX_KEYS=256;

//const static int NUM_PRIM_LAYERS = 1;

const static int MAX_LIGHTS=24;
const static int MAX_EVAL_LIGHTS=128;
const static int FLOATS_PER_LIGHT=12;



const static float UI_SCALE_FACTOR=1.0f;

const static int MAX_PLANT_GEN=16;

const static int MAX_TER_TEX=9;

const static bool RT_TRANSFORM=false;



const static int MAX_NODE_DIRS=6;
const static int MAX_NODE_VALS=4;
const static int TOT_NODE_VALS=
MAX_NODE_DIRS*MAX_NODE_VALS;

const static int TOT_MAP_DIRS=4;

const static int MAX_BLOCK_STACK=10;
const static int MAX_UI_LAYERS=4;

// solid, water, air
const static float CUBE_POINTS[42]={
        0,0,0,
        1,0,1,
        0,0,1,
        0,1,1,
        0,1,0,
        1,1,0,
        1,0,0,


        1,1,1,
        1,0,0,
        1,1,0,
        0,1,0,
        0,1,1,
        0,0,1,
        1,0,1



};




// solid, water, air
const static bool PROC_MATRIX[3][3]={
        {false, true, true},
        {false, false, true},
        {false, false, false}
};

bool TEMP_DEBUG;

const static int MAX_OBJ_TYPES=1673;
const static int ITEMS_PER_COL=48;
const static int ITEMS_PER_ROW=36;
const static int MAX_ICON_ID=ITEMS_PER_COL*ITEMS_PER_ROW;
const static int EMPTY_OBJECT_TYPE=1672;
const static int EMPTY_OBJECT_ID=1;

enum E_DIR_SPECS_SIGN
{
    E_DIR_XP,
    E_DIR_XM,
    E_DIR_YP,
    E_DIR_YM,
    E_DIR_ZP,
    E_DIR_ZM,
    E_DIR_MP_LENGTH
};


const static int NUM_ORIENTATIONS=6;
const static float DIR_VECS[NUM_ORIENTATIONS][3]={
    {1.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, -1.0f}
};
const static int DIR_VECS_I[NUM_ORIENTATIONS][3]={
    {1, 0, 0},
    {-1, 0, 0},
    {0, 1, 0},
    {0, -1, 0},
    {0, 0, 1},
    {0, 0, -1}
};


const static int NUM_PLANAR_ORIENTATIONS=4;
const static int BASE_MOVEABLE_Z=2;
const static int MAX_MOVEABLE_Z=BASE_MOVEABLE_Z*2+1;
const static int NUM_MOVEABLE_DIRS_ONE_AWAY=NUM_PLANAR_ORIENTATIONS*3;

const static int NUM_MOVEABLE_DIRS=MAX_MOVEABLE_Z*NUM_PLANAR_ORIENTATIONS;// 12 20 24
const static int DIR_VECS_MOVE[NUM_MOVEABLE_DIRS][3]={

    { 1,  0,  -2},
    {-1,  0,  -2},
    { 0,  1,  -2},
    { 0, -1,  -2},

    { 1,  0, -1},
    {-1,  0, -1},
    { 0,  1, -1},
    { 0, -1, -1},


    { 1,  0,  0},
    {-1,  0,  0},
    { 0,  1,  0},
    { 0, -1,  0},




    { 1,  0,  1},
    {-1,  0,  1},
    { 0,  1,  1},
    { 0, -1,  1},

    { 1,  0,  2},
    {-1,  0,  2},
    { 0,  1,  2},
    { 0, -1,  2}





    // ,
    // { 1,  0,  -3},
    // {-1,  0,  -3},
    // { 0,  1,  -3},
    // { 0, -1,  -3},

    // { 1,  0,  3},
    // {-1,  0,  3},
    // { 0,  1,  3},
    // { 0, -1,  3}



    // ,{ 1,  1,  1},
    // { 1, -1,  1},
    // {-1,  1,  1},
    // {-1, -1,  1},

    // { 1,  1,  0},
    // { 1, -1,  0},
    // {-1,  1,  0},
    // {-1, -1,  0},

    // { 1,  1, -1},
    // { 1, -1, -1},
    // {-1,  1, -1},
    // {-1, -1, -1}


};


float ALL_ROT[16*NUM_ORIENTATIONS*NUM_ORIENTATIONS*NUM_ORIENTATIONS];
const static int ROT_MAP[36]={

    // x, inc theta
    //-------------
    E_DIR_XP,// x+ -> x+
    E_DIR_XM,// x- -> x-
    E_DIR_ZP,// y+ -> z+
    E_DIR_ZM,// y- -> z-
    E_DIR_YM,// z+ -> y-
    E_DIR_YP,// z- -> y+

    // x, dec theta
    //-------------
    E_DIR_XP,// x+ -> x+
    E_DIR_XM,// x- -> x-
    E_DIR_ZM,// y+ -> z-
    E_DIR_ZP,// y- -> z+
    E_DIR_YP,// z+ -> y+
    E_DIR_YM,// z- -> y-

    // y, inc theta
    //-------------
    E_DIR_ZP,// x+ -> z+
    E_DIR_ZM,// x- -> z-
    E_DIR_YP,// y+ -> y+
    E_DIR_YM,// y- -> y-
    E_DIR_XM,// z+ -> x-
    E_DIR_XP,// z- -> x+

    // y, dec theta
    //-------------
    E_DIR_ZM,// x+ -> z-
    E_DIR_ZP,// x- -> z+
    E_DIR_YP,// y+ -> y+
    E_DIR_YM,// y- -> y-
    E_DIR_XP,// z+ -> x+
    E_DIR_XM,// z- -> x-

    // z, inc theta
    //-------------
    E_DIR_YP,// x+ -> y+
    E_DIR_YM,// x- -> y-
    E_DIR_XM,// y+ -> x-
    E_DIR_XP,// y- -> x+
    E_DIR_ZP,// z+ -> z+
    E_DIR_ZM,// z- -> z-

    // z, dec theta
    //-------------
    E_DIR_YM,// x+ -> y-
    E_DIR_YP,// x- -> y+
    E_DIR_XP,// y+ -> x+
    E_DIR_XM,// y- -> x-
    E_DIR_ZP,// z+ -> z+
    E_DIR_ZM// z- -> z-

};

int totalPointCount;


bool LAST_COMPILE_ERROR=false;

char *BUF_NAMES[]=
{
    "ublock0",
    "ublock1",
    "ublock2",
    "ublock3",
    "ublock4",
    "ublock5",
    "ublock6",
    "ublock7",
    "ublock8",
    "ublock9",
    "ublock10",
    "ublock11",
    "ublock12",
    "ublock13"
};

const static int R_CHANNEL=0;
const static int G_CHANNEL=1;
const static int B_CHANNEL=2;
const static int A_CHANNEL=3;

const static int MAX_VG_FBOS=2;
int CUR_VG_FBO=0;

const static int MAX_VGT_FBOS=2;
int CUR_VGT_FBO=0;

const static float M_PI=3.14159265359f;

const static float TBDIR_ARR[]={
    0.0f,
    M_PI*0.5f,
    M_PI,
    M_PI*1.5f
};


int PAGE_COUNT=0;
// set to 0 to disable
int MAX_HOLDERS=2048;


float MAX_GPU_MEM=2560.0f;
float TOT_GPU_MEM_USAGE=0.0f;

float MAX_CPU_MEM=4096.0f;
float TOT_CPU_MEM_USAGE=0.0f;

long long ENT_COUNTER=0;

bool TRACE_ON=false;
//#define DEBUG_MODE 1

#endif//_voxelquest_constants_h_
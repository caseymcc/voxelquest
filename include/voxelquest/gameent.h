#ifndef _voxelquest_gameent_h_
#define _voxelquest_gameent_h_

#include "voxelquest/gamepageholder.h"
#include "voxelquest/gameentnode.h"
#include "voxelquest/geom.h"

enum E_ENT_TYPES
{
    E_ET_GEOM,
    //E_ET_ACTOR,
    E_ET_LIGHT,
    E_ET_LENGTH

};

class Singleton;

class GameEnt
{
public:
    GameEnt();

    void initLight(
        FIVector4 *position,
        FIVector4 *color,
        float radius
    );

    // void setLightPos(FIVector4* newPos);

    FIVector4 *getVisMinInPixelsT();

    FIVector4 *getVisMaxInPixelsT();

    int getClampedRot();

    void rotate(int mod, bool ignoreConstraints);

    void initBounds(
        int _buildingType,
        int alignBottomMiddleTop,

        float _zOffset,

        FIVector4 *p1,
        FIVector4 *p2,
        FIVector4 *rad,
        FIVector4 *_cornerDisInPixels,
        FIVector4 *_visInsetFromMin,
        FIVector4 *_visInsetFromMax,
        FIVector4 *_powerVals,
        FIVector4 *_powerVals2,
        FIVector4 *_thickVals,
        FIVector4 *_matParams,
        FIVector4 *_anchorPoint,
        int _minRot,
        int _maxRot

    );

    void initLines(
        int _buildingType,
        float scale,

        FIVector4 *_offset,

        FIVector4 *_orgVec,
        FIVector4 *_tanVec, // already scaled
        FIVector4 *_bitVec,
        FIVector4 *_norVec,
        FIVector4 *_radVec0,
        FIVector4 *_radVec1,
        //FIVector4 *_radVecScale0,
        //FIVector4 *_radVecScale1,
        FIVector4 *_matParams
    );

    void initTree(
        int _buildingType,

        // p0, p1 = start, end
        // p2 = control point or tangent

        FIVector4 *p0,
        FIVector4 *p1,
        FIVector4 *p2,

        float radP0,
        float radP1,
        float sphereRad,

        FIVector4 *_matParams



    );

    void toggle();

    void toggleTransform();

    void applyTransform(int rotMod, bool ignoreConstraints);

    void initAnchorPoint(FIVector4 *_anchorPointInPixels, int _minRot, int _maxRot);

private:

    FIVector4 boundsMinInPixels;
    FIVector4 boundsMaxInPixels;
    FIVector4 visMinInPixels;
    FIVector4 visMaxInPixels;

public:

    int templateId;
    FIVector4 templatePos;

    int entType;
    //int toggleState;
    //int maxToggleStates;

    bool toggled;
    bool visible;
    //bool isFalling;

    float camDistance;



    FIVector4 geomParams[E_GP_LENGTH];

    FIVector4 anchorPointInPixels;
    FIVector4 moveMinInPixels;
    FIVector4 moveMaxInPixels;

    GameEnt *light;



    bool hasAnchor;

    //   1
    // 2   0
    //   3


    int rotDir;
    int minRot;
    int maxRot;
    int curRot;

    int buildingType;


    FIVector4 tempVec1;
    FIVector4 tempVec2;
    FIVector4 tempVec3;
    FIVector4 tempVec4;
    FIVector4 tempVec5;
};

struct EntVec
{
    std::vector<GameEnt> data;
};



class EntSelection
{
public:
    EntSelection();

    GameEnt* getSelectedEnt();

    void cycleEnts();

    int selEntListInd;
    std::vector<GameEnt *> selEntList;
    std::map<intPair, int> selEntMap;
};



class DynObject
{

public:
    DynObject();

    GameEnt *getLight();

    void init(
        int _x, int _y, int _z,
        int _r, int _g, int _b,
        bool _doRender, int _moveType,//bool _isRelative, bool _isTrackball,
        FIVector4 *_cameraPos,
        float _radius,
        float _lightRadius=0.0f
    );


    FIVector4 pos;
    FIVector4 color;
    FIVector4 posRel;
    FIVector4 posTrackball;

    int r;
    int g;
    int b;

    //bool isTrackball;
    //bool isRelative;
    int moveType;
    bool doRender;
    bool firstRun;

    float radius;
    float lightRadius;

    GameEnt childLight;
};

#endif//_voxelquest__h_
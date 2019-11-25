
class GameGeom
{
    GameGeom();

    void init(int _id);

    FIVector4 *getBoundsMinInPixels();
    FIVector4 *getBoundsMaxInPixels();
    FIVector4 *getVisMinInPixels();
    FIVector4 *getVisMaxInPixels();

    FIVector4 *getBoundsMinInPixelsT();
    FIVector4 *getBoundsMaxInPixelsT();
    FIVector4 *getVisMinInPixelsT();
    FIVector4 *getVisMaxInPixelsT();

    int getClampedRot();

    void rotate(int mod, bool ignoreConstraints);

    void initBounds(
        int _buildingType,
        int _id,
        int _globalId,
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
        FIVector4 *_centerPoint,
        FIVector4 *_anchorPoint,
        int _minRot,
        int _maxRot

    );


    void initLines(
        int _buildingType,
        int _id,
        int _globalId,
        float scale,

        FIVector4 *_offset,

        FIVector4 *_orgVec,
        FIVector4 *_tanVec, // already scaled
        FIVector4 *_bitVec,
        FIVector4 *_norVec,
        FIVector4 *_radVec0,
        FIVector4 *_radVec1,
        FIVector4 *_radVecScale0,
        FIVector4 *_radVecScale1,
        FIVector4 *_matParams



    );


    void initTree(
        int _buildingType,
        int _id,
        int _globalId,

        // p0, p1 = start, end
        // p2 = control point or tangent

        FIVector4 *p0,
        FIVector4 *p1,
        FIVector4 *p2,

        float radP0,
        float radP1,
        float sphereRad,

        // FIVector4* rad,
        // FIVector4* _cornerDisInPixels,
        //FIVector4* _visInsetFromMin,
        //FIVector4* _visInsetFromMax,
        // FIVector4* _powerVals,
        // FIVector4* _powerVals2,
        // FIVector4* _thickVals,
        FIVector4 *_matParams



    );


    void toggleTransform();

    void applyTransform(int rotMod, bool ignoreConstraints);

    void initAnchorPoint(FIVector4 *_anchorPointInPixels, int _minRot, int _maxRot);

private:

    FIVector4 tempVec1;
    FIVector4 tempVec2;
    FIVector4 tempVec3;
    // passed to GPU, transformed

    FIVector4 boundsMinInPixels;
    FIVector4 boundsMaxInPixels;
    FIVector4 visMinInPixels;
    FIVector4 visMaxInPixels;

    // passed to GPU

    FIVector4 geomParams[E_GP_LENGTH];

    // enum E_GEOM_PARAMS {
    // E_GP_BOUNDSMININPIXELST,
    // E_GP_BOUNDSMAXINPIXELST,
    // E_GP_VISMININPIXELST,
    // E_GP_VISMAXINPIXELST,
    //     E_GP_CORNERDISINPIXELS,
    //     E_GP_POWERVALS,
    //     E_GP_POWERVALS2,
    //     E_GP_THICKVALS,
    //     E_GP_MATPARAMS,
    //     E_GP_LENGTH
    // }

    // geomParams[E_GP_BOUNDSMININPIXELST]
    // geomParams[E_GP_BOUNDSMAXINPIXELST]
    // geomParams[E_GP_VISMININPIXELST]
    // geomParams[E_GP_VISMAXINPIXELST]
    // geomParams[E_GP_CORNERDISINPIXELS]
    // geomParams[E_GP_POWERVALS]
    // geomParams[E_GP_POWERVALS2]
    // geomParams[E_GP_THICKVALS]
    // geomParams[E_GP_MATPARAMS]

    // FIVector4 boundsMinInPixelsT;
    // FIVector4 boundsMaxInPixelsT;
    // FIVector4 visMinInPixelsT;
    // FIVector4 visMaxInPixelsT;
    // FIVector4 cornerDisInPixels;
    // FIVector4 powerVals;
    // FIVector4 powerVals2;
    // FIVector4 thickVals;
    // FIVector4 matParams;

    // internal use

    FIVector4 anchorPointInPixels;
    FIVector4 moveMinInPixels;
    FIVector4 moveMaxInPixels;

    GameLight *light;

    bool visible;
    bool hasAnchor;
    bool isToggled;

    //   1
    // 2   0
    //   3


    int rotDir;

    int minRot;
    int maxRot;
    int curRot;
    int buildingType;

    int id;
    int globalId;

    //float minRad;
    //float maxRad;
    //float matId;
};

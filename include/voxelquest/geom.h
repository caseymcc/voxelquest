#ifndef _voxelquest_geom_h_
#define _voxelquest_geom_h_

#include "voxelquest/vectors.h"
#include "voxelquest/object.h"

enum E_GEOM_PARAMS
{
    E_GP_RESERVED0, //visMin
    E_GP_RESERVED1, //visMax
    E_GP_BOUNDSMININPIXELST,
    E_GP_BOUNDSMAXINPIXELST,
    E_GP_CORNERDISINPIXELS,
    E_GP_POWERVALS,
    E_GP_POWERVALS2,
    E_GP_THICKVALS,
    E_GP_UNUSED,
    E_GP_MATPARAMS, // must be last
    E_GP_LENGTH
};

enum E_PRIMTEMP
{
    E_PRIMTEMP_VISMIN,
    E_PRIMTEMP_VISMAX,
    E_PRIMTEMP_BOUNDSMIN,
    E_PRIMTEMP_BOUNDSMAX,
    E_PRIMTEMP_CORNERDIS, //E_PRIMTEMP_CORNERDISTHICKNESSPOWER
    E_PRIMTEMP_MATPARAMS,
    E_PRIMTEMP_LENGTH
};

enum E_GEOM_POINTS
{
	E_GEOM_POINTS_ORIGIN,
	E_GEOM_POINTS_RAD_XYZ,
	E_GEOM_POINTS_OFFSET,
	E_GEOM_POINTS_CORNER,
	E_GEOM_POINTS_POWER_VALS,
	E_GEOM_POINTS_NEG_RAD_XY,
	E_GEOM_POINTS_POS_RAD_XY,
	E_GEOM_POINTS_NEG_RAD_Z,
	E_GEOM_POINTS_POS_RAD_Z,
	E_GEOM_POINTS_THICKNESS,
	E_GEOM_POINTS_LENGTH
};

enum E_PRIM_TYPE_TER
{
	E_PTT_TER,
	E_PTT_WAT,
	E_PTT_BLD,//E_PTT_LST,
	E_PTT_EMP,
	E_PTT_LENGTH
};

enum E_PRIM_TYPE_EXT
{
	E_PTT_IDE,
	E_PTT_STB,
	E_PTT_LST,//E_PTT_BLD,
	E_PTT_FLG,
	E_EXT_LENGTH
};

#ifdef GEOM_NOEXTERN
#define GEOM_EXTERN
#else
#define GEOM_EXTERN extern
#endif

GEOM_EXTERN std::vector <FIVector4> primTemplateStack;
GEOM_EXTERN int geomStep;
GEOM_EXTERN FIVector4 geomOrigOffset;
GEOM_EXTERN FIVector4 geomPoints[E_GEOM_POINTS_LENGTH];
GEOM_EXTERN std::vector<ObjectStruct> tempPrimList;


float &getGeoParam(int param);

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

void resetGeom();

FIVector4* getGeomRef(int templateId, int enumVal);

bool getPrimTemplateString();

#endif//_voxelquest_geom_h_

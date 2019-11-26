#ifndef _voxelquest_geom_h_
#define _voxelquest_geom_h_

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

#endif//_voxelquest_geom_h_

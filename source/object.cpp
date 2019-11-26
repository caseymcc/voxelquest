#include "voxelquest/object.h"

inline void getBoundsCenRad(
    ObjectStruct* curObj,
    FIVector4* baseGeom,
    vec3 &visCen,
    vec3 &visRad
)
{


    vec3 vmin=baseGeom[E_PRIMTEMP_BOUNDSMIN].getVec3();
    vec3 vmax=baseGeom[E_PRIMTEMP_BOUNDSMAX].getVec3();

    visCen=(vmax+vmin)*0.5f;
    visRad=(vmax-vmin)*0.5f;

    visCen+=curObj->offset;

}

void getVisCenRad(
    ObjectStruct* curObj,
    FIVector4* baseGeom,
    vec3 &visCen,
    vec3 &visRad
)
{


    vec3 vmin=baseGeom[E_PRIMTEMP_VISMIN].getVec3();
    vec3 vmax=baseGeom[E_PRIMTEMP_VISMAX].getVec3();

    visCen=(vmax+vmin)*0.5f;
    visRad=(vmax-vmin)*0.5f;

    visCen+=curObj->offset;

}

inline vec3 getUVW(
    vec3 _pos,
    ObjectStruct* curObj,
    FIVector4* baseGeom,

    vec3 uvwScale,
    float angModBase,
    bool mirrored
)
{

    float globIntersect=999.0f;

    vec3 centerPos;
    vec3 centerRad;
    getBoundsCenRad(curObj, baseGeom, centerPos, centerRad);


    float cornerRad=baseGeom[E_PRIMTEMP_CORNERDIS].getFX();
    vec3 centerOffset=_pos-centerPos;
    vec3 innerBoxSize=centerRad-cornerRad;

    float wallThickness=baseGeom[E_PRIMTEMP_CORNERDIS].getFY();
    vec3 centerOffsetAbs=centerOffset;
    centerOffsetAbs.doAbs();


    vec3 newOffset=centerOffsetAbs-innerBoxSize;
    newOffset.doMax(vec3(0.0));

    // vec3 newOffsetAbs = newOffset;
    // newOffsetAbs.doAbs();



    vec3 minCorner=centerPos-(innerBoxSize+cornerRad);
    vec3 pos=(_pos-minCorner)*uvwScale;



    vec3 uvwCoords=pos;

    vec3 newNorm1=newOffset;
    newNorm1.z=0.0f;
    newNorm1.normalize();

    if(centerOffset.x<0.0f)
    {
        newNorm1.x*=-1.0f;
    }
    if(centerOffset.y<0.0f)
    {
        newNorm1.y*=-1.0f;
    }

    vec3 newNorm2;
    newNorm2=vec3(
        newNorm1.length(),
        newOffset.z,
        0.0f
    );

    newNorm2.normalize();


    vec3 centerOffsetNorm=centerOffset/innerBoxSize;
    centerOffsetNorm.doAbs();

    // y side
    if(newNorm1.x==0.0f)
    {
        uvwCoords.x=pos.x;
        uvwCoords.y=pos.z;

    }

    // x side
    if(newNorm1.y==0.0f)
    {
        uvwCoords.x=pos.y;
        uvwCoords.y=pos.z;

    }

    float curPhi=atan2(newNorm1.y, newNorm1.x);
    float curThe=atan2(newNorm2.y, newNorm2.x);

    float angMod=
        (angModBase*2.0f/M_PI) *
        (max(floor(sqrt(cornerRad*cornerRad*2.0f)), 1.0f));


    // side corner
    if(newNorm1.x*newNorm1.y!=0.0f)
    {
        uvwCoords.x=curPhi*angMod;
        uvwCoords.y=pos.z;
    }

    // top corner
    if(newNorm2.x*newNorm2.y!=0.0f)
    {
        uvwCoords.y=curThe*angMod;
    }

    // top
    if(newNorm2.x==0.0f)
    {
        if(centerOffsetNorm.x>centerOffsetNorm.y)
        {
            uvwCoords.x=pos.y;
            uvwCoords.y=pos.x;

            if(centerOffset.x>0.0f)
            {
                uvwCoords.y*=-1.0f;

                uvwCoords.y+=0.5f;
            }
        }
        else
        {
            uvwCoords.x=pos.x;
            uvwCoords.y=pos.y;

            if(centerOffset.y>0.0f)
            {
                uvwCoords.y*=-1.0f;

                uvwCoords.y+=0.5f;

            }
        }

        globIntersect=abs(centerOffsetNorm.x-centerOffsetNorm.y);

    }


    uvwCoords.z=0.0;

    return uvwCoords;//vec4(uvwCoords,newNorm2.x);
}





inline float primDis(
    vec3 pos,
    ObjectStruct* curObj,
    FIVector4* baseGeom
)
{

    vec3 centerPos;
    vec3 centerRad;
    vec2 box_power;

    getBoundsCenRad(curObj, baseGeom, centerPos, centerRad);


    float cornerRad=baseGeom[E_PRIMTEMP_CORNERDIS].getFX();
    float wallThickness=baseGeom[E_PRIMTEMP_CORNERDIS].getFY();
    box_power.x=baseGeom[E_PRIMTEMP_CORNERDIS].getFZ();
    box_power.y=baseGeom[E_PRIMTEMP_CORNERDIS].getFW();

    vec3 innerBoxSize=centerRad-cornerRad;

    vec3 vectorFromCenter=pos-centerPos;



    vec3 absVecFromCenter=vectorFromCenter;
    absVecFromCenter.doAbs();



    vec3 newP=absVecFromCenter-innerBoxSize;
    newP.doMax(vec3(0.0));
    newP.doAbs();



    newP.x=pow(newP.x, box_power.x);
    newP.y=pow(newP.y, box_power.x);
    newP.x=pow(newP.x+newP.y, 1.0/box_power.x);

    newP.x=pow(newP.x, box_power.y);
    newP.z=pow(newP.z, box_power.y);
    newP.x=pow(newP.x+newP.z, 1.0/box_power.y);

    float finalRes=(newP.x-cornerRad);

    vec3 newBoxDim;

    if(
        (absVecFromCenter.x<=innerBoxSize.x)&&
        (absVecFromCenter.y<=innerBoxSize.y)&&
        (absVecFromCenter.z<=innerBoxSize.z)
        )
    {

        newBoxDim=innerBoxSize-absVecFromCenter;

        finalRes-=min(min(newBoxDim.x, newBoxDim.y), newBoxDim.z);
    }

    finalRes=abs(finalRes+wallThickness*0.5f)-wallThickness*0.5f;


    vec3 boxRad;
    vec3 boxCen;

    getVisCenRad(curObj, baseGeom, boxCen, boxRad);


    return (
        opI(
        sdBox(pos-boxCen, boxRad),
        finalRes
        )
        );

    // return vec3(
    // 		(newP.x-cornerRad),
    // 		( (cornerRad-wallThickness)-newP.x ),
    // 		(newP.x-(cornerRad-wallThickness))
    // );


}


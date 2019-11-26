#include "voxelquest/constants.h"
#include "voxelquest/jsonhelpers.h"

float conVals[E_CONST_LENGTH];

void loadConstants()
{
    int i;

   
    stopAllThreads();

    if(loadJSON("..\\data\\constants.js", &constRootJS))
    {

        for(i=0; i<E_CONST_LENGTH; i++)
        {
            conVals[i]=getConst(E_CONST_STRINGS[i]);
        }

    }

//    mapAmps.setFXYZW(
//        conVals[E_CONST_MAPAMP0],
//        conVals[E_CONST_MAPAMP1],
//        conVals[E_CONST_MAPAMP2],
//        conVals[E_CONST_MAPAMP3]
//    );
//    mapFreqs.setFXYZW(
//        conVals[E_CONST_MAPFREQ0],
//        conVals[E_CONST_MAPFREQ1],
//        conVals[E_CONST_MAPFREQ2],
//        conVals[E_CONST_MAPFREQ3]
//    );
//
//    STEP_TIME_IN_SEC=conVals[E_CONST_STEP_TIME_IN_MICRO_SEC]/1000000.0;
//
//    mipDis[0]=conVals[E_CONST_MIPDIS0];
//    mipDis[1]=conVals[E_CONST_MIPDIS1];
//    mipDis[2]=conVals[E_CONST_MIPDIS2];
//    mipDis[3]=conVals[E_CONST_MIPDIS3];
//    mipDis[4]=conVals[E_CONST_MIPDIS4];
//    mipDis[5]=conVals[E_CONST_MIPDIS5];
//    mipDis[6]=conVals[E_CONST_MIPDIS6];
//    mipDis[7]=conVals[E_CONST_MIPDIS7];
}

int iGetConst(int ev)
{
    return (int)conVals[ev];
}

float getConst(int ev)
{
    return conVals[ev];
}

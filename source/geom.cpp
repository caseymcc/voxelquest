#define GEOM_NOEXTERN
#include "voxelquest/geom.h"

#include "voxelquest/jsonhelpers.h"
#include "voxelquest/gamestate.h"
#include "voxelquest/gamefluid.h"
#include "voxelquest/helperfuncs.h"
#include "voxelquest/materials.h"
#include "voxelquest/gameworld.h"
#include "voxelquest/renderer.h"

#include <algorithm>
#include <iostream>

const static int FLOATS_PER_TEMPLATE=((int)E_PRIMTEMP_LENGTH)*4;
const static float defaultTemplate[FLOATS_PER_TEMPLATE]={
    -2.0,-2.0,-2.0, 0.0,
     2.0, 2.0, 2.0, 0.0,
    -2.0,-2.0,-2.0, 0.0,
     2.0, 2.0, 2.0, 0.0,
     1.0, 1.0, 1.0, 1.0,
     0.0, 0.0, 0.0, 0.0
};

float paramArrGeom[128];

float &getGeoParam(int param)
{
    return paramArrGeom[param];
}

float getMinGeom(int baseIndex)
{
    int newIndex=baseIndex*4;

    return std::min(
		std::min(
        paramArrGeom[newIndex+0],
        paramArrGeom[newIndex+1]
    ),
        paramArrGeom[newIndex+2]
    );

}


void setFXYZWGeom(int baseIndex, FIVector4* baseVec)
{
    int newIndex=baseIndex*4;
    int i;

    for(i=0; i<4; i++)
    {
        paramArrGeom[newIndex+i]=baseVec->getIndex(i);
    }

}

void setFXYGeom(int baseIndex, float xv, float yv)
{
    int newIndex=baseIndex*4;
    paramArrGeom[newIndex+0]=xv;
    paramArrGeom[newIndex+1]=yv;
}
void setFXGeom(int baseIndex, float xv)
{
    int newIndex=baseIndex*4;
    paramArrGeom[newIndex+0]=xv;
}
void setFYGeom(int baseIndex, float yv)
{
    int newIndex=baseIndex*4;
    paramArrGeom[newIndex+1]=yv;
}
void setFZGeom(int baseIndex, float zv)
{
    int newIndex=baseIndex*4;
    paramArrGeom[newIndex+2]=zv;
}
void setFWGeom(int baseIndex, float wv)
{
    int newIndex=baseIndex*4;
    paramArrGeom[newIndex+3]=wv;
}


float getFXGeom(int baseIndex)
{
    int newIndex=baseIndex*4;
    return paramArrGeom[newIndex+0];
}
float getFYGeom(int baseIndex)
{
    int newIndex=baseIndex*4;
    return paramArrGeom[newIndex+1];
}
float getFZGeom(int baseIndex)
{
    int newIndex=baseIndex*4;
    return paramArrGeom[newIndex+2];
}
float getFWGeom(int baseIndex)
{
    int newIndex=baseIndex*4;
    return paramArrGeom[newIndex+3];
}

void resetGeom()
{
    int i;
    geomStep=0;
    geomOrigOffset.setFXYZ(0.0f);
    geomPoints[0].setFXYZ(0.0f, 0.0f, 0.0f);

    for(i=0; i<FLOATS_PER_TEMPLATE; i++)
    {
        paramArrGeom[i]=defaultTemplate[i];
    }
}

FIVector4* getGeomRef(int templateId, int enumVal)
{
	return &(primTemplateStack[templateId*E_PRIMTEMP_LENGTH+enumVal]);
}

bool getPrimTemplateString()
{
	primTemplateStack.clear();

	JSONValue* jv=g_settings.fetchJSONData("primTemplates.js", true);
	JSONValue* jv2=jv->Child("primTemplates");
	JSONValue* jv3=NULL;
	JSONValue* jv4=NULL;

	int i;
	int j;
	int k;

	int tempInd;

	int numTemplates=jv2->CountChildren();
	int numProps=0;
	int numFields=0;

	int propCount=0;
	int maxProps=numTemplates*E_PRIMTEMP_LENGTH;

	float curNumber;

	std::string resString="";
	resString.append("const int PRIMS_PER_MACRO = "+i__s(GameState::gameFluid[E_FID_BIG]->primsPerMacro)+";\n");
	resString.append("const int VECS_PER_PRIM = "+i__s(GameState::gameFluid[E_FID_BIG]->floatsPerPrimEntry/4)+";\n");
	resString.append("const float PRIM_DIV = "+i__s(GameState::gameFluid[E_FID_BIG]->primDiv)+".0;\n");


	for(i=0; i<numTemplates; i++)
	{
		jv3=jv2->Child(i);
		numProps=jv3->CountChildren()-1; //minus one for comment
		if(numProps!=E_PRIMTEMP_LENGTH)
		{
			std::cout<<"ERROR: invalid number of properties\n";
			return false;
		}

		tempInd=propCount;

		for(j=0; j<numProps; j++)
		{
			jv4=jv3->Child(j);
			numFields=jv4->CountChildren();
			if(numFields!=4)
			{
				std::cout<<"ERROR: invalid number of fields\n";
				return false;
			}

			primTemplateStack.push_back(FIVector4());

			for(k=0; k<numFields; k++)
			{
				curNumber=(float)jv4->Child(k)->number_value;
				primTemplateStack.back().setIndex(k, curNumber);
			}

			propCount++;

		}

		primTemplateStack[tempInd+E_PRIMTEMP_VISMIN].multXYZRef(
			&(primTemplateStack[tempInd+E_PRIMTEMP_BOUNDSMIN])
		);
		primTemplateStack[tempInd+E_PRIMTEMP_VISMAX].multXYZRef(
			&(primTemplateStack[tempInd+E_PRIMTEMP_BOUNDSMAX])
		);


	}

	resString.append("const vec4 primTemp["+i__s(numTemplates*E_PRIMTEMP_LENGTH)+"] = vec4[](\n");

	numFields=4;

	for(i=0; i<primTemplateStack.size(); i++)
	{
		resString.append("vec4(");
		for(k=0; k<numFields; k++)
		{
			curNumber=primTemplateStack[i][k];
			resString.append(f__s(curNumber));
			if(k<numFields-1)
			{
				resString.append(",");
			}
		}
		resString.append(")");

		if(i<(primTemplateStack.size()-1))
		{
			resString.append(",");
		}

		resString.append("\n");
	}

	resString.append(");\n");

	//std::cout << resString << "\n";

	includeMap["primTemplates"]=resString;

//    GameState::gw->updatePrimTBOData();
    updatePrimTBOData();

	return true;
}

void updatePrimTBOData()
{
    int i;
    int j;

    for(i=0; i<primTemplateStack.size(); i++)
    {

        for(j=0; j<4; j++)
        {
            primTBOData[i*4+j]=primTemplateStack[i][j];
        }

    }

    Renderer::primTBO.update(primTBOData, NULL, (int)primTemplateStack.size()*4*4);
}

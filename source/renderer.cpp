#include "voxelquest/renderer.h"
#include "voxelquest/materials.h"
#include "voxelquest/geom.h"
#include "voxelquest/fileio.h"
#include "voxelquest/jsonhelpers.h"
#include "voxelquest/settings.h"
#include "voxelquest/fbos.h"
#include "voxelquest/bullethelpers.h"
#include "voxelquest/helperfuncs.h"
#include "voxelquest/gamestate.h"
#include "voxelquest/gameentmanager.h"
#include "voxelquest/glmhelpers.h"
#include "voxelquest/gameworld.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <algorithm>
#include <cassert>

FIVector4 tempVec1;
FIVector4 tempVec2;
FIVector4 tempVec3;

struct CompareStruct
{
    bool operator()(const std::string& first, const std::string& second)
    {
        return first.compare(second)<0;//first.size() < second.size();
    }
};

int getMatrixInd(int col, int row)
{
    return col*4+row;
}

const static int RASTER_LOW_SCALE_FACTOR=2;

int Renderer::baseW;
int Renderer::baseH;
int Renderer::lastW;
int Renderer::lastH;

float Renderer::FOV=45.0f;
float Renderer::heightOfNearPlane=1.0f;

Matrix4 Renderer::pmMatrix;

GLuint Renderer::fsqDL;
VBOWrapper Renderer::fsQuad;
TBOWrapper Renderer::limbTBO;
TBOWrapper Renderer::primTBO;

float Renderer::clipDist[2];
bool Renderer::perspectiveOn=false;
bool Renderer::lastPersp=false;
bool Renderer::sphereMapOn=false;
bool Renderer::drawOrient=false;
bool Renderer::bakeParamsOn=true;

int Renderer::forceShadowUpdate=0;
FIVector4 Renderer::lastLightPos;

float Renderer::cameraShake=0.0f;
Timer Renderer::shakeTimer;

GLfloat Renderer::camRotation[2];
GLfloat Renderer::curCamRotation[2];
float Renderer::cameraZoom=1.0f;
FIVector4 *Renderer::cameraPos;
FIVector4 Renderer::tempLerpPos;
FIVector4 Renderer::camLerpPos;
FIVector4 Renderer::resultCameraPos;
FIVector4 Renderer::targetCameraPos;
FIVector4 Renderer::baseCameraPos;

float Renderer::scaleFactor=1.0f;
FIVector4 Renderer::lookAtVec;
Matrix4 Renderer::viewMatrix;
Matrix4 Renderer::projMatrix;
Matrix4 Renderer::curObjMatrix;
Matrix3 Renderer::curObjMatrix3;
GLint Renderer::viewport[4];
float Renderer::viewMatrixDI[16];

FIVector4 Renderer::lightVec;
FIVector4 Renderer::lightVecOrig;
FIVector4 Renderer::lightLookAt;
Matrix4 Renderer::lightView;

int Renderer::currentFBOResolutionX;
int Renderer::currentFBOResolutionY;

int Renderer::shadersAreLoaded=0;
std::map<std::string, Shader*> Renderer::shaderMap;
std::string Renderer::curShader;
Shader *Renderer::curShaderPtr=nullptr;
std::vector<std::string> Renderer::shaderStrings;
std::vector<std::string> Renderer::shaderTextureIds;
int Renderer::readyToRecompile=0;

FIVector4 Renderer::bufferDim;
FIVector4 Renderer::bufferDimTarg;
FIVector4 Renderer::bufferDimHalf;
FIVector4 Renderer::bufferModDim;
FIVector4 Renderer::bufferRenderDim;

FIVector4 Renderer::rasterLowDim;
//GLuint Renderer::volIdMat;

bool Renderer::LAST_COMPILE_ERROR=false;

void Renderer::init(int width, int height)
{
    setWH(width, height);

    fsqDL=glGenLists(1);
    glNewList(fsqDL, GL_COMPILE);
    drawFSQuadOffset(0.0f, 0.0f, 1.0f);
    glEndList();

    clipDist[0]=1.0f;
    clipDist[1]=512.0f;

    lightVec.setFXYZ(0.3f, 0.4f, -1.0f);
    lightVec.normalize();
    lightVecOrig.copyFrom(&lightVec);

    bufferDim.setIXY(width, height);
    rasterLowDim.copyIntDiv(&bufferDim, RASTER_LOW_SCALE_FACTOR);
    bufferDimTarg.setIXY(width, height);

    bufferDimHalf.setIXY(width/2, height/2);
    bufferModDim.copyIntMult(&bufferDim, 1);
    bufferRenderDim.copyIntDiv(&bufferDimTarg, RENDER_SCALE_FACTOR);

    if(USE_SPHERE_MAP)
    {
        shaderStrings.push_back("PrimShader_330_DOTER_USESPHEREMAP");
    }
    else
    {
        //shaderStrings.push_back("PrimShader_330_DOTER_DOPOLY");
        shaderStrings.push_back("PrimShader_330_DOTER");
    }

    shaderStrings.push_back("PrimShader_330_DOPRIM");
    shaderStrings.push_back("SolidCombineShader");
    shaderStrings.push_back("CylBBShader");
    shaderStrings.push_back("FXAAShader");
    shaderStrings.push_back("TerGenShader");
    shaderStrings.push_back("GUIShader");
    shaderStrings.push_back("MedianShader");
    shaderStrings.push_back("MergeShader");
    shaderStrings.push_back("TopoShader");
    shaderStrings.push_back("PointShader");
    shaderStrings.push_back("NearestShader");
    shaderStrings.push_back("LightShader");
    shaderStrings.push_back("RoadShader");
    shaderStrings.push_back("SkeletonShader");
    shaderStrings.push_back("DilateShader");
    shaderStrings.push_back("TerrainMix");
    shaderStrings.push_back("Simplex2D");
    shaderStrings.push_back("WaveHeightShader");
    shaderStrings.push_back("WaterShader");
    shaderStrings.push_back("WaterShaderCombine");
    shaderStrings.push_back("CopyShader");
    shaderStrings.push_back("CopyShader2");
    shaderStrings.push_back("CopyShader3");
    shaderStrings.push_back("NoiseShader");
    shaderStrings.push_back("MapBorderShader");
    shaderStrings.push_back("BillboardShader");
    shaderStrings.push_back("PreLightingShader");
    shaderStrings.push_back("PostLightingShader");
    shaderStrings.push_back("BlurShader");
    shaderStrings.push_back("RadiosityShader");
    shaderStrings.push_back("RadiosityCombineShader");
    shaderStrings.push_back("FogShader");
    shaderStrings.push_back("OctShader");
    shaderStrings.push_back("RasterShader");
    shaderStrings.push_back("HolderShader");
    shaderStrings.push_back("BasicPrimShader");
    shaderStrings.push_back("BasicLimbShader");
    shaderStrings.push_back("ShadowMapShader");
    shaderStrings.push_back("GridShader");
    shaderStrings.push_back("GeomShader");
    shaderStrings.push_back("BoxShader");
    shaderStrings.push_back("PolyShader");
    shaderStrings.push_back("PolyCombineShader");

    CompareStruct compareStruct;

    std::sort(shaderStrings.begin(), shaderStrings.end(), compareStruct);

    shaderTextureIds.push_back("Texture0");
    shaderTextureIds.push_back("Texture1");
    shaderTextureIds.push_back("Texture2");
    shaderTextureIds.push_back("Texture3");
    shaderTextureIds.push_back("Texture4");
    shaderTextureIds.push_back("Texture5");
    shaderTextureIds.push_back("Texture6");
    shaderTextureIds.push_back("Texture7");
    shaderTextureIds.push_back("Texture8");
    shaderTextureIds.push_back("Texture9");
    shaderTextureIds.push_back("Texture10");
    shaderTextureIds.push_back("Texture11");
    shaderTextureIds.push_back("Texture12");
    shaderTextureIds.push_back("Texture13");
    shaderTextureIds.push_back("Texture14");
    shaderTextureIds.push_back("Texture15");

    for(int i=0; i<shaderStrings.size(); i++)
    {
        shaderMap.insert(std::pair<std::string, Shader*>(shaderStrings[i], new Shader(nullptr)));
    }

    doShaderRefresh(false);
}

void Renderer::setWH(int w, int h)
{
    baseW=w;
    baseH=h;
}

void Renderer::reshape(int w, int h)
{
    std::cout<<"reshape\n";

    setWH(w, h);

//    screenWidth=w;
//    screenHeight=h;

    setMatrices(baseW, baseH);
}

void Renderer::updateCamVals()
{
    if(camLerpPos.getFX()>((float)g_settings.cellsPerWorld)/2.0)
    {
        camLerpPos.setFX(camLerpPos.getFX()-((float)g_settings.cellsPerWorld));
    }
    if(camLerpPos.getFX()<-((float)g_settings.cellsPerWorld)/2.0)
    {
        camLerpPos.setFX(camLerpPos.getFX()+((float)g_settings.cellsPerWorld));
    }
    if(camLerpPos.getFY()>((float)g_settings.cellsPerWorld)/2.0)
    {
        camLerpPos.setFY(camLerpPos.getFY()-((float)g_settings.cellsPerWorld));
    }
    if(camLerpPos.getFY()<-((float)g_settings.cellsPerWorld)/2.0)
    {
        camLerpPos.setFY(camLerpPos.getFY()+((float)g_settings.cellsPerWorld));
    }

    if(cameraPos->getFX()>((float)g_settings.cellsPerWorld)/2.0)
    {
        cameraPos->setFX(cameraPos->getFX()-((float)g_settings.cellsPerWorld));
    }
    if(cameraPos->getFX()<-((float)g_settings.cellsPerWorld)/2.0)
    {
        cameraPos->setFX(cameraPos->getFX()+((float)g_settings.cellsPerWorld));
    }
    if(cameraPos->getFY()>((float)g_settings.cellsPerWorld)/2.0)
    {
        cameraPos->setFY(cameraPos->getFY()-((float)g_settings.cellsPerWorld));
    }
    if(cameraPos->getFY()<-((float)g_settings.cellsPerWorld)/2.0)
    {
        cameraPos->setFY(cameraPos->getFY()+((float)g_settings.cellsPerWorld));
    }

    if(g_settings.smoothMove)
    {
        // tempLerpPos.copyFrom(cameraPos);
        // tempLerpPos.lerpXYZ(&camLerpPos,0.1f);

        //&camLerpPos);
        // tempLerpPos.addXYZRef(cameraPos,-1.0f);
        // tempLerpPos.multXYZ(timeDelta*8.0f);

        cameraPos->lerpXYZ(&camLerpPos, getConst(E_CONST_CAM_LERP_AMOUNT));

        GameState::gw->amountInvalidMove=camLerpPos.length();
        GameState::gw->depthInvalidMove=GameState::gw->amountInvalidMove>0.01f;

        //cameraPos->addXYZRef(&tempLerpPos);
    }
    else
    {
        cameraPos->copyFrom(&camLerpPos);
    }

    GameState::lastHolderPos.copyIntDiv(cameraPos, g_settings.cellsPerHolder);

    float resultShake=-cameraShake*sinf((float)shakeTimer.getElapsedTimeInMilliSec()/20.0f);

    resultCameraPos.copyFrom(cameraPos);
    resultCameraPos.addXYZ(0.0f, 0.0f, resultShake*0.5f);

    cameraShake+=(0.0f-cameraShake)*(float)GameState::timeDelta*8.0f;
}

void Renderer::handleMovement()
{

}

GLfloat Renderer::getCamRot(int ind)
{
    return curCamRotation[ind];
}

void Renderer::performCamShake(BaseObj* ge, float fp)
{
    float lastCamShake=cameraShake;

    cameraShake=std::max(
        cameraShake,
        fp
    );

    if(cameraShake>lastCamShake)
    {
        shakeTimer.stop();
        shakeTimer.start();
    }
}

void Renderer::bindShader(std::string shaderName)
{
    int i;
    int totSize;

    if(shaderMap.find(shaderName)==shaderMap.end())
    {
        std::cout<<"invalid shader name "<<shaderName<<"\n";
        assert(false);
        exit(0);
    }

    if(shadersAreLoaded)
    {
        curShader=shaderName;
        curShaderPtr=shaderMap[curShader];
        curShaderPtr->bind();

        totSize=(int)curShaderPtr->paramVec.size();

        if(bakeParamsOn)
        {

        }
        else
        {
            for(i=0; i<totSize; i++)
            {

                // if (curShaderPtr->paramVec[i].compare("lightColBNight")) {
                // 	cout << curShaderPtr->paramMap[curShaderPtr->paramVec[i]] << "\n";
                // }

                setShaderFloat(
                    curShaderPtr->paramVec[i],
                    curShaderPtr->paramMap[curShaderPtr->paramVec[i]]
                );
            }
        }

    }

}

void Renderer::unbindShader()
{
    if(shadersAreLoaded)
    {
        curShaderPtr->unbind();
    }
}

void Renderer::bindFBODirect(FBOSet *fbos, int doClear)
{
    setMatrices(fbos->width, fbos->height);

    fbos->bind(doClear);
    currentFBOResolutionX=fbos->width;
    currentFBOResolutionY=fbos->height;
}

void Renderer::setShaderArrayfVec3(std::string paramName, float *x, int count)
{
    curShaderPtr->setShaderArrayfVec3(paramName, x, count);
}
void Renderer::setShaderArrayfVec4(std::string paramName, float *x, int count)
{
    curShaderPtr->setShaderArrayfVec4(paramName, x, count);
}
void Renderer::setShaderMatrix4x4(std::string paramName, float *x, int count)
{
    curShaderPtr->setShaderMatrix4x4(paramName, x, count);
}
void Renderer::setShaderMatrix3x3(std::string paramName, float *x, int count)
{
    curShaderPtr->setShaderMatrix3x3(paramName, x, count);
}
void Renderer::setShaderArray(std::string paramName, float *x, int count)
{
    curShaderPtr->setShaderArray(paramName, x, count);
}

GLint Renderer::getShaderLoc(std::string paramName)
{
    return curShaderPtr->getShaderLoc(paramName);
}

void Renderer::setShaderFloat(std::string paramName, float x)
{
    curShaderPtr->setShaderFloat(paramName, x);
}
void Renderer::setShaderInt(std::string paramName, int x)
{
    curShaderPtr->setShaderInt(paramName, x);
}
void Renderer::setShaderfVec2(std::string paramName, FIVector4 *v)
{
    curShaderPtr->setShaderfVec2(paramName, v);
}
void Renderer::setShaderVec2(std::string paramName, float x, float y)
{
    curShaderPtr->setShaderVec2(paramName, x, y);
}
void Renderer::setShaderVec3(std::string paramName, float x, float y, float z)
{
    curShaderPtr->setShaderVec3(paramName, x, y, z);
}
void Renderer::setShaderfVec3(std::string paramName, FIVector4 *v)
{
    curShaderPtr->setShaderfVec3(paramName, v);
}
void Renderer::setShaderbtVec3(std::string paramName, btVector3 v)
{
    curShaderPtr->setShaderbtVec3(paramName, v);
}

void Renderer::setShaderVec4(std::string paramName, float x, float y, float z, float w)
{
    curShaderPtr->setShaderVec4(paramName, x, y, z, w);
}
void Renderer::setShaderfVec4(std::string paramName, FIVector4 *v)
{
    curShaderPtr->setShaderfVec4(paramName, v);
}



void Renderer::setShaderFloatUB(std::string paramName, float x)
{
    curShaderPtr->setShaderFloatUB(paramName, x);
}
void Renderer::setShaderfVec4UB(std::string paramName, FIVector4 *v)
{
    curShaderPtr->setShaderfVec4UB(paramName, v);
}



void updateUniformBlock(int ubIndex, int ubDataSize=-1)
{
    Renderer::curShaderPtr->updateUniformBlock(ubIndex, ubDataSize);
}
void invalidateUniformBlock(int ubIndex)
{
    Renderer::curShaderPtr->invalidateUniformBlock(ubIndex);
}
void beginUniformBlock(int ubIndex)
{
    Renderer::curShaderPtr->beginUniformBlock(ubIndex);
}
bool Renderer::wasUpdatedUniformBlock(int ubIndex)
{

    return curShaderPtr->wasUpdatedUniformBlock(ubIndex);

}


void Renderer::setShaderTBO(int multitexNumber, GLuint tbo_tex, GLuint tbo_buf, bool isFloat)
{
    if(shadersAreLoaded)
    {
        glActiveTexture(GL_TEXTURE0+multitexNumber);
        glBindTexture(GL_TEXTURE_2D, tbo_tex);
        //glBindBuffer(GL_TEXTURE_BUFFER, tboIndices);
        if(tbo_tex!=0)
        {
            if(isFloat)
            {
                glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, tbo_buf);
            }
            else
            {
                glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, tbo_buf);
            }

        }
        curShaderPtr->setShaderInt(shaderTextureIds[multitexNumber], multitexNumber);
    }
}

void Renderer::setShaderTexture(int multitexNumber, uint texId)
{
    if(shadersAreLoaded)
    {
        glActiveTexture(GL_TEXTURE0+multitexNumber);
        glBindTexture(GL_TEXTURE_2D, texId);
        curShaderPtr->setShaderInt(shaderTextureIds[multitexNumber], multitexNumber);
    }
}

void Renderer::setShaderTexture3D(int multitexNumber, uint texId)
{
    if(shadersAreLoaded)
    {
        glActiveTexture(GL_TEXTURE0+multitexNumber);
        glBindTexture(GL_TEXTURE_3D, texId);
        curShaderPtr->setShaderInt(shaderTextureIds[multitexNumber], multitexNumber);
    }
}

void Renderer::doShaderRefresh(bool doBake)
{
    std::string globString;

    loadConstants();

    LAST_COMPILE_ERROR=false;

    readyToRecompile=0;

    int i;
    int j;

    Shader* curShader;

    refreshIncludeMap();


    // this is expensive
    for(i=0; i<shaderStrings.size(); i++)
    {
        shaderMap[shaderStrings[i]]->init(shaderStrings[i], doBake, &includeMap);
    }

    //"../src/glsl/" + shaderStrings[i] + ".c"

    if(DO_SHADER_DUMP)
    {
        std::cout<<"SHADER_DUMP\n";
        saveFileString("..\\data\\temp.txt", &globString);
    }


    shadersAreLoaded=1;
    readyToRecompile=1;

    if(LAST_COMPILE_ERROR)
    {

    }
    else
    {

        // load saved data (if exists)
        // merge saved data with existing data (if exists)
        // save merged data to saved data



        std::string stringBuf="{\n\t\"params\":[\n";


        CompareStruct compareStruct;

        for(i=0; i<shaderStrings.size(); i++)
        {
            curShader=shaderMap[shaderStrings[i]];

            std::sort(curShader->paramVec.begin(), curShader->paramVec.end(), compareStruct);

            for(j=0; j<curShader->paramVec.size(); j++)
            {
                stringBuf.append("\t\t{");
                stringBuf.append("\"shaderName\":\""+shaderStrings[i]+"\",");
                stringBuf.append("\"paramName\":\""+curShader->paramVec[j]+"\",");
                stringBuf.append("\"uid\":\"$shaderParams."+shaderStrings[i]+"."+curShader->paramVec[j]+"\"");
                stringBuf.append("},\n");
            }
        }

        stringBuf[stringBuf.size()-2]=' ';


        stringBuf.append("\t]\n}\n\n");

        // this should automatically clear the key
        // and deallocate existing entries

        processJSONFromString(
            &stringBuf,
            &(g_settings.externalJSON["E_SDT_SHADERPARAMS"].jv)
        );
    }
}

void Renderer::refreshIncludeMap()
{
    getMaterialString();
    getPrimTemplateString();
}

void Renderer::idrawCrossHairs(FIVector4 originVec, float radius)
{
    FIVector4 minV;
    FIVector4 maxV;

    float xm;
    float ym;
    float zm;

    float maxRad=4096.0f;

    int i;
    for(i=0; i<3; i++)
    {

        switch(i)
        {
        case 0:
            xm=maxRad;
            ym=radius;
            zm=radius;
            break;
        case 1:
            xm=radius;
            ym=maxRad;
            zm=radius;
            break;
        case 2:
            xm=radius;
            ym=radius;
            zm=maxRad;
            break;
        }

        minV.setFXYZ(
            originVec.getFX()-xm,
            originVec.getFY()-ym,
            originVec.getFZ()-zm
        );

        maxV.setFXYZ(
            originVec.getFX()+xm,
            originVec.getFY()+ym,
            originVec.getFZ()+zm
        );

        drawBox(&minV, &maxV);
    }


}

void Renderer::drawLine(FIVector4 *p0, FIVector4 *p1)
{


    glBegin(GL_LINES);
    glMultiTexCoord3f(GL_TEXTURE0, 0.0f, 0.0f, 0.0f);
    glVertex3f(p0->getFX(), p0->getFY(), p0->getFZ());
    glMultiTexCoord3f(GL_TEXTURE0, 0.0f, 0.0f, 1.0f);
    glVertex3f(p1->getFX(), p1->getFY(), p1->getFZ());
    glEnd();
}


void Renderer::drawCubeCentered(FIVector4 *originVec, float radius)
{
    FIVector4 minV;
    FIVector4 maxV;

    minV.setFXYZ(
        originVec->getFX()-radius,
        originVec->getFY()-radius,
        originVec->getFZ()-radius
    );

    maxV.setFXYZ(
        originVec->getFX()+radius,
        originVec->getFY()+radius,
        originVec->getFZ()+radius
    );

    drawBox(&minV, &maxV);
}

void Renderer::drawBoxUp(FIVector4 originVec, float radiusX, float radiusY, float diamZ)
{
    FIVector4 minV;
    FIVector4 maxV;

    minV.setFXYZ(
        originVec.getFX()-radiusX,
        originVec.getFY()-radiusY,
        originVec.getFZ()
    );

    maxV.setFXYZ(
        originVec.getFX()+radiusX,
        originVec.getFY()+radiusY,
        originVec.getFZ()+diamZ
    );

    drawBox(&minV, &maxV);
}

void Renderer::drawBoxMinMax(
    btVector3 v0,
    btVector3 v1
)
{
    tempVec1=convertToVQV(v0);
    tempVec2=convertToVQV(v1);
    drawBox(&tempVec1, &tempVec2);
}

void Renderer::drawBoxRad(
    btVector3 v0,
    btVector3 v1
)
{
    tempVec1=convertToVQV(v0-v1);
    tempVec2=convertToVQV(v0+v1);
    drawBox(&tempVec1, &tempVec2);
}

void Renderer::drawBox(
    FIVector4 *v0,
    FIVector4 *v1,
    int faceFlag
)
{


    float minX=std::min(v0->getFX(), v1->getFX());
    float minY=std::min(v0->getFY(), v1->getFY());
    float minZ=std::min(v0->getFZ(), v1->getFZ());

    float maxX=std::max(v0->getFX(), v1->getFX());
    float maxY=std::max(v0->getFY(), v1->getFY());
    float maxZ=std::max(v0->getFZ(), v1->getFZ());


    bool drawFront=false;
    bool drawBack=false;

    switch(faceFlag)
    {
    case 0:
        drawBack=true;
        break;
    case 1:
        drawFront=true;
        break;
    case 2:
        drawBack=true;
        drawFront=true;
        break;
    }

    glBegin(GL_QUADS);


    if(drawFront)
    {
        // front
        glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, 1.0f);
        glVertex3f(minX, minY, maxZ);
        glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, 1.0f);
        glVertex3f(maxX, minY, maxZ);
        glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
        glVertex3f(maxX, maxY, maxZ);
        glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, 1.0f);
        glVertex3f(minX, maxY, maxZ);
        // right
        glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, 1.0f);
        glVertex3f(maxX, minY, maxZ);
        glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, -1.0f);
        glVertex3f(maxX, minY, minZ);
        glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, -1.0f);
        glVertex3f(maxX, maxY, minZ);
        glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
        glVertex3f(maxX, maxY, maxZ);
        // top
        glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, 1.0f);
        glVertex3f(minX, maxY, maxZ);
        glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, 1.0f);
        glVertex3f(maxX, maxY, maxZ);
        glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, -1.0f);
        glVertex3f(maxX, maxY, minZ);
        glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, -1.0f);
        glVertex3f(minX, maxY, minZ);
    }


    if(drawBack)
    {
        // back
        glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, -1.0f);
        glVertex3f(minX, minY, minZ);
        glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, -1.0f);
        glVertex3f(maxX, minY, minZ);
        glMultiTexCoord3f(GL_TEXTURE0, 1.0f, 1.0f, -1.0f);
        glVertex3f(maxX, maxY, minZ);
        glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, -1.0f);
        glVertex3f(minX, maxY, minZ);
        // left
        glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, 1.0f);
        glVertex3f(minX, minY, maxZ);
        glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, -1.0f);
        glVertex3f(minX, minY, minZ);
        glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, -1.0f);
        glVertex3f(minX, maxY, minZ);
        glMultiTexCoord3f(GL_TEXTURE0, -1.0f, 1.0f, 1.0f);
        glVertex3f(minX, maxY, maxZ);
        // bottom
        glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, 1.0f);
        glVertex3f(minX, minY, maxZ);
        glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, 1.0f);
        glVertex3f(maxX, minY, maxZ);
        glMultiTexCoord3f(GL_TEXTURE0, 1.0f, -1.0f, -1.0f);
        glVertex3f(maxX, minY, minZ);
        glMultiTexCoord3f(GL_TEXTURE0, -1.0f, -1.0f, -1.0f);
        glVertex3f(minX, minY, minZ);
    }



    glEnd();
}

void Renderer::drawQuadWithCoords(
    FIVector4* p0,
    FIVector4* p1,
    FIVector4* p2,
    FIVector4* p3,

    float tx1,
    float ty1,
    float tx2,
    float ty2

)
{
    //glColor4f(1, 1, 1, 1);
    //glNormal3f(0, 0, 1);

    glBegin(GL_QUADS);

    glTexCoord2f(tx1, ty1);
    glVertex3f(p0->getFX(), p0->getFY(), p0->getFZ());

    glTexCoord2f(tx2, ty1);
    glVertex3f(p1->getFX(), p1->getFY(), p1->getFZ());

    glTexCoord2f(tx2, ty2);
    glVertex3f(p2->getFX(), p2->getFY(), p2->getFZ());

    glTexCoord2f(tx1, ty2);
    glVertex3f(p3->getFX(), p3->getFY(), p3->getFZ());

    glEnd();
}

void Renderer::drawQuadBounds(
    float fx1,
    float fy1,
    float fx2,
    float fy2,

    float fz
)
{
    //glColor4f(1, 1, 1, 1);
    //glNormal3f(0, 0, 1);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(fx1, fy1, fz);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(fx2, fy1, fz);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(fx2, fy2, fz);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(fx1, fy2, fz);

    glEnd();
}


void Renderer::drawFSQuad()
{
    glCallList(fsqDL);
}


void Renderer::drawFSQuadOffset(
    float xOff,
    float yOff,
    float zm
)
{
    float fx0=(xOff-1.0f) * zm;
    float fy0=(yOff-1.0f) * zm;
    float fx1=(xOff+1.0f) * zm;
    float fy1=(yOff+1.0f) * zm;

    glBegin(GL_QUADS);
    //glColor4f(1, 1, 1, 1);
    //glNormal3f(0, 0, 1);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(fx0, fy0, 0.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(fx1, fy0, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(fx1, fy1, 0.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(fx0, fy1, 0.0f);

    glEnd();
}

void Renderer::bindFBO(std::string fboName, int swapFlag, int doClear)
{

    FBOSet *fbos;

    if(swapFlag==-1)
    {
        fbos=FBOManager::getFBOByName(fboName);
    }
    else
    {

        if(swapFlag==0)
        {
            fbos=FBOManager::getFBOByName(fboName+"1");
        }
        else
        {
            fbos=FBOManager::getFBOByName(fboName+"0");
        }

    }

    if(fbos)
    {
        bindFBODirect(fbos, doClear);
    }
    else
    {
        doTrace("bindFBO: Invalid FBO Name");
    }


}

void Renderer::unbindFBO()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    setMatrices(baseW, baseH);

}

void Renderer::sampleFBODirect(
    FBOSet *fbos,
    int offset, /* write offset */
    int _minOff, /* read min */
    int _maxOff /* read max */

)
{
    int i;

    int minOff=_minOff;
    int maxOff=_maxOff;
    if(maxOff==-1)
    {
        maxOff=fbos->numBufs;
    }

    if(shadersAreLoaded)
    {
        for(i=minOff; i<maxOff; i++)
        {
            setShaderTexture(i-minOff+offset, fbos->fbos[i].color_tex);
        }
    }
}

void Renderer::sampleFBO(
    std::string fboName,
    int offset,
    int swapFlag,
    int minOff,
    int maxOff
)
{
    FBOSet *fbos;

    if(swapFlag==-1)
    {
        fbos=FBOManager::getFBOByName(fboName);//&(fboMap[fboName]);
    }
    else
    {
        if(swapFlag==0)
        {
            fbos=FBOManager::getFBOByName(fboName+"0");
        }
        else
        {
            fbos=FBOManager::getFBOByName(fboName+"1");
        }
    }

    if(fbos)
    {
        sampleFBODirect(fbos, offset, minOff, maxOff);
    }
    else
    {
        doTrace("sampleFBO: Invalid FBO Name");
    }
}

void Renderer::unsampleFBO(
    std::string fboName,
    int offset,
    int swapFlag,
    int minOff,
    int maxOff
)
{
    FBOSet *fbos;

    if(swapFlag==-1)
    {
        fbos=FBOManager::getFBOByName(fboName);
    }
    else
    {
        if(swapFlag==0)
        {
            fbos=FBOManager::getFBOByName(fboName+"0");
        }
        else
        {
            fbos=FBOManager::getFBOByName(fboName+"1");
        }
    }

    if(fbos)
    {
        unsampleFBODirect(fbos, offset, minOff, maxOff);
    }
    else
    {
        doTrace("unsampleFBO: Invalid FBO Name");
    }
}

void Renderer::unsampleFBODirect(
    FBOSet *fbos,
    int offset, /* write offset */
    int _minOff, /* read min */
    int _maxOff /* read max */

)
{
    int i;

    int minOff=_minOff;
    int maxOff=_maxOff;
    if(maxOff==-1)
    {
        maxOff=fbos->numBufs;
    }

    if(shadersAreLoaded)
    {
        for(i=maxOff-1; i>=minOff; i--)
        {
            setShaderTexture(i-minOff+offset, 0);
        }
    }
}

void Renderer::copyFBO(std::string src, std::string dest, int num)
{
    bindShader("CopyShader");
    bindFBO(dest);
    //sampleFBO(src, 0);
    setShaderTexture(0, FBOManager::getFBOWrapper(src, num)->color_tex);
    drawFSQuad();
    setShaderTexture(0, 0);
    unbindFBO();
    unbindShader();
}

void Renderer::copyFBO2(std::string src, std::string dest, int num1, int num2)
{
    bindShader("CopyShader2");
    bindFBO(dest);
    setShaderTexture(0, FBOManager::getFBOWrapper(src, num1)->color_tex);
    setShaderTexture(1, FBOManager::getFBOWrapper(src, num2)->color_tex);
    drawFSQuad();
    setShaderTexture(1, 0);
    setShaderTexture(0, 0);
    unbindFBO();
    unbindShader();
}

void Renderer::copyFBO3(std::string src, std::string dest, int num1, int num2, int num3)
{
    bindShader("CopyShader3");
    bindFBO(dest);
    setShaderTexture(0, FBOManager::getFBOWrapper(src, num1)->color_tex);
    setShaderTexture(1, FBOManager::getFBOWrapper(src, num2)->color_tex);
    setShaderTexture(2, FBOManager::getFBOWrapper(src, num3)->color_tex);
    drawFSQuad();
    setShaderTexture(2, 0);
    setShaderTexture(1, 0);
    setShaderTexture(0, 0);
    unbindFBO();
    unbindShader();
}

void Renderer::getMatrixFromFBO(std::string fboName)
{
    FBOSet *fbos=FBOManager::getFBOByName(fboName);
    setMatrices(fbos->width, fbos->height);
}

void Renderer::drawFBO(std::string fboName, int ind, float zm, int swapFlag)
{
    if(swapFlag==-1)
    {
        drawFBOOffset(fboName, ind, 0.0f, 0.0f, zm);
    }
    else
    {
        if(swapFlag==0)
        {
            drawFBOOffset(fboName+"1", ind, 0.0f, 0.0f, zm);
        }
        else
        {
            drawFBOOffset(fboName+"0", ind, 0.0f, 0.0f, zm);
        }

    }
}

void Renderer::drawFBOOffsetDirect(FBOSet *fbos, int ind, float xOff, float yOff, float zm)
{

    glBindTexture(GL_TEXTURE_2D, fbos->fbos[ind].color_tex);
    //glClearColor(0.2,0.2,0.2,0.0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawFSQuadOffset(xOff, yOff, zm);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::drawFBOOffset(std::string fboName, int ind, float xOff, float yOff, float zm)
{
    FBOSet *fbos=FBOManager::getFBOByName(fboName);

    if(fbos)
    {
        drawFBOOffsetDirect(fbos, ind, xOff, yOff, zm);
    }
    else
    {
        doTrace("drawFBOOffsetDirect: Invalid FBO Name");
    }

}


void Renderer::ComputeFOVProjection(
    float* result,
    float fov,
    float aspect,
    float nearDist,
    float farDist,
    bool leftHanded
)
{
    //
    // General form of the Projection Matrix
    //
    // uh = Cot( fov/2 ) == 1/Tan(fov/2)
    // uw / uh = 1/aspect
    // 
    //   uw         0       0       0    	0  1  2  3
    //    0        uh       0       0    	4  5  6  7
    //    0         0      f/(f-n)  1    	8  9  10 11
    //    0         0    -fn/(f-n)  0   	12 13 14 15
    //
    // Make result to be identity first

    int i;
    int j;

    float val;

    for(i=0; i<4; i++)
    {
        for(j=0; j<4; j++)
        {

            if(i==j)
            {
                val=1.0f;
            }
            else
            {
                val=0.0f;
            }

            result[getMatrixInd(i, j)]=val;
        }
    }


    float frustumDepth=farDist-nearDist;
    float oneOverDepth=1/frustumDepth;

    result[getMatrixInd(1, 1)]=1/tan(0.5f * fov);
    result[getMatrixInd(0, 0)]=(leftHanded?1:-1) * result[getMatrixInd(1, 1)]/aspect;
    result[getMatrixInd(2, 2)]=farDist*oneOverDepth;
    result[getMatrixInd(3, 2)]=(-farDist*nearDist) * oneOverDepth;
    result[getMatrixInd(2, 3)]=1;
    result[getMatrixInd(3, 3)]=0;
}

void Renderer::getLSMatrix(FIVector4* lightPosParam, Matrix4 &lsMat, float orthoSize)
{

    FIVector4 newCamPos;

    if(GameState::gem->getCurActor()==NULL)
    {
        newCamPos.copyFrom(cameraGetPosNoShake());
    }
    else
    {
        newCamPos=convertToVQV(GameState::gem->getCurActor()->getCenterPoint(0));
    }

    lightPosParam->copyFrom(&newCamPos);
    lightPosParam->addXYZRef(&lightVec, getConst(E_CONST_LIGHTDIS));
    lightLookAt.copyFrom(&newCamPos);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    gluLookAt(
//        lightLookAt[0],
//        lightLookAt[1],
//        lightLookAt[2],
//        lightPosParam->getFX(),
//        lightPosParam->getFY(),
//        lightPosParam->getFZ(),
//        0.0f,
//        0.0f,
//        1.0f
//    );
//    glGetFloatv(GL_MODELVIEW_MATRIX, lightView.get());

    glm::mat4 glmLightView=glm::lookAt(toVec3(lightLookAt), toVec3(*lightPosParam), glm::vec3(0.0f, 0.0f, 1.0f));
    lightView=toMatrix4(glmLightView);
    
    Matrix4 lightProjection;
    GLfloat near_plane=clipDist[0];
    GLfloat far_plane=clipDist[1];//+conVals[E_CONST_LIGHTDIS];
    lightProjection.orthoProjection(orthoSize, orthoSize, near_plane, far_plane);
    lsMat=lightProjection*lightView;
}

void Renderer::setMatrices(int w, int h)
{
//    int i;
//    float* ptr1;
//    float* ptr2;

    if(perspectiveOn)
    {
        glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
        //glMatrixMode(GL_PROJECTION); //set the matrix to projection
        //glLoadIdentity();
        //
        //
        //
        //gluPerspective(
        //    FOV,
        //    (GLfloat)w/(GLfloat)h,
        //    clipDist[0],
        //    clipDist[1]
        //); //set the perspective (angle of sight, width, height, , depth)
        //
        //
        //// ComputeFOVProjection(
        //// 	projMatrix.get(),
        //// 	FOV,
        //// 	w/h,
        //// 	clipDist[0],
        //// 	clipDist[1],
        //// 	false
        //// );
        //
        //glGetFloatv(GL_PROJECTION_MATRIX, projMatrix.get());
        //
        //glMatrixMode(GL_MODELVIEW); //set the matrix back to model
        //
        ////*180.0f/M_PI / 180 * M_PI
        //
        //glLoadIdentity();
        //
        //gluLookAt(
        //    cameraGetPos()->getFX(),
        //    cameraGetPos()->getFY(),
        //    cameraGetPos()->getFZ(),
        //    cameraGetPos()->getFX()+lookAtVec[0],
        //    cameraGetPos()->getFY()+lookAtVec[1],
        //    cameraGetPos()->getFZ()+lookAtVec[2],
        //    0.0f,
        //    0.0f,
        //    1.0f
        //);
        //
        //// glRotatef(getCamRot(1)*180.0f/M_PI,0.0f,1.0f,0.0f);
        //// glRotatef(getCamRot(0)*180.0f/M_PI,0.0f,0.0f,1.0f);
        //// glTranslated(
        //// 	-cameraGetPos()->getFX(),
        //// 	-cameraGetPos()->getFY(),
        //// 	-cameraGetPos()->getFZ()
        //// );
        //
        //
        //
        //glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix.get());
        float fovRad=FOV*((float)M_PI/180.0f);

        glm::mat4 glmProjMatrix=glm::perspectiveFov(fovRad, (float)w, (float)h, (float)clipDist[0], (float)clipDist[1]);
        projMatrix=toMatrix4(glmProjMatrix);

        glm::vec3 glmLookAt(cameraGetPos()->getFX()+lookAtVec[0], cameraGetPos()->getFY()+lookAtVec[1], cameraGetPos()->getFZ()+lookAtVec[2]);
        glm::mat4 glmLightView=glm::lookAt(toVec3(*cameraGetPos()), glmLookAt, glm::vec3(0.0f, 0.0f, 1.0f));
        lightView=toMatrix4(glmLightView);

//        ptr1=viewMatrix.get();
//        ptr2=projMatrix.get();

        pmMatrix=projMatrix*viewMatrix;

//        for(i=0; i<16; i++)
//        {
//            viewMatrixD[i]=ptr1[i];
//            projMatrixD[i]=ptr2[i];
//        }
//
//        gluInvertMatrix(viewMatrixD, viewMatrixDI);
//
//        glGetIntegerv(GL_VIEWPORT, viewport);


//        heightOfNearPlane=
//            (
//            ((float)abs(viewport[3]-viewport[1]))/
//                (2.0f*tan(0.5f*FOV*(float)M_PI/180.0f))
//                ) *
//                (
//                    2.0f/((float)scaleFactor)
//                    );

        heightOfNearPlane=(h/(2.0f*tan(0.5f*fovRad)))*(2.0f/((float)scaleFactor));

        // lastW = -1; 
        // lastH = -1;

    }
    else
    {
        if(
            (lastW==w)&&
            (lastH==h)&&
            (lastPersp==perspectiveOn)
            )
        {
            // do nothing	
        }
        else
        {

        }


        glViewport(0, 0, w, h);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // glMatrixMode(GL_PROJECTION);
        // glLoadIdentity();
        // glOrtho(-0.5, +0.5, -0.5, +0.5, clipDist[0], clipDist[1]);

        lastW=w;
        lastH=h;
    }

    lastPersp=perspectiveOn;

}




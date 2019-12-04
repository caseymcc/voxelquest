#include "voxelquest/vbos.h"
#include "voxelquest/gamestate.h"

#include <iostream>

VBOWrapper::VBOWrapper()
{
    vi=new VIStruct();
    viIsShared=false;
    lastVMUsage=0.0f;
    ibo=0;
    vbo=0;
    numVecs=-1;
    hasInit=false;
    procCount=0;
}

VBOWrapper::~VBOWrapper()
{
    remVI();
}

void VBOWrapper::remVI()
{
    if(vi==NULL)
    {

    }
    else
    {

        if(viIsShared)
        {
            // manually delete
        }
        else
        {
            delete vi;
            vi=NULL;
        }

    }
}

void VBOWrapper::setVI(VIStruct* _vi, bool _viIsShared)
{
    remVI();
    viIsShared=_viIsShared;
    vi=_vi;
}

int VBOWrapper::getNumVerts()
{
    return (int)(vi->vertexVec.size()/(numVecs*4));
}

void VBOWrapper::init(
    int _numVecs, // number of 4 component vecs
    int _drawEnum //GL_DYNAMIC_DRAW GL_STATIC_DRAW	
)
{

    numVecs=_numVecs;
    drawEnum=_drawEnum;

}

void VBOWrapper::deallocVBO()
{

    if(ibo!=0)
    {
        glDeleteBuffers(1, &ibo);
    }

    if(vbo!=0)
    {
        glDeleteBuffers(1, &vbo);
    }

    ibo=0;
    vbo=0;
    numVecs=-1;

    hasInit=false;
}

void VBOWrapper::clearVecs(bool shrinkToFit)
{

    vi->vertexVec.clear();
    vi->indexVec.clear();

    if(shrinkToFit)
    {
        vi->vertexVec.shrink_to_fit();
        vi->indexVec.shrink_to_fit();
    }

}

void VBOWrapper::checkInit()
{
    if(hasInit)
    {

    }
    else
    {
        if(
            (vi->vertexVec.size()>0)&&
            (vi->indexVec.size()>0)
            )
        {
            initBase(
                &(vi->vertexVec[0]),
                (int)vi->vertexVec.size(),
                (int)vi->vertexVec.size(),
                &(vi->indexVec[0]),
                (int)vi->indexVec.size(),
                (int)vi->indexVec.size()
            );
        }
        else
        {
            if(vi->vertexVec.size()>0)
            {
                initBase(
                    &(vi->vertexVec[0]),
                    (int)vi->vertexVec.size(),
                    (int)vi->vertexVec.size(),
                    NULL,
                    0,
                    0
                );
            }
            else
            {
                initBase(
                    NULL,
                    0,
                    0,
                    NULL,
                    0,
                    0
                );
            }
        }
    }
}

void VBOWrapper::update()
{

    checkInit();

    GLfloat* vertexPtr=NULL;
    GLuint* indexPtr=NULL;
    if(vi->vertexVec.size()>0)
    {
        vertexPtr=&(vi->vertexVec[0]);
    }
    if(vi->indexVec.size()>0)
    {
        indexPtr=&(vi->indexVec[0]);
    }

    updateBase(
        vertexPtr,
        (int)vi->vertexVec.size(),
        indexPtr,
        (int)vi->indexVec.size()
    );
}
void VBOWrapper::updateNew()
{

    checkInit();

    GLfloat* vertexPtr=NULL;
    GLuint* indexPtr=NULL;
    if(vi->vertexVec.size()>0)
    {
        vertexPtr=&(vi->vertexVec[0]);
    }
    if(vi->indexVec.size()>0)
    {
        indexPtr=&(vi->indexVec[0]);
    }

    updateNewBase(
        vertexPtr,
        (int)vi->vertexVec.size(),
        indexPtr,
        (int)vi->indexVec.size()
    );
}

void VBOWrapper::beginFill()
{
    procCount=0;
    vi->vertexVec.clear();
    vi->indexVec.clear();

}
void VBOWrapper::endFill()
{



    if(vi->vertexVec.size()>0)
    {
        updateNew();
    }
    else
    {
        // todo: handle case where vertex buffer has gone to zero
    }

    GameState::VERTEX_MEM_USAGE-=lastVMUsage;
    float vertMem=(float)(vi->vertexVec.size()+vi->indexVec.size())*4;
    GameState::VERTEX_MEM_USAGE+=vertMem/(1024.0f*1024.0f);
    lastVMUsage=vertMem/(1024.0f*1024.0f);

}


void VBOWrapper::draw()
{

    if(!hasInit)
    {
        std::cout<<"NOT INIT!\n";
        return;
    }

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sizeOfID, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void VBOWrapper::drawCubes(int numCubes)
{

    if(!hasInit)
    {
        std::cout<<"NOT INIT!\n";
        return;
    }

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (numCubes)*36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void VBOWrapper::drawPoints()
{

    if(!hasInit)
    {
        std::cout<<"NOT INIT!\n";
        return;
    }

    // glBindVertexArray(vao);
    // glDrawArrays(GL_POINTS, 0, sizeOfVD/8);
    // glBindVertexArray(0);

    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, sizeOfVD/attSize);
    glBindVertexArray(0);

}

void VBOWrapper::getPixVal(
    //FBOWrapper* fbow0,
    //FBOWrapper* fbow1,
    //int ind,
    float xb, float yb, float zb,
    float xm, float ym, float zm,
    float* data,
    int dataLen
)
{
    //int maskInd = xm + ym*2 + zm*4;

    // vi->vertexVec.push_back(xb+xm+NET_MASKS[mv[maskInd]].getX());
    // vi->vertexVec.push_back(yb+ym+NET_MASKS[mv[maskInd]].getY());
    // vi->vertexVec.push_back(zb+zm+NET_MASKS[mv[maskInd]].getZ());
    // vi->vertexVec.push_back(1.0f);

    vi->vertexVec.push_back(xb+xm);
    vi->vertexVec.push_back(yb+ym);
    vi->vertexVec.push_back(zb+zm);
    vi->vertexVec.push_back(1.0f);


    // vi->vertexVec.push_back(xb+xm);
    // vi->vertexVec.push_back(yb+ym);
    // vi->vertexVec.push_back(zb+zm);

    int i;

    for(i=0; i<dataLen; i++)
    {
        vi->vertexVec.push_back(data[i]);
    }





    // glMultiTexCoord3f(
    // 	GL_TEXTURE0,
    // 	xb+xm,
    // 	yb+ym,
    // 	zb+zm
    // );
    // glMultiTexCoord4f(
    // 	GL_TEXTURE1,
    // 	fbow1->getPixelAtIndex(ind,R_CHANNEL)/255.0f,
    // 	fbow1->getPixelAtIndex(ind,G_CHANNEL)/255.0f,
    // 	fbow1->getPixelAtIndex(ind,B_CHANNEL)/255.0f,

    // 	fbow0->getPixelAtIndex(ind,B_CHANNEL) +
    // 	fbow0->getPixelAtIndex(ind,A_CHANNEL)*256

    // );

    //glVertex3f(xb+xm,yb+ym,zb+zm);
}

void VBOWrapper::vboBox(
    float bpX,
    float bpY,
    float bpZ,

    float iv0,
    float iv1,

    uint procFlag,

    float* data,
    int dataLen
)
{


    // counter clockwise == front face

    //         z  
    //  4 5     . x
    //  6 7     y

    //    0 1
    //    2 3

    getPixVal(bpX, bpY, bpZ, iv0, iv0, iv0, data, dataLen); // 0
    getPixVal(bpX, bpY, bpZ, iv1, iv0, iv0, data, dataLen); // 1
    getPixVal(bpX, bpY, bpZ, iv0, iv1, iv0, data, dataLen); // 2
    getPixVal(bpX, bpY, bpZ, iv1, iv1, iv0, data, dataLen); // 3

    getPixVal(bpX, bpY, bpZ, iv0, iv0, iv1, data, dataLen); // 4
    getPixVal(bpX, bpY, bpZ, iv1, iv0, iv1, data, dataLen); // 5
    getPixVal(bpX, bpY, bpZ, iv0, iv1, iv1, data, dataLen); // 6
    getPixVal(bpX, bpY, bpZ, iv1, iv1, iv1, data, dataLen); // 7



    if(procFlags[0]&procFlag)
    { // x+

        vi->indexVec.push_back(5+procCount);
        vi->indexVec.push_back(1+procCount);
        vi->indexVec.push_back(3+procCount);
        vi->indexVec.push_back(3+procCount);
        vi->indexVec.push_back(7+procCount);
        vi->indexVec.push_back(5+procCount);



    }
    if(procFlags[1]&procFlag)
    { // x-

        vi->indexVec.push_back(6+procCount);
        vi->indexVec.push_back(2+procCount);
        vi->indexVec.push_back(0+procCount);
        vi->indexVec.push_back(0+procCount);
        vi->indexVec.push_back(4+procCount);
        vi->indexVec.push_back(6+procCount);


    }




    if(procFlags[2]&procFlag)
    { // y+

        vi->indexVec.push_back(7+procCount);
        vi->indexVec.push_back(3+procCount);
        vi->indexVec.push_back(2+procCount);
        vi->indexVec.push_back(2+procCount);
        vi->indexVec.push_back(6+procCount);
        vi->indexVec.push_back(7+procCount);


    }
    if(procFlags[3]&procFlag)
    { // y-

        vi->indexVec.push_back(4+procCount);
        vi->indexVec.push_back(0+procCount);
        vi->indexVec.push_back(1+procCount);
        vi->indexVec.push_back(1+procCount);
        vi->indexVec.push_back(5+procCount);
        vi->indexVec.push_back(4+procCount);

    }
    if(procFlags[4]&procFlag)
    { // z+

        vi->indexVec.push_back(4+procCount);
        vi->indexVec.push_back(5+procCount);
        vi->indexVec.push_back(7+procCount);
        vi->indexVec.push_back(7+procCount);
        vi->indexVec.push_back(6+procCount);
        vi->indexVec.push_back(4+procCount);

    }
    if(procFlags[5]&procFlag)
    { // z-

        vi->indexVec.push_back(3+procCount);
        vi->indexVec.push_back(1+procCount);
        vi->indexVec.push_back(0+procCount);
        vi->indexVec.push_back(0+procCount);
        vi->indexVec.push_back(2+procCount);
        vi->indexVec.push_back(3+procCount);

    }


    procCount+=8;



    // if (procFlags[0]&procFlag) { // x+

    // 	getPixVal(bpX,bpY,bpZ, iv1,iv1,iv1,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv1,iv0,iv1,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv1,iv1,iv0,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv1,iv0,iv0,data,dataLen);

    // 	getIndVal(procCount);
    // 	procCount++;


    // }
    // if (procFlags[1]&procFlag) { // x-

    // 	getPixVal(bpX,bpY,bpZ, iv0,iv1,iv1,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv0,iv0,iv1,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv0,iv1,iv0,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv0,iv0,iv0,data,dataLen);

    // 	getIndVal2(procCount);
    // 	procCount++;

    // }
    // if (procFlags[2]&procFlag) { // y+

    // 	getPixVal(bpX,bpY,bpZ, iv1,iv1,iv1,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv0,iv1,iv1,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv1,iv1,iv0,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv0,iv1,iv0,data,dataLen);

    // 	getIndVal2(procCount);
    // 	procCount++;

    // }
    // if (procFlags[3]&procFlag) { // y-


    // 	getPixVal(bpX,bpY,bpZ, iv1,iv0,iv1,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv0,iv0,iv1,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv1,iv0,iv0,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv0,iv0,iv0,data,dataLen);

    // 	getIndVal(procCount);
    // 	procCount++;
    // }
    // if (procFlags[4]&procFlag) { // z+

    // 	getPixVal(bpX,bpY,bpZ, iv1,iv1,iv1,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv0,iv1,iv1,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv1,iv0,iv1,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv0,iv0,iv1,data,dataLen);

    // 	getIndVal(procCount);
    // 	procCount++;
    // }
    // if (procFlags[5]&procFlag) { // z-

    // 	getPixVal(bpX,bpY,bpZ, iv1,iv1,iv0,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv0,iv1,iv0,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv1,iv0,iv0,data,dataLen);
    // 	getPixVal(bpX,bpY,bpZ, iv0,iv0,iv0,data,dataLen);

    // 	getIndVal2(procCount);
    // 	procCount++;
    // }
}




void VBOWrapper::updateBase(
    GLfloat* _vertexData,
    int _sizeOfVD,
    GLuint* _indexData,
    int _sizeOfID
)
{

    sizeOfVD=_sizeOfVD;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*_sizeOfVD, _vertexData);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    if(_indexData!=NULL)
    {
        sizeOfID=_sizeOfID;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*_sizeOfID, _indexData);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


}

void VBOWrapper::updateNewBase(
    GLfloat* _vertexData,
    int _sizeOfVD,
    GLuint* _indexData,
    int _sizeOfID
)
{

    sizeOfVD=_sizeOfVD;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*_sizeOfVD, _vertexData, (GLenum)drawEnum);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*_sizeOfVD, _vertexData);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    if(_indexData!=NULL)
    {
        sizeOfID=_sizeOfID;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*_sizeOfID, _indexData, (GLenum)drawEnum);
        //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*_sizeOfID, _indexData);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}



void VBOWrapper::initBase(
    GLfloat* _vertexData,
    int _sizeOfVD,
    int _maxSizeOfVD,
    GLuint* _indexData,
    int _sizeOfID,
    int _maxSizeOfID
)
{

    if(numVecs==-1)
    {
        std::cout<<"DID NOT INIT numVecs!\n";
        return;
    }



    sizeOfID=_sizeOfID;
    maxSizeOfID=_maxSizeOfID;

    sizeOfVD=_sizeOfVD;
    maxSizeOfVD=_maxSizeOfVD;

    // vertexData = _vertexData;
    // indexData = _indexData;


    int i;

    attSize=numVecs*4;

    // a value of zero means that its tightly packed
    int strideVal=attSize*sizeof(GLfloat);


    // vao and vbo handle


    // generate and bind the vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // generate and bind the vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);


    if(_vertexData!=NULL)
    {
        // fill with data
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*maxSizeOfVD, _vertexData, (GLenum)drawEnum);
    }



    for(i=0; i<numVecs; i++)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, strideVal, (char*)0+i*4*sizeof(GLfloat));
    }


    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // generate and bind the index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    if(_indexData!=NULL)
    {


        // fill with data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*maxSizeOfID, _indexData, (GLenum)drawEnum);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


    // "unbind" vao
    glBindVertexArray(0);

    hasInit=true;

}

VBOGrid::VBOGrid()
{

}

void VBOGrid::init(
    int _xpitch,
    int _ypitch
)
{

    xpitch=_xpitch+1;
    ypitch=_ypitch+1;

    totVerts=xpitch*ypitch;
    totInd=6*totVerts;

    int i;
    int j;

    float fxp=(float)xpitch-2;
    float fyp=(float)ypitch;

    float fi;
    float fj;



    for(j=0; j<ypitch; j++)
    {
        fj=(float)j;
        for(i=0; i<xpitch; i++)
        {
            fi=(float)i;
            vboWrapper.vi->vertexVec.push_back((float)i);
            vboWrapper.vi->vertexVec.push_back((float)j);
            vboWrapper.vi->vertexVec.push_back(1.0f);
            vboWrapper.vi->vertexVec.push_back(0.0f);



            vboWrapper.vi->vertexVec.push_back(fi/fxp);
            vboWrapper.vi->vertexVec.push_back(fj/fyp);
            vboWrapper.vi->vertexVec.push_back(0.0f);
            vboWrapper.vi->vertexVec.push_back(0.0f);

        }
    }

    int ind0;
    int ind1;
    int ind2;
    int ind3;

    int ip1;
    int jp1;

    for(j=0; j<(ypitch-1); j++)
    {

        for(i=0; i<(xpitch-1); i++)
        {

            ip1=i+1;
            jp1=j+1;

            // if (ip1 == xpitch) {
            // 	ip1 = 0;
            // }
            // if (jp1 == ypitch) {
            // 	jp1 = 0;
            // }

            ind0=(i)+(j)*xpitch;
            ind1=(ip1)+(j)*xpitch;
            ind2=(i)+(jp1)*xpitch;
            ind3=(ip1)+(jp1)*xpitch;

            // 0 1
            // 2 3

            vboWrapper.vi->indexVec.push_back(ind0);
            vboWrapper.vi->indexVec.push_back(ind1);
            vboWrapper.vi->indexVec.push_back(ind3);

            vboWrapper.vi->indexVec.push_back(ind3);
            vboWrapper.vi->indexVec.push_back(ind2);
            vboWrapper.vi->indexVec.push_back(ind0);


        }
    }


    vboWrapper.init(
        2,
        (int)GL_STATIC_DRAW
    );
}

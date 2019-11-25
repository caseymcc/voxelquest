#include "vbos.h"

VBOWrapper::VBOWrapper()
{

}

void VBOWrapper::update(
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

void VBOWrapper::updateNew()
{
    // todo: use glBufferData instead of glBufferSubData
    // to completely clear and create new buffer
}

void VBOWrapper::init(
    GLfloat* _vertexData,
    int _sizeOfVD,
    int _maxSizeOfVD,
    GLuint* _indexData,
    int _sizeOfID,
    int _maxSizeOfID,
    int _numVecs, // number of 4 component vecs
    int _drawEnum //GL_DYNAMIC_DRAW GL_STATIC_DRAW
)
{


    numVecs=_numVecs;

    sizeOfID=_sizeOfID;
    maxSizeOfID=_maxSizeOfID;

    sizeOfVD=_sizeOfVD;
    maxSizeOfVD=_maxSizeOfVD;

    // vertexData = _vertexData;
    // indexData = _indexData;
    drawEnum=_drawEnum;

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


    // fill with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*maxSizeOfVD, _vertexData, drawEnum);


    for(i=0; i<numVecs; i++)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, strideVal, (char*)0+i*4*sizeof(GLfloat));
    }


    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(_indexData!=NULL)
    {
        // generate and bind the index buffer object
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        // fill with data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*maxSizeOfID, _indexData, drawEnum);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


    // "unbind" vao
    glBindVertexArray(0);


}

void VBOWrapper::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sizeOfID, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void VBOWrapper::drawPoints()
{
    // glBindVertexArray(vao);
    // glDrawArrays(GL_POINTS, 0, sizeOfVD/8);
    // glBindVertexArray(0);

    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, sizeOfVD/attSize);
    glBindVertexArray(0);

}

};




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

    float fxp=xpitch-2;
    float fyp=ypitch;

    float fi;
    float fj;



    for(j=0; j<ypitch; j++)
    {
        fj=j;
        for(i=0; i<xpitch; i++)
        {
            fi=i;
            vertexVec.push_back(i);
            vertexVec.push_back(j);
            vertexVec.push_back(1.0f);
            vertexVec.push_back(0.0f);



            vertexVec.push_back(fi/fxp);
            vertexVec.push_back(fj/fyp);
            vertexVec.push_back(0.0f);
            vertexVec.push_back(0.0f);

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

            indexVec.push_back(ind0);
            indexVec.push_back(ind1);
            indexVec.push_back(ind3);

            indexVec.push_back(ind3);
            indexVec.push_back(ind2);
            indexVec.push_back(ind0);


        }
    }




    vboWrapper.init(
        &(vertexVec[0]),
        vertexVec.size(),
        vertexVec.size(),
        &(indexVec[0]),
        indexVec.size(),
        indexVec.size(),
        2,
        GL_STATIC_DRAW
    );
}


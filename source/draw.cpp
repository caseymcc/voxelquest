#include "voxelquest/draw.h"

void Singleton::idrawCrossHairs(FIVector4 originVec, float radius)
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

void Singleton::drawLine(FIVector4 *p0, FIVector4 *p1)
{


    glBegin(GL_LINES);
    glMultiTexCoord3f(GL_TEXTURE0, 0.0f, 0.0f, 0.0f);
    glVertex3f(p0->getFX(), p0->getFY(), p0->getFZ());
    glMultiTexCoord3f(GL_TEXTURE0, 0.0f, 0.0f, 1.0f);
    glVertex3f(p1->getFX(), p1->getFY(), p1->getFZ());
    glEnd();
}


void Singleton::drawCubeCentered(FIVector4 *originVec, float radius)
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

void Singleton::drawBoxUp(FIVector4 originVec, float radiusX, float radiusY, float diamZ)
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

void Singleton::drawBoxMinMax(
    btVector3 v0,
    btVector3v1
)
{
    tempVec1.setBTV(v0);
    tempVec2.setBTV(v1);
    drawBox(&tempVec1, &tempVec2);
}

void Singleton::drawBoxRad(
    btVector3 v0,
    btVector3 v1
)
{
    tempVec1.setBTV(v0-v1);
    tempVec2.setBTV(v0+v1);
    drawBox(&tempVec1, &tempVec2);
}

void Singleton::drawBox(
    FIVector4 *v0,
    FIVector4 *v1,
    int faceFlag=2
)
{


    float minX=min(v0->getFX(), v1->getFX());
    float minY=min(v0->getFY(), v1->getFY());
    float minZ=min(v0->getFZ(), v1->getFZ());

    float maxX=max(v0->getFX(), v1->getFX());
    float maxY=max(v0->getFY(), v1->getFY());
    float maxZ=max(v0->getFZ(), v1->getFZ());


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

void Singleton::drawQuadWithCoords(
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

void Singleton::drawQuadBounds(
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


void Singleton::drawFSQuad()
{
    glCallList(fsqDL);
}


void Singleton::drawFSQuadOffset(
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

void Singleton::drawFBO(string fboName, int ind, float zm, int swapFlag=-1)
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

void Singleton::drawFBOOffsetDirect(FBOSet *fbos, int ind, float xOff, float yOff, float zm)
{

    glBindTexture(GL_TEXTURE_2D, fbos->fbos[ind].color_tex);
    //glClearColor(0.2,0.2,0.2,0.0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawFSQuadOffset(xOff, yOff, zm);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Singleton::drawFBOOffset(string fboName, int ind, float xOff, float yOff, float zm)
{
    FBOSet *fbos=getFBOByName(fboName);

    if(fbos)
    {
        drawFBOOffsetDirect(fbos, ind, xOff, yOff, zm);
    }
    else
    {
        doTrace("drawFBOOffsetDirect: Invalid FBO Name");
    }

}

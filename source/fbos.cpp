#include "voxelquest/fbos.h"
#include "voxelquest/helperfuncs.h"
#include "voxelquest/gamestate.h"
#include "voxelquest/renderer.h"
#include "voxelquest/gameworld.h"
#include "voxelquest/imageloader.h"

#include <algorithm>
#include <iostream>
#include <cassert>

int FBOWrapper::init(
    int _width,
    int _height,
    int _bytesPerChannel,
    GLenum _slot,
    GLenum filterEnum,
    GLenum clampEnum,
    bool isMultisample
)
{
    numBytes=0;
    pixelsUINT=NULL;

    width=_width;
    height=_height;
    bytesPerChannel=_bytesPerChannel;
    //hasDepth = _hasDepth;

    pixelsCharMippedMin=NULL;
    pixelsCharMippedMax=NULL;
    pixelsCharMippedAvg=NULL;

    int w=width;
    int h=height;
    int num_samples=4;
    numMips=0;

    isFloat=false;

    pixelsChar=NULL;
    pixelsFloat=NULL;


    slot=GL_COLOR_ATTACHMENT0_EXT;

    switch(_slot)
    {
    case 0:
        slot=GL_COLOR_ATTACHMENT0_EXT;
        break;
    case 1:
        slot=GL_COLOR_ATTACHMENT1_EXT;
        break;
    case 2:
        slot=GL_COLOR_ATTACHMENT2_EXT;
        break;
    case 3:
        slot=GL_COLOR_ATTACHMENT3_EXT;
        break;
    case 4:
        slot=GL_COLOR_ATTACHMENT4_EXT;
        break;
    case 5:
        slot=GL_COLOR_ATTACHMENT5_EXT;
        break;
    case 6:
        slot=GL_COLOR_ATTACHMENT6_EXT;
        break;
    case 7:
        slot=GL_COLOR_ATTACHMENT7_EXT;
        break;
    }

    if(isMultisample)
    {
        glGenTextures(1, &color_tex);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, color_tex);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, filterEnum);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, filterEnum);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, clampEnum);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, clampEnum);

        switch(bytesPerChannel)
        {
        case 1:
            internalFormat=GL_RGBA8;

            break;
        case 2:
            internalFormat=GL_RGBA16;

            break;
        case 4:
            internalFormat=GL_RGBA32F;
            isFloat=true;

            break;
        }

        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, num_samples, internalFormat, w, h, false);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, slot, GL_TEXTURE_2D_MULTISAMPLE, color_tex, 0);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    else
    {
        glGenTextures(1, &color_tex);
        glBindTexture(GL_TEXTURE_2D, color_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterEnum);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterEnum);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampEnum);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampEnum);

        switch(bytesPerChannel)
        {
        case 1:
            internalFormat=GL_RGBA8;
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

            break;
        case 2:
            internalFormat=GL_RGBA16;
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_SHORT, 0);

            break;
        case 4:
            internalFormat=GL_RGBA32F;
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                internalFormat,
                w,
                h,
                0,
                GL_RGBA,
                GL_FLOAT,
                0
            );
            isFloat=true;

            break;
        }

        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, slot, GL_TEXTURE_2D, color_tex, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GameState::TOT_GPU_MEM_USAGE+=((float)(w * h * bytesPerChannel*4))/(1024.0f * 1024.0f);

    return 1;
    //
    /*
    if (hasDepth) {
      glGenRenderbuffersEXT(1, &depth_rb);
      glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
      glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, w, h);
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
    }
    */
    //
}


void FBOWrapper::cpuToGPU()
{
    if(pixelsChar==NULL)
    {
        doTrace("error: null pointer pixelsChar");
        return;
    }

    switch(bytesPerChannel)
    {
    case 1:

        glBindTexture(GL_TEXTURE_2D, color_tex);
        /*
        glTexSubImage2D(
          GL_TEXTURE_2D,
          0,

          0,
          0,
          width,
          height,

          GL_RGBA,
          GL_UNSIGNED_BYTE,
          pixelsChar
         );
         */


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsChar);

        glBindTexture(GL_TEXTURE_2D, 0);

        break;
    case 2:
        doTrace("TODO: implement 16 bit");
        break;
    case 4:
        doTrace("TODO: implement 32 bit");
        break;
    }
}



void FBOWrapper::getPixelAtF(FIVector4 *fv, int x, int y)
{

    if(isFloat)
    {


        if((pixelsFloat==NULL))
        {
            getPixels();
        }

        //a
        //b
        //g
        //r

        fv->setFX(pixelsFloat[(x+y*width)*4+0]);
        fv->setFY(pixelsFloat[(x+y*width)*4+1]);
        fv->setFZ(pixelsFloat[(x+y*width)*4+2]);
        fv->setFW(pixelsFloat[(x+y*width)*4+3]);

    }
    else
    {
        doTraceND("Attempted to call getPixelAtF on char buffer.");
    }

}

void FBOWrapper::setAllPixels(int channel, int value)
{
    int i;
    int j;
    int x;
    int y;

    for(j=0; j<height; j++)
    {
        for(i=0; i<width; i++)
        {
            x=i;
            y=j;
            pixelsChar[(x+y*width)*4+channel]=value;
        }
    }
}

void FBOWrapper::setPixelAtC(int x, int y, int channel, unsigned char value)
{
    pixelsChar[(x+y*width)*4+channel]=value;
}


int FBOWrapper::getIndex(int xs, int ys)
{
    int x=xs;
    int y=ys;

    while(x<0)
    {
        x+=width;
    }
    while(y<0)
    {
        y+=height;
    }

    x=x%width;
    y=y%height;

    return x+y*width;
}

void FBOWrapper::setPixelAtWrapped(int xs, int ys, int channel, int val)
{

    int x=xs;
    int y=ys;

    while(x<0)
    {
        x+=width;
    }
    while(y<0)
    {
        y+=height;
    }

    x=x%width;
    y=y%height;

    int ind=x+y*width;

    pixelsChar[ind*4+channel]=val;
}

int FBOWrapper::getPixelAtWrapped(int xs, int ys, int channel)
{

    int x=xs;
    int y=ys;

    while(x<0)
    {
        x+=width;
    }
    while(y<0)
    {
        y+=height;
    }

    x=x%width;
    y=y%height;

    int ind=x+y*width;

    return pixelsChar[ind*4+channel];
}




int FBOWrapper::getPixelAtIndex(int ind, int channel)
{

    return pixelsChar[ind*4+channel];

}
void FBOWrapper::setPixelAtIndex(int ind, int channel, int val)
{
    pixelsChar[ind*4+channel]=val;
}
void FBOWrapper::orPixelAtIndex(int ind, int channel, int val)
{
    pixelsChar[ind*4+channel]|=val;
}
void FBOWrapper::andPixelAtIndex(int ind, int channel, int val)
{
    pixelsChar[ind*4+channel]&=val;
}

int FBOWrapper::getPixelAtC(int x, int y, int channel)
{
    if(!isFloat)
    {
        if((pixelsChar==NULL))
        {
            getPixels();
        }

        return (int)pixelsChar[(x+y*width)*4+channel];

    }
    else
    {
        doTrace("Attempted to call getPixelAtC on float buffer.");
        return 0;
    }

}

float FBOWrapper::getPixelAtLinear(float xf, float yf, int channel)
{
    int x[2];
    int y[2];

    x[0]=(int)floor(xf);
    y[0]=(int)floor(yf);
    x[1]=x[0]+1;
    y[1]=y[0]+1;

    float percX=xf-x[0];
    float percY=yf-y[0];
    float percXI=1.0f-percX;
    float percYI=1.0f-percY;


    float v0, v1;
    float v2, v3;

    int i;

    for(i=0; i<2; i++)
    {

        while(x[i]<0)
        {
            x[i]+=width;
        }
        while(y[i]<0)
        {
            y[i]+=height;
        }

        x[i]=x[i]%width;
        y[i]=y[i]%height;
    }

    v0=getPixelAtC(x[0], y[0], channel)/255.0f;
    v1=getPixelAtC(x[1], y[0], channel)/255.0f;
    v2=getPixelAtC(x[0], y[1], channel)/255.0f;
    v3=getPixelAtC(x[1], y[1], channel)/255.0f;

    float vFinal=(v0 * percXI+v1*percX) * percYI+(v2 * percXI+v3*percX) * percY;

    return vFinal;

}


int FBOWrapper::getMipVal(
    int x,
    int y,
    int mipLev,
    int channel,
    int minMaxAvg,
    int val,
    int ox,
    int oy
)
{

    int w=mipWidths[mipLev];
    int curWidth=mipWidths[mipLev];
    int mipPitch=1<<mipLev;
    int xv;
    int yv;

//    float t1;
//    float t2;
//    float t3;

    xv=intDiv(x * curWidth, mipWidths[0])+ox;
    yv=intDiv(y * curWidth, mipWidths[0])+oy;


    while(xv<0)
    {
        xv+=curWidth;
    }
    while(xv>=curWidth)
    {
        xv-=curWidth;
    }
    while(yv<0)
    {
        yv+=curWidth;
    }
    while(yv>=curWidth)
    {
        yv-=curWidth;
    }

    int ind=xv+yv*curWidth;
    int res=0;
    int resInd=(ind)*4+channel;

    if(val!=-1)
    {
        switch(minMaxAvg)
        {
        case 0:
            (pixelsCharMippedMin[mipLev][resInd])=val;
            break;
        case 1:
            (pixelsCharMippedMax[mipLev][resInd])=val;
            break;
        case 2:
            (pixelsCharMippedAvg[mipLev][resInd])=val;
            break;
        }
    }

    switch(minMaxAvg)
    {
    case 0:
        res=(int)(pixelsCharMippedMin[mipLev][resInd]);
        break;
    case 1:
        res=(int)(pixelsCharMippedMax[mipLev][resInd]);
        break;
    case 2:
        res=(int)(pixelsCharMippedAvg[mipLev][resInd]);
        break;
    }



    return res;

}

int FBOWrapper::getMipAtIndex(int ind, int mipLev, int channel, int minMaxAvg)
{
    int res=0;
    int resInd=ind*4+channel;

    switch(minMaxAvg)
    {
    case 0:
        res=(int)(pixelsCharMippedMin[mipLev][resInd]);
        break;
    case 1:
        res=(int)(pixelsCharMippedMax[mipLev][resInd]);
        break;
    case 2:
        res=(int)(pixelsCharMippedAvg[mipLev][resInd]);
        break;
    }

    return res;
}

int FBOWrapper::getMipInd(int x, int y, int mipLev)
{
    int w=mipWidths[mipLev];

    return ((x * mipWidths[mipLev])/mipWidths[0])+((y * mipWidths[mipLev])/mipWidths[0]) * mipWidths[mipLev];
}

void FBOWrapper::updateMips()
{

    int i;
    int j;
    int k;
    int m;

    int ind, ind0, ind1, ind2, ind3;

    int mRead;
    int mWrite;

    if(pixelsCharMippedAvg==NULL)
    {
        doTrace("Error: no mip maps, first call getPixels()");
        return;
    }
    else
    {
        for(m=0; m<numMips-1; m++)
        {

            mRead=m;
            mWrite=m+1;

            for(k=0; k<4; k++)
            { // channels

                for(i=0; i<mipWidths[mWrite]; i++)
                {
                    for(j=0; j<mipWidths[mWrite]; j++)
                    {

                        ind=(i+j*mipWidths[mWrite])*4+k;

                        ind0=((i*2+0)+(j*2+0) * mipWidths[mRead])*4+k; //
                        ind1=((i*2+1)+(j*2+0) * mipWidths[mRead])*4+k; //
                        ind2=((i*2+0)+(j*2+1) * mipWidths[mRead])*4+k; //
                        ind3=((i*2+1)+(j*2+1) * mipWidths[mRead])*4+k; //

                        pixelsCharMippedAvg[mWrite][ind]=(
                            pixelsCharMippedAvg[mRead][ind0]+
                            pixelsCharMippedAvg[mRead][ind1]+
                            pixelsCharMippedAvg[mRead][ind2]+
                            pixelsCharMippedAvg[mRead][ind3]
                            )/4;

                        pixelsCharMippedMin[mWrite][ind]=std::min(
                            std::min(
                            pixelsCharMippedMin[mRead][ind0],
                            pixelsCharMippedMin[mRead][ind1]
                        ),
                            std::min(
                            pixelsCharMippedMin[mRead][ind2],
                            pixelsCharMippedMin[mRead][ind3]
                        )
                        );

                        pixelsCharMippedMax[mWrite][ind]=std::max(
                            std::max(
                            pixelsCharMippedMax[mRead][ind0],
                            pixelsCharMippedMax[mRead][ind1]
                        ),
                            std::max(
                            pixelsCharMippedMax[mRead][ind2],
                            pixelsCharMippedMax[mRead][ind3]
                        )
                        );




                    }
                }
            }
        }
    }
}



int FBOWrapper::getPixelAtIndex3DMip(int ind, int channel, int mval, int mipLev)
{
    int newInd=ind*4+channel;

    switch(mval)
    {
    case 0: // min
        return pixelsCharMippedMax[mipLev][newInd];
        break;

    case 1: // max
        return pixelsCharMippedMax[mipLev][newInd];
        break;

    case 2: // avg
        return pixelsCharMippedMax[mipLev][newInd];
        break;

    case 3: // no mip
        return pixelsChar[newInd];
        break;
    }

    return 0;
}

void FBOWrapper::updateMips3D(int basePitch)
{

    int i;
    int j;
    int k;
    int m;

    int c;

    int dest, ind0, ind1, ind2, ind3, ind4, ind5, ind6, ind7;

    int mRead;
    int mWrite;


    int mrPitch=basePitch;
    int mwPitch=basePitch/2;

    int mrPitch2;

    pixelsCharMippedAvg[0]=pixelsChar;
    pixelsCharMippedMax[0]=pixelsChar;
    pixelsCharMippedMin[0]=pixelsChar;


    if(pixelsCharMippedAvg==NULL)
    {
        doTrace("Error: no mip maps, first call getPixels()");
        return;
    }
    else
    {

        for(m=0; m<(MAX_MIP_LEV-1); m++)
        { //m < numMips - 1

            mRead=m;
            mWrite=m+1;



            // channels

            for(k=0; k<mwPitch; k++)
            {
                for(j=0; j<mwPitch; j++)
                {
                    for(i=0; i<mwPitch; i++)
                    {


                        for(c=0; c<4; c++)
                        {
                            dest=(i+j*mwPitch+k*mwPitch*mwPitch)*4+c;

                            mrPitch2=mrPitch*mrPitch;

                            ind0=((i*2+0)+(j*2+0) * mrPitch+(k*2+0) * mrPitch2)*4+c;
                            ind1=((i*2+1)+(j*2+0) * mrPitch+(k*2+0) * mrPitch2)*4+c;
                            ind2=((i*2+0)+(j*2+1) * mrPitch+(k*2+0) * mrPitch2)*4+c;
                            ind3=((i*2+1)+(j*2+1) * mrPitch+(k*2+0) * mrPitch2)*4+c;
                            ind4=((i*2+0)+(j*2+0) * mrPitch+(k*2+1) * mrPitch2)*4+c;
                            ind5=((i*2+1)+(j*2+0) * mrPitch+(k*2+1) * mrPitch2)*4+c;
                            ind6=((i*2+0)+(j*2+1) * mrPitch+(k*2+1) * mrPitch2)*4+c;
                            ind7=((i*2+1)+(j*2+1) * mrPitch+(k*2+1) * mrPitch2)*4+c;





                            // pixelsCharMippedMin[ mWrite ][ dest ] = pixelsCharMippedMin[ mRead ][ind0];
                            // pixelsCharMippedMax[ mWrite ][ dest ] = pixelsCharMippedMax[ mRead ][ind0];
                            // pixelsCharMippedAvg[ mWrite ][ dest ] = pixelsCharMippedAvg[ mRead ][ind0];


                            pixelsCharMippedAvg[mWrite][dest]=(
                                pixelsCharMippedAvg[mRead][ind0]+
                                pixelsCharMippedAvg[mRead][ind1]+
                                pixelsCharMippedAvg[mRead][ind2]+
                                pixelsCharMippedAvg[mRead][ind3]+
                                pixelsCharMippedAvg[mRead][ind4]+
                                pixelsCharMippedAvg[mRead][ind5]+
                                pixelsCharMippedAvg[mRead][ind6]+
                                pixelsCharMippedAvg[mRead][ind7]
                                )/8;

                            pixelsCharMippedMax[mWrite][dest]=

                                std::max(
                                    std::max(
                                    std::max(
                                    pixelsCharMippedMax[mRead][ind0],
                                    pixelsCharMippedMax[mRead][ind1]
                                ),
                                    std::max(
                                    pixelsCharMippedMax[mRead][ind2],
                                    pixelsCharMippedMax[mRead][ind3]
                                )
                                ),
                                    std::max(
                                    std::max(
                                    pixelsCharMippedMax[mRead][ind4],
                                    pixelsCharMippedMax[mRead][ind5]
                                ),
                                    std::max(
                                    pixelsCharMippedMax[mRead][ind6],
                                    pixelsCharMippedMax[mRead][ind7]
                                )
                                )
                                );


                            // pixelsCharMippedMin[ mWrite ][ dest ] = min(
                            // 		min(
                            // 			min(
                            // 				pixelsCharMippedMax[ mRead ][ind0],
                            // 				pixelsCharMippedMax[ mRead ][ind1]
                            // 			),
                            // 			min(
                            // 				pixelsCharMippedMax[ mRead ][ind2],
                            // 				pixelsCharMippedMax[ mRead ][ind3]
                            // 			)
                            // 		),
                            // 		min(
                            // 			min(
                            // 				pixelsCharMippedMax[ mRead ][ind4],
                            // 				pixelsCharMippedMax[ mRead ][ind5]
                            // 			),
                            // 			min(
                            // 				pixelsCharMippedMax[ mRead ][ind6],
                            // 				pixelsCharMippedMax[ mRead ][ind7]
                            // 			)							
                            // 		)
                            // );




                        }


                    }
                }
            }







            mrPitch=mrPitch/2;
            mwPitch=mwPitch/2;


        }




    }
}



void FBOWrapper::getPixelsFast()
{



    glBindTexture(GL_TEXTURE_2D, color_tex);
//    int totalWidth;
//    int curBytes;

    numBytes=width*height;

    if(pixelsUINT==NULL)
    {
        pixelsUINT=new uint[numBytes];
    }
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsUINT);
    glBindTexture(GL_TEXTURE_2D, 0);

}

// assumed that remoteBuffer is allocated
void FBOWrapper::getPixelsFastRemote(uint* remoteBuffer)
{


    glBindTexture(GL_TEXTURE_2D, color_tex);
//    int totalWidth;
//    int curBytes;
    numBytes=width*height;
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, remoteBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

}


void FBOWrapper::getPixels(bool _hasMipMap)
{
    glBindTexture(GL_TEXTURE_2D, color_tex);


    int targetlevel=0;
    int index;
    int i;
    int totalWidth;
    int curBytes;

    //GLint intForm;
    //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPONENTS, &intForm); // get internal format type of GL texture


    switch(internalFormat) // determine what type GL texture has...
    {
    case GL_RGB:
        numBytes=width*height*3;
        break;
    case GL_RGBA:
        numBytes=width*height*4;
        break;
    case GL_RGBA8:
        numBytes=width*height*4;
        break;
        //case GL_RGBA16:
        //
        //break;
    case GL_RGBA32F:
        numBytes=width*height*4;
        break;

    default:
        doTrace("Unsupported Format Type");
        return;
        break;
    }

    if(numBytes)
    {

        if(isFloat)
        {

            if(pixelsFloat==NULL)
            {
                pixelsFloat=new float[numBytes];//(float*)malloc(numBytes);
            }

            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixelsFloat);
        }
        else
        {

            if(pixelsChar==NULL)
            {
                pixelsChar=new unsigned char[numBytes];


                if(_hasMipMap)
                {

                    hasMipMap=_hasMipMap;

                }


                index=width;
                if(index==0)
                {
                    doTrace("Error: width of 0");
                    return;
                }
                else
                {
                    while(index>>=1)
                    {
                        ++targetlevel;
                    }
                    numMips=targetlevel;

                }

                pixelsCharMippedAvg=new unsigned char*[numMips];
                pixelsCharMippedMax=new unsigned char*[numMips];
                pixelsCharMippedMin=new unsigned char*[numMips];
                mipWidths=new int[numMips];


                pixelsCharMippedAvg[0]=pixelsChar;
                pixelsCharMippedMax[0]=pixelsChar;
                pixelsCharMippedMin[0]=pixelsChar;

                mipWidths[0]=width;

                if(hasMipMap)
                {

                    totalWidth=width/2;
                    curBytes=numBytes/2;

                    for(i=1; i<numMips; i++)
                    {
                        pixelsCharMippedMin[i]=new unsigned char[curBytes];
                        pixelsCharMippedMax[i]=new unsigned char[curBytes];
                        pixelsCharMippedAvg[i]=new unsigned char[curBytes];
                        mipWidths[i]=totalWidth;

                        totalWidth=totalWidth/2;
                        curBytes=curBytes/2;
                    }
                }

            }
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsChar);
        }


    }

    glBindTexture(GL_TEXTURE_2D, 0);


}




FBOWrapper *FBOSet::getFBOWrapper(int offset)
{
    return &(fbos[offset]);
}

void FBOSet::init(
    int _numBufs,
    int _width,
    int _height,
    int _bytesPerChannel,
    bool _hasDepth,
    GLenum filterEnum,
    GLenum clampEnum,
    bool isMultisample
)
{
    int i;

    hasDepth=_hasDepth;

    numBufs=_numBufs;
    height=_height;
    width=_width;

    // cout << width << "," << height <<"\n";

    bytesPerChannel=_bytesPerChannel;

    fbos=new FBOWrapper[numBufs];

    isReady=true;

    glGenFramebuffersEXT(1, &mFBO);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

    for(i=0; i<numBufs; i++)
    {
        fbos[i].init(width, height, bytesPerChannel, (GLenum)i, filterEnum, clampEnum, isMultisample);
    }


    if(hasDepth)
    {
        glGenRenderbuffersEXT(1, &depth_rb);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
    }


    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
void FBOSet::bind(int doClear)
{

    //setWH(width, height);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

    glDrawBuffers(numBufs, bufNames);

    if(doClear)
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }

}
void FBOSet::copyFromMem(int ind, unsigned char *dat)
{

    //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFBO);

    glBindTexture(GL_TEXTURE_2D, fbos[ind].getColorTex());

    //glTexSubImage2D(GLenum target​, GLint level​, GLint xoffset​, GLint yoffset​, GLsizei width​, GLsizei height​, GLenum format​, GLenum type​, const GLvoid * data​);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, dat);
    //glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, fbos[ind].slot, GL_TEXTURE_2D, fbos[ind].color_tex, 0);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

/////////////////////////////////////////////////////////////
FBOManager *FBOManager::singleton()
{
    static FBOManager fboManager;

    return &fboManager;
}

FBOManager::FBOManager()
{
    initFBOs();
}

void FBOManager::initFBOs()
{
    int numMaps=2;
    int numChannels=4;
    bool fboHasDepth=true;
    int bufferDiv=1;
    int newPitch=(GameState::gw->imageHM0->width)*2*HOLDER_MOD;
    int mapPitch=(GameState::gw->imageHM0->width);

    fboMap["prelightFBO"].init(4, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), 1, false, GL_LINEAR);

    // fboMap["allTargFBO"].init(6, bufferRenderDim.getIX(), bufferRenderDim.getIY(), numChannels, fboHasDepth);
    // fboMap["allDepthFBO"].init(numMaps, bufferDimTarg.getIX(), bufferDimTarg.getIY(), numChannels, fboHasDepth, GL_LINEAR);
    
    fboMap["prmTargFBO"].init(8, Renderer::bufferRenderDim.getIX(), Renderer::bufferRenderDim.getIY(), numChannels, fboHasDepth);
    fboMap["prmDepthFBO"].init(numMaps, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), numChannels, fboHasDepth, GL_LINEAR);
    
    fboMap["numstepsFBO"].init(1, Renderer::bufferRenderDim.getIX(), Renderer::bufferRenderDim.getIY(), numChannels, fboHasDepth);
    
    fboMap["terTargFBO"].init(8, Renderer::bufferRenderDim.getIX(), Renderer::bufferRenderDim.getIY(), numChannels, fboHasDepth);
    fboMap["limbFBO"].init(1, Renderer::bufferRenderDim.getIX(), Renderer::bufferRenderDim.getIY(), numChannels, fboHasDepth);
    fboMap["terDepthFBO"].init(numMaps, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), numChannels, fboHasDepth, GL_LINEAR);
    
    for(int i=0; i<=NUM_POLY_STRINGS; i++)
        fboMap[polyFBOStrings[i]].init(1, Renderer::bufferRenderDim.getIX(), Renderer::bufferRenderDim.getIY(), 4, true);
    
    fboMap["shadowMapFBO"].init(1, SHADOW_MAP_RES, SHADOW_MAP_RES, 4, true, GL_LINEAR);
    fboMap["rasterFBO0"].init(3, Renderer::bufferDim.getIX(), Renderer::bufferDim.getIY(), 4, true, GL_NEAREST);
    fboMap["rasterFBO1"].init(3, Renderer::bufferDim.getIX(), Renderer::bufferDim.getIY(), 4, true, GL_NEAREST);
    
    fboMap["rasterLowFBO"].init(3, Renderer::rasterLowDim.getIX(), Renderer::rasterLowDim.getIY(), 4, true, GL_NEAREST);
    fboMap["shadowLowFBO"].init(3, SHADOW_MAP_LOW_RES, SHADOW_MAP_LOW_RES, 4, true, GL_LINEAR);
    
    fboMap["readFBO"].init(3, Renderer::rasterLowDim.getIX(), Renderer::rasterLowDim.getIY(), 4, true, GL_NEAREST);
    
    // fboMap["rasterPosFBO"].init(1, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), numChannels, fboHasDepth, GL_LINEAR);//, GL_REPEAT);
    // fboMap["rasterSourceFBO"].init(1, bufferDim.getIX(), bufferDim.getIY(), 1, false, GL_NEAREST);//, GL_REPEAT);
    
    if(USE_SPHERE_MAP)
    {
        fboMap["sphTargFBO"].init(8, (int)((float)Renderer::bufferRenderDim.getIX()*SPHEREMAP_SCALE_FACTOR), (int)((float)Renderer::bufferRenderDim.getIY()*SPHEREMAP_SCALE_FACTOR), numChannels, fboHasDepth);
        fboMap["sphDepthFBO"].init(numMaps, (int)((float)Renderer::bufferDimTarg.getIX()*SPHEREMAP_SCALE_FACTOR), (int)((float)Renderer::bufferDimTarg.getIY()*SPHEREMAP_SCALE_FACTOR), numChannels, fboHasDepth, GL_LINEAR, GL_REPEAT);
    }
    
    fboMap["solidBaseTargFBO"].init(numMaps, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), numChannels, fboHasDepth);
    fboMap["solidTargFBO"].init(numMaps, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), numChannels, fboHasDepth);
    
    fboMap["waterTargFBO"].init(numMaps, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), numChannels, fboHasDepth);
    
    fboMap["swapTargFBO0"].init(numMaps, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), numChannels, false);
    fboMap["swapTargFBO1"].init(numMaps, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), numChannels, false);
    
    fboMap["geomBaseTargFBO"].init(numMaps+1, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), numChannels, true);
    fboMap["geomTargFBO"].init(numMaps, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), numChannels, true);
    fboMap["combineWithWaterTargFBO"].init(numMaps, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), numChannels, fboHasDepth);
    
    fboMap["debugTargFBO"].init(numMaps, Renderer::bufferDimTarg.getIX(), Renderer::bufferDimTarg.getIY(), numChannels, true);
    
    fboMap["noiseFBO"].init(1, Renderer::bufferDim.getIX(), Renderer::bufferDim.getIY(), 1, false, GL_NEAREST, GL_REPEAT);
    fboMap["noiseFBOLinear"].init(1, Renderer::bufferDim.getIX(), Renderer::bufferDim.getIY(), 1, false, GL_LINEAR, GL_REPEAT);
    //fboMap["guiFBO"].init(1, guiWinW, guiWinH, 1, false, GL_LINEAR);
    fboMap["resultFBO0"].init(1, Renderer::bufferDim.getIX(), Renderer::bufferDim.getIY(), 1, false, GL_NEAREST);
    fboMap["resultFBO1"].init(1, Renderer::bufferDim.getIX(), Renderer::bufferDim.getIY(), 1, false, GL_NEAREST);
    fboMap["lastFBO"].init(1, Renderer::bufferDim.getIX(), Renderer::bufferDim.getIY(), 1, false, GL_NEAREST);
    //fboMap["waveFBO"].init(1, Renderer::bufferDim.getIX()/2, Renderer::bufferDim.getIY()/2, 1, false, GL_LINEAR, GL_MIRRORED_REPEAT);
    fboMap["swapFBOLin0"].init(1, Renderer::bufferDim.getIX(), Renderer::bufferDim.getIY(), 1, false, GL_LINEAR);
    fboMap["swapFBOLin1"].init(1, Renderer::bufferDim.getIX(), Renderer::bufferDim.getIY(), 1, false, GL_LINEAR);
    fboMap["swapFBOBLin0"].init(1, Renderer::bufferDim.getIX()/bufferDiv, Renderer::bufferDim.getIY()/bufferDiv, 1, false, GL_LINEAR);
    fboMap["swapFBOBLin1"].init(1, Renderer::bufferDim.getIX()/bufferDiv, Renderer::bufferDim.getIY()/bufferDiv, 1, false, GL_LINEAR);
    fboMap["swapFBOLinHalf0"].init(1, Renderer::bufferDim.getIX()/bufferDiv, Renderer::bufferDim.getIY()/bufferDiv, 1, false, GL_LINEAR);
    fboMap["swapFBOLinHalf1"].init(1, Renderer::bufferDim.getIX()/bufferDiv, Renderer::bufferDim.getIY()/bufferDiv, 1, false, GL_LINEAR);
    fboMap["cityFBO"].init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
    fboMap["hmFBO"].init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
    fboMap["hmFBOLinear"].init(1, newPitch, newPitch, 1, false, GL_LINEAR, GL_REPEAT);
    fboMap["hmFBOLinearBig"].init(1, mapPitch, mapPitch, 1, false, GL_NEAREST, GL_REPEAT);
    fboMap["simplexFBO"].init(1, newPitch, newPitch, 1, false, GL_LINEAR, GL_REPEAT);
    fboMap["swapFBO0"].init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);
    fboMap["swapFBO1"].init(1, newPitch, newPitch, 1, false, GL_NEAREST, GL_REPEAT);

    fboMap["mapFBO"].init(1, 256, 256, 1, false, GL_NEAREST, GL_REPEAT);

    std::cout<<"FBOS ********************************\n";
    for(auto &item:fboMap)
    {
        std::cout<<"FBO: "<<item.first<<" "<<item.second.mFBO<<"\n";
        for(int i=0; i<item.second.numBufs; ++i)
            std::cout<<"    "<<i<<": "<<item.second.fbos[i].color_tex<<"\n";
    }
}
// todo: optimize this
FBOSet* FBOManager::getFBOByName(std::string &fboName)
{
    return singleton()->_getFBOByName(fboName);
}

FBOSet* FBOManager::_getFBOByName(std::string &fboName)
{

    if(fboMap.find(fboName)==fboMap.end())
    {
        std::cout<<"invalid key "<<fboName<<"\n";
        assert(false);
        exit(0);
    }

    return &(fboMap[fboName]);
}

//void FBOManager::sampleFBO(
//    std::string fboName,
//    int offset,
//    int swapFlag,
//    int minOff,
//    int maxOff
//)
//{
//    FBOSet *fbos;
//
//    if(swapFlag==-1)
//    {
//        fbos=getFBOByName(fboName);//&(fboMap[fboName]);
//    }
//    else
//    {
//
//        if(swapFlag==0)
//        {
//            fbos=getFBOByName(fboName+"0");
//        }
//        else
//        {
//            fbos=getFBOByName(fboName+"1");
//        }
//
//    }
//
//    if(fbos)
//    {
//        sampleFBODirect(fbos, offset, minOff, maxOff);
//    }
//    else
//    {
//        doTrace("sampleFBO: Invalid FBO Name");
//    }
//
//
//}
//
//void FBOManager::unsampleFBO(
//    std::string fboName,
//    int offset=0,
//    int swapFlag=-1,
//    int minOff=0,
//    int maxOff=-1
//)
//{
//
//    FBOSet *fbos;
//
//    if(swapFlag==-1)
//    {
//        fbos=getFBOByName(fboName);
//    }
//    else
//    {
//
//        if(swapFlag==0)
//        {
//            fbos=getFBOByName(fboName+"0");
//        }
//        else
//        {
//            fbos=getFBOByName(fboName+"1");
//        }
//
//    }
//
//    if(fbos)
//    {
//        unsampleFBODirect(fbos, offset, minOff, maxOff);
//    }
//    else
//    {
//        doTrace("unsampleFBO: Invalid FBO Name");
//    }
//
//}

FBOSet *FBOManager::getFBOSet(std::string fboName)
{
    return singleton()->getFBOByName(fboName);
}

FBOWrapper *FBOManager::getFBOWrapper(std::string fboName, int offset)
{
    FBOSet *fbos=singleton()->getFBOByName(fboName);
    return fbos->getFBOWrapper(offset);
}


//void FBOManager::copyFBO(std::string src, std::string dest, int num=0)
//{
//    bindShader("CopyShader");
//    bindFBO(dest);
//    //sampleFBO(src, 0);
//    setShaderTexture(0, getFBOWrapper(src, num)->color_tex);
//    drawFSQuad();
//    setShaderTexture(0, 0);
//    unbindFBO();
//    unbindShader();
//}
//
//void FBOManager::copyFBO2(std::string src, std::string dest, int num1=0, int num2=1)
//{
//    bindShader("CopyShader2");
//    bindFBO(dest);
//    setShaderTexture(0, getFBOWrapper(src, num1)->color_tex);
//    setShaderTexture(1, getFBOWrapper(src, num2)->color_tex);
//    drawFSQuad();
//    setShaderTexture(1, 0);
//    setShaderTexture(0, 0);
//    unbindFBO();
//    unbindShader();
//}
//
//void FBOManager::copyFBO3(std::string src, string dest, int num1=0, int num2=1, int num3=2)
//{
//    bindShader("CopyShader3");
//    bindFBO(dest);
//    setShaderTexture(0, getFBOWrapper(src, num1)->color_tex);
//    setShaderTexture(1, getFBOWrapper(src, num2)->color_tex);
//    setShaderTexture(2, getFBOWrapper(src, num3)->color_tex);
//    drawFSQuad();
//    setShaderTexture(2, 0);
//    setShaderTexture(1, 0);
//    setShaderTexture(0, 0);
//    unbindFBO();
//    unbindShader();
//}
//
//void FBOManager::bindFBO(std::string fboName, int swapFlag=-1, int doClear=1)
//{
//
//    FBOSet *fbos;
//
//    if(swapFlag==-1)
//    {
//        fbos=getFBOByName(fboName);
//    }
//    else
//    {
//
//        if(swapFlag==0)
//        {
//            fbos=getFBOByName(fboName+"1");
//        }
//        else
//        {
//            fbos=getFBOByName(fboName+"0");
//        }
//
//    }
//
//    if(fbos)
//    {
//        Renderer::bindFBODirect(fbos, doClear);
//    }
//    else
//    {
//        doTrace("bindFBO: Invalid FBO Name");
//    }
//
//
//}
//
//void FBOManager::unbindFBO()
//{
//    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
//    setMatrices(baseW, baseH);
//
//}
//
//void FBOManager::sampleFBODirect(
//    FBOSet *fbos,
//    int offset, /* write offset */
//    int _minOff, /* read min */
//    int _maxOff /* read max */
//
//)
//{
//    int i;
//
//    int minOff=_minOff;
//    int maxOff=_maxOff;
//    if(maxOff==-1)
//    {
//        maxOff=fbos->numBufs;
//    }
//
//    if(shadersAreLoaded)
//    {
//        for(i=minOff; i<maxOff; i++)
//        {
//            setShaderTexture(i-minOff+offset, fbos->fbos[i].color_tex);
//        }
//    }
//}
//
//void FBOManager::unsampleFBODirect(
//    FBOSet *fbos,
//    int offset, /* write offset */
//    int _minOff, /* read min */
//    int _maxOff /* read max */
//
//)
//{
//    int i;
//
//    int minOff=_minOff;
//    int maxOff=_maxOff;
//    if(maxOff==-1)
//    {
//        maxOff=fbos->numBufs;
//    }
//
//    if(shadersAreLoaded)
//    {
//        for(i=maxOff-1; i>=minOff; i--)
//        {
//            setShaderTexture(i-minOff+offset, 0);
//        }
//    }
//}
//
//
//void FBOManager::getMatrixFromFBO(std::string fboName)
//{
//    FBOSet *fbos=getFBOByName(fboName);
//    setMatrices(fbos->width, fbos->height);
//}



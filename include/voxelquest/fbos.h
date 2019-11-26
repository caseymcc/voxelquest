#ifndef _voxelquest_fbos_h_
#define _voxelquest_fbos_h_

#include "voxelquest/vectors.h"

#include <glbinding/gl/gl.h>

#include <map>

using namespace gl;

const static GLenum bufNames[]={
    GL_COLOR_ATTACHMENT0_EXT,
    GL_COLOR_ATTACHMENT1_EXT,
    GL_COLOR_ATTACHMENT2_EXT,
    GL_COLOR_ATTACHMENT3_EXT,
    GL_COLOR_ATTACHMENT4_EXT,
    GL_COLOR_ATTACHMENT5_EXT,
    GL_COLOR_ATTACHMENT6_EXT,
    GL_COLOR_ATTACHMENT7_EXT
};

class FBOWrapper
{
public:
    FBOWrapper() {}
    ~FBOWrapper() {}

    int init(
        int _width,
        int _height,
        int _bytesPerChannel,
        GLenum _slot,
        GLenum filterEnum,
        GLenum clampEnum,
        bool isMultisample=false
    );


    void cpuToGPU();



    void getPixelAtF(FIVector4 *fv, int x, int y);

    void setAllPixels(int channel, int value);

    void setPixelAtC(int x, int y, int channel, unsigned char value);


    int getIndex(int xs, int ys);

    void setPixelAtWrapped(int xs, int ys, int channel, int val);

    int getPixelAtWrapped(int xs, int ys, int channel);




    int getPixelAtIndex(int ind, int channel);
    void setPixelAtIndex(int ind, int channel, int val);
    void orPixelAtIndex(int ind, int channel, int val);
    void andPixelAtIndex(int ind, int channel, int val);

    int getPixelAtC(int x, int y, int channel);

    float getPixelAtLinear(float xf, float yf, int channel);


    int getMipVal(
        int x,
        int y,
        int mipLev,
        int channel,
        int minMaxAvg,
        int val=-1,
        int ox=0,
        int oy=0
    );

    int getMipAtIndex(int ind, int mipLev, int channel, int minMaxAvg);

    int getMipInd(int x, int y, int mipLev);

    void updateMips();



    int getPixelAtIndex3DMip(int ind, int channel, int mval, int mipLev);

    void updateMips3D(int basePitch);



    void getPixelsFast();

    // assumed that remoteBuffer is allocated
    void getPixelsFastRemote(uint* remoteBuffer);


    void getPixels(bool _hasMipMap=false);

    GLuint getColorTex() { return color_tex; }

private:
    GLuint color_tex;

    //uint color_buf;
    //uint depth_buf;
    GLenum slot;
    int width;
    int height;
    int bytesPerChannel;
    int numMips;
    bool hasMipMap;
    //bool hasDepth;

    GLint numBytes;
    GLenum internalFormat;

    uint *pixelsUINT;
    unsigned char *pixelsChar;
    unsigned char **pixelsCharMippedMin;
    unsigned char **pixelsCharMippedMax;
    unsigned char **pixelsCharMippedAvg;
    int *mipWidths;

    float *pixelsFloat;
    bool isFloat;
};


class FBOSet
{
public:
    FBOSet() {}
    ~FBOSet() {}

    FBOWrapper *getFBOWrapper(int offset);

    void init(
        int _numBufs,
        int _width,
        int _height,
        int _bytesPerChannel,
        bool _hasDepth,
        GLenum filterEnum=GL_NEAREST,
        GLenum clampEnum=GL_CLAMP_TO_EDGE,
        bool isMultisample=false
    );
    void bind(int doClear);
    void copyFromMem(int ind, unsigned char *dat);

private:
    int numBufs;
    int width;
    int height;
    int bytesPerChannel;

    uint depth_rb;

    GLuint mFBO;

    FBOWrapper *fbos;

    bool isReady;
    bool hasDepth;

   
};

class FBOManager
{
public:
    static FBOManager *singleton();

    FBOSet* getFBOByName(std::string &fboName);

    void sampleFBO(
        std::string fboName,
        int offset=0,
        int swapFlag=-1,
        int minOff=0,
        int maxOff=-1
    );

    void unsampleFBO(
        std::string fboName,
        int offset=0,
        int swapFlag=-1,
        int minOff=0,
        int maxOff=-1
    );

    static FBOSet *getFBOSet(std::string fboName);

    static FBOWrapper *getFBOWrapper(std::string fboName, int offset);

    void copyFBO(std::string src, std::string dest, int num=0);

    void copyFBO2(std::string src, std::string dest, int num1=0, int num2=1);

    void copyFBO3(std::string src, std::string dest, int num1=0, int num2=1, int num3=2);

    void bindFBO(std::string fboName, int swapFlag=-1, int doClear=1);

    void unbindFBO();

    void sampleFBODirect(
        FBOSet *fbos,
        int offset=0, /* write offset */
        int _minOff=0, /* read min */
        int _maxOff=-1 /* read max */

    );

    void unsampleFBODirect(
        FBOSet *fbos,
        int offset=0, /* write offset */
        int _minOff=0, /* read min */
        int _maxOff=-1 /* read max */

    );

    void getMatrixFromFBO(std::string fboName);

    void bindFBODirect(FBOSet *fbos, int doClear=1);

private:
    FBOManager() {}//make it a singleton

    std::map<std::string, FBOSet> fboMap;
};

#endif//_voxelquest__h_
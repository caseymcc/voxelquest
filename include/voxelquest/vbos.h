#ifndef _voxelquest_vbos_h_
#define _voxelquest_vbos_h_

#include "voxelquest/types.h"

#include <glbinding/gl/gl.h>
using namespace gl;

#include <vector>

struct VIStruct
{
    std::vector<float> vertexVec; //btScalar
    std::vector<uint> indexVec; //unsigned short
};

class VBOWrapper
{
public:
    VBOWrapper();

    ~VBOWrapper();

    void remVI();

    void setVI(VIStruct* _vi, bool _viIsShared);

    int getNumVerts();

    void init(
        int _numVecs, // number of 4 component vecs
        int _drawEnum //GL_DYNAMIC_DRAW GL_STATIC_DRAW	
    );

    void deallocVBO();

    void clearVecs(bool shrinkToFit=true);

    void checkInit();

    void update();

    void updateNew();

    void beginFill();

    void endFill();

    void draw();

    void drawCubes(int numCubes);

    void drawPoints();

    // inline void getIndVal(int procCount) ;

    // inline void getIndVal2(int procCount) ;

    inline void getPixVal(
        //FBOWrapper* fbow0,
        //FBOWrapper* fbow1,
        //int ind,
        float xb, float yb, float zb,
        float xm, float ym, float zm,
        float* data,
        int dataLen
    );

    void vboBox(
        float bpX,
        float bpY,
        float bpZ,

        float iv0,
        float iv1,

        uint procFlag,

        float* data,
        int dataLen
    );

private:


    void updateBase(
        GLfloat* _vertexData,
        int _sizeOfVD,
        GLuint* _indexData,
        int _sizeOfID
    );


    void updateNewBase(
        GLfloat* _vertexData,
        int _sizeOfVD,
        GLuint* _indexData,
        int _sizeOfID
    );

    void initBase(
        GLfloat* _vertexData,
        int _sizeOfVD,
        int _maxSizeOfVD,
        GLuint* _indexData,
        int _sizeOfID,
        int _maxSizeOfID
    );

public:
    GLuint vao, vbo, ibo;

    float lastVMUsage;
    int drawEnum;
    int sizeOfID;
    int maxSizeOfID;
    int sizeOfVD;
    int maxSizeOfVD;
    int numVecs;
    int attSize;
    bool hasInit;

    int procCount;

    VIStruct* vi;

    bool viIsShared;
};


class VBOGrid
{
public:
    VBOGrid();


    void init(
        int _xpitch,
        int _ypitch
    );


    int xpitch;
    int ypitch;

    int totVerts;
    int totInd;

    VBOWrapper vboWrapper;
};

#endif//_voxelquest__h_
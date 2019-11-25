#ifndef _voxelquest_vbos_h_
#define _voxelquest_vbos_h_

class VBOWrapper
{
public:
    VBOWrapper();

    void update(
        GLfloat* _vertexData,
        int _sizeOfVD,
        GLuint* _indexData,
        int _sizeOfID
    );

    void updateNew();

    void init(
        GLfloat* _vertexData,
        int _sizeOfVD,
        int _maxSizeOfVD,
        GLuint* _indexData,
        int _sizeOfID,
        int _maxSizeOfID,
        int _numVecs, // number of 4 component vecs
        int _drawEnum //GL_DYNAMIC_DRAW GL_STATIC_DRAW
    );

    void draw();

    void drawPoints();

private:
    GLuint vao, vbo, ibo;

    int drawEnum;
    int sizeOfID;
    int maxSizeOfID;
    int sizeOfVD;
    int maxSizeOfVD;
    int numVecs;
    int attSize;
    // GLfloat* vertexData;
    // GLuint* indexData;
};


class VBOGrid
{
public:
    VBOGrid();

    void init(
        int _xpitch,
        int _ypitch
    );

private:

    int xpitch;
    int ypitch;

    int totVerts;
    int totInd;

    VBOWrapper vboWrapper;


    std::vector<float> vertexVec;
    std::vector<uint> indexVec;
};

#endif//_voxelquest__h_
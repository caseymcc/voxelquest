#ifndef _voxelquest_texture3d_h_
#define _voxelquest_texture3d_h_

#include "voxelquest/types.h"

#include <glbinding/gl/gl.h>

using namespace gl;

class Texture3D
{
public:
    GLuint id;

    int x, y, z;
    int dataSize;

    Texture3D();

    void init(int x, int y, int z, uint *data=nullptr, int _dataSize=0);

    void update(uint * data, int _dataSize);
};

#endif//_voxelquest_texture3d_h_
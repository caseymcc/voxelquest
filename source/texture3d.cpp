#include "voxelquest/texture3d.h"

#include <cassert>

Texture3D::Texture3D()
{

}

void Texture3D::init(int _x, int _y, int _z, uint *data,int _dataSize)
{
    _x=x;
    _y=y;
    _z=z;
    dataSize=_x*_y*_z;

    glGenTextures(1, &id);
    
    glBindTexture(GL_TEXTURE_3D, id);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, x, y, z, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, 0);
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_3D, 0);

    if(data)
        update(data, _dataSize);
}

void Texture3D::update(uint* data, int _dataSize)
{
    assert(dataSize==_dataSize);

    glBindTexture(GL_TEXTURE_3D, id);
    
    glTexSubImage3D(
        GL_TEXTURE_3D,
        0,

        0,
        0,
        0,

        x,
        y,
        z,

        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );
    glBindTexture(GL_TEXTURE_3D, 0);
}

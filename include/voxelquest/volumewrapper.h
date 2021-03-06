#ifndef _voxelquest_volumewrapper_h_
#define _voxelquest_volumewrapper_h_

#include "voxelquest/fbos.h"

class VolumeWrapper
{
public:
    VolumeWrapper();

    void init(int z, GLenum clampMethod, bool _isFloat, int filterType);

    void copyFloatArr(float* floatArr);

    void copyCharArr(unsigned char* charArr);

//private:
    FBOSet fboSet;

    GLuint volId;
    FIVector4 terGenDim;

    FIVector4 genPosMin;
    FIVector4 genPosMax;

    bool isFloat;
    bool isReady;

};

#endif//_voxelquest__h_
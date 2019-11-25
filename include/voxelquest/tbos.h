#ifndef _voxelquest_tbos_h_
#define _voxelquest_tbos_h_

class TBOWrapper
{
public:
    GLuint tbo_buf;
    GLuint tbo_tex;

    int dataSize;

    bool isFloat;

    TBOWrapper();

    void init(bool _isFloat, float* tbo_data, uint* tbo_data2, int _dataSize);

    void update(float* tbo_data, uint* tbo_data2, int newDataSize);


};

#endif//_voxelquest_tbos_h_
#include "tbos.h"

TBOWrapper::TBOWrapper()
{

}

void TBOWrapper::init(bool _isFloat, float* tbo_data, uint* tbo_data2, int _dataSize)
{

    isFloat=_isFloat;

    dataSize=_dataSize;

    glGenBuffers(1, &tbo_buf);
    glBindBuffer(GL_TEXTURE_BUFFER, tbo_buf);

    if(isFloat)
    {
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, tbo_buf);
        glBufferData(GL_TEXTURE_BUFFER, dataSize, tbo_data, GL_DYNAMIC_DRAW); // todo: dynamic draw? //GL_STATIC_DRAW

    }
    else
    {
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, tbo_buf);
        glBufferData(GL_TEXTURE_BUFFER, dataSize, tbo_data2, GL_DYNAMIC_DRAW); // todo: dynamic draw? //GL_STATIC_DRAW

    }

    glGenTextures(1, &tbo_tex);
    glBindBuffer(GL_TEXTURE_BUFFER, 0);
}

void TBOWrapper::update(float* tbo_data, uint* tbo_data2, int newDataSize)
{

    int tempDataSize;

    if(newDataSize<0)
    {
        tempDataSize=dataSize;
    }
    else
    {
        tempDataSize=newDataSize;
    }


    tempDataSize=min(tempDataSize, dataSize);



    glBindBuffer(GL_TEXTURE_BUFFER, tbo_buf);
    if(isFloat)
    {
        glBufferSubData(GL_TEXTURE_BUFFER, 0, tempDataSize, tbo_data);
    }
    else
    {
        glBufferSubData(GL_TEXTURE_BUFFER, 0, tempDataSize, tbo_data2);
    }

    glBindBuffer(GL_TEXTURE_BUFFER, 0);
}

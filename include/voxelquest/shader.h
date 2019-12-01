#ifndef _voxelquest_shader_h_
#define _voxelquest_shader_h_

#include "voxelquest/vectors.h"
#include "voxelquest/uniformbuffer.h"

#include <LinearMath/btVector3.h>

#include <vector>
#include <string>
#include <map>

class Shader
{
public:
    Shader(Singleton* _singleton);
    ~Shader();

    static char* textFileRead(const char *fileName);

    static void validateShader(GLuint shader, const char* file=0);

    static int validateProgram(GLuint program);

    int countOc(std::string* src, std::string testStr);

    void init(std::string shaderName, bool doBake, std::map<std::string, std::string>* includeMap);

    unsigned int id();

    void bind();

    void updateUniformBlock(int ubIndex, int ubDataSize);
    void invalidateUniformBlock(int ubIndex);
    void beginUniformBlock(int ubIndex);
    bool wasUpdatedUniformBlock(int ubIndex);

    void unbind();

    void setTexture(const GLchar* name, int texUnit, int texId);


    void setVec(const GLchar* name, const GLfloat* vecData, int vecSize);

    void setVecString(std::string name, const GLfloat* vecData, int vecSize);

    // void glUniformMatrix4fv(	GLint location,
    //  	GLsizei count,
    //  	GLboolean transpose,
    //  	const GLfloat *value);

    void setShaderMatrix4x4(std::string paramName, float* x, int count);

    void setShaderMatrix3x3(std::string paramName, float* x, int count);


    void setShaderArrayfVec4(std::string paramName, float* x, int count);

    void setShaderArrayfVec3(std::string paramName, float* x, int count);

    void setShaderArray(std::string paramName, float* x, int count);

    GLint getShaderLoc(std::string paramName);

    void setShaderFloat(std::string paramName, float x);
    void setShaderVec2(std::string paramName, float x, float y);
    void setShaderVec3(std::string paramName, float x, float y, float z);
    void setShaderVec4(std::string paramName, float x, float y, float z, float w);

    void setShaderInt(std::string paramName, int x);

    void setShaderfVec2(std::string paramName, FIVector4* f);
    void setShaderfVec3(std::string paramName, FIVector4* f);
    void setShaderfVec4(std::string paramName, FIVector4* f);

    void setShaderbtVec3(std::string paramName, btVector3 f);

    void setShaderFloatUB(std::string paramName, float x);
    void setShaderfVec4UB(std::string paramName, FIVector4* f);

private:
    unsigned int shader_id;
    unsigned int shader_vp;
    unsigned int shader_fp;

    int curUBIndex;

    std::map<std::string, float> paramMap;
    std::map<std::string, float> paramMapTemp;
    std::vector<std::string> paramVec;
    std::vector<UniformBuffer> uniVec;
    Singleton* singleton;

	std::string localString;
};


#endif//_voxelquest__h_
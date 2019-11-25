#ifndef _voxelquest_shader_h_
#define _voxelquest_shader_h_

class Shader
{
public:
    Shader(Singleton* _singleton);
    ~Shader();

    static char* textFileRead(const char *fileName);

    static void validateShader(GLuint shader, const char* file=0);

    static int validateProgram(GLuint program);

    int countOc(string* src, string testStr);

    void init(string shaderName, bool doBake, map<string, string>* includeMap);

    unsigned int id();

    void bind();

    void updateUniformBlock(int ubIndex, int ubDataSize);
    void invalidateUniformBlock(int ubIndex);
    void beginUniformBlock(int ubIndex);
    bool wasUpdatedUniformBlock(int ubIndex);

    void unbind();

    void setTexture(const GLchar* name, int texUnit, int texId);


    void setVec(const GLchar* name, const GLfloat* vecData, int vecSize);

    void setVecString(string name, const GLfloat* vecData, int vecSize);

    // void glUniformMatrix4fv(	GLint location,
    //  	GLsizei count,
    //  	GLboolean transpose,
    //  	const GLfloat *value);

    void setShaderMatrix4x4(string paramName, float* x, int count);

    void setShaderMatrix3x3(string paramName, float* x, int count);


    void setShaderArrayfVec4(string paramName, float* x, int count);

    void setShaderArrayfVec3(string paramName, float* x, int count);

    void setShaderArray(string paramName, float* x, int count);

    GLint getShaderLoc(string paramName);

    void setShaderFloat(string paramName, float x);
    void setShaderVec2(string paramName, float x, float y);
    void setShaderVec3(string paramName, float x, float y, float z);
    void setShaderVec4(string paramName, float x, float y, float z, float w);

    void setShaderInt(string paramName, int x);



    void setShaderfVec2(string paramName, FIVector4* f);
    void setShaderfVec3(string paramName, FIVector4* f);
    void setShaderfVec4(string paramName, FIVector4* f);


    void setShaderbtVec3(string paramName, btVector3 f);


    void setShaderFloatUB(string paramName, float x);
    void setShaderfVec4UB(string paramName, FIVector4* f);

private:
    unsigned int shader_id;
    unsigned int shader_vp;
    unsigned int shader_fp;

    int curUBIndex;

    map<string, float> paramMap;
    map<string, float> paramMapTemp;
    vector<string> paramVec;
    vector<UniformBuffer> uniVec;
    Singleton* singleton;

    string localString;
};


#endif//_voxelquest__h_
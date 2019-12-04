#ifndef _voxelquest_helperclasses_h_
#define _voxelquest_helperclasses_h_

#include <glbinding/gl/gl.h>

using namespace gl;

class UniformBuffer
{
public:

	UniformBuffer();

	void init(GLuint _progId, int bufNameInd);


	void updateUniformBlock(int numFloats);

	void beginUniformBlock();

	void invalidateUniformBlock();
	bool wasUpdatedUniformBlock();

	~UniformBuffer();

//private:



    // GLchar* names[] =
    // {
    // 	"SurfaceColor",
    // 	"WarmColor",
    // 	"CoolColor",
    // 	"DiffuseWarm",
    // 	"DiffuseCool"
    // };

    int uniPosition;

    GLuint bufferId, uniformBlockIndex, index, progId;
    GLsizei uniformBlockSize;
    GLint singleSize, offset;
    GLfloat* uniData;

    // GLfloat colors[] = 
    // {
    // 	0.45,0.45,1,1,
    // 	0.45,0.45,1,1,
    // 	0.75,0.75,0.75,1,
    // 	0.0,0.0,1.0,1,
    // 	0.0,1.0,0.0,1
    // };


    bool wasUpdated;
    bool wasInit;




};









#endif//_voxelquest__h_
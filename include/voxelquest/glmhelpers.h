#include "voxelquest/mat.h"
#include "voxelquest/vectors.h"

#include <glm/glm.hpp>

inline glm::vec3 toVec3(const Vector3 &vec)
{
    return glm::vec3(vec[0], vec[1], vec[2]);
}

inline glm::vec3 toVec3(FIVector4 &vec)
{
    return glm::vec3(vec.getFX(), vec.getFY(), vec.getFZ());
}

inline glm::ivec3 toiVec3(FIVector4 &vec)
{
    return glm::ivec3(vec.getIX(), vec.getIY(), vec.getIZ());
}

inline glm::vec4 toVec4(FIVector4 &vec)
{
    return glm::vec4(vec.getFX(), vec.getFY(), vec.getFZ(), vec.getFW());
}

inline glm::ivec4 toiVec4(FIVector4 &vec)
{
    return glm::ivec4(vec.getIX(), vec.getIY(), vec.getIZ(), vec.getIW());
}

template<typename _Mat>
glm::mat4 toMat4(const _Mat &mat)
{
    return glm::mat4(
        mat[0], mat[1], mat[2], mat[3],
        mat[4], mat[5], mat[6], mat[7],
        mat[8], mat[9], mat[10], mat[11],
        mat[12], mat[13], mat[14], mat[15]
    );
}

inline Vector3 toVector3(const glm::vec3 &vec)
{
    return Vector3(vec.x, vec.y, vec.z);
}

inline FIVector4 toFIVector4(const glm::vec3 &vec)
{
    FIVector4 fiVec;

    fiVec.setFXYZ(vec.x, vec.y, vec.z);
    return fiVec;
}

inline FIVector4 toFIVector4(const glm::ivec3 &vec)
{
    FIVector4 fiVec;

    fiVec.setIXYZ(vec.x, vec.y, vec.z);
    return fiVec;
}

inline FIVector4 toFIVector4(const glm::vec4 &vec)
{
    FIVector4 fiVec;

    fiVec.setFXYZW(vec.x, vec.y, vec.z, vec.w);
    return fiVec;
}

inline FIVector4 toFIVector4(const glm::ivec4 &vec)
{
    FIVector4 fiVec;

    fiVec.setIXYZW(vec.x, vec.y, vec.z, vec.w);
    return fiVec;
}

inline Matrix4 toMatrix4(const glm::mat4 &mat)
{
    return Matrix4(
        mat[0][0], mat[0][1], mat[0][2], mat[0][3],
        mat[1][0], mat[1][1], mat[1][2], mat[1][3],
        mat[2][0], mat[2][1], mat[2][2], mat[2][3],
        mat[3][0], mat[3][1], mat[3][2], mat[3][3]
    );
}
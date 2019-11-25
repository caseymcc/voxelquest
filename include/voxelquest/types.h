#ifndef _voxelquest_types_h_
#define _voxelquest_types_h_

#include <vector>

typedef unsigned int uint;

struct intPair
{
    int v0;
    int v1;
};

bool operator==(const intPair& lhs, const intPair& rhs)
{
    return (lhs.v0==rhs.v0)&&(lhs.v1==rhs.v1);
}

bool operator>(const intPair& lhs, const intPair& rhs)
{
    long lhs2=lhs.v1;
    long rhs2=rhs.v1;

    lhs2=lhs2<<16;
    rhs2=rhs2<<16;

    lhs2|=lhs.v0;
    rhs2|=rhs.v0;


    return (lhs2)>(rhs2);
}

bool operator<(const intPair& lhs, const intPair& rhs)
{
    long lhs2=lhs.v1;
    long rhs2=rhs.v1;

    lhs2=lhs2<<16;
    rhs2=rhs2<<16;

    lhs2|=lhs.v0;
    rhs2|=rhs.v0;

    return (lhs2)<(rhs2);
}

struct intPairVec
{
    std::vector<intPair> data;
};

struct intVec
{
    std::vector<int> data;
};

struct FloatVec
{
    std::vector<float> data;
};

struct charArr
{
    charArr():data(nullptr), size(0) {}

    long size;
    char *data;
};

#endif//_voxelquest_types_h_
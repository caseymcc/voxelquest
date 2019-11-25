#include "ivec.h"

ivec3 toIVEC(vec3 val) {
    return ivec3(val.x,val.y,val.z);
}
vec3 toVEC(ivec3 val) {
    return vec3(val.x,val.y,val.z);
}


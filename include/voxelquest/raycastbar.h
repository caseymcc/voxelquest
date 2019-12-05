#ifndef _voxelquest_raycastbar_h_
#define _voxelquest_raycastbar_h_

#include <LinearMath/btVector3.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>

#define NUMRAYS 500

class btRaycastBar2
{
public:
    btVector3 source[NUMRAYS];
    btVector3 dest[NUMRAYS];
    btVector3 direction[NUMRAYS];
    btVector3 hit[NUMRAYS];
    btVector3 normal[NUMRAYS];
    struct GUIHelperInterface* m_guiHelper;

    int frame_counter;
    int ms;
    int sum_ms;
    int sum_ms_samples;
    int min_ms;
    int max_ms;

#ifdef USE_BT_CLOCK
    btClock frame_timer;
#endif //USE_BT_CLOCK

    btScalar dx;
    btScalar min_x;
    btScalar max_x;
    btScalar max_y;
    btScalar sign;

    btRaycastBar2();

    btRaycastBar2(btScalar ray_length, btScalar z, btScalar max_y, struct GUIHelperInterface* guiHelper);

    void move(btScalar dt);

    void cast(btCollisionWorld* cw);

    void draw();
};

#endif//_voxelquest_raycastbar_h_
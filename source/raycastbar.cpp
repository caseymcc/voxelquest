#include "voxelquest/raycastbar.h"

#include <LinearMath/btQuaternion.h>
#include <CommonInterfaces/CommonGUIHelperInterface.h>
#include <CommonInterfaces/CommonRenderInterface.h>

btRaycastBar2::btRaycastBar2()
{
    m_guiHelper=0;
    ms=0;
    max_ms=0;
    min_ms=9999;
    sum_ms_samples=0;
    sum_ms=0;
}

btRaycastBar2::btRaycastBar2(btScalar ray_length, btScalar z, btScalar max_y, struct GUIHelperInterface* guiHelper)
{
    m_guiHelper=guiHelper;
    frame_counter=0;
    ms=0;
    max_ms=0;
    min_ms=9999;
    sum_ms_samples=0;
    sum_ms=0;
    dx=10.0;
    min_x=0;
    max_x=0;
    this->max_y=max_y;
    sign=1.0;
    btScalar dalpha=2*SIMD_2_PI/NUMRAYS;
    for(int i=0; i<NUMRAYS; i++)
    {
        btScalar alpha=dalpha*i;
        // rotate around by alpha degrees y 
        btQuaternion q(btVector3(0.0, 1.0, 0.0), alpha);
        direction[i]=btVector3(1.0, 0.0, 0.0);
        direction[i]=quatRotate(q, direction[i]);
        direction[i]=direction[i]*ray_length;


        source[i]=btVector3(min_x, max_y, z);
        dest[i]=source[i]+direction[i];
        dest[i][1]=-1000;
        normal[i]=btVector3(1.0, 0.0, 0.0);
    }
}

void btRaycastBar2::move(btScalar dt)
{
    if(dt>btScalar(1.0/60.0))
        dt=btScalar(1.0/60.0);
    for(int i=0; i<NUMRAYS; i++)
    {
        source[i][0]+=dx*dt * sign;
        dest[i][0]+=dx*dt * sign;
    }
    if(source[0][0]<min_x)
        sign=1.0;
    else if(source[0][0]>max_x)
        sign=-1.0;
}

void btRaycastBar2::cast(btCollisionWorld* cw)
{
#ifdef USE_BT_CLOCK
    frame_timer.reset();
#endif //USE_BT_CLOCK

#ifdef BATCH_RAYCASTER
    if(!gBatchRaycaster)
        return;

    gBatchRaycaster->clearRays();
    for(int i=0; i<NUMRAYS; i++)
    {
        gBatchRaycaster->addRay(source[i], dest[i]);
    }
    gBatchRaycaster->performBatchRaycast();
    for(int i=0; i<gBatchRaycaster->getNumRays(); i++)
    {
        const SpuRaycastTaskWorkUnitOut& out=(*gBatchRaycaster)[i];
        hit[i].setInterpolate3(source[i], dest[i], out.hitFraction);
        normal[i]=out.hitNormal;
        normal[i].normalize();
    }
#else
    for(int i=0; i<NUMRAYS; i++)
    {
        btCollisionWorld::ClosestRayResultCallback cb(source[i], dest[i]);

        cw->rayTest(source[i], dest[i], cb);
        if(cb.hasHit())
        {
            hit[i]=cb.m_hitPointWorld;
            normal[i]=cb.m_hitNormalWorld;
            normal[i].normalize();
        }
        else
        {
            hit[i]=dest[i];
            normal[i]=btVector3(1.0, 0.0, 0.0);
        }

    }
#ifdef USE_BT_CLOCK
    ms+=frame_timer.getTimeMilliseconds();
#endif //USE_BT_CLOCK
    frame_counter++;
    if(frame_counter>50)
    {
        min_ms=ms<min_ms?ms:min_ms;
        max_ms=ms>max_ms?ms:max_ms;
        sum_ms+=ms;
        sum_ms_samples++;
        btScalar mean_ms=(btScalar)sum_ms/(btScalar)sum_ms_samples;
        printf("%d rays in %d ms %d %d %f\n", NUMRAYS * frame_counter, ms, min_ms, max_ms, mean_ms);
        ms=0;
        frame_counter=0;
    }
#endif
}

void btRaycastBar2::draw()
{

    if(m_guiHelper)
    {
        btAlignedObjectArray<unsigned int> indices;
        btAlignedObjectArray<btVector3FloatData> points;


        float lineColor[4]={1.0f, 0.4f, 0.4f, 1.0f};

        for(int i=0; i<NUMRAYS; i++)
        {
            btVector3FloatData s, h;
            for(int w=0; w<4; w++)
            {
                s.m_floats[w]=source[i][w];
                h.m_floats[w]=hit[i][w];
            }

            points.push_back(s);
            points.push_back(h);
            indices.push_back(indices.size());
            indices.push_back(indices.size());
        }

        m_guiHelper->getRenderInterface()->drawLines(&points[0].m_floats[0], lineColor, points.size(), sizeof(btVector3), &indices[0], indices.size(), 1);
    }

#if 0
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    int i;

    for(i=0; i<NUMRAYS; i++)
    {
        glVertex3f(source[i][0], source[i][1], source[i][2]);
        glVertex3f(hit[i][0], hit[i][1], hit[i][2]);
    }
    glEnd();
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    for(i=0; i<NUMRAYS; i++)
    {
        glVertex3f(hit[i][0], hit[i][1], hit[i][2]);
        glVertex3f(hit[i][0]+normal[i][0], hit[i][1]+normal[i][1], hit[i][2]+normal[i][2]);
    }
    glEnd();
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    for(i=0; i<NUMRAYS; i++)
    {
        glVertex3f(hit[i][0], hit[i][1], hit[i][2]);
    }
    glEnd();
    glEnable(GL_LIGHTING);
#endif //USE_GRAPHICAL_BENCHMARK

}
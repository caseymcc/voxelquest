#ifndef _voxelquest_mydebugdrawer_h_
#define _voxelquest_mydebugdrawer_h_

#include "LinearMath/btVector3.h"
#include "LinearMath/btIDebugDraw.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "CommonInterfaces/CommonGraphicsAppInterface.h"

struct MyDebugVec3
{
    MyDebugVec3(const btVector3& org)
        :x(org.x()),
        y(org.y()),
        z(org.z())
    {}

    float x;
    float y;
    float z;
};

class MyDebugDrawer: public btIDebugDraw
{
    CommonGraphicsApp* m_glApp;
    int m_debugMode;

    btAlignedObjectArray<MyDebugVec3> m_linePoints;
    btAlignedObjectArray<unsigned int> m_lineIndices;
    btVector3 m_currentLineColor;
    DefaultColors m_ourColors;

public:

    MyDebugDrawer(CommonGraphicsApp* app);

    virtual DefaultColors	getDefaultColors() const;

    ///the default implementation for setDefaultColors has no effect. A derived class can implement it and store the colors.
    virtual void setDefaultColors(const DefaultColors& colors);



    virtual void drawLine(const btVector3& from1, const btVector3& to1, const btVector3& color1);


    virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);



    virtual void reportErrorWarning(const char* warningString);

    virtual void draw3dText(const btVector3& location, const char* textString);

    virtual void setDebugMode(int debugMode);

    virtual int		getDebugMode() const;

    virtual void flushLines();


};

#endif//_voxelquest_mydebugdrawer_h_

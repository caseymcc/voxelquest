#include "voxelquest/mydebugdrawer.h"

#define BT_LINE_BATCH_SIZE 512

MyDebugDrawer::MyDebugDrawer(CommonGraphicsApp* app)
    : m_glApp(app)
    , m_debugMode(btIDebugDraw::DBG_DrawWireframe|btIDebugDraw::DBG_DrawAabb),
    m_currentLineColor(-1, -1, -1)
{


}

MyDebugDrawer::DefaultColors MyDebugDrawer::getDefaultColors() const
{
    return m_ourColors;
}
///the default implementation for setDefaultColors has no effect. A derived class can implement it and store the colors.
void MyDebugDrawer::setDefaultColors(const DefaultColors& colors)
{
    m_ourColors=colors;
}


void MyDebugDrawer::drawLine(const btVector3& from1, const btVector3& to1, const btVector3& color1)
{
    //float from[4] = {from1[0],from1[1],from1[2],from1[3]};
    //float to[4] = {to1[0],to1[1],to1[2],to1[3]};
    //float color[4] = {color1[0],color1[1],color1[2],color1[3]};
    //m_glApp->m_instancingRenderer->drawLine(from,to,color);
    if(m_currentLineColor!=color1||m_linePoints.size()>=BT_LINE_BATCH_SIZE)
    {
        flushLines();
        m_currentLineColor=color1;
    }
    MyDebugVec3 from(from1);
    MyDebugVec3 to(to1);

    m_linePoints.push_back(from);
    m_linePoints.push_back(to);

    m_lineIndices.push_back(m_lineIndices.size());
    m_lineIndices.push_back(m_lineIndices.size());

}

void MyDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
    drawLine(PointOnB, PointOnB+normalOnB, color);
}


void MyDebugDrawer::reportErrorWarning(const char* warningString)
{}

void MyDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{}

void MyDebugDrawer::setDebugMode(int debugMode)
{
    m_debugMode=debugMode;
}

int		MyDebugDrawer::getDebugMode() const
{
    return m_debugMode;
}

void MyDebugDrawer::flushLines()
{
    int sz=m_linePoints.size();
    if(sz)
    {
        float debugColor[4];
        debugColor[0]=m_currentLineColor.x();
        debugColor[1]=m_currentLineColor.y();
        debugColor[2]=m_currentLineColor.z();
        debugColor[3]=1.f;
        m_glApp->m_renderer->drawLines(&m_linePoints[0].x, debugColor,
            m_linePoints.size(), sizeof(MyDebugVec3),
            &m_lineIndices[0],
            m_lineIndices.size(),
            1);
        m_linePoints.clear();
        m_lineIndices.clear();
    }
}
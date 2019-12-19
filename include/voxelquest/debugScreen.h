#ifndef _voxelquest_debugscreen_h_
#define _voxelquest_debugscreen_h_

class DebugScreen
{
public:
    DebugScreen();

    void initialize();
    void terminate();

    void setSize(int width, int height);

    void startBuild();

    void update();
    void updateControls();
    void updateHelp();
    void updateMap();
    void updateSettings();

    void build();
    void draw();

private:
    int m_width;
    int m_height;
};

#endif //_voxelquest_debugscreen_h_

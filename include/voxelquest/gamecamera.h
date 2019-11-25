#ifndef _voxelquest_gamecamera_h_
#define _voxelquest_gamecamera_h_


class GameCamera {
private:
    
public:
    GLfloat unitPos[3];
    
    
    float accel[3];
    float accelA;
    float accelB;
    float lastAccelA;
    float lastAccelB;
    float subjectDistance;
    
    GLfloat rotation[2];
    
    GameCamera() 
    
    
    
    void focusOn(int entID) 
    
    
    
    void testCollision(float oldX, float oldY, float oldZ, float newX, float newY, float newZ, bool skipTest) 

    
    void setUnitPosition(float x, float y, float z) 
    
    void addUnitPosition(float x, float y, float z) 
    
    void setRotation(float a, float b) 
    
    void addRotation(float a, float b) 
    
    void init() 
    
};

#endif//_voxelquest__h_
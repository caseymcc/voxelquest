#ifndef _voxelquest_dynbuffer_h_
#define _voxelquest_dynbuffer_h_


class DynBuffer
{
public:
    DynBuffer();

    ~DynBuffer();

    void setVsync(bool enabled);

    ///////////////////////////////////////////////////////////////////////////////
    // initialize global variables
    ///////////////////////////////////////////////////////////////////////////////
    bool initSharedMem();

    ///////////////////////////////////////////////////////////////////////////////
    // clean up shared memory
    ///////////////////////////////////////////////////////////////////////////////
    void clearSharedMem();

    ///////////////////////////////////////////////////////////////////////////////
    // set camera position and lookat direction
    ///////////////////////////////////////////////////////////////////////////////
    void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);

    ///////////////////////////////////////////////////////////////////////////////
    // copy an image data to texture buffer
    ///////////////////////////////////////////////////////////////////////////////
    void updatePixels(GLubyte* dst, int size);

    ///////////////////////////////////////////////////////////////////////////////
    // display transfer rates
    ///////////////////////////////////////////////////////////////////////////////
    void showTransferRate();

    ///////////////////////////////////////////////////////////////////////////////
    // set projection matrix as orthogonal
    ///////////////////////////////////////////////////////////////////////////////
    void toOrtho();

    ///////////////////////////////////////////////////////////////////////////////
    // set the projection matrix as perspective
    ///////////////////////////////////////////////////////////////////////////////
    void toPerspective();

    //=============================================================================
    // CALLBACKS
    //=============================================================================

    void displayCB();
    
    void reshapeCB(int width, int height);

    void idleCB();

    void keyboardCB(unsigned char key, int x, int y);

    void mouseCB(int button, int state, int x, int y);

    void mouseMotionCB(int x, int y);

private:
    // constants
    int    SCREEN_WIDTH;
    int    SCREEN_HEIGHT;
    float  CAMERA_DISTANCE;
    int    TEXT_WIDTH;
    int    TEXT_HEIGHT;
    int    IMAGE_WIDTH;
    int    IMAGE_HEIGHT;
    int    CHANNEL_COUNT;
    int    DATA_SIZE;
    //GLenum PIXEL_FORMAT    = GL_BGRA;

    // global variables
    void *font;
    GLuint pboIds[2];                   // IDs of PBO
    GLuint textureId;                   // ID of texture
    GLubyte* imageData;             // pointer to texture buffer
    int screenWidth;
    int screenHeight;
    bool mouseLeftDown;
    bool mouseRightDown;
    float mouseX, mouseY;
    float cameraAngleX;
    float cameraAngleY;
    float cameraDistance;
    bool pboSupported;
    int pboMode;
    int drawMode;
    glInfo glInfo;
    //Timer timer, t1, t2;
    //float copyTime, updateTime;
};

#endif//_voxelquest__h_
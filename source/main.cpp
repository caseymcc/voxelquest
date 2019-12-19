#include "voxelquest/gamestate.h"
#include "voxelquest/renderer.h"
#include "voxelquest/glmhelpers.h"

#include "voxelquest/initGlew.h"
#include "voxelquest/debugScreen.h"

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <iostream>

using namespace gl;

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseMovementWithButton(int x, int y);
void mouseMovementWithoutButton(int x, int y);
void mouseMove(GLFWwindow* window, double xpos, double ypos);
void mouseClick(GLFWwindow *window, int button, int action, int modifiers);
void reshape(GLFWwindow* window, int w, int h);

void setLookAt();
void updatePosition();

bool key_w=false;
bool key_a=false;
bool key_d=false;
bool key_s=false;

float lastFrame;
float currentFrame;
float deltaTime;

DebugScreen *debugScreen=nullptr;

float yaw=180.0f;
float pitch=0.0f;

int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));
    int winW=1920;
    int winH=1080;

    int argCount=0;
    char** argStrs=NULL;

    std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

//    if(_helpRequested)
//    {
//        displayHelp();
//    }
//    else
    {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, (int)GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow *window=glfwCreateWindow(winW, winH, "Voxel Quest", NULL, NULL);

        glfwMakeContextCurrent(window);
        glbinding::initialize(glfwGetProcAddress);
        initGlew();//hack for imgui

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetFramebufferSizeCallback(window, reshape);
        glfwSetKeyCallback(window, keyboard);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, mouseMove);
        glfwSetMouseButtonCallback(window, mouseClick);

        GameState::init(winW, winH);
        
        setLookAt();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io=ImGui::GetIO();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __APPLE__    
        ImGui_ImplOpenGL3_Init("#version 410");
#else
        ImGui_ImplOpenGL3_Init();
#endif
        ImGui::StyleColorsDark();

        debugScreen=new DebugScreen();

        debugScreen->initialize();
        debugScreen->setSize(winW, winH);

        debugScreen->startBuild();
        debugScreen->update();
        debugScreen->build();
        while(!glfwWindowShouldClose(window))
        {
            updatePosition();
            GameState::display();

            debugScreen->draw();

            //get everything started
            glFlush();

            //handle background updates while gpu rendering
            debugScreen->startBuild();
            debugScreen->update();
            debugScreen->build();
            
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key==GLFW_KEY_ESCAPE&&action==GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else if((key==GLFW_KEY_GRAVE_ACCENT)&&(action==GLFW_RELEASE))
    {
        if(GameState::cursorCapture)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            GameState::cursorCapture=false;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            GameState::cursorCapture=true;
        }
    }

    if(mods &  GLFW_MOD_CONTROL)
    {
        if(key==GLFW_KEY_D)
        {
            if(action==GLFW_RELEASE)
                g_settings.toggleSetting(E_BS_DEBUG_VIEW);
        }
        else if(key==GLFW_KEY_E)
        {
            if(action==GLFW_RELEASE)
                Renderer::setCameraToElevation();
        }
        else if(key==GLFW_KEY_F)
        {
            if(action==GLFW_RELEASE)
                g_settings.toggleSetting(E_BS_FOG);
        }
        else if(key==GLFW_KEY_H)
        {
            if(action==GLFW_RELEASE)
                GameState::helpCommands=!GameState::helpCommands;
        }
        else if(key==GLFW_KEY_S)
        {
            if(action==GLFW_RELEASE)
                GameState::showSettings=!GameState::showSettings;
        }
    }

    if(!GameState::cursorCapture)
        return;

    if(!(mods &  GLFW_MOD_CONTROL))
    {
        if(key==GLFW_KEY_W)
        {
            if(action==GLFW_PRESS)
                key_w=true;
            else if(action==GLFW_RELEASE)
                key_w=false;
        }
        else if(key==GLFW_KEY_S)
        {
            if(action==GLFW_PRESS)
                key_s=true;
            else if(action==GLFW_RELEASE)
                key_s=false;
        }
        else if(key==GLFW_KEY_A)
        {
            if(action==GLFW_PRESS)
                key_a=true;
            else if(action==GLFW_RELEASE)
                key_a=false;
        }
        else if(key==GLFW_KEY_D)
        {
            if(action==GLFW_PRESS)
                key_d=true;
            else if(action==GLFW_RELEASE)
                key_d=false;
        }
        else if(key==GLFW_KEY_T)
        {
            if(action==GLFW_RELEASE)
                g_settings.toggleSetting(E_BS_RENDER_VOXELS);
        }
        else if(key==GLFW_KEY_M)
        {
            if(action==GLFW_RELEASE)
                GameState::showMap=!GameState::showMap;
        }
    }

    

 //   if(action==GLFW_RELEASE)
 //   {
//        if(key==GLFW_KEY_LEFT_SHIFT)
//            shiftPressed=false;
//        else if(key==GLFW_KEY_LEFT_CONTROL)
//            ctrlPressed=false;
//        else if(key==GLFW_KEY_LEFT_ALT)
//            altPressed=false;
        //            else
        //                doAction(progActionsDown[((int)programState)*256+key]);
//    }
//    else
//    {
//        if(key==GLFW_KEY_LEFT_SHIFT)
//            shiftPressed=true;
//        else if(key==GLFW_KEY_LEFT_CONTROL)
//            ctrlPressed=true;
//        else if(key==GLFW_KEY_LEFT_ALT)
//            altPressed=true;
//        //            else
//        //                doAction(progActionsU[((int)programState)*256+key]);
//    }
}

void mouseMovementWithButton(int x, int y)
{

}
void mouseMovementWithoutButton(int x, int y)
{

}

glm::vec3 calcDirection(float yaw, float pitch)
{
    glm::vec3 direction;
    float r=cos(pitch);

    direction.x=r*cos(yaw);
    direction.y=r*sin(yaw);
    direction.z=sin(pitch);
    direction=glm::normalize(direction);

    return direction;
}

void mouseMove(GLFWwindow* window, double xpos, double ypos)
{
    if(!GameState::cursorCapture)
        return;

    static bool firstMouse=true;
    static float lastX;
    static float lastY;
    if(firstMouse)
    {
        lastX=(float)xpos;
        lastY=(float)ypos;
        firstMouse=false;
    }

    float xoffset=lastX-(float)xpos;
    float yoffset=lastY-(float)ypos;

    lastX=(float)xpos;
    lastY=(float)ypos;

    float sensitivity=(360.f)/Renderer::baseW;

    xoffset*=sensitivity;
    yoffset*=sensitivity;

    yaw+=xoffset;
    pitch+=yoffset;

    if(yaw>360.0f)
        yaw-=360.0f;
    if(yaw<0)
        yaw+=360.0f;

    if(pitch>89.0f)
        pitch=89.0f;
    if(pitch<-89.0f)
        pitch=-89.0f;

    setLookAt();
//    Renderer::lookAtVec=toFIVector4(direction);
}

void setLookAt()
{
    glm::vec3 direction=calcDirection(glm::radians(yaw), glm::radians(pitch));

    Renderer::setLookAt(toFIVector4(direction));
}

void updatePosition()
{
    float movementSpeed=100;

    currentFrame=(float)glfwGetTime();
    deltaTime=currentFrame-lastFrame;
    lastFrame=currentFrame;

    glm::vec3 direction(0.0f, 0.0f, 0.0f);
    bool move=false;

    if(key_w)
    {
        direction+=glm::vec3(1.0f, 0.0f, 0.0f);
        move=true;
    }
    if(key_s)
    {
        direction+=glm::vec3(-1.0f, 0.0f, 0.0f);
        move=true;
    }
    if(key_a)
    {
        direction+=glm::vec3(0.0f, -1.0f, 0.0f);
        move=true;
    }
    if(key_d)
    {
        direction+=glm::vec3(0.0f, 1.0f, 0.0f);
        move=true;
    }
//    if(key_space)
//    {
//        direction+=glm::vec3(0.0f, 0.0f, 1.0f);
//        move=true;
//    }
//    if(key_left_shift)
//    {
//        direction+=glm::vec3(0.0f, 0.0f, -1.0f);
//        move=true;
//    }

    if(move)
    {
        glm::vec3 worldUp(0.0f, 0.0f, 1.0f);
        glm::vec3 velocity=direction*movementSpeed*deltaTime;
        glm::vec3 lookAt=toVec3(Renderer::lookAtVec);
        glm::vec3 right=glm::normalize(glm::cross(lookAt, worldUp));

        glm::vec3 delta=(lookAt*velocity.x)+(right*velocity.y)+(worldUp*velocity.z);

//        Renderer::camLerpPos->copyFrom(&Renderer::cameraPos);
//        Renderer::cameraPos->addXYZ(delta.x, delta.y, delta.z);

        FIVector4 position;

//        position.copyFrom(Renderer::cameraPos);
        position.setFXYZ(delta.x, delta.y, delta.z);

        Renderer::moveCamera(&position);

        Renderer::updateView();
    }
}

void mouseClick(GLFWwindow *window, int button, int action, int modifiers)
{
}

void reshape(GLFWwindow* window, int w, int h)
{
    Renderer::setWH(w, h);
    Renderer::setMatrices(w, h);
}
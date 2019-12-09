#include "voxelquest/gamestate.h"
#include "voxelquest/renderer.h"

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

using namespace gl;

void display(bool doFrameRender=false);
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseMovementWithButton(int x, int y);
void mouseMovementWithoutButton(int x, int y);
void mouseClick(GLFWwindow *window, int button, int action, int modifiers);
void reshape(GLFWwindow* window, int w, int h);

int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));
    int winW=1024;
    int winH=1024;

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

        glfwSetFramebufferSizeCallback(window, reshape);
        glfwSetKeyCallback(window, keyboard);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        //        glfwSetCursorPosCallback(window, mouseMovementWithoutButton);
        glfwSetMouseButtonCallback(window, mouseClick);

        GameState::init(winW, winH);

        while(!glfwWindowShouldClose(window))
        {
            if(GameState::display())
                glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action==GLFW_RELEASE)
    {
//        if(key==GLFW_KEY_LEFT_SHIFT)
//            shiftPressed=false;
//        else if(key==GLFW_KEY_LEFT_CONTROL)
//            ctrlPressed=false;
//        else if(key==GLFW_KEY_LEFT_ALT)
//            altPressed=false;
        //            else
        //                doAction(progActionsDown[((int)programState)*256+key]);
    }
    else
    {
//        if(key==GLFW_KEY_LEFT_SHIFT)
//            shiftPressed=true;
//        else if(key==GLFW_KEY_LEFT_CONTROL)
//            ctrlPressed=true;
//        else if(key==GLFW_KEY_LEFT_ALT)
//            altPressed=true;
        //            else
        //                doAction(progActionsU[((int)programState)*256+key]);
    }
}

void mouseMovementWithButton(int x, int y)
{

}
void mouseMovementWithoutButton(int x, int y)
{

}
void mouseClick(GLFWwindow *window, int button, int action, int modifiers)
{
}

void reshape(GLFWwindow* window, int w, int h)
{
    Renderer::setWH(w, h);
    Renderer::setMatrices(w, h);
}
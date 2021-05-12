#define GLEW_STATIC
#include "windows.h"
#include "C:\Library\glew-2.1.0\src\glew.c"
#include <glfw3.h>
#include "C:\Library\free_type\include\ft2build.h"
#include FT_FREETYPE_H
#include <stdio.h>
#include "practice.h"
#include "time.h"

#include "glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "utility.cpp"
#include "fonts.cpp"
#include "opengl_utility.cpp"
#include "opengl_code.cpp"
#include "collision.cpp"


static bool globalRunning;
static window_dimensions windowDimensions;
static input_state input;


static void
glfw_ProcessKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // NOTE: Buffering an input to keep track of two key pressed simultaneously
    // seems to smooth out the input to the point that I'm happy with it. 
    // Probably worth exploring actual input handling techniques...
    
    // NOTE: If you hold down more than two keys at once, input doesn't really work.
    // I will probably process the 'shoot' button seperately
    
    // TODO: track projectile shoot input
    
    switch(key)
    {
        case GLFW_KEY_W:
        case GLFW_KEY_A:
        case GLFW_KEY_S:
        case GLFW_KEY_D:
        case GLFW_KEY_F:
        case GLFW_KEY_SPACE:
        {
            if(action == GLFW_RELEASE)
            {
                if(input.keyBuffer)
                {
                    // If we released the buffered key
                    if(key == input.keyBuffer)
                    {
                        input.keyBuffer = 0;
                    }
                    // or if we released the current key, and should replace it with 
                    // the currently buffered key
                    else
                    {
                        input.key = input.keyBuffer;
                        input.keyBuffer = 0;
                        input.isDown = 1;
                    }
                }
                else
                {
                    input.key = 0;
                    input.isDown = 0;
                }
            }
            else if(action == GLFW_PRESS)
            {
                // If two keys are being pressed ...
                if(key != input.key &&
                   input.isDown &&
                   !input.keyBuffer)
                {
                    input.keyBuffer = input.key;
                    input.key = key;
                    input.isDown = 1;
                }
                else
                {
                    input.key = key;
                    input.isDown = 1;
                }
            }
        } break;
    }
}

static void
glfw_ResizeFrameBuffer(GLFWwindow* window, int width, int height)
{
    windowDimensions.width = width;
    windowDimensions.height = height;
}

int 
main(void)
{
    if(glfwInit())
    {
        GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, 
                                              WINDOW_HEIGHT,
                                              "Asteroids",
                                              NULL, NULL);
        if(window)
        {
            windowDimensions.width = WINDOW_WIDTH;
            windowDimensions.height = WINDOW_HEIGHT;
            
            glfwMakeContextCurrent(window);
            GLenum glewStatus = glewInit();
            if(glewStatus == GLEW_OK)
            {
                glfwSetKeyCallback(window, glfw_ProcessKeyboardInput);
                glfwSetFramebufferSizeCallback(window, glfw_ResizeFrameBuffer);
                
                // TODO: allocate memory for the open_gl_state and let the game code
                // layer set up the memory instead of the platform layer
                open_gl_state openGLState = {};
                ship_data shipData = {};
                projectile_data projectileData = {};
                asteroid_data asteroidData = {};
                game_state gameState = {};
                font_buffer fontBuffer = {};
                
                globalRunning = true;
                while(globalRunning)
                {
                    if(glfwWindowShouldClose(window))
                    {
                        globalRunning = false;
                    }
                    
                    openGLState.secondsElapsed = glfwGetTime();
                    glfwPollEvents();
                    UpdateGameAndRender(&openGLState, &gameState,
                                        windowDimensions, input, &shipData,
                                        &projectileData, &asteroidData, &fontBuffer);
                    glfwSwapBuffers(window);
                    openGLState.frameTime = glfwGetTime() - openGLState.secondsElapsed;
                }
            }
            else
            {
                OutputDebugStringA((LPCSTR)glewGetErrorString(glewStatus));
                OutputDebugStringA("\n");
                //TODO: Logging
            }
        }
        else
        {
            OutputDebugStringA("Error during GLFW window creation");
            //TODO: Logging
        }
    }
    else
    {
        OutputDebugStringA("An error occured during GLFW initialization.");
        //TODO: Logging
    }
    return 0;
}
//
//  main.cpp
//  SolarSystemObservatory
//
//  Created by Kevin Cox on 11/19/11.
//

#ifndef DEBUG   // For debug output, by default DEBUG is already enabled in XCode
#define DEBUG
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "EOGL.h"

// C++ Std Includes
#include <iostream>
#include <cmath>
#include <cctype>
#include <cstdlib>
#include <algorithm>

// Classes
#include "EOScene.h"

// Custom Includes
#include "astromathlib.h"
#include "EOGlobals.h"
#include "EOGlobalEnums.h"

// C++ Namespace
using namespace std;

// Custom Astronomical Math Library Namespace
using namespace ASTRO_MATH_LIB;

namespace
{
    constexpr double kAnimationIntervalSeconds = 1.0 / 30.0; // ~33ms
    GLFWwindow* g_window = nullptr;
    EOScene* scene = nullptr;
}

/*
--------------------------------------------------------------------------------
 Function Prototypes
------------------------------------------------------------------------------*/
// Control the Screen Window Size
void SetScreenWindowSize(void);

// Callback for Screen Window Resize Events
void HandleResize(GLFWwindow* window, int width, int height);

// Callback for Keyboard Events
void HandleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

// Callback for OpenGL Display Events
void HandleDisplay(void);

// Initialize - This is the main initialization function
void Initialize(void);

void ErrorCallback(int error, const char* description);

/*
--------------------------------------------------------------------------------
 Functions
------------------------------------------------------------------------------*/

void ErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << (description ? description : "<no description>") << std::endl;
}

void SetScreenWindowSize(void)
{
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (mode != nullptr)
    {
        screenHeight = mode->height;
        screenWidth = mode->width;

        if (screenWindowWidth < screenWidth)
        {
            screenWindowWidth = screenWidth;
        }

        if (screenWindowHeight < screenHeight)
        {
            screenWindowHeight = std::max(1, screenHeight - 150);
        }
    }
    else
    {
        screenHeight = screenWindowHeight;
        screenWidth = screenWindowWidth;
    }

#ifdef DEBUG
    cout << "Screen Width, Height: " << screenWidth << ", " << screenHeight << endl;
#endif
}

/*
 ** Function HandleResize()
 **
 ** Purpose:  Resize Callback to handle Window Resizes
 */
void HandleResize(GLFWwindow* /*window*/, int width, int height)
{
    screenWindowWidth = width;
    screenWindowHeight = std::max(height, 1);

    aspectRatio = static_cast<GLfloat>(screenWindowWidth) /
                  static_cast<GLfloat>(screenWindowHeight);

    glViewport(0, 0, screenWindowWidth, screenWindowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspectRatio, 0.01, 50.0);
    glMatrixMode(GL_MODELVIEW);

#ifdef DEBUG
    cout << "HandleResize called with " << width << ", " << height << "." << endl;
#endif
}

void HandleDisplay(void)
{
    if (scene == nullptr)
    {
        return;
    }

    scene->RenderScene();
    glfwSwapBuffers(g_window);
}

void HandleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (scene == nullptr)
    {
        return;
    }

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_F1:
                scene->HandleKeyboardInput('1');
                return;
            case GLFW_KEY_F2:
                scene->HandleKeyboardInput('2');
                return;
            case GLFW_KEY_F3:
                scene->HandleKeyboardInput('3');
                return;
            case GLFW_KEY_F4:
                scene->HandleKeyboardInput('4');
                return;
            case GLFW_KEY_F5:
                scene->HandleKeyboardInput('5');
                return;
            case GLFW_KEY_F6:
                scene->HandleKeyboardInput('6');
                return;
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                return;

            case GLFW_KEY_A:
                scene->SetCurrentlyAnimating(!scene->GetCurrentlyAnimating());
                scene->HandleKeyboardInput('a');
                return;

            default:
                break;
        }
    }

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_EQUAL || key == GLFW_KEY_KP_ADD)
        {
            scene->HandleKeyboardInput('+');
            return;
        }

        if (key == GLFW_KEY_MINUS || key == GLFW_KEY_KP_SUBTRACT)
        {
            scene->HandleKeyboardInput('-');
            return;
        }

        const char* keyName = glfwGetKeyName(key, scancode);
        if (keyName != nullptr && keyName[0] != '\0')
        {
            const char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(keyName[0])));
            scene->HandleKeyboardInput(lower);
        }
    }
}

// Create a new scene object and initialize.
void Initialize(void)
{
    scene = new EOScene();
    scene->SetRenderMode(RENDER_MODE_NORMAL);
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    glfwSetErrorCallback(ErrorCallback);

    if (glfwInit() != GLFW_TRUE)
    {
        cerr << "Failed to initialize GLFW" << endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    SetScreenWindowSize();

    g_window = glfwCreateWindow(screenWindowWidth, screenWindowHeight, primaryScreenWindowTitle, nullptr, nullptr);
    if (g_window == nullptr)
    {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(g_window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(g_window, HandleResize);
    glfwSetKeyCallback(g_window, HandleKeyboard);

    Initialize();

    cout << "The radius of the Earth is 6371.0KM or " << KMtoGU(6371.0) << endl;
    cout << "The radius of the Moon is 1737.0KM or " << KMtoGU(1737.1) << endl;
    cout << "The distance of the Moon from the Earth is 384,000KM or " << KMtoGU(384000.0) << endl;
    cout << "GL Light Enums" << GL_LIGHT0 << ", " << GL_LIGHT1 << endl;

    HandleResize(g_window, screenWindowWidth, screenWindowHeight);

    double lastTime = glfwGetTime();
    double accumulator = 0.0;

    while (!glfwWindowShouldClose(g_window))
    {
        const double currentTime = glfwGetTime();
        const double delta = currentTime - lastTime;
        lastTime = currentTime;
        accumulator += delta;

        while (accumulator >= kAnimationIntervalSeconds)
        {
            if (scene->GetCurrentlyAnimating())
            {
                scene->UpdateScene();
            }
            accumulator -= kAnimationIntervalSeconds;
        }

        HandleDisplay();
        glfwPollEvents();
    }

    delete scene;
    scene = nullptr;

    glfwDestroyWindow(g_window);
    g_window = nullptr;

    glfwTerminate();
    return EXIT_SUCCESS;
}

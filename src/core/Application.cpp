#include "core/Application.h"

#include "core/Layer.h"
#include "common/EOGlobals.h"
#include "utils/Log.h"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <utility>
#include <string>
#include <sstream>

namespace
{
void errorCallback(int error, const char* description)
{
    Log::error(std::string("GLFW error ") + std::to_string(error) + ": " +
               (description != nullptr ? description : "<no description>"));
}

} // namespace

Application::Application() : Application(ApplicationSpecification{}) {}

Application::Application(ApplicationSpecification specification)
    : m_specification(std::move(specification))
{
    initialize();
}

Application::~Application()
{
    shutdown();
}

void Application::initialize()
{
    glfwSetErrorCallback(errorCallback);

    if (glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    m_glfwInitialized = true;

    screenWindowWidth = m_specification.width;
    screenWindowHeight = m_specification.height;

    updateMonitorDimensions();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    m_window = glfwCreateWindow(screenWindowWidth,
                                screenWindowHeight,
                                m_specification.name.c_str(),
                                nullptr,
                                nullptr);

    if (m_window == nullptr)
    {
        glfwTerminate();
        m_glfwInitialized = false;
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(m_specification.enableVsync ? 1 : 0);

    m_windowTitleBase = m_specification.name;

    setupCallbacks();
}

void Application::shutdown()
{
    for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
    {
        if (*it != nullptr)
        {
            (*it)->onDetach();
        }
    }

    m_layers.clear();

    if (m_window != nullptr)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    if (m_glfwInitialized)
    {
        glfwTerminate();
        m_glfwInitialized = false;
    }
}

void Application::setupCallbacks()
{
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    glfwSetKeyCallback(m_window, keyCallback);
}

void Application::pushLayer(std::unique_ptr<Layer> layer)
{
    if (layer == nullptr)
    {
        return;
    }

    layer->onAttach(*this);
    m_layers.emplace_back(std::move(layer));
}

int Application::run()
{
    if (m_window == nullptr)
    {
        return EXIT_FAILURE;
    }

    m_running = true;

    double lastTime = glfwGetTime();

    while (m_running && glfwWindowShouldClose(m_window) == GLFW_FALSE)
    {
        const double currentTime = glfwGetTime();
        const double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        for (const auto& layer : m_layers)
        {
            if (layer != nullptr)
            {
                layer->onUpdate(deltaTime);
            }
        }

        for (const auto& layer : m_layers)
        {
            if (layer != nullptr)
            {
                layer->onRender();
            }
        }

        updateFps(deltaTime);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    shutdown();
    return EXIT_SUCCESS;
}

void Application::updateFps(double deltaTime)
{
    if (!m_displayFps || m_window == nullptr)
    {
        return;
    }

    m_fpsAccumulator += deltaTime;
    ++m_fpsFrameCount;

    if (m_fpsAccumulator >= 1.0)
    {
        const double fps = static_cast<double>(m_fpsFrameCount) / m_fpsAccumulator;
        m_lastFps = fps;
        m_fpsAccumulator = 0.0;
        m_fpsFrameCount = 0;

        std::ostringstream title;
        title.setf(std::ios::fixed);
        title.precision(1);
        title << m_windowTitleBase << " | FPS: " << fps;
        glfwSetWindowTitle(m_window, title.str().c_str());
    }
}

void Application::toggleFpsDisplay()
{
    m_displayFps = !m_displayFps;

    if (!m_displayFps)
    {
        m_fpsAccumulator = 0.0;
        m_fpsFrameCount = 0;
        m_lastFps = 0.0;
        if (m_window != nullptr)
        {
            glfwSetWindowTitle(m_window, m_windowTitleBase.c_str());
        }
    }
    else if (m_window != nullptr)
    {
        glfwSetWindowTitle(m_window, (m_windowTitleBase + " | FPS: --").c_str());
    }
}

void Application::close()
{
    m_running = false;
    if (m_window != nullptr)
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
}

void Application::dispatchResize(int width, int height)
{
    for (const auto& layer : m_layers)
    {
        if (layer != nullptr)
        {
            layer->onResize(width, height);
        }
    }
}

void Application::dispatchKey(int key, int scancode, int action, int mods)
{
    for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
    {
        if (*it != nullptr)
        {
            (*it)->onKey(key, scancode, action, mods);
        }
    }
}

void Application::updateMonitorDimensions()
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
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    auto* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (application != nullptr)
    {
        application->dispatchResize(width, height);
    }
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (application != nullptr)
    {
        application->dispatchKey(key, scancode, action, mods);
    }
}

#ifndef PLANETARY_OBSERVATORY_CORE_APPLICATION_H
#define PLANETARY_OBSERVATORY_CORE_APPLICATION_H

#include <memory>
#include <string>
#include <vector>

struct GLFWwindow;

struct ApplicationSpecification
{
    std::string name = "Planetary Observatory";
    int width = 1280;
    int height = 720;
    bool enableVsync = true;
};

class Layer;

class Application
{
public:
    Application();
    explicit Application(ApplicationSpecification specification);
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void pushLayer(std::unique_ptr<Layer> layer);

    int run();
    void close();

    GLFWwindow* window() const { return m_window; }
    const ApplicationSpecification& specification() const { return m_specification; }

private:
    void initialize();
    void shutdown();
    void setupCallbacks();
    void dispatchResize(int width, int height);
    void dispatchKey(int key, int scancode, int action, int mods);
    void updateMonitorDimensions();
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    ApplicationSpecification m_specification;
    GLFWwindow* m_window = nullptr;
    bool m_running = false;
    bool m_glfwInitialized = false;
    std::vector<std::unique_ptr<Layer>> m_layers;
};

#endif

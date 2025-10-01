#include "core/Application.h"

#include "common/EOGlobals.h"
#include "core/Layer.h"
#include "utils/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl2.h>
#include <imgui.h>

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <cstdint>

namespace {
void errorCallback(int error, const char *description) {
  Log::error(std::string("GLFW error ") + std::to_string(error) + ": " +
             (description != nullptr ? description : "<no description>"));
}

} // namespace

Application::Application() : Application(ApplicationSpecification{}) {}

Application::Application(ApplicationSpecification specification)
    : m_specification(std::move(specification)) {
  initialize();
}

Application::~Application() { shutdown(); }

void Application::initialize() {
  glfwSetErrorCallback(errorCallback);

  if (glfwInit() != GLFW_TRUE) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  m_glfwInitialized = true;

  screenWindowWidth = m_specification.width;
  screenWindowHeight = m_specification.height;

  updateMonitorDimensions();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

  m_window = glfwCreateWindow(screenWindowWidth, screenWindowHeight,
                              m_specification.name.c_str(), nullptr, nullptr);

  if (m_window == nullptr) {
    glfwTerminate();
    m_glfwInitialized = false;
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(m_specification.enableVsync ? 1 : 0);

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    glfwDestroyWindow(m_window);
    m_window = nullptr;
    glfwTerminate();
    m_glfwInitialized = false;
    throw std::runtime_error("Failed to initialize GLAD");
  }

  Log::info(std::string("GLAD initialised: glTexImage2D loaded = ") +
            (glad_glTexImage2D != nullptr ? "true" : "false"));

  m_windowTitleBase = m_specification.name;

  setupCallbacks();

  initializeImGui();
}

void Application::shutdown() {
  shutdownImGui();

  for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it) {
    if (*it != nullptr) {
      (*it)->onDetach();
    }
  }

  m_layers.clear();

  if (m_window != nullptr) {
    glfwDestroyWindow(m_window);
    m_window = nullptr;
  }

  if (m_glfwInitialized) {
    glfwTerminate();
    m_glfwInitialized = false;
  }
}

void Application::setupCallbacks() {
  glfwSetWindowUserPointer(m_window, this);
  glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
  glfwSetKeyCallback(m_window, keyCallback);
  glfwSetCharCallback(m_window, charCallback);
}

void Application::initializeImGui() {
  if (!m_imguiEnabled || m_window == nullptr) {
    return;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(m_window, false);
  ImGui_ImplOpenGL2_Init();
}

void Application::shutdownImGui() {
  if (!m_imguiEnabled || ImGui::GetCurrentContext() == nullptr) {
    return;
  }

  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Application::pushLayer(std::unique_ptr<Layer> layer) {
  if (layer == nullptr) {
    return;
  }

  layer->onAttach(*this);
  m_layers.emplace_back(std::move(layer));
}

int Application::run() {
  if (m_window == nullptr) {
    return EXIT_FAILURE;
  }

  m_running = true;

  double lastTime = glfwGetTime();

  while (m_running && glfwWindowShouldClose(m_window) == GLFW_FALSE) {
    const double currentTime = glfwGetTime();
    const double deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    if (m_imguiEnabled && m_mode == ApplicationMode::Edit) {
      ImGui_ImplOpenGL2_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
    }

    for (const auto &layer : m_layers) {
      if (layer != nullptr) {
        layer->onUpdate(deltaTime);
      }
    }

    for (const auto &layer : m_layers) {
      if (layer != nullptr) {
        layer->onRender();
      }
    }

    if (m_imguiEnabled && m_mode == ApplicationMode::Edit) {
      if (m_showImGuiDemo) {
        ImGui::ShowDemoWindow(&m_showImGuiDemo);
      }

      ImGui::Render();
      ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }

    updateFps(deltaTime);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
  }

  shutdown();
  return EXIT_SUCCESS;
}

void Application::updateFps(double deltaTime) {
  if (!m_displayFps || m_window == nullptr) {
    return;
  }

  m_fpsAccumulator += deltaTime;
  ++m_fpsFrameCount;

  if (m_fpsAccumulator >= 1.0) {
    const double fps = static_cast<double>(m_fpsFrameCount) / m_fpsAccumulator;
    m_lastFps = fps;
    m_fpsAccumulator = 0.0;
    m_fpsFrameCount = 0;

    m_lastFps = fps;
  }
}

void Application::toggleFpsDisplay() {
  m_displayFps = !m_displayFps;

  if (!m_displayFps) {
    m_fpsAccumulator = 0.0;
    m_fpsFrameCount = 0;
    m_lastFps = 0.0;
  }
}

void Application::close() {
  m_running = false;
  if (m_window != nullptr) {
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
  }
}

void Application::dispatchResize(int width, int height) {
  for (const auto &layer : m_layers) {
    if (layer != nullptr) {
      layer->onResize(width, height);
    }
  }
}

void Application::toggleEditMode() {
  const bool wasEdit = (m_mode == ApplicationMode::Edit);
  m_mode = wasEdit ? ApplicationMode::Play : ApplicationMode::Edit;

  if (wasEdit && m_imguiEnabled && ImGui::GetCurrentContext() != nullptr) {
    ImGuiIO &io = ImGui::GetIO();
    io.ClearInputCharacters();
    io.ClearInputKeys();
  }
}

void Application::dispatchKey(int key, int scancode, int action, int mods) {
  if (m_mode == ApplicationMode::Edit && m_imguiEnabled &&
      ImGui::GetCurrentContext() != nullptr) {
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureKeyboard) {
      return;
    }
  }

  for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it) {
    if (*it != nullptr) {
      (*it)->onKey(key, scancode, action, mods);
    }
  }
}

void Application::updateMonitorDimensions() {
  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  if (mode != nullptr) {
    screenHeight = mode->height;
    screenWidth = mode->width;

    if (screenWindowWidth < screenWidth) {
      screenWindowWidth = screenWidth;
    }

    if (screenWindowHeight < screenHeight) {
      screenWindowHeight = std::max(1, screenHeight - 150);
    }
  } else {
    screenHeight = screenWindowHeight;
    screenWidth = screenWindowWidth;
  }
}

void Application::framebufferSizeCallback(GLFWwindow *window, int width,
                                          int height) {
  auto *application =
      static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (application != nullptr) {
    application->dispatchResize(width, height);
  }
}

void Application::keyCallback(GLFWwindow *window, int key, int scancode,
                              int action, int mods) {
  ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
  auto *application =
      static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (application != nullptr) {
    application->dispatchKey(key, scancode, action, mods);
  }
}

void Application::charCallback(GLFWwindow *window, unsigned int codepoint) {
  ImGui_ImplGlfw_CharCallback(window, codepoint);
}

#include "layers/SceneLayer.h"

#include "common/EOGL.h"
#include "common/EOGlobals.h"
#include "core/Application.h"
#include "math/astromathlib.h"
#include "scene/Scene.h"
#include "scenegraph/SceneGraph.h"
#include "utils/Log.h"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <cctype>
#include <memory>
#include <string>

#include <imgui.h>

SceneLayer::SceneLayer() = default;

void SceneLayer::onAttach(Application &application) {
  m_application = &application;
  m_scene = std::make_unique<Scene>();
  m_scene->SetRenderMode(RENDER_MODE_NORMAL);
  m_sceneGraph = std::make_unique<SceneGraph>();
  m_sceneGraph->setRoot(std::make_unique<SceneNode>());
  m_sceneGraph->attach();

  int width = 0;
  int height = 0;
  if (application.window() != nullptr) {
    glfwGetFramebufferSize(application.window(), &width, &height);
  }

  if (width > 0 && height > 0) {
    onResize(width, height);
  }

  if (Log::kDebugLoggingEnabled) {
    Log::debug(std::string("Earth radius GU: ") +
               std::to_string(ASTRO_MATH_LIB::KMtoGU(6371.0)));
    Log::debug(std::string("Moon radius GU: ") +
               std::to_string(ASTRO_MATH_LIB::KMtoGU(1737.1)));
    Log::debug(std::string("Earth-Moon distance GU: ") +
               std::to_string(ASTRO_MATH_LIB::KMtoGU(384000.0)));
  }
}

void SceneLayer::onDetach() {
  if (m_sceneGraph) {
    m_sceneGraph->detach();
  }
  m_scene.reset();
  m_sceneGraph.reset();
  m_application = nullptr;
}

void SceneLayer::onUpdate(double deltaTime) {
  if (!m_scene) {
    return;
  }

  if (m_sceneGraph) {
    m_sceneGraph->update(deltaTime);
  }

  m_accumulator += deltaTime;

  while (m_accumulator >= m_animationIntervalSeconds) {
    if (m_scene->GetCurrentlyAnimating()) {
      m_scene->UpdateScene();
    }
    m_accumulator -= m_animationIntervalSeconds;
  }
}

void SceneLayer::onRender() {
  if (!m_scene) {
    return;
  }

  m_scene->RenderScene();

  if (m_sceneGraph) {
    m_sceneGraph->render();
  }

  if (m_application != nullptr &&
      m_application->mode() == ApplicationMode::Edit) {
    onImGuiRender();
  }
}

void SceneLayer::onResize(int width, int height) {
  screenWindowWidth = width;
  screenWindowHeight = std::max(height, 1);

  aspectRatio = static_cast<GLfloat>(screenWindowWidth) /
                static_cast<GLfloat>(screenWindowHeight);

  updateProjection(screenWindowWidth, screenWindowHeight);
}

void SceneLayer::onKey(int key, int scancode, int action, int mods) {
  (void)mods;

  if (!m_scene) {
    return;
  }

  if (action == GLFW_PRESS) {
    switch (key) {
    case GLFW_KEY_F:
      if (m_application != nullptr) {
        m_application->toggleFpsDisplay();
      }
      return;
    case GLFW_KEY_TAB:
      if (m_application != nullptr) {
        m_application->toggleEditMode();
      }
      return;
    case GLFW_KEY_F1:
      handleCharacterInput('1');
      return;
    case GLFW_KEY_F2:
      handleCharacterInput('2');
      return;
    case GLFW_KEY_F3:
      handleCharacterInput('3');
      return;
    case GLFW_KEY_F4:
      handleCharacterInput('4');
      return;
    case GLFW_KEY_F5:
      handleCharacterInput('5');
      return;
    case GLFW_KEY_F6:
      handleCharacterInput('6');
      return;
    case GLFW_KEY_Q:
    case GLFW_KEY_ESCAPE:
      if (m_application != nullptr) {
        m_application->close();
      }
      return;
    case GLFW_KEY_A:
      m_scene->SetCurrentlyAnimating(!m_scene->GetCurrentlyAnimating());
      handleCharacterInput('a');
      return;
    default:
      break;
    }
  }

  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    if (key == GLFW_KEY_EQUAL || key == GLFW_KEY_KP_ADD) {
      handleCharacterInput('+');
      return;
    }

    if (key == GLFW_KEY_MINUS || key == GLFW_KEY_KP_SUBTRACT) {
      handleCharacterInput('-');
      return;
    }

    const char *keyName = glfwGetKeyName(key, scancode);
    if (keyName != nullptr && keyName[0] != '\0') {
      const char lower = static_cast<char>(
          std::tolower(static_cast<unsigned char>(keyName[0])));
      handleCharacterInput(lower);
    }
  }
}

void SceneLayer::updateProjection(int width, int height) {
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50.0, aspectRatio, 0.01, 50.0);
  glMatrixMode(GL_MODELVIEW);
}

void SceneLayer::handleCharacterInput(char key) {
  if (m_scene) {
    m_scene->HandleKeyboardInput(static_cast<unsigned char>(key));
  }
}

void SceneLayer::onImGuiRender() {
  if (ImGui::Begin("Diagnostics", nullptr,
                   ImGuiWindowFlags_AlwaysAutoResize |
                       ImGuiWindowFlags_NoCollapse)) {
    const bool inEdit = m_application != nullptr &&
                        m_application->mode() == ApplicationMode::Edit;
    ImGui::Text("Mode: %s", inEdit ? "Edit" : "Play");

    if (m_scene) {
      ImGui::SeparatorText("Scene");
      ImGui::Text("Animating: %s",
                  m_scene->GetCurrentlyAnimating() ? "Yes" : "No");
      ImGui::Text("Axes: %s", m_scene->GetShowAxes() ? "On" : "Off");
      const auto &camera = m_scene->GetCamera();
      ImGui::Text("Camera radius: %.2f", camera ? camera->GetRadius() : 0.0f);
    }

    ImGui::SeparatorText("Controls");
    ImGui::Text("Tab: toggle edit mode");
    ImGui::Text("F: toggle FPS");
  }
  ImGui::End();
}

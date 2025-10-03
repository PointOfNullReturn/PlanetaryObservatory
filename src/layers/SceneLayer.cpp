#include "layers/SceneLayer.h"

#include "common/EOGL.h"
#include "common/EOGlobals.h"
#include "core/Application.h"
#include "math/astromathlib.h"
#include "render/SceneRenderer.h"
#include "scene/Scene.h"
#include "scenegraph/SceneGraph.h"
#include "scenegraph/components/CameraComponent.h"
#include "scenegraph/components/TransformComponent.h"
#include "utils/Log.h"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <cctype>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

SceneLayer::SceneLayer() = default;

void SceneLayer::onAttach(Application &application) {
  m_application = &application;
  m_sceneGraph = std::make_unique<SceneGraph>();
  m_sceneGraph->setRoot(std::make_unique<SceneNode>());
  m_scene = std::make_unique<Scene>(*m_sceneGraph);
  m_scene->SetRenderMode(RENDER_MODE_NORMAL);
  m_sceneGraph->attach();
  m_sceneRenderer = std::make_unique<SceneRenderer>();

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
  m_sceneRenderer.reset();
  m_sceneGraph.reset();
  m_application = nullptr;
}

void SceneLayer::onUpdate(double deltaTime) {
  if (!m_scene) {
    return;
  }

  m_scene->UpdateCinematic(deltaTime);

  if (m_sceneGraph) {
    m_sceneGraph->update(deltaTime);
  }

  m_lastDeltaTime = deltaTime;
  m_renderContext.deltaTimeSeconds = deltaTime;

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

  if (m_sceneGraph && m_sceneRenderer) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 viewMatrix(1.0f);
    m_renderContext.cameraPosition = glm::vec3(0.0f);
    if (auto camera = m_scene->GetCamera()) {
      viewMatrix = camera->viewMatrix();
      m_renderContext.cameraPosition = camera->position();
    }

    m_renderContext.viewMatrix = viewMatrix;
    m_renderContext.projectionMatrix = m_projectionMatrix;

    m_sceneRenderer->render(*m_sceneGraph, m_renderContext);
  }

  if (Log::kDebugLoggingEnabled) {
    Log::debug(std::string("Earth radius GU: ") +
               std::to_string(ASTRO_MATH_LIB::KMtoGU(6371.0)));
    Log::debug(std::string("Moon radius GU: ") +
               std::to_string(ASTRO_MATH_LIB::KMtoGU(1737.1)));
    Log::debug(std::string("Earth-Moon distance GU: ") +
               std::to_string(ASTRO_MATH_LIB::KMtoGU(384000.0)));
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

  m_projectionMatrix = glm::perspective(glm::radians(50.0f), aspectRatio, 0.01f, 50.0f);
}

void SceneLayer::handleCharacterInput(char key) {
  if (m_scene) {
    m_scene->HandleKeyboardInput(static_cast<unsigned char>(key));
  }
}

void SceneLayer::onImGuiRender() {
  if (!ImGui::Begin("Diagnostics", nullptr,
                    ImGuiWindowFlags_AlwaysAutoResize |
                        ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  const bool inEdit = m_application != nullptr &&
                      m_application->mode() == ApplicationMode::Edit;
  ImGui::Text("Mode: %s", inEdit ? "Edit" : "Play");

  if (m_scene) {
    ImGui::SeparatorText("Scene");
    ImGui::Text("Animating: %s",
                m_scene->GetCurrentlyAnimating() ? "Yes" : "No");
    ImGui::Text("Axes: %s", m_scene->GetShowAxes() ? "On" : "Off");
    const auto &camera = m_scene->GetCamera();
    ImGui::Text("Camera radius: %.2f",
                camera ? camera->radius() : 0.0f);
  }

  if (m_application != nullptr && m_application->isFpsDisplayed()) {
    ImGui::Text("FPS: %.1f", m_application->lastFps());
  }

  if (m_sceneGraph) {
    ImGui::SeparatorText("Hierarchy");

    std::function<void(SceneNode &)> drawNode;
    drawNode = [&](SceneNode &node) {
      const std::string &label =
          node.name().empty() ? "(unnamed)" : node.name();
      ImGuiTreeNodeFlags flags =
          ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
      if (m_selectedNode == &node) {
        flags |= ImGuiTreeNodeFlags_Selected;
      }

      const bool open =
          ImGui::TreeNodeEx(&node, flags, "%s", label.c_str());
      if (ImGui::IsItemClicked()) {
        m_selectedNode = &node;
      }
      if (open) {
        for (auto &child : node.children()) {
          if (child) {
            drawNode(*child);
          }
        }
        ImGui::TreePop();
      }
    };

    if (SceneNode *rootNode = m_sceneGraph->root()) {
      drawNode(*rootNode);
    }
  }

  if (m_selectedNode != nullptr) {
    ImGui::SeparatorText("Selected Node");
    static SceneNode *lastInspected = nullptr;
    static std::array<char, 128> nameBuffer{};
    if (m_selectedNode != lastInspected) {
      std::memset(nameBuffer.data(), 0, nameBuffer.size());
      if (!m_selectedNode->name().empty()) {
        std::strncpy(nameBuffer.data(), m_selectedNode->name().c_str(),
                     nameBuffer.size() - 1);
      }
      lastInspected = m_selectedNode;
    }

    if (ImGui::InputText("Name", nameBuffer.data(), nameBuffer.size())) {
      m_selectedNode->setName(nameBuffer.data());
    }

    if (auto* transform = m_selectedNode->getComponent<TransformComponent>()) {
        ImGui::DragFloat3("Position", &transform->position.x, 0.05f);
        ImGui::DragFloat3("Rotation", &transform->rotation.x, 0.5f);
        ImGui::DragFloat3("Scale", &transform->scale.x, 0.05f, 0.01f, 10.0f);
    }
  }

  ImGui::SeparatorText("Controls");
  ImGui::Text("Tab: toggle edit mode");
  ImGui::Text("F: toggle FPS");

  ImGui::End();
}

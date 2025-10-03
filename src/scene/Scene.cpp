#include "scene/Scene.h"

#include "utils/Log.h"
#include "render/TextureLoader.h"
#include "render/TextureCache.h"
#include "render/GlState.h"
#include "scenegraph/components/SphereMeshComponent.h"
#include "scenegraph/components/SkyboxComponent.h"
#include "scenegraph/components/DirectionalLightComponent.h"
#include "scenegraph/components/GlobalLightingComponent.h"
#include "scenegraph/components/TransformComponent.h"
#include "scenegraph/components/AxisComponent.h"
#include "scenegraph/components/MaterialComponent.h"
#include "math/astromathlib.h"
#include "common/EOPlanetaryConstants.h"

#include <memory>
#include <string>
#include <cmath>
#include <algorithm>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace {
float easeInOut(float t) {
  return t * t * (3.0f - 2.0f * t);
}

float deltaAngle(float fromDeg, float toDeg) {
  float diff = std::fmod(toDeg - fromDeg, 360.0f);
  if (diff > 180.0f) {
    diff -= 360.0f;
  } else if (diff < -180.0f) {
    diff += 360.0f;
  }
  return diff;
}

float lerpAngle(float fromDeg, float toDeg, float t) {
  return fromDeg + deltaAngle(fromDeg, toDeg) * t;
}
}

Scene::Scene(SceneGraph& sceneGraph) : m_sceneGraph(sceneGraph) {

  earthAnchor.focus.position = glm::vec3(0.0f);
  earthAnchor.focus.preferredRadius = 8.0f;
  earthAnchor.yawDegrees = 270.0f;
  earthAnchor.pitchDegrees = 0.0f;

  moonAnchor.focus.position = glm::vec3(0.0f);
  moonAnchor.focus.preferredRadius = 12.0f;
  moonAnchor.yawDegrees = 120.0f;
  moonAnchor.pitchDegrees = 5.0f;

  barycenterAnchor.focus.position = glm::vec3(0.0f);
  barycenterAnchor.focus.preferredRadius = 10.0f;
  barycenterAnchor.yawDegrees = 210.0f;
  barycenterAnchor.pitchDegrees = 15.0f;

  auto lightingNode = std::make_unique<SceneNode>();
  lightingNode->setName("Lighting");

  auto globalLighting = std::make_unique<GlobalLightingComponent>();
  auto &globalData = globalLighting->lighting();
  globalData.ambientColor = ambientLightColor;
  globalData.backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  lightingNode->addComponent(std::move(globalLighting));

  auto sunLight = std::make_unique<DirectionalLightComponent>();
  auto &sunData = sunLight->light();
  const glm::vec3 lightPosition = glm::vec3(light1Position);
  const float lightLength = glm::length(lightPosition);
  if (lightLength > 0.0f) {
    sunData.direction = -lightPosition / lightLength;
  }
  sunData.diffuseColor = specularLightColor;
  sunData.specularColor = specularLightColor;
  sunData.intensity = 0.75f;
  lightingNode->addComponent(std::move(sunLight));

  m_sceneGraph.root()->addChild(std::move(lightingNode));

  auto skyboxNode = std::make_unique<SceneNode>();
  skyboxNode->setName("Skybox");
  skyboxNode->addComponent(std::make_unique<SkyboxComponent>());
  m_sceneGraph.root()->addChild(std::move(skyboxNode));

  auto earthNode = std::make_unique<SceneNode>();
  earthNode->setName("Earth");
  auto earthMaterial = std::make_unique<MaterialComponent>();
  auto &earthMaterialData = earthMaterial->material();
  earthMaterialData.diffuseColor = glm::vec4(0.75f, 0.85f, 1.0f, 1.0f);
  earthMaterialData.specularStrength = 0.1f;
  earthMaterialData.shininess = 24.0f;
  earthMaterialData.ambientMix = 0.55f;
  earthMaterialData.exposure = 0.85f;
  earthMaterialData.gamma = 2.3f;
  earthNode->addComponent(std::move(earthMaterial));
  auto earthTextureLayers = std::make_unique<TextureLayerComponent>();
  earthTextureLayers->layers.push_back({GetTextureCache().getTexture2D("assets/textures/world.200407.3x5400x2700.png", true, false, true), TextureBlendMode::None, 1.0f});
  earthNode->addComponent(std::move(earthTextureLayers));
  auto earthSphere = std::make_unique<SphereMeshComponent>();
  earthSphere->radius = ASTRO_MATH_LIB::KMtoGU(EARTH_RADIUS_KM);
  earthNode->addComponent(std::move(earthSphere));
  this->earthNode = earthNode.get();
  m_sceneGraph.root()->addChild(std::move(earthNode));

  auto moonNode = std::make_unique<SceneNode>();
  moonNode->setName("Moon");
  auto moonMaterial = std::make_unique<MaterialComponent>();
  auto &moonMaterialData = moonMaterial->material();
  moonMaterialData.diffuseColor = glm::vec4(1.0f);
  moonMaterialData.specularStrength = 0.02f;
  moonMaterialData.shininess = 12.0f;
  moonMaterialData.ambientMix = 0.55f;
  moonMaterialData.exposure = 1.0f;
  moonMaterialData.gamma = 2.2f;
  moonNode->addComponent(std::move(moonMaterial));
  auto moonTextureLayers = std::make_unique<TextureLayerComponent>();
  moonTextureLayers->layers.push_back({GetTextureCache().getTexture2D("assets/textures/moon_sm.bmp", true, false), TextureBlendMode::None, 1.0f});
  moonNode->addComponent(std::move(moonTextureLayers));
  auto moonSphere = std::make_unique<SphereMeshComponent>();
  moonSphere->radius = 0.50;
  moonNode->addComponent(std::move(moonSphere));
  this->moonNode = moonNode.get();
  m_sceneGraph.root()->addChild(std::move(moonNode));

  auto axesNode = std::make_unique<SceneNode>();
  axesNode->setName("Axes");
  auto axisComponent = std::make_unique<AxisComponent>();
  axisComponent->length = 10.0;
  axisComponent->lineWidth = 2.0;
  axisComponent->enabled = false;
  axesNode->addComponent(std::move(axisComponent));
  this->axesNode = axesNode.get();
  m_sceneGraph.root()->addChild(std::move(axesNode));

  earthAnchor.focus = makeFocusForNode(this->earthNode, earthAnchor.focus.preferredRadius);
  moonAnchor.focus = makeFocusForNode(this->moonNode, moonAnchor.focus.preferredRadius);
  barycenterAnchor.focus = makeFocusForNode(nullptr, barycenterAnchor.focus.preferredRadius);

  m_cameraPresets = {
      {"Blue Marble Approach", earthAnchor, 6.0f, 2.0f, &this->earthNode},
      {"Lunar Flyby", CameraAnchor{makeFocusForNode(this->moonNode, 12.0f), 135.0f, 10.0f}, 7.0f, 2.0f, &this->moonNode},
      {"Earthrise", CameraAnchor{makeFocusForNode(this->moonNode, 15.0f), 240.0f, 12.0f}, 8.0f, 3.0f, &this->moonNode}}
  ;

  sceneCamera = std::make_shared<OrbitCamera>();
  applyCameraAnchor(earthAnchor, true);

  // TODO: Finish Lighting Object
  // ambientLight = std::make_unique<Light>("LIGHT0", GL_LIGHT0);
  InitializeScene();

  m_currentlyAnimating = false;
}

Scene::~Scene() {}

void Scene::InitializeScene(void) {
  // Initialize Background Color
  // Set Background Color to Black
  glstate::setClearColor({0.0f, 0.0f, 0.0f, 1.0f});

  // Core GL features configured via dedicated helpers.
  glstate::enableDepthTest(true);
  glstate::enableCullFace(true);

  // Lighting configuration is handled by GlobalLightingComponent and
  // DirectionalLightComponent instances on the scene graph.
}

// Accessor Methods
RenderModes Scene::GetRenderMode(void) { return m_renderMode; }

GLboolean Scene::GetShowAxes(void) { return showAxes; }

GLboolean Scene::GetCurrentlyAnimating(void) { return m_currentlyAnimating; }

// Mutator Methods
void Scene::SetRenderMode(RenderModes renderMode) {
  m_renderMode = renderMode;
  m_sceneGraph.traverse([renderMode](SceneNode& node) {
    if (auto* sphereMesh = node.getComponent<SphereMeshComponent>()) {
      sphereMesh->renderMode = renderMode;
    }
  });
}

void Scene::SetShowAxes(GLboolean show) {
  showAxes = show;
  if (axesNode) {
    if (auto* axisComponent = axesNode->getComponent<AxisComponent>()) {
      axisComponent->enabled = show;
    }
  }
}

void Scene::SetCurrentlyAnimating(GLboolean animating) {
  m_currentlyAnimating = animating;
}

// Other Methods
void Scene::UpdateScene(void) {
  // If currently animating, update all objects
  if (earthNode) {
    auto transform = earthNode->getComponent<TransformComponent>();
    transform->rotation.y += 0.05f;
  }
  if (moonNode) {
    auto transform = moonNode->getComponent<TransformComponent>();
    transform->rotation.y += 0.008f;
  }
}

void Scene::HandleKeyboardInput(unsigned char key) {
  switch (key) {
  case 'e':
    applyCameraAnchor(earthAnchor, false);
    break;

  case 'm':
    applyCameraAnchor(moonAnchor, false);
    break;

  case 'b':
    applyCameraAnchor(barycenterAnchor, false);
    break;


  // Scene Edit Helper Keys
  // Show Wireframe
  case 'w':
    if (GetRenderMode() == RENDER_MODE_NORMAL)
      SetRenderMode(RENDER_MODE_WIREFRAME);
    else
      SetRenderMode(RENDER_MODE_NORMAL);
    break;

  // Show Axes
  case 'x':
    if (GetShowAxes())
      SetShowAxes(false);
    else
      SetShowAxes(true);
    break;

  // Preset views along world axes
  case '1': // Preset 0
    playCameraPreset(0);
    break;

  case '2': // Preset 1
    playCameraPreset(1);
    break;

  case '3': // Preset 2
    playCameraPreset(2);
    break;

  case '4': // -Y
    applyCameraAnchor(moonAnchor, false);
    break;

  case '5': // +Z
    applyCameraAnchor(barycenterAnchor, false);
    break;

  case '6': // -Z
    applyCameraAnchor(earthAnchor, false);
    break;

  case 'p':
  case 'P':
    if (!m_cameraPresets.empty()) {
      std::size_t next = (m_activePreset.playing ? (m_activePreset.index + 1) : 0) % m_cameraPresets.size();
      playCameraPreset(next);
    }
    break;

  // Move Closer
  case '+':
    m_activePreset.playing = false;
    sceneCamera->zoom(-0.5f);
    break;

  // Move Back
  case '-':
    m_activePreset.playing = false;
    sceneCamera->zoom(0.5f);
    break;
  default:
    if (Log::kDebugLoggingEnabled) {
      Log::debug(std::string("Unhandled Keyboard Key: ") +
                 static_cast<char>(key));
    }
    break;
  }
}


OrbitCamera::Focus Scene::makeFocusForNode(const SceneNode *node, float radius) const {
  OrbitCamera::Focus focus{};
  if (node != nullptr) {
    focus.position = node->getTransform()[3];
  } else {
    focus.position = glm::vec3(0.0f);
  }
  focus.preferredRadius = radius;
  return focus;
}

void Scene::applyCameraAnchor(const CameraAnchor &anchor, bool snap) {
  if (!sceneCamera) {
    return;
  }

  m_activePreset.playing = false;

  OrbitCamera::Focus focus = anchor.focus;
  if (anchor.targetNode != nullptr && *anchor.targetNode != nullptr) {
    focus = makeFocusForNode(*anchor.targetNode, anchor.focus.preferredRadius);
  }

  sceneCamera->setFocus(focus, snap);
  sceneCamera->setAngles(anchor.yawDegrees, anchor.pitchDegrees, snap);
}

void Scene::UpdateCinematic(double deltaSeconds) {
  refreshAnchors();
  updateCameraPreset(deltaSeconds);
  if (!m_activePreset.playing && sceneCamera) {
    sceneCamera->update(deltaSeconds);
  }
}

void Scene::playCameraPreset(std::size_t index) {
  if (!sceneCamera || index >= m_cameraPresets.size()) {
    return;
  }

  auto &preset = m_cameraPresets[index];
  if (preset.targetNode != nullptr && *preset.targetNode != nullptr) {
    preset.anchor.focus =
        makeFocusForNode(*preset.targetNode, preset.anchor.focus.preferredRadius);
  }

  m_activePreset.playing = true;
  m_activePreset.index = index;
  m_activePreset.elapsedSeconds = 0.0f;
  m_activePreset.startFocus.position = sceneCamera->focusPosition();
  m_activePreset.startFocus.preferredRadius = sceneCamera->radius();
  m_activePreset.startYaw = sceneCamera->yawDegrees();
  m_activePreset.startPitch = sceneCamera->pitchDegrees();
}

void Scene::updateCameraPreset(double deltaSeconds) {
  if (!m_activePreset.playing || !sceneCamera || m_cameraPresets.empty()) {
    return;
  }

  const auto &preset = m_cameraPresets[m_activePreset.index];
  const float duration = std::max(0.01f, preset.transitionSeconds);
  const float hold = std::max(0.0f, preset.holdSeconds);

  m_activePreset.elapsedSeconds += static_cast<float>(deltaSeconds);
  const float t = std::clamp(m_activePreset.elapsedSeconds / duration, 0.0f, 1.0f);
  const float eased = animationEase(t);

  glm::vec3 focusPos = m_activePreset.startFocus.position * (1.0f - eased) +
                       preset.anchor.focus.position * eased;
  float radius = m_activePreset.startFocus.preferredRadius * (1.0f - eased) +
                 preset.anchor.focus.preferredRadius * eased;
  const float yaw = lerpAngle(m_activePreset.startYaw, preset.anchor.yawDegrees, eased);
  const float pitch = lerpAngle(m_activePreset.startPitch, preset.anchor.pitchDegrees, eased);

  OrbitCamera::Focus focus{};
  focus.position = focusPos;
  focus.preferredRadius = radius;
  sceneCamera->setFocus(focus, true);
  sceneCamera->snapAngles(yaw, pitch);

  if (t >= 1.0f && m_activePreset.elapsedSeconds >= duration + hold) {
    m_activePreset.playing = false;
    applyCameraAnchor(preset.anchor, true);
  }
}

float Scene::animationEase(float t) const {
  return easeInOut(std::clamp(t, 0.0f, 1.0f));
}

void Scene::refreshAnchor(CameraAnchor &anchor) {
  if (anchor.targetNode != nullptr && *anchor.targetNode != nullptr) {
    anchor.focus =
        makeFocusForNode(*anchor.targetNode, anchor.focus.preferredRadius);
  }
}

void Scene::refreshAnchors() {
  refreshAnchor(earthAnchor);
  refreshAnchor(moonAnchor);
  refreshAnchor(barycenterAnchor);
  for (auto &preset : m_cameraPresets) {
    if (preset.targetNode != nullptr && *preset.targetNode != nullptr) {
      preset.anchor.focus = makeFocusForNode(*preset.targetNode,
                                             preset.anchor.focus.preferredRadius);
    }
  }
}

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
#include "math/astromathlib.h"
#include "common/EOPlanetaryConstants.h"

#include <memory>
#include <string>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

Scene::Scene(SceneGraph& sceneGraph) : m_sceneGraph(sceneGraph) {

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
  lightingNode->addComponent(std::move(sunLight));

  m_sceneGraph.root()->addChild(std::move(lightingNode));

  auto skyboxNode = std::make_unique<SceneNode>();
  skyboxNode->setName("Skybox");
  skyboxNode->addComponent(std::make_unique<SkyboxComponent>());
  m_sceneGraph.root()->addChild(std::move(skyboxNode));

  auto earthNode = std::make_unique<SceneNode>();
  earthNode->setName("Earth");
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

  sceneCamera = std::make_shared<OrbitCamera>();

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
  case '1': // +X
    sceneCamera->SnapToPreset(0);
    break;

  case '2': // -X
    sceneCamera->SnapToPreset(1);
    break;

  case '3': // +Y
    sceneCamera->SnapToPreset(2);
    break;

  case '4': // -Y
    sceneCamera->SnapToPreset(3);
    break;

  case '5': // +Z
    sceneCamera->SnapToPreset(4);
    break;

  case '6': // -Z
    sceneCamera->SnapToPreset(5);
    break;

  // Move Closer
  case '+':
    sceneCamera->ZoomIn();
    break;

  // Move Back
  case '-':
    sceneCamera->ZoomOut();
    break;
  default:
    if (Log::kDebugLoggingEnabled) {
      Log::debug(std::string("Unhandled Keyboard Key: ") +
                 static_cast<char>(key));
    }
    break;
  }
}

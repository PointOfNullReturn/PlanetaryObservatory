#include "scene/Scene.h"

#include "utils/Log.h"
#include "render/TextureLoader.h"
#include "scenegraph/components/SphereMeshComponent.h"
#include "scenegraph/components/TextureLayerComponent.h"
#include "scenegraph/components/TransformComponent.h"
#include "math/astromathlib.h"
#include "common/EOPlanetaryConstants.h"

#include <memory>
#include <string>
#include <glm/gtc/type_ptr.hpp>

Scene::Scene(SceneGraph& sceneGraph) : m_sceneGraph(sceneGraph) {

  auto earthNode = std::make_unique<SceneNode>();
  earthNode->setName("Earth");
  auto earthSphere = std::make_unique<SphereMeshComponent>();
  earthSphere->radius = ASTRO_MATH_LIB::KMtoGU(EARTH_RADIUS_KM);
  earthNode->addComponent(std::move(earthSphere));
  auto earthTextureLayers = std::make_unique<TextureLayerComponent>();
  earthTextureLayers->layers.push_back({LoadTexture2D("assets/textures/world.200407.3x5400x2700.png", true, false, true), TextureBlendMode::None, 1.0f});
  earthNode->addComponent(std::move(earthTextureLayers));
  this->earthNode = earthNode.get();
  m_sceneGraph.root()->addChild(std::move(earthNode));

  auto moonNode = std::make_unique<SceneNode>();
  moonNode->setName("Moon");
  auto moonSphere = std::make_unique<SphereMeshComponent>();
  moonSphere->radius = 0.50;
  moonNode->addComponent(std::move(moonSphere));
  auto moonTextureLayers = std::make_unique<TextureLayerComponent>();
  moonTextureLayers->layers.push_back({LoadTexture2D("assets/textures/moon_sm.bmp", true, false), TextureBlendMode::None, 1.0f});
  moonNode->addComponent(std::move(moonTextureLayers));
  this->moonNode = moonNode.get();
  m_sceneGraph.root()->addChild(std::move(moonNode));

  sceneCamera = std::make_shared<OrbitCamera>();
  skybox = std::make_unique<Skybox>();

  // TODO: Finish Lighting Object
  // ambientLight = std::make_unique<Light>("LIGHT0", GL_LIGHT0);
  InitializeScene();

  // This all needs to go in a scene init function.
  axes = std::make_unique<Axis>(10.0, 2.0, false);

  if (axes->GetEnableAxes())
    showAxes = true;
  else
    showAxes = false;

  m_currentlyAnimating = false;
}

Scene::~Scene() {}

void Scene::InitializeScene(void) {
  // Initialize Background Color
  // Set Background Color to Black
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // Enable GL Features
  // Enable Line Smoothing
  glEnable(GL_LINE_SMOOTH_HINT);

  // Enable Depth Test
  glEnable(GL_DEPTH_TEST);

  // Enable Cull Face
  glEnable(GL_CULL_FACE);

  // Enable Shading
  glShadeModel(GL_SMOOTH);

  // Enable Normalization
  glEnable(GL_NORMALIZE);

  // Enable Lighting
  glEnable(GL_LIGHTING);

  // glEnable(GL_COLOR_MATERIAL);

  // glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  // Initialize Lighting
  // LIGHT_0 - Ambient Light
  glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(ambientLightColor));

  // LIGHT_1 - Specular Light
  glLightfv(GL_LIGHT1, GL_SPECULAR, glm::value_ptr(specularLightColor));
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
  axes->SetEnableAxes(showAxes);
}

void Scene::SetCurrentlyAnimating(GLboolean animating) {
  m_currentlyAnimating = animating;
}

// Other Methods
void Scene::RenderScene(void) {
  // Clear the window and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  if (sceneCamera) {
    sceneCamera->Render();
  }

  if (skybox) {
    skybox->render();
  }

  glEnable(GL_LIGHT0);

  glPushMatrix();
  glLightfv(GL_LIGHT1, GL_POSITION, glm::value_ptr(light1Position));
  glEnable(GL_LIGHT1);
  glPopMatrix();

  // The scene graph will handle the rendering of the objects
}

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

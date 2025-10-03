#ifndef PLANETARYOBSERVATORY_SCENE_H
#define PLANETARYOBSERVATORY_SCENE_H

#include "common/EOGL.h"

#include "common/EOGlobalEnums.h"
#include "render/OrbitCamera.h"
#include "scene/Light.h"
#include "scenegraph/SceneGraph.h"
#include "scenegraph/SceneNode.h"
#include "scenegraph/components/TextureLayerComponent.h"

#include <memory>

#include <glm/glm.hpp>

class Scene {
public:
  Scene(SceneGraph& sceneGraph);
  ~Scene();

  // Accessor Methods
  RenderModes GetRenderMode(void);
  GLboolean GetShowAxes(void);
  GLboolean GetCurrentlyAnimating(void);

  // Mutator Methods
  void SetRenderMode(RenderModes renderMode);
  void SetShowAxes(GLboolean show);
  void SetCurrentlyAnimating(GLboolean animating);

  // Other Methods
  void UpdateScene(void);
  void InitializeScene(void);
  void HandleKeyboardInput(unsigned char key);

  /// Returns the active orbit camera controlling the scene view.
  std::shared_ptr<OrbitCamera> GetCamera() { return sceneCamera; }
  /// Returns the active orbit camera controlling the scene view.
  std::shared_ptr<const OrbitCamera> GetCamera() const { return sceneCamera; }

private:
  // Data Members
  // Scene Objects
  SceneGraph& m_sceneGraph;
  SceneNode* earthNode = nullptr;
  SceneNode* moonNode = nullptr;
  SceneNode* axesNode = nullptr;

  // TODO: Implement All Planetary Objects in an array
  // vector<PlanetaryObject*> planetaryObjects;    // List of Planetary Objects
  // to Render

  // Lighting Objects
  [[maybe_unused]] std::unique_ptr<Light>
      ambientLight; // Scene Ambient Lighting

  // Camera Objects
  [[maybe_unused]] CameraModes currentMode;

  // Render Mode
  RenderModes m_renderMode; // Render the scene in wireframe or solids

  // Animating Mode
  GLboolean m_currentlyAnimating;

  GLboolean showAxes; // Whether or not to show the coordinate system axis

  glm::vec4 ambientLightColor = {0.18f, 0.18f, 0.18f, 1.0f};

  glm::vec4 specularLightColor = {0.7f, 0.7f, 0.7f, 1.0f};

  glm::vec4 light1Position = {0.0, 0.0, 10.0, 1.0};

  std::shared_ptr<OrbitCamera> sceneCamera;
};

#endif // PLANETARYOBSERVATORY_SCENE_H

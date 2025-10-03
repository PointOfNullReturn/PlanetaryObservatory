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
#include <string>
#include <vector>

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
  void UpdateCinematic(double deltaSeconds);

  void ToggleTimeLapse();
  void SetTimeLapseEnabled(bool enabled);
  void SetTimeLapseHold(bool enabled);
  void SetTimeLapseFactor(float factor);
  bool IsTimeLapseEnabled() const { return m_timeLapseEnabled; }
  bool IsTimeLapseActive() const { return m_timeLapseEnabled || m_timeLapseHoldActive; }
  float TimeLapseFactor() const { return m_timeLapseFactor; }

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
  bool m_timeLapseEnabled = false;
  bool m_timeLapseHoldActive = false;
  float m_timeLapseFactor = 10.0f;

  struct CameraAnchor {
    OrbitCamera::Focus focus;
    float yawDegrees = 270.0f;
    float pitchDegrees = 0.0f;
    SceneNode **targetNode = nullptr;
  };

  struct CameraPreset {
    std::string name;
    CameraAnchor anchor;
    float transitionSeconds = 5.0f;
    float holdSeconds = 1.5f;
    SceneNode **targetNode = nullptr;
  };

  struct ActiveCameraPreset {
    bool playing = false;
    std::size_t index = 0;
    float elapsedSeconds = 0.0f;
    OrbitCamera::Focus startFocus{};
    float startYaw = 270.0f;
    float startPitch = 0.0f;
  };

  OrbitCamera::Focus makeFocusForNode(const SceneNode *node, float radius) const;
  void refreshAnchor(CameraAnchor &anchor);
  void refreshAnchors();
  void applyCameraAnchor(const CameraAnchor &anchor, bool snap);
  void playCameraPreset(std::size_t index);
  void updateCameraPreset(double deltaSeconds);
  float animationEase(float t) const;

  CameraAnchor earthAnchor{};
  CameraAnchor moonAnchor{};
  CameraAnchor barycenterAnchor{};
  std::vector<CameraPreset> m_cameraPresets;
  ActiveCameraPreset m_activePreset;
};

#endif // PLANETARYOBSERVATORY_SCENE_H

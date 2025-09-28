#pragma once

#include "scenegraph/components/Component.h"

#include <memory>

class OrbitCamera;

/// Component wrapper exposing an OrbitCamera through the scene graph.
class CameraComponent : public Component {
public:
  explicit CameraComponent(std::shared_ptr<OrbitCamera> camera);

  std::shared_ptr<OrbitCamera> camera() const { return m_camera; }

  void onAttach(SceneNode &node) override;
  void onDetach(SceneNode &node) override;
  void onUpdate(SceneNode &node, double deltaTimeSeconds) override;
  void onRender(SceneNode &node) override;

private:
  std::shared_ptr<OrbitCamera> m_camera;
};

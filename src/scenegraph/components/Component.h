#pragma once

class SceneNode;

/// Base component type attached to a SceneNode.
class Component {
public:
  virtual ~Component() = default;

  /// Called when the parent node is attached to the scene graph.
  virtual void onAttach(SceneNode &) {}
  /// Called when the parent node is detached from the scene graph.
  virtual void onDetach(SceneNode &) {}
  /// Per-frame update hook.
  virtual void onUpdate(SceneNode &, double) {}
  /// Render hook invoked during the scene graph pass.
  virtual void onRender(SceneNode &) {}
};

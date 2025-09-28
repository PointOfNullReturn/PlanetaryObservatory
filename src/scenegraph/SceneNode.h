#pragma once

#include <glm/glm.hpp>
#include "scenegraph/components/Component.h"

#include <memory>
#include <string>
#include <vector>

class TransformComponent;

/// Basic node in the scene graph that owns children and a transform.
class SceneNode {
public:
  /// Constructs a node with identity transform.
  SceneNode();
  virtual ~SceneNode() = default;

  /// Returns the transform matrix for the node.
  glm::mat4 getTransform() const;

  /// Returns the parent node or nullptr for the root.
  SceneNode *parent();
  /// Returns the parent node or nullptr for the root (const).
  const SceneNode *parent() const;

  /// Assigns the parent pointer; used by SceneGraph during re-parenting.
  void setParent(SceneNode *parent);

  /// Adds a child node and updates its parent pointer.
  void addChild(std::unique_ptr<SceneNode> child);
  /// Returns the list of children.
  std::vector<std::unique_ptr<SceneNode>> &children();
  /// Returns the list of children (const).
  const std::vector<std::unique_ptr<SceneNode>> &children() const;

  /// Adds a component instance to this node.
  void addComponent(std::unique_ptr<Component> component);
  /// Returns the components attached to this node.
  std::vector<std::unique_ptr<Component>> &components();
  /// Returns the components attached to this node (const).
  const std::vector<std::unique_ptr<Component>> &components() const;

  template <typename T>
  T* getComponent() const {
    for (const auto& component : m_components) {
      if (auto* casted = dynamic_cast<T*>(component.get())) {
        return casted;
      }
    }
    return nullptr;
  }

  /// Sets a tooling-friendly name for this node.
  void setName(std::string name);
  /// Returns the tooling/debug name of the node.
  const std::string &name() const { return m_name; }

  /// Called when the node enters the active scene graph.
  virtual void onAttach();
  /// Called when the node is removed from the active scene graph.
  virtual void onDetach();
  /// Per-frame update hook; `deltaTimeSeconds` is wall-clock time between
  /// frames.
  virtual void onUpdate(double deltaTimeSeconds);
  /// Render hook invoked during the scene graph render pass.
  virtual void onRender();

protected:
  std::vector<std::unique_ptr<SceneNode>> m_children;
  SceneNode *m_parent = nullptr;
  std::string m_name;
  std::vector<std::unique_ptr<Component>> m_components;
};

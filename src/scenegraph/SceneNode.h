#pragma once

#include <memory>
#include <vector>

/// Represents a local transform for a scene node.
struct Transform {
  float position[3] = {0.0f, 0.0f, 0.0f};
  float rotation[3] = {0.0f, 0.0f, 0.0f};
  float scale[3] = {1.0f, 1.0f, 1.0f};
};

/// Basic node in the scene graph that owns children and a transform.
class SceneNode {
public:
  /// Constructs a node with identity transform.
  SceneNode();
  /// Constructs a node with a custom transform.
  explicit SceneNode(const Transform &transform);
  virtual ~SceneNode() = default;

  /// Returns a mutable reference to the node transform.
  Transform &transform();
  /// Returns a read-only reference to the node transform.
  const Transform &transform() const;

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

protected:
  Transform m_transform;
  std::vector<std::unique_ptr<SceneNode>> m_children;
  SceneNode *m_parent = nullptr;
};

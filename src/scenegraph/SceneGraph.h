#pragma once

#include "scenegraph/SceneNode.h"

#include <functional>
#include <memory>

/// Owns the root of the scene hierarchy and provides traversal utilities.
class SceneGraph {
public:
  /// Constructs an empty graph with no root node.
  SceneGraph();
  /// Destroys the graph and the owned node hierarchy.
  ~SceneGraph();

  /// Sets the root node for the graph and resets the parent pointer.
  void setRoot(std::unique_ptr<SceneNode> root);
  /// Returns a mutable pointer to the root node.
  SceneNode *root();
  /// Returns a read-only pointer to the root node.
  const SceneNode *root() const;

  /// Depth-first traversal yielding mutable nodes.
  void traverse(const std::function<void(SceneNode &)> &visitor);
  /// Depth-first traversal yielding read-only nodes.
  void traverse(const std::function<void(const SceneNode &)> &visitor) const;

  /// Calls `onAttach` on the root and descendants.
  void attach();
  /// Calls `onDetach` on the root and descendants.
  void detach();
  /// Calls `onUpdate` on the root and descendants.
  void update(double deltaTimeSeconds);
  /// Calls `onRender` on the root and descendants.
  void render();

private:
  std::unique_ptr<SceneNode> m_root;

  /// Helper used by traverse to walk children.
  static void traverseImpl(SceneNode &node,
                           const std::function<void(SceneNode &)> &visitor);
  /// Helper used by const traverse to walk children.
  static void
  traverseConstImpl(const SceneNode &node,
                    const std::function<void(const SceneNode &)> &visitor);
  /// Helper used by attach() to walk children.
  static void attachImpl(SceneNode &node);
  /// Helper used by detach() to walk children.
  static void detachImpl(SceneNode &node);
  /// Helper used by update() to walk children.
  static void updateImpl(SceneNode &node, double deltaTimeSeconds);
  /// Helper used by render() to walk children.
  static void renderImpl(SceneNode &node);
};

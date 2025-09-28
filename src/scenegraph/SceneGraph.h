#pragma once

#include "scenegraph/SceneNode.h"

#include <functional>
#include <memory>

class SceneGraph {
public:
  SceneGraph();
  ~SceneGraph();

  void setRoot(std::unique_ptr<SceneNode> root);
  SceneNode *root();
  const SceneNode *root() const;

  void traverse(const std::function<void(SceneNode &)> &visitor);
  void traverse(const std::function<void(const SceneNode &)> &visitor) const;

private:
  std::unique_ptr<SceneNode> m_root;

  static void traverseImpl(SceneNode &node,
                           const std::function<void(SceneNode &)> &visitor);
  static void
  traverseConstImpl(const SceneNode &node,
                    const std::function<void(const SceneNode &)> &visitor);
};

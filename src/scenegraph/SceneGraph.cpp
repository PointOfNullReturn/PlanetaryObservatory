#include "scenegraph/SceneGraph.h"

SceneGraph::SceneGraph() = default;

SceneGraph::~SceneGraph() = default;

void SceneGraph::setRoot(std::unique_ptr<SceneNode> root) {
  if (root) {
    root->setParent(nullptr);
  }
  m_root = std::move(root);
}

SceneNode *SceneGraph::root() { return m_root.get(); }

const SceneNode *SceneGraph::root() const { return m_root.get(); }

void SceneGraph::traverse(const std::function<void(SceneNode &)> &visitor) {
  if (!m_root || !visitor) {
    return;
  }
  traverseImpl(*m_root, visitor);
}

void SceneGraph::traverse(
    const std::function<void(const SceneNode &)> &visitor) const {
  if (!m_root || !visitor) {
    return;
  }
  traverseConstImpl(*m_root, visitor);
}

void SceneGraph::traverseImpl(SceneNode &node,
                              const std::function<void(SceneNode &)> &visitor) {
  visitor(node);
  for (auto &child : node.children()) {
    if (child) {
      traverseImpl(*child, visitor);
    }
  }
}

void SceneGraph::traverseConstImpl(
    const SceneNode &node,
    const std::function<void(const SceneNode &)> &visitor) {
  visitor(node);
  for (const auto &child : node.children()) {
    if (child) {
      traverseConstImpl(*child, visitor);
    }
  }
}

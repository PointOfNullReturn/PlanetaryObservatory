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

void SceneGraph::attach() {
  if (!m_root) {
    return;
  }
  attachImpl(*m_root);
}

void SceneGraph::detach() {
  if (!m_root) {
    return;
  }
  detachImpl(*m_root);
}

void SceneGraph::update(double deltaTimeSeconds) {
  if (!m_root) {
    return;
  }
  updateImpl(*m_root, deltaTimeSeconds);
}

void SceneGraph::render() {
  if (!m_root) {
    return;
  }
  renderImpl(*m_root);
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

void SceneGraph::attachImpl(SceneNode &node) {
  node.onAttach();
  for (auto &child : node.children()) {
    if (child) {
      attachImpl(*child);
    }
  }
}

void SceneGraph::detachImpl(SceneNode &node) {
  node.onDetach();
  for (auto &child : node.children()) {
    if (child) {
      detachImpl(*child);
    }
  }
}

void SceneGraph::updateImpl(SceneNode &node, double deltaTimeSeconds) {
  node.onUpdate(deltaTimeSeconds);
  for (auto &child : node.children()) {
    if (child) {
      updateImpl(*child, deltaTimeSeconds);
    }
  }
}

void SceneGraph::renderImpl(SceneNode &node) {
  node.onRender();
  for (auto &child : node.children()) {
    if (child) {
      renderImpl(*child);
    }
  }
}

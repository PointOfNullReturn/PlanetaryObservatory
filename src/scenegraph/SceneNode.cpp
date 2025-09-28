#include "scenegraph/SceneNode.h"

SceneNode::SceneNode() = default;

SceneNode::SceneNode(const Transform &transform) : m_transform(transform) {}

Transform &SceneNode::transform() { return m_transform; }

const Transform &SceneNode::transform() const { return m_transform; }

SceneNode *SceneNode::parent() { return m_parent; }

const SceneNode *SceneNode::parent() const { return m_parent; }

void SceneNode::addChild(std::unique_ptr<SceneNode> child) {
  if (child) {
    child->m_parent = this;
  }
  m_children.emplace_back(std::move(child));
}

std::vector<std::unique_ptr<SceneNode>> &SceneNode::children() {
  return m_children;
}

const std::vector<std::unique_ptr<SceneNode>> &SceneNode::children() const {
  return m_children;
}

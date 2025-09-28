#include "scenegraph/SceneNode.h"
#include "scenegraph/components/TransformComponent.h"

SceneNode::SceneNode() {
    addComponent(std::make_unique<TransformComponent>());
}

glm::mat4 SceneNode::getTransform() const {
    if (auto* transformComponent = getComponent<TransformComponent>()) {
        return transformComponent->getTransform();
    }
    return glm::mat4(1.0f);
}

SceneNode *SceneNode::parent() { return m_parent; }

const SceneNode *SceneNode::parent() const { return m_parent; }

void SceneNode::setParent(SceneNode *parent) { m_parent = parent; }

void SceneNode::addChild(std::unique_ptr<SceneNode> child) {
  if (child) {
    child->setParent(this);
  }
  m_children.emplace_back(std::move(child));
}

std::vector<std::unique_ptr<SceneNode>> &SceneNode::children() {
  return m_children;
}

const std::vector<std::unique_ptr<SceneNode>> &SceneNode::children() const {
  return m_children;
}

void SceneNode::addComponent(std::unique_ptr<Component> component) {
  if (component) {
    m_components.emplace_back(std::move(component));
  }
}

std::vector<std::unique_ptr<Component>> &SceneNode::components() {
  return m_components;
}

const std::vector<std::unique_ptr<Component>> &SceneNode::components() const {
  return m_components;
}

void SceneNode::setName(std::string name) { m_name = std::move(name); }

void SceneNode::onAttach() {}

void SceneNode::onDetach() {}

void SceneNode::onUpdate(double) {}

void SceneNode::onRender() {}

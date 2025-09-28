#pragma once

#include <memory>
#include <vector>

struct Transform {
  float position[3] = {0.0f, 0.0f, 0.0f};
  float rotation[3] = {0.0f, 0.0f, 0.0f};
  float scale[3] = {1.0f, 1.0f, 1.0f};
};

class SceneNode {
public:
  SceneNode();
  explicit SceneNode(const Transform &transform);
  virtual ~SceneNode() = default;

  Transform &transform();
  const Transform &transform() const;

  SceneNode *parent();
  const SceneNode *parent() const;

  void setParent(SceneNode *parent);

  void addChild(std::unique_ptr<SceneNode> child);
  std::vector<std::unique_ptr<SceneNode>> &children();
  const std::vector<std::unique_ptr<SceneNode>> &children() const;

protected:
  Transform m_transform;
  std::vector<std::unique_ptr<SceneNode>> m_children;
  SceneNode *m_parent = nullptr;
};

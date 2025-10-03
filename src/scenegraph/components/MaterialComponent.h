#ifndef PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_MATERIALCOMPONENT_H
#define PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_MATERIALCOMPONENT_H

#include "scenegraph/components/Component.h"

#include <glm/vec4.hpp>

class SceneNode;

/// Stores per-node material parameters used by the lighting shader.
class MaterialComponent : public Component {
public:
  /// Collection of material parameters consumed by the renderer.
  struct MaterialProperties {
    glm::vec4 diffuseColor{1.0f, 1.0f, 1.0f, 1.0f};
    float specularStrength{1.0f};
    float shininess{16.0f};
    float ambientMix{1.0f};
    float exposure{1.0f};
    float gamma{2.2f};
    glm::vec4 rimColor{0.0f, 0.0f, 0.0f, 1.0f};
    float rimStrength{0.0f};
    float rimExponent{2.0f};
  };

  MaterialComponent() = default;
  ~MaterialComponent() override = default;

  /// Returns the immutable material configuration.
  const MaterialProperties &material() const { return m_material; }
  /// Returns a mutable reference for configuring the material.
  MaterialProperties &material() { return m_material; }

  void onRender(SceneNode &node) override;

private:
  MaterialProperties m_material;
};

#endif // PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_MATERIALCOMPONENT_H

#ifndef PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_DIRECTIONALLIGHTCOMPONENT_H
#define PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_DIRECTIONALLIGHTCOMPONENT_H

#include "scenegraph/components/Component.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class SceneNode;

/// Represents a directional light source that casts parallel rays.
class DirectionalLightComponent : public Component {
public:
  /// Metadata describing the light for renderer consumption.
  struct LightData {
    glm::vec3 direction{0.0f, 0.0f, -1.0f};
    glm::vec4 diffuseColor{1.0f, 1.0f, 1.0f, 1.0f};
    glm::vec4 specularColor{1.0f, 1.0f, 1.0f, 1.0f};
    float intensity{1.0f};
    bool enabled{true};
  };

  DirectionalLightComponent() = default;
  ~DirectionalLightComponent() override = default;

  /// Returns the current light configuration.
  const LightData &light() const { return m_light; }
  /// Mutable access for editor tools.
  LightData &light() { return m_light; }

  void onRender(SceneNode &node) override;

private:
  LightData m_light;
};

#endif // PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_DIRECTIONALLIGHTCOMPONENT_H

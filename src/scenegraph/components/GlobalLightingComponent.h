#ifndef PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_GLOBALLIGHTINGCOMPONENT_H
#define PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_GLOBALLIGHTINGCOMPONENT_H

#include "scenegraph/components/Component.h"

#include <glm/vec4.hpp>

class SceneNode;

/// Configures the scene-wide ambient lighting parameters.
class GlobalLightingComponent : public Component {
public:
  /// Struct describing global lighting for consumption by future renderer layers.
  struct LightingData {
    glm::vec4 ambientColor{0.50f, 0.50f, 0.50f, 1.0f};
    glm::vec4 backgroundColor{0.0f, 0.0f, 0.0f, 1.0f};
    bool enableLighting{true};
    bool enableNormalization{true};
  };

  GlobalLightingComponent() = default;
  ~GlobalLightingComponent() override = default;

  const LightingData &lighting() const { return m_lighting; }
  LightingData &lighting() { return m_lighting; }

  /// Legacy hook retained for compatibility; performs no GL side effects.
  void onRender(SceneNode &node) override;

private:
  LightingData m_lighting;
};

#endif // PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_GLOBALLIGHTINGCOMPONENT_H

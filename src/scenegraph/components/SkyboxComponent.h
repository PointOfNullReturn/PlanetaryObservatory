#ifndef PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_SKYBOXCOMPONENT_H
#define PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_SKYBOXCOMPONENT_H

#include "scenegraph/components/Component.h"
#include "render/Skybox.h"

/// Render component that draws the omnidirectional background cubemap.
class SkyboxComponent : public Component {
public:
  SkyboxComponent() = default;
  ~SkyboxComponent() override = default;

  void onRender(SceneNode &node) override;

private:
  Skybox skybox;
};

#endif // PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_SKYBOXCOMPONENT_H

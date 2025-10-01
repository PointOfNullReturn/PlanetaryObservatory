#include "scenegraph/components/SkyboxComponent.h"

#include "scenegraph/SceneNode.h"

void SkyboxComponent::onRender(SceneNode &node) {
  (void)node;
  // Rendering is handled by SceneRenderer once per frame.
}

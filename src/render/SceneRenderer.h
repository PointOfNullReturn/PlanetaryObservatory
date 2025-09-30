#ifndef PLANETARY_OBSERVATORY_RENDER_SCENERENDERER_H
#define PLANETARY_OBSERVATORY_RENDER_SCENERENDERER_H

#include "render/RenderContext.h"

class SceneGraph;
class SceneNode;

/// Walks the scene graph to invoke render hooks using shared context data.
class SceneRenderer {
public:
  SceneRenderer() = default;
  ~SceneRenderer() = default;

  /// Renders the provided scene graph using the supplied context.
  void render(SceneGraph &sceneGraph, const RenderContext &context);

private:
  void renderNode(SceneNode &node, const RenderContext &context);
};

#endif // PLANETARY_OBSERVATORY_RENDER_SCENERENDERER_H

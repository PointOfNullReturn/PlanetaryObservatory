#include "render/SceneRenderer.h"

#include "scenegraph/SceneGraph.h"
#include "scenegraph/SceneNode.h"

void SceneRenderer::render(SceneGraph &sceneGraph, const RenderContext &context) {
  (void)context;
  SceneNode *root = sceneGraph.root();
  if (root == nullptr) {
    return;
  }
  renderNode(*root, context);
}

void SceneRenderer::renderNode(SceneNode &node, const RenderContext &context) {
  (void)context;
  node.onRender();
  for (auto &child : node.children()) {
    if (child) {
      renderNode(*child, context);
    }
  }
}

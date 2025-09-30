#ifndef PLANETARY_OBSERVATORY_RENDER_RENDERCONTEXT_H
#define PLANETARY_OBSERVATORY_RENDER_RENDERCONTEXT_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

/// Captures per-frame state shared across render components.
struct RenderContext {
  /// Model-view matrix calculated by the active camera.
  glm::mat4 viewMatrix{1.0f};
  /// Projection matrix currently bound.
  glm::mat4 projectionMatrix{1.0f};
  /// Camera (world) position for shading calculations.
  glm::vec3 cameraPosition{0.0f, 0.0f, 0.0f};
  /// Seconds elapsed since the last frame.
  double deltaTimeSeconds{0.0};
};

#endif // PLANETARY_OBSERVATORY_RENDER_RENDERCONTEXT_H

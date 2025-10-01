#ifndef PLANETARY_OBSERVATORY_RENDER_SCENERENDERER_H
#define PLANETARY_OBSERVATORY_RENDER_SCENERENDERER_H

#include "render/RenderContext.h"
#include "render/ShaderProgram.h"

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class SceneGraph;
class SceneNode;
class DirectionalLightComponent;
class GlobalLightingComponent;
class TextureLayerComponent;
class SphereMeshComponent;
class AxisComponent;
class SkyboxComponent;

struct DirectionalLightData {
  bool enabled = false;
  glm::vec3 direction{0.0f, 0.0f, -1.0f};
  glm::vec4 diffuse{1.0f};
  glm::vec4 specular{1.0f};
};

/// Walks the scene graph to invoke render hooks using shared context data.
class SceneRenderer {
public:
  SceneRenderer();
  ~SceneRenderer() = default;

  /// Renders the provided scene graph using the supplied context.
  void render(SceneGraph &sceneGraph, const RenderContext &context);

private:
  void renderNode(SceneNode &node, const RenderContext &context);
  void gatherLights(SceneNode &node);
  void applyGlobalLighting(const GlobalLightingComponent &component);
  void applyDirectionalLight(const DirectionalLightComponent &component);
  void renderSkybox(SkyboxComponent &component, const RenderContext &context);
  void renderSphere(SceneNode &node, SphereMeshComponent &mesh,
                    TextureLayerComponent *textures,
                    const RenderContext &context, const glm::mat4 &modelMatrix);
  void renderAxes(SceneNode &node, AxisComponent &axes,
                  const RenderContext &context, const glm::mat4 &modelMatrix);
  void bindShaderUniforms(const RenderContext &context,
                          const glm::mat4 &modelMatrix,
                          const glm::vec4 &materialColor, bool useTexture,
                          GLint textureUnit, bool useVertexColor,
                          bool enableLighting);

  ShaderProgram m_basicProgram;
  ShaderProgram m_skyboxProgram;
  std::vector<DirectionalLightData> m_directionalLights;
  glm::vec4 m_ambientColor{0.5f, 0.5f, 0.5f, 1.0f};
  bool m_basicLoaded = false;
  bool m_skyboxLoaded = false;
  bool m_globalLightingEnabled = true;
};

#endif // PLANETARY_OBSERVATORY_RENDER_SCENERENDERER_H

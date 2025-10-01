#include "render/SceneRenderer.h"

#include "render/GlCapabilities.h"
#include "render/GlState.h"
#include "utils/Log.h"
#include "scenegraph/SceneGraph.h"
#include "scenegraph/SceneNode.h"
#include "scenegraph/components/DirectionalLightComponent.h"
#include "scenegraph/components/GlobalLightingComponent.h"
#include "scenegraph/components/AxisComponent.h"
#include "scenegraph/components/SkyboxComponent.h"
#include "scenegraph/components/SphereMeshComponent.h"
#include "scenegraph/components/TextureLayerComponent.h"

#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <array>
#include <limits>

namespace {
constexpr int kMaxDirectionalLights = 4;

glm::vec3 normalizeOrDefault(const glm::vec3 &vector,
                             const glm::vec3 &fallback) {
  const float length = glm::length(vector);
  if (length <= std::numeric_limits<float>::epsilon()) {
    return fallback;
  }
  return vector / length;
}
}

SceneRenderer::SceneRenderer() {
  m_basicLoaded = m_basicProgram.loadFromFiles("assets/shaders/basic.vert",
                                              "assets/shaders/basic.frag");
  if (!m_basicLoaded) {
    Log::error("SceneRenderer: failed to load basic shader program; rendering "
               "will be skipped.");
  }

  m_skyboxLoaded =
      m_skyboxProgram.loadFromFiles("assets/shaders/skybox.vert",
                                    "assets/shaders/skybox.frag");
  if (!m_skyboxLoaded) {
    Log::warn("SceneRenderer: skybox shader failed to load; background will "
              "not be rendered.");
  }
}

void SceneRenderer::render(SceneGraph &sceneGraph, const RenderContext &context) {
  if (!m_basicLoaded) {
    return;
  }

  SceneNode *root = sceneGraph.root();
  if (!root) {
    return;
  }

  m_directionalLights.clear();
  m_globalLightingEnabled = true;
  m_ambientColor = glm::vec4(0.5f);

  gatherLights(*root);
  renderNode(*root, context);
}

void SceneRenderer::gatherLights(SceneNode &node) {
  if (auto *globalLighting = node.getComponent<GlobalLightingComponent>()) {
    applyGlobalLighting(*globalLighting);
  }
  if (auto *directional = node.getComponent<DirectionalLightComponent>()) {
    applyDirectionalLight(*directional, node);
  }

  for (auto &child : node.children()) {
    if (child) {
      gatherLights(*child);
    }
  }
}

void SceneRenderer::applyGlobalLighting(const GlobalLightingComponent &component) {
  const auto &data = component.lighting();
  m_globalLightingEnabled = data.enableLighting;
  m_ambientColor = data.ambientColor;
  glstate::setClearColor(data.backgroundColor);
}

void SceneRenderer::applyDirectionalLight(const DirectionalLightComponent &component,
                                         const SceneNode &node) {
  DirectionalLightData data;
  data.enabled = component.light().enabled;
  const glm::vec3 fallback(0.0f, 0.0f, -1.0f);
  const glm::vec3 localDirection =
      normalizeOrDefault(component.light().direction, fallback);
  const glm::mat4 transform = node.getTransform();
  const glm::mat3 rotation(transform);
  const glm::vec3 worldDirection =
      normalizeOrDefault(rotation * localDirection, fallback);
  data.direction = worldDirection;
  const float intensity = component.light().intensity;
  data.diffuse = component.light().diffuseColor * intensity;
  data.specular = component.light().specularColor * intensity;
  m_directionalLights.push_back(data);
}

void SceneRenderer::renderSkybox(SkyboxComponent &component,
                                 const RenderContext &context) {
  if (!m_skyboxLoaded) {
    return;
  }

  Skybox &skybox = component.cube();
  if (!skybox.isLoaded() || skybox.indexCount() == 0) {
    return;
  }

  glDepthMask(GL_FALSE);
  glDepthFunc(GL_LEQUAL);
  glCullFace(GL_FRONT);

  m_skyboxProgram.use();

  const glm::mat4 viewMatrix =
      glm::mat4(glm::mat3(context.viewMatrix));
  const GLint uView = glGetUniformLocation(m_skyboxProgram.id(), "uView");
  const GLint uProjection =
      glGetUniformLocation(m_skyboxProgram.id(), "uProjection");
  const GLint uSkybox = glGetUniformLocation(m_skyboxProgram.id(), "uSkybox");

  glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
  glUniformMatrix4fv(uProjection, 1, GL_FALSE,
                     glm::value_ptr(context.projectionMatrix));
  if (uSkybox >= 0) {
    glUniform1i(uSkybox, 0);
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureId());

  if (skybox.usesVertexArray() && glSupportsVertexArrayObjects() &&
      skybox.vao() != 0) {
    glBindVertexArray(skybox.vao());
    glDrawElements(GL_TRIANGLES, skybox.indexCount(), GL_UNSIGNED_SHORT,
                   nullptr);
    glBindVertexArray(0);
  } else if (skybox.vbo() != 0 && skybox.ebo() != 0) {
    glBindBuffer(GL_ARRAY_BUFFER, skybox.vbo());
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          static_cast<GLsizei>(6 * sizeof(GLfloat)),
                          reinterpret_cast<const void *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          static_cast<GLsizei>(6 * sizeof(GLfloat)),
                          reinterpret_cast<const void *>(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox.ebo());
    glDrawElements(GL_TRIANGLES, skybox.indexCount(), GL_UNSIGNED_SHORT,
                   nullptr);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glActiveTexture(GL_TEXTURE0);
  glUseProgram(0);

  glCullFace(GL_BACK);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
}

void SceneRenderer::renderSphere(SceneNode &node, SphereMeshComponent &mesh,
                                 TextureLayerComponent *textures,
                                 const RenderContext &context,
                                 const glm::mat4 &modelMatrix) {
  (void)node;

  const bool hasTexture =
      textures != nullptr && textures->bindForShader(0);

  bindShaderUniforms(context, modelMatrix, glm::vec4(1.0f), hasTexture, 0,
                     /*useVertexColor=*/false, m_globalLightingEnabled);

  if (mesh.renderMode == RENDER_MODE_WIREFRAME) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  mesh.renderWithShader();

  if (mesh.renderMode == RENDER_MODE_WIREFRAME) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  if (textures != nullptr) {
    textures->unbindFromShader(0);
  }

  glUseProgram(0);
}

void SceneRenderer::renderAxes(SceneNode &node, AxisComponent &axes,
                               const RenderContext &context,
                               const glm::mat4 &modelMatrix) {
  if (!axes.enabled) {
    return;
  }

  axes.ensureGeometry(node);
  if (!axes.hasGeometry()) {
    return;
  }

  bindShaderUniforms(context, modelMatrix, glm::vec4(1.0f), false, 0,
                     /*useVertexColor=*/true, /*enableLighting=*/false);

  glLineWidth(axes.lineWidth);
  axes.draw();
  glLineWidth(1.0f);

  glUseProgram(0);
}

void SceneRenderer::renderNode(SceneNode &node, const RenderContext &context) {
  const glm::mat4 model = node.getTransform();

  auto *skybox = node.getComponent<SkyboxComponent>();
  if (skybox != nullptr) {
    renderSkybox(*skybox, context);
  }

  auto *textures = node.getComponent<TextureLayerComponent>();
  auto *mesh = node.getComponent<SphereMeshComponent>();
  if (mesh != nullptr) {
    renderSphere(node, *mesh, textures, context, model);
  }

  auto *axes = node.getComponent<AxisComponent>();
  if (axes != nullptr) {
    renderAxes(node, *axes, context, model);
  }

  for (auto &component : node.components()) {
    const auto *raw = component.get();
    if (raw == mesh || raw == textures || raw == axes || raw == skybox) {
      continue;
    }
    component->onRender(node);
  }

  for (auto &child : node.children()) {
    if (child) {
      renderNode(*child, context);
    }
  }
}

void SceneRenderer::bindShaderUniforms(const RenderContext &context,
                                       const glm::mat4 &modelMatrix,
                                       const glm::vec4 &materialColor,
                                       bool useTexture, GLint textureUnit,
                                       bool useVertexColor,
                                       bool enableLighting) {
  m_basicProgram.use();

  const GLint uModel = glGetUniformLocation(m_basicProgram.id(), "uModel");
  const GLint uView = glGetUniformLocation(m_basicProgram.id(), "uView");
  const GLint uProjection =
      glGetUniformLocation(m_basicProgram.id(), "uProjection");
  const GLint uCameraPos =
      glGetUniformLocation(m_basicProgram.id(), "uCameraPos");
  const GLint uAmbient =
      glGetUniformLocation(m_basicProgram.id(), "uAmbientColor");
  const GLint uMaterial =
      glGetUniformLocation(m_basicProgram.id(), "uMaterialDiffuse");
  const GLint uUseTexture =
      glGetUniformLocation(m_basicProgram.id(), "uUseTexture");
  const GLint uTexture = glGetUniformLocation(m_basicProgram.id(), "uTexture");
  const GLint uUseVertexColor =
      glGetUniformLocation(m_basicProgram.id(), "uUseVertexColor");
  const GLint uEnableLighting =
      glGetUniformLocation(m_basicProgram.id(), "uEnableLighting");

  glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
  glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(context.viewMatrix));
  glUniformMatrix4fv(uProjection, 1, GL_FALSE,
                     glm::value_ptr(context.projectionMatrix));
  glUniform3fv(uCameraPos, 1, glm::value_ptr(context.cameraPosition));
  if (uAmbient >= 0) {
    glUniform4fv(uAmbient, 1, glm::value_ptr(m_ambientColor));
  }
  if (uMaterial >= 0) {
    glUniform4fv(uMaterial, 1, glm::value_ptr(materialColor));
  }
  if (uUseTexture >= 0) {
    glUniform1i(uUseTexture, useTexture ? 1 : 0);
  }
  if (uTexture >= 0) {
    glUniform1i(uTexture, textureUnit);
  }
  if (uUseVertexColor >= 0) {
    glUniform1i(uUseVertexColor, useVertexColor ? 1 : 0);
  }
  if (uEnableLighting >= 0) {
    glUniform1i(uEnableLighting, enableLighting ? 1 : 0);
  }

  const GLint uLightCount =
      glGetUniformLocation(m_basicProgram.id(), "uDirectionalLightCount");
  const GLint uLightDirections =
      glGetUniformLocation(m_basicProgram.id(), "uLightDirections");
  const GLint uLightDiffuse =
      glGetUniformLocation(m_basicProgram.id(), "uLightDiffuse");
  const GLint uLightSpecular =
      glGetUniformLocation(m_basicProgram.id(), "uLightSpecular");
  const GLint uLightEnabled =
      glGetUniformLocation(m_basicProgram.id(), "uLightEnabled");

  const std::size_t clampedSize =
      enableLighting
          ? std::min<std::size_t>(m_directionalLights.size(),
                                  kMaxDirectionalLights)
          : 0;
  const int count = static_cast<int>(clampedSize);

  std::array<glm::vec3, kMaxDirectionalLights> directions{};
  std::array<glm::vec4, kMaxDirectionalLights> diffuses{};
  std::array<glm::vec4, kMaxDirectionalLights> speculars{};
  std::array<GLint, kMaxDirectionalLights> enabled{};

  for (std::size_t i = 0; i < clampedSize; ++i) {
    const auto &light = m_directionalLights[i];
    directions[i] = light.direction;
    diffuses[i] = light.diffuse;
    speculars[i] = light.specular;
    enabled[i] = light.enabled ? 1 : 0;
  }

  if (uLightCount >= 0) {
    glUniform1i(uLightCount, count);
  }
  if (uLightDirections >= 0) {
    glUniform3fv(uLightDirections, kMaxDirectionalLights,
                 reinterpret_cast<const GLfloat *>(directions.data()));
  }
  if (uLightDiffuse >= 0) {
    glUniform4fv(uLightDiffuse, kMaxDirectionalLights,
                 reinterpret_cast<const GLfloat *>(diffuses.data()));
  }
  if (uLightSpecular >= 0) {
    glUniform4fv(uLightSpecular, kMaxDirectionalLights,
                 reinterpret_cast<const GLfloat *>(speculars.data()));
  }
  if (uLightEnabled >= 0) {
    glUniform1iv(uLightEnabled, kMaxDirectionalLights, enabled.data());
  }
}

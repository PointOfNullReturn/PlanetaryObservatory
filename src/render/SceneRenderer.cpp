#include "render/SceneRenderer.h"

#include "render/GlCapabilities.h"
#include "render/GlState.h"
#include "utils/Log.h"
#include "scenegraph/SceneGraph.h"
#include "scenegraph/SceneNode.h"
#include "scenegraph/components/DirectionalLightComponent.h"
#include "scenegraph/components/GlobalLightingComponent.h"
#include "scenegraph/components/AxisComponent.h"
#include "scenegraph/components/MaterialComponent.h"
#include "scenegraph/components/SkyboxComponent.h"
#include "scenegraph/components/SphereMeshComponent.h"
#include "scenegraph/components/TextureLayerComponent.h"

#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <algorithm>
#include <array>
#include <cmath>
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

void SceneRenderer::cacheBasicUniformLocations() {
  if (m_basicUniforms.initialized) {
    return;
  }

  const GLuint programId = m_basicProgram.id();
  m_basicUniforms.model = glGetUniformLocation(programId, "uModel");
  m_basicUniforms.view = glGetUniformLocation(programId, "uView");
  m_basicUniforms.projection = glGetUniformLocation(programId, "uProjection");
  m_basicUniforms.cameraPos = glGetUniformLocation(programId, "uCameraPos");
  m_basicUniforms.ambient = glGetUniformLocation(programId, "uAmbientColor");
  m_basicUniforms.materialDiffuse =
      glGetUniformLocation(programId, "uMaterialDiffuse");
  m_basicUniforms.materialAmbientMix =
      glGetUniformLocation(programId, "uMaterialAmbientMix");
  m_basicUniforms.materialSpecularStrength =
      glGetUniformLocation(programId, "uMaterialSpecularStrength");
  m_basicUniforms.materialShininess =
      glGetUniformLocation(programId, "uMaterialShininess");
  m_basicUniforms.materialExposure =
      glGetUniformLocation(programId, "uMaterialExposure");
  m_basicUniforms.materialGamma =
      glGetUniformLocation(programId, "uMaterialGamma");
  m_basicUniforms.useTexture = glGetUniformLocation(programId, "uUseTexture");
  m_basicUniforms.texture = glGetUniformLocation(programId, "uTexture");
  m_basicUniforms.textureLayerCount =
      glGetUniformLocation(programId, "uTextureLayerCount");
  m_basicUniforms.textureLayers =
      glGetUniformLocation(programId, "uTextureLayers");
  m_basicUniforms.textureBlendModes =
      glGetUniformLocation(programId, "uTextureBlendModes");
  m_basicUniforms.textureBlendFactors =
      glGetUniformLocation(programId, "uTextureBlendFactors");
  m_basicUniforms.useVertexColor =
      glGetUniformLocation(programId, "uUseVertexColor");
  m_basicUniforms.enableLighting =
      glGetUniformLocation(programId, "uEnableLighting");
  m_basicUniforms.normalMatrix =
      glGetUniformLocation(programId, "uNormalMatrix");
  m_basicUniforms.lightCount =
      glGetUniformLocation(programId, "uDirectionalLightCount");
  m_basicUniforms.lightDirections =
      glGetUniformLocation(programId, "uLightDirections");
  m_basicUniforms.lightDiffuse =
      glGetUniformLocation(programId, "uLightDiffuse");
  m_basicUniforms.lightSpecular =
      glGetUniformLocation(programId, "uLightSpecular");
  m_basicUniforms.lightEnabled =
      glGetUniformLocation(programId, "uLightEnabled");
  m_basicUniforms.initialized = true;
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
  MaterialComponent::MaterialProperties materialProperties;
  if (auto *material = node.getComponent<MaterialComponent>()) {
    materialProperties = material->material();
  }

  std::array<GLint, TextureLayerComponent::kMaxLayers> textureUnits{};
  std::array<GLint, TextureLayerComponent::kMaxLayers> blendModes{};
  std::array<float, TextureLayerComponent::kMaxLayers> blendFactors{};
  int layerCount = 0;
  if (textures != nullptr) {
    layerCount = textures->bindForShader(0, textureUnits, blendModes, blendFactors);
  }

  bindShaderUniforms(context, modelMatrix, materialProperties.diffuseColor,
                     materialProperties.specularStrength,
                     materialProperties.shininess,
                     materialProperties.ambientMix, materialProperties.exposure,
                     materialProperties.gamma, layerCount,
                     layerCount > 0 ? textureUnits.data() : nullptr,
                     layerCount > 0 ? blendModes.data() : nullptr,
                     layerCount > 0 ? blendFactors.data() : nullptr,
                     /*useVertexColor=*/false, m_globalLightingEnabled);

  if (mesh.renderMode == RENDER_MODE_WIREFRAME) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  mesh.renderWithShader();

  if (mesh.renderMode == RENDER_MODE_WIREFRAME) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  if (textures != nullptr) {
    textures->unbindFromShader(0, layerCount);
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

  bindShaderUniforms(context, modelMatrix, glm::vec4(1.0f), 0.0f, 1.0f, 1.0f,
                     1.0f, 1.0f,
                     /*textureLayerCount=*/0, nullptr, nullptr, nullptr,
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

void SceneRenderer::bindShaderUniforms(
    const RenderContext &context, const glm::mat4 &modelMatrix,
    const glm::vec4 &materialColor, float specularStrength, float shininess,
    float ambientMix, float exposure, float gamma, int textureLayerCount,
    const GLint *textureUnits, const GLint *textureBlendModes,
    const float *textureBlendFactors, bool useVertexColor, bool enableLighting) {
  m_basicProgram.use();
  cacheBasicUniformLocations();

  if (m_basicUniforms.model >= 0) {
    glUniformMatrix4fv(m_basicUniforms.model, 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));
  }
  if (m_basicUniforms.view >= 0) {
    glUniformMatrix4fv(m_basicUniforms.view, 1, GL_FALSE,
                       glm::value_ptr(context.viewMatrix));
  }
  if (m_basicUniforms.projection >= 0) {
    glUniformMatrix4fv(m_basicUniforms.projection, 1, GL_FALSE,
                     glm::value_ptr(context.projectionMatrix));
  }
  if (m_basicUniforms.cameraPos >= 0) {
    glUniform3fv(m_basicUniforms.cameraPos, 1,
                 glm::value_ptr(context.cameraPosition));
  }
  if (m_basicUniforms.ambient >= 0) {
    glUniform4fv(m_basicUniforms.ambient, 1, glm::value_ptr(m_ambientColor));
  }
  if (m_basicUniforms.materialDiffuse >= 0) {
    glUniform4fv(m_basicUniforms.materialDiffuse, 1,
                 glm::value_ptr(materialColor));
  }
  if (m_basicUniforms.materialAmbientMix >= 0) {
    glUniform1f(m_basicUniforms.materialAmbientMix,
                std::clamp(ambientMix, 0.0f, 1.0f));
  }
  if (m_basicUniforms.materialSpecularStrength >= 0) {
    glUniform1f(m_basicUniforms.materialSpecularStrength,
                std::max(0.0f, specularStrength));
  }
  if (m_basicUniforms.materialShininess >= 0) {
    glUniform1f(m_basicUniforms.materialShininess,
                std::max(1.0f, shininess));
  }
  if (m_basicUniforms.materialExposure >= 0) {
    glUniform1f(m_basicUniforms.materialExposure,
                std::max(0.0f, exposure));
  }
  if (m_basicUniforms.materialGamma >= 0) {
    glUniform1f(m_basicUniforms.materialGamma,
                std::max(0.1f, gamma));
  }
  const bool hasTextureLayers = textureLayerCount > 0 && textureUnits != nullptr &&
                                textureBlendModes != nullptr &&
                                textureBlendFactors != nullptr;

  if (m_basicUniforms.useTexture >= 0) {
    glUniform1i(m_basicUniforms.useTexture, hasTextureLayers ? 1 : 0);
  }
  if (m_basicUniforms.texture >= 0) {
    glUniform1i(m_basicUniforms.texture, hasTextureLayers ? textureUnits[0] : 0);
  }
  if (m_basicUniforms.textureLayerCount >= 0) {
    glUniform1i(m_basicUniforms.textureLayerCount, textureLayerCount);
  }
  if (hasTextureLayers) {
    if (m_basicUniforms.textureLayers >= 0) {
      glUniform1iv(m_basicUniforms.textureLayers, textureLayerCount, textureUnits);
    }
    if (m_basicUniforms.textureBlendModes >= 0) {
      glUniform1iv(m_basicUniforms.textureBlendModes, textureLayerCount,
                   textureBlendModes);
    }
    if (m_basicUniforms.textureBlendFactors >= 0) {
      glUniform1fv(m_basicUniforms.textureBlendFactors, textureLayerCount,
                   textureBlendFactors);
    }
  }
  if (m_basicUniforms.useVertexColor >= 0) {
    glUniform1i(m_basicUniforms.useVertexColor, useVertexColor ? 1 : 0);
  }
  if (m_basicUniforms.enableLighting >= 0) {
    glUniform1i(m_basicUniforms.enableLighting, enableLighting ? 1 : 0);
  }

  glm::mat3 normalMatrix(1.0f);
  const glm::mat3 upperLeft(modelMatrix);
  const float determinant = glm::determinant(upperLeft);
  if (std::abs(determinant) > std::numeric_limits<float>::epsilon()) {
    normalMatrix = glm::transpose(glm::inverse(upperLeft));
  }
  if (m_basicUniforms.normalMatrix >= 0) {
    glUniformMatrix3fv(m_basicUniforms.normalMatrix, 1, GL_FALSE,
                       glm::value_ptr(normalMatrix));
  }

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

  if (m_basicUniforms.lightCount >= 0) {
    glUniform1i(m_basicUniforms.lightCount, count);
  }
  if (m_basicUniforms.lightDirections >= 0) {
    glUniform3fv(m_basicUniforms.lightDirections, kMaxDirectionalLights,
                 reinterpret_cast<const GLfloat *>(directions.data()));
  }
  if (m_basicUniforms.lightDiffuse >= 0) {
    glUniform4fv(m_basicUniforms.lightDiffuse, kMaxDirectionalLights,
                 reinterpret_cast<const GLfloat *>(diffuses.data()));
  }
  if (m_basicUniforms.lightSpecular >= 0) {
    glUniform4fv(m_basicUniforms.lightSpecular, kMaxDirectionalLights,
                 reinterpret_cast<const GLfloat *>(speculars.data()));
  }
  if (m_basicUniforms.lightEnabled >= 0) {
    glUniform1iv(m_basicUniforms.lightEnabled, kMaxDirectionalLights,
                 enabled.data());
  }
}

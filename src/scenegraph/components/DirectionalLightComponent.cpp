#include "scenegraph/components/DirectionalLightComponent.h"

#include "scenegraph/SceneNode.h"

#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <limits>

namespace {
glm::vec3 normalizeOrDefault(const glm::vec3 &vector, const glm::vec3 &fallback) {
  const float length = glm::length(vector);
  if (length <= std::numeric_limits<float>::epsilon()) {
    return fallback;
  }
  return vector / length;
}
} // namespace

void DirectionalLightComponent::onRender(SceneNode &node) {
  if (!m_light.enabled) {
    glDisable(lightId);
    return;
  }

  glEnable(GL_LIGHTING);
  glEnable(lightId);

  const glm::mat4 transform = node.getTransform();
  const glm::mat3 rotation(transform);

  const glm::vec3 localDirection =
      normalizeOrDefault(m_light.direction, glm::vec3(0.0f, 0.0f, -1.0f));
  glm::vec3 worldDirection = normalizeOrDefault(rotation * localDirection,
                                                glm::vec3(0.0f, 0.0f, -1.0f));

  const glm::vec4 glDirection(worldDirection, 0.0f);

  const glm::vec4 diffuse = m_light.diffuseColor * m_light.intensity;
  const glm::vec4 specular = m_light.specularColor * m_light.intensity;

  glLightfv(lightId, GL_POSITION, glm::value_ptr(glDirection));
  glLightfv(lightId, GL_DIFFUSE, glm::value_ptr(diffuse));
  glLightfv(lightId, GL_SPECULAR, glm::value_ptr(specular));
  const glm::vec4 ambient(0.0f, 0.0f, 0.0f, 1.0f);
  glLightfv(lightId, GL_AMBIENT, glm::value_ptr(ambient));
}

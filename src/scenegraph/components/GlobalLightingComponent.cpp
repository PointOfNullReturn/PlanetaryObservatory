#include "scenegraph/components/GlobalLightingComponent.h"

#include "scenegraph/SceneNode.h"
#include "render/GlState.h"

#include <glm/gtc/type_ptr.hpp>

void GlobalLightingComponent::onRender(SceneNode &node) {
  (void)node;

  if (!m_lighting.enableLighting) {
    glstate::enableLighting(false);
    glDisable(ambientLightId);
    return;
  }

  glstate::enableLighting(true);
  glEnable(ambientLightId);

  if (m_lighting.enableNormalization) {
    glstate::enableNormalize(true);
  }

  if (!m_lighting.enableNormalization) {
    glstate::enableNormalize(false);
  }

  glstate::setClearColor(m_lighting.backgroundColor);

  glLightfv(ambientLightId, GL_AMBIENT, glm::value_ptr(m_lighting.ambientColor));

  const glm::vec4 zero(0.0f, 0.0f, 0.0f, 1.0f);
  glLightfv(ambientLightId, GL_DIFFUSE, glm::value_ptr(zero));
  glLightfv(ambientLightId, GL_SPECULAR, glm::value_ptr(zero));
}

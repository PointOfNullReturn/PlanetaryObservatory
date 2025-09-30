#include "scenegraph/components/GlobalLightingComponent.h"

#include "scenegraph/SceneNode.h"

#include <glm/gtc/type_ptr.hpp>

void GlobalLightingComponent::onRender(SceneNode &node) {
  (void)node;

  if (!m_lighting.enableLighting) {
    glDisable(GL_LIGHTING);
    glDisable(ambientLightId);
    return;
  }

  glEnable(GL_LIGHTING);
  glEnable(ambientLightId);

  if (m_lighting.enableNormalization) {
    glEnable(GL_NORMALIZE);
  }

  glClearColor(m_lighting.backgroundColor.r, m_lighting.backgroundColor.g,
               m_lighting.backgroundColor.b, m_lighting.backgroundColor.a);

  glLightfv(ambientLightId, GL_AMBIENT, glm::value_ptr(m_lighting.ambientColor));

  const glm::vec4 zero(0.0f, 0.0f, 0.0f, 1.0f);
  glLightfv(ambientLightId, GL_DIFFUSE, glm::value_ptr(zero));
  glLightfv(ambientLightId, GL_SPECULAR, glm::value_ptr(zero));
}

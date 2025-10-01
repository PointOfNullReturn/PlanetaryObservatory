#ifndef PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_AXISCOMPONENT_H
#define PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_AXISCOMPONENT_H

#include "scenegraph/components/Component.h"
#include "common/EOGL.h"
#include "render/GlCapabilities.h"

#include <glm/glm.hpp>
#include <vector>

class SceneNode;

/// Debug helper that renders XYZ axes at the node origin using modern GL.
class AxisComponent : public Component {
public:
  AxisComponent() = default;
  ~AxisComponent() override;

  void onRender(SceneNode &node) override;

  bool enabled = false;
  GLfloat length = 1.0f;
  GLfloat lineWidth = 2.0f;

  void ensureGeometry(SceneNode &node);
  GLuint vao() const { return m_vao; }
  GLsizei lineVertexCount() const { return m_lineVertexCount; }
  GLsizei triangleVertexCount() const { return m_triangleVertexCount; }
  bool hasGeometry() const {
    const bool hasVertices = (m_lineVertexCount + m_triangleVertexCount) > 0;
    if (!hasVertices) {
      return false;
    }
    if (glSupportsVertexArrayObjects()) {
      return m_vao != 0;
    }
    return m_vbo != 0;
  }
  GLuint vbo() const { return m_vbo; }
  void draw() const;

private:
  struct Vertex {
    glm::vec3 position{0.0f};
    glm::vec4 color{1.0f};
  };

  void updateGeometry(SceneNode &node);
  void upload();
  void destroyBuffers();

  std::vector<Vertex> m_vertices;
  GLuint m_vao = 0;
  GLuint m_vbo = 0;
  GLsizei m_lineVertexCount = 0;
  GLsizei m_triangleVertexCount = 0;
  bool m_dirty = true;
  GLfloat m_cachedLength = -1.0f;
  bool m_cachedEnabled = false;
};

#endif // PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_AXISCOMPONENT_H

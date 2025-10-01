#include "scenegraph/components/AxisComponent.h"

#include "scenegraph/SceneNode.h"
#include "render/GlCapabilities.h"

#include <algorithm>
#include <cstddef>

namespace {
constexpr GLfloat kMinArrowLength = 0.2f;
constexpr GLfloat kMinArrowWidth = 0.1f;
constexpr GLfloat kLengthToArrowLength = 0.2f;
constexpr GLfloat kLengthToArrowWidth = 0.08f;

const glm::vec4 kColorX{1.0f, 0.0f, 0.0f, 1.0f};
const glm::vec4 kColorY{0.0f, 1.0f, 0.0f, 1.0f};
const glm::vec4 kColorZ{0.0f, 0.0f, 1.0f, 1.0f};
} // namespace

AxisComponent::~AxisComponent() { destroyBuffers(); }

void AxisComponent::onRender(SceneNode &node) { ensureGeometry(node); }

void AxisComponent::ensureGeometry(SceneNode &node) {
  const bool stateChanged =
      m_dirty || m_cachedLength != length || m_cachedEnabled != enabled;
  if (!stateChanged) {
    return;
  }

  m_cachedLength = length;
  m_cachedEnabled = enabled;
  updateGeometry(node);
}

void AxisComponent::updateGeometry(SceneNode &node) {
  (void)node;

  m_vertices.clear();
  m_lineVertexCount = 0;
  m_triangleVertexCount = 0;

  if (!enabled) {
    m_dirty = false;
    upload();
    return;
  }

  const GLfloat axisLength = std::max(std::fabs(length), 0.0f);
  if (axisLength <= 0.0f) {
    m_dirty = false;
    upload();
    return;
  }

  const GLfloat arrowLength =
      std::max(axisLength * kLengthToArrowLength, kMinArrowLength);
  const GLfloat arrowWidth =
      std::max(axisLength * kLengthToArrowWidth, kMinArrowWidth);

  auto pushLine = [this](const glm::vec3 &a, const glm::vec3 &b,
                         const glm::vec4 &color) {
    m_vertices.push_back({a, color});
    m_vertices.push_back({b, color});
  };
  auto pushTriangle = [this](const glm::vec3 &a, const glm::vec3 &b,
                             const glm::vec3 &c, const glm::vec4 &color) {
    m_vertices.push_back({a, color});
    m_vertices.push_back({b, color});
    m_vertices.push_back({c, color});
  };

  // Axis lines
  pushLine({-axisLength, 0.0f, 0.0f}, {axisLength, 0.0f, 0.0f}, kColorX);
  pushLine({0.0f, -axisLength, 0.0f}, {0.0f, axisLength, 0.0f}, kColorY);
  pushLine({0.0f, 0.0f, -axisLength}, {0.0f, 0.0f, axisLength}, kColorZ);

  m_lineVertexCount = static_cast<GLsizei>(m_vertices.size());

  // Arrowheads
  const glm::vec3 tipX{axisLength + arrowLength, 0.0f, 0.0f};
  const glm::vec3 bx1{axisLength, arrowWidth, 0.0f};
  const glm::vec3 bx2{axisLength, 0.0f, arrowWidth};
  const glm::vec3 bx3{axisLength, -arrowWidth, 0.0f};
  const glm::vec3 bx4{axisLength, 0.0f, -arrowWidth};
  pushTriangle(tipX, bx1, bx2, kColorX);
  pushTriangle(tipX, bx2, bx3, kColorX);
  pushTriangle(tipX, bx3, bx4, kColorX);
  pushTriangle(tipX, bx4, bx1, kColorX);

  const glm::vec3 tipY{0.0f, axisLength + arrowLength, 0.0f};
  const glm::vec3 by1{arrowWidth, axisLength, 0.0f};
  const glm::vec3 by2{0.0f, axisLength, arrowWidth};
  const glm::vec3 by3{-arrowWidth, axisLength, 0.0f};
  const glm::vec3 by4{0.0f, axisLength, -arrowWidth};
  pushTriangle(tipY, by1, by2, kColorY);
  pushTriangle(tipY, by2, by3, kColorY);
  pushTriangle(tipY, by3, by4, kColorY);
  pushTriangle(tipY, by4, by1, kColorY);

  const glm::vec3 tipZ{0.0f, 0.0f, axisLength + arrowLength};
  const glm::vec3 bz1{arrowWidth, 0.0f, axisLength};
  const glm::vec3 bz2{0.0f, arrowWidth, axisLength};
  const glm::vec3 bz3{-arrowWidth, 0.0f, axisLength};
  const glm::vec3 bz4{0.0f, -arrowWidth, axisLength};
  pushTriangle(tipZ, bz1, bz2, kColorZ);
  pushTriangle(tipZ, bz2, bz3, kColorZ);
  pushTriangle(tipZ, bz3, bz4, kColorZ);
  pushTriangle(tipZ, bz4, bz1, kColorZ);

  m_triangleVertexCount =
      static_cast<GLsizei>(m_vertices.size()) - m_lineVertexCount;

  upload();
  m_dirty = false;
}

void AxisComponent::upload() {
  if (m_vertices.empty()) {
    return;
  }

  const bool supportsVao = glSupportsVertexArrayObjects();

  if (supportsVao && m_vao == 0) {
    glGenVertexArrays(1, &m_vao);
  }
  if (m_vbo == 0) {
    glGenBuffers(1, &m_vbo);
  }

  if (supportsVao && m_vao != 0) {
    glBindVertexArray(m_vao);
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
               m_vertices.data(), GL_DYNAMIC_DRAW);

  if (supportsVao && m_vao != 0) {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<const void *>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<const void *>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void AxisComponent::draw() const {
  if (glSupportsVertexArrayObjects() && m_vao != 0) {
    glBindVertexArray(m_vao);
    if (m_lineVertexCount > 0) {
      glDrawArrays(GL_LINES, 0, m_lineVertexCount);
    }
    if (m_triangleVertexCount > 0) {
      glDrawArrays(GL_TRIANGLES, m_lineVertexCount, m_triangleVertexCount);
    }
    glBindVertexArray(0);
    return;
  }

  if (m_vbo == 0) {
    return;
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<const void *>(offsetof(Vertex, position)));
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<const void *>(offsetof(Vertex, color)));

  if (m_lineVertexCount > 0) {
    glDrawArrays(GL_LINES, 0, m_lineVertexCount);
  }
  if (m_triangleVertexCount > 0) {
    glDrawArrays(GL_TRIANGLES, m_lineVertexCount, m_triangleVertexCount);
  }

  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void AxisComponent::destroyBuffers() {
  if (glSupportsVertexArrayObjects() && m_vao != 0) {
    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;
  }
  if (m_vbo != 0) {
    glDeleteBuffers(1, &m_vbo);
    m_vbo = 0;
  }
}

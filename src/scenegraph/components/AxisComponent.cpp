#include "scenegraph/components/AxisComponent.h"
#include "scenegraph/SceneNode.h"

#include <algorithm>
#include <cmath>

namespace {

constexpr GLfloat kMinArrowLength = 0.2f;
constexpr GLfloat kMinArrowWidth = 0.1f;
constexpr GLfloat kLengthToArrowLength = 0.2f;
constexpr GLfloat kLengthToArrowWidth = 0.08f;

void drawXAxisArrow(GLfloat axisLength, GLfloat arrowLength, GLfloat arrowWidth) {
  const GLfloat baseX = axisLength;
  const GLfloat tipX = axisLength + arrowLength;

  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(tipX, 0.0f, 0.0f);
  glVertex3f(baseX, arrowWidth, 0.0f);
  glVertex3f(baseX, 0.0f, arrowWidth);
  glVertex3f(baseX, -arrowWidth, 0.0f);
  glVertex3f(baseX, 0.0f, -arrowWidth);
  glVertex3f(baseX, arrowWidth, 0.0f);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3f(tipX, 0.0f, 0.0f);
  glVertex3f(baseX, arrowWidth, 0.0f);
  glVertex3f(baseX, 0.0f, arrowWidth);
  glVertex3f(baseX, -arrowWidth, 0.0f);
  glVertex3f(baseX, 0.0f, -arrowWidth);
  glEnd();
}

void drawYAxisArrow(GLfloat axisLength, GLfloat arrowLength, GLfloat arrowWidth) {
  const GLfloat baseY = axisLength;
  const GLfloat tipY = axisLength + arrowLength;

  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.0f, tipY, 0.0f);
  glVertex3f(arrowWidth, baseY, 0.0f);
  glVertex3f(0.0f, baseY, arrowWidth);
  glVertex3f(-arrowWidth, baseY, 0.0f);
  glVertex3f(0.0f, baseY, -arrowWidth);
  glVertex3f(arrowWidth, baseY, 0.0f);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3f(0.0f, tipY, 0.0f);
  glVertex3f(arrowWidth, baseY, 0.0f);
  glVertex3f(0.0f, baseY, arrowWidth);
  glVertex3f(-arrowWidth, baseY, 0.0f);
  glVertex3f(0.0f, baseY, -arrowWidth);
  glEnd();
}

void drawZAxisArrow(GLfloat axisLength, GLfloat arrowLength, GLfloat arrowWidth) {
  const GLfloat baseZ = axisLength;
  const GLfloat tipZ = axisLength + arrowLength;

  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.0f, 0.0f, tipZ);
  glVertex3f(arrowWidth, 0.0f, baseZ);
  glVertex3f(0.0f, arrowWidth, baseZ);
  glVertex3f(-arrowWidth, 0.0f, baseZ);
  glVertex3f(0.0f, -arrowWidth, baseZ);
  glVertex3f(arrowWidth, 0.0f, baseZ);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3f(0.0f, 0.0f, tipZ);
  glVertex3f(arrowWidth, 0.0f, baseZ);
  glVertex3f(0.0f, arrowWidth, baseZ);
  glVertex3f(-arrowWidth, 0.0f, baseZ);
  glVertex3f(0.0f, -arrowWidth, baseZ);
  glEnd();
}

} // namespace

void AxisComponent::onRender(SceneNode &node) {
  if (!enabled) {
    return;
  }

  const GLfloat axisLength = std::max(std::fabs(length), 0.0f);
  if (axisLength <= 0.0f) {
    return;
  }

  glPushMatrix();

  const glm::mat4 transform = node.getTransform();
  glMultMatrixf(glm::value_ptr(transform));

  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LINE_BIT);
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glLineWidth(lineWidth);

  glBegin(GL_LINES);

  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
  glVertex3f(-axisLength, 0.0f, 0.0f);
  glVertex3f(axisLength, 0.0f, 0.0f);

  glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, -axisLength, 0.0f);
  glVertex3f(0.0f, axisLength, 0.0f);

  glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, -axisLength);
  glVertex3f(0.0f, 0.0f, axisLength);

  glEnd();

  const GLfloat arrowLength =
      std::max(axisLength * kLengthToArrowLength, kMinArrowLength);
  const GLfloat arrowWidth =
      std::max(axisLength * kLengthToArrowWidth, kMinArrowWidth);

  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
  drawXAxisArrow(axisLength, arrowLength, arrowWidth);

  glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
  drawYAxisArrow(axisLength, arrowLength, arrowWidth);

  glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
  drawZAxisArrow(axisLength, arrowLength, arrowWidth);

  glPopAttrib();

  glPopMatrix();
}

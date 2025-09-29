#include "scenegraph/components/AxisComponent.h"
#include "scenegraph/SceneNode.h"

void AxisComponent::onRender(SceneNode &node)
{
    if (!enabled) {
        return;
    }

    glPushMatrix();

    glm::mat4 transform = node.getTransform();
    glMultMatrixf(glm::value_ptr(transform));

    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LINE_BIT);
    glDisable(GL_LIGHTING);
    glLineWidth(lineWidth);

    glBegin(GL_LINES);

    // X axis (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(length, 0.0f, 0.0f);

    // Y axis (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, length, 0.0f);

    // Z axis (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, length);

    glEnd();

    glPopAttrib();

    glPopMatrix();
}

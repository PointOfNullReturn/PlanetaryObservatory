#include "scenegraph/components/SphereMeshComponent.h"
#include "scenegraph/SceneNode.h"
#include <glm/gtc/type_ptr.hpp>
#include "utils/Log.h"
#include <string>

void SphereMeshComponent::onRender(SceneNode &node)
{
    if (!quadric) {
        quadric.reset(gluNewQuadric());
        if (quadric) {
            gluQuadricNormals(quadric.get(), GLU_SMOOTH);
        }
    }

    if (!quadric) {
        return;
    }

    glPushMatrix();

    glm::mat4 transform = node.getTransform();
    if (Log::kDebugLoggingEnabled) {
        Log::debug("Rendering node: " + node.name());
    }
    glMultMatrixf(glm::value_ptr(transform));

    if (renderMode == RENDER_MODE_WIREFRAME)
    {
        gluQuadricTexture(quadric.get(), GL_TRUE);
        gluQuadricDrawStyle(quadric.get(), GLU_LINE);
        gluSphere(quadric.get(), radius, slices, stacks);
        gluQuadricDrawStyle(quadric.get(), GLU_FILL);
    }
    else
    {
        gluQuadricTexture(quadric.get(), GL_TRUE);
        gluQuadricDrawStyle(quadric.get(), GLU_FILL);
        gluSphere(quadric.get(), radius, slices, stacks);
    }

    glPopMatrix();
}
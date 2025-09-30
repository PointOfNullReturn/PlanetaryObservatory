#include "scenegraph/components/SphereMeshComponent.h"
#include "scenegraph/SceneNode.h"
#include "render/MeshBuilder.h"
#include <glm/gtc/type_ptr.hpp>
#include "utils/Log.h"
#include <string>

void SphereMeshComponent::onRender(SceneNode &node)
{
    if (!quadric) {
        quadric = MeshBuilder::createSphere();
    }

    if (!quadric || !quadric->handle()) {
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
        gluQuadricDrawStyle(quadric->handle(), GLU_LINE);
        gluSphere(quadric->handle(), radius, slices, stacks);
        gluQuadricDrawStyle(quadric->handle(), GLU_FILL);
    }
    else
    {
        gluQuadricDrawStyle(quadric->handle(), GLU_FILL);
        gluSphere(quadric->handle(), radius, slices, stacks);
    }

    glPopMatrix();
}

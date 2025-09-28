#include "scenegraph/components/SphereMeshComponent.h"
#include "scenegraph/SceneNode.h"
#include <glm/gtc/type_ptr.hpp>
#include "utils/Log.h"
#include <string>

void SphereMeshComponent::onRender(SceneNode &node)
{
    if (Log::kDebugLoggingEnabled) {
        Log::debug("Rendering node: " + node.name());
    }
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
        Log::debug("Transform matrix:");
        for (int i = 0; i < 4; ++i) {
            Log::debug(std::to_string(transform[i][0]) + " " + std::to_string(transform[i][1]) + " " + std::to_string(transform[i][2]) + " " + std::to_string(transform[i][3]));
        }
    }
    glMultMatrixf(glm::value_ptr(transform));

    const bool hasTexture = texture > 0;

    if (renderMode == RENDER_MODE_WIREFRAME)
    {
        gluQuadricTexture(quadric.get(), hasTexture ? GL_TRUE : GL_FALSE);
        gluQuadricDrawStyle(quadric.get(), GLU_LINE);

        if (hasTexture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
        }

        gluSphere(quadric.get(), radius, slices, stacks);

        if (hasTexture)
        {
            glDisable(GL_TEXTURE_2D);
        }

        gluQuadricDrawStyle(quadric.get(), GLU_FILL);
    }
    else
    {
        gluQuadricTexture(quadric.get(), hasTexture ? GL_TRUE : GL_FALSE);
        gluQuadricDrawStyle(quadric.get(), GLU_FILL);

        if (hasTexture)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
        }

        gluSphere(quadric.get(), radius, slices, stacks);

        if (hasTexture)
        {
            glDisable(GL_TEXTURE_2D);
        }
    }

    glPopMatrix();
}

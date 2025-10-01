#include "scenegraph/components/TextureLayerComponent.h"
#include "scenegraph/SceneNode.h"

void TextureLayerComponent::onRender(SceneNode &node)
{
    (void)node;
    // Rendering handled by SceneRenderer's shader path.
}

bool TextureLayerComponent::bindForShader(GLuint textureUnit) const
{
    if (layers.empty() || layers.front().textureId == 0) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        return false;
    }

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, layers.front().textureId);
    glActiveTexture(GL_TEXTURE0);
    return true;
}

void TextureLayerComponent::unbindFromShader(GLuint textureUnit) const
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

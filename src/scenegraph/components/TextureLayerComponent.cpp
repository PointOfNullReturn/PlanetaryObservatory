#include "scenegraph/components/TextureLayerComponent.h"
#include "scenegraph/SceneNode.h"

void TextureLayerComponent::onRender(SceneNode &node)
{
    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    // Iterate through layers and apply textures/blending
    for (const auto& layer : layers) {
        if (layer.textureId > 0) {
            glBindTexture(GL_TEXTURE_2D, layer.textureId);

            // Apply blending based on mode
            switch (layer.blendMode) {
                case TextureBlendMode::None:
                    // No blending, just replace (default behavior)
                    break;
                case TextureBlendMode::Multiply:
                    // Example: glBlendFunc(GL_DST_COLOR, GL_ZERO); // Multiplies current fragment by incoming fragment
                    // This is more complex with fixed function, usually done with glTexEnv
                    // For simplicity, we'll assume basic alpha blending for now or rely on texture environment
                    break;
                case TextureBlendMode::Add:
                    // Example: glBlendFunc(GL_ONE, GL_ONE); // Adds colors
                    break;
                case TextureBlendMode::Alpha:
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    // For fixed function, alpha value might be part of the texture or vertex color
                    break;
            }
        }
    }

    // Disable texturing after all layers are processed
    glDisable(GL_TEXTURE_2D);
}

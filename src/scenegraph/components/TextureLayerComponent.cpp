#include "scenegraph/components/TextureLayerComponent.h"
#include "scenegraph/SceneNode.h"
#include "render/GlState.h"

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
                    // Example placeholder: glBlendFunc(GL_DST_COLOR, GL_ZERO);
                    glstate::enableBlend(true, GL_DST_COLOR, GL_ZERO);
                    break;
                case TextureBlendMode::Add:
                    glstate::enableBlend(true, GL_ONE, GL_ONE);
                    break;
                case TextureBlendMode::Alpha:
                    glstate::enableBlend(true);
                    break;
            }
        }
    }

    glstate::enableBlend(false);
}

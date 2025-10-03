#include "scenegraph/components/TextureLayerComponent.h"
#include "scenegraph/SceneNode.h"

#include <algorithm>

void TextureLayerComponent::onRender(SceneNode &node) {
    (void)node;
    // Rendering handled by SceneRenderer's shader path.
}

int TextureLayerComponent::bindForShader(
    GLuint baseTextureUnit, std::array<GLint, kMaxLayers> &textureUnits,
    std::array<GLint, kMaxLayers> &blendModes,
    std::array<float, kMaxLayers> &blendFactors) const {

    std::fill(textureUnits.begin(), textureUnits.end(), 0);
    std::fill(blendModes.begin(), blendModes.end(), 0);
    std::fill(blendFactors.begin(), blendFactors.end(), 0.0f);

    const std::size_t availableLayers = std::min(layers.size(), kMaxLayers);
    int activeLayers = 0;

    for (std::size_t index = 0; index < availableLayers; ++index) {
        const auto &layer = layers[index];
        if (layer.textureId == 0) {
            continue;
        }

        const GLuint unit = baseTextureUnit + static_cast<GLuint>(activeLayers);
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, layer.textureId);

        textureUnits[activeLayers] = static_cast<GLint>(unit);
        blendModes[activeLayers] = static_cast<GLint>(layer.blendMode);
        blendFactors[activeLayers] = layer.blendFactor;
        ++activeLayers;
    }

    glActiveTexture(GL_TEXTURE0);
    return activeLayers;
}

void TextureLayerComponent::unbindFromShader(GLuint baseTextureUnit,
                                             int layerCount) const {
    for (int i = 0; i < layerCount; ++i) {
        const GLuint unit = baseTextureUnit + static_cast<GLuint>(i);
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glActiveTexture(GL_TEXTURE0);
}

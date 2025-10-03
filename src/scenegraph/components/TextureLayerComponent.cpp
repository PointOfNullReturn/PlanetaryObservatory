#include "scenegraph/components/TextureLayerComponent.h"
#include "scenegraph/SceneNode.h"

#include <algorithm>
#include <cmath>
#include <glm/common.hpp>
#include <glm/gtc/constants.hpp>

void TextureLayerComponent::onUpdate(SceneNode &node, double deltaSeconds) {
    (void)node;
    const std::size_t layerCount = std::min(layers.size(), kMaxLayers);
    for (std::size_t i = 0; i < layerCount; ++i) {
        const auto &layer = layers[i];
        auto &state = m_animationStates[i];

        if (layer.animateRotation) {
            state.rotationRadians += glm::radians(layer.rotationSpeedDegPerSec) * static_cast<float>(deltaSeconds);
            state.rotationRadians = std::fmod(state.rotationRadians, glm::two_pi<float>());
            if (state.rotationRadians < 0.0f) {
                state.rotationRadians += glm::two_pi<float>();
            }
        } else {
            state.rotationRadians = 0.0f;
        }

        if (layer.animateScroll) {
            state.scroll += layer.scrollSpeed * static_cast<float>(deltaSeconds);
            state.scroll = glm::mod(state.scroll, glm::vec2(1.0f));
        } else {
            state.scroll = glm::vec2(0.0f);
        }
    }
}

void TextureLayerComponent::onRender(SceneNode &node) {
    (void)node;
    // Rendering handled by SceneRenderer's shader path.
}

int TextureLayerComponent::bindForShader(
    GLuint baseTextureUnit, std::array<GLint, kMaxLayers> &textureUnits,
    std::array<GLint, kMaxLayers> &blendModes,
    std::array<float, kMaxLayers> &blendFactors,
    std::array<TextureAnimationState, kMaxLayers> &animStates) const {

    std::fill(textureUnits.begin(), textureUnits.end(), 0);
    std::fill(blendModes.begin(), blendModes.end(), 0);
    std::fill(blendFactors.begin(), blendFactors.end(), 0.0f);
    std::fill(animStates.begin(), animStates.end(), TextureAnimationState{});

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

        TextureAnimationState finalState{};
        finalState.rotationRadians = m_animationStates[index].rotationRadians +
                                     glm::radians(layer.rotationOffsetDeg);
        finalState.rotationRadians = std::fmod(finalState.rotationRadians, glm::two_pi<float>());
        if (finalState.rotationRadians < 0.0f) {
            finalState.rotationRadians += glm::two_pi<float>();
        }
        finalState.scroll = glm::mod(m_animationStates[index].scroll + layer.scrollOffset, glm::vec2(1.0f));

        animStates[activeLayers] = finalState;
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

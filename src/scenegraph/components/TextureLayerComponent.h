#ifndef PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_TEXTURELAYERCOMPONENT_H
#define PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_TEXTURELAYERCOMPONENT_H

#include "scenegraph/components/Component.h"
#include "common/EOGL.h"

#include <array>
#include <vector>
#include <glm/vec2.hpp>

enum class TextureBlendMode {
    None,       // No blending, just replace
    Multiply,   // Multiply colors
    Add,        // Add colors
    Alpha,      // Alpha blending
};

struct TextureLayer {
    GLuint textureId = 0;
    TextureBlendMode blendMode = TextureBlendMode::None;
    float blendFactor = 1.0f; // For alpha blending or general intensity
    bool animateRotation = false;
    float rotationSpeedDegPerSec = 0.0f;
    float rotationOffsetDeg = 0.0f;
    bool animateScroll = false;
    glm::vec2 scrollSpeed = glm::vec2(0.0f);
    glm::vec2 scrollOffset = glm::vec2(0.0f);
};

struct TextureAnimationState {
    float rotationRadians = 0.0f;
    glm::vec2 scroll = glm::vec2(0.0f);
};

class TextureLayerComponent : public Component {
public:
    static constexpr std::size_t kMaxLayers = 4;
    std::vector<TextureLayer> layers;

    TextureLayerComponent() = default;

    void onUpdate(SceneNode &node, double deltaSeconds) override;
    void onRender(SceneNode &node) override;

    int bindForShader(GLuint baseTextureUnit,
                      std::array<GLint, kMaxLayers> &textureUnits,
                      std::array<GLint, kMaxLayers> &blendModes,
                      std::array<float, kMaxLayers> &blendFactors,
                      std::array<TextureAnimationState, kMaxLayers> &animStates) const;
    void unbindFromShader(GLuint baseTextureUnit, int layerCount) const;

private:
    mutable std::array<TextureAnimationState, kMaxLayers> m_animationStates{};
};

#endif //PLANETARYOBSERVATORY_SCENEGRAPH_COMPONENTS_TEXTURELAYERCOMPONENT_H

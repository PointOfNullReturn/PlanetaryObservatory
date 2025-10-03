#ifndef PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_TEXTURELAYERCOMPONENT_H
#define PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_TEXTURELAYERCOMPONENT_H

#include "scenegraph/components/Component.h"
#include "common/EOGL.h"

#include <array>
#include <vector>

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
};

class TextureLayerComponent : public Component {
public:
    static constexpr std::size_t kMaxLayers = 4;
    std::vector<TextureLayer> layers;

    TextureLayerComponent() = default;

    void onRender(SceneNode &node) override;

    int bindForShader(GLuint baseTextureUnit,
                      std::array<GLint, kMaxLayers> &textureUnits,
                      std::array<GLint, kMaxLayers> &blendModes,
                      std::array<float, kMaxLayers> &blendFactors) const;
    void unbindFromShader(GLuint baseTextureUnit, int layerCount) const;
};

#endif //PLANETARYOBSERVATORY_SCENEGRAPH_COMPONENTS_TEXTURELAYERCOMPONENT_H

#ifndef PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_TEXTURELAYERCOMPONENT_H
#define PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_TEXTURELAYERCOMPONENT_H

#include "scenegraph/components/Component.h"
#include "common/EOGL.h"

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
    std::vector<TextureLayer> layers;

    TextureLayerComponent() = default;

    void onRender(SceneNode &node) override;

    bool bindForShader(GLuint textureUnit = 0) const;
    void unbindFromShader(GLuint textureUnit = 0) const;
};

#endif //PLANETARYOBSERVATORY_SCENEGRAPH_COMPONENTS_TEXTURELAYERCOMPONENT_H

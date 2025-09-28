#ifndef PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_SPHEREMESHCOMPONENT_H
#define PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_SPHEREMESHCOMPONENT_H

#include "scenegraph/components/Component.h"
#include "common/EOGL.h"
#include "common/EOGlobalEnums.h"
#include <memory>

class SphereMeshComponent : public Component {
public:
    GLdouble radius = 1.0;
    GLint slices = 64;
    GLint stacks = 64;
    RenderModes renderMode = RENDER_MODE_NORMAL;

    SphereMeshComponent() = default;

    void onRender(SceneNode &node) override;

private:
    using QuadricPtr = std::unique_ptr<GLUquadric, void (*)(GLUquadric*)>;
    QuadricPtr quadric{nullptr, gluDeleteQuadric};
};

#endif //PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_SPHEREMESHCOMPONENT_H

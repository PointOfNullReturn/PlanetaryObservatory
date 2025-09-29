#ifndef PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_AXISCOMPONENT_H
#define PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_AXISCOMPONENT_H

#include "scenegraph/components/Component.h"
#include "common/EOGL.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class AxisComponent : public Component {
public:
    GLfloat length = 1.0f;
    GLfloat lineWidth = 2.0f;
    bool enabled = false;

    AxisComponent() = default;

    void onRender(SceneNode &node) override;
};

#endif //PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_AXISCOMPONENT_H

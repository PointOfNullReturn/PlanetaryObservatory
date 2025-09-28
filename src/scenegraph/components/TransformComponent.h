#ifndef PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_TRANSFORMCOMPONENT_H
#define PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_TRANSFORMCOMPONENT_H

#include "scenegraph/components/Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TransformComponent : public Component {
public:
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    TransformComponent() = default;

    [[nodiscard]] glm::mat4 getTransform() const {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, position);
        transform = glm::rotate(transform, glm::radians(rotation.x), {1.0f, 0.0f, 0.0f});
        transform = glm::rotate(transform, glm::radians(rotation.y), {0.0f, 1.0f, 0.0f});
        transform = glm::rotate(transform, glm::radians(rotation.z), {0.0f, 0.0f, 1.0f});
        transform = glm::scale(transform, scale);
        return transform;
    }
};

#endif //PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_TRANSFORMCOMPONENT_H

#ifndef PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_SPHEREMESHCOMPONENT_H
#define PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_SPHEREMESHCOMPONENT_H

#include "scenegraph/components/Component.h"
#include "common/EOGL.h"
#include "common/EOGlobalEnums.h"
#include "render/MeshBuilder.h"
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class SphereMeshComponent : public Component {
public:
    GLdouble radius = 1.0;
    GLint slices = 64;
    GLint stacks = 64;
    RenderModes renderMode = RENDER_MODE_NORMAL;

    SphereMeshComponent() = default;
    ~SphereMeshComponent() override;

    void onRender(SceneNode &node) override;
    void renderWithShader();
    GLuint vao() const { return m_vao; }
    GLsizei indexCount() const { return static_cast<GLsizei>(m_indices.size()); }

private:
    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_texCoords;
    std::vector<unsigned int> m_indices;
    GLuint m_vao = 0;
    GLuint m_vboPositions = 0;
    GLuint m_vboNormals = 0;
    GLuint m_vboTexCoords = 0;
    GLuint m_ebo = 0;
    bool m_buffersDirty = true;

    void updateMeshIfNeeded();
    void uploadBuffers();
    void destroyBuffers();
};

#endif //PLANETARY_OBSERVATORY_SCENEGRAPH_COMPONENTS_SPHEREMESHCOMPONENT_H

#include "scenegraph/components/SphereMeshComponent.h"

#include "render/GlCapabilities.h"
#include "render/MeshBuilder.h"
#include "scenegraph/SceneNode.h"
#include "utils/Log.h"

#include <glm/gtc/type_ptr.hpp>
#include <utility>

SphereMeshComponent::~SphereMeshComponent() { destroyBuffers(); }

void SphereMeshComponent::onRender(SceneNode &node) {
    (void)node;
    updateMeshIfNeeded();
}

void SphereMeshComponent::renderWithShader() {
    updateMeshIfNeeded();
    if (m_indices.empty()) {
        return;
    }

    if (glSupportsVertexArrayObjects() && m_vao != 0) {
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, indexCount(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        return;
    }

    if (m_vboPositions == 0 || m_vboNormals == 0 || m_vboTexCoords == 0 ||
        m_ebo == 0) {
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoords);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glDrawElements(GL_TRIANGLES, indexCount(), GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SphereMeshComponent::updateMeshIfNeeded() {
    if (!m_buffersDirty) {
        return;
    }

    MeshData mesh = buildSphere(static_cast<float>(radius), slices, stacks);
    m_positions = std::move(mesh.positions);
    m_normals = std::move(mesh.normals);
    m_texCoords = std::move(mesh.texCoords);
    m_indices = std::move(mesh.indices);
    uploadBuffers();
    m_buffersDirty = false;
}

void SphereMeshComponent::uploadBuffers() {
    const bool supportsVao = glSupportsVertexArrayObjects();

    if (supportsVao && m_vao == 0) {
        glGenVertexArrays(1, &m_vao);
    }
    if (m_vboPositions == 0) {
        glGenBuffers(1, &m_vboPositions);
    }
    if (m_vboNormals == 0) {
        glGenBuffers(1, &m_vboNormals);
    }
    if (m_vboTexCoords == 0) {
        glGenBuffers(1, &m_vboTexCoords);
    }
    if (m_ebo == 0) {
        glGenBuffers(1, &m_ebo);
    }

    if (supportsVao) {
        glBindVertexArray(m_vao);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vboPositions);
    glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);
    if (supportsVao) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals);
    glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW);
    if (supportsVao) {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vboTexCoords);
    glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW);
    if (supportsVao) {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

    if (supportsVao) {
        glBindVertexArray(0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SphereMeshComponent::destroyBuffers() {
    if (glSupportsVertexArrayObjects() && m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    if (m_vboPositions != 0) {
        glDeleteBuffers(1, &m_vboPositions);
        m_vboPositions = 0;
    }
    if (m_vboNormals != 0) {
        glDeleteBuffers(1, &m_vboNormals);
        m_vboNormals = 0;
    }
    if (m_vboTexCoords != 0) {
        glDeleteBuffers(1, &m_vboTexCoords);
        m_vboTexCoords = 0;
    }
    if (m_ebo != 0) {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }
}

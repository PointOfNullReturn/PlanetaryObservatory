#include "render/Skybox.h"

#include "render/GlCapabilities.h"
#include "render/TextureLoader.h"
#include "utils/Log.h"

#include <array>
#include <cstddef>
#include <string>

namespace {
constexpr GLfloat kSkyboxSize = 40.0f;

struct SkyboxVertex {
  GLfloat position[3];
  GLfloat texCoord[3];
};

constexpr SkyboxVertex kSkyboxVertices[] = {
    // +X
    {{kSkyboxSize, -kSkyboxSize, -kSkyboxSize}, {1.0f, -1.0f, -1.0f}},
    {{kSkyboxSize, -kSkyboxSize, kSkyboxSize}, {1.0f, -1.0f, 1.0f}},
    {{kSkyboxSize, kSkyboxSize, kSkyboxSize}, {1.0f, 1.0f, 1.0f}},
    {{kSkyboxSize, kSkyboxSize, -kSkyboxSize}, {1.0f, 1.0f, -1.0f}},
    // -X
    {{-kSkyboxSize, -kSkyboxSize, kSkyboxSize}, {-1.0f, -1.0f, 1.0f}},
    {{-kSkyboxSize, -kSkyboxSize, -kSkyboxSize}, {-1.0f, -1.0f, -1.0f}},
    {{-kSkyboxSize, kSkyboxSize, -kSkyboxSize}, {-1.0f, 1.0f, -1.0f}},
    {{-kSkyboxSize, kSkyboxSize, kSkyboxSize}, {-1.0f, 1.0f, 1.0f}},
    // +Y
    {{-kSkyboxSize, kSkyboxSize, -kSkyboxSize}, {-1.0f, 1.0f, -1.0f}},
    {{-kSkyboxSize, kSkyboxSize, kSkyboxSize}, {-1.0f, 1.0f, 1.0f}},
    {{kSkyboxSize, kSkyboxSize, kSkyboxSize}, {1.0f, 1.0f, 1.0f}},
    {{kSkyboxSize, kSkyboxSize, -kSkyboxSize}, {1.0f, 1.0f, -1.0f}},
    // -Y
    {{-kSkyboxSize, -kSkyboxSize, kSkyboxSize}, {-1.0f, -1.0f, 1.0f}},
    {{kSkyboxSize, -kSkyboxSize, kSkyboxSize}, {1.0f, -1.0f, 1.0f}},
    {{kSkyboxSize, -kSkyboxSize, -kSkyboxSize}, {1.0f, -1.0f, -1.0f}},
    {{-kSkyboxSize, -kSkyboxSize, -kSkyboxSize}, {-1.0f, -1.0f, -1.0f}},
    // +Z
    {{-kSkyboxSize, -kSkyboxSize, kSkyboxSize}, {-1.0f, -1.0f, 1.0f}},
    {{kSkyboxSize, -kSkyboxSize, kSkyboxSize}, {1.0f, -1.0f, 1.0f}},
    {{kSkyboxSize, kSkyboxSize, kSkyboxSize}, {1.0f, 1.0f, 1.0f}},
    {{-kSkyboxSize, kSkyboxSize, kSkyboxSize}, {-1.0f, 1.0f, 1.0f}},
    // -Z
    {{kSkyboxSize, -kSkyboxSize, -kSkyboxSize}, {1.0f, -1.0f, -1.0f}},
    {{-kSkyboxSize, -kSkyboxSize, -kSkyboxSize}, {-1.0f, -1.0f, -1.0f}},
    {{-kSkyboxSize, kSkyboxSize, -kSkyboxSize}, {-1.0f, 1.0f, -1.0f}},
    {{kSkyboxSize, kSkyboxSize, -kSkyboxSize}, {1.0f, 1.0f, -1.0f}},
};

constexpr unsigned short kSkyboxIndices[] = {
    0, 1, 2, 0, 2, 3,       // +X
    4, 5, 6, 4, 6, 7,       // -X
    8, 9, 10, 8, 10, 11,    // +Y
    12, 13, 14, 12, 14, 15, // -Y
    16, 17, 18, 16, 18, 19, // +Z
    20, 21, 22, 20, 22, 23  // -Z
};
}

Skybox::Skybox() {
  const auto faces = defaultFacePaths();
  m_textureId = LoadCubemap(faces);
  if (m_textureId == 0) {
    Log::warn("Skybox cubemap failed to load; rendering will continue without "
              "a backdrop.");
  } else {
    Log::info("Skybox cubemap loaded");
  }
  initializeBuffers();
}

Skybox::~Skybox() {
  destroyBuffers();
  if (m_textureId != 0) {
    glDeleteTextures(1, &m_textureId);
    m_textureId = 0;
  }
}

Skybox::Skybox(Skybox &&other) noexcept {
  *this = std::move(other);
}

Skybox &Skybox::operator=(Skybox &&other) noexcept {
  if (this == &other) {
    return *this;
  }

  destroyBuffers();
  if (m_textureId != 0) {
    glDeleteTextures(1, &m_textureId);
  }

  m_textureId = other.m_textureId;
  m_vao = other.m_vao;
  m_vbo = other.m_vbo;
  m_ebo = other.m_ebo;
  m_indexCount = other.m_indexCount;
  m_useVertexArray = other.m_useVertexArray;

  other.m_textureId = 0;
  other.m_vao = 0;
  other.m_vbo = 0;
  other.m_ebo = 0;
  other.m_indexCount = 0;
  other.m_useVertexArray = false;

  return *this;
}

bool Skybox::isLoaded() const { return m_textureId != 0; }

void Skybox::initializeBuffers() {
  m_indexCount = static_cast<GLsizei>(std::size(kSkyboxIndices));

  m_useVertexArray = glSupportsVertexArrayObjects();

  if (m_useVertexArray && m_vao == 0) {
    glGenVertexArrays(1, &m_vao);
  }
  if (m_vbo == 0) {
    glGenBuffers(1, &m_vbo);
  }
  if (m_ebo == 0) {
    glGenBuffers(1, &m_ebo);
  }

  if (m_useVertexArray && m_vao != 0) {
    glBindVertexArray(m_vao);
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(kSkyboxVertices), kSkyboxVertices,
               GL_STATIC_DRAW);

  if (m_useVertexArray && m_vao != 0) {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkyboxVertex),
                          reinterpret_cast<const void *>(offsetof(SkyboxVertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SkyboxVertex),
                          reinterpret_cast<const void *>(offsetof(SkyboxVertex, texCoord)));
    glEnableVertexAttribArray(1);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kSkyboxIndices),
               kSkyboxIndices, GL_STATIC_DRAW);

  if (m_useVertexArray && m_vao != 0) {
    glBindVertexArray(0);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Skybox::destroyBuffers() {
  if (m_useVertexArray && m_vao != 0) {
    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;
  }
  if (m_vbo != 0) {
    glDeleteBuffers(1, &m_vbo);
    m_vbo = 0;
  }
  if (m_ebo != 0) {
    glDeleteBuffers(1, &m_ebo);
    m_ebo = 0;
  }
  m_useVertexArray = false;
}
std::array<std::string, 6> Skybox::defaultFacePaths() {
  return {"assets/textures/skybox/space_right.ppm",
          "assets/textures/skybox/space_left.ppm",
          "assets/textures/skybox/space_top.ppm",
          "assets/textures/skybox/space_bottom.ppm",
          "assets/textures/skybox/space_front.ppm",
          "assets/textures/skybox/space_back.ppm"};
}

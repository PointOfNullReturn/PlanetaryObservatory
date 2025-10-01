#pragma once

#include "common/EOGL.h"

#include <array>
#include <string>

/// Represents the background skybox shown behind the planetary scene.
class Skybox {
public:
  Skybox();
  ~Skybox();

  Skybox(const Skybox &) = delete;
  Skybox &operator=(const Skybox &) = delete;
  Skybox(Skybox &&other) noexcept;
  Skybox &operator=(Skybox &&other) noexcept;

  /// Returns whether the skybox cubemap was successfully loaded.
  bool isLoaded() const;

  GLuint textureId() const { return m_textureId; }
  GLuint vao() const { return m_vao; }
  GLuint vbo() const { return m_vbo; }
  GLuint ebo() const { return m_ebo; }
  bool usesVertexArray() const { return m_useVertexArray; }
  GLsizei indexCount() const { return m_indexCount; }

private:
  static std::array<std::string, 6> defaultFacePaths();
  void initializeBuffers();
  void destroyBuffers();

  GLuint m_textureId = 0;
  GLuint m_vao = 0;
  GLuint m_vbo = 0;
  GLuint m_ebo = 0;
  GLsizei m_indexCount = 0;
  bool m_useVertexArray = false;
};

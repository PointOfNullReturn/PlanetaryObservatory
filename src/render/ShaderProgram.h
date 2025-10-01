#ifndef PLANETARY_OBSERVATORY_RENDER_SHADERPROGRAM_H
#define PLANETARY_OBSERVATORY_RENDER_SHADERPROGRAM_H

#include "common/EOGL.h"

#include <string>

/// RAII wrapper for an OpenGL shader program.
class ShaderProgram {
public:
  ShaderProgram();
  ~ShaderProgram();

  ShaderProgram(const ShaderProgram &) = delete;
  ShaderProgram &operator=(const ShaderProgram &) = delete;

  ShaderProgram(ShaderProgram &&other) noexcept;
  ShaderProgram &operator=(ShaderProgram &&other) noexcept;

  /// Compiles and links the shader program from GLSL source files.
  bool loadFromFiles(const std::string &vertexPath, const std::string &fragmentPath);

  void use() const;
  GLuint id() const { return m_program; }

private:
  GLuint compileShader(GLenum type, const std::string &path);
  void destroy();

  GLuint m_program = 0;
};

#endif // PLANETARY_OBSERVATORY_RENDER_SHADERPROGRAM_H

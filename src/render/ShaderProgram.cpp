#include "render/ShaderProgram.h"

#include "utils/Log.h"

#include <fstream>
#include <sstream>
#include <utility>

namespace {
std::string readFile(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    Log::error("Failed to open shader file: " + path);
    return {};
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
}

ShaderProgram::ShaderProgram() = default;

ShaderProgram::~ShaderProgram() { destroy(); }

ShaderProgram::ShaderProgram(ShaderProgram &&other) noexcept
    : m_program(other.m_program) {
  other.m_program = 0;
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other) noexcept {
  if (this != &other) {
    destroy();
    m_program = other.m_program;
    other.m_program = 0;
  }
  return *this;
}

bool ShaderProgram::loadFromFiles(const std::string &vertexPath,
                                  const std::string &fragmentPath) {
  destroy();

  const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexPath);
  if (vertexShader == 0) {
    return false;
  }

  const GLuint fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, fragmentPath);
  if (fragmentShader == 0) {
    glDeleteShader(vertexShader);
    return false;
  }

  m_program = glCreateProgram();
  glAttachShader(m_program, vertexShader);
  glAttachShader(m_program, fragmentShader);
  glBindAttribLocation(m_program, 0, "aPosition");
  glBindAttribLocation(m_program, 1, "aNormal");
  glBindAttribLocation(m_program, 2, "aTexCoord");
  glBindAttribLocation(m_program, 3, "aColor");

  glLinkProgram(m_program);

  GLint linked = GL_FALSE;
  glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
  if (linked == GL_FALSE) {
    GLint logLength = 0;
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
    std::string log;
    log.resize(static_cast<std::size_t>(logLength), '\0');
    glGetProgramInfoLog(m_program, logLength, nullptr, log.data());
    Log::error("Shader program link failed: " + log);
    destroy();
  }

  glDetachShader(m_program, vertexShader);
  glDetachShader(m_program, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return m_program != 0;
}

GLuint ShaderProgram::compileShader(GLenum type, const std::string &path) {
  const std::string source = readFile(path);
  if (source.empty()) {
    return 0;
  }

  const char *cstr = source.c_str();
  const GLint length = static_cast<GLint>(source.size());

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &cstr, &length);
  glCompileShader(shader);

  GLint compiled = GL_FALSE;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (compiled == GL_FALSE) {
    GLint logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::string log;
    log.resize(static_cast<std::size_t>(logLength), '\0');
    glGetShaderInfoLog(shader, logLength, nullptr, log.data());
    Log::error("Shader compilation failed for " + path + ": " + log);
    glDeleteShader(shader);
    shader = 0;
  }

  return shader;
}

void ShaderProgram::use() const { glUseProgram(m_program); }

void ShaderProgram::destroy() {
  if (m_program != 0) {
    glDeleteProgram(m_program);
    m_program = 0;
  }
}

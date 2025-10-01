#ifndef PLANETARY_OBSERVATORY_RENDER_GLSTATE_H
#define PLANETARY_OBSERVATORY_RENDER_GLSTATE_H

#include "common/EOGL.h"

#include <glm/vec4.hpp>

/// Small helpers for toggling common OpenGL fixed-function state.
namespace glstate {

inline void enableDepthTest(bool enable) {
  if (enable) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
}

inline void enableBlend(bool enable, GLenum src = GL_SRC_ALPHA,
                        GLenum dst = GL_ONE_MINUS_SRC_ALPHA) {
  if (enable) {
    glEnable(GL_BLEND);
    glBlendFunc(src, dst);
  } else {
    glDisable(GL_BLEND);
  }
}

inline void enableCullFace(bool enable) {
  if (enable) {
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }
}

inline void setDepthMask(bool enableWrites) {
  glDepthMask(enableWrites ? GL_TRUE : GL_FALSE);
}

inline void setLineWidth(GLfloat width) { glLineWidth(width); }

inline void setClearColor(const glm::vec4 &color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

} // namespace glstate

#endif // PLANETARY_OBSERVATORY_RENDER_GLSTATE_H

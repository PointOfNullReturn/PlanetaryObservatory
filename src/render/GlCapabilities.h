#ifndef PLANETARY_OBSERVATORY_RENDER_GLCAPABILITIES_H
#define PLANETARY_OBSERVATORY_RENDER_GLCAPABILITIES_H

#include "common/EOGL.h"

/// Returns true when the current context exposes vertex array objects.
inline bool glSupportsVertexArrayObjects() {
  return glad_glGenVertexArrays != nullptr && glad_glBindVertexArray != nullptr;
}

#endif // PLANETARY_OBSERVATORY_RENDER_GLCAPABILITIES_H

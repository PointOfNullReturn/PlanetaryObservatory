#ifndef PLANETARY_OBSERVATORY_RENDER_MESHBUILDER_H
#define PLANETARY_OBSERVATORY_RENDER_MESHBUILDER_H

#include "common/EOGL.h"

#include <memory>

class GLUSphere;

/// Utility for constructing simple meshes backed by GLU quadrics.
class MeshBuilder {
public:
  static std::unique_ptr<GLUSphere> createSphere();
};

class GLUSphere {
public:
  explicit GLUSphere(GLUquadric *quadric);
  ~GLUSphere();

  GLUSphere(const GLUSphere &) = delete;
  GLUSphere &operator=(const GLUSphere &) = delete;

  GLUSphere(GLUSphere &&other) noexcept;
  GLUSphere &operator=(GLUSphere &&other) noexcept;

  GLUquadric *handle() const { return m_quadric; }

private:
  GLUquadric *m_quadric = nullptr;
};

#endif // PLANETARY_OBSERVATORY_RENDER_MESHBUILDER_H

#include "render/MeshBuilder.h"

#include "utils/Log.h"

#include <utility>

std::unique_ptr<GLUSphere> MeshBuilder::createSphere() {
  GLUquadric *quadric = gluNewQuadric();
  if (!quadric) {
    Log::error("Failed to allocate GLU quadric for sphere mesh");
    return nullptr;
  }
  gluQuadricNormals(quadric, GLU_SMOOTH);
  gluQuadricTexture(quadric, GL_TRUE);
  return std::make_unique<GLUSphere>(quadric);
}

GLUSphere::GLUSphere(GLUquadric *quadric) : m_quadric(quadric) {}

GLUSphere::~GLUSphere() {
  if (m_quadric) {
    gluDeleteQuadric(m_quadric);
    m_quadric = nullptr;
  }
}

GLUSphere::GLUSphere(GLUSphere &&other) noexcept { std::swap(m_quadric, other.m_quadric); }

GLUSphere &GLUSphere::operator=(GLUSphere &&other) noexcept {
  if (this != &other) {
    std::swap(m_quadric, other.m_quadric);
  }
  return *this;
}

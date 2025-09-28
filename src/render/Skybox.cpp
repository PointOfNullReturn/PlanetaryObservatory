#include "render/Skybox.h"

#include "render/TextureLoader.h"
#include "utils/Log.h"

#include <array>
#include <string>

namespace {
constexpr GLfloat kSkyboxSize = 40.0f;
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
}

Skybox::~Skybox() {
  if (m_textureId != 0) {
    glDeleteTextures(1, &m_textureId);
    m_textureId = 0;
  }
}

bool Skybox::isLoaded() const { return m_textureId != 0; }

void Skybox::render() const {
  if (!isLoaded()) {
    return;
  }

  GLfloat viewMatrix[16] = {0.0f};
  glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
  viewMatrix[12] = 0.0f;
  viewMatrix[13] = 0.0f;
  viewMatrix[14] = 0.0f;

  glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_FALSE);

  glPushMatrix();
  glLoadMatrixf(viewMatrix);

  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_CUBE_MAP);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);

  const GLfloat s = kSkyboxSize;

  glBegin(GL_QUADS);
  // +X (right)
  glTexCoord3f(1.0f, -1.0f, -1.0f);
  glVertex3f(s, -s, -s);
  glTexCoord3f(1.0f, -1.0f, 1.0f);
  glVertex3f(s, -s, s);
  glTexCoord3f(1.0f, 1.0f, 1.0f);
  glVertex3f(s, s, s);
  glTexCoord3f(1.0f, 1.0f, -1.0f);
  glVertex3f(s, s, -s);

  // -X (left)
  glTexCoord3f(-1.0f, -1.0f, 1.0f);
  glVertex3f(-s, -s, s);
  glTexCoord3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(-s, -s, -s);
  glTexCoord3f(-1.0f, 1.0f, -1.0f);
  glVertex3f(-s, s, -s);
  glTexCoord3f(-1.0f, 1.0f, 1.0f);
  glVertex3f(-s, s, s);

  // +Y (top)
  glTexCoord3f(-1.0f, 1.0f, -1.0f);
  glVertex3f(-s, s, -s);
  glTexCoord3f(-1.0f, 1.0f, 1.0f);
  glVertex3f(-s, s, s);
  glTexCoord3f(1.0f, 1.0f, 1.0f);
  glVertex3f(s, s, s);
  glTexCoord3f(1.0f, 1.0f, -1.0f);
  glVertex3f(s, s, -s);

  // -Y (bottom)
  glTexCoord3f(-1.0f, -1.0f, 1.0f);
  glVertex3f(-s, -s, s);
  glTexCoord3f(1.0f, -1.0f, 1.0f);
  glVertex3f(s, -s, s);
  glTexCoord3f(1.0f, -1.0f, -1.0f);
  glVertex3f(s, -s, -s);
  glTexCoord3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(-s, -s, -s);

  // +Z (front)
  glTexCoord3f(-1.0f, -1.0f, 1.0f);
  glVertex3f(-s, -s, s);
  glTexCoord3f(1.0f, -1.0f, 1.0f);
  glVertex3f(s, -s, s);
  glTexCoord3f(1.0f, 1.0f, 1.0f);
  glVertex3f(s, s, s);
  glTexCoord3f(-1.0f, 1.0f, 1.0f);
  glVertex3f(-s, s, s);

  // -Z (back)
  glTexCoord3f(1.0f, -1.0f, -1.0f);
  glVertex3f(s, -s, -s);
  glTexCoord3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(-s, -s, -s);
  glTexCoord3f(-1.0f, 1.0f, -1.0f);
  glVertex3f(-s, s, -s);
  glTexCoord3f(1.0f, 1.0f, -1.0f);
  glVertex3f(s, s, -s);
  glEnd();

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glDisable(GL_TEXTURE_CUBE_MAP);

  glPopMatrix();
  glPopAttrib();
}
std::array<std::string, 6> Skybox::defaultFacePaths() {
  return {"assets/textures/skybox/space_right.ppm",
          "assets/textures/skybox/space_left.ppm",
          "assets/textures/skybox/space_top.ppm",
          "assets/textures/skybox/space_bottom.ppm",
          "assets/textures/skybox/space_front.ppm",
          "assets/textures/skybox/space_back.ppm"};
}

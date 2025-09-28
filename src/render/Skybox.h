#pragma once

#include "common/EOGL.h"

#include <array>
#include <string>

/// Represents the background skybox shown behind the planetary scene.
class Skybox {
public:
  Skybox();
  ~Skybox();

  /// Returns whether the skybox cubemap was successfully loaded.
  bool isLoaded() const;

  /// Renders the skybox using the current view matrix.
  void render() const;

private:
  static std::array<std::string, 6> defaultFacePaths();

  GLuint m_textureId = 0;
};

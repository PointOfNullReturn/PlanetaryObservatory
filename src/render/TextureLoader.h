#pragma once

#include "common/EOGL.h"

#include <array>
#include <string>

// Loads an image file from disk and creates an OpenGL 2D texture. Returns 0 on
// failure.
GLuint LoadTexture2D(const std::string &path, bool generateMipmaps = true,
                     bool flipVertically = true, bool flipHorizontally = false);

// Loads a cubemap texture from 6 faces in order: right, left, top, bottom,
// front, back. Returns 0 on failure.
GLuint LoadCubemap(const std::array<std::string, 6> &facePaths,
                   bool generateMipmaps = false);

#pragma once

#include "EOGL.h"

#include <string>

// Loads an image file from disk and creates an OpenGL 2D texture. Returns 0 on failure.
GLuint LoadTexture2D(const std::string& path,
                     bool generateMipmaps = true,
                     bool flipVertically = true);

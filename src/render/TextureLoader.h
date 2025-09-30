#ifndef PLANETARY_OBSERVATORY_RENDER_TEXTURELOADER_H
#define PLANETARY_OBSERVATORY_RENDER_TEXTURELOADER_H

#include "common/EOGL.h"

#include <array>
#include <string>

GLuint LoadTexture2D(const std::string &path, bool generateMipmaps = true,
                     bool flipVertically = false, bool flipHorizontally = false);

GLuint LoadCubemap(const std::array<std::string, 6> &facePaths,
                   bool generateMipmaps = true);

#endif // PLANETARY_OBSERVATORY_RENDER_TEXTURELOADER_H

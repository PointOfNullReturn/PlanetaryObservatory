#include "render/TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "utils/Log.h"

#include <GLFW/glfw3.h>
#if defined(__APPLE__)
#include <dlfcn.h>
#endif

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

namespace {

template <typename Proc>
Proc loadProc(const char *name) {
  if (glfwGetCurrentContext() != nullptr) {
    if (auto proc = reinterpret_cast<Proc>(glfwGetProcAddress(name))) {
      return proc;
    }
  }
#if defined(__APPLE__)
  if (auto proc = reinterpret_cast<Proc>(dlsym(RTLD_DEFAULT, name))) {
    return proc;
  }
#endif
  return nullptr;
}

void ensureTextureFunctionsLoaded() {
  if (glad_glGenTextures == nullptr) {
    glad_glGenTextures = loadProc<PFNGLGENTEXTURESPROC>("glGenTextures");
  }
  if (glad_glBindTexture == nullptr) {
    glad_glBindTexture = loadProc<PFNGLBINDTEXTUREPROC>("glBindTexture");
  }
  if (glad_glTexImage2D == nullptr) {
    glad_glTexImage2D = loadProc<PFNGLTEXIMAGE2DPROC>("glTexImage2D");
  }
  if (glad_glTexParameteri == nullptr) {
    glad_glTexParameteri = loadProc<PFNGLTEXPARAMETERIPROC>("glTexParameteri");
  }
  if (glad_glGenerateMipmap == nullptr) {
    glad_glGenerateMipmap = loadProc<PFNGLGENERATEMIPMAPPROC>("glGenerateMipmap");
  }
}

} // namespace

GLuint LoadTexture2D(const std::string &path, bool generateMipmaps,
                     bool flipVertically, bool flipHorizontally) {
  stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

  ensureTextureFunctionsLoaded();

  if (glad_glGenTextures == nullptr || glad_glBindTexture == nullptr ||
      glad_glTexImage2D == nullptr) {
    Log::error("LoadTexture2D called before OpenGL was initialised; skipping " +
               path);
    return 0;
  }

  int width = 0;
  int height = 0;
  int channels = 0;
  stbi_uc *pixels =
      stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

  if (!pixels) {
    Log::error(std::string("Failed to load texture: ") + path);
    return 0;
  }

  const GLenum format = GL_RGBA;

  if (flipHorizontally) {
    const int bytesPerPixel = 4; // STBI_rgb_alpha forces 4 channels
    const int rowStride = width * bytesPerPixel;

    for (int y = 0; y < height; ++y) {
      stbi_uc *row = pixels + y * rowStride;
      for (int x = 0; x < width / 2; ++x) {
        stbi_uc *left = row + x * bytesPerPixel;
        stbi_uc *right = row + (width - 1 - x) * bytesPerPixel;
        for (int channel = 0; channel < bytesPerPixel; ++channel) {
          std::swap(left[channel], right[channel]);
        }
      }
    }
  }

  GLuint textureId = 0;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, pixels);

  if (generateMipmaps) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  generateMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(pixels);

  if (Log::kDebugLoggingEnabled) {
    Log::debug(std::string("Loaded texture ") + path + " (" +
               std::to_string(width) + "x" + std::to_string(height) +
               ", channels: " + std::to_string(channels) +
               ") id=" + std::to_string(textureId));
  }

  return textureId;
}

GLuint LoadCubemap(const std::array<std::string, 6> &facePaths,
                   bool generateMipmaps) {
  stbi_set_flip_vertically_on_load(0);

  ensureTextureFunctionsLoaded();

  std::fprintf(stderr, "LoadCubemap: glTexImage2D pointer = %p\n",
               reinterpret_cast<void *>(glad_glTexImage2D));
  Log::info(std::string("LoadCubemap: glTexImage2D pointer = ") +
            std::to_string(reinterpret_cast<std::uintptr_t>(glad_glTexImage2D)));

  if (glad_glGenTextures == nullptr || glad_glBindTexture == nullptr ||
      glad_glTexImage2D == nullptr) {
    Log::error("LoadCubemap called before OpenGL was initialised; aborting");
    return 0;
  }

  GLuint textureId = 0;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

  int width = 0;
  int height = 0;
  int channels = 0;

  for (std::size_t i = 0; i < facePaths.size(); ++i) {
    const std::string &path = facePaths[i];
    stbi_uc *pixels =
        stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (!pixels) {
      Log::error(std::string("Failed to load cubemap face: ") + path);
      glDeleteTextures(1, &textureId);
      glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
      return 0;
    }

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(i), 0,
                 GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    stbi_image_free(pixels);
  }

  const GLint filter = generateMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  if (generateMipmaps) {
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  }

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  if (Log::kDebugLoggingEnabled) {
    Log::debug("Loaded cubemap texture id=" + std::to_string(textureId) +
               " size=" + std::to_string(width) + "x" + std::to_string(height));
  }

  return textureId;
}

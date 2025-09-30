#ifndef PLANETARY_OBSERVATORY_RENDER_TEXTURECACHE_H
#define PLANETARY_OBSERVATORY_RENDER_TEXTURECACHE_H

#include <string>
#include <unordered_map>

#include "render/TextureLoader.h"

/// Caches OpenGL texture handles keyed by asset path.
class TextureCache {
public:
  TextureCache() = default;
  ~TextureCache();

  /// Returns an existing texture ID for `path` or loads a new one if missing.
  GLuint getTexture2D(const std::string &path, bool generateMipmaps = true,
                      bool flipVertically = false, bool flipHorizontally = false);

  /// Clears all cached textures, deleting the OpenGL resources.
  void clear();

private:
  struct TextureRecord {
    GLuint id = 0;
    bool mipmapped = false;
  };

  std::unordered_map<std::string, TextureRecord> m_textures;
};

/// Returns a shared cache instance used by legacy loaders for now.
TextureCache &GetTextureCache();

#endif // PLANETARY_OBSERVATORY_RENDER_TEXTURECACHE_H

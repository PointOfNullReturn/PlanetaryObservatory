#include "render/TextureCache.h"

#include "common/EOGL.h"
#include "utils/Log.h"

TextureCache::~TextureCache() { clear(); }

GLuint TextureCache::getTexture2D(const std::string &path, bool generateMipmaps,
                                  bool flipVertically, bool flipHorizontally) {
  auto it = m_textures.find(path);
  if (it != m_textures.end()) {
    if (generateMipmaps && !it->second.mipmapped) {
      Log::warn("Texture requested with mipmaps after non-mipmap load: " + path);
    }
    return it->second.id;
  }

  const GLuint id = LoadTexture2D(path, generateMipmaps, flipVertically,
                                  flipHorizontally);
  if (id == 0) {
    return 0;
  }

  m_textures.emplace(path, TextureRecord{id, generateMipmaps});
  return id;
}

void TextureCache::clear() {
  for (auto &entry : m_textures) {
    if (entry.second.id != 0) {
      glDeleteTextures(1, &entry.second.id);
    }
  }
  m_textures.clear();
}

TextureCache &GetTextureCache() {
  static TextureCache cache;
  return cache;
}

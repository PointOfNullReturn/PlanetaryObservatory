#include "render/TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "utils/Log.h"

#include <algorithm>
#include <string>

GLuint LoadTexture2D(const std::string& path,
                     bool generateMipmaps,
                     bool flipVertically,
                     bool flipHorizontally)
{
    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

    int width = 0;
    int height = 0;
    int channels = 0;
    stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!pixels)
    {
        Log::error(std::string("Failed to load texture: ") + path);
        return 0;
    }

    const GLenum format = GL_RGBA;

    if (flipHorizontally)
    {
        const int bytesPerPixel = 4; // STBI_rgb_alpha forces 4 channels
        const int rowStride = width * bytesPerPixel;

        for (int y = 0; y < height; ++y)
        {
            stbi_uc* row = pixels + y * rowStride;
            for (int x = 0; x < width / 2; ++x)
            {
                stbi_uc* left = row + x * bytesPerPixel;
                stbi_uc* right = row + (width - 1 - x) * bytesPerPixel;
                for (int channel = 0; channel < bytesPerPixel; ++channel)
                {
                    std::swap(left[channel], right[channel]);
                }
            }
        }
    }

    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

    if (generateMipmaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(pixels);

    Log::debug(std::string("Loaded texture ") + path + " (" +
               std::to_string(width) + "x" + std::to_string(height) +
               ", channels: " + std::to_string(channels) + ") id=" +
               std::to_string(textureId));

    return textureId;
}

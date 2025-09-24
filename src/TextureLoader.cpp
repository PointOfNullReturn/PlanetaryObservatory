#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

GLuint LoadTexture2D(const std::string& path, bool generateMipmaps, bool flipVertically)
{
    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

    int width = 0;
    int height = 0;
    int channels = 0;
    stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!pixels)
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return 0;
    }

    const GLenum format = GL_RGBA;

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

    std::cout << "Loaded texture " << path << " (" << width << "x" << height << ", channels: " << channels << ") id=" << textureId << std::endl;

    return textureId;
}

#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include <spdlog/spdlog.h>

Texture Texture::create(const std::string &texture)
{
    int width, height, num_channels;
    unsigned char* data = stbi_load(texture.c_str(), &width, &height, &num_channels, 0);
    if (!data) {
        spdlog::error("Failed to open file: {} !", texture);
    }

    uint32_t tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate texture
    if (num_channels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else if (num_channels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return Texture(M{
        .texture = tex
    });
}

Texture::Texture(WithResultOf &&res)
{
    Texture other = std::move(res.func());

	m = std::move(other.m);
}

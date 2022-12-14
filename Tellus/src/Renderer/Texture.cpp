#include "Texture.hpp"
#include "tspch.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace ts {

Texture2D::Texture2D(const std::string& filepath) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    TS_ASSERT(data, "Failed to load image!");
    m_Width = width;
    m_Height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
    glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height);

    glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}
Texture2D::~Texture2D() {
    glDeleteTextures(1, &m_ID);
}

void Texture2D::Bind(unsigned int slot) const {
    glBindTextureUnit(slot, m_ID);
}
void Texture2D::Unbind(unsigned int slot) const {
    glBindTextureUnit(slot, 0);
}

TextureCubeMap::TextureCubeMap(const std::string& directoryPath) {
    std::vector<std::string> texture_paths(6);
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        std::string p = entry.path().filename().u8string();
        std::transform(p.begin(), p.end(), p.begin(), ::tolower);
        if (p.find("right") != std::string::npos) {
            texture_paths[0] = entry.path().u8string();
        } else if (p.find("left") != std::string::npos) {
            texture_paths[1] = entry.path().u8string();
        } else if (p.find("top") != std::string::npos) {
            texture_paths[2] = entry.path().u8string();
        } else if (p.find("bottom") != std::string::npos) {
            texture_paths[3] = entry.path().u8string();
        } else if (p.find("back") != std::string::npos) {
            texture_paths[4] = entry.path().u8string();
        } else if (p.find("front") != std::string::npos) {
            texture_paths[5] = entry.path().u8string();
        }
    }

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    int width, height, channels;
    unsigned char* data;
    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < texture_paths.size(); i++) {
        data = stbi_load(texture_paths[i].c_str(), &width, &height, &channels, 0);
        TS_ASSERT(data, "Failed to load image!");
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data

        );
        stbi_image_free(data);
    }

    m_Width = width;
    m_Height = height;
}
TextureCubeMap::~TextureCubeMap() {}

void TextureCubeMap::Bind(unsigned int slot) const {
    glBindTextureUnit(slot, m_ID);
}
void TextureCubeMap::Unbind(unsigned int slot) const {
    glBindTextureUnit(slot, 0);
}

TextureBuffer::TextureBuffer(float* data, unsigned int byteSize, TextureDataType type) {
    unsigned int temp;
    glGenBuffers(1, &temp);
    glBindBuffer(GL_TEXTURE_BUFFER, temp);
    glBufferData(GL_TEXTURE_BUFFER, byteSize, data, GL_STATIC_DRAW);

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_BUFFER, m_ID);
    glTexBuffer(GL_TEXTURE_BUFFER, static_cast<int>(type), temp);

    glBindTexture(GL_TEXTURE_BUFFER, 0);
    glBindBuffer(GL_TEXTURE_BUFFER, 0);
    glDeleteBuffers(1, &temp);
}
TextureBuffer::~TextureBuffer() {
    glDeleteTextures(1, &m_ID);
}

void TextureBuffer::Bind(unsigned int slot) const {
    glBindTextureUnit(slot, m_ID);
}

void TextureBuffer::Unbind(unsigned int slot) const {
    glBindTextureUnit(slot, 0);
}

};  // namespace ts
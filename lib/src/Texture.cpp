#include "Texture.hpp"
#include "stb_image.hpp"
Texture::Texture(const std::string& filePath) 
    : m_RendererID(0),
      m_FilePath(filePath),
      m_LocalBuffer(nullptr),
      m_height(0),
      m_width(0),
      m_BPP(0) {

    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(filePath.c_str(), &m_width, &m_height, &m_BPP, 4);
    GLCall( glGenTextures(1, &m_RendererID) );
    GLCall( glBindTexture(GL_TEXTURE_2D, m_RendererID) ); // Bind without slot selection

    GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
    GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
    GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
    GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );

    GLCall( glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer) );
    unbind();

    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::bind(uint32_t slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::unbind() {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

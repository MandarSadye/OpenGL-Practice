#pragma once

#include "Renderer.h"

class Texture {
private:
    uint32_t m_RendererID;
    std::string m_FilePath;
    u_char* m_LocalBuffer;
    int32_t m_width, m_height, m_BPP; 
public:
    Texture(const std::string& filePath);
    ~Texture();

    void bind(uint32_t slots = 0) const;
    void unbind();

    inline uint32_t getWidth() {return m_width;};
    inline uint32_t getHeight() {return m_height;};
    inline uint32_t getBitsPerPixcel() {return m_BPP;};
};
#pragma once


#include "util/gl_helpers.hpp"

#include "std_header.hpp"

namespace ay::rend
{


class Texture
{

  private:
    std::string m_path;
    uint32_t m_width;
    uint32_t m_height;
    
    uint32_t m_id;
    GLenum m_internal_format;
    GLenum m_data_format;

  public:
    Texture(uint32_t width, uint32_t height);

    explicit Texture(const std::string& path);

    ~Texture()
    {
        glDeleteTextures(1, &m_id);
    }

    uint32_t width() const
    {
        return m_width;
    }

    uint32_t height() const
    {
        return m_height;
    }

    uint32_t id() const
    {
        return m_id;
    }

    void set_data(void* data)
    {
        // uint32_t bpp = m_data_format == GL_RGBA ? 4 : 3;
        glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);
    }

    void bind(uint32_t slot = 0)
    {
        glBindTextureUnit(slot, m_id);
    }

    bool operator==(const Texture& other) const
    {
        return m_id == (other).m_id;
    }

};

using TexturePtr = std::shared_ptr<Texture>;


inline TexturePtr create_texture(const std::string& path)
{
    return std::make_shared<Texture>(path);
}

inline TexturePtr create_texture(uint32_t width, uint32_t heigh)
{
    return std::make_shared<Texture>(width, heigh);
}


}

#pragma once


#include "util/gl_helpers.hpp"

#include "std_header.hpp"


namespace ay::rend
{

enum class TextureFormat : GLenum
{
    NONE            = 0,
    RGB             = GL_RGB,
    RGBA            = GL_RGBA,
    LUMINANCE       = GL_LUMINANCE,
    LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA
};

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
    Texture(uint32_t width, uint32_t height, TextureFormat format = TextureFormat::RGBA);

    explicit Texture(const std::string &path);

    ~Texture();

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

    void set_data(void *data, TextureFormat format = TextureFormat::RGBA);

    void bind(uint32_t slot = 0);

    bool operator==(const Texture &other) const
    {
        return m_id == (other).m_id;
    }
};

using TexturePtr = std::shared_ptr<Texture>;

inline TexturePtr create_texture(const std::string &path)
{
    return std::make_shared<Texture>(path);
}

inline TexturePtr create_texture(uint32_t width, uint32_t heigh)
{
    return std::make_shared<Texture>(width, heigh);
}

inline TexturePtr create_texture(uint32_t width,
                                 uint32_t heigh,
                                 void *data,
                                 TextureFormat format = TextureFormat::RGBA)
{
    auto tex = std::make_shared<Texture>(width, heigh);
    tex->set_data(data, format);
    return tex;
}


}  // namespace ay::rend

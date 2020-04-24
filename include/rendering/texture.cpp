#include "rendering/texture.hpp"

#include <stb_image.h>

namespace ay::rend
{

Texture::Texture(uint32_t width, uint32_t height)
{
    m_width  = width;
    m_height = height;

    m_internal_format = GL_RGBA8;
    m_data_format     = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureStorage2D(m_id, 1, m_internal_format, m_width, m_height);

    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(const std::string &path)
{
    m_path = path;
    int width{}, height{}, channels{};
    stbi_set_flip_vertically_on_load(1);
    stbi_uc *data = nullptr;
    data          = stbi_load(path.c_str(), &width, &height, &channels, 0);
    m_width       = width;
    m_height      = height;

    if (channels == 4)
    {
        m_internal_format = GL_RGBA8;
        m_data_format     = GL_RGBA;
    }
    else if (channels == 3)
    {
        m_internal_format = GL_RGB8;
        m_data_format     = GL_RGB;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureStorage2D(m_id, 1, m_internal_format, m_width, m_height);

    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(
      m_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}


}  // namespace ay::rend

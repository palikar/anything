#pragma once

#include "rendering/texture.hpp"

#include "util/gl_helpers.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::rend
{

class FrameBuffer
{
  private:
    uint32_t m_fbo;
    uint32_t m_depth;

    TexturePtr m_texture;

    // std::vector<TexturePtr> m_color_atachments;

    uint32_t m_width;
    uint32_t m_height;

    glm::vec4 m_clear_color;

  public:
    FrameBuffer(uint32_t width, uint32_t height);

    ~FrameBuffer();

    void bind() const;

    void unbind() const;

    void clear();


    inline uint32_t width() const
    {
        return m_width;
    }

    inline uint32_t height() const
    {
        return m_height;
    }

    inline Texture *texture() const
    {
        return m_texture.get();
    }

    inline void set_clear_color(const glm::vec4 &color)
    {
        m_clear_color = color;
    }
};


using FrameBufferPtr = std::unique_ptr<FrameBuffer>;

inline FrameBufferPtr create_fbo(uint32_t width, uint32_t height)
{
    return std::make_unique<FrameBuffer>(width, height);
}


}  // namespace ay::rend

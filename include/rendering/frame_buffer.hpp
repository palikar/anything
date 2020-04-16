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
    uint32_t m_Height;

    glm::vec4 m_clear_color;
    
  public:

    FrameBuffer(uint32_t width, uint32_t height)
    {
        GLCall(glGenFramebuffers(1, &m_fbo));
		GLCall(glGenRenderbuffers(1, &m_depth));
        
		m_texture = std::make_shared<Texture>(width, height);

		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_depth));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height));

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->id(), 0));
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    ~FrameBuffer()
    {
        GLCall(glDeleteFramebuffers(1, &m_fbo));
    }

    void bind() const
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
		GLCall(glViewport(0, 0, m_width, m_height));
    }
    
    void unbind() const
    {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void clear() override
    {
        GLCall(glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, m_clear_color.w));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }
    

    inline uint32_t width() const { return m_width; }
    
    inline uint32_t height() const { return m_height; }

    inline Texture* texture() const override { return m_texture.get(); }
    
    inline void set_clear_color(const maths::vec4& color) { m_clear_color = color; }
}


}

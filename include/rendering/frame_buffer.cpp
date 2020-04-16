
#include "rendering/frame_buffer.hpp"


namespace ay::rend
{


FrameBuffer::FrameBuffer(uint32_t width, uint32_t height)
  : m_width(width), m_height(height)
{

    // check size here

    GLCall(glGenFramebuffers(1, &m_fbo));
    GLCall(glGenRenderbuffers(1, &m_depth));

    m_texture = std::make_shared<Texture>(width, height);

    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_depth));
    GLCall(
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height));

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    GLCall(glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->id(), 0));
    GLCall(glFramebufferRenderbuffer(
      GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

FrameBuffer::~FrameBuffer()
{
    GLCall(glDeleteFramebuffers(1, &m_fbo));
}

void FrameBuffer::bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    GLCall(glViewport(0, 0, m_width, m_height));
}

void FrameBuffer::unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::clear()
{
    GLCall(
      glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, m_clear_color.w));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}


}  // namespace ay::rend

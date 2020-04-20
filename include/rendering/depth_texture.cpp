

#include "rendering/depth_texture.hpp"

namespace ay::rend
{


DepthTexture::DepthTexture(uint32_t width, uint32_t height)
  : m_width(width), m_height(height)
{

    GLCall(glGenTextures(1, &m_id));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_id));

    GLCall(glTexImage2D(GL_TEXTURE_2D,
                        0,
                        GL_DEPTH_COMPONENT16,
                        m_width,
                        m_height,
                        0,
                        GL_DEPTH_COMPONENT,
                        GL_UNSIGNED_SHORT,
                        NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
    GLCall(
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}


DepthTexture::~DepthTexture()
{
    GLCall(glDeleteTextures(1, &m_id));
}


void DepthTexture::bind(uint32_t slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
}


void DepthTexture::unbind(uint32_t slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}


}  // namespace ay::rend

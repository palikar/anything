#pragma once

#include "rendering/texture.hpp"




namespace ay::rend
{


class DepthTexture
{
  private:
    uint32_t m_width;
    uint32_t m_height;

    uint32_t m_id;

  public: 

    DepthTexture(uint32_t width, uint32_t height);

    ~DepthTexture();


    void bind(uint32_t slot = 0) const;
    
    void unbind(uint32_t slot = 0) const;


    uint32_t id() const { return m_id; }


};

using DepthTexturePtr = std::shared_ptr<DepthTexture>;



}

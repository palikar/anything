#pragma once

#include "graphics/material.hpp"
#include "rendering/texture.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::grph
{


class TextureMaterial : public Material
{
  private:

    rend::TexturePtr m_texture;
    
  public:
    TextureMaterial(rend::TexturePtr tex) : m_texture(std::move(tex))
    {
    }

    virtual ~TextureMaterial() = default;

    void init_shader(rend::ShaderLibrary &t_shader_lib) override
    {
        m_shader =  t_shader_lib.load("textured");
    }

    void update_uniforms(rend::TextureBinder&) override
    {
        m_texture->bind(1);
        m_shader->set_sampler("tex", 1);
    }
    
};


inline MaterialPtr texture_material(rend::TexturePtr tex)
{
    return std::make_unique<TextureMaterial>(std::move(tex));
}


}  // namespace ay::grph

#pragma once


#include "graphics/material.hpp"

#include "rendering/api.hpp"
#include "rendering/texture.hpp"
#include "rendering/cube_texture.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::grph
{


class TextureMaterial : public Material
{
  private:
    glm::vec3 m_color{1.0f, 1.0f, 1.0f};

    rend::CubeTexturePtr m_env_map;
    rend::TexturePtr m_map;
    rend::TexturePtr m_alpha_map;
    rend::TexturePtr m_specular_map;
    rend::TexturePtr m_ao_map;

    float m_ao_intensity{0.0};

    rend::Combine m_combine{rend::Combine::MIX};
    float m_reflectivity;
    float m_refraction_ration;

  public:

    TextureMaterial(rend::TexturePtr tex) : m_map(std::move(tex))
    {
    }

    virtual ~TextureMaterial() = default;

    void init_shader(rend::ShaderLibrary &t_shader_lib) override
    {
        m_shader = t_shader_lib.load("textured");
    }

    void update_uniforms(rend::TextureBinder &binder) override
    {
        m_shader->set("mixing", static_cast<int>(m_combine));
        m_shader->set("reflectivity", m_reflectivity);
        m_shader->set("refraction_ration", m_refraction_ration);

        m_shader->set_sampler("tex", binder.resolve(m_map.get()));

        if (m_ao_map)
        {
            m_shader->set_sampler("ao_map", binder.resolve(m_ao_map.get()));
        }

        if (m_specular_map)
        {
            m_shader->set_sampler("specular_map", binder.resolve(m_specular_map.get()));
        }

        if (m_alpha_map)
        {
            m_shader->set_sampler("alpha_map", binder.resolve(m_alpha_map.get()));            
        }

        if (m_env_map)
        {
            m_shader->set_sampler("env_map", binder.resolve(m_env_map.get()));
        }
        
    }
};


inline MaterialPtr texture_material(rend::TexturePtr tex)
{
    return std::make_unique<TextureMaterial>(std::move(tex));
}


}  // namespace ay::grph

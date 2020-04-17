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
    glm::vec3 m_color{0.0f, 0.0f, 0.0f};

    rend::CubeTexturePtr m_env_map;
    rend::TexturePtr m_map;
    rend::TexturePtr m_alpha_map;
    rend::TexturePtr m_specular_map;
    rend::TexturePtr m_ao_map;

    float m_ao_intensity{0.0};

    rend::Combine m_combine{rend::Combine::MIX};

    bool m_is_reflection{true};
    
    float m_reflectivity{0.0f};
    float m_refraction_ration{0.0};

  public:

    TextureMaterial(rend::TexturePtr tex) : m_map(std::move(tex))
    {
    }

    virtual ~TextureMaterial() = default;

    void init_shader(rend::ShaderLibrary &t_shader_lib) override
    {
        m_shader = t_shader_lib.load("textured");
    }

    void update_uniforms(rend::TextureBinder &binder, rend::RenderContext& ctx) override
    {
        m_shader->set("mixing", static_cast<int>(m_combine));
        m_shader->set("reflectivity", m_reflectivity);
        m_shader->set("refraction_ration", m_refraction_ration);

        m_shader->set("is_relfection", m_is_reflection);
        
        m_shader->set("color", m_color);

        m_shader->set("camera_pos", ctx.camera_pos);

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

    void set_map(rend::TexturePtr tex)
    {
        m_map = std::move(tex);
    }

    void set_alpha_map(rend::TexturePtr tex)
    {
        m_alpha_map = std::move(tex);
    }
    
    void set_ao_map(rend::TexturePtr tex)
    {
        m_ao_map = std::move(tex);
    }
    
    void set_specular_map(rend::TexturePtr tex)
    {
        m_specular_map = std::move(tex);
    }
    
    void set_env_map(rend::CubeTexturePtr tex, bool reflection = true)
    {
        m_is_reflection = reflection;
        m_env_map = std::move(tex);
    }

    void set_color(glm::vec3 color)
    {
        m_color = color;
    }
    
    void set_reflectivity(float value)
    {
        m_reflectivity = value;
    }

    void set_refraction(float value)
    {
        m_refraction_ration = value;
    }

    void set_ao_intensity(float value)
    {
        m_ao_intensity = value;
    }

    void set_is_reflection(bool reflection)
    {
        m_is_reflection = reflection;
    }

    
};


inline MaterialPtr texture_material(rend::TexturePtr tex)
{
    return std::make_unique<TextureMaterial>(std::move(tex));
}


}  // namespace ay::grph

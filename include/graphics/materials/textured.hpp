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
    glm::vec3 m_color{ 0.0f, 0.0f, 0.0f };

    rend::CubeTexturePtr m_env_map;
    rend::TexturePtr m_map;
    rend::TexturePtr m_alpha_map;
    rend::TexturePtr m_specular_map;
    rend::TexturePtr m_ao_map;

    float m_ao_intensity{ 0.0 };

    rend::Combine m_combine{ rend::Combine::MIX };

    bool m_is_reflection{ true };

    float m_reflectivity{ 0.0f };
    float m_refraction_ration{ 0.0 };

  public:
    TextureMaterial(rend::TexturePtr tex);

    virtual ~TextureMaterial() = default;

    void init_shader(rend::ShaderLibrary &t_shader_lib) override;

    void update_uniforms(rend::TextureBinder &binder, rend::RenderContext &ctx) override;

    bool needs_lighting() override
    {
        return false;
    }

    inline void set_map(rend::TexturePtr tex)
    {
        m_map = std::move(tex);
    }

    inline void set_alpha_map(rend::TexturePtr tex)
    {
        m_alpha_map = std::move(tex);
    }

    inline void set_ao_map(rend::TexturePtr tex)
    {
        m_ao_map = std::move(tex);
    }

    inline void set_specular_map(rend::TexturePtr tex)
    {
        m_specular_map = std::move(tex);
    }

    inline void set_env_map(rend::CubeTexturePtr tex, bool reflection = true)
    {
        m_is_reflection = reflection;
        m_env_map       = std::move(tex);
    }

    inline void set_color(glm::vec3 color)
    {
        m_color = color;
    }

    inline void set_reflectivity(float value)
    {
        m_reflectivity = value;
    }

    inline void set_refraction(float value)
    {
        m_refraction_ration = value;
    }

    inline void set_ao_intensity(float value)
    {
        m_ao_intensity = value;
    }

    inline void set_is_reflection(bool reflection)
    {
        m_is_reflection = reflection;
    }
};


inline MaterialPtr texture_material(rend::TexturePtr tex)
{
    return std::make_unique<TextureMaterial>(std::move(tex));
}


}  // namespace ay::grph

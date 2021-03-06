#pragma once


#include "graphics/material.hpp"

#include "rendering/api.hpp"
#include "rendering/texture.hpp"
#include "rendering/cube_texture.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::grph
{

struct TextureParameters
{
    glm::vec3 m_color{ 0.0f, 0.0f, 0.0f };

    float m_color_intensity{ 0.5f };

    rend::CubeTexturePtr m_env_map{ nullptr };
    rend::TexturePtr m_map{ nullptr };
    rend::TexturePtr m_alpha_map{ nullptr };
    rend::TexturePtr m_specular_map{ nullptr };
    rend::TexturePtr m_ao_map{ nullptr };

    float m_ao_intensity{ 0.0 };

    rend::Combine m_combine{ rend::Combine::MIX };

    bool m_is_reflection{ true };

    float m_reflectivity{ 0.0f };
    float m_refraction_ration{ 0.98f };
};

class TextureMaterial : public Material
{
  private:
    TextureParameters m_parameters;

  public:
    TextureMaterial(rend::TexturePtr tex);

    virtual ~TextureMaterial() = default;

    void init_shader(gmt::ShaderLibrary &t_shader_lib) override;

    void update_uniforms(rend::UniformBinder &,
                         rend::TextureBinder &binder,
                         rend::RenderContext &ctx) override;

    bool needs_lighting() override
    {
        return false;
    }

    inline void set_map(rend::TexturePtr tex)
    {
        m_parameters.m_map = std::move(tex);
    }

    inline void set_alpha_map(rend::TexturePtr tex)
    {
        m_parameters.m_alpha_map = std::move(tex);
    }

    inline void set_ao_map(rend::TexturePtr tex)
    {
        m_parameters.m_ao_map = std::move(tex);
    }

    inline void set_specular_map(rend::TexturePtr tex)
    {
        m_parameters.m_specular_map = std::move(tex);
    }

    inline void set_env_map(rend::CubeTexturePtr tex, bool reflection = true)
    {
        m_parameters.m_is_reflection = reflection;
        m_parameters.m_env_map       = std::move(tex);
    }

    inline void set_color(glm::vec3 color)
    {
        m_parameters.m_color = color;
    }

    inline void set_reflectivity(float value)
    {
        m_parameters.m_reflectivity = value;
    }

    inline void set_refraction(float value)
    {
        m_parameters.m_refraction_ration = value;
    }

    inline void set_ao_intensity(float value)
    {
        m_parameters.m_ao_intensity = value;
    }

    inline void set_is_reflection(bool reflection)
    {
        m_parameters.m_is_reflection = reflection;
    }

    TextureParameters &parameters()
    {
        return m_parameters;
    }
};


using TextureMaterialPtr = std::unique_ptr<TextureMaterial>;

inline TextureMaterialPtr texture_material(rend::TexturePtr tex)
{
    return std::make_unique<TextureMaterial>(std::move(tex));
}


}  // namespace ay::grph

#pragma once


#include "graphics/material.hpp"

#include "rendering/api.hpp"
#include "rendering/texture.hpp"
#include "rendering/cube_texture.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::grph
{

struct PhongParameters
{

    float m_ao_intensity{ 0.0 };
    rend::TexturePtr m_ao_map;

    float m_bump_scale{ 0.0 };
    rend::TexturePtr m_bump_map;

    glm::vec3 m_color{ 0.0f, 0.0f, 0.0f };

    float m_displ_scale{ 0.0 };
    float m_displ_bias{ 0.0 };
    rend::TexturePtr m_displ_map;

    float m_emissive_scale{ 0.0 };
    glm::vec3 m_emissive{ 0.0f, 0.0f, 0.0f };
    rend::TexturePtr m_emissive_map;

    rend::CubeTexturePtr m_env_map;

    rend::TexturePtr m_map;

    rend::TexturePtr m_normal_map;
};

class PhongMaterial : public Material
{
  private:
    PhongParameters m_parameters;

  public:
    PhongMaterial()
    {
    }

    PhongParameters &parameters()
    {
        return m_parameters;
    }

    void init_shader(rend::ShaderLibrary &t_shader_lib) override;

    void update_uniforms(rend::TextureBinder &binder, rend::RenderContext &ctx) override;
};


}  // namespace ay::grph

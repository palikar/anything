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

    bool m_lighting{ true };

    float m_ao_intensity{ 0.0 };
    rend::TexturePtr m_ao_map{ nullptr };

    float m_height_scale{ 0.01 };
    rend::TexturePtr m_height_map{ nullptr };

    glm::vec3 m_color{ 0.0f, 0.0f, 0.0f };

    glm::vec3 m_ambient{ 0.2f, 0.2f, 0.2f };

    float m_shininess{ 30.0f };
    glm::vec3 m_specular{ 0.8f, 0.8f, 0.8f };
    rend::TexturePtr m_specular_map{ nullptr };

    // float m_displ_scale{ 0.0 };
    // float m_displ_bias{ 0.0 };
    // rend::TexturePtr m_displ_map;

    glm::vec3 m_emissive{ 0.0f, 0.0f, 0.0f };
    float m_emissive_scale{ 0.0 };
    rend::TexturePtr m_emissive_map{ nullptr };

    bool m_is_reflection{ true };
    float m_reflectivity{ 0.0f };
    float m_refraction_ration{ 0.98f };
    rend::CubeTexturePtr m_env_map{ nullptr };

    rend::TexturePtr m_map{ nullptr };

    glm::vec2 m_normal_scle{ 1.0, 1.0 };
    rend::TexturePtr m_normal_map{ nullptr };
};

class PhongMaterial : public Material
{

  private:
    PhongParameters m_parameters;

  public:
    PhongMaterial(glm::vec3 color) : m_parameters()
    {
        m_parameters.m_color = color;
    }

    PhongParameters &parameters();

    void init_shader(gmt::ShaderLibrary &t_shader_lib) override;

    void update_uniforms(rend::TextureBinder &binder, rend::RenderContext &ctx) override;

    bool needs_lighting() override;
};

using PhongMaterialPtr = std::unique_ptr<PhongMaterial>;

inline PhongMaterialPtr phong_material(float r, float g, float b)
{
    return std::make_unique<PhongMaterial>(glm::vec3(r, g, b));
}


}  // namespace ay::grph

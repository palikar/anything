#pragma once

#include "graphics/material.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::grph
{


struct SolidColorParameters
{
    glm::vec3 m_color;
    float m_shininess{30.0};
    bool m_lighting{true};
};

class SolidColorMaterial : public Material
{
  private:
    SolidColorParameters m_params;

  public:
    SolidColorMaterial(glm::vec3 t_color, bool t_wireframe = false);

    virtual ~SolidColorMaterial() = default;

    void init_shader(rend::ShaderLibrary &t_shader_lib) override;

    void update_uniforms(rend::TextureBinder &, rend::RenderContext &) override;

    void no_lighting()
    {
        m_params.m_lighting = false;
    }

    void lighting()
    {
        m_params.m_lighting = true;
    }
        
    bool needs_lighting() override
    {
        return m_params.m_lighting;
    }

    SolidColorParameters &params()
    {
        return m_params;
    }

    glm::vec3 &color();

    void set_color(glm::vec3 t_color);
};

inline MaterialPtr solid_color(glm::vec3 color, bool wireframe = false)
{
    return std::make_unique<SolidColorMaterial>(std::move(color), wireframe);
}


inline MaterialPtr solid_color(float r, float g, float b)
{
    return std::make_unique<SolidColorMaterial>(glm::vec3(r, g, b));
}


}  // namespace ay::grph

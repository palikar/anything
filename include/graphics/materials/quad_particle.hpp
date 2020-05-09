#pragma once

#include "graphics/material.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::grph
{


struct QuadParticleParameters
{
    glm::vec3 m_color;
};

class QuadParticleMaterial : public Material
{
  private:
    QuadParticleParameters m_params;

  public:
    QuadParticleMaterial(glm::vec3 t_color = { 1.0, 0.0, 0.0 }, bool t_wireframe = false)
      : m_params{ t_color }
    {
        m_parameters.m_wire_frame = t_wireframe;
    }

    virtual ~QuadParticleMaterial() = default;

    void init_shader(gmt::ShaderLibrary &t_shader_lib) override
    {
        m_shader = t_shader_lib.load("quad_particle");
    }

    void update_uniforms(rend::UniformBinder &,
                         rend::TextureBinder &,
                         rend::RenderContext &) override
    {
    }

    bool needs_lighting() override
    {
        return false;
    }

    QuadParticleParameters &parameters()
    {
        return m_params;
    }

    glm::vec3 &color();

    void set_color(glm::vec3 t_color)
    {
        m_params.m_color = t_color;
    }
};


}  // namespace ay::grph

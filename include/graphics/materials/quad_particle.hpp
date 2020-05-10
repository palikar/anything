#pragma once

#include "graphics/material.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::grph
{


struct QuadParticleParameters
{
    glm::vec3 m_color;

    size_t m_width_segments;
    size_t m_height_segments;

    rend::TexturePtr m_atlas{ nullptr };
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

    void update_uniforms(rend::UniformBinder &uni_binder,
                         rend::TextureBinder &,
                         rend::RenderContext &) override
    {
        uni_binder.set_uniform("width_segments", m_params.m_width_segments);
        uni_binder.set_uniform("height_segments", m_params.m_height_segments);
        uni_binder.set_uniform("atlas", m_params.m_atlas);
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

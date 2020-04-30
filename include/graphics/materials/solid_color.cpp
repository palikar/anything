
#include "graphics/materials/solid_color.hpp"

namespace ay::grph
{


SolidColorMaterial::SolidColorMaterial(glm::vec3 t_color, bool t_wireframe)
  : m_params{ t_color, 1.0 }
{
    m_parameters.m_wire_frame = t_wireframe;
}


void SolidColorMaterial::init_shader(gmt::ShaderLibrary &t_shader_lib)
{
    m_shader = t_shader_lib.load("solid_color");
}

void SolidColorMaterial::update_uniforms(rend::UniformBinder &uniform,
                                         rend::TextureBinder &,
                                         rend::RenderContext &ctx)
{
    uniform.set_uniform("camera_pos", ctx.camera_pos);
    uniform.set_uniform("color", m_params.m_color);
    uniform.set_uniform("specular_exponent", m_params.m_shininess);
}

glm::vec3 &SolidColorMaterial::color()
{
    return m_params.m_color;
}

void SolidColorMaterial::set_color(glm::vec3 t_color)
{
    m_params.m_color = t_color;
}


}  // namespace ay::grph

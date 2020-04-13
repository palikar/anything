
#include "graphics/materials/solid_color.hpp"

namespace ay::grph
{


SolidColorMaterial::SolidColorMaterial(glm::vec3 t_color, bool t_wireframe)
  : m_color(t_color)
{
    m_wire_frame = t_wireframe;
}


void SolidColorMaterial::init_shader(rend::ShaderLibrary &t_shader_lib)
{
    m_shader = t_shader_lib.load("solid_color");
}

void SolidColorMaterial::update_uniforms()
{
    m_shader->set("color", m_color);
}

glm::vec3 &SolidColorMaterial::color()
{
    return m_color;
}

void SolidColorMaterial::set_color(glm::vec3 t_color)
{
    m_color = t_color;
}


}  // namespace ay::grph

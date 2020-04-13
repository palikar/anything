#pragma once

#include "graphics/material.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay
{



class SolidColorMaterial : public Material
{
  private:
    glm::vec3 m_color;

  public:

    SolidColorMaterial(glm::vec3 t_color, bool t_wireframe = false);

    virtual ~SolidColorMaterial() = default;

    void init_shader(ShaderLibrary& t_shader_lib) override;

    void update_uniforms() override;

    glm::vec3& color();

    void set_color(glm::vec3 t_color);
    
};


inline MaterialPtr solid_color(glm::vec3 color)
{
    return std::make_unique<SolidColorMaterial>(std::move(color));
}


}

#pragma once


#include "rendering/shaders.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay
{

class Material
{
  protected:

    bool m_wire_frame;
    ShaderPtr m_shader;

  public:

    Material()
    {}

    virtual ~Material()
    {}

    virtual void init_shader(ShaderLibrary& t_shader_lib) = 0;
    virtual void update_uniforms() = 0;

    void set_wire_frame(bool value) { m_wire_frame = value; }
    
    bool wire_frame() const { return m_wire_frame ; }
    Shader* shader() { return m_shader.get(); }

    template<typename T>
    T* as(){
        return static_cast<T*>(this);
    }
};

using MaterialPtr = std::unique_ptr<Material>;

class SolidColorMaterial : public Material
{
  private:
    glm::vec3 m_color;

  public:

    SolidColorMaterial(glm::vec3 t_color, bool t_wireframe = false) : m_color(t_color)
    {
        m_wire_frame = t_wireframe;
    }

    virtual ~SolidColorMaterial(){};

    void init_shader(ShaderLibrary& t_shader_lib) override
    {
        m_shader = t_shader_lib.load("solid_color");
    }

    void update_uniforms() override
    {
        m_shader->set("color", m_color);
    }

    glm::vec3& color()
    {
        return m_color;
    }

    void set_color(glm::vec3 t_color)
    {
        m_color = t_color;
    }
};



inline MaterialPtr solid_color(glm::vec3 color)
{
    return std::make_unique<SolidColorMaterial>(std::move(color));
}


}

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

}

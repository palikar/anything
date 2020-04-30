#pragma once


#include "rendering/shaders.hpp"
#include "rendering/texture.hpp"
#include "rendering/cube_texture.hpp"
#include "rendering/texture_binder.hpp"

#include "std_header.hpp"

namespace ay::rend
{
using namespace std::string_literals;

class UniformBinder
{


  private:
    Shader *m_shader;
    TextureBinder &m_binder;

  public:
    UniformBinder(rend::TextureBinder &t_binder) : m_binder(t_binder){};

    void change_shader(Shader *shader)
    {
        m_shader = shader;
    }

    template<typename T>
    void set_uniform(const std::string &name, T value)
    {
        m_shader->set(name, value);
    }

    void set_uniform(const std::string &name, CubeTexture *value)
    {
        if (value)
        {
            m_shader->set_sampler(name, m_binder.resolve(value));
            m_shader->set("has_"s += name, true);
            return;
        }
        m_shader->set("has_"s += name, false);
        m_shader->set_sampler(name, m_binder.free_cube_slot());
    }

    void set_uniform(const std::string &name, Texture *value)
    {

        if (value)
        {
            m_shader->set_sampler(name, m_binder.resolve(value));
            m_shader->set("has_"s += name, true);
            return;
        }

        m_shader->set_sampler(name, m_binder.free_2d_slot());
        m_shader->set("has_"s += name, false);
    }
};


}  // namespace ay::rend

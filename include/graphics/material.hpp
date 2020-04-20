#pragma once

#include "rendering/shaders.hpp"
#include "rendering/api.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

#include "rendering/texture_binder.hpp"
#include "rendering/render_context.hpp"


namespace ay::grph
{

// bool m_depth_test;
// bool m_depth_write;
// int m_stencil_write;
// int m_stencil_mask;
// int m_stencil_func;
// int m_stencil_ref;
// int m_stencil_fail;
// int m_stencil_zpass;
// int m_flat_shading;
// int m_fog;


class Material
{
  protected:
    rend::ShaderPtr m_shader;

    float m_opacity{ 1.0 };
    rend::BlendingSetup m_blending{};

    rend::Side m_side{ rend::Side::FRONT };

    bool m_wire_frame{ false };
    bool m_visible{ true };
    bool m_transparent{ false };

    rend::DepthFunc m_depth_func{ GL_LESS };
    bool m_depth_test{ true };
    bool m_depth_write{ true };

  public:
    Material()
    {
    }

    virtual ~Material()
    {
    }

    virtual void init_shader(rend::ShaderLibrary &t_shader_lib)                = 0;
    virtual void update_uniforms(rend::TextureBinder &, rend::RenderContext &) = 0;

    void set_wire_frame(bool value)
    {
        m_wire_frame = value;
    }

    bool wire_frame() const
    {
        return m_wire_frame;
    }

    rend::Shader *shader()
    {
        return m_shader.get();
    }

    template<typename T>
    T *as()
    {
        return static_cast<T *>(this);
    }

    bool &transparent()
    {
        return m_transparent;
    }

    bool &visible()
    {
        return m_visible;
    }

    rend::Side &side()
    {
        return m_side;
    }

    void set_opacity(float val)
    {
        m_opacity = val;
    }

    float opacity()
    {
        return m_opacity;
    }

    bool depth_test()
    {
        return m_depth_test;
    }

    void set_depth_test(bool value)
    {
        m_depth_test = value;
    }

    bool depth_write()
    {
        return m_depth_write;
    }

    void set_depth_write(bool value)
    {
        m_depth_write = value;
    }

    rend::DepthFunc depth_func()
    {
        return m_depth_func;
    }

    void set_depth_func(rend::DepthFunc value)
    {
        m_depth_func = value;
    }

    rend::BlendingSetup &blending_setup()
    {
        return m_blending;
    }
};

using MaterialPtr = std::unique_ptr<Material>;

}  // namespace ay::grph

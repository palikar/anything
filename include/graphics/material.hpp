#pragma once

#include "rendering/shaders.hpp"
#include "rendering/api.hpp"

#include "engine/shader_library.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

#include "rendering/texture_binder.hpp"
#include "rendering/uniform_binder.hpp"
#include "rendering/render_context.hpp"

namespace ay::grph
{

// int m_stencil_write;
// int m_stencil_mask;
// int m_stencil_func;
// int m_stencil_ref;
// int m_stencil_fail;
// int m_stencil_zpass;

// int m_flat_shading;
// int m_fog;

struct MaterialParameters
{
    float m_opacity{ 1.0 };
    rend::BlendingSetup m_blending{};

    rend::Side m_side{ rend::Side::FRONT };

    bool m_wire_frame{ false };
    bool m_visible{ true };
    bool m_transparent{ false };

    rend::DepthFunc m_depth_func{ GL_LESS };
    bool m_depth_test{ true };
    bool m_depth_write{ true };
};

class Material
{
  protected:
    rend::ShaderPtr m_shader;

    MaterialParameters m_parameters;

  public:
    Material()
    {
    }

    virtual ~Material()
    {
    }

    virtual void init_shader(gmt::ShaderLibrary &t_shader_lib) = 0;
    virtual void update_uniforms(rend::UniformBinder &,
                                 rend::TextureBinder &,
                                 rend::RenderContext &)        = 0;
    virtual bool needs_lighting()                              = 0;

    void set_wire_frame(bool value)
    {
        m_parameters.m_wire_frame = value;
    }

    bool wire_frame() const
    {
        return m_parameters.m_wire_frame;
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
        return m_parameters.m_transparent;
    }

    bool &visible()
    {
        return m_parameters.m_visible;
    }

    rend::Side &side()
    {
        return m_parameters.m_side;
    }

    void set_opacity(float val)
    {
        m_parameters.m_opacity = val;
    }

    float opacity()
    {
        return m_parameters.m_opacity;
    }

    bool depth_test()
    {
        return m_parameters.m_depth_test;
    }

    void set_depth_test(bool value)
    {
        m_parameters.m_depth_test = value;
    }

    bool depth_write()
    {
        return m_parameters.m_depth_write;
    }

    void set_depth_write(bool value)
    {
        m_parameters.m_depth_write = value;
    }

    rend::DepthFunc depth_func()
    {
        return m_parameters.m_depth_func;
    }

    void set_depth_func(rend::DepthFunc value)
    {
        m_parameters.m_depth_func = value;
    }

    rend::BlendingSetup &blending_setup()
    {
        return m_parameters.m_blending;
    }

    MaterialParameters &base_parameters()
    {
        return m_parameters;
    }
};

using MaterialPtr = std::unique_ptr<Material>;

}  // namespace ay::grph

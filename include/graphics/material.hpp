#pragma once

#include "rendering/shaders.hpp"
#include "rendering/api.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

#include "rendering/texture_binder.hpp"


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

struct BlendingSetup
{
    float alpha_test{0.0f};
    rend::BlenidngDestination blend_dst{rend::BlenidngDestination::ONE_MINUS_SRC_ALPHA};
    int blend_dst_alpha{0};
    rend::BlenidngEquation blend_equation{rend::BlenidngEquation::ADD};
    int blend_equation_alpha{0};
    rend::BlenidngSource blend_src{rend::BlenidngSource::SRC_ALPHA};
    int blend_src_alpha{0};
    bool blending{false};
};

class Material
{
  protected:
    rend::ShaderPtr m_shader;

    float m_opacity{1.0};
    BlendingSetup m_blending{};
    
    rend::Side m_side;
    
    bool m_wire_frame{false};
    bool m_visible{true};
    bool m_transparent{false};

  public:
    Material()
    {
    }

    virtual ~Material()
    {
    }

    virtual void init_shader(rend::ShaderLibrary &t_shader_lib) = 0;
    virtual void update_uniforms(rend::TextureBinder&)  = 0;

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

    bool& transparent() { return m_transparent; }

    bool& visible() { return m_visible; }

    rend::Side& side() { return m_side; }
    

    void set_opacity(float val) { m_opacity = val;  }
    float opacity() { return m_opacity; }

    BlendingSetup& blending_setup()
    {
        return m_blending;
    }
};

using MaterialPtr = std::unique_ptr<Material>;

}  // namespace ay::grph

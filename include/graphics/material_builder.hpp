#pragma once

#include "rendering/shaders.hpp"
#include "rendering/api.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

#include "rendering/texture_binder.hpp"

#include "graphics/material.hpp"
#include "graphics/materials/solid_color.hpp"
#include "graphics/materials/textured.hpp"


namespace ay::grph
{

class MaterialBuilder
{
  private:
    Material *m_mat;

  public:
    MaterialBuilder(Material *t_mat) : m_mat(t_mat)
    {
    }

    static MaterialBuilder from_existing(Material *mat)
    {
        return MaterialBuilder(mat);
    }

    MaterialBuilder &enable_blending()
    {
        m_mat->blending_setup().blending = true;
        return *this;
    }

    MaterialBuilder &disable_blending()
    {
        m_mat->blending_setup().blending = true;
        return *this;
    }

    MaterialBuilder &back_side()
    {
        m_mat->side() = rend::Side::BACK;
        return *this;
    }

    MaterialBuilder &front_side()
    {
        m_mat->side() = rend::Side::FRONT;
        return *this;
    }

    MaterialBuilder &both_side()
    {
        m_mat->side() = rend::Side::BOTH;
        return *this;
    }

    MaterialBuilder &alpha_test(float alpha_test)
    {
        m_mat->blending_setup().alpha_test = alpha_test;
        return *this;
    }

    MaterialBuilder &blend_dst(rend::BlenidngDestination blend_dst)
    {
        m_mat->blending_setup().blend_dst = blend_dst;
        return *this;
    }

    MaterialBuilder &blend_dst_alpha(rend::BlenidngDestination blend_dst_alpha)
    {
        m_mat->blending_setup().blend_dst_alpha = blend_dst_alpha;
        return *this;
    }

    MaterialBuilder &blend_equation(rend::BlenidngEquation blend_equation)
    {
        m_mat->blending_setup().blend_equation = blend_equation;
        return *this;
    }

    MaterialBuilder &blend_equation_alpha(rend::BlenidngEquation blend_equation_alpha)
    {
        m_mat->blending_setup().blend_equation_alpha = blend_equation_alpha;
        return *this;
    }

    MaterialBuilder &blend_src(rend::BlenidngSource blend_src)
    {
        m_mat->blending_setup().blend_src = blend_src;
        return *this;
    }

    MaterialBuilder &blend_src_alpha(rend::BlenidngSource blend_src_alpha)
    {
        m_mat->blending_setup().blend_src_alpha = blend_src_alpha;
        return *this;
    }

    MaterialBuilder &transparent(bool value = false)
    {
        m_mat->transparent() = value;
        return *this;
    }

    MaterialBuilder &visible(bool value = false)
    {
        m_mat->visible() = value;
        return *this;
    }

    MaterialBuilder &opacity(float value = 1.0)
    {
        m_mat->set_opacity(value);
        return *this;
    }

    MaterialBuilder &wire_frame(bool value = false)
    {
        m_mat->set_wire_frame(value);
        return *this;
    }

    MaterialBuilder &alpha_blending()
    {
        m_mat->blending_setup().blend_equation       = rend::BlenidngEquation::ADD;
        m_mat->blending_setup().blend_equation_alpha = rend::BlenidngEquation::ADD;

        m_mat->blending_setup().blend_src = rend::BlenidngSource::SRC_ALPHA;
        m_mat->blending_setup().blend_dst =
          rend::BlenidngDestination::ONE_MINUS_SRC_ALPHA;

        m_mat->blending_setup().blend_src_alpha = rend::BlenidngSource::SRC_ALPHA;
        m_mat->blending_setup().blend_dst_alpha =
          rend::BlenidngDestination::ONE_MINUS_SRC_ALPHA;

        return *this;
    }

    MaterialBuilder &depth_test(bool value = false)
    {
        m_mat->set_depth_test(value);
        return *this;
    }

    MaterialBuilder &depth_write(bool value = false)
    {
        m_mat->set_depth_write(value);
        return *this;
    }
};


class TexturedMaterialBuilder
{
  private:
    TextureMaterial *m_mat;

  public:
    TexturedMaterialBuilder(TextureMaterial *t_mat) : m_mat(t_mat)
    {
    }

    static TexturedMaterialBuilder from_existing(TextureMaterial *mat)
    {
        return TexturedMaterialBuilder(mat);
    }

    TexturedMaterialBuilder &reflectivity(float value)
    {
        m_mat->set_reflectivity(value);
        return *this;
    }

    TexturedMaterialBuilder &refraction(float value)
    {
        m_mat->set_refraction(value);
        return *this;
    }

    TexturedMaterialBuilder &reflective(bool value = true)
    {
        m_mat->set_is_reflection(value);
        return *this;
    }

    TexturedMaterialBuilder &ao_intensity(bool value = true)
    {
        m_mat->set_is_reflection(value);
        return *this;
    }

    TexturedMaterialBuilder &color(glm::vec3 color)
    {
        m_mat->set_color(color);
        return *this;
    }
};


}  // namespace ay::grph

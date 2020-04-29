#pragma once

#include "rendering/shaders.hpp"
#include "rendering/api.hpp"
#include "rendering/texture.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

#include "rendering/texture_binder.hpp"

#include "graphics/material.hpp"
#include "graphics/materials/solid_color.hpp"
#include "graphics/materials/textured.hpp"
#include "graphics/materials/phong.hpp"


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

    MaterialBuilder &addative_blending()
    {

        m_mat->blending_setup().blend_equation       = rend::BlenidngEquation::ADD;
        m_mat->blending_setup().blend_equation_alpha = rend::BlenidngEquation::ADD;

        m_mat->blending_setup().blend_src = rend::BlenidngSource::ONE;
        m_mat->blending_setup().blend_src_alpha = rend::BlenidngSource::ONE;

        m_mat->blending_setup().blend_dst = rend::BlenidngDestination::ONE;
        m_mat->blending_setup().blend_dst_alpha = rend::BlenidngDestination::ONE;

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

class ColorMaterialBuilder
{
  private:
    SolidColorMaterial *m_mat;

  public:
    ColorMaterialBuilder(SolidColorMaterial *t_mat) : m_mat(t_mat)
    {
    }

    static ColorMaterialBuilder from_existing(SolidColorMaterial *mat)
    {
        return ColorMaterialBuilder(mat);
    }

    void color(float r, float g, float b)
    {
        m_mat->parameters().m_color = {r, g, b};
    }

    void lighting(bool value = true)
    {
        m_mat->parameters().m_lighting = value;
    }

    void shininess(float value = 30.0)
    {
        m_mat->parameters().m_shininess = value;
    }

};

class PhongMaterialBuilder
{
  private:
    PhongMaterial *m_mat;

  public:
    PhongMaterialBuilder(PhongMaterial *t_mat) : m_mat(t_mat)
    {
    }

    static PhongMaterialBuilder from_existing(PhongMaterial *mat)
    {
        return PhongMaterialBuilder(mat);
    }

    void color(float r, float g, float b)
    {
        m_mat->parameters().m_color = {r, g, b};
    }

    void ambient(float r, float g, float b)
    {
        m_mat->parameters().m_ambient = {r, g, b};
    }

    void specular(float r, float g, float b)
    {
        m_mat->parameters().m_specular = {r, g, b};
    }

    void emissive(float r, float g, float b)
    {
        m_mat->parameters().m_emissive = {r, g, b};
    }

    void diffuse_map(rend::TexturePtr tex)
    {
        m_mat->parameters().m_map = std::move(tex);
    }

    void normal_map(rend::TexturePtr tex)
    {
        m_mat->parameters().m_normal_map = std::move(tex);
    }

    void emissive_map(rend::TexturePtr tex)
    {
        m_mat->parameters().m_emissive_map = std::move(tex);
    }

    void specular_map(rend::TexturePtr tex)
    {
        m_mat->parameters().m_specular_map = std::move(tex);
    }

    void height_map(rend::TexturePtr tex)
    {
        m_mat->parameters().m_height_map = std::move(tex);
    }

    void ao_map(rend::TexturePtr tex)
    {
        m_mat->parameters().m_ao_map = std::move(tex);
    }

    void ao_intensity(float value)
    {
        m_mat->parameters().m_ao_intensity = value;
    }

    void shininess(float value = 30.0)
    {
        m_mat->parameters().m_shininess = value;
    }

    void lighting(bool value = true)
    {
        m_mat->parameters().m_lighting = value;
    }

};


}  // namespace ay::grph

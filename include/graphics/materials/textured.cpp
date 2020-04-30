#include "graphics/materials/textured.hpp"

namespace ay::grph
{


TextureMaterial::TextureMaterial(rend::TexturePtr tex) : m_parameters()
{
    m_parameters.m_map = std::move(tex);
}

void TextureMaterial::init_shader(gmt::ShaderLibrary &t_shader_lib)
{
    m_shader = t_shader_lib.load("textured");
}


void TextureMaterial::update_uniforms(rend::UniformBinder &uniform,
                                      rend::TextureBinder &,
                                      rend::RenderContext &ctx)
{


    uniform.set_uniform("mixing", static_cast<int>(m_parameters.m_combine));
    uniform.set_uniform("reflectivity", m_parameters.m_reflectivity);
    uniform.set_uniform("refraction_ration", m_parameters.m_refraction_ration);
    uniform.set_uniform("is_relfection", m_parameters.m_is_reflection);
    uniform.set_uniform("color", m_parameters.m_color);
    uniform.set_uniform("color_intensity", m_parameters.m_color_intensity);
    uniform.set_uniform("ao_intensity", m_parameters.m_ao_intensity);
    uniform.set_uniform("camera_pos", ctx.camera_pos);
    uniform.set_uniform("tex", m_parameters.m_map.get());
    uniform.set_uniform("ao_map", m_parameters.m_ao_map.get());
    uniform.set_uniform("specular_map", m_parameters.m_specular_map.get());
    uniform.set_uniform("alpha_map", m_parameters.m_alpha_map.get());
    uniform.set_uniform("env_map", m_parameters.m_env_map.get());
}


}  // namespace ay::grph

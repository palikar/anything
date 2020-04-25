#include "graphics/materials/textured.hpp"

namespace ay::grph
{


TextureMaterial::TextureMaterial(rend::TexturePtr tex) : m_parameters()
{
    m_parameters.m_map = std::move(tex);
}

void TextureMaterial::init_shader(rend::ShaderLibrary &t_shader_lib)
{
    m_shader = t_shader_lib.load("textured");
}


void TextureMaterial::update_uniforms(rend::TextureBinder &binder,
                                      rend::RenderContext &ctx)
{

    m_shader->set("mixing", static_cast<int>(m_parameters.m_combine));

    m_shader->set("reflectivity", m_parameters.m_reflectivity);
    m_shader->set("refraction_ration", m_parameters.m_refraction_ration);

    m_shader->set("is_relfection", m_parameters.m_is_reflection);

    m_shader->set("color", m_parameters.m_color);
    m_shader->set("color_intensity", m_parameters.m_color_intensity);

    
    m_shader->set("ao_intensity", m_parameters.m_ao_intensity);

    m_shader->set("camera_pos", ctx.camera_pos);

    m_shader->set_sampler("tex", binder.resolve(m_parameters.m_map.get()));

    if (m_parameters.m_ao_map)
    {
        m_shader->set_sampler("ao_map", binder.resolve(m_parameters.m_ao_map.get()));
        m_shader->set("has_ao_map", true);
    }
    else
    {
        m_shader->set("has_ao_map", false);
        m_shader->set_sampler("ao_map", binder.free_2d_slot());
    }

    if (m_parameters.m_specular_map)
    {
        m_shader->set_sampler("specular_map",
                              binder.resolve(m_parameters.m_specular_map.get()));
        m_shader->set("has_specular_map", true );
    }
    else
    {
        m_shader->set_sampler("specular_map", binder.free_2d_slot());
        m_shader->set("has_specular_map", false);
    }

    if (m_parameters.m_alpha_map)
    {
        m_shader->set_sampler("alpha_map",
                              binder.resolve(m_parameters.m_alpha_map.get()));
        m_shader->set("has_alpha_map", true);
    }
    else
    {
        m_shader->set_sampler("alpha_map", binder.free_2d_slot());
        m_shader->set("has_alpha_map", false);
    }


    if (m_parameters.m_env_map)
    {
        m_shader->set("has_env", true);
        m_shader->set_sampler("env_map", binder.resolve(m_parameters.m_env_map.get()));
    }
    else
    {
        m_shader->set_sampler("env_map", binder.free_cube_slot());
        m_shader->set("has_env", false);
    }
}


}  // namespace ay::grph

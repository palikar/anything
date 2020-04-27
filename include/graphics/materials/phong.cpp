#include "graphics/materials/phong.hpp"


namespace ay::grph
{

void PhongMaterial::init_shader(rend::ShaderLibrary &t_shader_lib)
{
    m_shader = t_shader_lib.load("phong");
}

void PhongMaterial::update_uniforms(rend::TextureBinder &binder, rend::RenderContext &ctx)
{
    m_shader->set("camera_pos", ctx.camera_pos);

    m_shader->set("ao_intensity", m_parameters.m_ao_intensity);
    m_shader->set_sampler("ao_map", binder.resolve(m_parameters.m_ao_map.get()));

    m_shader->set("bump_scale", m_parameters.m_bump_scale);
    m_shader->set_sampler("bump_map", binder.resolve(m_parameters.m_bump_map.get()));

    m_shader->set("color", m_parameters.m_color);

    m_shader->set("disp_scale", m_parameters.m_displ_scale);
    m_shader->set("disp_bias", m_parameters.m_displ_bias);
    m_shader->set_sampler("displ_map", binder.resolve(m_parameters.m_displ_map.get()));

    m_shader->set("emissive", m_parameters.m_emissive);
    m_shader->set("emissive_scale", m_parameters.m_emissive_scale);
    m_shader->set_sampler("emissive_map", binder.resolve(m_parameters.m_emissive_map.get()));

    m_shader->set_sampler("env_map", binder.resolve(m_parameters.m_env_map.get()));

    m_shader->set_sampler("map", binder.resolve(m_parameters.m_map.get()));

    m_shader->set_sampler("normal_map", binder.resolve(m_parameters.m_normal_map.get()));

}

bool PhongMaterial::needs_lighting()
{
    return true;
}

PhongParameters &PhongMaterial::parameters()
{

    return m_parameters;
}

}

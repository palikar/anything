#include "graphics/materials/phong.hpp"


namespace ay::grph
{

void PhongMaterial::init_shader(gmt::ShaderLibrary &t_shader_lib)
{
    m_shader = t_shader_lib.load("phong");
}

void PhongMaterial::update_uniforms(rend::TextureBinder &binder, rend::RenderContext &ctx)
{
    m_shader->set("camera_pos", ctx.camera_pos);

    m_shader->set("ao_intensity", m_parameters.m_ao_intensity);

    if (m_parameters.m_ao_map)
    {
        m_shader->set_sampler("ao_map", binder.resolve(m_parameters.m_ao_map.get()));
        m_shader->set("has_ao_map", true);
    }
    else
    {
        m_shader->set_sampler("ao_map", binder.free_2d_slot());
        m_shader->set("has_ao_map", false);
    }

    m_shader->set("height_scale", m_parameters.m_height_scale);

    if (m_parameters.m_height_map)
    {
        const auto slot = binder.resolve(m_parameters.m_height_map.get());
        // std::cout << "bump: " << slot << "\n";
        m_shader->set_sampler("height_map", slot);
        m_shader->set("has_height_map", true);
    }
    else
    {
        m_shader->set_sampler("height_map", binder.free_2d_slot());
        m_shader->set("has_height_map", false);
    }

    m_shader->set("color", m_parameters.m_color);
    m_shader->set("ambient", m_parameters.m_ambient);
    
    m_shader->set("emissive", m_parameters.m_emissive);
    m_shader->set("emissive_scale", m_parameters.m_emissive_scale);

    if (m_parameters.m_emissive_map)
    {
        m_shader->set_sampler("emissive_map",
                              binder.resolve(m_parameters.m_emissive_map.get()));
        m_shader->set("has_emissive_map", true);
    }
    else
    {
        m_shader->set("has_emissive_map", false);
        m_shader->set_sampler("emissive_map", binder.free_2d_slot());
    }

    m_shader->set("is_reflection", m_parameters.m_is_reflection);
    m_shader->set("reflectivity", m_parameters.m_reflectivity);
    m_shader->set("refraction_ration", m_parameters.m_refraction_ration);

    if (m_parameters.m_env_map)
    {
        m_shader->set("has_env", true);
        m_shader->set_sampler("env_map", binder.resolve(m_parameters.m_env_map.get()));
    }
    else
    {
        m_shader->set("has_env", false);
        m_shader->set_sampler("env_map", binder.free_cube_slot());
    }

    m_shader->set("shininess", m_parameters.m_shininess);
    m_shader->set("specular", m_parameters.m_specular);

    if (m_parameters.m_specular_map)
    {
        m_shader->set("has_specular_map", true);
        m_shader->set_sampler("specular_map",
                              binder.resolve(m_parameters.m_specular_map.get()));
    }
    else
    {
        m_shader->set("has_specular_map", false);
        m_shader->set_sampler("specular_map", binder.free_2d_slot());
    }

    if (m_parameters.m_map)
    {
        const auto slot = binder.resolve(m_parameters.m_map.get());
        // std::cout << "map: " << slot << "\n";
        m_shader->set_sampler("map", slot);
        m_shader->set("has_map", true);
    }
    else
    {
        // std::cout << "map: " << binder.free_2d_slot() << "\n";
        m_shader->set_sampler("map", binder.free_2d_slot());
        m_shader->set("has_map", false);
    }

    if (m_parameters.m_normal_map)
    {
        const auto slot = binder.resolve(m_parameters.m_normal_map.get());
        // std::cout << "normal: " << slot << "\n";
        m_shader->set_sampler("normal_map", slot);
        m_shader->set("has_normal_map", true);
    }
    else
    {
        m_shader->set_sampler("normal_map", binder.free_2d_slot());
        m_shader->set("has_normal_map", false);
    }

    // std::cout << "2d free:" <<  binder.free_2d_slot() << "\n";
    // std::cout << "cube free:" <<  binder.free_cube_slot() << "\n";
}

bool PhongMaterial::needs_lighting()
{
    return m_parameters.m_lighting;
}

PhongParameters &PhongMaterial::parameters()
{

    return m_parameters;
}

}  // namespace ay::grph

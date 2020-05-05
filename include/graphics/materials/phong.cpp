#include "graphics/materials/phong.hpp"
#include "graphics/shader_builder/shader_builder.hpp"
#include "graphics/shader_builder/chunk_lib.hpp"

namespace ay::grph
{

void PhongMaterial::init_shader(gmt::ShaderLibrary &t_shader_lib)
{
    if (m_instanced)
    {

        if (t_shader_lib.exists("PhongShader_I"))
        {
            m_shader = t_shader_lib.get("PhongShader_I");
            return;
        }
        
        std::vector<shdr::ShaderChunk> chunks;
        chunks.push_back(shdr::Chunk::define("INSTANCING"));
        chunks.push_back(shdr::Chunk::vertex_from_file(app::ResouceLoader::path("shaders/phong.vert")));
        chunks.push_back(shdr::Chunk::fragment_from_file(app::ResouceLoader::path("shaders/phong.frag")));
        m_shader = shdr::ShaderBuilder::build("PhongShader_I", std::move(chunks));    
        t_shader_lib.add("PhongShader_I", m_shader);
        return;
        
    }

    if (t_shader_lib.exists("PhongShader"))
    {
        m_shader = t_shader_lib.get("PhongShader");
        return;
    }
    
    std::vector<shdr::ShaderChunk> chunks;
    chunks.push_back(shdr::Chunk::vertex_from_file(app::ResouceLoader::path("shaders/phong.vert")));
    chunks.push_back(shdr::Chunk::fragment_from_file(app::ResouceLoader::path("shaders/phong.frag")));
    m_shader = shdr::ShaderBuilder::build("PhongShader", std::move(chunks));    
    t_shader_lib.add("PhongShader", m_shader);
    
    return;

}

void PhongMaterial::update_uniforms(rend::UniformBinder &uniforms,
                                    rend::TextureBinder &,
                                    rend::RenderContext &ctx)
{

    uniforms.set_uniform("camera_pos", ctx.camera_pos);
    uniforms.set_uniform("ao_intensity", m_parameters.m_ao_intensity);
    uniforms.set_uniform("ao_map", m_parameters.m_ao_map.get());
    uniforms.set_uniform("height_scale", m_parameters.m_height_scale);
    uniforms.set_uniform("height_map", m_parameters.m_height_map.get());
    uniforms.set_uniform("color", m_parameters.m_color);
    uniforms.set_uniform("ambient", m_parameters.m_ambient);
    uniforms.set_uniform("emissive", m_parameters.m_emissive);
    uniforms.set_uniform("emissive_scale", m_parameters.m_emissive_scale);
    uniforms.set_uniform("emissive_scale", m_parameters.m_emissive_map.get());
    uniforms.set_uniform("is_reflection", m_parameters.m_is_reflection);
    uniforms.set_uniform("reflectivity", m_parameters.m_reflectivity);
    uniforms.set_uniform("refraction_ration", m_parameters.m_refraction_ration);
    uniforms.set_uniform("env_map", m_parameters.m_env_map.get());
    uniforms.set_uniform("shininess", m_parameters.m_shininess);
    uniforms.set_uniform("specular", m_parameters.m_specular);
    uniforms.set_uniform("specular_map", m_parameters.m_specular_map.get());
    uniforms.set_uniform("map", m_parameters.m_map.get());
    uniforms.set_uniform("normal_map", m_parameters.m_normal_map.get());
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

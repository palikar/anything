#pragma once

#include "application/resource_loader.hpp"
#include "shader_chunk.hpp"

namespace ay::shdr
{



struct Chunk
{

    static ShaderChunk vertex_from_file(const std::string &t_file)
    {
        ShaderChunk chunk;
        chunk.set_file(t_file);
        chunk.stage() = ChunkStage::VERTEX;
        return chunk;
    }

    static ShaderChunk fragment_from_file(const std::string &t_file)
    {

        ShaderChunk chunk;
        chunk.set_file(t_file);
        chunk.stage() = ChunkStage::FRAGMENT;
        return chunk;

    }

    static ShaderChunk define(const std::string &t_value)
    {
        ShaderChunk chunk;
        chunk.set_content("");
        chunk.stage() = ChunkStage::FRAGMENT;
        chunk.add_define(t_value);
        return chunk;
    }
    
    static ShaderChunk begin_main_fragment()
    {
        ShaderChunk chunk;
        chunk.set_content("void main(){");
        chunk.stage() = ChunkStage::FRAGMENT;
        return chunk;
    }

    static ShaderChunk end_main_fragment()
    {
        ShaderChunk chunk;
        chunk.set_content("}");
        chunk.stage() = ChunkStage::FRAGMENT;
        return chunk;
    }

    static ShaderChunk begin_main_vertex()
    {
        ShaderChunk chunk;
        chunk.set_content("void main(){");
        chunk.stage() = ChunkStage::VERTEX;
        return chunk;
    }

    static ShaderChunk end_main_vertex()
    {
        ShaderChunk chunk;
        chunk.set_content("}");
        chunk.stage() = ChunkStage::VERTEX;
        return chunk;
    }

    static ShaderChunk frag_uniform(const std::string &name, rend::ShaderDataType type)
    {
        ShaderChunk chunk;
        chunk.set_content(fmt::format("uniform {} {} ", rend::data_str(type), name));
        chunk.add_provide(fmt::format("uni:{}", name));
        chunk.stage() = ChunkStage::FRAGMENT;
        return chunk;
    }

    static ShaderChunk vertex_uniform(const std::string &name, rend::ShaderDataType type)
    {
        ShaderChunk chunk;
        chunk.set_content(fmt::format("uniform {} {} ", rend::data_str(type), name));
        chunk.add_provide(fmt::format("uni:{}", name));
        chunk.stage() = ChunkStage::VERTEX;
        return chunk;
    }

    static ShaderChunk attribute(const std::string &name, int location, rend::ShaderDataType type)
    {
        ShaderChunk chunk;
        chunk.set_content(fmt::format("layout (location = {}) in {} {}", location, rend::data_str(type), name));
        chunk.add_provide(fmt::format("attr:{}", name));
        chunk.stage() = ChunkStage::VERTEX;
        return chunk;
    }

    // Vertex shader chunks

    static ShaderChunk basic_attributes()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/vertex/basic_attr.glsl"));
        chunk.stage() = ChunkStage::VERTEX;
        chunk.add_provide("attr:i_pos");
        chunk.add_provide("attr:i_norm");
        chunk.add_provide("attr:i_uv");
        return chunk;
    }

    static ShaderChunk extended_attributes()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/vertex/extended_attr.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

        chunk.add_provide("attr:i_pos");
        chunk.add_provide("attr:i_norm");
        chunk.add_provide("attr:i_uv");
        chunk.add_provide("attr:i_tan");
        chunk.add_provide("attr:i_bitan");

        return chunk;
    }

    static ShaderChunk basic_vertex_out()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/vertex/basic_out.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

        chunk.add_provide("out:uv");
        chunk.add_provide("out:pos");
        chunk.add_provide("out:norm");

        return chunk;
    }

    static ShaderChunk extended_vertex_out()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/vertex/extended_out.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

        chunk.add_provide("out:uv");
        chunk.add_provide("out:pos");
        chunk.add_provide("out:norm");
        chunk.add_provide("out:TBN");
        chunk.add_provide("out:tan_pos");
        chunk.add_provide("out:tan_view_pos");
        return chunk;
    }

    static ShaderChunk mvp_uniforms()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/vertex/mvp_uniforms.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

        chunk.add_provide("uni:view_matrix");
        chunk.add_provide("uni:projection_matrix");
        chunk.add_provide("uni:model_matrix");

        return chunk;
    }

    static ShaderChunk basic_mvp_calc()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/vertex/basic_mvp_calc.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

        chunk.add_requirement("attr:i_pos");
        chunk.add_requirement("attr:i_norm");
        chunk.add_requirement("attr:i_uv");

        chunk.add_requirement("out:pos");
        chunk.add_requirement("out:norm");
        chunk.add_requirement("out:uv");

        chunk.add_requirement("uni:view_matrix");
        chunk.add_requirement("uni:projection_matrix");
        chunk.add_requirement("uni:model_matrix");

        return chunk;
    }

    static ShaderChunk tbn_calc()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/vertex/tbn_calc.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

        chunk.add_requirement("uni:model_matrix");

        chunk.add_requirement("attr:i_tan");
        chunk.add_requirement("attr:i_bitan");
        chunk.add_requirement("attr:i_norm");

        chunk.add_requirement("out:TBN");
        chunk.add_requirement("out:tan_pos");
        chunk.add_requirement("out:tan_view_pos");

        chunk.add_provide("var:T");
        chunk.add_provide("var:B");
        chunk.add_provide("var:N");

        return chunk;
    }

    // Fog chunks

    static ShaderChunk fog_in()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/fog/fog_in.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("in:fog_depth");

        return chunk;
    }

    static ShaderChunk fog_out()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/fog/fog_out.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

        chunk.add_provide("out:fog_depth");

        return chunk;
    }

    static ShaderChunk fog_uniforms()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/fog/fog_uniforms.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("uni:fog_type");
        chunk.add_provide("uni:fog_color");
        chunk.add_provide("uni:fog_near");
        chunk.add_provide("uni:fog_far");
        chunk.add_provide("uni:fog_density");

        return chunk;
    }

    static ShaderChunk fog_color_calc()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/fog/fog_color_calc.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("in:fog_depth");
        chunk.add_requirement("uni:fog_type");
        chunk.add_requirement("uni:fog_color");
        chunk.add_requirement("uni:fog_near");
        chunk.add_requirement("uni:fog_far");
        chunk.add_requirement("uni:fog_density");

        return chunk;
    }

    static ShaderChunk fog_depth_calc()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/fog/fog_depth_calc.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

        chunk.add_requirement("out:fog_depth");
        chunk.add_requirement("out:pos");
        chunk.add_requirement("uni:view_matrix");

        return chunk;
    }


    // Alpha test

    static ShaderChunk alpha_test()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/fragment/alpha_test.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;
        chunk.add_provide("alpha_test");
        chunk.add_requirement("out:frag_color");
        chunk.add_requirement("out:alpha_threshold");
        return chunk;
    }


    // Fragment shader building

    static ShaderChunk light_setup()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/lighting/light_setup.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("light_setup");
        chunk.add_provide("uni:lighting_enabled");
        chunk.add_provide("uni:lighting");

        return chunk;
    }

    static ShaderChunk light_functions()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/lighting/light_functions.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("type:blin_phong");
        chunk.add_requirement("light_setup");

        chunk.add_provide("light_functions");

        return chunk;
    }

    static ShaderChunk blin_phong_type()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/blin_phong_type.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("type:blin_phong");

        return chunk;
    }

    static ShaderChunk build_bling_phong()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/build_blin.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("var:ambient_color");
        chunk.add_requirement("var:diffuse_color");
        chunk.add_requirement("var:specular_color");
        chunk.add_requirement("var:specular_strenght");
        chunk.add_requirement("var:shininess_strenght");

        chunk.add_provide("var:material");

        return chunk;
    }

    static ShaderChunk camera_pos_uniform(ChunkStage t_stage = ChunkStage::FRAGMENT)
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/camera_pos.glsl"));
        chunk.stage() = t_stage;

        chunk.add_provide("uni:camera_pos");

        return chunk;
    }

    static ShaderChunk out_color()
    {
        ShaderChunk chunk;
        // chunk.set_file(app::ResouceLoader::path("shaders/chunks/camera_pos.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.set_content("out vec4 frag_color;");
        chunk.add_provide("out:frag_color");

        return chunk;
    }

    static ShaderChunk paralax_functions()
    {

        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/paralax_mapping.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("uni:height_scale");
        chunk.add_requirement("uni:height_map");
        chunk.add_provide("fun:paralax_mapping");

        return chunk;

    }

    // Fragment inputs

    static ShaderChunk basic_fragment_in()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/fragment/basic_in.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("in:uv");
        chunk.add_provide("in:pos");
        chunk.add_provide("in:normal");

        return chunk;
    }

    static ShaderChunk extended_fragment_in()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/fragment/extended_in.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("in:uv");
        chunk.add_provide("in:pos");
        chunk.add_provide("in:normal");
        chunk.add_provide("in:TBN");
        chunk.add_provide("in:tan_pos");
        chunk.add_provide("in:tan_view_pos");

        return chunk;
    }


    // Material unifroms

    static ShaderChunk textured_mat_uniforms()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/textured_mat_uniforms.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("uni:color");
        chunk.add_provide("uni:color_intensity");
        chunk.add_provide("uni:tex");
        chunk.add_provide("uni:has_alpha_map");
        chunk.add_provide("uni:alpha_map");
        chunk.add_provide("uni:has_specular_map");
        chunk.add_provide("uni:specular_map");
        chunk.add_provide("uni:ao_intensity");
        chunk.add_provide("uni:has_ao_map");
        chunk.add_provide("uni:ao_map");
        chunk.add_provide("uni:has_env");
        chunk.add_provide("uni:env_map");
        chunk.add_provide("uni:is_relfection");
        chunk.add_provide("uni:reflectivity");
        chunk.add_provide("uni:refraction_ration");
        chunk.add_provide("uni:mixing");

        return chunk;
    }

    static ShaderChunk basic_material_uniforms()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/basic_mat_uniforms.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("uni:opacity");
        chunk.add_provide("uni:alpha_threshold");
        chunk.add_provide("uni:visible");

        return chunk;
    }

    static ShaderChunk solid_material_uniforms()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/solid_mat_uniforms.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("uni:color");

        return chunk;
    }

    static ShaderChunk phong_material_uniforms()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/phong_mat_uniforms.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("uni:color");
        chunk.add_provide("uni:ambient");

        chunk.add_provide("uni:has_ao_map");
        chunk.add_provide("uni:ao_map");
        chunk.add_provide("uni:ao_intensity");

        chunk.add_provide("uni:height_scale");
        chunk.add_provide("uni:has_height_map");
        chunk.add_provide("uni:height_map");

        chunk.add_provide("uni:emissive");
        chunk.add_provide("uni:emissive_scale");
        chunk.add_provide("uni:has_emissive_map");
        chunk.add_provide("uni:emissive_map");

        chunk.add_provide("uni:has_env");
        chunk.add_provide("uni:env_map");
        chunk.add_provide("uni:is_reflection");
        chunk.add_provide("uni:reflectivity");
        chunk.add_provide("uni:refraction_ration");
        chunk.add_provide("uni:has_map");
        chunk.add_provide("uni:map");
        chunk.add_provide("uni:has_normal_map");
        chunk.add_provide("uni:normal_map");
        chunk.add_provide("uni:shininess");
        chunk.add_provide("uni:specular");
        chunk.add_provide("uni:has_specular_map");
        chunk.add_provide("uni:specular_map");

        return chunk;
    }

    // Material Calculation

    static ShaderChunk basic_mat_calc()
    {

        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/basic_mat_calc.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("uni:opacity");
        chunk.add_provide("var:opacity_strength");

        return chunk;

    }

    static ShaderChunk texture_coords()
    {

        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/texture_coords.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("in:uv");
        chunk.add_requirement("in:tan_pos");
        chunk.add_requirement("in:tan_view_pos");

        chunk.add_requirement("uni:has_height_map");
        chunk.add_requirement("uni:has_height_map");

        chunk.add_provide("var:tex_coords");

        return chunk;

    }

    static ShaderChunk ambient_color()
    {

        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/ambient_color.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("uni:ambient");
        chunk.add_provide("var:ambient_color");

        return chunk;

    }

    static ShaderChunk diffuse_color()
    {

        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/diffuse_color.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("uni:has_map");
        chunk.add_requirement("uni:map");
        chunk.add_requirement("var:tex_coords");
        chunk.add_provide("var:diffuse_color");

        return chunk;

    }

    static ShaderChunk specular_color()
    {

        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/specular_color.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("uni:specular");
        chunk.add_requirement("uni:shininess");
        chunk.add_requirement("uni:has_specular_map");
        chunk.add_requirement("uni:specular_map");
        chunk.add_requirement("var:tex_coords");
        chunk.add_provide("var:specular_color");
        chunk.add_provide("var:specular_strenght");
        chunk.add_provide("var:shininess_strenght");

        return chunk;

    }

    static ShaderChunk normal_mapping()
    {

        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/normal_mapping.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("uni:normal_map");
        chunk.add_requirement("uni:has_normal_map");

        chunk.add_requirement("var:tex_coords");

        chunk.add_requirement("in:TBN");
        chunk.add_requirement("in:normal");

        chunk.add_provide("var:normal");

        return chunk;

    }

    static ShaderChunk emission()
    {

        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/emission.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_define("HAS_EMISSION");

        chunk.add_requirement("uni:emissive");
        chunk.add_requirement("uni:emissive_map");
        chunk.add_requirement("uni:has_emissive_map");

        chunk.add_provide("var:total_emissive");

        return chunk;

    }

    static ShaderChunk phong_lighting()
    {

        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/phong_lighting.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("light_functions");
        chunk.add_requirement("light_setup");

        chunk.add_requirement("uni:lighting");
        chunk.add_requirement("uni:lighting_enabled");
        chunk.add_requirement("uni:camera_pos");

        chunk.add_requirement("var:material");
        chunk.add_requirement("var:normal");

        chunk.add_requirement("in:pos");

        chunk.add_provide("var:outgoing_light");

        return chunk;

    }

    static ShaderChunk ao_mapping()
    {

        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/ao_mapping.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("uni:has_ao_map");
        chunk.add_requirement("uni:ao_map");
        chunk.add_requirement("uni:ao_intensity");

        chunk.add_requirement("var:tex_coords");
        chunk.add_requirement("var:outgoing_light");


        return chunk;

    }

    static ShaderChunk environment()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/environment.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("uni:has_env");
        chunk.add_requirement("uni:env_map");
        chunk.add_requirement("uni:camera_pos");
        chunk.add_requirement("uni:is_reflection");
        chunk.add_requirement("uni:refraction_ration");
        chunk.add_requirement("uni:reflectivity");
        chunk.add_requirement("var:material");
        chunk.add_requirement("var:outgoing_light");


        chunk.add_requirement("in:pos");

        return chunk;

    }

    static ShaderChunk frag_color()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/materials/environment.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("var:opacity_strength");
        chunk.add_requirement("var:outgoing_light");
        chunk.add_requirement("out:frag_color");

        return chunk;

    }


};


};

// auto sh = shdr::ShaderBuilder::build({

// Vertex
// shdr::Chunk::extended_attributes(),
// shdr::Chunk::extended_vertex_out(),

// shdr::Chunk::mvp_uniforms(),
// shdr::Chunk::camera_pos_uniform(shdr::ChunkStage::VERTEX),
// shdr::Chunk::fog_out(),

// shdr::Chunk::begin_main_vertex(),
// shdr::Chunk::basic_mvp_calc(),
// shdr::Chunk::tbn_calc(),

// shdr::Chunk::fog_depth_calc(),
// shdr::Chunk::end_main_vertex(),

// Vertex
// shdr::Chunk::vertex_from_file(app::ResouceLoader::path("shaders/phong.vert")),
// shdr::Chunk::fragment_from_file(app::ResouceLoader::path("shaders/phong.frag"))
// shdr::Chunk::out_color(),
// shdr::Chunk::extended_fragment_in(),
// shdr::Chunk::fog_in(),
// shdr::Chunk::fog_uniforms(),
// shdr::Chunk::basic_material_uniforms(),
// shdr::Chunk::phong_material_uniforms(),
// shdr::Chunk::camera_pos_uniform(shdr::ChunkStage::FRAGMENT),

// shdr::Chunk::light_setup(),
// shdr::Chunk::blin_phong_type(),
// shdr::Chunk::light_functions(),
// shdr::Chunk::paralax_functions(),

// shdr::Chunk::begin_main_fragment(),

// shdr::Chunk::basic_mat_calc(),
// shdr::Chunk::texture_coords(),

// shdr::Chunk::ambient_color(),
// shdr::Chunk::diffuse_color(),
// shdr::Chunk::specular_color(),

// shdr::Chunk::normal_mapping(),

// shdr::Chunk::emission(),

// shdr::Chunk::build_bling_phong(),
// shdr::Chunk::phong_lighting(),

// shdr::Chunk::ao_mapping(),
// shdr::Chunk::environment(),

// shdr::Chunk::frag_color(),

// shdr::Chunk::fog_color_calc(),

// shdr::Chunk::end_main_fragment()

// });

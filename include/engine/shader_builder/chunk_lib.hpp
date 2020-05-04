#pragma once

#include "application/resource_loader.hpp"
#include "shader_chunk.hpp"

namespace ay::shdr
{


struct Chunk
{
    
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

    static ShaderChunk alpha_test()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/alpha_test.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;
        chunk.add_provide("alpha_test");
        chunk.add_requirement("out:frag_color");
        chunk.add_requirement("out:alpha_threshold");
        return chunk;
    }

    static ShaderChunk basic_attributes()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/basic_attr.glsl"));
        chunk.stage() = ChunkStage::VERTEX;
        chunk.add_provide("attr:i_pos");
        chunk.add_provide("attr:i_norm");
        chunk.add_provide("attr:i_uv");
        return chunk;
    }

    static ShaderChunk basic_mvp_calc()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/basic_attr.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

        chunk.add_requirement("attr:i_pos");
        chunk.add_requirement("attr:i_norm");
        chunk.add_requirement("attr:i_uv");

        chunk.add_requirement("out:i_pos");
        chunk.add_requirement("out:i_norm");
        chunk.add_requirement("out:i_uv");
        
        chunk.add_requirement("uni:model_matrix");
        chunk.add_requirement("uni:view_matrix");
        chunk.add_requirement("uni:projeciton_matrix");
        
        return chunk;
    }

    static ShaderChunk basic_fragment_in()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/basic_attr.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

        chunk.add_provide("in:uv");
        chunk.add_provide("in:pos");
        chunk.add_provide("in:normal");
        
        return chunk;
    }

    static ShaderChunk basic_material_uniforms()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/basic_mat_uniforms.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("uni:opacity");
        chunk.add_provide("uni:alpha_threshold");
        chunk.add_provide("uni:visible");
        
        return chunk;
    }

    static ShaderChunk basic_vertex_out()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/basic_out.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

        chunk.add_provide("out:uv");
        chunk.add_provide("out:pos");
        chunk.add_provide("out:norm");
        
        return chunk;
    }

    static ShaderChunk blin_phong_type()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/blin_phong_type.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("type:blin_phong");
        
        return chunk;
    }

    static ShaderChunk build_bling_phong()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/build_blin.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("var:ambient");
        chunk.add_requirement("var:diffuse_color");
        chunk.add_requirement("var:shininess");
        chunk.add_requirement("var:specular");
        chunk.add_requirement("var:specular_strength");
        
        chunk.add_provide("var:material");
        
        return chunk;
    }

    static ShaderChunk camera_pos()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/camera_pos.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("uni:camera_pos");
        
        return chunk;
    }

    static ShaderChunk extended_attributes()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/extended_attr.glsl"));
        chunk.stage() = ChunkStage::VERETEX;

        chunk.add_provide("attr:i_pos");
        chunk.add_provide("attr:i_norm");
        chunk.add_provide("attr:i_uv");
        chunk.add_provide("attr:i_tan");
        chunk.add_provide("attr:i_bitan");
        
        return chunk;
    }

    static ShaderChunk extended_out()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/extended_attr.glsl"));
        chunk.stage() = ChunkStage::VERETEX;

        chunk.add_provide("out:uv");
        chunk.add_provide("out:pos");
        chunk.add_provide("out:norm");
        chunk.add_provide("out:TBN");
        chunk.add_provide("out:tan_pos");
        chunk.add_provide("out:tan_view_pos");

        
        return chunk;
    }

    static ShaderChunk fog_in()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/fog_in.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_provide("in:fog_depth");
        
        return chunk;
    }

    static ShaderChunk fog_out()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/fog_out.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

        chunk.add_provide("out:fog_depth");
        
        return chunk;
    }

    static ShaderChunk fog_uniforms()
    {
        ShaderChunk chunk;
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/fog_out.glsl"));
        chunk.stage() = ChunkStage::VERTEX;

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
        chunk.set_file(app::ResouceLoader::path("shaders/chunks/fogcolor_calc.glsl"));
        chunk.stage() = ChunkStage::FRAGMENT;

        chunk.add_requirement("in:fog_depth");
        chunk.add_requirement("uni:fog_type");
        chunk.add_requirement("uni:fog_color");
        chunk.add_requirement("uni:fog_near");
        chunk.add_requirement("uni:fog_far");
        chunk.add_requirement("uni:fog_density");
        
        return chunk;
    }

    

};


};

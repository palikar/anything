#pragma once


#include "shader_chunk.hpp"

#include "util/files.hpp"
#include "util/logging.hpp"

#include "rendering/shaders.hpp"


#include "std_header.hpp"

namespace ay::shdr
{



class ShaderBuilder
{

  private:
    static constexpr const char* GLSL_VERSION = "#version 460 core";

    std::vector<std::string> m_uniforms;
    std::vector<std::string> m_attributes;
    std::vector<std::string> m_variables;
    std::vector<std::string> m_types;

    void check_features(ShaderChunk &ch, std::vector<std::string> &current)
    {
        for (auto& req : ch.requires_featrues())
        {
            auto res = std::find(current.begin(), current.end(), req);
                    
            if (res == std::end(current))
            {
                AY_ERROR("Missing shader feature");
            }
                    
        }        
    }
    
  public:
    ShaderBuilder()
    {
    }


    rend::ShaderPtr build_shader(std::vector<ShaderChunk> t_chunks)
    {
        std::stringstream vertex;
        std::stringstream fragment;

        std::vector<std::string> vertex_features;
        std::vector<std::string> fragment_features;

        std::stringstream defines;

        for (auto& ch : t_chunks)
        {
            for (auto& def : ch.defines())
            {
                defines << "#define " << def << "\n";
            }
            
            if (ch.stage() == ChunkStage::VERTEX)
            {
                check_features(ch, vertex_features);
                
                vertex << ch.content() << "\n\n";
            }
            else if (ch.stage() == ChunkStage::FRAGMENT)
            {
                check_features(ch, fragment_features);

                std::copy(std::begin(ch.provides()), std::end(ch.provides()), std::back_inserter(fragment_features));
                
                fragment << ch.content() << "\n\n";
            }

        }

        std::stringstream vertex_content;
        vertex_content << GLSL_VERSION << "\n\n";
        vertex_content << defines.str() << "\n\n";
        vertex_content << vertex.str() << "\n\n";

        std::stringstream fragment_content;
        fragment_content << GLSL_VERSION << "\n\n";
        fragment_content << defines.str() << "\n\n";
        fragment_content << fragment.str() << "\n\n";

        
        return std::make_shared<rend::Shader>("", vertex.str(), fragment.str());
    }


    static rend::ShaderPtr build(std::vector<ShaderChunk> t_chunks)
    {
        ShaderBuilder builder;
        return builder.build_shader(std::move(t_chunks));
    }

};


}


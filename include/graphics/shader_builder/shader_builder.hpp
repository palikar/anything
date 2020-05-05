#pragma once


#include "shader_chunk.hpp"

#include "util/files.hpp"
#include "util/logging.hpp"
#include "util/strings.hpp"

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
                AY_ERROR(fmt::format("Missing shader feature: {}", req));
            }

        }
    }

  public:
    ShaderBuilder()
    {
    }


    rend::ShaderPtr build_shader(std::string t_name, std::vector<ShaderChunk> t_chunks)
    {
        AY_INFO(fmt::format("Building shader: {}", t_name));

        std::stringstream vertex;
        std::stringstream fragment;

        std::vector<std::string> vertex_features;
        std::vector<std::string> fragment_features;

        std::stringstream defines;
        defines << "#define BUILDER\n";

        for (auto& ch : t_chunks)
        {
            ch.init();

            for (auto& def : ch.defines())
            {
                defines << "#define " << def << "\n";
            }

            if (ch.stage() == ChunkStage::VERTEX)
            {
                check_features(ch, vertex_features);

                std::copy(std::begin(ch.provides()), std::end(ch.provides()), std::back_inserter(vertex_features));

                vertex << ch.content() << "\n";
            }
            else if (ch.stage() == ChunkStage::FRAGMENT)
            {
                check_features(ch, fragment_features);

                std::copy(std::begin(ch.provides()), std::end(ch.provides()), std::back_inserter(fragment_features));

                fragment << ch.content() << "\n";
            }

            }

        std::stringstream vertex_content;
        vertex_content << GLSL_VERSION << "\n";
        vertex_content << defines.str() << "\n";
        vertex_content << util::replace_all(vertex.str(), "#version 460 core", "") << "\n";

        std::stringstream fragment_content;
        fragment_content << GLSL_VERSION << "\n";
        fragment_content << defines.str() << "\n";
        fragment_content << util::replace_all(fragment.str(), "#version 460 core", "") << "\n";

        // std::cout << "vertex shader:" << "\n";
        // std::cout << vertex_content.str() << "\n";
        // std::cout << "-------------------" << "\n";
        // std::cout << "fragment shader:" << "\n";
        // std::cout << fragment_content.str() << "\n";
        
        return std::make_shared<rend::Shader>(t_name, vertex_content.str(), fragment_content.str());
    }


    static rend::ShaderPtr build(std::string t_name, std::vector<ShaderChunk> t_chunks)
    {
        ShaderBuilder builder;
        return builder.build_shader(std::move(t_name), std::move(t_chunks));
    }

};


}

#pragma once


#include "shader_chunk.hpp"
#include "rendering/shaders.hpp"
#include "rendering/api.hpp"

#include "std_header.hpp"

namespace ay::shdr
{


class ShaderBuilder
{

  private:
    std::vector<std::string> m_uniforms;
    std::vector<std::string> m_attributes;
    std::vector<std::string> m_variables;
    std::vector<std::string> m_types;

    void check_features(ShaderChunk &ch, std::vector<std::string> &current);

  public:
    ShaderBuilder() = default;

    rend::ShaderPtr build_shader(std::string t_name, std::vector<ShaderChunk> t_chunks);

    static rend::ShaderPtr build(std::string t_name, std::vector<ShaderChunk> t_chunks)
    {
        ShaderBuilder builder;
        return builder.build_shader(std::move(t_name), std::move(t_chunks));
    }
};


}  // namespace ay::shdr

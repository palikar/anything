#pragma once

#include "application/resource_loader.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"

#include "util/gl_helpers.hpp"
#include "util/logging.hpp"

namespace ay::rend
{

class Shader
{
  private:
    uint32_t m_id;
    std::string m_name;

  public:
    Shader(const std::string &t_name,
           const std::string &vertex_src,
           const std::string &fragment_src);

    ~Shader();

    const std::string &get_name() const
    {
        return m_name;
    }

    uint32_t id() const
    {
        return m_id;
    }

    const std::string &name() const
    {
        return m_name;
    }

    void reload(const std::string &vertex_src, const std::string &fragment_src)
    {
        std::unordered_map<GLenum, std::string> sources{
            { GL_VERTEX_SHADER, vertex_src }, { GL_FRAGMENT_SHADER, fragment_src }
        };

        compile_program(sources, true);
    }

    void set(const std::string &name, bool value);

    void set(const std::string &name, int value);

    void set(const std::string &name, int *values, uint32_t count);

    void set(const std::string &name, float value);

    void set(const std::string &name, const glm::vec2 &value);

    void set(const std::string &name, const glm::vec3 &value);

    void set(const std::string &name, const glm::vec4 &value);

    void set(const std::string &name, const glm::mat3 &mat);

    void set(const std::string &name, const glm::mat4 &mat);

    void set_sampler(const std::string &name, size_t unit);

    void bind();

    void unbind();

  private:
    GLint get_uniform(const std::string &name);

    std::unordered_map<std::string, GLint> m_uniforms;

    void compile_program(const std::unordered_map<GLenum, std::string> &shader_sources,
                         bool reaload = false);
    GLuint compile_shader(const GLchar *t_src, GLenum type);
};


using ShaderPtr = std::shared_ptr<Shader>;


ShaderPtr load_shader(std::string_view t_name);


}  // namespace ay::rend

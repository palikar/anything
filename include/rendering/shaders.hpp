#pragma once

#include "application/resource_loader.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"

#include "util/gl_helpers.hpp"

namespace ay
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

    void set(const std::string &name, int value);

    void set(const std::string &name, int *values, uint32_t count);

    void set(const std::string &name, float value);

    void set(const std::string &name, const glm::vec2 &value);

    void set(const std::string &name, const glm::vec3 &value);

    void set(const std::string &name, const glm::vec4 &value);

    void set(const std::string &name, const glm::mat3 &mat);

    void set(const std::string &name, const glm::mat4 &mat);

    void bind();

    void unbind();

  private:
    void compile_program(const std::unordered_map<GLenum, std::string> &shader_sources);
    GLuint compile_shader(const GLchar *t_src, GLenum type);
};


using ShaderPtr = std::shared_ptr<Shader>;


ShaderPtr load_shader(std::string_view t_name);


class ShaderLibrary
{
  public:
    void add(const std::string &name, const ShaderPtr &shader);

    void add(const ShaderPtr &shader);

    ShaderPtr load(const std::string &t_name);

    ShaderPtr get(const std::string &name);

    ShaderPtr bind(const std::string &name);

    bool exists(const std::string &name) const;

  private:
    std::unordered_map<std::string, ShaderPtr> m_shaders;
};


}  // namespace ay

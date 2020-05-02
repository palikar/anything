
#include "rendering/shaders.hpp"

#include "util/gl_helpers.hpp"

using namespace std::string_literals;

namespace ay::rend
{

Shader::Shader(const std::string &t_name,
               const std::string &vertex_src,
               const std::string &fragment_src)
  : m_name(t_name)
{

    std::unordered_map<GLenum, std::string> sources{
        { GL_VERTEX_SHADER, vertex_src }, { GL_FRAGMENT_SHADER, fragment_src }
    };

    compile_program(sources);
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

void Shader::set(const std::string &name, bool value)
{
    GLint location = get_uniform(name);
    glUniform1i(location, value ? 1 : 0);
}

void Shader::set(const std::string &name, int value)
{
    GLint location = get_uniform(name);
    glUniform1i(location, value);
}

void Shader::set(const std::string &name, int *values, uint32_t count)
{
    GLint location = get_uniform(name);
    glUniform1iv(location, count, values);
}

void Shader::set(const std::string &name, float value)
{
    GLint location = get_uniform(name);
    glUniform1f(location, value);
}

void Shader::set(const std::string &name, const glm::vec2 &value)
{
    GLint location = get_uniform(name);
    glUniform2f(location, value.x, value.y);
}

void Shader::set(const std::string &name, const glm::vec3 &value)
{
    GLint location = get_uniform(name);
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::set(const std::string &name, const glm::vec4 &value)
{
    GLint location = get_uniform(name);
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::set(const std::string &name, const glm::mat3 &mat)
{
    GLint location = get_uniform(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::set(const std::string &name, const glm::mat4 &mat)
{
    GLint location = get_uniform(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::set_sampler(const std::string &name, size_t unit)
{
    auto location = glGetUniformLocation(m_id, name.c_str());
    glUniform1i(location, unit);
}

GLint Shader::get_uniform(const std::string &name)
{
    auto f = m_uniforms.find(name);
    if (f != m_uniforms.end())
    {
        return f->second;
    }

    GLint location = glGetUniformLocation(m_id, name.c_str());
    m_uniforms.insert({ name, location });
    return location;
}

void Shader::bind()
{
    glUseProgram(m_id);
}

void Shader::unbind()
{
    glUseProgram(0);
}

void Shader::compile_program(
  const std::unordered_map<GLenum, std::string> &shader_sources,
  bool reload)
{

    if (!reload)
    {
        m_id = glCreateProgram();
    }
    else
    {
        glDeleteProgram(m_id);
        m_id = glCreateProgram();
    }

    std::vector<GLenum> shader_ids;
    for (const auto &[type, src] : shader_sources)
    {
        const GLchar *source_cstr = src.c_str();
        auto id                   = compile_shader(source_cstr, type);
        glAttachShader(m_id, id);
        shader_ids.push_back(id);
    }
    glLinkProgram(m_id);

    GLint is_linked = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, (int *)&is_linked);
    if (is_linked == GL_FALSE)
    {
        GLint max_length = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetProgramInfoLog(m_id, max_length, &max_length, &info_log[0]);

        glDeleteProgram(m_id);
        for (auto id : shader_ids)
        {
            glDeleteShader(id);
        }

        AY_DEBUG("Shader m_id linking error:\n\t");
        AY_ERROR(info_log.data());
    }

    glValidateProgram(m_id);
    GLint is_valid = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, (int *)&is_valid);
    if (is_valid == GL_FALSE)
    {
        GLint max_length = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetProgramInfoLog(m_id, max_length, &max_length, &info_log[0]);

        glDeleteProgram(m_id);
        for (auto id : shader_ids)
        {
            glDeleteShader(id);
        }

        AY_DEBUG("Shader m_id validation error:\n\t");
        AY_ERROR(info_log.data());
    }

    for (auto id : shader_ids)
    {
        glDetachShader(m_id, id);
        glDeleteShader(id);
    }

    AY_DEBUG("Compiled program: "s += m_name);
}

GLuint Shader::compile_shader(const GLchar *t_src, GLenum type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &t_src, 0);
    glCompileShader(shader);
    GLint is_compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE)
    {
        GLint max_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);
        glDeleteShader(shader);

        AY_DEBUG("Shader compilation error:\n\t");
        AY_ERROR(info_log.data());
    }


    return shader;
}


ShaderPtr load_shader(std::string_view t_name)
{
    auto [vert, frag] = app::ResouceLoader::get_instance()->get_shader_sources(t_name);
    return std::make_shared<Shader>(std::string(t_name), vert, frag);
}

}  // namespace ay::rend

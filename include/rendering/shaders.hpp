#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>

#include "application/resource_loader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

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
           const std::string &fragment_src) : m_name(t_name)
    {

        std::unordered_map<GLenum, std::string> sources{
            { GL_VERTEX_SHADER, vertex_src }, { GL_FRAGMENT_SHADER, fragment_src }
        };

        compile_program(sources);
    }

    ~Shader()
    {
        glDeleteProgram(m_id);
    }

    const std::string& get_name() const
    {
        return m_name;
    }
    
    void set_int(const std::string& name, int value)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
		glUniform1i(location, value);
    }

    void set_int_array(const std::string& name, int* values, uint32_t count)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
		glUniform1iv(location, count, values);
    }

    void set_float(const std::string& name, float value)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
		glUniform1f(location, value);
    }

    void set_float2(const std::string& name, const glm::vec2& value)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
		glUniform2f(location, value.x, value.y);
    }
    
    void set_float3(const std::string& name, const glm::vec3& value)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
		glUniform2f(location, value.x, value.y);
    }

    void set_float4(const std::string& name, const glm::vec4& value)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
    }

    void set_mat3(const std::string& name, const glm::mat3& mat)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }

    void set_mat4(const std::string& name, const glm::mat4& mat)
    {
        GLint location = glGetUniformLocation(m_id, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }

    void bind(){
        
    }

    void unbind(){

    }

  private:
    void compile_program(const std::unordered_map<GLenum, std::string>& shader_sources)
	{
        m_id = glCreateProgram();
        std::vector<GLenum> shader_ids;
        for (const auto& [type, src] : shader_sources)
        {
            const GLchar* source_cstr = src.c_str();
            auto id = compile_shader(source_cstr, type);
            glAttachShader(m_id, id);
            shader_ids.push_back(id);
        }
        glLinkProgram(m_id);
        glValidateProgram(m_id);

        GLint is_linked = 0;
		glGetProgramiv(m_id, GL_LINK_STATUS, (int*)&is_linked);
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

            std::cout << "Shader m_id linking error:\n\t" << info_log.data() << "\n";
        }

        GLint is_valid = 0;
		glGetProgramiv(m_id, GL_LINK_STATUS, (int*)&is_valid);
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
            std::cout << "Shader m_id validation error:\n\t" << info_log.data() << "\n";
        }
        
        for (auto id : shader_ids)
        {
            glDetachShader(m_id, id);
            glDeleteShader(id);
        }

        std::cout << "Compiled program: " << m_name << "\n";
        
    }

    GLuint compile_shader(const GLchar* t_src, GLenum type)
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
            std::cout << "Shader compilation error:\n\t" << info_log.data() << "\n";
        }
        

        return shader;
    }
};


using ShaderPtr = std::shared_ptr<Shader>;


inline ShaderPtr load_shader(std::string_view t_name)
{
    auto [vert, frag] = ResouceLoader::get_instance()->get_shader_sources(t_name);
    return std::make_shared<Shader>(std::string(t_name), vert, frag);
}


class ShaderLibrary
{
  public:

    void add(const std::string& name, const ShaderPtr& shader)
    {
        m_shaders.insert({name, shader});
    }

    void add(const ShaderPtr& shader)
    {
        m_shaders.insert({shader->get_name(), shader});
    }

    ShaderPtr load(const std::string& t_name)
    {
        auto sh = load_shader(t_name);
        m_shaders.insert({t_name, sh});
        return sh;
    }

    ShaderPtr get(const std::string& name)
    {
        if (m_shaders.count(name) == 0)
        {
            return nullptr;
        }

        return m_shaders.at(name);
    }

    bool exists(const std::string& name) const
    {
        return m_shaders.count(name) == 0;
    }

  private:

    std::unordered_map<std::string, ShaderPtr> m_shaders;
};


}
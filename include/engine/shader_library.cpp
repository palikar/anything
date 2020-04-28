#include "engine/shader_library.hpp"


namespace ay::gmt
{

void ShaderLibrary::add(const std::string &name, const rend::ShaderPtr &shader)
{
    m_shaders.insert({ name, shader });
}

void ShaderLibrary::add(const rend::ShaderPtr &shader)
{
    m_shaders.insert({ shader->get_name(), shader });
}

rend::ShaderPtr ShaderLibrary::load(const std::string &t_name)
{
    if (m_shaders.count(t_name) != 0)
    {
        return m_shaders.at(t_name);
    }
    auto sh = rend::load_shader(t_name);
    m_shaders.insert({ t_name, sh });
    return sh;
}

rend::ShaderPtr ShaderLibrary::get(const std::string &name)
{
    if (m_shaders.count(name) == 0)
    {
        return nullptr;
    }

    return m_shaders.at(name);
}

rend::ShaderPtr ShaderLibrary::bind(const std::string &name)
{
    auto s = get(name);
    s->bind();
    return s;
}

bool ShaderLibrary::exists(const std::string &name) const
{
    return m_shaders.count(name) == 0;
}


}  // namespace ay::gmt

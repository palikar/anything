#include "engine/shader_library.hpp"

#include "application/resource_loader.hpp"
#include "util/logging.hpp"

namespace ay::gmt
{

rend::ShaderPtr ShaderLibrary::add(const std::string &name, const rend::ShaderPtr &shader)
{

    if (m_shaders.count(name) > 0)
    {
        return m_shaders.at(name);
    }

    return m_shaders.insert({ name, shader }).first->second;
}

rend::ShaderPtr ShaderLibrary::add(const rend::ShaderPtr &shader)
{
    return m_shaders.insert({ shader->get_name(), shader }).first->second;
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
    return m_shaders.count(name) > 0;
}

void ShaderLibrary::reload(const std::string &name)
{
    auto [vert, frag] = app::ResouceLoader::get_instance()->get_shader_sources(name);
    m_shaders[name]->reload(vert, frag);
}

void ShaderLibrary::reload_all()
{
    AY_DEBUG("Reloading all shaders");
    for (auto &[name, shader] : m_shaders)
    {
        auto [vert, frag] = app::ResouceLoader::get_instance()->get_shader_sources(name);
        shader->reload(vert, frag);
    }
}

}  // namespace ay::gmt

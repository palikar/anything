#pragma once

#include "application/resource_loader.hpp"

#include "rendering/shaders.hpp"

#include "std_header.hpp"

namespace ay::gmt
{

class ShaderLibrary
{
  public:
    void add(const std::string &name, const rend::ShaderPtr &shader);

    void add(const rend::ShaderPtr &shader);

    rend::ShaderPtr load(const std::string &t_name);

    rend::ShaderPtr get(const std::string &name);

    rend::ShaderPtr bind(const std::string &name);

    bool exists(const std::string &name) const;

    void reload(const std::string &name)
    {
        auto [vert, frag] = app::ResouceLoader::get_instance()->get_shader_sources(name);
        m_shaders[name]->reload(vert, frag);
    }

    void reload_all()
    {
        for (auto &[name, shader] : m_shaders)
        {
            auto [vert, frag] =
              app::ResouceLoader::get_instance()->get_shader_sources(name);
            shader->reload(vert, frag);
        }
    }

  private:
    std::unordered_map<std::string, rend::ShaderPtr> m_shaders;
};


}  // namespace ay::gmt

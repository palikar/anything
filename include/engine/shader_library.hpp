#pragma once

#include "rendering/shaders.hpp"

#include "std_header.hpp"

namespace ay::gmt
{

class ShaderLibrary
{
  public:
    rend::ShaderPtr add(const std::string &name, const rend::ShaderPtr &shader);

    rend::ShaderPtr add(const rend::ShaderPtr &shader);

    rend::ShaderPtr load(const std::string &t_name);

    rend::ShaderPtr get(const std::string &name);

    rend::ShaderPtr bind(const std::string &name);

    bool exists(const std::string &name) const;

    void reload(const std::string &name);

    void reload_all();

  private:
    std::unordered_map<std::string, rend::ShaderPtr> m_shaders;
};


}  // namespace ay::gmt

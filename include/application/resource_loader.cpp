#include "application/resource_loader.hpp"

#include "util/files.hpp"

namespace ay::app
{


void ResouceLoader::init(std::string t_root)
{
    m_root    = fs::absolute(t_root);
    m_shaders = m_root / "shaders";
}

std::string ResouceLoader::get_file_path(std::string_view t_file)
{
    fs::path path = m_root / t_file;
    if (!fs::exists(path))
    {
        AY_DEBUG(fmt::format("Resource does not exist: {}", path.string()));
    }

    return path;
}

std::string ResouceLoader::get_file_text(std::string_view t_file)
{
    fs::path path = m_root / t_file;
    if (!fs::exists(path))
    {
        AY_DEBUG(fmt::format("Resource does not exist: {}", path.string()));
    }

    return files::load_file(path);
}

std::pair<std::string, std::string>
  ResouceLoader::get_shader_files(std::string_view t_shader)
{
    fs::path vertex   = (m_shaders / t_shader) += ".vert";
    fs::path fragment = (m_shaders / t_shader) += ".frag";

    if (!fs::exists(vertex))
    {
        AY_DEBUG(fmt::format("Vertex shader is not found: {}", vertex.string()));
    }

    if (!fs::exists(fragment))
    {
        AY_DEBUG(fmt::format("Fragment shader is not found: {}", vertex.string()));
    }

    return { vertex, fragment };
}

std::pair<std::string, std::string>
  ResouceLoader::get_shader_sources(std::string_view t_shader)
{
    auto [vert, frag] = get_shader_files(t_shader);
    return { files::load_file(vert), files::load_file(frag) };
}

}  // namespace ay::app

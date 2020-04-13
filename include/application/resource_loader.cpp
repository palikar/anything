
#include "application/resource_loader.hpp"
#include "util/files.hpp"

namespace ay::app
{


void ResouceLoader::init(std::string t_root)
{
    m_root    = fs::absolute(t_root);
    m_shaders = m_root / "shaders";
}

std::string ResouceLoader::get_file_text(std::string_view t_file)
{
    fs::path path = m_root / t_file;
    if (!fs::exists(path))
    {
        std::cout << "No such resource: " << path << "\n";
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
        std::cout << "No such resource: " << vertex << "\n";
    }

    if (!fs::exists(fragment))
    {
        std::cout << "No such resource: " << fragment << "\n";
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

#pragma once


#include "std_header.hpp"


namespace ay
{
namespace fs = std::filesystem;

namespace detail
{
    bool skip_bom(std::ifstream &infile);

    bool skip_elf(std::ifstream &infile);

    bool check_elf(const std::string &t_filename);

    std::string load_file(const std::string &t_filename);

}  // namespace detail

class ResouceLoader
{
  private:
    static inline std::unique_ptr<ResouceLoader> m_instance;

    fs::path m_root;
    fs::path m_shaders;

  public:
    static ResouceLoader *get_instance()
    {
        if (!m_instance)
        {
            m_instance = std::make_unique<ResouceLoader>();
        }
        return m_instance.get();
    }

    void init(std::string t_root)
    {
        m_root    = fs::absolute(t_root);
        m_shaders = m_root / "shaders";
    }

    std::string get_file_text(std::string_view t_file)
    {
        fs::path path = m_root / t_file;
        if (!fs::exists(path))
        {
            std::cout << "No such resource: " << path << "\n";
        }

        return detail::load_file(path);
    }

    std::pair<std::string, std::string> get_shader_files(std::string_view t_shader)
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

    std::pair<std::string, std::string> get_shader_sources(std::string_view t_shader)
    {
        auto [vert, frag] = get_shader_files(t_shader);
        return { detail::load_file(vert), detail::load_file(frag) };
    }
};


}  // namespace ay

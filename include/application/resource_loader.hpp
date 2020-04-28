#pragma once

#include "util/logging.hpp"

#include "std_header.hpp"

namespace ay::app
{

namespace fs = std::filesystem;


class ResouceLoader
{
  private:
    static inline std::unique_ptr<ResouceLoader> m_instance;

    fs::path m_root;
    fs::path m_shaders;
    fs::path m_textures;
    fs::path m_cube_textures;
    fs::path m_objs;

  public:
    static ResouceLoader *get_instance()
    {
        if (!m_instance)
        {
            m_instance = std::make_unique<ResouceLoader>();
        }
        return m_instance.get();
    }

    void init(std::string t_root);

    std::string get_file_text(std::string_view t_file);

    std::string get_file_path(std::string_view t_file);

    std::pair<std::string, std::string> get_shader_files(std::string_view t_shader);

    std::pair<std::string, std::string> get_shader_sources(std::string_view t_shader);

    std::string get_texture(std::string_view t_file);

    std::string get_obj(std::string_view t_file);

    std::string get_skybox(std::string_view t_file);
    
    static auto path(std::string_view t_path)
    {
        return get_instance()->get_file_path(t_path);
    }

    static auto skybox(std::string_view t_path)
    {
        return get_instance()->get_skybox(t_path);
    }

    static auto texture(std::string_view t_path)
    {
        return get_instance()->get_texture(t_path);
    }

    static auto obj(std::string_view t_path)
    {
        return get_instance()->get_obj(t_path);
    }

    static auto text(std::string_view t_path)
    {
        return get_instance()->get_file_text(t_path);
    }
};


}  // namespace ay::app

#pragma once


#include "util/gl_helpers.hpp"

#include "rendering/texture.hpp"

#include "util/logging.hpp"

#include "std_header.hpp"
#include <stb_image.h>


namespace ay::rend
{


class CubeTexture
{
  private:
    uint32_t m_id;


    uint32_t m_height;
    uint32_t m_width;

  private:
    unsigned char *load_image(const std::string &t_file);

    void load_from_files(const std::vector<std::string> &t_files, TextureFormat t_format);

  public:
    CubeTexture(std::vector<std::string> t_files,
                TextureFormat t_format = TextureFormat::RGBA)
    {
        load_from_files(t_files, t_format);
    }

    ~CubeTexture()
    {
        GLCall(glDeleteTextures(1, &m_id));
    }

    void bind(uint slot = 0) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_id));
    }

    void Unbind(uint slot = 0) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }

    uint32_t id()
    {
        return m_id;
    }
};

using CubeTexturePtr = std::shared_ptr<CubeTexture>;


inline CubeTexturePtr create_cubetexture_pngs(std::filesystem::path t_folder)
{
    namespace fs = std::filesystem;
    std::vector<std::string> files;

    files.push_back(fs::path(t_folder) /= "left.png");
    files.push_back(fs::path(t_folder) /= "right.png");

    files.push_back(fs::path(t_folder) /= "up.png");
    files.push_back(fs::path(t_folder) /= "down.png");

    files.push_back(fs::path(t_folder) /= "front.png");
    files.push_back(fs::path(t_folder) /= "back.png");

    for (auto &f : files)
    {
        if (!fs::exists(f))
        {
            AY_ERROR("Texture file does not exsits"s += f.str());
            return nullptr;
        }
    }

    return std::make_shared<CubeTexture>(std::move(files), TextureFormat::RGBA);
}

inline CubeTexturePtr create_cubetexture_jpgs(std::filesystem::path t_folder)
{
    namespace fs = std::filesystem;
    std::vector<std::string> files;

    files.push_back(fs::path(t_folder) /= "left.jpg");
    files.push_back(fs::path(t_folder) /= "right.jpg");

    files.push_back(fs::path(t_folder) /= "up.jpg");
    files.push_back(fs::path(t_folder) /= "down.jpg");

    files.push_back(fs::path(t_folder) /= "front.jpg");
    files.push_back(fs::path(t_folder) /= "back.jpg");

    for (auto &f : files)
    {
        if (!fs::exists(f))
        {
            AY_ERROR("Texture file does not exsits"s += f.str());
            return nullptr;
        }
    }

    return std::make_shared<CubeTexture>(std::move(files), TextureFormat::RGB);
}


}  // namespace ay::rend

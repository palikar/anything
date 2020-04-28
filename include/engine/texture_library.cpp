#include "engine/texture_library.hpp"


namespace ay::gmt
{

rend::TexturePtr TextureLibrary::load(const std::string &file, MapType type)
{
    namespace fs = std::filesystem;
    return load(file, fs::path(file).stem(), type);
}

rend::TexturePtr
  TextureLibrary::load(const std::string &file, const std::string &name, MapType type)
{
    auto t = rend::create_texture(app::ResouceLoader::texture(file));
    m_maps.insert({ name, TextureEntry{ type, t } });
    return t;
}

rend::TexturePtr TextureLibrary::get(const std::string &name)
{
    if (m_maps.count(name) > 0)
    {
        return m_maps.at(name).tex;
    }

    return nullptr;
}

std::vector<rend::TexturePtr> TextureLibrary::all_of(MapType type)
{
    std::vector<rend::TexturePtr> textures;
    for (auto &[name, entr] : m_maps)
    {
        if (entr.type == type)
        {
            textures.push_back(entr.tex);
        }
    }

    return textures;
}

std::vector<std::string> TextureLibrary::names_of(MapType type)
{

    std::vector<std::string> textures;
    for (auto &[name, entr] : m_maps)
    {
        if (entr.type == type)
        {
            textures.push_back(name);
        }
    }

    return textures;
}

rend::TexturePtr TextureLibrary::get_indexed(MapType type, size_t index)
{
    auto texs = all_of(type);

    if (index >= texs.size())
    {
        return nullptr;
    }

    return texs[index];
}


}  // namespace ay::gmt

#pragma once


#include "application/resource_loader.hpp"
#include "rendering/texture.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"
#include "macros.hpp"

namespace ay::gmt
{

enum class MapType
{

    DIFFUSE_MAP = 0,
    NORMAL_MAP,
    AO_MAP,
    ALBEDO_MAP,
    HEIGHT_MAP

};

class TextureLibrary
{
    struct TextureEntry
    {
        MapType type;
        rend::TexturePtr tex;
    };

  private:
    std::unordered_map<std::string, TextureEntry> m_maps;


  public:
    TextureLibrary() = default;

    rend::TexturePtr load(const std::string &file, MapType type);

    rend::TexturePtr load(const std::string &file, const std::string &name, MapType type);

    rend::TexturePtr get(const std::string &name);

    std::vector<rend::TexturePtr> all_of(MapType type);

    std::vector<std::string> names_of(MapType type);

    rend::TexturePtr get_indexed(MapType type, size_t index);
};

}  // namespace ay::gmt

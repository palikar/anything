#pragma once

#include "util/gl_helpers.hpp"

#include "rendering/texture.hpp"

#include "macros.hpp"
#include "std_header.hpp"

namespace ay::rend
{


class TextureBinder
{
  private:
    std::unordered_map<uint32_t, uint32_t> m_slots;
    std::vector<uint32_t> m_free_list;
    uint32_t m_free_slot{ 1 };


  public:
    TextureBinder() = default;

    void force_resolve(rend::Texture *t_texture, uint32_t slot);

    template<typename T>
    uint32_t resolve(T *t_texture)
    {

        if (m_slots.count(t_texture->id()) != 0)
        {
            auto slot = m_slots.at(t_texture->id());
            t_texture->bind(slot);
            return slot;
        };

        if (m_slots.size() > 20)
        {
        }

        if (!m_free_list.empty())
        {
            auto slot = m_free_list.back();
            m_slots.insert({ { t_texture->id(), slot } });
            t_texture->bind(slot);
            return slot;
        }

        auto slot = m_free_slot++;
        t_texture->bind(slot);
        m_slots.insert({ { t_texture->id(), slot } });

        return slot;
    }

    void free(rend::Texture *t_texture);
};


}  // namespace ay::rend

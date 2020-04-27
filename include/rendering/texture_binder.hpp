#pragma once

#include "util/gl_helpers.hpp"
#include "util/logging.hpp"

#include "rendering/texture.hpp"
#include "rendering/cube_texture.hpp"

#include "macros.hpp"
#include "std_header.hpp"

namespace ay::rend
{

struct Slot
{
    uint32_t tex_id;
    uint32_t slot_num;
    bool is_cube{ false };
    bool stale{ false };
};


struct SlotField
{
    bool active{ false };
    bool is_cube{ false };
};

class TextureBinder
{
  private:
    std::unordered_map<uint32_t, Slot> m_slots;

    std::array<bool, 32> m_slot_filed;

    std::vector<uint32_t> m_free_list;
    uint32_t m_free_slot{ 1 };

    mutable int undef_cube = -1;
    mutable int undef_2d   = -1;

    template<bool cube, typename T>
    uint32_t resolve(T *t_texture)
    {

        if (m_slots.count(t_texture->id()) != 0)
        {
            auto &slot = m_slots.at(t_texture->id());
            slot.stale = false;
            t_texture->bind(slot.slot_num);
            return slot.slot_num;
        };

        if (m_slots.size() > 29)
        {
            auto it = std::find_if(
              m_slots.begin(), m_slots.end(), [](auto &sl) { return sl.second.stale; });
            m_free_list.push_back(it->second.slot_num);
            m_slots.erase(it);
            AY_ERROR("Using more than 20 texture slots.");
        }


        if (!m_free_list.empty())
        {
            auto slot = m_free_list.back();
            m_slots.insert({ t_texture->id(), { t_texture->id(), slot, cube, false } });
            t_texture->bind(slot);
            m_slot_filed[slot] = true;
            return slot;
        }

        auto slot = m_free_slot++;
        while (slot == undef_2d || slot == undef_cube) {
            ++slot;
        }
        t_texture->bind(slot);
        m_slots.insert({ t_texture->id(), { t_texture->id(), slot, cube, false } });
        m_slot_filed[slot] = true;

        return slot;
    }

  public:
    TextureBinder() = default;

    void force_resolve(rend::Texture *t_texture, uint32_t slot);

    uint32_t resolve(Texture *t_texture)
    {
        return resolve<false>(t_texture);
    }

    uint32_t resolve(CubeTexture *t_texture)
    {
        return resolve<true>(t_texture);
    }

    uint32_t free_cube_slot() const;

    uint32_t free_2d_slot() const;

    void begin_draw_call();
};


}  // namespace ay::rend

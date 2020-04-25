#include "rendering/texture_binder.hpp"

namespace ay::rend
{

void TextureBinder::force_resolve(rend::Texture *t_texture, uint32_t slot)
{
    // m_slots[t_texture->id()] = slot;
    t_texture->bind(slot);
}

uint32_t TextureBinder::free_cube_slot() const
{
    if (undef_cube != -1)
    {
        return undef_cube;
    }

    for (size_t i = 0; i < std::size(m_slot_filed); ++i)
    {
        if (!m_slot_filed[i] and static_cast<int>(i) != undef_2d)
        {
            undef_cube = i;
            return i;
        }
    }


    return 0;
}

uint32_t TextureBinder::free_2d_slot() const
{

    if (undef_2d != -1)
    {
        return undef_2d;
    }


    for (size_t i = 0; i < std::size(m_slot_filed); ++i)
    {
        if (!m_slot_filed[i] and static_cast<int>(i) != undef_cube)
        {

            undef_2d = i;
            return i;
        }
    }

    return 0;
}

void TextureBinder::begin_draw_call()
{
    undef_cube = -1;
    undef_2d   = -1;
    for (auto &[id, slot] : m_slots)
    {
        slot.stale = true;
    }
}


}  // namespace ay::rend

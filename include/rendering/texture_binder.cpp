#include "rendering/texture_binder.hpp"

namespace ay::rend
{

void TextureBinder::free(rend::Texture *t_texture)
{

    if (m_slots.count(t_texture->id()))
    {
        m_free_list.push_back(m_slots.at(t_texture->id()));
        m_slots.erase(m_slots.find(t_texture->id()), m_slots.end());
    }
}


void TextureBinder::force_resolve(rend::Texture *t_texture, uint32_t slot)
{
    m_slots[t_texture->id()] = slot;
    t_texture->bind(slot);
}


}  // namespace ay::rend

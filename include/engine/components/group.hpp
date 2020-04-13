#pragma once


#include "engine/entity.hpp"

#include "std_header.hpp"
#include "macros.hpp"

namespace ay
{

class GroupComponent : public Component
{
  public:
    std::vector<EntityPtr> children;
  public:
    AY_COMPONENT(Mesh)

    GroupComponent() : children()
    {};

    GroupComponent(std::vector<EntityPtr> t_children) : children(std::move(t_children)){};
};



inline std::vector<EntityPtr>& children(Entity* t_entity)
{
    auto p = t_entity->component<GroupComponent>();
    if (!p)
    {}
    return p->children;
}


template<typename ... Args>
inline Entity* add_children(Entity* t_entity, Args ...  t_children)
{
    auto p = t_entity->component<GroupComponent>();
    if (!p)
    {
        t_entity->add_component(std::make_unique<GroupComponent>());
        p = t_entity->component<GroupComponent>();
    }
    
    (t_children->set_game(t_entity->game()), ...);
    (p->children.push_back(std::move(t_children)), ...);

    return t_entity;
}


inline Entity* add_children(Entity* t_entity, std::vector<EntityPtr>& t_children)
{
    auto p = t_entity->component<GroupComponent>();
    if (!p)
    {
        t_entity->add_component(std::make_unique<GroupComponent>());
        p = t_entity->component<GroupComponent>();
    }

    for (auto& ch : t_children)
    {
        ch->set_game(t_entity->game());
        p->children.push_back(std::move(ch));
    }

    return t_entity;
}


}

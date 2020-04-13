#pragma once


#include "engine/entity.hpp"

#include "std_header.hpp"
#include "macros.hpp"

namespace ay::cmp
{

class GroupComponent : public gmt::Component
{
  public:
    std::vector<gmt::EntityPtr> children;

  public:
    AY_COMPONENT(Mesh)

    GroupComponent() : children(){};

    GroupComponent(std::vector<gmt::EntityPtr> t_children)
      : children(std::move(t_children)){};
};


inline std::vector<gmt::EntityPtr> &children(gmt::Entity *t_entity)
{
    auto p = t_entity->component<GroupComponent>();
    if (!p)
    {
    }
    return p->children;
}


template<typename... Args>
inline gmt::Entity *add_children(gmt::Entity *t_entity, Args... t_children)
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


inline gmt::Entity *add_children(gmt::Entity *t_entity,
                                 std::vector<gmt::EntityPtr> &t_children)
{
    auto p = t_entity->component<GroupComponent>();
    if (!p)
    {
        t_entity->add_component(std::make_unique<GroupComponent>());
        p = t_entity->component<GroupComponent>();
    }

    for (auto &ch : t_children)
    {
        ch->set_game(t_entity->game());
        p->children.push_back(std::move(ch));
    }

    return t_entity;
}


}  // namespace ay::cmp

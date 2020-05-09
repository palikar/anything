#pragma once


#include "engine/entity.hpp"

#include "std_header.hpp"
#include "macros.hpp"

namespace ay::cmp
{

class GroupComponent : public gmt::Component
{
  public:
    using construct_type = typename std::vector<gmt::EntityPtr>;


    std::vector<gmt::EntityPtr> children;

    AY_COMPONENT(Mesh)

    GroupComponent() : children(){};

    GroupComponent(std::vector<gmt::EntityPtr> t_children)
      : children(std::move(t_children)){};

    void init(gmt::GameBase *g) override
    {
        for (auto &c : children)
        {
            c->set_game(g, 23);
            c->init(g);
        }
    }
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
        p->children.push_back(std::move(ch));
    }

    return t_entity;
}


}  // namespace ay::cmp

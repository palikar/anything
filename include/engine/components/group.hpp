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
};



inline std::vector<EntityPtr>& children(Entity* t_entity)
{
    auto p = t_entity->component<GroupComponent>();
    if (!p)
    {}
    return p->children;
}


}

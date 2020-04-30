#pragma once

#include "engine/game_base.hpp"
#include "graphics/mesh.hpp"

#include "engine/entity.hpp"
#include "engine/components/transform.hpp"

#include "commons.hpp"
#include "std_header.hpp"
#include "macros.hpp"


namespace ay::cmp
{


class NameComponent : public gmt::Component
{
  public:
    std::string name;

    using construct_type = std::string;

    AY_COMPONENT(Name)

    NameComponent(std::string t_name = "") : name(t_name)
    {
    }

    void init(gmt::GameBase *) override
    {
    }

    void update(double) override
    {
    }
};

inline std::string &name(gmt::Entity *t_entity)
{
    auto p = t_entity->component<NameComponent>();
    if (!p)
    {
    }
    return p->name;
}

}  // namespace ay::cmp

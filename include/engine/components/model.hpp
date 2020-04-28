#pragma once

#include "engine/game_base.hpp"
#include "graphics/model.hpp"

#include "engine/entity.hpp"

#include "commons.hpp"
#include "std_header.hpp"
#include "macros.hpp"

namespace ay::cmp
{

class ModelComponent : public gmt::Component
{
  public:
    grph::Model model;

  public:
    AY_COMPONENT(Mesh)

    ModelComponent(grph::Model t_model) : model(std::move(t_model)){};

    void init(gmt::GameBase *) override
    {
        // mesh.material()->init_shader(t_game->shaders());
    }
};

inline grph::Model &model(gmt::Entity *t_entity)
{
    auto p = t_entity->component<ModelComponent>();
    if (!p)
    {
    }
    return p->model;
}

}  // namespace ay::cmp

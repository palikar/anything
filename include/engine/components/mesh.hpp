#pragma once

#include "engine/game_base.hpp"
#include "graphics/mesh.hpp"

#include "engine/entity.hpp"

#include "commons.hpp"
#include "std_header.hpp"
#include "macros.hpp"

namespace ay::cmp
{

class MeshComponent : public gmt::Component
{
  public:

    using construct_type = grph::Mesh;
    
    grph::Mesh mesh;

  public:
    AY_COMPONENT(Mesh)

    MeshComponent(grph::Mesh t_mesh) : mesh(std::move(t_mesh)){};

    void init(gmt::GameBase *t_game) override
    {
        mesh.material()->init_shader(t_game->shaders());
    }
};

inline grph::Mesh &mesh(gmt::Entity *t_entity)
{
    auto p = t_entity->component<MeshComponent>();
    if (!p)
    {
    }
    return p->mesh;
}

}  // namespace ay::cmp

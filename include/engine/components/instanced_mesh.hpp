#pragma once

#include "engine/game_base.hpp"
#include "graphics/instanced_mesh.hpp"

#include "engine/entity.hpp"

#include "commons.hpp"
#include "std_header.hpp"
#include "macros.hpp"

namespace ay::cmp
{

class InstancedMeshComponent : public gmt::Component
{
  public:
    using construct_type = grph::InstancedMesh;

    grph::InstancedMesh mesh;

    AY_COMPONENT(InstancedMesh)

    InstancedMeshComponent(grph::InstancedMesh t_mesh) : mesh(std::move(t_mesh)){};

    void init(gmt::GameBase *t_game) override
    {
        mesh.material()->make_instanced();
        mesh.material()->init_shader(t_game->shaders());
    }
};

inline grph::InstancedMesh &instanced_mesh(gmt::Entity *t_entity)
{
    auto p = t_entity->component<InstancedMeshComponent>();

    if (!p)
    {
    }

    return p->mesh;
}

}  // namespace ay::cmp

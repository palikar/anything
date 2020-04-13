#pragma once

#include "engine/game_base.hpp"
#include "graphics/mesh.hpp"

#include "engine/entity.hpp"

#include "commons.hpp"
#include "std_header.hpp"
#include "macros.hpp"

namespace ay
{

class MeshComponent : public Component
{
  public:
    Mesh mesh;
  public:
    AY_COMPONENT(Mesh)

    MeshComponent(Mesh t_mesh) : mesh(std::move(t_mesh))
    {};

    void init(GameBase* t_game) override
    {
        mesh.material()->init_shader(t_game->shaders());
        
    }
};

inline Mesh& mesh(Entity* t_entity) {
    auto p = t_entity->component<MeshComponent>();
    if (!p)
    {}
    return p->mesh;
}

}

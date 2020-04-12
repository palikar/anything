#pragma once

#include "std_header.hpp"

#include "application/event.hpp"

#include "engine/component.hpp"

#include "graphics/mesh.hpp"

namespace ay
{




inline EntityPtr mesh_entity(Mesh mesh, std::string name = {})
{
    EntityPtr new_ent = std::make_unique<Entity>(name);
    new_ent->add_component(std::make_unique<TransformComponent>());
    new_ent->add_component(std::make_unique<MeshComponent>(std::move(mesh)));
    return new_ent;
}






}

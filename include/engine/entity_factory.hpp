#pragma once

#include "std_header.hpp"

#include "engine/components/group.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/transform.hpp"

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


inline EntityPtr group_entity(std::vector<EntityPtr> children)
{
    EntityPtr new_ent = std::make_unique<Entity>();
    new_ent->add_component(std::make_unique<TransformComponent>());
    new_ent->add_component(std::make_unique<GroupComponent>(std::move(children)));
    return new_ent;
}


}  // namespace ay

#pragma once

#include "std_header.hpp"

#include "engine/components/group.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/instanced_mesh.hpp"
#include "engine/components/model.hpp"
#include "engine/components/transform.hpp"
#include "engine/components/line_segments.hpp"

#include "graphics/mesh.hpp"

namespace ay::gmt
{

inline EntityPtr mesh_entity(grph::Mesh mesh)
{
    EntityPtr new_ent = std::make_unique<Entity>();
    new_ent->add_component(std::make_unique<cmp::TransformComponent>());
    new_ent->add_component(std::make_unique<cmp::MeshComponent>(std::move(mesh)));
    return new_ent;
}

inline EntityPtr instanced_mesh_entity(grph::InstancedMesh mesh)
{
    EntityPtr new_ent = std::make_unique<Entity>();
    new_ent->add_component<cmp::InstancedMeshComponent>(std::move(mesh));
    return new_ent;
}


inline EntityPtr model_entity(grph::Model model)
{
    EntityPtr new_ent = std::make_unique<Entity>();
    new_ent->add_component(std::make_unique<cmp::TransformComponent>());
    new_ent->add_component(std::make_unique<cmp::ModelComponent>(std::move(model)));
    return new_ent;
}

inline EntityPtr line_segments_entity(grph::LineSegments segs)
{
    EntityPtr new_ent = std::make_unique<Entity>();
    new_ent->add_component(std::make_unique<cmp::TransformComponent>());
    new_ent->add_component(std::make_unique<cmp::LineSegmentsComponent>(std::move(segs)));
    return new_ent;
}

inline EntityPtr group_entity(std::vector<EntityPtr> children)
{
    EntityPtr new_ent = std::make_unique<Entity>();
    new_ent->add_component(std::make_unique<cmp::TransformComponent>());
    new_ent->add_component(std::make_unique<cmp::GroupComponent>(std::move(children)));
    return new_ent;
}

template<typename... Args>
inline EntityPtr group_entity(Args... children)
{
    EntityPtr new_ent = std::make_unique<Entity>();
    new_ent->add_component(std::make_unique<cmp::TransformComponent>());

    new_ent->add_component(std::make_unique<cmp::GroupComponent>());
    cmp::add_children(new_ent.get(), std::move(children)...);

    return new_ent;
}


}  // namespace ay::gmt

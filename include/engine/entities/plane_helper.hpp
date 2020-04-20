#pragma once

#include "application/event.hpp"

#include "graphics/material.hpp"
#include "graphics/material_builder.hpp"
#include "graphics/materials/solid_color.hpp"

#include "engine/entity.hpp"
#include "engine/component.hpp"
#include "engine/game_base.hpp"
#include "engine/entity_factory.hpp"
#include "engine/components/line_segments.hpp"
#include "engine/components/group.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/transform.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::gmt
{


class PlaneHelper : public Entity
{
  private:
    cmp::TransformComponent *m_transform;
    cmp::GroupComponent *m_children;

    glm::vec3 m_color;
    size_t m_size;

  public:
    PlaneHelper(glm::vec3 color = { 1.0f, 0.0f, 0.0f }, size_t size = 5);

    void init_points();

    void update(double) override
    {
    }

    bool event(app::Event &) override
    {
        return false;
    }

    void init(GameBase *) override{};
};


using PlaneHelperPtr = std::unique_ptr<PlaneHelper>;

inline PlaneHelperPtr plane_helper()
{
    return std::make_unique<PlaneHelper>();
}


}  // namespace ay::gmt

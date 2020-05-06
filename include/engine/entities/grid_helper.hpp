#pragma once

#include "graphics/line_segments.hpp"
#include "graphics/material.hpp"
#include "graphics/material_builder.hpp"
#include "graphics/materials/solid_color.hpp"

#include "engine/entity.hpp"
#include "engine/game_base.hpp"
#include "engine/entity_factory.hpp"
#include "engine/component.hpp"
#include "engine/components/line_segments.hpp"
#include "engine/components/group.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/transform.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::gmt
{

class GridHelper : public Entity
{
  private:
    cmp::LineSegmentsComponent *m_line_segments;
    cmp::TransformComponent *m_transform;

    size_t m_size;
    size_t m_divisions;
    glm::vec3 m_color;


  public:
    GridHelper(size_t size      = 10,
               size_t divisions = 10,
               glm::vec3 color  = { 0.7f, 0.7f, 0.7f });

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

using GridHelperPtr = std::unique_ptr<GridHelper>;

template<typename... T>
inline GridHelperPtr grid_helper(T &&... args)
{
    return std::make_unique<GridHelper>(args...);
}


}  // namespace ay::gmt

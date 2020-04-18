#pragma once


#include "application/event.hpp"

#include "engine/game_base.hpp"
#include "engine/component.hpp"
#include "engine/entity.hpp"

#include "graphics/material.hpp"
#include "graphics/material_builder.hpp"
#include "graphics/materials/solid_color.hpp"


#include "engine/entity.hpp"
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


class PlaneHelper : public Entity
{
  private:
    cmp::LineSegmentsComponent *m_line_segments;
    cmp::TransformComponent *m_transform;
    cmp::GroupComponent* m_children;

    glm::vec3 m_color;
    size_t m_size;

  public:
    PlaneHelper(glm::vec3 color = {1.0f, 0.0f, 0.0f}, size_t size = 5)
        : m_color(color), m_size(size)
    {

        m_transform     = add_component(std::make_unique<cmp::TransformComponent>());
        m_children = add_component(std::make_unique<cmp::GroupComponent>());

        init_points();
    }

    void init_points()
    {

        grph::Geometry quad_geom;
        grph::Geometry lines_geom;

        //generate quad
        quad_geom.set_attribute("position", { 1, 1, 0,
                                              -1, 1, 0,
                                              -1, -1, 0,
                                              -1, -1, 0,
                                              1, -1, 0} , 3);

        quad_geom.set_index({2, 1, 0,
                             4, 3, 0});


        //generate lines
        lines_geom.set_attribute("position", { 1, - 1, 0,
                                               - 1, 1, 0,
                                               - 1, - 1, 0,
                                               1, 1, 0,
                                               - 1, 1, 0,
                                               - 1, - 1, 0,
                                               1, - 1, 0,
                                               1, 1, 0,
                                               0, 0, 0,
                                               0, 0, 1} , 3);


        auto mat = grph::solid_color(m_color);
        grph::MaterialBuilder::from_existing(mat.get())
            .opacity(0.2)
            .enable_blending()
            .alpha_blending();

        cmp::add_children(
            this,
            line_segments_entity({ std::move(lines_geom),  grph::solid_color(m_color)}),
            mesh_entity({ std::move(quad_geom), std::move(mat) })
            );

        m_transform->transform.scale() = glm::vec3(m_size*0.5f, m_size*0.5f, 1);

    }

    void update(double) override
    {}

    bool event(app::Event &) override
    {
        return false;
    }

    void init(GameBase *) override{};
};


using PlaneHelperPtr = std::unique_ptr<PlaneHelper>;

PlaneHelperPtr plane_helper()
{
    return std::make_unique<PlaneHelper>();
}


}  // namespace ay::gmt

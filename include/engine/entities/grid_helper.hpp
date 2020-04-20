#pragma once


#include "application/event.hpp"

#include "engine/game_base.hpp"
#include "engine/component.hpp"
#include "engine/entity.hpp"

#include "graphics/line_segments.hpp"
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


class GridHelper : public Entity
{
  private:

    cmp::LineSegmentsComponent *m_line_segments;
    cmp::TransformComponent *m_transform;

    size_t m_size;
    size_t m_divisions;
    glm::vec3 m_color;


  public:
    GridHelper(size_t size=10, size_t divisions=10, glm::vec3 color = {0.7f, 0.7f, 0.7f})
        : m_size(size), m_divisions(divisions), m_color(color)
    {

        m_transform     = add_component(std::make_unique<cmp::TransformComponent>());

        init_points();
    }

    void init_points()
    {

        grph::Geometry lines_geom;


        std::vector<float> verts;
        verts.reserve((m_divisions + 1) * 12);
        
        // float center = m_divisions / 2;
        const float step = m_size / m_divisions;
        const float halfSize = static_cast<float>(m_size) / 2.0f;

        float k = - halfSize;
        for ( size_t i = 0 ; i <= m_divisions; i ++) {

            verts.push_back(-halfSize);
            verts.push_back(0);
            verts.push_back(k);
            verts.push_back(halfSize);
            verts.push_back(0);
            verts.push_back(k);

            verts.push_back(k);
            verts.push_back(0);
            verts.push_back(-halfSize);
            verts.push_back(k);
            verts.push_back(0);
            verts.push_back(halfSize);

            k += step;

        }

        
        lines_geom.set_attribute("position", std::move(verts), 3);
        lines_geom.pack();
        
        m_line_segments = add_component(std::make_unique<cmp::LineSegmentsComponent>(
                                            grph::LineSegments{ std::move(lines_geom), grph::solid_color(m_color) }));
        
        

    }

    void update(double) override
    {}

    bool event(app::Event &) override
    {
        return false;
    }

    void init(GameBase *) override{};
};


using GridHelperPtr = std::unique_ptr<GridHelper>;

template<typename ... T>
GridHelperPtr grid_helper(T && ... args)
{
    return std::make_unique<GridHelper>(args ...);
}


}  // namespace ay::gmt

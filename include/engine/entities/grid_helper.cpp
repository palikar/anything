#include "engine/entities/grid_helper.hpp"

namespace ay::gmt
{

GridHelper::GridHelper(size_t size, size_t divisions, glm::vec3 color)
  : m_size(size), m_divisions(divisions), m_color(color)
{

    m_transform = add_component(std::make_unique<cmp::TransformComponent>());

    init_points();
}

void GridHelper::init_points()
{

    grph::Geometry lines_geom;


    std::vector<float> verts;
    verts.reserve((m_divisions + 1) * 12);

    // float center = m_divisions / 2;
    const float step     = static_cast<float>(m_size) / m_divisions;
    const float halfSize = static_cast<float>(m_size) / 2.0f;

    float k = -halfSize;
    for (size_t i = 0; i <= m_divisions; i++)
    {

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


}  // namespace ay::gmt

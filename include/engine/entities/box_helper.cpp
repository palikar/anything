#include "engine/entities/box_helper.hpp"


namespace ay::gmt
{

BoxHelper::BoxHelper(mth::Box3 t_box, glm::vec3 t_color) : m_box(t_box), m_color(t_color)
{
    m_transform = add_component(std::make_unique<cmp::TransformComponent>());

    init_points();
}


void BoxHelper::init_points()
{

    grph::Geometry lines_geom;

    const std::vector<uint32_t> index{ 0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6,
                                       6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7 };

    const std::vector<float> verts{ 1, 1, 1,  -1, 1, 1,  -1, -1, 1,  1, -1, 1,
                                    1, 1, -1, -1, 1, -1, -1, -1, -1, 1, -1, -1 };


    lines_geom.set_index(index);
    lines_geom.set_attribute("position", std::move(verts), 3);
    lines_geom.pack();
    lines_geom.compute_bounding_sphere();

    m_line_segments = add_component(std::make_unique<cmp::LineSegmentsComponent>(
      grph::LineSegments{ std::move(lines_geom), grph::solid_color(m_color) }));

    m_transform->transform.set_position(m_box.center());
    m_transform->transform.set_scale(m_box.size() * 0.5f);
}


}  // namespace ay::gmt

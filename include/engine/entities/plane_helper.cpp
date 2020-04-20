#include "engine/entities/plane_helper.hpp"

namespace ay::gmt
{


PlaneHelper::PlaneHelper(glm::vec3 color, size_t size)
    : m_color(color), m_size(size)
{

    m_transform     = add_component(std::make_unique<cmp::TransformComponent>());
    m_children = add_component(std::make_unique<cmp::GroupComponent>());

    init_points();
}

void PlaneHelper::init_points()
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
        .both_side()
        .alpha_blending();

    cmp::add_children(
        this,
        line_segments_entity({ std::move(lines_geom),  grph::solid_color(m_color)}),
        mesh_entity({ std::move(quad_geom), std::move(mat) })
        );

    m_transform->transform.scale() = glm::vec3(m_size*0.5f, m_size*0.5f, 1);

}

}

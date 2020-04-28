#include "engine/entities/spotlight_helper.hpp"


namespace ay::gmt
{


SpotlightHelper::SpotlightHelper(float t_distance,
                                 float t_angle,
                                 glm::vec3 t_dir,
                                 glm::vec3 color)
  : m_color(color), m_dir(t_dir), m_distance(t_distance), m_angle(t_angle)
{
    m_transform = add_component(std::make_unique<cmp::TransformComponent>());

    init_points();
}


void SpotlightHelper::init_points()
{

    std::vector<float> verts{ 0,  0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0,  0,
                              -1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, -1, 1 };


    for (size_t i = 0, j = 1, l = 32; i < l; i++, j++)
    {

        const float p1 = (static_cast<float>(i) / l) * mth::PI * 2;
        const float p2 = (static_cast<float>(j) / l) * mth::PI * 2;

        verts.push_back(std::cos(p1));
        verts.push_back(std::sin(p1));
        verts.push_back(1.0f);

        verts.push_back(std::cos(p2));
        verts.push_back(std::sin(p2));
        verts.push_back(1.0f);
    }

    grph::Geometry lines_geom;
    lines_geom.set_attribute("position", std::move(verts), 3);
    lines_geom.pack();

    m_line_segments = add_component<cmp::LineSegmentsComponent>(
      std::move(lines_geom), grph::solid_color(m_color));

    update_tranform();
}

void SpotlightHelper::update_tranform()
{
    float cone_length = m_distance;
    float cone_width  = cone_length * std::tan(m_angle);

    m_transform->transform.set_scale({ cone_width, cone_width, cone_length });

    m_transform->transform.set_rotation(m_dir);
}

void SpotlightHelper::update(float t_distance, float t_angle)
{
    m_distance = t_distance;
    m_angle    = t_angle;
    update_tranform();
}

void SpotlightHelper::dir(glm::vec3 t_dir)
{
    m_dir = t_dir;
    update_tranform();
}

}  // namespace ay::gmt

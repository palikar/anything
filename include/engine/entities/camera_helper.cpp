#include "engine/entities/camera_helper.hpp"

namespace ay::gmt
{


void CameraHelper::add_point(std::string a)
{
    m_point_data->push_back(0);
    m_point_data->push_back(0);
    m_point_data->push_back(0);
    m_points_map[a].push_back(m_point_data->size() - 2 - 1);

    m_line_segments->segments.add_point();
}

void CameraHelper::set_point(const std::string &a, glm::vec3 value)
{
    const auto v = m_proj_mat_inv * glm::vec4(value, 1);

    for (auto p : m_points_map.at(a))
    {
        m_point_data->at(p)     = v.x / v.w;
        m_point_data->at(p + 1) = v.y / v.w;
        m_point_data->at(p + 2) = v.z / v.w;
    }
}

void CameraHelper::add_line(const std::string &a, const std::string &b)
{
    add_point(a);
    add_point(b);
}


CameraHelper::CameraHelper(glm::mat4 t_proj_mat)
  : m_proj_mat(t_proj_mat), m_proj_mat_inv(glm::inverse(m_proj_mat))
{
    m_transform     = add_component(std::make_unique<cmp::TransformComponent>());
    m_line_segments = add_component(std::make_unique<cmp::LineSegmentsComponent>(
      grph::LineSegments(grph::solid_color({ 1.0f, 1.0f, 1.0f }))));
    m_point_data    = &m_line_segments->segments.geometry().attribute("position");


    // m_view = glm::lookAt(m_pos, m_pos + glm::axis(m_rot), glm::vec3(0.0, 1.0,
    // 0.0));

    init_points();
}

void CameraHelper::init_points()
{

    add_line("n1", "n2");
    add_line("n2", "n4");
    add_line("n4", "n3");
    add_line("n3", "n1");

    // far

    add_line("f1", "f2");
    add_line("f2", "f4");
    add_line("f4", "f3");
    add_line("f3", "f1");

    // sides

    add_line("n1", "f1");
    add_line("n2", "f2");
    add_line("n3", "f3");
    add_line("n4", "f4");

    // cone

    add_line("p", "n1");
    add_line("p", "n2");
    add_line("p", "n3");
    add_line("p", "n4");

    // up

    add_line("u1", "u2");
    add_line("u2", "u3");
    add_line("u3", "u1");

    // target

    add_line("c", "t");
    add_line("p", "c");

    // cross

    add_line("cn1", "cn2");
    add_line("cn3", "cn4");

    add_line("cf1", "cf2");
    add_line("cf3", "cf4");
}

void CameraHelper::update(double dt)
{
    if (m_update_components)
    {
        update_components(dt);
    }

    const float w = 1;
    const float h = 1;

    set_point("c", glm::vec3(0, 0, -1));
    set_point("t", glm::vec3(0, 0, 1));

    // near

    set_point("n1", glm::vec3(-w, -h, -1));
    set_point("n2", glm::vec3(w, -h, -1));
    set_point("n3", glm::vec3(-w, h, -1));
    set_point("n4", glm::vec3(w, h, -1));

    // far

    set_point("f1", glm::vec3(-w, -h, 1));
    set_point("f2", glm::vec3(w, -h, 1));
    set_point("f3", glm::vec3(-w, h, 1));
    set_point("f4", glm::vec3(w, h, 1));

    // up

    set_point("u1", glm::vec3(w * 0.7, h * 1.1, -1));
    set_point("u2", glm::vec3(-w * 0.7, h * 1.1, -1));
    set_point("u3", glm::vec3(0, h * 2, -1));

    // cross

    set_point("cf1", glm::vec3(-w, 0, 1));
    set_point("cf2", glm::vec3(w, 0, 1));
    set_point("cf3", glm::vec3(0, -h, 1));
    set_point("cf4", glm::vec3(0, h, 1));

    set_point("cn1", glm::vec3(-w, 0, -1));
    set_point("cn2", glm::vec3(w, 0, -1));
    set_point("cn3", glm::vec3(0, -h, -1));
    set_point("cn4", glm::vec3(0, h, -1));

    m_line_segments->segments.geometry().pack();
}


}  // namespace ay::gmt

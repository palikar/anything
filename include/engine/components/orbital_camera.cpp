#include "engine/components/orbital_camera.hpp"

namespace ay::cmp
{

OrbitalCameraComponent::OrbitalCameraComponent(float t_radius) : m_radius(t_radius)
{
}

OrbitalCameraComponent::OrbitalCameraComponent(gmt::Camera *t_camera, float t_radius)
  : m_camera(t_camera), m_radius(t_radius), m_pos(t_camera->pos())
{
}

bool OrbitalCameraComponent::scroll_radius(app::MouseScrolledEvent &t_e)
{
    const auto diff = m_last_scroll - t_e.y_offset();
    m_radius += diff;
    m_radius      = mth::clamp(m_radius, m_min_radius, m_max_radius);
    m_last_scroll = t_e.x_offset();

    update_rotation();

    return true;
}

void OrbitalCameraComponent::update_rotation()
{
    m_orbit.y = mth::clamp(m_orbit.y, -1.57f, 1.57f);

    if (m_orbit.x > 2 * mth::PI)
    {
        m_orbit.x -= 2 * mth::PI;
    }

    if (m_orbit.x < -2 * mth::PI)
    {
        m_orbit.x += 2 * mth::PI;
    }

    const float camX = -m_radius * sin(m_orbit.x) * cos(m_orbit.y);
    const float camY = -m_radius * sin(m_orbit.y);
    const float camZ = -m_radius * cos(m_orbit.x) * cos(m_orbit.y);

    m_camera->set_look_at(glm::vec3(camX, camY, camZ) + m_pos, m_pos);
}

void OrbitalCameraComponent::update(double dt)
{
    if (app::Input::is_released(MouseCode::BUTTON_1))
    {
        if (m_initial_click)
        {
            app::Input::normal_cursor();
        }
        m_initial_click = false;
    }

    if (app::Input::is_pressed(MouseCode::BUTTON_1))
    {

        const auto pos = app::Input::mouse_pos();
        if (!m_initial_click)
        {
            m_initial_click = true;
            app::Input::camera_cursor();
        }
        else
        {
            const auto diff = m_last_mouse_pos - pos;
            m_orbit += diff * static_cast<float>(dt * m_damping);
            update_rotation();
        }
        m_last_mouse_pos = pos;
    }

    if (app::Input::is_pressed(MouseCode::BUTTON_2))
    {
        const auto pos = app::Input::mouse_pos();

        if (!m_initial_right_click)
        {
            m_initial_right_click = true;
            m_last_mouse_pos      = pos;
            app::Input::camera_cursor();
        }
        else
        {
            const auto diff = m_last_mouse_pos - pos;

            m_pos.x += m_camera->at().x * 1.0 * -mth::clamp(diff.y, -1.0f, 1.0f);
            m_pos.z += m_camera->at().z * 1.0 * -mth::clamp(diff.y, -1.0f, 1.0f);

            auto dir = glm::cross(glm::vec3(0.0, 1.0, 0.0),
                                  glm::vec3(m_camera->at().x, 0, m_camera->at().z));
            m_pos.x += dir.x * 1.0 * -mth::clamp(diff.x, -1.0f, 1.0f);
            m_pos.z += dir.z * 1.0 * -mth::clamp(diff.x, -1.0f, 1.0f);

            update_rotation();

            m_last_mouse_pos = pos;
        }
        m_initial_right_click = true;
    }

    if (app::Input::is_released(MouseCode::BUTTON_2))
    {
        if (m_initial_right_click)
        {
            app::Input::normal_cursor();
        }
        m_initial_right_click = false;
    }

    if (app::Input::is_pressed(KeyCode::W))
    {
        m_pos.x += m_camera->at().x * 1.0;
        m_pos.z += m_camera->at().z * 1.0;
        update_rotation();
    }

    if (app::Input::is_pressed(KeyCode::S))
    {
        m_pos.x -= m_camera->at().x * 1.0;
        m_pos.z -= m_camera->at().z * 1.0;
        update_rotation();
    }

    if (app::Input::is_pressed(KeyCode::A))
    {
        auto dir = glm::cross(glm::vec3(0.0, 1.0, 0.0),
                              glm::vec3(m_camera->at().x, 0, m_camera->at().z));
        m_pos.x += dir.x * 1.0;
        m_pos.z += dir.z * 1.0;
        update_rotation();
    }

    if (app::Input::is_pressed(KeyCode::D))
    {
        auto dir = glm::cross(glm::vec3(0.0, 1.0, 0.0),
                              glm::vec3(m_camera->at().x, 0, m_camera->at().z));
        m_pos.x -= dir.x * 1.0;
        m_pos.z -= dir.z * 1.0;
        update_rotation();
    }
}

bool OrbitalCameraComponent::event(app::Event &e)
{
    app::Dispatcher dispatch{ e };
    dispatch.dispatch<app::MouseScrolledEvent>(
      [this](auto &ev) { return scroll_radius(ev); });
    return false;
}


}  // namespace ay::cmp

#pragma once

#include "engine/camera.hpp"
#include "engine/component.hpp"

#include "math/utils.hpp"

#include "application/input.hpp"

#include "macros.hpp"
#include "glm_header.hpp"

namespace ay::cmp
{

class FloatingCameraComponent : public gmt::Component
{

  private:
    gmt::Camera *m_camera;
    glm::vec3 m_pos{ 0.0f, 0.0f, 0.0f };

    glm::vec2 m_orbit{};

    bool m_initial_click{ false };
    glm::vec2 m_last_mouse_pos{};

    float m_damping{ 0.5 };

    void update_rotation()
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

        // m_camera->rotate(m_camera->right(), m_orbit.y);

        const float camX = -2.0f * sin(m_orbit.x) * cos(m_orbit.y);
        const float camY = -2.0f * sin(m_orbit.y);
        const float camZ = -2.0f * cos(m_orbit.x) * cos(m_orbit.y);
        m_camera->set_look_at(m_pos, m_pos + glm::vec3(camX, camY, camZ));
    }

  public:
    AY_COMPONENT(FloatingCamera)

    FloatingCameraComponent(gmt::Camera *t_camera, glm::vec3 t_pos = { 0.0f, 0.0f, 0.0f })
      : m_camera(t_camera), m_pos(t_pos)
    {
    }

    void update(double dt) override
    {

        if (app::Input::is_released(MouseCode::BUTTON_1))
        {
            m_initial_click = false;
        }

        if (app::Input::is_pressed(MouseCode::BUTTON_1))
        {

            const auto pos = app::Input::mouse_pos();
            if (!m_initial_click)
            {
                m_initial_click = true;
            }
            else
            {
                const auto diff = m_last_mouse_pos - pos;

                m_orbit += diff * static_cast<float>(dt * m_damping);

                update_rotation();
            }
            m_last_mouse_pos = pos;
        }

        if (app::Input::is_pressed(KeyCode::W))
        {
            m_pos += m_camera->at() * 0.5f;
            update_rotation();
        }

        if (app::Input::is_pressed(KeyCode::S))
        {
            m_pos += m_camera->at() * -0.5f;
            update_rotation();
        }

        if (app::Input::is_pressed(KeyCode::A))
        {
            m_pos += m_camera->right() * -0.5f;
            update_rotation();
        }

        if (app::Input::is_pressed(KeyCode::D))
        {
            m_pos += m_camera->right() * 0.5f;
            update_rotation();
        }
    }

    bool event(app::Event &) override
    {
        return false;
    }
};


}  // namespace ay::cmp

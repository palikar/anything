#pragma once

#include "application/input.hpp"

#include "engine/game_base.hpp"
#include "engine/camera.hpp"
#include "graphics/mesh.hpp"

#include "engine/entity.hpp"
#include "engine/component.hpp"
#include "engine/components/transform.hpp"

#include "math/transform.hpp"

#include "commons.hpp"
#include "std_header.hpp"
#include "math_header.hpp"
#include "macros.hpp"


namespace ay::cmp
{


class TrackingCameraComponent : public gmt::Component
{
  private:
    mth::Transform &transform;
    gmt::Camera &camera;

    glm::vec2 m_orbit{ 0.0f, 0.0f };

    float m_last_scroll = 10;

    float m_damping{ 0.3 };
    bool m_initial_click{ false };
    glm::vec2 m_last_mouse_pos{};

    float m_radius     = 10;
    float m_min_radius = 3;
    float m_max_radius = 30;

    void update()
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

        auto pos = transform.position();

        const float camX = -m_radius * sin(m_orbit.x) * cos(m_orbit.y);
        const float camY = -m_radius * sin(m_orbit.y);
        const float camZ = -m_radius * cos(m_orbit.x) * cos(m_orbit.y);

        camera.set_look_at(glm::vec3(camX, camY, camZ) + pos, pos);
    }

    bool scroll_radius(app::MouseScrolledEvent &t_e)
    {

        const auto diff = m_last_scroll - t_e.y_offset();
        m_radius += diff;
        m_radius      = mth::clamp(m_radius, m_min_radius, m_max_radius);
        m_last_scroll = t_e.x_offset();

        update();
        return false;
    }

  public:
    using construct_type = mth::Transform &;

    AY_COMPONENT(TrackingCamera)

    TrackingCameraComponent(mth::Transform &t_transform, gmt::Camera &t_camera)
      : transform(t_transform), camera(t_camera)
    {
    }


    void init(gmt::GameBase *) override
    {
        update();
    }

    bool event(app::Event &e) override
    {
        app::Dispatcher dispatch{ e };
        dispatch.dispatch<app::MouseScrolledEvent>(
          [this](auto &ev) { return scroll_radius(ev); });

        return false;
    }

    void update(double dt) override
    {

        float delta = static_cast<float>(dt);

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
                m_orbit += diff * m_damping * delta;
                update();
            }
            m_last_mouse_pos = pos;
        }


        update();
    }
};


}  // namespace ay::cmp

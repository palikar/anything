#pragma once

#include "engine/camera.hpp"
#include "engine/component.hpp"

#include "math/utils.hpp"

#include "application/input.hpp"

#include "macros.hpp"
#include "glm_header.hpp"

namespace ay::cmp
{

class OrbitalCameraComponent : public gmt::Component
{
  private:
    gmt::Camera *m_camera;
    float m_radius{ 10.0f };
    glm::vec3 m_pos{ 0.0f, 0.0f, 0.0f };
    glm::vec2 m_orbit{};
    glm::vec2 m_last_mouse_pos{};
    bool m_initial_click{ false };
    float m_last_scroll = 10;

    bool m_initial_right_click{ false };

    float m_max_radius{ 100.0f };
    float m_min_radius{ 5.0f };

    float m_max_yangle{ 1.57f };
    float m_min_yangle{ -1.57f };

    float m_damping{ 0.3 };

    bool scroll_radius(app::MouseScrolledEvent &t_e);

    void update_rotation();

  public:
    AY_COMPONENT(OrbitalCamera)

    OrbitalCameraComponent(float t_radius = 15.0f);

    OrbitalCameraComponent(gmt::Camera *t_camera, float t_radius = 15.0f);

    void set_camera(gmt::Camera *t_camera)
    {
        m_camera = t_camera;
    }

    void set_max_radius(float t_rad)
    {
        m_max_radius = t_rad;
    }

    void set_min_radius(float t_rad)
    {
        m_min_radius = t_rad;
    }

    void set_max_yangle(float t_rad)
    {
        m_max_yangle = t_rad;
    }

    void set_min_yangle(float t_rad)
    {
        m_min_yangle = t_rad;
    }

    void set_position(glm::vec3 t_pos)
    {
        m_pos = t_pos;
    }

    void update(double dt) override;

    bool event(app::Event &e) override;
};

}  // namespace ay::cmp

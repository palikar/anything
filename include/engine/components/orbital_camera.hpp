#pragma once

#include "engine/camera.hpp"
#include "engine/component.hpp"

#include "math/utils.hpp"

#include "application/input.hpp"

#include "commons.hpp"
#include "macros.hpp"
#include "glm_header.hpp"

namespace ay
{

class OribalCamearComponent : public Component
{
  private:
    Camera* m_camera;
    float m_radius{10.0f};
    glm::vec3 m_pos{0.0f,0.0f,0.0f};
    glm::vec2 m_orbit;
    glm::vec2 m_last_mouse_pos;
    bool m_initial_click{false};
    float m_last_scroll = 10;

    float m_max_radius{5.0f};
    float m_min_radius{30.0f};

    float m_max_yangle{1.57f};
    float m_min_yangle{-1.57f};

    float m_damping{0.3};

    bool scroll_radius(MouseScrolledEvent& t_e)
    {
        const auto diff = m_last_scroll - t_e.y_offset();
        m_radius += diff;
        m_radius = clamp(m_radius, 5.0f, 30.0f);
        m_last_scroll = t_e.x_offset();

        update_rotation();
        
        return true;
    }

    void update_rotation()
    {
        m_orbit.y = clamp(m_orbit.y, -1.57f, 1.57f);
        m_orbit.x = clamp(m_orbit.x, -2*PI, 2*PI);

        const float camX = -m_radius * sin(m_orbit.x) * cos(m_orbit.y);
        const float camY = -m_radius * sin(m_orbit.y);
        const float camZ = -m_radius * cos(m_orbit.x) * cos(m_orbit.y);
        
        m_camera->set_look_at(glm::vec3(camX, camY, camZ) + m_pos, m_pos);
    }

  public:
    AY_COMPONENT(OrbitalCamera)

    OribalCamearComponent(float t_radius = 15.0f) : m_radius(t_radius){}

    OribalCamearComponent(Camera *t_camera, float t_radius = 15.0f)
      : m_camera(t_camera), m_radius(t_radius)
    {
    }

    void set_camera(Camera *t_camera)
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
    
    void update(double dt) override
    {
        if(Input::is_released(MouseCode::BUTTON_1)) {
            m_initial_click = false;
        }
        
        if(Input::is_pressed(MouseCode::BUTTON_1)) {
            
            const auto pos = Input::mouse_pos();
            if (!m_initial_click) {
                m_initial_click = true;
            } else {
                const auto diff = m_last_mouse_pos - pos;
                m_orbit += diff * static_cast<float>(dt * m_damping);
                update_rotation();
            }
            m_last_mouse_pos = pos;
        }
        
    }

    bool event(Event& e) override
    {
        Dispatcher dispatch{e};
        dispatch.dispatch<MouseScrolledEvent>([this](auto& ev){ return scroll_radius(ev); });
        return false;
    }

};

}

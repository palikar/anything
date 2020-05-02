#pragma once

#include "application/input.hpp"

#include "engine/game_base.hpp"
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


class MovementComponent : public gmt::Component
{
  private:
    mth::Transform &transform;

    glm::vec3 velocity{ 0.0f, 0.0f, 0.0f };

    float speed{ 4.5f };
    bool m_move[4];

    void update()
    {

        if (m_move[0] || m_move[1] || m_move[2] || m_move[3])
        {
            transform.translate(velocity);
            transform.update();
        }
    }

  public:
    using construct_type = mth::Transform &;

    AY_COMPONENT(Movement)

    MovementComponent(mth::Transform &t_transform) : transform(t_transform)
    {
    }

    void init(gmt::GameBase *) override
    {
    }

    bool event(app::Event &) override
    {
        return false;
    }

    void update(double dt) override
    {
        float delta = static_cast<float>(dt);

        if (app::Input::is_pressed(KeyCode::W) and !m_move[0])
        {
            velocity += glm::vec3(0.0f, 0.0f, 1.0f) * delta * speed;
            m_move[0] = true;
        }

        if (app::Input::is_pressed(KeyCode::S) and !m_move[1])
        {
            velocity -= glm::vec3(0.0f, 0.0f, 1.0f) * delta * speed;
            m_move[1] = true;
        }

        if (app::Input::is_pressed(KeyCode::A) and !m_move[2])
        {
            velocity += glm::vec3(1.0f, 0.0f, 0.0f) * delta * speed;
            m_move[2] = true;
        }

        if (app::Input::is_pressed(KeyCode::D) and !m_move[3])
        {
            velocity -= glm::vec3(1.0f, 0.0f, 0.0f) * delta * speed;
            m_move[3] = true;
        }


        if (app::Input::is_released(KeyCode::W) and m_move[0])
        {
            velocity.z = 0;
            m_move[0]  = false;
        }

        if (app::Input::is_released(KeyCode::S) and m_move[1])
        {
            velocity.z = 0;
            m_move[1]  = false;
        }

        if (app::Input::is_released(KeyCode::A) and m_move[2])
        {
            velocity.x = 0;
            m_move[2]  = false;
        }

        if (app::Input::is_released(KeyCode::D) and m_move[3])
        {
            velocity.x = 0;
            m_move[3]  = false;
        }


        update();
    }
};


}  // namespace ay::cmp

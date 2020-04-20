#pragma once


#include "math/transform.hpp"
#include "math/utils.hpp"
#include "commons.hpp"


#include "glm_header.hpp"

namespace ay::gmt
{


class Camera
{
  private:
    glm::mat4 m_projection;
    glm::mat4 m_view;

    glm::vec3 m_pos;
    glm::quat m_rot;

    void update()
    {
        m_view = glm::lookAt(m_pos, m_pos + glm::axis(m_rot), glm::vec3(0.0, 1.0, 0.0));
    }

  public:
    Camera()
      : m_pos(glm::vec3(0, 0, 0))
      , m_rot(glm::angleAxis(0.0f, glm::vec3(0.0f, 0.0f, -1.0f)))
    {

        update();
    }


    void
      init_prescpective_projection(float fov, float aspectRatio, float zNear, float zFar)
    {
        m_projection = glm::perspective(fov, aspectRatio, zNear, zFar);
    }

    void init_orthographic(float left,
                           float right,
                           float bottom,
                           float top,
                           float near,
                           float far)
    {
        m_projection = glm::ortho(left, right, bottom, top, near, far);
    }

    glm::mat4 projection()
    {
        return m_projection;
    }

    glm::mat4 view()
    {
        return m_view;
    }

    glm::mat4 view_projection()
    {
        return m_projection * m_view;
    }

    void move(glm::vec3 dir, float amt)
    {
        m_pos += dir * amt;
        update();
    }

    void rotate(glm::vec3 axis, float amt)
    {
        m_rot *= glm::angleAxis(amt, axis);
        update();
    }

    void set_position(glm::vec3 point)
    {
        m_pos = point;
        update();
    }

    void set_rotation(glm::vec3 r)
    {

        m_rot = glm::angleAxis(glm::radians(180.0f), r);
        update();
    }

    void set_rotation(glm::quat rot)
    {
        m_rot = rot;
        update();
    }

    void set_look_at(glm::vec3 position, glm::vec3 point)
    {

        m_pos = position;
        m_rot = glm::angleAxis(glm::radians(180.0f), glm::normalize(point - position));
        update();
    }

    glm::vec3 pos()
    {
        return m_pos;
    }

    glm::quat rot()
    {
        return m_rot;
    }

    glm::vec3 at()
    {
        return mth::forward(m_rot);
    }

    glm::vec3 up()
    {
        return glm::vec3(0, 1, 0);
    }

    glm::vec3 right()
    {
        return glm::normalize(glm::cross(at(), up()));
    }
};


}  // namespace ay::gmt

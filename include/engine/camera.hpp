#pragma once

#include "application/resource_loader.hpp"
#include "rendering/buffers.hpp"
#include "rendering/vertex_array.hpp"
#include "rendering/shaders.hpp"
#include "math/transform.hpp"
#include "commons.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ay
{

class Camera
{
  private:
    
    glm::mat4 m_projection;
    glm::vec2 m_center;

    Transform m_transform;

  public:

    Camera()
    {
    }

    void init_prescpective_projection(float fov, float aspectRatio, float zNear, float zFar)
    {
        m_projection = glm::perspective(fov, aspectRatio, zNear, zFar);
    }

    void init_orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        m_projection = glm::ortho(left, right, bottom, top, near, far);
    }

    glm::mat4 view_projection()
    {
        glm::mat4 rot = glm::conjugate(m_transform.rotation());
        glm::mat4 pos = m_transform.position() * -1.0f;
        glm::mat4 trans = init_translate(pos);

        return m_projection * ( rot * trans);
    }

    void Move(glm::vec3 dir, float amt)
    {
        m_transform.set_position(m_transform.position() + dir * amt);
    }

    void rotate(glm::vec3 axis, float amt)
    {
        m_transform.rotat(axis, amt);
    }

    

};


}

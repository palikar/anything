#pragma once
#include <iostream>

#include "application/resource_loader.hpp"
#include "rendering/buffers.hpp"
#include "rendering/vertex_array.hpp"
#include "rendering/shaders.hpp"
#include "math/transform.hpp"
#include "commons.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>


namespace ay
{

class Camera
{
  private:
    
    glm::mat4 m_projection;
    glm::vec2 m_center;

    Transform m_transform;

  public:

    Camera() :
        m_transform(glm::vec3(0, 0, 0), glm::angleAxis(0.0f, glm::vec3(0, 1, 0)), glm::vec3(0,0,0))
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
        glm::mat4 rot = glm::mat4(glm::conjugate(m_transform.rotation()));
        glm::vec3 pos = m_transform.position() * -1.0f;
        glm::mat4 trans = init_translate(pos);
        
        return m_projection * ( rot * trans);
    }

    void move(glm::vec3 dir, float amt)
    {
        
        m_transform.set_position(m_transform.position() + (dir * amt));
    }

    void rotate(glm::vec3 axis, float amt)
    {
        m_transform.rotate(axis, amt);
    }

    void set_position(glm::vec3 point)
    {
        m_transform.set_position(point);
    }

    void set_rotation(glm::quat rot)
    {
        m_transform.set_rotation(rot);
    }

    Transform& transform()
    {
        return m_transform;
    }


};


}

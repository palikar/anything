#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "math/utils.hpp"

namespace ay
{

class Transform {
    
  public:

    Transform()
    {}

    void rotate(glm::vec3 axis, float angle)
    {
        m_rotation = glm::normalize(glm::quat(angle, axis) * m_rotation);
    }

    void look_at(glm::vec3 point, glm::vec3 up)
    {
        auto m = init_rotation(glm::normalize((point - m_position)), up);
        m_rotation = glm::quat(m);
    }

    glm::mat4 get_tranformation()
    {
        auto trans = glm::mat4(m_position.x,0,0,0,0,m_position.y,0,0,0,0,m_position.z,0,0,0,0,1);
        auto rot = glm::mat4(m_rotation);
        auto scal = glm::mat4(0,0,0,m_position.x,
                              0,0,0,m_position.y,
                              0,0,0,m_position.z,
                              0,0,0,1);
        
        if (parent) {
            return parent_mat * (trans * (rot * scal));
        }
        
        return (trans * (rot * scal));

    }

    glm::mat4 get_parent() { return parent_mat; }

    void set_parent(Transform* t_parent) {
        parent = t_parent;
    }

    glm::vec3 position() { return m_position; }
    glm::quat rotation() { return m_rotation; }
    glm::vec3 scale() { return m_scale; }

    void set_position(glm::vec3 v) { m_position = v; }
    void set_rotation(glm::quat q) { m_rotation = q; }
    void set_scale(glm::vec3 v) { m_scale = v; }
        
    
  private:
    Transform* parent{nullptr};
    glm::mat4 parent_mat;

    
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;

};

    
}
#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "math/utils.hpp"

namespace ay
{

class Transform {
    
  public:

    Transform(glm::vec3 t_position, glm::quat t_rotation, glm::vec3 t_scale) :
        m_position(t_position),
        m_rotation(t_rotation),
        m_scale(t_scale)
    {}

    Transform():
        m_position(glm::vec3(0,0,0)),
        // m_rotation(glm::angleAxis(glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
        m_rotation(glm::vec3(0.0, 0.0, 0.0)),
        m_scale(glm::vec3(1,1,1))
    {}

    void rotate(glm::vec3 axis, float angle, bool local = false)
    {
        if (local) {
            m_rotation = glm::normalize(glm::quat(angle, m_rotation*glm::normalize(axis)) * m_rotation);
        }else {
            m_rotation = glm::normalize(glm::quat(angle, glm::normalize(axis)) * m_rotation);
        }
    }

    void move(glm::vec3 axis, float amt)
    {
        m_position = glm::normalize(axis)*amt + m_position;
    }

    void look_at(glm::vec3 point, glm::vec3 up)
    {
        auto m = init_rotation(glm::normalize((point - m_position)), up);
        m_rotation = glm::quat(m);
    }

    glm::mat4 get_tranformation()
    {
        auto trans = glm::translate(glm::mat4(1.0f), m_position);
        auto rot = glm::toMat4(m_rotation);
        auto scal = glm::scale(glm::mat4(1.0f), m_scale);
        
        if (parent) {
            return parent_mat * (trans * rot * scal);
        }
        return trans * rot * scal;
    }

    glm::mat4 get_parent() { return parent_mat; }

    void set_parent(Transform* t_parent) {
        parent = t_parent;
    }

    glm::vec3& position() { return m_position; }
    glm::quat& rotation() { return m_rotation; }
    glm::vec3& scale() { return m_scale; }

    const glm::vec3& position() const { return m_position; }
    const glm::quat& rotation() const { return m_rotation; }
    const glm::vec3& scale() const { return m_scale; }

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

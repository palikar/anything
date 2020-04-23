#pragma once


#include "glm_header.hpp"
#include "std_header.hpp"

#include <glm/gtc/matrix_inverse.hpp>

namespace ay::mth
{

class Ray
{
  private:
    glm::vec3 m_origin;
    glm::vec3 m_dir;

  public:
    Ray(glm::vec3 t_origin, glm::vec3 t_dir) : m_origin(t_origin), m_dir(t_dir)
    {
    }

    const glm::vec3 &origin() const
    {
        return m_origin;
    }

    const glm::vec3 &dir() const
    {
        return m_dir;
    }

    glm::vec3 at(float t) const
    {
        return m_dir * t + m_origin;
    }

    glm::vec3 lookAt(glm::vec3 v) const
    {
        return glm::normalize(v - m_origin);
    }

    float distance_sq_to_point(glm::vec3 p) const
    {
        auto directionDistance = glm::dot(m_dir, p - m_origin);

        if (directionDistance < 0)
        {
            return glm::length2(p - m_origin);
        }

        return glm::length2(m_dir * directionDistance + m_origin);
    }

    Ray &recast(float t)
    {
        m_origin = at(t);
        return *this;
    }

    Ray transform(const glm::mat4& matr)
    {
        
        auto n = glm::mat3(glm::inverseTranspose(matr));
        auto o = matr * glm::vec4(m_origin, 1.0);
        return {{o.x, o.y, o.z}, glm::normalize(n * m_dir) };
    }
};


}  // namespace ay::mth

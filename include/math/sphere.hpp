#pragma once


#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::mth
{

class Sphere
{
  private:
    glm::vec3 m_center;
    float m_radius;

  public:
    Sphere(glm::vec3 t_center = glm::vec3{ 0.0, 0.0, 0.0 }, float t_radius = 0.0)
      : m_center(t_center), m_radius(t_radius)
    {
    }

    const glm::vec3 &center() const
    {
        return m_center;
    }

    const float &radius() const
    {
        return m_radius;
    }

    void set_radius(float r)
    {
        m_radius = r;
    }

    void set_center(glm::vec3 v)
    {
        m_center = v;
    }

    float distance_to_point(glm::vec3 point) const
    {
        return glm::length(m_center - point) - m_radius;
    }

    bool contains_point(glm::vec3 point) const
    {
        return glm::length2(point - m_center) <= m_radius * m_radius;
    }

    bool intersects_sphere(Sphere sphere) const
    {
        const float radiusSum = m_radius + sphere.m_radius;
        return glm::length2(sphere.m_center - m_center) <= (radiusSum * radiusSum);
    };

    Sphere &expand_by_point(glm::vec3 p)
    {
        auto l = glm::distance(m_center, p);

        if (m_radius < l)
        {
            m_radius = l;
        }

        return *this;
    };
};


}  // namespace ay::mth

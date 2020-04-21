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

    glm::vec3 &center()
    {
        return m_center;
    }

    float &radius()
    {
        return m_radius;
    }

    float distance_to_point(glm::vec3 point)
    {
        return glm::length(m_center - point) - m_radius;
    }

    bool contains_point(glm::vec3 point)
    {
        return glm::length2(point - m_center) <= m_radius * m_radius;
    }

    bool intersects_sphere(Sphere sphere)
    {
        const float radiusSum = m_radius + sphere.m_radius;
        return glm::length2(sphere.m_center - m_center) <= (radiusSum * radiusSum);
    };

    Sphere &expand_by_point(glm::vec3 p)
    {
        auto l = glm::length(m_center - p);
        if (l < m_radius)
        {
            m_radius = l;
        }

        return *this;
    };
};


}  // namespace ay::mth

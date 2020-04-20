#pragma once

#include "math/sphere.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::mth
{

class Plane
{

  private:
    glm::vec3 m_normal;
    float m_constant;

  public:
    Plane(glm::vec3 t_normal, float t_const) : m_normal(t_normal), m_constant(t_const)
    {
    }

    glm::vec3 &normal()
    {
        return m_normal;
    }

    float &constant()
    {
        return m_constant;
    }

    void set(glm::vec3 t_normal, float t_const)
    {
        m_normal   = t_normal;
        m_constant = t_const;
    }

    void set(float x, float y, float z, float t_const)
    {
        m_normal   = glm::vec3(x, y, z);
        m_constant = t_const;
    }

    Plane &normalize()
    {
        float inverseNormalLength = 1.0f / m_normal.length();
        m_normal *= inverseNormalLength;
        m_constant *= inverseNormalLength;
        return *this;
    }

    Plane &negate()
    {
        m_constant *= -1;
        m_normal = -m_normal;

        return *this;
    }

    float distance_to_point(glm::vec3 point)
    {
        return glm::dot(m_normal, point) + m_constant;
    }

    float distance_to_sphere(Sphere &sphere)
    {
        return distance_to_point(sphere.center()) - sphere.radius();
    }

    glm::vec3 project_point(glm::vec3 point)
    {
        return m_normal * distance_to_point(point) + point;
    }
};


}  // namespace ay::mth

#pragma once


#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::mth
{


class Line
{
  private:

    glm::vec3 m_start;
    glm::vec3 m_end;

  public:

    Line(glm::vec3 t_min = {}, glm::vec3 t_max = {}) : m_start(t_min), m_end(t_max){}


    glm::vec3 &start()
    {
        return m_start;
    }

    glm::vec3 &end()
    {
        return m_end;
    }

    glm::vec3 center() const
    {
        return (m_end + m_start) * 0.5f;
    }

    glm::vec3 delta() const
    {
        return (m_end - m_start);
    }

    float distance() const
    {
        return glm::length(m_end - m_start);
    }

    float distance_sq() const
    {
        return glm::length2(m_end - m_start);
    }

    glm::vec3 at(float t) const
    {
        return delta() * t + m_start;
    }
    
    
};



}

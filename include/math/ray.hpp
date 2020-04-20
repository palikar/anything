#pragma once


#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::mth
{

class Ray
{
  private:
    glm::vec3 m_origin;
    glm::vec3 m_dir;
  public:

    Ray(glm::vec3 t_origin, glm::vec3 t_dir) : m_origin(t_origin), m_dir(t_dir)
    {}


    
    glm::vec3& origin() { return m_origin; }

    glm::vec3& dir() { return m_dir; }

    glm::vec3 at(float t)
    {
        return m_dir  * t + m_origin;
    }

    glm::vec3 lookAt(glm::vec3 v)
    {
        return glm::normalize(v - m_origin);
    }

    Ray& recast(float t)
    {
        m_origin = at(t);
        return *this;
    }
    

};



}

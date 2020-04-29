#pragma once


#include "glm_header.hpp"

namespace ay::grph
{

enum class FogType
{
    NONE = 0,  
    LINEAR,
    EXP2
};

class Fog
{

  private:
    FogType m_type{FogType::NONE};
    glm::vec3 m_color{0.2,0.2,0.2};

    float m_density{0.00025};
    float m_near{1};
    float m_far{1000};

    bool m_active{false};

    
  public:

    Fog() = default;
    
    
    Fog(FogType t_type,  glm::vec3 t_color, float t_density) :
        m_type(t_type), m_color(t_color), m_density(t_density)
    {
    }

    
    Fog(FogType t_type, glm::vec3 t_color, float t_near, float t_far) :
        m_type(t_type), m_color(t_color), m_near(t_near), m_far(t_far)
    {
    }

    FogType type() const { return m_type; }

    float near() const { return m_near; }

    float far() const { return m_far; }

    float density() const { return m_density; }

    glm::vec3 color() const { return m_color; }

    bool activ() const { return m_active; }
    
    void  enable() { m_active = true; }

    void disable() { m_active = false; }
    
};

inline Fog linear_fog(glm::vec3 color, float near = 1.0f, float far = 1000.0f)

{
    return Fog(FogType::LINEAR, color, near, far );
}



inline Fog exp2_fog(glm::vec3 color, float density = 0.00025f)
{
    return Fog(FogType::LINEAR, color, density);
}

}

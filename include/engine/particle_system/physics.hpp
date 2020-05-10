#pragma once

#include "std_header.hpp"
#include "glm_header.hpp"
#include "math_header.hpp"

#include <variant>

#include "util/random.hpp"


namespace ay::part
{

struct PhisicsParameters
{
    bool m_gravity{ true };
    float m_gravity_factor{ 1.0f };
};

class ParticlePhisics
{
  private:

    PhisicsParameters m_parameters{};
    glm::vec3 m_final_force{0.0, 0.0, 0.0};
    
  public:

    ParticlePhisics();
    
    void update();

    void apply_force(float mass, glm::vec3 &acceleration);

};

}  // namespace ay::part

#pragma once

#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::part
{

struct Force
{
    glm::vec3 direction;
    float magnitude{ 1.0 };
};

struct PhysicsParameters
{
    bool m_gravity{ true };
    float m_gravity_factor{ 1.0f };

    std::vector<Force> m_forces;
};

class ParticlePhysics
{
  private:
    PhysicsParameters m_parameters{};
    glm::vec3 m_final_force{ 0.0, 0.0, 0.0 };

  public:
    ParticlePhysics();

    void update();

    void apply_force(float mass, glm::vec3 &acceleration);

    inline PhysicsParameters &parameters()
    {
        return m_parameters;
    }
};

}  // namespace ay::part

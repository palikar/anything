#include "physics.hpp"

namespace ay::part
{

ParticlePhysics::ParticlePhysics()
{
    update();
};


void ParticlePhysics::update()
{
    m_final_force.x = 0.0;
    m_final_force.y = 0.0;
    m_final_force.z = 0.0;

    if (m_parameters.m_gravity)
    {
        m_final_force += m_parameters.m_gravity_factor * glm::vec3(0.0f, -9.81f, 0.0f);
    }

    for (const auto &force : m_parameters.m_forces)
    {
        m_final_force += force.direction * force.magnitude;
    }
}

void ParticlePhysics::apply_force(float mass, glm::vec3 &acceleration)
{
    acceleration = m_final_force / mass;
}


}  // namespace ay::part

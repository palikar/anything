#include "physics.hpp"

namespace ay::part
{

ParticlePhisics::ParticlePhisics()
{
    update();
};


void ParticlePhisics::update()
{
    m_final_force.x = 0.0;
    m_final_force.y = 0.0;
    m_final_force.z = 0.0;

    if (m_parameters.m_gravity)
    {
        m_final_force +=
            m_parameters.m_gravity_factor * glm::vec3(0.0f, -9.81f, 0.0f);
    }

    m_final_force.x += 10.0f;
        
}

void ParticlePhisics::apply_force(float mass, glm::vec3 &acceleration)
{ 
    acceleration = m_final_force / mass;
}


}

#pragma once

#include "engine/entity.hpp"
#include "engine/game_base.hpp"

#include "rendering/shaders.hpp"
#include "rendering/vertex.hpp"
#include "rendering/vertex_array.hpp"

#include "graphics/material.hpp"
#include "graphics/instanced_mesh.hpp"
#include "graphics/geometry_factory.hpp"

#include "util/random.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"
#include "math_header.hpp"

#include "emitters.hpp"
#include "physics.hpp"


namespace ay::part
{


class ParticleSystemBase
{
  public:
    virtual ~ParticleSystemBase()
    {
    }

    virtual grph::Geometry &geometry() = 0;

    virtual grph::Material *material() = 0;

    virtual void update(float) = 0;

    virtual size_t count() = 0;
};

struct ParticleSystemParameters
{
    float life_base = 3.0f;
    glm::vec2 life_error{ 0.0, 1.0f };

    int spawn_rate = 5;
};

template<typename ParticleType>
class ParticleSystem : public ParticleSystemBase
{
  public:
    using Particle           = ParticleType;
    using ParticleInitParams = typename ParticleType::init_parameters_type;
    using Material           = typename ParticleType::material_type;
    static constexpr size_t instance_buffers = Particle::per_instance_buffers;

  private:
    ParticleSystemParameters m_parameters{};

    grph::Geometry m_geometry;
    std::unique_ptr<Material> m_material;
    std::array<std::pair<rend::VertexBuffer *, size_t>, instance_buffers> m_buffers;
    std::array<std::vector<float>, instance_buffers> m_data;

    const size_t max_particles;

    std::vector<ParticleType> m_particles;

    size_t last_used_particle = 0;
    size_t m_count            = 0;

    ParticleEmitterPtr m_emitter{ nullptr };
    ParticlePhysics m_phisics{};

    size_t find_unused_particle()
    {

        for (size_t i = last_used_particle; i < max_particles; i++)
        {
            if (m_particles[i].life < 0.0f)
            {
                last_used_particle = i;
                return i;
            }
        }

        for (size_t i = 0; i < last_used_particle; i++)
        {
            if (m_particles[i].life < 0)
            {
                last_used_particle = i;
                return i;
            }
        }

        return 0;
    }

    inline void init_particle(Particle &t_particle)
    {
        t_particle.position = m_emitter->next_position();

        t_particle.velocity = m_emitter->next_velocity();

        t_particle.life = m_parameters.life_base
                          + util::Random::uniform_real(m_parameters.life_error.x,
                                                       m_parameters.life_error.y);

        t_particle.init();
    }

    inline void spawn(float dt)
    {
        for (size_t i = 0; i < m_parameters.spawn_rate * dt; ++i)
        {
            const size_t index = find_unused_particle();
            init_particle(m_particles[index]);
        }
    }

    inline void update_intance_buffers()
    {
        for (size_t i = 0; i < instance_buffers; ++i)
        {
            m_buffers[i].first->set_data(&m_data[i][0],
                                         sizeof(float) * m_count * m_buffers[i].second);
        }
    }

    inline void update_particle(float dt, Particle &t_particle)
    {

        t_particle.life -= dt;

        m_phisics.apply_force(t_particle.mass, t_particle.acc);

        t_particle.velocity += t_particle.acc * dt * 0.5f;
        t_particle.position += t_particle.velocity * dt;

        t_particle.update(dt);
    }


  public:
    ParticleSystem(const size_t t_max_particles = 4000)
      : m_geometry(ParticleType::geometry())
      , m_material(std::make_unique<Material>())
      , max_particles(t_max_particles)
    {
        m_geometry.pack();

        m_buffers = ParticleType::init_buffers(m_geometry, max_particles);
        ParticleType::init_material(m_material.get());


        for (size_t i = 0; i < instance_buffers; ++i)
        {
            m_data[i].resize(max_particles * m_buffers[i].second);
        }

        m_particles.resize(max_particles);

        m_emitter = std::make_unique<LineEmitter>(glm::vec3{ 0.0, 0.0, 0.0 },
                                                  glm::vec3{ 0.0, 0.0, 0.0 });
    }

    void update(float dt) override
    {
        spawn(dt);

        m_count      = 0;
        size_t index = 0;
        for (size_t i = 0; i < max_particles; ++i)
        {

            if (m_particles[i].life > 0.0)
            {
                update_particle(dt, m_particles[i]);
                m_particles[i].update_buffers(index++, m_data);
                ++m_count;
            }
        }

        update_intance_buffers();
    }

    // Stuff needed for the rendering

    grph::Geometry &geometry() override
    {
        return m_geometry;
    }

    grph::Material *material() override
    {
        return m_material.get();
    }

    Material *particle_material()
    {
        return m_material.get();
    }

    size_t count() override
    {
        return m_count;
    }

    // Particle system settings

    PhysicsParameters &phisics_parameters()
    {
        return m_phisics.parameters();
    }

    ParticleSystemParameters &parameters()
    {
        return m_parameters;
    }

    EmitterParameters &emitter_parameters()
    {
        return m_emitter->parameters();
    }

    ParticleInitParams &particle_parameters()
    {
        return Particle::g_init_params;
    }

    // Emitter Settings

    void make_sphere_emitter()
    {
        m_emitter = std::make_unique<SphereEmitter>();
    }

    void make_circle_emitter()
    {
        m_emitter = std::make_unique<CircleEmitter>();
    }

    void make_circle_area_emitter()
    {
        m_emitter = std::make_unique<CircleAreaEmitter>();
    }

    void make_line_emitter()
    {
        m_emitter = std::make_unique<LineEmitter>();
    }
};

}  // namespace ay::part

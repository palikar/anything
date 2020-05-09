#pragma once

#include "engine/entity.hpp"
#include "engine/game_base.hpp"

#include "rendering/shaders.hpp"
#include "rendering/vertex.hpp"
#include "rendering/vertex_array.hpp"

#include "graphics/material.hpp"
#include "graphics/materials/quad_particle.hpp"
#include "graphics/instanced_mesh.hpp"
#include "graphics/geometry_factory.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"
#include "math_header.hpp"

#include "util/random.hpp"

#include "emitters.hpp"


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

template<typename ParticleType>
class ParticleSystem : public ParticleSystemBase
{
  public:
    using Particle                           = ParticleType;
    using Material                           = typename ParticleType::material_type;
    static constexpr size_t instance_buffers = Particle::per_instance_buffers;

  private:
    grph::Geometry m_geometry;
    grph::MaterialPtr m_material;
    std::array<std::pair<rend::VertexBuffer *, size_t>, instance_buffers> m_buffers;
    std::array<std::vector<float>, instance_buffers> m_data;

    const size_t max_particles;

    std::vector<ParticleType> m_particles;

    size_t last_used_particle = 0;
    size_t m_count            = 0;

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

  public:
    ParticleSystem(const size_t t_max_particles = 4000)
      : m_geometry(ParticleType::geometry())
      , m_material(std::make_unique<Material>())
      , max_particles(t_max_particles)
    {
        m_geometry.pack();

        m_buffers = ParticleType::init_buffers(m_geometry, max_particles);

        for (size_t i = 0; i < instance_buffers; ++i)
        {
            m_data[i].resize(max_particles * m_buffers[i].second);
        }

        m_particles.resize(max_particles);
    }


    void update(float dt) override
    {

        for (size_t i = 0; i < 5; ++i)
        {

            size_t index = find_unused_particle();

            m_particles[index].init();
        }

        size_t count = 0;
        size_t index = 0;
        for (size_t i = 0; i < max_particles; ++i)
        {

            if (m_particles[i].life > 0.0)
            {
                ++count;
                m_particles[i].update(dt);
                m_particles[i].update_buffers(index++, m_data);
            }
        }

        m_count = count;

        for (size_t i = 0; i < instance_buffers; ++i)
        {
            // std::cout << "data isze " << sizeof(float) * max_particles *
            // m_buffers[i].second << "\n";
            m_buffers[i].first->set_data(
              &m_data[i][0], sizeof(float) * max_particles * m_buffers[i].second);
        }
    }

    grph::Geometry &geometry() override
    {
        return m_geometry;
    }

    grph::Material *material() override
    {
        return m_material.get();
    }

    size_t count() override
    {
        return m_count;
    }
};

}  // namespace ay::part

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

namespace ay::part
{


class ParticleSystemBase
{
  public:

    virtual grph::Geometry &geometry() = 0;

    virtual grph::Material *material() = 0;

    virtual void update(float) = 0;

    virtual size_t count() = 0;

};

struct QuadParticle
{

    glm::vec3 position{};
    glm::vec3 velocity{};
    glm::vec3 acc{};
    glm::vec4 color{};
    float life{-1.0};

    using material_type = grph::QuadParticleMaterial;

    static constexpr size_t per_instance_buffers = 2;

    static grph::Geometry geometry()
    {
        auto gem = grph::cube_geometry(0.5, 0.5, 0.5, 1, 1, 1);
        gem.drop_attribute("uv");
        gem.drop_attribute("normal");
        gem.drop_attribute("tangents");
        gem.drop_attribute("bitangents");
        return gem;
    }

    static std::array<std::pair<rend::VertexBuffer*, size_t>, per_instance_buffers>
    init_buffers(grph::Geometry &geom, size_t t_particle_count)
    {

        auto position_buf =
            std::make_unique<rend::VertexBuffer>(sizeof(glm::vec3) * t_particle_count);
        position_buf->set_layout(rend::BufferLayout(
                                     { rend::BufferElement{ "position", rend::ShaderDataType::Float3, true } }));

        auto color_buf =
            std::make_unique<rend::VertexBuffer>(sizeof(glm::vec4) * t_particle_count);
        color_buf->set_layout(rend::BufferLayout(
                                     { rend::BufferElement{ "color", rend::ShaderDataType::Float4, true } }));

        std::cout << "adding instance data" << "\n";
        auto pos = geom.gl_buffers()->add_vertex_buffer(std::move(position_buf));
        auto color = geom.gl_buffers()->add_vertex_buffer(std::move(color_buf));

        return {std::make_pair(pos, 3), std::make_pair(color, 4)};
    }

    void update_buffers(size_t index, std::array<std::vector<float>, per_instance_buffers> &data)
    {

        data[0][index*3 + 0] = position.x;
        data[0][index*3 + 1] = position.y;
        data[0][index*3 + 2] = position.z;

        data[1][index*4 + 0] = color.r;
        data[1][index*4 + 1] = color.g;
        data[1][index*4 + 2] = color.b;
        data[1][index*4 + 3] = color.a;

    }

    void update(float dt)
    {
        life -= dt;
        velocity += glm::vec3(0.0f,-9.81f, 0.0f) * dt * 0.5f;
        position += velocity * dt;
    }

    void init()
    {
        auto r = util::Random::uniform_real(0, 3.14*2);
        
        position = {0.0, 0.0, 0.0};
        velocity = {3*std::sin(r), 5.0, 3*std::cos(r)};
        life = 3.0f + util::Random::uniform_real(0.0, 0.5);
    }


};

template<typename ParticleType>
class ParticleSystem : public ParticleSystemBase
{
  public:
    using Particle = ParticleType;
    using Material = typename ParticleType::material_type;
    static constexpr size_t instance_buffers =Particle::per_instance_buffers;

  private:

    const size_t max_particles = 4000;
    
    grph::Geometry m_geometry;
    grph::MaterialPtr m_material;

    std::array<std::pair<rend::VertexBuffer *, size_t>, instance_buffers>  m_buffers;

    std::array<std::vector<float>, instance_buffers> m_data;

    std::vector<ParticleType> m_particles;

    size_t last_used_particle = 0;
    size_t m_count = 0;
    
    size_t find_unused_particle()
    {

        for(size_t  i = last_used_particle; i<max_particles; i++)
        {
            if (m_particles[i].life < 0.0f)
            {
                last_used_particle = i;
                return i;
            }
        }

        for(size_t i = 0; i<last_used_particle; i++)
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
    
    ParticleSystem() : m_geometry(ParticleType::geometry()), m_material(std::make_unique<Material>())
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

            if (m_particles[i].life > 0.0) {
                ++count;
                m_particles[i].update(dt);
                m_particles[i].update_buffers(index++, m_data);
            }
        }

        m_count = count;

        for (size_t i = 0; i < instance_buffers; ++i)
        {
            // std::cout << "data isze " << sizeof(float) * max_particles * m_buffers[i].second << "\n";
            m_buffers[i].first->set_data(&m_data[i][0], sizeof(float) * max_particles * m_buffers[i].second);
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

}

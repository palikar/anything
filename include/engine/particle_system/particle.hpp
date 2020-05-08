#pragma once

#include "engine/entity.hpp"
#include "engine/entity.hpp"

#include "rendering/shaders.hpp"
#include "rendering/vertex.hpp"
#include "rendering/vertex_array.hpp"

#include "graphics/material.hpp"
#include "graphics/materials/solid_color.hpp"
#include "graphics/instanced_mesh.hpp"
#include "graphics/geometry_factory.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"
#include "math_header.hpp"

namespace ay::part
{

struct QuadParticle
{

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acc;
    glm::vec4 color;
    float life;

    using material_type = grph::SolidColorMaterial;

    static constexpr size_t per_instance_buffers = 2;

    static grph::Geometry geometry()
    {
        auto gem = grph::cube_geometry(0.5, 0.5, 0.5, 1, 1, 1);
        gem.drop_attribute("uv");
        gem.drop_attribute("normal");
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
        position_buf->set_layout(rend::BufferLayout(
                                     { rend::BufferElement{ "color", rend::ShaderDataType::Float4, true } }));

        auto pos = geom.gl_buffers()->add_vertex_buffer(std::move(position_buf));
        auto color = geom.gl_buffers()->add_vertex_buffer(std::move(color_buf));

        return {std::make_pair(pos, 3), std::make_pair(color, 4)};
    }

    void update_buffers(size_t index, std::array<std::vector<float>, per_instance_buffers> data)
    {
        data[0][index*3 + 0] = position.x;
        data[0][index*3 + 1] = position.y;
        data[0][index*3 + 3] = position.z;

        data[1][index*4 + 0] = color.r;
        data[1][index*4 + 1] = color.g;
        data[1][index*4 + 3] = color.b;
        data[1][index*4 + 4] = color.a;

    }

    void update(float dt)
    {
        life -= dt;
        velocity += glm::vec3(0.0f,-9.81f, 0.0f) * delta * 0.5f;
        position += velocity *dt;
        

    }

    void init()
    {
        velocity = {1.0, 20.0, 0.0};
        life = 3.0f;
    }


};


class ParticleSystemBase
{
  public:

    virtual grph::Geometry &geometry() = 0;

    virtual grph::Material &material() = 0;

};

template<typename ParticleType>
class ParticleSystem
{
  public:
    using Particle = ParticleType;
    using Material = typename ParticleType::material_type;
    static constexpr size_t instance_buffers =Particle::per_instance_buffers;

  private:
    grph::InstancedMesh m_mesh;

    std::array<std::pair<rend::VertexBuffer *, size_t>, per_instance_buffers>,
        Particle::per_instance_buffers > m_buffers;

    std::array<std::vector<float>, Particle::per_instance_buffers> m_data;

    std::vector<ParticleType> m_particles;
    const size_t max_particles = 400;

    int last_used_particle = 0;
    int find_unused_particle()
    {

        for(int i=last_used_particle; i<max_particles; i++)
        {
            if (m_particles[i].life < 0.0f)
            {
                last_used_particle = i;
                return i;
            }
        }

        for(int i=0; i<last_used_particle; i++)
        {
            if (m_particles[i].life < 0)
            {
                last_used_particle = i;
                return i;
            }
        }

        return 0; // All particles are taken, override the first one
    }

  public:

    ParticleSystem()
    {
        m_mesh(ParticleType::geometry(), std::make_unique<Material>());
        m_buffers = ParticleType::init_buffers(m_mesh.geometry(), max_particles);

        for (i = 0; i < instance_buffers; ++i)
        {
            m_data[i].resize(max_particles * m_buffers[i].second);
        }
            
        m_particles.resize(max_particles);
    }

    void update(float dt)
    {

        for (size_t i = 0; i < 5; ++i)
        {
            size_t index = find_unused_particle();
            m_particles[index].init();
        }

        for (size_t i = 0; i < max_particles; ++i)
        {

            if (m_particles[i].life > 0.0) {
                m_particles[i].update(dt);
                m_particles[i].update_buffers(i, m_data);
            }

        }

        for (size_t i = 0; i < instance_buffers; ++i)
        {
            m_buffers[i].set_data(m_data[i], sizeof(float) * max_particles * 4);
        }


    }

    

};


}

#include "quad_particle.hpp"
#include "engine/particle_system/particle_system.hpp"
#include "util/random.hpp"

#include "graphics/material_builder.hpp"

namespace ay::part
{

grph::Geometry QuadParticle::geometry()
{
    auto gem = grph::plane_geometry(1, 1, 1, 1);
    // auto gem = grph::cube_geometry();
    // gem.rotate_y(glm::radians(90.0));
    gem.drop_attribute("uv");
    gem.drop_attribute("normal");
    gem.drop_attribute("tangents");
    gem.drop_attribute("bitangents");
    return gem;
}

std::array<std::pair<rend::VertexBuffer *, size_t>, QuadParticle::per_instance_buffers>
  QuadParticle::init_buffers(grph::Geometry &geom, size_t t_particle_count)
{

    auto position_buf = std::make_unique<rend::VertexBuffer>(
      (sizeof(glm::vec4) + sizeof(float)) * t_particle_count);

    position_buf->set_layout(rend::BufferLayout(
      { rend::BufferElement{ "position_scale", rend::ShaderDataType::Float4, true },
        rend::BufferElement{ "angle", rend::ShaderDataType::Float, true } }));

    auto color_buf =
      std::make_unique<rend::VertexBuffer>(sizeof(glm::vec4) * t_particle_count);
    color_buf->set_layout(rend::BufferLayout(
      { rend::BufferElement{ "color", rend::ShaderDataType::Float4, true } }));

    auto pos   = geom.gl_buffers()->add_vertex_buffer(std::move(position_buf));
    auto color = geom.gl_buffers()->add_vertex_buffer(std::move(color_buf));

    return { std::make_pair(pos, 5), std::make_pair(color, 4) };
}

void QuadParticle::update_buffers(
  size_t index,
  std::array<std::vector<float>, per_instance_buffers> &data)
{

    data[0][index * 5 + 0] = position.x;
    data[0][index * 5 + 1] = position.y;
    data[0][index * 5 + 2] = position.z;
    data[0][index * 5 + 3] = 1.0f;
    data[0][index * 5 + 4] = 3.14156592 / 4;

    data[1][index * 4 + 0] = color.r;
    data[1][index * 4 + 1] = color.g;
    data[1][index * 4 + 2] = color.b;
    data[1][index * 4 + 3] = color.a;
}

void QuadParticle::init_material(QuadParticle::material_type *material)
{
    grph::MaterialBuilder::from_existing(material)
      .addative_blending()
      .enable_blending()
      .both_side();
}

void QuadParticle::update(float dt)
{
    life -= dt;
    velocity += glm::vec3(0.0f, -9.81f, 0.0f) * dt * 0.5f;
    position += velocity * dt;
}

void QuadParticle::init()
{
    color = { util::Random::uniform_real(0, 1.0f),
              util::Random::uniform_real(0, 1.0f),
              util::Random::uniform_real(0, 1.0f),
              util::Random::uniform_real(0, 0.5f) };
}


template class ParticleSystem<QuadParticle>;


}  // namespace ay::part

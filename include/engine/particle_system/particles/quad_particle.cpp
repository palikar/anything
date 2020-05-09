#include "quad_particle.hpp"
#include "engine/particle_system/particle_system.hpp"
#include "util/random.hpp"

namespace ay::part
{

grph::Geometry QuadParticle::geometry()
{
    auto gem = grph::cube_geometry(0.5, 0.5, 0.5, 1, 1, 1);
    gem.drop_attribute("uv");
    gem.drop_attribute("normal");
    gem.drop_attribute("tangents");
    gem.drop_attribute("bitangents");
    return gem;
}

std::array<std::pair<rend::VertexBuffer *, size_t>, QuadParticle::per_instance_buffers>
  QuadParticle::init_buffers(grph::Geometry &geom, size_t t_particle_count)
{

    auto position_buf =
      std::make_unique<rend::VertexBuffer>(sizeof(glm::vec3) * t_particle_count);
    position_buf->set_layout(rend::BufferLayout(
      { rend::BufferElement{ "position", rend::ShaderDataType::Float3, true } }));

    auto color_buf =
      std::make_unique<rend::VertexBuffer>(sizeof(glm::vec4) * t_particle_count);
    color_buf->set_layout(rend::BufferLayout(
      { rend::BufferElement{ "color", rend::ShaderDataType::Float4, true } }));

    std::cout << "adding instance data"
              << "\n";
    auto pos   = geom.gl_buffers()->add_vertex_buffer(std::move(position_buf));
    auto color = geom.gl_buffers()->add_vertex_buffer(std::move(color_buf));

    return { std::make_pair(pos, 3), std::make_pair(color, 4) };
}

void QuadParticle::update_buffers(
  size_t index,
  std::array<std::vector<float>, per_instance_buffers> &data)
{

    data[0][index * 3 + 0] = position.x;
    data[0][index * 3 + 1] = position.y;
    data[0][index * 3 + 2] = position.z;

    data[1][index * 4 + 0] = color.r;
    data[1][index * 4 + 1] = color.g;
    data[1][index * 4 + 2] = color.b;
    data[1][index * 4 + 3] = color.a;
}

void QuadParticle::update(float dt)
{
    life -= dt;
    velocity += glm::vec3(0.0f, -9.81f, 0.0f) * dt * 0.5f;
    position += velocity * dt;
}

void QuadParticle::init()
{
    auto r = util::Random::uniform_real(0, 3.14 * 2);
    color  = { util::Random::uniform_real(0, 1.0f),
              util::Random::uniform_real(0, 1.0f),
              util::Random::uniform_real(0, 1.0f),
              util::Random::uniform_real(0, 1.0f) };

    auto q = util::Random::uniform_real(0, 1.0f);

    auto p = (q - 1) * glm::vec3{ 20.0, 0.0, 0.0 } + q * glm::vec3{ 0.0, 0.0, 0.0 };

    position = p;
    velocity = { 0.0f, 8.0, 3 * std::cos(r) };
    life     = 3.0f + util::Random::uniform_real(0.0, 0.5);
}


template class ParticleSystem<QuadParticle>;


}  // namespace ay::part

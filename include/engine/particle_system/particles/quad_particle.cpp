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
    gem.rotate_y(glm::radians(180.0));
    // gem.drop_attribute("uv");
    gem.drop_attribute("normal");
    gem.drop_attribute("tangents");
    gem.drop_attribute("bitangents");
    return gem;
}

std::array<std::pair<rend::VertexBuffer *, size_t>, QuadParticle::per_instance_buffers>
  QuadParticle::init_buffers(grph::Geometry &geom, size_t t_particle_count)
{

    auto position_buf = std::make_unique<rend::VertexBuffer>(
      (sizeof(glm::vec4) + sizeof(glm::vec3)) * t_particle_count);

    position_buf->set_layout(rend::BufferLayout(
      { rend::BufferElement{ "position_scale", rend::ShaderDataType::Float4, true },
        rend::BufferElement{
          "angle_factor_index", rend::ShaderDataType::Float3, true } }));

    auto color_buf =
      std::make_unique<rend::VertexBuffer>(sizeof(glm::vec4) * t_particle_count);
    color_buf->set_layout(rend::BufferLayout(
      { rend::BufferElement{ "color", rend::ShaderDataType::Float4, true } }));

    auto pos   = geom.gl_buffers()->add_vertex_buffer(std::move(position_buf));
    auto color = geom.gl_buffers()->add_vertex_buffer(std::move(color_buf));

    return { std::make_pair(pos, 7), std::make_pair(color, 4) };
}

void QuadParticle::update_buffers(
  size_t index,
  std::array<std::vector<float>, per_instance_buffers> &data)
{

    data[0][index * 7 + 0] = position.x;
    data[0][index * 7 + 1] = position.y;
    data[0][index * 7 + 2] = position.z;
    data[0][index * 7 + 3] = m_parameters.scale;

    data[0][index * 7 + 4] = m_parameters.angle;
    data[0][index * 7 + 5] = m_parameters.factor;
    data[0][index * 7 + 6] = m_parameters.index;

    data[1][index * 4 + 0] = m_parameters.color.r;
    data[1][index * 4 + 1] = m_parameters.color.g;
    data[1][index * 4 + 2] = m_parameters.color.b;
    data[1][index * 4 + 3] = m_parameters.color.a;
}

void QuadParticle::init_material(QuadParticle::material_type *material)
{
    grph::MaterialBuilder::from_existing(material)
      .addative_blending()
      .enable_blending()
      .depth_write(false);
}

void QuadParticle::update(float)
{

    const float d     = max_life / g_init_params.atlas_entries;
    const float index = (max_life - life) / d;

    m_parameters.index  = static_cast<int>(index);
    m_parameters.factor = index - static_cast<int>(index);
}

void QuadParticle::init()
{
    m_parameters.color =
      mth::vec4_with_error(g_init_params.color, g_init_params.color_error);
    m_parameters.scale =
      mth::real_with_error(g_init_params.scale, g_init_params.scale_error);
    m_parameters.angle =
      mth::real_with_error(g_init_params.angle, g_init_params.angle_error);
}


template class ParticleSystem<QuadParticle>;

InitParams QuadParticle::g_init_params{};

}  // namespace ay::part

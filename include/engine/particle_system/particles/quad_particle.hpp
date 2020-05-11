#pragma once

#include "rendering/shaders.hpp"
#include "rendering/vertex.hpp"
#include "rendering/vertex_array.hpp"

#include "graphics/materials/quad_particle.hpp"
#include "graphics/geometry_factory.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"
#include "math_header.hpp"


namespace ay::part
{

struct QuadParticleAttributes
{
    glm::vec4 color{};
    float angle{ 0.0 };
    float scale{ 0.0 };
};

struct InitParams
{
    glm::vec4 color{ 1.0f, 0.0f, 0.0f, 1.0f };
    glm::vec4 color_error[2] = { { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } };

    float angle{ 0.0 };
    float angle_error[2] = { 0.0, 0.0 };

    float scale{ 1.0 };
    float scale_error[2] = { 0.0, 0.0 };
};

struct QuadParticle
{
    using material_type        = grph::QuadParticleMaterial;
    using parameters_type      = QuadParticleAttributes;
    using init_parameters_type = InitParams;

    // Manditory
    glm::vec3 position{};
    glm::vec3 velocity{};
    glm::vec3 acc{};
    float mass{ 1.0 };
    float life{ -1.0 };

    QuadParticleAttributes m_parameters;


    static constexpr size_t per_instance_buffers = 2;

    static InitParams g_init_params;

    static grph::Geometry geometry();

    static void init_material(material_type *material);

    static std::array<std::pair<rend::VertexBuffer *, size_t>, per_instance_buffers>
      init_buffers(grph::Geometry &geom, size_t t_particle_count);


    void update_buffers(size_t index,
                        std::array<std::vector<float>, per_instance_buffers> &data);

    void update(float dt);

    void init();
};


}  // namespace ay::part

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

struct QuadParticle
{

    glm::vec3 position{};
    glm::vec3 velocity{};
    glm::vec3 acc{};
    glm::vec4 color{};
    float life{ -1.0 };

    using material_type = grph::QuadParticleMaterial;

    static constexpr size_t per_instance_buffers = 2;

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

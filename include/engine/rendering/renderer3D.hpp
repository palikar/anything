#pragma once


#include "rendering/shaders.hpp"

#include "graphics/material.hpp"
#include "graphics/light.hpp"
#include "graphics/uniform_binder.hpp"
#include "graphics/texture_binder.hpp"
#include "graphics/render_context.hpp"

#include "engine/entity.hpp"

#include "math/transform.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::gmt
{
class Entity;
class Entity;
class Scene3D;
class Skybox;
}  // namespace ay::gmt


namespace ay::cmp
{

class ModelComponent;
class MeshComponent;
class InstancedMeshComponent;
class GroupComponent;
class LineSegmentsComponent;
class ParticleComponent;
}  // namespace ay::cmp


#define HANDLER(fun) [this](auto obj, auto comp) { this->fun(obj, comp); }

namespace ay::rend
{

class RenderAPI;

class RendererScene3D
{
  private:
    RenderAPI *m_api;

    TextureBinder m_binder;
    UniformBinder m_uniform_binder;

    uint32_t current_shader = 0;
    std::unordered_map<uint32_t, bool> lighting_updated;

    std::vector<glm::mat4> m_mat_stack;
    glm::mat4 m_view;
    glm::mat4 m_view_projection;
    glm::mat4 m_projection;

    RenderContext m_current_context;

    void render_entity(gmt::Entity *object);

    template<typename T, typename... Ts, typename Func>
    void dispatch_component(gmt::Entity *object, Func &&func)
    {
        auto comp = object->component<T>();

        if (comp == nullptr)
        {
            return;
        }

        bool valid = true;
        valid      = ((object->component<Ts>() != nullptr) && ...);

        if (valid)
        {
            func(object, comp);
        }
    }


    void bind_lighting(Shader *shader);

    void process_material(grph::Material *material, Shader *shader);

    void switch_shader(Shader *shader);

    void switch_mvp(Shader *shader, glm::mat4 transform);

    void push_transform(glm::mat4 transform);


    void handle_sky(gmt::Skybox *sky);

    void handle_mesh(gmt::Entity *object, cmp::MeshComponent *mesh_comp);

    void handle_particles(gmt::Entity *object, cmp::ParticleComponent *particle_comp);

    void handle_instanced_mesh(gmt::Entity *object,
                               cmp::InstancedMeshComponent *mesh_comp);

    void handle_model(gmt::Entity *object, cmp::ModelComponent *mesh_comp);

    void handle_line_segments(gmt::Entity *object, cmp::LineSegmentsComponent *line);

    void handle_group(gmt::Entity *object, cmp::GroupComponent *group_comp);


  public:
    RendererScene3D();

    void init(RenderAPI *t_api);

    void render_scene(gmt::Scene3D &scene);
};

}  // namespace ay::rend

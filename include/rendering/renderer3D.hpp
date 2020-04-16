#pragma once

#include "rendering/texture_binder.hpp"
#include "rendering/shaders.hpp"

#include "graphics/material.hpp"

#include "engine/entity.hpp"

#include "math/transform.hpp"

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::gmt
{
class Entity;
class Entity;
class Scene3D;
}  // namespace ay::gmt


namespace ay::cmp
{

class MeshComponent;
class GroupComponent;
class LineSegmentsComponent;

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

    uint32_t current_shader = 0;

    std::vector<glm::mat4> m_mat_stack;
    glm::mat4 m_projection;

    void render_entity(gmt::Entity *object);

  public:
    RendererScene3D() = default;

    void init(RenderAPI *t_api);

    void render_scene(gmt::Scene3D &scene);

    void handle_mesh(gmt::Entity *object, cmp::MeshComponent *mesh_comp);

    void handle_line_segments(gmt::Entity *object, cmp::LineSegmentsComponent *line);

    void handle_group(gmt::Entity *object, cmp::GroupComponent *group_comp);

    void handle_material(grph::Material *material, Shader *shader);

    void switch_shader(Shader *shader);

    void push_transform(glm::mat4 transform);

    void switch_mvp(Shader *shader, glm::mat4 transform);

    template<typename T, typename... Ts, typename Func>
    void dispatch_component(gmt::Entity *object, Func &&func)
    {
        auto comp = object->component<T>();

        bool valid = true;
        valid      = ((object->component<Ts>() != nullptr) && ...);

        if (comp != nullptr && valid)
        {
            func(object, comp);
        }
    }
};

}  // namespace ay::rend

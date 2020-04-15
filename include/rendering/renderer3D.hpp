#pragma once

#include "rendering/texture_binder.hpp"

#include "engine/entity.hpp"

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

}


#define HANDLER(fun) [this](auto obj, auto comp){this->fun(obj, comp);}

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

    void handle_group(gmt::Entity *object, cmp::GroupComponent *group_comp);

    template<typename T, typename Func>
    void dispatch_component(gmt::Entity *object, Func && func)
    {
        auto comp  = object->component<T>();
        if (comp != nullptr) {
            func(object, comp);
        }
        
    }
};

}  // namespace ay::rend

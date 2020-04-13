#pragma once

#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::gmt
{
class Entity;
class Entity;
class Scene3D;
}  // namespace ay::gmt

namespace ay::rend
{

class RenderAPI;

class RendererScene3D
{
  private:
    RenderAPI *m_api;

    uint32_t current_shader = 0;

    std::vector<glm::mat4> m_mat_stack;
    glm::mat4 m_projection;

    void render_entity(gmt::Entity *object);

  public:
    RendererScene3D() = default;

    void init(RenderAPI *t_api);

    void render_scene(gmt::Scene3D &scene);
};

}  // namespace ay::rend

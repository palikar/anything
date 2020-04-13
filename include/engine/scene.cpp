#include "engine/components/transform.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/group.hpp"

#include "engine/scene.hpp"
#include "engine/game_base.hpp"

namespace ay
{

void Scene3D::init(GameBase *t_game)
{
    m_game = t_game;

    // m_base_shader = m_game->shaders().load("simple");
    // m_base_shader->bind();
    // m_base_shader->set("red_channel", 0.3f);
    // m_base_shader->set("projection_matrix", m_camera.view_projection());
}


Entity *Scene3D::add(EntityPtr t_entity)
{
    t_entity->set_game(m_game);
    t_entity->init(m_game);
    m_entities.push_back(std::move(t_entity));

    return m_entities.back().get();
}


}  // namespace ay

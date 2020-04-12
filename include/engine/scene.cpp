
#include "engine/scene.hpp"
#include "engine/game_base.hpp"

namespace ay {

void Scene3D::init(GameBase* t_game)
{
    m_game = t_game;
    m_base_shader = m_game->shaders().load("simple");

    m_base_shader->bind();
    m_base_shader->set("red_channel", 0.3f);
    m_base_shader->set("projection_matrix", m_camera.view_projection());
    std::cout << "initing scene" << "\n";

}

    
}

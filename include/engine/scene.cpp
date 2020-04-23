#include "engine/components/transform.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/group.hpp"

#include "engine/entities/skybox.hpp"

#include "engine/scene.hpp"
#include "engine/game_base.hpp"

namespace ay::gmt
{

void Scene3D::init(GameBase *t_game)
{
    m_game = t_game;
    // m_raycaster.update_camera(&m_camera);
}


Entity *Scene3D::add(EntityPtr t_entity)
{
    t_entity->set_game(m_game, temp_id++);
    t_entity->init(m_game);
    m_entities.push_back(std::move(t_entity));

    return m_entities.back().get();
}


Skybox *Scene3D::set_skybox(EntityPtr t_sky)
{
    t_sky->set_game(m_game, temp_id++);
    t_sky->init(m_game);
    m_entities.push_back(std::move(t_sky));

    m_skybox = static_cast<Skybox *>(m_entities.back().get());
    return m_skybox;
}

Skybox *Scene3D::skybox()
{
    return m_skybox;
}


}  // namespace ay::gmt

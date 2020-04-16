#pragma once


#include "rendering/renderer.hpp"

#include "engine/component.hpp"
#include "engine/entity.hpp"
#include "engine/camera.hpp"


#include "application/event.hpp"

#include "std_header.hpp"

namespace ay::gmt
{

class GameBase;
class Skybox;
// using SkyboxPtr;

class Scene3D
{
  private:
    std::vector<EntityPtr> m_entities;
    std::vector<ComponentPtr> m_game_components;
    Skybox* m_skybox; 

    Camera m_camera;

    GameBase *m_game;

  public:
    Scene3D()
    {
    }

    void init(GameBase *t_game);

    Entity *add(EntityPtr t_entity);

    template<typename T, typename... Args>
    T *add_component(Args... args)
    {
        m_game_components.push_back(std::make_unique<T>(args...));
        return static_cast<T *>(m_game_components.back().get());
    }

    void update(double dt)
    {
        for (auto &object : m_entities)
        {
            object->update(dt);
        }

        for (auto &comp : m_game_components)
        {
            comp->update(dt);
        }
    }

    void event(app::Event &t_ev)
    {
        for (auto &object : m_entities)
        {
            object->event(t_ev);
        }

        for (auto &comp : m_game_components)
        {
            comp->event(t_ev);
        }
    }

    Skybox* set_skybox(EntityPtr t_sky);
    
    Skybox* skybox();
    
    Camera &camera()
    {
        return m_camera;
    }

    std::vector<EntityPtr> &entities()
    {
        return m_entities;
    }
};


}  // namespace ay::gmt

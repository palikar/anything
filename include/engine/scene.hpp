#pragma once

#include "rendering/renderer.hpp"

#include "graphics/light.hpp"

#include "engine/component.hpp"
#include "engine/entity.hpp"
#include "engine/camera.hpp"

#include "application/event.hpp"

#include "std_header.hpp"

namespace ay::gmt
{

class GameBase;
class Skybox;
class Raycaster;
// using SkyboxPtr;

class Scene3D
{
  private:
    std::vector<EntityPtr> m_entities;
    std::vector<ComponentPtr> m_game_components;

    grph::LightSetup m_light_setup;

    Skybox *m_skybox{ nullptr };

    Camera m_camera;

    GameBase *m_game;

    uint32_t temp_id{ 0 };

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
        m_game_components.back().get()->set_game(m_game, temp_id++);
        return static_cast<T *>(m_game_components.back().get());
    }

    void remove(Entity *t_ent)
    {
        auto it = std::remove_if(m_entities.begin(), m_entities.end(), [t_ent](auto &el) {
                                                                           return el.get()->id() == t_ent->id();
                                                                       });
        m_entities.erase(it, m_entities.end());
    }

    void remove_component(Component *t_comp)
    {

        auto it = std::remove_if(
            m_game_components.begin(), m_game_components.end(), [t_comp](auto &el) {
                                                                    std::cout << (el.get()->id() == t_comp->id()) << "\n";
                                                                    return el.get()->id() == t_comp->id();
                                                                });

        m_game_components.erase(it, m_game_components.end());
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
        app::Dispatcher dispatch{ t_ev };
        // dispatch.dispatch<app::WindowResizeEvent>([this](auto &e) {
        //                                               m_raycaster.update_viewport(e.width(), e.height());
        //                                               return false;
        //                                           });

        for (auto &object : m_entities)
        {
            object->event(t_ev);
        }

        for (auto &comp : m_game_components)
        {
            comp->event(t_ev);
        }
    }

    Skybox *set_skybox(EntityPtr t_sky);

    Skybox *skybox();

    Camera &camera()
    {
        return m_camera;
    }

    std::vector<EntityPtr> &entities()
    {
        return m_entities;
    }

    grph::LightSetup &light_setup()
    {
        return m_light_setup;
    }

    void directional_light(glm::vec3 dir, glm::vec3 color = { 0.3f, 0.0f, 0.0f })
    {
        m_light_setup.directional_light.dir    = dir;
        m_light_setup.directional_light.color  = color;
        m_light_setup.directional_light.active = true;
    }

    void ambient_light(glm::vec3 color = { 0.3f, 0.0f, 0.0f })
    {
        m_light_setup.ambient_light.color  = color;
        m_light_setup.ambient_light.active = true;
    }

    void point_light(size_t index,
                     glm::vec3 color    = { 0.3f, 0.0f, 0.0f },
                     glm::vec3 position = { 0.0f, 3.0f, 0.0f })
    {
        m_light_setup.point_lights[index].active   = true;
        m_light_setup.point_lights[index].color    = color;
        m_light_setup.point_lights[index].position = position;
    }

    void spot_light(size_t index,
                    glm::vec3 color    = { 0.3f, 0.0f, 0.0f },
                    glm::vec3 dir      = { 0.0f, -1.0f, 0.0f },
                    glm::vec3 position = { 0.0f, 5.0f, 0.0f })
    {
        m_light_setup.spot_lights[index].active   = true;
        m_light_setup.spot_lights[index].color    = color;
        m_light_setup.spot_lights[index].dir      = dir;
        m_light_setup.spot_lights[index].position = position;
    }
};


}  // namespace ay::gmt

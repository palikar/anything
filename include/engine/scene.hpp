#pragma once


#include "engine/component.hpp"
#include "engine/entity.hpp"
#include "engine/camera.hpp"

#include "rendering/renderer.hpp"

#include "application/event.hpp"

#include "std_header.hpp"

namespace ay
{

class GameBase;

class Scene3D
{
  private:
    std::vector<EntityPtr> m_entities;
    std::vector<ComponentPtr> m_game_components;
    Camera m_camera;
    GameBase* m_game;
    ShaderPtr m_base_shader;

  public:
    
    Scene3D()
    {}

    void init(GameBase* t_game);

    Entity* add(EntityPtr t_entity)
    {
        m_entities.push_back(std::move(t_entity));
        return m_entities.back().get();
    }


    template<typename T, typename ... Args>
    T* add_component(Args ... args)
    {
        m_game_components.push_back(std::make_unique<T>(args ...));
        return static_cast<T*>(m_game_components.back().get());
    }
    
    void update(double dt)
    {
        for (auto& object : m_entities) {
            object->update(dt);
        }

        for (auto& comp : m_game_components) {
            comp->update(dt);
        }
        
        
        m_base_shader->bind();
        m_base_shader->set("projection_matrix", m_camera.view_projection());
    }

    void event(Event& t_ev)
    {
        for (auto& object : m_entities) {
            object->event(t_ev);
        }

        for (auto& comp : m_game_components) {
            comp->event(t_ev);
        }
    }

    void render(Renderer& render_api)
    {
        m_base_shader->bind();

        for (auto& object : m_entities) {

            auto mesh_comp = object->component<MeshComponent>();
            auto trans_comp = object->component<TransformComponent>();
            
            if (mesh_comp && trans_comp ) {

                m_base_shader->set("model_matrix", trans_comp->transform.get_tranformation());
                mesh_comp->mesh.geometry()->bind();
                render_api.draw_indexed(mesh_comp->mesh.geometry());
            }
            
        }        
    }

    Camera& camera()
    {
        return m_camera;
    }    

};



}

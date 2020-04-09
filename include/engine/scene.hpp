#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <memory>
#include <vector>


#include "engine/component.hpp"
#include "engine/entity.hpp"
#include "engine/camera.hpp"

#include "rendering/renderer.hpp"



namespace ay
{

class GameBase;

class Scene3D
{
  private:
    std::vector<EntityPtr> m_entities;
    Camera m_camera;
    GameBase* m_game;

  public:
    
    Scene3D()
    {}

    void init(){

    }

    Entity* add(EntityPtr t_entity)
    {}
    
    void update(double)
    {}

    void render(Renderer&)
    {}

    Camera& camera()
    {
        return m_camera;
    }    

};



}

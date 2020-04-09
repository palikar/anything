#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <memory>


namespace ay {

class GameEngine;
class Renderer;
class ShaderLibrary;

class GameBase {
    
  protected:
    GameEngine* m_engine;
    
  public:

    ShaderLibrary& shaders();
    
    void set_up(GameEngine* t_engine);

    virtual void init() = 0;
    virtual void update(double dt) = 0;
    virtual void render(Renderer& renderer) = 0;

};





}

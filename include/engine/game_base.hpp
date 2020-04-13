#pragma once

#include "engine/scene.hpp"
#include "application/event.hpp"

#include "std_header.hpp"

namespace ay
{

class GameEngine;
class Renderer;
class ShaderLibrary;

class GameBase
{

  protected:
    GameEngine *m_engine;
    std::unordered_map<std::string, Scene3D> m_scenes;

  public:
    virtual ~GameBase(){};

    ShaderLibrary &shaders();
    void set_up(GameEngine *t_engine);
    Scene3D *init_scene(const std::string &t_name);
    GameEngine *engine()
    {
        return m_engine;
    }

    virtual void init()                      = 0;
    virtual void update(double dt)           = 0;
    virtual void render(RenderAPI &renderer) = 0;
    virtual bool event(Event &)
    {
        return false;
    };
};


}  // namespace ay

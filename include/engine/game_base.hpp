#pragma once

#include "engine/scene.hpp"

#include "engine/texture_library.hpp"
#include "engine/shader_library.hpp"
#include "engine/animation/animator.hpp"


#include "application/event.hpp"
#include "std_header.hpp"


namespace ay::rend
{
class ShaderLibrary;
}
namespace ay::gmt
{

class GameEngine;
class Scene3D;

class GameBase
{

  protected:
    GameEngine *m_engine;
    std::unordered_map<std::string, Scene3D> m_scenes;

  public:
    virtual ~GameBase(){};

    gmt::ShaderLibrary &shaders();

    gmt::TextureLibrary &textures();

    anim::Animator &animator();

    void set_up(GameEngine *t_engine);

    Scene3D *init_scene(const std::string &t_name);

    GameEngine *engine()
    {
        return m_engine;
    }

    virtual void init()                            = 0;
    virtual void update(double dt)                 = 0;
    virtual void render(rend::RenderAPI &renderer) = 0;
    virtual bool event(app::Event &)
    {
        return false;
    };
};


}  // namespace ay::gmt

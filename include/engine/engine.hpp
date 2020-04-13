#pragma once

#include "application/resource_loader.hpp"
#include "application/event.hpp"

#include "rendering/shaders.hpp"
#include "rendering/renderer.hpp"


namespace ay::gmt
{


class GameBase;


class GameEngine
{

  private:
    int m_width;
    int m_height;

    rend::ShaderLibrary m_shader_lib;
    GameBase *m_game;
    rend::RenderAPI m_renderAPI;


  public:
    explicit GameEngine(GameBase *t_game);

    void init(int width, int height);

    void update(double dt);

    void render();

    rend::ShaderLibrary &shader_lib()
    {
        return m_shader_lib;
    }

    rend::RenderAPI *api()
    {
        return &m_renderAPI;
    }

    bool on_event(app::Event &t_event);

    bool resize(app::WindowResizeEvent &e);
};


}  // namespace ay::gmt

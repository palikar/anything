#pragma once

#include "application/resource_loader.hpp"
#include "application/event.hpp"

#include "rendering/buffers.hpp"
#include "rendering/vertex_array.hpp"
#include "rendering/shaders.hpp"
#include "rendering/renderer.hpp"

#include "engine/game_base.hpp"

#include "math/transform.hpp"
#include "commons.hpp"

#include "util/gl_helpers.hpp"

namespace ay
{


class GameBase;


class GameEngine
{

  private:
    int m_width;
    int m_height;

    ShaderLibrary m_shader_lib;
    GameBase *m_game;
    RenderAPI m_renderAPI;


  public:
    explicit GameEngine(GameBase *t_game) : m_game(t_game), m_renderAPI()
    {
    }

    void init(int width, int height)
    {
        m_width  = width;
        m_height = height;

        m_renderAPI.init();
        m_renderAPI.set_viewport(0, 0, m_width, m_height);
        m_renderAPI.set_clear_color(glm::vec4(0.0, 0.0, 0.0, 0.0));

        m_game->set_up(this);

        m_game->init();
    }

    void update(double dt)
    {
        m_game->update(dt);
    }

    void render()
    {
        m_renderAPI.clear();
        m_game->render(m_renderAPI);
    }

    ShaderLibrary &shader_lib()
    {
        return m_shader_lib;
    }

    RenderAPI *api()
    {
        return &m_renderAPI;
    }

    bool on_event(Event &t_event)
    {
        return m_game->event(t_event);
    }

    bool resize(WindowResizeEvent &e)
    {
        m_renderAPI.set_viewport(0, 0, e.width(), e.height());
        m_width  = e.width();
        m_height = e.height();
        return true;
    }
};


}  // namespace ay

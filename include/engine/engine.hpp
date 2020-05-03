#pragma once

#include "application/resource_loader.hpp"
#include "application/event.hpp"

#include "rendering/shaders.hpp"

#include "rendering/render_api.hpp"

#include "engine/texture_library.hpp"
#include "engine/shader_library.hpp"
#include "engine/animation/animator.hpp"

namespace ay::gmt
{


class GameBase;


class GameEngine
{

  private:
    int m_width;
    int m_height;

    gmt::ShaderLibrary m_shader_lib;

    gmt::TextureLibrary m_texture_lib;

    anim::Animator m_main_animator;

    rend::RenderAPI m_renderAPI;

    GameBase *m_game;


  public:
    explicit GameEngine(GameBase *t_game);

    void init(int width, int height);

    void update(double dt);

    void render();

    inline gmt::ShaderLibrary &shader_lib()
    {
        return m_shader_lib;
    }

    inline gmt::TextureLibrary &tex_lib()
    {
        return m_texture_lib;
    }

    inline anim::Animator &animator()
    {
        return m_main_animator;
    }

    inline rend::RenderAPI *api()
    {
        return &m_renderAPI;
    }

    bool on_event(app::Event &t_event);

    bool resize(app::WindowResizeEvent &e);

    int width() const;

    int height() const;
};


}  // namespace ay::gmt

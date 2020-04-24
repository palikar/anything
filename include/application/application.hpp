#pragma once

#include "util/gl_helpers.hpp"
#include <GLFW/glfw3.h>

#include "application/window.hpp"
#include "application/resource_loader.hpp"
#include "application/event.hpp"
#include "engine/engine.hpp"


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


namespace ay::gmt
{
class GameBase;
}

namespace ay::app
{


class Application
{
  private:
    int m_width;
    int m_height;

    bool m_running{ true };

    Window *m_window;
    gmt::GameEngine m_engine;

  public:
    Application(int t_width, int t_height, gmt::GameBase *t_game);
    ~Application() = default;

    int run();


  private:
    void init();

    void init_imgui();

    void render_engine();

    void on_event(Event &t_event);

    bool on_close(WindowCloseEvent &)
    {
        AY_DEBUG(fmt::format("Closing window"));
        m_running = false;
        return false;
    }

    bool on_resize(WindowResizeEvent &e)
    {
        AY_DEBUG(fmt::format("Resizing: {}x{}", e.width(), e.height()));
        m_engine.resize(e);
        return false;
    }

    bool on_resposition(WindowPositionEvent &)
    {

        return false;
    }
};


}  // namespace ay::app

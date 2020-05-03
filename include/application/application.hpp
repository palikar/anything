#pragma once

#include "util/gl_helpers.hpp"
#include <GLFW/glfw3.h>

#include "application/window.hpp"
#include "application/resource_loader.hpp"
#include "application/event.hpp"

#include "engine/engine.hpp"

#include "imgui_header.hpp"


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

    void set_title(const std::string &name);

    void set_icon(const std::string &icon_file);

    void focus();

  private:
    void init();

    void shutdown();

    void init_imgui();

    void render_engine();

    void on_event(Event &t_event);

    bool on_close(WindowCloseEvent &);

    bool on_resize(WindowResizeEvent &e);

    bool on_resposition(WindowPositionEvent &);
};


}  // namespace ay::app

#pragma once

#include "util/gl_helpers.hpp"
#include <GLFW/glfw3.h>

#include "application/input.hpp"
#include "application/event.hpp"

#include "util/logging.hpp"

#include "std_header.hpp"

namespace ay::app
{


class Event;

class Window
{

  public:
    using EventCallbackFn = std::function<void(Event &)>;

  private:
    inline static std::unique_ptr<Window> m_instance;

    EventCallbackFn m_callback;

    GLFWwindow *m_window;
    Input *m_input;

    int m_width;
    int m_height;

    int m_xpos;
    int m_ypos;

  public:
    static Window *get_instance()
    {
        if (!m_instance)
        {
            m_instance = std::make_unique<Window>();
        }
        return m_instance.get();
    }

    Window() = default;

    void init(int t_width, int t_height, std::string_view t_name);

    void set_eventcall(EventCallbackFn t_callback)
    {
        m_callback = t_callback;
    }

    void set_title(std::string_view t_name)
    {
        glfwSetWindowTitle(m_window, t_name.data());
    }

    void dispose();

    void update();

    void render();

    bool closed();

    GLFWwindow *native()
    {
        return m_window;
    }

    int width() const
    {
        return m_width;
    }

    int height() const
    {
        return m_height;
    }

    int xpos() const
    {
        return m_xpos;
    }

    int ypos() const
    {
        return m_ypos;
    }
};


}  // namespace ay::app

#pragma once

#include "util/gl_helpers.hpp"

#include "application/keycodes.hpp"
#include "std_header.hpp"

#include "macros.hpp"

#include <GLFW/glfw3.h>

namespace ay::app
{

enum class EventType
{
    NONE = 0,
    WINDOWCLOSE,
    WINDOWRESIZE,
    WINDOWFOCUS,
    WINDOWLOSTFOCUS,
    WINDOWMOVED,
    APPTICK,
    APPUPDATE,
    APPRENDER,
    KEYPRESSED,
    KEYRELEASED,
    KEYTYPED,
    MOUSEBUTTONPRESSED,
    MOUSEBUTTONRELEASED,
    MOUSEMOVED,
    MOUSESCROLLED,
    WINDOWPOSITION
};


class Event
{
  public:
    bool handled = false;

    virtual EventType type() const         = 0;
    virtual const std::string name() const = 0;
};

class WindowResizeEvent : public Event
{
  public:
    WindowResizeEvent(unsigned int width, unsigned int height)
      : m_width(width), m_height(height)
    {
    }

    inline unsigned int width() const
    {
        return m_width;
    }
    inline unsigned int height() const
    {
        return m_height;
    }


    EVENT_TYPE(WINDOWRESIZE)

  private:
    unsigned int m_width, m_height;
};

class WindowPositionEvent : public Event
{
  public:
    WindowPositionEvent(unsigned int xpos, unsigned int ypos) : m_xpos(xpos), m_ypos(ypos)
    {
    }

    inline unsigned int xpos() const
    {
        return m_xpos;
    }
    inline unsigned int ypos() const
    {
        return m_ypos;
    }


    EVENT_TYPE(WINDOWPOSITION)

  private:
    unsigned int m_xpos, m_ypos;
};

class WindowCloseEvent : public Event
{
  public:
    WindowCloseEvent()
    {
    }

    EVENT_TYPE(WINDOWCLOSE)
};

class KeyPressedEvent : public Event
{
  public:
    KeyPressedEvent(KeyCode keycode, int repeatCount, int mod = 0)
      : m_keycode(keycode), m_repeat_count(repeatCount), m_mod(mod)
    {
    }

    EVENT_TYPE(KEYPRESSED)

    inline KeyCode key_code() const
    {
        return m_keycode;
    }

    inline int repeat_count() const
    {
        return m_repeat_count;
    }

    bool alt()
    {
        return (m_mod & GLFW_MOD_ALT) > 0;
    }

    bool ctrl()
    {
        return (m_mod & GLFW_MOD_CONTROL) > 0;
    }

    bool shift()
    {
        return (m_mod & GLFW_MOD_SHIFT) > 0;
    }

  private:
    KeyCode m_keycode;
    int m_repeat_count;
    int m_mod;
};

class KeyReleasedEvent : public Event
{
  public:
    KeyReleasedEvent(KeyCode keycode, int mod = 0) : m_keycode(keycode), m_mod(mod)
    {
    }

    inline KeyCode key_code() const
    {
        return m_keycode;
    }

    EVENT_TYPE(KEYRELEASED)

    bool alt()
    {
        return (m_mod & GLFW_MOD_ALT) > 0;
    }

    bool ctrl()
    {
        return (m_mod & GLFW_MOD_CONTROL) > 0;
    }

    bool shift()
    {
        return (m_mod & GLFW_MOD_SHIFT) > 0;
    }


  private:
    KeyCode m_keycode;
    int m_mod;
};

class KeyTypedEvent : public Event
{
  public:
    KeyTypedEvent(KeyCode keycode) : m_keycode(keycode)
    {
    }

    inline KeyCode key_code() const
    {
        return m_keycode;
    }

    EVENT_TYPE(KEYTYPED)
  private:
    KeyCode m_keycode;
};

class MouseMovedEvent : public Event
{
  public:
    MouseMovedEvent(float x, float y) : m_mousex(x), m_mousey(y)
    {
    }

    inline float get_x() const
    {
        return m_mousex;
    }
    inline float get_y() const
    {
        return m_mousey;
    }

    EVENT_TYPE(MOUSEMOVED)

  private:
    float m_mousex, m_mousey;
};

class MouseScrolledEvent : public Event
{
  public:
    MouseScrolledEvent(float xoffset, float yoffset)
      : m_xoffset(xoffset), m_yoffset(yoffset)
    {
    }

    inline float x_offset() const
    {
        return m_xoffset;
    }
    inline float y_offset() const
    {
        return m_yoffset;
    }

    EVENT_TYPE(MOUSESCROLLED)

  private:
    float m_xoffset, m_yoffset;
};

class MouseButtonPressedEvent : public Event
{
  public:
    MouseButtonPressedEvent(MouseCode button, int mod) : m_button(button), m_mod(mod)
    {
    }

    MouseCode button()
    {
        return m_button;
    }

    bool alt()
    {
        return (m_mod & GLFW_MOD_ALT) > 0;
    }

    bool ctrl()
    {
        return (m_mod & GLFW_MOD_CONTROL) > 0;
    }

    bool shift()
    {
        return (m_mod & GLFW_MOD_SHIFT) > 0;
    }

    EVENT_TYPE(MOUSEBUTTONPRESSED)

  private:
    MouseCode m_button;
    int m_mod;
};

class MouseButtonReleasedEvent : public Event
{
  public:
    MouseButtonReleasedEvent(MouseCode button, int mod) : m_button(button), m_mod(mod)
    {
    }

    MouseCode button()
    {
        return m_button;
    }

    bool alt()
    {
        return (m_mod & GLFW_MOD_ALT) > 0;
    }

    bool ctrl()
    {
        return (m_mod & GLFW_MOD_CONTROL) > 0;
    }

    bool shift()
    {
        return (m_mod & GLFW_MOD_SHIFT) > 0;
    }

    EVENT_TYPE(MOUSEBUTTONRELEASED)

  private:
    MouseCode m_button;
    int m_mod;
};


class Dispatcher
{
  public:
    Dispatcher(Event &event) : m_event(event)
    {
    }

    template<typename T, typename F>
    bool dispatch(const F &func)
    {
        if (m_event.type() == T::static_type())
        {
            m_event.handled = func(static_cast<T &>(m_event));
            return true;
        }
        return false;
    }

  private:
    Event &m_event;
};


}  // namespace ay::app

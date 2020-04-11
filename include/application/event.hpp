#pragma once

#include <string>

#include "application/keycodes.hpp"

#include "commons.hpp"

#include <GLFW/glfw3.h>


namespace ay
{

enum class EventType
{
    NONE = 0,
    WINDOWCLOSE, WINDOWRESIZE, WINDOWFOCUS, WINDOWLOSTFOCUS, WINDOWMOVED,
    APPTICK, APPUPDATE, APPRENDER,
    KEYPRESSED, KEYRELEASED, KEYTYPED,
    MOUSEBUTTONPRESSED, MOUSEBUTTONRELEASED, MOUSEMOVED, MOUSESCROLLED
};


#define EVENT_TYPE(t_type) static EventType static_type() { return EventType::t_type; } \
    virtual EventType type() const override { return static_type(); }   \
    virtual const std::string name() const override { return #t_type; }

class Event
{
  public:
    bool handled = false;

    virtual EventType type() const = 0;
    virtual const std::string name() const = 0;    

};


class WindowResizeEvent : public Event
{
  public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_width(width), m_height(height) {}

    inline unsigned int width() const { return m_width; }
    inline unsigned int height() const { return m_height; }

    
    EVENT_TYPE(WINDOWRESIZE)

	private:
    unsigned int m_width, m_height;
};

class WindowCloseEvent : public Event
{
  public:
    WindowCloseEvent() {}

    EVENT_TYPE(WINDOWCLOSE)    
};

class KeyPressedEvent : public Event
{
  public:
    KeyPressedEvent(KeyCode keycode, int repeatCount)
      : m_keycode(keycode), m_repeat_count(repeatCount)
    {
    }

    inline KeyCode key_code() const
    {
        return m_keycode;
    }
    
    inline int repeat_count() const
    {
        return m_repeat_count;
    }


    EVENT_TYPE(KEYPRESSED)
  private:
    KeyCode m_keycode;
    int m_repeat_count;
};

class KeyReleasedEvent : public Event
{
  public:
    KeyReleasedEvent(KeyCode keycode)
        : m_keycode(keycode)
    {
    }

    inline KeyCode key_code() const
    {
        return m_keycode;
    }

    EVENT_TYPE(KEYPRESSED)
    private:
    KeyCode m_keycode;
};

class KeyTypedEvent : public Event
{
  public:
    KeyTypedEvent(KeyCode keycode)
        : m_keycode(keycode)
    {
    }

    inline KeyCode key_code() const
    {
        return m_keycode;
    }

    EVENT_TYPE(KEYPRESSED)
    private:
    KeyCode m_keycode;
};

class MouseMovedEvent : public Event
{
  public:
    MouseMovedEvent(float x, float y)
        : m_mousex(x), m_mousey(y) {}

    inline float get_x() const { return m_mousex; }
    inline float get_y() const { return m_mousey; }

    EVENT_TYPE(MOUSEMOVED)
    
	private:
    float m_mousex, m_mousey;
};

class MouseScrolledEvent : public Event
{
  public:
    MouseScrolledEvent(float xoffset, float yoffset)
        : m_xoffset(xoffset), m_yoffset(yoffset) {}

    inline float x_offset() const { return m_xoffset; }
    inline float y_offset() const { return m_yoffset; }

    EVENT_TYPE(MOUSESCROLLED)

	private:
    float m_xoffset, m_yoffset;
};

class MouseButtonPressedEvent : public Event
{
  public:
    MouseButtonPressedEvent(MouseCode button)
        : m_button(button) {}

    EVENT_TYPE(MOUSEBUTTONPRESSED)
    
    private:
    MouseCode m_button;
};

class MouseButtonReleasedEvent : public Event
{
  public:
    MouseButtonReleasedEvent(MouseCode button)
        : m_button(button) {}

    EVENT_TYPE(MOUSEBUTTONPRESSED)
    
    private:
    MouseCode m_button;
};


class Dispatcher
{
  public:
    Dispatcher(Event& event)
        : m_event(event)
    {}
		
    template<typename T, typename F>
    bool dispatch(const F& func)
    {
        if (m_event.type() == T::static_type())
        {
            m_event.handled = func(static_cast<T&>(m_event));
            return true;
        }
        return false;
    }
  private:
    Event& m_event;
};




}

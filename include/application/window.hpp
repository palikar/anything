#pragma once

#include <iostream>
#include <string_view>
#include <string>
#include <sstream>
#include <cstdio>
#include <memory>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "application/input.hpp"
#include "application/event.hpp"


namespace ay {


struct Event;

class Window {

  public:
    using EventCallbackFn = std::function<void(Event&)>;

  private:
    inline static std::unique_ptr<Window> m_instance;

    EventCallbackFn m_callback;

    GLFWwindow* m_window;
    Input* m_input;

    int m_width;
    int m_height;

  public:

    static Window *get_instance()
    {
        if (!m_instance)
        {
            m_instance = std::make_unique<Window>();
        }
        return m_instance.get();
    }

    Window() {


    }

    void init(int t_width, int t_height, std::string_view t_name) {

        m_width = t_width;
        m_height = t_height;
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);


        m_window = glfwCreateWindow(t_width, t_height, t_name.data(), nullptr, nullptr);

        if (!m_window)
        {
            std::cout << "Cannot open glfw window" << "\n";
            return;
        }

        glfwMakeContextCurrent(m_window);
        glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
        glfwShowWindow(m_window);
        glfwSwapInterval(1);


        glfwSetWindowUserPointer(m_window, this);

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            auto& win = *(Window*)glfwGetWindowUserPointer(window);
            WindowResizeEvent event(width, height);
            win.m_callback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
        {
            auto& win = *(Window*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            win.m_callback(event);

        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int, int action, int)
        {
            auto& win = *(Window*)glfwGetWindowUserPointer(window);

            switch (action)
			{
              case GLFW_PRESS:
              {
                  KeyPressedEvent event(static_cast<KeyCode>(key), 0);
                  win.m_callback(event);
                  break;
              }
              case GLFW_RELEASE:
              {
                  KeyReleasedEvent event(static_cast<KeyCode>(key));
                  win.m_callback(event);
                  break;
              }
              case GLFW_REPEAT:
              {
                  KeyPressedEvent event(static_cast<KeyCode>(key), 1);
                  win.m_callback(event);
                  break;
              }
			}

        });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
        {
            auto& win = *(Window*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(static_cast<KeyCode>(keycode));
			win.m_callback(event);
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int)
        {
            auto& win = *(Window*)glfwGetWindowUserPointer(window);

            switch (action)
			{
              case GLFW_PRESS:
              {
                  MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                  win.m_callback(event);
                  break;
              }
              case GLFW_RELEASE:
              {
                  MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
                  win.m_callback(event);
                  break;
              }
			}
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            auto& win = *(Window*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)xOffset, (float)yOffset);
			win.m_callback(event);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
        {
            auto& win = *(Window*)glfwGetWindowUserPointer(window);
            MouseMovedEvent event((float)xPos, (float)yPos);
			win.m_callback(event);
        });


        m_input = Input::get_instance();
        m_input->init(m_window);

    }

    void set_eventcall(EventCallbackFn t_callback)
    {
        m_callback = t_callback;
    }

    void set_title(std::string_view t_name) {
        glfwSetWindowTitle(m_window, t_name.data());
    }

    void dispose() {
        glfwDestroyWindow(m_window);
    }

    void update() {
        glfwPollEvents();

    }

    void render() {
        glfwSwapBuffers(m_window);
    }

    bool closed() {
        return glfwWindowShouldClose(m_window);
    }


};


}

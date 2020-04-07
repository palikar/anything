#pragma once

#include <iostream>
#include <string_view>
#include <string>
#include <sstream>
#include <cstdio>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "application/input.hpp"


namespace ay {

class Window {

  private:
    inline static std::unique_ptr<Window> m_instance;

    GLFWwindow* m_window;
    Input* m_input;

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

        m_input = Input::get_instance();
        m_input->init(m_window);

        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        glViewport(0, 0, width, height);
        
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

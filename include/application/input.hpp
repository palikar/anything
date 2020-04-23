#pragma once

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "application/keycodes.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::app
{

class Keyboard
{
  public:
    static constexpr int MAX_KEYS = 360;

    static void
      key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        Keyboard::get_instance()->callback(window, key, scancode, action, mods);
    }

  private:
    static inline std::unique_ptr<Keyboard> m_instance;

    std::array<bool, MAX_KEYS> pressed;
    std::array<bool, MAX_KEYS> released;

    bool shift{ false };
    bool ctrl{ false };
    bool alt{ false };

    void callback(GLFWwindow *, int key, int, int action, int mods)
    {

        for (auto &state : pressed)
        {
            state = false;
        }
        for (auto &state : released)
        {
            state = false;
        }


        if (key >= MAX_KEYS)
        {
            return;
        }

        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            pressed[key] = true;
        }

        if (action == GLFW_RELEASE)
        {
            released[key] = true;
        }

        shift = (GLFW_MOD_SHIFT & mods) > 0;
        ctrl  = (GLFW_MOD_CONTROL & mods) > 0;
        alt   = (GLFW_MOD_ALT & mods) > 0;
    }

  public:
    static Keyboard *get_instance()
    {
        if (!m_instance)
        {
            m_instance = std::make_unique<Keyboard>();
        }
        return m_instance.get();
    }

    void init(GLFWwindow *t_window)
    {
        glfwSetKeyCallback(t_window, &Keyboard::key_callback);
    }

    bool shift_pressed()
    {
        return shift;
    }
    bool ctrl_pressed()
    {
        return ctrl;
    }
    bool alt_pressed()
    {
        return alt;
    }

    bool is_released(KeyCode t_key)
    {
        return released[static_cast<int>(t_key)];
    }
    bool is_pressed(KeyCode t_key)
    {
        return pressed[static_cast<int>(t_key)];
    }
};

class Mouse
{
  public:
    static void mouse_position_callback(GLFWwindow *window, double xpos, double ypos)
    {
        Mouse::get_instance()->pos_callback(window, xpos, ypos);
    }

    static void
      mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
    {
        Mouse::get_instance()->button_callback(window, button, action, mods);
    }

    static void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
    {
        Mouse::get_instance()->scroll_callback(window, xoffset, yoffset);
    }

  private:
    static constexpr int MAX_KEYS = 10;
    static inline std::unique_ptr<Mouse> m_instance;

    glm::vec2 m_last_position;
    glm::vec2 m_position;

    std::array<bool, 10> m_pressed;
    std::array<bool, 10> m_released;

    double m_scoll_xoffset;
    double m_scoll_yoffset;
    double m_last_scoll_xoffset;
    double m_last_scoll_yoffset;

    void pos_callback(GLFWwindow *, double xpos, double ypos)
    {
        m_last_position = m_position;
        m_position.x    = xpos;
        m_position.y    = ypos;
    }

    void button_callback(GLFWwindow *, int button, int action, int)
    {
        for (auto &state : m_pressed)
        {
            state = false;
        }
        for (auto &state : m_released)
        {
            state = false;
        }

        if (button >= MAX_KEYS)
        {
            return;
        }

        if (action == GLFW_PRESS)
        {
            m_pressed[button] = true;
        }

        if (action == GLFW_RELEASE)
        {
            m_released[button] = true;
        }
    }

    void scroll_callback(GLFWwindow *, double xoffset, double yoffset)
    {
        m_last_scoll_xoffset = m_scoll_xoffset;
        m_last_scoll_yoffset = m_scoll_yoffset;

        m_scoll_xoffset = xoffset;
        m_scoll_yoffset = yoffset;
    }

  public:
    static Mouse *get_instance()
    {
        if (!m_instance)
        {
            m_instance = std::make_unique<Mouse>();
        }
        return m_instance.get();
    }

    void init(GLFWwindow *t_window)
    {
        glfwSetCursorPosCallback(t_window, &Mouse::mouse_position_callback);
        glfwSetMouseButtonCallback(t_window, &Mouse::mouse_button_callback);
        glfwSetScrollCallback(t_window, &Mouse::mouse_scroll_callback);
    }

    glm::vec2 last_pos()
    {
        return m_last_position;
    }
    glm::vec2 pos()
    {
        return m_position;
    }

    double last_scroll()
    {
        return m_last_scoll_yoffset;
    }
    double scroll()
    {
        return m_scoll_yoffset;
    }

    bool is_pressed(MouseCode key)
    {
        return m_pressed[static_cast<int>(key)];
    }
    bool is_released(MouseCode key)
    {
        return m_released[static_cast<int>(key)];
    }
};

class Input
{
  private:
    static inline std::unique_ptr<Input> m_instance;

    GLFWwindow *m_window;

  public:
    static Input *get_instance()
    {
        if (!m_instance)
        {
            m_instance = std::make_unique<Input>();
        }
        return m_instance.get();
    }

    GLFWwindow *window()
    {
        return m_window;
    }


    void init(GLFWwindow *t_window)
    {
        m_window = t_window;
    }

    static bool is_pressed(MouseCode key)
    {
        auto state =
          glfwGetMouseButton(get_instance()->window(), static_cast<int32_t>(key));
        return (state == GLFW_PRESS || state == GLFW_REPEAT)
               and !ImGui::GetIO().WantCaptureMouse;
    }

    static bool is_pressed(KeyCode key)
    {
        auto state = glfwGetKey(get_instance()->window(), static_cast<int32_t>(key));
        return state == GLFW_PRESS and !ImGui::GetIO().WantCaptureKeyboard;
    }

    static bool is_released(MouseCode key)
    {
        auto state =
          glfwGetMouseButton(get_instance()->window(), static_cast<int32_t>(key));
        return state == GLFW_RELEASE and !ImGui::GetIO().WantCaptureMouse;
    }

    static bool is_released(KeyCode key)
    {

        auto state = glfwGetKey(get_instance()->window(), static_cast<int32_t>(key));
        return state == GLFW_RELEASE and !ImGui::GetIO().WantCaptureKeyboard;
    }

    static glm::vec2 mouse_pos()
    {
        auto window = static_cast<GLFWwindow *>(get_instance()->window());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return { (float)xpos, (float)ypos };
    }

    static void camera_cursor()
    {
        auto window = static_cast<GLFWwindow *>(get_instance()->window());
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    static void normal_cursor()
    {
        auto window = static_cast<GLFWwindow *>(get_instance()->window());
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    static void set_mouse_pos(glm::vec2 pos)
    {
        auto window = static_cast<GLFWwindow *>(get_instance()->window());
        glfwSetCursorPos(window, pos.x, pos.y);
    }
};

}  // namespace ay::app

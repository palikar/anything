#include "application/window.hpp"


namespace ay::app
{


void Window::init(int t_width, int t_height, std::string_view t_name)
{

    m_width  = t_width;
    m_height = t_height;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

    m_window = glfwCreateWindow(t_width, t_height, t_name.data(), nullptr, nullptr);

    if (!m_window)
    {
        std::cout << "Cannot open glfw window"
                  << "\n";
        return;
    }

    glfwGetWindowPos(m_window, &m_xpos, &m_ypos);

    glfwMakeContextCurrent(m_window);
    // glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwShowWindow(m_window);
    glfwSwapInterval(1);

    glfwSetWindowUserPointer(m_window, this);

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int width, int height) {
        auto &win = *(Window *)glfwGetWindowUserPointer(window);
        WindowResizeEvent event(width, height);
        win.m_width  = width;
        win.m_height = height;
        win.m_callback(event);
    });

    glfwSetWindowPosCallback(m_window, [](GLFWwindow *window, int xpos, int ypos) {
        auto &win  = *(Window *)glfwGetWindowUserPointer(window);
        win.m_xpos = xpos;
        win.m_ypos = ypos;

        WindowPositionEvent event(xpos, ypos);
        win.m_callback(event);
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window) {
        auto &win = *(Window *)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        win.m_callback(event);
    });

    glfwSetKeyCallback(m_window,
                       [](GLFWwindow *window, int key, int, int action, int mod) {
                           auto &win = *(Window *)glfwGetWindowUserPointer(window);

                           switch (action)
                           {
                           case GLFW_PRESS:
                           {
                               KeyPressedEvent event(static_cast<KeyCode>(key), 0, mod);
                               win.m_callback(event);
                               break;
                           }
                           case GLFW_RELEASE:
                           {

                               KeyReleasedEvent event(static_cast<KeyCode>(key), mod);
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

    glfwSetCharCallback(m_window, [](GLFWwindow *window, unsigned int keycode) {
        auto &win = *(Window *)glfwGetWindowUserPointer(window);
        KeyTypedEvent event(static_cast<KeyCode>(keycode));
        win.m_callback(event);
    });

    glfwSetMouseButtonCallback(
      m_window, [](GLFWwindow *window, int button, int action, int mod) {
          auto &win = *(Window *)glfwGetWindowUserPointer(window);

          switch (action)
          {
          case GLFW_PRESS:
          {
              MouseButtonPressedEvent event(static_cast<MouseCode>(button), mod);
              win.m_callback(event);
              break;
          }
          case GLFW_RELEASE:
          {
              MouseButtonReleasedEvent event(static_cast<MouseCode>(button), mod);
              win.m_callback(event);
              break;
          }
          }
      });

    glfwSetScrollCallback(m_window,
                          [](GLFWwindow *window, double xOffset, double yOffset) {
                              auto &win = *(Window *)glfwGetWindowUserPointer(window);
                              MouseScrolledEvent event((float)xOffset, (float)yOffset);
                              win.m_callback(event);
                          });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double xPos, double yPos) {
        auto &win = *(Window *)glfwGetWindowUserPointer(window);
        MouseMovedEvent event((float)xPos, (float)yPos);
        win.m_callback(event);
    });


    m_input = Input::get_instance();
    m_input->init(m_window);
}

void Window::dispose()
{
    glfwDestroyWindow(m_window);
}

void Window::update()
{
    glfwPollEvents();
}

void Window::render()
{
    glfwSwapBuffers(m_window);
}

bool Window::closed()
{
    return glfwWindowShouldClose(m_window);
}

}  // namespace ay::app




#include "application/application.hpp"


namespace ay::app
{

Application::Application(int t_width, int t_height, gmt::GameBase *t_game)
  : m_width(t_width)
  , m_height(t_height)
  , m_window(Window::get_instance())
  , m_engine(t_game)
{
}


int Application::run()
{
    init();

    double t             = 0.0;
    const double dt      = 1 / 60.0;
    double currentTime   = glfwGetTime();
    double accumulator   = 0.0;
    int frames           = 0;
    int updates          = 0;
    double frame_counter = 0.0;


    while (m_running)
    {
        double newTime   = glfwGetTime();
        double frameTime = newTime - currentTime;
        frame_counter += frameTime;
        bool render = false;
        currentTime = newTime;

        accumulator += frameTime;

        while (accumulator >= dt)
        {
            render    = true;
            m_running = !m_window->closed();

            m_window->update();
            m_engine.update(dt);
            ++updates;

            accumulator -= dt;
            t += dt;
        }

        if (frame_counter >= 1.0)
        {
            std::cout << "FPS: " << frames << " UPS: " << updates << "\n";
            frames        = 0;
            updates       = 0;
            frame_counter = 0;
        }

        if (render)
        {
            m_engine.render();
            m_window->render();
            ++frames;
        }
    }

    m_window->dispose();
    glfwTerminate();


    return 0;
}

void Application::init()
{
    if (!glfwInit())
    {
        std::cout << "Cannot initialize GLFW!"
                  << "\n";
        return;
    }

    m_window->init(m_width, m_height, "Anything");
    m_window->set_eventcall([this](Event &t_event) { this->on_event(t_event); });

    glewExperimental = GL_TRUE;
    GLenum err       = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "Glew Error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return;
    }

    ResouceLoader::get_instance()->init("../resources");
    std::cout << glGetString(GL_VERSION) << "\n";

    m_engine.init(m_width, m_height);
}


void Application::on_event(Event &t_event)
{
    Dispatcher dispatch{ t_event };
    dispatch.dispatch<WindowCloseEvent>([this](auto &e) { return this->on_close(e); });
    dispatch.dispatch<WindowResizeEvent>([this](auto &e) { return this->on_resize(e); });

    m_engine.on_event(t_event);
}


}  // namespace ay::app

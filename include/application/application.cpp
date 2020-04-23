#include "application/application.hpp"

#include <chrono>
#include <thread>

#include "ImGuizmo.h"

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
    using namespace std::chrono_literals;

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
            render_engine();
            m_window->render();
            ++frames;
        }
        std::this_thread::sleep_for(5ms);
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    m_window->dispose();
    glfwTerminate();


    return 0;
}


void Application::render_engine()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();
    ImGui::SetNextWindowPos(
      { static_cast<float>(m_window->xpos()), static_cast<float>(m_window->ypos()) });
    ImGui::SetNextWindowSize(
      { static_cast<float>(m_window->width()), static_cast<float>(m_window->height()) });
    ImGuizmo::SetRect(static_cast<float>(m_window->xpos()),
                      static_cast<float>(m_window->ypos()),
                      static_cast<float>(m_window->width()),
                      static_cast<float>(m_window->height()));
    ImGuizmo::SetDrawlist();

    ImGuizmo::BeginFrame();
    m_engine.render();

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void Application::init()
{
    if (!glfwInit())
    {
        std::cout << "Cannot initialize GLFW!"
                  << "\n";
        return;
    }

    ImGui::CreateContext();

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


    IMGUI_CHECKVERSION();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad
    // Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
    io.ConfigFlags |=
      ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform Windows
    // io.ConfigViewportsNoAutoMerge = true;
    // io.ConfigViewportsNoTaskBarIcon = true;
    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(m_window->native(), true);
    ImGui_ImplOpenGL3_Init("#version 410");

    m_engine.init(m_width, m_height);
}


void Application::on_event(Event &t_event)
{
    Dispatcher dispatch{ t_event };
   
    dispatch.dispatch<WindowResizeEvent>([this](auto &e) { return this->on_resize(e); });
    dispatch.dispatch<WindowCloseEvent>([this](auto &e) { return this->on_close(e); });
    dispatch.dispatch<WindowPositionEvent>(
        [this](auto &e) { return this->on_resposition(e); });

    
    if (!ImGui::GetIO().WantCaptureKeyboard)
    {
        m_engine.on_event(t_event);
    }
}


}  // namespace ay::app

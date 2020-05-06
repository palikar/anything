#include "application/application.hpp"

#include <chrono>
#include <thread>

#include "util/logging.hpp"

#include "commons.hpp"

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
            AY_INFO(fmt::format("FPS: {}; UPS: {}", frames, updates));
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

    shutdown();

    return 0;
}

void Application::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    m_window->dispose();
    glfwTerminate();
}

void Application::render_engine()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiIO &io = ImGui::GetIO();
    ImGuizmo::SetRect(io.DisplayFramebufferScale.x,
                      io.DisplayFramebufferScale.y,
                      io.DisplaySize.x,
                      io.DisplaySize.y);


    m_engine.render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Application::init()
{
    logging::init_logging(true, true);

    AY_DEBUG(fmt::format("Anything version: {}", BuildInfo::build()));

    if (!glfwInit())
    {
        AY_ERROR("Cannot initilize GLFW");
        return;
    }

    m_window->init(m_width, m_height, "Anything");
    m_window->set_eventcall([this](Event &t_event) { this->on_event(t_event); });

    glewExperimental = GL_TRUE;
    GLenum err       = glewInit();
    if (GLEW_OK != err)
    {
        AY_ERROR(
          fmt::format("Cannot initilize Glew. Error: {}", glewGetErrorString(err)));
        glfwTerminate();
        return;
    }

    AY_INFO(fmt::format("Using resource folder: {}", "../resources"));
    ResouceLoader::get_instance()->init("../resources");


    AY_DEBUG(fmt::format("Vendor: {}", glGetString(GL_VENDOR)));
    AY_DEBUG(fmt::format("OpenGL version: {}", glGetString(GL_VERSION)));
    AY_DEBUG(fmt::format("GLSL version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION)));


    init_imgui();

    m_engine.init(m_width, m_height);
}

void Application::init_imgui()
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.ConfigDockingWithShift = true;
    // io.ConfigViewportsNoAutoMerge   = true;
    // io.ConfigViewportsNoTaskBarIcon = true;
    io.ConfigWindowsResizeFromEdges = true;

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(m_window->native(), true);
    ImGui_ImplOpenGL3_Init("#version 410");
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

bool Application::on_close(WindowCloseEvent &)
{
    AY_DEBUG(fmt::format("Closing window"));
    m_running = false;
    return false;
}

bool Application::on_resize(WindowResizeEvent &e)
{
    AY_DEBUG(fmt::format("Resizing: {}x{}", e.width(), e.height()));
    m_engine.resize(e);
    return false;
}

bool Application::on_resposition(WindowPositionEvent &)
{
    return false;
}

void Application::set_title(const std::string &name)
{
    m_window->set_title(name);
}

void Application::set_icon(const std::string &icon_file)
{
    m_window->set_icon(icon_file);
}

void Application::focus()
{
}

}  // namespace ay::app

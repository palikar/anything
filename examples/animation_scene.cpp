#include "ay.hpp"

using namespace ay;

class AnimScene : public gmt::GameBase
{

  private:
    gmt::Scene3D* main_scene;
    rend::RendererScene3D renderer;
    cmp::OrbitalCameraComponent *oribital_camera_controller;


  public:

    AnimScene() = default;

    void init_basic()
    {
        renderer.init(engine()->api());
        main_scene = init_scene("main");
        main_scene->camera().init_prescpective_projection(glm::radians(65.0f), 1024.0/768.0, 0.001, 1000.0);

        main_scene->camera().set_look_at( glm::vec3(0.0f,0.0f,0.0f),
                                          glm::vec3(10,10, 10));

        oribital_camera_controller = main_scene->add_component<cmp::OrbitalCameraComponent>(&main_scene->camera());
        oribital_camera_controller->set_max_radius(30.0f);
        oribital_camera_controller->set_min_radius(0.10f);

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::Enable(true);
    }

    void init_lighting()
    {
        main_scene->directional_light(glm::vec3(0.5, 0.5f, 0), glm::vec3(0.8, 0.8f, 0.6f));
        main_scene->light_setup().directional_light.intensity = 1.1f;

        main_scene->ambient_light(glm::vec3(0.8, 0.8f, 0.6f));
        main_scene->light_setup().ambient_light.intensity = 0.15;
        

    }

    void init() override
    {
        init_basic();

        main_scene->add(gmt::axis());
        main_scene->add(gmt::grid_helper(50, 30));
        auto top_grid = main_scene->add(gmt::grid_helper(50, 30));
        cmp::transform(top_grid).translateY(10.0f);

        auto move_sphere = main_scene->add(gmt::object_mesh(
            { grph::sphere_geometry(0.7f, 20, 20), grph::solid_color(0.7, 0.2, 0.4) }));
        cmp::transform(move_sphere).set_position({5.0, 0.4, 5.0});

        auto move_animation = anim::vector_track(
            { { 5.0, 0.4, 5.0 }, { 5.0, 10.0 - 0.4, 5.0 }, { 5.0, 0.4, 5.0 } },
            { 0.0, 2.0, 4.0 });
        move_animation->set_target(&move_sphere->transform().position());
        move_animation->easing() = mth::EasingType::Bounce_in;
        animator().main_timeline().push_track("sphere_move", std::move(move_animation));

        auto color_sphere = main_scene->add(gmt::object_mesh(                                                
            { grph::sphere_geometry(0.7f, 20, 20), grph::solid_color(0.7, 0.2, 0.4) }));
        cmp::transform(color_sphere).set_position({-5.0, 0.4, 5.0});

        auto color_animation = anim::vector_track(
            { {0.7, 0.2, 0.4}, {0.2, 0.7, 0.5}, {0.7, 0.2, 0.4} },
            { 0.0, 1.0, 2.0 });
        color_animation->set_target(&color_sphere->material<grph::SolidColorMaterial>().parameters().m_color);
        color_animation->easing() = mth::EasingType::Quadratic_in_out;
        animator().main_timeline().push_track("sphere_color", std::move(color_animation));

        init_lighting();
    }

    void update(double dt) override
    {
        main_scene->update(dt);
    }

    bool event(app::Event& e) override
    {

        app::Dispatcher dispatch{ e };
        dispatch.dispatch<app::MouseButtonReleasedEvent>(MEMBER(mouse_press));
        dispatch.dispatch<app::KeyReleasedEvent>(MEMBER(key_press));
        dispatch.dispatch<app::WindowResizeEvent>(MEMBER(resizing));
        main_scene->event(e);
        return false;
    }

    bool key_press(app::KeyReleasedEvent& event)
    {

        if (event.key_code() == KeyCode::F5)
        {
            AY_INFO("Realoading shaders");
            this->shaders().reload_all();
        }

        return true;
    }

    bool resizing(app::WindowResizeEvent&)
    {
        return true;
    }

    bool mouse_press(app::MouseButtonReleasedEvent& )
    {
        return true;
    }

    void render(rend::RenderAPI&) override
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowBgAlpha(0.0f);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        bool open = true;
        ImGui::Begin("DockSpace Demo", &open, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        ImGuiDockNodeFlags dockspace_flags =  ImGuiDockNodeFlags_PassthruCentralNode;
        ImGui::DockSpace(dockspace_id, ImGui::GetMainViewport()->Size, dockspace_flags);
        
        static auto firstTime = true;
        if (firstTime)
        {
            firstTime = false;
            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_PassthruCentralNode);
            ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);
            
            ImGuiID dock_main_id = dockspace_id;
            ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.3f, NULL, &dock_main_id);

            ImGui::DockBuilderDockWindow("Animation", dock_id_bottom);
            ImGui::DockBuilderFinish(dockspace_id);
        }
        ImGui::End();

        
        if (ImGui::Begin("Animation"))
        {
            if (ImGui::TreeNode("Moving ball"))
            {
                
                ImGui::TreePop();
            }
            
        }
        ImGui::End();
        
        
        renderer.render_scene(*main_scene);

    }

};



int main()
{
    auto game = std::make_unique<AnimScene>();
    ay::app::Application app{1024, 768, game.get()};
    return app.run();
}

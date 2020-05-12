#include "ay.hpp"

using namespace ay;


class BallScene : public gmt::GameBase
{

    static constexpr int ball_grid_x = 20;
    static constexpr int ball_grid_y = 20;

  private:
    gmt::Scene3D* main_scene;
    rend::RendererScene3D renderer;

    cmp::OrbitalCameraComponent *oribital_camera_controller;
    cmp::FloatingCameraComponent *floating_camera_controller;

    gmt::Entity* entities[ball_grid_x][ball_grid_y];
    gmt::Entity* selected;

    gmt::Raycaster raycaster;

    ImGuizmo::OPERATION current_gizmo_operation{ImGuizmo::TRANSLATE};

  public:

    BallScene() = default;
    
    void init_basic()
    {
        renderer.init(engine()->api());
        main_scene = init_scene("main");
        main_scene->camera().init_prescpective_projection(glm::radians(65.0f), 1024.0/768.0, 0.001, 1000.0);
        main_scene->camera().set_look_at(glm::vec3(10,10, 10), glm::vec3(0.0f,0.0f,0.0f));
        init_orbital_camera();
    }

    void init_floating_camera()
    {
        main_scene->remove_component(oribital_camera_controller);
        oribital_camera_controller = nullptr;
        floating_camera_controller = main_scene->add_component<cmp::FloatingCameraComponent>(&main_scene->camera());
    }

    void init_orbital_camera()
    {
        main_scene->remove_component(floating_camera_controller);
        floating_camera_controller = nullptr;
        oribital_camera_controller = main_scene->add_component<cmp::OrbitalCameraComponent>(&main_scene->camera());
        oribital_camera_controller->set_max_radius(60.0f);
    }

    void init_lighting()
    {

        main_scene->directional_light(glm::vec3(0.5, 0.5f, 0), glm::vec3(0.8, 0.8f, 0.6f));
        main_scene->light_setup().directional_light.intensity = 0.08;

        main_scene->ambient_light(glm::vec3(0.8, 0.8f, 0.6f));
        main_scene->light_setup().ambient_light.intensity = 0.08;

        main_scene->point_light(0);
        main_scene->light_setup().point_lights[0].constant = 0.0;
        main_scene->light_setup().point_lights[0].linear = 0.098;
        main_scene->light_setup().point_lights[0].quadratic = 0.009;
        main_scene->light_setup().point_lights[0].quadratic = 0.009;
        main_scene->light_setup().point_lights[0].position[0] = 20;

        main_scene->spot_light(0);
        main_scene->light_setup().spot_lights[0].outer_cut_off = 1.05;
        main_scene->light_setup().spot_lights[0].cut_off = 0.88;

    }

    void init() override
    {
        init_basic();

        auto tex = rend::create_texture(app::ResouceLoader::path("textures/floor/floor-albedo.png"));
        auto sky = rend::create_cubetexture_jpgs(app::ResouceLoader::path("textures/cube/night_sky/"));

        main_scene->set_skybox(gmt::skybox(sky));

        main_scene->add(gmt::axis());
        main_scene->add(gmt::grid_helper(60, 20, rend::Colors::black));

        static constexpr float offset = 2.5f;
        static constexpr float radius = 0.5f;

        for (int i = 0; i < ball_grid_x; ++i)
        {
            for (int j = 0; j < ball_grid_y; ++j)
            {

                entities[i][j] = main_scene->add(gmt::mesh_entity({ grph::sphere_geometry(radius, 80, 80),
                                                                    grph::solid_color((1.0f * i) / ball_grid_x,
                                                                                      (1.0f * j) / ball_grid_y,
                                                                                      0.4f) }));

                cmp::transform(entities[i][j]).position() =
                    glm::vec3(i * offset - (ball_grid_x * offset * 0.5f),
                              0.0f,
                              j * offset - (ball_grid_y * offset * 0.5f)) ;

                cmp::transform(entities[i][j]).update();

                cmp::mesh(entities[i][j]).geometry().compute_bounding_box();
                cmp::mesh(entities[i][j]).geometry().compute_bounding_sphere();

            }
        }

        selected = nullptr;

        raycaster.update_camera(&main_scene->camera());
        raycaster.update_viewport(engine()->width(), engine()->height());

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

        if (!e.handled) {
            main_scene->event(e);
        }

        return false;
    }

    bool key_press(app::KeyReleasedEvent& event)
    {

        if (event.key_code() == KeyCode::F7)
        {
            if (oribital_camera_controller != nullptr)
            {
                init_floating_camera();
            }
            else
            {
                init_orbital_camera();
            }
        }

        if (event.key_code() == KeyCode::F5)
        {
            this->shaders().reload_all();

        }

        if (event.ctrl() and event.key_code() == KeyCode::R)
        {
            current_gizmo_operation = ImGuizmo::ROTATE;
        }

        if (event.ctrl() and event.key_code() == KeyCode::T)
        {
            current_gizmo_operation = ImGuizmo::TRANSLATE;
        }

        if (event.ctrl() and event.key_code() == KeyCode::S)
        {
            current_gizmo_operation = ImGuizmo::SCALE;
        }

        if (event.ctrl() and event.key_code() == KeyCode::A)
        {
            selected = nullptr;
        }

        return true;
    }

    bool resizing(app::WindowResizeEvent& event)
    {
        raycaster.update_viewport(event.width(), event.height());
        return true;
    }

    bool mouse_press(app::MouseButtonReleasedEvent& event)
    {

        if (event.ctrl()) {
            auto [obj, _] = raycaster.intersect_objects(&entities[0][0], ball_grid_y*ball_grid_x);
            if (obj) {
                selected = obj;
            }

        }

        return true;
    }

    float shining{1.0f};

    void render(rend::RenderAPI&) override
    {

        bool changed = false;
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::Enable(true);
        
        if (ImGui::CollapsingHeader("Ligting"))
        {

            if (ImGui::TreeNode("Directional light"))
            {
                ImGui::Text("Color");
                ImGui::SameLine();
                changed |= ImGui::ColorEdit3("color", (float*)&main_scene->light_setup().directional_light.color);
                changed |=ImGui::SliderFloat("Intensity:", (float*)&main_scene->light_setup().directional_light.intensity, 0.0f, 2.0f, "Inesity = %.3f");
                changed |=ImGui::SliderFloat("Angle:", (float*)&main_scene->light_setup().directional_light.dir[1], -1.0f, 3.0f, "Direction = %.3f");
                ImGui::TreePop();
            }


            if (ImGui::TreeNode("Ambient light"))
            {
                ImGui::Text("Color");
                ImGui::SameLine();
                changed |=ImGui::ColorEdit3("color", (float*)&main_scene->light_setup().ambient_light.color);
                changed |=ImGui::SliderFloat("Intensity:", (float*)&main_scene->light_setup().ambient_light.intensity, 0.0f, 2.0f, "Inesity = %.3fp");
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Point light"))
            {
                ImGui::Text("Color");
                ImGui::SameLine();
                changed |=ImGui::ColorEdit3("Color:", (float*)&main_scene->light_setup().point_lights[0].color);
                changed |=ImGui::SliderFloat("Constant:", (float*)&main_scene->light_setup().point_lights[0].constant, 0.0f, 0.50f, "Value = %.3f");
                changed |=ImGui::SliderFloat("Linear:", (float*)&main_scene->light_setup().point_lights[0].linear, 0.0f, 0.50f, "Value = %.3f");
                changed |=ImGui::SliderFloat("Quadratic:", (float*)&main_scene->light_setup().point_lights[0].quadratic, 0.0f, 0.5f, "Value = %.3f");
                changed |=ImGui::SliderFloat("Y pos:", (float*)&main_scene->light_setup().point_lights[0].position[1], 0.0f, 10.0f, "Value = %.3f");
                ImGui::TreePop();
            }

            
            if (ImGui::TreeNode("Spot light"))
            {
                ImGui::Text("Color");
                ImGui::SameLine();
                changed |=ImGui::ColorEdit3("Color:", (float*)&main_scene->light_setup().spot_lights[0].color);

                changed |=ImGui::SliderFloat("Inner:", (float*)&main_scene->light_setup().spot_lights[0].cut_off, 0.0f, 6.28f, "Value = %.3f");
                changed |=ImGui::SliderFloat("Outer:", (float*)&main_scene->light_setup().spot_lights[0].outer_cut_off, 0.0f, 5.0f, "Value = %.3f");

                // changed |=ImGui::SliderFloat("X pos:", (float*)&main_scene->light_setup().spot_lights[0].position[0], 0.0f, 10.0f, "Value = %.3f");
                changed |=ImGui::SliderFloat("Y pos:", (float*)&main_scene->light_setup().spot_lights[0].position[1], 0.0f, 20.0f, "Value = %.3f");
                ImGui::TreePop();
            }

        }

        changed |=ImGui::SliderFloat("Specular", &shining, 0.1f, 20.0f, "Inesity = %.3f");


        if (selected ) {
            ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
            ImGuizmo::BeginFrame();
            ImGuizmo::Manipulate(glm::value_ptr(main_scene->camera().view()),
                                 glm::value_ptr(main_scene->camera().projection()),
                                 current_gizmo_operation,
                                 mCurrentGizmoMode,
                                 glm::value_ptr(cmp::transform(selected).transform()));

        }


        float x = 10 * sin(glfwGetTime());
        float y = 10 * cos(glfwGetTime());

        main_scene->light_setup().spot_lights[0].position[0] = x;
        main_scene->light_setup().spot_lights[0].position[2] = y;

        main_scene->light_setup().point_lights[0].position[2] = 20 * sin(glfwGetTime());

        main_scene->light_setup().needs_update = true;

        for (int i = 0; i < ball_grid_x; ++i)
        {
            for (int j = 0; j < ball_grid_y; ++j)
            {
                cmp::mesh(entities[i][j]).material<grph::SolidColorMaterial>()->parameters().m_shininess = shining;
            }
        }

        renderer.render_scene(*main_scene);

    }

};

int main()
{
    auto game = std::make_unique<BallScene>();
    ay::app::Application app{1024, 768, game.get()};
    return app.run();
}

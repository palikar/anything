#include "ay.hpp"

using namespace ay;


class TerrainScene : public gmt::GameBase
{

  private:
    gmt::Scene3D* main_scene;
    rend::RendererScene3D renderer;
    
    gmt::Entity* floor_mesh;

    gmt::Object3D* sphere;

  public:

    TerrainScene() = default;

    void init_basic()
    {
        renderer.init(engine()->api());
        main_scene = init_scene("main");
        main_scene->camera().init_prescpective_projection(glm::radians(45.0f), 1024.0/768.0, 0.001, 10000.0);
        main_scene->camera().set_look_at(glm::vec3(10,10, 10), glm::vec3(0.0f,0.0f,0.0f));

        // oribital_camera_controller = main_scene->add_component<cmp::OrbitalCameraComponent>(&main_scene->camera());
        // oribital_camera_controller->set_max_radius(200.0f);
        // oribital_camera_controller->set_min_radius(0.005f);

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::Enable(true);
    }

    void init_lighting()
    {
        main_scene->directional_light(glm::vec3(0.5, 0.5f, 0), glm::vec3(0.8, 0.8f, 0.6f));
        main_scene->light_setup().directional_light.intensity = 1.01f;

        main_scene->ambient_light(glm::vec3(0.8, 0.8f, 0.6f));
        main_scene->light_setup().ambient_light.intensity = 0.08f;

        // main_scene->point_light(0);
        // main_scene->light_setup().point_lights[0].constant = 0.0;
        // main_scene->light_setup().point_lights[0].linear = 0.074;
        // main_scene->light_setup().point_lights[0].quadratic = 0.000;
        // main_scene->light_setup().point_lights[0].position = {5.0f, 10.0f, 5.0f};
        // main_scene->light_setup().point_lights[0].color = {0.8f,0.8f,0.8f};

        // pointlight = main_scene->add(gmt::pointlight_helper(5.0f));
        // cmp::transform(pointlight).set_position({5.0f, 10.0f, 5.0f});

    }

    void init() override
    {
        init_basic();

        auto sky = rend::create_cubetexture_jpgs(app::ResouceLoader::path("textures/cube/sky/"));
        auto night_sky = rend::create_cubetexture_jpgs(app::ResouceLoader::path("textures/cube/night_sky/"));

        auto floor = textures().load("floor/floor-albedo.png", gmt::MapType::ALBEDO_MAP);
        auto floor_normal = textures().load("floor/floor-normal.png", gmt::MapType::NORMAL_MAP);
        auto floor_bump = textures().load("floor/floor-height.png", gmt::MapType::HEIGHT_MAP);

        auto brick_ao = textures().load("bricks/brick_ao.jpg", gmt::MapType::AO_MAP);
        auto brick = textures().load("bricks/brick_base.jpg", gmt::MapType::DIFFUSE_MAP);
        auto brick_normal =textures().load("bricks/brick_normal.jpg", gmt::MapType::NORMAL_MAP);
        auto brick_bump = textures().load("bricks/brick_height.png", gmt::MapType::HEIGHT_MAP);

        auto rocks = textures().load("stones/rocks_color.jpg", gmt::MapType::DIFFUSE_MAP);
        auto rocks_ao = textures().load("stones/rocks_ao.jpg", gmt::MapType::AO_MAP);
        auto rocks_normal = textures().load("stones/rocks_normal.jpg", gmt::MapType::NORMAL_MAP);
        auto rocks_bump = textures().load("stones/rocks_displacement.jpg", gmt::MapType::HEIGHT_MAP);

        main_scene->add(gmt::axis());
        main_scene->set_skybox(gmt::skybox(sky));

        floor_mesh = main_scene->add(gmt::mesh_entity({grph::plane_geometry(500, 500, 50, 50), grph::texture_material(rocks)}));
        cmp::transform(floor_mesh).rotateX(glm::radians(90.0f));

        auto x_wing = main_scene->add(gmt::model_entity(load::Loader::load_model(app::ResouceLoader::obj("star-wars-x-wing.blend"))));
        cmp::transform(x_wing).rotateY(glm::radians(180.0f));
        cmp::transform(x_wing).rotateX(glm::radians(-90.0f));
        cmp::transform(x_wing).translateY(1.0);
        cmp::transform(x_wing).set_scale({0.2f, 0.2f, 0.2f});
        cmp::transform(x_wing).update();

        x_wing->add_component<cmp::TrackingCameraComponent>(cmp::transform(x_wing), main_scene->camera());
        x_wing->add_component<cmp::MovementComponent>(cmp::transform(x_wing));

        for (size_t i = 0; i < 130; ++i)
        {

            float x = util::Random::uniform_real(-150.0f, 150.0f);
            float z = util::Random::uniform_real(-150.0f, 150.0f);

            auto block =
                main_scene->add(gmt::mesh_entity({ grph::cube_geometry(3, 20, 3, 1, 1, 1),
                                                   grph::solid_color(0.7, x / 250.0, 0.3) }));
            cmp::transform(block).set_position({x, 7, z});

        }

        sphere = main_scene->add(gmt::object_mesh({grph::sphere_geometry(0.7f, 20, 20), grph::solid_color(1.0, 0.0, 0.0)}));
        
        auto sphere_move = anim::vector_track(
            { { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 5.0f, 5.0f }, { 0.0f, 0.0f, 0.0f } },
            { 0.0, 2.0, 4.0, 6.0 });
        sphere_move->set_target(&sphere->transform().position());
        animator().main_timeline().push_track("sphere_move", std::move(sphere_move));

        auto sphere_scale = anim::vector_track(
            { { 1.0f, 1.0f, 1.0f }, { 5.0f, 5.0f, 5.0f }, { 3.0f, 3.0f, 3.0f }, { 1.0f, 1.0f, 1.0f } },
            { 0.0, 2.0, 4.0, 6.0 });
        sphere_scale->set_target(&sphere->transform().scale());
        animator().main_timeline().push_track("sphere_scale", std::move(sphere_scale));
        animator().main_timeline().ping_pong();
        
        animator().play();

        init_lighting();
        main_scene->set_fog(grph::linear_fog({0.1, 1.0, 1.0}, 50, 100));
    }
    
    void update(double dt) override
    {
        main_scene->update(dt);
        sphere->transform().update();
    }

    bool event(app::Event& e) override
    {

        app::Dispatcher dispatch{e};
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
            AY_DEBUG("Realoading shaders");
            this->shaders().reload_all();
        }

        if (event.key_code() == KeyCode::F10)
        {
            gl::take_screenshot("screen.png");
        }

        return false;
    }

    bool resizing(app::WindowResizeEvent&)
    {
        return false;
    }

    bool mouse_press(app::MouseButtonReleasedEvent& )
    {
        return false;
    }

    void render(rend::RenderAPI&) override
    {
        bool changed = false;

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
            ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);

            ImGui::DockBuilderDockWindow("Fog", dock_id_left);
            ImGui::DockBuilderDockWindow("Lighting", dock_id_left);
            ImGui::DockBuilderFinish(dockspace_id);
        }
        
        ImGui::End();

        if (ImGui::Begin("Fog"))
        {

            ImGui::Checkbox("Active", &main_scene->fog().activ());
            ImGui::Separator();
            ImGui::ColorEdit3("Color", (float*)&main_scene->fog().color());
            ImGui::Separator();

            const char* type[] = {"None", "Linear", "Exp2"};
            static int type_current = 1;
            if (ImGui::Combo("Fog type", &type_current, type, 3)) {
                main_scene->fog().type() = grph::FogType{type_current};
            }

            ImGui::Separator();
            ImGui::SliderFloat("Near", (float*)&main_scene->fog().near(), 0.0f, 100.0f, "Value = %.3f");
            ImGui::SliderFloat("Far", (float*)&main_scene->fog().far(), 0.0f, 500.0f, "Value = %.3f");
            ImGui::Separator();
            ImGui::SliderFloat("Density", (float*)&main_scene->fog().density(), 0.0f, 0.005f, "Value = %.3f");
            ImGui::End();
        }
        else
        {
            ImGui::End();
        }
        if (ImGui::Begin("Lighting"))
        {
            if (ImGui::TreeNode("Directional light"))
            {
                ImGui::Text("Color");
                ImGui::SameLine();
                changed |= ImGui::ColorEdit3("color", (float*)&main_scene->light_setup().directional_light.color);
                changed |= ImGui::SliderFloat("Intensity:", (float*)&main_scene->light_setup().directional_light.intensity, 0.0f, 2.0f, "Inesity = %.3f");
                changed |= ImGui::SliderFloat("Angle:", (float*)&main_scene->light_setup().directional_light.dir[1], -1.0f, 3.0f, "Direction = %.3f");
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
                changed |= ImGui::SliderFloat("Y pos:", (float*)&main_scene->light_setup().spot_lights[0].position[1], 0.0f, 20.0f, "Value = %.3f");
                ImGui::TreePop();
            }
            main_scene->light_setup().needs_update = changed;
            ImGui::End();
        }
        else
        {
            ImGui::End();
        }


        renderer.render_scene(*main_scene);

    }

};



int main()
{
    auto game = std::make_unique<TerrainScene>();
    ay::app::Application app{1024, 768, game.get()};
    return app.run();
}

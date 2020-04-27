#include "ay.hpp"

using namespace ay;

class MatScene : public gmt::GameBase
{

  private:
    gmt::Scene3D* main_scene;
    rend::RendererScene3D renderer;
    cmp::OrbitalCameraComponent *oribital_camera_controller;

    gmt::Entity* sphere;
    gmt::Entity* cube;
    gmt::Entity* torus;

    gmt::BoxHelper* box;
    gmt::PointlightHelper* pointlight;
    gmt::SpotlightHelper* spotlight;

    std::vector<rend::CubeTexturePtr> env_maps;
    std::vector<rend::TexturePtr> texs;

  public:

    MatScene() = default;

    void init_basic()
    {
        renderer.init(engine()->api());
        main_scene = init_scene("main");
        main_scene->camera().init_prescpective_projection(glm::radians(65.0f), 1024.0/768.0, 0.001, 1000.0);
        main_scene->camera().set_look_at(glm::vec3(10,10, 10), glm::vec3(0.0f,0.0f,0.0f));
        oribital_camera_controller = main_scene->add_component<cmp::OrbitalCameraComponent>(&main_scene->camera());
        oribital_camera_controller->set_max_radius(60.0f);

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::Enable(true);
    }

    void init_lighting()
    {

    }

    void init() override
    {
        init_basic();

        rend::TexturePtr floor = rend::create_texture(app::ResouceLoader::path("textures/floor/floor-albedo.png"));
        rend::TexturePtr floor_normal = rend::create_texture(app::ResouceLoader::path("textures/floor/floor-normal.png"));
        rend::TexturePtr brick = rend::create_texture(app::ResouceLoader::path("textures/bricks/brick_base.jpg"));
        rend::CubeTexturePtr sky = rend::create_cubetexture_jpgs(app::ResouceLoader::path("textures/cube/sky/"));
        rend::CubeTexturePtr night_sky = rend::create_cubetexture_jpgs(app::ResouceLoader::path("textures/cube/night_sky/"));
        rend::TexturePtr brick_ao = rend::create_texture(app::ResouceLoader::path("textures/bricks/brick_ao.jpg"));

        rend::TexturePtr rocks = rend::create_texture(app::ResouceLoader::path("textures/stones/rocks_color.jpg"));
        rend::TexturePtr rocks_normal = rend::create_texture(app::ResouceLoader::path("textures/stones/rocks_normal.jpg"));

        texs.push_back(brick);
        texs.push_back(floor);
        env_maps.push_back(sky);
        env_maps.push_back(night_sky);

        texs.push_back(brick_ao);

        // main_scene->add(gmt::axis());
        // main_scene->set_skybox(gmt::skybox(sky));

        auto floor_mesh = main_scene->add(gmt::mesh_entity({grph::plane_geometry(100, 100, 50, 50), grph::phong_material(0.01, 0.01, 0.01)}));
        cmp::transform(floor_mesh).rotateX(glm::radians(-90.0f));
        cmp::mesh(floor_mesh).geometry().calculate_tangents();
        cmp::mesh(floor_mesh).material<grph::PhongMaterial>()->parameters().m_map = floor;
        cmp::mesh(floor_mesh).material<grph::PhongMaterial>()->parameters().m_normal_map = floor_normal;

        // auto wall_mesh = main_scene->add(gmt::mesh_entity({grph::plane_geometry(100, 100, 50, 50), grph::texture_material(floor)}));
        // cmp::transform(wall_mesh).translateY(50.0f);
        // cmp::transform(wall_mesh).translateZ(-50.0f);

        

        // // Initing the cube
        // cube = main_scene->add(gmt::mesh_entity({grph::plane_geometry(50, 50, 30, 30), grph::solid_color(rend::Colors::blue)}));
        // cmp::transform(cube).set_position({0.0f , 5.0f, 0.0f});
        // cmp::transform(cube).rotateX(glm::radians(-90.0f));
        

        // Initing the torus
        // auto phong_mat = grph::phong_material(1.0, 0.2, 0.2);
        // torus = main_scene->add(gmt::mesh_entity({grph::torus_geometry(5, 1, 30, 30), std::move(phong_mat)}));
        // cmp::transform(torus).set_position({0.0f , 10.0f, 0.0f});
        // cmp::mesh(torus).geometry().calculate_tangents();
        // cmp::mesh(torus).material<grph::PhongMaterial>()->parameters().m_map = rocks;
        // cmp::mesh(torus).material<grph::PhongMaterial>()->parameters().m_normal_map = rocks_normal;

        // Initing the sphere

        // auto brick_mat = grph::texture_material(brick);
        // grph::TexturedMaterialBuilder::from_existing(brick_mat.get()).color(glm::vec3{0.0f, 0.6f, 0.0f});
        // sphere = main_scene->add(gmt::mesh_entity({grph::sphere_geometry(5, 40, 40), std::move(brick_mat)}));
        // cmp::transform(sphere).set_position({0.0f , 10.0f, 15.0f});
        // cmp::mesh(sphere).geometry().compute_bounding_box();

        main_scene->directional_light(glm::vec3(0.5, 0.5f, 0), glm::vec3(0.8, 0.8f, 0.6f));
        main_scene->light_setup().directional_light.intensity = 0.08f;

        main_scene->ambient_light(glm::vec3(0.8, 0.8f, 0.6f));
        main_scene->light_setup().ambient_light.intensity = 0.08f;

        main_scene->point_light(0);
        main_scene->light_setup().point_lights[0].constant = 0.0;
        main_scene->light_setup().point_lights[0].linear = 0.074;
        main_scene->light_setup().point_lights[0].quadratic = 0.000;
        main_scene->light_setup().point_lights[0].position = {5.0f, 10.0f, 5.0f};
        main_scene->light_setup().point_lights[0].color = {0.8f,0.8f,0.8f};

        // box = main_scene->add(gmt::box_helper(mth::Box3{{-50.0f, -50.0f, -50.0f}, {50.0f, 50.0f, 50.0f}}));
        // box = main_scene->add(gmt::box_helper(cmp::mesh(sphere).geometry().bounding_box()));

        

        pointlight = main_scene->add(gmt::pointlight_helper(5.0f));
        cmp::transform(pointlight).set_position({5.0f, 10.0f, 5.0f});

        // spotlight = main_scene->add(gmt::spotlight_helper(20));
        // cmp::transform(spotlight).set_position({-20.0f, 10.0f, -20.0f});

    }

    void update(double dt) override
    {

        // cmp::transform(torus).rotate(glm::vec3{1,1,1}, static_cast<float>(dt) * 1.5);
        // cmp::transform(cube).rotate(glm::vec3{1,1,1}, static_cast<float>(dt) * 1.5);

        // cmp::transform(sphere).translateY(std::sin(glfwGetTime() * 10.5f)*0.3);
        // cmp::transform(box).set_position(cmp::transform(sphere).position());

        float x = 15*std::sin(glfwGetTime());
        float z = 15*std::cos(glfwGetTime());        
        main_scene->light_setup().point_lights[0].position[0] = x;
        main_scene->light_setup().point_lights[0].position[1] = 10;
        main_scene->light_setup().point_lights[0].position[2] = z;
        cmp::transform(pointlight).set_position({x, 10, z});
        main_scene->light_setup().needs_update = true;


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

        if (event.key_code() == KeyCode::F5)
        {
            std::cout << "Realoading shaders" << "\n";
            this->shaders().reload_all();
        }

        if (event.key_code() == KeyCode::F10)
        {
            gl::take_screenshot("screen.png");
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

        bool changed = false;
        
        if (ImGui::TreeNode("Sphere"))
        {
            ImGui::Text("Base material");

            ImGui::Checkbox("Visible", (bool*)&cmp::mesh(sphere).material<grph::Material>()->parameters().m_visible);
            ImGui::SameLine();
            ImGui::Checkbox("Transparent", (bool*)&cmp::mesh(sphere).material<grph::Material>()->parameters().m_transparent);
            ImGui::SameLine();
            ImGui::Checkbox("Wire Frame", (bool*)&cmp::mesh(sphere).material<grph::Material>()->parameters().m_wire_frame);


            ImGui::Checkbox("Enable blending", (bool*)&cmp::mesh(sphere).material<grph::Material>()->parameters().m_blending.blending);
            ImGui::SliderFloat("Opacity", (float*)&cmp::mesh(sphere).material<grph::Material>()->parameters().m_opacity, 0.0f, 1.0f, "Value = %.3f");
            ImGui::SliderFloat("Alpha test", (float*)&cmp::mesh(sphere).material<grph::Material>()->parameters().m_blending.alpha_test, 0.0f, 1.0f, "Value = %.3f");

            ImGui::Checkbox("Depth test", (bool*)&cmp::mesh(sphere).material<grph::Material>()->parameters().m_depth_test);
            ImGui::SameLine();
            ImGui::Checkbox("Depth write", (bool*)&cmp::mesh(sphere).material<grph::Material>()->parameters().m_depth_write);

            const char* sides[] = {"Front", "Back", "Both"};
            static int current = 0;
            ImGui::Combo("Render side", &current, sides, 3);
            cmp::mesh(sphere).material<grph::Material>()->parameters().m_side = rend::Side{current};

            ImGui::Separator();
            ImGui::Text("Texture material");

            ImGui::Text("Color");
            ImGui::SameLine();
            ImGui::ColorEdit3("Color", (float*)&cmp::mesh(sphere).material<grph::TextureMaterial>()->parameters().m_color);
            ImGui::SliderFloat("Color intesity", (float*)&cmp::mesh(sphere).material<grph::TextureMaterial>()->parameters().m_color_intensity, 0.0f, 1.0f, "Value = %.3f");

            ImGui::Checkbox("Is Reflection", (bool*)&cmp::mesh(sphere).material<grph::TextureMaterial>()->parameters().m_is_reflection);
            ImGui::SliderFloat("Reflectivity", (float*)&cmp::mesh(sphere).material<grph::TextureMaterial>()->parameters().m_reflectivity, 0.0f, 1.0f, "Value = %.3f");
            ImGui::SliderFloat("Refraction ration", (float*)&cmp::mesh(sphere).material<grph::TextureMaterial>()->parameters().m_refraction_ration, 0.0f, 1.0f, "Value = %.3f");

            ImGui::SliderFloat("AO intesity", (float*)&cmp::mesh(sphere).material<grph::TextureMaterial>()->parameters().m_ao_intensity, 0.0f, 1.0f, "Value = %.3f");

            const char* maps[] = {"Bricks", "Floor"};
            static int map_current = 0;
            ImGui::SetNextItemWidth(100);
            ImGui::Combo("Map", &map_current, maps, 2);
            cmp::mesh(sphere).material<grph::TextureMaterial>()->parameters().m_map = texs[map_current];
            cmp::mesh(sphere).material<grph::TextureMaterial>()->parameters().m_ao_map = texs[2];
            cmp::mesh(sphere).material<grph::TextureMaterial>()->parameters().m_specular_map = texs[2];

            const char* envs[] = {"Sky", "Night Sky"};
            static int env_current = 0;

            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            ImGui::Combo("Env Map", &env_current, envs, 2);
            cmp::mesh(sphere).material<grph::TextureMaterial>()->parameters().m_env_map = env_maps[env_current];

            const char* mixing[] = {"Add", "Multiply", "Mix"};
            static int mixing_current = 0;
            ImGui::Combo("Mixing", &mixing_current, mixing, 3);
            cmp::mesh(sphere).material<grph::TextureMaterial>()->parameters().m_combine = rend::Combine{mixing_current};

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Torus"))
        {
            ImGui::Text("Texture material");

            ImGui::ColorEdit3("Diffuse", (float*)&cmp::mesh(torus).material<grph::PhongMaterial>()->parameters().m_color);
            ImGui::ColorEdit3("Ambient", (float*)&cmp::mesh(torus).material<grph::PhongMaterial>()->parameters().m_ambient);
            ImGui::ColorEdit3("Specular", (float*)&cmp::mesh(torus).material<grph::PhongMaterial>()->parameters().m_specular);

            ImGui::SliderFloat("Shininess:", (float*)&cmp::mesh(torus).material<grph::PhongMaterial>()->parameters().m_shininess, 5.0f, 100.0f, "Exponent = %.3f");

            ImGui::Separator();
            
            ImGui::ColorEdit3("Emissiive", (float*)&cmp::mesh(torus).material<grph::PhongMaterial>()->parameters().m_emissive);
            
            ImGui::TreePop();
            
        }

        if (ImGui::CollapsingHeader("Ligting"))
        {

            if (ImGui::SliderFloat("Light pos", (float*)&main_scene->light_setup().point_lights[0].position[0], -10.0f, 10.0f, "Position = %.3f")){
                cmp::transform(pointlight).position()[0] = main_scene->light_setup().point_lights[0].position[0];
                cmp::transform(pointlight).update();
            };
            
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
                changed |=ImGui::SliderFloat("Y pos:", (float*)&main_scene->light_setup().spot_lights[0].position[1], 0.0f, 20.0f, "Value = %.3f");
                ImGui::TreePop();
            }

        }

        renderer.render_scene(*main_scene);
        
    }

};



int main()
{
    auto game = std::make_unique<MatScene>();
    ay::app::Application app{1024, 768, game.get()};
    return app.run();
}
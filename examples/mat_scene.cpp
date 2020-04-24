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
        
        auto floor = rend::create_texture(app::ResouceLoader::path("textures/floor/floor-albedo.png"));
        auto brick = rend::create_texture(app::ResouceLoader::path("textures/bricks/brick_base.jpg"));
        auto sky = rend::create_cubetexture_jpgs(app::ResouceLoader::path("textures/cube/sky/"));

        main_scene->set_skybox(gmt::skybox(sky));

        main_scene->add(gmt::axis());

        auto floor_mesh = main_scene->add(gmt::mesh_entity({grph::plane_geometry(150, 150, 50, 50), grph::texture_material(floor)}));
        cmp::transform(floor_mesh).rotateX(glm::radians(-90.0f));
        
        auto wall_mesh = main_scene->add(gmt::mesh_entity({grph::plane_geometry(150, 150, 50, 50), grph::texture_material(floor)}));
        cmp::transform(wall_mesh).translateY(75.0f);
        cmp::transform(wall_mesh).translateZ(-75.0f);


        // Initing the cube
        cube = main_scene->add(gmt::mesh_entity({grph::cube_geometry(7, 7, 7, 40, 40, 40), grph::solid_color(rend::Colors::blue)}));
        cmp::transform(cube).set_position({0.0f , 10.0f, 0.0f});

        // Initing the torus
        torus = main_scene->add(gmt::mesh_entity({grph::torus_geometry(5, 1, 30, 30), grph::solid_color(rend::Colors::green)}));
        cmp::transform(torus).set_position({0.0f , 10.0f, -15.0f});

        // Initing the sphere
        auto brick_mat = grph::texture_material(brick);
        grph::TexturedMaterialBuilder::from_existing(brick_mat.get()).color(glm::vec3{0.0f, 0.6f, 0.0f});
        
        sphere = main_scene->add(gmt::mesh_entity({grph::sphere_geometry(5, 40, 40), std::move(brick_mat)}));
        cmp::transform(sphere).set_position({0.0f , 10.0f, 15.0f});


    }

    void update(double dt) override
    {

        cmp::transform(torus).rotate(glm::vec3{1,1,1}, static_cast<float>(dt) * 1.5);
        cmp::transform(cube).rotate(glm::vec3{1,1,1}, static_cast<float>(dt) * 1.5);
        cmp::transform(sphere).translateY(std::sin(glfwGetTime() * 10.5f)*0.3);
        
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
                
            ImGui::TreePop();
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

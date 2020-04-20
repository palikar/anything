#include <iostream>
#include <memory>
#include <utility>
#include <cmath>

#include "ay.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace ay;


class SimpleGame : public gmt::GameBase {

  private:
    gmt::Scene3D* main_scene;
    rend::RendererScene3D renderer;

    gmt::Entity* cube_1;
    gmt::Entity* cube_2;
    gmt::Entity* cube_3;

    gmt::Entity* cone;
    
    gmt::Entity* plane;

    cmp::OrbitalCameraComponent* camera_controller;

  public:

    SimpleGame()
    {

    }

    void init_basic()
    {
        renderer.init(engine()->api());
        main_scene = init_scene("main");
        main_scene->camera().init_prescpective_projection(glm::radians(55.0f), 1024.0/768.0, 0.001, 1000.0);
        main_scene->camera().set_look_at(glm::vec3(0,10,1), glm::vec3(0.0f,0.0f,0.0f));
        camera_controller = main_scene->add_component<cmp::OrbitalCameraComponent>(&main_scene->camera());
        camera_controller->set_max_radius(60.0f);
    }

    void init() override
    {
        init_basic();

        
        auto tex = rend::create_texture(app::ResouceLoader::get_instance()->get_file_path("textures/floor/floor-albedo.png"));
        // plane = main_scene->add(gmt::mesh_entity(
        //                             {grph::sphere_geometry(2, 10, 10),
        //                              grph::texture_material(tex)}));

        // cmp::transform(plane).rotation() = glm::angleAxis(glm::radians(90.0f), glm::vec3(1,0,0));
        // cmp::transform(plane).translateY(1.0);

        

        auto sky = rend::create_cubetexture_jpgs(app::ResouceLoader::path("textures/cube/sky/"));
        main_scene->set_skybox(gmt::skybox(sky));

        // cube_2 = main_scene->add(gmt::axis());

        // auto geom = load::Loader::load_geometry(app::ResouceLoader::path("objs/bunny.obj"));
        // geom.pack();

        // cone = main_scene->add(gmt::mesh_entity({std::move(geom), grph::solid_color(1, 0, 0.5)}));
        cone = main_scene->add(gmt::grid_helper(10, 10, rend::Colors::black));

        // cmp::mesh(plane).material()->set_wire_frame(true);
        // static_cast<grph::TextureMaterial*>(cmp::mesh(plane).material())->set_env_map(sky);
        
        
        // auto cube_mat = grph::solid_color({ 0.0f, 0.0f, 1.0f });
        // grph::MaterialBuilder::from_existing(cube_mat.get())
        //     .wire_frame(true)
        //     // .enable_lending()
        //     // .alpha_blending()
        //     // .opacity(0.5)
        //     .transparent(false);



        // cube_2 = main_scene->add(gmt::plane_helper());
        
        // // cmp::transform(cube_2).translateY(3.0f);

        // cube_1 = main_scene->add(gmt::mesh_entity({grph::torus_geometry(), std::move(cube_mat)}));

        // cube_3 = main_scene->add(gmt::mesh_entity({grph::cube_geometry(), grph::solid_color(0, 0, 1)}));
        // cmp::transform(cube_3).translateZ(3.0f);
        // cmp::transform(cube_3).translateY(1.0f);

        
        // // glm::mat4 orth = glm::ortho(-3.0f, 3.0f, -5.0f, 5.0f, 0.1f, 5.0f);
        // glm::mat4 orth = glm::perspective(glm::radians(32.0f), 1024.0f/768.0f, 0.001f, 100.0f);
        

        // main_scene->add(std::make_unique<gmt::CameraHelper>(orth));
        
        
        // cone = main_scene->add(gmt::line_segments_entity( {grph::solid_color({ 0.0f, 1.0f, 1.0f })}));
        // cmp::line_segments(cone).add_point({0.0f, 0.0f, 0.0f});
        // cmp::line_segments(cone).add_point({3.0f, 0.0f, 0.0f});
        
        // cmp::line_segments(cone).add_point({0.0f, 0.0f, 0.0f});
        // cmp::line_segments(cone).add_point({0.0f, 3.0f, 0.0f});
        
        // cmp::line_segments(cone).add_point({0.0f, 0.0f, 0.0f});
        // cmp::line_segments(cone).add_point({0.0f, 0.0f, 3.0f});
        
        // cmp::line_segments(cone).geometry().pack();


    }

    void update(double dt) override
    {

        // cmp::transform(cube_2).rotateZ(glm::radians(50*dt));

        main_scene->update(dt);
    }

    bool event(app::Event& e) override
    {
        main_scene->event(e);
        return false;
    }

    glm::vec3 plane_color{1.0f, 0.0f, 1.0f};
    float reflectivity;
    float refraction;
    bool reflective{true};
    float rot{0};

    void render(rend::RenderAPI&) override
    {
        renderer.render_scene(*main_scene);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();


        ImGui::NewFrame();

        // ImGui::Begin("Hello, world!");

        // ImGui::ColorEdit3("clear color", (float*)&plane_color);
        // ImGui::SliderFloat("reflectivity", &reflectivity, 0.0f, 1.0f);
        // ImGui::SliderFloat("refraction", &refraction, 0.0f, 1.0f);
        // ImGui::Checkbox("reflective", &reflective);


        // ImGui::SliderFloat("rotatation", &rot, -90.0f, 90.0f);
        // cmp::transform(plane).rotation() = glm::angleAxis(glm::radians(rot), glm::vec3(1,0,0));


        // ImGui::SliderFloat("tranlate", glm::value_ptr(cmp::transform(plane).position()), -90.0f, 90.0f);
        
        

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        // grph::TexturedMaterialBuilder::from_existing(static_cast<grph::TextureMaterial*>(cmp::mesh(plane).material()))
        //     .color(plane_color)
        //     .reflective(reflective)
        //     .reflectivity(reflectivity)
        //     .refraction(refraction);
        
    }
    
};




int main()
{
    std::unique_ptr<SimpleGame> game = std::make_unique<SimpleGame>();

    ay::app::Application app{1024, 768, game.get()};

    return app.run();
}

#include <iostream>
#include <memory>
#include <utility>
#include <cmath>

#include "ay.hpp"

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
        plane = main_scene->add(gmt::mesh_entity(
                                    {grph::plane_geometry(200, 200, 20, 20),
                                     grph::texture_material(tex)}));
        cmp::transform(plane).rotation() = glm::angleAxis(glm::radians(90.0f), glm::vec3(1,0,0));

        auto sky = rend::create_cubetexture_jpgs(app::ResouceLoader::path("textures/cube/sky/"));
        
        // auto cube_mat = grph::solid_color({ 0.0f, 0.0f, 1.0f });
        // grph::MaterialBuilder::from_existing(cube_mat.get())
        //     .wire_frame(true)
        //     // .enable_lending()
        //     // .alpha_blending()
        //     // .opacity(0.5)
        //     .transparent(false);



        // cube_2 = main_scene->add(grph::axis());
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

    bool event(app::Event& e) override {
        main_scene->event(e);
        return false;
    }

    void render(rend::RenderAPI&) override
    {
        renderer.render_scene(*main_scene);
    }


};




int main()
{
    std::unique_ptr<SimpleGame> game = std::make_unique<SimpleGame>();

    ay::app::Application app{1024, 768, game.get()};

    return app.run();
}

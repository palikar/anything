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
    gmt::Entity* plane;

    cmp::OrbitalCameraComponent* camera_controller;
    
  public:

    SimpleGame()
    {

    }

    void init() override
    {
        renderer.init(engine()->api());
        main_scene = init_scene("main");

        
        main_scene->camera().init_prescpective_projection(glm::radians(55.0f), 1024.0/768.0, 0.001, 1000.0);
        main_scene->camera().set_look_at(glm::vec3(10,10,10), glm::vec3(0.0f,0.0f,0.0f));

        camera_controller = main_scene->add_component<cmp::OrbitalCameraComponent>(&main_scene->camera());

        plane = main_scene->add(gmt::mesh_entity(
                                    {grph::plane_geometry(10, 10, 10, 10),
                                     grph::solid_color({ 1.0f, 0.0f, 0.0f })}));
        
        cmp::mesh(plane).material()->set_wire_frame(true);
        cmp::transform(plane).rotation() = glm::angleAxis(glm::radians(90.0f), glm::vec3(1,0,0));
        

        // cube_2 = main_scene->add(gmt::mesh_entity(
        //                              {grph::cube_geometry(2.0f, 2.0f, 2.0f),
        //                               grph::solid_color({ 0.0f, 0.0f, 1.0f })}));



        auto tex = rend::create_texture(app::ResouceLoader::get_instance()->get_file_path("textures/happy_boo.png"));
        
        cube_2 = main_scene->add(gmt::mesh_entity(
                                     {grph::cube_geometry(10,10,10, 100, 100, 100),
                                      grph::texture_material(tex)}));
        


        
        
        // cube_2 = main_scene->add(gmt::mesh_entity({
        //             grph::cylinder_geometry(1.0, 1.0, 4.0, 10.0f, 5.0f, true),
        //             grph::solid_color({ 0.0f, 1.0f, 0.0f })}));
        
        // cmp::mesh(cube_2).material()->set_wire_frame(true);
        
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

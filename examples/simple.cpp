#include <iostream>
#include <memory>
#include <utility>
#include <cmath>

#include "ay.hpp"

using namespace ay;


class SimpleGame : public ay::GameBase {

  private:
    Scene3D* main_scene;


    Entity* cube_1;
    Entity* plane;

    OrbitalCameraComponent* camera_controller;
    
  public:

    SimpleGame()
    {

    }

    void init() override
    {
        main_scene = init_scene("main");

        
        main_scene->camera().init_prescpective_projection(glm::radians(55.0f), 1024.0/768.0, 0.001, 1000.0);
        main_scene->camera().set_look_at(glm::vec3(10,10,10), glm::vec3(0.0f,0.0f,0.0f));

        camera_controller = main_scene->add_component<OrbitalCameraComponent>(&main_scene->camera());

        plane = main_scene->add(mesh_entity({plane_geometry(10, 10, 10, 10), solid_color({ 1.0f, 0.0f, 0.0f })}));
        mesh(plane).material()->set_wire_frame(true);
        transform(plane).rotation() = glm::angleAxis(glm::radians(90.0f), glm::vec3(1,0,0));

        cube_1 = main_scene->add(mesh_entity({sphere_geometry(2, 20, 20), solid_color({ 0.0f, 1.0f, 0.0f })}));

        add_children(cube_1, 
                     mesh_entity({sphere_geometry(1, 20, 20), solid_color({ 0.0f, 1.0f, 0.0f })}),
                     mesh_entity({sphere_geometry(1, 20, 20), solid_color({ 0.0f, 1.0f, 1.0f })}),
                     mesh_entity({sphere_geometry(1, 20, 20), solid_color({ 0.0f, 1.0f, 1.0f })}),
                     mesh_entity({sphere_geometry(1, 20, 20), solid_color({ 1.0f, 1.0f, 0.0f })}));

        
        transform(children(cube_1)[0].get()).translateX(-3.0f);
        transform(children(cube_1)[1].get()).translateX(3.0f);
        transform(children(cube_1)[2].get()).translateZ(3.0f);
        transform(children(cube_1)[3].get()).translateZ(-3.0f);
        

        
        
    }
    
    void update(double dt) override
    {
        main_scene->update(dt);
    }

    bool event(Event& e) override {
        main_scene->event(e);

        return false;
    }

    void render(ay::Renderer& render_api) override
    {        
        main_scene->render(render_api);   
    }


};




int main()
{
    std::unique_ptr<SimpleGame> game = std::make_unique<SimpleGame>();

    ay::Application app{1024, 768, game.get()};

    return app.run();
}

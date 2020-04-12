#include <iostream>
#include <memory>
#include <utility>
#include <cmath>

#include "ay.hpp"

using namespace ay;


class SimpleGame : public ay::GameBase {

  private:
    Scene3D* main_scene;
    double rot_speed = 1.0;

    Entity* cube_1;
    Entity* cube_2;
    Entity* plane;

    OrbitalCameraComponent* camera_controller;
    Clock* clock;

    
  public:

    SimpleGame()
    {

    }

    void init() override
    {
        main_scene = init_scene("main");

        plane = main_scene->add(mesh_entity(create_plane(10, 10, 10, 10)));

        get_transform(plane).rotation() = glm::angleAxis(glm::radians(90.0f), glm::vec3(1,0,0));

        cube_1 = main_scene->add(mesh_entity(create_cube()));

        main_scene->camera().init_prescpective_projection(glm::radians(55.0f), 1024.0/768.0, 0.001, 1000.0);
        main_scene->camera().set_look_at(glm::vec3(10,10,10), glm::vec3(0.0f,0.0f,0.0f));

        camera_controller = main_scene->add_component<OrbitalCameraComponent>(&main_scene->camera());

        clock = main_scene->add_component<Clock>();
        clock->add_callback(2.0, [](){
            std::cout << "Two second!" << "\n";
        });
        clock->start();

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
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        main_scene->render(render_api);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }


};




int main()
{
    std::unique_ptr<SimpleGame> game = std::make_unique<SimpleGame>();

    ay::Application app{1024, 768, game.get()};

    return app.run();
}

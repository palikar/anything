#include <iostream>
#include <memory>
#include <utility>

#include <cmath>

#include "ay.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace ay;


class SimpleGame : public ay::GameBase {

  private:
    Scene3D* main_scene;
    double rot_speed = 1.0;

    Entity* cube_1;
    Entity* cube_2;
    
  public:
    
    SimpleGame()
    {
        
    }

    void init() override
    {
        main_scene = init_scene("main");

        cube_1 = main_scene->add(mesh_entity(create_cube()));
        cube_2 = main_scene->add(mesh_entity(create_cube()));

        cube_1->component<TransformComponent>()->transform.set_position({2.0f, 0.0f, 0.0f});
        cube_2->component<TransformComponent>()->transform.set_position({-2.0f, 0.0f, 0.0f});
            
        main_scene->camera().init_prescpective_projection(glm::radians(75.0f), 1024.0/768.0, 0.001, 1000.0);
        main_scene->camera().set_look_at(glm::vec3(10, 10, 10), glm::vec3(0.0f,0.0f,0.0f));
    }
    
    void update(double dt) override
    {
        // const float radius = 10.0f;
        // float camX = sin(glfwGetTime()) * radius;
        // float camZ = cos(glfwGetTime()) * radius;

        // main_scene->camera().set_look_at(glm::vec3(camX, 10, camZ), glm::vec3(0.0f,0.0f,0.0f));
            
        cube_1->component<TransformComponent>()->transform.rotate(glm::vec3(0,1,0), rot_speed*dt);
        cube_2->component<TransformComponent>()->transform.rotate(glm::vec3(0,1,0), -1.0f * rot_speed*dt);

        main_scene->update(dt);
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

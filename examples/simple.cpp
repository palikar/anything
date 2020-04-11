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
    Entity* plane;
    
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
        // get_transform(cube_1).rotate(glm::vec3(0,1 ,0), glm::radians(90.0f));

        
        main_scene->camera().init_prescpective_projection(glm::radians(55.0f), 1024.0/768.0, 0.001, 1000.0);
        main_scene->camera().set_look_at(glm::vec3(10,10,10), glm::vec3(0.0f,0.0f,0.0f));

    }
    
    void update(double dt) override
    {
        // const float radius = 20.0f;
        // float camX = sin(glfwGetTime()) * radius;
        // float camZ = cos(glfwGetTime()) * radius;
        // main_scene->camera().set_look_at(glm::vec3(camX, 5, camZ), glm::vec3(0.0f,0.0f,0.0f));

            
        // get_transform(cube_1).rotate(glm::vec3(1,0,0), rot_speed*dt*0.1f);

        if(Input::is_pressed(KeyCode::A)) {
            get_transform(plane).rotateY(rot_speed*dt*0.3);
        }
        if(Input::is_pressed(KeyCode::D)) {
            get_transform(plane).rotateY(-rot_speed*dt*0.3);
        }


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

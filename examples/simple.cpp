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

    cmp::OrbitalCameraComponent *oribital_camera_controller;
    cmp::FloatingCameraComponent *floating_camera_controller;

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

        // oribital_camera_controller = main_scene->add_component<cmp::OrbitalCameraComponent>(&main_scene->camera());
        // oribital_camera_controller->set_max_radius(60.0f);

        floating_camera_controller = main_scene->add_component<cmp::FloatingCameraComponent>(&main_scene->camera(), glm::vec3(0,10,1));
        
    }

    void init() override
    {
        init_basic();

        auto tex = rend::create_texture(app::ResouceLoader::path("textures/floor/floor-albedo.png"));
        auto sky = rend::create_cubetexture_jpgs(app::ResouceLoader::path("textures/cube/sky/"));
        main_scene->set_skybox(gmt::skybox(sky));
        
        main_scene->add(gmt::axis());
        main_scene->add(gmt::grid_helper(40, 20, rend::Colors::black));

        



    }

    void update(double dt) override
    {
        main_scene->update(dt);

        main_scene->raycaster().camera_to_mouse().dir();
    }

    bool event(app::Event& e) override
    {
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

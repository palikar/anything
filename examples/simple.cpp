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

    static constexpr int ball_grid_x = 20;
    static constexpr int ball_grid_y = 20;

  private:
    gmt::Scene3D* main_scene;
    rend::RendererScene3D renderer;

    cmp::OrbitalCameraComponent *oribital_camera_controller;
    cmp::FloatingCameraComponent *floating_camera_controller;

    gmt::Entity* entities[ball_grid_x][ball_grid_y];

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

        init_orbital_camera();
    }

    void init_floating_camera()
    {
        main_scene->remove_component(oribital_camera_controller);
        oribital_camera_controller = nullptr;
        floating_camera_controller = main_scene->add_component<cmp::FloatingCameraComponent>(&main_scene->camera());
    }

    void init_orbital_camera()
    {
        main_scene->remove_component(floating_camera_controller);
        floating_camera_controller = nullptr;
        oribital_camera_controller = main_scene->add_component<cmp::OrbitalCameraComponent>(&main_scene->camera());
        oribital_camera_controller->set_max_radius(60.0f);
    }

    void init() override
    {
        init_basic();

        auto tex = rend::create_texture(app::ResouceLoader::path("textures/floor/floor-albedo.png"));
        auto sky = rend::create_cubetexture_jpgs(app::ResouceLoader::path("textures/cube/sky/"));
        main_scene->set_skybox(gmt::skybox(sky));
        
        main_scene->add(gmt::axis());
        main_scene->add(gmt::grid_helper(60, 20, rend::Colors::black));

        static constexpr float offset = 2.5f;
        static constexpr float radius = 0.5f;
        for (int i = 0; i < ball_grid_x; ++i)
        {
            for (int j = 0; j < ball_grid_y; ++j)
            {

                entities[i][j] = main_scene->add(
                    gmt::mesh_entity({ grph::sphere_geometry(radius, 10, 10),
                                       grph::solid_color((1.0f * i) / ball_grid_x,
                                                         (1.0f * j) / ball_grid_y,
                                                         0.4f) }));

                cmp::transform(entities[i][j]).position() =
                    glm::vec3(i * offset - (ball_grid_x * offset / 2),
                              1.5f,
                              j * 2.5f - (ball_grid_y * offset / 2));

                cmp::mesh(entities[i][j]).geometry().compute_bounding_box();
            }
        }


        main_scene->directional_light(glm::vec3(0.5, 0.5f, 0));
        



    }

    void update(double dt) override
    {
        main_scene->update(dt);

        main_scene->raycaster().camera_to_mouse().dir();
    }

    bool event(app::Event& e) override
    {
        main_scene->event(e);

        app::Dispatcher dispatch{ e };
        dispatch.dispatch<app::KeyReleasedEvent>([this](auto &event) {
            if (event.key_code() == KeyCode::F7)
            {
                
                if (oribital_camera_controller != nullptr)
                {
                    init_floating_camera();
                }
                else
                {
                    init_orbital_camera();
                }
            }

            return true;
        });

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

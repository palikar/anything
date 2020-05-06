#include "ay.hpp"

using namespace ay;

class AnimScene : public gmt::GameBase
{

  private:
    gmt::Scene3D* main_scene;
    rend::RendererScene3D renderer;
    cmp::OrbitalCameraComponent *oribital_camera_controller;


  public:

    AnimScene() = default;

    void init_basic()
    {
        renderer.init(engine()->api());
        main_scene = init_scene("main");
        main_scene->camera().init_prescpective_projection(glm::radians(65.0f), 1024.0/768.0, 0.001, 1000.0);

        main_scene->camera().set_look_at( glm::vec3(0.0f,0.0f,0.0f),
                                          glm::vec3(10,10, 10));

        oribital_camera_controller = main_scene->add_component<cmp::OrbitalCameraComponent>(&main_scene->camera());
        oribital_camera_controller->set_max_radius(30.0f);
        oribital_camera_controller->set_min_radius(0.10f);

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::Enable(true);
    }

    void init_lighting()
    {
        main_scene->directional_light(glm::vec3(0.5, 0.5f, 0), glm::vec3(0.8, 0.8f, 0.6f));
        main_scene->light_setup().directional_light.intensity = 0.1f;

        main_scene->ambient_light(glm::vec3(0.8, 0.8f, 0.6f));
        main_scene->light_setup().ambient_light.intensity = 0.15;

    }

    void init() override
    {
        init_basic();

        main_scene->add(gmt::axis());
        main_scene->add(gmt::grid_helper(50, 30));

        

        init_lighting();
    }

    void update(double dt) override
    {
        main_scene->update(dt);
    }

    bool event(app::Event& e) override
    {

        app::Dispatcher dispatch{ e };
        dispatch.dispatch<app::MouseButtonReleasedEvent>(MEMBER(mouse_press));
        dispatch.dispatch<app::KeyReleasedEvent>(MEMBER(key_press));
        dispatch.dispatch<app::WindowResizeEvent>(MEMBER(resizing));
        main_scene->event(e);
        return false;
    }

    bool key_press(app::KeyReleasedEvent& event)
    {

        if (event.key_code() == KeyCode::F5)
        {
            AY_INFO("Realoading shaders");
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

        renderer.render_scene(*main_scene);

    }

};



int main()
{
    auto game = std::make_unique<AnimScene>();
    ay::app::Application app{1024, 768, game.get()};
    return app.run();
}

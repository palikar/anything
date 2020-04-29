#include "ay.hpp"

using namespace ay;

class TerrainScene : public gmt::GameBase
{

  private:
    gmt::Scene3D* main_scene;
    rend::RendererScene3D renderer;
    cmp::OrbitalCameraComponent *oribital_camera_controller;
    
    gmt::Entity* floor_mesh;
    
    // gmt::PointlightHelper* pointlight;
    
  public:

    TerrainScene() = default;

    void init_basic()
    {
        renderer.init(engine()->api());
        main_scene = init_scene("main");
        main_scene->camera().init_prescpective_projection(glm::radians(65.0f), 1024.0/768.0, 0.001, 100000.0);
        main_scene->camera().set_look_at(glm::vec3(10,10, 10), glm::vec3(0.0f,0.0f,0.0f));
        oribital_camera_controller = main_scene->add_component<cmp::OrbitalCameraComponent>(&main_scene->camera());

        oribital_camera_controller->set_max_radius(200.0f);

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::Enable(true);
    }

    void init_lighting()
    {
        // main_scene->directional_light(glm::vec3(0.5, 0.5f, 0), glm::vec3(0.8, 0.8f, 0.6f));
        // main_scene->light_setup().directional_light.intensity = 0.1f;

        // main_scene->ambient_light(glm::vec3(0.8, 0.8f, 0.6f));
        // main_scene->light_setup().ambient_light.intensity = 0.08f;
        
        // main_scene->point_light(0);
        // main_scene->light_setup().point_lights[0].constant = 0.0;
        // main_scene->light_setup().point_lights[0].linear = 0.074;
        // main_scene->light_setup().point_lights[0].quadratic = 0.000;
        // main_scene->light_setup().point_lights[0].position = {5.0f, 10.0f, 5.0f};
        // main_scene->light_setup().point_lights[0].color = {0.8f,0.8f,0.8f};

        // pointlight = main_scene->add(gmt::pointlight_helper(5.0f));
        // cmp::transform(pointlight).set_position({5.0f, 10.0f, 5.0f});

    }

    void init() override
    {
        init_basic();

        auto sky = rend::create_cubetexture_jpgs(app::ResouceLoader::path("textures/cube/sky/"));
        auto night_sky = rend::create_cubetexture_jpgs(app::ResouceLoader::path("textures/cube/night_sky/"));

        auto floor = textures().load("floor/floor-albedo.png", gmt::MapType::ALBEDO_MAP);
        auto floor_normal = textures().load("floor/floor-normal.png", gmt::MapType::NORMAL_MAP);
        auto floor_bump = textures().load("floor/floor-height.png", gmt::MapType::HEIGHT_MAP);

        auto brick_ao =textures().load("bricks/brick_ao.jpg", gmt::MapType::AO_MAP);
        auto brick = textures().load("bricks/brick_base.jpg", gmt::MapType::DIFFUSE_MAP);
        auto brick_normal =textures().load("bricks/brick_normal.jpg", gmt::MapType::NORMAL_MAP);
        auto brick_bump = textures().load("bricks/brick_height.png", gmt::MapType::HEIGHT_MAP);

        auto rocks = textures().load("stones/rocks_color.jpg", gmt::MapType::DIFFUSE_MAP);
        auto rocks_ao = textures().load("stones/rocks_ao.jpg", gmt::MapType::AO_MAP);
        auto rocks_normal = textures().load("stones/rocks_normal.jpg", gmt::MapType::NORMAL_MAP);
        auto rocks_bump = textures().load("stones/rocks_displacement.jpg", gmt::MapType::HEIGHT_MAP);

        main_scene->add(gmt::axis());
        main_scene->set_skybox(gmt::skybox(sky));

        floor_mesh = main_scene->add(gmt::mesh_entity({grph::plane_geometry(500, 500, 50, 50), grph::texture_material(floor)}));
        cmp::transform(floor_mesh).rotateX(glm::radians(-90.0f));


        auto mod = load::Loader::load_model(app::ResouceLoader::obj("star-wars-x-wing.blend"));

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

        if (!e.handled) {
            main_scene->event(e);
        }

        return false;
    }

    bool key_press(app::KeyReleasedEvent& event)
    {

        if (event.key_code() == KeyCode::F5)
        {
            std::cout << "Realoading shaders" << "\n";
            this->shaders().reload_all();
        }

        if (event.key_code() == KeyCode::F10)
        {
            gl::take_screenshot("screen.png");
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
    auto game = std::make_unique<TerrainScene>();
    ay::app::Application app{1024, 768, game.get()};
    return app.run();
}

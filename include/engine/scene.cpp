#include "engine/components/transform.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/group.hpp"

#include "engine/scene.hpp"
#include "engine/game_base.hpp"

namespace ay {

void Scene3D::init(GameBase* t_game)
{
    m_game = t_game;

// m_base_shader = m_game->shaders().load("simple");
    // m_base_shader->bind();
    // m_base_shader->set("red_channel", 0.3f);
    // m_base_shader->set("projection_matrix", m_camera.view_projection());

    std::cout << "initing scene" << "\n";

}


Entity* Scene3D::add(EntityPtr t_entity)
{
    m_entities.push_back(std::move(t_entity));
    auto obj = m_entities.back().get();
    auto mesh = obj->component<MeshComponent>();
    if (mesh) {
        mesh->mesh.material()->init_shader(m_game->shaders());
    }

    return obj;
}

void Scene3D::render(Renderer& render_api)
{

    uint32_t current_shader = 0;

    for (auto& object : m_entities) {

        auto mesh_comp = object->component<MeshComponent>();
        auto trans_comp = object->component<TransformComponent>();

        if (mesh_comp && trans_comp ) {
            auto shader = mesh_comp->mesh.material()->shader();

            shader->bind();
            if (current_shader != shader->id()) {

                current_shader = shader->id();
            }

            shader->set("model_matrix", trans_comp->transform.get_tranformation());
            shader->set("projection_matrix", m_camera.view_projection());

            auto mat = mesh_comp->mesh.material();
            mat->update_uniforms();

            mesh_comp->mesh.geometry()->bind();

            EnableDisableWireframe wireframe_raii{render_api, mat->wire_frame()};
            render_api.draw_indexed(mesh_comp->mesh.geometry());

        }
        
    }
}

}

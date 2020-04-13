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
}


Entity* Scene3D::add(EntityPtr t_entity)
{
    t_entity->set_game(m_game);
    t_entity->init(m_game);
    m_entities.push_back(std::move(t_entity));

    // auto mesh = obj->component<MeshComponent>();    
    // if (mesh) {
    //     mesh->mesh.material()->init_shader(m_game->shaders());
    // }

    return m_entities.back().get();
}

void Scene3D::render(Renderer& render_api)
{

    for (auto& object : m_entities) {        
        render_entity(render_api, object.get());
    }
}

void Scene3D::render_entity(Renderer& render_api, Entity* object)
{
    
    auto mesh_comp = object->component<MeshComponent>();
    auto trans_comp = object->component<TransformComponent>();

    if (mesh_comp && trans_comp ) {

        auto shader = mesh_comp->mesh.material()->shader();

        if (current_shader != shader->id())
        {
            shader->bind();
            current_shader = shader->id();
        }

        if (!m_mat_stack.empty())
        {
            shader->set("model_matrix",
                        m_mat_stack.back() * trans_comp->transform.get_tranformation());
        } else {
            shader->set("model_matrix", trans_comp->transform.get_tranformation());
        }
        
        shader->set("projection_matrix", m_camera.view_projection());

        auto mat = mesh_comp->mesh.material();
        mat->update_uniforms();

        mesh_comp->mesh.geometry()->bind();

        EnableDisableWireframe wireframe_raii{render_api, mat->wire_frame()};
        render_api.draw_indexed(mesh_comp->mesh.geometry());

    }

    auto group_comp = object->component<GroupComponent>();

    if (group_comp) {
        auto& ch = children(object);

        if (m_mat_stack.empty()) {
            m_mat_stack.push_back(trans_comp->transform.get_tranformation());
        }

        for (auto& child : ch ) {
            render_entity(render_api, child.get());
        }
        

        m_mat_stack.pop_back();

            
    }

}


}

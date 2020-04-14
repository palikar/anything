
#include "rendering/renderer3D.hpp"


#include "engine/scene.hpp"
#include "engine/entity.hpp"

#include "rendering/renderer.hpp"

#include "engine/components/transform.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/group.hpp"


namespace ay::rend
{

void RendererScene3D::render_entity(gmt::Entity *object)
{
    auto mesh_comp  = object->component<cmp::MeshComponent>();
    auto trans_comp = object->component<cmp::TransformComponent>();

    if (mesh_comp && trans_comp)
    {

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
        }
        else
        {
            shader->set("model_matrix", trans_comp->transform.get_tranformation());
        }

        shader->set("projection_matrix", m_projection);

        auto mat = mesh_comp->mesh.material();
        mat->update_uniforms(m_binder);

        mesh_comp->mesh.geometry()->bind();

        EnableDisableWireframe wireframe_raii{ *m_api, mat->wire_frame() };
        m_api->draw_indexed(mesh_comp->mesh.geometry());
    }

    auto group_comp = object->component<cmp::GroupComponent>();

    if (group_comp)
    {
        auto &ch = cmp::children(object);

        if (m_mat_stack.empty())
        {
            m_mat_stack.push_back(trans_comp->transform.get_tranformation());
        }
        else
        {
            m_mat_stack.push_back(m_mat_stack.back()
                                  * trans_comp->transform.get_tranformation());
        }

        for (auto &child : ch)
        {
            render_entity(child.get());
        }

        m_mat_stack.pop_back();
    }
}

void RendererScene3D::init(RenderAPI *t_api)
{
    m_api = t_api;
}

void RendererScene3D::render_scene(gmt::Scene3D &scene)
{

    m_projection = scene.camera().view_projection();

    for (auto &object : scene.entities())
    {
        render_entity(object.get());
    }
}

}  // namespace ay::rend

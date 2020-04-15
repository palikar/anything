
#include "rendering/renderer3D.hpp"


#include "engine/scene.hpp"
#include "engine/entity.hpp"

#include "rendering/renderer.hpp"

#include "engine/components/transform.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/group.hpp"


namespace ay::rend
{

void RendererScene3D::init(RenderAPI *t_api)
{
    m_api = t_api;
}

void RendererScene3D::handle_mesh(gmt::Entity *object, cmp::MeshComponent *mesh_comp)
{
    auto trans_comp = object->component<cmp::TransformComponent>();
    
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

    glDepthFunc(static_cast<GLenum>(mat->depth_func()));
    if (mat->depth_test()) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    if (mat->depth_write()) {
        glDepthMask(GL_TRUE);  
    } else {
        glDepthMask(GL_FALSE);
    }
        
    if (mat->blending_setup().blending)
    {
        glEnable(GL_BLEND);

        glBlendEquation(static_cast<GLenum>(mat->blending_setup().blend_equation));
        glBlendFuncSeparate(static_cast<GLenum>(mat->blending_setup().blend_src),
                            static_cast<GLenum>(mat->blending_setup().blend_dst),
                            static_cast<GLenum>(mat->blending_setup().blend_src_alpha),
                            static_cast<GLenum>(mat->blending_setup().blend_dst_alpha));

        shader->set("opacity", mat->opacity());
        shader->set("alpha_threshold", mat->blending_setup().alpha_test);
        std::cout << mat->blending_setup().alpha_test << "\n";
    }
    shader->set("visible", mat->visible());
        


    mat->update_uniforms(m_binder);

    mesh_comp->mesh.buffers()->bind();
    EnableDisableWireframe wireframe_raii{ *m_api, mat->wire_frame() };
    m_api->draw_indexed(mesh_comp->mesh.buffers());

    if (mat->blending_setup().blending)
    {
        glDisable(GL_BLEND);
    }

}

void RendererScene3D::handle_group(gmt::Entity *object, cmp::GroupComponent *group_comp)
{
    auto trans_comp = object->component<cmp::TransformComponent>();
    
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

void RendererScene3D::render_entity(gmt::Entity *t_obj)
{

    dispatch_component<cmp::MeshComponent>(t_obj, HANDLER(handle_mesh));
    
    dispatch_component<cmp::GroupComponent>(t_obj, HANDLER(handle_group));

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

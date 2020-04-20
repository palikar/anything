#include "rendering/renderer3D.hpp"


#include "engine/scene.hpp"
#include "engine/entity.hpp"

#include "rendering/renderer.hpp"

#include "engine/entities/skybox.hpp"

#include "engine/components/transform.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/group.hpp"
#include "engine/components/line_segments.hpp"

namespace ay::rend
{

void RendererScene3D::init(RenderAPI *t_api)
{
    m_api = t_api;
}

void RendererScene3D::switch_shader(Shader *shader)
{

    if (current_shader != shader->id())
    {
        shader->bind();
        current_shader = shader->id();
    }
}

void RendererScene3D::switch_mvp(Shader *shader, glm::mat4 transform)
{

    shader->set("projection_matrix", m_view_projection);
    if (!m_mat_stack.empty())
    {
        shader->set("model_matrix", m_mat_stack.back() * transform);
    }
    else
    {
        shader->set("model_matrix", transform);
    }
}

void RendererScene3D::push_transform(glm::mat4 transform)
{

    if (m_mat_stack.empty())
    {
        m_mat_stack.push_back(transform);
    }
    else
    {
        m_mat_stack.push_back(m_mat_stack.back() * transform);
    }
}

void RendererScene3D::handle_material(grph::Material *material, Shader *shader)
{

    m_api->depth_func(material->depth_func());
    m_api->depth_test(material->depth_test());
    m_api->depth_write(material->depth_write());

    m_api->blending(material->blending_setup().blending);
    m_api->submit_blending(material->blending_setup());

    m_api->culling(material->side());


    shader->set("opacity", material->opacity());
    shader->set("alpha_threshold", material->blending_setup().alpha_test);
    shader->set("visible", material->visible());


    material->update_uniforms(m_binder, m_current_context);
}

void RendererScene3D::handle_mesh(gmt::Entity *object, cmp::MeshComponent *mesh_comp)
{
    auto transform = cmp::transform(object);
    auto shader    = mesh_comp->mesh.material()->shader();
    auto mat       = mesh_comp->mesh.material();

    switch_shader(shader);
    switch_mvp(shader, transform.get_tranformation());
    handle_material(mat, shader);

    EnableDisableWireframe wireframe_raii{ *m_api, mat->wire_frame() };


    m_api->draw_indexed(mesh_comp->mesh.buffers());
}

void RendererScene3D::handle_group(gmt::Entity *object, cmp::GroupComponent *)
{
    auto transform = cmp::transform(object);
    push_transform(transform.get_tranformation());

    auto &ch = cmp::children(object);
    for (auto &child : ch)
    {
        render_entity(child.get());
    }

    m_mat_stack.pop_back();
}

void RendererScene3D::handle_line_segments(gmt::Entity *object,
                                           cmp::LineSegmentsComponent *line)
{

    auto transform = cmp::transform(object);
    auto shader    = line->segments.material()->shader();
    auto mat       = line->segments.material();

    switch_shader(shader);
    switch_mvp(shader, transform.get_tranformation());
    handle_material(mat, shader);

    m_api->draw_lines(line->segments.buffers(), line->segments.count());
}

void RendererScene3D::handle_sky(gmt::Skybox *sky)
{
    auto shader  = sky->shader();
    auto tex     = sky->texture();
    auto buffers = sky->buffers();

    switch_shader(shader);
    auto slot = m_binder.resolve(tex);
    shader->set_sampler("skybox", slot);

    auto sky_view = m_view;
    sky_view[3]   = glm::vec4(0, 0, 0, 1);

    shader->set("projection_matrix", m_projection * sky_view);


    GLCall(glCullFace(GL_FRONT));
    m_api->draw_indexed(buffers);
    GLCall(glCullFace(GL_BACK));
}

void RendererScene3D::render_entity(gmt::Entity *t_obj)
{

    dispatch_component<cmp::MeshComponent, cmp::TransformComponent>(t_obj,
                                                                    HANDLER(handle_mesh));

    dispatch_component<cmp::LineSegmentsComponent, cmp::TransformComponent>(
      t_obj, HANDLER(handle_line_segments));

    dispatch_component<cmp::GroupComponent>(t_obj, HANDLER(handle_group));
}


void RendererScene3D::render_scene(gmt::Scene3D &scene)
{

    m_projection = scene.camera().projection();
    m_view       = scene.camera().view();

    m_view_projection            = m_projection * m_view;
    m_current_context.camera_pos = scene.camera().pos();

    handle_sky(scene.skybox());

    for (auto &object : scene.entities())
    {
        render_entity(object.get());
    }
}

}  // namespace ay::rend

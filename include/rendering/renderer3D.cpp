#include "rendering/renderer3D.hpp"

#include "engine/scene.hpp"
#include "engine/entity.hpp"

#include "rendering/renderer.hpp"

#include "engine/entities/skybox.hpp"

#include "engine/components/transform.hpp"
#include "engine/components/model.hpp"
#include "engine/components/mesh.hpp"
#include "engine/components/group.hpp"
#include "engine/components/line_segments.hpp"

#include <fmt/format.h>

namespace ay::rend
{

RendererScene3D::RendererScene3D() : m_uniform_binder(m_binder)
{
}

void RendererScene3D::init(RenderAPI *t_api)
{
    m_api = t_api;
}

void RendererScene3D::bind_lighting(Shader *shader)
{
    if (!m_current_context.light_setup->needs_update and lighting_updated[shader->id()])
    {
        return;
    }

    lighting_updated[shader->id()]              = true;
    m_current_context.light_setup->needs_update = false;

    shader->set("lighting.dir_light.act",
                m_current_context.light_setup->directional_light.active);
    shader->set("lighting.dir_light.direction",
                m_current_context.light_setup->directional_light.dir);
    shader->set("lighting.dir_light.color",
                m_current_context.light_setup->directional_light.color);
    shader->set("lighting.dir_light.intensity",
                m_current_context.light_setup->directional_light.intensity);


    shader->set("lighting.ambient_light.act",
                m_current_context.light_setup->ambient_light.active);
    shader->set("lighting.ambient_light.intensity",
                m_current_context.light_setup->ambient_light.intensity);
    shader->set("lighting.ambient_light.color",
                m_current_context.light_setup->ambient_light.color);

    for (size_t i = 0; i < grph::MAX_LIGHT; ++i)
    {
        shader->set(fmt::format("lighting.point_lights[{}].act", i),
                    m_current_context.light_setup->point_lights[i].active);
        shader->set(fmt::format("lighting.point_lights[{}].color", i),
                    m_current_context.light_setup->point_lights[i].color);
        shader->set(fmt::format("lighting.point_lights[{}].position", i),
                    m_current_context.light_setup->point_lights[i].position);
        shader->set(fmt::format("lighting.point_lights[{}].constant", i),
                    m_current_context.light_setup->point_lights[i].constant);
        shader->set(fmt::format("lighting.point_lights[{}].linear", i),
                    m_current_context.light_setup->point_lights[i].linear);
        shader->set(fmt::format("lighting.point_lights[{}].quadratic", i),
                    m_current_context.light_setup->point_lights[i].quadratic);
    }

    for (size_t i = 0; i < grph::MAX_LIGHT; ++i)
    {

        shader->set(fmt::format("lighting.spot_lights[{}].act", i),
                    m_current_context.light_setup->spot_lights[i].active);
        shader->set(fmt::format("lighting.spot_lights[{}].color", i),
                    m_current_context.light_setup->spot_lights[i].color);
        shader->set(fmt::format("lighting.spot_lights[{}].position", i),
                    m_current_context.light_setup->spot_lights[i].position);
        shader->set(fmt::format("lighting.spot_lights[{}].dir", i),
                    m_current_context.light_setup->spot_lights[i].dir);
        shader->set(fmt::format("lighting.spot_lights[{}].cut_off", i),
                    m_current_context.light_setup->spot_lights[i].cut_off);
        shader->set(fmt::format("lighting.spot_lights[{}].outer_cut_off", i),
                    m_current_context.light_setup->spot_lights[i].outer_cut_off);
    }
}

void RendererScene3D::switch_shader(Shader *shader)
{

    if (lighting_updated.count(shader->id()) == 0)
    {
        lighting_updated[shader->id()] = false;
    }

    if (current_shader != shader->id())
    {
        shader->bind();
        current_shader = shader->id();
        m_uniform_binder.change_shader(shader);
    }
}

void RendererScene3D::switch_mvp(Shader *shader, glm::mat4 transform)
{

    shader->set("projection_matrix", m_view_projection);
    shader->set("view_matrix", m_current_context.view);

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

    if (m_current_context.fog.activ())
    {
        shader->set("fog_color", m_current_context.fog.color());
        shader->set("fog_near", m_current_context.fog.near());
        shader->set("fog_far", m_current_context.fog.far());
        shader->set("fog_density", m_current_context.fog.density());
        shader->set("fog_type", static_cast<int>(m_current_context.fog.type()));
    }
    else
    {
        shader->set("fog_type", 0);
    }


    shader->set("opacity", material->opacity());
    shader->set("alpha_threshold", material->blending_setup().alpha_test);
    shader->set("visible", material->visible());


    material->update_uniforms(m_uniform_binder, m_binder, m_current_context);
}

void RendererScene3D::handle_model(gmt::Entity *object, cmp::ModelComponent *model_comp)
{

    auto &model    = model_comp->model;
    auto transform = cmp::transform(object);

    for (size_t i = 0; i < model.size(); ++i)
    {
        m_binder.begin_draw_call();

        auto mesh = model.get(i);

        auto mat    = mesh->material();
        auto shader = mesh->material()->shader();

        switch_shader(shader);
        switch_mvp(shader, transform.get_tranformation());
        handle_material(mat, shader);

        if (mat->needs_lighting())
        {
            shader->set("lighting_enabled", true);
            bind_lighting(shader);
        }
        else
        {
            shader->set("lighting_enabled", false);
        }

        EnableDisableWireframe wireframe_raii{ *m_api, mat->wire_frame() };
        m_api->draw_triangles(mesh->geometry());
    }
}

void RendererScene3D::handle_mesh(gmt::Entity *object, cmp::MeshComponent *mesh_comp)
{
    m_binder.begin_draw_call();

    auto transform = cmp::transform(object);
    auto shader    = mesh_comp->mesh.material()->shader();
    auto mat       = mesh_comp->mesh.material();


    switch_shader(shader);
    switch_mvp(shader, transform.get_tranformation());
    handle_material(mat, shader);

    if (mat->needs_lighting())
    {
        shader->set("lighting_enabled", true);
        bind_lighting(shader);
    }
    else
    {
        shader->set("lighting_enabled", false);
    }

    EnableDisableWireframe wireframe_raii{ *m_api, mat->wire_frame() };

    m_api->culling(Side::BOTH);
    m_api->draw_triangles(mesh_comp->mesh.geometry());
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
    m_binder.begin_draw_call();

    auto transform = cmp::transform(object);
    auto shader    = line->segments.material()->shader();
    auto mat       = line->segments.material();

    switch_shader(shader);
    switch_mvp(shader, transform.get_tranformation());
    handle_material(mat, shader);

    shader->set("lighting_enabled", false);

    m_api->draw_lines(line->segments.geometry());
}

void RendererScene3D::handle_sky(gmt::Skybox *sky)
{
    m_binder.begin_draw_call();

    auto shader = sky->shader();
    auto tex    = sky->texture();
    auto &geom  = sky->geometry();

    switch_shader(shader);
    const auto slot = m_binder.resolve(tex);
    shader->set_sampler("skybox", slot);

    if (m_current_context.fog.activ())
    {
        shader->set("fog_color", m_current_context.fog.color());
        shader->set("lower", m_current_context.fog.lower());
        shader->set("upper", m_current_context.fog.upper());
        shader->set("fog_type", 1);
    }
    else
    {
        shader->set("fog_type", 0);
    }


    auto sky_view = m_view;
    sky_view[3]   = glm::vec4(0, 0, 0, 1);

    shader->set("projection_matrix", m_projection * sky_view);

    m_api->culling(Side::FRONT);
    m_api->draw_triangles(geom);
    m_api->culling(Side::BACK);
}

void RendererScene3D::render_entity(gmt::Entity *t_obj)
{

    dispatch_component<cmp::MeshComponent, cmp::TransformComponent>(t_obj,
                                                                    HANDLER(handle_mesh));

    dispatch_component<cmp::LineSegmentsComponent, cmp::TransformComponent>(
      t_obj, HANDLER(handle_line_segments));

    dispatch_component<cmp::GroupComponent>(t_obj, HANDLER(handle_group));

    dispatch_component<cmp::ModelComponent, cmp::TransformComponent>(
      t_obj, HANDLER(handle_model));
}

void RendererScene3D::render_scene(gmt::Scene3D &scene)
{

    m_projection = scene.camera().projection();
    m_view       = scene.camera().view();

    m_view_projection = m_projection * m_view;

    m_current_context.proj        = m_projection;
    m_current_context.view        = m_view;
    m_current_context.camera_pos  = scene.camera().pos();
    m_current_context.light_setup = &scene.light_setup();
    m_current_context.fog         = scene.fog();

    if (m_current_context.light_setup->needs_update)
    {
        for (auto &[_, val] : lighting_updated)
        {
            val = false;
        }
    }

    auto *sky = scene.skybox();
    if (sky != nullptr)
    {
        handle_sky(sky);
    }

    for (auto &object : scene.entities())
    {
        render_entity(object.get());
    }
}

}  // namespace ay::rend

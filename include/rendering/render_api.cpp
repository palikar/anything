#include "rendering/render_api.hpp"

namespace ay::rend
{

RenderAPI::RenderAPI()
{
}

void RenderAPI::init()
{

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glCullFace(GL_BACK));
    GLCall(glFrontFace(GL_CW));
    // GLCall(glEnable(GL_FRAMEBUFFER_SRGB));
}

void RenderAPI::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    GLCall(glViewport(x, y, width, height));
}

void RenderAPI::set_clear_color(glm::vec4 color)
{
    GLCall(glClearColor(color.r, color.g, color.b, color.a));
}

void RenderAPI::clear()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RenderAPI::draw_mutli_indexed(const VertexArray *vertex_array, uint32_t index_count)
{
    vertex_array->bind();

    for (size_t i = 0; i < vertex_array->index_cnt(); ++i)
    {
        vertex_array->bind_index(i);
        uint32_t count =
          index_count <= 0 ? vertex_array->index_buffer(i)->count() : index_count;
        GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
    }
}

void RenderAPI::draw_lines(grph::Geometry &geometry, uint32_t element_count)
{

    if (geometry.is_dirty())
    {
        geometry.pack();
    }

    geometry.gl_buffers()->bind();

    if (geometry.is_indexed())
    {
        uint32_t count = element_count <= 0
                           ? geometry.gl_buffers()->index_buffer()->count()
                           : element_count;

        GLCall(glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, nullptr));
    }
    else
    {

        GLCall(glDrawArrays(GL_LINES, 0, geometry.attribute("position").size() / 2));
    }
}

void RenderAPI::draw_triangles(grph::Geometry &geometry, uint32_t element_count)
{

    if (geometry.is_dirty())
    {
        geometry.pack();
    }

    geometry.gl_buffers()->bind();

    if (geometry.is_indexed())
    {
        uint32_t count = element_count <= 0
                           ? geometry.gl_buffers()->index_buffer()->count()
                           : element_count;

        GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
    }
    else
    {
        GLCall(glDrawArrays(GL_TRIANGLES, 0, element_count));
    }
}

void RenderAPI::depth_test(bool value)
{
    if (value)
    {
        GLCall(glEnable(GL_DEPTH_TEST));
    }
    else
    {
        GLCall(glDisable(GL_DEPTH_TEST));
    }
}

void RenderAPI::depth_write(bool value)
{
    GLCall(glDepthMask(value ? GL_TRUE : GL_FALSE));
}

void RenderAPI::wireframe(bool value)
{
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, value ? GL_LINE : GL_FILL));
}

void RenderAPI::depth_func(DepthFunc func)
{
    GLCall(glDepthFunc(static_cast<GLenum>(func)));
}

void RenderAPI::enable_wireframe()
{
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
}

void RenderAPI::disable_wireframe()
{
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
}

void RenderAPI::blending(bool value)
{
    if (value)
    {
        GLCall(glEnable(GL_BLEND));
    }
    else
    {
        GLCall(glDisable(GL_BLEND));
    }
}

void RenderAPI::submit_blending(BlendingSetup &setup)
{
    GLCall(glBlendEquation(static_cast<GLenum>(setup.blend_equation)));
    GLCall(glBlendFuncSeparate(static_cast<GLenum>(setup.blend_src),
                               static_cast<GLenum>(setup.blend_dst),
                               static_cast<GLenum>(setup.blend_src_alpha),
                               static_cast<GLenum>(setup.blend_dst_alpha)));
}

void RenderAPI::culling(const Side &setup)
{
    switch (setup)
    {
    case Side::BACK:
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));
        break;
    case Side::FRONT:
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_BACK));
        break;

    case Side::BOTH: GLCall(glDisable(GL_CULL_FACE)); break;
    }
}


}  // namespace ay::rend

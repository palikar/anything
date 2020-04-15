#pragma once

#include "util/gl_helpers.hpp"

#include "rendering/buffer_factory.hpp"
#include "rendering/buffers.hpp"
#include "rendering/api.hpp"

#include "macros.hpp"
#include "glm_header.hpp"

namespace ay::rend
{


class RenderAPI
{
  private:
  public:
    RenderAPI()
    {
    }

    void init()
    {

        GLCall(glEnable(GL_DEPTH_TEST));

        GLCall(glEnable(GL_CULL_FACE));
        
        GLCall(glCullFace(GL_BACK));
        GLCall(glFrontFace(GL_CW));
    }

    void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        GLCall(glViewport(x, y, width, height));
    }

    void set_clear_color(glm::vec4 color)
    {
        GLCall(glClearColor(color.r, color.g, color.b, color.a));
    }

    void clear()
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void draw_mutli_indexed(const VertexArray *vertex_array, uint32_t index_count = 0)
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
    
    void draw_indexed(const VertexArray *vertex_array, uint32_t index_count = 0)
    {
        
        if (vertex_array->multi_indexed())
        {
            draw_mutli_indexed(vertex_array);
        }

        vertex_array->bind();
        uint32_t count =
            index_count <= 0 ? vertex_array->index_buffer()->count() : index_count;
        GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
        
    }

    void enable_wireframe()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void disable_wireframe()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
};

struct EnableDisableWireframe
{

    EnableDisableWireframe(RenderAPI &t_renderer, bool t_execute = true)
      : m_renderer(t_renderer), m_execute(t_execute)
    {
        if (t_execute)
        {
            t_renderer.enable_wireframe();
        }
    }

    ~EnableDisableWireframe()
    {
        if (m_execute)
        {
            m_renderer.disable_wireframe();
        }
    }

    AY_RAII_OBJECT(EnableDisableWireframe);

  private:
    RenderAPI &m_renderer;
    bool m_execute;
};

}  // namespace ay::rend

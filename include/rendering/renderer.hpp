#pragma once

#include "util/gl_helpers.hpp"

#include "rendering/buffer_factory.hpp"
#include "rendering/buffers.hpp"
#include "rendering/api.hpp"

#include "graphics/geometry.hpp"

#include "macros.hpp"
#include "glm_header.hpp"

namespace ay::rend
{


class RenderAPI
{
  public:
    RenderAPI();

    void init();

    void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    void set_clear_color(glm::vec4 color);

    void clear();

    void draw_mutli_indexed(const VertexArray *vertex_array, uint32_t index_count = 0);

    void draw_lines(grph::Geometry& geometry, uint32_t element_count = 0);

    void draw_triangles(grph::Geometry& geometry, uint32_t element_count = 0);

    void depth_test(bool value);

    void depth_write(bool value);

    void wireframe(bool value);

    void depth_func(DepthFunc func);

    void enable_wireframe();

    void disable_wireframe();

    void blending(bool value);

    void submit_blending(BlendingSetup &setup);

    void culling(const Side &setup);
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

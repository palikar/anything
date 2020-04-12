#pragma once

#include "rendering/buffers.hpp"
#include "util/gl_helpers.hpp"

#include "commons.hpp"
#include "glm_header.hpp"

#include <GL/glew.h>


namespace ay
{



class Renderer
{
  private:


  public:

    Renderer() {}

    void init(){

        GLCall(glEnable(GL_DEPTH_TEST));
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
        
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

    void draw_indexed(const VertexArray* vertex_array, uint32_t index_count = 0)
    {
        
        vertex_array->bind();
        uint32_t count = index_count <= 0 ? vertex_array->index_buffer()->count() : index_count;
		GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
    }


};

}

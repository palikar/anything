#pragma once

#include <GL/glew.h>

#include <vector>

#include "rendering/buffers.hpp"


namespace ay
{

class VertexArray
{
  public:
    VertexArray()
    {
        glCreateVertexArrays(1, &m_vao);
    }

    ~VertexArray()
    {
        glDeleteVertexArrays(1, &m_vao);
    }

    void bind() const
    {        
        glBindVertexArray(m_vao);
    }

    void unbind() const
    {
        glBindVertexArray(0);
    }

    void add_vertex_buffer(const VertexBufferPtr& vertex_buffer)
    {
        glBindVertexArray(m_vao);
		vertex_buffer->bind();
        
        const auto& layout = vertex_buffer->get_layout();
        for (const auto& element : layout.elements()) {
            glEnableVertexAttribArray(m_index);            
            glVertexAttribPointer(m_index,
                                  data_type_element_count(element.type),
                                  data_type_gl_base_type(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE,
                                  layout.get_stride(),
                                  (const void*)element.offset);
            ++m_index;
        }
        m_vertex_buffers.push_back(vertex_buffer);
    }
    
    void set_index_buffer(const IndexBufferPtr& index_buffer)
    {
        glBindVertexArray(m_vao);
        index_buffer->bind();
        m_index_buffer = index_buffer;
        
    }

    const std::vector<VertexBufferPtr>& GetVertexBuffers() const { return m_vertex_buffers; }
    const IndexBufferPtr& GetIndexBuffer() const { return m_index_buffer; }
  private:
    
    uint32_t m_vao;
    uint32_t m_index = 0;
    
    std::vector<VertexBufferPtr> m_vertex_buffers;
    IndexBufferPtr m_index_buffer;
};

}

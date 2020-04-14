
#include "rendering/vertex_array.hpp"

#include "util/gl_helpers.hpp"

namespace ay::rend
{

void VertexArray::bind() const
{
    GLCall(glBindVertexArray(m_vao));
}

void VertexArray::unbind() const
{
    GLCall(glBindVertexArray(0));
}


VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &m_vao);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_vao);
}


void VertexArray::add_vertex_buffer(VertexBufferPtr vertex_buffer)
{
    GLCall(glBindVertexArray(m_vao));
    vertex_buffer->bind();

    const auto &layout = vertex_buffer->get_layout();
    for (const auto &element : layout.elements())
    {
        // std::cout << "index: " << m_index << "\n";
        // std::cout << "elemnt_coutn: " << data_type_element_count(element.type) << "\n";
        // std::cout << "stride: " << layout.get_stride() << "\n";
        // std::cout << "offset: " << element.offset << "\n";
        // std::cout << "--------------" << "\n";
        
        GLCall(glEnableVertexAttribArray(m_index));
        GLCall(glVertexAttribPointer(m_index,
                                     data_type_element_count(element.type),
                                     data_type_gl_base_type(element.type),
                                     element.normalized ? GL_TRUE : GL_FALSE,
                                     layout.get_stride(),
                                     (const void *)element.offset));
        ++m_index;
    }
    m_vertex_buffers.push_back(std::move(vertex_buffer));
}

void VertexArray::set_index_buffer(IndexBufferPtr index_buffer)
{
    glBindVertexArray(m_vao);
    index_buffer->bind();
    m_index_buffer = std::move(index_buffer);
    glBindVertexArray(0);
}

}  // namespace ay::rend

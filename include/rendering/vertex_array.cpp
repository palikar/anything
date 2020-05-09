
#include "rendering/vertex_array.hpp"

#include "util/gl_helpers.hpp"

#include "glm_header.hpp"

namespace ay::rend
{

void VertexArray::bind() const
{
    GLCall(glBindVertexArray(m_vao));
}

void VertexArray::bind_index(size_t index) const
{
    GLCall(glBindVertexArray(m_vao));
    m_index_buffers.at(index)->bind();
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


VertexBuffer *VertexArray::add_vertex_buffer(VertexBufferPtr vertex_buffer)
{

    GLCall(glBindVertexArray(m_vao));
    vertex_buffer->bind();

    const auto &layout = vertex_buffer->get_layout();
    for (const auto &element : layout.elements())
    {

        if (element.type == ShaderDataType::Mat4)
        {
            GLCall(glEnableVertexAttribArray(m_index + 0));
            GLCall(glVertexAttribPointer(m_index + 0,
                                         4,
                                         GL_FLOAT,
                                         element.normalized ? GL_TRUE : GL_FALSE,
                                         data_type_size(ShaderDataType::Mat4),
                                         (const void *)0));

            GLCall(glEnableVertexAttribArray(m_index + 1));
            GLCall(glVertexAttribPointer(m_index + 1,
                                         4,
                                         GL_FLOAT,
                                         element.normalized ? GL_TRUE : GL_FALSE,
                                         data_type_size(ShaderDataType::Mat4),
                                         (const void *)(static_cast<long>(
                                           1 * data_type_size(ShaderDataType::Float4)))));

            GLCall(glEnableVertexAttribArray(m_index + 2));
            GLCall(glVertexAttribPointer(m_index + 2,
                                         4,
                                         GL_FLOAT,
                                         element.normalized ? GL_TRUE : GL_FALSE,
                                         data_type_size(ShaderDataType::Mat4),
                                         (const void *)(static_cast<long>(
                                           2 * data_type_size(ShaderDataType::Float4)))));

            GLCall(glEnableVertexAttribArray(m_index + 3));
            GLCall(glVertexAttribPointer(m_index + 3,
                                         4,
                                         GL_FLOAT,
                                         element.normalized ? GL_TRUE : GL_FALSE,
                                         data_type_size(ShaderDataType::Mat4),
                                         (const void *)(static_cast<long>(
                                           3 * data_type_size(ShaderDataType::Float4)))));


            if (element.per_instance)
            {
                GLCall(glVertexAttribDivisor(m_index + 0, 1));
                GLCall(glVertexAttribDivisor(m_index + 1, 1));
                GLCall(glVertexAttribDivisor(m_index + 2, 1));
                GLCall(glVertexAttribDivisor(m_index + 3, 1));
            }


            m_index += 4;
            continue;
        }

        // std::cout << "index " << m_index << "\n";
        // std::cout << "cont " << data_type_element_count(element.type) << "\n";
        // std::cout << "stride " << layout.get_stride() << "\n";
        // std::cout << "offset " << element.offset << "\n";

        GLCall(glEnableVertexAttribArray(m_index));
        GLCall(glVertexAttribPointer(m_index,
                                     data_type_element_count(element.type),
                                     data_type_gl_base_type(element.type),
                                     element.normalized ? GL_TRUE : GL_FALSE,
                                     layout.get_stride(),
                                     (const void *)element.offset));

        if (element.per_instance)
        {
            std::cout << "per isntance"
                      << "\n";
            glVertexAttribDivisor(m_index, 1);
        }

        ++m_index;
    }

    m_vertex_buffers.push_back(std::move(vertex_buffer));
    return m_vertex_buffers.back().get();
}

void VertexArray::set_index_buffer(IndexBufferPtr index_buffer, size_t index)
{
    glBindVertexArray(m_vao);
    index_buffer->bind();
    m_index_buffers.insert(std::begin(m_index_buffers) + index, std::move(index_buffer));
    glBindVertexArray(0);
}

}  // namespace ay::rend

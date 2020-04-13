#pragma once


#include "rendering/buffers.hpp"

#include "std_header.hpp"

namespace ay::rend
{

class VertexArray
{
  public:
    VertexArray();

    ~VertexArray();

    void bind() const;

    void unbind() const;

    void add_vertex_buffer(VertexBufferPtr vertex_buffer);

    void set_index_buffer(IndexBufferPtr index_buffer);

    const std::vector<VertexBufferPtr> &vertex_buffers() const
    {
        return m_vertex_buffers;
    }

    const IndexBufferPtr &index_buffer() const
    {
        return m_index_buffer;
    }

  private:
    uint32_t m_vao;
    uint32_t m_index = 0;

    std::vector<VertexBufferPtr> m_vertex_buffers;
    IndexBufferPtr m_index_buffer;
};


using VertexArrayPtr = std::unique_ptr<VertexArray>;

}  // namespace ay::rend

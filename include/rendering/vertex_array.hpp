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

    void bind_index(size_t index = 0) const;

    void unbind() const;

    void add_vertex_buffer(VertexBufferPtr vertex_buffer);

    void set_index_buffer(IndexBufferPtr index_buffer, size_t index = 0);

    bool multi_indexed() const
    {
        return m_index_buffers.size() > 1;
    }

    size_t index_cnt() const
    {
        return m_index_buffers.size();
    }

    const std::vector<VertexBufferPtr> &vertex_buffers() const
    {
        return m_vertex_buffers;
    }

    const IndexBufferPtr &index_buffer(size_t index = 0) const
    {
        return m_index_buffers.at(index);
    }

  private:
    uint32_t m_vao;
    uint32_t m_index = 0;

    std::vector<VertexBufferPtr> m_vertex_buffers;
    std::vector<IndexBufferPtr> m_index_buffers;
};


using VertexArrayPtr = std::unique_ptr<VertexArray>;

}  // namespace ay::rend

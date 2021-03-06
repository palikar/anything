#pragma once

#include "rendering/vertex.hpp"
#include "rendering/vertex_array.hpp"
#include "commons.hpp"

#include "std_header.hpp"
#include <string>

namespace ay::rend
{

enum class VetexType
{
    POS
};


template<size_t count, typename... T, ShaderDataType... T_shader>
inline VertexBufferPtr make_buffer(
  std::vector<Vertex3fGen<count, Types<T...>, ShaderTypes<T_shader...>>> vertecies)
{
    auto buf = std::make_unique<VertexBuffer>(
      vertecies.front().data(),
      vertecies.size()
        * Vertex3fGen<count, Types<T...>, ShaderTypes<T_shader...>>::element_size);

    buf->set_layout(BufferLayout({ (BufferElement{ "position", T_shader })... }));

    return buf;
}


inline VertexBufferPtr make_vertex_buffer(uint32_t size)
{
    return std::make_unique<VertexBuffer>(size);
}

inline VertexBufferPtr make_vertex_buffer(std::vector<Vertex3f> vertecies)
{
    auto buf = std::make_unique<VertexBuffer>(vertecies.front().data(),
                                              vertecies.size() * Vertex3f::element_size);


    buf->set_layout({ { "position", ShaderDataType::Float3 } });

    return buf;
}

template<typename... Vertex>
inline VertexBufferPtr make_vertex_buffer(Vertex... vertecies)
{
    Vertex3f temp[] = { (vertecies, ...) };

    auto buf = std::make_unique<VertexBuffer>(
      temp[0].data(), sizeof...(vertecies) * Vertex3f::element_size);
    buf->set_layout({ { "position", ShaderDataType::Float3 } });
    return buf;
}

template<int N>
inline VertexBufferPtr make_vertex_buffer(Vertex3f vertecies[N])
{
    auto buf =
      std::make_unique<VertexBuffer>(vertecies[0].data(), N * Vertex3f::element_size);
    buf->set_layout({ { "position", ShaderDataType::Float3 } });
    return buf;
}

template<int N>
inline VertexBufferPtr make_vertex_buffer(VetexType type, float vertecies[N])
{
    if (type == VetexType::POS)
    {
        auto buf =
          std::make_unique<VertexBuffer>(vertecies[0], N * Vertex3f::element_size);
        buf->set_layout({ { "position", ShaderDataType::Float3 } });
        return buf;
    }
    return nullptr;
}

inline IndexBufferPtr make_index_buffer(std::vector<uint32_t> indices)
{
    auto index = std::make_unique<IndexBuffer>(indices.data(), std::size(indices));
    return index;
}

inline IndexBufferPtr make_index_buffer(std::vector<Index3i> indices)
{
    auto index = std::make_unique<IndexBuffer>(&indices[0].i_0, 3 * std::size(indices));
    return index;
}

template<int N>
inline IndexBufferPtr make_index_buffer(uint32_t indices[N])
{
    auto index = std::make_unique<IndexBuffer>(&indices[0], N);
    return index;
}

template<int N>
inline IndexBufferPtr make_index_buffer(Index3i indices[N])
{
    auto index = std::make_unique<IndexBuffer>(&indices[0].i_0, 3 * N);
    return index;
}

inline VertexArrayPtr make_vertex_array()
{
    return std::make_unique<VertexArray>();
}

inline VertexArrayPtr make_vertex_array(IndexBufferPtr index,
                                        std::vector<VertexBufferPtr> vertex_bufffers)
{
    auto ind = std::make_unique<VertexArray>();
    ind->set_index_buffer(std::move(index));
    for (auto &ver : vertex_bufffers)
    {
        ind->add_vertex_buffer(std::move(ver));
    }
    return ind;
}

template<typename... Buffer>
inline VertexArrayPtr make_vertex_array(IndexBufferPtr index, Buffer... vertex_bufffers)
{
    auto ind = std::make_unique<VertexArray>();
    ind->set_index_buffer(std::move(index));

    (ind->add_vertex_buffer(std::move(vertex_bufffers)), ...);

    return ind;
}


}  // namespace ay::rend

#pragma once


#include <vector>
#include <memory>

#include "rendering/vertex.hpp"
#include "rendering/vertex_array.hpp"
#include "commons.hpp"


namespace ay
{

enum class VetexType {
    POS
};


VertexBufferPtr make_vertex_buffer(uint32_t size){
    return std::make_unique<VertexBuffer>(size);
}


VertexBufferPtr make_vertex_buffer(std::vector<Vertex3f> vertecies) {
    auto buf = std::make_unique<VertexBuffer>(vertecies.front().data(), vertecies.size() * Vertex3f::element_size);
    buf->set_layout({{"position", ay::ShaderDataType::Float3}});
    return buf;
}

template<typename ... Vertex>
VertexBufferPtr make_vertex_buffer(Vertex ... vertecies) {
    Vertex3f temp[] = {(vertecies, ...)};
    
    auto buf = std::make_unique<VertexBuffer>(temp[0].data(), sizeof...(vertecies) * Vertex3f::element_size);
    buf->set_layout({{"position", ay::ShaderDataType::Float3}});
    return buf;
}

template<int N>
VertexBufferPtr make_vertex_buffer(Vertex3f vertecies[N]) {
    auto buf = std::make_unique<VertexBuffer>(vertecies[0].data(), N * Vertex3f::element_size);
    buf->set_layout({{"position", ay::ShaderDataType::Float3}});
    return buf;
}

template<int N>
VertexBufferPtr make_vertex_buffer(VetexType type, float vertecies[N]) {
    if (type == VetexType::POS) {
        auto buf = std::make_unique<VertexBuffer>(vertecies[0], N * Vertex3f::element_size);
        buf->set_layout({{"position", ay::ShaderDataType::Float3}});
        return buf;
    }
    return nullptr;
    
}

IndexBufferPtr make_index_buffer(std::vector<uint32_t> indices) {
    auto index = std::make_unique<IndexBuffer>(indices.data(), std::size(indices));
    return index;
}

IndexBufferPtr make_index_buffer(std::vector<Index3i> indices) {
    auto index = std::make_unique<IndexBuffer>(&indices[0].i_0, 3*std::size(indices));
    return index;
}

template<int N>
IndexBufferPtr make_index_buffer(uint32_t indices[N]) {
    auto index = std::make_unique<IndexBuffer>(&indices[0], N);
    return index;
}

template<int N>
IndexBufferPtr make_index_buffer(Index3i indices[N]) {
    auto index = std::make_unique<IndexBuffer>(&indices[0].i_0, 3*N);
    return index;
}

VertexArrayPtr make_vertex_array(){
    return std::make_unique<ay::VertexArray>();
}

VertexArrayPtr make_vertex_array(IndexBufferPtr index, std::vector<VertexBufferPtr> vertex_bufffers){
    auto ind = std::make_unique<ay::VertexArray>();
    ind->set_index_buffer(std::move(index));
    for (auto& ver : vertex_bufffers) {
        ind->add_vertex_buffer(std::move(ver));
    }
    return ind;    
}

template<typename ... Buffer>
VertexArrayPtr make_vertex_array(IndexBufferPtr index,  Buffer ... vertex_bufffers){
    auto ind = std::make_unique<ay::VertexArray>();
    ind->set_index_buffer(std::move(index));

    (ind->add_vertex_buffer(std::move( vertex_bufffers)), ...);
    
    return ind;    
}


}

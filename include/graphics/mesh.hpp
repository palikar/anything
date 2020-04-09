#pragma once


#include <vector>
#include <memory>
#include <utility>


#include "rendering/vertex.hpp"
#include "rendering/vertex_array.hpp"
#include "commons.hpp"


namespace ay
{

class Mesh
{
  private:
    VertexArrayPtr m_geometry;

  public:


    Mesh(VertexArrayPtr t_geometry) : m_geometry(std::move(t_geometry))
    {}

    const VertexArray* geometry() const { return m_geometry.get(); }
    
};










}

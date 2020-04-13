#pragma once

#include "rendering/vertex.hpp"
#include "rendering/vertex_array.hpp"

#include "graphics/material.hpp"

#include "std_header.hpp"

namespace ay
{

class Mesh
{
  private:
    VertexArrayPtr m_geometry;
    MaterialPtr m_material;

  public:

    Mesh() : m_geometry()
    {}

    Mesh(VertexArrayPtr t_geometry, MaterialPtr t_material) :
        m_geometry(std::move(t_geometry)), m_material(std::move(t_material))
    {}

    void set_geometry(VertexArrayPtr t_geometry)
    {
        m_geometry = std::move(t_geometry);
    }
    
    const VertexArray* geometry() const
    {
        return m_geometry.get();
    }

    Material* material() const
    {
        return m_material.get();
    }
    
};










}

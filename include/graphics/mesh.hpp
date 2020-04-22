#pragma once

#include "rendering/vertex.hpp"
#include "rendering/vertex_array.hpp"

#include "graphics/material.hpp"
#include "graphics/geometry.hpp"

#include "std_header.hpp"

namespace ay::grph
{

class Mesh
{
  private:
    Geometry m_geometry;
    MaterialPtr m_material;

  public:
    Mesh() : m_geometry()
    {
    }

    Mesh(Geometry t_geometry, MaterialPtr t_material)
      : m_geometry(std::move(t_geometry)), m_material(std::move(t_material))
    {
    }

    void set_geometry(Geometry t_geometry)
    {
        m_geometry = std::move(t_geometry);
    }

    Geometry &geometry()
    {
        return m_geometry;
    }

    const rend::VertexArray *buffers()
    {
        if (m_geometry.is_dirty())
        {
            m_geometry.pack();
        }
        return m_geometry.gl_buffers();
    }

    template<typename T>
    T *material() const
    {
        return static_cast<T *>(m_material.get());
    }

    Material *material() const
    {
        return m_material.get();
    }
};


}  // namespace ay::grph

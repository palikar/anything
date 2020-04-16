#pragma once

#include "graphics/geometry.hpp"
#include "graphics/material.hpp"


#include "glm_header.hpp"
#include "std_header.hpp"

namespace ay::grph
{


class LineSegments
{
  private:

    MaterialPtr m_material;
    Geometry m_geometry;
    uint32_t m_count{0};
    
  public:
    LineSegments(MaterialPtr t_mat) : m_material(std::move(t_mat)), m_geometry()
    {
        m_geometry.set_attribute("position", {}, 3);
        
    }


    LineSegments(std::vector<glm::vec3> points, MaterialPtr t_mat) : m_material(std::move(t_mat)), m_geometry()
    {
        std::vector<float> pts;
        pts.reserve(points.size());
        for (auto& p : points)
        {
            pts.emplace_back(p.x);
            pts.emplace_back(p.y);
            pts.emplace_back(p.z);
        }

        m_geometry.set_attribute("position", (pts), 3);
    }

    void add_point(glm::vec3 point)
    {
        auto& pos = m_geometry.attribute("position");
        pos.push_back(point.x);
        pos.push_back(point.y);
        pos.push_back(point.z);
        ++m_count;
    }

    Geometry& geometry()
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
    
    uint32_t count() const
    {
        return m_count;
    }

    Material *material() const
    {
        return m_material.get();
    }
    
};

}

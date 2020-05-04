#pragma once

#include "rendering/vertex.hpp"
#include "rendering/vertex_array.hpp"

#include "graphics/material.hpp"
#include "graphics/geometry.hpp"

#include "std_header.hpp"
#include "glm_header.hpp"

namespace ay::grph
{

class InstancedMesh
{
  private:

    Geometry m_geometry;
    MaterialPtr m_material;
    size_t m_max_num;
    std::vector<glm::mat4> m_transforms;

  public:
    InstancedMesh(Geometry t_geometry, MaterialPtr t_material, size_t t_max_num)
        : m_geometry(std::move(t_geometry))
        , m_material(std::move(t_material))
        , m_max_num(t_max_num)
    {
        m_transforms.resize(m_max_num);

        auto instance_buf = std::make_unique<rend::VertexBuffer>(
          m_transforms.size() * rend::data_type_size(rend::ShaderDataType::Mat4));

        instance_buf->set_layout(rend::BufferLayout(
          { rend::BufferElement{ "transform", rend::ShaderDataType::Mat4, true } }));

        m_geometry.gl_buffers()->add_vertex_buffer(std::move(instance_buf));
        
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

    glm::mat4& operator[](int index)
    {
        return m_transforms[index];
    }

    void set_position(int index, glm::vec3 pos)
    {
        m_transforms[index] = glm::translate(m_transforms[index], pos);
    }
    
    void set_scale(int index, glm::vec3 scale)
    {
        m_transforms[index] = glm::scale(m_transforms[index], scale);
    }

    void set_rotation(int index, glm::vec3 axis, float amt)
    {
        m_transforms[index] = glm::rotate(m_transforms[index], amt, axis);
    }

    size_t count() const
    {
        return m_max_num;
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

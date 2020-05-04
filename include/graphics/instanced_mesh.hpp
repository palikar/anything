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
    const size_t m_max_num;
    std::vector<glm::mat4> m_transforms;
    rend::VertexBuffer *m_transforms_buffer;


  public:
    InstancedMesh(Geometry t_geometry, MaterialPtr t_material, size_t t_max_num);

    void set_geometry(Geometry t_geometry);

    Geometry &geometry();

    inline size_t count() const
    {
        return m_max_num;
    }

    inline Material *material() const
    {
        return m_material.get();
    }

    const rend::VertexArray *buffers();
    
    glm::mat4 &operator[](int index);    

    void set_position(int index, glm::vec3 pos);

    void set_scale(int index, glm::vec3 scale);

    void set_rotation(int index, glm::vec3 axis, float amt);

    void upate();

    template<typename T>
    T *material() const
    {
        return static_cast<T *>(m_material.get());
    }
};


}  // namespace ay::grph

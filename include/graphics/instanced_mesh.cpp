#include "instanced_mesh.hpp"



namespace ay::grph
{

InstancedMesh::InstancedMesh(Geometry t_geometry, MaterialPtr t_material, size_t t_max_num)
    : m_geometry(std::move(t_geometry))
    , m_material(std::move(t_material))
    , m_max_num(t_max_num)
{
    m_transforms.resize(m_max_num);

    auto instance_buf = std::make_unique<rend::VertexBuffer>(
        m_transforms.size() * rend::data_type_size(rend::ShaderDataType::Mat4));

    instance_buf->set_layout(rend::BufferLayout(
                                 { rend::BufferElement{ "transform", rend::ShaderDataType::Mat4, true } }));

    m_transforms_buffer =
        m_geometry.gl_buffers()->add_vertex_buffer(std::move(instance_buf));

    upate();
}

void InstancedMesh::set_geometry(Geometry t_geometry)
{
    m_geometry = std::move(t_geometry);
}

Geometry &InstancedMesh::geometry()
{
    return m_geometry;
}

const rend::VertexArray *InstancedMesh::buffers()
{
    if (m_geometry.is_dirty())
    {
        m_geometry.pack();
    }
    return m_geometry.gl_buffers();
}

glm::mat4 &InstancedMesh::operator[](int index)
{
    return m_transforms[index];
}

void InstancedMesh::set_position(int index, glm::vec3 pos)
{
    m_transforms[index] = glm::translate(m_transforms[index], pos);
    upate();
}

void InstancedMesh::set_scale(int index, glm::vec3 scale)
{
    m_transforms[index] = glm::scale(m_transforms[index], scale);
    upate();
}

void InstancedMesh::set_rotation(int index, glm::vec3 axis, float amt)
{
    m_transforms[index] = glm::rotate(m_transforms[index], amt, axis);
    upate();
}

void InstancedMesh::upate()
{
    m_transforms_buffer->set_data(&m_transforms[0][0][0], m_max_num * 16);
}


}

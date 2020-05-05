#include "instanced_mesh.hpp"


namespace ay::grph
{

InstancedMesh::InstancedMesh(Geometry t_geometry, MaterialPtr t_material, size_t t_max_num)
    : m_geometry(std::move(t_geometry))
    , m_material(std::move(t_material))
    , m_max_num(t_max_num)
{
    m_transforms.resize(m_max_num);
    m_matirces.resize(m_max_num);

    m_geometry.pack();

    auto instance_buf = std::make_unique<rend::VertexBuffer>(sizeof(glm::mat4));

    instance_buf->set_layout(rend::BufferLayout(
                                 { rend::BufferElement{ "transform", rend::ShaderDataType::Mat4, true } }));
    
    m_transforms_buffer =
        m_geometry.gl_buffers()->add_vertex_buffer(std::move(instance_buf));

    update();
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

mth::Transform &InstancedMesh::operator[](int index)
{
    return m_transforms[index];
}

void InstancedMesh::set_position(int index, glm::vec3 pos)
{
    m_transforms[index].set_position(pos);
    update();
}

void InstancedMesh::set_scale(int index, glm::vec3 scale)
{
    m_transforms[index].set_scale(scale);
    update();
}

void InstancedMesh::set_rotation(int index, glm::vec3 rot)
{
    m_transforms[index].set_rotation(rot);
    update();
}

void InstancedMesh::update()
{
    
    for (size_t i = 0; i < m_max_num; ++i)
    {
        m_transforms[i].update();
        m_matirces[i] = m_transforms[i].get_tranformation();
    }

    m_transforms_buffer->set_data(&m_matirces[0][0][0], m_max_num * 16 * sizeof(float));
}


}

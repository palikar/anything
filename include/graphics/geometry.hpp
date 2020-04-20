#pragma once

#include "rendering/vertex.hpp"
#include "rendering/buffer_factory.hpp"


#include "glm_header.hpp"
#include "std_header.hpp"
#include "math_header.hpp"


namespace ay::grph
{

inline rend::ShaderDataType stride_to_data_type(size_t s)
{
    switch (s)
    {
    case 2: return rend::ShaderDataType::Float2;
    case 3: return rend::ShaderDataType::Float3;
    case 4: return rend::ShaderDataType::Float4;
    }
    return rend::ShaderDataType::None;
}

class Geometry
{ 

    struct Attriubute
    {
        std::vector<float> data;
        size_t stride;
        int max_size{-1};
        int buffer_index{-1};
    };

  private:
    std::vector<uint32_t> m_index;
    std::unordered_map<std::string, Attriubute> m_buffers;
    std::vector<std::tuple<int32_t, int32_t>> m_groups;
    rend::VertexArrayPtr m_glbuffers;

    bool m_dirty{ true };
    bool m_dynamic{ false };

  public:
    
    Geometry(bool t_dynamic = false) : m_dynamic(t_dynamic)
    {
    }

    auto &buffers()
    {
        return m_buffers;
    }

    void set_attribute(std::string name, std::vector<float> buffer, size_t stride)
    {
        m_buffers.insert({ std::move(name), { std::move(buffer), stride, -1, -1}});
    }

    void declare_attribute(std::string name, size_t stride, int max_size)
    {
        m_buffers.insert({ std::move(name), { {}, stride, max_size, -1}});
    }
    
    void drop_attribute(std::string name)
    {
        m_buffers.erase(m_buffers.find(name));
    }

    std::vector<float> &attribute(const std::string &name)
    {
        return (m_buffers.at(name)).data;
    }

    void set_index(std::vector<uint32_t> buffer)
    {
        m_index = std::move(buffer);
    }

    std::vector<uint32_t> &index()
    {
        return m_index;
    }

    void apply(glm::mat4 t_mat);
    

    void compute_bounding_box()
    {
    }

    void compute_bounding_sphere()
    {
    }

    void compute_vertex_normals()
    {
    }

    void look_at(glm::vec3 t_pos)
    {
        apply(glm::lookAt(glm::vec3(1, 0, 0), t_pos, glm::vec3(0, 1, 0)));
    }

    void merge(Geometry &other);
    
    void normalize_normals()
    {
    }

    void rotate_x(float t_angle)
    {
        apply(glm::rotate(glm::mat4(1), t_angle, glm::vec3(1, 0, 0)));
    }

    void rotate_y(float t_angle)
    {
        apply(glm::rotate(glm::mat4(1), t_angle, glm::vec3(0, 1, 0)));
    }

    void rotate_z(float t_angle)
    {
        apply(glm::rotate(glm::mat4(1), t_angle, glm::vec3(0, 0, 1)));
    }

    void scale(float t_factor)
    {
        apply(glm::scale(glm::mat4(1), glm::vec3(1, 1, 1) * t_factor));
    }

    void translate(float t_x, float t_y, float t_z)
    {
        apply(glm::translate(glm::mat4(1), glm::vec3(t_x, t_y, t_z)));
    }

    void pack();
    

    void add_group(uint32_t start, uint32_t count)
    {
        m_groups.push_back({ start, count });
    }

    const rend::VertexArray *gl_buffers() const
    {
        return m_glbuffers.get();
    }

    rend::VertexArrayPtr take_buffers()
    {
        if (m_dirty)
        {
            pack();
        }
        m_dirty = true;
        return std::move(m_glbuffers);
    }

    bool is_dirty() const
    {
        return m_dirty;
    }

    bool indexed()
    {
        return !m_index.empty();
    }

  private:

    void pack_vertex_buffers();

    void pack_vertex_buffers_dynamic();

    void pack_group(uint32_t start, uint32_t count, size_t index = 0);
    
};


}  // namespace ay::grph

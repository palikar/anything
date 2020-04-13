#pragma once


#include "glm_header.hpp"
#include "std_header.hpp"

#include "rendering/vertex.hpp"
#include "rendering/buffer_factory.hpp"

#include "math/utils.hpp"

namespace ay::grph
{

rend::ShaderDataType stride_to_data_type(size_t s)
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

  private:
    std::unordered_map<std::string, std::tuple<std::vector<float>, size_t>> m_buffers;
    std::vector<uint32_t> m_index;

  public:
    Geometry()
    {
    }

    void set_attribute(std::string name, std::vector<float> buffer, size_t stride)
    {
        m_buffers.insert({ std::move(name), { std::move(buffer), stride } });
    }

    std::vector<float> attribute(std::string name)
    {
        return std::get<0>(m_buffers.at(name));
    }

    void set_index(std::vector<uint32_t> buffer)
    {
        m_index = std::move(buffer);
    }

    std::vector<uint32_t> index()
    {
        return m_index;
    }

    void apply(glm::mat4 t_mat)
    {

        if (m_buffers.count("position"))
        {
            auto &pos = std::get<0>(m_buffers.at("position"));
            for (size_t i = 0; i < pos.size() - 3; i += 3)
            {
                auto res = glm::vec4(pos[i], pos[i + 1], pos[i + 2], 1.0f) * t_mat;

                pos[i]     = res.x / res.w;
                pos[i + 1] = res.y / res.w;
                pos[i + 2] = res.z / res.w;
            }
        }

        if (m_buffers.count("normal"))
        {
            auto &norm      = std::get<0>(m_buffers.at("normal"));
            auto normal_mat = mth::normal(t_mat);
            for (size_t i = 0; i < norm.size() - 3; i += 3)
            {
                auto res = glm::vec3(norm[i], norm[i + 1], norm[i + 2]) * normal_mat;

                norm[i]     = res.x;
                norm[i + 1] = res.y;
                norm[i + 2] = res.z;
            }
        }

        if (m_buffers.count("tangent"))
        {
            auto &tan       = std::get<0>(m_buffers.at("normal"));
            auto normal_mat = mth::normal(t_mat);
            for (size_t i = 0; i < tan.size() - 3; i += 3)
            {
                auto res = glm::vec3(tan[i], tan[i + 1], tan[i + 2]) * normal_mat;
                res      = glm::normalize(res);

                tan[i]     = res.x;
                tan[i + 1] = res.y;
                tan[i + 2] = res.z;
            }
        }
    }

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

    void merge(Geometry &)
    {
    }

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

    rend::VertexArrayPtr to_vertex_buffer()
    {
        auto arr = std::make_unique<rend::VertexArray>();
        arr->set_index_buffer(rend::make_index_buffer(m_index));

        for (auto &[name, buff] : m_buffers)
        {
            auto &data  = std::get<0>(buff);
            auto stride = std::get<1>(buff);

            auto vert = std::make_unique<rend::VertexBuffer>(data.data(), data.size());
            vert->set_layout({ { name, stride_to_data_type(stride) } });

            arr->add_vertex_buffer(std::move(vert));
        }

        return arr;
    }
};


}  // namespace ay::grph

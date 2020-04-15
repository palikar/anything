#pragma once


#include "glm_header.hpp"
#include "std_header.hpp"

#include "rendering/vertex.hpp"
#include "rendering/buffer_factory.hpp"

#include "math/utils.hpp"

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

  private:
    std::vector<uint32_t> m_index;
    std::unordered_map<std::string, std::tuple<std::vector<float>, size_t>> m_buffers;
    std::vector<std::tuple<int32_t, int32_t>> m_groups;
    rend::VertexArrayPtr m_glbuffers;

    bool m_dirty{true};

  public:
    
    Geometry()
    {
    }

    auto& buffers()
    {
        return m_buffers;
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
            for (size_t i = 0; i < pos.size() - 2; i += 3)
            {
                auto res = t_mat * glm::vec4(pos[i], pos[i + 1], pos[i + 2], 1.0f);

                pos[i]     = res.x ;
                pos[i + 1] = res.y ;
                pos[i + 2] = res.z ;
            }
        }

        if (m_buffers.count("normal"))
        {
            auto &norm      = std::get<0>(m_buffers.at("normal"));
            auto normal_mat = mth::normal(t_mat);
            for (size_t i = 0; i < norm.size() - 2; i += 3)
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
            for (size_t i = 0; i < tan.size() - 2; i += 3)
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

    void merge(Geometry& other)
    {
        auto& oth = other.m_buffers;
        auto offset = std::get<0>(m_buffers.at("position")).size() / 3;
        
        for (auto& [name, buff] : oth)
        {
            if (m_buffers.count(name) > 0 )
            {
                if(std::get<1>(buff) != std::get<1>(m_buffers.at(name)))
                {
                    continue;
                }

                auto& src = std::get<0>(buff);
                auto& target = std::get<0>(m_buffers.at(name));
                target.insert(target.end(), src.begin(), src.end());
            }
            
        }
        
        for (auto i : other.m_index)
        {
            m_index.push_back(i + offset);
        }
        m_dirty = true;

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

    void pack()
    {

        pack_vertex_buffers();
            
        if (m_groups.empty())
        {
            pack_group(0, m_index.size());
        }
        
        size_t last_start;
        size_t last_count;
        size_t i{0};
        
        for (auto& [start, count] : m_groups)
        {
            pack_group(start, count, i++);
            last_start = start;
            last_count = count;
        }

        m_dirty = false;

    }

    void add_group(uint32_t start, uint32_t count)
    {
        m_groups.push_back({start, count});
    }

    const rend::VertexArray* gl_buffers() const
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

  private:

    void pack_vertex_buffers()
    {
        bool standard = false;
        m_glbuffers = std::make_unique<rend::VertexArray>();

        if (m_buffers.count("position") > 0
            && m_buffers.count("normal") > 0
            && m_buffers.count("uv") > 0)
        {
            standard = true;
            auto& pos = std::get<0>(m_buffers.at("position"));
            auto& norm = std::get<0>(m_buffers.at("normal"));
            auto& uv = std::get<0>(m_buffers.at("uv"));
            std::vector<Vertex8fg> verts;

            for (size_t i = 0, j = 0; i < pos.size() - 2; i += 3, j += 2)
            {
                verts.push_back({
                        pos[i], pos[i + 1], pos[i + 2],
                        norm[i], norm[i + 1], norm[i + 2],
                        uv[j], uv[j + 1]
                    });
            }
            
            m_glbuffers->add_vertex_buffer(rend::make_buffer(verts));
        }

        for (auto& [name, buf] : m_buffers)
        {
            if (standard
                && (name.compare("position") == 0 || name.compare("normal") == 0
                    || name.compare("uv") == 0))
            {
                continue;
            }

            auto &data  = std::get<0>(buf);
            auto stride = std::get<1>(buf);

            auto vert = std::make_unique<rend::VertexBuffer>(data.data(), data.size());
            vert->set_layout({ { name, stride_to_data_type(stride) } });

            m_glbuffers->add_vertex_buffer(std::move(vert));
        }        

    }
    
    void pack_group(uint32_t start, uint32_t count, size_t index = 0)
    {
        m_glbuffers->set_index_buffer(std::make_unique<rend::IndexBuffer>(m_index.data() + start, count), index);
    }

    
};


}  // namespace ay::grph
